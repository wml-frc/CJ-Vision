#include "Vision.h" // required before other #includes

CJ::Camera::Cam cam;
CJ::Image inputImage;

void Vision::init() {
	/**
	 * Logic/Code that is placed in this function runs only once, used to perform
	 * initial setup of the program before the looping code
	 */
	CJ::Core::init();

	cam.config = {
		0,
		cv::CAP_ANY,
		30,
		640,
		480,
		0.1,
		true,
		"CJ Cam"
	};

	CJ::Core::setupVision(&inputImage, &cam); // bind the image to the camera, and run cam in loop
}

void Vision::periodic() {
 /**
	* Logic here will run periodically (looping). Place code in here that needs constant processing
	* (Contour detection, networking, colour filtering....)
	*/

	CJ::Output::Display(&inputImage); // display the image locally (doesn't display on coproc)
}