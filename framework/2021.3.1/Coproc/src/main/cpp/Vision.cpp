#include "Vision.h" // required before other #includes

bool closeApp = false;

/**
 * Example Layer for Vision App
 */
class ExampleLayer : public CJ::Layer {
 public:
	ExampleLayer() : Layer("Example Layer") {
		CJ_PRINT_INFO("Example Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Example Layer Attached");
	}

	void onDetach() override {
		CJ_PRINT_WARN("Example Layer Detached");
	}

	void onUpdate() override {
		// CJ_PRINT("Example Layer update");
		CJ_PRINT_TRACE("Example Layer On Update");
		// closeApp = true;
	}
};


/**
 * Example Vision application
 */
class ExampleVisionApplication : public CJ::Application {
 public:
	ExampleVisionApplication() : CJ::Application(closeApp, "Example App") {
		CJ_PRINT_INFO("Example App created");
		pushLayer(new ExampleLayer());
	}

	~ExampleVisionApplication() {
		CJ_PRINT_WARN("Test App Destroyed");
	}
};

CJ_CREATE_APPLICATION(ExampleVisionApplication);