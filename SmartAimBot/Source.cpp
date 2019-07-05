// SmartAimBot.cpp : Defines the entry point for the application.

//#include "scan.h"
#include "iostream"
#include "Windows.h"

using namespace std;

//bool paused = false;

void capture(POINT a, POINT b) {
// source: https://causeyourestuck.io/2016/01/12/screenshot-c-win32-api/
	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY);

	// save bitmap to clipboard
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	CloseClipboard();

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
}

//void SetupBitmapInfo(BITMAPINFO& bmi, int bWidth, int bHeight, int bitsPerPixel);
//bool CompareColour(RGBQUAD * pPixels, int height, int width, int x, int y);
//void ScanBMP(Scan * scan);

bool Aim() {
	POINT a, b;
	a.x = 720;
	a.y = 405;
	b.x = 1200;
	b.y = 675;

	POINT pos;



	while (true) { // while rmb pressed
		if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
			capture(a, b);

			GetCursorPos(&pos);
			pos.x += 1;
			HWND mWindow = GetFocus();
			mouse_event(MOUSEEVENTF_MOVE, -1, 1, 0, 0);
		}
		Sleep(5);
		// get bitmap
		// compute offset to first colour match
		// move mouse
		// wait duration
	}

	// !paused && 
	return true;
}

//MouseCoordinates CurrentMouseXY(0, 0);


int main() {


	Aim();
	//system("pause");
	return 0;
}
