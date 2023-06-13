#include "Object3D.h"
#include "GL/glew.h"
#include <iostream>

Object3D::Object3D()
    : VAO(0), VBO(0), EBO(0), transformation(glm::mat4(1.0f))
{
}

Object3D::~Object3D()
{
    Destroy();
}
bool Object3D::LoadFromFile(const std::string& filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        return false;
    }

    for (const auto& shape : shapes) {
        const auto& mesh = shape.mesh;
        const auto& indices = mesh.indices;
        const auto& num_face_vertices = mesh.num_face_vertices;

        size_t index_offset = 0;

        for (size_t f = 0; f < num_face_vertices.size(); f++) {
            size_t num_vertices = num_face_vertices[f];

            for (size_t v = 0; v < num_vertices; v++) {
                tinyobj::index_t idx = indices[index_offset + v];

                for (int j = 0; j < 3; j++) {
                    positions.push_back(attrib.vertices[3 * idx.vertex_index + j]);

                    if (idx.normal_index >= 0) {
                        normals.push_back(attrib.normals[3 * idx.normal_index + j]);
                    }
                }

                if (idx.texcoord_index >= 0) {
                    texcoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    texcoords.push_back(1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]);
                }

                this->indices.push_back(this->indices.size());
            }

            index_offset += num_vertices;
        }
    }

    // Create VBO and VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float) + normals.size() * sizeof(float) + texcoords.size() * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(float), positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), normals.size() * sizeof(float), normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(float) + normals.size() * sizeof(float), texcoords.size() * sizeof(float), texcoords.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(positions.size() * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(positions.size() * sizeof(float) + normals.size() * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

std::string Object3D::GetType()
{
    return type;
}

Shader& Object3D::GetShader()
{
    return shader;
}

void Object3D::BindShader(Shader& pShader) {
    shader = pShader;
}

void Object3D::ResetRotation() 
{
    transformation = glm::mat4(1.0f); // Réinitialiser la transformation à la matrice identité
}

void Object3D::SetRotation(float angle, const glm::vec3& axis) 
{
    ResetRotation(); // Optionnel : réinitialiser la rotation
    transformation = glm::rotate(transformation, glm::radians(angle), axis);
}

void Object3D::SetPosition(const glm::vec3& pos) {
    transformation = glm::translate(transformation, pos);
}

void Object3D::SetScale(float scale) {
    transformation = glm::scale(transformation, glm::vec3(scale, scale, scale));
}

const glm::mat4& Object3D::GetTransformation() const {
    return transformation;
}

void Object3D::Draw()
{
    // Vérifier si le VAO est valide
    if (VAO == 0)
    {
        std::cerr << "Failed to draw the object. VAO is not initialized." << std::endl;
        return;
    }

    // Bind VAO et dessiner l'objet
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Object3D::Destroy()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (EBO != 0)
    {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }

    positions.clear();
    normals.clear();
    texcoords.clear();
    indices.clear();
}

