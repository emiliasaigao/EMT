#pragma once
#include "EMT/Renderer/ComputeShader.h"
#include <glm/gtc/type_ptr.hpp>

namespace EMT {
	class OpenGLComputeShader : public ComputeShader {
	public:
		OpenGLComputeShader(const std::string& computePath);
		~OpenGLComputeShader();
		virtual void Bind() override;
		virtual void Unbind() override;

	public:
		virtual void setBool(const std::string& name, bool value) override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setFloat(const std::string& name, float value);
		virtual void setMat4f(const std::string& name, const glm::mat4& value) override;
		virtual void setMat3f(const std::string& name, const glm::mat3& value) override;
		virtual void setVec3f(const std::string& name, float x, float y, float z) override;
		virtual void setVec3f(const std::string& name, const glm::vec3& value) override;
		virtual void setVec2f(const std::string& name, float x, float y) override;
		virtual void setVec2f(const std::string& name, const glm::vec2& value) override;
		virtual void setVec3i(const std::string& name, int x, int y, int z) override;
		virtual void setVec3i(const std::string& name, const glm::ivec3& value) override;

	public:
		virtual void ProcessCompute(unsigned int x, unsigned int y = 1, unsigned int z = 1) override;

	private:
		unsigned int m_RendererID;
	};
}