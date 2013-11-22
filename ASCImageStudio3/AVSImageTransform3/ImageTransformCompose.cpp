
#include "stdafx.h"
#include "ImageTransform.h"
#include <TransformConstants.h>

BOOL ImageTransform3::WipeDoor(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*Frame );
	int nLevelY = (int)( (nSizeY - 1)*Frame );
	int nLevel1X = (int)( 0.5*(nSizeX - 1)*Frame );
	int nLevel1Y = (int)( 0.5*(nSizeY - 1)*Frame );
	int nLevel2X = (int)( 0.5*(nSizeX - 1)*(1 - Frame) );
	int nLevel2Y = (int)( 0.5*(nSizeY - 1)*(1 - Frame) );

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nWipeDoorVerticalIn)
			{
				if (nPatternY <= nLevel1Y || nPatternY >= (nSizeY - 1) - nLevel1Y)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeDoorVerticalOut)
			{
				if (nPatternY >= nLevel2Y && nPatternY <= (nSizeY - 1) - nLevel2Y)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeDoorHorizontalIn)
			{
				if (nPatternX <= nLevel1X || nPatternX >= (nSizeX - 1) - nLevel1X)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeDoorHorizontalOut)
			{
				if (nPatternX >= nLevel2X && nPatternX <= (nSizeX - 1) - nLevel2X)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeDoorDiagonal)
			{
				if ((nPatternY <= nLevelY && nPatternX <= nLevelX) || (nPatternY >= (nSizeY - 1) - nLevelY && nPatternX >= (nSizeX - 1) - nLevelX))
					nPatternValue = 1;
			}
			else if (Type == c_nWipeDoorDiagonalRotated)
			{
				if ((nPatternY >= (nSizeY - 1) - nLevelY && nPatternX <= nLevelX) || (nPatternY <= nLevelY && nPatternX >= (nSizeX - 1) - nLevelX))
					nPatternValue = 1;
			}

			// combine colors
			if (nPatternValue == 0)
			{
				// do nothing
			}
			else
			{
				m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dBlue);
				m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dGreen);
				m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dRed);
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::WipeSide(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*Frame );
	int nLevelY = (int)( (nSizeY - 1)*Frame );

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nWipeSideTypeLeftToRight)
			{
				if (nPatternX <= nLevelX)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeSideTypeRightToLeft)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeSideTypeBottomToTop)
			{
				if (nPatternY <= nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeSideTypeTopToBottom)
			{
				if (nPatternY >= (nSizeY - 1) - nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeSideTypeLeftTopToRightBottom)
			{
				if (nPatternX <= nLevelX && nPatternY >= (nSizeY - 1) - nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeSideTypeLeftBottomToRightTop)
			{
				if (nPatternX <= nLevelX && nPatternY <= nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeSideTypeRightBottomToLeftTop)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX && nPatternY <= nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeSideTypeRightTopToLeftBottom)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX && nPatternY >= (nSizeY - 1) - nLevelY)
					nPatternValue = 1;
			}
			
			// combine colors
			if (nPatternValue == 0)
			{
				// do nothing
			}
			else
			{
				m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dBlue);
				m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dGreen);
				m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dRed);
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::WipeCenter(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;

	double dCenterX = 0.5*(nSizeX - 1);
	double dCenterY = 0.5*(nSizeY - 1);

	double dLevelX = 0.5*nSizeX*Frame;
	double dLevelY = 0.5*nSizeY*Frame;

	double dRadius;
	double dRadiusMin = 0.25*(nSizeX*nSizeX + nSizeY*nSizeY)*Frame;
	double dRadiusMax = 0.25*(nSizeX*nSizeX + nSizeY*nSizeY)*(1 - Frame);

	double dDiamond;
	double dDiamondMin = 0.5*(nSizeX + nSizeY)*Frame;
	double dDiamondMax = 0.5*(nSizeX + nSizeY)*(1 - Frame);

	double dCross1X = 0.5*(nSizeX - 1)*Frame;
	double dCross1Y = 0.5*(nSizeY - 1)*Frame;
	double dCross2X = 0.5*(nSizeX - 1)*(1 - Frame);
	double dCross2Y = 0.5*(nSizeY - 1)*(1 - Frame);

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nWipeCenterTypeBoxIn)
			{
				if (nPatternX < dLevelX || nPatternX > (nSizeX - 1) - dLevelX || nPatternY < dLevelY || nPatternY > (nSizeY - 1) - dLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeCenterTypeBoxOut)
			{
				if (nPatternX > dCenterX - dLevelX && nPatternX < dCenterX + dLevelX && 
					nPatternY > dCenterY - dLevelY && nPatternY < dCenterY + dLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeCenterTypeCircleIn)
			{
				dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
				if (dRadius > dRadiusMax)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeCenterTypeCircleOut)
			{
				dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
				if (dRadius < dRadiusMin)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeCenterTypeDiamondIn)
			{
				dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
				if (dDiamond > dDiamondMax)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeCenterTypeDiamondOut)
			{
				dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
				if (dDiamond < dDiamondMin)
					nPatternValue = 1;
			}
			else if (Type == c_nWipeCenterTypeCrossIn)
			{
				if ((nPatternY <= dCross1Y && nPatternX <= dCross1X) ||
					(nPatternY <= dCross1Y && nPatternX >= (nSizeX - 1) - dCross1X) ||
					(nPatternY >= (nSizeY - 1) - dCross1Y && nPatternX >= (nSizeX - 1) - dCross1X) ||
					(nPatternY >= (nSizeY - 1) - dCross1Y && nPatternX <= dCross1X))
					nPatternValue = 1;
			}
			else if (Type == c_nWipeCenterTypeCrossOut)
			{
				if ((nPatternY >= dCross2Y && nPatternY <= (nSizeY - 1) - dCross2Y) ||
					(nPatternX >= dCross2X && nPatternX <= (nSizeX - 1) - dCross2X))
					nPatternValue = 1;
			}

			// combine colors
			if (nPatternValue == 0)
			{
				// do nothing
			}
			else
			{
				m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dBlue);
				m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dGreen);
				m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dRed);
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::WipeStrips(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	double dCenterX = 0.5*(nSizeX - 1);
	double dCenterY = 0.5*(nSizeY - 1);
	double dLevelX = nSizeX*Frame;
	double dLevelY = nSizeY*Frame;

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nWipeStripsTypeVertical)
			{
				if ((nPatternX < dCenterX && nPatternY < dLevelY) ||
					(nPatternX >= dCenterX && nPatternY > (nSizeY - 1) - dLevelY))
					nPatternValue = 1;
			}
			else if (Type == c_nWipeStripsTypeHorizontal)
			{
				if ((nPatternY < dCenterY && nPatternX < dLevelX) ||
					(nPatternY >= dCenterY && nPatternX > (nSizeX - 1) - dLevelX))
					nPatternValue = 1;
			}

			// combine colors
			if (nPatternValue == 0)
			{
				// do nothing
			}
			else
			{
				m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dBlue);
				m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dGreen);
				m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dRed);
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::PushDoor(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*Frame );
	int nLevelY = (int)( (nSizeY - 1)*Frame );
	int nLevel1X = (int)( 0.5*(nSizeX - 1)*Frame );
	int nLevel1Y = (int)( 0.5*(nSizeY - 1)*Frame );
	int nLevel2X = (int)( 0.5*(nSizeX - 1)*(1 - Frame) );
	int nLevel2Y = (int)( 0.5*(nSizeY - 1)*(1 - Frame) );

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nPushDoorVerticalIn)
			{
				if (nPatternY <= nLevel1Y)
				{
					nSourceY = nY + (nSizeY - 1)/2 - nLevel1Y;
					nPatternValue = 1;
				}
				else if (nPatternY <= (nSizeY - 1)/2)
				{
					nSourceY = nY - nLevel1Y;
					nPatternValue = 0;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevel1Y)
				{
					nSourceY = nY + nLevel1Y - (nSizeY - 1)/2;
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1)/2)
				{
					nSourceY = nY + nLevel1Y;
					nPatternValue = 0;
				}
			}
			else if (Type == c_nPushDoorVerticalOut)
			{
				if (nPatternY <= nLevel2Y)
				{
					nSourceY = nY + (nSizeY - 1)/2 - nLevel2Y;
					nPatternValue = 0;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevel2Y)
				{
					nSourceY = nY + nLevel2Y - (nSizeY - 1)/2;
					nPatternValue = 0;
				}
				else if (nPatternY >= nLevel2Y && nPatternY <= (nSizeY - 1)/2)
				{
					nSourceY = nY - nLevel2Y;
					nPatternValue = 1;
				}
				else if (nPatternY <= (nSizeY - 1) - nLevel2Y && nPatternY >= (nSizeY - 1)/2)
				{
					nSourceY = nY + nLevel2Y;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nPushDoorHorizontalIn)
			{
				if (nPatternX <= nLevel1X)
				{
					nSourceX = nX + (nSizeX - 1)/2 - nLevel1X;
					nPatternValue = 1;
				}
				else if (nPatternX <= (nSizeX - 1)/2)
				{
					nSourceX = nX - nLevel1X;
					nPatternValue = 0;
				}
				else if (nPatternX >= (nSizeX - 1) - nLevel1X)
				{
					nSourceX = nX + nLevel1X - (nSizeX - 1)/2;
					nPatternValue = 1;
				}
				else if (nPatternX >= (nSizeX - 1)/2)
				{
					nSourceX = nX + nLevel1X;
					nPatternValue = 0;
				}
			}
			else if (Type == c_nPushDoorHorizontalOut)
			{
				if (nPatternX <= nLevel2X)
				{
					nSourceX = nX + (nSizeX - 1)/2 - nLevel2X;
					nPatternValue = 0;
				}
				else if (nPatternX >= (nSizeX - 1) - nLevel2X)
				{
					nSourceX = nX + nLevel2X - (nSizeX - 1)/2;
					nPatternValue = 0;
				}
				else if (nPatternX >= nLevel2X && nPatternX <= (nSizeX - 1)/2)
				{
					nSourceX = nX - nLevel2X;
					nPatternValue = 1;
				}
				else if (nPatternX <= (nSizeX - 1) - nLevel2X && nPatternX >= (nSizeX - 1)/2)
				{
					nSourceX = nX + nLevel2X;
					nPatternValue = 1;
				}
			}

			// combine colors
			if (nSourceX < 0 || nSourceX >= m_nWidth || nSourceY < 0 || nSourceY >= m_nHeight)
			{
				m_pPixels[nIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nIndex + 2] = m_pBackup[nIndex + 2];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				if (nPatternValue == 0)
				{
					m_pPixels[nIndex + 0] = m_pBackup[nSourceIndex + 0];
					m_pPixels[nIndex + 1] = m_pBackup[nSourceIndex + 1];
					m_pPixels[nIndex + 2] = m_pBackup[nSourceIndex + 2];
				}
				else
				{
					m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 0] + dBlue);
					m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 1] + dGreen);
					m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 2] + dRed);
				}
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::PushSide(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*Frame );
	int nLevelY = (int)( (nSizeY - 1)*Frame );

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nPushSideTypeLeftToRight)
			{
				if (nPatternX <= nLevelX)
				{
					nSourceX = nX + (nSizeX - 1) - nLevelX;
					nPatternValue = 1;
				}
				else
				{
					nSourceX = nX - nLevelX;
					nPatternValue = 0;
				}
			}
			else if (Type == c_nPushSideTypeRightToLeft)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX)
				{
					nSourceX = nX + nLevelX - (nSizeX - 1);
					nPatternValue = 1;
				}
				else
				{
					nSourceX = nX + nLevelX;
					nPatternValue = 0;
				}
			}
			else if (Type == c_nPushSideTypeBottomToTop)
			{
				if (nPatternY <= nLevelY)
				{
					nSourceY = nY + (nSizeY - 1) - nLevelY;
					nPatternValue = 1;
				}
				else
				{
					nSourceY = nY - nLevelY;
					nPatternValue = 0;
				}
			}
			else if (Type == c_nPushSideTypeTopToBottom)
			{
				if (nPatternY >= (nSizeY - 1) - nLevelY)
				{
					nSourceY = nY + nLevelY - (nSizeY - 1);
					nPatternValue = 1;
				}
				else
				{
					nSourceY = nY + nLevelY;
					nPatternValue = 0;
				}
			}

			// combine colors
			if (nSourceX < 0 || nSourceX >= m_nWidth || nSourceY < 0 || nSourceY >= m_nHeight)
			{
				m_pPixels[nIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nIndex + 2] = m_pBackup[nIndex + 2];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				if (nPatternValue == 0)
				{
					m_pPixels[nIndex + 0] = m_pBackup[nSourceIndex + 0];
					m_pPixels[nIndex + 1] = m_pBackup[nSourceIndex + 1];
					m_pPixels[nIndex + 2] = m_pBackup[nSourceIndex + 2];
				}
				else
				{
					m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 0] + dBlue);
					m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 1] + dGreen);
					m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 2] + dRed);
				}
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::PushStrips(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nCenterX = (nSizeX - 1)/2;
	int nCenterY = (nSizeY - 1)/2;
	int nLevelX = (int)((nSizeX - 1)*Frame);
	int nLevelY = (int)((nSizeY - 1)*Frame);

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nPushStripsTypeVertical)
			{
				if (nPatternX <= nCenterX)
				{
					if (nPatternY <= nLevelY)
					{
						nSourceY = nY + (nSizeY - 1) - nLevelY;
						nPatternValue = 1;
					}
					else
					{
						nSourceY = nY - nLevelY;
						nPatternValue = 0;
					}
				}
				else
				{
					if (nPatternY >= (nSizeY - 1) - nLevelY)
					{
						nSourceY = nY + nLevelY - (nSizeY - 1);
						nPatternValue = 1;
					}
					else
					{
						nSourceY = nY + nLevelY;
						nPatternValue = 0;
					}
				}
			}
			else if (Type == c_nPushStripsTypeHorizontal)
			{
				if (nPatternY <= nCenterY)
				{
					if (nPatternX <= nLevelX)
					{
						nSourceX = nX + (nSizeX - 1) - nLevelX;
						nPatternValue = 1;
					}
					else
					{
						nSourceX = nX - nLevelX;
						nPatternValue = 0;
					}
				}
				else
				{
					if (nPatternX >= (nSizeX - 1) - nLevelX)
					{
						nSourceX = nX + nLevelX - (nSizeX - 1);
						nPatternValue = 1;
					}
					else
					{
						nSourceX = nX + nLevelX;
						nPatternValue = 0;
					}
				}
			}

			// combine colors
			if (nSourceX < 0 || nSourceX >= m_nWidth || nSourceY < 0 || nSourceY >= m_nHeight)
			{
				m_pPixels[nIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nIndex + 2] = m_pBackup[nIndex + 2];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				if (nPatternValue == 0)
				{
					m_pPixels[nIndex + 0] = m_pBackup[nSourceIndex + 0];
					m_pPixels[nIndex + 1] = m_pBackup[nSourceIndex + 1];
					m_pPixels[nIndex + 2] = m_pBackup[nSourceIndex + 2];
				}
				else
				{
					m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 0] + dBlue);
					m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 1] + dGreen);
					m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nSourceIndex + 2] + dRed);
				}
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::FxMosaic(double Frame, int Type, int Color, int Alpha)
{
	// check for simpliest case
	if (Frame < c_dFrameMin)
		return TRUE;
	else if (Frame > c_dFrameMax)
		return ApplyColor(m_pPixels, m_nSize, Color, Alpha);

	// variables
	double dAlpha1 = Alpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute tile size
	int nSizeX = (m_nSizeX < 2) ? m_nWidth : m_nSizeX;
	int nSizeY = (m_nSizeY < 2) ? m_nHeight : m_nSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevel = (int)((8*8 - 1)*Frame);

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = (int)(8.0*(nY % nSizeY)/(double)(nSizeY - 1));
		nPatternY = min(nPatternY, 7);

		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = (int)(8.0*(nX % nSizeX)/(double)(nSizeX - 1));
			nPatternX = min(nPatternX, 7);

			// compute pattern value - the level of transition
			nPatternValue = 0;

			// check for patterns
			if (Type == c_nMosaicTypeSpiralIn)				nPatternValue = c_byMosaicSpiralIn					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeSpiralOut)				nPatternValue = c_byMosaicSpiralOut					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeStrips)				nPatternValue = c_byMosaicStrips					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeDissolve)				nPatternValue = c_byMosaicDissolve					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeLeftTopToRightBottom)	nPatternValue = c_byMosaicDiagonalLeftTop			[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeRightTopToLeftBottom)	nPatternValue = c_byMosaicDiagonalRightTop			[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeRightBottomToLeftTop)	nPatternValue = c_byMosaicDiagonalRightBottom		[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeLeftBottomToRightTop)	nPatternValue = c_byMosaicDiagonalLeftBottom		[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeWallLeftToRight)		nPatternValue = c_byMosaicWallLeft					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeWallRightToLeft)		nPatternValue = c_byMosaicWallRight					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeWallTopToBottom)		nPatternValue = c_byMosaicWallTop					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeWallBottomToTop)		nPatternValue = c_byMosaicWallBottom				[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeChessLeftToRight)		nPatternValue = c_byMosaicChessLeft					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeChessRightToLeft)		nPatternValue = c_byMosaicChessRight				[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeChessTopToBottom)		nPatternValue = c_byMosaicChessTop					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeChessBottomToTop)		nPatternValue = c_byMosaicChessBottom				[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeClockwise)				nPatternValue = c_byMosaicClockwise					[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeCounterClockwise)		nPatternValue = c_byMosaicCounterClockwise			[nPatternY][nPatternX];
			if (Type == c_nMosaicTypeRandom)				nPatternValue = c_byMosaicRandom					[nPatternY][nPatternX];

			// combine colors
			if (nPatternValue >= nLevel)
			{
				m_pPixels[nIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nIndex + 2] = m_pBackup[nIndex + 2];
			}
			else
			{
				m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dBlue);
				m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dGreen);
				m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dRed);
			}
		}
	}

	// all ok
	return TRUE;
}
	
