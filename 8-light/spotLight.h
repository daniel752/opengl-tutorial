#include "pointLight.h"

enum SpotLightProperties
{
    SL_AMBIENT,
    SL_DIFFUSE,
    SL_SPECULAR,
    SL_POSITION,
    SL_DIRECTION,
    SL_CONSTANT,
    SL_LINEAR,
    SL_QUADRATIC,
    SL_CUTOFF,
    SL_OUTER_CUTOFF,
};

class SpotLight : PointLight
{
private:
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    std::map<int, std::string> spotLightPropsMap = 
    {
        {SL_AMBIENT, "spotLight.ambient"},
        {SL_DIFFUSE, "spotLight.diffuse"},
        {SL_SPECULAR, "spotLight.specular"},
        {SL_POSITION, "spotLight.position"},
        {SL_DIRECTION, "spotLight.direction"},
        {SL_CONSTANT, "spotLight.constant"},
        {SL_LINEAR, "spotLight.linear"},
        {SL_QUADRATIC, "spotLight.quadratic"},
        {SL_CUTOFF, "spotLight.cutOff"},
        {SL_OUTER_CUTOFF, "spotLight.outerCutOff"},
    };

public:
    SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    ~SpotLight();

    void load(GLuint shaderId);
    void clear();

};
