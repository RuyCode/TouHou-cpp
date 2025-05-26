#include "scripts/glGraphics/model/Model.h"
#include "scripts/glGraphics/Shader.h"
#include "scripts/glGraphics/model/Mesh.h"
#include "scripts/glGraphics/Camera.h"
#include "scripts/utils/Logger.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <iostream>

Model::Model(const std::string& path){
    loadModel(path);
    Logger::Instance().Info("Loaded model: ", path);
}

void Model::Draw(std::vector<Shader> &shaders) {
    for (unsigned int i = 0; i < meshes.size(); ++i) {
        meshes[i].Draw(shaders);
    }
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_PreTransformVertices);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }

    directory = std::filesystem::path(path).parent_path().string();


    processNode(scene->mRootNode, scene);
    loadCameras(scene);
}

void Model::processNode(aiNode* node, const aiScene* scene  ) {
    Logger::Instance().Info("Found ", node->mNumMeshes, " meshes" );
    
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    Logger::Instance().Info("Processing mesh: ", mesh->mName.C_Str(), " (", mesh->mNumVertices, ") vertices");
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        
        Vertex vertex;
        glm::vec3 bufferVector;

        bufferVector.x = mesh->mVertices[i].x;
        bufferVector.y = mesh->mVertices[i].y;
        bufferVector.z = mesh->mVertices[i].z;

        vertex.Position = bufferVector;

        if (mesh->mNormals) {
            bufferVector.x = mesh->mNormals[i].x;
            bufferVector.y = mesh->mNormals[i].y;
            bufferVector.z = mesh->mNormals[i].z;
            vertex.Normal = bufferVector;
        } else {
            vertex.Normal = glm::vec3(0.f, 1.f, 0.f);   
        }

        glm::vec2 textureCoordVector(0.f, 0.f);

        if (mesh->mTextureCoords[0] != nullptr) {
            textureCoordVector.x = mesh->mTextureCoords[0][i].x;
            textureCoordVector.y = mesh->mTextureCoords[0][i].y;
        }

        if (mesh->mTangents) {
            bufferVector.x = mesh->mTangents[i].x;
            bufferVector.y = mesh->mTangents[i].y;
            bufferVector.z = mesh->mTangents[i].z;
            vertex.Tangent = bufferVector;
        } else {
            vertex.Tangent = glm::vec3(0.f, 0.f, 1.f);
        }
        vertex.TexCoords = textureCoordVector;

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        textures = loadMaterialTextures(material, scene);
    }

    std::string meshName = (mesh->mName).C_Str();

    std::uint16_t shaderID = 0u;

    std::size_t sPos = meshName.find("_s");
    if (sPos != std::string::npos) {
        std::size_t start = sPos + 2;
        std::size_t end = meshName.find('-', start);

        if (end == std::string::npos) {
            end = meshName.length();
        }

        shaderID = static_cast<std::uint16_t>(std::stoi(meshName.substr(start, end - start)));
    }

    return Mesh(vertices, indices, textures, shaderID, meshName);
}

std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial* material, const aiScene* scene) {
    std::vector<Texture2D> textures;

    std::vector<int> albedoTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_BASE_COLOR);
    std::vector<int> roughnessTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_DIFFUSE_ROUGHNESS);
    std::vector<int> metallicTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_METALNESS);
    std::vector<int> normalTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_NORMALS);
    std::vector<int> heightTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_HEIGHT);
    std::vector<int> aoTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_AMBIENT_OCCLUSION);

    std::vector<Texture2D> albedoTextures = loadEmbddedTextures(albedoTextureIndices, TextureType::Albedo, scene);
    std::vector<Texture2D> roughnessTextures = loadEmbddedTextures(roughnessTextureIndices, TextureType::Roughness, scene);
    std::vector<Texture2D> metallicTextures = loadEmbddedTextures(metallicTextureIndices, TextureType::Metallic, scene);
    std::vector<Texture2D> normalTextures = loadEmbddedTextures(normalTextureIndices, TextureType::NormalMap, scene);
    std::vector<Texture2D> heightTextures = loadEmbddedTextures(heightTextureIndices, TextureType::Height, scene);
    std::vector<Texture2D> aoTextures = loadEmbddedTextures(aoTextureIndices, TextureType::AmbientOcclusion, scene);

    textures.insert(textures.end(), albedoTextures.begin(), albedoTextures.end());
    textures.insert(textures.end(), roughnessTextures.begin(), roughnessTextures.end());
    textures.insert(textures.end(), metallicTextures.begin(), metallicTextures.end());
    textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
    textures.insert(textures.end(), heightTextures.begin(), heightTextures.end());
    textures.insert(textures.end(), aoTextures.begin(), aoTextures.end());

    Logger::Instance().Info("Loaded material: ", material->GetName().C_Str());

    return textures;
}

