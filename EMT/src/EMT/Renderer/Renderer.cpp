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
	
	void Renderer::Render(const Ref<Scene>& scene, const Ref<Shader>& shader, bool isUseMaterial) {
		shader->Bind();
		auto models = scene->GetModels();
		for (const auto& model : models) {
			SetupModelMatrix(model, shader, isUseMaterial);
			model->Draw(shader, isUseMaterial);
		}
		shader->Unbind();
	}

	void Renderer::RenderSkybox(const Ref<Scene>& scene) {
		Ref<Skybox> skybox = scene->GetSkybox();
		auto skyboxShader = skybox->m_SkyboxShader;
		skyboxShader->Bind();
		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::DepthFunc::LessEqual);

		glm::mat4 view = glm::mat4(glm::mat3(scene->GetCamera()->getViewMatrix()));
		skyboxShader->setMat4f("view", view);
		skyboxShader->setMat4f("projection", scene->GetCamera()->getProjectionMatrix());
		skybox->m_Cubemap->Bind();
		skyboxShader->setInt("skybox", 0);

		skybox->m_Cube->Draw(skyboxShader, false);

		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::DepthFunc::Less);
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


	void Renderer::SetupModelMatrix(const Ref<Model>& model, const Ref<Shader>& shader, bool isUseMaterial) {
		glm::mat4 modelMatrix(1.0f);
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), model->GetPosition());

		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(model->GetRotation()), model->GetRotateAxis());
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), model->GetScale());
		glm::mat4 centerTranslate = glm::translate(glm::mat4(1.0f), (-model->GetCenter()));

		
		modelMatrix = translate * rotate * scale * centerTranslate;
		shader->setMat4f("model", modelMatrix);

		if (isUseMaterial)
		{
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
			shader->setMat3f("normalMatrix", normalMatrix);
		}
	}
}