#pragma once
#include "iostream"
#include "process.h"
#include "Windows.h"

class Scan {
public:
	BITMAP Bm;
	RECT RcWindow;
	RGBQUAD* PPixels;

	Scan(BITMAP bm, RECT rcWindow, RGBQUAD* pPixels) {
		Bm = bm;
		RcWindow = rcWindow;
		PPixels = pPixels;
	}
};

class MouseCoordinates {
public:
	int X;
	int Y;
	MouseCoordinates(int x, int y) {
		X = x;
		Y = y;
	}
};