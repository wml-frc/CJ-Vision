#ifndef COLOUR_FILTER_H
#define COLOUR_FILTER_H

#include "cjHeaders.h"
#include "ImageContainer.h"

namespace CJ {
	class ColourFilter {

	public:

		static const int RETRO_H_MIN = 50;
		static const int RETRO_H_MAX = 70;

		static const int RETRO_S_MIN = 250;
		static const int RETRO_S_MAX = 255;

		static const int RETRO_V_MIN = 30;
		static const int RETRO_V_MAX = 255;


		struct Options {
			int HColourLowRange = 0, 
					HColourHighRange = 255,
					SColourLowRange = 0,
					SColourHighRange = 255,
					VColourLowRange = 0,
					VColourHighRange = 255;

			int ErosionSize = 0;
			int DilationSize = 0;
			bool GreyScale = false;
		};

		static void filter(Image *inputImage, Image *outputImage, Options options);
	};

	class ColorFilter {
	public:

		static const int RETRO_H_MIN = 50;
		static const int RETRO_H_MAX = 70;

		static const int RETRO_S_MIN = 250;
		static const int RETRO_S_MAX = 255;

		static const int RETRO_V_MIN = 30;
		static const int RETRO_V_MAX = 255;

		struct Options {
			int HColorLowRange = 0, 
					HColorHighRange = 255,
					SColorLowRange = 0,
					SColorHighRange = 255,
					VColorLowRange = 0,
					VColorHighRange = 255;
			
			int ErosionSize = 0;
			int DilationSize = 0;
			bool GrayScale = false;
		};

		static void filter(Image *inputImage, Image *outputImage, Options options) {
			ColourFilter::filter(inputImage, outputImage, {
				options.HColorLowRange,
				options.HColorHighRange,
				options.SColorLowRange,
				options.SColorHighRange,
				options.VColorLowRange,
				options.VColorLowRange,
				options.ErosionSize,
				options.DilationSize,
				options.GrayScale});
		}
	};
}
#endif