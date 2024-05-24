#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"

namespace EMT {

	class Renderer {
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}