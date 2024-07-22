#pragma once
#include "EMT/Core.h"

namespace EMT {
	class Shader {
	public:
		virtual ~Shader() {}
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void setBool(const std::string& name, bool value) = 0;
		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setFloat(const std::string& name, float value) = 0;
		virtual void setMat4f(const std::string& name, const glm::mat4& value) = 0;
		virtual void setMat3f(const std::string& name, const glm::mat3& value) = 0;
		virtual void setVec3f(const std::string& name, float x, float y, float z) = 0;
		virtual void setVec3f(const std::string& name, const glm::vec3& value) = 0;
		virtual void setVec3i(const std::string& name, int x, int y, int z) = 0;
		virtual void setVec3i(const std::string& name, const glm::ivec3& value) = 0;


		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
	};
}