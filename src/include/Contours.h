#ifndef CONTOURS_H
#define CONTOURS_H
namespace CJ {
  class Contours {
   public:
    /**
     * Detect Edges using canny algorithm
     */
    void cannyTrack();

    /**
     * Detect Contours in image
     */
    void detectContours();

    /**
     * Detect edges by thresholding image until point of interest is left
     */
    void thresholdImage();
  };
}
#endif