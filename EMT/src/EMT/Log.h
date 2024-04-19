#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace EMT {

	class EMT_API Log {
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetCilentLogger() { return s_CilentLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_CilentLogger;
	};
}

#define EMT_CORE_TRACE(...)		::EMT::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EMT_CORE_INFO(...)		::EMT::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EMT_CORE_WARN(...)		::EMT::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EMT_CORE_ERROR(...)		::EMT::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EMT_CORE_FATAL(...)		::EMT::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define EMT_TRACE(...)		::EMT::Log::GetCilentLogger()->trace(__VA_ARGS__)
#define EMT_INFO(...)		::EMT::Log::GetCilentLogger()->info(__VA_ARGS__)
#define EMT_WARN(...)		::EMT::Log::GetCilentLogger()->warn(__VA_ARGS__)
#define EMT_ERROR(...)		::EMT::Log::GetCilentLogger()->error(__VA_ARGS__)
#define EMT_FATAL(...)		::EMT::Log::GetCilentLogger()->fatal(__VA_ARGS__)
