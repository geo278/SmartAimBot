// SmartAimBot.cpp : Defines the entry point for the application.

//#include "scan.h"
#include "iostream"
#include "Windows.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;


RGBQUAD * capture(POINT a, POINT b) {
	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY); // BitBlt does the copying

	// save bitmap to clipboard
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	CloseClipboard();


	// Array conversion:
	RGBQUAD* pixels = new RGBQUAD[160000];

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = 400;
	bmi.biHeight = -400;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;
	
	GetDIBits(hDC, hBitmap, 0, 400, pixels, (BITMAPINFO*)& bmi, DIB_RGB_COLORS);


	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
	return pixels;
}

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
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
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
	POINT targetPos;
	Mat pic;
	CascadeClassifier upperBody;
	if (upperBody.load("C:\\Users\\georg\\CPSC\\opencv-4.1.0\\data\\haarcascades\\haarcascade_upperbody.xml") && !upperBody.empty()) {
		cout << "Successfully loaded haarcascade_upperbody.xml" << endl;
	}
	else cout << "Error Loading XML file" << endl;

	while (true) {
		if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { // while rmb pressed

			pic = capture(NULL);
			vector<Rect> targets;
			upperBody.detectMultiScale(pic, targets, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(20, 20), Size(200, 200));

			if (targets.size() > 0) {
				targetPos.x = targets[0].x + targets[0].width * 0.5;
				targetPos.y = targets[0].y + targets[0].height * 0.5;
				mouse_event(MOUSEEVENTF_MOVE, targetPos.x - 200, targetPos.y - 200, 0, 0);
			}
		}
		//Sleep(5); // extra buffer time
	}
	//free(pixels);
	return true;
}

int main() {
	Aim();
	return 0;
}
