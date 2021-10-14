#include "Vision.h" // required before other #includes
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;

/**
 * Example Layer for Vision App
 */
class ExampleLayer : public CJ::Layer {
 public:
	ExampleLayer(CJ::Application &app) : Layer("Example Layer"), _app(app) {
		CJ_PRINT_INFO("Example Layer Created");
		network = std::make_shared<Network>(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);
	}

	void onAttach() override {
		CJ_PRINT_INFO("Example Layer Attached");
	}

	void onDetach() override {
		CJ_PRINT_WARN("Example Layer Detached");
	}

	void onUpdate() override {
		CJ_PRINT_INFO("Example Layer update");
		_app.setRunning(false);
	}

 private:
	CJ::Application &_app;
	std::shared_ptr<Network> network; // UDP_TransferNT::
};


/**
 * Example Vision application
 */
class ExampleVisionApplication : public CJ::Application {
 public:
	ExampleVisionApplication() : CJ::Application("Example App") {
		CJ_PRINT_INFO("Example App created");
		pushLayer(new ExampleLayer(get()));
	}

	~ExampleVisionApplication() {
		CJ_PRINT_WARN("Test App Destroyed");
	}
};

CJ_CREATE_APPLICATION(ExampleVisionApplication)