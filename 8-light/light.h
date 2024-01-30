#pragma once

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr glm::vec3 DEFAULT_AMBIENT_LIGHT = glm::vec3(0.1f);
constexpr glm::vec3 DEFAULT_DIFFUSE_LIGHT = glm::vec3(0.5f);
constexpr glm::vec3 DEFAULT_SPECULAR_LIGHT = glm::vec3(0.8f);

class Light
{
protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    virtual void load(GLuint shaderId) = 0;
    virtual void clear() = 0;

    void setAmbient(glm::vec3 ambient) { this->ambient = ambient; };
    glm::vec3 getAmbient() { return ambient; };

    void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; };
    glm::vec3 getDiffuse() { return diffuse; };
    
    void setSpecular(glm::vec3 specular) { this->specular = specular; };
    glm::vec3 getSpecular() { return specular; };
};

