#include "directionalLight.h"

DirectionalLight::DirectionalLight()
{
    direction = DEFAULT_LIGHT_DIRECTION;
    ambient = DEFAULT_AMBIENT_LIGHT;
    diffuse = DEFAULT_DIFFUSE_LIGHT;
    specular = DEFAULT_SPECULAR_LIGHT;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this->direction = direction;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

DirectionalLight::~DirectionalLight()
{
    // clearLight();
}

void DirectionalLight::load(GLuint shaderId)
{
    glUniform3fv(glGetUniformLocation(shaderId, directionalLightPropsMap[DL_AMBIENT].c_str()), 1, glm::value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(shaderId, directionalLightPropsMap[DL_DIFFUSE].c_str()), 1, glm::value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shaderId, directionalLightPropsMap[DL_SPECULAR].c_str()), 1, glm::value_ptr(specular));
    glUniform3fv(glGetUniformLocation(shaderId, directionalLightPropsMap[DL_DIRECTION].c_str()), 1, glm::value_ptr(direction));
}

void DirectionalLight::clear()
{

}