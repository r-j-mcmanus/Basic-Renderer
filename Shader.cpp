#include <string>
#include <iostream>
#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "errors.h"
#include "Shader.h"
#include "helper.h"

const bool READ_BACK_VALUE = true;

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


// Used to test if the location exists
GLint Shader::getUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1)
	{
		std::cout << "[Warning] Uniform '" << name << "' not found or optimized out in shader program " << ID << std::endl;
	}
	return location;
}

// thanks learnopengl!
 // utility uniform functions
 // ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform1i(loc, (int)value));
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform1i(loc, value));
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform1f(loc, value));
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform2fv(loc, 1, &value[0]));
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform2f(loc, x, y));
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform3fv(loc, 1, &value[0]));
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform3f(loc, x, y, z));
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform4fv(loc, 1, glm::value_ptr(value)));
	if (READ_BACK_VALUE)
	{
		GLfloat vals[4];
		glGetUniformfv(ID, loc, vals);
		printVec4(glm::make_vec4(vals), name);
	}
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniform4f(loc, x, y, z, w));
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
		GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
	if (READ_BACK_VALUE)
	{
		GLfloat vals[16];
		glGetUniformfv(ID, loc, vals);
		printMat4(glm::make_mat4(vals), name);
	}
}
