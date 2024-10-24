#pragma once
#include "Camera.h"
#include "EMT/Renderer/model/Model.h"
#include "EMT/Renderer/model/shapes/Cube.h"
#include "EMT/Renderer/Texture/Cubemap.h"
#include "EMT/Renderer/Shader.h"

namespace EMT {
	class Skybox {
	public:
		Skybox(const esgstl::vector<std::string>& paths);
		~Skybox() {}
		
	public:
		Ref<Cubemap> m_Cubemap;
		Ref<Shader> m_SkyboxShader;
	};
};