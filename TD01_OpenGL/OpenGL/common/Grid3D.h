#pragma once
#include <string>
#include <vector>
#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include "Shader.h"

class Grid3D {
public:
    Grid3D();
    Grid3D(int size, float scale, const Shader& shader);

    void generateGrid(int size, float scale);

    void drawGrid();

private:
    GLuint vao, vbo;
    std::vector<glm::vec3> vertices;
    Shader shader;
};