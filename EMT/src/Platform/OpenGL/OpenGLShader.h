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
		virtual void setBool(const std::string& name, bool value) override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setFloat(const std::string& name, float value);
		virtual void setMat4f(const std::string& name, const glm::mat4& value) override;
		virtual void setMat3f(const std::string& name, const glm::mat3& value) override;
		virtual void setVec3f(const std::string& name, float x, float y, float z) override;
		virtual void setVec3f(const std::string& name, const glm::vec3& value) override;
		virtual void setVec3i(const std::string& name, int x, int y, int z) override;
		virtual void setVec3i(const std::string& name, const glm::ivec3& value) override;

	private:
		unsigned int m_RendererID;
	};
}