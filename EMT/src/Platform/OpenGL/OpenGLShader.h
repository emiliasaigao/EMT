#pragma once
#include "EMT/Renderer/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace EMT {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
		~OpenGLShader();
		virtual void Bind() override;
		virtual void UnBind() override;

	public:
		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setMat4f(const std::string& name, const glm::mat4& value);
		void setMat3f(const std::string& name, const glm::mat3& value);
		void setVec3f(const std::string& name, float x, float y, float z);
		void setVec3f(const std::string& name, const glm::vec3& value);

	private:
		unsigned int m_RendererID;
	};
}