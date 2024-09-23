#include "emtpch.h"
#include "Skybox.h"

namespace EMT {
	Skybox::Skybox(const esgstl::vector<std::string>& paths) {
		m_SkyboxShader = Shader::Create("../EMT/assets/shader/common/skybox.vert", "../EMT/assets/shader/common/skybox.frag");
		// 加载立方体贴图
		CubemapSettings settings;
		settings.IsSRGB = true;
		m_Cubemap = TextureLoader::LoadCubemapTexture(paths, &settings);
	}

}
