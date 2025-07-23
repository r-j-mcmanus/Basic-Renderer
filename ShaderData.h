#pragma once

#include <string>

enum ShaderName
{
	PhongShader,
	SolidColor,
	Basic2,
	Basic3,
};

struct ShaderDataContainer
{
	ShaderName name;
	std::string filePath;
};

namespace ShaderData {
	const ShaderDataContainer PhongShader = { ShaderName::PhongShader, std::string("Resources/Shaders/phong.shader") };
	const ShaderDataContainer SolidColor = { ShaderName::SolidColor, std::string("Resources/Shaders/mvp_uniform_buffer.shader") };
	const ShaderDataContainer Basic2 = { ShaderName::Basic2, std::string("Resources/Shaders/basic_2.shader") };
	const ShaderDataContainer Basic3 = { ShaderName::Basic3, std::string("Resources/Shaders/basic_3.shader") };
}