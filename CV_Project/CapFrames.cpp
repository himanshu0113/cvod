#include "stdafx.h";
#include "CapFrames.h"

int capture()
{
	/**VideoCapture cap(0);
	while (!cap.isOpened())
	{
		//cap.open("images/test_object.mp4");
		waitKey(1000);
		print("Wait for header");
	}

	for (;;)
	{
		Mat frame;
		cap.read(frame);
		imwrite("images/img_0.jpg", frame);
		if(waitKey(1) >= 0) break;
	}
	return 0;
	**/

	VideoCapture cap("images/test_object.mp4"); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}