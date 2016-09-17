#include "stdafx.h"
#include "harris.h"

Mat harris_dst, harris_src_gray;
Mat harris_dst_scaled, harris_dst_norm;

int blocksize, ksize;
double k;
double minval, maxval;
int thresh = 200;

char* window = "Response";
//char* window_name2 = "Corners";
int qualityLevel = 50;
int max_qualityLevel = 100;

/*void harris_qual(int, void*)
{
	if (qualityLevel < 1) { qualityLevel = 1; }

	for (int i = 0; i < harris_src_gray.rows; i++)
	{
		for (int j = 0; j < harris_src_gray.cols; j++)
		{
			if (harris_dst.at<float>(i, j)>0.5*maxval)
			{
				circle(harris_copy, Point(i, j), 2, Scalar(0,0,255), -1, 8, 0);
			}
		}
	}

	imshow(window, harris_copy);
}*/

int callharris(Mat src)
{
	if (!src.data)
	{
		return -1;
	}

	blocksize = 5;
	k = 0.04;
	ksize = 7;

	/// Convert the image to grayscale
	cvtColor(src, harris_src_gray, CV_BGR2GRAY);

	/// Image to store Harris detector response
	harris_dst.create(harris_src_gray.size(), CV_32FC(6));

	///calling harris detector
	cornerHarris(harris_src_gray, harris_dst, blocksize, ksize, k, BORDER_DEFAULT);

	//minMaxLoc(harris_dst, &minval, &maxval, 0, 0, Mat());

	//changes
	normalize(harris_dst, harris_dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(harris_dst_norm, harris_dst_scaled);

	//drawing circle
	for (int i = 0; i < harris_src_gray.rows; i++)
	{
		for (int j = 0; j < harris_src_gray.cols; j++)
		{
			if (harris_dst_scaled.at<float>(i, j) > thresh)
			{
				circle(harris_dst_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}

	/// Create a windows
	namedWindow(window, CV_WINDOW_AUTOSIZE);
	//createTrackbar(" Quality Level:", window, &qualityLevel, max_qualityLevel, harris_qual);
	imshow(window, harris_dst_scaled);
	//harris_copy = src.clone();
	//harris_qual(0, 0);

	waitKey(0);
	return 0;
}