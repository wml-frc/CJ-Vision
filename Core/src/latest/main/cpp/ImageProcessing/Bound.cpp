#include "ImageProcessing/Bound.h"

namespace CJ {
	void Bound::drawConvexHull(Image &input, Image &output, bool disableDraw) {
		if (input.data.empty()) {
			CJ_CORE_PRINT_ERROR("Can't get input image: " + input.name + ". (Colour Filter)");
			return;
		}

		output.hull.resize(input.contours.size());
		for (size_t i = 0; i < input.contours.size(); i++) {
			cv::convexHull(cv::Mat(input.contours[i]), output.hull[i], false);
		}

		if (!disableDraw) {
			output.data = cv::Mat::zeros(input.data.size(), CV_8UC3);
			for (size_t i = 0; i < input.contours.size(); i++) {
				cv::Scalar colour = cv::Scalar(0, 0, 255);
				cv::drawContours(output.data, output.hull, i, colour);
			}
		}
	}

	BoundingPoints Bound::drawBoundingBox(Image &input, Image &output, bool disableDraw) {
		if (input.data.empty()) {
			CJ_CORE_PRINT_ERROR("Can't get input image: " + input.name + ". (Colour Filter)");
			return {0,0};
		}

		std::vector<std::vector<cv::Point>> contours_poly(input.hull.size());
		std::vector<cv::Rect> boundRect(input.hull.size());

		BoundingPoints center;

		if (!disableDraw) {
			output.data = cv::Mat::zeros(input.data.size(), CV_8UC3);
		}

		for (size_t i = 0; i < input.hull.size(); i++) {
			cv::approxPolyDP(input.hull[i], contours_poly[i], 3, true);
			boundRect[i] = cv::boundingRect(contours_poly[i]);
			center.center_x = boundRect[i].x + boundRect[i].width/2;
			center.center_y = boundRect[i].y + boundRect[i].height/2;

			// Draw
			if (!disableDraw) {
				cv::Point centerRect(center.center_x, center.center_y);
				cv::putText(output.data, "xy(" + std::to_string(center.center_x) + "," + std::to_string(center.center_y) + ")", (centerRect + cv::Point(-25,25)), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,30,255));
				cv::circle(output.data, centerRect, 3, cv::Scalar(255, 0, 0));
				cv::drawContours(output.data, contours_poly, (int)i, cv::Scalar(255,0,0));
				cv::rectangle(output.data, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(255, 0, 0));
			}
		}

		return center;
	}
}