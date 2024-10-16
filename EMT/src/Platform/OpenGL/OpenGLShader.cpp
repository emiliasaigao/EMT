#include "emtpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

namespace EMT {
	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	{
		std::ifstream vertexFile;
		std::ifstream fragmentFile;
		std::ifstream geometryFile;
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		// 保证ifstream对象可以抛出异常：
		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);
			std::stringstream vertexss;
			std::stringstream fragmentss;
			vertexss << vertexFile.rdbuf();
			fragmentss << fragmentFile.rdbuf();
			vertexCode = vertexss.str();
			fragmentCode = fragmentss.str();
			if (geometryPath != "") {
				geometryFile.open(geometryPath);
				std::stringstream geometryss;
				geometryss << geometryFile.rdbuf();
				geometryCode = geometryss.str();
			}
		}
		catch (std::fstream::failure e) {
			EMT_CORE_ERROR("打开shader文件失败!:{0},{1}", vertexPath, fragmentPath);
			return;
		}

		const char* vertexShaderSource = vertexCode.c_str();
		const char* fragmentShaderSource = fragmentCode.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		int success;
		char infolog[1024];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 1024, NULL, infolog);
			EMT_CORE_ERROR("{0} :顶点着色器编译失败！错误信息：\n", vertexPath);
			EMT_CORE_ERROR(infolog);
			glDeleteShader(vertexShader);
			return;
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
			EMT_CORE_ERROR("{0} :片元着色器编译失败！错误信息：\n", fragmentPath);
			EMT_CORE_ERROR(infolog);
			glDeleteShader(fragmentShader);
			return;
		}

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		unsigned int geometryShader;
		if (geometryPath != "") {
			const char* geometryShaderSource = geometryCode.c_str();
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
			glCompileShader(geometryShader);
			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(geometryShader, 512, NULL, infolog);
				EMT_CORE_ERROR("{0} 几何着色器编译失败！错误信息：\n", geometryPath);
				EMT_CORE_ERROR(infolog);
				return;
			}
			glAttachShader(m_RendererID, geometryShader);
			glDeleteShader(geometryShader);
		}


		glLinkProgram(m_RendererID);
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_RendererID, 1024, NULL, infolog);
			EMT_CORE_ERROR("{0} :着色器链接失败！错误信息：\n", vertexPath);
			EMT_CORE_ERROR(infolog);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			if (geometryPath != "") glDeleteShader(geometryShader);
			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::setBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
	}

	void OpenGLShader::setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLShader::setFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLShader::setMat4f(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::setMat3f(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::setVec3f(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
	}

	void OpenGLShader::setVec3f(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value[0], value[1], value[2]);
	}

	void OpenGLShader::setVec2f(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), value[0], value[1]);
	}

	void OpenGLShader::setVec2f(const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), x, y);
	}

	void OpenGLShader::setVec3i(const std::string& name, int x, int y, int z)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
	}

	void OpenGLShader::setVec3i(const std::string& name, const glm::ivec3& value)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value[0], value[1], value[2]);
	}
}