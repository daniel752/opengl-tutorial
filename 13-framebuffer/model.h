#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class Model
{
    public:
        std::vector<Texture> texturesLoaded;
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;

        Model(const char *path);
        ~Model();
        void draw(Shader &shader);
    
    private:
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        void processMaterials(const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
        unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma = false);
};