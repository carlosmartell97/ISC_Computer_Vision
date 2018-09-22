#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;

Mat src_canny, src_gray;
Mat dst_canny, detected_edges;
int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name_canny = "Canny edges";

static void CannyThreshold(int, void*){
  blur( src_canny, detected_edges, Size(3,3) );
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
  dst_canny = Scalar::all(0);
  src_canny.copyTo( dst_canny, detected_edges);
  imshow( window_name_canny, dst_canny );
}

void canny(Mat src){
  src_canny = src;
  dst_canny.create( src_canny.size(), src_canny.type() );
  namedWindow( window_name_canny, WINDOW_AUTOSIZE );
  createTrackbar( "Min Threshold:", window_name_canny, &lowThreshold, max_lowThreshold, CannyThreshold );
  CannyThreshold(0, 0);
}
