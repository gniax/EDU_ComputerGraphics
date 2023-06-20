#pragma once
#include <GL/glew.h>
#include <string>
#include <stb_image.h>

class Texture {
public:
    Texture(const std::string& fileLoc);

    bool LoadTexture();
    bool LoadTextureA(); // alpha textures
    GLuint GetTextureID();
    void UseTexture();
    void ClearTexture();

    ~Texture();

private:
    GLuint textureID;
    int width, height, bitDepth;
    std::string fileLocation;
};