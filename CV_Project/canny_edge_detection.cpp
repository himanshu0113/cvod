/* Canny edge detection */
#include "stdafx.h"
/*
#include <stdio.h>
#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

// Scale floating point magnitudes and angles to 8 bits
#define ORI_SCALE 40.0
#define MAG_SCALE 20.0
#define PI 3.1415926535

// Biggest possible filter mask 
#define MAX_MASK_SIZE 20

// Fraction of pixels that should be above the HIGH threshold 
float ratio = 0.1f;
int WIDTH = 0;

int range(Mat x, int i, int j) {
	if ((i >= 0) && (i < x.rows) && (j >= 0) && (j < x.cols))
		return 1;
	else
		return  0;
}

float norm(float x, float y) {
	return (float)sqrt((double)(x*x + y*y));
}

float** f2d(int n, int m) {
	float **temp = new float*[n];
	for (int i = 0; i < n; i++)
		temp[i] = new float[m];
	return temp;
}

// Gaussian
float gauss(float x, float sigma) {
	float xx;

	if (sigma == 0) return 0.0;
	xx = (float)exp((double)((-x*x) / (2 * sigma*sigma)));
	return xx;
}

float meanGauss(float x, float sigma) {
	float z;

	z = (gauss(x, sigma) + gauss(x + 0.5f, sigma) + gauss(x - 0.5f, sigma)) / 3.0f;
	z = z / (2 * PI*sigma*sigma);
	return z;
}

// First derivative of Gaussian 
float dgauss(float x, float sigma) {
	return -x / (sigma*sigma)*gauss(x, sigma);
}


void estimate_thresh(Mat mag, int *hi, int *low) {
	int i, j, k, hist[256], count;

	// Build a histogram of the magnitude image.
	for (k = 0; k < 256; hist[k] = 0, k++);

	for (i = WIDTH; i < mag.rows - WIDTH; i++)
		for (j = WIDTH; j < mag.cols - WIDTH; j++)
			hist[mag.at<uchar>(i, j)]++;

	// The high threshold should be > 80 or 90% of the pixels
	//j = (int)(ratio*mag->info->nr*mag->info->nc);
	

	j = mag.rows;
	if (j < mag.cols)
		j = mag.cols;
	j = (int)(0.9*j);
	k = 255;

	count = hist[255];
	while (count < j) {
		k--;
		if (k < 0) break;
		count += hist[k];
	}
	*hi = k;

	i = 0;
	while (hist[i] == 0) i++;

	*low = (*hi + 1) / 2.0f;
}

// TRACE - recursively trace edge pixels that have a
//threshold > the low edge threshold, continuing
//from the pixel at (i,j).

int trace(int i, int j, int low, Mat im, Mat mag, Mat ori) {
	int n, m;
	char flag = 0;

	if (im.at<uchar>(i, j) == 0) {
		im.at<uchar>(i, j) = 225;
		flag = 0;
		for (n = -1; n <= 1; n++) {
			for (m = -1; m <= 1; m++) {
				if (i == 0 && m == 0) continue;
				if (range(mag, i + n, j + m) && mag.at<uchar>(i + n, j + m) >= low)
					if (trace(i + n, j + m, low, im, mag, ori)) {
						flag = 1;
						break;
					}
			}
			if (flag)
				break;
		}
		return 1;
	}
	return 0;
}


// HYSTERESIS thersholding of edge pixels. Starting at pixels with a
//value greater than the HIGH threshold, trace a connected sequence
//of pixels that have a value greater than the LOW threhsold. 

void hysteresis(int high, int low, Mat im, Mat mag, Mat oriim) {
	int i, j;

	printf("Beginning hysteresis thresholding...\n");
	for (i = 0; i < im.rows; i++)
		for (j = 0; j < im.cols; j++)
			im.at<uchar>(i, j) = 0;

	if (high < low) {
		estimate_thresh(mag, &high, &low);
		printf("Hysteresis thresholds(from image) : HI %d LOW %D\n", high, low);
	}

	// For each edge with a magnitude above the high threshold, begin
	//tracing edge pixels that are above the low threshold.
	for (i = 0; i < im.rows; i++)
		for (j = 0; j < im.cols; j++)
			if (mag.at<uchar>(i, j) >= high)
				trace(i, j, low, im, mag, oriim);

	// Make the edge black (to be the same as the other methods)
	for (i = 0; i < im.rows; i++)
		for (j = 0; j < im.cols; j++)
			if (im.at<uchar>(i, j) == 0)
				im.at<uchar>(i, j) = 255;
			else
				im.at<uchar>(i, j) = 0;
}


void seperable_convolution(Mat im, float *gau, int width, float **smx, float **smy) {
	int i, j, k, I1, I2, nr, nc;
	float x, y;

	nr = im.rows;
	nc = im.cols;

	for (i = 0; i < nr; i++)
		for (j = 0; j < nc; j++) {
			x = gau[0] * im.at<uchar>(i, j);
			y = gau[0] * im.at<uchar>(i, j);
			for (k = 1; k < width; k++) {
				I1 = (i + k) % nr;
				I2 = (i - k + nr) % nr;
				y += gau[k] * im.at<uchar>(I1, j) + gau[k] * im.at<uchar>(I2, j);
				I1 = (j + k) % nc;
				I2 = (j - k + nc) % nc;
				x += gau[k] * im.at<uchar>(i, I1) + gau[k] * im.at<uchar>(i, I2);
			}
			smx[i][j] = x;
			smy[i][j] = y;
		}
}

void dxy_seperable_convolution(float **im, int nr, int nc, float *gau, int width, float **sm, int which) {
	int i, j, k, I1, I2;
	float x;

	for (i = 0; i < nr; i++)
		for (j = 0; j < nc; j++) {
			x = 0.0;
			for (k = 1; k < width; k++) {
				if (which == 0) {
					I1 = (i + k) % nr;
					I2 = (i - k + nr) % nr;
					x += -gau[k] * im[I1][j] + gau[k] * im[I2][j];
				}
				else {
					I1 = (j + k) % nc;
					I2 = (j - k + nc) % nc;
					x += -gau[k] * im[i][I1] + gau[k] * im[i][I2];
				}
			}
			sm[i][j] = x;
		}
}

void nonmax_supress(float **dx, float **dy, int nr, int nc, Mat mag, Mat ori) {
	int i, j;
	float xx, yy, g, g1, g2, g3, g4, xc, yc;

	for (i = 1; i < mag.rows-1; i++) {
		for (j = 1; j < mag.cols-1; j++) {
			mag.at<uchar>(i, j) = 0;

			// Treat the x and y derivatives as components of a vector
			xc = dx[i][j];
			yc = dy[i][j];
			if (fabs(xc) < 0.01 && fabs(yc) < 0.01) continue;

			g = norm(xc, yc);

			// Follow the gradient direction, as indicated by the direction of
			//the vector (xc, yc); retain pixels that are a local maximum.

			if (fabs(yc) > fabs(xc)) {
				// The Y component is biggest, so gradient direction is basically UP/DOWN
				xx = fabs(xc) / fabs(yc);
				yy = 1.0;

				g2 = norm(dx[i - 1][j], dy[i - 1][j]);
				g4 = norm(dx[i + 1][j], dy[i + 1][j]);
				if (xc*yc > 0.0) {
					g3 = norm(dx[i + 1][j + 1], dy[i + 1][j + 1]);
					g1 = norm(dx[i - 1][j - 1], dy[i - 1][j - 1]);
				}
				else {
					g3 = norm(dx[i + 1][j - 1], dy[i + 1][j - 1]);
					g1 = norm(dx[i - 1][j + 1], dy[i - 1][j + 1]);
				}
			}
			else {
				// The X component is biggest, so gradient direction is basically LEFT/RIGHT
				xx = fabs(yc) / fabs(xc);
				yy = 1.0;

				g2 = norm(dx[i][j + 1], dy[i][j + 1]);
				g4 = norm(dx[i][j - 1], dy[i][j - 1]);
				if (xc*yc > 0.0) {
					g3 = norm(dx[i - 1][j - 1], dy[i - 1][j - 1]);
					g1 = norm(dx[i + 1][j + 1], dy[i + 1][j + 1]);
				}
				else {
					g1 = norm(dx[i - 1][j + 1], dy[i - 1][j + 1]);
					g3 = norm(dx[i + 1][j - 1], dy[i + 1][j - 1]);
				}
			}

			// Compute the interpolated value of the gradient magnitude
			if ((g > (xx*g1 + (yy - xx)*g2)) && (g > (xx*g3 + (yy - xx)*g4))) {
				if (g*MAG_SCALE <= 255)
					mag.at<uchar>(i, j) = (uchar)(g*MAG_SCALE);
				else
					mag.at<uchar>(i, j) = 255;
				ori.at<uchar>(i, j) = (uchar)(atan2(yc, xc)*ORI_SCALE);
			}
			else {
				mag.at<uchar>(i, j) = 0;
				mag.at<uchar>(i, j) = 0;
			}
		}
	}
}


void canny(float s, Mat im, Mat mag, Mat ori) {
	printf("INSIDE CANNY()");
	int width = 20;		//initialize
	float **smx, **smy;
	float **dx, **dy;
	int i, j, n;
	float gau[MAX_MASK_SIZE], dgau[MAX_MASK_SIZE], z;

	// Create a Gaussian and a derivative of Gaussian filter mask
	for (i = 0; i < MAX_MASK_SIZE; i++) {
		gau[i] = meanGauss((float)i, s);
		if (gau[i] < 0.005) {
			width = i;
			break;
		}
		dgau[i] = dgauss((float)i, s);
	}

	n = width + width + 1;
	WIDTH = width / 2;
	printf("Smoothing with a guassian (width = %d) ...\n", n);

	smx = f2d(im.rows, im.cols);
	smy = f2d(im.rows, im.cols);

	// Convolution of source image with a Gaussian in X and Y directions
	seperable_convolution(im, gau, width, smx, smy);

	// Now convolve smoothed data with a derivative
	printf("Convolution with the derivative of a Gaussian...\n");
	dx = f2d(im.rows, im.cols);
	dxy_seperable_convolution(smx, im.rows, im.cols, dgau, width, dx, 1);
	free(smx[0]);
	free(smx);

	dy = f2d(im.rows, im.cols);
	dxy_seperable_convolution(smy, im.rows, im.cols, dgau, width, dy, 0);
	free(smy[0]);
	free(smy);

	// Create an image of the norm of dx,dy 
	for (i = 0; i < im.rows; i++)
		for (j = 0; j < im.cols; j++) {
			z = norm(dx[i][j], dy[i][j]);
			mag.at<uchar>(i, j) = (unsigned char)(z*MAG_SCALE);
		}

	// Non-maximum suppression - edge pixels should be a local max 
	nonmax_supress(dx, dy, (int)im.rows, (int)im.cols, mag, ori);

	free(dx[0]); free(dx);
	free(dy[0]); free(dy);
}


int call_canny_new() {
	int i, j;
	float s = 1.0;
	int low = 0, high = -1;
	Mat im, magim, oriim;

	im = imread("cube1.jpg");
	if (im.empty()) {
		printf("Error: Image can not be loaded!...");
		system("pause");
		return -1;
	}

	//namedWindow("Original", CV_WINDOW_NORMAL);
	imshow("Original", im);

	//params
	
	//printf("Enter following parameters: \n");
	//printf("Lower Threshold: ");
	//scanf_s("%d", &low);
	//printf("Higher Threshhold: ");
	//scanf_s("%d", &high);
	//printf("Gaussian Standard Deviation: ");
	//scanf_s("%f", &s);
	
	
	s = 1.0;
	low = 10;
	high = 100;
	
	//magim = Mat(im.rows, im.cols, CV_8UC3);
	//oriim = Mat(im.rows, im.cols, CV_8UC3);

	magim.create(im.size(), im.type());
	oriim.create(im.size(), im.type());

	if (magim.empty()) {
		printf("Out of storage:Magnitude...");
		system("pause");
		return -1;
	}

	if (oriim.empty()) {
		printf("Out of storage:Orientation...");
		system("pause");
		return -1;
	}

	//cvt to grayscale
	cvtColor(im, im, CV_BGR2GRAY);

	// Apply the filter 
	canny(s, im, magim, oriim);

	// Hysteresis thresholding of edge pixels 
	hysteresis(high, low, im, magim, oriim);

	for (i = 0; i < WIDTH; i++)
		for (j = 0; j < im.cols; j++)
			im.at<uchar>(i, j) = 255;

	for (i = im.rows - 1; i > im.rows - 1 - WIDTH; i--)
		for (j = 0; j < im.cols; j++)
			im.at<uchar>(i, j) = 255;

	for (i = 0; i < im.rows; i++)
		for (j = 0; j < WIDTH; j++)
			im.at<uchar>(i, j) = 255;

	for (i = 0; i < im.rows; i++)
		for (j = im.cols - WIDTH - 1; j < im.cols; j++)
			im.at<uchar>(i, j) = 255;

	//namedWindow("Detected", CV_WINDOW_NORMAL);
	imshow("Detected", im);
	waitKey(0);

	return 0;
}
*/