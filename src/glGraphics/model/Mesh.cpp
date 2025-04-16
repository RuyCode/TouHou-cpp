#include "glGraphics/model/Mesh.h"
#include "glGraphics/Shader.h"
#include "glGraphics/Texture2D.h"

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <unordered_map>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture2D>& textures, std::uint16_t shaderID, const std::string& name) :
    vertices(vertices),
    indices(indices),
    textures(textures),
    VAO(),
    VBO(&vertices[0], vertices.size() * sizeof(Vertex)),
    EBO(&indices[0], indices.size() * sizeof(unsigned int)),
    name(name),
    ShaderID(shaderID) {

    VAO.Bind();
    VBO.Bind();
    EBO.Bind();

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(0));
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Tangent)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));

    VAO.Unbind();

    std::cout << "Loaded mesh: " << name << " with shaderID of " << ShaderID << std::endl;
}

void Mesh::Draw(std::vector<Shader> &shaders) {
    shaders[ShaderID].Activate();

    std::unordered_map<TextureType, unsigned int> textureCounters = {
        {TextureType::Height, 1},
        {TextureType::NormalMap, 1},
        {TextureType::Albedo, 1},
        {TextureType::Metallic, 1},
        {TextureType::Roughness, 1},
        {TextureType::AmbientOcclusion, 1}
    };    

    for (unsigned int i = 0; i < textures.size(); ++i) {   
        TextureType type = textures[i].GetType();
        std::string name;

        switch (type) {
            case TextureType::Height: 
                name = "height"; break;
            case TextureType::NormalMap:      
                name = "normal"; break;
            case TextureType::Albedo:         
                name = "albedo"; break;
            case TextureType::Metallic:       
                name = "metallic"; break;
            case TextureType::Roughness:      
                name = "roughness"; break;
            case TextureType::AmbientOcclusion: 
                name = "ao"; break;
        }

        std::string number = std::to_string(textureCounters[type]++);

        // material_<type>_<number>
        std::string uniformName = ("material_" + name + "_" + number);

        textures[i].BindToUnit(i);
        shaders[ShaderID].SetInt(uniformName, i);
    }

    VAO.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    VAO.Unbind();
    
    VBO.Unbind();
    EBO.Unbind();
}
