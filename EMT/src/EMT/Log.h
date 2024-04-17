#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"

namespace EMT {

	class Log {
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetCilentLogger() { return s_CilentLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_CilentLogger;
	};
}