std::vector<int> Model::getEmbeddedTextureTypeIndices(const aiMaterial* material, aiTextureType type) {
    std::vector<int> indices;

    unsigned int textureNumber = material->GetTextureCount(type);

    for (unsigned int i = 0; i < textureNumber; ++i) {
        aiString path;

        if (material->GetTexture(type, i, &path) == AI_SUCCESS) {
            if (path.C_Str()[0] == '*') {
                int index = std::stoi(path.C_Str() + 1);
                indices.push_back(index);
            }
        }
    }

    return indices;
}

std::vector<Texture2D> Model::loadEmbddedTextures(std::vector<int> indices, TextureType type, const aiScene* scene) {
    std::vector<Texture2D> textures;
    
    for (int index : indices) {
        aiTexture* texture = scene->mTextures[index];

        if (!texture) {
            Logger::Instance().Error("Failed to loda embedded texture");
            throw std::runtime_error("Error: Failed to load embedded texture");
        }

        std::string textureKey = (texture->mHeight == 0)
            ? "texture_" + std::to_string(index) + "_" + scene->mName.C_Str() + "." + std::string(texture->achFormatHint)
            : "embedded_" + std::to_string(index);

        if (texturesLoaded.find(textureKey) != texturesLoaded.end()) {
            textures.push_back(texturesLoaded[textureKey]);
            continue;
        }

        Texture2D tex;
        if (texture->mHeight == 0) {
            std::string folderPath = "src/assets/textures/scenes/" + std::string(scene->mName.C_Str());

            if (!std::filesystem::exists(folderPath)) {
                std::filesystem::create_directory(folderPath);
            }

            std::string path = folderPath + "/" + textureKey;
            std::ofstream file(path, std::ios::binary);

            file.write(reinterpret_cast<char*>(texture->pcData), texture->mWidth);
            file.close();

            tex = Texture2D(path, type);
        } else {
            unsigned int width = texture->mWidth;
            unsigned int height = texture->mHeight;

            std::vector<std::uint8_t> rawData(width * height * 4);

            for (unsigned int i = 0; i < width * height * 4; ++i) {
                rawData[i * 4] = texture->pcData[i].a;
                rawData[i * 4 + 1] = texture->pcData[i].r;
                rawData[i * 4 + 2] = texture->pcData[i].g;
                rawData[i * 4 + 3] = texture->pcData[i].b;
            }

            tex = Texture2D(rawData, index, type);
        }

        texturesLoaded[textureKey] = tex;
        textures.push_back(tex);
    }

    return textures;
}

glm::mat4 convertToGLMMatrix(const aiMatrix4x4& assimpMatrix) {
    glm::mat4 glmMatrix;

    glmMatrix[0][0] = assimpMatrix.a1; glmMatrix[1][0] = assimpMatrix.a2; glmMatrix[2][0] = assimpMatrix.a3; glmMatrix[3][0] = assimpMatrix.a4;
    glmMatrix[0][1] = assimpMatrix.b1; glmMatrix[1][1] = assimpMatrix.b2; glmMatrix[2][1] = assimpMatrix.b3; glmMatrix[3][1] = assimpMatrix.b4;
    glmMatrix[0][2] = assimpMatrix.c1; glmMatrix[1][2] = assimpMatrix.c2; glmMatrix[2][2] = assimpMatrix.c3; glmMatrix[3][2] = assimpMatrix.c4;
    glmMatrix[0][3] = assimpMatrix.d1; glmMatrix[1][3] = assimpMatrix.d2; glmMatrix[2][3] = assimpMatrix.d3; glmMatrix[3][3] = assimpMatrix.d4;

    return glmMatrix;
}

void Model::loadCameras(const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumCameras; ++i) {
        aiCamera* assimpCamera = scene->mCameras[i];
        const aiNode* camNode = scene->mRootNode->FindNode(assimpCamera->mName);

        glm::mat4 glmTransform = convertToGLMMatrix(camNode->mTransformation);

        glm::vec4 worldPosition = glmTransform * glm::vec4(assimpCamera->mPosition.x, assimpCamera->mPosition.y, assimpCamera->mPosition.z, 1.0f);
        glm::vec3 position(worldPosition.x, worldPosition.y, worldPosition.z);

        glm::vec4 worldLookAt = glmTransform * glm::vec4(assimpCamera->mLookAt.x, assimpCamera->mLookAt.y, assimpCamera->mLookAt.z, 0.0f);
        glm::vec3 lookAt(worldLookAt.x, worldLookAt.y, worldLookAt.z);

        glm::vec3 direction = glm::normalize(lookAt - position);

        glm::vec4 worldUp = glmTransform * glm::vec4(0, 1, 0, 0);
        glm::vec3 up = glm::normalize(glm::vec3(worldUp.x, worldUp.y, worldUp.z));

        float aspect = assimpCamera->mAspect;
        float near = assimpCamera->mClipPlaneNear;
        float far = assimpCamera->mClipPlaneFar;
        float hFov = assimpCamera->mHorizontalFOV;
        float vFov = hFov / aspect;

        Camera camera = Camera(position, direction, up, vFov, aspect, near, far);
        cameras.push_back(camera);

        Logger::Instance().Info("Loaded camera: ", camNode->mName.C_Str());
    }
}

std::vector<Camera> Model::GetCameras() {
    return cameras;
}
