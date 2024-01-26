#include "texture.h"
#include "stb_image.h"

Texture::Texture()
{
    textureId = 0;
    target = DEFAULT_TARGET;
    wrappingParams = DEFAULT_WRAPPING;
    filterParams = DEFAULT_FILTERING;
    width = 0;
    height = 0;
    nrComponents = 0;
    path = "";
    textureUnit = GL_TEXTURE0;
}

Texture::Texture(GLuint textureId, GLenum target, GLenum wrappingParams, GLenum filterParams, int width, int height, std::string path, unsigned int textureUnit)
{
    this->textureId = textureId;
    this->target = target;
    this->wrappingParams = wrappingParams;
    this->filterParams = filterParams;
    this->width = width;
    this->height = height;
    this->path = path;
    this->textureUnit = textureUnit;
}

Texture::~Texture()
{
    clearTexture();
}

void Texture::loadTexture()
{
    // Load image via "stbi_image" library
    unsigned char* textureData = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if(!textureData)
    {
        std::cerr << "ERROR::Texture::loadTexture()::can't load texture file " << path << std::endl;
        return;
    }

    // Generate 1 texture and bind it to "textureId"
    glGenTextures(1, &textureId);
    glBindTexture(target, textureId);

    // Set texture wrapping params
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrappingParams);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrappingParams);

    // Set texture filtering params
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterParams);
    glTexParameterf(target, GL_TEXTURE_MAG_FILTER, filterParams);

    stbi_set_flip_vertically_on_load(true);

    // Define the format of image according to number of channels in image (number of color components)
    GLint format;
    switch(nrComponents)
    {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }
    // Create 2D texture image
    glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
    // Generate mipmap for image
    glGenerateMipmap(target);
    // Set state back to default after configuration
    glBindTexture(target, 0);
    // Free pointer to texture data
    free(textureData);
}

void Texture::useTexture()
{
    glActiveTexture(textureUnit);
    glBindTexture(target, textureId);
}

void Texture::clearTexture()
{
    glDeleteTextures(1, &textureId);
    textureId = 0;
    target = 0;
    wrappingParams = 0;
    filterParams = 0;
    width = 0;
    height = 0;
    nrComponents = 0;
    path = "";
    textureUnit = 0;
}