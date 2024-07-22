#include "EMT/Renderer/RendererAPI.h"

namespace EMT {
	class OpenGLAPI : public RendererAPI {
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual float GetTime() override;
		virtual void EnableDepthTest() override;
	};
}