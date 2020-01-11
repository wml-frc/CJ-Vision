#include "VisTrack.h"

CJ::VisionTracking visionProcessing;

cv::Mat LocalProcessImage_Contours;
cv::Mat LocalProcessImage_Hull;

cv::RNG rng(12345);

void CannyTrackThread(cv::Mat *InputImage, cv::Mat *Canny_Output, int Threshold) {
  while (true) {
    cv::Canny(*InputImage, *Canny_Output, Threshold, Threshold * 2);
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::CannyTrack(cv::Mat *Image, cv::Mat *Canny_Output, int Threshold) {
  std::thread CannyThread(CannyTrackThread, Image, Canny_Output, Threshold);
  CannyThread.detach();
}

void ThreshImageThreadFunction(cv::Mat *Image, int ThreshMin, int ThreshMax) {
  while (true) {
    cv::threshold(*Image, *Image, ThreshMin, ThreshMax, cv::THRESH_BINARY);
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::ThresholdImage(cv::Mat *Image, int ThreshMin, int ThreshMax) {
  std::thread ThresholdImageThread(ThreshImageThreadFunction, Image, ThreshMin, ThreshMax);
  ThresholdImageThread.detach();
}

void ContourDetectThread(cv::Mat *Image, std::vector<std::vector<cv::Point> > *Contours) {
  
  while (true) {
    cv::findContours(*Image, *Contours, cv::RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
      cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
    }
    for( size_t i = 0; i< contours.size(); i++ ) {
      cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
      drawContours( *Image, contours_poly, (int)i, color );
    }
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::ContourDetect(cv::Mat *Image, std::vector<std::vector<cv::Point> > *contours) {
  std::thread ContourThread(ContourDetectThread, Image, contours);
  ContourThread.detach();
}


void GetHullThread(cv::Mat *Image) {
  while (true) {
    std::vector<std::vector<cv::Point> > contours; 
    cv::findContours(*Image, contours, cv::RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> >hull( contours.size());
    //cv::cvtColor(*Image, *Image, CV_GRAY2RGB);
    for (size_t i = 0; i < contours.size(); i++) {
      convexHull( contours[i], hull[i] );
      drawContours(*Image, contours, (int)i, cv::Scalar(0,100,255));
    }
  }
}
void CJ::VisionProcessing::VisionHullGeneration::GetHull(cv::Mat *Image) {
  std::thread HullThread(GetHullThread, Image);
  HullThread.detach();
}

void BoundingBoxThread(cv::Mat *Image, cv::Mat *OutputImage, double *CenterX, double *CenterY, double Contour_Size_Threshold) {
  cv::Point CenterOfBoundingBox;
  cv::Point TextOffset;


  while (true) {
    std::vector<std::vector<cv::Point> > contours;
    //contours = *Contours;
    cv::findContours(*Image, contours, cv::RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    std::vector<cv::Point2f> centers(contours.size());
    std::vector<float> radius( contours.size());
    std::vector<std::vector<cv::Point> >hull( contours.size() );

    std::stringstream cx;
    std::stringstream cy;

    for (std::vector<std::vector<cv::Point> >::iterator it = contours.begin(); it!=contours.end(); ) {
      if (it->size() < Contour_Size_Threshold)
        it=contours.erase(it);
      else
        ++it;
    }

    for (size_t i = 0; i < contours.size(); i++) {
      convexHull( contours[i], hull[i] );
      cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
      boundRect[i] = cv::boundingRect(contours_poly[i]);
      minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
      *CenterX = boundRect[i].x + boundRect[i].width/2;
      *CenterY = boundRect[i].y + boundRect[i].height/2;
      CenterOfBoundingBox = (boundRect[i].br() + boundRect[i].tl()) *0.5;
      cx << *CenterX;
      cy << *CenterY;
    }

    cv::cvtColor(*Image, *OutputImage, CV_GRAY2RGB);
    TextOffset = CenterOfBoundingBox + cv::Point(-25, 25);
    for( size_t i = 0; i< contours.size(); i++ ) {
      cv::Scalar color = cv::Scalar( rng.uniform(0,256), rng.uniform(0,256), rng.uniform(0,256));
      drawContours(*OutputImage, contours_poly, (int)i, color );
      rectangle(*OutputImage, boundRect[i].tl(), boundRect[i].br(), color, 2 );
    }
    circle(*OutputImage, CenterOfBoundingBox, 3, cv::Scalar(0, 30, 255));
    cv::putText(*OutputImage, "xy(" + cx.str() + "," + cy.str() + ")", TextOffset, cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,30,255));
  }
}
void CJ::VisionProcessing::VisionHullGeneration::BoundingBox(cv::Mat *Image, cv::Mat *OutputImage, double *CenterX, double *CenterY, double Contour_Size_Threshold) {
  std::thread BoundingThread(BoundingBoxThread, Image, OutputImage, CenterX, CenterY, Contour_Size_Threshold);
  BoundingThread.detach();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Bounding Box Thread Setup Complete" << std::endl;
}

void CJ::VisionProcessing::VisionDetectionType::CentroidDetect(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionDetectionType::RectangleDetect(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionDetectionType::CircleDetect(cv::Mat *Image) {

}