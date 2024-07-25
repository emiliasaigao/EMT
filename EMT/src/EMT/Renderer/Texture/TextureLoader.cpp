#include "emtpch.h"
#include "TextureLoader.h"

namespace EMT {
	
	std::unordered_map<std::string, Ref<Texture>> TextureLoader::mTextureCache;
	Ref<Texture> TextureLoader::s_DefaultAlbedo = nullptr;
	Ref<Texture> TextureLoader::s_DefaultNormal = nullptr;
	Ref<Texture> TextureLoader::s_WhiteTexture = nullptr;
	Ref<Texture> TextureLoader::s_BlackTexture = nullptr;

	// 加载一些默认贴图
	void TextureLoader::InitDefaultTextures()
	{
		// 设置纹理过滤方式，因为它们都是1*1的纹理，所以其他的都不用设置
		TextureSettings srgbTextureSettings, formalTextureSettings;
		srgbTextureSettings.IsSRGB = true;
		srgbTextureSettings.TextureMinificationFilterMode = GL_NEAREST;
		srgbTextureSettings.TextureMagnificationFilterMode = GL_NEAREST;
		srgbTextureSettings.HasMips = false;
		formalTextureSettings.IsSRGB = false;
		formalTextureSettings.TextureMinificationFilterMode = GL_NEAREST;
		formalTextureSettings.TextureMagnificationFilterMode = GL_NEAREST;
		formalTextureSettings.HasMips = false;

		s_DefaultAlbedo = Load2DTexture(std::string("assets/texture/defaultAlbedo.png"), &srgbTextureSettings);
		s_DefaultNormal = Load2DTexture(std::string("assets/texture/defaultNormal.png"), &formalTextureSettings);
		s_WhiteTexture = Load2DTexture(std::string("assets/texture/white.png"), &formalTextureSettings);
		s_BlackTexture = Load2DTexture(std::string("assets/texture/black.png"), &formalTextureSettings);
	}

	Ref<Texture> TextureLoader::Load2DTexture(const std::string& path, TextureSettings* settings)
	{
		// 看看缓存里有没有
		auto iter = mTextureCache.find(path);
		if (iter != mTextureCache.end())
			return iter->second;


		// 没有的话加载进来
		int width, height, numComponents;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &numComponents, 0);
		if (!data)
		{
			std::cout << "TEXTURE LOAD FAIL - path:" << path << "\n";
			stbi_image_free(data);
			return nullptr;
		}

		GLenum dataFormat;
		switch (numComponents)
		{
			case 1: dataFormat = GL_RED;  break;
			case 3: dataFormat = GL_RGB;  break;
			case 4: dataFormat = GL_RGBA; break;
		}

		Ref<Texture> texture;
		if (settings != nullptr)
			texture.reset(new Texture(*settings));
		else
			texture.reset(new Texture());

		texture->GetTextureSettings().ChannelNum = numComponents;
		texture->Generate2DTexture(width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		mTextureCache.insert(std::pair<std::string, Ref<Texture>>(path, texture));
		stbi_image_free(data);

		std::cout << path << " load successfully!\n";
		return mTextureCache[path];
	}

	Ref<Cubemap> TextureLoader::LoadCubemapTexture(const std::vector<std::string>& paths, CubemapSettings* settings)
	{
		Ref<Cubemap> cubemap;
		if (settings != nullptr)
			cubemap.reset(new Cubemap(*settings));
		else
			cubemap.reset(new Cubemap());

		int width, height, numComponents;
		for (size_t i = 0; i < paths.size(); i++)
		{
			unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &numComponents, 0);

			if (data)
			{
				GLenum dataFormat = GL_NONE;
				switch (numComponents)
				{
					case 1: dataFormat = GL_RED;  break;
					case 3: dataFormat = GL_RGB;  break;
					case 4: dataFormat = GL_RGBA; break;
				}

				std::cout << paths[i] << " load successfully!\n";
				cubemap->GenerateCubemapFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
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
