#include "Capture/Camera.h"

namespace CJ {
	Camera::~Camera() {
		CJ_CORE_PRINT_WARN("Destroyed Camera, cap released");
		cap.release();
	}

	int Camera::init() {
		CJ_CORE_PRINT_INFO("Creating OpenCV Cap");

		cap.set(cv::CAP_PROP_FPS, config.fps);
		cap.open(config.port, config.apiID);
		if (!cap.isOpened()) {
			CJ_CORE_PRINT_ERROR(config.name + " Failed to open on port: " + std::to_string(config.port));
			return 1;
		}

		CJ_CORE_PRINT_INFO("Setting frame res");
		cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.resHeight);
		cap.set(cv::CAP_PROP_FRAME_WIDTH, config.resWidth);

		if (config.autoExposure) {
			CJ_CORE_PRINT_INFO("Auto exposure enabled");
			cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_autoExpose);
		} else {
			CJ_CORE_PRINT_INFO("Auto exposure disabled");
			cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_manualExpose);
			cap.set(cv::CAP_PROP_EXPOSURE, config.exposure);
			CJ_CORE_PRINT_INFO("Exposure: " + std::to_string(config.exposure));
		}

		return 0;
	}

	void Camera::capture(Image &image) {
		cap.read(image.data);
		if (image.data.empty()) {
			CJ_CORE_PRINT_ERROR("Cannot get input image. Image empty");
		}
	}
}