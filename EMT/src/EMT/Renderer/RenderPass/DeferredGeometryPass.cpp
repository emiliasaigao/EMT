#include "emtpch.h"
#include "DeferredGeometryPass.h"

namespace EMT {
	DeferredGeometryPass::DeferredGeometryPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/GBufferGenerate.vert", "../EMT/assets/shader/GBufferGenerate.frag");
		
		RenderPass::s_Context.geometryOutput.gbuffer = GBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}

	void DeferredGeometryPass::Draw() {
		auto mGBuffer = RenderPass::s_Context.geometryOutput.gbuffer;
		mGBuffer->Bind();
		mGBuffer->Clear();

		RenderCommand::SetViewport(0, 0, mGBuffer->GetWidth(), mGBuffer->GetHeight());

		Ref<Camera> camera = m_Scene->GetCamera();

		m_Shader->Bind();
		// set view, projection matrix, note the model matrix is set in modelrenderer
		m_Shader->setVec3f("viewPos", camera->Position);
		m_Shader->setMat4f("projection", camera->getProjectionMatrix());

		RenderCommand::EnableDepthTest();
		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::DepthFunc::Less);
		Renderer::Render(m_Scene, m_Shader, true);
	}

	void DeferredGeometryPass::OnWindowResize() {
		RenderPass::s_Context.geometryOutput.gbuffer->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}


}