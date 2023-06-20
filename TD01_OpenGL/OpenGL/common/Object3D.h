#pragma once
#include <string>
#include <vector>
#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include "Shader.h"
#include "../libs/tinyobjloader/tiny_obj_loader.h"

class Object3D
{
private:
    // Ici nous aurons nos vecteurs pour stocker les données de vertex
    // Les vecteurs vont stocker les positions, les coords de texture et les normales
    std::vector<float> positions;
    std::vector<float> texcoords;
    std::vector<float> normals;
    glm::mat4 transformation;
    std::vector<unsigned int> indices;
    std::string type;
    Shader shader;

    GLuint VAO = 0, VBO = 0, EBO = 0;

public:
    Object3D();
    ~Object3D();

    // harger l'objet à partir d'un .obj
    bool LoadFromFile(const std::string& filename);

    std::string GetType();
    glm::quat GetRotation() const;
    glm::vec3 GetPosition() const;
    Shader& GetShader();

    void SetRotation(float angle, const glm::vec3& axis);
    void ResetRotation();
    void SetPosition(const glm::vec3& pos);

    void SetScale(float scale);

    const glm::mat4& GetTransformation() const;

    // dessiner l'objet
    void Draw();

    // libérer la mémoire
    void Destroy();
};
