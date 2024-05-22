#pragma once

namespace EMT {
	class Shader {
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
		~Shader();

		void Bind();
		void UnBind();

	private:
		unsigned int m_RendererID;
	};
}