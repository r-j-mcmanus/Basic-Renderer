#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "shader.h"


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class ShaderManager {

public:
    // Load and store a new shader
    //template <typename T>
    unsigned int loadShader(const std::string& name, const std::string& fragAndVertexPath);

    // Retrieve a shader by name
    std::shared_ptr<Shader> getShader(const unsigned int shaderId) const;

    std::shared_ptr<Shader> operator[](const unsigned int shaderId) const;

private:
    const ShaderProgramSource PaseShader(const std::string& filepath);

private:
    std::unordered_map<std::string, unsigned int> shaderPathMap;
    std::unordered_map<unsigned int, std::shared_ptr<Shader>> shaderMap;
    unsigned int nextShaderID;

};
