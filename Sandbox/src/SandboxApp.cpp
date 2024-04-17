#include <EMT.h>

class SandboxApp : public EMT::Application {
public:
	SandboxApp(){}
	~SandboxApp(){}
};

EMT::Application* EMT::CreateApplication() {
	return new SandboxApp();
}
