#include "emtpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

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

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->setMat4f("uView", s_SceneData->ViewMatrix);
		shader->setMat4f("uProjection", s_SceneData->ProjectionMatrix);

		RenderCommand::DrawIndexed(vertexArray);
	}
}