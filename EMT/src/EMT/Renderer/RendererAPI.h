#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace EMT {
	class RendererAPI {
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		enum class DepthFunc {
			Never = 0, Less = 1, Equal = 2, LessEqual = 3, Greater = 4,
			NotEqual = 5, GreatEqual = 6, Always = 7
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

		virtual void ChangeDepthFunc(DepthFunc func) = 0;

		virtual void SetViewport(int x, int y, int width, int height) = 0;

		inline static API GetAPI() { return s_API; }
		
	private:
		static API s_API;
	};
}