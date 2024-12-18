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
		virtual void EnableStencilTest() override;
		virtual void DisableStencilTest() override;

		virtual void ChangeStencilFunc(CompareFunc func, int ref, unsigned int mask) override;
		virtual void ChangeStencilOption(StencilOption fail, StencilOption zfail, StencilOption zpass) override;
		virtual void ChangeStencilMask(unsigned int mask) override;

		virtual void SetViewport(int x, int y, int width, int height) override;
		virtual void CopyFBODepthStencil(const Ref<FrameBuffer>& srcFBO, const Ref<FrameBuffer>& dstFBO) override;
		virtual void ChangeDepthFunc(CompareFunc func) override;
		virtual void BindImageTexture(unsigned int location, const Ref<Texture>& texture, int mipMapLevel, RendererAPI::ImageAccess access, unsigned int format) override;
	};
}