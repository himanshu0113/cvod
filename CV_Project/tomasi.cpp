#include "stdafx.h"
#include "tomasi.h"
//Detecting corners locations usign subpixels


// Global variables
Mat gsrc, gsrc_gray;

int maxCorners = 10;
int maxTrackbar = 25;

RNG rng(12345);

/** @function main */
void calltomasi(Mat src)
{
	/// Load source image and convert it to gray
	gsrc = src;
	cvtColor(src, gsrc_gray, CV_BGR2GRAY);

	/// Create Window
	namedWindow("Image", CV_WINDOW_AUTOSIZE);

	/// Create Trackbar to set the number of corners
	createTrackbar("Max  corners:", "Image", &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo);

	imshow("Image", src);

	goodFeaturesToTrack_Demo(0, 0);

	waitKey(0);
}

/**
* @function goodFeaturesToTrack_Demo.cpp
* @brief Apply Shi-Tomasi corner detector
*/
void goodFeaturesToTrack_Demo(int, void*)
{
	if (maxCorners < 1) { maxCorners = 1; }

	/// Parameters for Shi-Tomasi algorithm
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;

	/// Copy the source image
	Mat copy;
	copy = gsrc.clone();

	/// Apply corner detection
	goodFeaturesToTrack(gsrc_gray, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);


	/// Draw corners detected
	cout << "** Number of corners detected: " << corners.size() << endl;
	int r = 4;
	for (int i = 0; i < corners.size(); i++)
	{
		circle(copy, corners[i], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
			rng.uniform(0, 255)), -1, 8, 0);
	}

	/// Show what you got
	namedWindow("Image", CV_WINDOW_AUTOSIZE);
	imshow("Image", copy);

	/// Set the neeed parameters to find the refined corners
	Size winSize = Size(5, 5);
	Size zeroZone = Size(-1, -1);
	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001);

	/// Calculate the refined corner locations
	cornerSubPix(gsrc_gray, corners, winSize, zeroZone, criteria);

	/// Write them down
	for (int i = 0; i < corners.size(); i++)
	{
		cout << " -- Refined Corner [" << i << "]  (" << corners[i].x << "," << corners[i].y << ")" << endl;
	}
} 