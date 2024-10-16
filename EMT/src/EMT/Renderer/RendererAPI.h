#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "FrameBuffer.h"

namespace EMT {
	class RendererAPI {
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		enum class CompareFunc {
			Never = 0, Less = 1, Equal = 2, LessEqual = 3, Greater = 4,
			NotEqual = 5, GreatEqual = 6, Always = 7
		};

		enum class StencilOption {
			Keep = 0, Replace = 1, Zero = 2
		};

		enum class ImageAccess {
			ReadOnly = 0, WriteOnly = 1, ReadWrite = 2
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawArray(const Ref<VertexArray>& vertexArray) = 0;
		virtual float GetTime() = 0;

		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;
		virtual void EnableMultiSample() = 0;
		virtual void DisableMultiSample() = 0;
		virtual void EnableCubeMapSeampless() = 0;
		virtual void EnableStencilTest() = 0;
		virtual void DisableStencilTest() = 0;

		virtual void ChangeDepthFunc(CompareFunc func) = 0;
		virtual void ChangeStencilFunc(CompareFunc func, int ref, unsigned int mask) = 0;
		virtual void ChangeStencilOption(StencilOption fail, StencilOption zfail, StencilOption zpass) = 0;
		virtual void ChangeStencilMask(unsigned int mask) = 0;

		virtual void SetViewport(int x, int y, int width, int height) = 0;
		virtual void CopyFBODepthStencil(const Ref<FrameBuffer>& srcFBO, const Ref<FrameBuffer>& dstFBO) = 0;

		virtual void BindImageTexture(unsigned int location, const Ref<Texture>& texture, int mipMapLevel, RendererAPI::ImageAccess access, unsigned int format) = 0;

		inline static API GetAPI() { return s_API; }
		
	private:
		static API s_API;
	};
}