#include "glGraphics/Model.h"
#include "glGraphics/Shader.h"
#include "glGraphics/Mesh.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& path){
    loadModel(path);
}

void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); ++i) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_PreTransformVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
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

        std::vector<Texture2D> metallicMaps = loadMaterialTextures(material, aiTextureType_METALNESS, TextureType::Metallic);
        textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());

        std::vector<Texture2D> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::NormalMap);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());  

        std::vector<Texture2D> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::Height);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        std::vector<Texture2D> aoMaps = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, TextureType::AmbientOcclusion);
        textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());

        std::vector<Texture2D> roughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, TextureType::Roughness);
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

        std::vector<Texture2D> albedoMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, TextureType::Albedo);
        textures.insert(textures.end(), albedoMaps.begin(), albedoMaps.end());  
    }

    return Mesh(vertices, indices, textures, mesh->mName.C_Str());
}

std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texture2DType) {
    std::vector<Texture2D> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;

        for (unsigned int j = 0; j < texturesLoaded.size(); ++j) {
            if (std::strcmp(texturesLoaded[j].GetPath().data(), str.C_Str()) == 0 && texturesLoaded[j].GetType() == texture2DType) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture2D texture(str.C_Str(), texture2DType);
            textures.push_back(texture);
            
            texturesLoaded.push_back(texture);
        }
    } 
    return textures;
}