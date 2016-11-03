
#include "stdafx.h"

//#include "canny.h"
//#include "MH.h"
//#include "harris.h"
#include "fourier_transform.h"


int main()
{
	Mat src = imread("line.jpg");
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
	callFourier(src);

	return 0;
}
