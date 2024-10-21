#include "emtpch.h"
#include "SSRPass.h"

namespace EMT {
	SSRPass::SSRPass(const Ref<Scene>& scene)
		:RenderPass(scene) {

		RenderPass::s_Context.ssrOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
		m_Shader = Shader::Create("../EMT/assets/shader/postprocess/SSR.vert", "../EMT/assets/shader/postprocess/SSR.frag");
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGBA32F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.ssrOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.ssrOutput.fbo->AddDepthStencilRBO(EMT_DEPTH24_STENCIL8, EMT_DEPTH_STENCIL_ATTACHMENT);
		RenderPass::s_Context.ssrOutput.fbo->SetUpFrameBuffer();

		m_Shader->Bind();
		m_Shader->setInt("normalTexture", 1);
		m_Shader->setInt("positionTexture", 2);
		m_Shader->setInt("mixtureTexture", 3);
		m_Shader->setInt("colorTexture", 4);
		m_Shader->Unbind();
	}

	void SSRPass::Draw() {
		auto mfbo = RenderPass::s_Context.ssrOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();
		RenderCommand::DisableDepthTest();
		RenderCommand::DisableMultiSample();
		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		// 将Gbuffer的深度模板缓冲拷贝过来（减少fragment绘制）
		RenderCommand::CopyFBODepthStencil(RenderPass::s_Context.geometryOutput.gbuffer, mfbo);
		Ref<Camera> camera = m_Scene->GetCamera();

		m_Shader->Bind();

		m_Shader->setVec3f("viewPos", camera->Position);
		m_Shader->setMat4f("view", camera->getViewMatrix());
		m_Shader->setMat4f("invView", glm::inverse(camera->getViewMatrix()));
		m_Shader->setMat4f("projection", camera->getProjectionMatrix());
		m_Shader->setBool("useSSR", RenderPass::s_Context.useSSR);
		m_Shader->setFloat("thickness", RenderPass::s_Context.ssrThickness);
		m_Shader->setFloat("ssrEffect", RenderPass::s_Context.ssrEffect);

		RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(1)->Bind(1);
		RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(2)->Bind(2);
		RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(3)->Bind(3);
		RenderPass::s_Context.lightOutput.fbo->GetColorTexture()->Bind(4);

		// 打开模板测试，只对有片元存在的像素进行着色计算
		RenderCommand::EnableStencilTest();
		RenderCommand::ChangeStencilOption(EMT::RendererAPI::StencilOption::Keep,
			EMT::RendererAPI::StencilOption::Keep, EMT::RendererAPI::StencilOption::Keep);
		RenderCommand::ChangeStencilFunc(EMT::RendererAPI::CompareFunc::Equal, 1, 0xFF);
		RenderCommand::ChangeStencilMask(0x00);

		Renderer::RenderNDCPlane();

		RenderCommand::DisableStencilTest();
		RenderCommand::EnableDepthTest();
	}

	void SSRPass::OnWindowResize() {
		RenderPass::s_Context.ssrOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}

}
