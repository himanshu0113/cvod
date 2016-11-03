/*
*Histogram analysis
*/
#include "stdafx.h"
#include "hist_chk.h"

int hist_chk(Mat src) {

	Mat gray, dst;
	int histsize = 256;
	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	if (!src.data)
	{
		return -1;
	}

	/// Convert the image to grayscale
	cvtColor(src, gray, CV_BGR2GRAY);

	//Calculating Histogram
	calcHist(&gray, 1, 0, Mat(), dst, 1, &histsize, &histRange, true, false);

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histsize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(dst, dst, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histsize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(dst.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(dst.at<float>(i))),
			Scalar(0, 0, 0), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist", CV_WINDOW_AUTOSIZE);
	imshow("calcHist", histImage);

	waitKey(0);

	return 0;
}
