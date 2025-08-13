#pragma once

#include <string>
#include <gl/glew.h>
#include <glm/mat4x4.hpp>

class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    ~Shader();

    void Bind() const;

    void SetLocation(const GLchar* name);

    int GetLocation(const std::string& name) const;

    void validate();

    unsigned int GetId() const { return ID; };

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int ID;
    int m_location;

    /*
    *unsigned int type - openGL types are normally unsigned ints so we use this rather than the type to decouple from opengl
    */
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    GLint getUniformLocation(const std::string& name) const;
    static void link(unsigned int program);
    static void validate(unsigned int program);

    /* We take in the sorce code for the shaders as strings.*/
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

