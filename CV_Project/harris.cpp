#include "stdafx.h"
#include "harris.h"


int callharris(Mat src)
{
	Mat harris_dst, harris_src_gray;
	Mat harris_dst_scaled, harris_dst_norm;

	int blocksize, ksize;
	double k;
	int thresh = 200;

	if (!src.data)
	{
		return -1;
	}

	blocksize = 2;
	k = 0.05;
	ksize = 3;

	/// Convert the image to grayscale
	cvtColor(src, harris_src_gray, CV_BGR2GRAY);

	/// Image to store Harris detector response
	harris_dst.create(harris_src_gray.size(), CV_32FC(6));

	///calling harris detector
	cornerHarris(harris_src_gray, harris_dst, blocksize, ksize, k, BORDER_DEFAULT);

	//changes
	normalize(harris_dst, harris_dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(harris_dst_norm, harris_dst_scaled);

	//harris_dst_norm.copyTo(harris_dst_scaled);
	//drawing circle
	for (int i = 0; i < harris_src_gray.rows; i++)
	{
		for (int j = 0; j < harris_src_gray.cols; j++)
		{
			if (harris_dst_scaled.at<uchar>(i, j) > thresh)			//error due to size mismatch-convertscaleabs changes size to 8bit - resolved
			{
				circle(harris_dst_scaled, Point(j, i), 5, Scalar(0), 2, 8, 0);
			}
		}
	}

	/// Create a windows
	namedWindow("Response", CV_WINDOW_AUTOSIZE);
	imshow("Response", harris_dst_scaled);

	waitKey(0);
	return 0;
}