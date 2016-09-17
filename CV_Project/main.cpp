
#include "stdafx.h"

//#include "canny.h"
//#include "MH.h"
#include "harris.h"


int main()
{
	Mat src = imread("line.jpg");
	//callcanny(src);
	callharris(src);
	//callMH(src);
	return 0;
}
