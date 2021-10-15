#ifndef COLOUR_FILTER_H
#define COLOUR_FILTER_H

#include "common_headers.h"
#include "ImageContainer.h"

namespace CJ {

	/**
	 * Filter Image class
	 * Filter image based on colour (HSV)
	 * Filter based on gray scale.
	 * And image pre processing options (including dev toolkit)
	 */
	class ColourFilter {
	 public:

		static const int RETRO_HL = 50;
		static const int RETRO_HH = 70;

		static const int RETRO_SL = 250;
		static const int RETRO_SH = 255;

		static const int RETRO_VL = 30;
		static const int RETRO_VH = 255;

		/**
		 * HSV Options
		 */
		struct HSV_Options {
			int HL = 0,		// hue low
					HH = 255,	// hue high
					SL = 0,		// sat low
					SH = 255,	// sat high
					VL = 0,		// value low
					VH = 255;	// value high

			int erosionSize = 0;
			int dilationSize = 0;
			int blur = 0;
			int binaryThreshold_Low = 0;
			int binaryThreshold_High = 0;
		};

		/**
		 * Gray Scale Options
		 */
		struct GrayScale_Options {
			int erosionSize = 0;
			int dilationSize = 0;
			int blur = 0;
			int binaryThreshold_Low = 0;
			int binaryThreshold_High = 0;
		};

		/**
		 * Filter using hsv
		 */
		static void filter(Image &input, Image &output, HSV_Options options);

		/**
		 * (Adds live trackbar for development)
		 */
		static void createFilterTrackbar(HSV_Options &options);
		
		/**
		 * Filter using gray scale
		 */
		static void filter(Image &input, Image &output, GrayScale_Options options);

		/**
		 * (Adds live trackbar for development)
		 */
		static void createFilterTrackbar(GrayScale_Options &options);
	};
}

#endif