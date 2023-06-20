#include "Grid3D.h"
#include "GL/glew.h"
#include <iostream>

Grid3D::Grid3D() {}

Grid3D::Grid3D(int size, float scale, const Shader& shader)
{
    this->shader = shader;
    generateGrid(size, scale);
}

void Grid3D::generateGrid(int size, float scale)
{
    float halfSize = size / 2.0f;
    for (int i = -halfSize; i <= halfSize; i++) {
        vertices.push_back(glm::vec3(i * scale, 0, -halfSize * scale));
        vertices.push_back(glm::vec3(i * scale, 0, halfSize * scale));
        vertices.push_back(glm::vec3(-halfSize * scale, 0, i * scale));
        vertices.push_back(glm::vec3(halfSize * scale, 0, i * scale));
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Grid3D::drawGrid()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices.size());
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}