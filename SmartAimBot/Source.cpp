// SmartAimBot.cpp : Defines the entry point for the application.

//#include "scan.h"
#include "iostream"
#include "Windows.h"

using namespace std;

//bool paused = false;

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



	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = 400;
	bmi.biHeight = -400;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;


	static RGBQUAD * pixels = new RGBQUAD[400 * 400];
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
	while (true) { // while rmb pressed
		if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
			pixels = capture(a, b);

			for (int i = 0; i < 400; i++) {
				red = (int)pixels[i].rgbRed;
				green = (int)pixels[i].rgbGreen;
				blue = (int)pixels[i].rgbBlue;
			}

			red = (int)pixels[400-1].rgbRed;
			green = (int)pixels[400 - 1].rgbGreen;
			blue = (int)pixels[400 - 1].rgbBlue;
			cout << red << ", " << green << ", " << blue << endl;


			if ((int)pixels[0].rgbRed == 255) {
				mouse_event(MOUSEEVENTF_MOVE, -10, 1, 0, 0);
			}

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
