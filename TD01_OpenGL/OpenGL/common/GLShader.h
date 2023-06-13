#pragma once

#include <glm.hpp> 
#include <GL/glew.h> 
#include <string>

class GLShader
{
private:
    uint32_t m_Program;
    uint32_t m_VertexShader;
    uint32_t m_GeometryShader;
    uint32_t m_FragmentShader;

    bool CompileShader(uint32_t type);
public:
    GLShader(const std::string& vertexPath, const std::string& fragmentPath);
    ~GLShader();

    inline GLuint getProgramID() { return m_Program; }
    void use();

    void setVec3(const std::string& name, const glm::vec3& value);
    void setFloat(const std::string& name, float value);
    // autres setters pour d'autres types d'uniformes, si nécessaire...

    bool LoadVertexShader(const char* filename);
    bool LoadGeometryShader(const char* filename);
    bool LoadFragmentShader(const char* filename);
    bool Create();
    void Destroy();
};