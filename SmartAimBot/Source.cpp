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



bool Aim() {
	POINT a, b;
	a.x = 760;
	a.y = 340;
	b.x = 1160;
	b.y = 740;
	RGBQUAD * pixels;

	int red;
	int green;
	int blue;
	POINT targetPos;

	double radius, angle, angleDiffFactor;
	int x, y, index;

	while (true) {
		x = 199;
		y = 199;
		angleDiffFactor = 100;
		angle = 3.1415 / angleDiffFactor;
		radius = 1;
		if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { // while rmb pressed
			pixels = capture(a, b);

			for (int i = 0; i < 160000; i++) {

				x = radius * cos(angle) + 200;
				y = radius * sin(angle) + 200;
				angle += 2 * 3.1415 / angleDiffFactor;
				radius += 1 / angleDiffFactor;

				if (x < 0 || x > 399 || y < 0 || y > 399) {

					cout << "OUT OF BOUNDS  " << x << "  " << y << endl;
					break;
				}
				index = y * 400 + x;

				red = (int)pixels[index].rgbRed;
				green = (int)pixels[index].rgbGreen;
				blue = (int)pixels[index].rgbBlue;


				if (red > 230 && green > 230 && blue > 230) {
					targetPos.x = index % 400;
					targetPos.y = index / 400;
					mouse_event(MOUSEEVENTF_MOVE, targetPos.x - 200, targetPos.y - 200, 0, 0); // x and y are deltas, not abs coordinates
					break;
				}
				//cout << red << ", " << green << ", " << blue << endl;
			}
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
