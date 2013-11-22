/*!	\file	ImageRawRoutines.cpp
 *	
 *	\brief	Вспомогательные функции
 */
#include "stdafx.h"
#include "gdiplus.h"
using namespace Gdiplus;

namespace ImageRaw3{

HRESULT ImageToMemory(Bitmap* pBitmap, BYTE* pPixels)
{
	HRESULT hr;
	BOOL Flipped;
	if (!pBitmap || !pPixels)
		return E_POINTER;

	// pre-flip the image



	Flipped = (pBitmap->RotateFlip(RotateNoneFlipY) == Ok);

	// create locking settings
	Rect rectLock(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	BitmapData* pBitmapData = new BitmapData();

	if (!pBitmapData)
		return E_OUTOFMEMORY;

	// variables
	BOOL Success = FALSE;
	int nX, nY, nIndex = 0;
	int nOffset = 0;

	// try to process 24bpp image first

	// lock the bitmap bits
	if ((pBitmap->GetPixelFormat() == PixelFormat1bppIndexed ||
		pBitmap->GetPixelFormat() == PixelFormat4bppIndexed  ||
		pBitmap->GetPixelFormat() == PixelFormat16bppGrayScale ||
		pBitmap->GetPixelFormat() == PixelFormat16bppRGB555 ||
		pBitmap->GetPixelFormat() == PixelFormat16bppRGB565 ||
		pBitmap->GetPixelFormat() == PixelFormat24bppRGB ||
		pBitmap->GetPixelFormat() == PixelFormat32bppRGB ) &&
		pBitmap->LockBits(&rectLock, ImageLockModeRead, PixelFormat24bppRGB, pBitmapData) == Ok)
	{
		// compute stride value
		int nStride = pBitmapData->Stride;
		if (!Flipped) nStride *= -1;

		// retrieve pointer to the locked bits
		BYTE* pBitmapPixels = (BYTE*)pBitmapData->Scan0;

		// copy pixel info
		long lH = (int)pBitmap->GetHeight();
		long lW = (int)pBitmap->GetWidth();
		for (nY = 0; nY < lH; ++nY)
		{
			nOffset = (nStride < 0) ? (pBitmap->GetHeight() - 1 - nY)*(-nStride) : nY*nStride;

			for (nX = 0; nX < lW; ++nX, nIndex += 4, nOffset += 3)
			{
				pPixels[nIndex + 0] = pBitmapPixels[nOffset + 0];
				pPixels[nIndex + 1] = pBitmapPixels[nOffset + 1];
				pPixels[nIndex + 2] = pBitmapPixels[nOffset + 2];
				pPixels[nIndex + 3] = 255;
			}
		}

		// unlock the bits
		pBitmap->UnlockBits(pBitmapData);

		// update output flag
		Success = TRUE;
	}
	else
	{
		// lock the bitmap bits
		if (pBitmap->LockBits(&rectLock, ImageLockModeRead, PixelFormat32bppARGB, pBitmapData) == Ok)
		{
			// compute stride value
			int nStride = pBitmapData->Stride; if (!Flipped) nStride *= -1;

			// retrieve pointer to the locked bits
			BYTE* pBitmapPixels = (BYTE*)pBitmapData->Scan0;

			// copy pixel info
			for (nY = 0; nY < (int)pBitmap->GetHeight(); ++nY)
			{
				nOffset = (nStride < 0) ? (pBitmap->GetHeight() - 1 - nY)*(-nStride) : nY*nStride;

				for (nX = 0; nX < (int)pBitmap->GetWidth(); ++nX, nIndex += 4, nOffset += 4)
				{
					pPixels[nIndex + 0] = pBitmapPixels[nOffset + 0];
					pPixels[nIndex + 1] = pBitmapPixels[nOffset + 1];
					pPixels[nIndex + 2] = pBitmapPixels[nOffset + 2];
					pPixels[nIndex + 3] = pBitmapPixels[nOffset + 3];
				}
			}

			// unlock the bits
			pBitmap->UnlockBits(pBitmapData);

			// update output flag
			Success = TRUE;
		}
	}

	// delete unnecessary bitmap data pointer
	if (pBitmapData)
		delete pBitmapData;

	// raise error (if necessary)
	return Success?S_OK:E_FAIL;
}


static const double XYZ_to_RGB[3][3] = {{ 3.24071,	-0.969258,  0.0556352 },{-1.53726,	1.87599,    -0.203996 },{-0.498571,	0.0415557,  1.05707 } };
/*! \brief	Convert between Temperature and RGB.
 *
 *	\details	Base on information from http://www.brucelindbloom.com/
 * 				The fit for D-illuminant between 4000K and 15000K are from CIE
 * 				The generalization to 2000K < T < 4000K and the blackbody fits
 * 				are my own and should be taken with a grain of salt.
 */
void Temperature_to_RGB(double T, double RGB[3])
{
	int c;
	double xD, yD, X, Y, Z, max;
	// Fit for CIE Daylight illuminant
	if (T<= 4000) {
		xD = 0.27475e9/(T*T*T) - 0.98598e6/(T*T) + 1.17444e3/T + 0.145986;
	} else if (T<= 7000) {
		xD = -4.6070e9/(T*T*T) + 2.9678e6/(T*T) + 0.09911e3/T + 0.244063;
	} else {
		xD = -2.0064e9/(T*T*T) + 1.9018e6/(T*T) + 0.24748e3/T + 0.237040;
	}
	yD = -3*xD*xD + 2.87*xD - 0.275;

	// Fit for Blackbody using CIE standard observer function at 2 degrees
	//xD = -1.8596e9/(T*T*T) + 1.37686e6/(T*T) + 0.360496e3/T + 0.232632;
	//yD = -2.6046*xD*xD + 2.6106*xD - 0.239156;

	// Fit for Blackbody using CIE standard observer function at 10 degrees
	//xD = -1.98883e9/(T*T*T) + 1.45155e6/(T*T) + 0.364774e3/T + 0.231136;
	//yD = -2.35563*xD*xD + 2.39688*xD - 0.196035;

	X = xD/yD;
	Y = 1;
	Z = (1-xD-yD)/yD;
	max = 0;
	for (c=0; c<3; c++) {
		RGB[c] = X*XYZ_to_RGB[0][c] + Y*XYZ_to_RGB[1][c] + Z*XYZ_to_RGB[2][c];
		if (RGB[c]>max) max = RGB[c];
	}
	for (c=0; c<3; c++) RGB[c] = RGB[c]/max;
}

void RGB_to_Temperature(double RGB[3], float *T, float *Green)
{
	double Tmax, Tmin, testRGB[3];
	Tmin = 2000;
	Tmax = 15000;
	for (*T=(Tmax+Tmin)/2; Tmax-Tmin>10; *T=(Tmax+Tmin)/2)
	{
		Temperature_to_RGB(*T, testRGB);
		if (testRGB[2]/testRGB[0] > RGB[2]/RGB[0])
			Tmax = *T;
		else
			Tmin = *T;
	}
	*Green = (testRGB[1]/testRGB[0]) / (RGB[1]/RGB[0]);
}

};