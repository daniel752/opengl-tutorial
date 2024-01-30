#include "spotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this->position = position;
    this->direction = direction;
    this->cutOff = cutOff;
    this->outerCutOff = outerCutOff;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

SpotLight::~SpotLight()
{

}

void SpotLight::load(GLuint shaderId)
{
    glUniform3fv(glGetUniformLocation(shaderId, spotLightPropsMap[SL_AMBIENT].c_str()), 1, glm::value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(shaderId, spotLightPropsMap[SL_DIFFUSE].c_str()), 1, glm::value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shaderId, spotLightPropsMap[SL_SPECULAR].c_str()), 1, glm::value_ptr(specular));
    glUniform3fv(glGetUniformLocation(shaderId, spotLightPropsMap[SL_POSITION].c_str()), 1, glm::value_ptr(position));
    glUniform3fv(glGetUniformLocation(shaderId, spotLightPropsMap[SL_DIRECTION].c_str()), 1, glm::value_ptr(direction));
    glUniform1f(glGetUniformLocation(shaderId, spotLightPropsMap[SL_CONSTANT].c_str()), constant);
    glUniform1f(glGetUniformLocation(shaderId, spotLightPropsMap[SL_LINEAR].c_str()), linear);
    glUniform1f(glGetUniformLocation(shaderId, spotLightPropsMap[SL_QUADRATIC].c_str()), quadratic);
    glUniform1f(glGetUniformLocation(shaderId, spotLightPropsMap[SL_CUTOFF].c_str()), glm::cos(glm::radians(cutOff)));
    glUniform1f(glGetUniformLocation(shaderId, spotLightPropsMap[SL_OUTER_CUTOFF].c_str()), glm::cos(glm::radians(outerCutOff)));
}

void SpotLight::clear()
{

}