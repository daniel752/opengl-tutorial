#pragma once

#include "light.h"

const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f);
constexpr float DEFAULT_CONSTANT = 1.0f;
constexpr float DEFAULT_LINEAR = 0.1f;
constexpr float DEFAULT_QUADRATIC = 0.032f;

enum PointLightProperties
{
    PL_AMBIENT,
    PL_DIFFUSE,
    PL_SPECULAR,
    PL_POSITION,
    PL_CONSTANT,
    PL_LINEAR,
    PL_QUADRATIC,
};

class PointLight : protected Light
{
protected:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    std::map<int, std::string> pointLightPropsMap = 
    {
        {PL_AMBIENT, "pointLights[%d].ambient"},
        {PL_DIFFUSE, "pointLights[%d].diffuse"},
        {PL_SPECULAR, "pointLights[%d].specular"},
        {PL_POSITION, "pointLights[%d].position"},
        {PL_CONSTANT, "pointLights[%d].constant"},
        {PL_LINEAR, "pointLights[%d].linear"},
        {PL_QUADRATIC, "pointLights[%d].quadratic"},
    };

public:
    PointLight();
    PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    ~PointLight();

    void load(GLuint shaderId);
    void clear();
    void injectIndex(unsigned int index);
};
