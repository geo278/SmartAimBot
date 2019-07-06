// SmartAimBot.cpp : Defines the entry point for the application.

//#include "scan.h"
#include "iostream"
#include "Windows.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;


Mat * capture(POINT a, POINT b) {
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


	Mat pixels;
	pixels.create(400, 400, CV_8UC4);


	BYTE* bmpData = new BYTE[3 * 400 * 400];

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);	
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = 400;
	bmi.biHeight = -400;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;
	GetDIBits(hDC, hBitmap, 0, 400, bmpData, (BITMAPINFO*)& bmi, DIB_RGB_COLORS);
	memcpy(pixels.data, bmpData, 3 * pixels.cols * pixels.rows);
	delete[] bmpData;
	//ConvertHBitmapToMat(hBitmap, pixels);


	/*
	// Array conversion:

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = 400;
	bmi.biHeight = -400;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;
	
	GetDIBits(hDC, hBitmap, 0, 400, pixels, (BITMAPINFO*)& bmi, DIB_RGB_COLORS);
	*/

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
	return &pixels;
}



bool Aim() {
	POINT a, b;
	a.x = 760;
	a.y = 340;
	b.x = 1160;
	b.y = 740;
	Mat * pixels;

	//int red;
	//int green;
	//int blue;
	POINT targetPos;

	CascadeClassifier upperBody;
	if (upperBody.load("C:\\Users\\georg\\CPSC\\opencv-4.1.0\\data\\haarcascades\\haarcascade_upperbody.xml") && !upperBody.empty()) {
		cout << "successfully loaded haarcascade_upperbody.xml" << endl;
	} else cout << "Error Loading XML file" << endl;

	while (true) {
		if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { // while rmb pressed
			pixels = capture(a, b);


			/*
			for (int i = 0; i < 400; i++) {
				red = (int)pixels[i].rgbRed;
				green = (int)pixels[i].rgbGreen;
				blue = (int)pixels[i].rgbBlue;
			}

			red = (int)pixels[400-1].rgbRed;
			green = (int)pixels[400 - 1].rgbGreen;
			blue = (int)pixels[400 - 1].rgbBlue;
			cout << red << ", " << green << ", " << blue << endl;
			*/




			vector<Rect> targets;
			upperBody.detectMultiScale(*pixels, targets, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(10, 10));

			if (targets.size() > 0) {
				targetPos.x = targets[0].x + targets[0].width * 0.5;
				targetPos.y = targets[0].y + targets[0].height * 0.5;
				mouse_event(MOUSEEVENTF_MOVE, targetPos.x - 200, targetPos.y - 200, 0, 0);
			}

			break;
			//mouse_event(MOUSEEVENTF_MOVE, -10, 1, 0, 0); // x and y are deltas, not abs coordinates
		}
		Sleep(5); // extra buffer time
	}
	free(pixels);
	return true;
}

int main() {
	Aim();
	return 0;
}
