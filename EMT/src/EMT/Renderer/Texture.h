#pragma once
#include "EMT/Core.h"

namespace EMT {
	class Texture {
	public:
		virtual unsigned int GetHeight() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual ~Texture() {}
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}