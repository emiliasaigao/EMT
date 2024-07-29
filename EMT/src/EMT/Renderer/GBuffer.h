#pragma once
#include "FrameBuffer.h"

namespace EMT {
	class GBuffer : virtual public FrameBuffer {
	public:
		static Ref<GBuffer> Create(int width, int height);
		virtual inline Ref<Texture> GetColorTextureByIndex(int index) = 0;
		virtual inline std::vector<Ref<Texture>>& GetColorTextures() = 0;
	};
}