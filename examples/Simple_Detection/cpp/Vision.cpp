#include "Vision.h" // required before other #includes
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;

/**
 * Container for images passed between layers
 */
struct Images {
	CJ::Image origin;
	CJ::Image filtered;
	CJ::Image processed;
};

/**
 * Camera Capture Layer
 */
class CaptureLayer : public CJ::Layer {
 public:
	CaptureLayer(CJ::Application &app, CJ::Image &origin) : Layer("Capture Layer"), _app(app), _origin(origin) {
		CJ_PRINT_INFO("Capture Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Capture Layer Attached");
		_camera.config.port = 0;
		_camera.config.fps = 60;
		_camera.config.autoExposure = true;

		_camera.config.name = "Cam";
		_origin.name = "Origin Image";

		if (_camera.init() != 0) {
			CJ_PRINT_ERROR("Could not init camera");
			_app.setRunning(false);
		}
	}

	void onDetach() override {
		CJ_PRINT_WARN("Capture Layer Detached");
	}

	void onUpdate() override {
		_camera.capture(_origin);
	}

 private:
	CJ::Application &_app;
	CJ::Camera _camera; // main camera
	CJ::Image &_origin; // reference to origin passed into layer
};


class ProcessingLayer : public CJ::Layer {
 public:
	ProcessingLayer(CJ::Application &app, Images &images) : Layer("Processing Layer"), _app(app), _images(images) {
		CJ_PRINT_INFO("Processing Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Processing Layer Attached");
		_images.filtered.name = "Filtered Images";
		_hsv_options = { // green track for low exposure logitech 1080p webcam
			30, // H
			83,

			113, // S
			214,

			104, // V
			168,

			0, // Erosion
			15 // Dilation
		};

		_images.filtered.name = "Filtered";
		_images.processed.name = "Processed Image";


		// Create filter trackbar to edit hsv_options
		CJ::ColourFilter::createFilterTrackbar(_hsv_options);
	}

	void onDetach() override {
		CJ_PRINT_INFO("Processing Layer Detached");
	}

	void onUpdate() override {
		// Filter image
		CJ::ColourFilter::filter(_images.origin, _images.filtered, _hsv_options);

		// Detect Contours
		CJ::Contours::detectContours(_images.filtered, _images.processed);
		
		// Draw Convex Around contours
		CJ::Bound::drawConvexHull(_images.processed, _images.processed);

		// Draw Bounding Box
		CJ::Bound::drawBoundingBox(_images.processed, _images.processed);
	}

 private:
	CJ::Application &_app;
	Images &_images;

	// HSV Options
	CJ::ColourFilter::HSV_Options _hsv_options;
};

/**
 * Output Layer
 * Has no real purpose unless specific output methods are needed
 * I.e MJPEG Streaming
 */
class OutputLayer : public CJ::Layer {
 public:
	OutputLayer(CJ::Application &app, Images &images) : Layer("Output Layer"), _app(app), _images(images) {
		CJ_PRINT_INFO("Output Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Output Layer Attached");
	}

	void onDetach() override {
		CJ_PRINT_INFO("Output Layer Detached");
	}

	void onUpdate() override {
		if (CJ::Output::display(30, _images.origin, _images.filtered, _images.processed) == 27) {
			CJ_PRINT_WARN("Esc Called");
			_app.setRunning(false);
		}
	}

 private:
	CJ::Application &_app;
	Images &_images;
};


/**
 * Simple Detection Vision application
 */
class SimpleDetection : public CJ::Application {
 public:
	SimpleDetection() : CJ::Application("Simple Detection") {
		CJ_PRINT_INFO("App created");
		pushLayer(new CaptureLayer(get(), _images.origin));
		pushLayer(new ProcessingLayer(get(), _images));
		pushOverlay(new OutputLayer(get(), _images));
	}

	~SimpleDetection() {
		CJ_PRINT_WARN("App Destroyed");
	}

 private:

	/**
	 * Images shared between layers
	 */
	Images _images;
};

CJ_CREATE_APPLICATION(SimpleDetection)