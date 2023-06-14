#include "Grid3D.h"
#include "GL/glew.h"
#include <iostream>

Grid3D::Grid3D(int size, float scale, const Shader& shader)
{
    this->shader = shader;
    generateGrid(size, scale);
}