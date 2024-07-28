#pragma once
#include "EMT/Renderer/Texture/Cubemap.h"
#include "glad/glad.h"

namespace EMT {
	class OpenGLCubemap : public Cubemap {
	public:
		OpenGLCubemap(const EMT::CubemapSettings& settings);
		~OpenGLCubemap();

		void Bind(int unit = 0);
		void UnBind();

		virtual void GenerateCubemapFace(int face, unsigned int width, unsigned int height,
			int dataFormat, int pixelDataType = EMT_UNSIGNED_BYTE,
			unsigned char* data = nullptr) override;



		virtual inline int GetWidth() override { return m_Width; }
		virtual inline int GetHeight() override { return m_Height; }
		virtual inline CubemapSettings* GetCubemapSettings() override { return &m_CubeMapSettings; }
		virtual unsigned int GetCubemapID() override { return m_RendererID; }
		virtual inline void SetCubemapSettings(const CubemapSettings& settings) override { m_CubeMapSettings = settings; }

	private:
		void ApplyCubemapSettings();
	private:
		unsigned int m_RendererID;
		int m_Width, m_Height;
		int m_GeneratedFacesNum;
		CubemapSettings m_CubeMapSettings;
	};
}