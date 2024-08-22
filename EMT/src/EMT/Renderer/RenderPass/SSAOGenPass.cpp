#include "emtpch.h"
#include "SSAOGenPass.h"
#include <random>

namespace EMT {
	SSAOGenPass::SSAOGenPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/postprocess/SSAO.vert", "../EMT/assets/shader/postprocess/SSAO.frag");
		RenderPass::s_Context.ssaoOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_R16F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.ssaoOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RED, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.ssaoOutput.fbo->SetUpFrameBuffer();

        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;
        
        for (int i = 0; i < 64; ++i) {
            // xy随机采样到[-1,1]，z只能采样到[0,1]（正半球）
            // 为什么是z不是y？因为TBN矩阵N在最后，因此最后一维对应法线方向
            glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = float(i) / 64.0;

            // 集中更多采样点至片元附近  
            scale = 0.1 + scale * scale * (1.0f - 0.1f);
            sample *= scale;
            ssaoKernel.push_back(sample);
        }

        // 噪音向量贴图，用来随机获取一个切线方向，变相旋转了样本点集
        for (int i = 0; i < 16; i++) {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                0.0f);
            ssaoNoise.push_back(noise);
        }

        TextureSettings noiseTextureSettings;
        noiseTextureSettings.TextureFormat = EMT_RGB16F;
        noiseTextureSettings.TextureWrapSMode = EMT_REPEAT;
        noiseTextureSettings.TextureWrapTMode = EMT_REPEAT;
        noiseTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
        noiseTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
        noiseTextureSettings.HasMips = false;

        noiseTexture = Texture::Create(noiseTextureSettings);
        noiseTexture->Generate2DTexture(4, 4, EMT_RGB, EMT_FLOAT, &ssaoNoise[0]);

        m_Shader->Bind();
        m_Shader->setInt("positionTexture", 0);
        m_Shader->setInt("normalTexture", 1);
        m_Shader->setInt("noiseMap", 2);
        
        for (int i = 0; i < 64; ++i) {
            m_Shader->setVec3f(std::string("samples[") + std::to_string(i) + std::string("]"), ssaoKernel[i]);
        }
	}

	void SSAOGenPass::Draw() {
		auto mfbo = RenderPass::s_Context.ssaoOutput.fbo;
		mfbo->Bind();
        RenderCommand::DisableDepthTest();
        RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());
		mfbo->Clear();
        
        Ref<Camera> camera = m_Scene->GetCamera();

        m_Shader->Bind();
        RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(2)->Bind(0);
        RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(1)->Bind(1);
        noiseTexture->Bind(2);
        m_Shader->setMat4f("projection", camera->getProjectionMatrix());
        m_Shader->setMat4f("view", camera->getViewMatrix());
        m_Shader->setFloat("windowWidth", RenderPass::s_Context.windowWidth);
        m_Shader->setFloat("windowHeight", RenderPass::s_Context.windowHeight);
        m_Shader->setBool("useSSAO", RenderPass::s_Context.useSSAO);
        m_Shader->setFloat("SSAOEffect", RenderPass::s_Context.SSAOEffect);
        m_Shader->setFloat("SSAORadius", RenderPass::s_Context.SSAORadius);
        m_Shader->setInt("SSAOKernelSize", RenderPass::s_Context.SSAOKernelSize);

        Renderer::RenderNDCPlane();

        mfbo->UnBind();
        RenderCommand::EnableDepthTest();
	}

	void SSAOGenPass::OnWindowResize() {
		RenderPass::s_Context.ssaoOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}



}
