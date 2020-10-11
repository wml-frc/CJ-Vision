#include "Contours.h"
#include <thread>
#include <vector>

cv::RNG rng(12345); // Best bit of code ever

namespace CJ {
	void cannyTrack_thread(Image *inputImage, Image *outputImage, int threshold) {
		while (PROG::THREADS_RUNNING()) {
			cv::Canny(inputImage->data, outputImage->data, threshold, threshold*2);
		}
	}

	void CJ::Contours::cannyTrack(Image *inputImage, Image *outputImage, int threshold)  {
		while(inputImage->data.empty()) {
			std::cout << "Can't get input image. (canny thread)" << std::endl;
		}
		std::thread cannyTrack_t(cannyTrack_thread, inputImage, outputImage, threshold);
		while (outputImage->data.empty()) {
			std::cout << "Can't get output image. (canny thread)" << std::endl;
		}
		cannyTrack_t.detach();
	}

	void detectContours_thread(Image *inputImage, Image *outputImage, int threshold) {
		cv::Mat processing;
		while (PROG::THREADS_RUNNING()) {
			std::vector<cv::Vec4i> hierarchy;
			std::vector<std::vector<cv::Point>> contours;
			
			cv::Canny(inputImage->data, processing, threshold, threshold*2);
			cv::findContours(processing, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

			outputImage->data = cv::Mat::zeros(processing.size(), CV_8UC3);
			for (int i = 0; i < contours.size(); i++) {
				cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
				cv::drawContours(outputImage->data, contours, i, color);
			}
		}
	}

	void CJ::Contours::detectContours(Image *inputImage, Image *outputImage, int threshold) {
		while (inputImage->data.empty()) {
			std::cout << "Can't get input image. (detect contours thread)" << std::endl;
		}
		std::thread detectContours_t(detectContours_thread, inputImage, outputImage, threshold);
		while (outputImage->data.empty()) {
			std::cout << "Can't get output image. (detect contours thread)" << std::endl;
		}
		detectContours_t.detach();
	}
}

