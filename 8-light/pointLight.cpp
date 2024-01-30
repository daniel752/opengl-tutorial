#include "pointLight.h"

// std::map<int, std::string> pointLightPropsMap = 
// {
//     {AMBIENT, "pointLight[%d].ambient"},
//     {DIFFUSE, "pointLight[%d].diffuse"},
//     {SPECULAR, "pointLight[%d].specular"},
//     {PL_POSITION, "pointLight[%d].position"},
//     {PL_CONSTANT, "pointLight[%d].constant"},
//     {PL_LINEAR, "pointLight[%d].linear"},
//     {PL_QUADRATIC, "pointLight[%d].quadratic"},
// };

PointLight::PointLight()
{
    position = DEFAULT_POSITION;
    constant = DEFAULT_CONSTANT;
    linear = DEFAULT_LINEAR;
    quadratic = DEFAULT_QUADRATIC;
    ambient = DEFAULT_AMBIENT_LIGHT;
    diffuse = DEFAULT_DIFFUSE_LIGHT;
    specular = DEFAULT_SPECULAR_LIGHT;
}

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

PointLight::~PointLight()
{

}

void PointLight::load(GLuint shaderId)
{
    glUniform3fv(glGetUniformLocation(shaderId, pointLightPropsMap[PL_AMBIENT].c_str()), 1, glm::value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(shaderId, pointLightPropsMap[PL_DIFFUSE].c_str()), 1, glm::value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shaderId, pointLightPropsMap[PL_SPECULAR].c_str()), 1, glm::value_ptr(specular));
    glUniform3fv(glGetUniformLocation(shaderId, pointLightPropsMap[PL_POSITION].c_str()), 1, glm::value_ptr(position));
    glUniform1f(glGetUniformLocation(shaderId, pointLightPropsMap[PL_CONSTANT].c_str()), constant);
    glUniform1f(glGetUniformLocation(shaderId, pointLightPropsMap[PL_LINEAR].c_str()), linear);
    glUniform1f(glGetUniformLocation(shaderId, pointLightPropsMap[PL_QUADRATIC].c_str()), quadratic);
}

void PointLight::clear()
{

}

void PointLight::injectIndex(unsigned int index) 
{
    for (auto& entry : pointLightPropsMap) 
        std::sprintf(&entry.second[0], entry.second.c_str(), index);    // Using std::sprintf to inject the index into the existing string
}
