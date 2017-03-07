#include "stdafx.h"

//#include "canny.h"
//#include "MH.h"
//#include "harris.h"
//#include "fourier_transform.h"
//#include "hist_chk.h"
//#include "tomasi.h"
#include "SURF.h";

int main()
{
	Mat src = imread("cube1.jpg");
	Mat src2 = imread("cube2.jpg");
	if (!src.data || !src2.data)
	{
		printf("Source image not found. Please check the source address.");
		_getch();
		//char ch;
		//scanf("%c", &ch);
		return -1;
	}

	//callcanny(src);
	//callharris(src);
	//callMH(src);
	//callFourier(src);
	//hist_chk(src);
	//calltomasi(src);
	callsurf(src, src2);

	/*Mat dst;
	cvtColor(src, src, CV_BGR2GRAY);
	equalizeHist(src, dst);
	namedWindow("img", CV_WINDOW_AUTOSIZE);
	imshow("img", src);*/
	return 0;
}
