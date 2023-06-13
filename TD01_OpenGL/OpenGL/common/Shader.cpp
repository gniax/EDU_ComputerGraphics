#include "Shader.h"
#include <fstream>
#include <iostream>
#include <gtc/type_ptr.hpp>

Shader::Shader()
{
    shaderName = "";
}

Shader::Shader(std::string pShaderName)
{
    shaderName = pShaderName;
}

Shader::Shader(std::string pShaderName, const std::string& vertexPath, const std::string& fragmentPath) {
    shaderName = pShaderName;
    LoadVertexShader(vertexPath.c_str());
    LoadFragmentShader(fragmentPath.c_str());
    Create();
}

Shader::~Shader() {
    Destroy();
}

std::string Shader::getName() {
    return shaderName;
}

void Shader::use() {
    glUseProgram(m_Program);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}

bool ValidateShader(GLuint shader) {
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char* infoLog = new char[infoLen + 1];
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cout << "Error compiling shader: " << infoLog << std::endl;
            delete[] infoLog;
        }

        // on supprime le shader object car il est inutilisable
        glDeleteShader(shader);

        return false;
    }

    return true;
}

bool Shader::LoadVertexShader(const char* filename) {
    std::cout << "Loading vertex shader: " << filename << std::endl;

    // 1. Charger le fichier en memoire
    std::ifstream fin(filename, std::ios::in | std::ios::binary);
    fin.seekg(0, std::ios::end);
    uint32_t length = (uint32_t)fin.tellg();
    fin.seekg(0, std::ios::beg);
    char* buffer = new char[length + 1];
    fin.read(buffer, length);
    buffer[length] = '\0';

    // 2. Creer le shader object
    m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_VertexShader, 1, &buffer, nullptr);
    // 3. Le compiler
    glCompileShader(m_VertexShader);
    // 4. Nettoyer
    delete[] buffer;
    fin.close();

    // 5. verifie le status de la compilation
    if (ValidateShader(m_VertexShader)) {
        std::cout << "Vertex shader loaded and compiled successfully." << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to load or compile vertex shader." << std::endl;
        return false;
    }
}

bool Shader::LoadFragmentShader(const char* filename) {
    std::ifstream fin(filename, std::ios::in | std::ios::binary);
    fin.seekg(0, std::ios::end);
    uint32_t length = (uint32_t)fin.tellg();
    fin.seekg(0, std::ios::beg);
    char* buffer = new char[length + 1];
    fin.read(buffer, length);
    buffer[length] = '\0';

    // 2. Creer le shader object
    m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_FragmentShader, 1, &buffer, nullptr);
    // 3. Le compiler
    glCompileShader(m_FragmentShader);
    // 4. Nettoyer
    delete[] buffer;
    fin.close();

    // 5. verifie le status de la compilation
    if (ValidateShader(m_FragmentShader)) {
        std::cout << "Fragment shader loaded and compiled successfully." << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to load or compile fragment shader." << std::endl;
        return false;
    }
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

bool Shader::Create() {
    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertexShader);
    glAttachShader(m_Program, m_FragmentShader);

    // Définir les indices des attributs de shader
    glBindAttribLocation(m_Program, 0, "position");
    glBindAttribLocation(m_Program, 1, "normal");
    glBindAttribLocation(m_Program, 2, "texcoord");

    glLinkProgram(m_Program);

    int32_t linked = 0;
    int32_t infoLen = 0;
    // verification du statut du linkage
    glGetProgramiv(m_Program, GL_LINK_STATUS, &linked);

    if (!linked) {
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char* infoLog = new char[infoLen + 1];
            glGetProgramInfoLog(m_Program, infoLen, NULL, infoLog);
            std::cout << "Erreur de lien du programme: " << infoLog << std::endl;
            delete[] infoLog;
        }

        glDeleteProgram(m_Program);

        return false;
    }

    return true;
}

void Shader::Destroy() {
    glDetachShader(m_Program, m_VertexShader);
    glDetachShader(m_Program, m_FragmentShader);
    glDeleteShader(m_VertexShader);
    glDeleteShader(m_FragmentShader);
    glDeleteProgram(m_Program);
}
