#pragma once

/*

#include "Shader.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"

// note if Shader was private we cannot downcast a pointer to is, this is a problem for the shader manager
class LightingShader : public Shader
{
public:
    LightingShader(const std::string& vertexSource, const std::string& fragmentSource)
        : Shader(vertexSource, fragmentSource) {}

    void SetUniforms(const glm::mat4 Model, const Camera camera, const Material material, const Light light);
};

*/