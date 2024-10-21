#include "emtpch.h"
#include "OpenGLComputeShader.h"

#include <glad/glad.h>

namespace EMT {
	void OpenGLComputeShader::ProcessCompute(unsigned int x, unsigned int y, unsigned int z) {
		glDispatchCompute(x, y, z);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	OpenGLComputeShader::OpenGLComputeShader(const std::string& computePath)
	{
		std::ifstream computeFile;
		
		std::string computeCode;
		// 保证ifstream对象可以抛出异常：
		computeFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			computeFile.open(computePath);
			std::stringstream computess;
			computess << computeFile.rdbuf();
			computeCode = computess.str();

		}
		catch (std::fstream::failure e) {
			EMT_CORE_ERROR("打开shader文件失败!:{0}", computePath);
			return;
		}

		const char* computeShaderSource = computeCode.c_str();

		unsigned int computeShader;
		computeShader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(computeShader, 1, &computeShaderSource, NULL);
		glCompileShader(computeShader);
		int success;
		char infolog[1024];
		glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(computeShader, 1024, NULL, infolog);
			EMT_CORE_ERROR("{0} :计算着色器编译失败！错误信息：\n", computePath);
			EMT_CORE_ERROR(infolog);
			glDeleteShader(computeShader);
			return;
		}

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, computeShader);

		glLinkProgram(m_RendererID);
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_RendererID, 1024, NULL, infolog);
			EMT_CORE_ERROR("{0} :着色器链接失败！错误信息：\n", computePath);
			EMT_CORE_ERROR(infolog);
			glDeleteShader(computeShader);
			return;
		}
		glDeleteShader(computeShader);
	}

	OpenGLComputeShader::~OpenGLComputeShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLComputeShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLComputeShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLComputeShader::setBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
	}

	void OpenGLComputeShader::setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLComputeShader::setFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLComputeShader::setMat4f(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLComputeShader::setMat3f(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLComputeShader::setVec3f(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
	}

	void OpenGLComputeShader::setVec3f(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value[0], value[1], value[2]);
	}

	void OpenGLComputeShader::setVec2f(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), value[0], value[1]);
	}

	void OpenGLComputeShader::setVec2f(const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), x, y);
	}

	void OpenGLComputeShader::setVec3i(const std::string& name, int x, int y, int z)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
	}

	void OpenGLComputeShader::setVec3i(const std::string& name, const glm::ivec3& value)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value[0], value[1], value[2]);
	}

}
