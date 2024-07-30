#include "emtpch.h"
#include "TextureLoader.h"

namespace EMT {
	
	std::unordered_map<std::string, Ref<Texture>> TextureLoader::mTextureCache;
	Ref<Texture> TextureLoader::s_DefaultAlbedo = nullptr;
	Ref<Texture> TextureLoader::s_DefaultNormal = nullptr;
	Ref<Texture> TextureLoader::s_WhiteTexture = nullptr;
	Ref<Texture> TextureLoader::s_BlackTexture = nullptr;

	// ����һЩĬ����ͼ
	void TextureLoader::InitDefaultTextures()
	{
		// �����������˷�ʽ����Ϊ���Ƕ���1*1�����������������Ķ���������
		TextureSettings srgbTextureSettings, formalTextureSettings;
		srgbTextureSettings.IsSRGB = true;
		srgbTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
		srgbTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
		srgbTextureSettings.HasMips = false;
		formalTextureSettings.IsSRGB = false;
		formalTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
		formalTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
		formalTextureSettings.HasMips = false;

		s_DefaultAlbedo = Load2DTexture(std::string("assets/texture/defaultAlbedo.png"), &srgbTextureSettings);
		s_DefaultNormal = Load2DTexture(std::string("assets/texture/defaultNormal.png"), &formalTextureSettings);
		s_WhiteTexture = Load2DTexture(std::string("assets/texture/white.png"), &formalTextureSettings);
		s_BlackTexture = Load2DTexture(std::string("assets/texture/black.png"), &formalTextureSettings);
	}

	Ref<Texture> TextureLoader::Load2DTexture(const std::string& path, TextureSettings* settings)
	{
		// ������������û��
		auto iter = mTextureCache.find(path);
		if (iter != mTextureCache.end())
			return iter->second;


		// û�еĻ����ؽ���
		int width, height, numComponents;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &numComponents, 0);
		if (!data)
		{
			std::cout << "TEXTURE LOAD FAIL - path:" << path << "\n";
			stbi_image_free(data);
			return nullptr;
		}

		int dataFormat;
		switch (numComponents)
		{
			case 1: dataFormat = EMT_RED;  break;
			case 3: dataFormat = EMT_RGB;  break;
			case 4: dataFormat = EMT_RGBA; break;
		}

		Ref<Texture> texture;
		if (settings != nullptr)
			texture = (Texture::Create(*settings));
		else
			texture = (Texture::Create());

		texture->GetTextureSettings().ChannelNum = numComponents;
		texture->Generate2DTexture(width, height, dataFormat, EMT_UNSIGNED_BYTE, data);

		mTextureCache.insert(std::pair<std::string, Ref<Texture>>(path, texture));
		stbi_image_free(data);

		std::cout << path << " load successfully!\n";
		return mTextureCache[path];
	}

	Ref<Cubemap> TextureLoader::LoadCubemapTexture(const std::vector<std::string>& paths, CubemapSettings* settings)
	{
		Ref<Cubemap> cubemap;
		if (settings != nullptr)
			cubemap = Cubemap::Create(*settings);
		else
			cubemap = Cubemap::Create();

		int width, height, numComponents;
		for (size_t i = 0; i < paths.size(); i++)
		{
			unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &numComponents, 0);

			if (data)
			{
				int dataFormat = EMT_NONE;
				switch (numComponents)
				{
					case 1: dataFormat = EMT_RED;  break;
					case 3: dataFormat = EMT_RGB;  break;
					case 4: dataFormat = EMT_RGBA; break;
				}

				std::cout << paths[i] << " load successfully!\n";
				cubemap->GenerateCubemapFace(EMT_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, dataFormat, EMT_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "TEXTURE LOAD FAIL - path:" << paths[i] << "\n";
				stbi_image_free(data);
				return nullptr;
			}
		}

		return cubemap;
	}

	void TextureLoader::DestroyCachedTexture()
	{
		for (auto& item : mTextureCache)
			item.second.reset();
	}

}