#include "stdafx.h"

//#include <fstream>

//#include "canny.h"
//#include "MH.h"
//#include "harris.h"
//#include "fourier_transform.h"
//#include "hist_chk.h"
//#include "tomasi.h"
//#include "SURF.h"
#include "CapFrames.h"

//#include <objbase.h>  
//#include <msxml6.h> 

int main()
{
	Mat src = imread("images/ct_14.png");
	Mat src2 = imread("images/ct_15.png");
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
	//callsurf(src, src2);
	capture();



	/*Mat dst;
	cvtColor(src, src, CV_BGR2GRAY);
	equalizeHist(src, dst);
	namedWindow("img", CV_WINDOW_AUTOSIZE);
	imshow("img", src);*/
	return 0;
}
