#ifndef OUTPUT_H
#define OUTPUT_H

#include "ImageContainer.h"

namespace CJ {

	/**
	 * Main output class
	 */
	class Output {
	 public:

		/**
		 * Input wait in ms, and diplay image/s
		 * Returns waitkey output, or returns -1 on error
		 */
		static void display() {
			return;
		}

		/**
		 * Input wait in ms, and diplay image/s
		 * Returns waitkey output, or returns -1 on error
		 */
		template<typename T, typename... Args>
		static void display(T image, Args... rest) {
			#ifndef COPROC
			if (image.data.empty()) {
				CJ_CORE_PRINT_ERROR("Display Error: " + image.name + ". Is empty");
			} else {
				cv::imshow(image.name, image.data);
			}

			display(rest...);
			#endif
		}

		/**
		 * Input wait in ms, and diplay image/s
		 * Returns waitkey output, or returns -1 on error
		 */
		template<typename T, typename... Args>
		static int display(int wait_ms, T image, Args... rest) {
			#ifndef COPROC
			if (image.data.empty()) {
				CJ_CORE_PRINT_ERROR("Display Error: " + image.name + ". Is empty");
				return -1;
			} else {
				cv::imshow(image.name, image.data);
			}
			
			display(rest...);

			if (wait_ms != 0) {
				int key = cv::waitKey(wait_ms);
				return key;
			}

			#endif
			return 0;
		}
	};
}

#endif