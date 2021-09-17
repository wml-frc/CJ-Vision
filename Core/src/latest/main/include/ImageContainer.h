#ifndef IMAGE_CONTAINER_H
#define IMAGE_CONTAINER_H

#include "common_headers.h"

namespace CJ {
	struct Image {
		/**
		 * Data
		 */
		cv::Mat data; // Main image data
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