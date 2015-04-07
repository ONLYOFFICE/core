// ImageComposeIPP : implementation file

#include "stdafx.h"
#include "ImageComposeIPP.h"
#include "Patterns.h"


BOOL IPPResizeCenter(BYTE* pBGRASource, BYTE* pBGRAResult, int nWidth, int nHeight, double dXFactor, double dYFactor, double dXCenter, double dYCenter)
{

	for ( int nYIndex = 0, nIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
		{
			pBGRASource[ nIndex + 3 ] = 255;
			pBGRAResult[ nIndex + 3 ] = 255;
		}
	}

	BYTE* pBufferSource = new BYTE[4 * nWidth * nHeight];
	BYTE* pBufferResult = new BYTE[4 * nWidth * nHeight];
		
	::memcpy( pBufferSource, pBGRASource, 4 * nWidth * nHeight);
	::memcpy( pBufferResult, pBGRAResult, 4 * nWidth * nHeight);
	
	pBufferSource += 4 * nWidth * (nHeight - 1);
	pBufferResult += 4 * nWidth * (nHeight - 1);
	
	Gdiplus::Bitmap oSourceImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBufferSource );
	Gdiplus::Bitmap oResultImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBufferResult );
	
	Gdiplus::Graphics oRender( &oResultImage );
		
	oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
	
	double dRectWidth  = nWidth * dXFactor;
	double dRectHeight = nHeight * dYFactor;

	oRender.DrawImage(&oSourceImage, Gdiplus::RectF( float(dXCenter * (1 - dXFactor)),  float(dYCenter * (1 - dYFactor)), float(dRectWidth), float(dRectHeight)), 0, 0, 
			nWidth, nHeight, Gdiplus::Unit::UnitPixel, 0, 0, 0); 
	
	pBufferSource -= 4 * nWidth * (nHeight - 1);
	pBufferResult -= 4 * nWidth * (nHeight - 1);
	
	::memcpy( pBGRAResult, pBufferResult, 4 * nWidth * nHeight);
	
	delete []pBufferSource;
	delete []pBufferResult;
	return TRUE;
}

BOOL IPPRotate(const SmartImage &first,const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bIn, int nFirstAngle, BOOL bTransparent)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	BYTE *pBuffer = new BYTE [4 * nWidth * nHeight]; 
	double dInFrame = dFrame;

	if( !bIn )
		dInFrame = 1 - dFrame; 
	double dAngle = nFirstAngle * dInFrame;

	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiRect roiTileRect = {0,0,nSizeX,nSizeY};
	IppiSize roiSize = {nWidth,nHeight};
	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;

	IppiSize roiRightBorderTile = {nWidth % nSizeX,nSizeY};
	IppiSize roiBottomBorderTile = {nSizeX,nHeight % nSizeY};

	double dKoef = dFrame;
	LPBYTE pIn = first.Pixels;
	LPBYTE pOut = last.Pixels;
	if (bIn)
	{
		dKoef = 1.0 - dFrame;
		pOut = first.Pixels;
		pIn = last.Pixels;
	}
	
	memcpy(pBuffer,pIn,4*nWidth*nHeight);
	
	int roiScaleSizeW = max(1, nSizeX*dKoef);
	int roiScaleSizeH = max(1, nSizeY*dKoef);
	IppiSize roiScaleSize = {roiScaleSizeW,roiScaleSizeH};
	IppiRect roiScaleRect = {0,0,roiScaleSizeW,roiScaleSizeH};
	Ipp8u *pScaleBuffer = new Ipp8u[4*roiScaleSize.width*roiScaleSize.height];
	if (NULL==pScaleBuffer)
		return FALSE;


	double xShift = 0.5*(1.0-dKoef)*nSizeX;
	double yShift = 0.5*(1.0-dKoef)*nSizeY;
	ippiAddRotateShift(0.5*roiScaleSize.width, 0.5*roiScaleSize.height,
						dAngle, &xShift, &yShift);

	IppiRect srcROI = {0,0,nSizeX,nSizeY};
	if (0==roiRightBorderTile.width)
	{
		for (int j=0;j<nVTileCount;j++)
		{
			srcROI.x = 0;
			for (int i=0;i<nHTileCount;i++)
			{
				ippiResize_8u_AC4R(
					pOut, roiSize, 4*nWidth, srcROI,
					pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
					dKoef, dKoef, IPPI_INTER_LINEAR);


				ippiRotate_8u_AC4R(
					pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiScaleRect,
					pBuffer + 4*(srcROI.x+nWidth*srcROI.y), 4*nWidth, roiTileRect,
					dAngle, xShift, yShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

				srcROI.x+=nSizeX;
			}
			srcROI.y += nSizeY;
		}
	}
	else
	{
		IppiRect srcROIRightBorder = {nSizeX*nHTileCount,0,roiRightBorderTile.width,nSizeY};
		IppiRect roiRightBorderScaleRect = {0,0,roiRightBorderTile.width*dKoef,nSizeY*dKoef};
		IppiRect roiRightBorderTileRect = {0,0,roiRightBorderTile.width,nSizeY};

		double xRBShift = 0.5*(1.0-dKoef)*roiRightBorderTile.width;
		double yRBShift = 0.5*(1.0-dKoef)*nSizeY;
		ippiAddRotateShift(0.5*roiRightBorderScaleRect.width, 0.5*roiScaleSize.height,
							dAngle, &xRBShift, &yRBShift);
		for (int j=0;j<nVTileCount;j++)
		{
			srcROI.x = 0;
			for (int i=0;i<nHTileCount;i++)
			{
				ippiResize_8u_AC4R(
					pOut, roiSize, 4*nWidth, srcROI,
					pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
					dKoef, dKoef, IPPI_INTER_LINEAR);


				IppStatus iStatus = ippiRotate_8u_AC4R(
					pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiScaleRect,
					pBuffer + 4*(srcROI.x+nWidth*srcROI.y), 4*nWidth, roiTileRect,
					dAngle, xShift, yShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

				srcROI.x+=nSizeX;
			}

			ippiResize_8u_AC4R(
				pOut, roiSize, 4*nWidth, srcROIRightBorder,
				pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
				dKoef, dKoef, IPPI_INTER_LINEAR);


			IppStatus iStatus = ippiRotate_8u_AC4R(
				pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiRightBorderScaleRect,
				pBuffer + 4*(srcROI.x+nWidth*srcROI.y), 4*nWidth, roiRightBorderTileRect,
				dAngle, xRBShift, yRBShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

				
			srcROI.y += nSizeY;
			srcROIRightBorder.y = srcROI.y;
		}
	}

	if (0!=roiBottomBorderTile.height)
	{
		IppiRect srcROIBottomBorder = {0,nSizeY*nVTileCount,nSizeX,roiBottomBorderTile.height};
		IppiRect roiBottomBorderScaleRect = {0,0,nSizeX*dKoef,roiBottomBorderTile.height*dKoef};
		IppiRect roiBottomBorderTileRect = {0,0,nSizeX,roiBottomBorderTile.height};

		double xBBShift = 0.5*(1.0-dKoef)*nSizeX;
		double yBBShift = 0.5*(1.0-dKoef)*roiBottomBorderTile.height;
		ippiAddRotateShift(0.5*roiScaleSize.width, 0.5*roiBottomBorderScaleRect.height,
							dAngle, &xBBShift, &yBBShift);
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(
				pOut, roiSize, 4*nWidth, srcROIBottomBorder,
				pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
				dKoef, dKoef, IPPI_INTER_LINEAR);

			ippiRotate_8u_AC4R(
				pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiBottomBorderScaleRect,
				pBuffer + 4*(srcROIBottomBorder.x+nWidth*srcROIBottomBorder.y), 4*nWidth, roiBottomBorderTileRect,
				dAngle, xBBShift, yBBShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

			srcROIBottomBorder.x += nSizeX;
		}
		if (0!=roiRightBorderTile.width)
		{
			srcROIBottomBorder.width = roiRightBorderTile.width;
			roiBottomBorderTileRect.width = roiRightBorderTile.width;
			roiBottomBorderScaleRect.width = roiRightBorderTile.width*dKoef;

			double xBBShift = 0.5*(1.0-dKoef)*roiRightBorderTile.width;
			double yBBShift = 0.5*(1.0-dKoef)*roiBottomBorderTile.height;
			ippiAddRotateShift(0.5*roiBottomBorderScaleRect.width, 0.5*roiBottomBorderScaleRect.height,
								dAngle, &xBBShift, &yBBShift);
			ippiResize_8u_AC4R(
				pOut, roiSize, 4*nWidth, srcROIBottomBorder,
				pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
				dKoef, dKoef, IPPI_INTER_LINEAR);

			ippiRotate_8u_AC4R(
				pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiBottomBorderScaleRect,
				pBuffer + 4*(srcROIBottomBorder.x+nWidth*srcROIBottomBorder.y), 4*nWidth, roiBottomBorderTileRect,
				dAngle, xBBShift, yBBShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
		
		}
	}
	if ( bTransparent )
	{
		int nIndex = 0;
		double dKoef = dFrame;
		BYTE* pSource;
		if ( bIn )
			pSource = last.Pixels;
		else
		{
			pSource = first.Pixels;
			dKoef = 1 - dFrame;
		}
		for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
		{
			for(int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
			{
				result.Pixels[nIndex]     = pSource[nIndex]     * dKoef + pBuffer[nIndex]     * (1 - dKoef);
				result.Pixels[nIndex + 1] = pSource[nIndex + 1] * dKoef + pBuffer[nIndex + 1] * (1 - dKoef);
				result.Pixels[nIndex + 2] = pSource[nIndex + 2] * dKoef + pBuffer[nIndex + 2] * (1 - dKoef);
			}
		}
	}
	else
	{
		::memcpy( result.Pixels, pBuffer, 4 * nWidth * nHeight);
	}

	delete []pBuffer;	
	delete []pScaleBuffer;
	return TRUE;
}



BOOL IPPStretchDoorVertical(const SmartImage &first,const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;
	IppiSize roiSize = {nWidth,nHeight};

	Ipp8u *pA = NULL;
	Ipp8u *pB = NULL;
	Ipp8u *pResult = result.Pixels;
	double dKoef = dFrame;

	if (bIn)
	{
		pA = last.Pixels;
		pB = first.Pixels;
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoef = 1.0- dFrame;
	}
	
	dKoef = floor(dKoef*nSizeY)/nSizeY;

	int nAScaledHeight = nSizeY*dKoef;
	dKoef = (double)(nAScaledHeight/2) / (nSizeY/2);
	int nBScaledHeight = nSizeY-(int)(nAScaledHeight/2)*2;
	double dKoefB = (double)(nBScaledHeight) / nSizeY;
	int nVTileCount = nHeight / nSizeY;

	int nBottomBorder = nHeight % nSizeY;
	IppiRect srcRectA1 = {0,0,nWidth,nSizeY/2};
	IppiRect srcRectB = {0,0,nWidth,nSizeY};
	IppiRect srcRectA2 = {0,nSizeY/2,nWidth,nSizeY - srcRectA1.height};
	IppiSize roiAScaleSize = {nWidth,nAScaledHeight/2};
	IppiSize roiBScaleSize = {nWidth,nBScaledHeight};


	Ipp8u *pTemp = result.Pixels;
	for (int i=0;i<nVTileCount;i++)
	{
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA1,
			pResult, 4*nWidth, roiAScaleSize,
			1.0, dKoef, IPPI_INTER_LINEAR);
		srcRectA1.y += nSizeY;
		pTemp += 4*nWidth*roiAScaleSize.height;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRectB,
			pTemp, 4*nWidth, roiBScaleSize,
			1.0, /*1.0-dKoef*/dKoefB, IPPI_INTER_LINEAR);
		srcRectB.y += srcRectB.height;
		pTemp += 4*nWidth*roiBScaleSize.height;

		dKoef = (double)(nAScaledHeight/2) / (nSizeY/2 + nSizeY%2);
		
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA2,
			pTemp, 4*nWidth, roiAScaleSize,
			1.0, dKoef, IPPI_INTER_LINEAR);
		srcRectA2.y += nSizeY;
		pResult += 4*nWidth*nSizeY;			
		pTemp = pResult;
	}
	if (0!=nBottomBorder)
	{
		dKoef = floor(dKoef*nBottomBorder)/nBottomBorder;
	
		srcRectA1.height = nBottomBorder/2;
		srcRectB.height = nBottomBorder;
		srcRectA2.y		= srcRectA2.y - nSizeY + srcRectA1.height;
		srcRectA2.height = nBottomBorder - srcRectA1.height;
		roiAScaleSize.height = nBottomBorder*dKoef/2;
		roiBScaleSize.height = nBottomBorder - 2*roiAScaleSize.height;		
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA1,
			pResult, 4*nWidth, roiAScaleSize,
			1.0, dKoef, IPPI_INTER_LINEAR);
		pResult += 4*nWidth*roiAScaleSize.height;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRectB,
			pResult, 4*nWidth, roiBScaleSize,
			1.0, /*1.0-dKoef*/dKoefB, IPPI_INTER_LINEAR);
		pResult += 4*nWidth*roiBScaleSize.height;
		
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA2,
			pResult, 4*nWidth, roiAScaleSize,
			1.0, dKoef, IPPI_INTER_LINEAR);			
	}
	return TRUE;
}

BOOL IPPStretchDoorHorizontal(const SmartImage &first,const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;
	IppiSize roiSize = {nWidth,nHeight};

	Ipp8u *pA = NULL;
	Ipp8u *pB = NULL;
	Ipp8u *pResult = result.Pixels;
	double dKoef = dFrame;

	if (bIn)
	{
		pA = last.Pixels;
		pB = first.Pixels;
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoef = 1.0- dFrame;
	}

	dKoef = floor(dKoef*nSizeX)/nSizeX;
	
	int nAScaledWidth = nSizeX*dKoef;
	dKoef = (double)(nAScaledWidth/2) / (nSizeX/2);
	int nBScaledWidth = nSizeX-(int)(nAScaledWidth/2)*2;
	int nHTileCount = nWidth / nSizeX;

	int nRightBorder = nWidth % nSizeX;
	IppiRect srcRectA1 = {0,0,nSizeX/2,nHeight};
	IppiRect srcRectB = {0,0,nSizeX,nHeight};
	IppiRect srcRectA2 = {nSizeX/2,0,nSizeX - nSizeX/2,nHeight};
	IppiSize roiAScaleSize = {nAScaledWidth/2,nHeight};
	IppiSize roiBScaleSize = {nBScaledWidth,nHeight};
	
	Ipp8u *pTemp = result.Pixels;
	for (int i=0;i<nHTileCount;i++)
	{
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA1,
			pResult, 4*nWidth, roiAScaleSize,
			dKoef, 1.0, IPPI_INTER_LINEAR);
		srcRectA1.x += nSizeX;
		pTemp += 4*roiAScaleSize.width;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRectB,
			pTemp, 4*nWidth, roiBScaleSize,
			1.0-dKoef, 1.0, IPPI_INTER_LINEAR);
		srcRectB.x += srcRectB.width;
		pTemp += 4*roiBScaleSize.width;

		
		dKoef = (double)(nAScaledWidth/2) / (nSizeX/2 + nSizeX%2);
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA2,
			pTemp, 4*nWidth, roiAScaleSize,
			dKoef, 1.0, IPPI_INTER_LINEAR);
		srcRectA2.x += nSizeX;			
		pResult += 4*nSizeX;
		pTemp = pResult;
	}
	if (0!=nRightBorder)
	{
		dKoef = floor(dKoef*nRightBorder)/nRightBorder;
	
		srcRectA1.width = nRightBorder/2;
		srcRectB.width  = nRightBorder;
		srcRectA2.x		= srcRectA2.x - nSizeX + srcRectA1.width;
		srcRectA2.width = nRightBorder - nRightBorder/2;
		roiAScaleSize.width = nRightBorder*dKoef/2;
		roiBScaleSize.width = nRightBorder - roiAScaleSize.width*2;
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA1,
			pResult, 4*nWidth, roiAScaleSize,
			dKoef, 1.0, IPPI_INTER_LINEAR);
		pResult += 4*roiAScaleSize.width;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRectB,
			pResult, 4*nWidth, roiBScaleSize,
			1.0-dKoef, 1.0, IPPI_INTER_LINEAR);
		pResult += 4*roiBScaleSize.width;

		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRectA2,
			pResult, 4*nWidth, roiAScaleSize,
			dKoef, 1.0, IPPI_INTER_LINEAR);			
	}
	return TRUE;
}


BOOL IPPStretchStripsVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};
	nSizeX += nSizeX%2;

	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;

	Ipp8u *pA = first.Pixels;
	Ipp8u *pB = last.Pixels;
	double dKoef1 = dFrame;
	double dKoef2 = 1.0 - dFrame;

	IppiRect srcRect = {0,0,nSizeX/2,nSizeY};
	IppiSize roiBScaleSize = {nSizeX/2,nSizeY*dFrame};
	IppiSize roiAScaleSize = {nSizeX/2,nSizeY - roiBScaleSize.height};

	int nRBorder = nWidth % nSizeX;
	int nBBorder = nHeight % nSizeY;
	IppiSize roiRBorderBScaleSize = {nRBorder/2,nSizeY*dFrame};
	IppiSize roiRBorderAScaleSize = {nRBorder/2,nSizeY - roiRBorderBScaleSize.height};
	IppiRect srcRBorderRect = {0,0,nRBorder/2,nSizeY};

	dKoef2 = (float)roiAScaleSize.height/nSizeY;
	dKoef1 = (float)roiBScaleSize.height/nSizeY;

	Ipp8u *pResult = result.Pixels;
	for (int j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + 4*nWidth*j*nSizeY;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	
			srcRect.x += srcRect.width;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*(nWidth * roiBScaleSize.height + srcRect.width)), 
					4*nWidth, roiAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	

			srcRect.x -= srcRect.width;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*nWidth*roiAScaleSize.height),
					4*nWidth, roiBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRect.x += srcRect.width;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*srcRect.width), 
					4*nWidth, roiBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRect.x += srcRect.width;

			pResult += 4*nSizeX;
		}
		if (0!=nRBorder)
		{
			srcRBorderRect.x = srcRect.x;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					pResult, 4*nWidth, roiRBorderAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	
			srcRBorderRect.x += srcRBorderRect.width;
			
			srcRBorderRect.width = nRBorder/2 + nRBorder%2;
			roiRBorderAScaleSize.width =nRBorder/2 + nRBorder%2;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*(nWidth * roiRBorderBScaleSize.height + nRBorder/2)), 
					4*nWidth, roiRBorderAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	
			roiRBorderAScaleSize.width =nRBorder/2;
			srcRBorderRect.width = nRBorder/2;
			

			srcRBorderRect.x = srcRect.x;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*nWidth*roiRBorderAScaleSize.height),
					4*nWidth, roiRBorderBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRBorderRect.x += srcRBorderRect.width;

			srcRBorderRect.width = nRBorder/2 + nRBorder%2;
			roiRBorderBScaleSize.width +=nRBorder/2 + nRBorder%2;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + nRBorder/2*4), 
					4*nWidth, roiRBorderBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRBorderRect.width = nRBorder/2;
			roiRBorderBScaleSize.width +=nRBorder/2;
			
			srcRBorderRect.y += srcRect.height;
		}
		srcRect.x = 0;
		srcRect.y += srcRect.height;
	}//for j

	if (0!=nBBorder)
	{
		pResult = result.Pixels + 4*nWidth*(nHeight-nBBorder);
		roiBScaleSize.height = nBBorder*dFrame;
		roiAScaleSize.height = nBBorder - roiBScaleSize.height;
		srcRect.x = 0;
		srcRect.y = nHeight-nBBorder; 
		srcRect.height = nBBorder;
		dKoef2 = (float)roiAScaleSize.height/nBBorder;
		dKoef1 = (float)roiBScaleSize.height/nBBorder;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	
			srcRect.x += srcRect.width;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*(nWidth * roiBScaleSize.height + srcRect.width)), 
					4*nWidth, roiAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	

			srcRect.x -= srcRect.width;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*nWidth*roiAScaleSize.height),
					4*nWidth, roiBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRect.x += srcRect.width;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*srcRect.width), 
					4*nWidth, roiBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRect.x += srcRect.width;

			pResult += 4*nSizeX;
		}
		if (0!=nRBorder)
		{
			roiRBorderBScaleSize.height = nBBorder*dFrame;
			roiRBorderAScaleSize.height = nBBorder - roiRBorderBScaleSize.height;
			srcRBorderRect.x = srcRect.x;
			srcRBorderRect.y = nVTileCount*nSizeY; 
			srcRBorderRect.height = nBBorder;
			dKoef2 = (float)roiRBorderAScaleSize.height/nBBorder;
			dKoef1 = (float)roiRBorderBScaleSize.height/nBBorder;


			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					pResult, 4*nWidth, roiRBorderAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	
			srcRBorderRect.x += srcRBorderRect.width;

			srcRBorderRect.width = nRBorder/2 + nRBorder%2;
			roiRBorderAScaleSize.width =nRBorder/2 + nRBorder%2;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*(nWidth * roiRBorderBScaleSize.height + nRBorder/2)), 
					4*nWidth, roiRBorderAScaleSize,
					1.0, dKoef2, IPPI_INTER_LINEAR);	
			srcRBorderRect.width = nRBorder/2;
			roiRBorderAScaleSize.width =nRBorder/2;
			

			srcRBorderRect.x = srcRect.x;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*nWidth*roiRBorderAScaleSize.height),
					4*nWidth, roiRBorderBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRBorderRect.x += srcRBorderRect.width;

			srcRBorderRect.width = nRBorder/2 + nRBorder%2;
			roiRBorderBScaleSize.width =nRBorder/2 + nRBorder%2;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + nRBorder/2*4), 
					4*nWidth, roiRBorderBScaleSize,
					1.0, dKoef1, IPPI_INTER_LINEAR);	
			srcRBorderRect.width = nRBorder/2;
			roiRBorderBScaleSize.width =nRBorder/2;
		}
	}
	return TRUE;
}
BOOL IPPStretchStripsHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	//nSizeY += nSizeY%2;
	IppiSize roiSize = {nWidth,nHeight};

	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;

	Ipp8u *pA = first.Pixels;
	Ipp8u *pB = last.Pixels;

	IppiRect srcRect = {0,0,nSizeX,nSizeY/2};
	IppiRect srcRect2 = {0,0,nSizeX,nSizeY/2 + nSizeY%2};
	IppiSize roiBScaleSize = {nSizeX*dFrame,nSizeY/2};
	IppiSize roiAScaleSize = {nSizeX - roiBScaleSize.width,nSizeY/2};
	IppiSize roiAScaleSize2 = {nSizeX - roiBScaleSize.width,nSizeY/2 + nSizeY%2};
	IppiSize roiBScaleSize2 = {nSizeX*dFrame,nSizeY/2 + nSizeY%2};

	int nBBorder = nHeight % nSizeY;
	int nRBorder = nWidth % nSizeX;

	IppiSize roiRBorderBScaleSize = {nRBorder*dFrame,nSizeY/2};
	IppiSize roiRBorderAScaleSize = {nRBorder - roiRBorderBScaleSize.width,nSizeY/2};
	IppiRect srcRBorderRect = {0,0,nRBorder,nSizeY/2};
	double dKoef2 = (double)roiAScaleSize.width/nSizeX;
	double dKoef1 = (double)roiBScaleSize.width/nSizeX;
	double dKoef2r = (double)roiRBorderAScaleSize.width/max(1, nRBorder);
	double dKoef1r = (double)roiRBorderBScaleSize.width/max(1, nRBorder);

	Ipp8u *pResult = result.Pixels;
	for (int j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + 4*nWidth*j*nSizeY;
		for (int i=0;i<nHTileCount;i++)
		{
			//double dKoef2mem = (double)roiAScaleSize.height / (roiAScaleSize.height + nSizeY%2);
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect2,
					pResult, 4*nWidth, roiAScaleSize2,
					dKoef2, 1.0, IPPI_INTER_LINEAR);
			srcRect.y += srcRect2.height;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*(nWidth * (srcRect2.height) + roiBScaleSize.width)), 
					4*nWidth, roiAScaleSize,
					dKoef2, 1.0, IPPI_INTER_LINEAR);	

			srcRect.y -= srcRect2.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect2,
					(Ipp8u*)(pResult + 4*roiAScaleSize.width),
					4*nWidth, roiBScaleSize2,
					dKoef1, 1.0, IPPI_INTER_LINEAR);	
			srcRect.y += srcRect2.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*nWidth * (srcRect2.height)), 
					4*nWidth, roiBScaleSize,
					dKoef1, 1.0, IPPI_INTER_LINEAR);	
			srcRect.y -= srcRect2.height;
			srcRect.x += srcRect.width;

			pResult += 4*nSizeX;
		}
		if (0!=nRBorder)
		{
			srcRBorderRect.x = srcRect.x;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					pResult, 4*nWidth, roiRBorderAScaleSize,
					dKoef2r, 1.0,  IPPI_INTER_LINEAR);	
			srcRBorderRect.y += srcRBorderRect.height;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*(nWidth * srcRBorderRect.height + roiRBorderBScaleSize.width)), 
					4*nWidth, roiRBorderAScaleSize,
					dKoef2r, 1.0, IPPI_INTER_LINEAR);	
			

			srcRBorderRect.y -= srcRBorderRect.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*roiRBorderAScaleSize.width),
					4*nWidth, roiRBorderBScaleSize,
					dKoef1r, 1.0, IPPI_INTER_LINEAR);	
			srcRBorderRect.y += srcRBorderRect.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*nWidth*srcRBorderRect.height), 
					4*nWidth, roiRBorderBScaleSize,
					dKoef1r, 1.0, IPPI_INTER_LINEAR);	
			
			srcRBorderRect.y += srcRBorderRect.height;
		}
		srcRect.x = 0;
		srcRect.y += 2*srcRect.height;
	}//for j

	if (0!=nBBorder)
	{
		pResult = result.Pixels + 4*nWidth*nVTileCount*nSizeY;
		roiBScaleSize.height = nBBorder/2;
		roiAScaleSize.height = nBBorder/2;
		srcRect.x = 0;
		srcRect.y = nVTileCount*nSizeY; 
		srcRect.height = nBBorder/2;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiAScaleSize,
					dKoef2, 1.0, IPPI_INTER_LINEAR);	
			srcRect.y += srcRect.height;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*(nWidth * srcRect.height + roiBScaleSize.width)), 
					4*nWidth, roiAScaleSize,
					dKoef2, 1.0, IPPI_INTER_LINEAR);	

			srcRect.y -= srcRect.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*roiAScaleSize.width),
					4*nWidth, roiBScaleSize,
					dKoef1, 1.0, IPPI_INTER_LINEAR);	
			srcRect.y += srcRect.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRect,
					(Ipp8u*)(pResult + 4*nWidth * srcRect.height), 
					4*nWidth, roiBScaleSize,
					dKoef1, 1.0, IPPI_INTER_LINEAR);	
			srcRect.y -= srcRect.height;
			srcRect.x += srcRect.width;

			pResult += 4*nSizeX;
		}
		if (0!=nRBorder)
		{
			roiRBorderBScaleSize.height = roiBScaleSize.height;
			roiRBorderAScaleSize.height = roiAScaleSize.height;
			srcRBorderRect.x = srcRect.x;
			srcRBorderRect.y = nVTileCount*nSizeY; 
			srcRBorderRect.height = nBBorder/2;

			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					pResult, 4*nWidth, roiRBorderAScaleSize,
					dKoef2r, 1.0, IPPI_INTER_LINEAR);	
			srcRBorderRect.y += srcRBorderRect.height;
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*(nWidth * srcRBorderRect.height + roiRBorderBScaleSize.width)), 
					4*nWidth, roiRBorderAScaleSize,
					dKoef2r, 1.0, IPPI_INTER_LINEAR);	
			

			srcRBorderRect.y -= srcRBorderRect.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*roiRBorderAScaleSize.width),
					4*nWidth, roiRBorderBScaleSize,
					dKoef1r, 1.0, IPPI_INTER_LINEAR);	
			srcRBorderRect.y += srcRBorderRect.height;
			ippiResize_8u_AC4R(
					pB, roiSize, 4*nWidth, srcRBorderRect,
					(Ipp8u*)(pResult + 4*nWidth*srcRBorderRect.height), 
					4*nWidth, roiRBorderBScaleSize,
					dKoef1r, 1.0, IPPI_INTER_LINEAR);	
		}
	}
	return TRUE;
}
BOOL IPPStretchSideHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bLtoR)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};

	int nHTileCount = nWidth / nSizeX;

	Ipp8u *pA = first.Pixels;
	Ipp8u *pB = last.Pixels;
	double dKoefA = 1.0 - dFrame;
	double dKoefB = dFrame;
	if (bLtoR)
	{
		pA = last.Pixels;
		pB = first.Pixels;
		dKoefA = dFrame;
		dKoefB = 1.0 - dFrame;
	}


	IppiRect srcRect = {0,0,nSizeX,nHeight};
	IppiSize roiAScaleSize = {nSizeX*dKoefA,nHeight};
	IppiSize roiBScaleSize = {nSizeX - roiAScaleSize.width,nHeight};
	dKoefB = (double)roiBScaleSize.width / nSizeX;

	Ipp8u *pResult = result.Pixels;
	for (int i=0;i<nHTileCount;i++)
	{
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiAScaleSize,
			dKoefA, 1.0, IPPI_INTER_LINEAR);	

		pResult += 4*roiAScaleSize.width;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiBScaleSize,
			dKoefB, 1.0, IPPI_INTER_LINEAR);	
		pResult += 4*roiBScaleSize.width;
		srcRect.x += srcRect.width; 
	}
	int nRBorder = nWidth % nSizeX;
	if (0!=nRBorder)
	{
		srcRect.width = nRBorder;
		roiAScaleSize.width = nRBorder*dKoefA;
		roiBScaleSize.width = nRBorder - roiAScaleSize.width;
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiAScaleSize,
			dKoefA, 1.0, IPPI_INTER_LINEAR);	

		pResult += 4*roiAScaleSize.width;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiBScaleSize,
			dKoefB, 1.0, IPPI_INTER_LINEAR);	
	}
	return TRUE;
}

BOOL IPPStretchSideVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bTtoB)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};

	int nVTileCount = nHeight / nSizeY;

	Ipp8u *pA = last.Pixels;
	Ipp8u *pB = first.Pixels;
	double dKoefA = dFrame;
	double dKoefB = 1.0 - dFrame;
	if (bTtoB)
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoefA = 1.0 - dFrame;
		dKoefB = dFrame;
	}


	IppiRect srcRect = {0,0,nWidth,nSizeY};
	IppiSize roiAScaleSize = {nWidth,nSizeY*dKoefA};
	IppiSize roiBScaleSize = {nWidth,nSizeY - roiAScaleSize.height};
	dKoefB = (double)(roiBScaleSize.height) / nSizeY;

	Ipp8u *pResult = result.Pixels;
	for (int i=0;i<nVTileCount;i++)
	{
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiAScaleSize,
			1.0, dKoefA, IPPI_INTER_LINEAR);	

		pResult += 4*nWidth*roiAScaleSize.height;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiBScaleSize,
			1.0, dKoefB, IPPI_INTER_LINEAR);	
		pResult += 4*nWidth*roiBScaleSize.height;
		srcRect.y += srcRect.height; 
	}
	int nBorder = nHeight % nSizeY;
	if (0!=nBorder)
	{
		srcRect.height = nBorder;
		roiAScaleSize.height = nBorder*dKoefA;
		roiBScaleSize.height = nBorder - roiAScaleSize.height;
		ippiResize_8u_AC4R(
			pA, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiAScaleSize,
			1.0, dKoefA, IPPI_INTER_LINEAR);	

		pResult += 4*nWidth*roiAScaleSize.height;

		ippiResize_8u_AC4R(
			pB, roiSize, 4*nWidth, srcRect,
			pResult, 4*nWidth, roiBScaleSize,
			1.0, dKoefB, IPPI_INTER_LINEAR);	
	}
	return TRUE;
}


BOOL IPPStretchSideLTToRB(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};

	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);

	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
	IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

	Ipp8u *pResult = result.Pixels;
	for (int j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + 4*nWidth*(nSizeY*(j+1) - roiScaleSize.height);
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRect.x += srcRect.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRect.x;
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			srcRectRightBorder.y += nSizeY;
		}
		srcRect.x = 0;
		srcRect.y += nSizeY;
	}
	if (0!=srcRectBottomBorder.height)
	{
		IppiSize roiBottomBorderScaleSize = {nSizeX*dFrame,srcRectBottomBorder.height*dFrame};
		srcRectBottomBorder.y = srcRect.y;
		pResult = result.Pixels + 4*nWidth*(nSizeY*nVTileCount + srcRectBottomBorder.height - roiBottomBorderScaleSize.height);
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectBottomBorder,
					pResult, 4*nWidth, roiBottomBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRectBottomBorder.x += srcRectBottomBorder.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRectBottomBorder.x;
			srcRectRightBorder.height = srcRectBottomBorder.height;
			roiRightBorderScaleSize.height = roiBottomBorderScaleSize.height;
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
		}
	}
	return TRUE;
}


BOOL IPPStretchSideLBToRT(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};

	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);

	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
	IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

	Ipp8u *pResult = result.Pixels;
	for (int j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + 4*nSizeY*nWidth*j;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRect.x += srcRect.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRect.x;
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			srcRectRightBorder.y += nSizeY;
		}
		srcRect.x = 0;
		srcRect.y += nSizeY;
	}
	if (0!=srcRectBottomBorder.height)
	{
		IppiSize roiBottomBorderScaleSize = {nSizeX*dFrame,srcRectBottomBorder.height*dFrame};
		srcRectBottomBorder.y = srcRect.y;
		pResult = result.Pixels + 4*nSizeY*nWidth*nVTileCount;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectBottomBorder,
					pResult, 4*nWidth, roiBottomBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRectBottomBorder.x += srcRectBottomBorder.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRectBottomBorder.x;
			srcRectRightBorder.height = srcRectBottomBorder.height;
			roiRightBorderScaleSize.height = roiBottomBorderScaleSize.height;
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
		}
	}
	return TRUE;
}

BOOL IPPStretchSideRBToLT(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};

	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);

	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
	IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

	Ipp8u *pResult = result.Pixels;

	for (int j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + 4*(nSizeY*nWidth*j + nSizeX-roiScaleSize.width);
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRect.x += srcRect.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			pResult -= 4*nSizeX;
			pResult += 4*(roiScaleSize.width + srcRectRightBorder.width - roiRightBorderScaleSize.width);
			srcRectRightBorder.x = srcRect.x;
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			srcRectRightBorder.y += nSizeY;
		}
		srcRect.x = 0;
		srcRect.y += nSizeY;
	}
	if (0!=srcRectBottomBorder.height)
	{
		IppiSize roiBottomBorderScaleSize = {nSizeX*dFrame,srcRectBottomBorder.height*dFrame};
		srcRectBottomBorder.y = srcRect.y;
		pResult = result.Pixels + 4*(nSizeY*nWidth*nVTileCount + nSizeX - roiBottomBorderScaleSize.width);
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectBottomBorder,
					pResult, 4*nWidth, roiBottomBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRectBottomBorder.x += srcRectBottomBorder.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRectBottomBorder.x;
			srcRectRightBorder.height = srcRectBottomBorder.height;
			roiRightBorderScaleSize.height = roiBottomBorderScaleSize.height;

			pResult -= 4*nSizeX;
			pResult += 4*(roiBottomBorderScaleSize.width + srcRectRightBorder.width - roiRightBorderScaleSize.width);

			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
		}
	}
	return TRUE;
}
BOOL IPPStretchSideRTToLB(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};

	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);

	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
	IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

	Ipp8u *pResult = result.Pixels;
	for (int j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + 4*(nWidth*(nSizeY*(j+1) - roiScaleSize.height)
			+ (nSizeX-roiScaleSize.width));
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRect.x += srcRect.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRect.x;
			pResult -= 4*nSizeX;
			pResult += 4*(roiScaleSize.width + srcRectRightBorder.width - roiRightBorderScaleSize.width);
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			srcRectRightBorder.y += nSizeY;
		}
		srcRect.x = 0;
		srcRect.y += nSizeY;
	}
	if (0!=srcRectBottomBorder.height)
	{
		IppiSize roiBottomBorderScaleSize = {nSizeX*dFrame,srcRectBottomBorder.height*dFrame};
		srcRectBottomBorder.y = srcRect.y;
		pResult = result.Pixels + 4*(nWidth*(nSizeY*nVTileCount + srcRectBottomBorder.height - roiBottomBorderScaleSize.height)
			+ (nSizeX - roiBottomBorderScaleSize.width));
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectBottomBorder,
					pResult, 4*nWidth, roiBottomBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRectBottomBorder.x += srcRectBottomBorder.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRectBottomBorder.x;
			srcRectRightBorder.height = srcRectBottomBorder.height;
			roiRightBorderScaleSize.height = roiBottomBorderScaleSize.height;
			pResult -= 4*nSizeX;
			pResult += 4*(roiScaleSize.width + srcRectRightBorder.width - roiRightBorderScaleSize.width);

			ippiResize_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dFrame, dFrame, IPPI_INTER_LINEAR);	
		}
	}
	return TRUE;
}

