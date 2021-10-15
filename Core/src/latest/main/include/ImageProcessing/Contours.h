#ifndef CONTOURS_H
#define CONTOURS_H

#include "common_headers.h"
#include "ImageContainer.h"

namespace CJ {
	class Contours {
	 public:
		/**
		 * Detect Edges using canny algorithm
		 */
		static void cannyTrack(Image &input, Image &output, int threshold);

		/**
		 * Detect contours (Stores inside Image)
		 * Disable draw is false by deault. Speed up prograb by setting true
		 */
		static void detectContours(Image &input, Image &output, bool disableDraw = false);

		/**
		 * Creates live trackbar for contours
		 */
		static void createContoursTrackbar(int &threshold);
	};
}

#endif