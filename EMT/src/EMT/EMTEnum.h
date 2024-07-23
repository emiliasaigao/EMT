#pragma once

namespace EMT {
	// ToDO
	// 原本是为了将Texture抽象出来，以适配多个渲染API，需要定义Texture的各种枚举类
	// 并且在各个API实现Texture的时候，还要写转换函数，工程量较大，之后再考虑剥离
	enum class TextureFormat {
		NONE,
	};
}