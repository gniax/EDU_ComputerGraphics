#pragma once
#include <glm.hpp> 
#include <GL/glew.h> 
#include <string>

class Shader
{
private:
    std::string shaderName;
    uint32_t m_Program;
    uint32_t m_VertexShader;
    uint32_t m_GeometryShader;
    uint32_t m_FragmentShader;

    bool CompileShader(uint32_t type);
public:
    Shader();
    Shader(std::string pShaderName);
    Shader(std::string pShaderName, const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    inline GLuint getProgramID() { return m_Program; }
    void use();

    std::string getName();
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    // autres setters pour d'autres types d'uniformes, si nécessaire...
    bool LoadVertexShader(const char* filename);
    bool LoadGeometryShader(const char* filename);
    bool LoadFragmentShader(const char* filename);
    bool Create();
    void Destroy();
};