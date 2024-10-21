#pragma once
#include "RendererAPI.h"

namespace EMT {
	class RenderCommand {
	public:
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); };
		inline static void DrawArray(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawArray(vertexArray); }
		inline static float GetTime() { return s_RendererAPI->GetTime(); }
		inline static void EnableDepthTest() { s_RendererAPI->EnableDepthTest(); }
		inline static void DisableDepthTest() { s_RendererAPI->DisableDepthTest(); }
		inline static void EnableMultiSample() { s_RendererAPI->EnableMultiSample(); }
		inline static void EnableCubeMapSeampless() { s_RendererAPI->EnableCubeMapSeampless(); }
		inline static void DisableMultiSample() { s_RendererAPI->DisableMultiSample(); }
		inline static void ChangeDepthFunc(RendererAPI::CompareFunc func) { s_RendererAPI->ChangeDepthFunc(func); }
		inline static void SetViewport(int x, int y, int width, int height) { s_RendererAPI->SetViewport(x,y,width,height); }
		inline static void CopyFBODepthStencil(const Ref<FrameBuffer>& srcFBO, const Ref<FrameBuffer>& dstFBO) { s_RendererAPI->CopyFBODepthStencil(srcFBO, dstFBO); }
		inline static void EnableStencilTest() { s_RendererAPI->EnableStencilTest(); }
		inline static void DisableStencilTest() { s_RendererAPI->DisableStencilTest(); }

		inline static void ChangeStencilFunc(RendererAPI::CompareFunc func, int ref, unsigned int mask) { s_RendererAPI->ChangeStencilFunc(func, ref, mask); }
		inline static void ChangeStencilOption(RendererAPI::StencilOption fail, RendererAPI::StencilOption zfail, RendererAPI::StencilOption zpass) { s_RendererAPI->ChangeStencilOption(fail, zfail, zpass); }
		inline static void ChangeStencilMask(unsigned int mask) { s_RendererAPI->ChangeStencilMask(mask); }
	
		inline static void BindImageTexture(unsigned int location, const Ref<Texture>& texture, int mipMapLevel, RendererAPI::ImageAccess access, unsigned int format) {
			s_RendererAPI->BindImageTexture(location, texture, mipMapLevel, access, format);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}