BOOL IPPStretchCenter(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	IppiSize roiSize = {nWidth,nHeight};

	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	Ipp8u *pA = NULL;
	Ipp8u *pB = NULL;
	double dKoef = dFrame;

	if (bIn)
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoef = 1.0 - dFrame;
	}
	else
	{
		pA = last.Pixels;
		pB = first.Pixels;		
	}

	IppiRect srcRect = {0,0,nSizeX,nSizeY};
	IppiSize roiTileSize = {nSizeX,nSizeY};
	IppiSize roiScaleSize = {nSizeX*dKoef,nSizeY*dKoef};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dKoef,nSizeY*dKoef};

	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
	IppiSize roiBottomBorderScaleSize = {nSizeX*dKoef,srcRectBottomBorder.height*dKoef};

	Ipp8u *pResult = result.Pixels;
	memcpy(pResult,pB,4*nWidth*nHeight);


	for (int j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + 4*(nWidth*(nSizeY*j + (nSizeY - roiScaleSize.height)/2) + 
			(nSizeX - roiScaleSize.width)/2);
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiScaleSize,
					dKoef, dKoef, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRect.x += srcRect.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRect.x;
			pResult -= 4*nSizeX;
			pResult += 4*((nSizeX + roiScaleSize.width)/2 + (srcRectRightBorder.width - roiRightBorderScaleSize.width)/2);
			ippiResize_8u_AC4R(pA, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dKoef, dKoef, IPPI_INTER_LINEAR);	
			srcRectRightBorder.y += nSizeY;
		}
		srcRect.x = 0;
		srcRect.y += nSizeY;
	}
	if (0!=srcRectBottomBorder.height)
	{
		pResult = result.Pixels + 4*(nWidth*(nSizeY*nVTileCount + (srcRectBottomBorder.height - roiBottomBorderScaleSize.height)/2) + 
			(nSizeX - roiScaleSize.width)/2);
		for (int i=0;i<nHTileCount;i++)
		{
			ippiResize_8u_AC4R(
					pA, roiSize, 4*nWidth, srcRect,
					pResult, 4*nWidth, roiBottomBorderScaleSize,
					dKoef, dKoef, IPPI_INTER_LINEAR);	
			pResult += 4*nSizeX;
			srcRect.x += srcRect.width;
		}
		if (0!=srcRectRightBorder.width)
		{
			srcRectRightBorder.x = srcRect.x;
			srcRectRightBorder.height = srcRectBottomBorder.height;
			roiRightBorderScaleSize.height = roiBottomBorderScaleSize.height;

			pResult -= 4*nSizeX;
			pResult += 4*((nSizeX + roiScaleSize.width)/2 + (srcRectRightBorder.width - roiRightBorderScaleSize.width)/2);
			ippiResize_8u_AC4R(pA, roiSize, 4*nWidth, srcRectRightBorder,
					pResult, 4*nWidth, roiRightBorderScaleSize,
					dKoef, dKoef, IPPI_INTER_LINEAR);	
		}
	}
	return TRUE;
}

BOOL IPPStretchWipeSideHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bIn, BOOL bLtoR)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	int nHTileCount = nWidth / nSizeX;

	Ipp8u *pResult = result.Pixels;

	Ipp8u *pA = NULL;
	Ipp8u *pB = NULL;
	double dKoef = floor((1.0 - dFrame)*nSizeX)/nSizeX;
	if (bIn)
	{
		dKoef = dFrame;
		pA = last.Pixels;
		pB = first.Pixels;
		if (!bLtoR)
			pResult += (4*(nSizeX - (int)(nSizeX*dKoef)));
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		if (bLtoR)
			pResult += (4*(nSizeX - (int)(nSizeX*dKoef)));
	}

	IppiSize roiSize = {nWidth,nHeight};
	IppiRect srcRect = {0,0,nSizeX,nHeight};
	IppiSize roiScaleSize = {nSizeX*dKoef,nHeight};
	int nRightBorder = nWidth % nSizeX;

	memcpy(result.Pixels,pB,4*nWidth*nHeight);

	for (int i=0;i<nHTileCount;i++)
	{
		ippiResize_8u_AC4R(
				pA, roiSize, 4*nWidth, srcRect,
				pResult, 4*nWidth, roiScaleSize,
				dKoef, 1.0, IPPI_INTER_LINEAR);	
		srcRect.x += srcRect.width;		
		pResult += 4*nSizeX;
	}

	if (0!=nRightBorder)
	{
		dKoef = floor(dKoef*nRightBorder)/nRightBorder;
		pResult = result.Pixels + 4*nSizeX*nHTileCount;
		if (((bIn)&&(!bLtoR))||((!bIn)&&(bLtoR)))
		{
			pResult += 4*(nRightBorder - (int)(nRightBorder*dKoef));
		}

		srcRect.width = nRightBorder;
		roiScaleSize.width = nRightBorder*dKoef;
		ippiResize_8u_AC4R(
				pA, roiSize, 4*nWidth, srcRect,
				pResult, 4*nWidth, roiScaleSize,
				dKoef, 1.0, IPPI_INTER_LINEAR);	
	}
	return TRUE;
}

BOOL IPPStretchWipeSideVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
			int nSizeX, int nSizeY,	double dFrame, BOOL bIn, BOOL bBtoT)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

	int nVTileCount = nHeight / nSizeY;

	Ipp8u *pResult = result.Pixels;

	Ipp8u *pA = NULL;
	Ipp8u *pB = NULL;	
	double dKoef = floor((1.0 - dFrame)*nSizeY)/nSizeY;
	if (bIn)
	{
		dKoef = floor(dFrame*nSizeY)/nSizeY;
		pA = last.Pixels;
		pB = first.Pixels;
		if (bBtoT)
			pResult += (4*nWidth*(nSizeY - (int)(nSizeY*dKoef)));
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		if (!bBtoT)
			pResult += (4*nWidth*(nSizeY - (int)(nSizeY*dKoef)));
	}

	IppiSize roiSize = {nWidth,nHeight};
	IppiRect srcRect = {0,0,nWidth,nSizeY};
	IppiSize roiScaleSize = {nWidth,nSizeY*dKoef};
	int nBottomBorder = nHeight % nSizeY;

	memcpy(result.Pixels,pB,4*nWidth*nHeight);

	for (int i=0;i<nVTileCount;i++)
	{
		ippiResize_8u_AC4R(
				pA, roiSize, 4*nWidth, srcRect,
				pResult, 4*nWidth, roiScaleSize,
				1.0, dKoef, IPPI_INTER_LINEAR);	
		srcRect.y += srcRect.height;		
		pResult += 4*nSizeY*nWidth;
	}

	if (0!=nBottomBorder)
	{
		dKoef = floor(dKoef*nBottomBorder)/nBottomBorder;
		pResult = result.Pixels + 4*nWidth*(nHeight - nBottomBorder);
		if (((bIn)&&(bBtoT))||((!bIn)&&(!bBtoT)))
		{
			pResult += (4*nWidth*(nBottomBorder - (int)(nBottomBorder*dKoef)));
		}

		srcRect.height = nBottomBorder;
		roiScaleSize.height = dKoef*nBottomBorder;
		ippiResize_8u_AC4R(
				pA, roiSize, 4*nWidth, srcRect,
				pResult, 4*nWidth, roiScaleSize,
				 1.0, dKoef, IPPI_INTER_LINEAR);	
	}
	return TRUE;
}


//transform single image
BOOL IPPTransformTwirl(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Angle, long Degree, double dZoom,DWORD dwBackColor, Utility* g_utility)
{
	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;


	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];
	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];

	if (pBuffer && pxMap && pyMap)
	{
		int nHalfWidth = nWidth/2;
		int nHalfHeight = nHeight/2;
		int nRmax = (nHalfWidth*nHalfWidth + nHalfHeight*nHalfHeight);

		Ipp32f* pxMapCur = pxMap;
		Ipp32f* pyMapCur = pyMap;
		int nVMax = nHeight - nHalfHeight;
		int nHMax = nWidth - nHalfWidth;

		float dR;
		float dPhi;
		float dAngleKoef = Angle * Frame;

		if ((dZoom>1.001)||(dZoom<0.999))
		{
			float fCos[720];
			float fSin[720];
			for (int i=0;i<720;i++)
			{
				fCos[i] = g_utility->g_fCos[i]*dZoom;
				fSin[i] = g_utility->g_fSin[i]*dZoom;
			}

			for (int j=-nHalfHeight;j<nVMax;j++)
			{
				for (int i=-nHalfWidth; i<nHMax;i++)
				{
					dR = sqrt((float)(i*i+j*j)/nRmax);
					dPhi = dAngleKoef;			
					for (int k=0;k<Degree;k++)
						dPhi *= (1.0-dR);

					int nPhi = (int)(2*dPhi);
					while (nPhi<0)
						nPhi += 720;
					while (nPhi>=720)
						nPhi -= 720;

					*pxMapCur++ = (float)i*fCos[nPhi] - (float)j*fSin[nPhi] + nHalfWidth;
					*pyMapCur++ = (float)i*fSin[nPhi] + (float)j*fCos[nPhi] + nHalfHeight;
				}
			}
		}
		else
		{
			for (int j=-nHalfHeight;j<nVMax;j++)
			{
				for (int i=-nHalfWidth; i<nHMax;i++)
				{
					dR = sqrt((float)(i*i+j*j)/nRmax);
					dPhi = dAngleKoef;			
					for (int k=0;k<Degree;k++)
						dPhi *= (1.0-dR);

					int nPhi = (int)(2*dPhi);
					while (nPhi<0)
						nPhi += 720;
					while (nPhi>=720)
						nPhi -= 720;

					*pxMapCur++ = (float)i*g_utility->g_fCos[nPhi] - (float)j*g_utility->g_fSin[nPhi] + nHalfWidth;
					*pyMapCur++ = (float)i*g_utility->g_fSin[nPhi] + (float)j*g_utility->g_fCos[nPhi] + nHalfHeight;
				}
			}
		}

		Ipp8u backColor[4] = { GetBValue(dwBackColor), GetGValue(dwBackColor), GetRValue(dwBackColor), 255 };
		ippiSet_8u_C4R(backColor, (Ipp8u*)pBuffer, 4*nWidth, roiSize);

		ippiRemap_8u_AC4R(pPixels, roiSize, 4*nWidth, srcROI, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			pBuffer, 4*nWidth, roiSize, IPPI_INTER_LINEAR);

		memcpy(pPixels,pBuffer,4*nWidth*nHeight);
	}
	else
		return FALSE;

	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	if (pBuffer)
		delete []pBuffer;
	
	return TRUE;
}

BOOL IPPTwirl(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bClockwise,int nZoom, Utility* g_utility)
{
	double dAngle = -720;
	if (bClockwise)
		dAngle = -dAngle;
	int nDegree = 2;
		
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize = {nWidth,nHeight};

	int nTwirlStep=1;
	for (;nTwirlStep<c_nTwirlStepCount;nTwirlStep++)
	{
		if (dFrame<c_dTwirlStepBeginFrame[nTwirlStep])
			break;
	}

	if (c_nTwirlStepCount <= nTwirlStep)
		nTwirlStep = c_nTwirlStepCount - 1;
	else if (1 >= nTwirlStep)
		nTwirlStep = 1;


	double dFrameIn;
	switch (nTwirlStep)
	{
	case 1:
		memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);
		dFrameIn = dFrame/(c_dTwirlStepBeginFrame[2] - c_dTwirlStepBeginFrame[0]);
		break;
	case 2:
		{
			dFrameIn = dFrame/(c_dTwirlStepBeginFrame[2] - c_dTwirlStepBeginFrame[0]);
			Ipp8u alpha = 0.5*(dFrame - c_dTwirlStepBeginFrame[1])/(c_dTwirlStepBeginFrame[2] - c_dTwirlStepBeginFrame[1]) * 255;
			ippiAlphaCompC_8u_AC4R( 
				last.Pixels, 4*nWidth, alpha,
				first.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 3:
		{			
			dFrameIn = 1.0 - (dFrame - c_dTwirlStepBeginFrame[2])/(c_dTwirlStepBeginFrame[4] - c_dTwirlStepBeginFrame[2]);
			Ipp8u alpha = 0.5*(1.0 - (dFrame - c_dTwirlStepBeginFrame[2])/(c_dTwirlStepBeginFrame[3] - c_dTwirlStepBeginFrame[2])) * 255;
			ippiAlphaCompC_8u_AC4R( 
				first.Pixels, 4*nWidth, alpha,
				last.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 4:
		dFrameIn = 1.0 - (dFrame - c_dTwirlStepBeginFrame[2])/(c_dTwirlStepBeginFrame[4] - c_dTwirlStepBeginFrame[2]);
		memcpy(result.Pixels,last.Pixels,4*nWidth*nHeight);
		break;
	}

	double dZoom = 1.0;
	switch (nZoom)
	{
	case 1:
		dZoom = 1.0/max(0.00001, 1.0 - dFrameIn);
		break;
	case 2:
		dZoom = (1.0 - dFrameIn);
		break;
	}

	IPPTransformTwirl(result.Pixels,nWidth,nHeight,dFrameIn,dAngle,nDegree,dZoom, 0, g_utility);
	return TRUE;
}

BOOL IPPTwirlDrain(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bClockwise, Utility* pUtility)
{
	
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	double dAngle = -360;
	if (bClockwise)
		dAngle = -dAngle;
	int nDegree = 2;
				
	BYTE *pBuffer = new BYTE[4 * nWidth * nHeight];
	::memcpy(pBuffer, pBGRASource1, 4 * nWidth * nHeight);
	IppiSize roiSize = {nWidth,nHeight};
	DWORD dwBackColor = 10;
	IPPTransformTwirl(pBuffer, nWidth, nHeight, dFrame, dAngle + 3*dAngle*dFrame*dFrame, nDegree, 1.0/max(0.00001,1.0 - dFrame), dwBackColor, pUtility);
	::memcpy(pBGRAResult, pBuffer, 4 * nWidth * nHeight);					
	int nIndex = 0;

	for ( int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
		{
			if( pBuffer[nIndex] == GetBValue(dwBackColor) && pBuffer[nIndex + 1] == GetGValue(dwBackColor) && pBuffer[nIndex + 2] == GetRValue(dwBackColor))
			{
				pBGRAResult[nIndex] = pBGRASource2[nIndex];
				pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
				pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
			}
		}
	}
	delete[] pBuffer;
	return TRUE;
}


BOOL IPPTransformSphere(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long CenterX, long CenterY,  long RadiusX, long RadiusY, double Degree, DWORD dwBackColor)
{
	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;

	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];
	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];

	int nRadiusX = max(1, RadiusX*Frame);
	int nRadiusY = max(1, RadiusY*Frame);
	int nRmax = max(1, nRadiusX*nRadiusY);
	Ipp32f* pOffset = 0;
	
	pOffset = new Ipp32f[nRmax];

	if (pBuffer && pxMap && pyMap && pOffset)
	{
		pOffset[0] = 0.0f;
		Ipp32f* pTemp = pOffset+1;		
		float fTemp;
		for (int i=1;i<nRmax;i++)
		{
			fTemp = (float)i/nRmax;
			
			*pTemp = pow((double)fTemp, Degree);
			pTemp++;
		}
			


		Ipp32f* pxMapCur = pxMap;
		Ipp32f* pyMapCur = pyMap;
		for (int j=0;j<nHeight;j++)
		{
			int nY = (j-CenterY);
			for (int i=0; i<nWidth;i++)
			{
				int nX = (i-CenterX);
				float fR = sqrt((float)nX*nX/nRadiusX/nRadiusX+(float)nY*nY/nRadiusY/nRadiusY);
				if (fR<1.0f)
				{
					int nR = fR*nRadiusX*nRadiusY;
				//int nR = sqrt(nX*nX*nRadiusY*nRadiusY+nY*nY*nRadiusX*nRadiusX);
				//if (nR<nRadiusX*nRadiusY)
				//{
					*pxMapCur++ = nX*pOffset[nR] + CenterX;
					*pyMapCur++ = nY*pOffset[nR] + CenterY;
				}
				else
				{
					*pxMapCur++ = i;
					*pyMapCur++ = j;
				}
			}
		}

		Ipp8u backColor[4] = { GetBValue(dwBackColor), GetGValue(dwBackColor), GetRValue(dwBackColor), 255 };
		ippiSet_8u_C4R(backColor, (Ipp8u*)pBuffer, 4*nWidth, roiSize);

		ippiRemap_8u_AC4R(pPixels, roiSize, 4*nWidth, srcROI, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			pBuffer, 4*nWidth, roiSize, IPPI_INTER_LINEAR);

		memcpy(pPixels,pBuffer,4*nWidth*nHeight);
	}
	else
		return FALSE;

	if (pOffset)
		delete []pOffset;
	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	if (pBuffer)
		delete []pBuffer;
	
	return TRUE;
}

BOOL IPPSphere(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bIn)
{
	double dDegree = 3;
	if (bIn)
		dDegree = -0.6;

	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize = {nWidth,nHeight};

	int nSphereStep=1;
	for (;nSphereStep<c_nSphereStepCount;nSphereStep++)
	{
		if (dFrame<c_dSphereStepBeginFrame[nSphereStep])
			break;
	}

	if (c_nSphereStepCount <= nSphereStep)
		nSphereStep = c_nSphereStepCount - 1;
	else if (1 >= nSphereStep)
		nSphereStep = 1;


	double dFrameIn;
	switch (nSphereStep)
	{
	case 1:
		memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);
		dFrameIn = dFrame/(c_dSphereStepBeginFrame[2] - c_dSphereStepBeginFrame[0]);
		break;
	case 2:
		{
			dFrameIn = dFrame/(c_dSphereStepBeginFrame[2] - c_dSphereStepBeginFrame[0]);
			Ipp8u alpha = 0.5*(dFrame - c_dSphereStepBeginFrame[1])/(c_dSphereStepBeginFrame[2] - c_dSphereStepBeginFrame[1]) * 255;
			ippiAlphaCompC_8u_AC4R( 
				last.Pixels, 4*nWidth, alpha,
				first.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 3:
		{			
			dFrameIn = 1.0 - (dFrame - c_dSphereStepBeginFrame[2])/(c_dSphereStepBeginFrame[4] - c_dSphereStepBeginFrame[2]);
			Ipp8u alpha = 0.5*(1.0 - (dFrame - c_dSphereStepBeginFrame[2])/(c_dSphereStepBeginFrame[3] - c_dSphereStepBeginFrame[2])) * 255;
			ippiAlphaCompC_8u_AC4R( 
				first.Pixels, 4*nWidth, alpha,
				last.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 4:
		dFrameIn = 1.0 - (dFrame - c_dSphereStepBeginFrame[2])/(c_dSphereStepBeginFrame[4] - c_dSphereStepBeginFrame[2]);
		memcpy(result.Pixels,last.Pixels,4*nWidth*nHeight);
		break;
	}

	if (bIn)
		IPPTransformSphere(result.Pixels,nWidth,nHeight,dFrameIn,nWidth/2,nHeight/2,nWidth/2,nHeight/2,dDegree,0);
	else
	{
		int sphere_size = max(nWidth,nHeight);
		IPPTransformSphere(result.Pixels,nWidth,nHeight,dFrameIn,nWidth/2,nHeight/2,sphere_size,sphere_size, dDegree,0);
	}
	return TRUE;
}


BOOL IPPSwingSideVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
				int nSizeX, int nSizeY,	double dFrame, BOOL bLeft, BOOL bIn)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX,nSizeY};

	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileVStep = nIppStep*nSizeY;
	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	Ipp8u *pA = NULL;
	Ipp8u *pB = NULL;
	double dKoef = dFrame;

	if (bIn)
	{
		pA = last.Pixels;
		pB = first.Pixels;		
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoef = 1.0 - dFrame;
	}

	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffs[3][3];
	
	if ( bLeft )
	{
		quadDst[0][0] = quadDst[3][0] = 0;
		quadDst[1][0] = quadDst[2][0] = (nSizeX) * dKoef;
		quadDst[0][1] = 0;
		quadDst[3][1] = nSizeY-1;
		quadDst[1][1] = 0.3 * nSizeY * (1 - dKoef);
		quadDst[2][1] = nSizeY - quadDst[1][1];
	}
	else
	{
		quadDst[0][0] = quadDst[3][0] = (nSizeX) * (1 - dKoef);
		quadDst[1][0] = quadDst[2][0] = nSizeX - 1;
		quadDst[0][1] = 0.3 * nSizeY * (1 - dKoef);
		quadDst[3][1] = nSizeY - quadDst[0][1];
		quadDst[1][1] = 0;
		quadDst[2][1] = nSizeY-1;
	}
	ippiGetPerspectiveTransform(srcRect, quadDst, coeffs);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

	memcpy(pResult,pB,nIppStep*nHeight);

	int j = 0;

	for (j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
			roiSize.width = nSizeX;
		}
	}

	if (0!=srcRectBottomBorder.height)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		roiSize.height = srcRectBottomBorder.height;

		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
										srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
		}
	}
	return TRUE;
}


BOOL IPPSwingSideHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
				int nSizeX, int nSizeY,	double dFrame, BOOL bTop, BOOL bIn)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX,nSizeY};
	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileVStep = nIppStep*nSizeY;
	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	Ipp8u *pA = NULL;
	Ipp8u *pB = NULL;
	double dKoef = dFrame;

	if (bIn)
	{
		pA = last.Pixels;
		pB = first.Pixels;		
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoef = 1.0 - dFrame;
	}

	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffs[3][3];
	
	if ( bTop )
	{
		quadDst[0][1] = quadDst[1][1] = (nSizeY) * (1 - dKoef);
		quadDst[2][1] = quadDst[3][1] = nSizeY;
		quadDst[0][0] = 0.3 * nSizeX * (1 - dKoef);
		quadDst[1][0] = nSizeX - quadDst[0][0];
		quadDst[2][0] = nSizeX-1;
		quadDst[3][0] = 0;
	}
	else
	{
		quadDst[0][1] = quadDst[1][1] = 0;
		quadDst[2][1] = quadDst[3][1] = (nSizeY) * dKoef;
		quadDst[0][0] = 0;
		quadDst[1][0] = nSizeX-1;
		quadDst[3][0] = 0.3 * nSizeX * (1 - dKoef);
		quadDst[2][0] = nSizeX - quadDst[3][0];
	}
	ippiGetPerspectiveTransform(srcRect, quadDst, coeffs);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;
	memcpy(pResult,pB,nIppStep*nHeight);

	int j = 0;

	for (j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
			roiSize.width = nSizeX;
		}
	}
	if (0!=srcRectBottomBorder.height)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		roiSize.height = srcRectBottomBorder.height;

		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
										srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
		}
	}
	return TRUE;

}

BOOL IPPSwingSideChangeVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
				int nSizeX, int nSizeY,	double dFrame, BOOL bLeft)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX,nSizeY};

	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileVStep = nIppStep*nSizeY;
	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	Ipp8u *pA = NULL;
	double dKoef;
	
	if (dFrame < 0.5)
	{
		pA = first.Pixels;
		dKoef = 1.0 - 2*dFrame;
	}
	else
	{
		pA = last.Pixels;
		dKoef = 2*dFrame - 1.0;
	}
	
	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffs[3][3];
		
	if ( bLeft )
	{
		quadDst[0][0] = quadDst[3][0] = 0;
		quadDst[1][0] = quadDst[2][0] = (nSizeX-1) * dKoef;
		quadDst[0][1] = 0;
		quadDst[3][1] = nSizeY-1;
		quadDst[1][1] = 0.3 * nSizeY * (1 - dKoef);
		quadDst[2][1] = nSizeY - quadDst[1][1];
	}
	else
	{
		quadDst[0][0] = quadDst[3][0] = (nSizeX-1) * (1 - dKoef);
		quadDst[1][0] = quadDst[2][0] = nSizeX - 1;
		quadDst[0][1] = 0.3 * nSizeY * (1 - dKoef);
		quadDst[3][1] = nSizeY - quadDst[0][1];
		quadDst[1][1] = 0;
		quadDst[2][1] = nSizeY-1;
	}
	ippiGetPerspectiveTransform(srcRect, quadDst, coeffs);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

	memset(pResult,0,nIppStep*nHeight);

	int j = 0;
	for (j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
			roiSize.width = nSizeX;
		}
	}

	if (0!=srcRectBottomBorder.height)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		roiSize.height = srcRectBottomBorder.height;

		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
										srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
		}
	}
	
	return TRUE;
}

BOOL IPPSwingSideChangeHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
				int nSizeX, int nSizeY,	double dFrame, BOOL bTop)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX,nSizeY};
	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileVStep = nIppStep*nSizeY;
	int nHTileCount = nWidth / nSizeX;
	int nVTileCount = nHeight / nSizeY;
	Ipp8u *pA = NULL;
	double dKoef;

	if (dFrame < 0.5)
	{
		pA = first.Pixels;
		dKoef = 1.0 - 2*dFrame;
	}
	else
	{
		pA = last.Pixels;
		dKoef = 2*dFrame - 1.0;
	}

	IppiRect srcRect = {0,0,nSizeX,nSizeY};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffs[3][3];
	if ( bTop )
	{
		quadDst[0][1] = quadDst[1][1] = (nSizeY-1) * (1 - dKoef);
		quadDst[2][1] = quadDst[3][1] = nSizeY - 1;
		quadDst[0][0] = 0.3 * nSizeX * (1 - dKoef);
		quadDst[1][0] = nSizeX - quadDst[0][0];
		quadDst[2][0] = nSizeX-1;
		quadDst[3][0] = 0;
	}
	else
	{
		quadDst[0][1] = quadDst[1][1] = 0;
		quadDst[2][1] = quadDst[3][1] = (nSizeY-1) * dKoef;
		quadDst[0][0] = 0;
		quadDst[1][0] = nSizeX-1;
		quadDst[3][0] = 0.3 * nSizeX * (1 - dKoef);
		quadDst[2][0] = nSizeX - quadDst[3][0];
	}
	ippiGetPerspectiveTransform(srcRect, quadDst, coeffs);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;
	memset(pResult,0,nIppStep*nHeight);

	int j = 0;

	for (j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
			roiSize.width = nSizeX;
		}
	}
	if (0!=srcRectBottomBorder.height)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		roiSize.height = srcRectBottomBorder.height;

		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
										srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
			pResult += nTileHStep;
			pSource += nTileHStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
		}
	}
	return TRUE;

}

BOOL IPPSwingDoorsVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
				int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX/2,nSizeY};

	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileHHalfStep = roiSize.width * 4;
	int		 nTileVStep = nIppStep*nSizeY;
	int		 nHTileCount = nWidth / nSizeX;
	int		 nVTileCount = nHeight / nSizeY;
	Ipp8u	 *pA = NULL;
	Ipp8u	 *pB = NULL;
	double	 dKoef = dFrame;

	if (bIn)
	{
		pA = last.Pixels;
		pB = first.Pixels;		
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoef = 1.0 - dFrame;
	}

	IppiRect srcRect = {0,0,nSizeX/2,nSizeY};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRectBottomBorder = {0,0,nSizeX/2,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffsL[3][3], coeffsR[3][3];
	
	quadDst[0][0] = quadDst[3][0] = 0;
	quadDst[1][0] = quadDst[2][0] = (srcRect.width) * dKoef;
	quadDst[0][1] = 0;
	quadDst[3][1] = nSizeY-1;
	quadDst[1][1] = 0.3 * nSizeY * (1 - dKoef);
	quadDst[2][1] = nSizeY - quadDst[1][1];
	
	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsL);

	quadDst[0][0] = quadDst[3][0] = (srcRect.width) * (1 - dKoef);
	quadDst[1][0] = quadDst[2][0] = srcRect.width;
	quadDst[0][1] = 0.3 * nSizeY * (1 - dKoef);
	quadDst[3][1] = nSizeY - quadDst[0][1];
	quadDst[1][1] = 0;
	quadDst[2][1] = nSizeY-1;

	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsR);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

	memcpy(pResult,pB,nIppStep*nHeight);

	int j = 0;

	for (j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
			pResult += (nTileHStep - nTileHHalfStep);
			pSource += (nTileHStep - nTileHHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsR, IPPI_INTER_LINEAR );
			pResult += nTileHHalfStep;
			pSource += nTileHHalfStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectRightBorder.width;
			if ( srcRectRightBorder.width <= srcRect.width )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsL, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectRightBorder.width - srcRect.width;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
											nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
				pResult += nTileHHalfStep;
				pSource += nTileHHalfStep;
				
				srcRectRightBorder.width = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsR, IPPI_INTER_LINEAR );
				srcRectRightBorder.width = rest + srcRect.width;
			}
			roiSize.width = srcRect.width;
		}
	}
	if (0!=srcRectBottomBorder.height)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		roiSize.height = srcRectBottomBorder.height;

		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
			pResult += (nTileHStep - nTileHHalfStep);
			pSource += (nTileHStep - nTileHHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
			pResult += nTileHHalfStep;
			pSource += nTileHHalfStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
			if ( srcRectRightBorder.width <= srcRect.width )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectBottomBorder.width - srcRect.width;
				srcRectBottomBorder.width = srcRect.width;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
				pResult += nTileHHalfStep;
				pSource += nTileHHalfStep;
				
				srcRectBottomBorder.width = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
			}
		}
	}
	return TRUE;
}


BOOL IPPSwingDoorsHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
							int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX,nSizeY/2};

	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileVStep = nIppStep*nSizeY;
	int		 nTileVHalfStep = roiSize.height * nIppStep;
	int		 nHTileCount = nWidth / nSizeX;
	int		 nVTileCount = nHeight / nSizeY;
	Ipp8u	 *pA = NULL;
	Ipp8u	 *pB = NULL;
	double	 dKoef = dFrame;

	if (bIn)
	{
		pA = last.Pixels;
		pB = first.Pixels;		
	}
	else
	{
		pA = first.Pixels;
		pB = last.Pixels;
		dKoef = 1.0 - dFrame;
	}

	IppiRect srcRect = {0,0,nSizeX,nSizeY/2};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY/2};
	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffsT[3][3], coeffsB[3][3];
	
	quadDst[0][1] = quadDst[1][1] = 0;
	quadDst[2][1] = quadDst[3][1] = (srcRect.height) * dKoef;
	quadDst[0][0] = 0;
	quadDst[1][0] = nSizeX-1;
	quadDst[3][0] = 0.3 * nSizeX * (1 - dKoef);
	quadDst[2][0] = nSizeX - quadDst[3][0];

	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsT);

	quadDst[0][1] = quadDst[1][1] = (srcRect.height) * (1 - dKoef);
	quadDst[2][1] = quadDst[3][1] = srcRect.height;
	quadDst[0][0] = 0.3 * nSizeX * (1 - dKoef);
	quadDst[1][0] = nSizeX - quadDst[0][0];
	quadDst[2][0] = nSizeX-1;
	quadDst[3][0] = 0;

	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsB);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

	memcpy(pResult,pB,4*nWidth*nHeight);

	int j = 0;

	for (j=0;j<nHTileCount;j++)
	{
		pResult = result.Pixels + nTileHStep*j;
		pSource = pA + nTileHStep*j;
		for (int i=0;i<nVTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
			pResult += (nTileVStep - nTileVHalfStep);
			pSource += (nTileVStep - nTileVHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsB, IPPI_INTER_LINEAR );
			pResult += nTileVHalfStep;
			pSource += nTileVHalfStep;
		}
		if (0!=srcRectBottomBorder.height)
		{
			roiSize.height = srcRectBottomBorder.height;
			if ( srcRectBottomBorder.height <= srcRect.height )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsT, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectBottomBorder.height - srcRect.height;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
											nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
				pResult += nTileVHalfStep;
				pSource += nTileVHalfStep;
				
				srcRectBottomBorder.height = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsB, IPPI_INTER_LINEAR );
				srcRectBottomBorder.height = rest + srcRect.height;
			}
			roiSize.height = srcRect.height;
		}
	}
	if (0!=srcRectRightBorder.width)
	{
		pResult = result.Pixels + nTileHStep*j;
		pSource = pA + nTileHStep*j;
		roiSize.width = srcRectRightBorder.width;

		for (int i=0;i<nVTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
			pResult += (nTileVStep - nTileVHalfStep);
			pSource += (nTileVStep - nTileVHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
			pResult += nTileVHalfStep;
			pSource += nTileVHalfStep;
		}
		if (0!=srcRectBottomBorder.height)
		{
			roiSize.height = srcRectRightBorder.height = srcRectBottomBorder.height;
			if ( srcRectBottomBorder.height <= srcRect.height )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectRightBorder.height - srcRect.height;
				srcRectRightBorder.height = srcRect.height;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
				pResult += nTileVHalfStep;
				pSource += nTileVHalfStep;
				
				srcRectRightBorder.height = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
			}
		}
	}
	return TRUE;
}

BOOL IPPSwingDoorsChangeVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
				int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX/2,nSizeY};

	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileHHalfStep = roiSize.width * 4;
	int		 nTileVStep = nIppStep*nSizeY;
	int		 nHTileCount = nWidth / nSizeX;
	int		 nVTileCount = nHeight / nSizeY;
	Ipp8u	 *pA = NULL;
	double	 dKoef;

	if (dFrame < 0.5)
	{
		pA = first.Pixels;
		dKoef = 1.0 - 2*dFrame;
	}
	else
	{
		pA = last.Pixels;
		dKoef = 2*dFrame - 1.0;
	}
	IppiRect srcRect = {0,0,nSizeX/2,nSizeY};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
	IppiRect srcRectBottomBorder = {0,0,nSizeX/2,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffsL[3][3], coeffsR[3][3];
	
	quadDst[0][0] = quadDst[3][0] = 0;
	quadDst[1][0] = quadDst[2][0] = (srcRect.width) * dKoef;
	quadDst[0][1] = 0;
	quadDst[3][1] = nSizeY-1;
	quadDst[1][1] = 0.3 * nSizeY * (1 - dKoef);
	quadDst[2][1] = nSizeY - quadDst[1][1];
	
	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsL);

	quadDst[0][0] = quadDst[3][0] = (srcRect.width) * (1 - dKoef);
	quadDst[1][0] = quadDst[2][0] = srcRect.width;
	quadDst[0][1] = 0.3 * nSizeY * (1 - dKoef);
	quadDst[3][1] = nSizeY - quadDst[0][1];
	quadDst[1][1] = 0;
	quadDst[2][1] = nSizeY-1;

	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsR);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

	memset(pResult,0,nIppStep*nHeight);

	int j = 0;

	for (j=0;j<nVTileCount;j++)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
			pResult += (nTileHStep - nTileHHalfStep);
			pSource += (nTileHStep - nTileHHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsR, IPPI_INTER_LINEAR );
			pResult += nTileHHalfStep;
			pSource += nTileHHalfStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectRightBorder.width;
			if ( srcRectRightBorder.width <= srcRect.width )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsL, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectRightBorder.width - srcRect.width;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
											nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
				pResult += nTileHHalfStep;
				pSource += nTileHHalfStep;
				
				srcRectRightBorder.width = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsR, IPPI_INTER_LINEAR );
				srcRectRightBorder.width = rest + srcRect.width;
			}
			roiSize.width = srcRect.width;
		}
	}
	if (0!=srcRectBottomBorder.height)
	{
		pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
		roiSize.height = srcRectBottomBorder.height;

		for (int i=0;i<nHTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
			pResult += (nTileHStep - nTileHHalfStep);
			pSource += (nTileHStep - nTileHHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
										nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
			pResult += nTileHHalfStep;
			pSource += nTileHHalfStep;
		}
		if (0!=srcRectRightBorder.width)
		{
			roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
			if ( srcRectRightBorder.width <= srcRect.width )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectBottomBorder.width - srcRect.width;
				srcRectBottomBorder.width = srcRect.width;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
				pResult += nTileHHalfStep;
				pSource += nTileHHalfStep;
				
				srcRectBottomBorder.width = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
			}
		}
	}
	return TRUE;
}


BOOL IPPSwingDoorsChangeHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
							int nSizeX, int nSizeY,	double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	IppiSize roiSize = {nSizeX,nSizeY/2};

	int		 nIppStep = 4*nWidth;
	int		 nTileHStep = 4*nSizeX;
	int		 nTileVStep = nIppStep*nSizeY;
	int		 nTileVHalfStep = roiSize.height * nIppStep;
	int		 nHTileCount = nWidth / nSizeX;
	int		 nVTileCount = nHeight / nSizeY;
	Ipp8u	 *pA = NULL;
	double	 dKoef;

	if (dFrame < 0.5)
	{
		pA = first.Pixels;
		dKoef = 1.0 - 2*dFrame;
	}
	else
	{
		pA = last.Pixels;
		dKoef = 2*dFrame - 1.0;
	}

	IppiRect srcRect = {0,0,nSizeX,nSizeY/2};

	IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY/2};
	IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

	double	quadDst[4][2];
	double  coeffsT[3][3], coeffsB[3][3];
	
	quadDst[0][1] = quadDst[1][1] = 0;
	quadDst[2][1] = quadDst[3][1] = (srcRect.height) * dKoef;
	quadDst[0][0] = 0;
	quadDst[1][0] = nSizeX-1;
	quadDst[3][0] = 0.3 * nSizeX * (1 - dKoef);
	quadDst[2][0] = nSizeX - quadDst[3][0];

	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsT);

	quadDst[0][1] = quadDst[1][1] = (srcRect.height) * (1 - dKoef);
	quadDst[2][1] = quadDst[3][1] = srcRect.height;
	quadDst[0][0] = 0.3 * nSizeX * (1 - dKoef);
	quadDst[1][0] = nSizeX - quadDst[0][0];
	quadDst[2][0] = nSizeX-1;
	quadDst[3][0] = 0;

	ippiGetPerspectiveTransform(srcRect, quadDst, coeffsB);

	Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

	memset(pResult,0,4*nWidth*nHeight);

	int j = 0;

	for (j=0;j<nHTileCount;j++)
	{
		pResult = result.Pixels + nTileHStep*j;
		pSource = pA + nTileHStep*j;
		for (int i=0;i<nVTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
			pResult += (nTileVStep - nTileVHalfStep);
			pSource += (nTileVStep - nTileVHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
										nIppStep, srcRect, coeffsB, IPPI_INTER_LINEAR );
			pResult += nTileVHalfStep;
			pSource += nTileVHalfStep;
		}
		if (0!=srcRectBottomBorder.height)
		{
			roiSize.height = srcRectBottomBorder.height;
			if ( srcRectBottomBorder.height <= srcRect.height )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsT, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectBottomBorder.height - srcRect.height;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
											nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
				pResult += nTileVHalfStep;
				pSource += nTileVHalfStep;
				
				srcRectBottomBorder.height = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
											nIppStep, srcRectBottomBorder, coeffsB, IPPI_INTER_LINEAR );
				srcRectBottomBorder.height = rest + srcRect.height;
			}
			roiSize.height = srcRect.height;
		}
	}
	if (0!=srcRectRightBorder.width)
	{
		pResult = result.Pixels + nTileHStep*j;
		pSource = pA + nTileHStep*j;
		roiSize.width = srcRectRightBorder.width;

		for (int i=0;i<nVTileCount;i++)
		{
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
			pResult += (nTileVStep - nTileVHalfStep);
			pSource += (nTileVStep - nTileVHalfStep);
			ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
										nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
			pResult += nTileVHalfStep;
			pSource += nTileVHalfStep;
		}
		if (0!=srcRectBottomBorder.height)
		{
			roiSize.height = srcRectRightBorder.height = srcRectBottomBorder.height;
			if ( srcRectBottomBorder.height <= srcRect.height )
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
			else
			{
				int rest = srcRectRightBorder.height - srcRect.height;
				srcRectRightBorder.height = srcRect.height;
				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
				pResult += nTileVHalfStep;
				pSource += nTileVHalfStep;
				
				srcRectRightBorder.height = rest;

				ippiWarpPerspective_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
											nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
			}
		}
	}
	return TRUE;
}

BOOL IPPFlipOverVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
                         int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
    int nWidth = first.Width;
    int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

    IppiSize roiSize = {nSizeX,nSizeY};

    int		 nIppStep = 4*nWidth;
    int		 nTileHStep = 4*nSizeX;
    int		 nTileVStep = nIppStep*nSizeY;
    int nHTileCount = nWidth / nSizeX;
    int nVTileCount = nHeight / nSizeY;
    Ipp8u *pA = NULL;
    double dKoef = dFrame;

    if (dFrame < 0.5)
    {
        pA = first.Pixels;
    }
    else
    {
        pA = last.Pixels;
        dKoef = 1.0 - dFrame;
    }

    IppiRect srcRect = {0,0,nSizeX,nSizeY};

    IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
    IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

    double	quadDst[4][2];
    double  coeffs[2][4];

    quadDst[0][0] = quadDst[3][0] = (nSizeX-1) * dKoef;
    quadDst[1][0] = quadDst[2][0] = (nSizeX-1) * (1 - dKoef);

    if ( bIn && (dFrame < 0.5) || !bIn && (dFrame >= 0.5)  )
    {
        quadDst[1][1] = 0.3 * nSizeY * dKoef * 2;
        quadDst[0][1] = -quadDst[1][1];
        quadDst[3][1] = nSizeY-1 + quadDst[1][1];
        quadDst[2][1] = nSizeY - quadDst[1][1];
    }
    else
    {
        quadDst[0][1] = 0.3 * nSizeY * dKoef * 2;
        quadDst[3][1] = nSizeY - quadDst[0][1];
        quadDst[1][1] = -quadDst[0][1];
        quadDst[2][1] = nSizeY-1 + quadDst[0][1];
    }
    ippiGetBilinearTransform(srcRect, quadDst, coeffs);
    
	Ipp8u *pResult = result.Pixels;
    Ipp8u *pSource = pA;

    memset(pResult,0,nIppStep*nHeight);

	int j = 0;

    for (j=0;j<nVTileCount;j++)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
                nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );

			pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectRightBorder.width;
		    ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
                nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );        
			roiSize.width = nSizeX;
        }
    }

    if (0!=srcRectBottomBorder.height)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        roiSize.height = srcRectBottomBorder.height;

        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
                srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );

			pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
                nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
		}
    }

    return TRUE;
}
BOOL IPPFlipOverHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
                           int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
    int nWidth = first.Width;
    int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

    IppiSize roiSize = {nSizeX,nSizeY};

    int		 nIppStep = 4*nWidth;
    int		 nTileHStep = 4*nSizeX;
    int		 nTileVStep = nIppStep*nSizeY;
    int nHTileCount = nWidth / nSizeX;
    int nVTileCount = nHeight / nSizeY;
    Ipp8u *pA = NULL;
    double dKoef = dFrame;

    if (dFrame < 0.5)
    {
        pA = first.Pixels;
    }
    else
    {
        pA = last.Pixels;
        dKoef = 1.0 - dFrame;
    }

    IppiRect srcRect = {0,0,nSizeX,nSizeY};

    IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
    IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

    double	quadDst[4][2];
    double  coeffs[2][4];

    quadDst[0][1] = quadDst[1][1] = (nSizeY-1) * dKoef;
    quadDst[2][1] = quadDst[3][1] = (nSizeY-1) * (1 - dKoef);

    if ( bIn && (dFrame < 0.5) || !bIn && (dFrame >= 0.5)  )
    {
        quadDst[3][0] = 0.3 * nSizeX * dKoef * 2;
        quadDst[0][0] = -quadDst[3][0];
        quadDst[1][0] = nSizeX-1 + quadDst[3][0];
        quadDst[2][0] = nSizeX - quadDst[3][0];
    }
    else
    {
        quadDst[0][0] = 0.3 * nSizeX * dKoef * 2;
        quadDst[1][0] = nSizeX - quadDst[0][0];
        quadDst[2][0] = nSizeX-1 + quadDst[0][0];
        quadDst[3][0] = -quadDst[0][0];
    }
    ippiGetBilinearTransform(srcRect, quadDst, coeffs);

    Ipp8u *pResult = result.Pixels;
    Ipp8u *pSource = pA;

    memset(pResult,0,nIppStep*nHeight);

	int j = 0;

    for (j=0;j<nVTileCount;j++)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
                nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
                nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
            roiSize.width = nSizeX;
        }
    }

    if (0!=srcRectBottomBorder.height)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        roiSize.height = srcRectBottomBorder.height;

        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
                srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
                nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
        }
    }

    return TRUE;
}

BOOL IPPFlipAwayVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
                         int nSizeX, int nSizeY, double dFrame, BOOL bIn, BOOL bForward )
{
    int nWidth = first.Width;
    int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

    IppiSize roiSize = {nSizeX,nSizeY};

    int		 nIppStep = 4*nWidth;
    int		 nTileHStep = 4*nSizeX;
    int		 nTileVStep = nIppStep*nSizeY;
    int      nHTileCount = nWidth / nSizeX;
    int      nVTileCount = nHeight / nSizeY;
    Ipp8u    *pA = NULL;
    Ipp8u    *pB = NULL;
    double   dKoef = 5;
    int      nResizedWidth, nResizedHeight;
        
    if ( bIn )
    {
        pA = last.Pixels;
        pB = first.Pixels;
        dFrame = 1 - dFrame;
    }
    else
    {
        pB = last.Pixels;
        pA = first.Pixels;
    }

    int nParts = (int)(dFrame/0.2);

    dKoef *= (( nParts%2 != 0 )? (dFrame - nParts*0.2) : ((nParts+1)*0.2 - dFrame));

    IppiRect srcRect = {0,0,nSizeX,nSizeY};

    IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
    IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

    if ( dFrame <= 0.2 )
    {
        nResizedWidth = (int)((nSizeX-1)*(1-dFrame/2));
        nResizedHeight = (int)((nSizeY-1)*(1-dFrame/2));
    }
    else if ( dFrame > 0.2 && dFrame <= 0.4 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.9-(dFrame-0.2)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.9-(dFrame-0.2)*0.35/0.2));
    }
    else if ( dFrame > 0.4 && dFrame <= 0.6 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.55-(dFrame-0.4)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.55-(dFrame-0.4)/2));
    }
    else if ( dFrame > 0.6 && dFrame <= 0.8 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.45-(dFrame-0.6)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.45-(dFrame-0.6)*0.35/0.2));
    }
    else 
    {
        nResizedWidth = (int)((nSizeX-1)*(0.1-(dFrame-0.8)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.1-(dFrame-0.8)/2));
    }

    double	quadDst[4][2];
    double  coeffs[2][4];

	int nLevelX, nLevelY;
	nLevelY = (int)((nResizedHeight - 1)*(dKoef * 0.7 + 0.3));
	nLevelX = (int)((nResizedWidth - 1)*dKoef);
		
	quadDst[0][0] = quadDst[3][0] = (nSizeX - 1 - nLevelX)/2;
    quadDst[1][0] = quadDst[2][0] = nSizeX - 1 - quadDst[0][0];
 
    if ( bForward && (( dFrame > 0.2 && dFrame <= 0.4 || dFrame > 0.6 && dFrame <= 0.8 )) || 
         !bForward && ( dFrame <= 0.2 || dFrame > 0.4 && dFrame <= 0.6 || dFrame > 0.8 ) )
    {
        quadDst[0][1] = (nSizeY - 1 - nResizedHeight)/2;
        quadDst[3][1] =  nSizeY - 1 - quadDst[0][1];
        quadDst[1][1] = (nSizeY - 1 - nLevelY)/2;
        quadDst[2][1] = nSizeY - 1 - quadDst[1][1];
    }
    else
    {
        quadDst[1][1] = (nSizeY - 1 - nResizedHeight)/2;
        quadDst[2][1] =  nSizeY - 1 - quadDst[1][1];
        quadDst[0][1] = (nSizeY - 1 - nLevelY)/2;
        quadDst[3][1] = nSizeY - 1 - quadDst[0][1];
    }

    ippiGetBilinearTransform(srcRect, quadDst, coeffs);

    Ipp8u *pResult = result.Pixels;
    Ipp8u *pSource = pA;

    memcpy(pResult,pB,nIppStep*nHeight);

	int j = 0;

    for (j=0;j<nVTileCount;j++)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
                nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
                nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
            roiSize.width = nSizeX;
        }
    }

    if (0!=srcRectBottomBorder.height)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        roiSize.height = srcRectBottomBorder.height;

        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
                srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
                nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
        }
    }

    return TRUE;
}

BOOL IPPFlipAwayHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
                           int nSizeX, int nSizeY,	double dFrame, BOOL bIn, BOOL bForward )
{
    int nWidth = first.Width;
    int nHeight = first.Height;
    
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;
    
    IppiSize roiSize = {nSizeX,nSizeY};

    int		 nIppStep = 4*nWidth;
    int		 nTileHStep = 4*nSizeX;
    int		 nTileVStep = nIppStep*nSizeY;
    int      nHTileCount = nWidth / nSizeX;
    int      nVTileCount = nHeight / nSizeY;
    Ipp8u    *pA = NULL;
    Ipp8u    *pB = NULL;
    double   dKoef = 5;
    int      nResizedWidth, nResizedHeight;
        
    if ( bIn )
    {
        pA = last.Pixels;
        pB = first.Pixels;
        dFrame = 1 - dFrame;
    }
    else
    {
        pB = last.Pixels;
        pA = first.Pixels;
    }

    int nParts = (int)(dFrame/0.2);

    dKoef *= (( nParts%2 != 0 )? (dFrame - nParts*0.2) : ((nParts+1)*0.2 - dFrame));

    IppiRect srcRect = {0,0,nSizeX,nSizeY};

    IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
    IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

    if ( dFrame <= 0.2 )
    {
        nResizedWidth = (int)((nSizeX-1)*(1-dFrame/2));
        nResizedHeight = (int)((nSizeY-1)*(1-dFrame/2));
    }
    else if ( dFrame > 0.2 && dFrame <= 0.4 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.9-(dFrame-0.2)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.9-(dFrame-0.2)*0.35/0.2));
    }
    else if ( dFrame > 0.4 && dFrame <= 0.6 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.55-(dFrame-0.4)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.55-(dFrame-0.4)/2));
    }
    else if ( dFrame > 0.6 && dFrame <= 0.8 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.45-(dFrame-0.6)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.45-(dFrame-0.6)*0.35/0.2));
    }
    else 
    {
        nResizedWidth = (int)((nSizeX-1)*(0.1-(dFrame-0.8)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.1-(dFrame-0.8)/2));
    }

    double	quadDst[4][2];
    double  coeffs[2][4];

	int nLevelX, nLevelY;
	nLevelY = (int)((nResizedHeight - 1)*dKoef);
	nLevelX = (int)((nResizedWidth - 1)*(dKoef * 0.7 + 0.3));
		
	quadDst[0][1] = quadDst[1][1] = (nSizeY - 1 - nLevelY)/2;
    quadDst[2][1] = quadDst[3][1] = nSizeY - 1 - quadDst[0][1];

    if ( bForward && (( dFrame > 0.2 && dFrame <= 0.4 || dFrame > 0.6 && dFrame <= 0.8 )) || 
         !bForward && ( dFrame <= 0.2 || dFrame > 0.4 && dFrame <= 0.6 || dFrame > 0.8 ) )
    {
        quadDst[0][0] = (nSizeX - 1 - nResizedWidth)/2;
        quadDst[1][0] =  nSizeX - 1 - quadDst[0][0];
        quadDst[3][0] = (nSizeX - 1 - nLevelX)/2;
        quadDst[2][0] = nSizeX - 1 - quadDst[3][0];
	}
    else
    {
        quadDst[3][0] = (nSizeX - 1 - nResizedWidth)/2;
        quadDst[2][0] =  nSizeX - 1 - quadDst[3][0];
        quadDst[0][0] = (nSizeX - 1 - nLevelX)/2;
        quadDst[1][0] = nSizeX - 1 - quadDst[0][0];
	}

    ippiGetBilinearTransform(srcRect, quadDst, coeffs);

    Ipp8u *pResult = result.Pixels;
    Ipp8u *pSource = pA;

    memcpy(pResult,pB,nIppStep*nHeight);

	int j = 0;

    for (j=0;j<nVTileCount;j++)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        for (int i=0;i<nHTileCount;i++)
        {
            IppStatus ippst = ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
                nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );

            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
                nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
            roiSize.width = nSizeX;
        }
    }
    if (0!=srcRectBottomBorder.height)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        roiSize.height = srcRectBottomBorder.height;

        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
                srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
                nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
        }
    }
    return TRUE;
}


BOOL IPPTwistHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bLtoR, Utility* g_utility)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;

	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pHelp = new Ipp32f[nWidth];

	if (pxMap && pyMap && pHelp)
	{
		Ipp8u backColor[4] = { 0,0,0,255};
		ippiSet_8u_C4R(backColor, result.Pixels, 4*nWidth, roiSize);

		int nHalfHeight = nHeight/2;
		int nVMax = nHeight - nHalfHeight;

		float fT;
		if (bLtoR)
			fT = (3.0 * dFrame - 1.0)* nWidth;
		else
			fT = (3.0 * (1.0-dFrame) - 1.0)* nWidth;

		if (fT<nWidth-1)
		{
			int nPhi;
			for (int i=0; i<nWidth;i++)
			{
				nPhi = (i-fT)*180/nWidth;//массив в удвоенных градусах
				while (nPhi<0)
					nPhi+=720;
				while (nPhi>720)
					nPhi-=720;
				pHelp[i] = g_utility->g_fSin[nPhi];
			}
			
			Ipp32f* pxMapCur = pxMap;
			Ipp32f* pyMapCur = pyMap;
			if (fT<0)
			{
				int nHMax = fT+nWidth;
				for (int j=-nHalfHeight;j<nVMax;j++)
				{

					for (int i=0; i<nHMax;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = nHalfHeight + (float)j/pHelp[i];							
					}
					for (int i=nHMax; i<nWidth;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = nHalfHeight + j;
					}
				}
			}
			else
			{
				int nHMin = fT;
				for (int j=-nHalfHeight;j<nVMax;j++)
				{
					for (int i=0; i<nHMin;i++)
					{
						*pxMapCur++ = -1;
						*pyMapCur++ = -1;
					}
					for (int i=nHMin; i<nWidth;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = nHalfHeight + (float)j/pHelp[i];
					}
				}
			}
			if (bLtoR)
				ippiRemap_8u_AC4R(first.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
			else
				ippiRemap_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
		}

		if (fT>0)
		{
			int nPhi;
			for (int i=0; i<nWidth;i++)
			{
				nPhi = (fT-i)*180/nWidth;;//массив в удвоенных градусах
				while (nPhi<0)
					nPhi+=720;
				while (nPhi>720)
					nPhi-=720;
				pHelp[i] = g_utility->g_fSin[nPhi];
			}
			Ipp32f* pxMapCur = pxMap;
			Ipp32f* pyMapCur = pyMap;
			if (fT>nWidth)
			{
				int nHMax = fT-nWidth;
				for (int j=-nHalfHeight;j<nVMax;j++)
				{
					for (int i=0; i<nHMax;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = nHalfHeight + j;
					}
					for (int i=nHMax; i<nWidth;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = nHalfHeight + (float)j/pHelp[i];
					}
				}
			}
			else
			{
				int nHMin = fT;				
				for (int j=-nHalfHeight;j<nVMax;j++)
				{
					for (int i=0; i<nHMin;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = nHalfHeight + (float)j/pHelp[i];
					}
					for (int i=nHMin; i<nWidth;i++)
					{
						*pxMapCur++ = -1;
						*pyMapCur++ = -1;
					}
				}
			}
			if (bLtoR)
				ippiRemap_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
			else
				ippiRemap_8u_AC4R(first.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
		}
	}

	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	if (pHelp)
		delete []pHelp;
	
	return TRUE;
}


BOOL IPPTwistVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bTtoB, Utility* g_utility)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;

	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pHelp = new Ipp32f[nHeight];

	if (pxMap && pyMap && pHelp)
	{
		Ipp8u backColor[4] = { 0,0,0,255};
		ippiSet_8u_C4R(backColor, result.Pixels, 4*nWidth, roiSize);

		int nHalfWidth = nWidth/2;
		int nHMax = nWidth - nHalfWidth;

		float fT;
		if (bTtoB)
			fT = (3.0 * (1.0-dFrame) - 1.0)* nHeight;
		else
			fT = (3.0 * dFrame - 1.0)* nHeight;

		if (fT<nHeight-1)
		{
			int nPhi;
			for (int i=0; i<nHeight;i++)
			{
				nPhi = (i-fT)*180/nHeight;//массив в удвоенных градусах
				while (nPhi<0)
					nPhi+=720;
				while (nPhi>720)
					nPhi-=720;
				pHelp[i] = g_utility->g_fSin[nPhi];
			}
			
			Ipp32f* pxMapCur = pxMap;
			Ipp32f* pyMapCur = pyMap;
			if (fT<0)
			{
				int nVMax = fT+nHeight;
				for (int j=0;j<nVMax;j++)
				{
					for (int i=-nHalfWidth; i<nHMax;i++)
					{
						*pxMapCur++ = nHalfWidth + (float)i/pHelp[j];
						*pyMapCur++ = j;
					}
				}
				for (int j=nVMax;j<nHeight;j++)
				{
					for (int i=0; i<nWidth;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = j;
					}
				}
			}
			else
			{
				int nVMin = fT + 2;//+2 чтобы наверняка проскочить нулевой синус
				for (int j=0;j<nVMin;j++)
				{
					for (int i=0; i<nWidth;i++)
					{
						*pxMapCur++ = -1;
						*pyMapCur++ = -1;
					}
				}
				for (int j=nVMin;j<nHeight;j++)
				{
					for (int i=-nHalfWidth; i<nHMax;i++)
					{
						*pxMapCur++ = nHalfWidth + (float)i/pHelp[j];
						*pyMapCur++ = j;
					}
				}
			}
			if (bTtoB)
				ippiRemap_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
			else
				ippiRemap_8u_AC4R(first.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
		}

		if (fT>0)
		{
			int nPhi;
			for (int i=0; i<nHeight;i++)
			{
				nPhi = (fT-i)*180/nHeight;//массив в удвоенных градусах
				while (nPhi<0)
					nPhi+=720;
				while (nPhi>720)
					nPhi-=720;
				pHelp[i] = g_utility->g_fSin[nPhi];
			}
			Ipp32f* pxMapCur = pxMap;
			Ipp32f* pyMapCur = pyMap;
			if (fT>nHeight)
			{
				int nVMax = fT-nHeight;
				for (int j=0;j<nVMax;j++)
				{
					for (int i=0; i<nWidth;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = j;
					}
				}
				for (int j=nVMax;j<nHeight;j++)
				{
					for (int i=-nHalfWidth; i<nHMax;i++)
					{
						*pxMapCur++ = nHalfWidth + (float)i/pHelp[j];
						*pyMapCur++ = j;
					}
				}
			}
			else
			{
				int nVMin = fT;				
				for (int j=0;j<nVMin;j++)
				{
					for (int i=-nHalfWidth; i<nHMax;i++)
					{
						*pxMapCur++ = nHalfWidth + (float)i/pHelp[j];
						*pyMapCur++ = j;
					}
				}
				for (int j=nVMin;j<nHeight;j++)
				{
					for (int i=0; i<nWidth;i++)
					{
						*pxMapCur++ = -1;
						*pyMapCur++ = -1;
					}
				}
			}
			if (bTtoB)
				ippiRemap_8u_AC4R(first.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
			else
				ippiRemap_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcROI, 
					pxMap, sizeof(Ipp32f)*nWidth, 
					pyMap, sizeof(Ipp32f)*nWidth, 
					result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
		}
	}

	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	if (pHelp)
		delete []pHelp;
	
	return TRUE;
}

BOOL IPPTwistDiagonal(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection, Utility* g_utility)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;

	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];

	if (pxMap && pyMap)
	{
		Ipp8u backColor[4] = { 0,0,0,255};
		ippiSet_8u_C4R(backColor, result.Pixels, 4*nWidth, roiSize);

		float fT = (3.0 * dFrame - 1.0)* nHeight;
		float fRad = sqrt((double)nWidth*nWidth + nHeight*nHeight);
		float fCosPhi = nWidth/fRad;
		float fSinPhi = nHeight/fRad;

		float fOmega = 360.0/fRad;
		
		Ipp32f* pxMapCur = pxMap;
		Ipp32f* pyMapCur = pyMap;
		int nX,nY;
		switch (nDirection)
		{
		case 0:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*i + fSinPhi*(nHeight-j);
						if (nX<fT+2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX>fT+fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*i + fCosPhi*(nHeight-j);
						nY = (float)nY/g_utility->g_fSin[(int)(((float)nX-fT)*fOmega)];
						*pxMapCur++ = fCosPhi*nX - fSinPhi*nY;
						*pyMapCur++ = nHeight-(fSinPhi*nX + fCosPhi*nY);
					}
				}
			}
			break;
		case 1:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*i + fSinPhi*j;
						if (nX<fT+2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX>fT+fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*i + fCosPhi*j;
						nY = (float)nY/g_utility->g_fSin[(int)(((float)nX-fT)*fOmega)];
						*pxMapCur++ = fCosPhi*nX - fSinPhi*nY;
						*pyMapCur++ = fSinPhi*nX + fCosPhi*nY;
					}
				}
			}
			break;
		case 2:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*(nWidth - i) + fSinPhi*(nHeight-j);
						if (nX<fT+2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX>fT+fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*(nWidth - i) + fCosPhi*(nHeight-j);
						nY = (float)nY/g_utility->g_fSin[(int)(((float)nX-fT)*fOmega)];
						*pxMapCur++ = nWidth - (fCosPhi*nX - fSinPhi*nY);
						*pyMapCur++ = nHeight - (fSinPhi*nX + fCosPhi*nY);
					}
				}
			}
			break;
		case 3:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*(nWidth-i) + fSinPhi*j;
						if (nX<fT+2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX>fT+fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*(nWidth-i) + fCosPhi*j;
						nY = (float)nY/g_utility->g_fSin[(int)(((float)nX-fT)*fOmega)];
						*pxMapCur++ = nWidth - (fCosPhi*nX - fSinPhi*nY);
						*pyMapCur++ = fSinPhi*nX + fCosPhi*nY;
					}
				}
			}
			break;
		}
		ippiRemap_8u_AC4R(first.Pixels, roiSize, 4*nWidth, srcROI, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);

		//второй рисунок
		pxMapCur = pxMap;
		pyMapCur = pyMap;
		switch(nDirection)
		{
		case 0:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*i + fSinPhi*(nHeight-j);
						if (nX>fT-2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX<fT-fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*i + fCosPhi*(nHeight-j);
						nY = (float)nY/g_utility->g_fSin[(int)((fT-(float)nX)*fOmega)];					
						*pxMapCur++ = fCosPhi*nX - fSinPhi*nY;
						*pyMapCur++ = nHeight- (fSinPhi*nX + fCosPhi*nY);
					}
				}
			}
			break;
		case 1:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*i + fSinPhi*j;
						if (nX>fT-2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX<fT-fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*i + fCosPhi*j;
						nY = (float)nY/g_utility->g_fSin[(int)((fT-(float)nX)*fOmega)];					
						*pxMapCur++ = fCosPhi*nX - fSinPhi*nY;
						*pyMapCur++ = fSinPhi*nX + fCosPhi*nY;
					}
				}
			}
			break;
		case 2:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*(nWidth-i) + fSinPhi*(nHeight-j);
						if (nX>fT-2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX<fT-fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*(nWidth-i) + fCosPhi*(nHeight-j);
						nY = (float)nY/g_utility->g_fSin[(int)((fT-(float)nX)*fOmega)];					
						*pxMapCur++ = nWidth - (fCosPhi*nX - fSinPhi*nY);
						*pyMapCur++ = nHeight- (fSinPhi*nX + fCosPhi*nY);
					}
				}
			}
			break;
		case 3:
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						nX = fCosPhi*(nWidth-i) + fSinPhi*j;
						if (nX>fT-2)
						{
							*pxMapCur++ = -1;
							*pyMapCur++ = -1;
							continue;
						}
						if (nX<fT-fRad/2.0)
						{
							*pxMapCur++ = i;
							*pyMapCur++ = j;
							continue;
						}
						nY = -fSinPhi*(nWidth-i) + fCosPhi*j;
						nY = (float)nY/g_utility->g_fSin[(int)((fT-(float)nX)*fOmega)];					
						*pxMapCur++ = nWidth - (fCosPhi*nX - fSinPhi*nY);
						*pyMapCur++ = fSinPhi*nX + fCosPhi*nY;
					}
				}
			}
			break;
		}
		ippiRemap_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcROI, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);

	}
	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	
	return TRUE;
}

BOOL IPPRipple(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bIsOut)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;

	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];


	if (pxMap && pyMap && pBuffer)
	{
		int nRippleStep=1;
		for (;nRippleStep<c_nRippleStepCount;nRippleStep++)
		{
			if (dFrame<c_dRippleStepBeginFrame[nRippleStep])
				break;
		}

		if (c_nRippleStepCount <= nRippleStep)
			nRippleStep = c_nRippleStepCount - 1;
		else if (1 >= nRippleStep)
			nRippleStep = 1;


		switch (nRippleStep)
		{
		case 1:
			memcpy(pBuffer,first.Pixels,4*nWidth*nHeight);
			break;
		case 2:
			{
				Ipp8u alpha = (dFrame - c_dRippleStepBeginFrame[1])/(c_dRippleStepBeginFrame[2] - c_dRippleStepBeginFrame[1]) * 255;
				ippiAlphaCompC_8u_AC4R( 
					last.Pixels, 4*nWidth, alpha,
					first.Pixels, 4*nWidth, 255, 
					pBuffer, 4*nWidth, roiSize, ippAlphaOver);
			}
			break;
		case 3:
			memcpy(pBuffer,last.Pixels,4*nWidth*nHeight);
			break;
		}

		Ipp32f* pxMapCur = pxMap;
		Ipp32f* pyMapCur = pyMap;

		int nHalfWidth = nWidth/2;
		int nHalfHeight = nHeight/2;
		int nVMax = nHeight - nHalfHeight;
		int nHMax = nWidth - nHalfWidth;
		double dHypot = _hypot(nWidth, nHeight);

		float fA;
		if (dFrame>0.5)
			fA = (1.0 - dFrame)*dHypot; // 500
		else
			fA = dFrame*dHypot; // 500

		float fOmega = 15.0*M_PI/dHypot;
		float fOffset = 2.0*M_PI*dFrame;
		if (!bIsOut)
			fOffset = -fOffset;
		float fTemp;
		for (int j=-nHalfHeight;j<nVMax;j++)
		{
			for (int i=-nHalfWidth;i<nHMax;i++)
			{
				fTemp = sin(fOmega*(sqrt((float)(i*i+j*j)))-fOffset);
				*pxMapCur++ = nHalfWidth + i + fA*(fTemp - sin(fOmega*(sqrt((float)((i-1)*(i-1)+j*j)))-fOffset));
				*pyMapCur++ = nHalfHeight + j + fA*(fTemp - sin(fOmega*(sqrt((float)(i*i+(j-1)*(j-1))))-fOffset));
			}
		}

		Ipp8u backColor[4] = { 0,0,0,255};
		ippiSet_8u_C4R(backColor, result.Pixels, 4*nWidth, roiSize);
		ippiRemap_8u_AC4R(pBuffer, roiSize, 4*nWidth, srcROI, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
	}


	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	if (pBuffer)
		delete []pBuffer;
	
	return TRUE;
}
BOOL IPPWave(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bIsHorizontal)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;

	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];


	if (pxMap && pyMap && pBuffer)
	{
		int nWaveStep=1;
		for (;nWaveStep<c_nWaveStepCount;nWaveStep++)
		{
			if (dFrame<c_dWaveStepBeginFrame[nWaveStep])
				break;
		}

		if (c_nWaveStepCount <= nWaveStep)
			nWaveStep = c_nWaveStepCount - 1;
		else if (1 >= nWaveStep)
			nWaveStep = 1;


		switch (nWaveStep)
		{
		case 1:
			memcpy(pBuffer,first.Pixels,4*nWidth*nHeight);
			break;
		case 2:
			{
				Ipp8u alpha = (dFrame - c_dWaveStepBeginFrame[1])/(c_dWaveStepBeginFrame[2] - c_dRippleStepBeginFrame[1]) * 255;
				ippiAlphaCompC_8u_AC4R( 
					last.Pixels, 4*nWidth, alpha,
					first.Pixels, 4*nWidth, 255, 
					pBuffer, 4*nWidth, roiSize, ippAlphaOver);
			}
			break;
		case 3:
			memcpy(pBuffer,last.Pixels,4*nWidth*nHeight);
			break;
		}

		Ipp32f* pxMapCur = pxMap;
		Ipp32f* pyMapCur = pyMap;

		float fOffset = 6*M_PI*dFrame;
		if (bIsHorizontal)
		{
			float fA;
			if (dFrame>0.5)
				fA = (1.0 - dFrame)*nHeight/10;
			else
				fA = dFrame*nHeight/10;

			float fOmega = 10*M_PI/nWidth;
			int nHalfWidth = nWidth/2;

			Ipp32f* pHelp = new Ipp32f[nWidth];
			for (int i=0;i<nWidth;i++)
				pHelp[i] = fA*sin(fOffset + fOmega*(i-nHalfWidth));


			if (pHelp)
			{
				for (int j=0;j<nHeight;j++)
				{
					for (int i=0;i<nWidth;i++)
					{
						*pxMapCur++ = i;
						*pyMapCur++ = j + pHelp[i];
					}
				}
				delete [] pHelp;
			}
		}
		else
		{
			float fA;
			if (dFrame>0.5)
				fA = (1.0 - dFrame)*nWidth/10;
			else
				fA = dFrame*nWidth/10;

			float fOmega = 10*M_PI/nHeight;
			int nHalfHeight = nHeight/2;
			int nVMax = nHeight - nHalfHeight;
			float fTemp;
			for (int j=-nHalfHeight;j<nVMax;j++)
			{
				fTemp = fA*sin(fOffset + fOmega*j);
				for (int i=0;i<nWidth;i++)
				{
					*pxMapCur++ = i + fTemp;
					*pyMapCur++ = j + nHalfHeight;
				}
			}
		}
		Ipp8u backColor[4] = { 0,0,0,255};
		ippiSet_8u_C4R(backColor, result.Pixels, 4*nWidth, roiSize);
		ippiRemap_8u_AC4R(pBuffer, roiSize, 4*nWidth, srcROI, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
	}


	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	if (pBuffer)
		delete []pBuffer;
	
	return TRUE;
}

BOOL IPPFadeZoom(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bZoomIn, BOOL bZoomOut)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	
	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];

	int nHalfWidth = nWidth/2;
	int nHalfHeight = nHeight/2;
	int nVMax = nHeight - nHalfHeight;
	int nHMax = nWidth - nHalfWidth;


	
	if (pxMap && pyMap && pBuffer)
	{
		Ipp8u* pFirst = NULL;
		Ipp8u* pSecond = NULL;

		if (bZoomIn)
		{
			float fZoomFirst = dFrame*6.0 + 1.0;

			Ipp32f* pxMapCur = pxMap;
			Ipp32f* pyMapCur = pyMap;
			for (int j=-nHalfHeight;j<nVMax;j++)
			{
				for (int i=-nHalfWidth;i<nHMax;i++)
				{
					*pxMapCur++ = nHalfWidth + i/fZoomFirst;
					*pyMapCur++ = nHalfHeight + j/fZoomFirst;
				}
			}

			IppiRect srcROI;
			srcROI.x = 0;
			srcROI.y = 0;
			srcROI.width = nWidth;
			srcROI.height = nHeight;

			ippiRemap_8u_AC4R(first.Pixels, roiSize, 4*nWidth, srcROI, 
				pxMap, sizeof(Ipp32f)*nWidth, 
				pyMap, sizeof(Ipp32f)*nWidth, 
				pBuffer, 4*nWidth, roiSize, IPPI_INTER_LINEAR);	        

			pFirst = pBuffer;
		}
		else
			pFirst = first.Pixels;

		if (bZoomOut)
		{
			float fZoomSecond = (1.0 - dFrame)*6.0 + 1.0;
	
			Ipp32f* pxMapCur = pxMap;
			Ipp32f* pyMapCur = pyMap;
			for (int j=-nHalfHeight;j<nVMax;j++)
			{
				for (int i=-nHalfWidth;i<nHMax;i++)
				{
					*pxMapCur++ = nHalfWidth + i/fZoomSecond;
					*pyMapCur++ = nHalfHeight + j/fZoomSecond;
				}
			}

			IppiRect srcROI;
			srcROI.x = 0;
			srcROI.y = 0;
			srcROI.width = nWidth;
			srcROI.height = nHeight;

			ippiRemap_8u_AC4R(last.Pixels, roiSize, 4*nWidth, srcROI, 
				pxMap, sizeof(Ipp32f)*nWidth, 
				pyMap, sizeof(Ipp32f)*nWidth, 
				result.Pixels, 4*nWidth, roiSize, IPPI_INTER_LINEAR);


			//srcScaleROI.width = (int)((float)nWidth/dZoomSecond);
			//if (0<nWidth - (int)(dZoomSecond*srcScaleROI.width))
			//	srcScaleROI.width ++;
			//srcScaleROI.height = (int)((float)(nHeight)/dZoomSecond);
			//if (0<nHeight - (int)(dZoomSecond*srcScaleROI.height))
			//	srcScaleROI.height ++;

			//srcScaleROI.x = (nWidth - srcScaleROI.width)/2;
			//srcScaleROI.y = (nHeight - srcScaleROI.height)/2;

			//ippiResize_8u_AC4R(
			//	last.Pixels, roiSize, 4*nWidth, srcScaleROI,
			//	result.Pixels, 4*nWidth, roiSize,
			//	dZoomSecond, dZoomSecond,IPPI_INTER_LINEAR);

			//ippiResize_8u_AC4R(
			//	last.Pixels + 4*nWidth*nHalfHeight, roiSize, 4*nWidth, srcScaleROI,
			//	result.Pixels + 4*nWidth*nHalfHeight, 4*nWidth, roiSize,
			//	dZoomSecond, dZoomSecond,IPPI_INTER_LINEAR);

			//IppiRect srcROI;
			//srcROI.x = 0;
			//srcROI.y = 0;
			//srcROI.width = nWidth/2;
			//srcROI.height = nHeight/2;
			//double xFr,yFr;
			//ippiGetResizeFract(roiSize, srcScaleROI,
			//dZoomSecond, dZoomSecond, &xFr, &yFr,
			//IPPI_INTER_LINEAR);

			//ippiResizeShift_8u_AC4R(
			//	last.Pixels, roiSize, 4*nWidth, srcScaleROI,
			//	result.Pixels, 4*nWidth, roiSize,
			//	xFr, yFr, 0.0 ,0.0,
			//	IPPI_INTER_LINEAR);


			pSecond = result.Pixels;
		}
		else
			pSecond = last.Pixels;

		Ipp8u alpha = dFrame*255;
		ippiAlphaCompC_8u_AC4R( 
			pSecond, 4*nWidth, alpha,
			pFirst, 4*nWidth, 255, 
			result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
	}
	if (pyMap)
		delete []pyMap;
	if (pxMap)
		delete []pxMap;
	if (pBuffer)
		delete []pBuffer;
	
	return TRUE;

}



