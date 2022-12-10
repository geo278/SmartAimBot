// SmartAimBot.cpp : Defines the entry point for the application.

//#include "scan.h"
#include "iostream"
#include "Windows.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <thread>
#include <vector>

using namespace std;
using namespace cv;

Mat capture(HWND hwnd) { // screen capture code adapted from https://stackoverflow.com/questions/34466993/opencv-desktop-capture
	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	//RECT windowsize;    // get the height and width of the screen
	//GetClientRect(hwnd, &windowsize);

	srcheight = 400;
	srcwidth = 400;
	height = 400 / 1;  //change this to whatever size you want to resize to
	width = 400 / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    // http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 760, 340, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)& bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow


	// avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	return src;
}

bool Aim() {
	vector<Rect> targets;
	POINT targetPos;
	Mat pic;
	CascadeClassifier target;
	if (target.load("C:\\Users\\georg\\CPSC\\SmartAimBot\\SmartAimBot\\haarcascade_upperbody.xml") && !target.empty()) {
		cout << "Successfully loaded XML file" << endl;
	}
	else cout << "Error Loading XML file" << endl;

	while (true) {
		if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { // while rmb pressed
			pic = capture(NULL);
			target.detectMultiScale(pic, targets, 1.05, 1, 0, Size(5, 5), Size(150, 150));
			if (targets.size() > 0) {
				targetPos.x = targets[0].x + targets[0].width * 0.5;
				targetPos.y = targets[0].y + targets[0].height * 0.5;
				mouse_event(MOUSEEVENTF_MOVE, targetPos.x - 200, targetPos.y - 200, 0, 0);
			}
		}
		targets.shrink_to_fit();
		Sleep(1); // reduce resource consumption
	}
	return true;
}

int main() {
	Aim();
	return 0;
}
