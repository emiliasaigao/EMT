#include "emtpch.h"
#include "Skybox.h"

namespace EMT {
	Skybox::Skybox(const std::vector<std::string>& paths) {
		m_SkyboxShader = Shader::Create("assets/shader/common/skybox.vert", "assets/shader/common/skybox.frag");
		m_Cube = std::make_shared<Model>(Cube());
		// ������������ͼ
		CubemapSettings settings;
		settings.IsSRGB = true;
		m_Cubemap = TextureLoader::LoadCubemapTexture(paths, &settings);
	}

}
