#ifndef CONTOURS_H
#define CONTOURS_H

#include "cjHeaders.h"
#include "ImageContainer.h"

namespace CJ {
	class Contours {
	public:
		/**
		 * Detect Edges using canny algorithm
		 */
		static void cannyTrack(Image *inputImage, Image *outputImage, int threshold);

		/**
		 * Detect Contours in image (Stores contours in Image::Contours)
		 */
		static void detectContours(Image *image, int threshold);

		/**
		 * Detect Contours in image (Stores contours in Image::Contours)
		 */
		static void detectContours(Image *inputImage, Image *outputImage, int threshold);

		/**
		 * Detect edges by thresholding image until point of interest is left
		 */
		static void thresholdImage(Image *inputImage, Image *outputImage, int thresh_min, int thresh_max);
	};
}
#endif