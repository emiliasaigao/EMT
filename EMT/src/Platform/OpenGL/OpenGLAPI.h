#include "EMT/Renderer/RendererAPI.h"

namespace EMT {
	class OpenGLAPI : public RendererAPI {
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawArray(const Ref<VertexArray>& vertexArray) override;
		virtual float GetTime() override;

		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;
		virtual void EnableMultiSample() override;
		virtual void DisableMultiSample() override;
		virtual void EnableCubeMapSeampless() override;

		virtual void SetViewport(int x, int y, int width, int height) override;

		virtual void ChangeDepthFunc(DepthFunc func) override;
	};
}