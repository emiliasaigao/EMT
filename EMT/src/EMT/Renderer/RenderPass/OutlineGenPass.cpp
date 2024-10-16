#include "emtpch.h"
#include "OutlineGenPass.h"

namespace EMT {
	OutlineGenPass::OutlineGenPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/common/OutlineGen.vert", "../EMT/assets/shader/common/OutlineGen.frag");
		m_DepthGenShader = Shader::Create("../EMT/assets/shader/common/DepthGen.vert", "../EMT/assets/shader/common/DepthGen.frag");
		RenderPass::s_Context.outlineGenOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);

		// 写一张Outline遮罩出来
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_R16F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.outlineGenOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RED, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		// 需要深度、模板缓冲
		TextureSettings depthStencilTextureSettings;
		depthStencilTextureSettings.TextureFormat = EMT_DEPTH24_STENCIL8;
		depthStencilTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.HasBorder = true;
		depthStencilTextureSettings.HasMips = false;
		RenderPass::s_Context.outlineGenOutput.fbo->AddDepthStencilTexture(depthStencilTextureSettings, EMT_DEPTH_STENCIL, EMT_UNSIGNED_INT_24_8, EMT_DEPTH_STENCIL_ATTACHMENT);

		RenderPass::s_Context.outlineGenOutput.fbo->SetUpFrameBuffer();
	}

	void OutlineGenPass::Draw() {
		auto mfbo = RenderPass::s_Context.outlineGenOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();
		if (RenderPass::s_Context.outlineGenOutput.model) {
			auto mModel = RenderPass::s_Context.outlineGenOutput.model;

			auto camera = m_Scene->GetCamera();

			// 打开深度、模板测试，绘制被选中物体的深度图，绘制的位置模板值置为1
			RenderCommand::EnableDepthTest();
			RenderCommand::ChangeDepthFunc(EMT::RendererAPI::CompareFunc::Less);
			RenderCommand::EnableStencilTest();
			RenderCommand::ChangeStencilOption(EMT::RendererAPI::StencilOption::Keep,
				EMT::RendererAPI::StencilOption::Keep, EMT::RendererAPI::StencilOption::Replace);
			RenderCommand::ChangeStencilFunc(EMT::RendererAPI::CompareFunc::Always, 1, 0xFF);
			RenderCommand::ChangeStencilMask(0xFF);

			m_DepthGenShader->Bind();
			m_DepthGenShader->setMat4f("view", camera->getViewMatrix());
			m_DepthGenShader->setMat4f("projection", camera->getProjectionMatrix());
			Renderer::SetupModelMatrix(mModel, m_DepthGenShader, false);
			mModel->Draw(m_DepthGenShader, m_Scene->GetFrustumPlanes(), false);

			// 关闭模板测试，只允许在模板值非1的位置绘制图像
			RenderCommand::ChangeStencilOption(EMT::RendererAPI::StencilOption::Keep,
				EMT::RendererAPI::StencilOption::Keep, EMT::RendererAPI::StencilOption::Keep);
			RenderCommand::ChangeStencilFunc(EMT::RendererAPI::CompareFunc::NotEqual, 1, 0xFF);
			RenderCommand::ChangeStencilMask(0x00);

			m_Shader->Bind();
			auto view = camera->getViewMatrix();
			m_Shader->setMat4f("view", view);
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(view)));
			m_Shader->setMat3f("normalMatrix", normalMatrix);
			m_Shader->setMat4f("projection", camera->getProjectionMatrix());
			m_Shader->setFloat("outlineThickness", RenderPass::s_Context.OutlineThickness);
			Renderer::SetupModelMatrix(mModel, m_Shader, false);
			mModel->Draw(m_Shader, m_Scene->GetFrustumPlanes(), false);
			RenderCommand::DisableStencilTest();
		}
	}

	void OutlineGenPass::OnWindowResize() {
		RenderPass::s_Context.outlineGenOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}

}
