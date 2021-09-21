#include "ImageProcessing/ColourFilter.h"
#include <thread>

namespace CJ {
	void ColourFilter::filter(Image &input, Image &output, HSV_Options options) {
		if (input.data.empty()) {
			CJ_CORE_PRINT_ERROR("Can't get input image: " + input.name + ". (Colour Filter)");
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

	void ColourFilter::filter(Image &input, Image &output, GrayScale_Options options) {
		if (input.data.empty()) {
			CJ_CORE_PRINT_ERROR("Can't get input image: " + input.name + ". (Colour Filter)");
			return;
		}

		cv::cvtColor(input.data, output.data, cv::COLOR_BGR2GRAY);

		// Erosion and Dilation
		if (options.erosionSize > 0) { cv::erode(output.data, output.data, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(options.erosionSize, options.erosionSize))); }
		if (options.dilationSize > 0) { cv::dilate(output.data, output.data, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(options.dilationSize, options.dilationSize))); }

		// Blur
		if (options.blur > 0) { cv::blur(output.data, output.data, cv::Size(options.blur, options.blur)); }

		// Bin thresh
		if (options.binaryThreshold_Low > 0 || options.binaryThreshold_High > 0) { cv::threshold(output.data, output.data, options.binaryThreshold_Low, options.binaryThreshold_High, cv::THRESH_BINARY); }
	}

	void ColourFilter::createFilterTrackbar(HSV_Options &options) {
		#ifndef COPROC
		std::string windowName = "Colour Filter Editor";
		cv::namedWindow(windowName);
		cv::createTrackbar("Hue Low", windowName, &options.HL, 255);
		cv::createTrackbar("Hue High", windowName, &options.HH, 255);

		cv::createTrackbar("Saturation Low", windowName, &options.SL, 255);
		cv::createTrackbar("Saturation High", windowName, &options.SH, 255);

		cv::createTrackbar("Value Low", windowName, &options.VL, 255);
		cv::createTrackbar("Value High", windowName, &options.VH, 255);

		cv::createTrackbar("Erosion", windowName, &options.erosionSize, 255);
		cv::createTrackbar("Dilation", windowName, &options.dilationSize, 255);

		cv::createTrackbar("Blur", windowName, &options.blur, 255);

		cv::createTrackbar("Binary Thresh Low", windowName, &options.binaryThreshold_Low, 255);
		cv::createTrackbar("Binary Thresh High", windowName, &options.binaryThreshold_High, 255);
		#endif
	}

	void ColourFilter::createFilterTrackbar(GrayScale_Options &options) {
		#ifndef COPROC
		std::string windowName = "Gray Filter Editor";
		cv::namedWindow(windowName);
		cv::createTrackbar("Erosion", windowName, &options.erosionSize, 255);
		cv::createTrackbar("Dilation", windowName, &options.dilationSize, 255);

		cv::createTrackbar("Blur", windowName, &options.blur, 255);

		cv::createTrackbar("Binary Thresh Low", windowName, &options.binaryThreshold_Low, 255);
		cv::createTrackbar("Binary Thresh High", windowName, &options.binaryThreshold_High, 255);
		#endif
	}
};