#include "glGraphics/Shader.h"
#include "glGraphics/Mesh.h"
#include "glGraphics/Camera.h"

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <type_traits>

#include <iostream>

class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture2D> texturesLoaded;
    std::vector<Camera> cameras;
    std::vector<aiMetadataEntry> metadata;

    void loadModel(const std::string& path);

    void applyNodeTransform(aiNode* node, const aiScene* scene, aiMatrix4x4 parentTransform);

    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture2D> loadMaterialTextures(aiMaterial* material, const aiScene* scene);

    std::vector<int> getEmbeddedTextureTypeIndices(const aiMaterial* material, aiTextureType type);

    std::vector<Texture2D> loadEmbdeedTextures(std::vector<int> indices, TextureType type, const aiScene* scene);

    void loadCameras(const aiScene* scene);

public:
    Model(const std::string& path);

    void Draw(std::vector<Shader>& shaders);

    std::vector<Camera> GetCameras();
};

#endif //MODEL_H