
#include "stdafx.h"

#include "canny.h"
//#include "MH.h"
#include "hist_chk.h"


int main()
{
	Mat src = imread("blur.jpg");
	callcanny(src);
	//hist_chk(src);

	return 0;
}
