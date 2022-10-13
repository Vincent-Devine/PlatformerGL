#include "Shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Assertion.hpp"

namespace Resources
{
	Shader::Shader(const std::string& p_name, const std::string& p_path1, const std::string& p_path2, const unsigned int p_id)
		: vertexShader(0)
		, fragmentShader(0)
		, shaderProgram(0)
	{
		name = p_name;
		path1 = p_path1;
		path2 = p_path2;
		id = p_id;
	}

	void Shader::Init()
	{
		// build and compile our shader program
		Assertion(SetVertexShader(), "Fail on SetVertexShader()");
		Assertion(SetFragmentShader(), "Fail on SetFragmentShader()");
		Assertion(Link(), "Fail on link shader");
	}

	void Shader::Draw(const Core::Maths::Mat4& p_transform, const Core::Maths::Mat4& p_mvp)
	{
		
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_TRUE, &p_transform.mat[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_TRUE, &p_mvp.mat[0][0]);
	}

	bool Shader::SetVertexShader()
	{
		std::ifstream sourcefile;
		std::string tempSource;
		std::string line;
		OpenShader(path1, sourcefile);
		while (std::getline(sourcefile, line))
			tempSource += "\n" + line;
		CloseShader(path1, sourcefile);
		const char* source = tempSource.c_str();

		// vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &source, NULL);
		glCompileShader(vertexShader);

		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			DEBUG_LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + *infoLog);
			return false;
		}
		return true;
	}

	bool Shader::SetFragmentShader()
	{
		std::ifstream sourcefile;
		std::string tempSource;
		std::string line;
		OpenShader(path2, sourcefile);
		while (std::getline(sourcefile, line))
			tempSource += "\n" + line;
		CloseShader(path2, sourcefile);
		const char* source = tempSource.c_str();

		// fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &source, NULL);
		glCompileShader(fragmentShader);

		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			DEBUG_LOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + *infoLog);
			return false;
		}
		return true;
	}

	bool Shader::Link()
	{
		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// check for linking errors
		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			DEBUG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + *infoLog);
			return false;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return true;
	}

	void Shader::OpenShader(const std::string& p_path, std::ifstream& p_source)
	{
		p_source.open(p_path, std::ios::in);
		Assertion(p_source.is_open(), "Fail to open shader " + p_path);
		Core::Debug::Log::Print("Open shader " + p_path + "\n", Core::Debug::LogLevel::Notification);
	}

	void Shader::CloseShader(const std::string& p_path, std::ifstream& p_source)
	{
		p_source.close();
		Core::Debug::Log::Print("Close shader " + p_path + "\n", Core::Debug::LogLevel::Notification);
	}
}