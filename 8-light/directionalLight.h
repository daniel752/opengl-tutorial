#pragma once

#include "light.h"

constexpr glm::vec3 DEFAULT_LIGHT_DIRECTION = glm::vec3(-0.2f, -1.0f, -0.3f);

enum DirectionalLightPropeties
{
    DL_AMBIENT,
    DL_DIFFUSE,
    DL_SPECULAR,
    DL_DIRECTION,    
};

class DirectionalLight : Light
{
private:
    glm::vec3 direction;

    std::map<int, std::string> directionalLightPropsMap
    {
        {DL_AMBIENT, "directionalLight.ambient"},
        {DL_DIFFUSE, "directionalLight.diffuse"},
        {DL_SPECULAR, "directionalLight.specular"},
        {DL_DIRECTION, "directionalLight.direction"},
    };

public:
    DirectionalLight();
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    ~DirectionalLight();

    void load(GLuint shaderId);
    void clear();
};
