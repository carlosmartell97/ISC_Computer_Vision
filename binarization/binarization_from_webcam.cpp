#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;
Mat src, src_gray_binary, dst_binary;
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
    threshold( src_gray_binary, dst_binary, threshold_value, max_binary_value, threshold_type );
    imshow( window_name, dst_binary );
}

int main( int argc, char** argv ){
  VideoCapture cap;
  if(!cap.open(0)){
    printf("webcam is not active! exiting now...\n");
    return 0;
  }
  printf("Starting. Press ESC key to quit.\n");
  for(;;){
    Mat src, gray_src, dst;

    /// Load image
    cap >> src;

    if( src.empty() ) break; // end of video stream
    if( !src.data )
      { return -1; }

    cvtColor(src, gray_src, CV_BGR2GRAY);
    imshow("Source", gray_src);

    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( gray_src, bgr_planes );
    // split( src, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    // calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    // calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    // normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    // normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        // line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
        //                  Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
        //                  Scalar( 0, 255, 0), 2, 8, 0  );
        // line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
        //                  Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
        //                  Scalar( 0, 0, 255), 2, 8, 0  );
    }

    /// Display
    namedWindow("Histogram", CV_WINDOW_AUTOSIZE );
    imshow("Histogram", histImage );

    // Binarization
    cvtColor( src, src_gray_binary, COLOR_BGR2GRAY ); // Convert the image to Gray
    namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
    createTrackbar( trackbar_type,
                    window_name, &threshold_type,
                    max_type, Threshold_Demo ); // Create Trackbar to choose type of Threshold
    createTrackbar( trackbar_value,
                    window_name, &threshold_value,
                    max_value, Threshold_Demo ); // Create Trackbar to choose Threshold value
    Threshold_Demo( 0, 0 ); // Call the function to initialize

    // waitKey(0);
    if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC
  }

  return 0;
}
