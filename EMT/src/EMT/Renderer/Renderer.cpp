#include "emtpch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLCubemap.h"

namespace EMT {
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(const Camera& camera)
	{
		s_SceneData->ViewMatrix = camera.getViewMatrix();
		s_SceneData->ProjectionMatrix = camera.getProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4f("uView", s_SceneData->ViewMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4f("uProjection", s_SceneData->ProjectionMatrix);

		RenderCommand::DrawIndexed(vertexArray);
	}
	
	void Renderer::Render(const Ref<Scene>& scene, const Ref<Shader>& shader, bool isUseMaterial, const glm::mat4& VP) {
		shader->Bind();
		auto models = scene->GetViewableModels(VP);
		for (auto& model : models) {
			SetupModelMatrix(model, shader, isUseMaterial);
			model->Draw(shader, scene->GetFrustumPlanes(), isUseMaterial);
		}
		shader->Unbind();
	}

	void Renderer::RenderSkybox(const Ref<Scene>& scene) {
		Ref<Skybox> skybox = scene->GetSkybox();
		auto skyboxShader = skybox->m_SkyboxShader;
		skyboxShader->Bind();
		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::CompareFunc::LessEqual);

		glm::mat4 view = glm::mat4(glm::mat3(scene->GetCamera()->getViewMatrix()));
		skyboxShader->setMat4f("view", view);
		skyboxShader->setMat4f("projection", scene->GetCamera()->getProjectionMatrix());
		skybox->m_Cubemap->Bind();
		skyboxShader->setInt("skybox", 0);

		RenderCube();

		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::CompareFunc::Less);
		skyboxShader->Unbind();
	}



	void Renderer::RenderNDCPlane() {
		static Ref<Quad> s_NDC_Plane = std::make_shared<Quad>();
		s_NDC_Plane->Draw();
	}

	void Renderer::RenderCube() {
		static Ref<Cube> s_Cube = std::make_shared<Cube>();
		s_Cube->Draw();
	}


	void Renderer::SetupModelMatrix(Model* model, const Ref<Shader>& shader, bool isUseMaterial) {
		glm::mat4 modelMatrix = model->GetModelMatrix();
		
		shader->setMat4f("model", modelMatrix);

		if (isUseMaterial)
		{
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
			shader->setMat3f("normalMatrix", normalMatrix);
		}
	}
}