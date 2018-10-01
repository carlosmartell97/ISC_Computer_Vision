#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(){
  VideoCapture cap(0); //capture the video from webcam

  if(!cap.isOpened()){  // if not success, exit program
   printf("webcam is not active! exiting now...\n");
   return -1;
  }

  namedWindow("Control Red", CV_WINDOW_AUTOSIZE); //create a window called "Control Red"
  namedWindow("Control Green", CV_WINDOW_AUTOSIZE); //create a window called "Control Green"
  namedWindow("Control Blue", CV_WINDOW_AUTOSIZE); //create a window called "Control Blue"

  int iLowH_red=170; int iHighH_red=179;
  int iLowS_red=150; int iHighS_red=255;
  int iLowV_red=60; int iHighV_red=255;

  int iLowH_green=38; int iHighH_green=75;
  int iLowS_green=150; int iHighS_green=255;
  int iLowV_green=60; int iHighV_green=255;

  int iLowH_blue=80; int iHighH_blue=130;
  int iLowS_blue=150; int iHighS_blue=255;
  int iLowV_blue=60; int iHighV_blue=255;

  //Create trackbars in "Control Red" window
  createTrackbar("LowH", "Control Red", &iLowH_red, 179); //Hue (0 - 179)
  createTrackbar("HighH", "Control Red", &iHighH_red, 179);
  createTrackbar("LowS", "Control Red", &iLowS_red, 255); //Saturation (0 - 255)
  createTrackbar("HighS", "Control Red", &iHighS_red, 255);
  createTrackbar("LowV", "Control Red", &iLowV_red, 255);//Value (0 - 255)
  createTrackbar("HighV", "Control Red", &iHighV_red, 255);

  //Create trackbars in "Control Green" window
  createTrackbar("LowH", "Control Green", &iLowH_green, 179); //Hue (0 - 179)
  createTrackbar("HighH", "Control Green", &iHighH_green, 179);
  createTrackbar("LowS", "Control Green", &iLowS_green, 255); //Saturation (0 - 255)
  createTrackbar("HighS", "Control Green", &iHighS_green, 255);
  createTrackbar("LowV", "Control Green", &iLowV_green, 255);//Value (0 - 255)
  createTrackbar("HighV", "Control Green", &iHighV_green, 255);

  //Create trackbars in "Control Green" window
  createTrackbar("LowH", "Control Blue", &iLowH_blue, 179); //Hue (0 - 179)
  createTrackbar("HighH", "Control Blue", &iHighH_blue, 179);
  createTrackbar("LowS", "Control Blue", &iLowS_blue, 255); //Saturation (0 - 255)
  createTrackbar("HighS", "Control Blue", &iHighS_blue, 255);
  createTrackbar("LowV", "Control Blue", &iLowV_blue, 255);//Value (0 - 255)
  createTrackbar("HighV", "Control Blue", &iHighV_blue, 255);

  int iLastX_red = -1; int iLastY_red = -1;
  int iLastX_green = -1; int iLastY_green = -1;
  int iLastX_blue = -1; int iLastY_blue = -1;

  //Capture a temporary image from the camera
  Mat imgTmp;
  cap.read(imgTmp);

  //Create a black image with the size as the camera output
  Mat imgLines_red = Mat::zeros( imgTmp.size(), CV_8UC3 );
  Mat imgLines_green = Mat::zeros( imgTmp.size(), CV_8UC3 );
  Mat imgLines_blue = Mat::zeros( imgTmp.size(), CV_8UC3 );

  while(true){
    Mat imgOriginal;

    bool bSuccess = cap.read(imgOriginal); // read a new frame from video

    if(!bSuccess){ //if not success, break loop
      printf("Cannot read a frame from video stream, exiting now...\n");
      break;
    }

    Mat imgHSV;

    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    Mat imgThresholded_red;
    Mat imgThresholded_green;
    Mat imgThresholded_blue;

    inRange(imgHSV, Scalar(iLowH_red, iLowS_red, iLowV_red), Scalar(iHighH_red, iHighS_red, iHighV_red), imgThresholded_red); //Threshold the image for red
    inRange(imgHSV, Scalar(iLowH_green, iLowS_green, iLowV_green), Scalar(iHighH_green, iHighS_green, iHighV_green), imgThresholded_green); //Threshold the image for green
    inRange(imgHSV, Scalar(iLowH_blue, iLowS_blue, iLowV_blue), Scalar(iHighH_blue, iHighS_blue, iHighV_blue), imgThresholded_blue); //Threshold the image for blue

    //morphological opening (removes small objects from the foreground)
    erode(imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded_green, imgThresholded_green, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded_green, imgThresholded_green, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (removes small holes from the foreground)
    dilate( imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded_green, imgThresholded_green, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded_green, imgThresholded_green, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //Calculate the moments of the thresholded images
    Moments oMoments_red = moments(imgThresholded_red);
    Moments oMoments_green = moments(imgThresholded_green);
    Moments oMoments_blue = moments(imgThresholded_blue);

    double dM01_red=oMoments_red.m01; double dM10_red=oMoments_red.m10; double dArea_red=oMoments_red.m00;
    double dM01_green=oMoments_green.m01; double dM10_green=oMoments_green.m10; double dArea_green=oMoments_green.m00;
    double dM01_blue=oMoments_blue.m01; double dM10_blue=oMoments_blue.m10; double dArea_blue=oMoments_blue.m00;

    // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
    if(dArea_red > 10000){
      //calculate the position of the ball
      int posX_red = dM10_red / dArea_red;
      int posY_red = dM01_red / dArea_red;

      if(iLastX_red>=0 && iLastY_red>=0 && posX_red>=0 && posY_red>=0){
        //Draw a red line from the previous point to the current point
        line(imgLines_red, Point(posX_red, posY_red), Point(iLastX_red, iLastY_red), Scalar(0,0,255), 2);
      }

      iLastX_red = posX_red;
      iLastY_red = posY_red;
    }
    if(dArea_green > 10000){
      //calculate the position of the ball
      int posX_green = dM10_green / dArea_green;
      int posY_green = dM01_green / dArea_green;

      if(iLastX_green>=0 && iLastY_green>=0 && posX_green>=0 && posY_green>=0){
        //Draw a red line from the previous point to the current point
        line(imgLines_green, Point(posX_green, posY_green), Point(iLastX_green, iLastY_green), Scalar(0,255,0), 2);
      }

      iLastX_green = posX_green;
      iLastY_green = posY_green;
    }
    if(dArea_blue > 10000){
      //calculate the position of the ball
      int posX_blue = dM10_blue / dArea_blue;
      int posY_blue = dM01_blue / dArea_blue;

      if(iLastX_blue>=0 && iLastY_blue>=0 && posX_blue>=0 && posY_blue>=0){
        //Draw a red line from the previous point to the current point
        line(imgLines_blue, Point(posX_blue, posY_blue), Point(iLastX_blue, iLastY_blue), Scalar(255,0,0), 2);
      }

      iLastX_blue = posX_blue;
      iLastY_blue = posY_blue;
    }

    imshow("Thresholded Image Red", imgThresholded_red); //show the thresholded image for red
    imshow("Thresholded Image Green", imgThresholded_green); //show the thresholded image for green
    imshow("Thresholded Image Blue", imgThresholded_blue); //show the thresholded image for blue

    imgOriginal = imgOriginal + imgLines_red + imgLines_green + imgLines_blue;
    imshow("Original", imgOriginal); //show the original image

    if(waitKey(30) == 27){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
  return 0;
}
