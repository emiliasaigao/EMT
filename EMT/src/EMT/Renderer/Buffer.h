#pragma once

namespace EMT {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static VertexBuffer* Create(float* vertices, unsigned int size);
	};

	class ElementBuffer {
	public:
		virtual ~ElementBuffer() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual unsigned int GetCount() const = 0;

		static ElementBuffer* Create(unsigned int* indices, unsigned int count);
	};
}