#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

Mat prewitt(Mat src){
	Mat gray,Kernelx,Kernely;

  Kernelx = (Mat_<double>(3,3) <<1, 1, 1,
                                 0, 0, 0,
                                 -1, -1, -1);
  Kernely = (Mat_<double>(3,3) <<-1, 0, 1,
                                 -1, 0, 1,
                                 -1, 0, 1);

  Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, grad;

  filter2D(src, grad_x, CV_16S , Kernelx, Point(-1,-1));
  filter2D(src, grad_y, CV_16S , Kernely, Point(-1,-1));
  convertScaleAbs( grad_x, abs_grad_x );
  convertScaleAbs( grad_y, abs_grad_y );

  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  return grad;
}
