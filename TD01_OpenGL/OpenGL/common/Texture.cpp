#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <iostream>

Texture::Texture(const std::string& fileLoc)
    : textureID(1), width(0), height(0), bitDepth(0), fileLocation(fileLoc)
{
}

// inspire du cours
bool Texture::LoadTexture()
{
    unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
    if (!texData)
    {
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);

    return true;
}

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint Texture::GetTextureID()
{
    return textureID;
}

void Texture::ClearTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}

Texture::~Texture()
{
    ClearTexture();
}