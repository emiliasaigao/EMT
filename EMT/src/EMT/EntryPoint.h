#pragma once

#ifdef EMT_PLATFORM_WINDOWS

extern EMT::Application* EMT::CreateApplication();

int main(int* argc, int** argv) {
	EMT::Log::Init();
	EMT::Log::GetCoreLogger()->warn("Hello this's EMT!");
	EMT::Log::GetCilentLogger()->error("Hello this's Cilent!");
	auto app = EMT::CreateApplication();
	app->Run();
	delete app;
}

#else error EMT ENGINE ONLY SUPPORT WINDOWS!
#endif // EMT_PLATFORM_WINDOWS