BOOL IPPBurning(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, Utility* g_utility)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	g_utility->GenerateBurningMask(nWidth,nHeight);
	int nFrame = dFrame*256.0;
	Ipp8u* pBuffer = new Ipp8u[nWidth*nHeight];

	if (pBuffer)
	{
		ippiCompareC_8u_C1R(g_utility->g_pBurningMask, nWidth, nFrame,
			pBuffer, nWidth, roiSize, ippCmpGreaterEq);

		ippiCopy_8u_AC4MR(first.Pixels, 4*nWidth, 
			result.Pixels, 4*nWidth, roiSize,
			pBuffer, nWidth);

		ippiNot_8u_C1IR(pBuffer, nWidth, roiSize);
		
		ippiCopy_8u_AC4MR(last.Pixels, 4*nWidth, 
			result.Pixels, 4*nWidth, roiSize,
			pBuffer, nWidth);
		
		delete [] pBuffer;
	}
	return TRUE;
}

BOOL IPPBurningFade(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, Utility* g_utility)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	g_utility->GenerateBurningMask(nWidth,nHeight);
	int nFrame = dFrame*256.0;
	BYTE nLTTreshold = nFrame;
	BYTE nGTTreshold = min(nFrame + 20,255);

	Ipp8u* pBuffer = new Ipp8u[nWidth*nHeight];
	if (pBuffer)
	{
		memcpy(pBuffer,g_utility->g_pBurningMask,nWidth*nHeight);

		ippiThreshold_LTValGTVal_8u_C1IR(pBuffer, nWidth, roiSize, 
			nLTTreshold, 0, nGTTreshold, 255);

		ippiCopy_8u_C1C4R(pBuffer, nWidth, first.Pixels+3, 4*nWidth, roiSize);
		
		ippiNot_8u_C1IR(pBuffer, nWidth, roiSize);
		ippiCopy_8u_C1C4R(pBuffer, nWidth, last.Pixels+3, 4*nWidth, roiSize);

		ippiAlphaComp_8u_AC4R(first.Pixels, 4*nWidth, last.Pixels, 4*nWidth,
			result.Pixels, 4*nWidth, roiSize, ippAlphaPlus);
		delete [] pBuffer;
	}
	return TRUE;
}

BOOL IPPBurningFire(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, Utility* g_utility)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	g_utility->GenerateBurningMask(nWidth,nHeight);
	int nFrame = dFrame*256.0;
	BYTE nLTTreshold = nFrame;
	BYTE nGTTreshold = min(nFrame + 30,255);

	Ipp8u* pBuffer = new Ipp8u[nWidth*nHeight];
	Ipp8u* pBuffer1 = new Ipp8u[nWidth*nHeight];
	Ipp32s *pLevels = new Ipp32s[257];
	Ipp32s *pValues = new Ipp32s[max(1, nGTTreshold - nLTTreshold + 1)];
	if (pBuffer && pBuffer1 && pLevels && pValues)
	{
		memcpy(pBuffer,g_utility->g_pBurningMask,nWidth*nHeight);

		ippiThreshold_LTValGTVal_8u_C1IR(pBuffer, nWidth, roiSize, 
			nLTTreshold, 0, nGTTreshold, 0);


		for (int i=nLTTreshold;i<nGTTreshold+1;i++)
		{
			pLevels[i-nLTTreshold] = i;
			if (i>(nLTTreshold+nGTTreshold)/2)
				pValues[i-nLTTreshold] = (nGTTreshold-i+1)*510/(nGTTreshold - nLTTreshold);
			else
				pValues[i-nLTTreshold] = (i-nLTTreshold)*510/(nGTTreshold - nLTTreshold);
		}

		ippiLUT_8u_C1R(pBuffer, nWidth, pBuffer, nWidth, roiSize,
			pValues, pLevels, nGTTreshold - nLTTreshold+1);
		
		for (int i=0;i<257;i++)
			pLevels[i] = i;

		ippiLUT_8u_C1R(pBuffer, nWidth, pBuffer1, nWidth, roiSize,
						g_utility->g_sBurningPalette, pLevels, 256);
		ippiCopy_8u_C1C4R(pBuffer1, nWidth, result.Pixels+2, 4*nWidth, roiSize);
		
		ippiLUT_8u_C1R(pBuffer, nWidth, pBuffer1, nWidth, roiSize,
						g_utility->g_sBurningPalette+256, pLevels, 256);
		ippiCopy_8u_C1C4R(pBuffer1, nWidth, result.Pixels+1, 4*nWidth, roiSize);
		
		ippiLUT_8u_C1R(pBuffer, nWidth, pBuffer1, nWidth, roiSize,
						g_utility->g_sBurningPalette+512, pLevels, 256);
		ippiCopy_8u_C1C4R(pBuffer1, nWidth, result.Pixels, 4*nWidth, roiSize);

		//нарисовали огонь - копируем исходные картинки
		ippiCompareC_8u_C1R(g_utility->g_pBurningMask, nWidth, nLTTreshold,
			pBuffer, nWidth, roiSize, ippCmpLessEq);

		ippiCopy_8u_AC4MR(last.Pixels, 4*nWidth, 
			result.Pixels, 4*nWidth, roiSize,
			pBuffer, nWidth);

		ippiCompareC_8u_C1R(g_utility->g_pBurningMask, nWidth, nGTTreshold,
			pBuffer, nWidth, roiSize, ippCmpGreaterEq);
		
		ippiCopy_8u_AC4MR(first.Pixels, 4*nWidth, 
			result.Pixels, 4*nWidth, roiSize,
			pBuffer, nWidth);
	}
	if (pLevels)
        delete []pLevels;
	if (pValues)
        delete []pValues;
	if (pBuffer1)
		delete [] pBuffer1;
	if (pBuffer)
		delete [] pBuffer;
	return TRUE;
}

BOOL IPPFlipAwayOutInVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
                         int nSizeX, int nSizeY, double dFrame, BOOL bForward )
{
    int nWidth = first.Width;
    int nHeight = first.Height;
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

    IppiSize roiSize = {nSizeX,nSizeY};

    int		 nIppStep = 4*nWidth;
    int		 nTileHStep = 4*nSizeX;
    int		 nTileVStep = nIppStep*nSizeY;
    int      nHTileCount = nWidth / nSizeX;
    int      nVTileCount = nHeight / nSizeY;
    Ipp8u    *pA = NULL;
    Ipp8u    *pB = NULL;
    double   dKoef = 5;
    int      nResizedWidth, nResizedHeight;
        
	if ( dFrame > 0.5 )
    {
        pA = last.Pixels;
        dFrame = 2 - 2*dFrame;
		bForward = (bForward != TRUE) ? TRUE : FALSE;
    }
    else
    {
        pA = first.Pixels;
		dFrame *= 2;
    }

    int nParts = (int)(dFrame/0.2);

    dKoef *= (( nParts%2 != 0 )? (dFrame - nParts*0.2) : ((nParts+1)*0.2 - dFrame));

    IppiRect srcRect = {0,0,nSizeX,nSizeY};

    IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
    IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

    if ( dFrame <= 0.2 )
    {
        nResizedWidth = (int)((nSizeX-1)*(1-dFrame/2));
        nResizedHeight = (int)((nSizeY-1)*(1-dFrame/2));
    }
    else if ( dFrame > 0.2 && dFrame <= 0.4 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.9-(dFrame-0.2)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.9-(dFrame-0.2)*0.35/0.2));
    }
    else if ( dFrame > 0.4 && dFrame <= 0.6 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.55-(dFrame-0.4)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.55-(dFrame-0.4)/2));
    }
    else if ( dFrame > 0.6 && dFrame <= 0.8 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.45-(dFrame-0.6)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.45-(dFrame-0.6)*0.35/0.2));
    }
    else 
    {
        nResizedWidth = (int)((nSizeX-1)*(0.1-(dFrame-0.8)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.1-(dFrame-0.8)/2));
    }

    double	quadDst[4][2];
	double  coeffs[2][4] = {0,0,0,0,0,0,0,0};

	int nLevelX, nLevelY;
	nLevelY = (int)((nResizedHeight - 1)*(dKoef * 0.7 + 0.3));
	nLevelX = (int)((nResizedWidth - 1)*dKoef);
		
	quadDst[0][0] = quadDst[3][0] = (nSizeX - 1 - nLevelX)/2;
    quadDst[1][0] = quadDst[2][0] = nSizeX - 1 - quadDst[0][0];
 
    if ( bForward && (( dFrame > 0.2 && dFrame <= 0.4 || dFrame > 0.6 && dFrame <= 0.8 )) || 
         !bForward && ( dFrame <= 0.2 || dFrame > 0.4 && dFrame <= 0.6 || dFrame > 0.8 ) )
    {
        quadDst[0][1] = (nSizeY - 1 - nResizedHeight)/2;
        quadDst[3][1] =  nSizeY - 1 - quadDst[0][1];
        quadDst[1][1] = (nSizeY - 1 - nLevelY)/2;
        quadDst[2][1] = nSizeY - 1 - quadDst[1][1];
    }
    else
    {
        quadDst[1][1] = (nSizeY - 1 - nResizedHeight)/2;
        quadDst[2][1] =  nSizeY - 1 - quadDst[1][1];
        quadDst[0][1] = (nSizeY - 1 - nLevelY)/2;
        quadDst[3][1] = nSizeY - 1 - quadDst[0][1];
    }

    ippiGetBilinearTransform(srcRect, quadDst, coeffs);

    Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

    memset(pResult,0,nIppStep*nHeight);

	int j = 0;

    for (j=0;j<nVTileCount;j++)
    {
        pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
                nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
                nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
            roiSize.width = nSizeX;
        }
    }

    if (0!=srcRectBottomBorder.height)
    {
        pResult = result.Pixels + nTileVStep*j;
		pSource = pA + nTileVStep*j;
        roiSize.height = srcRectBottomBorder.height;

        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
                srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
                nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
        }
    }

    return TRUE;
}

BOOL IPPFlipAwayOutInHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
                           int nSizeX, int nSizeY,	double dFrame, BOOL bForward )
{
	int nWidth = first.Width;
    int nHeight = first.Height;
    
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;
    
    IppiSize roiSize = {nSizeX,nSizeY};

    int		 nIppStep = 4*nWidth;
    int		 nTileHStep = 4*nSizeX;
    int		 nTileVStep = nIppStep*nSizeY;
    int      nHTileCount = nWidth / nSizeX;
    int      nVTileCount = nHeight / nSizeY;
    Ipp8u    *pA = NULL;
    Ipp8u    *pB = NULL;
    double   dKoef = 5;
    int      nResizedWidth, nResizedHeight;
 
    if ( dFrame > 0.5 )
    {
        pA = last.Pixels;
        dFrame = 2 - 2*dFrame;
		bForward = (bForward != TRUE) ? TRUE : FALSE;
    }
    else
    {
        pA = first.Pixels;
		dFrame *= 2;
    }

    int nParts = (int)(dFrame/0.2);

    dKoef *= (( nParts%2 != 0 )? (dFrame - nParts*0.2) : ((nParts+1)*0.2 - dFrame));

    IppiRect srcRect = {0,0,nSizeX,nSizeY};

    IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
    IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

    if ( dFrame <= 0.2 )
    {
        nResizedWidth = (int)((nSizeX-1)*(1-dFrame/2));
        nResizedHeight = (int)((nSizeY-1)*(1-dFrame/2));
    }
    else if ( dFrame > 0.2 && dFrame <= 0.4 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.9-(dFrame-0.2)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.9-(dFrame-0.2)*0.35/0.2));
    }
    else if ( dFrame > 0.4 && dFrame <= 0.6 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.55-(dFrame-0.4)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.55-(dFrame-0.4)/2));
    }
    else if ( dFrame > 0.6 && dFrame <= 0.8 )
    {
        nResizedWidth = (int)((nSizeX-1)*(0.45-(dFrame-0.6)*0.35/0.2));
        nResizedHeight = (int)((nSizeY-1)*(0.45-(dFrame-0.6)*0.35/0.2));
    }
    else 
    {
        nResizedWidth = (int)((nSizeX-1)*(0.1-(dFrame-0.8)/2));
        nResizedHeight = (int)((nSizeY-1)*(0.1-(dFrame-0.8)/2));
    }

    double	quadDst[4][2];
    double  coeffs[2][4] = {0,0,0,0,0,0,0,0};

	int nLevelX, nLevelY;
	nLevelY = (int)((nResizedHeight - 1)*dKoef);
	nLevelX = (int)((nResizedWidth - 1)*(dKoef * 0.7 + 0.3));
		
	quadDst[0][1] = quadDst[1][1] = (nSizeY - 1 - nLevelY)/2;
    quadDst[2][1] = quadDst[3][1] = nSizeY - 1 - quadDst[0][1];

    if ( bForward && (( dFrame > 0.2 && dFrame <= 0.4 || dFrame > 0.6 && dFrame <= 0.8 )) || 
         !bForward && ( dFrame <= 0.2 || dFrame > 0.4 && dFrame <= 0.6 || dFrame > 0.8 ) )
    {
        quadDst[0][0] = (nSizeX - 1 - nResizedWidth)/2;
        quadDst[1][0] =  nSizeX - 1 - quadDst[0][0];
        quadDst[3][0] = (nSizeX - 1 - nLevelX)/2;
        quadDst[2][0] = nSizeX - 1 - quadDst[3][0];
	}
    else
    {
        quadDst[3][0] = (nSizeX - 1 - nResizedWidth)/2;
        quadDst[2][0] =  nSizeX - 1 - quadDst[3][0];
        quadDst[0][0] = (nSizeX - 1 - nLevelX)/2;
        quadDst[1][0] = nSizeX - 1 - quadDst[0][0];
	}

    ippiGetBilinearTransform(srcRect, quadDst, coeffs);

    Ipp8u *pResult = result.Pixels;
	Ipp8u *pSource = pA;

    memset(pResult,0,nIppStep*nHeight);

	int j = 0;

    for (j=0;j<nVTileCount;j++)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = pA + nTileVStep*j;
        for (int i=0;i<nHTileCount;i++)
        {
            IppStatus ippst = ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRect, pResult,
                nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );

            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
                nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
            roiSize.width = nSizeX;
        }
    }
    if (0!=srcRectBottomBorder.height)
    {
        pResult = result.Pixels + nTileVStep*j;
        pSource = (Ipp8u*)pA + nTileVStep*j;
        roiSize.height = srcRectBottomBorder.height;

        for (int i=0;i<nHTileCount;i++)
        {
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
                srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
            pResult += nTileHStep;
            pSource += nTileHStep;
        }
        if (0!=srcRectRightBorder.width)
        {
            roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
            ippiWarpBilinear_8u_AC4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
                nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
        }
    }
    return TRUE;
}

BOOL IPPFoldVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
                           int nSizeX, int nSizeY,	double dFrame, BOOL bTop )
{
    int nWidth = first.Width;
    int nHeight = first.Height;
    
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	int      nHTileCount = nHeight / nSizeY;
	int		 nBottomBorder = nHeight % nSizeY;
	if ( nBottomBorder > 1 )
		nHTileCount++;
	IppiSize roiTileSize = {nWidth,nSizeY};
	IppiSize roiSize = {nWidth,nHeight};
	IppiSize roiResizedSize = {nWidth,nSizeY};
    
	int		 nIppStep = 4*nWidth;
    int		 nTileHStep = max(1, nIppStep*nSizeY);
    double	 dFrameStep = 1.0/(2*nHTileCount - 1);
	int		 nParts = (int)(dFrame/dFrameStep);        
	double	 dKoef;
	IppiRect roiSrcRect = {0,0,nWidth,nSizeY};
	Ipp8u *  pDst = result.Pixels;
	int		 nDstShift = 0;

 	if ( nParts%2 != 0 )
	{
		dKoef = (dFrame - nParts*dFrameStep)/dFrameStep;
		roiResizedSize.height = (int)(dKoef * roiResizedSize.height);
	}
	else
	{
		dKoef = ((nParts+1)*dFrameStep - dFrame)/dFrameStep;
		roiResizedSize.height = (int)(dKoef * roiResizedSize.height);
		nDstShift = (nSizeY - roiResizedSize.height)*nIppStep;
	}
	if ( bTop )
	{
		if ( nParts == 0 )
		{
			memcpy(result.Pixels, last.Pixels, nTileHStep);	
			memcpy(result.Pixels+nTileHStep, first.Pixels+nTileHStep, nIppStep*nHeight - nTileHStep);
		}
		else if ( nParts == 2*(nHTileCount-1) || nParts == 2*(nHTileCount-1)-1)
		{
			if ( nParts == 2*(nHTileCount-1))
				memcpy(result.Pixels, last.Pixels, nIppStep*nHeight);
			else
			{
                if ( nBottomBorder>1 )
				{
					int nBytes = nIppStep*nHeight - nBottomBorder*nIppStep;
					memcpy(result.Pixels, last.Pixels, nBytes);
					memcpy(result.Pixels+nBytes, first.Pixels+nBytes, nIppStep*nHeight - nBytes );
				}
				else
				{	
					int nBytes = nIppStep*nHeight - nTileHStep;
					memcpy(result.Pixels, last.Pixels, nBytes);
					memcpy(result.Pixels+nBytes, first.Pixels+nBytes, nTileHStep );
				}
			}
			roiSrcRect.y = nSizeY * (nHTileCount-2);
			pDst += (roiSrcRect.y + nSizeY)*nIppStep;
			if ( nBottomBorder > 1 )
			{
                if ( nParts == 2*(nHTileCount-1)-1 )
                {
                    Ipp8u * pTmp = new	Ipp8u[ nTileHStep ];	
                    ippiResize_8u_AC4R(first.Pixels, roiSize, nIppStep, roiSrcRect, pTmp, nIppStep, roiTileSize,
                        1, dKoef, IPPI_INTER_LINEAR);
                    roiTileSize.height = roiResizedSize.height;
                    ippiMirror_8u_AC4IR( pTmp, nIppStep, roiTileSize, ippAxsHorizontal );
                    roiTileSize.height = ( roiResizedSize.height < nBottomBorder ) ? roiResizedSize.height : nBottomBorder;
                    ippiCopy_8u_AC4R( pTmp, nIppStep, pDst, nIppStep, roiTileSize );
                    delete [] pTmp;
                    return TRUE;
                }
                else
                {
                    roiSrcRect.height = roiTileSize.height = nBottomBorder;
				    roiSrcRect.y += (nSizeY-nBottomBorder);
				    roiResizedSize.height = (int)(dKoef * nBottomBorder);
				    nDstShift = (nBottomBorder - roiResizedSize.height)*nIppStep;
                }
			}
		}
		else for ( int i=1; i<nHTileCount-1; i++ )
			{
				if ( nParts == 2*i || nParts == 2*i-1 )
				{
					int nBytes;
					if ( nParts == 2*i )
						nBytes = nTileHStep*(i+1);
					else
						nBytes = nTileHStep*i;
					memcpy(result.Pixels, last.Pixels, nBytes);
					memcpy(result.Pixels+nBytes, first.Pixels+nBytes, nIppStep*nHeight - nBytes);
					roiSrcRect.y = nSizeY * (i-1);
					pDst += (roiSrcRect.y + nSizeY)*nIppStep;
					break;
				}
			}
	}
	else
	{
		nDstShift = (nSizeY - roiResizedSize.height)*nIppStep - nDstShift;
		if ( nParts == 0 )
		{
			int nBytes = nIppStep*nHeight - nTileHStep;
			memcpy(result.Pixels, first.Pixels, nBytes);
			memcpy(result.Pixels + nBytes, last.Pixels + nBytes, nTileHStep);	
			roiSrcRect.y = nHeight - nSizeY;
			pDst += roiSrcRect.y * nIppStep;
		}
		else if ( nParts == 2*(nHTileCount-1) || nParts == 2*(nHTileCount-1)-1)
		{
			if ( nParts == 2*(nHTileCount-1))
				memcpy(result.Pixels, last.Pixels, nIppStep*nHeight);
			else
			{
				if ( nBottomBorder>1 )
				{
					int nBytes = nBottomBorder*nIppStep;
					memcpy(result.Pixels, first.Pixels, nBytes);
					memcpy(result.Pixels+nBytes, last.Pixels+nBytes, nIppStep*nHeight - nBytes );
				}
				else
				{	
					int nBytes = nTileHStep;
					memcpy(result.Pixels, first.Pixels, nTileHStep);
					memcpy(result.Pixels+nTileHStep, last.Pixels+nTileHStep, nIppStep*nHeight - nTileHStep );
				}
			}
			roiSrcRect.y = nHeight - nSizeY*(nHTileCount-1);
			if ( nBottomBorder > 1 )
			{
                if ( nParts == 2*(nHTileCount-1)-1 )
                {
                    Ipp8u * pTmp = new	Ipp8u[ nTileHStep ];	
                    int nTmpShift;
                    ippiResize_8u_AC4R(first.Pixels, roiSize, nIppStep, roiSrcRect, pTmp, nIppStep, roiTileSize,
                        1, dKoef, IPPI_INTER_LINEAR);
                    roiTileSize.height = roiResizedSize.height;
                    ippiMirror_8u_AC4IR( pTmp, nIppStep, roiTileSize, ippAxsHorizontal );
                    if ( roiResizedSize.height < nBottomBorder )
                    {
                        roiTileSize.height = roiResizedSize.height;
                        nDstShift = (nBottomBorder - roiTileSize.height)*nIppStep;
                        nTmpShift = 0;
                    }
                    else
                    {
                        roiTileSize.height = nBottomBorder;
                        nDstShift = 0;
                        nTmpShift = (roiResizedSize.height - nBottomBorder)*nIppStep;
                    }
                    ippiCopy_8u_AC4R( pTmp+nTmpShift, nIppStep, pDst+nDstShift, nIppStep, roiTileSize );
                    delete [] pTmp;
                    return TRUE;
                }
                else				
                {
                    roiSrcRect.height = roiTileSize.height = nBottomBorder;
				    roiResizedSize.height = (int)(dKoef * nBottomBorder);
                }
			}
		}
		else for ( int i=1; i<nHTileCount-1; i++ )
			{
				if ( nParts == 2*i || nParts == 2*i-1 )
				{
					int nBytes;
					if ( nParts == 2*i )
						nBytes = nIppStep*nHeight - nTileHStep*(i+1);
					else
						nBytes = nIppStep*nHeight - nTileHStep*i;
					memcpy(result.Pixels+nBytes, last.Pixels+nBytes, nIppStep*nHeight - nBytes);
					memcpy(result.Pixels, first.Pixels, nBytes);
					roiSrcRect.y = nHeight - nSizeY*i;
					pDst += (roiSrcRect.y - nSizeY)*nIppStep;
					break;
				}
			}
	}
	
    ippiResize_8u_AC4R(first.Pixels, roiSize, nIppStep, roiSrcRect, pDst+nDstShift, nIppStep, roiResizedSize,
    					1, dKoef, IPPI_INTER_LINEAR);
    if ( nParts != 0 )
		ippiMirror_8u_AC4IR( pDst + nDstShift, nIppStep, roiResizedSize, ippAxsHorizontal );

	return TRUE;
}



BOOL IPPFoldHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
                           int nSizeX, int nSizeY,	double dFrame, BOOL bLeft )
{
    int nWidth = first.Width;
    int nHeight = first.Height;
    
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	int      nWTileCount = nWidth / nSizeX;
	int		 nRightBorder = nWidth % nSizeX;
	if ( nRightBorder > 1 )
		nWTileCount++;
	IppiSize roiTileSize = {nSizeX,nHeight};
	IppiSize roiSize = {nWidth,nHeight};
	IppiSize roiResizedSize = {nSizeX,nHeight};
    
	int		 nIppStep = 4*nWidth;
    int		 nTileWStep = 4*nSizeX;
    double	 dFrameStep = 1.0/(2*nWTileCount - 1);
	int		 nParts = (int)(dFrame/dFrameStep);        
	double	 dKoef;
	IppiRect roiSrcRect = {0,0,nSizeX,nHeight};
	Ipp8u *  pDst = result.Pixels;
	int		 nDstShift = 0;

	if ( nParts%2 != 0 )
	{
		dKoef = (dFrame - nParts*dFrameStep)/dFrameStep;
		roiResizedSize.width = (int)(dKoef * roiResizedSize.width);
	}
	else
	{
		dKoef = ((nParts+1)*dFrameStep - dFrame)/dFrameStep;
		roiResizedSize.width = (int)(dKoef * roiResizedSize.width);
		nDstShift = (nSizeX - roiResizedSize.width)*4;
	}
	if ( bLeft )
	{
		if ( nParts == 0 )
		{
			ippiCopy_8u_AC4R( last.Pixels, nIppStep, result.Pixels, nIppStep, roiTileSize );
			roiSize.width -= nSizeX;
			ippiCopy_8u_AC4R( first.Pixels + nTileWStep, nIppStep, result.Pixels + nTileWStep, nIppStep, roiSize );
			roiSize.width += nSizeX;
		}
		else if ( nParts == 2*(nWTileCount-1) || nParts == 2*(nWTileCount-1)-1)
		{
			if ( nParts == 2*(nWTileCount-1))
				memcpy(result.Pixels, last.Pixels, nIppStep*nHeight);
			else
			{
				if ( nRightBorder>1 )
				{
                    int nBytes = (nWidth - nRightBorder)*4;
					roiSize.width = (nWidth - nRightBorder);
					ippiCopy_8u_AC4R( last.Pixels, nIppStep, result.Pixels, nIppStep, roiSize );				
					roiSize.width = nRightBorder;
					ippiCopy_8u_AC4R( first.Pixels + nBytes, nIppStep, result.Pixels + nBytes, nIppStep, roiSize );
					roiSize.width = nWidth;
				}
				else
				{	
					roiSize.width = (nWidth - nSizeX);
					int nBytes = roiSize.width*4;
					ippiCopy_8u_AC4R( last.Pixels, nIppStep, result.Pixels, nIppStep, roiSize );				
					ippiCopy_8u_AC4R( first.Pixels + nBytes, nIppStep, result.Pixels + nBytes, nIppStep, roiTileSize );
					roiSize.width = nWidth;
				}
			}
			roiSrcRect.x = nSizeX * (nWTileCount-2);
			pDst += (roiSrcRect.x + nSizeX)*4;
			if ( nRightBorder > 1 )
			{
                if ( nParts == 2*(nWTileCount-1)-1 )
                {
                    Ipp8u * pTmp = new	Ipp8u[ nSizeX*nHeight*4 ];	
                    ippiResize_8u_AC4R(first.Pixels, roiSize, nIppStep, roiSrcRect, pTmp, nTileWStep, roiTileSize,
                                        dKoef, 1, IPPI_INTER_LINEAR);
                    roiTileSize.width = roiResizedSize.width;
                    ippiMirror_8u_AC4IR( pTmp, nTileWStep, roiTileSize, ippAxsVertical );
                    roiTileSize.width = ( roiResizedSize.width < nRightBorder ) ? roiResizedSize.width : nRightBorder;
                    ippiCopy_8u_AC4R( pTmp, nTileWStep, pDst, nIppStep, roiTileSize );
                    delete [] pTmp;
                    return TRUE;
                }
                else
                {
                    roiSrcRect.width = roiTileSize.width = nRightBorder;
    				roiSrcRect.x += (nSizeX-nRightBorder);
	    			roiResizedSize.width = (int)(dKoef * nRightBorder);
		    		nDstShift = (nRightBorder - roiResizedSize.width)*4;
                }
			}
		}
		else for ( int i=1; i<nWTileCount-1; i++ )
			{
				if ( nParts == 2*i || nParts == 2*i-1 )
				{
					if ( nParts == 2*i )
						roiSize.width = nSizeX*(i+1);
					else
						roiSize.width = nSizeX*i;
					int nBytes = roiSize.width * 4;
					ippiCopy_8u_AC4R( last.Pixels, nIppStep, result.Pixels, nIppStep, roiSize );
					roiSize.width = nWidth - roiSize.width;
					ippiCopy_8u_AC4R( first.Pixels + nBytes, nIppStep, result.Pixels + nBytes, nIppStep, roiSize );
					roiSize.width = nWidth;
					roiSrcRect.x = nSizeX * (i-1);
					pDst += (roiSrcRect.x + nSizeX)*4;
					break;
				}
			}
	}
	else
	{
		nDstShift = (nSizeX - roiResizedSize.width)*4 - nDstShift;
		if ( nParts == 0 )
		{
			roiSize.width -= nSizeX;
			int nBytes = roiSize.width * 4;
			ippiCopy_8u_AC4R( first.Pixels, nIppStep, result.Pixels, nIppStep, roiSize );
			ippiCopy_8u_AC4R( last.Pixels + nBytes, nIppStep, result.Pixels + nBytes, nIppStep, roiTileSize );
			roiSize.width = nWidth;
			roiSrcRect.x = nWidth - nSizeX;
			pDst += roiSrcRect.x * 4;
		}
		else if ( nParts == 2*(nWTileCount-1) || nParts == 2*(nWTileCount-1)-1)
		{
			if ( nParts == 2*(nWTileCount-1))
				memcpy(result.Pixels, last.Pixels, nIppStep*nHeight);
			else
			{
				if ( nRightBorder>1 )
				{
					roiSize.width = nRightBorder;
					int nBytes = nRightBorder*4;
					ippiCopy_8u_AC4R( first.Pixels, nIppStep, result.Pixels, nIppStep, roiSize );				
					roiSize.width = nWidth - nRightBorder;
					ippiCopy_8u_AC4R( last.Pixels + nBytes, nIppStep, result.Pixels + nBytes, nIppStep, roiSize );
					roiSize.width = nWidth;
				}
				else
				{	
					ippiCopy_8u_AC4R( first.Pixels, nIppStep, result.Pixels, nIppStep, roiTileSize );				
					roiSize.width = (nWidth - nSizeX);
					ippiCopy_8u_AC4R( last.Pixels + nTileWStep, nIppStep, result.Pixels + nTileWStep, nIppStep, roiSize );
					roiSize.width = nWidth;
				}
			}
			roiSrcRect.x = nWidth - nSizeX*(nWTileCount-1);
			if ( nRightBorder > 1 )
			{
                if ( nParts == 2*(nWTileCount-1)-1 )
                {
                    Ipp8u * pTmp = new	Ipp8u[ nSizeX*nHeight*4 ];	
                    int nTmpShift;
                    ippiResize_8u_AC4R(first.Pixels, roiSize, nIppStep, roiSrcRect, pTmp, nTileWStep, roiTileSize,
                        dKoef, 1, IPPI_INTER_LINEAR);
                    roiTileSize.width = roiResizedSize.width;
                    ippiMirror_8u_AC4IR( pTmp, nTileWStep, roiTileSize, ippAxsVertical );
                    if ( roiResizedSize.width < nRightBorder )
                    {
                        roiTileSize.width = roiResizedSize.width;
                        nDstShift = (nRightBorder - roiTileSize.width)*4;
                        nTmpShift = 0;
                    }
                    else
                    {
                        roiTileSize.width = nRightBorder;
                        nDstShift = 0;
                        nTmpShift = (roiResizedSize.width - nRightBorder)*4;
                    }
                    ippiCopy_8u_AC4R( pTmp+nTmpShift, nTileWStep, pDst+nDstShift, nIppStep, roiTileSize );
                    delete [] pTmp;
                    return TRUE;
                }
                else
                {
                    roiSrcRect.width = roiTileSize.width = nRightBorder;
				    roiResizedSize.width = (int)(dKoef * nRightBorder);
                }
			}
		}
		else for ( int i=1; i<nWTileCount-1; i++ )
			{
				if ( nParts == 2*i || nParts == 2*i-1 )
				{
					if ( nParts == 2*i )
						roiSize.width = nWidth - nSizeX*(i+1);
					else
						roiSize.width = nWidth - nSizeX*i;
					int nBytes = roiSize.width * 4;
					ippiCopy_8u_AC4R( first.Pixels, nIppStep, result.Pixels, nIppStep, roiSize );
					roiSize.width = nWidth - roiSize.width;
					ippiCopy_8u_AC4R( last.Pixels + nBytes, nIppStep, result.Pixels + nBytes, nIppStep, roiSize );
					roiSize.width = nWidth;
					roiSrcRect.x = nWidth - nSizeX*i;
					pDst += (roiSrcRect.x - nSizeX)*4;
					break;
				}
			}
	}
    ippiResize_8u_AC4R(first.Pixels, roiSize, nIppStep, roiSrcRect, pDst+nDstShift, nIppStep, roiResizedSize,
						dKoef, 1, IPPI_INTER_LINEAR);
	if ( nParts != 0 )
		ippiMirror_8u_AC4IR( pDst + nDstShift, nIppStep, roiResizedSize, ippAxsVertical );

	return TRUE;
}

