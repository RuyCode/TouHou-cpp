#include "glGraphics/Mesh.h"
#include "glGraphics/Shader.h"
#include "glGraphics/Texture2D.h"

#include <glad/glad.h>
#include <string>

#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures, std::string name) :
    vertices(vertices),
    indices(indices),
    textures(textures),
    VAO(),
    VBO(&vertices[0], vertices.size() * sizeof(Vertex)),
    EBO(&indices[0], indices.size() * sizeof(unsigned int)) {

    this->name = name;

    VAO.Bind();
    VBO.Bind();
    EBO.Bind();

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(0));
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Tangent)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));

    VAO.Unbind();
}

void Mesh::Draw(Shader &shader) {

    unsigned int heightNumber = 1;
    unsigned int normalNumber = 1;
    unsigned int albedoNumber = 1;
    unsigned int metallicNumber = 1;
    unsigned int roughnessNumber = 1;
    unsigned int aoNumber = 1;

    for (unsigned int i = 0; i < textures.size(); ++i) {
        std::string name{};
        std::string number{};
        TextureType type = textures[i].GetType();

        switch (type) {
            case TextureType::Height : {
                name = "height";
                number = std::to_string(heightNumber++);
                break;
            }
            case TextureType::NormalMap : {
                name = "normal";
                number = std::to_string(normalNumber++);
                break;
            }
            case TextureType::Albedo : {
                name = "albedo";
                number = std::to_string(albedoNumber++);
                break;
            }
            case TextureType::Metallic : {
                name = "metallic";
                number = std::to_string(metallicNumber++);
                break;
            }
            case TextureType::Roughness : {
                name = "roughness";
                number = std::to_string(roughnessNumber++);
                break;
            }
            case TextureType::AmbientOcclusion : {
                name = "ao";
                number = std::to_string(aoNumber++);
                break;
            }
        }

        // material_<type>_<number>
        shader.Activate();
        glUseProgram(shader.ID);
        textures[i].BindToUnit(i);
        shader.SetInt(("material_" + name + "_" + number).c_str(), i);   
    }

    VAO.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    VAO.Unbind();
}
