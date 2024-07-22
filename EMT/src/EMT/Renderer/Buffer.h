#pragma once
#include "EMT/Core.h"

namespace EMT {

	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, FLoat4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
			case EMT::ShaderDataType::None:		return 0;
			case EMT::ShaderDataType::Float:	return 4;
			case EMT::ShaderDataType::Float2:	return 2 * 4;
			case EMT::ShaderDataType::Float3:	return 3 * 4;
			case EMT::ShaderDataType::FLoat4:	return 4 * 4;
			case EMT::ShaderDataType::Int:		return 4;
			case EMT::ShaderDataType::Int2:		return 2 * 4;
			case EMT::ShaderDataType::Int3:		return 3 * 4;
			case EMT::ShaderDataType::Int4:		return 4 * 4;
			case EMT::ShaderDataType::Mat3:		return 4 * 3 * 3;
			case EMT::ShaderDataType::Mat4:		return 4 * 4 * 4;
			case EMT::ShaderDataType::Bool:		return 1;
		}
		EMT_CORE_ASSERT(false, "未知类型!");
		return 0;
	}

	// 顶点属性抽象结构体
	struct BufferElement {
		ShaderDataType Type;
		std::string Name;
		unsigned int Size;
		unsigned int Offset;
		bool Normalized;

		unsigned int GetComponentCount() const {
			switch (Type)
			{
				case EMT::ShaderDataType::None:		return 0;
				case EMT::ShaderDataType::Float:	return 1;
				case EMT::ShaderDataType::Float2:	return 2;
				case EMT::ShaderDataType::Float3:	return 3;
				case EMT::ShaderDataType::FLoat4:	return 4;
				case EMT::ShaderDataType::Int:		return 1;
				case EMT::ShaderDataType::Int2:		return 2;
				case EMT::ShaderDataType::Int3:		return 3;
				case EMT::ShaderDataType::Int4:		return 4;
				case EMT::ShaderDataType::Mat3:		return 3 * 3;
				case EMT::ShaderDataType::Mat4:		return 4 * 4;
				case EMT::ShaderDataType::Bool:		return 1;
			}
			EMT_CORE_ASSERT(false, "未知类型!");
			return 0;
		}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) 
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}
	};

	// 保存所有结构体并设置缓冲区布局的类
	class BufferLayout {
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetBufferElements() const { return m_Elements; }
		inline const unsigned int GetStribe() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride;

		void CalculateOffsetAndStride() {
			unsigned int offset = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
			}
			m_Stride = offset;
		}
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(float* vertices, unsigned int size);
	};

	class ElementBuffer {
	public:
		virtual ~ElementBuffer() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual unsigned int GetCount() const = 0;

		static Ref<ElementBuffer> Create(unsigned int* indices, unsigned int count);
	};
}