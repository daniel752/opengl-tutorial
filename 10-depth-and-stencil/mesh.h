#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

constexpr int MAX_BONE_INFLUENCE = 4;

/**
 * @brief Vertex struct to store vertex related data in mesh
*/
struct Vertex
{
    // Vertex position
    glm::vec3 position;
    // Vertex normal vector
    glm::vec3 normal;
    // Vertex texture coordinates
    glm::vec2 textureCoordinates;
    // Vertex tangent vector
    glm::vec3 tangent;
    // Vertex bitangent vector
    glm::vec3 bitangent;

    // Bone indexes which will influence this vertex
    int boneIds[MAX_BONE_INFLUENCE];
    // Weights from each bone
    float boneWeights[MAX_BONE_INFLUENCE];
};

/**
 * @brief Texture struct to store texture related data in mesh
*/
struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
    private:
        // Render data
        unsigned int vertexBuffer, elementBuffer;
        void setupMesh();

    public:
        // Mesh data
        unsigned int vertexArray;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        ~Mesh();
        void draw(Shader &shader);
};