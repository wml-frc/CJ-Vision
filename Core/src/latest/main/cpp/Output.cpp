#include "Output.h"
#include <cstdarg>

namespace CJ {
	// int Output::Display(int wait_ms, ...) {
	// 	#ifndef COPROC
	// 	va_list args;
	// 	va_start(args, wait_ms);
	// 	Image image = va_arg(args, Image);
	// 	if (image.data.empty()) {
	// 		CJ_CORE_PRINT_ERROR("Display Error, " + image.name + ": is empty");
	// 		return -1;
	// 	} else {
	// 		cv::imshow(image.name, image.data);
	// 	}
	// 	va_end(args);
	// 	int key = cv::waitKey(wait_ms);
	// 	return key;
	// 	#endif

	// 	return 0;
	// }

	// template<typename Img, typename ... Imgs>
	// int Output::_display(Img image, Imgs... rest) {
	// 	if (image.data.empty()) {
	// 		CJ_CORE_PRINT_ERROR("Display Error, " + image.name + ": is empty");
	// 		return -1;
	// 	} else {
	// 		cv::imshow(image.name, image.data);
	// 	}

	// 	_display(rest...);
	// }

	// template<typename Img, typename ... Imgs>
	// int Output::display(int wait_ms, Img image, Imgs... rest) {
	// 	#ifndef COPROC
	// 	if (image.data.empty()) {
	// 		CJ_CORE_PRINT_ERROR("Display Error, " + image.name + ": is empty");
	// 		return -1;
	// 	} else {
	// 		cv::imshow(image.name, image.data);
	// 	}
	// 	display(0, rest...);
	// 	int key = cv::waitKey(wait_ms);
	// 	return key;
	// 	#endif

	// 	return 0;
	// }
}