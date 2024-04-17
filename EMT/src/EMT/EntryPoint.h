#pragma once

#ifdef EMT_PLATFORM_WINDOWS

extern EMT::Application* EMT::CreateApplication();

int main(int* argc, int** argv) {
	auto app = EMT::CreateApplication();
	app->Run();
	delete app;
}

#else error EMT ENGINE ONLY SUPPORT WINDOWS!
#endif // EMT_PLATFORM_WINDOWS
