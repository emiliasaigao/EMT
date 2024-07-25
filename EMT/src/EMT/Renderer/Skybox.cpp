#include "emtpch.h"
#include "Skybox.h"

namespace EMT {
	Skybox::Skybox(const std::vector<std::string>& paths) {
		m_SkyboxShader = Shader::Create("assets/shader/common/skybox.vert", "assets/shader/common/skybox.frag");
		m_Cube = std::make_shared<Model>(Cube());
		// 加载立方体贴图
		CubemapSettings settings;
		settings.IsSRGB = true;
		m_Cubemap = TextureLoader::LoadCubemapTexture(paths, &settings);
	}

}
