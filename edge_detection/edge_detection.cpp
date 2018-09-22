#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "sobel.cpp"
#include "prewitt.cpp"
#include "canny.cpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat src, gray_src, gray_src_binary, dst, dst_binary;

int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;
const char* window_name = "Binarization";
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";

static void Threshold_Demo( int, void* ){
    /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
    */
    threshold( gray_src_binary, dst_binary, threshold_value, max_binary_value, threshold_type );
    imshow( window_name, dst_binary );

    //  SOBEL
    Mat sobel_dst = sobel(dst_binary);
    namedWindow("Sobel edges", CV_WINDOW_AUTOSIZE);
    imshow("Sobel edges", sobel_dst);

    //  PREWITT
    Mat prewitt_dst = prewitt(dst_binary);
    namedWindow("Prewitt edges", CV_WINDOW_AUTOSIZE);
    imshow("Prewitt edges", prewitt_dst);

    //  CANNY
    canny(dst_binary);
}

int main( int argc, char** argv ){
  VideoCapture cap;
  // open the default camera, use something different from 0 otherwise
  if(!cap.open(0)){
    printf("webcam is not active! exiting now...\n");
    return 0;
  }
  printf("Starting. Press ESC key to quit.\n");

  // Create a window
  namedWindow("Brightness and Contrast", 1);

  //Create trackbar to change brightness
  int iSliderValue1 = 50;
  createTrackbar("Brightness", "Brightness and Contrast", &iSliderValue1, 100);

  //Create trackbar to change contrast
  int iSliderValue2 = 50;
  createTrackbar("Contrast", "Brightness and Contrast", &iSliderValue2, 100);

  for(;;){
      cap >> src;
      cvtColor(src, gray_src, CV_BGR2GRAY);
      //  BRIGHTNESS AND CONTRAST ------------
      int iBrightness  = iSliderValue1 - 50;
      double dContrast = iSliderValue2 / 50.0;
      gray_src.convertTo(dst, -1, dContrast, iBrightness);
      gray_src.convertTo(gray_src_binary, -1, dContrast, iBrightness);
       //show the brightness and contrast adjusted image
      imshow("Brightness and Contrast", dst);

      // HISTOGRAM ------------
      bool uniform = true; bool accumulate = false;
      vector<Mat> bgr_planes;
      split( dst, bgr_planes );
      Mat b_hist;
      /// Establish the number of bins
      int histSize = 256;
      /// Set the ranges ( for B,G,R) )
      float range[] = { 0, 256 } ;
      const float* histRange = { range };
      /// Compute the histogram:
      calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

      // Draw the histograms for B, G and R
      int hist_w = 512; int hist_h = 400;
      int bin_w = cvRound( (double) hist_w/histSize );

      Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

      /// Normalize the result to [ 0, histImage.rows ]
      normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

      for( int i = 1; i < histSize; i++ ){
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                           Scalar( 255, 0, 0), 2, 8, 0  );
      }
      namedWindow("Histogram", CV_WINDOW_AUTOSIZE );
      imshow("Histogram", histImage );

      //  BINARIZATION
      namedWindow( window_name, CV_WINDOW_AUTOSIZE ); // Create a window to display results
      createTrackbar( trackbar_type,
                      window_name, &threshold_type,
                      max_type, Threshold_Demo ); // Create Trackbar to choose type of Threshold
      createTrackbar( trackbar_value,
                      window_name, &threshold_value,
                      max_value, Threshold_Demo ); // Create Trackbar to choose Threshold value
      Threshold_Demo( 0, 0 ); // Call the function to initialize

      if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC
  }

  return 0;
}
