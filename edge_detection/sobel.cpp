#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat sobel(Mat src){
  Mat src_gray;
  Mat grad;
  const String window_name = "Sobel Demo - Simple Edge Detector";
  int ksize = 1;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  //![reduce_noise]
  // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
  GaussianBlur(src, src_gray, Size(3, 3), 0, 0, BORDER_DEFAULT);
  //![reduce_noise]

  //![sobel]
  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);

  /// Gradient Y
  Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
  //![sobel]

  //![convert]
  // converting back to CV_8U
  convertScaleAbs(grad_x, abs_grad_x);
  convertScaleAbs(grad_y, abs_grad_y);
  //![convert]

  //![blend]
  /// Total Gradient (approximate)
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
  //![blend]

  return grad;
}
