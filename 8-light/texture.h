#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <string>
#include <iostream>

constexpr GLenum DEFAULT_TARGET = GL_TEXTURE_2D;
constexpr GLenum DEFAULT_WRAPPING = GL_REPEAT;
constexpr GLenum DEFAULT_FILTERING = GL_LINEAR;

/**
 * @brief Class Texture to handle texture operations such as - loading, using and clearing textures.
*/
class Texture
{
private:
    GLuint textureId;                   // ID to reference texture
    GLenum target;                      // Defines the type of texture (1D/2D/3D), for some reason OpenGL is calling it "target" instead of "type".
    GLenum wrappingParams;              // Specifies how to wrap the texture on mesh
    GLenum filterParams;                // Specifies filter options for texture
    int width, height, nrComponents;    // Specifies the width, height and depth in space for texture
    std::string path;                   // Path to texture file
    unsigned int textureUnit;          // Specifies which texture unit should this texture use (usually most computers will have a minimum of 16 units), starting from GL_TEXTURE0 - GL_TEXTURE[N]

public:
    Texture();
    Texture(GLuint textureId, GLenum target, GLenum wrappingParams, GLenum filterParams, int width, int height, std::string path, unsigned int textureUnit);
    ~Texture();

    void loadTexture();
    void useTexture();
    void clearTexture();

    GLuint getTextureId() { return textureId; };
    void setTextureId(GLuint textureId) { this->textureId = textureId; };

    GLenum getTarget() { return target; };
    void setTarget(GLenum target) { this->target = target; };

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

    unsigned int getTextureUnit() { return textureUnit; };
    void setTextureUnit(unsigned int textureUnit) { this->textureUnit = textureUnit; };
};
