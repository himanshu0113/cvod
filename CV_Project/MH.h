#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "opencv/cv.h"

using namespace cv;
using namespace std;

float norm(float x, float y);
float distance(float a, float b, float c, float d);
float** f2d(int n, int m);
float gauss(float x, float sigma);
float LoG(float x, float sigma);
void convolution(Mat im, float **mask, int nr, int nc, float **res, int NR, int NC);
void zero_cross(float **lapim, Mat im);
void marr(double s, Mat im);
std::string getImageType(int number);
int MH();
