#include "Vision.h" // required before other #includes
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;

/**
 * Example Layer for Vision App
 * (All in one layer. Webcam, networking and display. Should be split up into multiple layers.)
 */
class ExampleLayer : public CJ::Layer {
 public:
	ExampleLayer(CJ::Application &app) : Layer("Example Layer"), _app(app) {
		CJ_PRINT_INFO("Example Layer Created");
		_network = std::make_shared<Network>(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);
	}

	void onAttach() override {
		CJ_PRINT_INFO("Example Layer Attached");
		_camera.config.port = 0;
		_camera.config.fps = 60;
		_camera.config.autoExposure = true;

		_camera.config.name = "Example Camera";
		_image.name = "Example Image";

		if (_camera.init() != 0) {
			CJ_PRINT_ERROR("Could not init camera");
			_app.setRunning(false);
		}
	}

	void onDetach() override {
		CJ_PRINT_WARN("Example Layer Detached");
	}

	void onUpdate() override {
		_camera.capture(_image);
		CJ::Output::display(30, _image);
	}

 private:
	CJ::Application &_app;
	CJ::Image _image;
	CJ::Camera _camera;
	std::shared_ptr<Network> _network; // UDP_TransferNT::
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