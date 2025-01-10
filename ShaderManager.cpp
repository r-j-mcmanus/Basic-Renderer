#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "ShaderManager.h"


unsigned int ShaderManager::loadShader(const std::string& name, const std::string& fragAndVertexPath)
{
    // check if already loaded
    auto shaderIt = shaderPathMap.find(fragAndVertexPath);
    if (shaderIt != shaderPathMap.end())
    {
        return shaderIt->second;
    }

    ShaderProgramSource source = PaseShader(fragAndVertexPath);
    std::shared_ptr<Shader> shader_ptr = std::make_shared<Shader>(source.VertexSource, source.FragmentSource);

    unsigned int renderID = nextShaderID++;
    shaderMap[renderID] = shader_ptr;
    shaderPathMap[fragAndVertexPath] = renderID;
    shaderNameMap[name] = renderID;
    return renderID;
}

// Retrieve a shader by ID
std::shared_ptr<Shader> ShaderManager::getShader(const unsigned int shaderId) const {
    auto it = shaderMap.find(shaderId);
    if (it != shaderMap.end()) {
        return it->second;
    }
    return nullptr;  // Return null if shader not found
}

// Retrieve a shader by Name
std::shared_ptr<Shader> ShaderManager::getShader(const std::string shaderName) const {
    auto it = shaderNameMap.find(shaderName);
    if (it == shaderNameMap.end()) {
        return nullptr;
    }
    return getShader(it->second);
}

std::shared_ptr<Shader> ShaderManager::operator[](const unsigned int shaderId) const {
    auto it = shaderMap.find(shaderId);
    if (it != shaderMap.end()) {
        return it->second;
    }
    return nullptr;  // Return nullptr if shader not found
}

const ShaderProgramSource ShaderManager::PaseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    // Note file should not have a new line at the end
    std::fstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    std::cout << "VERTEX" << std::endl;
    std::cout << ss[0].str() << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << ss[1].str() << std::endl;

    return { ss[0].str(), ss[1].str() };
}
