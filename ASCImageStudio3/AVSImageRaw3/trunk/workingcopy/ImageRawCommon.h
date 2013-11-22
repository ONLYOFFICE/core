#pragma once
#include "gdiplus.h"


namespace ImageRaw3{

	HRESULT ImageToMemory(Gdiplus::Bitmap* pBitmap, BYTE* pPixels);
	void Temperature_to_RGB(double T, double RGB[3]);
	void RGB_to_Temperature(double RGB[3], float *T, float *Green);
}