#pragma once
#include "EMT/Core.h"

namespace EMT {
	class Shader {
	public:
		virtual ~Shader() {}
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
	};
}