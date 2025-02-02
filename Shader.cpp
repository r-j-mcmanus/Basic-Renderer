#include <string>
#include <iostream>
#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "errors.h"

#include "Shader.h"

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
	ID = CreateShader(vertexSource, fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(ID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(ID));
}

void Shader::SetLocation(const GLchar* name)
{
	GLCall(m_location = glGetUniformLocation(ID, name));
}


int Shader::GetLocation(const std::string& name) const
{
	GLCall(int location = glGetUniformLocation(ID, name.c_str()));
	if (location == -1)
		std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	return location;
}


/*
	unsigned int type - openGL types are normally unsigned ints so we use this rather than the type to decouple from opengl
*/
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str(); // pointer to the start of the source string
	// id for the shader
	// number of source codes
	// pointer to pointer to string
	// if length is null, it is assumed the string is null terminated
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// error handeling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}


/* We take in the sorce code for the shaders as strings.*/
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// openGL requires the source code for the shader to compile, and it returns an id back for accssing the function
	GLCall(unsigned int program = glCreateProgram());

	GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

	//two files, need to link them 
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	//we have linked the shaders stored in a program, so we can delete the shader.
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

// thanks learnopengl!
 // utility uniform functions
 // ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const
{
    //bools are stored as 4 bytes
	GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value));
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
	GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value));
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
	GLCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value));
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	GLCall(glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	GLCall(glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y));
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	GLCall(glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	GLCall(glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z));
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	GLCall(glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)));
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	GLCall(glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w));
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	GLCall(glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	GLCall(glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
