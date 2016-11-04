
#include "stdafx.h"

//#include "canny.h"
//#include "MH.h"
//#include "harris.h"
//#include "fourier_transform.h"
//#include "hist_chk.h"
#include "sift.h"

int main()
{
	Mat src = imread("shapes.jpg");
	if (!src.data)
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
	callSift(src);

	/*Mat dst;
	cvtColor(src, src, CV_BGR2GRAY);
	equalizeHist(src, dst);
	namedWindow("img", CV_WINDOW_AUTOSIZE);
	imshow("img", src);*/
	return 0;
}
