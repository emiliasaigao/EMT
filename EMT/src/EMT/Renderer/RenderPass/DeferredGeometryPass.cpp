#include "emtpch.h"
#include "DeferredGeometryPass.h"

namespace EMT {
	DeferredGeometryPass::DeferredGeometryPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/deferred/GBufferGenerate.vert", "../EMT/assets/shader/deferred/GBufferGenerate.frag");
		
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
		m_Shader->setMat4f("view", camera->getViewMatrix());
		m_Shader->setMat4f("projection", camera->getProjectionMatrix());

		RenderCommand::EnableDepthTest();
		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::CompareFunc::Less);
		RenderCommand::EnableStencilTest();
		RenderCommand::ChangeStencilOption(EMT::RendererAPI::StencilOption::Keep,
			EMT::RendererAPI::StencilOption::Keep, EMT::RendererAPI::StencilOption::Replace);
		RenderCommand::ChangeStencilFunc(EMT::RendererAPI::CompareFunc::Always, 1, 0xFF);
		RenderCommand::ChangeStencilMask(0xFF);

		Renderer::Render(m_Scene, m_Shader, true);

		RenderCommand::DisableStencilTest();
	}

	void DeferredGeometryPass::OnWindowResize() {
		RenderPass::s_Context.geometryOutput.gbuffer->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}


}