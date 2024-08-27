#pragma once
#include "Buffer.h"
#include "EMT/Core.h"

namespace EMT {
	class VertexArray {
	public:
		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetElementBuffer(const Ref<ElementBuffer>& elementBuffer) = 0;

		virtual const esgstl::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<ElementBuffer>& GetElementBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}