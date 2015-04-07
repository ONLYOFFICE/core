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
		for (nY = 0; nY < (int)pBitmap->GetHeight(); ++nY)
		{
			nOffset = (nStride < 0) ? (pBitmap->GetHeight() - 1 - nY)*(-nStride) : nY*nStride;

			for (nX = 0; nX < (int)pBitmap->GetWidth(); ++nX, nIndex += 4, nOffset += 3)
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


};