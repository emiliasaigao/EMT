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
		inline static void ChangeDepthFunc(RendererAPI::DepthFunc func) { s_RendererAPI->ChangeDepthFunc(func); }
		inline static void SetViewport(int x, int y, int width, int height) { s_RendererAPI->SetViewport(x,y,width,height); }
	private:
		static RendererAPI* s_RendererAPI;
	};
}