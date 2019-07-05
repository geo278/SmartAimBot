// SmartAimBot.cpp : Defines the entry point for the application.

//#include "scan.h"
#include "iostream"
#include "Windows.h"

using namespace std;

//bool paused = false;

HBITMAP capture(POINT a, POINT b) {
// code for taking screenshots: https://causeyourestuck.io/2016/01/12/screenshot-c-win32-api/
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

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
	return hBitmap;
}



bool Aim() {
	POINT a, b;
	a.x = 760;
	a.y = 340;
	b.x = 1160;
	b.y = 740;
	HBITMAP hbm;

	while (true) { // while rmb pressed
		if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
			hbm = capture(a, b);





		// get bitmap 
		// match colours
		// compute offset to first colour match





			mouse_event(MOUSEEVENTF_MOVE, -10, 1, 0, 0); // x and y are deltas, not abs coordinates
		}
		Sleep(5); // extra buffer time
	}

	return true;
}

int main() {
	Aim();
	return 0;
}
