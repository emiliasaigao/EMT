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
	private:
		static RendererAPI* s_RendererAPI;
	};
}