#ifndef IMAGE_CONTAINER_H
#define IMAGE_CONTAINER_H

#include "common_headers.h"

namespace CJ {
	struct Image {
		/**
		 * Data
		 */
		// std::shared_ptr<cv::Mat> data; // Main data pointer
		cv::Mat data;
		std::string name = "Image"; // Name

		/**
		 * Operators
		 */
		// mat operator
		operator cv::Mat() {
			return data;
		}
	};
}

#endif