#include "stdafx.h"
#include "sift.h"

int callSift(Mat src)
{
	Mat src_gray, dst;

	cvtColor(src, src_gray, CV_BGR2GRAY);

	Feature2D detector;
	std::vector<KeyPoint> keypoints;

	detector.detectAndCompute(src_gray, noArray(), keypoints, dst);

	drawKeypoints(src_gray, keypoints, dst);
	namedWindow("SIFT", CV_WINDOW_AUTOSIZE);
	imshow("SIFT", dst);


	return 0;
}