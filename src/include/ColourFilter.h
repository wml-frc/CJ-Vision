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
          SColourHighRange = 0,
          SColourLowRange = 255,
          VColourHighRange = 0,
          VColourLowRange = 255;

      int ErosionSize = 0;
      int dilationSize = 0;
    };

    static void filter(Image *inputImage, Image *outputImage, Options options);
    static void filter(Image *inputImage, Image *outputImage, int ErosionSize = 0, int dilationSize = 0) {
      Options options;
      options.ErosionSize = ErosionSize;
      options.dilationSize = dilationSize;
      filter(inputImage, outputImage, options);
    }
    static void filter(Image *inputImage, Image *outputImage, int HColourLowRange = 0, int HColourHighRange = 255, int SColourLowRange = 0, int SColourHighRange = 255, int VColourLowRange = 0, int VColourHighRange = 255, int ErosionSize = 0, int dilationSize = 0) {
      Options options;
      options.HColourLowRange = HColourLowRange;
      options.HColourHighRange = HColourHighRange;
      options.SColourLowRange = SColourLowRange;
      options.SColourHighRange = SColourHighRange;
      options.VColourLowRange = VColourLowRange;
      options.VColourHighRange = VColourHighRange;
      options.ErosionSize = ErosionSize;
      options.dilationSize = dilationSize;
      filter(inputImage, outputImage, options);
    }
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
          SColorHighRange = 0,
          SColorLowRange = 255,
          VColorHighRange = 0,
          VColorLowRange = 255;
      
      int ErosionSize = 0;
      int dilationSize = 0;
    };

    static void filter(Image *inputImage, Image *outputImage, Options options) {
      ColourFilter::filter(inputImage, outputImage, options.HColorLowRange, options.HColorHighRange, options.SColorLowRange, options.SColorHighRange, options.VColorLowRange, options.VColorHighRange, options.ErosionSize, options.dilationSize);
    }
    static void filter(Image *inputImage, Image *outputImage, int ErosionSize = 0, int dilationSize = 0) {
      Options options;
      options.ErosionSize = ErosionSize;
      options.dilationSize = dilationSize;
      filter(inputImage, outputImage, options);
    }
    static void filter(Image *inputImage, Image *outputImage, int HColorLowRange = 0, int HColorHighRange = 255, int SColorLowRange = 0, int SColorHighRange = 255, int VColorLowRange = 0, int VColorHighRange = 255, int ErosionSize = 0, int dilationSize = 0) {
      Options options;
      options.HColorLowRange = HColorLowRange;
      options.HColorHighRange = HColorHighRange;
      options.SColorLowRange = SColorLowRange;
      options.SColorHighRange = SColorHighRange;
      options.VColorLowRange = VColorLowRange;
      options.VColorHighRange = VColorHighRange;
      options.ErosionSize = ErosionSize;
      options.dilationSize = dilationSize;
      filter(inputImage, outputImage, options);
    }
  };
}
#endif