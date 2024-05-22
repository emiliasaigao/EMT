#include "emtpch.h"
#include "Shader.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

namespace EMT {
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	{
		std::ifstream vertexFile;
		std::ifstream fragmentFile;
		std::ifstream geometryFile;
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		// ��֤ifstream��������׳��쳣��
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
			EMT_CORE_ERROR("��shader�ļ�ʧ�ܣ�");
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
			EMT_CORE_ERROR("������ɫ������ʧ�ܣ�������Ϣ��\n");
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
			EMT_CORE_ERROR("ƬԪ��ɫ������ʧ�ܣ�������Ϣ��\n");
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
				EMT_CORE_ERROR("������ɫ������ʧ�ܣ�������Ϣ��\n");
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
			EMT_CORE_ERROR("��ɫ������ʧ�ܣ�������Ϣ��\n");
			EMT_CORE_ERROR(infolog);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			if (geometryPath != "") glDeleteShader(geometryShader);
			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}
}