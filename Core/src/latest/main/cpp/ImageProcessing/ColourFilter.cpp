#include "ImageProcessing/ColourFilter.h"
#include <thread>

namespace CJ {
	cv::Mat localMat;
	void ColourFilter::filter(Image &input, Image &output, HSV_Options options) {
		if (input.data.empty()) {
			CJ_CORE_PRINT_ERROR("Can't get input image. (Colour Filter)");
			return;
		}

		cv::cvtColor(input.data, output.data, cv::COLOR_BGR2HSV);
		cv::inRange(output.data, cv::Scalar(options.HL, options.SL, options.VL),
														 cv::Scalar(options.HH, options.SH, options.VH), output.data);

		// Erosion and Dilation
		if (options.erosionSize > 0) { cv::erode(output.data, output.data, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(options.erosionSize, options.erosionSize))); }
		if (options.dilationSize > 0) { cv::dilate(output.data, output.data, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(options.dilationSize, options.dilationSize))); }

		// Blur
		if (options.blur > 0) { cv::blur(output.data, output.data, cv::Size(options.blur, options.blur)); }

		// Bin thresh
		if (options.binaryThreshold_Low > 0 || options.binaryThreshold_High > 0) { cv::threshold(output.data, output.data, options.binaryThreshold_Low, options.binaryThreshold_High, cv::THRESH_BINARY); }
	}

	void ColourFilter::createFilterTrackbar(HSV_Options &options) {
		
	}
};