#include "glGraphics/Shader.h"
#include "glGraphics/Mesh.h"

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture2D> texturesLoaded;

    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texture2DType);

public:
    Model(const std::string& path);

    void Draw(Shader& shader);
};

#endif //MODEL_H