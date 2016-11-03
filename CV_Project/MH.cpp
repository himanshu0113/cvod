/* Marr/Hildreth edge detection */

#include "stdafx.h"

#include "MH.h"

float norm(float x, float y) {
	return (float)sqrt((double)(x*x + y*y));
}

float distance(float a, float b, float c, float d) {
	return norm((a - c), (b - d));
}

float** f2d(int n, int m) {
	float **temp = new float*[n];
	for (int i = 0; i < n; i++)
		temp[i] = new float[m];
	return temp;
}

	// Gaussian
float gauss(float x, float sigma) {
	return (float)exp((double)((-x*x) / (2 * sigma*sigma)));
}

float LoG(float x, float sigma) {
	float x1;
	x1 = gauss(x, sigma);
	return (x*x - 2 * sigma*sigma) / (sigma*sigma*sigma*sigma) * x1;
}

void convolution(Mat im, float **mask, int nr, int nc, float **res, int NR, int NC) {
	int i, j, ii, jj, n, m, k, kk;
	float x, y;

	k = nr / 2;
	kk = nc / 2;

	for (i = 0; i < NR; i++)
		for (j = 0; j < NC; j++) {
			x = 0;
			for (ii = 0; ii<nr; ii++)
			{
				n = i - k + ii;
				if (n<0 || n >= NR) continue;
				for (jj = 0; jj<nc; jj++)
				{
					m = j - kk + jj;
					if (m<0 || m >= NC) continue;

					x += mask[ii][jj] * (im.at<uchar>(n, m));
				}
			}
			res[i][j] = x;
		}
}

void zero_cross(float **lapim, Mat im) {
	int i, j, k, n, m, dx, dy;
	float x, y, z;
	int xi, xj, yi, yj, count = 0;
	Mat deriv;

	for (i = 1; i < im.rows - 1; i++)
		for (j = 0; j < im.cols - 1; j++) {
			im.at<uchar>(i, j) = 0;
			if (lapim[i - 1][j] * lapim[i + 1][j]<0){ im.at<uchar>(i, j) = 255; continue; }
			if (lapim[i][j - 1] * lapim[i][j + 1]<0){ im.at<uchar>(i, j) = 255; continue; }
			if (lapim[i + 1][j - 1] * lapim[i - 1][j + 1]<0){ im.at<uchar>(i, j) = 255; continue; }
			if (lapim[i - 1][j - 1] * lapim[i + 1][j + 1]<0){ im.at<uchar>(i, j) = 255; continue; }
		}
}

void marr(double s, Mat im) {
	int width;
	float **smx;
	int i, j, k, n;
	float **lgau, z;

	// Create a Gaussian and a derivative of Gaussian filter mask
	width = 3.35*s + 0.33;
	n = width + width + 1;
	printf("Smoothing with a Gaussian of size %dx%d\n", n, n);
	lgau = f2d(n, n);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			lgau[i][j] = LoG(distance((float)i, (float)j, (float)width, (float)width), s);
		}

	// Convolution of source image with a Gaussian in X and Y directions
	smx = f2d(im.rows, im.cols);
	printf("Convolution with LoG : \n");
	convolution(im, lgau, n, n, smx, im.rows, im.cols);

	// Locate the zero crossings
	printf("Zero crossings:\n");
	zero_cross(smx, im);

	printf("Zero crossings returned:\n");
	 // Clear the boundary
	for (i = 0; i<im.rows; i++)
	{
		for (j = 0; j <= width; j++) im.at<uchar>(i, j) = 0;
		for (j = im.cols - width - 1; j<im.cols; j++)
			im.at<uchar>(i, j) = 0;
	}

	for (j = 0; j<im.cols; j++) {
		for (i = 0; i <= width; i++) im.at<uchar>(i, j) = 0;
		for (i = im.rows - width - 1; i < im.rows; i++)
			im.at<uchar>(i, j) = 0;
	}
	

	free(smx[0]); free(smx);
	free(lgau[0]); free(lgau);
}

std::string getImageType(int number)
{
	// find type
	int imgTypeInt = number % 8;
	std::string imgTypeString;

	switch (imgTypeInt)
	{
	case 0:
		imgTypeString = "8U";
		break;
	case 1:
		imgTypeString = "8S";
		break;
	case 2:
		imgTypeString = "16U";
		break;
	case 3:
		imgTypeString = "16S";
		break;
	case 4:
		imgTypeString = "32S";
		break;
	case 5:
		imgTypeString = "32F";
		break;
	case 6:
		imgTypeString = "64F";
		break;
	default:
		break;
	}

	// find channel
	int channel = (number / 8) + 1;

	std::stringstream type;
	type << "CV_" << imgTypeString << "C" << channel;

	return type.str();
}

int callMH(Mat src)
{
	int i, j;
	double s = 1.0;
	Mat scr_gray, img2;

	//img1 = imread("img3.jpg");
	//imshow("Original", img1);
	cvtColor(src, scr_gray, CV_BGR2GRAY);

	s = 1.2;

	string str = getImageType(src.type());
	printf("%s", str.c_str());

	scr_gray.copyTo(img2);

	marr(s - 0.8, scr_gray);
	marr(s + 0.8, img2);

	for (i = 0; i < src.rows; i++)
		for (j = 0; j < src.cols; j++) {
			if (scr_gray.at<uchar>(i, j)>0 && img2.at<uchar>(i, j)>0)
				scr_gray.at<uchar>(i, j) = 0;
			else
				scr_gray.at<uchar>(i, j) = 255;
		}


	imshow("Filtered", scr_gray);
	waitKey(0);
	return 0;
}