BOOL IPPGaussianBlur(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize = {nWidth,nHeight};

	int nGaussianBlurStep=1;
	for (;nGaussianBlurStep<c_nGaussianBlurStepCount;nGaussianBlurStep++)
	{
		if (dFrame<c_dGaussianBlurStepBeginFrame[nGaussianBlurStep])
			break;
	}

	if (c_nGaussianBlurStepCount <= nGaussianBlurStep)
		nGaussianBlurStep = c_nGaussianBlurStepCount - 1;
	else if (1 >= nGaussianBlurStep)
		nGaussianBlurStep = 1;


	double dFrameIn;
	switch (nGaussianBlurStep)
	{
	case 1:
		memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);
		dFrameIn = dFrame/(c_dGaussianBlurStepBeginFrame[2] - c_dGaussianBlurStepBeginFrame[0]);
		break;
	case 2:
		{
			dFrameIn = dFrame/(c_dGaussianBlurStepBeginFrame[2] - c_dGaussianBlurStepBeginFrame[0]);
			Ipp8u alpha = 0.5*(dFrame - c_dGaussianBlurStepBeginFrame[1])/(c_dGaussianBlurStepBeginFrame[2] - c_dGaussianBlurStepBeginFrame[1]) * 255;
			ippiAlphaCompC_8u_AC4R( 
				last.Pixels, 4*nWidth, alpha,
				first.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 3:
		{			
			dFrameIn = 1.0 - (dFrame - c_dGaussianBlurStepBeginFrame[2])/(c_dGaussianBlurStepBeginFrame[4] - c_dGaussianBlurStepBeginFrame[2]);
			Ipp8u alpha = 0.5*(1.0 - (dFrame - c_dGaussianBlurStepBeginFrame[2])/(c_dGaussianBlurStepBeginFrame[3] - c_dGaussianBlurStepBeginFrame[2])) * 255;
			ippiAlphaCompC_8u_AC4R( 
				first.Pixels, 4*nWidth, alpha,
				last.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 4:
		dFrameIn = 1.0 - (dFrame - c_dGaussianBlurStepBeginFrame[2])/(c_dGaussianBlurStepBeginFrame[4] - c_dGaussianBlurStepBeginFrame[2]);
		memcpy(result.Pixels,last.Pixels,4*nWidth*nHeight);
		break;
	}

	//GaussianBlur
	//IPPTransformTwirl(result.Pixels,nWidth,nHeight,dFrameIn,dAngle,nDegree,dZoom, 0);

	int nSize = dFrameIn*nWidth/10.0;
	int nCount = 2*nSize+1;
	Ipp32s* pKernel = new Ipp32s[nCount];

	IppiSize roiBufferExtVSize;
	roiBufferExtVSize.width = nWidth;
	roiBufferExtVSize.height = nHeight + 2*nSize;
	Ipp8u* pBufferExtV = new Ipp8u[4*roiBufferExtVSize.width*roiBufferExtVSize.height];

	IppiSize roiBufferExtHSize;
	roiBufferExtHSize.width	= nWidth + 2*nSize;
	roiBufferExtHSize.height = nHeight;
	Ipp8u* pBufferExtH = new Ipp8u[4*roiBufferExtHSize.width*roiBufferExtHSize.height];

	if (pKernel && pBufferExtV && pBufferExtH)
	{
		for (int i = 0; i < nCount; ++i)
			pKernel[i] = 1;

		ippiCopyReplicateBorder_8u_AC4R(result.Pixels, 4*nWidth, roiSize, pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, nSize, 0);
		ippiFilterColumn_8u_AC4R((Ipp8u*)(pBufferExtV + nSize*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, result.Pixels, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);

		ippiCopyReplicateBorder_8u_AC4R(result.Pixels, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, nSize);
		ippiFilterRow_8u_AC4R((Ipp8u*)(pBufferExtH + 4*nSize), 4*roiBufferExtHSize.width, result.Pixels, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);
		
		ippiCopyReplicateBorder_8u_AC4R(result.Pixels, 4*nWidth, roiSize,  pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, nSize, 0);
		ippiFilterColumn_8u_AC4R((Ipp8u*)(pBufferExtV + nSize*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, result.Pixels, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);
		
		ippiCopyReplicateBorder_8u_AC4R(result.Pixels, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, nSize);
		ippiFilterRow_8u_AC4R((Ipp8u*)(pBufferExtH + 4*nSize), 4*roiBufferExtHSize.width, result.Pixels, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);
	}
	
	if (pBufferExtH)
		delete[] pBufferExtH;
	if (pBufferExtV)
		delete[] pBufferExtV;
	if (pKernel)
		delete[] pKernel;

	return TRUE;
}


BOOL IPPNoise(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame)
{
	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize = {nWidth,nHeight};

	int nNoiseStep=1;
	for (;nNoiseStep<c_nNoiseStepCount;nNoiseStep++)
	{
		if (dFrame<c_dNoiseStepBeginFrame[nNoiseStep])
			break;
	}

	if (c_nNoiseStepCount <= nNoiseStep)
		nNoiseStep = c_nNoiseStepCount - 1;
	else if (1 >= nNoiseStep)
		nNoiseStep = 1;


	double dFrameIn;
	switch (nNoiseStep)
	{
	case 1:
		memcpy(result.Pixels,first.Pixels,4*nWidth*nHeight);
		dFrameIn = dFrame/(c_dNoiseStepBeginFrame[2] - c_dNoiseStepBeginFrame[0]);
		break;
	case 2:
		{
			dFrameIn = dFrame/(c_dNoiseStepBeginFrame[2] - c_dNoiseStepBeginFrame[0]);
			Ipp8u alpha = 0.5*(dFrame - c_dNoiseStepBeginFrame[1])/(c_dNoiseStepBeginFrame[2] - c_dNoiseStepBeginFrame[1]) * 255;
			ippiAlphaCompC_8u_AC4R( 
				last.Pixels, 4*nWidth, alpha,
				first.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 3:
		{			
			dFrameIn = 1.0 - (dFrame - c_dNoiseStepBeginFrame[2])/(c_dNoiseStepBeginFrame[4] - c_dNoiseStepBeginFrame[2]);
			Ipp8u alpha = 0.5*(1.0 - (dFrame - c_dNoiseStepBeginFrame[2])/(c_dNoiseStepBeginFrame[3] - c_dNoiseStepBeginFrame[2])) * 255;
			ippiAlphaCompC_8u_AC4R( 
				first.Pixels, 4*nWidth, alpha,
				last.Pixels, 4*nWidth, 255, 
				result.Pixels, 4*nWidth, roiSize, ippAlphaOver);
		}
		break;
	case 4:
		dFrameIn = 1.0 - (dFrame - c_dNoiseStepBeginFrame[2])/(c_dNoiseStepBeginFrame[4] - c_dNoiseStepBeginFrame[2]);
		memcpy(result.Pixels,last.Pixels,4*nWidth*nHeight);
		break;
	}

	BYTE nMax = 255*dFrameIn;

	unsigned int nSeed	= GetTickCount();
	ippiAddRandUniform_Direct_8u_AC4IR(result.Pixels, 4*nWidth, roiSize, 0, nMax, &nSeed);

	return TRUE;
}
	
BOOL IPPFlipBoxesVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
                           int nSizeX, int nSizeY,	double dFrame, BOOL bTop )
{
    int nWidth = first.Width;
    int nHeight = first.Height;
    
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	int      nHTileCount = nHeight / nSizeY;
	int		 nBottomBorder = nHeight % nSizeY;
	if ( nBottomBorder > 1 )
		nHTileCount++;
	IppiSize roiTileSize = {nWidth,nSizeY};
 	IppiRect roiSrcRect = {0,0,nWidth,nSizeY};
   
	int		 nIppStep = 4*nWidth;
    int		 nTileHStep = nIppStep*nSizeY;
	double	 dKoeff;
	Ipp8u *  pDst = result.Pixels;
	int		 nDstShift = ( !bTop ) ? 0 : ( nIppStep * nHeight - nTileHStep );
    double	 dFlipTime = 4.0/(nHTileCount + 3);
	double	 dNextFlip = dFlipTime * 0.25;
	int		 i = 0;
	double	 dPrevFlips = 0;
	Ipp8u *	 pSrc = NULL;
	double	 yCenter = nSizeY * 0.5;
	double	 xCenter = nWidth * 0.5;

	memset(pDst,0,nIppStep*nHeight);

	for ( i=0; i<nHTileCount; i++ )
	{
		dKoeff = 1;
		dPrevFlips = i * dNextFlip;
		if ( dFrame < dPrevFlips )
		{
			pSrc = first.Pixels;
			if ( i == (nHTileCount - 1) && nBottomBorder > 1)
				roiTileSize.height = nBottomBorder;
			ippiCopy_8u_AC4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
		}
		else if ( dFrame > dPrevFlips+dFlipTime )
		{
			pSrc = last.Pixels;
			if ( i == (nHTileCount - 1) && nBottomBorder > 1)
				roiTileSize.height = nBottomBorder;
			ippiCopy_8u_AC4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
		}
		else 
		{
			dKoeff = (dFrame - dPrevFlips)/dFlipTime;
			if ( dKoeff > 0.5 )
			{
				pSrc = last.Pixels;
				dKoeff = 2 * (dKoeff - 0.5);
			}
			else
			{
				pSrc = first.Pixels;
				dKoeff = 2 * (0.5 - dKoeff);
			}
			if ( dKoeff > 0.0001 )
			{
				if ( i == (nHTileCount - 1) && nBottomBorder > 1)
				{	
					roiSrcRect.height =  roiTileSize.height = nBottomBorder;
					ippiResizeCenter_8u_AC4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
										nIppStep, roiTileSize, 1, dKoeff, xCenter, nBottomBorder*0.5, IPPI_INTER_LINEAR);
				}
				else
					ippiResizeCenter_8u_AC4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
										nIppStep, roiTileSize, 1, dKoeff, xCenter, yCenter , IPPI_INTER_LINEAR);
			}
		}
		if ( !bTop )
			nDstShift += nTileHStep;
		else
		{	
			nDstShift = max(nDstShift-nTileHStep,0);
		}
	}
	return TRUE;
}

BOOL IPPFlipBoxesHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
                           int nSizeX, int nSizeY,	double dFrame, BOOL bLeft )
{
    int nWidth = first.Width;
    int nHeight = first.Height;
    
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

	int      nWTileCount = nWidth / nSizeX;
	int		 nRightBorder = nWidth % nSizeX;
	if ( nRightBorder > 1 )
		nWTileCount++;
	IppiSize roiTileSize = {nSizeX,nHeight};
 	IppiRect roiSrcRect = {0,0,nSizeX,nHeight};
   
	int		 nIppStep = 4*nWidth;
    int		 nTileWStep = 4*nSizeX;
    double	 dFlipTime = 4.0/(nWTileCount + 3);
	double	 dNextFlip = dFlipTime * 0.25;

	double	 dKoeff;
	Ipp8u *  pDst = result.Pixels;
	int		 nDstShift = ( bLeft ) ? 0 : ( nIppStep - nTileWStep );
	int		 i = 0;
	double	 dPrevFlips = 0;
	Ipp8u *	 pSrc = NULL;
	double	 xCenter = nSizeX * 0.5;
	double	 yCenter = nHeight * 0.5;

	memset(pDst,0,nIppStep*nHeight);

	for ( i=0; i<nWTileCount; i++ )
	{
		dKoeff = 1;
		dPrevFlips = i * dNextFlip;
		if ( dFrame < dPrevFlips )
		{
			pSrc = first.Pixels;
			if ( i == (nWTileCount - 1) && nRightBorder > 1)
				roiTileSize.width = nRightBorder;
			ippiCopy_8u_AC4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
		}
		else if ( dFrame > dPrevFlips+dFlipTime )
		{
			pSrc = last.Pixels;
			if ( i == (nWTileCount - 1) && nRightBorder > 1)
				roiTileSize.width = nRightBorder;
			ippiCopy_8u_AC4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
		}
		else 
		{
			dKoeff = (dFrame - dPrevFlips)/dFlipTime;
			if ( dKoeff > 0.5 )
			{
				pSrc = last.Pixels;
				dKoeff = 2 * (dKoeff - 0.5);
			}
			else
			{
				pSrc = first.Pixels;
				dKoeff = 2 * (0.5 - dKoeff);
			}
			if ( dKoeff > 0.0001 )
			{
				if ( i == (nWTileCount - 1) && nRightBorder > 1)
				{	
					roiSrcRect.width =  roiTileSize.width = nRightBorder;
					ippiResizeCenter_8u_AC4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
										nIppStep, roiTileSize, dKoeff, 1, nRightBorder*0.5, yCenter, IPPI_INTER_LINEAR);
				}
				else
					ippiResizeCenter_8u_AC4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
										nIppStep, roiTileSize, dKoeff, 1, xCenter, yCenter , IPPI_INTER_LINEAR);
			}
		}
		if ( bLeft )
			nDstShift += nTileWStep;
		else
		{	
			nDstShift = max(nDstShift-nTileWStep,0);
		}
	}
	return TRUE;
}

BOOL IPPFlipOutBoxesVertical(const SmartImage &first, const SmartImage &last, SmartImage &result,
                         int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
    int nWidth = first.Width;
    int nHeight = first.Height;
	
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

    IppiSize roiSize = {nWidth,nHeight};
	IppiSize roiTileSize  = {nSizeX, nSizeY};

	IppiSize FinishTileSize = { (int)(nSizeX * 0.1), (int)(nSizeY * 0.1) };
	int		 nDistW = 4*FinishTileSize.width;
	int		 nDistH = 4*FinishTileSize.height;
	IppiSize FinishRoiSize;

	int      nWTileCount = nWidth / nSizeX;
	int		 nRightBorder = nWidth % nSizeX;
	int		 nFinishRightBorder = 0;
	FinishRoiSize.width = nWTileCount * FinishTileSize.width;
	if ( nRightBorder > 1 )
	{
		nWTileCount++;
		nFinishRightBorder = (int)(nRightBorder * 0.1);	
		FinishRoiSize.width += nFinishRightBorder;
	}
	FinishRoiSize.width += (nWTileCount-1) * nDistW;

	int		 nHTileCount = nHeight / nSizeY;
	int		 nBottomBorder = nHeight % nSizeY;
	int		 nFinishBottomBorder = 0;
	FinishRoiSize.height = nHTileCount * FinishTileSize.height;
	if ( nBottomBorder > 1 )
	{
		nHTileCount++;
		nFinishBottomBorder = (int)(nBottomBorder * 0.1);
		FinishRoiSize.height += nFinishBottomBorder;
	}
	FinishRoiSize.height += (nHTileCount-1) * nDistH;

	IppiRect srcRect = {0,0,nSizeX,nSizeY};
	int		 nIppStep = 4*nWidth;
    Ipp8u *	 pA = NULL;
    Ipp8u *	 pDst = NULL;
	double dFlipKoef, dResize, dXYKoef;
	
	int i, j;
	int nSrcShift = 0, nDstShift = 0;
	bool bFlip = false;
	bool bFirst = true;
	int  nStartX, nStartY, nFinishX, nFinishY, nCurrX, nCurrY;
	int nFinishTileX, nFinishTileY;

	nStartX = nStartY = 0;
	nFinishX = (nWidth - FinishRoiSize.width)/2;
	nFinishY = (nHeight - FinishRoiSize.height)/2;
	
	memset(result.Pixels,0,nIppStep*nHeight);

    if (dFrame <= 0.5)
    {
        pA = first.Pixels;
		dFrame = 1.0 - dFrame;
    }
    else
    {
        pA = last.Pixels;
		bFirst = false;
    }

	if ( dFrame < 0.7 )
	{
		bFlip = true;
		dResize = 0.1;	
		dXYKoef = 0;
		pDst = new Ipp8u[ nIppStep * nHeight ];
		memset(pDst,0,nIppStep*nHeight);
	}
	else
	{
		dResize = ( dFrame - 0.7 )/0.3*0.9 + 0.1;					
		dXYKoef = ( dFrame - 0.7 )/0.3;
		pDst = result.Pixels;
	}

	IppiSize CurrTileSize = { (int)(nSizeX * dResize), (int)(nSizeY * dResize) };
	int nCurrBottomBorder = (int)(nBottomBorder * dResize);
	int nCurrRightBorder = (int)(nRightBorder * dResize);
	int nTileSizeWidth = CurrTileSize.width;

	for ( i=0; i<nHTileCount; i++ )
	{
		nStartY = i * nSizeY;
		nStartX = 0;
		nFinishTileY = nFinishY + i * (FinishTileSize.height + nDistH);
		nFinishTileX = nFinishX;
		nCurrY = (int)(nStartY * dXYKoef + nFinishTileY * (1 - dXYKoef));
		if ( i == nHTileCount-1 && nBottomBorder > 1 )
		{
			roiTileSize.height = srcRect.height = nBottomBorder;
			CurrTileSize.height = nCurrBottomBorder;
		}
		for ( j=0; j<nWTileCount; j++ )
		{
			nCurrX = (int)(nStartX * dXYKoef + nFinishTileX * (1 - dXYKoef));
			if ( (j == (nWTileCount-1) && bFirst || j == 0 && !bFirst)  && nRightBorder > 1 )		
			{
				roiTileSize.width = srcRect.width = nRightBorder;
				CurrTileSize.width = nCurrRightBorder;
			}
			nSrcShift = nStartY * nIppStep + nStartX * 4;
			nDstShift = nCurrY * nIppStep + nCurrX * 4;
			ippiResize_8u_AC4R( pA + nSrcShift, roiTileSize, nIppStep, srcRect, pDst + nDstShift,
								nIppStep, CurrTileSize,	dResize, dResize, IPPI_INTER_LINEAR );
			
			if ( j == 0 && !bFirst && nRightBorder > 1 )
				nFinishTileX += (nFinishRightBorder + nDistW);
			else
				nFinishTileX += (FinishTileSize.width + nDistW);
			nStartX += roiTileSize.width;
			roiTileSize.width = srcRect.width = nSizeX;
			CurrTileSize.width = nTileSizeWidth;
		}
	}
	
    if ( bFlip )
	{
		srcRect.x = srcRect.y = 0;
		srcRect.width = nWidth;
		srcRect.height = nHeight;
		double	quadDst[4][2], quadSrc[4][2];
		dFlipKoef = 2.5 * ( 0.7 - dFrame );	
		quadSrc[0][0] = quadSrc[3][0] = srcRect.x;
		quadSrc[1][0] = quadSrc[2][0] = srcRect.x + nWidth - 1;
		quadSrc[0][1] = quadSrc[1][1] = srcRect.y;
		quadSrc[2][1] = quadSrc[3][1] = srcRect.y + nHeight - 1;
		
		quadDst[0][0] = quadDst[3][0] = (nWidth-1) * dFlipKoef;
		quadDst[1][0] = quadDst[2][0] = (nWidth-1) * (1 - dFlipKoef);
	
		if ( bIn && bFirst || !bIn && !bFirst)
		{
			quadDst[1][1] = 0.3 * (nHeight-1) * dFlipKoef * 2;
			quadDst[0][1] = -quadDst[1][1];
			quadDst[3][1] = nHeight-1 + quadDst[1][1];
			quadDst[2][1] = nHeight-1 - quadDst[1][1];
		}
		else
		{
			quadDst[0][1] = 0.3 * (nHeight-1) * dFlipKoef * 2;
			quadDst[3][1] = nHeight-1 - quadDst[0][1];
			quadDst[1][1] = -quadDst[0][1];
			quadDst[2][1] = nHeight-1 + quadDst[0][1];
		}	
		ippiWarpBilinearQuad_8u_AC4R( pDst, roiSize, nIppStep, srcRect, quadSrc, result.Pixels,
							        nIppStep, srcRect, quadDst, IPPI_INTER_LINEAR );

		delete [] pDst;	
	}

    return TRUE;
}
BOOL IPPFlipOutBoxesHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result,
                         int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
{
    int nWidth = first.Width;
    int nHeight = first.Height;
	
	nSizeX = (nSizeX < 2 || nSizeX > nWidth) ? nWidth : nSizeX;
	nSizeY = (nSizeY < 2 || nSizeY > nHeight) ? nHeight : nSizeY;

    IppiSize roiSize = {nWidth,nHeight};
	IppiSize roiTileSize  = {nSizeX, nSizeY};

	IppiSize FinishTileSize = { (int)(nSizeX * 0.1), (int)(nSizeY * 0.1) };
	int		 nDistW = 4*FinishTileSize.width;
	int		 nDistH = 4*FinishTileSize.height;
	IppiSize FinishRoiSize;

	int      nWTileCount = nWidth / nSizeX;
	int		 nRightBorder = nWidth % nSizeX;
	int		 nFinishRightBorder = 0;
	FinishRoiSize.width = nWTileCount * FinishTileSize.width;
	if ( nRightBorder > 1 )
	{
		nWTileCount++;
		nFinishRightBorder = (int)(nRightBorder * 0.1);	
		FinishRoiSize.width += nFinishRightBorder;
	}
	FinishRoiSize.width += (nWTileCount-1) * nDistW;

	int		 nHTileCount = nHeight / nSizeY;
	int		 nBottomBorder = nHeight % nSizeY;
	int		 nFinishBottomBorder = 0;
	FinishRoiSize.height = nHTileCount * FinishTileSize.height;
	if ( nBottomBorder > 1 )
	{
		nHTileCount++;
		nFinishBottomBorder = (int)(nBottomBorder * 0.1);
		FinishRoiSize.height += nFinishBottomBorder;
	}
	FinishRoiSize.height += (nHTileCount-1) * nDistH;

	IppiRect srcRect = {0,0,nSizeX,nSizeY};
	int		 nIppStep = 4*nWidth;
    Ipp8u *	 pA = NULL;
    Ipp8u *	 pDst = NULL;
	double dFlipKoef, dResize, dXYKoef;
	
	int i, j;
	int nSrcShift = 0, nDstShift = 0;
	bool bFlip = false;
	bool bFirst = true;
	int  nStartX, nStartY, nFinishX, nFinishY, nCurrX, nCurrY;
	int nFinishTileX, nFinishTileY;

	nStartX = nStartY = 0;
	nFinishTileX = nFinishX = (nWidth - FinishRoiSize.width)/2;
	nFinishTileY = nFinishY = (nHeight - FinishRoiSize.height)/2;
	
	memset(result.Pixels,0,nIppStep*nHeight);

    if (dFrame < 0.5)
    {
        pA = first.Pixels;
		dFrame = 1.0 - dFrame;
    }
    else
    {
        pA = last.Pixels;
		bFirst = false;
    }

	if ( dFrame < 0.7 )
	{
		bFlip = true;
		dResize = 0.1;	
		dXYKoef = 0;
		pDst = new Ipp8u[ nIppStep * nHeight ];
		memset(pDst,0,nIppStep*nHeight);
	}
	else
	{
		dResize = ( dFrame - 0.7 )/0.3*0.9 + 0.1;					
		dXYKoef = ( dFrame - 0.7 )/0.3;
		pDst = result.Pixels;
	}

	IppiSize CurrTileSize = { (int)(nSizeX * dResize), (int)(nSizeY * dResize) };
	int nCurrBottomBorder = (int)(nBottomBorder * dResize);
	int nCurrRightBorder = (int)(nRightBorder * dResize);
	int nTileSizeWidth = CurrTileSize.width;
	int nTileSizeHeight = CurrTileSize.height;

	for ( i=0; i<nHTileCount; i++ )
	{
		nCurrY = (int)(nStartY * dXYKoef + nFinishTileY * (1 - dXYKoef));
		if ( (i == (nHTileCount-1) && bFirst || i == 0 && !bFirst)  && nBottomBorder > 1 )
		{
			roiTileSize.height = srcRect.height = nBottomBorder;
			CurrTileSize.height = nCurrBottomBorder;
		}
		for ( j=0; j<nWTileCount; j++ )
		{
			nCurrX = (int)(nStartX * dXYKoef + nFinishTileX * (1 - dXYKoef));
			if ( j == (nWTileCount-1) && nRightBorder > 1 )		
			{
				roiTileSize.width = srcRect.width = nRightBorder;
				CurrTileSize.width = nCurrRightBorder;
			}
			nSrcShift = nStartY * nIppStep + nStartX * 4;
			nDstShift = nCurrY * nIppStep + nCurrX * 4;
			ippiResize_8u_AC4R( pA + nSrcShift, roiTileSize, nIppStep, srcRect, pDst + nDstShift,
								nIppStep, CurrTileSize,	dResize, dResize, IPPI_INTER_LINEAR );
			
			roiTileSize.width = srcRect.width = nSizeX;
			CurrTileSize.width = nTileSizeWidth;
			nFinishTileX += (FinishTileSize.width + nDistW);
			nStartX += roiTileSize.width;
		}
		nStartX = 0;
		nFinishTileX = nFinishX;
		if ( !bFirst && nBottomBorder > 1 )
		{
			nStartY = i * nSizeY + nBottomBorder;
			nFinishTileY = nFinishY + i * (FinishTileSize.height + nDistH) + 
											(nFinishBottomBorder + nDistH);
			if (i == 0 )
			{
				roiTileSize.height = srcRect.height = nSizeY;
				CurrTileSize.height = nTileSizeHeight;
			}
		}
		else
		{
			nStartY = (i+1) * nSizeY;
			nFinishTileY = nFinishY + (i+1) * (FinishTileSize.height + nDistH);
		}
	}
	
    if ( bFlip )
	{
		srcRect.x = srcRect.y = 0;
		srcRect.width = nWidth;
		srcRect.height = nHeight;
		double	quadDst[4][2], quadSrc[4][2];
		dFlipKoef = 2.5 * ( 0.7 - dFrame );	
	
		quadSrc[0][0] = quadSrc[3][0] = srcRect.x;
		quadSrc[1][0] = quadSrc[2][0] = srcRect.x + nWidth - 1;
		quadSrc[0][1] = quadSrc[1][1] = srcRect.y;
		quadSrc[2][1] = quadSrc[3][1] = srcRect.y + nHeight - 1;
		

		quadDst[0][1] = quadDst[1][1] = (nHeight-1) * dFlipKoef;
		quadDst[2][1] = quadDst[3][1] = (nHeight-1) * (1 - dFlipKoef);

		if ( bIn && bFirst || !bIn && !bFirst)
		{
			quadDst[3][0] = 0.3 * (nWidth-1) * dFlipKoef * 2;
			quadDst[0][0] = -quadDst[3][0];
			quadDst[1][0] = nWidth-1 + quadDst[3][0];
			quadDst[2][0] = nWidth-1 - quadDst[3][0];
		}
		else
		{
			quadDst[0][0] = 0.3 * (nWidth-1) * dFlipKoef * 2;
			quadDst[1][0] = nWidth-1 - quadDst[0][0];
			quadDst[2][0] = nWidth-1 + quadDst[0][0];
			quadDst[3][0] = -quadDst[0][0];
		}

		ippiWarpBilinearQuad_8u_AC4R( pDst, roiSize, nIppStep, srcRect, quadSrc, result.Pixels,
							        nIppStep, srcRect, quadDst, IPPI_INTER_LINEAR );

		delete [] pDst;	
	}

    return TRUE;
}


BOOL IPPFlash(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int Color)
{
	dFrame = min(max(dFrame, 0.0), 1.0);

	BYTE* ps = first.Pixels;
	BYTE* pr = result.Pixels;

	int nSize = first.Height*first.Width;

	BYTE* pMapB = new BYTE[256];
	BYTE* pMapG = new BYTE[256];
	BYTE* pMapR = new BYTE[256];

	double dK2 = 2.0*dFrame;
	double dK1 = 1.0 - dK2;

	if (dFrame < 0.5)
		ps = first.Pixels;
	else
	{
		dK1 = 2.0*(dFrame - 0.5);
		dK2 = 1.0 - dK1;

		ps = last.Pixels;
	}

	int nIndex = 0;

	// construct color map
	for (nIndex = 0; nIndex < 256; ++nIndex)
	{
		double dB = dK1*nIndex + dK2*GetBValue(Color);
		double dG = dK1*nIndex + dK2*GetGValue(Color);
		double dR = dK1*nIndex + dK2*GetRValue(Color);

		pMapB[nIndex] = (BYTE)min(255, max(0, dB));
		pMapG[nIndex] = (BYTE)min(255, max(0, dG));
		pMapR[nIndex] = (BYTE)min(255, max(0, dR));
	}
	
	// apply color map
	for (nIndex = 0; nIndex < nSize; ++nIndex)
	{
		// combine colors
		*pr = *(pMapB + *ps); ps++; pr++;
		*pr = *(pMapG + *ps); ps++; pr++;
		*pr = *(pMapR + *ps); ps++; pr++;

		// offset to next pixel
		ps++; pr++;
	}

	delete[] pMapB;
	delete[] pMapG;
	delete[] pMapR;

	return TRUE;
}

BOOL IPPMask(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Mask* pMask, int nFadeWide)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;

	BYTE* pBGRAResult  = result.Pixels;

	int nHeight = first.Height;
	int nWidth = first.Width;

	IppiSize roiSize;

	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiRect roiRect = { 0, 0, nWidth, nHeight};

	pMask->GenerateMask( nWidth, nHeight, nType);
	int nFrame = dFrame * 256.0;
	int nIndex = 0;

	for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
	{
		for( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
		{
			if(pMask->g_pMask[nYIndex*nWidth + nXIndex] < nFrame - nFadeWide / 2)
			{
				pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
			}
			else if(pMask->g_pMask[nYIndex*nWidth + nXIndex] <= nFrame && pMask->g_pMask[nYIndex*nWidth + nXIndex] >= nFrame - nFadeWide / 2 &&(nFadeWide > 0) )
			{
				double dValue = 0.5 - (-pMask->g_pMask[nYIndex*nWidth + nXIndex] + nFrame) * 1. / nFadeWide;
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * dValue + pBGRASource2[nIndex + 2] * (1 - dValue);
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * dValue + pBGRASource2[nIndex + 1] * (1 - dValue);
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * dValue + pBGRASource2[nIndex + 0] * (1 - dValue);
			}
			else if(pMask->g_pMask[nYIndex*nWidth + nXIndex] - nFrame <= nFadeWide / 2 && pMask->g_pMask[nYIndex*nWidth + nXIndex] >= nFrame && (nFadeWide > 0) )
			{
				double dValue = (pMask->g_pMask[nYIndex*nWidth + nXIndex] - nFrame) * 1. / nFadeWide + 0.5;
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * dValue + pBGRASource2[nIndex + 2] * (1 - dValue);
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * dValue + pBGRASource2[nIndex + 1] * (1 - dValue);
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * dValue + pBGRASource2[nIndex + 0] * (1 - dValue);
			}
			else
			{
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
			}


		}
	}
	return TRUE;
}

BOOL IPPTetris(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* pUtility)	
{

	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	for ( int nYIndex = 0, nIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
		{
			pBGRASource1[nIndex + 3] = 255;
			pBGRASource2[nIndex + 3] = 255;
			pBGRAResult[nIndex + 3] = 255;
		}
	}

	BYTE* ps1 = pBGRASource1;
	BYTE* ps2 = pBGRASource2;
	BYTE* pr = pBGRAResult;
										
	if ( c_nTetrisCollapse == nType)
	{
		ps1 = pBGRASource2;
		ps2 = pBGRASource1;
	}
					
	pUtility->GenerateRand(4);
	dFrame = min(max(dFrame, 0.0), 1.0);
	memcpy(pr, ps1, 4 * nWidth * nHeight);
	//IPP_CpyBar(pr, ps1, nWidth, nHeight, 0, 0, nWidth, nHeight, 0, 0);
	if(0 == pUtility->g_nRand)
	{
		CTetrisFigure pFigures[5];
		int nBottomLineFigures[6];
		for(int i = 0; i < 6; i++)
			nBottomLineFigures[i] = (int)6 * ( ( nHeight) * dFrame - nHeight * i / 6);
		pFigures[0].SetValue( 3 * nWidth / 5,     nHeight / 2, 2, ps2, nWidth, nHeight, 2 * nWidth / 5,	              0);
		pFigures[1].SetValue( 2 * nWidth / 5, 3 * nHeight / 4, 6, ps2, nWidth, nHeight,              0,	              0);					
		pFigures[2].SetValue( 2 * nWidth / 5,     nHeight / 2, 1, ps2, nWidth, nHeight,     nWidth / 5,     nHeight / 4);					
		pFigures[3].SetValue( 2 * nWidth / 5, 3 * nHeight / 4, 4, ps2, nWidth, nHeight, 3 * nWidth / 5,     nHeight / 4);					
		pFigures[4].SetValue( 4 * nWidth / 5,     nHeight / 4, 1, ps2, nWidth, nHeight,              0,	3 * nHeight / 4);					
		if ( c_nTetris == nType )
		{
			pUtility->RunTetris(pFigures, 6, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
		else
		{
			pUtility->CollapseTetris(pFigures, 6, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
	}
	else if(1 == pUtility->g_nRand)
	{
		CTetrisFigure pFigures[9];
		int nBottomLineFigures[10];
		for(int i = 0; i < 10; i++)
			nBottomLineFigures[i] = (int)10*((nHeight)*dFrame - nHeight*i/10);
		pFigures[0].SetValue( 2 * nWidth / 6, 2 * nHeight / 6,	1, ps2, nWidth, nHeight, 4*nWidth/6,	0);					
		pFigures[1].SetValue(     nWidth / 6, 4 * nHeight / 6,	1, ps2, nWidth, nHeight, 0,				0);					
		pFigures[2].SetValue( 2 * nWidth / 6, 3 * nHeight / 6,	6, ps2, nWidth, nHeight, 2*nWidth/6,	0);					
		pFigures[3].SetValue( 2 * nWidth / 6, 3 * nHeight / 6,	3, ps2, nWidth, nHeight, 3*nWidth/6,	nHeight/6);					
		pFigures[4].SetValue( 2 * nWidth / 6, 3 * nHeight / 6,	4, ps2, nWidth, nHeight, 4*nWidth/6,	2*nHeight/6);					
		pFigures[5].SetValue(     nWidth / 6, 4 * nHeight / 6,	1, ps2, nWidth, nHeight, nWidth/6,		0);					
		pFigures[6].SetValue( 3 * nWidth / 6, 2 * nHeight / 6,	5, ps2, nWidth, nHeight, nWidth/6,		3*nHeight/6);					
		pFigures[7].SetValue( 3 * nWidth / 6, 2 * nHeight / 6,	5, ps2, nWidth, nHeight, 3*nWidth/6,	4*nHeight/6);					
		pFigures[8].SetValue( 3 * nWidth / 6, 2 * nHeight / 6,	7, ps2, nWidth, nHeight, 0,				4*nHeight/6);					
		if ( c_nTetris == nType )
		{
			pUtility->RunTetris(pFigures, 10, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
		else
		{
			pUtility->CollapseTetris(pFigures, 10, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
	}
	else if(2 == pUtility->g_nRand)
	{
		CTetrisFigure pFigures[9];
		int nBottomLineFigures[10];
		for(int i = 0; i < 10; i++)
			nBottomLineFigures[i] = (int)10*((nHeight)*dFrame - nHeight*i/10);
		pFigures[0].SetValue(3*nWidth/6,	2*nHeight/6,	2, ps2, nWidth, nHeight, 0,				0);					
		pFigures[1].SetValue(2*nWidth/6,	2*nHeight/6,	1, ps2, nWidth, nHeight, 3*nWidth/6,	0);					
		pFigures[2].SetValue(2*nWidth/6,	3*nHeight/6,	3, ps2, nWidth, nHeight, 0,				nHeight/6);					
		pFigures[3].SetValue(3*nWidth/6,	2*nHeight/6,	7, ps2, nWidth, nHeight, 2*nWidth/6,	nHeight/6);					
		pFigures[4].SetValue(nWidth/6,		4*nHeight/6,	1, ps2, nWidth, nHeight, 5*nWidth/6,	0);					
		pFigures[5].SetValue(3*nWidth/6,	2*nHeight/6,	2, ps2, nWidth, nHeight, nWidth/6,		3*nHeight/6);					
		pFigures[6].SetValue(3*nWidth/6,	2*nHeight/6,	5, ps2, nWidth, nHeight, 3*nWidth/6,	3*nHeight/6);					
		pFigures[7].SetValue(4*nWidth/6,	nHeight/6,		1, ps2, nWidth, nHeight, 2*nWidth/6,	5*nHeight/6);					
		pFigures[8].SetValue(2*nWidth/6,	2*nHeight/6,	1, ps2, nWidth, nHeight, 0,				4*nHeight/6);					
		if ( c_nTetris == nType )
		{
			pUtility->RunTetris(pFigures, 10, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
		else
		{
			pUtility->CollapseTetris(pFigures, 10, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
	}
	else if(3 == pUtility->g_nRand)
	{
		CTetrisFigure pFigures[4];
		int nBottomLineFigures[5];
		for(int i = 0; i < 5; i++)
			nBottomLineFigures[i] = (int)5*((nHeight)*dFrame - nHeight*i/5);
		pFigures[0].SetValue(nWidth,		nHeight/4,		1, ps2, nWidth, nHeight, 0,				0);					
		pFigures[1].SetValue(3*nWidth/4,	2*nHeight/4,	2, ps2, nWidth, nHeight, 0,				nHeight/4);					
		pFigures[2].SetValue(2*nWidth/4,	3*nHeight/4,	4, ps2, nWidth, nHeight, 2*nWidth/4,	nHeight/4);					
		pFigures[3].SetValue(3*nWidth/4,	2*nHeight/4,	7, ps2, nWidth, nHeight, 0,				2*nHeight/4);					
		if ( c_nTetris == nType )
		{
			pUtility->RunTetris(pFigures, 5, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
		else
		{
			pUtility->CollapseTetris(pFigures, 5, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
	}
	else if(4 == pUtility->g_nRand || !(4 == pUtility->g_nRand))
	{
		CTetrisFigure pFigures[6];
		int nBottomLineFigures[6];
		for(int i = 0; i < 6; i++)
			nBottomLineFigures[i] = (int)6*((nHeight)*dFrame - nHeight*i/6);
		pFigures[0].SetValue(2*nWidth/4,	2*nHeight/5,	1, ps2, nWidth, nHeight, 2*nWidth/4,	0);					
		pFigures[1].SetValue(2*nWidth/4,	3*nHeight/5,	6, ps2, nWidth, nHeight, 0,				0);					
		pFigures[2].SetValue(2*nWidth/4,	3*nHeight/5,	3, ps2, nWidth, nHeight, nWidth/4,		nHeight/5);					
		pFigures[3].SetValue(2*nWidth/4,	3*nHeight/5,	4, ps2, nWidth, nHeight, 2*nWidth/4,	2*nHeight/5);					
		pFigures[4].SetValue(3*nWidth/4,	2*nHeight/5,	7, ps2, nWidth, nHeight, 0,				3*nHeight/5);					
		if ( c_nTetris == nType )
		{
			pUtility->RunTetris(pFigures, 6, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
		else
		{
			pUtility->CollapseTetris(pFigures, 6, dFrame, pr, nWidth, nHeight, 0, nBottomLineFigures);
		}
	}

	return TRUE;
}


BOOL IPPShotChangeHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* pUtility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
{

	int nWidth = first.Width;
	int nHeight = first.Height;

	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	for ( int nYIndex = 0, nIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
		{
			pBGRASource1[ nIndex + 3 ] = 255;
			pBGRASource2[ nIndex + 3 ] = 255;			
			pBGRAResult[ nIndex + 3 ] = 255;
		}
	}
	
	dFrame = min(max(dFrame, 0.0), 1.0);
	const double dPercent  = 9. / 35;

	pUtility->GenerateShot(nWidth, nHeight, nType, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
	IppiSize roiSize = {nWidth,nHeight};
	IppiRect srcRect = {0,0,nWidth,nHeight};
	IppiSize roiScaleSize = {nWidth,nHeight};
	BYTE *pResult = pBGRAResult;

	if(dFrame < 1./3.)
	{						
		double dKoef = 1 - dFrame*3;
		double dFactor = (1./(1 - dPercent) * dKoef + 1 *(1-dKoef));
		double dFactor2 = ((1 - dPercent) * (1 - dKoef) + 1 * dKoef);
		IPPResizeCenter(pUtility->g_pShotSource1, pResult, nWidth, nHeight, dFactor, dFactor, nWidth / 2, nHeight / 2);
		IPPResizeCenter(pBGRASource1, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
		
	}
	else if(dFrame < 2./3.)
	{
		BYTE* pBuffer1 = new BYTE[4 * nWidth * nHeight];
		BYTE* pBuffer2 = new BYTE[4 * nWidth * nHeight];

		::memcpy(pBuffer1, pUtility->g_pShotSource1, 4 * nWidth * nHeight);
		IPPResizeCenter(pBGRASource1, pBuffer1, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);

		::memcpy(pBuffer2, pUtility->g_pShotSource2, 4 * nWidth * nHeight);
		IPPResizeCenter(pBGRASource2, pBuffer2, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);
		if(c_nShotChangeGHorizontalLToR == nType || c_nShotChangeBHorizontalLToR == nType)
		{

			int nMidLine = (nWidth - nWidth * dPercent/2)* (1 - ( dFrame * 3 - 1));
			IppiSize roiSizeR1 = { nMidLine + nWidth * dPercent/2, nHeight};
			IppiSize roiSizeR2 = { nWidth - nMidLine, nHeight};
			ippiCopy_8u_C4R(pBuffer1 + int((nWidth - nMidLine - int(nWidth*dPercent/2))*4), 4*nWidth, pResult, 4*nWidth, roiSizeR1);
			ippiCopy_8u_C4R(pBuffer2, 4*nWidth, pResult + nMidLine*4, 4*nWidth, roiSizeR2);
		}
		else
		{	
			int nMidLine = (nWidth - nWidth * dPercent / 2) * (dFrame* 3  - 1);
			IppiSize roiSizeR2 = {nMidLine + nWidth * dPercent / 2,nHeight};
			IppiSize roiSizeR1 = {nWidth - nMidLine,nHeight};
			ippiCopy_8u_C4R(pBuffer2 + (nWidth - nMidLine - int(nWidth * dPercent / 2))*4, 4*nWidth, pResult, 4*nWidth, roiSizeR2);
			ippiCopy_8u_C4R(pBuffer1, 4*nWidth, pResult + nMidLine*4, 4*nWidth, roiSizeR1);
		}
		delete []pBuffer1;
		delete []pBuffer2;
	}
	else
	{
		double dKoef = 1 - (dFrame*3 - 2);
		double dFactor = (1./(1 - dPercent) * (1 - dKoef) + 1 *dKoef);
		double dFactor2 = ((1 - dPercent) * dKoef + 1 * (1 - dKoef));
		IPPResizeCenter(pUtility->g_pShotSource2, pResult, nWidth, nHeight, dFactor, dFactor, nWidth / 2, nHeight / 2);
		IPPResizeCenter(pBGRASource2, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
	}
					
	return TRUE;
}

BOOL IPPShotChangeVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* pUtility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
{					
	int nWidth = first.Width;
	int nHeight = first.Height;

	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	for ( int nYIndex = 0, nIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
		{
			pBGRASource1[ nIndex + 3 ] = 255;
			pBGRASource2[ nIndex + 3 ] = 255;			
			pBGRAResult[ nIndex + 3 ] = 255;
		}
	}

	dFrame = min(max(dFrame, 0.0), 1.0);
	const double dPercent = 9. / 35;

	pUtility->GenerateShot(nWidth, nHeight, nType, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
	
	IppiSize roiSize = {nWidth,nHeight};
	IppiRect srcRect = {0,0,nWidth,nHeight};
	IppiSize roiScaleSize = {nWidth,nHeight};
	
	Ipp8u *pResult = pBGRAResult;

	if(dFrame < 1./3.)
	{						
		double dKoef = 1 - dFrame*3;
		double dFactor1 = (1./(1- dPercent) * dKoef + 1 *(1-dKoef));
		double dFactor2 = 1 * dKoef + (1 - dPercent) * (1 - dKoef);

		IPPResizeCenter(pUtility->g_pShotSource1, pResult, nWidth, nHeight, dFactor1, dFactor1, nWidth / 2, nHeight / 2);
		IPPResizeCenter(pBGRASource1, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
	}
	else if(dFrame < 2./3.)
	{
		BYTE* pBuffer1 = new BYTE[4 * nWidth * nHeight];
		BYTE* pBuffer2 = new BYTE[4 * nWidth * nHeight];

		::memcpy(pBuffer1, pUtility->g_pShotSource1, 4 * nWidth * nHeight);
		IPPResizeCenter(pBGRASource1, pBuffer1, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);

		::memcpy(pBuffer2, pUtility->g_pShotSource2, 4 * nWidth * nHeight);
		IPPResizeCenter(pBGRASource2, pBuffer2, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);

		if(c_nShotChangeGVerticalBToU == nType || c_nShotChangeBVerticalBToU == nType)
		{
			int nMidLine = (nHeight - nHeight * dPercent / 2)* (1 - (dFrame*3 - 1));
			IppiSize roiSizeR2 = {nWidth, nHeight - nMidLine};
			IppiSize roiSizeR1 = {nWidth, nMidLine + nHeight * dPercent / 2};
			ippiCopy_8u_C4R(pBuffer1 + 4*nWidth*(nHeight - nMidLine - int(nHeight * dPercent / 2)), 4*nWidth, pResult, 4*nWidth, roiSizeR1);
			ippiCopy_8u_C4R(pBuffer2, 4*nWidth, pResult + 4*nWidth*nMidLine, 4*nWidth, roiSizeR2);
		}
		else
		{	
			int nMidLine = (nHeight - nHeight * dPercent / 2)*(dFrame*3 - 1);
			IppiSize roiSizeR1 = {nWidth, nHeight - nMidLine};
			IppiSize roiSizeR2 = {nWidth, nMidLine + nHeight * dPercent / 2};
			ippiCopy_8u_C4R(pBuffer2 + 4*nWidth*(nHeight - nMidLine - int(nHeight * dPercent / 2)), 4*nWidth, pResult, 4*nWidth, roiSizeR2);
			ippiCopy_8u_C4R(pBuffer1, 4*nWidth, pResult + 4*nWidth*nMidLine, 4*nWidth, roiSizeR1);
		}
		
		delete []pBuffer1;
		delete []pBuffer2;
	}
	else
	{
		double dKoef = 1 - (dFrame*3 - 2);
		double dFactor1 = (1./(1- dPercent) * (1 - dKoef) + 1 *dKoef);
		double dFactor2 = (1 - dPercent) * dKoef + 1 * (1 - dKoef);
		IPPResizeCenter(pUtility->g_pShotSource2, pResult, nWidth, nHeight, dFactor1, dFactor1, nWidth / 2, nHeight / 2);
		IPPResizeCenter(pBGRASource2, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
	}
	return TRUE;
}

BOOL IPPShotChangeHorizontalWithoutZoom(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* pUtility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	dFrame = min(max(dFrame, 0.0), 1.0);

	BYTE *pS1 = pBGRASource1;
	BYTE *pS2 = pBGRASource2;

	IppiSize roiSize = {nWidth,nHeight};

	const double dPercent  = 9. / 35;
	const double dXFactor = 0.01;

	pUtility->GenerateShot(nWidth, nHeight, nType, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);

	roiSize.width  = nWidth * ( 1 - 2 * dXFactor );
	roiSize.height = int(nHeight * ( 1 - dPercent));
	BYTE* pRR1 = pUtility->g_pShotSource1;
	BYTE* pRR2 = pUtility->g_pShotSource2;

	int nSdvig = 4 * nWidth * int(nHeight / 2 * dPercent) + 4 * int(nWidth * dXFactor);
	ippiCopy_8u_C4R( pS1 + nSdvig, 4 * nWidth, pRR1 + nSdvig, 4 * nWidth, roiSize);
	ippiCopy_8u_C4R( pS2 + nSdvig, 4 * nWidth, pRR2 + nSdvig, 4 * nWidth, roiSize);
					
	BYTE* pR1 = pUtility->g_pShotSource1;
	BYTE* pR2 = pUtility->g_pShotSource2;
	Ipp8u *pResult = pBGRAResult;
					
	if( dFrame < 1. / 3.)
	{
		for( int nYIndex = 0; nYIndex < nHeight; nYIndex++)
		{
			for( int nXIndex = 0; nXIndex < nWidth; nXIndex++)
			{
				for(int i = 0; i < 3; i++)
				{
					*pResult = *pS1 * ( 1- dFrame * 3) + *pRR1 * (dFrame * 3);
					pS1++;
					pResult++;
					pRR1++;
				}
				pS1++;
				pResult++;
				pRR1++;
			}
		}
	}
	else if(dFrame < 2./3.)
	{
		if(c_nShotChangeGHorizontalLToRNoZ == nType || c_nShotChangeBHorizontalLToRNoZ == nType)
		{
			int nMidLine = nWidth * ( 1 - (dFrame * 3 - 1));
			IppiSize roiSizeR1 = { nMidLine,          nHeight};
			IppiSize roiSizeR2 = { nWidth - nMidLine, nHeight};
			ippiCopy_8u_C4R( pR1 + ( nWidth - nMidLine) * 4, 4 * nWidth, pResult, 4 * nWidth, roiSizeR1);
			ippiCopy_8u_C4R( pR2, 4 * nWidth, pResult + nMidLine * 4, 4 * nWidth, roiSizeR2);
		}
		else
		{	
			int nMidLine = nWidth * (dFrame * 3 - 1);
			IppiSize roiSizeR2 = { nMidLine,         nHeight};
			IppiSize roiSizeR1 = { nWidth - nMidLine,nHeight};
			ippiCopy_8u_C4R( pR2 + ( nWidth - nMidLine) * 4, 4 * nWidth, pResult, 4 * nWidth, roiSizeR2);
			ippiCopy_8u_C4R( pR1, 4 * nWidth, pResult + nMidLine * 4, 4 * nWidth, roiSizeR1);
		}
	}
	else
	{
		for( int nYIndex = 0; nYIndex < nHeight; nYIndex++)
		{
			for( int nXIndex = 0; nXIndex < nWidth; nXIndex++)
			{
				for(int i = 0; i < 3; i++)
				{
					*pResult = *pRR2 * ( 1- (dFrame * 3 - 2)) + *pS2 * (dFrame * 3 - 2);
					pS2++;
					pResult++;
					pRR2++;
				}
				pS2++;
				pResult++;
				pRR2++;
			}
		}
	}
	return TRUE;
}


BOOL IPPShotChangeVerticalWithoutZoom(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* pUtility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)				
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	dFrame = min(max(dFrame, 0.0), 1.0);

	BYTE *pS1 = pBGRASource1;
	BYTE *pS2 = pBGRASource2;

	IppiSize roiSize = {nWidth,nHeight};

	const double dPercent  = 9. / 35;
	const double dYFactor = 0.01;

	pUtility->GenerateShot(nWidth, nHeight, nType, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);

	roiSize.width  = int(nWidth * ( 1 - dPercent));
	roiSize.height = int(nHeight * ( 1 - 2 * dYFactor ));
	BYTE* pRR1 = pUtility->g_pShotSource1;
	BYTE* pRR2 = pUtility->g_pShotSource2;

	int nSdvig = 4 * int(nWidth / 2 * dPercent) + 4 * nWidth * int(nHeight * dYFactor);
	ippiCopy_8u_C4R( pS1 + nSdvig, 4 * nWidth, pRR1 + nSdvig, 4 * nWidth, roiSize);
	ippiCopy_8u_C4R( pS2 + nSdvig, 4 * nWidth, pRR2 + nSdvig, 4 * nWidth, roiSize);
		
	BYTE* pR1 = pUtility->g_pShotSource1;
	BYTE* pR2 = pUtility->g_pShotSource2;
	Ipp8u *pResult = pBGRAResult;
					
	if( dFrame < 1. / 3.)
	{
		for( int nYIndex = 0; nYIndex < nHeight; nYIndex++)
		{
			for( int nXIndex = 0; nXIndex < nWidth; nXIndex++)
			{
				for(int i = 0; i < 3; i++)
				{
					*pResult = *pS1 * ( 1- dFrame * 3) + *pRR1 * (dFrame * 3);
					pS1++;
					pResult++;
					pRR1++;
				}
				pS1++;
				pResult++;
				pRR1++;
			}
		}
	}
	else if(dFrame < 2./3.)
	{
		if(c_nShotChangeGVerticalBToUNoZ == nType || c_nShotChangeBVerticalBToUNoZ == nType)
		{
			int nMidLine = nHeight * ( 1 - (dFrame * 3 - 1));
			IppiSize roiSizeR1 = { nWidth, nMidLine};
			IppiSize roiSizeR2 = { nWidth, nHeight - nMidLine};
			ippiCopy_8u_C4R( pR1 + ( nHeight - nMidLine) * 4 * nWidth, 4 * nWidth, pResult, 4 * nWidth, roiSizeR1);
			ippiCopy_8u_C4R( pR2, 4 * nWidth, pResult + nMidLine * 4 * nWidth, 4 * nWidth, roiSizeR2);
		}
		else
		{	
			int nMidLine = nHeight * (dFrame * 3 - 1);
			IppiSize roiSizeR2 = { nWidth, nMidLine};
			IppiSize roiSizeR1 = { nWidth, nHeight - nMidLine};
			ippiCopy_8u_C4R( pR2 + ( nHeight - nMidLine) * 4 * nWidth, 4 * nWidth, pResult, 4 * nWidth, roiSizeR2);
			ippiCopy_8u_C4R( pR1, 4 * nWidth, pResult + nMidLine * 4 * nWidth, 4 * nWidth, roiSizeR1);
		}
	}
	else
	{
		for( int nYIndex = 0; nYIndex < nHeight; nYIndex++)
		{
			for( int nXIndex = 0; nXIndex < nWidth; nXIndex++)
			{
				for(int i = 0; i < 3; i++)
				{
					*pResult = *pRR2 * ( 1- (dFrame * 3 - 2)) + *pS2 * (dFrame * 3 - 2);
					pS2++;
					pResult++;
					pRR2++;
				}
				pS2++;
				pResult++;
				pRR2++;
			}
		}
	}
	return TRUE;
}
BOOL IPPBlackWhiteLines(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bInverse)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	dFrame = min(max(dFrame, 0.0), 1.0);
	BYTE* pSource1 = pBGRASource1;
	BYTE* pSource2 = pBGRASource2;
	BYTE* pResult = pBGRAResult;
					
	const int nCountLines = 10;
	const double dUnitX = nWidth  * 2  / nCountLines;  
	const double dUnitY = nHeight * 2  / nCountLines;
	const Color cWhiteBorder( 128 , 128, 128);
	const Color cWhiteMid( 255 , 255, 255);
	const Color cBlackBorder( 0, 0, 0);
	const Color cBlackMid( 128, 128, 128);
	const Color cBlendBlackLine( 200, 200, 200);
	const Color cBlendWhiteLine( 100, 100, 100);
	const float fX =  sin( dUnitY / dUnitX ) * nHeight;

	const float fY = -cos( dUnitY / dUnitX ) * nHeight;

	BYTE* pBlackWhitePicture = new BYTE[4 * nWidth * nHeight];
	if(dFrame < 0.5)
	{
		::memcpy( pBlackWhitePicture, pSource1, 4 * nWidth * nHeight);
		pBlackWhitePicture += 4 * nWidth * (nHeight - 1);
		Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBlackWhitePicture );
		if( Gdiplus::Ok == oImage.GetLastStatus() )
		{
			Gdiplus::Graphics oRender( &oImage );
				
			oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
			if( Gdiplus::Ok == oRender.GetLastStatus() )
			{
				Gdiplus::LinearGradientBrush* oBlackBrush;
				Gdiplus::LinearGradientBrush* oWhiteBrush;
				if(!bInverse)
				{
					oBlackBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(fX, fY), Gdiplus::Point(0,0), cBlackBorder, cBlackMid);									
					oWhiteBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(-fX, -fY), Gdiplus::Point(0,0), cWhiteBorder, cWhiteMid);
				}
				else
				{
					oBlackBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(nWidth - fX, fY), Gdiplus::Point(nWidth,0), cBlackBorder, cBlackMid);
					oWhiteBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(nWidth + fX, -fY), Gdiplus::Point(nWidth,0), cWhiteBorder, cWhiteMid);
				}
				oBlackBrush->SetWrapMode(Gdiplus::WrapMode::WrapModeTileFlipXY);
				oWhiteBrush->SetWrapMode(Gdiplus::WrapMode::WrapModeTileFlipXY);
				
				Gdiplus::PointF *arrPoints = new Gdiplus::PointF[4];
				Gdiplus::Pen* oBlackPen = new Gdiplus::Pen(cBlendBlackLine);
				Gdiplus::Pen* oWhitePen = new Gdiplus::Pen(cBlendWhiteLine);
						
				for(int i = 0; i < nCountLines / 2; i++)
				{
					if(bInverse)
					{
						arrPoints->X = nWidth - fX - 2 * i * dUnitX / 2; 
						arrPoints->Y = fY + 2 * i * dUnitY / 2; 
						( arrPoints + 1 )->X = arrPoints->X - dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = (arrPoints->X) * (1 - dFrame * 2) + (arrPoints->X + 2 * fX) * (dFrame * 2); 
						( arrPoints + 3 )->Y = (arrPoints->Y) * (1 - dFrame * 2) + (arrPoints->Y - 2 * fY) * (dFrame * 2); 
						( arrPoints + 2 )->X = (arrPoints + 3)->X - dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					else
					{
						arrPoints->X = fX + 2 * i * dUnitX / 2; 
						arrPoints->Y = fY + 2 * i * dUnitY / 2; 
						( arrPoints + 1 )->X = arrPoints->X + dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = (fX + 2 * i * dUnitX / 2) * (1 - dFrame * 2) + (- fX + 2 * i * dUnitX / 2) * (dFrame * 2); 
						( arrPoints + 3 )->Y = (fY + 2 * i * dUnitY / 2) * (1 - dFrame * 2) + (- fY + 2 * i * dUnitY / 2) * (dFrame * 2); 
						( arrPoints + 2 )->X = (arrPoints + 3)->X + dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					oRender.FillPolygon( oBlackBrush, arrPoints, 4);
					oRender.DrawPolygon( oBlackPen, arrPoints, 4);
				}
				for(int i = 0; i < nCountLines / 2; i++)
				{
					if(bInverse)
					{
						arrPoints->X = (nWidth - fX - (2 * i + 1) * dUnitX / 2) * dFrame * 2 + (nWidth - fX - (2 * i + 1) * dUnitX / 2 + 2 * fX) * (1 - dFrame * 2); 
						arrPoints->Y = (fY + (2 * i + 1) * dUnitY / 2) * dFrame * 2 + (fY + (2 * i + 1) * dUnitY / 2 - 2 * fY) * (1 - dFrame * 2); 
						( arrPoints + 1 )->X = arrPoints->X - dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = nWidth - fX - (2 * i + 1) * dUnitX / 2 + 2 * fX; 
						( arrPoints + 3 )->Y = fY + (2 * i + 1) * dUnitY / 2 - 2 * fY; 
						( arrPoints + 2 )->X = (arrPoints + 3)->X - dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					else
					{
						arrPoints->X = (fX + (2 * i + 1) * dUnitX / 2) * dFrame * 2 + (- fX + (2 * i + 1) * dUnitX / 2) * (1 - dFrame * 2); 
						arrPoints->Y = (fY + (2 * i + 1) * dUnitY / 2) * dFrame * 2 + (- fY + (2 * i + 1) * dUnitY / 2) * (1 - dFrame * 2); 
						( arrPoints + 1 )->X = arrPoints->X + dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = -fX + (2 * i + 1) * dUnitX / 2; 
						( arrPoints + 3 )->Y = -fY + (2 * i + 1) * dUnitY / 2; 
						( arrPoints + 2 )->X = (arrPoints + 3)->X + dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					oRender.FillPolygon( oWhiteBrush, arrPoints, 4);
					oRender.DrawPolygon( oWhitePen, arrPoints, 4);
				}
				delete oBlackBrush;
				delete oWhiteBrush;
				delete oBlackPen;
				delete oWhitePen;
				delete[] arrPoints;
			}
		}		
		pBlackWhitePicture -= 4 * nWidth * (nHeight - 1);
		::memcpy(pResult, pBlackWhitePicture, 4 * nWidth * nHeight);
	}
	else
	{
		::memcpy( pBlackWhitePicture, pSource2, 4 * nWidth * nHeight);
		pBlackWhitePicture += 4 * nWidth * (nHeight - 1);
		Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBlackWhitePicture );
		if( Gdiplus::Ok == oImage.GetLastStatus() )
		{
			Gdiplus::Graphics oRender( &oImage );
			
			oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
			if( Gdiplus::Ok == oRender.GetLastStatus() )
			{
				Gdiplus::LinearGradientBrush* oBlackBrush;
				Gdiplus::LinearGradientBrush* oWhiteBrush;
				if(!bInverse)
				{
					oBlackBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(fX, fY), Gdiplus::Point(0,0), cBlackBorder, cBlackMid);									
					oWhiteBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(-fX, -fY), Gdiplus::Point(0,0), cWhiteBorder, cWhiteMid);
				}
				else
				{
					oBlackBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(nWidth - fX, fY), Gdiplus::Point(nWidth,0), cBlackBorder, cBlackMid);
					oWhiteBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(nWidth + fX, -fY), Gdiplus::Point(nWidth,0), cWhiteBorder, cWhiteMid);
				}
				oBlackBrush->SetWrapMode(Gdiplus::WrapMode::WrapModeTileFlipXY);
				oWhiteBrush->SetWrapMode(Gdiplus::WrapMode::WrapModeTileFlipXY);
				Gdiplus::PointF *arrPoints = new Gdiplus::PointF[4];
				Gdiplus::Pen* oBlackPen = new Gdiplus::Pen(cBlendBlackLine);
				Gdiplus::Pen* oWhitePen = new Gdiplus::Pen(cBlendWhiteLine);
				for(int i = 0; i < nCountLines / 2; i++)
				{
					if(bInverse)
					{
						arrPoints->X = (nWidth - fX - 2 * i * dUnitX / 2) * (2 - dFrame * 2) + (nWidth - fX - 2 * i * dUnitX / 2 + 2 * fX) * (dFrame * 2 - 1) ;
						arrPoints->Y = (fY + 2 * i * dUnitY / 2) * (2 - dFrame * 2) + (fY + 2 * i * dUnitY / 2 - 2 * fY) * (dFrame * 2 - 1); 
						( arrPoints + 1 )->X = arrPoints->X - dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = nWidth - fX - 2 * i * dUnitX / 2 + 2 * fX; 
						( arrPoints + 3 )->Y = fY + 2 * i * dUnitY / 2 - 2 * fY; 
						( arrPoints + 2 )->X = (arrPoints + 3)->X - dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					else
					{
						arrPoints->X = (fX + 2 * i * dUnitX / 2) * ( 2 - dFrame * 2) + (- fX + 2 * i * dUnitX / 2) * (dFrame * 2 - 1); 
						arrPoints->Y = (fY + 2 * i * dUnitY / 2) * ( 2 - dFrame * 2) + (- fY + 2 * i * dUnitY / 2) * (dFrame * 2 - 1); 
						( arrPoints + 1 )->X = arrPoints->X + dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = -fX + 2 * i * dUnitX / 2; 
						( arrPoints + 3 )->Y = -fY + 2 * i * dUnitY / 2; 
						( arrPoints + 2 )->X = (arrPoints + 3)->X + dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					oRender.FillPolygon( oBlackBrush, arrPoints, 4);
					oRender.DrawPolygon( oBlackPen, arrPoints, 4);
				}
				for(int i = 0; i < nCountLines / 2; i++)
				{
					if(bInverse)
					{
						arrPoints->X = (nWidth - fX - (2 * i + 1) * dUnitX / 2); 
						arrPoints->Y = (fY + (2 * i + 1) * dUnitY / 2);
						( arrPoints + 1 )->X = arrPoints->X - dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = (nWidth - fX - (2 * i + 1) * dUnitX / 2) * (dFrame * 2 - 1) + (nWidth - fX - (2 * i + 1) * dUnitX / 2 + 2 * fX) * ( 2 - dFrame * 2);  
						( arrPoints + 3 )->Y = (fY + (2 * i + 1) * dUnitY / 2) * (dFrame * 2 - 1) + (fY + (2 * i + 1) * dUnitY / 2 - 2 * fY) * ( 2 - dFrame * 2); 
						( arrPoints + 2 )->X = (arrPoints + 3)->X - dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					else
					{
						arrPoints->X = fX + ( 2 * i + 1 ) * dUnitX / 2; 
						arrPoints->Y = fY + ( 2 * i + 1 ) * dUnitY / 2; 
						( arrPoints + 1 )->X = arrPoints->X + dUnitX / 2; 
						( arrPoints + 1 )->Y = arrPoints->Y + dUnitY / 2; 
						( arrPoints + 3 )->X = (fX + ( 2 * i + 1 ) * dUnitX / 2) * (dFrame * 2 - 1) + (- fX + ( 2 * i + 1 ) * dUnitX / 2) * ( 2 - dFrame * 2); 
						( arrPoints + 3 )->Y = (fY + ( 2 * i + 1 ) * dUnitY / 2) * (dFrame * 2 - 1) + (- fY + ( 2 * i + 1 ) * dUnitY / 2) * ( 2 - dFrame * 2); 
						( arrPoints + 2 )->X = (arrPoints + 3)->X + dUnitX / 2;
						( arrPoints + 2 )->Y = (arrPoints + 3)->Y + dUnitY / 2;
					}
					oRender.FillPolygon( oWhiteBrush, arrPoints, 4);
					oRender.DrawPolygon( oWhitePen, arrPoints, 4);
				}
				delete oBlackBrush;
				delete oWhiteBrush;
				delete oBlackPen;
				delete oWhitePen;
				delete[] arrPoints;		
			}
		}		
		pBlackWhitePicture -= 4 * nWidth * (nHeight - 1);
		::memcpy(pResult, pBlackWhitePicture, 4 * nWidth * nHeight);
	}
	delete[] pBlackWhitePicture;
	return TRUE;
}

BOOL IPPEye(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nWideFade)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	int nFrame = dFrame * 256.0;					
	double dRadius = sqrt( double(nWidth * nWidth / 4 + nHeight * nHeight)) * nFrame / 255. + (1 - nFrame / 255.) * ( 0.01 * nHeight );
	int nY1 = (1 - nFrame / 255.) * ( nHeight / 2 ); 
	int nY2 =  ( nHeight / 2 ) * (1 - nFrame / 255.) + nFrame / 255. * nHeight;
	int nX  = nWidth / 2;
	int nIndex = 0;
	for( int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
		{
			double dPatterValue = 0;
			double dRadius1 = sqrt(double((nYIndex - nY1) * (nYIndex - nY1) + (nXIndex - nX) * (nXIndex - nX)));
			double dRadius2 = sqrt(double((nYIndex - nY2) * (nYIndex - nY2) + (nXIndex - nX) * (nXIndex - nX)));
			if( (nWideFade > dRadius1 - dRadius && dRadius1 - dRadius > 0 && nYIndex >= nHeight / 2) || 
				(nWideFade > dRadius2 - dRadius && dRadius2 - dRadius > 0 && nYIndex <= nHeight / 2))
			{
				if( nYIndex > nHeight / 2 )
					dPatterValue = 1 - (dRadius1 - dRadius) / nWideFade; 
				else
					dPatterValue = 1 - (dRadius2 - dRadius) / nWideFade;
			}
			if( dRadius >= dRadius1 && dRadius >= dRadius2 )
				dPatterValue = 1;
			
			pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * (1 - dPatterValue) + pBGRASource2[nIndex + 2] * dPatterValue;
			pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * (1 - dPatterValue) + pBGRASource2[nIndex + 1] * dPatterValue;
			pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * (1 - dPatterValue) + pBGRASource2[nIndex + 0] * dPatterValue;
			
		}
	}
	return TRUE;

	//int nFrame = dFrame * 256.0;					
	//int nRadius = sqrt( double(nWidth * nWidth / 4 + nHeight * nHeight)) * nFrame / 255. + (1 - nFrame / 255.) * ( 0.01 * nHeight );
	//int nSqrRadius = nRadius * nRadius;
	//int nY1 = (1 - nFrame / 255.) * ( nHeight / 2 ); 
	//int nY2 =  ( nHeight / 2 ) * (1 - nFrame / 255.) + nFrame / 255. * nHeight;
	//int nX  = nWidth / 2;
	//int nIndex = 0;
	//for( int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
	//{
	//	for( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
	//	{
	//		int nPatternValue = 0;
	//		if( (nYIndex - nY1) * (nYIndex - nY1) + (nXIndex - nX) * (nXIndex - nX) < nSqrRadius &&
	//			(nYIndex - nY2) * (nYIndex - nY2) + (nXIndex - nX) * (nXIndex - nX) < nSqrRadius)
	//		{
	//			nPatternValue = 1; 
	//		}
	//		if ( nPatternValue == 0 )
	//		{
	//			pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
	//			pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
	//			pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
	//		}
	//		else
	//		{
	//			pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
	//			pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
	//			pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
	//		}
	//	}
	//}
	//return TRUE;
}

BOOL IPPHeart(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nWideFade)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	double dTime = 1 - dFrame;
	int nXC1 = dTime * nWidth / 2 + ( 20 * nWidth / 19 ) * ( 1 - dTime ); 
	int nXC2 = dTime * nWidth / 2 + ( -nWidth / 19 ) * ( 1 - dTime ); 
	int nYC  = dTime * nHeight / 2 + ( 6 * nHeight / 5 ) * ( 1 - dTime ); 
	int nX = nWidth / 2; 
	int nY = dTime * nHeight / 2 + ( - 10 * nHeight / 9 ) * ( 1 - dTime ); 

	int nRadius = 100 * (nXC1 - nXC2) / 199;
	int nR = sqrt( double( (nXC1 - nX) * (nXC1 - nX) + (nYC - nY) * (nYC - nY) ) );
	int nSqrRadius = nRadius * nRadius;
	int nLowSide = sqrt( double( nR * nR - nSqrRadius ) );
	double dAngle = 3.14159265 / 2 - ( asin( double(nRadius*1. / nR) ) + asin( double((nXC1 - nXC2) / 2. / nR) ) ) ; 
	int nDX = nLowSide * cos( dAngle );
	double dWideRadiusFade = cos ( dAngle ) * nWideFade;

	int nIndex = 0;

	for( int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
		{
			double dPatterValue = 0;
			double dRadius1 = sqrt(double((nXIndex - nXC1) * (nXIndex - nXC1) + (nYIndex - nYC) * (nYIndex - nYC)));
			double dRadius2 = sqrt(double((nXIndex - nXC2) * (nXIndex - nXC2) + (nYIndex - nYC) * (nYIndex - nYC)));
			double dInclination1 = tan( dAngle ) * ( nXIndex - nX ) + nY - nYIndex;
			double dInclination2 = - dInclination1 + 2 * (nY - nYIndex);

			if( dRadius1 < nRadius || dRadius2 < nRadius ||
				( 0 > dInclination1 && 0 > dInclination2 && 
				 nYIndex < nYC &&
				 nXIndex < nX + nDX && nXIndex > nX - nDX))
			{
				dPatterValue = 1; 
			}

			if ( 0 == dPatterValue )
			{
				if ( dWideRadiusFade > dRadius1 - nRadius && nXIndex >= nWidth / 2)
					dPatterValue = 1 - (dRadius1 - nRadius) / dWideRadiusFade; 
				if ( dWideRadiusFade > dRadius2 - nRadius && nXIndex <= nWidth / 2)							
					dPatterValue = 1 - (dRadius2 - nRadius) / dWideRadiusFade;
				if ( dInclination1 < nWideFade &&  dInclination1 > 0 && 
					 nYIndex < nYC && nXIndex >= nWidth / 2 && nXIndex < nX + nDX )
				{
					double dValue = 1 - dInclination1 / nWideFade;
					dPatterValue = max(dValue, dPatterValue); 
				}
				if ( dInclination2 < nWideFade && dInclination2 > 0 &&
				 	 nYIndex < nYC && nXIndex <= nWidth / 2 && nXIndex > nX - nDX )
				{
					double dValue = 1 - dInclination2 / nWideFade;
					dPatterValue = max(dValue, dPatterValue); 
				}
			}
			pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * (1 - dPatterValue) + pBGRASource2[nIndex + 2] * dPatterValue;
			pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * (1 - dPatterValue) + pBGRASource2[nIndex + 1] * dPatterValue;
			pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * (1 - dPatterValue) + pBGRASource2[nIndex + 0] * dPatterValue;
		}
	}
	return TRUE;

	//double dTime = 1 - dFrame;
	//double dSqrTime = sqrt( (1 - dTime)*(1 - dTime)*(1 - dTime) );
	//int nXC1 = nWidth / 2 + ( 20 * nWidth / 19 - nWidth /2 ) * dSqrTime; 
	//int nXC2 = nWidth / 2 + ( -nWidth / 19  - nWidth / 2 ) * dSqrTime; 
	//int nYC  = nHeight / 2 + ( 6 * nHeight / 5  - nHeight / 2 ) * dSqrTime; 
	//int nX = nWidth / 2; 
	//int nY = nHeight / 2 + ( - 10 * nHeight / 9  - nHeight / 2) * dSqrTime; 

	////int nXC1 = dTime * nWidth / 2 + ( 20 * nWidth / 19 ) * ( 1 - dTime ); 
	////int nXC2 = dTime * nWidth / 2 + ( -nWidth / 19 ) * ( 1 - dTime ); 
	////int nYC  = dTime * nHeight / 2 + ( 6 * nHeight / 5 ) * ( 1 - dTime ); 
	////int nX = nWidth / 2; 
	////int nY = dTime * nHeight / 2 + ( - 10 * nHeight / 9 ) * ( 1 - dTime ); 

	//int nRadius = 6 * (nXC1 - nXC2) / 11;
	//int nR = sqrt( double( (nXC1 - nX) * (nXC1 - nX) + (nYC - nY) * (nYC - nY) ) );
	//int nSqrRadius = nRadius * nRadius;
	//int nLowSide = sqrt( double( nR * nR - nSqrRadius ) );
	//double dAngle = 3.14159265 / 2 - ( asin( double(nRadius*1. / nR) ) + asin( double((nXC1 - nXC2) / 2. / nR) ) ) ; 
	//int nDX = nLowSide * cos( dAngle );

	//int nIndex = 0;
	//for( int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
	//{
	//	for( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
	//	{
	//		int nPatternValue = 0;
	//		if( (nXIndex - nXC1) * (nXIndex - nXC1) + (nYIndex - nYC) * (nYIndex - nYC) <nSqrRadius ||
	//			(nXIndex - nXC2) * (nXIndex - nXC2) + (nYIndex - nYC) * (nYIndex - nYC) <nSqrRadius ||
	//			(nYIndex > tan( dAngle ) * ( nXIndex - nX ) + nY &&
	//			 nYIndex > -tan( dAngle ) * ( nXIndex - nX ) + nY && 
	//			 nYIndex < nYC &&
	//			 nXIndex < nX + nDX && nXIndex > nX - nDX))
	//		{
	//			nPatternValue = 1; 
	//		}
	//		if ( nPatternValue == 0 )
	//		{
	//			pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
	//			pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
	//			pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
	//		}
	//		else
	//		{
	//			pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
	//			pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
	//			pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
	//		}
	//	}
	//}
	//return TRUE;
}

BOOL IPPPixelate(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	dFrame = min(max(dFrame, 0.0), 1.0);
	int nBarsMaxCount = nWidth/2;
	int nBarsMidCount = 0.20 * nWidth;
	int nBarsMinCount = 0.10 * nWidth;
	BYTE* pSource1 = pBGRASource1;
	BYTE* pSource2 = pBGRASource2;
	BYTE* pResult = pBGRAResult;
	if( dFrame < 1./3. )
	{
		int nBarsCount = nBarsMaxCount - (nBarsMaxCount - nBarsMidCount) * sqrt( dFrame * 3  );
		int nBarWidth = nWidth / nBarsCount;
		for(int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
		{
			for(int nXIndex = 0; nXIndex < nWidth; nXIndex++ )
			{
				for(int i = 0; i < 3; i++)
				{
							
					*pResult = *(pSource1 + (min(nYIndex / nBarWidth *nBarWidth + nBarWidth / 2, nHeight - 1  )) * 4 * nWidth + 4 * min(nXIndex / nBarWidth * nBarWidth + nBarWidth / 2, nWidth - 1) + i);
					pResult++;
				}
				pResult++;
			}
		}
	}
	else if( dFrame < 2./3.)
	{
		int nBarsCount = 1;
		if( dFrame < 0.5 )
		{
			nBarsCount = nBarsMinCount * ( dFrame * 6 - 2 ) + nBarsMidCount * ( 1 - (dFrame * 6 - 2)  );
		}
		else
		{
			nBarsCount = nBarsMinCount * (1 - ( dFrame * 6 - 3 )) + nBarsMidCount * (dFrame * 6 - 3);
		}
		int nBarWidth = nWidth / nBarsCount;
		for(int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
		{
			for(int nXIndex = 0; nXIndex < nWidth; nXIndex++ )
			{
				for(int i = 0; i < 3; i++)
				{
					BYTE nS1, nS2;
					nS1 = *(pSource1 + (min(nYIndex / nBarWidth *nBarWidth + nBarWidth / 2, nHeight - 1  )) * 4 * nWidth + 4 * min(nXIndex / nBarWidth * nBarWidth + nBarWidth / 2, nWidth - 1) + i);
					nS2 = *(pSource2 + (min(nYIndex / nBarWidth *nBarWidth + nBarWidth / 2, nHeight - 1  )) * 4 * nWidth + 4 * min(nXIndex / nBarWidth * nBarWidth + nBarWidth / 2, nWidth - 1) + i);									
					*pResult =  nS2 * ( dFrame * 3 - 1 ) + nS1 * ( 1 - (dFrame * 3 - 1)  );
					pResult++;
				}
				pResult++;
			}
		}
	}
	else
	{
		int nBarsCount = nBarsMidCount - ( nBarsMidCount - nBarsMaxCount ) * ( dFrame * 3 - 2 )*( dFrame * 3 - 2 );
		int nBarWidth = nWidth / nBarsCount;
		for(int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
		{
			for(int nXIndex = 0; nXIndex < nWidth; nXIndex++ )
			{
				for(int i = 0; i < 3; i++)
				{					
					*pResult = *(pSource2 + (min(nYIndex / nBarWidth *nBarWidth + nBarWidth / 2, nHeight - 2  )) * 4 * nWidth + 4 * min(nXIndex / nBarWidth * nBarWidth + nBarWidth / 2, nWidth - 2) + i);
					pResult++;
				}
				pResult++;
			}
		}
	}
	return TRUE;
}

BOOL IPPRoll(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize = {nWidth, nHeight};
	IppiRect roiRect = { 0, 0, nWidth, nHeight};
	dFrame = min(max(dFrame, 0.0), 1.0);
	BYTE* pSource1 = pBGRASource1;
	BYTE* pSource2 = pBGRASource2;
	BYTE* pResult = pBGRAResult;
	::memcpy(pResult, pSource2, 4 * nWidth * nHeight);
	if( 1 == nDirection )
	{
		ippiRotate_8u_AC4R(
			pSource1, roiSize, 4 * nWidth, roiRect,
			pResult, 4*nWidth, roiRect,
			- 90 * dFrame, 0, 0, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
	}
	else
	{
		ippiRotateCenter_8u_AC4R(
			pSource1, roiSize, 4 * nWidth, roiRect,
			pResult, 4*nWidth, roiRect,
			90 * (dFrame), nWidth, 0, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
	}
	return TRUE;
}

BOOL IPPSlide(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize = {nWidth, nHeight};
	IppiRect roiRect = { 0, 0, nWidth, nHeight};
	dFrame = min(max(dFrame, 0.0), 1.0);
	BYTE* pSource1 = pBGRASource1;
	BYTE* pSource2 = pBGRASource2;
	BYTE* pResult = pBGRAResult;
	::memcpy(pResult, pSource2, 4 * nWidth * nHeight);
	if( 0 == nDirection )
	{
		int nMidLine = nHeight * ( 1 - dFrame);
		::memcpy(pResult + 4 * (nHeight - nMidLine) * nWidth, pSource1, 4*nMidLine *nWidth);
	}
	else if (1 == nDirection)
	{
		int nMidLine = nWidth * dFrame;
		IppiSize roiSizeR = {nWidth - nMidLine,nHeight};
		ippiCopy_8u_C4R(pSource1, 4 * nWidth, pResult + nMidLine * 4, 4 * nWidth, roiSizeR);
	}
	else if( 2 == nDirection )
	{
		int nMidLine = nHeight * (1 - dFrame);
		::memcpy(pResult, pSource1 + 4 * (nHeight - nMidLine) * nWidth, 4*nMidLine *nWidth);
	}
	else if( 3 == nDirection)
	{
		int nMidLine = nWidth * ( 1 - dFrame );
		IppiSize roiSizeR = { nMidLine , nHeight};
		ippiCopy_8u_C4R(pSource1 + int((nWidth - nMidLine) * 4), 4 * nWidth, pResult, 4 * nWidth, roiSizeR);
	}
	return TRUE;
}

BOOL IPPSlideCenter(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	IppiSize roiSize = {nWidth, nHeight};
	IppiRect roiRect = { 0, 0, nWidth, nHeight};
	dFrame = min(max(dFrame, 0.0), 1.0);
	BYTE* pSource1 = pBGRASource1;
	BYTE* pSource2 = pBGRASource2;
	BYTE* pResult = pBGRAResult;
	BYTE* pBuffer = new BYTE[4 * nWidth * nHeight];
	::memcpy(pResult, pSource2, 4 * nWidth * nHeight);
	::memcpy(pBuffer, pSource1, 4 * nWidth * nHeight);
	IPPResizeCenter( pSource1, pBuffer, nWidth, nHeight, (1 - dFrame) + 0.1*dFrame, (1 - dFrame) + 0.1*dFrame, 0, nHeight);
	//ippiResize_8u_AC4R(
	//	pSource1, roiSize, 4*nWidth, roiRect,
	//	pBuffer, 4*nWidth, roiSize,
	//	(1 - dFrame) + 0.1*dFrame, (1 - dFrame) + 0.1*dFrame, IPPI_INTER_LINEAR);
	if( 0 == nDirection )
	{
		int nMidLine = nHeight /5 * dFrame;
		IppiSize roiSizeR = {nWidth * ( (1 - dFrame) + 0.1*dFrame ), nHeight * ( (1 - dFrame) + 0.1*dFrame ) - nMidLine};
		ippiCopy_8u_C4R(pBuffer, 4 * nWidth, pResult + 4 * int(nHeight - roiSizeR.height) * nWidth + 4 * int(nWidth / 2 - nWidth * ( (1 - dFrame) + 0.1*dFrame )/2), 4 * nWidth, roiSizeR);
	}
	else if (1 == nDirection)
	{
		int nMidLine = nWidth / 5 * dFrame;
		IppiSize roiSizeR = {nWidth * ( (1 - dFrame) + 0.1*dFrame ) - nMidLine, nHeight * ( (1 - dFrame) + 0.1*dFrame )};
		ippiCopy_8u_C4R(pBuffer, 4 * nWidth, pResult + int(nWidth - roiSizeR.width) * 4 + 4 * nWidth * int(nHeight / 2 - nHeight * ( (1 - dFrame) + 0.1*dFrame )/2), 4 * nWidth, roiSizeR);
	}
	else if( 2 == nDirection )
	{
		int nMidLine = nHeight / 5 * dFrame;
		IppiSize roiSizeR = {nWidth * ( (1 - dFrame) + 0.1*dFrame ), nHeight * ( (1 - dFrame) + 0.1*dFrame ) - nMidLine};
		ippiCopy_8u_C4R(pBuffer + 4 * nWidth * nMidLine, 4 * nWidth, pResult + 4 * int(nWidth / 2 - nWidth * ( (1 - dFrame) + 0.1*dFrame )/2), 4 * nWidth, roiSizeR);
	}
	else if( 3 == nDirection)
	{
		int nMidLine = nWidth / 5 * dFrame;
		IppiSize roiSizeR = {nWidth * ( (1 - dFrame) + 0.1*dFrame ) - nMidLine, nHeight * ( (1 - dFrame) + 0.1*dFrame )};
		ippiCopy_8u_C4R(pBuffer + 4 * nMidLine, 4 * nWidth, pResult + 4 * nWidth * int(nHeight / 2 - nHeight * ( (1 - dFrame) + 0.1*dFrame )/2), 4 * nWidth, roiSizeR);
	}
	delete [] pBuffer;
	return TRUE;
}

BOOL IPPShrink(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nWidth = first.Width;
	int nHeight = first.Height;

	dFrame = min(max(dFrame, 0.0), 1.0);
	BYTE* pSource1 = pBGRASource1;
	BYTE* pSource2 = pBGRASource2;
	BYTE* pResult = pBGRAResult;

	const double dXFactor = 0.03;
	const double dYFactor = 0.03;

	IppiSize roiSize = {nWidth, nHeight};
	IppiRect roiRect = {0, 0, nWidth, nHeight};

	BYTE* pBuffer = new BYTE[4 * nWidth * nHeight];
	::memcpy(pBuffer, pSource2, 4 * nWidth * nHeight);
	if(dFrame < 0.5)
	{
		if( 0 == nDirection )
		{
			ippiResizeCenter_8u_AC4R(
				pSource1, roiSize, 4*nWidth, roiRect,
				pBuffer, 4*nWidth, roiSize,
				(1 - dFrame * 2) + dXFactor * dFrame * 2, 1, nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
		}
		else
		{
			ippiResizeCenter_8u_AC4R(
				pSource1, roiSize, 4*nWidth, roiRect,
				pBuffer, 4*nWidth, roiSize,
				1, (1 - dFrame * 2) + dYFactor * dFrame * 2, nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
		}
		::memcpy(pResult, pBuffer, 4 * nWidth * nHeight);
	}
	else
	{
		if( 0 == nDirection )
		{
			ippiResizeCenter_8u_AC4R(
				pSource1, roiSize, 4*nWidth, roiRect,
				pBuffer, 4*nWidth, roiSize,
				dXFactor, (1 - (dFrame * 2 - 1)) + dYFactor * (dFrame * 2 - 1), nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
		}
		else
		{
			ippiResizeCenter_8u_AC4R(
				pSource1, roiSize, 4*nWidth, roiRect,
				pBuffer, 4*nWidth, roiSize,
				(1 - (dFrame * 2 - 1)) + dXFactor * (dFrame * 2 - 1), dYFactor, nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
		}
		for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
		{
			for(int nXIndex = 0; nXIndex < nWidth; nXIndex++)
			{
				for(int i = 0; i < 3; i++)
				{
					*pResult = *pSource2 * ( dFrame * 2 - 1 ) + *pBuffer * (1 - ( dFrame * 2 - 1 ));
					pResult++;
					pSource2++;
					pBuffer++;
				}
				pResult++;
				pSource2++;
				pBuffer++;
			}
		}
		pBuffer -= 4 * nWidth * nHeight;
	}
	delete [] pBuffer;
	return TRUE;
}
BOOL IPPSpiral(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;
	const double dPi = 3.14159265358979323846;;
	int nWidth = first.Width;
	int nHeight = first.Height;
	dFrame = min(max(dFrame, 0.0), 1.0);

	BYTE* pSource1 = pBGRASource1;
	BYTE* pSource2 = pBGRASource2;

	double dStartAngle = 360;
	int nStep = nWidth / 2 * ( 1 - dFrame) + nWidth / 10 * dFrame;
	int nX = 0;
	int nY = 0;
	double dResKoef = 0.5 + dFrame / 2;
	double dBlendKoef = dFrame * 3. / 2.;
	double dAngle;
	if ( 4 == nDirection || 5 == nDirection)
	{
		dBlendKoef = 1 - dFrame;
		dStartAngle = 720;
		dResKoef = 1 - dFrame;
		pSource1 = pBGRASource2;
		pSource2 = pBGRASource1;
		if ( 4 == nDirection)
		{
			dStartAngle -= 180 / dPi * atan(double(nHeight * 1. / nWidth));
			nStep = nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * dPi / 180));
		}
		else
		{
			dStartAngle -= 90 + 180 / dPi * atan(double(nWidth * 1. / nHeight));
			nStep = -nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * dPi / 180));
		}
		dAngle = dStartAngle * (1. - dFrame);
	}
	else if ( 6 == nDirection || 7 == nDirection)
	{
		dBlendKoef = 2;
		dStartAngle = 720;
		dResKoef = dFrame;
		if ( 6 == nDirection)
		{
			dStartAngle -= 180 / dPi * atan(double(nHeight * 1. / nWidth));
			nStep = nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * dPi / 180));
		}
		else
		{
			dStartAngle -= 90 + 180 / dPi * atan(double(nWidth * 1. / nHeight));
			nStep = -nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * dPi / 180));
		}
		dAngle = dStartAngle * dFrame;
	}
	else
	{
		dAngle = dStartAngle * (1. - dFrame);
	}
	nX = nStep * dAngle / 360 * cos(dAngle * dPi / 180);
	nY = nStep * dAngle / 360 * sin(dAngle * dPi / 180);
	switch( nDirection )
	{
	case 0: 
		nX *= -1;
		break;
	case 2:
		nY *= -1;
		break;
	case 3:
		nX *= -1;
		nY *= -1;
		break;
	case 4:
		nX *= -1;
		nX += nWidth / 2;
		nY += nHeight / 2;
		break;
	case 5:
		nX += nWidth / 2;
		nY += nHeight / 2;
		break;
	case 6:
		nX *= -1;
		nX += nWidth / 2;
		nY += nHeight / 2;
		break;
	case 7:
		nX += nWidth / 2;
		nY += nHeight / 2;
		break;
	}
	nX = (nX <= - nWidth? -nWidth + 1: nX);
	nY = (nY <= - nHeight? -nHeight + 1: nY);
	nX = (nX >=  nWidth? nWidth - 1: nX);
	nY = (nY >=  nHeight? nHeight - 1: nY);
	int nXR = (nX < 0? 0: nX);
	int nYR = (nY < 0? 0: nY);
	int nXS = (nX > 0? 0: -nX);
	int nYS = (nY > 0? 0: -nY);
	BYTE* pBuffer = new BYTE[4 * nWidth * nHeight];
	IppiSize roiSize = {nWidth, nHeight};
	IppiRect roiRect = {0, 0, nWidth, nHeight};
	BYTE* pBuffer2 = new BYTE[4 * nWidth * nHeight];

	::memcpy(pBuffer, pSource1, 4 * nWidth * nHeight );
	ippiResize_8u_AC4R(
		pSource2, roiSize, 4*nWidth, roiRect,
		pBuffer2, 4*nWidth, roiSize,
		dResKoef, dResKoef, IPPI_INTER_LINEAR);
	roiSize.width = nWidth - nWidth * ( 1 - dResKoef );
	if(nX < 0)
		roiSize.width += nX;
	else if( roiSize.width + nX > nWidth)
	{
		roiSize.width += nWidth - roiSize.width - nX;
	}
	roiSize.height = nHeight - nHeight * ( 1 - dResKoef );
	if(nY < 0)
		roiSize.height += nY;
	else if( roiSize.height + nY > nHeight)
	{
		roiSize.height += nHeight - roiSize.height - nY;
	}
	ippiCopy_8u_C4R(pBuffer2 + 4 * nXS + 4 * nWidth * nYS, 4 * nWidth, pBuffer + 4 * nXR + 4 * nWidth * nYR, 4 * nWidth, roiSize);
	roiSize.height = nHeight;
	roiSize.width = nWidth;
	if(dBlendKoef <= 1 && 1 - dBlendKoef <= 1)
	{
		ippiAlphaCompC_8u_AC4R(pBuffer, 4 * nWidth, 255 * dBlendKoef, pSource1, 4*nWidth, 255 * ( 1 - dBlendKoef ), pBGRAResult, 4*nWidth, roiSize, ippAlphaPlus);
	}
	else
	{
		::memcpy(pBGRAResult, pBuffer, 4 * nWidth * nHeight);
	}

	delete []pBuffer2;
	delete []pBuffer;
	return TRUE;

}

BOOL IPPSimpleWave(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nWideFade)
{					
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;
	int nWidth = first.Width;
	int nHeight = first.Height;
	double dCompleteness = dFrame;

	int nIndex = 0;
	int dX = -2 * nWidth;
	int nX[4] = { 0, nWidth / 2, nWidth, 3 * nWidth / 2};
	int nRadius = nWidth / 2;
	int nSqrRadius = nRadius * nRadius;
	int nAmplit = nRadius * ( 1 - sqrt( 0.75 ));
	int nMidLine = - nAmplit * (1 - dCompleteness) + nHeight * dCompleteness;
	int nY = nMidLine + nRadius;

	for ( int nCounter = 0 ; nCounter < 4; nCounter++)
	{
		nX[nCounter] += dCompleteness * dX;
		while ( nX[nCounter] < -nWidth / 2 )
			nX[nCounter] += 2 * nWidth;
		while ( nX[nCounter] > 3 * nWidth / 2)
			nX[nCounter] -= 2 * nWidth;
	}
	for(int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for(int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
		{
			double dPatterValue = 0;
			int nCurSector = 0;
			for ( int nCounter = 0 ; nCounter < 4; nCounter++)
			{
				if ( nXIndex > nX[nCounter % 4] && nXIndex <= nX[ (nCounter + 1 ) % 4])
				{
					nCurSector = nCounter;
					break;
				}
			}
			double dRadius1 = sqrt(double((nXIndex - nX[nCurSector % 4]) * (nXIndex - nX[nCurSector % 4]) + (nYIndex - nY) * (nYIndex - nY)));
			double dRadius2 = sqrt(double((nXIndex - nX[( nCurSector + 1 ) % 4]) * (nXIndex - nX[( nCurSector + 1 ) % 4]) + (nYIndex - nY) * (nYIndex - nY)));

			double dRadius3 = sqrt(double((nXIndex - nX[nCurSector % 4]) * (nXIndex - nX[nCurSector % 4]) + (nYIndex - nY - nWideFade) * (nYIndex - nY - nWideFade)));
			double dRadius4 = sqrt(double((nXIndex - nX[( nCurSector + 1 ) % 4]) * (nXIndex - nX[( nCurSector + 1 ) % 4]) + (nYIndex - nY - nWideFade) * (nYIndex - nY - nWideFade)));
			if ( dRadius1 > nRadius && dRadius2 > nRadius && nYIndex <= nY)
			{
				dPatterValue = 1;
			}
			if ( 0 == dPatterValue )
			{
				if ( dRadius3 > nRadius && dRadius4 > nRadius && nYIndex <= nY + nWideFade)
					dPatterValue = min(dRadius3 - nRadius, dRadius4 - nRadius) / nWideFade;
			}
			pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * (1 - dPatterValue) + pBGRASource2[nIndex + 2] * dPatterValue;
			pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * (1 - dPatterValue) + pBGRASource2[nIndex + 1] * dPatterValue;
			pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * (1 - dPatterValue) + pBGRASource2[nIndex + 0] * dPatterValue;
		}
	}
	//int nIndex = 0;
	//int nAmplit = 0.1 * nHeight;
	//int nMidLine = - nAmplit * (1 - dCompleteness) + nHeight * dCompleteness;
	//for(int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
	//{
	//	for(int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
	//	{
	//		int nPatterValue = 0;
	//		double dX = (nXIndex + nYIndex) % (nWidth / 2);
	//		double dY;
	//		if( dX > nWidth / 4)
	//			dX -= nWidth / 2;
	//		dX /= nWidth / 4;
	//		dY = (dX) * (dX)  * nAmplit;
	//		
	//		if(nYIndex  <= nMidLine + dY)
	//			nPatterValue = 1;
	//		if (0 == nPatterValue)
	//		{
	//			pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
	//			pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
	//			pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
	//		}
	//		else
	//		{
	//			pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
	//			pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
	//			pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
	//		}
	//	}
	//}
	return TRUE;
}


BOOL IPPDrawAlbumList(BYTE* pResult, int nWidth, int nHeight, double dAngle, double dResizeFactor, double dXShift, double dYShift, int nWide, BOOL bCorners, double dFinalAngle)
{
	double dKoef = dResizeFactor;
	BYTE clBkColor = 220;

	double dX[4] = { 0, (nWidth - 2) * dKoef, (nWidth - 2) * dKoef, 0};
	double dY[4] = { nHeight * ( 1 - dKoef), nHeight * ( 1 - dKoef), nHeight - 1, nHeight - 1 };
	const double dsin = sin(-IPP_PI180 * (dAngle));
	const double dcos = cos(-IPP_PI180 * (dAngle));

	for ( int nIndex = 0; nIndex < 4; nIndex++ )
	{
		double dCurX = dX[nIndex];
		double dCurY = dY[nIndex];

		dX[nIndex] = (dCurY - nHeight) * dsin + dCurX * dcos + dXShift;
		dY[nIndex] = (dCurY - nHeight) * dcos - dCurX * dsin + nHeight - dYShift;

	}

	BYTE* pBuffer = pResult; 
	pBuffer += 4 * nWidth * (nHeight - 1);
	
	Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBuffer );
	if( Gdiplus::Ok == oImage.GetLastStatus() )
	{
		Gdiplus::Graphics oRender( &oImage );
		
		oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
		
		Gdiplus::Pen *oPen = new Gdiplus::Pen(Color(0 , 0, 0),3);
		
		for ( int nIndex = 0; nIndex < nWide; nIndex++ )
		{
			for(int nPoint = 0; nPoint < 4; nPoint++ )
			{
				
				if ( 0 == nPoint || 3 == nPoint )
					dX[nPoint] -= 1;
				else
					dX[nPoint] += 1;
				if ( 0 == nPoint || 1 == nPoint )
					dY[nPoint] -= 1;
				else
					dY[nPoint] += 1;
			}
			
			for(int nPoint = 0; nPoint < 4; nPoint++ )
			{
				int nTopColor = 255 * (nIndex / 5.) + 128 * (1 - nIndex / 5.);
				int nBottomColor = 255 * (nIndex / 5.) + 128 * (1 - nIndex / 5.);
				if ( nIndex == 0 && nWide > 1)
					oPen->SetColor(Color(0,0,0));
				else
					oPen->SetColor(Color(255,255,255));
				oRender.DrawLine(oPen, float(dX[nPoint % 4]), float(dY[nPoint % 4]), float(dX[(nPoint + 1) % 4]), float(dY[(nPoint + 1) % 4]));
			}
		}

		oPen->SetColor(Color( 0, 0, 0));
		oPen->SetWidth(1);

		//прорисовка углов

		if ( bCorners )
		{
			//double dCornerWidth = 0.075 * nWidth * dKoef;
			//double dCornerHeight = 0.075 * nHeight * dKoef;
			
			double dCornerWidth = 0.2 * nWidth * dKoef;
			double dCornerHeight = 0.2 * nHeight * dKoef;
			Gdiplus::PointF *arrPoints = new Gdiplus::PointF[3];
			
			double dSinF = sin( (-dFinalAngle ) * IPP_PI180);
			double dCosF = cos( (-dFinalAngle ) * IPP_PI180);

			const double dSinStep = sin( (dFinalAngle - dAngle) * IPP_PI180);
			const double dCosStep = cos( (dFinalAngle - dAngle) * IPP_PI180);

			double dX1[4] = { 0, (nWidth - 2) * dKoef, (nWidth - 2) * dKoef, 0};
			double dY1[4] = { nHeight * ( 1 - dKoef), nHeight * ( 1 - dKoef), nHeight - 1, nHeight - 1 };
			
			if ( 0 > dFinalAngle )
			{
				dX1[0] -= dSinF * nHeight * dKoef;
				dX1[3] -= dSinF * nHeight * dKoef;
				dY1[0] -= dSinF * nWidth * dKoef;
				dY1[1] -= dSinF * nWidth * dKoef;
			}
			else if ( 0 < dFinalAngle )
			{
				dX1[1] -= dSinF * nHeight * dKoef;
				dX1[2] -= dSinF * nHeight * dKoef;
				dY1[2] -= dSinF * nWidth * dKoef;
				dY1[3] -= dSinF * nWidth * dKoef;
			}

			for ( int nIndex = 0; nIndex < 4; nIndex++ )
			{
				double dCurX = dX1[nIndex];
				double dCurY = dY1[nIndex];

				dX1[nIndex] = (dCurY - nHeight) * dSinStep + dCurX * dCosStep + dXShift;
				dY1[nIndex] = (dCurY - nHeight) * dCosStep - dCurX * dSinStep + nHeight - dYShift;

			}


			for ( int nIndex = 0; nIndex < 4; nIndex++ )
			{
				int nX = 1;
				int nY = 1;

				switch(nIndex)
				{
				case 0: nX = -1; nY = -1; break;
				case 1: nX = 1; nY = -1; break;
				case 2: nX = 1; nY = 1; break;
				case 3: nX = -1; nY = 1; break;
				}

				arrPoints->X = dX1[nIndex] + nX * dCornerWidth / 2;
				arrPoints->Y = dY1[nIndex] + nY * dCornerHeight / 2;

				(arrPoints + 1)->Y = arrPoints->Y - nY * 2 * dSinStep * dCornerWidth;
				(arrPoints + 2)->Y = arrPoints->Y - nY * 2 * dCosStep * dCornerHeight;
				(arrPoints + 1)->X = arrPoints->X - nX * 2 * dCosStep * dCornerWidth;
				(arrPoints + 2)->X = arrPoints->X - nX * 2 * dSinStep * dCornerHeight;

				Gdiplus::PointF oPoint1(arrPoints->X, arrPoints->Y);
				Gdiplus::PointF oPoint2( (arrPoints + 1)->X, (arrPoints + 2)->Y );

				Gdiplus::LinearGradientBrush* oBrush = new Gdiplus::LinearGradientBrush(oPoint1, oPoint2, Color(clBkColor, clBkColor, clBkColor), Color(clBkColor, clBkColor, clBkColor));									
				oRender.FillPolygon(oBrush, arrPoints, 3);

				//точки полигона на котором у нас градиент будет
				Gdiplus::PointF *arrBrushPoints = new Gdiplus::PointF[4];
				double dGradientFactor = 1;

				double dGradientAngle = atan(double( dCornerHeight / dCornerWidth ));
				double dGradientStep = dGradientFactor * dCornerWidth * sin( dGradientAngle ) * 2;
				double dXGrStep = dGradientStep * sin( dGradientAngle );
				double dYGrStep = dGradientStep * cos( dGradientAngle );
				
				arrBrushPoints->X = (arrPoints + 1)->X;
				arrBrushPoints->Y = (arrPoints + 1)->Y;
				(arrBrushPoints + 1)->X = (arrPoints + 2)->X;
				(arrBrushPoints + 1)->Y = (arrPoints + 2)->Y;

				(arrBrushPoints + 2)->X = dX[nIndex];
				(arrBrushPoints + 2)->Y = dY[nIndex];

				Gdiplus::GraphicsPath oPath;
				oPath.AddPolygon(arrBrushPoints, 3);
				Gdiplus::PathGradientBrush *oBrush1 = new Gdiplus::PathGradientBrush(&oPath);
				oBrush1->SetCenterPoint(Gdiplus::PointF( ((arrBrushPoints + 0)->X + (arrBrushPoints + 1)->X) / 2, ((arrBrushPoints + 0)->Y + (arrBrushPoints + 1)->Y) / 2));
				oBrush1->SetCenterColor(Color( 255, 255, 255));
				int nPathCount = 1;
				Color clSurrond( 200, 200, 200);
				oBrush1->SetSurroundColors( &clSurrond, &nPathCount);

				oRender.FillPolygon(oBrush1, arrBrushPoints, 3);

				oRender.DrawLine(oPen, (arrPoints + 1)->X, (arrPoints + 1)->Y, (arrPoints + 2)->X, (arrPoints + 2)->Y);
				delete []oBrush1;

				delete []arrBrushPoints;
				delete []oBrush;

			}

			delete []arrPoints;
		}
		delete oPen;
	}
	pBuffer -= 4 * nWidth * (nHeight - 1);
	return TRUE;

}


BOOL IPPPageTurn(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
{
	double dFrame = dCompleteness;
	
	if( nType >= 5 && nType <= 8 )
		return IPPPageTurnSimple( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness );
	
	::memcpy( pBGRAResult, pBGRASource2, nWidth * nHeight * 4 );

	Ipp32f* oMapX = new Ipp32f[nWidth * nHeight];
	if( !oMapX )
		return FALSE;

	Ipp32f* oMapY = new Ipp32f[nWidth * nHeight];
	if( !oMapY )
	{
		delete [] oMapX;
		return FALSE;
	}

	// угол наклона изгиба относительно оси X
	const double dsin = sin(IPP_PI180 * 30);
	const double dcos = cos(IPP_PI180 * 30);

	// основные константы
	const double omega    = IPP_PI2 + 40 * IPP_PI180;
	const double radius   = nWidth / 3.0;
	const double factor   = 1.0 / radius;
	const double distance = dFrame * (radius + sqrt( double(nWidth * nWidth + nHeight * nHeight)));
	const double alpha    = distance * factor;
	const double offset   = distance - radius * (alpha > IPP_PI2 ? 1 : sin(distance * factor));

	// дополнительные константы для ускорения вычислений
	const double limit_alpha = distance - radius * cos(alpha - IPP_PI2);
	const double limit_omega = distance - radius * cos(omega - IPP_PI2);
	const double limit_omega_offset = limit_omega + (distance - omega * radius) * cos(IPP_PI - omega);
	const double omega_offset = omega + tan(IPP_PI - omega);
	const double distance_free = distance - omega * radius;
	const double factor_cos_omega = 1.0 / cos(IPP_PI - omega);


	int nInc  = 0;
	int nStep = 0;
	int nDist = 0;
	int nDX   = 0;
	int nDY   = 0;
	int nFX   = 0;
	int nFY   = 0;

	switch( nType )
	{
	case 9:  nInc =  1; nStep = -2 * nWidth; nDist = nWidth * (nHeight - 1); nDX = 0;          nDY = nHeight - 1; nFX =  1; nFY = -1; break;
	case 10: nInc =  1; nStep =  0;          nDist = 0;                      nDX = 0;          nDY = 0;           nFX =  1; nFY =  1; break;
	case 11: nInc = -1; nStep =  2 * nWidth; nDist = nWidth - 1;             nDX = nWidth - 1; nDY = 0;           nFX = -1; nFY =  1; break;
	case 12: nInc = -1; nStep =  0;          nDist = nWidth *  nHeight - 1;  nDX = nWidth - 1; nDY = nHeight - 1; nFX = -1; nFY = -1; break;
	}

	Ipp32f* pMapX = oMapX + nDist;
	Ipp32f* pMapY = oMapY + nDist;

	for( int nRow = 0; nRow < nHeight; ++nRow, pMapX += nStep, pMapY += nStep )
	{
		for( int nPos = 0; nPos < nWidth; ++nPos, pMapX += nInc, pMapY += nInc )
		{
			double dX = nRow * dsin + nPos * dcos;
			double dY = nRow * dcos - nPos * dsin;

			if( dX < offset )
			{
				*pMapX = -1.f;
				*pMapY = -1.f;
				continue;
			}

			double dCopyX = dX;

			if( alpha <= IPP_PI2 )
			{
				if( dX < distance )
					dX = distance - radius * asin((distance - dX) * factor);
			}
			else
			if( alpha <= omega )
			{
				if( dX <= limit_alpha )
					dX = distance - radius * (acos((distance - dX) * factor) + IPP_PI2);
				else
				if( dX < distance )
					dX = distance - radius * asin((distance - dX) * factor);
			}
			else
			if( alpha <= omega_offset )
			{
				if( dX <= limit_omega )
					dX = distance - radius * (acos((distance - dX) * factor) + IPP_PI2);
				else
				if( dX <= distance )
				{
					if( dX <= limit_omega_offset )
						dX = distance_free - (dX - limit_omega) * factor_cos_omega;
					else
						dX = distance - radius * asin((distance - dX) * factor);
				}
			}
			else
			{
				if( dX <= limit_omega )
					dX = distance - radius * (acos((distance - dX) * factor) + IPP_PI2);
				else
				if( dX <= limit_omega_offset )
					dX = distance_free - (dX - limit_omega) * factor_cos_omega;
			}

			double x = dX * dcos - dY * dsin;
			double y = dX * dsin + dY * dcos;
			
			if( x < 0 || y < 0 )
			{
				dX = distance - radius * asin((distance - dCopyX) * factor);
				x = dX * dcos - dY * dsin;
				y = dX * dsin + dY * dcos;
			}

			*pMapX = float(nFX * x + nDX);
			*pMapY = float(nFY * y + nDY);
		}
	}

	// ремэпим
	IppiSize SrcSize = {nWidth, nHeight};
	IppiRect SrcRect = {0, 0, nWidth, nHeight};

	ippiRemap_8u_AC4R( pBGRASource1, SrcSize, 4*nWidth, SrcRect, 
		oMapX, sizeof(Ipp32f)*nWidth, 
		oMapY, sizeof(Ipp32f)*nWidth, 
		pBGRAResult, 4*nWidth, SrcSize, IPPI_INTER_LINEAR);

	delete [] oMapX;
	delete [] oMapY;

	return TRUE;
}

BOOL IPPPageTurnSimple(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
{
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nTileX, nTileY;
	int nLevel = (int)((nTileSizeX + nTileSizeY - 1)*dCompleteness );
	int nLevel1 = (int)((nTileSizeX - 1)*dCompleteness );
	int nLevel2 = (int)((nTileSizeY - 1)*dCompleteness );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nTileSizeY;
		nTileY = nY / nTileSizeY;

		for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nTileSizeX;
			nTileX = nX / nTileSizeX;

			nSourceX = nX;
			nSourceY = nY;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (nType == c_nPageTurnLeftBottom)
			{
				if ( nPatternX + nPatternY <= nLevel ) 
					nPatternValue = 1;
				else if ( nPatternX <= nLevel && nPatternY <= nLevel)
				{
					nSourceX = nTileX * nTileSizeX + nLevel - nPatternY;
					nSourceY = nTileY * nTileSizeY + nLevel - nPatternX;
				}
			}
			else if (nType == c_nPageTurnLeftTop)
			{
				if ( nPatternY - nPatternX >= nTileSizeY - 1 - nLevel ) 
					nPatternValue = 1;
				else if ( nPatternX <= nLevel && nPatternY >= nTileSizeY - 1 - nLevel)
				{
					nSourceX = nTileX * nTileSizeX + nPatternY - (nTileSizeY - 1 - nLevel);
					nSourceY = nTileY * nTileSizeY + nPatternX + (nTileSizeY - 1 - nLevel);
				}
			}
			else if (nType == c_nPageTurnRightBottom)
			{
				if ( nPatternX - nPatternY >= nTileSizeX - 1 - nLevel ) 
					nPatternValue = 1;
				else if ( nPatternX >= nTileSizeX - 1 - nLevel && nPatternY <= nLevel)
				{
					nSourceX = nTileX * nTileSizeX + nPatternY + (nTileSizeX - 1 - nLevel);
					nSourceY = nTileY * nTileSizeY + nPatternX - (nTileSizeX - 1 - nLevel);
				}
			}
			else if (nType == c_nPageTurnRightTop)
			{
				if ( nPatternX + nPatternY >= nTileSizeX - 1 + nTileSizeY - 1 - nLevel1 - nLevel2 ) 
					nPatternValue = 1;
				else if ( nPatternX >= nTileSizeX - 1 - nLevel2 - nLevel1 && nPatternY >= nTileSizeY - 1 - nLevel2 - nLevel1)
				{
					nSourceX = nTileX * nTileSizeX + nTileSizeX - 1 + (nTileSizeY - 1 - nLevel2 - nLevel1) - nPatternY;
					nSourceY = nTileY * nTileSizeY + nTileSizeY - 1 + (nTileSizeX - 1 - nLevel2 - nLevel1) - nPatternX;
				}
			}

			// combine colors
			if (nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight || nPatternValue == 1)
			{
				pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
			}
			else
			{
				nSourceIndex = 4*(nSourceX + nSourceY*nWidth);
				
				pBGRAResult[nIndex + 2] = pBGRASource1[nSourceIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource1[nSourceIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource1[nSourceIndex + 0];
			}
		}
	}

	return TRUE;
}
BOOL IPPPageTurnWhiteBack(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
{
	if( nType >= 5 && nType <= 8 )
		return IPPPageTurnSimple( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness );

	BOOL bWhiteBack;
	if ( c_nPageTurnLeftTopWhiteBack3d <= nType && c_nPageTurnRightTopWhiteBack3d >= nType )
		bWhiteBack = TRUE;
	else
		bWhiteBack = FALSE;

	BYTE* pBuffer = new BYTE[4 * nWidth * nHeight];

	::memcpy( pBuffer, pBGRASource1, 4 * nWidth * nHeight);

	// угол наклона изгиба относительно оси X
	const double dsin = sin(IPP_PI180 * 30);
	const double dcos = cos(IPP_PI180 * 30);

	// основные константы
	const double omega    = IPP_PI2 + 40 * IPP_PI180;
	const double radius   = nWidth / 3.0;
	const double factor   = 1.0 / radius;
	const double distance = dCompleteness * (radius + sqrt( double(nWidth * nWidth + nHeight * nHeight)));
	const double alpha    = distance * factor;

	int nInc  = 0;
	int nStep = 0;
	int nDist = 0;
	int nDX   = 0;
	int nDY   = 0;
	int nFX   = 0;
	int nFY   = 0;
	int nTypeNoBack = 0;

	if ( c_nPageTurnLeftTopWhiteBack3d == nType || c_nPageTurnLeftTop3d == nType )
	{
		nInc =  1; nStep = -2 * nWidth; nDist = nWidth * (nHeight - 1); nDX = 0;          nDY = nHeight - 1; nFX =  1; nFY = -1; nTypeNoBack = c_nPageTurnLeftTop3d;
	}
	else if ( c_nPageTurnLeftBottomWhiteBack3d == nType || c_nPageTurnLeftBottom3d == nType)
	{
		nInc =  1; nStep =  0;          nDist = 0;                      nDX = 0;          nDY = 0;           nFX =  1; nFY =  1; nTypeNoBack = c_nPageTurnLeftBottom3d;
	}
	else if ( c_nPageTurnRightBottomWhiteBack3d == nType || c_nPageTurnRightBottom3d == nType)
	{
		nInc = -1; nStep =  2 * nWidth; nDist = nWidth - 1;             nDX = nWidth - 1; nDY = 0;           nFX = -1; nFY =  1; nTypeNoBack = c_nPageTurnRightBottom3d;
	}
	else if ( c_nPageTurnRightTopWhiteBack3d == nType || c_nPageTurnRightTop3d == nType)
	{
		nInc = -1; nStep =  0;          nDist = nWidth *  nHeight - 1;  nDX = nWidth - 1; nDY = nHeight - 1; nFX = -1; nFY = -1; nTypeNoBack = c_nPageTurnRightTop3d;
	}

	int nIndex = 4 * nDist;
	for ( int nYIndex = 0; nYIndex < nHeight; nYIndex++, nIndex += nStep * 4)
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 * nInc)
		{

			double dX = nYIndex * dsin + nXIndex * dcos;

			double dCurveLine = (distance - radius * IPP_PI2);
			double dFadeLine = (distance - radius * omega)/2;

			if ( dX <= dCurveLine)
			{
				if ( bWhiteBack )
				{
					BYTE cBorderColor = 128;
					BYTE cMiddleColor = 255;
					double dMidLine = dCurveLine / 2;
					if ( dX <= dMidLine)
					{
						double dKoef = 1 - dX / dMidLine;
						pBuffer[nIndex + 0] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						pBuffer[nIndex + 1] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						pBuffer[nIndex + 2] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						pBuffer[nIndex + 3] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						continue;
					}
					else
					{
						double dKoef = (dX - dMidLine) / (dMidLine);
						pBuffer[nIndex + 0] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						pBuffer[nIndex + 1] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						pBuffer[nIndex + 2] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						pBuffer[nIndex + 3] = cBorderColor * dKoef + (1 - dKoef) * cMiddleColor;
						continue;
					}
				}
			}
			else if( dX < dFadeLine + distance  )
			{
					BYTE cBlackColor = 0;
					double dKoef = 1 - (distance + dFadeLine  - dX) / (dFadeLine + radius * IPP_PI2);
					dKoef = sqrt(dKoef);
					
					pBuffer[nIndex + 0] = cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 0];
					pBuffer[nIndex + 1] = cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 1];
					pBuffer[nIndex + 2] = cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 2];
					pBuffer[nIndex + 3] = cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 3];
					continue;
			}



		}
	}
	::memcpy(pBGRASource1, pBuffer, 4 * nWidth * nHeight);
	IPPPageTurn(pBuffer, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nTypeNoBack, dCompleteness);
	delete []pBuffer;
	return TRUE;
}

BOOL IPPPageTurnAlbum(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
{	
	double dFrame = dCompleteness;
	double dResizeFactor = 0.6;
	double dFirstAngleFactor =  -2;
	double dLastAngleFactor = 1;
	BYTE clBkColor = 220;
	Ipp8u backColor[4] = { clBkColor, clBkColor, clBkColor, 255 };
	BOOL bCorners = 0;
//	int nBorder  = 10;
	int nBorder  = 0.02 * sqrt( double(nHeight * nHeight + nWidth * nWidth));
	int nCurType = 0;

	int nTypeCorner = 0;
	
	switch( nType )
	{
	case c_nPageTurnAlbumLeftTop:             nCurType = c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 0; break;
	case c_nPageTurnAlbumLeftTopCorners1:     nCurType = c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 1; break;
	case c_nPageTurnAlbumLeftTopCorners2:     nCurType = c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 2; break;
	case c_nPageTurnAlbumLeftTopCorners3:     nCurType = c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 3; break;
	case c_nPageTurnAlbumLeftTopCorners4:     nCurType = c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 4; break;

	case c_nPageTurnAlbumRightTop:            nCurType = c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 0; break;
	case c_nPageTurnAlbumRightTopCorners1:    nCurType = c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 1; break;
	case c_nPageTurnAlbumRightTopCorners2:    nCurType = c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 2; break;
	case c_nPageTurnAlbumRightTopCorners3:    nCurType = c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 3; break;
	case c_nPageTurnAlbumRightTopCorners4:    nCurType = c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 4; break;

	case c_nPageTurnAlbumRightBottom:         nCurType = c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 0; break;
	case c_nPageTurnAlbumRightBottomCorners1: nCurType = c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 1; break;
	case c_nPageTurnAlbumRightBottomCorners2: nCurType = c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 2; break;
	case c_nPageTurnAlbumRightBottomCorners3: nCurType = c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 3; break;
	case c_nPageTurnAlbumRightBottomCorners4: nCurType = c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 4; break;

	case c_nPageTurnAlbumLeftBottom:          nCurType = c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 0; break;
	case c_nPageTurnAlbumLeftBottomCorners1:  nCurType = c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 1; break;
	case c_nPageTurnAlbumLeftBottomCorners2:  nCurType = c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 2; break;
	case c_nPageTurnAlbumLeftBottomCorners3:  nCurType = c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 3; break;
	case c_nPageTurnAlbumLeftBottomCorners4:  nCurType = c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 4; break;
	}

	if ( c_nPageTurnAlbumLeftTop <= nType && c_nPageTurnAlbumLeftBottom >= nType )
		bCorners = FALSE;
	else 
		bCorners = TRUE;

	if ( dFrame < 1. / 3. )
	{
		double dAngle = dFirstAngleFactor * dFrame * 3;
		double dKoef = 1 * (1 - dFrame * 3) + dResizeFactor * dFrame * 3;
		double dXShift = nWidth * (1 - dKoef) / 2, dYShift = nHeight * (1 - dKoef) / 2;
		IppiSize roiSize = { nWidth, nHeight};
		BYTE* pBuffer = new BYTE[ 4 * nWidth * nHeight ];
		ippiSet_8u_C4R(backColor, pBuffer, 4 * nWidth, roiSize);


		IppiRect sourceRect = {0, 0, nWidth * dKoef, nHeight * dKoef};
		IppiRect resultRect = {0, 0, nWidth, nHeight};
		IPPResizeCenter(pBGRASource1, pBGRASource1, nWidth, nHeight, dKoef, dKoef, 0, nHeight);
		ippiRotate_8u_C4R(pBGRASource1, roiSize, 4 * nWidth, sourceRect, pBuffer, 4 * nWidth, resultRect, dAngle, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
		
		IPPDrawAlbumList(pBuffer, nWidth, nHeight, dAngle, dKoef, dXShift, dYShift, nBorder, bCorners, dFirstAngleFactor);
		::memcpy(pBGRAResult, pBuffer, 4 * nWidth * nHeight);
		delete []pBuffer;
	}

	else if ( dFrame < 2. / 3. )
	{

		BYTE* pBuffer1 = new BYTE[ 4 * nWidth * nHeight ];
		BYTE* pBuffer2 = new BYTE[ 4 * nWidth * nHeight ];

		double dXShift = nWidth / 2 * (1 - dResizeFactor), dYShift = nHeight / 2 * (1 - dResizeFactor);
		
		IppiSize roiSize = { nWidth, nHeight};
		IppiRect sourceRect = {0, 0, nWidth * dResizeFactor, nHeight * dResizeFactor};
		IppiRect resultRect = {0, 0, nWidth, nHeight};
		
		double dKoef = dResizeFactor;

		ippiSet_8u_C4R(backColor, pBuffer1, 4 * nWidth, roiSize);
		ippiSet_8u_C4R(backColor, pBuffer2, 4 * nWidth, roiSize);

		IPPResizeCenter(pBGRASource1, pBGRASource1, nWidth, nHeight, dResizeFactor, dResizeFactor, 0, nHeight);
		ippiRotate_8u_C4R(pBGRASource1, roiSize, 4 * nWidth, sourceRect, pBuffer1, 4 * nWidth, resultRect, dFirstAngleFactor, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
		IPPResizeCenter(pBGRASource2, pBGRASource2, nWidth, nHeight, dResizeFactor, dResizeFactor, 0, nHeight);
		ippiRotate_8u_C4R(pBGRASource2, roiSize, 4 * nWidth, sourceRect, pBuffer2, 4 * nWidth, resultRect, dLastAngleFactor, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

		IPPDrawAlbumList(pBuffer1, nWidth, nHeight, dFirstAngleFactor, dResizeFactor, dXShift, dYShift, nBorder, bCorners, dFirstAngleFactor);
		IPPDrawAlbumList(pBuffer2, nWidth, nHeight, dLastAngleFactor, dResizeFactor, dXShift, dYShift, nBorder, bCorners, dLastAngleFactor);
		IPPPageTurnWhiteBack(pBuffer1 , pBuffer2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nCurType, dFrame * 3 - 1);

		delete []pBuffer1;
		delete []pBuffer2;
	}
	else
	{
		BYTE* pBuffer = new BYTE[ 4 * nWidth * nHeight ];
		double dAngle = dLastAngleFactor * ( 1 - (dFrame * 3 - 2));
		double dKoef = 1 * (dFrame * 3 - 2) + dResizeFactor * ( 1 - (dFrame * 3 - 2));
		double dXShift = nWidth / 2 * (1 - dKoef), dYShift = nHeight / 2 * (1 - dKoef);

		IppiSize roiSize = { nWidth, nHeight};
		IppiRect sourceRect = {0, 0, nWidth * dKoef, nHeight * dKoef};
		IppiRect resultRect = {0, 0, nWidth, nHeight};

		ippiSet_8u_C4R(backColor, pBuffer, 4 * nWidth, roiSize);

		IPPResizeCenter(pBGRASource2, pBGRASource2, nWidth, nHeight, dKoef, dKoef, 0, nHeight);
		ippiRotate_8u_C4R(pBGRASource2, roiSize, 4 * nWidth, sourceRect, pBuffer, 4 * nWidth, resultRect, dAngle, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

		IPPDrawAlbumList(pBuffer, nWidth, nHeight, dAngle, dKoef, dXShift, dYShift, nBorder, bCorners, dLastAngleFactor);

		::memcpy(pBGRAResult, pBuffer, 4 * nWidth * nHeight);
		delete []pBuffer;
	}
	return TRUE;
}

BOOL IPPPinwheelCurved(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, MaskPinwheelCurved* pMask, int nFadeWide)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;

	BYTE* pBGRAResult  = result.Pixels;

	int nHeight = first.Height;
	int nWidth = first.Width;

	IppiSize roiSize;

	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiRect roiRect = { 0, 0, nWidth, nHeight};

	pMask->GenerateMask( nWidth, nHeight, nType);
	int nFrame = dFrame * 256.0;
	int nIndex = 0;

	for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
	{
		for( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
		{
			if(pMask->g_pMask[nYIndex*nWidth + nXIndex] < nFrame - nFadeWide / 2)
			{
				pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
			}
			else if(pMask->g_pMask[nYIndex*nWidth + nXIndex] <= nFrame && pMask->g_pMask[nYIndex*nWidth + nXIndex] >= nFrame - nFadeWide / 2 &&(nFadeWide > 0) )
			{
				double dValue = 0.5 - (-pMask->g_pMask[nYIndex*nWidth + nXIndex] + nFrame) * 1. / nFadeWide;
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * dValue + pBGRASource2[nIndex + 2] * (1 - dValue);
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * dValue + pBGRASource2[nIndex + 1] * (1 - dValue);
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * dValue + pBGRASource2[nIndex + 0] * (1 - dValue);
			}
			else if(pMask->g_pMask[nYIndex*nWidth + nXIndex] - nFrame <= nFadeWide / 2 && pMask->g_pMask[nYIndex*nWidth + nXIndex] >= nFrame && (nFadeWide > 0) )
			{
				double dValue = (pMask->g_pMask[nYIndex*nWidth + nXIndex] - nFrame) * 1. / nFadeWide + 0.5;
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * dValue + pBGRASource2[nIndex + 2] * (1 - dValue);
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * dValue + pBGRASource2[nIndex + 1] * (1 - dValue);
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * dValue + pBGRASource2[nIndex + 0] * (1 - dValue);
			}
			else
			{
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
			}


		}
	}
	return TRUE;
}

BOOL IPPPuzzle(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, PuzzleUtility *pUtility, int nXCount, int nYCount, int nFlyTime, int nCorners, BOOL bPuzzle, int nShuffleType, BOOL bCollapse, int nType)
{
	
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nHeight = first.Height;
	int nWidth = first.Width;
		
	BYTE *pSource1 = pBGRASource1;
	BYTE *pSource2 = pBGRASource2;
	BYTE *pResult  = pBGRAResult;
	
	int nSize = 4 * nWidth * nHeight;

	if ( bCollapse )
	{
		dFrame = 1 - dFrame;
		pSource1 = pBGRASource2;
		pSource2 = pBGRASource1;
	}

	double dFadeTime = 0.1;
	double dTime = dFrame / (1 - dFadeTime);

	const int nCount = nXCount * nYCount;

	PuzzleFigure *pFigures = new PuzzleFigure[nCount];
	
	int nStep = dTime * nCount;
	//nStep = (nStep >= (nCount + 1)? nCount : nStep);
	if ( dFrame < 1 - dFadeTime )
		::memcpy(pResult, pSource1, nSize);
	else
		::memcpy(pResult, pSource2, nSize);

	int nFigWidth = nWidth / nXCount;
	int nFigHeight = nHeight / nYCount;

	pUtility->GeneratePuzzle(nYCount, nXCount, nType);
	pUtility->GenerateRandomShuffle(nXCount, nYCount, nShuffleType, nType);
	pUtility->GenerateCorners(nWidth, nHeight, nCount, nCorners, nType);

	if ( bPuzzle )
	{
		for (int nYIndex = 0, nIndex = 0; nYIndex < nYCount; nYIndex ++ )
		{
			for ( int nXIndex = 0; nXIndex < nXCount; nXIndex++, nIndex += 2 )
			{
				pFigures[nXIndex + nXCount * nYIndex].SetValue( nFigWidth, nFigHeight, pUtility->g_pHorizontalFactor[nIndex], pUtility->g_pVerticalFactor[nIndex + 1], pUtility->g_pHorizontalFactor[nIndex + 1], pUtility->g_pVerticalFactor[nIndex], pSource2, nWidth, nHeight, nXIndex * nWidth / nXCount, nYIndex * nHeight / nYCount );
			}
		}
	}
	else
	{
		for (int nYIndex = 0, nIndex = 0; nYIndex < nYCount; nYIndex ++ )
		{
			for ( int nXIndex = 0; nXIndex < nXCount; nXIndex++, nIndex += 2 )
			{
				pFigures[nXIndex + nXCount * nYIndex].SetValue( nFigWidth, nFigHeight, 0, 0, 0, 0, pSource2, nWidth, nHeight, nXIndex * nWidth / nXCount, nYIndex * nHeight / nYCount );
			}
		}
	}


	//
	// Создаем различные графические инструменты для отрисовки
	//

	int nOffset = 4 * nWidth * (nHeight - 1);
	
	Gdiplus::Bitmap oMainImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pResult + nOffset);

	if( Gdiplus::Ok != oMainImage.GetLastStatus() )
		return FALSE;

	Gdiplus::Graphics oMainRender( &oMainImage );
	
	oMainRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	oMainRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
	
	Gdiplus::Bitmap oSourceImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pSource2 + nOffset );
	if( Gdiplus::Ok != oSourceImage.GetLastStatus() )
		return FALSE;
	
	Gdiplus::Bitmap oBrushImage( nWidth, nHeight, PixelFormat32bppARGB);
	if( Gdiplus::Ok != oBrushImage.GetLastStatus() )
		return FALSE;

	Gdiplus::Graphics oBrushRender( &oBrushImage );
	
	oBrushRender.DrawImage( &oSourceImage, 0, 0);
		
	Gdiplus::TextureBrush oBrush( &oBrushImage, Gdiplus::WrapModeTile);
		
	//
	// 
	//



	int nFullWidth  = 3 * nFigWidth;
	int nFullHeight = 3 * nFigHeight;

	int nXStart = 0; 
	int nYStart = 0;

	if ( dFrame < 1 - dFadeTime )
	{
		int nIndex = 0;
		
		int nCurFlyTime = 1;

		for ( ; nIndex < nFlyTime; nIndex++, nCurFlyTime++)
		{
			int nCurIndex = pUtility->g_pShuffle[nIndex];
			if ( nIndex < nStep + nFlyTime && nIndex >= nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				double dKoef = (nCurFlyTime - (nIndex + 1 - dTime * nCount)) / nCurFlyTime;
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_nXCoorSource * dKoef + ( 1 - dKoef ) * (nXStart), pFigures[nCurIndex].m_nYCoorSource * dKoef + ( 1 - dKoef ) * (nYStart), nWidth, nHeight, 1);
			}
			else if ( nIndex < nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush,  pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, nWidth, nHeight, 1);
			}
		}

		for ( ; nIndex < nCount; nIndex++ )
		{
			int nCurIndex = pUtility->g_pShuffle[nIndex];
			if ( nIndex < nStep + nFlyTime && nIndex >= nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				double dKoef = (nFlyTime - (nIndex + 1  - dTime * nCount)) / nFlyTime;
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush,  pFigures[pUtility->g_pShuffle[nIndex]].m_nXCoorSource * dKoef + ( 1 - dKoef ) * (nXStart), pFigures[pUtility->g_pShuffle[nIndex]].m_nYCoorSource * dKoef + ( 1 - dKoef ) * (nYStart), nWidth, nHeight, 1);
			}
			else if ( nIndex < nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, nWidth, nHeight, 1);
			}

		}
	}
	else
	{
		double dCurTime = (dFrame - (1 - dFadeTime)) / dFadeTime;
		for ( int nIndex = 0; nIndex <= nCount - 1; nIndex++)
			pFigures[pUtility->g_pShuffle[nIndex]].Draw(&oMainRender, &oBrush, pFigures[pUtility->g_pShuffle[nIndex]].m_nXCoorSource, pFigures[pUtility->g_pShuffle[nIndex]].m_nYCoorSource, nWidth, nHeight, 1 - dCurTime);
	}

	delete []pFigures;
	return TRUE;
}

BOOL IPPPuzzle(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, PuzzleUtility *pUtility, int nXCount, const int nYCount, const int nFlyTime, int nCorners, BOOL bPuzzle, int nShuffleType, BOOL bCollapse)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nHeight = first.Height;
	int nWidth = first.Width;

	for ( int nYIndex = 0, nIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
		{
			pBGRASource1[nIndex + 3] = 255;
			pBGRASource2[nIndex + 3] = 255;
			pBGRAResult[nIndex + 3] = 255;
		}
	}

	BYTE *pSource1 = pBGRASource1;
	BYTE *pSource2 = pBGRASource2;
	BYTE *pResult  = pBGRAResult;
	
	int nSize = 4 * nWidth * nHeight;

	if ( bCollapse )
	{
		dFrame = 1 - dFrame;
		pSource1 = pBGRASource2;
		pSource2 = pBGRASource1;
	}

	double dFadeTime = 0.1;
	double dTime = dFrame / (1 - dFadeTime);

	const int nCount = nXCount * nYCount;

	PuzzleFigure *pFigures = new PuzzleFigure[nCount];
	
	int nStep = dTime * nCount;
	//nStep = (nStep >= (nCount + 1)? nCount : nStep);
	if ( dFrame < 1 - dFadeTime )
		::memcpy(pResult, pSource1, nSize);
	else
		::memcpy(pResult, pSource2, nSize);

	int nFigWidth = nWidth / nXCount;
	int nFigHeight = nHeight / nYCount;

	pUtility->GeneratePuzzle(nYCount, nXCount, nType);
	pUtility->GenerateRandomShuffle(nXCount, nYCount, nShuffleType, nType);
	pUtility->GenerateCorners(nWidth, nHeight, nCount, nCorners, nType);

	if ( bPuzzle )
	{
		for (int nYIndex = 0, nIndex = 0; nYIndex < nYCount; nYIndex ++ )
		{
			for ( int nXIndex = 0; nXIndex < nXCount; nXIndex++, nIndex += 2 )
			{
				pFigures[nXIndex + nXCount * nYIndex].SetValue( nFigWidth, nFigHeight, pUtility->g_pHorizontalFactor[nIndex], pUtility->g_pVerticalFactor[nIndex + 1], pUtility->g_pHorizontalFactor[nIndex + 1], pUtility->g_pVerticalFactor[nIndex], pSource2, nWidth, nHeight, nXIndex * nWidth / nXCount, nYIndex * nHeight / nYCount );
			}
		}
	}
	else
	{
		for (int nYIndex = 0, nIndex = 0; nYIndex < nYCount; nYIndex ++ )
		{
			for ( int nXIndex = 0; nXIndex < nXCount; nXIndex++, nIndex += 2 )
			{
				pFigures[nXIndex + nXCount * nYIndex].SetValue( nFigWidth, nFigHeight, 0, 0, 0, 0, pSource2, nWidth, nHeight, nXIndex * nWidth / nXCount, nYIndex * nHeight / nYCount );
			}
		}
	}


	//
	// Создаем различные графические инструменты для отрисовки
	//

	int nOffset = 4 * nWidth * (nHeight - 1);
	
	Gdiplus::Bitmap oMainImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pResult + nOffset);

	if( Gdiplus::Ok != oMainImage.GetLastStatus() )
		return FALSE;

	Gdiplus::Graphics oMainRender( &oMainImage );
	
	oMainRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	oMainRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
	
	Gdiplus::Bitmap oSourceImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pSource2 + nOffset );
	if( Gdiplus::Ok != oSourceImage.GetLastStatus() )
		return FALSE;
	
	Gdiplus::Bitmap oBrushImage( nWidth, nHeight, PixelFormat32bppARGB);
	if( Gdiplus::Ok != oBrushImage.GetLastStatus() )
		return FALSE;

	Gdiplus::Graphics oBrushRender( &oBrushImage );
	
	oBrushRender.DrawImage( &oSourceImage, 0, 0);
		
	Gdiplus::TextureBrush oBrush( &oBrushImage, Gdiplus::WrapModeTile);
		
	//
	// 
	//



	int nFullWidth  = 3 * nFigWidth;
	int nFullHeight = 3 * nFigHeight;

	int nXStart = 0; 
	int nYStart = 0;

	if ( dFrame < 1 - dFadeTime )
	{
		int nIndex = 0;
		
		int nCurFlyTime = 1;

		for ( ; nIndex < nFlyTime; nIndex++, nCurFlyTime++)
		{
			int nCurIndex = pUtility->g_pShuffle[nIndex];
			if ( nIndex < nStep + nFlyTime && nIndex >= nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				double dKoef = (nCurFlyTime - (nIndex + 1 - dTime * nCount)) / nCurFlyTime;
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_nXCoorSource * dKoef + ( 1 - dKoef ) * (nXStart), pFigures[nCurIndex].m_nYCoorSource * dKoef + ( 1 - dKoef ) * (nYStart), nWidth, nHeight, 1);
			}
			else if ( nIndex < nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush,  pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, nWidth, nHeight, 1);
			}
		}

		for ( ; nIndex < nCount; nIndex++ )
		{
			int nCurIndex = pUtility->g_pShuffle[nIndex];
			if ( nIndex < nStep + nFlyTime && nIndex >= nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				double dKoef = (nFlyTime - (nIndex + 1  - dTime * nCount)) / nFlyTime;
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush,  pFigures[pUtility->g_pShuffle[nIndex]].m_nXCoorSource * dKoef + ( 1 - dKoef ) * (nXStart), pFigures[pUtility->g_pShuffle[nIndex]].m_nYCoorSource * dKoef + ( 1 - dKoef ) * (nYStart), nWidth, nHeight, 1);
			}
			else if ( nIndex < nStep)
			{
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, pUtility->g_pCorners[nIndex]);
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_nXCoorSource, pFigures[nCurIndex].m_nYCoorSource, nWidth, nHeight, 1);
			}

		}
	}
	else
	{
		double dCurTime = (dFrame - (1 - dFadeTime)) / dFadeTime;
		for ( int nIndex = 0; nIndex <= nCount - 1; nIndex++)
			pFigures[pUtility->g_pShuffle[nIndex]].Draw(&oMainRender, &oBrush, pFigures[pUtility->g_pShuffle[nIndex]].m_nXCoorSource, pFigures[pUtility->g_pShuffle[nIndex]].m_nYCoorSource, nWidth, nHeight, 1 - dCurTime);
	}

	delete []pFigures;
	return TRUE;

}



BOOL IPPTiles(const SmartImage &first, const SmartImage &last, SmartImage &result, int nFigureType, double dFrame, double dSideFactor, int nType, PuzzleUtility* pUtility, int nFlyTime, int nShuffleType, int nStartType, BOOL bCollapse)
{

	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nHeight = first.Height;
	int nWidth = first.Width;

	for ( int nYIndex = 0, nIndex = 0; nYIndex < nHeight; nYIndex++ )
	{
		for ( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
		{
			pBGRASource1[nIndex + 3] = 255;
			pBGRASource2[nIndex + 3] = 255;
			pBGRAResult[nIndex + 3] = 255;
		}
	}

	BYTE *pSource1 = pBGRASource1;
	BYTE *pSource2 = pBGRASource2;
	BYTE *pResult  = pBGRAResult;

	double dSide = nWidth / dSideFactor; // 10 на 10 около 15, 

	int nSize = 4 * nWidth * nHeight;

	if ( bCollapse )
	{
		dFrame = 1 - dFrame;
		pSource1 = pBGRASource2;
		pSource2 = pBGRASource1;
	}

	//
	// Создаем различные графические инструменты для отрисовки
	//

	int nOffset = 4 * nWidth * (nHeight - 1);
	
	Gdiplus::Bitmap oMainImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pResult + nOffset);

	if( Gdiplus::Ok != oMainImage.GetLastStatus() )
		return FALSE;

	Gdiplus::Graphics oMainRender( &oMainImage );
	
	oMainRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	oMainRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
	
	Gdiplus::Bitmap oSourceImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pSource2 + nOffset );
	if( Gdiplus::Ok != oSourceImage.GetLastStatus() )
		return FALSE;
	
	Gdiplus::Bitmap oBrushImage( nWidth, nHeight, PixelFormat32bppARGB);
	if( Gdiplus::Ok != oBrushImage.GetLastStatus() )
		return FALSE;

	Gdiplus::Graphics oBrushRender( &oBrushImage );
	
	oBrushRender.DrawImage( &oSourceImage, 0, 0);
		
	Gdiplus::TextureBrush oBrush( &oBrushImage, Gdiplus::WrapModeTileFlipXY);
		
	//
	// Создаем список кусочков
	//

	int nXCount = 0;
	int nYCount = 0;

	//
	// Расчет числа кусочков
	//
	if ( 6 == nFigureType )
	{
		nXCount = nWidth / int(1.5 * dSide) + 1;
		if( nWidth % int(1.5 * dSide) > 0 )
			nXCount++;
		
		nYCount = nHeight / int(sqrt(3.0) * dSide) + 1;
		if( nHeight % int(sqrt(3.0) * dSide) > 0 )
			nYCount++;

	}
	else if ( 3 == nFigureType )
	{
		nXCount = nWidth / int(0.5 * dSide) + 1;
		if( nWidth % int(0.5 * dSide) > 0 )
			nXCount++;

		nYCount = nHeight / int(sqrt(3.0) * 0.5 * dSide) + 1;
		if( nHeight % int(sqrt(3.0) * 0.5 * dSide) > 0 )
			nYCount++;
	}
	else if ( 41 == nFigureType )
	{
		double dRadius = dSide / sqrt(2.0); // радиус описанной окружности

		nXCount = nWidth / int(dRadius) + 2;

		nYCount = nHeight / int(2 * dRadius) + 2;
	}
	else if ( 42 == nFigureType )
	{
		double dXShift = dSide;
		double dYShift = dSide * (12.0 / 25.0);

		nXCount = nWidth / int(dXShift) + 2;

		nYCount = nHeight / int(dYShift) + 2;
	}
	else if ( 43 == nFigureType )
	{
		double dAngle =  atan(25.0 / 24.0) - IPP_PI4;
		double dXShift = dSide * sqrt( 1.0 / 4.0 + 144.0 / 625.0 ) * cos(dAngle);

		nXCount = (nWidth + nHeight * tan(dAngle)) / int(dXShift) + 2;

		nYCount = (nHeight + nWidth * tan(dAngle)) / int(dXShift) + 2;
	}
	int nCount = nXCount * nYCount;

	Figure *pFigures = new Figure[nCount];

	pUtility->GenerateRandomShuffle(nXCount, nYCount, nShuffleType, nType);
	pUtility->GenerateCorners(nWidth, nHeight, nCount, nStartType, nType);

	//
	// Заполняем данные о кусочках
	//

	int nFullWidth = 0;
	int nFullHeight = 0;

	if ( 6 == nFigureType )
	{					
		nFullWidth = 3 * dSide;
		nFullHeight = 3 * dSide;

		double dCurX = 0; 
		double dCurY = 0;
		int nYLevel = 0;

		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		{
			int nYShift = sqrt(3.0) * dSide / 2;
			if ( dCurX > nWidth + 2 * dSide )
			{
				if ( 0 == nYLevel )
					dCurX = dSide * 1.5;
				else
					dCurX = 0;
				dCurY += nYShift;
				nYLevel++;
				nYLevel = nYLevel % 2;
			}
			pFigures[nIndex].SetValue(nFigureType, dSide, pSource2, nWidth, nHeight, dCurX, dCurY);
			dCurX += 3 * dSide;
		}
	}
	else if ( 3 == nFigureType )
	{					
		nFullWidth = dSide;
		nFullHeight = dSide;

		int nCurX = 0; 
		int nCurY = 0;
		int nXLevel = 0;

		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		{
			if ( nCurX >= nXCount )
			{
				nCurX = 0;
				nXLevel = 0;
				nCurY++;
			}
			
			pFigures[nIndex].SetValue(31 - nXLevel, dSide, pSource2, nWidth, nHeight, nCurX * dSide / 2, nCurY * sqrt(3.0) * 0.5 * dSide + nXLevel * sqrt(3.0) / 6 * dSide);
			
			nCurX++;
			nXLevel = (nXLevel + 1) % 2;
		}
	}
	else if ( 41 == nFigureType )
	{
		nFullWidth = 2 * dSide;
		nFullHeight = 2 * dSide;

		double dRadius = dSide / sqrt(2.0); // радиус описанной окружности
		int nCurX = 0; 
		int nCurY = 0;
		int nXLevel = 0;

		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		{
			if ( nCurX >= nXCount )
			{
				nCurX = 0;
				nXLevel = 0;
				nCurY++;
			}
			
			pFigures[nIndex].SetValue(41, dSide, pSource2, nWidth, nHeight, nCurX * dRadius, nCurY * 2*dRadius + nXLevel * dRadius);
			
			nCurX++;
			nXLevel = (nXLevel + 1) % 2;
		}

	}
	else if ( 42 == nFigureType )
	{
		nFullWidth = dSide;
		nFullHeight = dSide;

		double dXShift = dSide;
		double dYShift = dSide * (12.0 / 25.0);
		int nCurX = 0; 
		int nCurY = 0;
		int nYLevel = 0;

		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		{
			if ( nCurX >= nXCount )
			{
				nCurX = 0;
				nYLevel = (nYLevel + 1) % 2;
				nCurY++;
			}
			
			pFigures[nIndex].SetValue(42, dSide, pSource2, nWidth, nHeight, nCurX * dXShift + nYLevel * dXShift / 2, nCurY * dYShift);
			
			nCurX++;
		}

	}
	else if ( 43 == nFigureType )
	{
		nFullWidth = 2 * dSide;
		nFullHeight = 2 * dSide;

		double dAngle = -IPP_PI4 + atan(25.0 / 24.0);
		double dDiagonal = dSide * sqrt (1.0 / 4.0 + 144.0 / 625.0);
		double dXShift = dDiagonal * cos(dAngle);
		double dXYShift = dDiagonal * sin(dAngle);

		int nCurX = 0; 
		int nCurY = 0;
		int nYLevel = 0;

		for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		{
			if ( nCurX >= nXCount )
			{
				nCurX = 0;
				nYLevel = (nYLevel + 1) % 2;
				nCurY++;
			}
			
			pFigures[nIndex].SetValue(43, dSide, pSource2, nWidth, nHeight, nCurX * dXShift - nCurY * dXYShift, nCurY * dXShift - nCurX * dXYShift);
			
			nCurX++;
		}

	}

	//
	// Обработка движения кусочков
	//

	double dFadeTime = 0.1;
	double dTime = dFrame / (1 - dFadeTime);
	int nStep = dTime * nCount;

	if ( dFrame < 1 - dFadeTime )
		::memcpy(pResult, pSource1, nSize);
	else
		::memcpy(pResult, pSource2, nSize);

	int nXStart = 0;
	int nYStart = 0;

	if ( dFrame < 1 - dFadeTime )
	{
		int nIndex = 0;
		
		int nCurFlyTime = 1;

		for ( ; nIndex < nFlyTime; nIndex++, nCurFlyTime++)
		{
			int nCurIndex = pUtility->g_pShuffle[nIndex];
			if ( nIndex < nStep + nFlyTime && nIndex >= nStep)
			{
				double dKoef = (nCurFlyTime - (nIndex + 1 - dTime * nCount)) / nCurFlyTime;
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_dXCoorSource, pFigures[nCurIndex].m_dYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_dXCoorSource, pFigures[nCurIndex].m_dYCoorSource, pUtility->g_pCorners[nIndex]);
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_dXCoorSource * dKoef + ( 1 - dKoef ) * (nXStart), pFigures[nCurIndex].m_dYCoorSource * dKoef + ( 1 - dKoef ) * (nYStart), nWidth, nHeight, 1);
			}
			else if ( nIndex < nStep)
			{
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_dXCoorSource, pFigures[nCurIndex].m_dYCoorSource, nWidth, nHeight, 1);
			}
		}

		for ( ; nIndex < nCount; nIndex++ )
		{
			int nCurIndex = pUtility->g_pShuffle[nIndex];
			if ( nIndex < nStep + nFlyTime && nIndex >= nStep)
			{
				double dKoef = (nFlyTime - (nIndex + 1  - dTime * nCount)) / nFlyTime;
				nXStart = pUtility->GetStartX(nFullWidth, nFullHeight, pFigures[nCurIndex].m_dXCoorSource, pFigures[nCurIndex].m_dYCoorSource, pUtility->g_pCorners[nIndex], nCurIndex % nXCount);
				nYStart = pUtility->GetStartY(nFullWidth, nFullHeight, pFigures[nCurIndex].m_dXCoorSource, pFigures[nCurIndex].m_dYCoorSource, pUtility->g_pCorners[nIndex]);
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_dXCoorSource * dKoef + ( 1 - dKoef ) * (nXStart), pFigures[nCurIndex].m_dYCoorSource * dKoef + ( 1 - dKoef ) * (nYStart), nWidth, nHeight, 1);
			}
			else if ( nIndex < nStep)
			{
				pFigures[nCurIndex].Draw(&oMainRender, &oBrush, pFigures[nCurIndex].m_dXCoorSource, pFigures[nCurIndex].m_dYCoorSource, nWidth, nHeight, 1);
			}

		}
	}
	else
	{
		double dCurTime = (dFrame - (1 - dFadeTime)) / dFadeTime;

		for ( int nIndex = 0; nIndex <= nCount - 1; nIndex++)
			pFigures[pUtility->g_pShuffle[nIndex]].Draw(&oMainRender, &oBrush, pFigures[pUtility->g_pShuffle[nIndex]].m_dXCoorSource, pFigures[pUtility->g_pShuffle[nIndex]].m_dYCoorSource, nWidth, nHeight, 1 - dCurTime);
	}

	delete []pFigures;

	return TRUE;
}


BOOL IPPMaskGraduate(const SmartImage &first, const SmartImage &last, SmartImage &result, int nTilesX, int nTilesY, int nType, double dFrame, MaskGraduate* oMaskManager, int nFadeWide, BOOL bWipeSide)
{
	BYTE* pBGRASource1 = first.Pixels;
	BYTE* pBGRASource2 = last.Pixels;
	BYTE* pBGRAResult  = result.Pixels;

	int nHeight = first.Height;
	int nWidth = first.Width;

	oMaskManager->GenerateMask( nWidth, nHeight, nType, bWipeSide);

	BYTE* pMask = oMaskManager->g_pMask;
	if( !pMask )
		return FALSE;

	int nFrame = int((255 + nFadeWide) * dFrame + 0.5);
	double dFactor = 256.0 / (nFadeWide ? nFadeWide : 1);

	//for( int nSize = nWidth * nHeight; nSize > 0; --nSize, ++pMask, pBGRASource1 += 4, pBGRASource2 += 4, pBGRAResult += 4 )
	//{
	//	int mask = pMask[0];
	//	
	//	if( mask > nFrame )
	//	{
	//		*((DWORD*)pBGRAResult) = *((DWORD*)pBGRASource1);
	//		continue;
	//	}
	//	
	//	if( mask > nFrame - nFadeWide )
	//	{
	//		int alpha = int((nFrame - mask) * dFactor);
	//		pBGRAResult[0] = AlphaBlend( pBGRASource1[0], pBGRASource2[0], alpha );
	//		pBGRAResult[1] = AlphaBlend( pBGRASource1[1], pBGRASource2[1], alpha );
	//		pBGRAResult[2] = AlphaBlend( pBGRASource1[2], pBGRASource2[2], alpha );
	//		pBGRAResult[3] = AlphaBlend( pBGRASource1[3], pBGRASource2[3], alpha );
	//		continue;
	//	}

	//	*((DWORD*)pBGRAResult) = *((DWORD*)pBGRASource2);
	//}

	int nIndex = 0;
	for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
	{
		for( int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4)
		{
			if(oMaskManager->g_pMask[nYIndex*nWidth + nXIndex] < nFrame - nFadeWide / 2)
			{
				pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
			}
			else if(oMaskManager->g_pMask[nYIndex*nWidth + nXIndex] <= nFrame && oMaskManager->g_pMask[nYIndex*nWidth + nXIndex] >= nFrame - nFadeWide / 2 &&(nFadeWide > 0) )
			{
				double dValue = 0.5 - (-oMaskManager->g_pMask[nYIndex*nWidth + nXIndex] + nFrame) * 1. / nFadeWide;
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * dValue + pBGRASource2[nIndex + 2] * (1 - dValue);
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * dValue + pBGRASource2[nIndex + 1] * (1 - dValue);
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * dValue + pBGRASource2[nIndex + 0] * (1 - dValue);
			}
			else if(oMaskManager->g_pMask[nYIndex*nWidth + nXIndex] - nFrame <= nFadeWide / 2 && oMaskManager->g_pMask[nYIndex*nWidth + nXIndex] >= nFrame && (nFadeWide > 0) )
			{
				double dValue = (oMaskManager->g_pMask[nYIndex*nWidth + nXIndex] - nFrame) * 1. / nFadeWide + 0.5;
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2] * dValue + pBGRASource2[nIndex + 2] * (1 - dValue);
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1] * dValue + pBGRASource2[nIndex + 1] * (1 - dValue);
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0] * dValue + pBGRASource2[nIndex + 0] * (1 - dValue);
			}
			else
			{
				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
			}
		}
	}
	return TRUE;
}

