#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
 
using namespace cv;
using namespace std;
 
int main(int argc, char** argv)
{
    /**
    VideoCapture cap(0); //capture the video from web cam 
 
    if (!cap.isOpened())  // if not success, exit program 
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    **/
 
    namedWindow("Control", CV_WINDOW_NORMAL); //create a window called "Control" 
 
    int iLowH = 0;
    int iHighH = 359;
 
    int iLowS = 0;
    int iHighS = 255;
 
    int iLowV = 0;
    int iHighV = 255;
 
    //Create trackbars in "Control" window 
    cvCreateTrackbar("LowH", "Control", &iLowH, 359); //Hue (0 - 359) 
    cvCreateTrackbar("HighH", "Control", &iHighH, 359);
 
    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255) 
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);
 
    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255) 
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    
    Mat imgOriginal;
    imgOriginal=imread("./lena.jpg");

    while (true)
    {
        //Mat imgOriginal;
 
        //bool bSuccess = cap.read(imgOriginal); // read a new frame from video 
        //imgOriginal=imread("./lena.jpg");
 /*
        if (!bSuccess) //if not success, break loop 
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }*/
 
        Mat imgHSV;
        vector<Mat> hsvSplit;
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV 
        
        split(imgHSV, hsvSplit);
        equalizeHist(hsvSplit[2], hsvSplit[2]);
        merge(hsvSplit, imgHSV);
        Mat imgThresholded;
 
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image 
 
        //open (remove noise) 
        Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
        morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
 
        //close (connect connected-component) 
        morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
        imshow("Thresholded Image", imgThresholded); //show the thresholded image 
        imshow("Original", imgOriginal); //show the original image 
 
        char key = (char)waitKey(300);
        if (key == 27)
            break;
    }
 
    return 0;
 
}