BOOL ImageTransform3::ApplyCompose(double Frame, long Type, int Color, int Alpha)
{
	// check for composition type
	if (Type >= c_nWipeDoorVerticalIn && Type <= c_nWipeDoorDiagonalRotated)
		return WipeDoor(Frame, Type, Color, Alpha);
	else if (Type >= c_nWipeSideTypeLeftToRight && Type <= c_nWipeSideTypeRightTopToLeftBottom)
		return WipeSide(Frame, Type, Color, Alpha);
	else if (Type >= c_nWipeCenterTypeBoxIn && Type <= c_nWipeCenterTypeCrossOut)
		return WipeCenter(Frame, Type, Color, Alpha);
	else if (Type >= c_nWipeStripsTypeVertical && Type <= c_nWipeStripsTypeHorizontal)
		return WipeStrips(Frame, Type, Color, Alpha);
	else if (Type >= c_nPushDoorVerticalIn && Type <= c_nPushDoorHorizontalOut)
		return PushDoor(Frame, Type, Color, Alpha);
	else if (Type >= c_nPushSideTypeLeftToRight && Type <= c_nPushSideTypeBottomToTop)
		return PushSide(Frame, Type, Color, Alpha);
	else if (Type >= c_nPushStripsTypeVertical && Type <= c_nPushStripsTypeHorizontal)
		return PushStrips(Frame, Type, Color, Alpha);
	else if (Type >= c_nMosaicTypeSpiralIn && Type <= c_nMosaicTypeChessBottomToTop)
		return FxMosaic(Frame, Type, Color, Alpha);

	// all ok
	return FALSE;
}