#include "glGraphics/Model.h"
#include "glGraphics/Shader.h"
#include "glGraphics/Mesh.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

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
        textures = loadMaterialTextures(material, scene);
    }

    return Mesh(vertices, indices, textures, mesh->mName.C_Str());
}

std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial* material, const aiScene* scene) {
    std::vector<Texture2D> textures;

    std::vector<int> albedoTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_BASE_COLOR);
    std::vector<int> roughnessTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_DIFFUSE_ROUGHNESS);
    std::vector<int> metallicTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_METALNESS);
    std::vector<int> normalTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_NORMALS);
    std::vector<int> heightTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_HEIGHT);
    std::vector<int> aoTextureIndices = getEmbeddedTextureTypeIndices(material, aiTextureType_AMBIENT_OCCLUSION);

    std::vector<Texture2D> albedoTextures = loadEmbdeedTextures(albedoTextureIndices, TextureType::Albedo, scene);
    std::vector<Texture2D> roughnessTextures = loadEmbdeedTextures(roughnessTextureIndices, TextureType::Roughness, scene);
    std::vector<Texture2D> metallicTextures = loadEmbdeedTextures(metallicTextureIndices, TextureType::Metallic, scene);
    std::vector<Texture2D> normalTextures = loadEmbdeedTextures(normalTextureIndices, TextureType::NormalMap, scene);
    std::vector<Texture2D> heightTextures = loadEmbdeedTextures(heightTextureIndices, TextureType::Height, scene);
    std::vector<Texture2D> aoTextures = loadEmbdeedTextures(aoTextureIndices, TextureType::AmbientOcclusion, scene);

    textures.insert(textures.end(), albedoTextures.begin(), albedoTextures.end());
    textures.insert(textures.end(), roughnessTextures.begin(), roughnessTextures.end());
    textures.insert(textures.end(), metallicTextures.begin(), metallicTextures.end());
    textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
    textures.insert(textures.end(), heightTextures.begin(), heightTextures.end());
    textures.insert(textures.end(), aoTextures.begin(), aoTextures.end());

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

std::vector<Texture2D> Model::loadEmbdeedTextures(std::vector<int> indices, TextureType type, const aiScene* scene) {
    std::vector<Texture2D> textures;
    
    for (int index : indices) {
        aiTexture* texture = scene->mTextures[index];

        if (!texture) {
            throw std::runtime_error("Error: Failed to load embedded texture");
        }

        if (texture->mHeight == 0) {
            std::string path = "src/assets/textures/texture" + std::string(scene->mName.C_Str()) + std::to_string(index) + "." + std::string(texture->achFormatHint);
            if (std::filesystem::exists(path)) {
                bool skip = false;

                for (Texture2D tex : texturesLoaded) {
                    if (tex.GetPath() == path && tex.GetType() == type) {
                        textures.push_back(tex);
                        skip = true;
                        break;
                    }
                }
                
                if (!skip) {
                    Texture2D tex = Texture2D(path, type);

                    texturesLoaded.push_back(tex);
                    textures.push_back(tex);
                }

                continue;
            }

            std::ofstream file(path, std::ios::binary);
            file.write(reinterpret_cast<char*>(texture->pcData), texture->mWidth);
            file.close();
            
            Texture2D tex = Texture2D(path, type);

            texturesLoaded.push_back(tex);
            textures.push_back(tex);
        } else {
            bool skip = false;

            for (Texture2D tex : texturesLoaded) {
                if (tex.GetPath() == "*" + std::to_string(index) && tex.GetType() == type) {
                    textures.push_back(tex);
                    skip = true;
                    break;
                }
            }
            
            if (skip) {
                continue;
            }
            
            unsigned int width = texture->mWidth;
            unsigned int height = texture->mHeight;
            
            std::vector<std::uint8_t> rawData(width * height * 4);
            for (unsigned int i = 0; i < width * height; ++i) {
                rawData[i * 4] = texture->pcData[i].a;
                rawData[i * 4 + 1] = texture->pcData[i].r;
                rawData[i * 4 + 2] = texture->pcData[i].g;
                rawData[i * 4 + 3] = texture->pcData[i].b;
            }

            Texture2D tex = Texture2D(rawData, index, type);
            
            texturesLoaded.push_back(tex);
            textures.push_back(tex);
        }
    }

    return textures;
}