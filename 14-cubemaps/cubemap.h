#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class Cubemap
{
    private:
        unsigned int textureId;
    
    public:
        Cubemap(std::vector<std::string> faces);
        ~Cubemap();

        unsigned int getTextureId() { return textureId; };
        void setTextureId(unsigned int textureId) { this->textureId = textureId; };
};