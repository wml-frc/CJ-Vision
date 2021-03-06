#include "Output.h"

namespace CJ {
	void Output::Display(Image *image) {
		if (image->data.empty()) {
			std::cerr << "Display Error: Image empty" << std::endl;
		} else {
				// Temp (imshow doesn't work without GUI. << Pi error)
			#ifdef COPROC
				// Use other means of displaying image. (Web Stream)
			#else
				cv::imshow(image->name, image->data);
			#endif
		}
		cv::waitKey(30);
	}
}