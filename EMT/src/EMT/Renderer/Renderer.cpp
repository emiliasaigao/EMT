#include "emtpch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

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
}