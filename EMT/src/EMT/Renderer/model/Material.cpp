#include "emtpch.h"
#include "Material.h"

namespace EMT {
	Material::Material(Ref<Texture> albedoMap, Ref<Texture> normalMap, Ref<Texture> metallicMap, Ref<Texture> roughnessMap, Ref<Texture> ambientOcclusionMap, Ref<Texture> mixtureMap) :
		m_AlbedoMap(albedoMap), m_NormalMap(normalMap),
		m_MetallicMap(metallicMap), m_RoughnessMap(roughnessMap),
		m_AOMap(ambientOcclusionMap), m_MixtureMap(mixtureMap)
	{
	}

	void Material::SeperateMixture()
	{
		if (m_MixtureMap != nullptr)
		{
			int width = m_MixtureMap->GetWidth();
			int height = m_MixtureMap->GetHeight();
			TextureSettings settings = m_MixtureMap->GetTextureSettings();
			int channel = settings.ChannelNum;
			unsigned char* mixtureData = new unsigned char[width * height * channel];
			unsigned char* metalData = new unsigned char[width * height];
			unsigned char* roughData = new unsigned char[width * height];
			unsigned char* aoData = new unsigned char[width * height];
			m_MixtureMap->Bind(0);
			glGetTexImage(GL_TEXTURE_2D, 0, settings.dataFormat, GL_UNSIGNED_BYTE, mixtureData);
			m_MixtureMap->Unbind();

			if (channel == 1)
			{
				EMT_CORE_ERROR("WRONG MIXTURE FORMATE: Only one channel mixture");
				return;
			}


			// gltf贴图三通道分别对应： r - ao, g - roughness, b - metallic
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					int index1 = (i * width + j) * channel;
					int index2 = (i * width + j);
					metalData[index2] = mixtureData[index1 + 2];
					roughData[index2] = mixtureData[index1 + 1];
					if (channel > 2)
						aoData[index2] = mixtureData[index1];
				}
			}
			m_MetallicMap.reset(new Texture());
			m_MetallicMap->Generate2DTexture(width, height, GL_RED, GL_UNSIGNED_BYTE, metalData);
			delete[] metalData;
			m_RoughnessMap.reset(new Texture());
			m_RoughnessMap->Generate2DTexture(width, height, GL_RED, GL_UNSIGNED_BYTE, roughData);
			delete[] roughData;
			if (channel > 2)
			{
				m_AOMap.reset(new Texture());
				m_AOMap->Generate2DTexture(width, height, GL_RED, GL_UNSIGNED_BYTE, aoData);
				delete[] aoData;
			}
		}
		else
		{
			EMT_CORE_ERROR("No mixture map has been set, sperate muxture failed!");
		}
	}

	void Material::BindMaterial(const Ref<Shader>& shader) const
	{
		// 记得先绑定shader

		// 0号贴图位留给了阴影贴图
		// 1号贴图位留给了预卷积漫反射贴图
		// 2号贴图位留给了预滤波光照贴图
		// 3号贴图位留给了brdf的LUT贴图
		int currentTextureUnit = 4;

		shader->setInt("material.texture_albedo", currentTextureUnit);
		if (m_AlbedoMap)
			m_AlbedoMap->Bind(currentTextureUnit++);
		else
			TextureLoader::GetDefaultAlbedo()->Bind(currentTextureUnit++);


		shader->setInt("material.texture_normal", currentTextureUnit);
		if (m_NormalMap)
			m_NormalMap->Bind(currentTextureUnit++);
		else
			TextureLoader::GetDefaultNormal()->Bind(currentTextureUnit++);

		shader->setInt("material.texture_metallic", currentTextureUnit);
		if (m_MetallicMap)
			m_MetallicMap->Bind(currentTextureUnit++);
		else
			TextureLoader::GetDefaultMetallic()->Bind(currentTextureUnit++);

		shader->setInt("material.texture_roughness", currentTextureUnit);
		if (m_RoughnessMap)
			m_RoughnessMap->Bind(currentTextureUnit++);
		else
			TextureLoader::GetDefaultRoughness()->Bind(currentTextureUnit++);

		shader->setInt("material.texture_ao", currentTextureUnit);
		if (m_AOMap)
			m_AOMap->Bind(currentTextureUnit++);
		else
			TextureLoader::GetDefaultAO()->Bind(currentTextureUnit++);

	}

	void Material::OnImGuiRender() {
		
		if (m_AlbedoMap) {
			if (ImGui::TreeNode("albedoMap")) {
				m_AlbedoMap->DisplayTexture();
				ImGui::TreePop();
			}
		}
		if (m_NormalMap) {
			if (ImGui::TreeNode("normalMap")) {
				m_NormalMap->DisplayTexture();
				ImGui::TreePop();
			}
		}
		if (m_MetallicMap) {
			if (ImGui::TreeNode("metalicMap")) {
				m_MetallicMap->DisplayTexture();
				ImGui::TreePop();
			}
		}
		if (m_RoughnessMap) {
			if (ImGui::TreeNode("roughnessMap")) {
				m_RoughnessMap->DisplayTexture();
				ImGui::TreePop();
			}
		}
		if (m_AOMap) {
			if (ImGui::TreeNode("aoMap")) {
				m_AOMap->DisplayTexture();
				ImGui::TreePop();
			}
		}
		if (m_MixtureMap) {
			if (ImGui::TreeNode("mixtureMap")) {
				m_MixtureMap->DisplayTexture();
				ImGui::TreePop();
			}
		}

	}

}
