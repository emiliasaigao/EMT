#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"
#include "Scene.h"

namespace EMT {

	class Renderer {
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		static void Render(const Ref<Scene>& scene, const Ref<Shader>& shader, bool isUseMaterial);
		static void RenderSSRModels(const Ref<Scene>& scene, const Ref<Shader>& shader);
		static void RenderSkybox(const Ref<Scene>& scene);
		static void RenderNDCPlane();
		static void RenderCube();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:

		struct SceneData {
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
		};

		static SceneData* s_SceneData;
		static void SetupModelMatrix(const Ref<Model>& model, const Ref<Shader>& shader, bool isUseMaterial);
	};
}