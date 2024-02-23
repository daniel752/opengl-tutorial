#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <string>
#include <iostream>
#include <map>

// constexpr GLenum DEFAULT_TARGET = GL_TEXTURE_2D;
// std::string DEFAULT_TYPE = "texture_diffuse";
// constexpr GLenum DEFAULT_WRAPPING = GL_REPEAT;
// constexpr GLenum DEFAULT_FILTERING = GL_LINEAR;

enum TextureTypes
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
};

const std::map<TextureTypes, std::string> TextureMap
{
    {DIFFUSE, "texture_diffuse"},
    {SPECULAR, "texture_specular"},
    {NORMAL, "texture_normal"},
    {HEIGHT, "texture_height"},
};

/**
 * @brief Class Texture to handle texture operations such as - loading, using and clearing textures.
*/
class Texture
{
private:
    GLuint textureId;                   // ID to reference texture
    GLenum target;                      // Defines the target texture (1D/2D/3D)
    std::string type;                   // Defines type of texture (diffuse/specular/normal/height)
    GLenum wrappingParams;              // Specifies how to wrap the texture on mesh
    GLenum filterParams;                // Specifies filter options for texture
    int width, height, nrComponents;    // Specifies the width, height and depth in space for texture
    std::string path;                   // Path to texture file
    unsigned int textureUnit;           // Specifies which texture unit should this texture use (usually most computers will have a minimum of 16 units), starting from GL_TEXTURE0 - GL_TEXTURE[N]

public:
    Texture(std::string path);
    Texture(GLenum target, std::string type, GLenum wrappingParams, GLenum filterParams, int width, int height, std::string path);
    ~Texture();

    void loadTexture();
    void useTexture();
    void clearTexture();

    GLuint getTextureId() { return textureId; };
    void setTextureId(GLuint textureId) { this->textureId = textureId; };

    GLenum getTarget() { return target; };
    void setTarget(GLenum target) { this->target = target; };

    std::string getType() { return type; };
    void setType(std::string type) { this->type = type; };

    GLenum getWrappingParams() { return wrappingParams; };
    void setWrappingParams(GLenum wrappingParams) { this->wrappingParams = wrappingParams; };

    GLenum getFilterParams() { return filterParams; };
    void setFilterParams(GLenum filterParams) { this->filterParams = filterParams; };

    int getWidth() { return width; };
    void setWidth(int width) { this->width = width; };

    int getHeight() { return height; };
    void setHeight(int height) { this->height = height; };

    int getNrComponents() { return nrComponents; };
    void setNrComponents(int nrComponents) { this->nrComponents = nrComponents; };

    std::string getPath() { return path; };
    void setPath(std::string path) { this->path = path; };

    unsigned int getTextureUnit() { return textureUnit; };
    void setTextureUnit(unsigned int textureUnit) { this->textureUnit = textureUnit; };
};
