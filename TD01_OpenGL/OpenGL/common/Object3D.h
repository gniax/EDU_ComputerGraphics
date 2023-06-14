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
    // Les vecteurs vont stocker les positions, les coordonnées de texture et les normales
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

    // Cette fonction va charger l'objet à partir d'un fichier .obj
    bool LoadFromFile(const std::string& filename);

    std::string GetType();
    glm::quat GetRotation() const;
    glm::vec3 GetPosition() const;
    Shader& GetShader();
    void BindShader(Shader& pShader);

    void SetRotation(float angle, const glm::vec3& axis);
    void ResetRotation();
    void SetPosition(const glm::vec3& pos);

    void SetScale(float scale);

    const glm::mat4& GetTransformation() const;

    // Cette fonction va dessiner l'objet à l'écran
    void Draw();

    // Cette fonction va libérer la mémoire utilisée par l'objet
    void Destroy();
};
