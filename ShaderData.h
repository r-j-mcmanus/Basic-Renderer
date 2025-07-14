#pragma once

#include <string>

enum ShaderName
{
	PhongShader,
	SolidColor
};

struct ShaderDataContainer
{
	ShaderName name;
	std::string filePath;
};

namespace ShaderData {
	const ShaderDataContainer PhongShader = { ShaderName::PhongShader, std::string("Resources/Shaders/phong.shader") };
	const ShaderDataContainer SolidColor = { ShaderName::SolidColor, std::string("Resources/Shaders/mvp_uniform_buffer.shader") };
}