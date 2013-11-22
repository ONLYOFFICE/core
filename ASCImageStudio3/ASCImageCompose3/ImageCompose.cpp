// ImageCompose.cpp : Implementation of ImageCompose

#include "stdafx.h"
#include "ImageCompose.h"
#include "ImageComposeIPP.h"
#include "../Common/ImageTransformsCoreWrappers.h"


inline int AlphaBlend( int dst, int src, int alpha )
{
	return ((src - dst) * alpha >> 8) + dst;
}

void BGRA_AlphaBlend( const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame )
{
	int alpha = int(dFrame * 256 + 0.5);
	
	if( alpha < 0 ) alpha = 0;
	else if( alpha > 256 ) alpha = 256;

	BYTE* pResult = result.Pixels;
	BYTE* pSrc    = last.Pixels;
	BYTE* pDst    = first.Pixels;

	int nPixels = result.Width * result.Height;
	
	for( ; nPixels > 0; pResult += 4, pDst += 4, pSrc += 4, --nPixels )
	{
		pResult[0] = AlphaBlend( pDst[0], pSrc[0], alpha );
		pResult[1] = AlphaBlend( pDst[1], pSrc[1], alpha );
		pResult[2] = AlphaBlend( pDst[2], pSrc[2], alpha );
		pResult[3] = AlphaBlend( pDst[3], pSrc[3], alpha );
	}
}


// f/x
BOOL ImageCompose3::Fade(int nType)
{
	// combine two images
	switch( nType )
	{
	case c_nFadeZoomIn:
		IPPFadeZoom(m_first, m_last, m_result, m_dFrame, TRUE, FALSE);
		break;
	case c_nFadeZoomOut:
		IPPFadeZoom(m_first, m_last, m_result, m_dFrame, FALSE, TRUE);
		break;
	case c_nFadeZoomBoth:
		IPPFadeZoom(m_first, m_last, m_result, m_dFrame, TRUE, TRUE);
		break;
	case c_nPatternSolid:
		BGRA_AlphaBlend(m_first, m_last, m_result, m_dFrame);
		break;

	default:

		int nTileSizeX = m_nTileSizeX;
		int nTileSizeY = m_nTileSizeY;

		if( nType >= c_nPatternDots2 && nType <= c_nPatternCross2 )
		{
			nType -= 30;
			nTileSizeX = nTileSizeY = max(12, max(m_result.Width, m_result.Height) / 20);
		}
		else
		if( nType >= c_nPatternDots3 && nType <= c_nPatternCross3 )
		{
			nType -= 60;
			nTileSizeX = nTileSizeY = max(16, max(m_result.Width, m_result.Height) / 10);
		}

		const BYTE* pPattern = NULL;

		switch( nType )
		{
		case c_nPatternDots:                pPattern = &c_byPatternDots[0][0]; break;
		case c_nPatternDotsDense:           pPattern = &c_byPatternDotsDense[0][0]; break;
		case c_nPatternGrid:                pPattern = &c_byPatternGrid[0][0]; break;
		case c_nPatternGridRotated:         pPattern = &c_byPatternGridRotated[0][0]; break;
		case c_nPatternLineVertical:        pPattern = &c_byPatternLineVertical[0][0]; break;
		case c_nPatternLineHorizontal:      pPattern = &c_byPatternLineHorizontal[0][0]; break;
		case c_nPatternLineDiagonal:        pPattern = &c_byPatternLineDiagonal[0][0]; break;
		case c_nPatternLineDiagonalRotated: pPattern = &c_byPatternLineDiagonalRotated[0][0]; break;
		case c_nPatternChess:               pPattern = &c_byPatternChessBoard[0][0]; break;
		case c_nPatternDashes:              pPattern = &c_byPatternDashes[0][0]; break;
		case c_nPatternSpiner:              pPattern = &c_byPatternSpiner[0][0]; break;
		case c_nPatternThatches:            pPattern = &c_byPatternThatches[0][0]; break;
		case c_nPatternHeart:		        pPattern = &c_byPatternHeart[0][0]; break;
		case c_nPatternCross:	            pPattern = &c_byPatternCross[0][0]; break;
		}

		if( !pPattern )
		{
			BGRA_AlphaBlend(m_first, m_last, m_result, m_dFrame);
			break;
		}

		double dTileFactorX = 1.0 / nTileSizeX;
		double dTileFactorY = 1.0 / nTileSizeY;

		int nAlpha = int(m_dFrame * 256 + 0.5);

		BYTE* pResult = m_result.Pixels;
		BYTE* pSrc    = m_last.Pixels;
		BYTE* pDst    = m_first.Pixels;

		int nWidth  = m_result.Width;
		int nHeight = m_result.Height;

		int nTileY = 0;
		for( int nY = 0; nY < nHeight; ++nY )
		{
			// compute pattern Y coordinate
			if( ++nTileY >= nTileSizeY )
				nTileY = 0;

			int nPatternY1 = int((nTileY * 8 * 256) * dTileFactorY);
			int nFactorY   = nPatternY1 & 255; nPatternY1 >>= 8;
			int nPatternY2 = (nPatternY1 < 8 - 1) ? nPatternY1 + 1 : 0;

			int nTileX = 0;
			for( int nX = 0; nX < nWidth; ++nX, pResult += 4, pDst += 4, pSrc += 4 )
			{
				// compute pattern X coordinate
				if( ++nTileX >= nTileSizeX )
					nTileX = 0;

				int nPatternX1 = int((nTileX * 8 * 256) * dTileFactorX);
				int nFactorX   = nPatternX1 & 255; nPatternX1 >>= 8;
				int nPatternX2 = (nPatternX1 < 8 - 1) ? nPatternX1 + 1 : 0;

				// compute pattern value
				int nP11 = pPattern[nPatternY1 * 8 + nPatternX1] * 256;
				int nP12 = pPattern[nPatternY1 * 8 + nPatternX2] * 256;
				int nP21 = pPattern[nPatternY2 * 8 + nPatternX1] * 256;
				int nP22 = pPattern[nPatternY2 * 8 + nPatternX2] * 256;

				nP11 = (nP12 - nP11) * nFactorX + nP11 * 256;
				nP21 = (nP22 - nP21) * nFactorX + nP21 * 256;

				int nPatternValue = ((nP21 - nP11) * nFactorY + nP11 * 256) >> 16;

				// compute color koefficients

				if( nAlpha < 128 )
				{
					nPatternValue = (nPatternValue < 128) ? (128 - nPatternValue) * nAlpha >> 6 : 0;
				}
				else
				{
					nPatternValue = (nPatternValue < 128) ? (nPatternValue * (nAlpha - 128) >> 6) + (128 - nPatternValue) * 2 : (nAlpha - 128) * 2;
				}

				// combine colors
				pResult[0] = AlphaBlend( pDst[0], pSrc[0], nPatternValue );
				pResult[1] = AlphaBlend( pDst[1], pSrc[1], nPatternValue );
				pResult[2] = AlphaBlend( pDst[2], pSrc[2], nPatternValue );
				pResult[3] = AlphaBlend( pDst[3], pSrc[3], nPatternValue );
			}
		}
	}


	//// compute tile size
	//int nSizeX = m_nTileSizeX;
	//int nSizeY = m_nTileSizeY;

	//// variables
	//int nX, nY, nIndex;
	//int nPatternX, nPatternY, nPatternValue;
	//BYTE dR[2];
	//BYTE dG[2];
	//BYTE dB[2];
	//double dK1, dK2;

	//// compute starting index
	//nIndex = 0;

	//// combine two images
	//if (Type == c_nFadeZoomIn)
	//	IPPFadeZoom(m_first, m_last, m_result, m_dFrame, TRUE, FALSE);
	//else if (Type == c_nFadeZoomOut)
	//	IPPFadeZoom(m_first, m_last, m_result, m_dFrame, FALSE, TRUE);
	//else if (Type == c_nFadeZoomBoth)
	//	IPPFadeZoom(m_first, m_last, m_result, m_dFrame, TRUE, TRUE);
	//else if (Type == c_nPatternSolid)
	//{
	//	// compute last koefficient
	//	dK2 = m_dFrame;
	//	dK1 = 1.0 - dK2;

	//	BYTE* pf = m_first.Pixels;
	//	BYTE* pl = m_last.Pixels;
	//	BYTE* pr = m_result.Pixels;

	//	int nSize = m_first.Height*m_first.Width;

	//	for (nIndex = 0; nIndex < nSize; ++nIndex)
	//	{
	//		/*
	//		// combine colors
	//		*pr = (int)((*pf) + (int)(*pl))/2; pf++; pl++; pr++;
	//		*pr = (int)((*pf) + (int)(*pl))/2; pf++; pl++; pr++;
	//		*pr = (int)((*pf) + (int)(*pl))/2; pf++; pl++; pr++;

	//		// offset to next pixel
	//		pf++; pl++; pr++;

	//		*/
	//		// combine colors
	//		*pr = (BYTE)(dK1*(*pf) + dK2*(*pl)); pf++; pl++; pr++;
	//		*pr = (BYTE)(dK1*(*pf) + dK2*(*pl)); pf++; pl++; pr++;
	//		*pr = (BYTE)(dK1*(*pf) + dK2*(*pl)); pf++; pl++; pr++;

	//		// offset to next pixel
	//		pf++; pl++; pr++;
	//	}
	//}
	//else
	//{
	//	nIndex = 0;

	//	for (nY = 0; nY < m_first.Height; ++nY)
	//	{
	//		// compute pattern Y coordinate
	//		nPatternY = (int)(8.0*(nY % nSizeY)/(double)(nSizeY - 1));
	//		nPatternY = min(nPatternY, 7);

	//		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
	//		{
	//			// compute pattern X coordinate
	//			nPatternX = (int)(8.0*(nX % nSizeX)/(double)(nSizeX - 1));
	//			nPatternX = min(nPatternX, 7);

	//			// retrieve pixel colors
	//			dR[0] = m_first.Pixels[nIndex + 2];
	//			dG[0] = m_first.Pixels[nIndex + 1];
	//			dB[0] = m_first.Pixels[nIndex + 0];

	//			dR[1] = m_last.Pixels[nIndex + 2];
	//			dG[1] = m_last.Pixels[nIndex + 1];
	//			dB[1] = m_last.Pixels[nIndex + 0];

	//			// compute pattern value: if -1 - then simple fading, else - combined
	//			nPatternValue = -1;

	//			// check for patterns
	//			if (Type == c_nPatternDots)						nPatternValue = c_byPatternDots					[nPatternY][nPatternX];
	//			else if (Type == c_nPatternDotsDense)			nPatternValue = c_byPatternDotsDense			[nPatternY][nPatternX];
	//			else if (Type == c_nPatternGrid)				nPatternValue = c_byPatternGrid					[nPatternY][nPatternX];
	//			else if (Type == c_nPatternGridRotated)			nPatternValue = c_byPatternGridRotated			[nPatternY][nPatternX];
	//			else if (Type == c_nPatternLineVertical)		nPatternValue = c_byPatternLineVertical			[nPatternY][nPatternX];
	//			else if (Type == c_nPatternLineHorizontal)		nPatternValue = c_byPatternLineHorizontal		[nPatternY][nPatternX];
	//			else if (Type == c_nPatternLineDiagonal)		nPatternValue = c_byPatternLineDiagonal			[nPatternY][nPatternX];
	//			else if (Type == c_nPatternLineDiagonalRotated)	nPatternValue = c_byPatternLineDiagonalRotated	[nPatternY][nPatternX];
	//			else if (Type == c_nPatternChess)				nPatternValue = c_byPatternChessBoard			[nPatternY][nPatternX];
	//			else if (Type == c_nPatternDashes)				nPatternValue = c_byPatternDashes				[nPatternY][nPatternX];
	//			else if (Type == c_nPatternSpiner)				nPatternValue = c_byPatternSpiner				[nPatternY][nPatternX];
	//			else if (Type == c_nPatternThatches)			nPatternValue = c_byPatternThatches				[nPatternY][nPatternX];

	//			// compute color koefficients
	//			if (nPatternValue < 0)
	//				dK2 = m_dFrame;
	//			else
	//			{
	//				if (nPatternValue == 0)
	//				{
	//					if (m_dFrame < 0.5)
	//						dK2 = m_dFrame / 0.5;
	//					else
	//						dK2 = 1.0;
	//				}
	//				else
	//				{
	//					if (m_dFrame < 0.5)
	//						dK2 = 0.0;
	//					else
	//						dK2 = (m_dFrame - 0.5)/0.5;
	//				}
	//			}

	//			// compute last koefficient
	//			dK1 = 1.0 - dK2;

	//			// combine colors
	//			m_result.Pixels[nIndex + 2] = (BYTE)(dK1*dR[0] + dK2*dR[1]);
	//			m_result.Pixels[nIndex + 1] = (BYTE)(dK1*dG[0] + dK2*dG[1]);
	//			m_result.Pixels[nIndex + 0] = (BYTE)(dK1*dB[0] + dK2*dB[1]);
	//		}
	//	}
	//}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::Mosaic(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevel = (int)((8*8 - 1)*m_dFrame);

	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = (int)(8.0*(nY % nSizeY)/(double)(nSizeY - 1));
		nPatternY = min(nPatternY, 7);

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = (int)(8.0*(nX % nSizeX)/(double)(nSizeX - 1));
			nPatternX = min(nPatternX, 7);

			// compute pattern value - the level of transition
			nPatternValue = 0;

			// check for patterns
			if (Type == c_nMosaicSpiralIn)				nPatternValue = c_byMosaicSpiralIn					[nPatternY][nPatternX];
			if (Type == c_nMosaicSpiralOut)				nPatternValue = c_byMosaicSpiralOut					[nPatternY][nPatternX];
			if (Type == c_nMosaicStrips)				nPatternValue = c_byMosaicStrips					[nPatternY][nPatternX];
			if (Type == c_nMosaicDissolve)				nPatternValue = c_byMosaicDissolve					[nPatternY][nPatternX];
			if (Type == c_nMosaicLeftTopToRightBottom)	nPatternValue = c_byMosaicDiagonalLeftTop			[nPatternY][nPatternX];
			if (Type == c_nMosaicRightTopToLeftBottom)	nPatternValue = c_byMosaicDiagonalRightTop			[nPatternY][nPatternX];
			if (Type == c_nMosaicRightBottomToLeftTop)	nPatternValue = c_byMosaicDiagonalRightBottom		[nPatternY][nPatternX];
			if (Type == c_nMosaicLeftBottomToRightTop)	nPatternValue = c_byMosaicDiagonalLeftBottom		[nPatternY][nPatternX];
			if (Type == c_nMosaicWallLeftToRight)		nPatternValue = c_byMosaicWallLeft					[nPatternY][nPatternX];
			if (Type == c_nMosaicWallRightToLeft)		nPatternValue = c_byMosaicWallRight					[nPatternY][nPatternX];
			if (Type == c_nMosaicWallTopToBottom)		nPatternValue = c_byMosaicWallTop					[nPatternY][nPatternX];
			if (Type == c_nMosaicWallBottomToTop)		nPatternValue = c_byMosaicWallBottom				[nPatternY][nPatternX];
			if (Type == c_nMosaicChessLeftToRight)		nPatternValue = c_byMosaicChessLeft					[nPatternY][nPatternX];
			if (Type == c_nMosaicChessRightToLeft)		nPatternValue = c_byMosaicChessRight				[nPatternY][nPatternX];
			if (Type == c_nMosaicChessTopToBottom)		nPatternValue = c_byMosaicChessTop					[nPatternY][nPatternX];
			if (Type == c_nMosaicChessBottomToTop)		nPatternValue = c_byMosaicChessBottom				[nPatternY][nPatternX];
			if (Type == c_nMosaicClockwise)				nPatternValue = c_byMosaicClockwise					[nPatternY][nPatternX];
			if (Type == c_nMosaicCounterClockwise)		nPatternValue = c_byMosaicCounterClockwise			[nPatternY][nPatternX];
			if (Type == c_nMosaicRandom)				nPatternValue = c_byMosaicRandom					[nPatternY][nPatternX];

			// combine colors
			if (nPatternValue >= nLevel)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::Diffuse(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = max(1, (int)((nSizeX - 1)*m_dFrame));
	int nLevelY = max(1, (int)((nSizeY - 1)*m_dFrame));
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		// nPatternY = (int)(8.0*(nY % nSizeY)/(double)(nSizeY - 1));
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			// nPatternX = (int)(8.0*(nX % nSizeX)/(double)(nSizeX - 1));
			nPatternX = nX % nSizeX;

			// compute pattern value - the level of transition
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nSideLeftToRight)
			{
				if (nPatternX <= nLevelX)
				{
					double dPatternLevel = nPatternX/(double)nLevelX;
					int nPosLevel = (int)( 100*nLevelX/(nSizeX - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 0;
					else
						nPatternValue = 1;
				}
				else
				{
					double dPatternLevel = (nSizeX - 1 - nPatternX)/(double)(nSizeX - 1 - nLevelX);
					int nPosLevel = (int)( 100*(nSizeX - 1 - nLevelX)/(nSizeX - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 1;
					else
						nPatternValue = 0;
				}
			}
			else if (Type == c_nSideRightToLeft)
			{
				if ((nSizeX - 1 - nPatternX) <= nLevelX)
				{
					double dPatternLevel = (nSizeX - 1 - nPatternX)/(double)nLevelX;
					int nPosLevel = (int)( 100*nLevelX/(nSizeX - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 0;
					else
						nPatternValue = 1;
				}

				else
				{
					double dPatternLevel = (nSizeX - 1 - (nSizeX - 1 - nPatternX))/(double)(nSizeX - 1 - nLevelX);
					int nPosLevel = (int)( 100*(nSizeX - 1 - nLevelX)/(nSizeX - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 1;
					else
						nPatternValue = 0;
				}
			}
			else if (Type == c_nSideBottomToTop)
			{
				if (nPatternY <= nLevelY)
				{
					double dPatternLevel = nPatternY/(double)nLevelY;
					int nPosLevel = (int)( 100*nLevelY/(nSizeY - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 0;
					else
						nPatternValue = 1;
				}
				else
				{
					double dPatternLevel = (nSizeY - 1 - nPatternY)/(double)(nSizeY - 1 - nLevelY);
					int nPosLevel = (int)( 100*(nSizeY - 1 - nLevelY)/(nSizeY - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 1;
					else
						nPatternValue = 0;
				}
			}
			else if (Type == c_nSideTopToBottom)
			{
				if ((nSizeY - 1 - nPatternY) <= nLevelY)
				{
					double dPatternLevel = (nSizeY - 1 - nPatternY)/(double)nLevelY;
					int nPosLevel = (int)( 100*nLevelY/(nSizeY - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 0;
					else
						nPatternValue = 1;
				}
				else
				{
					double dPatternLevel = (nSizeY - 1 - (nSizeY - 1 - nPatternY))/(double)(nSizeY - 1 - nLevelY);
					int nPosLevel = (int)( 100*(nSizeY - 1 - nLevelY)/(nSizeY - 1)*dPatternLevel + 100*(1.0 - dPatternLevel) );
					int nPosValue = rand() % 100;

					if (nPosValue > nPosLevel)
						nPatternValue = 1;
					else
						nPatternValue = 0;
				}
			}

			// combine colors
			if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_last.Width);

				m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::Rotate(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dKoef1 = 1.0 - m_dFrame;
	double dKoef2 = m_dFrame;
	double dAngle = 2*3.1415*m_dFrame*((Type == c_nRotateTwiceIn || Type == c_nRotateTwiceOut) ? 2 : 1);
	double dR, dA;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// reset pattern value
			nPatternValue = 0;

			// compute color value
			if (Type == c_nRotateSingleIn || Type == c_nRotateTwiceIn)
			{
				// compute pixel radius and angle
				GetRadiusAndAngle(nPatternX, nPatternY, 0.5*(nSizeX - 1), 0.5*(nSizeY - 1), dR, dA);

				// compute destination coordinates
				dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) + (dR/dKoef1)*cos(dA - dAngle);
				dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) + (dR/dKoef1)*sin(dA - dAngle);

				// check values
				if (dSourceX < nTileX * nSizeX || dSourceX >= nTileX * nSizeX + nSizeX - 1 || dSourceY < nTileY * nSizeY || dSourceY >= nTileY * nSizeY + nSizeY - 1)
					nPatternValue = 0;
				else
					nPatternValue = 1;
			}
			else if (Type == c_nRotateSingleOut || Type == c_nRotateTwiceOut)
			{
				// compute pixel radius and angle
				GetRadiusAndAngle(nPatternX, nPatternY, 0.5*(nSizeX - 1), 0.5*(nSizeY - 1), dR, dA);

				// compute destination coordinates
				dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) + (dR/dKoef2)*cos(dA - dAngle);
				dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) + (dR/dKoef2)*sin(dA - dAngle);

				// check values
				if (dSourceX < nTileX * nSizeX || dSourceX >= nTileX * nSizeX + nSizeX - 1 || dSourceY < nTileY * nSizeY || dSourceY >= nTileY * nSizeY + nSizeY - 1)
					nPatternValue = 0;
				else
					nPatternValue = 1;
			}

			// combine colors
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height || nPatternValue == 0)
			{
				if (Type == c_nRotateSingleIn || Type == c_nRotateTwiceIn)
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				}
				else if (Type == c_nRotateSingleOut || Type == c_nRotateTwiceOut)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
			}
			else
			{
				if (Type == c_nRotateSingleIn || Type == c_nRotateTwiceIn)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (Type == c_nRotateSingleOut || Type == c_nRotateTwiceOut)
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else
	int nFirstAngle = -360;
	if ( (Type == c_nRotateTwiceIn) || (Type == c_nRotateTwiceOut) || (Type == c_nRotateTransparentTwiceIn) || (Type == c_nRotateTransparentTwiceOut))
		nFirstAngle = -720;
	if ( (Type == c_nRotateHalfIn) || (Type == c_nRotateHalfOut) || (Type == c_nRotateTransparentHalfIn) || (Type == c_nRotateTransparentHalfOut))
		nFirstAngle = -90;
	if ( c_nRotateSingleIn <= Type && c_nRotateHalfOut >= Type)
		IPPRotate(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, ((Type == c_nRotateSingleIn) || (Type == c_nRotateTwiceIn) || (c_nRotateHalfIn == Type)), nFirstAngle, FALSE);
	else
		IPPRotate(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, ((Type == c_nRotateTransparentSingleIn) || (Type == c_nRotateTransparentTwiceIn) || (c_nRotateTransparentHalfIn == Type)), nFirstAngle, TRUE);

#endif

	// all ok
	return TRUE;
}
BOOL ImageCompose3::Twirl(int Type)
{
	BOOL bIsClockwise;
	int nZoom = 0;
	switch (Type)
	{
	case c_nTwirlClockwise:
		 bIsClockwise = TRUE;
		 nZoom = 0;
	     IPPTwirl(m_first, m_last, m_result, m_dFrame, bIsClockwise,nZoom, &m_utility);
		 break;
	case c_nTwirlClockwiseZoomOut:
		 bIsClockwise = TRUE;
		 nZoom = 1;
	     IPPTwirl(m_first, m_last, m_result, m_dFrame, bIsClockwise,nZoom, &m_utility);
		 break;
	case c_nTwirlClockwiseZoomIn:
		 bIsClockwise = TRUE;
		 nZoom = 2;
	     IPPTwirl(m_first, m_last, m_result, m_dFrame, bIsClockwise,nZoom, &m_utility);
		 break;
	case c_nTwirlCounterClockwise:
		 bIsClockwise = FALSE;
		 nZoom = 0;
	     IPPTwirl(m_first, m_last, m_result, m_dFrame, bIsClockwise,nZoom, &m_utility);
		 break;
	case c_nTwirlCounterClockwiseZoomOut:
		 bIsClockwise = FALSE;
		 nZoom = 1;
	     IPPTwirl(m_first, m_last, m_result, m_dFrame, bIsClockwise,nZoom, &m_utility);
		 break;
	case c_nTwirlCounterClockwiseZoomIn:
		 bIsClockwise = FALSE;
		 nZoom = 2;
	 	 IPPTwirl(m_first, m_last, m_result, m_dFrame, bIsClockwise,nZoom, &m_utility);
		 break;
	case c_nTwirlDrain:
		 IPPTwirlDrain(m_first, m_last, m_result, m_dFrame, TRUE, &m_utility);
		 break;
	case c_nTwirlReverseDrain:
		 IPPTwirlDrain(m_first, m_last, m_result, m_dFrame, FALSE, &m_utility);
		 break;
	}
	return TRUE;
}

BOOL ImageCompose3::Sphere(int Type)
{
	IPPSphere(m_first, m_last, m_result, m_dFrame, (c_nSphereIn==Type));
	return TRUE;
}

BOOL ImageCompose3::Twist(int Type)
{
	switch (Type)
	{
	case c_nTwistLeftToRight:
	case c_nTwistRightToLeft:
		IPPTwistHorizontal(m_first, m_last, m_result, m_dFrame,(c_nTwistLeftToRight==Type), &m_utility);
		break;
	case c_nTwistTopToBottom:
	case c_nTwistBottomToTop:
		IPPTwistVertical(m_first, m_last, m_result, m_dFrame,(c_nTwistTopToBottom==Type), &m_utility);
		break;
	case c_nTwistLeftTopToRightBottom:
		IPPTwistDiagonal(m_first, m_last, m_result, m_dFrame, 0, &m_utility);
		break;
	case c_nTwistLeftBottomToRightTop:
		IPPTwistDiagonal(m_first, m_last, m_result, m_dFrame, 1, &m_utility);
		break;
	case c_nTwistRightTopToLeftBottom:
		IPPTwistDiagonal(m_first, m_last, m_result, m_dFrame, 2, &m_utility);
		break;
	case c_nTwistRightBottomToLeftTop:
		IPPTwistDiagonal(m_first, m_last, m_result, m_dFrame, 3, &m_utility);
		break;
	}
	return TRUE;
}

BOOL ImageCompose3::Ripple(int Type)
{
	IPPRipple(m_first, m_last, m_result, m_dFrame, (c_nRippleOut==Type));
	return TRUE;
}

BOOL ImageCompose3::Wave(int Type)
{
	IPPWave(m_first, m_last, m_result, m_dFrame, (c_nWaveHorizontal==Type));
	return TRUE;
}

BOOL ImageCompose3::Burning(int Type)
{
	switch (Type)
	{
	case c_nBurning:
		IPPBurning(m_first, m_last, m_result, m_dFrame, &m_utility);
		break;
	case c_nBurningFade:
		IPPBurningFade(m_first, m_last, m_result, m_dFrame, &m_utility);
		break;
	case c_nBurningFire:
		IPPBurningFire(m_first, m_last, m_result, m_dFrame, &m_utility);
		break;
	}
	return TRUE;
}
BOOL ImageCompose3::Fold(int Type)
{
#ifndef NOT_OPTIM
if ( Type == c_nSideTopToBottom || Type == c_nSideBottomToTop )
    IPPFoldVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSideBottomToTop ));
else if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft )
    IPPFoldHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSideLeftToRight) );
#endif
	// all ok
	return TRUE;
}
BOOL ImageCompose3::GaussianBlur(int Type)
{
	IPPGaussianBlur(m_first, m_last, m_result, m_dFrame);
	return TRUE;
}
BOOL ImageCompose3::Noise(int Type)
{
	switch (Type)
	{
	case c_nNoiseColored:
		IPPNoise(m_first, m_last, m_result, m_dFrame);
		break;
	case c_nNoiseGrayscale:
		IPPNoise(m_first, m_last, m_result, m_dFrame);
		break;
	case c_nFlashLight:
		IPPFlash(m_first, m_last, m_result, m_dFrame, RGB(255, 255, 255));
		break;
	case c_nFlashDark:
		IPPFlash(m_first, m_last, m_result, m_dFrame, RGB(0, 0, 0));
		break;
	}
	return TRUE;
}
	
// wipe
BOOL ImageCompose3::WipeDoor(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	int nLevel1X = (int)( 0.5*(nSizeX - 1)*m_dFrame );
	int nLevel1Y = (int)( 0.5*(nSizeY - 1)*m_dFrame );
	int nLevel2X = (int)( 0.5*(nSizeX - 1)*(1.0 - m_dFrame) );
	int nLevel2Y = (int)( 0.5*(nSizeY - 1)*(1.0 - m_dFrame) );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nDoorHorizontalIn /*c_nDoorVerticalIn*/)
			{
				if (nPatternY <= nLevel1Y || nPatternY >= (nSizeY - 1) - nLevel1Y)
					nPatternValue = 1;
			}
			else if (Type == c_nDoorHorizontalOut /*c_nDoorVerticalOut*/)
			{
				if (nPatternY >= nLevel2Y && nPatternY <= (nSizeY - 1) - nLevel2Y)
					nPatternValue = 1;
			}
			else if (Type == c_nDoorVerticalIn /*c_nDoorHorizontalIn*/)
			{
				if (nPatternX <= nLevel1X || nPatternX >= (nSizeX - 1) - nLevel1X)
					nPatternValue = 1;
			}
			else if (Type == c_nDoorVerticalOut /*c_nDoorHorizontalOut*/)
			{
				if (nPatternX >= nLevel2X && nPatternX <= (nSizeX - 1) - nLevel2X)
					nPatternValue = 1;
			}
			else if (Type == c_nDoorDiagonal)
			{
				if ((nPatternY <= nLevelY && nPatternX <= nLevelX) || (nPatternY >= (nSizeY - 1) - nLevelY && nPatternX >= (nSizeX - 1) - nLevelX))
					nPatternValue = 1;
			}
			else if (Type == c_nDoorDiagonalRotated)
			{
				if ((nPatternY >= (nSizeY - 1) - nLevelY && nPatternX <= nLevelX) || (nPatternY <= nLevelY && nPatternX >= (nSizeX - 1) - nLevelX))
					nPatternValue = 1;
			}

			// combine colors
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::WipeStrips(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	double dCenterX = 0.5*(nSizeX - 1);
	double dCenterY = 0.5*(nSizeY - 1);
	double dLevelX = nSizeX*m_dFrame;
	double dLevelY = nSizeY*m_dFrame;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nStripsVertical)
			{
				if ((nPatternX < dCenterX && nPatternY < dLevelY) ||
					(nPatternX >= dCenterX && nPatternY > (nSizeY - 1) - dLevelY))
					nPatternValue = 1;
			}
			else if (Type == c_nStripsHorizontal)
			{
				if ((nPatternY < dCenterY && nPatternX < dLevelX) ||
					(nPatternY >= dCenterY && nPatternX > (nSizeX - 1) - dLevelX))
					nPatternValue = 1;
			}

			// combine colors
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::WipeSide(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nSideLeftToRight)
			{
				if (nPatternX <= nLevelX)
					nPatternValue = 1;
			}
			else if (Type == c_nSideRightToLeft)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX)
					nPatternValue = 1;
			}
			else if (Type == c_nSideBottomToTop)
			{
				if (nPatternY <= nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nSideTopToBottom)
			{
				if (nPatternY >= (nSizeY - 1) - nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nSideLeftTopToRightBottom)
			{
				if (nPatternX <= nLevelX && nPatternY >= (nSizeY - 1) - nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nSideLeftBottomToRightTop)
			{
				if (nPatternX <= nLevelX && nPatternY <= nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nSideRightBottomToLeftTop)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX && nPatternY <= nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nSideRightTopToLeftBottom)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX && nPatternY >= (nSizeY - 1) - nLevelY)
					nPatternValue = 1;
			}

			// combine colors
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::WipeCenter(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;

	double dCenterX = 0.5*(nSizeX - 1);
	double dCenterY = 0.5*(nSizeY - 1);

	double dLevelX = 0.5*nSizeX*m_dFrame;
	double dLevelY = 0.5*nSizeY*m_dFrame;

	double dRadius;
	double dRadiusMin = 0.25*(nSizeX*nSizeX + nSizeY*nSizeY)*m_dFrame;
	double dRadiusMax = 0.25*(nSizeX*nSizeX + nSizeY*nSizeY)*(1.0 - m_dFrame);

	double dDiamond;
	double dDiamondMin = 0.5*(nSizeX + nSizeY)*m_dFrame;
	double dDiamondMax = 0.5*(nSizeX + nSizeY)*(1.0 - m_dFrame);

	double dCross1X = 0.5*(nSizeX - 1)*m_dFrame;
	double dCross1Y = 0.5*(nSizeY - 1)*m_dFrame;
	double dCross2X = 0.5*(nSizeX - 1)*(1.0 - m_dFrame);
	double dCross2Y = 0.5*(nSizeY - 1)*(1.0 - m_dFrame);
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nCenterBoxIn)
			{
				if (nPatternX < dLevelX || nPatternX > (nSizeX - 1) - dLevelX || nPatternY < dLevelY || nPatternY > (nSizeY - 1) - dLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nCenterBoxOut)
			{
				if (nPatternX > dCenterX - dLevelX && nPatternX < dCenterX + dLevelX && 
					nPatternY > dCenterY - dLevelY && nPatternY < dCenterY + dLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nCenterCircleIn)
			{
				dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
				if (dRadius > dRadiusMax)
					nPatternValue = 1;
			}
			else if (Type == c_nCenterCircleOut)
			{
				dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
				if (dRadius < dRadiusMin)
					nPatternValue = 1;
			}
			else if (Type == c_nCenterDiamondIn)
			{
				dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
				if (dDiamond > dDiamondMax)
					nPatternValue = 1;
			}
			else if (Type == c_nCenterDiamondOut)
			{
				dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
				if (dDiamond < dDiamondMin)
					nPatternValue = 1;
			}
			else if (Type == c_nCenterCrossIn)
			{
				if ((nPatternY <= dCross1Y && nPatternX <= dCross1X) ||
					(nPatternY <= dCross1Y && nPatternX >= (nSizeX - 1) - dCross1X) ||
					(nPatternY >= (nSizeY - 1) - dCross1Y && nPatternX >= (nSizeX - 1) - dCross1X) ||
					(nPatternY >= (nSizeY - 1) - dCross1Y && nPatternX <= dCross1X))
					nPatternValue = 1;
			}
			else if (Type == c_nCenterCrossOut)
			{
				if ((nPatternY >= dCross2Y && nPatternY <= (nSizeY - 1) - dCross2Y) ||
					(nPatternX >= dCross2X && nPatternX <= (nSizeX - 1) - dCross2X))
					nPatternValue = 1;
			}

			// combine colors
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}
BOOL ImageCompose3::WipeChecker(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nPatternY_Tmp;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nPatternY_Tmp = nPatternY;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute color value
			if (Type == c_nSideLeftToRight)
			{
				if ( nPatternY >= (nSizeY-1)/2 )
					nPatternX += ( nPatternX <= (nSizeX-1)/2 ) ? (nSizeX-1)/2 : -(nSizeX-1)/2;
				if ( nPatternX <= nLevelX  )
					nPatternValue = 1;
			}
			else if (Type == c_nSideRightToLeft)
			{
				if ( nPatternY >= (nSizeY-1)/2 )
					nPatternX += ( nPatternX <= (nSizeX-1)/2 ) ? (nSizeX-1)/2 : -(nSizeX-1)/2;
				
				if (nPatternX >= (nSizeX - 1) - nLevelX)
					nPatternValue = 1;
			}
			else if (Type == c_nSideBottomToTop)
			{
				if ( nPatternX >= (nSizeX-1)/2 )
					nPatternY_Tmp += ( nPatternY <= (nSizeY-1)/2 ) ? (nSizeY-1)/2 : -(nSizeY-1)/2;

				if (nPatternY_Tmp <= nLevelY)
					nPatternValue = 1;
			}
			else if (Type == c_nSideTopToBottom)
			{
				if ( nPatternX >= (nSizeX-1)/2 )
					nPatternY_Tmp += ( nPatternY <= (nSizeY-1)/2 ) ? (nSizeY-1)/2 : -(nSizeY-1)/2;

				if (nPatternY_Tmp >= (nSizeY - 1) - nLevelY)
					nPatternValue = 1;
			}
	
			// combine colors
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::WipeClock(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex, nSectorIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nParts = 1;
	double dSector = 3.1415*2;
	double dAngle = 3.1415*m_dFrame*((Type == c_nBottomToTopOpposite || Type == c_nTopToBottomOpposite || 
									  Type == c_nLeftToRightOpposite || Type == c_nRightToLeftOpposite ) ? 1 : 2);
	double dStartAngle = 3.1415 * 0.5;
	double dR, dA;

	// compute starting index
	nIndex = 0;

	if ( Type == c_n2AngleClockwise  || Type == c_n2AngleCounterClockwise  )
		nParts = 2;
	else if ( Type == c_n3AngleClockwise  || Type == c_n3AngleCounterClockwise  )
		nParts = 3;
	else if ( Type == c_n4AngleClockwise  || Type == c_n4AngleCounterClockwise  )
		nParts = 4;
	else if ( Type == c_n6AngleClockwise  || Type == c_n6AngleCounterClockwise  )
		nParts = 6;
	else if ( Type == c_n8AngleClockwise  || Type == c_n8AngleCounterClockwise  )
		nParts = 8;
	else if ( Type == c_n12AngleClockwise  || Type == c_n12AngleCounterClockwise  )
		nParts = 12;

	dAngle /= nParts;
	dSector /= nParts;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// reset pattern value
			nPatternValue = 0;

			// compute color value
	
			if (Type == c_nBottomToTopClockwise || Type == c_nBottomToTopCounterClockwise || Type == c_nBottomToTopOpposite)
				dStartAngle = -3.1415 * 0.5;
			else if (Type == c_nLeftToRightClockwise || Type == c_nLeftToRightCounterClockwise || Type == c_nLeftToRightOpposite)
				dStartAngle = 3.1415 ;
			else if (Type == c_nRightToLeftClockwise || Type == c_nRightToLeftCounterClockwise || Type == c_nRightToLeftOpposite)
				dStartAngle = 0.;
			
			// compute pixel radius and angle
			GetRadiusAndAngle(nPatternX, nPatternY, 0.5*(nSizeX - 1), 0.5*(nSizeY - 1), dR, dA);
				
			dA -= dStartAngle;
			
			if ( Type == c_nBottomToTopClockwise || Type == c_nTopToBottomClockwise ||
				 Type == c_nLeftToRightClockwise || Type == c_nRightToLeftClockwise ||
				 Type == c_n3AngleClockwise  || Type == c_n2AngleClockwise ||
				 Type == c_n4AngleClockwise  || Type == c_n6AngleClockwise ||
				 Type == c_n8AngleClockwise  || Type == c_n12AngleClockwise )
				 dA *= -1;
			
			if ( dA < 0 ) dA += 3.1415 * 2;
				
			for ( nSectorIndex=0; nSectorIndex<nParts; ++nSectorIndex )
				if ( dA-nSectorIndex*dSector >=0 && dA-nSectorIndex*dSector <=dAngle )
				{	
					nPatternValue = 1;
					break;
				}
			if ( dA >= (3.1415 * 2 - dAngle) && ( Type == c_nBottomToTopOpposite || 
												  Type == c_nTopToBottomOpposite ||
												  Type == c_nLeftToRightOpposite ||
												  Type == c_nRightToLeftOpposite ) )
				nPatternValue = 1;

			// combine colors
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::WipeStripsRandom(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;
	
	// variables
	int nX, nY, nIndex;
	int nPatternValue;
	int nLevel = (int)( 100*m_dFrame );
	int nPosValue, nTile;
	
	// compute starting index
	nIndex = 0;
	//srand(1);
	// combine two images
	if ( Type == c_nStripsHorizontal )
		for (nY = 0; nY < m_first.Height; ++nY)
		{
			nPosValue = rand()%100;
			nPatternValue = ( nPosValue <= nLevel ) ? 1 : 0;
			nTile = 0;

			for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
			{
				if ( nTile != nX/nSizeX )
				{
					nTile = nX/nSizeX;
					nPosValue = rand()%100;
					// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
					nPatternValue = ( nPosValue <= nLevel ) ? 1 : 0;
				}
				// combine colors
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				}
			}
		}
	else if ( Type == c_nStripsVertical )
		for (nX = 0; nX < m_first.Width; ++nX)
		{
			nPosValue = rand()%100;
			nPatternValue = ( nPosValue <= nLevel ) ? 1 : 0;
			nTile = 0;
			
			for (nY = 0; nY < m_first.Height; ++nY)
			{
				nIndex = (nY * m_first.Width + nX)*4;
				if ( nTile != nY/nSizeY )
				{
					nTile = nY/nSizeY;
					nPosValue = rand()%100;
					// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
					nPatternValue = ( nPosValue <= nLevel ) ? 1 : 0;
				}
				// combine colors
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				}
			}
		}
		
	// all ok
	return TRUE;
}

BOOL ImageCompose3::WipeFadeSide(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int		nX, nY, nIndex;
	int		nPatternX, nPatternY, nPatternValue;
	int		nLevelLast, nLevelFirst;
	double  dLevel = m_dFrame * 1.25;
	int		nAddFadeLevel = 0;	
	int		nFadeSize = ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft ) ? (nSizeX)/4 : (nSizeY)/4;
	double	dAlfa;
	int		nCheckPoint = 0;
	int		nWidthInBytes = m_first.Width * 4;

	if ( dLevel <= 0.25 )
	{
		if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft )
		{
			nLevelFirst = (int)(dLevel * (nSizeX-1));
			nLevelLast = 0;
			nAddFadeLevel = nFadeSize - nLevelFirst;
		}
		else if ( Type == c_nSideBottomToTop || Type == c_nSideTopToBottom )
		{
			nLevelFirst = (int)(dLevel * (nSizeY-1));
			nLevelLast = 0;
			nAddFadeLevel = nFadeSize - nLevelFirst;
		}
	}		
	else if ( dLevel >= 1. )
	{
		if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft )
		{
			nLevelFirst = nSizeX-1;
			nLevelLast = (int)((dLevel-0.25) * (nSizeX-1));
		}
		else if ( Type == c_nSideBottomToTop || Type == c_nSideTopToBottom )
		{
			nLevelFirst = nSizeY-1;
			nLevelLast = (int)((dLevel-0.25) * (nSizeY-1));
		}
	}
	else
	{
		if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft )
		{
			nLevelFirst = (int)(dLevel * (nSizeX-1));
			nLevelLast = nLevelFirst - nFadeSize;
		}
		else if ( Type == c_nSideBottomToTop || Type == c_nSideTopToBottom )
		{
			nLevelFirst = (int)(dLevel * (nSizeY-1));
			nLevelLast = nLevelFirst - nFadeSize;
		}
	}

	// compute starting index
	nIndex = 0;
	if (Type == c_nSideBottomToTop || Type == c_nSideTopToBottom)
		// combine two images
		for (nY = 0; nY < m_first.Height; ++nY)
		{
			// compute pattern Y coordinate
			nPatternY = nY % nSizeY;
			nPatternValue = 0;
		
			nCheckPoint = ( Type == c_nSideBottomToTop ) ? nPatternY : (nSizeY-1 - nPatternY);
			if (nCheckPoint < nLevelLast)
				nPatternValue = 1;
			else if (nCheckPoint <= nLevelFirst)
			{
				nPatternValue = 2;
				dAlfa = (double)(nCheckPoint - nLevelLast + nAddFadeLevel)/nFadeSize;
			}
			for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
			{
				// combine colors
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
				else if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				} 
				else
				{
					m_result.Pixels[nIndex + 2] = (BYTE)(m_first.Pixels[nIndex + 2]*dAlfa + m_last.Pixels[nIndex + 2]*(1-dAlfa));
					m_result.Pixels[nIndex + 1] = (BYTE)(m_first.Pixels[nIndex + 1]*dAlfa + m_last.Pixels[nIndex + 1]*(1-dAlfa));
					m_result.Pixels[nIndex + 0] = (BYTE)(m_first.Pixels[nIndex + 0]*dAlfa + m_last.Pixels[nIndex + 0]*(1-dAlfa));
				}
			}
		}
	else if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft )
		for (nX = 0; nX < m_first.Width; ++nX)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nPatternValue = 0;

			// compute color value
			nCheckPoint = ( Type == c_nSideLeftToRight ) ? nPatternX : (nSizeX-1 - nPatternX);	
			if (nCheckPoint < nLevelLast)
				nPatternValue = 1;
			else if (nCheckPoint <= nLevelFirst)
			{
				nPatternValue = 2;
				dAlfa = (double)(nCheckPoint - nLevelLast + nAddFadeLevel)/nFadeSize;
			}

			nIndex = nX*4;
			for (nY = 0; nY < m_first.Height; ++nY)
			{
				// combine colors
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
				else if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				} 
				else
				{
					m_result.Pixels[nIndex + 2] = (BYTE)(m_first.Pixels[nIndex + 2]*dAlfa + m_last.Pixels[nIndex + 2]*(1-dAlfa));
					m_result.Pixels[nIndex + 1] = (BYTE)(m_first.Pixels[nIndex + 1]*dAlfa + m_last.Pixels[nIndex + 1]*(1-dAlfa));
					m_result.Pixels[nIndex + 0] = (BYTE)(m_first.Pixels[nIndex + 0]*dAlfa + m_last.Pixels[nIndex + 0]*(1-dAlfa));
				}
				nIndex += nWidthInBytes;
			}
		}
	// all ok
	return TRUE;
}
//BOOL ImageCompose3::WipeCircleCorner(int Type)
//{
//	// compute tile size
//	int nSizeX = m_nTileSizeX;
//	int nSizeY = m_nTileSizeY;
//
//	// variables
//	int nX, nY, nIndex;
//	int nPatternX, nPatternY, nPatternValue;
//
//	int nCenterX;
//	int nCenterY;
//	double dMaxRadius = (nSizeX*nSizeX + nSizeY*nSizeY)*m_dFrame;
//	double dRadius;
//	if ( Type == c_nCircleCornerLeftBottom )
//	{
//		nCenterY = nCenterX = 0;
//	}
//	else if ( Type == c_nCircleCornerLeftTop )
//	{
//		nCenterX = 0;
//		nCenterY = nSizeY-1;
//	}
//	else if ( Type == c_nCircleCornerRightBottom )
//	{
//		nCenterX = nSizeX-1;
//		nCenterY = 0;
//	}
//	else if ( Type == c_nCircleCornerRightTop )
//	{
//		nCenterX = nSizeX-1;	
//		nCenterY = nSizeY-1;
//	}
//	// compute starting index
//	nIndex = 0;
//
//	// combine two images
//	for (nY = 0; nY < m_first.Height; ++nY)
//	{
//		// compute pattern Y coordinate
//		nPatternY = nY % nSizeY;
//
//		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
//		{
//			// compute pattern X coordinate
//			nPatternX = nX % nSizeX;
//
//			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
//			nPatternValue = 0;
//
//			// compute color value
//			dRadius = (nPatternX - nCenterX)*(nPatternX - nCenterX) + (nPatternY - nCenterY)*(nPatternY - nCenterY);
//			if ( dRadius < dMaxRadius )
//				nPatternValue = 1;
//
//			// combine colors
//			if (nPatternValue == 0)
//			{
//				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
//				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
//				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
//			}
//			else
//			{
//				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
//				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
//				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
//			}
//		}
//	}
//
//	// all ok
//	return TRUE;
//}
BOOL ImageCompose3::WipeClockCorner(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int		nX, nY, nIndex;
	int		nPatternX, nPatternY, nPatternValue;
	double  dMaxTan;
	double  dTan;
	int		nCenterY=0, nCenterX=0;
	// compute starting index
	nIndex = 0;
	if (Type == c_nCircleCornerLeftBottomClockwise || Type == c_nCircleCornerLeftTop ||
		Type == c_nCircleCornerRightBottom || Type == c_nCircleCornerRightTopClockwise )
		dMaxTan = tan(3.1415*0.5 *(1-m_dFrame));
	else
		dMaxTan = tan(3.1415*0.5 * m_dFrame);
	if ( Type == c_nCircleCornerLeftBottom || Type == c_nCircleCornerLeftBottomClockwise )
	{
		nCenterY = nCenterX = 0;
	}
	else if ( Type == c_nCircleCornerLeftTop || Type == c_nCircleCornerLeftTopClockwise )
	{
		nCenterX = 0;
		nCenterY = nSizeY-1;
	}
	else if ( Type == c_nCircleCornerRightBottom || Type == c_nCircleCornerRightBottomClockwise )
	{
		nCenterX = nSizeX-1;
		nCenterY = 0;
	}
	else if ( Type == c_nCircleCornerRightTop || Type == c_nCircleCornerRightTopClockwise )
	{
		nCenterX = nSizeX-1;	
		nCenterY = nSizeY-1;
	}


	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// reset pattern value
			nPatternValue = 0;

			// compute color value
	
			if (Type == c_nCircleCornerLeftBottom || Type == c_nCircleCornerLeftTopClockwise ||
				Type == c_nCircleCornerRightBottomClockwise || Type == c_nCircleCornerRightTop )
			{
				if ( nPatternX != nCenterX )
				{
					dTan = fabs((double)(nPatternY - nCenterY)/(nPatternX - nCenterX));
					if ( dTan < dMaxTan )
						nPatternValue = 1;
				}
			}
			else
			{
				if ( nPatternX == nCenterX )
					nPatternValue = 1;
				else
				{
					dTan = fabs((double)(nPatternY - nCenterY)/(nPatternX - nCenterX));
					if ( dTan > dMaxTan )
						nPatternValue = 1;
				}
			}
			
			// combine colors
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

	
BOOL ImageCompose3::WipeSmooth(int Type)
{
	GraphicsPath curPath;
	switch (Type)
	{
	case 0: 
	case 1:    // circle, circles
		curPath.AddEllipse(10, 10, 80,80);
		break;
	case 2:
	case 3:   // diamond, diamonds
		curPath.AddLine(50, 10, 10, 50);
		curPath.AddLine(10, 50, 50, 90);
		curPath.AddLine(50, 90, 90, 50);
		curPath.CloseFigure();
		break;
	case 4:
	case 5:  // star, stars
		{
			const int start_angle = 90;
			const int anglesCnt = 5;
			const int radius		= 45;
			const int inner_radius  = 15;
			const int center_x		= 50;
			const int center_y		= 50;
			#define DEGREE          0.017453292519943295769236907684886 
			
			PointF* points = new PointF[anglesCnt*2];
		
			int index = 0;

			for (int a = start_angle; a < 360 + start_angle; a += 180/anglesCnt)
			{
				if (index >= anglesCnt*2) 
					break;
				points[index] = PointF(
					center_x + (int)(((index % 2 == 0) ? radius : inner_radius) * cos(DEGREE * a)),
					center_y + (int)(((index % 2 == 0) ? radius : inner_radius) * sin(DEGREE * a)));
				index++;
			}
			
			curPath.AddLines(points,anglesCnt*2);

			curPath.CloseFigure();

			delete[] points;
		}
		break;
	case 6:
	case 7:   // keyhole, keyholes
		curPath.AddLine(30, 10, 40, 45);
		curPath.AddBezier(40,45, 20, 90, 80,90, 60,45);
		curPath.AddLine(60, 45, 70, 10);
		curPath.CloseFigure();
		break;
	default:
		return FALSE;
	}
	WipePathCommon(m_nTilesX, m_nTilesY,&curPath);
	return TRUE;
}
// slide
BOOL ImageCompose3::SlideDoor(int Type)
{
	// compute tile size
	int nTile = 8;
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	int nLevel1X = (int)( 0.5*(nSizeX - 1)*m_dFrame );
	int nLevel1Y = (int)( 0.5*(nSizeY - 1)*m_dFrame );
	int nLevel2X = (int)( 0.5*(nSizeX - 1)*(1.0 - m_dFrame) );
	int nLevel2Y = (int)( 0.5*(nSizeY - 1)*(1.0 - m_dFrame) );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nDoorHorizontalIn /*c_nDoorVerticalIn*/)
			{
				if (nPatternY <= nLevel1Y)
				{
					nSourceY = nY + (nSizeY - 1)/2 - nLevel1Y;
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevel1Y)
				{
					nSourceY = nY + nLevel1Y - (nSizeY - 1)/2;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nDoorHorizontalOut /*c_nDoorVerticalOut*/)
			{
				if (nPatternY >= nLevel2Y && nPatternY <= (nSizeY - 1)/2)
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
			else if (Type == c_nDoorVerticalIn /*c_nDoorHorizontalIn*/)
			{
				if (nPatternX <= nLevel1X)
				{
					nSourceX = nX + (nSizeX - 1)/2 - nLevel1X;
					nPatternValue = 1;
				}
				else if (nPatternX >= (nSizeX - 1) - nLevel1X)
				{
					nSourceX = nX + nLevel1X - (nSizeX - 1)/2;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nDoorVerticalOut /*c_nDoorHorizontalOut*/)
			{
				if (nPatternX >= nLevel2X && nPatternX <= (nSizeX - 1)/2)
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
			else if (Type == c_nDoorDiagonal)
			{
				if (nPatternY <= nLevelY && nPatternX <= nLevelX && nPatternY >= (nSizeY - 1) - nLevelY && nPatternX >= (nSizeX - 1) - nLevelX)
				{
					if ( (nX/nTile % 2 && !(nY/nTile % 2)) || (!(nX/nTile % 2) && nY/nTile % 2) )
					{
						nSourceY = nY + (nSizeY - 1) - nLevelY;
						nSourceX = nX + (nSizeX - 1) - nLevelX;
					}
					else
					{
						nSourceY = nY + nLevelY - (nSizeY - 1);
						nSourceX = nX + nLevelX - (nSizeX - 1);
					}
					
					nPatternValue = 1;
				}
				else if (nPatternY <= nLevelY && nPatternX <= nLevelX)
				{
					nSourceY = nY + (nSizeY - 1) - nLevelY;
					nSourceX = nX + (nSizeX - 1) - nLevelX;
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevelY && nPatternX >= (nSizeX - 1) - nLevelX)
				{
					nSourceY = nY + nLevelY - (nSizeY - 1);
					nSourceX = nX + nLevelX - (nSizeX - 1);
					nPatternValue = 1;
				}
			}
			else if (Type == c_nDoorDiagonalRotated)
			{
				if (nPatternY >= (nSizeY - 1) - nLevelY && nPatternX <= nLevelX && nPatternY <= nLevelY && nPatternX >= (nSizeX - 1) - nLevelX)
				{
					if ( (nX/nTile % 2 && !(nY/nTile % 2)) || (!(nX/nTile % 2) && nY/nTile % 2) )
					{
						nSourceY = nY + nLevelY - (nSizeY - 1);
						nSourceX = nX + (nSizeX - 1) - nLevelX;
					}
					else
					{
						nSourceX = nX + nLevelX - (nSizeX - 1);
						nSourceY = nY + (nSizeY - 1) - nLevelY;
					}
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevelY && nPatternX <= nLevelX)
				{
					nSourceY = nY + nLevelY - (nSizeY - 1);
					nSourceX = nX + (nSizeX - 1) - nLevelX;
					nPatternValue = 1;
				}
				else if (nPatternY <= nLevelY && nPatternX >= (nSizeX - 1) - nLevelX)
				{
					nSourceX = nX + nLevelX - (nSizeX - 1);
					nSourceY = nY + (nSizeY - 1) - nLevelY;
					nPatternValue = 1;
				}
			}

			// combine colors
			if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_last.Width);

				m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::SlideStrips(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nCenterX = (nSizeX - 1)/2;
	int nCenterY = (nSizeY - 1)/2;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nStripsVertical)
			{
				if (nPatternX <= nCenterX)
				{
					if (nPatternY <= nLevelY)
					{
						nSourceY = nY + (nSizeY - 1) - nLevelY;
						nPatternValue = 1;
					}
				}
				else
				{
					if (nPatternY >= (nSizeY - 1) - nLevelY)
					{
						nSourceY = nY + nLevelY - (nSizeY - 1);
						nPatternValue = 1;
					}
				}
			}
			else if (Type == c_nStripsHorizontal)
			{
				if (nPatternY <= nCenterY)
				{
					if (nPatternX <= nLevelX)
					{
						nSourceX = nX + (nSizeX - 1) - nLevelX;
						nPatternValue = 1;
					}
				}
				else
				{
					if (nPatternX >= (nSizeX - 1) - nLevelX)
					{
						nSourceX = nX + nLevelX - (nSizeX - 1);
						nPatternValue = 1;
					}
				}
			}

			// combine colors
			if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_last.Width);

				m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::SlideSide(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nSideLeftToRight)
			{
				if (nPatternX <= nLevelX)
				{
					nSourceX = nX + (nSizeX - 1) - nLevelX;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideRightToLeft)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX)
				{
					nSourceX = nX + nLevelX - (nSizeX - 1);
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideBottomToTop)
			{
				if (nPatternY <= nLevelY)
				{
					nSourceY = nY + (nSizeY - 1) - nLevelY;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideTopToBottom)
			{
				if (nPatternY >= (nSizeY - 1) - nLevelY)
				{
					nSourceY = nY + nLevelY - (nSizeY - 1);
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideLeftTopToRightBottom)
			{
				if (nPatternX <= nLevelX && nPatternY >= (nSizeY - 1) - nLevelY)
				{
					nSourceX = nX + (nSizeX - 1) - nLevelX;
					nSourceY = nY + nLevelY - (nSizeY - 1);
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideLeftBottomToRightTop)
			{
				if (nPatternX <= nLevelX && nPatternY <= nLevelY)
				{
					nSourceX = nX + (nSizeX - 1) - nLevelX;
					nSourceY = nY + (nSizeY - 1) - nLevelY;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideRightBottomToLeftTop)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX && nPatternY <= nLevelY)
				{
					nSourceX = nX + nLevelX - (nSizeX - 1);
					nSourceY = nY + (nSizeY - 1) - nLevelY;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideRightTopToLeftBottom)
			{
				if (nPatternX >= (nSizeX - 1) - nLevelX && nPatternY >= (nSizeY - 1) - nLevelY)
				{
					nSourceX = nX + nLevelX - (nSizeX - 1);
					nSourceY = nY + nLevelY - (nSizeY - 1);
					nPatternValue = 1;
				}
			}

			// combine colors
			if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_last.Width);

				m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
				m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
				m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::SlideCenter(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevel1X = (int)( 0.5*(nSizeX - 1)*m_dFrame );
	int nLevel1Y = (int)( 0.5*(nSizeY - 1)*m_dFrame );
	int nLevel2X = (int)( 0.5*(nSizeX - 1)*(1.0 - m_dFrame) );
	int nLevel2Y = (int)( 0.5*(nSizeY - 1)*(1.0 - m_dFrame) );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nCenterCrossIn)
			{
				if (nPatternY <= nLevel1Y && nPatternX <= nLevel1X)
				{
					nSourceX = nX + (nSizeX - 1)/2 - nLevel1X;
					nSourceY = nY + (nSizeY - 1)/2 - nLevel1Y;
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevel1Y && nPatternX <= nLevel1X)
				{
					nSourceX = nX + (nSizeX - 1)/2 - nLevel1X;
					nSourceY = nY + nLevel1Y - (nSizeY - 1)/2;
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevel1Y && nPatternX >= (nSizeX - 1) - nLevel1X)
				{
					nSourceX = nX + nLevel1X - (nSizeX - 1)/2;
					nSourceY = nY + nLevel1Y - (nSizeY - 1)/2;
					nPatternValue = 1;
				}
				else if (nPatternY <= nLevel1Y && nPatternX >= (nSizeX - 1) - nLevel1X)
				{
					nSourceX = nX + nLevel1X - (nSizeX - 1)/2;
					nSourceY = nY + (nSizeY - 1)/2 - nLevel1Y;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nCenterCrossOut)
			{
				if (nPatternY <= nLevel2Y && nPatternX <= nLevel2X)
				{
					nSourceX = nX + (nSizeX - 1)/2 - nLevel2X;
					nSourceY = nY + (nSizeY - 1)/2 - nLevel2Y;
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevel2Y && nPatternX <= nLevel2X)
				{
					nSourceX = nX + (nSizeX - 1)/2 - nLevel2X;
					nSourceY = nY + nLevel2Y - (nSizeY - 1)/2;
					nPatternValue = 1;
				}
				else if (nPatternY >= (nSizeY - 1) - nLevel2Y && nPatternX >= (nSizeX - 1) - nLevel2X)
				{
					nSourceX = nX + nLevel2X - (nSizeX - 1)/2;
					nSourceY = nY + nLevel2Y - (nSizeY - 1)/2;
					nPatternValue = 1;
				}
				else if (nPatternY <= nLevel2Y && nPatternX >= (nSizeX - 1) - nLevel2X)
				{
					nSourceX = nX + nLevel2X - (nSizeX - 1)/2;
					nSourceY = nY + (nSizeY - 1)/2 - nLevel2Y;
					nPatternValue = 1;
				}
			}

			// combine colors
			if (nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_first.Width);

				if ((Type == c_nCenterCrossIn && nPatternValue == 1) || (Type == c_nCenterCrossOut && nPatternValue == 0))
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
				}
				else if ((Type == c_nCenterCrossIn && nPatternValue == 0) || (Type == c_nCenterCrossOut && nPatternValue == 1))
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nSourceIndex + 0];
				}
			}
		}
	}

	// all ok
	return TRUE;
}
BOOL ImageCompose3::SlideChange(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX1 = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY1 = (int)( (nSizeY - 1)*m_dFrame );
	int nLevelX2 = (int)( (nSizeX - 1)*(1-m_dFrame) );
	int nLevelY2 = (int)( (nSizeY - 1)*(1-m_dFrame) );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nSideLeftToRight)
			{
				if (nPatternX <= nLevelX1)
				{
					nSourceX += ( nLevelX1 <= 0.5*(nSizeX-1) ) ? nLevelX1 : nLevelX2;
					nPatternValue = 1;
				}
				else
				{
					nSourceX -= ( nLevelX1 <= 0.5*(nSizeX-1) ) ? nLevelX1 : nLevelX2;
					nPatternValue = 0;
				}
			}
			else if (Type == c_nSideRightToLeft)
			{

				if (nPatternX <= nLevelX2)
				{
					nSourceX += ( nLevelX1 <= 0.5*(nSizeX-1) ) ? nLevelX1 : nLevelX2;
					nPatternValue = 0;
				}
				else
				{
					nSourceX -= ( nLevelX1 <= 0.5*(nSizeX-1) ) ? nLevelX1 : nLevelX2;
					nPatternValue = 1;
				}
			}
			else if (Type == c_nSideBottomToTop)
			{
				if (nPatternY <= nLevelY1)
				{
					nSourceY += ( nLevelY1 <= 0.5*(nSizeY-1) ) ? nLevelY1 : nLevelY2;
					nPatternValue = 1;
				}
				else
				{
					nSourceY -= ( nLevelY1 <= 0.5*(nSizeY-1) ) ? nLevelY1 : nLevelY2;
					nPatternValue = 0;
				}
			}
			else if (Type == c_nSideTopToBottom)
			{
				if (nPatternY <= nLevelY2)
				{
					nSourceY += ( nLevelY1 <= 0.5*(nSizeY-1) ) ? nLevelY1 : nLevelY2;
					nPatternValue = 0;
				}
				else
				{
					nSourceY -= ( nLevelY1 <= 0.5*(nSizeY-1) ) ? nLevelY1 : nLevelY2;
					nPatternValue = 1;
				}
			}

			// combine colors
			if (nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_result.Width);
				
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nSourceIndex + 0];
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
				}
			}
		}
	}

	// all ok
	return TRUE;
}
BOOL ImageCompose3::SlideBoxes(int Type)
{
    // compute tile size
    int nSizeX = m_nTileSizeX;
    int nSizeY = m_nTileSizeY;
    
    // variables
    int nX, nY, nIndex, nBoxIndex;
    int nSourceX, nSourceY, nSourceIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dBoxWidth = 0.05;
	int nFilledBoxes = ( m_dFrame <= 0.5 )? 0 : ((int)((m_dFrame-0.5)/(dBoxWidth/2) + 1));
	m_dFrame -= dBoxWidth*nFilledBoxes;
	int nBoxes = (int)(m_dFrame/dBoxWidth);
    // compute starting index
    nIndex = 0;
    
    // combine two images
    for (nY = 0; nY < m_first.Height; ++nY)
    {
        // compute pattern Y coordinate
        nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;
        
        for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
        {
            // compute pattern X coordinate
            nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;
            
            // compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
            nPatternValue = 0;
            
            // compute default source pixel coordinates
            nSourceX = nX;
            nSourceY = nY;
            
            // compute color value
            if (Type == c_nSideLeftToRight)
            {
				if ( nFilledBoxes > 0 && nPatternX >= (int)((nSizeX - 1)*(1 - dBoxWidth*nFilledBoxes)) )				
					nPatternValue = 1;
				for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
                {
					if ( nPatternX <= (int)(2*(nSizeX-1)*(m_dFrame - dBoxWidth*nBoxIndex)) && nPatternX >= (int)(2*(nSizeX-1)*(m_dFrame - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
					{
                        nSourceX = nTileX * nSizeX + (int)(nPatternX + (nSizeX - 1)*(1 - 2*m_dFrame + dBoxWidth*(nBoxIndex - nFilledBoxes)));
						nPatternValue = 1;
                    }
                }
            }
            else if (Type == c_nSideRightToLeft)
            {
				if ( nFilledBoxes > 0 && nPatternX <= (int)((nSizeX - 1)*dBoxWidth*nFilledBoxes) )
					nPatternValue = 1;
                for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
                {
					if ( nPatternX >= nSizeX - 1 - (int)(2*(nSizeX-1)*(m_dFrame - dBoxWidth*nBoxIndex)) && nPatternX <= nSizeX - 1 - (int)(2*(nSizeX-1)*(m_dFrame - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
                    {
                        nSourceX = nTileX * nSizeX + (int)(nPatternX - (nSizeX - 1)*(1 - 2*m_dFrame + dBoxWidth*(nBoxIndex - nFilledBoxes)));
						nPatternValue = 1;
                    }
                }
            }
            if (Type == c_nSideBottomToTop)
            {
				if ( nFilledBoxes > 0 && nPatternY >= (int)((nSizeY- 1)*(1 - dBoxWidth*nFilledBoxes)) )
					nPatternValue = 1;
                for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
                {
					if ( nPatternY <= (int)(2*(nSizeY-1)*(m_dFrame - dBoxWidth*nBoxIndex)) && nPatternY >= (int)(2*(nSizeY-1)*(m_dFrame - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
                    {
						nSourceY = nTileY * nSizeY + (int)(nPatternY + (nSizeY - 1)*(1 - 2*m_dFrame + dBoxWidth*(nBoxIndex - nFilledBoxes)));
                        nPatternValue = 1;
                    }
                }
            }
            if (Type == c_nSideTopToBottom)
            {
				if ( nFilledBoxes > 0 && nPatternY <= (int)((nSizeY - 1)*dBoxWidth*nFilledBoxes) )
					nPatternValue = 1;
                for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
                {
					if ( nPatternY >= nSizeY - 1 - (int)(2*(nSizeY-1)*(m_dFrame - dBoxWidth*nBoxIndex)) && nPatternY <= nSizeY - 1 - (int)(2*(nSizeY-1)*(m_dFrame - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
                    {
						nSourceY = nTileY * nSizeY + (int)(nPatternY - (nSizeY - 1)*(1 - 2*m_dFrame + dBoxWidth*(nBoxIndex - nFilledBoxes)));
						nPatternValue = 1;
                    }
                }
            }
            
            // combine colors
            if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
            {
                m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
                m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
                m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
            }
            else
            {
                // compute source pixel index
                nSourceIndex = 4*(nSourceX + nSourceY*m_last.Width);
                
                m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
                m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
                m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
            }
        }
    }
    
    // all ok
    return TRUE;
}
	
// push
BOOL ImageCompose3::PushDoor(int Type)
{
	// compute tile size
	int nTile = 8;
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	int nLevel1X = (int)( 0.5*(nSizeX - 1)*m_dFrame );
	int nLevel1Y = (int)( 0.5*(nSizeY - 1)*m_dFrame );
	int nLevel2X = (int)( 0.5*(nSizeX - 1)*(1.0 - m_dFrame) );
	int nLevel2Y = (int)( 0.5*(nSizeY - 1)*(1.0 - m_dFrame) );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nDoorHorizontalIn /*c_nDoorVerticalIn*/)
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
			else if (Type == c_nDoorHorizontalOut /*c_nDoorVerticalOut*/)
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
			else if (Type == c_nDoorVerticalIn /*c_nDoorHorizontalIn*/)
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
			else if (Type == c_nDoorVerticalOut /*c_nDoorHorizontalOut*/)
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
			if (nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_result.Width);

				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nSourceIndex + 0];
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
				}
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::PushStrips(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nCenterX = (nSizeX - 1)/2;
	int nCenterY = (nSizeY - 1)/2;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nStripsVertical)
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
			else if (Type == c_nStripsHorizontal)
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
			if (nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_result.Width);

				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nSourceIndex + 0];
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
				}
			}
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageCompose3::PushSide(int Type)
{
	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nPatternX, nPatternY, nPatternValue;
	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			nSourceX = nX;
			nSourceY = nY;

			// compute color value
			if (Type == c_nSideLeftToRight)
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
			else if (Type == c_nSideRightToLeft)
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
			else if (Type == c_nSideBottomToTop)
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
			else if (Type == c_nSideTopToBottom)
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
			if (nSourceX < 0 || nSourceX >= m_first.Width || nSourceY < 0 || nSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_result.Width);

				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nSourceIndex + 0];
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nSourceIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nSourceIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nSourceIndex + 0];
				}
			}
		}
	}

	// all ok
	return TRUE;
}
	
// stretch
BOOL ImageCompose3::StretchDoor(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nTile = 8;
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dLevelX = (nSizeX - 1)*m_dFrame;
	double dLevelY = (nSizeY - 1)*m_dFrame;
	double dLevel1X = 0.5*(nSizeX - 1)*m_dFrame;
	double dLevel1Y = 0.5*(nSizeY - 1)*m_dFrame;
	double dLevel2X = 0.5*(nSizeX - 1)*(1.0 - m_dFrame);
	double dLevel2Y = 0.5*(nSizeY - 1)*(1.0 - m_dFrame);
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nDoorVerticalIn)
			{
				if (nPatternY <= dLevel1Y)
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) * nPatternY / max(1, dLevel1Y);
					nPatternValue = 1;
				}
				else if (nPatternY <= 0.5*(nSizeY - 1))
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) * (nPatternY - dLevel1Y) / max(1, (0.5*(nSizeY - 1) - dLevel1Y));
					nPatternValue = 0;
				}
				else if (nPatternY >= (nSizeY - 1) - dLevel1Y)
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) + 0.5*(nSizeY - 1) * (nPatternY - (nSizeY - 1) + dLevel1Y) / max(1, dLevel1Y);
					nPatternValue = 1;
				}
				else if (nPatternY >= 0.5*(nSizeY - 1))
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) + 0.5*(nSizeY - 1) * (nPatternY - 0.5*(nSizeY - 1)) / max(1, (0.5*(nSizeY - 1) - dLevel1Y));
					nPatternValue = 0;
				}
			}
			else if (Type == c_nDoorVerticalOut)
			{
				if (nPatternY <= dLevel2Y)
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) * nPatternY / max(1, dLevel2Y);
					nPatternValue = 0;
				}
				else if (nPatternY >= (nSizeY - 1) - dLevel2Y)
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) + 0.5*(nSizeY - 1) * (nPatternY - (nSizeY - 1) + dLevel2Y) / max(1, dLevel2Y);
					nPatternValue = 0;
				}
				else if (nPatternY >= dLevel2Y && nPatternY <= 0.5*(nSizeY - 1))
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) * (nPatternY - dLevel2Y) / max(1, (0.5*(nSizeY - 1) - dLevel2Y));
					nPatternValue = 1;
				}
				else if (nPatternY <= (nSizeY - 1) - dLevel2Y && nPatternY >= 0.5*(nSizeY - 1))
				{
					dSourceY = nTileY * nSizeY + 0.5*(nSizeY - 1) + 0.5*(nSizeY - 1) * (nPatternY - 0.5*(nSizeY - 1)) / max(1, (0.5*(nSizeY - 1) - dLevel2Y));
					nPatternValue = 1;
				}
			}
			else if (Type == c_nDoorHorizontalIn)
			{
				if (nPatternX <= dLevel1X)
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) * nPatternX / max(1, dLevel1X);
					nPatternValue = 1;
				}
				else if (nPatternX <= 0.5*(nSizeX - 1))
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) * (nPatternX - dLevel1X) / max(1, (0.5*(nSizeX - 1) - dLevel1X));
					nPatternValue = 0;
				}
				else if (nPatternX >= (nSizeX - 1) - dLevel1X)
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) + 0.5*(nSizeX - 1) * (nPatternX - (nSizeX - 1) + dLevel1X) / max(1, dLevel1X);
					nPatternValue = 1;
				}
				else if (nPatternX >= 0.5*(nSizeX - 1))
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) + 0.5*(nSizeX - 1) * (nPatternX - 0.5*(nSizeX - 1)) / max(1, (0.5*(nSizeX - 1) - dLevel1X));
					nPatternValue = 0;
				}
			}
			else if (Type == c_nDoorHorizontalOut)
			{
				if (nPatternX <= dLevel2X)
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) * nPatternX / max(1, dLevel2X);
					nPatternValue = 0;
				}
				else if (nPatternX >= (nSizeX - 1) - dLevel2X)
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) + 0.5*(nSizeX - 1) * (nPatternX - (nSizeX - 1) + dLevel2X) / max(1, dLevel2X);
					nPatternValue = 0;
				}
				else if (nPatternX >= dLevel2X && nPatternX <= 0.5*(nSizeX - 1))
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) * (nPatternX - dLevel2X) / max(1, (0.5*(nSizeX - 1) - dLevel2X));
					nPatternValue = 1;
				}
				else if (nPatternX <= (nSizeX - 1) - dLevel2X && nPatternX >= 0.5*(nSizeX - 1))
				{
					dSourceX = nTileX * nSizeX + 0.5*(nSizeX - 1) + 0.5*(nSizeX - 1) * (nPatternX - 0.5*(nSizeX - 1)) / max(1, (0.5*(nSizeX - 1) - dLevel2X));
					nPatternValue = 1;
				}
			}

			// combine colors
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}
	
#else
	
	if ((Type == c_nDoorVerticalIn)||(Type == c_nDoorVerticalOut))
		IPPStretchDoorHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nDoorVerticalIn));
	else if ((Type == c_nDoorHorizontalIn)||(Type == c_nDoorHorizontalOut))
		IPPStretchDoorVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nDoorHorizontalIn));

#endif

	// all ok
	return TRUE;
}

BOOL ImageCompose3::StretchStrips(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dCenterX = (nSizeX - 1)/2.0;
	double dCenterY = (nSizeY - 1)/2.0;
	double dSourceX, dSourceY;
	double dLevelX = (nSizeX - 1)*m_dFrame;
	double dLevelY = (nSizeY - 1)*m_dFrame;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nStripsVertical)
			{
				if (nPatternX <= dCenterX)
				{
					if (nPatternY <= dLevelY)
					{
						dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, dLevelY);
						nPatternValue = 1;
					}
					else
					{
						dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - dLevelY) / max(1, nSizeY - 1 - dLevelY);
						nPatternValue = 0;
					}
				}
				else
				{
					if (nPatternY >= (nSizeY - 1) - dLevelY)
					{
						dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - (nSizeY - 1) + dLevelY) / max(1, dLevelY);
						nPatternValue = 1;
					}
					else
					{
						dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, (nSizeY - 1) - dLevelY);
						nPatternValue = 0;
					}
				}
			}
			else if (Type == c_nStripsHorizontal)
			{
				if (nPatternY <= dCenterY)
				{
					if (nPatternX <= dLevelX)
					{
						dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, dLevelX);
						nPatternValue = 1;
					}
					else
					{
						dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - dLevelX) / max(1, nSizeX - 1 - dLevelX);
						nPatternValue = 0;
					}
				}
				else
				{
					if (nPatternX >= (nSizeX - 1) - dLevelX)
					{
						dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - (nSizeX - 1) + dLevelX) / max(1, dLevelX);
						nPatternValue = 1;
					}
					else
					{
						dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, (nSizeX - 1) - dLevelX);
						nPatternValue = 0;
					}
				}
			}

			// combine colors
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else

	if (Type == c_nStripsVertical)
		IPPStretchStripsVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);
	else if (Type == c_nStripsHorizontal)
		IPPStretchStripsHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);

#endif
	// all ok
	return TRUE;
}

BOOL ImageCompose3::StretchSide(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dLevelX = (nSizeX - 1)*m_dFrame;
	double dLevelY = (nSizeY - 1)*m_dFrame;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nSideLeftToRight)
			{
				if (nPatternX <= dLevelX)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, dLevelX);
					nPatternValue = 1;
				}
				else
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - dLevelX) / max(1, nSizeX - 1 - dLevelX);
					nPatternValue = 0;
				}
			}
			else if (Type == c_nSideRightToLeft)
			{
				if (nPatternX >= (nSizeX - 1) - dLevelX)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - (nSizeX - 1) + dLevelX) / max(1, dLevelX);
					nPatternValue = 1;
				}
				else
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, (nSizeX - 1) - dLevelX);
					nPatternValue = 0;
				}
			}
			else if (Type == c_nSideBottomToTop)
			{
				if (nPatternY <= dLevelY)
				{
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, dLevelY);
					nPatternValue = 1;
				}
				else
				{
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - dLevelY) / max(1, nSizeY - 1 - dLevelY);
					nPatternValue = 0;
				}
			}
			else if (Type == c_nSideTopToBottom)
			{
				if (nPatternY >= (nSizeY - 1) - dLevelY)
				{
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - (nSizeY - 1) + dLevelY) / max(1, dLevelY);
					nPatternValue = 1;
				}
				else
				{
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, (nSizeY - 1) - dLevelY);
					nPatternValue = 0;
				}
			}
			else if (Type == c_nSideLeftTopToRightBottom)
			{
				if (nPatternX <= dLevelX && nPatternY >= (nSizeY - 1) - dLevelY)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, dLevelX);
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - (nSizeY - 1) + dLevelY) / max(1, dLevelY);
					nPatternValue = 1;
				}
				else
					dSourceX = dSourceY = -1;
			}
			else if (Type == c_nSideLeftBottomToRightTop)
			{
				if (nPatternX <= dLevelX && nPatternY <= dLevelY)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, dLevelX);
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, dLevelY);
					nPatternValue = 1;
				}
				else
					dSourceX = dSourceY = -1;
			}
			else if (Type == c_nSideRightBottomToLeftTop)
			{
				if (nPatternX >= (nSizeX - 1) - dLevelX && nPatternY <= dLevelY)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - (nSizeX - 1) + dLevelX) / max(1, dLevelX);
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, dLevelY);
					nPatternValue = 1;
				}
				else
					dSourceX = dSourceY = -1;
			}
			else if (Type == c_nSideRightTopToLeftBottom)
			{
				if (nPatternX >= (nSizeX - 1) - dLevelX && nPatternY >= (nSizeY - 1) - dLevelY)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - (nSizeX - 1) + dLevelX) / max(1, dLevelX);
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - (nSizeY - 1) + dLevelY) / max(1, dLevelY);
					nPatternValue = 1;
				}
				else
					dSourceX = dSourceY = -1;
			}

			// combine colors
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else
	
	switch (Type)
	{
	case c_nSideLeftToRight:
	case c_nSideRightToLeft:
		IPPStretchSideHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (c_nSideLeftToRight == Type));
		break;
	case c_nSideBottomToTop:
	case c_nSideTopToBottom:
		IPPStretchSideVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (c_nSideTopToBottom == Type));
		break;
	case c_nSideLeftTopToRightBottom:
		IPPStretchSideLTToRB(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);
		break;
	case c_nSideLeftBottomToRightTop:
		IPPStretchSideLBToRT(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);
		break;
	case c_nSideRightBottomToLeftTop:
		IPPStretchSideRBToLT(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);
		break;
	case c_nSideRightTopToLeftBottom:
		IPPStretchSideRTToLB(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);
		break;
	}

#endif	
	
	// all ok
	return TRUE;
}

BOOL ImageCompose3::StretchCenter(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	double dSourceX, dSourceY;
	int nPatternX, nPatternY, nPatternValue;

	double dBox1X = 0.5*(nSizeX - 1)*m_dFrame;
	double dBox1Y = 0.5*(nSizeY - 1)*m_dFrame;
	double dBox2X = 0.5*(nSizeX - 1)*(1.0 - m_dFrame);
	double dBox2Y = 0.5*(nSizeY - 1)*(1.0 - m_dFrame);
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nCenterBoxIn)
			{
				if (dBox1X <= nPatternX && nPatternX <= nSizeX - 1 - dBox1X &&
					dBox1Y <= nPatternY && nPatternY <= nSizeY - 1 - dBox1Y)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - dBox1X) / max(1, nSizeX - 1 - 2*dBox1X);
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - dBox1Y) / max(1, nSizeY - 1 - 2*dBox1Y);
					nPatternValue = 0;
				}
				else
					nPatternValue = 1;
			}
			else if (Type == c_nCenterBoxOut)
			{
				if (dBox2X <= nPatternX && nPatternX <= nSizeX - 1 - dBox2X &&
					dBox2Y <= nPatternY && nPatternY <= nSizeY - 1 - dBox2Y)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - dBox2X) / max(1, nSizeX - 1 - 2*dBox2X);
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - dBox2Y) / max(1, nSizeY - 1 - 2*dBox2Y);
					nPatternValue = 1;
				}
				else
					nPatternValue = 0;
			}

			// combine colors
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else

	IPPStretchCenter(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nCenterBoxIn));

#endif

	// all ok
	return TRUE;
}

BOOL ImageCompose3::StretchWipeSide(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dLevelX = (nSizeX - 1)*m_dFrame;
	double dLevelY = (nSizeY - 1)*m_dFrame;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nStretchLeftToRightIn)
			{
				if (nPatternX <= dLevelX)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, dLevelX);
					nPatternValue = 1;
				}
				else
					nPatternValue = 0;
			}
			else if ( Type == c_nStretchLeftToRightOut )
			{
				if (nPatternX >= dLevelX)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - dLevelX) / max(1, nSizeX - 1 - dLevelX);
					nPatternValue = 0;
				}
				else
					nPatternValue = 1;
			}
			else if ( Type == c_nStretchRightToLeftIn )
			{
				if (nPatternX >= (nSizeX - 1) - dLevelX)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * (nPatternX - (nSizeX - 1) + dLevelX) / max(1, dLevelX);
					nPatternValue = 1;
				} 
				else 
					nPatternValue = 0;
			}
			else if ( Type == c_nStretchRightToLeftOut )
			{
				if (nPatternX <= (nSizeX - 1) - dLevelX)
				{
					dSourceX = nTileX * nSizeX + (nSizeX - 1) * nPatternX / max(1, (nSizeX - 1) - dLevelX);
					nPatternValue = 0;
				} 
				else
					nPatternValue = 1;
			}
			else if (Type == c_nStretchBottomToTopIn)
			{
				if (nPatternY <= dLevelY)
				{
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, dLevelY);
					nPatternValue = 1;
				} 
				else
					nPatternValue = 0;
			}
			else if (Type == c_nStretchBottomToTopOut)
			{
				if (nPatternY >= dLevelY)
				{	dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - dLevelY) / max(1, nSizeY - 1 - dLevelY);
					nPatternValue = 0;
				} 
				else
					nPatternValue = 1;
			}
			else if (Type == c_nStretchTopToBottomIn)
			{
				if (nPatternY >= (nSizeY - 1) - dLevelY)
				{
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * (nPatternY - (nSizeY - 1) + dLevelY) / max(1, dLevelY);
					nPatternValue = 1;
				} 
				else
					nPatternValue = 0;
			}
			else
			{
				if (nPatternY <= (nSizeY - 1) - dLevelY)
				{
					dSourceY = nTileY * nSizeY + (nSizeY - 1) * nPatternY / max(1, (nSizeY - 1) - dLevelY);
					nPatternValue = 0;
				} 
				else
					nPatternValue = 1;
			}

			// combine colors
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else

	switch (Type)
	{
	case c_nStretchLeftToRightIn:
	case c_nStretchLeftToRightOut:
	case c_nStretchRightToLeftIn:
	case c_nStretchRightToLeftOut:
		IPPStretchWipeSideHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, 
			((Type == c_nStretchLeftToRightIn)||(Type == c_nStretchRightToLeftIn)),
			((Type == c_nStretchLeftToRightIn)||(Type == c_nStretchLeftToRightOut)));
		break;
	case c_nStretchBottomToTopIn:
	case c_nStretchBottomToTopOut:
	case c_nStretchTopToBottomIn:
	case c_nStretchTopToBottomOut:
		IPPStretchWipeSideVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, 
			((Type == c_nStretchBottomToTopIn)||(Type == c_nStretchTopToBottomIn)),
			((Type == c_nStretchBottomToTopIn)||(Type == c_nStretchBottomToTopOut)));
		break;
	}

#endif

	// all ok
	return TRUE;
}
	
// swing
BOOL ImageCompose3::SwingSideChange(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dAngle = 3.1415*(( m_dFrame <= 0.5 )? m_dFrame : (1-m_dFrame) );
	int nLevelX, nLevelY;
	int nSizeXY;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = ( m_dFrame <= 0.5 ) ? 1 : 2;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nSideTopToBottom)
			{
				nLevelX = (int)((nSizeX - 1)*(1-0.6*sin(dAngle)));
				nLevelY = (int)((nSizeY - 1)*cos(dAngle));
				nSizeXY = (nSizeX-1) - (int)((nSizeY - 1 - nPatternY)*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

				if ( nSizeY - 1 - nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
					dSourceY = nTileY * nSizeY + nSizeY - 1 - (double)(nSizeY - 1 - nPatternY) * (nSizeY - 1)/max(1,nLevelY);
				}
				else
					nPatternValue = 0;
			}
			else if (Type == c_nSideBottomToTop)
			{
				nLevelX = (int)((nSizeX - 1)*(1-0.6*sin(dAngle)));
				nLevelY = (int)((nSizeY - 1)*cos(dAngle));
				nSizeXY = (nSizeX-1) - (int)(nPatternY*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

				if ( nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
					dSourceY = nTileY * nSizeY + (double)nPatternY * (nSizeY - 1)/max(1,nLevelY);
				}
				else
					nPatternValue = 0;
			}
			else if (Type == c_nSideLeftToRight)
			{
				nLevelY = (int)((nSizeY - 1)*(1-0.6*sin(dAngle)));
				nLevelX = (int)((nSizeX - 1)*cos(dAngle));
				nSizeXY = (nSizeY-1) - (int)(nPatternX*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

				if ( nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + (double)nPatternX * (nSizeX - 1)/max(1,nLevelX);
					dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
				}
				else
					nPatternValue = 0;
			}
			else if (Type == c_nSideRightToLeft)
			{
				nLevelY = (int)((nSizeY - 1)*(1-0.6*sin(dAngle)));
				nLevelX = (int)((nSizeX - 1)*cos(dAngle));
				nSizeXY = (nSizeY-1) - (int)((nSizeX - 1 - nPatternX)*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

				if ( nSizeX - 1 - nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + nSizeX - 1 - (double)(nSizeX - 1 - nPatternX) * (nSizeX - 1)/max(1,nLevelX);
					dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
				}
				else
					nPatternValue = 0;
			}

			// combine colors
			// if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height || nPatternValue == 0)
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = 0;
				m_result.Pixels[nIndex + 1] = 0;
				m_result.Pixels[nIndex + 0] = 0;
			}
			else
			{
				if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 2)
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else

	if (Type == c_nSideBottomToTop || Type == c_nSideTopToBottom)
		IPPSwingSideChangeHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSideTopToBottom));
	else if (Type == c_nSideRightToLeft || Type == c_nSideLeftToRight)
		IPPSwingSideChangeVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSideLeftToRight));

#endif

	// all ok
	return TRUE;
}

BOOL ImageCompose3::SwingSide(int Type)
{
#ifdef NOT_OPTIM	

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dAngle = 3.1415*0.5*((Type == c_nSwingTopOut || Type == c_nSwingBottomOut ||
								Type == c_nSwingLeftOut || Type == c_nSwingRightOut) ? m_dFrame : (1-m_dFrame));
	int nLevelX, nLevelY;
	int nSizeXY;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = (Type == c_nSwingTopOut || Type == c_nSwingBottomOut ||
							 Type == c_nSwingLeftOut || Type == c_nSwingRightOut) ? 1 : 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if ( Type == c_nSwingTopOut || Type == c_nSwingTopIn )
			{
				nLevelX = (int)((nSizeX - 1)*(1-0.6*sin(dAngle)));
				nLevelY = (int)((nSizeY - 1)*cos(dAngle));
				nSizeXY = (nSizeX-1) - (int)((nSizeY - 1 - nPatternY)*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

				if ( nSizeY - 1 - nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
					dSourceY = nTileY * nSizeY + nSizeY - 1 - (double)(nSizeY - 1 - nPatternY) * (nSizeY - 1)/max(1,nLevelY);
					nPatternValue = 1 - nPatternValue;
				}
			}
			else if (Type == c_nSwingBottomOut || Type == c_nSwingBottomIn)
			{
				nLevelX = (int)((nSizeX - 1)*(1-0.6*sin(dAngle)));
				nLevelY = (int)((nSizeY - 1)*cos(dAngle));
				nSizeXY = (nSizeX-1) - (int)(nPatternY*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

				if ( nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
					dSourceY = nTileY * nSizeY + (double)nPatternY * (nSizeY - 1)/max(1,nLevelY);
					nPatternValue = 1 - nPatternValue;
				}
			}
			else if (Type == c_nSwingLeftOut || Type == c_nSwingLeftIn)
			{
				nLevelY = (int)((nSizeY - 1)*(1-0.6*sin(dAngle)));
				nLevelX = (int)((nSizeX - 1)*cos(dAngle));
				nSizeXY = (nSizeY-1) - (int)(nPatternX*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

				if ( nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + (double)nPatternX * (nSizeX - 1)/max(1,nLevelX);
					dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
					nPatternValue = 1 - nPatternValue;
				}
			}
			else if (Type == c_nSwingRightOut || Type == c_nSwingRightIn)
			{
				nLevelY = (int)((nSizeY - 1)*(1-0.6*sin(dAngle)));
				nLevelX = (int)((nSizeX - 1)*cos(dAngle));
				nSizeXY = (nSizeY-1) - (int)((nSizeX - 1 - nPatternX)*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

				if ( nSizeX - 1 - nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
				{
					dSourceX = nTileX * nSizeX + nSizeX - 1 - (double)(nSizeX - 1 - nPatternX) * (nSizeX - 1)/max(1,nLevelX);
					dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
					nPatternValue = 1 - nPatternValue;
				}
			}

			// combine colors
			/*
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = 0;
				m_result.Pixels[nIndex + 1] = 0;
				m_result.Pixels[nIndex + 0] = 0;
			}
			else */if ( Type == c_nSwingTopOut || Type == c_nSwingBottomOut ||
					  Type == c_nSwingLeftOut || Type == c_nSwingRightOut )
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				}
			} else
			{
				if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
			}
		}
	}

#else
	
	if (Type == c_nSwingTopOut || Type == c_nSwingBottomOut || Type == c_nSwingTopIn || Type == c_nSwingBottomIn)
		IPPSwingSideHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSwingTopOut || Type == c_nSwingTopIn), (Type == c_nSwingTopIn || Type == c_nSwingBottomIn));
	else if (Type == c_nSwingLeftOut || Type == c_nSwingLeftIn || Type == c_nSwingRightOut || Type == c_nSwingRightIn)
		IPPSwingSideVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSwingLeftIn || Type == c_nSwingLeftOut), (Type == c_nSwingLeftIn || Type == c_nSwingRightIn));
#endif

	// all ok
	return TRUE;

}

BOOL ImageCompose3::SwingDoors(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dAngle = 3.1415*0.5*((Type == c_nDoorVerticalOut || Type == c_nDoorHorizontalOut) ? m_dFrame : (1-m_dFrame));
	int nLevelX, nLevelY;
	int nSizeXY;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = (Type == c_nDoorVerticalOut || Type == c_nDoorHorizontalOut) ? 1 : 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if ( Type == c_nDoorVerticalOut || Type == c_nDoorVerticalIn )
			{
				nLevelX = (int)((nSizeX - 1)*(1-0.6*sin(dAngle)));
				nLevelY = (int)((nSizeY - 1)*0.5*cos(dAngle));
				if ( nPatternY <= (nSizeY - 1)/2 )
				{
					nSizeXY = (nSizeX-1) - (int)(nPatternY*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

					if ( nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
						dSourceY = nTileY * nSizeY + (double)nPatternY * (nSizeY - 1)/2.0/max(1,nLevelY);
						nPatternValue = 1 - nPatternValue;
					}
				}
				else
				{
					nSizeXY = (nSizeX-1) - (int)((nSizeY - 1 - nPatternY)*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

					if ( nSizeY - 1 - nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
						dSourceY = nTileY * nSizeY + nSizeY - 1 - (double)(nSizeY - 1 - nPatternY) * (nSizeY - 1)/2.0/max(1,nLevelY);
						nPatternValue = 1 - nPatternValue;
					}
				}
			}
			else if (Type == c_nDoorHorizontalOut || Type == c_nDoorHorizontalIn)
			{
				nLevelY = (int)((nSizeY - 1)*(1-0.6*sin(dAngle)));
				nLevelX = (int)((nSizeX - 1)*0.5*cos(dAngle));
				
				if ( nPatternX <= (nSizeX-1)/2 )
				{
					nSizeXY = (nSizeY-1) - (int)(nPatternX*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

					if ( nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + (double)nPatternX * (nSizeX - 1)/2.0/max(1,nLevelX);
						dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
						nPatternValue = 1 - nPatternValue;
					}
				}
				else
				{
					nSizeXY = (nSizeY-1) - (int)((nSizeX - 1 - nPatternX)*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

					if ( nSizeX - 1 - nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + nSizeX - 1 - (double)(nSizeX - 1 - nPatternX) * (nSizeX - 1)/2.0/max(1,nLevelX);
						dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
						nPatternValue = 1 - nPatternValue;
					}
				}
			}
			// combine colors
			/*if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = 0;
				m_result.Pixels[nIndex + 1] = 0;
				m_result.Pixels[nIndex + 0] = 0;
			}
			else */if ( Type == c_nDoorVerticalOut || Type == c_nDoorHorizontalOut )
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				}
			} else
			{
				if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
			}
		}
	}

#else

	if (Type == c_nDoorVerticalOut || Type == c_nDoorVerticalIn)
		IPPSwingDoorsVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nDoorVerticalIn));
	else if (Type == c_nDoorHorizontalOut || Type == c_nDoorHorizontalIn )
		IPPSwingDoorsHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nDoorHorizontalIn));

#endif

	// all ok
	return TRUE;

}

BOOL ImageCompose3::SwingDoorsChange(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dAngle = 3.1415*(( m_dFrame <= 0.5 )? m_dFrame : (1-m_dFrame) );
	int nLevelX, nLevelY;
	int nSizeXY;
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = ( m_dFrame <= 0.5 ) ? 1 : 2;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nDoorVerticalOut)
			{
				nLevelX = (int)((nSizeX - 1)*(1-0.6*sin(dAngle)));
				nLevelY = (int)((nSizeY - 1)*0.5*cos(dAngle));
				
				if ( nPatternY <= (nSizeY-1)/2 )
				{
					nSizeXY = (nSizeX-1) - (int)(nPatternY*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

					if ( nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
						dSourceY = nTileY * nSizeY + (double)nPatternY * (nSizeY - 1)/2.0/max(1,nLevelY);
					}
					else
						nPatternValue = 0;
				}
				else
				{
					nSizeXY = (nSizeX-1) - (int)((nSizeY - 1 - nPatternY)*(nSizeX - 1 - nLevelX)/(double)max(1,nLevelY));

					if ( nSizeY - 1 - nPatternY <= nLevelY && nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + (double)(nSizeX - 1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
						dSourceY = nTileY * nSizeY + nSizeY - 1 - (double)(nSizeY - 1 - nPatternY) * (nSizeY - 1)/2.0/max(1,nLevelY);
					}
					else
						nPatternValue = 0;
				}
			}
			else if (Type == c_nDoorHorizontalOut)
			{
				nLevelY = (int)((nSizeY - 1)*(1-0.6*sin(dAngle)));
				nLevelX = (int)((nSizeX - 1)*0.5*cos(dAngle));
				
				if ( nPatternX <= (nSizeX-1)/2 )
				{
					nSizeXY = (nSizeY-1) - (int)(nPatternX*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

					if ( nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + (double)nPatternX * (nSizeX - 1)/2.0/max(1,nLevelX);
						dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
					}
					else
						nPatternValue = 0;
				}
				else
				{
					nSizeXY = (nSizeY-1) - (int)((nSizeX - 1 - nPatternX)*(nSizeY - 1 - nLevelY)/(double)max(1,nLevelX));

					if ( nSizeX - 1 - nPatternX <= nLevelX && nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
					{
						dSourceX = nTileX * nSizeX + nSizeX - 1 - (double)(nSizeX - 1 - nPatternX) * (nSizeX - 1)/2.0/max(1,nLevelX);
						dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
					}
					else
						nPatternValue = 0;
				}
			}
			// combine colors
			// if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height || nPatternValue == 0)
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = 0;
				m_result.Pixels[nIndex + 1] = 0;
				m_result.Pixels[nIndex + 0] = 0;
			}
			else
			{
				if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 2)
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else

	if (Type == c_nDoorVerticalOut)
		IPPSwingDoorsChangeVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);
	else if (Type == c_nDoorHorizontalOut)
		IPPSwingDoorsChangeHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame);

#endif

	// all ok
	return TRUE;
}
	
// page
BOOL ImageCompose3::Page(int Type)
{
	
	BYTE* pBGRASource1 = m_first.Pixels;
	BYTE* pBGRASource2 = m_last.Pixels;
	BYTE* pBGRAResult = m_result.Pixels;

	int nWidth = m_first.Width;
	int nHeight = m_first.Height;

	if( Type < c_nPageTurnLeftTopWhiteBack3d )
		return IPPPageTurnWhiteBack( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, m_nTileSizeX, m_nTileSizeY, Type, m_dFrame );
	else if ( Type >= c_nPageTurnLeftTopWhiteBack3d && Type <= c_nPageTurnRightTopWhiteBack3d)
		return IPPPageTurnWhiteBack( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, m_nTileSizeX, m_nTileSizeY, Type, m_dFrame );
	else if ( Type >= c_nPageTurnAlbumLeftTop && Type <= c_nPageTurnAlbumLeftBottomCorners4)
		return IPPPageTurnAlbum( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, m_nTileSizeX, m_nTileSizeY, Type, m_dFrame );
	return FALSE;
}
	
// flip
BOOL ImageCompose3::FlipOver(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	int nLevelX, nLevelY, nBottomX, nBottomY;
	int nFarSize, nNearSize;
	int nSizeXY;

	if ( Type == c_nFlipVerticalInBackward || Type == c_nFlipHorizontalInBackward )
		m_dFrame = 1 - m_dFrame;
	
	double dAngle = 3.1415*(( m_dFrame <= 0.5 )? m_dFrame : (1-m_dFrame) );
	double dCoeff = 1.0/(2-0.4*sin(dAngle));
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			// compute color value
			if (Type == c_nFlipVerticalIn || Type == c_nFlipVerticalInBackward )
			{
				nNearSize = (int)(2*(nSizeX - 1)*dCoeff);
				nFarSize = 2*(nSizeX - 1) - nNearSize;
				nLevelY = (int)((nSizeY - 1)*cos(dAngle));
				nBottomY = ( nSizeY - 1 - nLevelY )/2;
				if ( nPatternY >= nBottomY && nPatternY <= nSizeY - 1 - nBottomY )
				{
					if ( m_dFrame <= 0.5 )
					{
						nSizeXY = nNearSize - (int)((nSizeY - 1 - nPatternY - nBottomY)*(nNearSize - nFarSize)/(double)max(1,nLevelY));

						if ( nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
						{
							dSourceX = nTileX * nSizeX + (double)(nSizeX-1)/max(1,nSizeXY) * (nPatternX - (nSizeX-1 - nSizeXY)/2.0);
							dSourceY = nTileY * nSizeY + nSizeY - 1 - (double)(nSizeY - 1 - nPatternY - nBottomY) * (nSizeY - 1)/max(1,nLevelY);
							nPatternValue = (Type == c_nFlipVerticalIn) ? 1 : 2;
						}
					} else
					{
						nSizeXY = nNearSize - (int)((nPatternY - nBottomY)*(nNearSize - nFarSize)/(double)max(1,nLevelY));

						if ( nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
						{
							dSourceX = nTileX * nSizeX + (double)(nSizeX-1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
							dSourceY = nTileY * nSizeY + (double)(nPatternY - nBottomY) * (nSizeY - 1)/max(1,nLevelY);
							nPatternValue = (Type == c_nFlipVerticalIn) ? 2 : 1;
						}
					}
				}
			}
			else if (Type == c_nFlipHorizontalIn || Type == c_nFlipHorizontalInBackward)
			{
				nNearSize = (int)(2*(nSizeY - 1)*dCoeff);
				nFarSize = 2*(nSizeY - 1) - nNearSize;
				nLevelX = (int)((nSizeX - 1)*cos(dAngle));
				nBottomX = ( nSizeX - 1 - nLevelX )/2;

				if ( nPatternX >= nBottomX && nPatternX <= nSizeX - 1 - nBottomX )
				{
					if (m_dFrame <= 0.5)
					{
						nSizeXY = nNearSize - (int)((nPatternX - nBottomX)*(nNearSize - nFarSize)/(double)max(1,nLevelX));

						if ( nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
						{
							dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
							dSourceX = nTileX * nSizeX + (double)(nPatternX - nBottomX) * (nSizeX - 1)/max(1,nLevelX);
							nPatternValue = (Type == c_nFlipHorizontalIn) ? 1 : 2;
						}
					}
					else
					{
						nSizeXY = nNearSize - (int)((nSizeX - 1 - nPatternX - nBottomX)*(nNearSize - nFarSize)/(double)max(1,nLevelX));

						if ( nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
						{
							dSourceX = nTileX * nSizeX + nSizeX - 1 - (double)(nSizeX - 1 - nPatternX - nBottomX) * (nSizeX - 1)/max(1,nLevelX);
							dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
							nPatternValue = (Type == c_nFlipHorizontalIn) ? 2 : 1;
						}
					}
				}
			}

			// combine colors
			// if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height || nPatternValue == 0)
			if (nPatternValue == 0)
			{
				m_result.Pixels[nIndex + 2] = 0;
				m_result.Pixels[nIndex + 1] = 0;
				m_result.Pixels[nIndex + 0] = 0;
			}
			else
			{
				if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 2)
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
			}
		}
	}

#else

	if (Type == c_nFlipVerticalInBackward || Type == c_nFlipVerticalIn)
		IPPFlipOverVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipVerticalIn));
	else if (Type == c_nFlipHorizontalInBackward || Type == c_nFlipHorizontalIn )
		IPPFlipOverHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipHorizontalIn));

#endif

	// all ok
	return TRUE;
}

BOOL ImageCompose3::FlipAway(int Type)
{
#ifdef NOT_OPTIM

	// compute tile size
	int nSizeX = m_nTileSizeX;
	int nSizeY = m_nTileSizeY;

	// variables
	int nX, nY, nIndex;
	int nTileX, nTileY;
	int nPatternX, nPatternY, nPatternValue;
	double dSourceX, dSourceY;
	double dAngle = 3.1415/0.4;
	int nParts = 0;
	int nLevelX, nLevelY, nBottomX, nBottomY;
	int nStartSizeX, nStartSizeY;
	int nSizeXY;
	
	if (Type == c_nFlipVerticalIn || Type == c_nFlipVerticalInBackward ||
		Type == c_nFlipHorizontalIn || Type == c_nFlipHorizontalInBackward )
		m_dFrame = 1 - m_dFrame;
	
	nParts = (int)(m_dFrame/0.2);
	
	dAngle *= (( nParts%2 == 0 )? (m_dFrame - nParts*0.2) : ((nParts+1)*0.2 - m_dFrame));
	
	// compute starting index
	nIndex = 0;

	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		nPatternY = nY % nSizeY;
		nTileY = nY / nSizeY;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			nPatternX = nX % nSizeX;
			nTileX = nX / nSizeX;

			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
			nPatternValue = (Type == c_nFlipVerticalOut || Type == c_nFlipHorizontalOut || 
							 Type == c_nFlipVerticalOutBackward || Type == c_nFlipHorizontalOutBackward) ? 1 : 0;

			// compute default source pixel coordinates
			dSourceX = nX;
			dSourceY = nY;

			//if ( Type == c_nFlipVerticalOut || Type == c_nFlipHorizontalOut )
           // {
			if ( m_dFrame <= 0.2 )
			{
				nStartSizeX = (int)((nSizeX-1)*(1-m_dFrame/2));
				nStartSizeY = (int)((nSizeY-1)*(1-m_dFrame/2));
			}
			else if ( m_dFrame > 0.2 && m_dFrame <= 0.4 )
			{
				nStartSizeX = (int)((nSizeX-1)*(0.9-(m_dFrame-0.2)*0.35/0.2));
				nStartSizeY = (int)((nSizeY-1)*(0.9-(m_dFrame-0.2)*0.35/0.2));
			}
			else if ( m_dFrame > 0.4 && m_dFrame <= 0.6 )
			{
				nStartSizeX = (int)((nSizeX-1)*(0.55-(m_dFrame-0.4)/2));
				nStartSizeY = (int)((nSizeY-1)*(0.55-(m_dFrame-0.4)/2));
			}
			else if ( m_dFrame > 0.6 && m_dFrame <= 0.8 )
			{
				nStartSizeX = (int)((nSizeX-1)*(0.45-(m_dFrame-0.6)*0.35/0.2));
				nStartSizeY = (int)((nSizeY-1)*(0.45-(m_dFrame-0.6)*0.35/0.2));
			}
			else 
			{
				nStartSizeX = (int)((nSizeX-1)*(0.1-(m_dFrame-0.8)/2));
				nStartSizeY = (int)((nSizeY-1)*(0.1-(m_dFrame-0.8)/2));
			}
 
			// compute color value
			if ( Type == c_nFlipVerticalOut || Type == c_nFlipVerticalIn ||
				 Type == c_nFlipVerticalOutBackward || Type == c_nFlipVerticalInBackward )
			{
				nLevelX = (int)((nStartSizeX - 1)*(1-0.4*sin(dAngle)));
				nLevelY = (int)((nStartSizeY - 1)*cos(dAngle));
				nBottomY = ( nSizeY - 1 - nLevelY )/2;
				if ( nPatternY >= nBottomY && nPatternY <= nSizeY - 1 - nBottomY )
				{
					if ((Type == c_nFlipVerticalOut || Type == c_nFlipVerticalIn) &&
						( m_dFrame > 0.2 && m_dFrame <= 0.4 || m_dFrame > 0.6 && m_dFrame <= 0.8 ) ||
						( Type == c_nFlipVerticalOutBackward || Type == c_nFlipVerticalInBackward ) &&
						( m_dFrame <= 0.2 || m_dFrame > 0.4 && m_dFrame <= 0.6 || m_dFrame > 0.8 ))
					{
						nSizeXY = nStartSizeX - (int)((nSizeY - 1 - nPatternY - nBottomY)*(nStartSizeX - nLevelX)/(double)max(1,nLevelY));
						if ( nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
						{
							dSourceX = nTileX * nSizeX + (double)(nSizeX-1)/max(1,nSizeXY) * (nPatternX - (nSizeX-1 - nSizeXY)/2.0);
							dSourceY = nTileY * nSizeY + nSizeY - 1 - (double)(nSizeY - 1 - nPatternY - nBottomY) * (nSizeY - 1)/max(1,nLevelY);
							nPatternValue = 1 - nPatternValue;
						}
					}
					else if ((Type == c_nFlipVerticalOut || Type == c_nFlipVerticalIn) &&
							(m_dFrame <= 0.2 || m_dFrame > 0.4 && m_dFrame <= 0.6 || m_dFrame > 0.8) ||
							( Type == c_nFlipVerticalOutBackward || Type == c_nFlipVerticalInBackward ) &&
							( m_dFrame > 0.2 && m_dFrame <= 0.4 || m_dFrame > 0.6 && m_dFrame <= 0.8 ))
					{
						nSizeXY = nStartSizeX - (int)((nPatternY - nBottomY)*(nStartSizeX - nLevelX)/(double)max(1,nLevelY));
						if ( nPatternX <= (nSizeX - 1 + nSizeXY)/2 && nPatternX >= (nSizeX - 1 - nSizeXY)/2 )
						{
							dSourceX = nTileX * nSizeX + (double)(nSizeX-1)/max(1,nSizeXY) * (nPatternX - (nSizeX - 1 - nSizeXY)/2.0);
							dSourceY = nTileY * nSizeY + (double)(nPatternY - nBottomY) * (nSizeY - 1)/max(1,nLevelY);
							nPatternValue = 1 - nPatternValue;
						}
					}
				}
			}
			else if (Type == c_nFlipHorizontalOut || Type == c_nFlipHorizontalIn || 
					 Type == c_nFlipHorizontalOutBackward || Type == c_nFlipHorizontalInBackward )
			{
                nLevelY = (int)((nStartSizeY - 1)*(1-0.4*sin(dAngle)));
                nLevelX = (int)((nStartSizeX - 1)*cos(dAngle));
				nBottomX = ( nSizeX - 1 - nLevelX )/2;
				if ( nPatternX >= nBottomX && nPatternX <= nSizeX - 1 - nBottomX )
				{
					if ( ( Type == c_nFlipHorizontalOut || Type == c_nFlipHorizontalIn ) &&
						(m_dFrame > 0.2 && m_dFrame <= 0.4 || m_dFrame > 0.6 && m_dFrame <= 0.8) ||
						(Type == c_nFlipHorizontalOutBackward || Type == c_nFlipHorizontalInBackward) &&
						( m_dFrame <= 0.2 || m_dFrame > 0.4 && m_dFrame <= 0.6 || m_dFrame > 0.8 ))
					{
						nSizeXY = nStartSizeY - (int)((nSizeX - 1 - nPatternX - nBottomX)*(nStartSizeY - nLevelY)/(double)max(1,nLevelX));

						if ( nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
						{
							dSourceX = nTileX * nSizeX + nSizeX - 1 - (double)(nSizeX - 1 - nPatternX - nBottomX) * (nSizeX - 1)/max(1,nLevelX);
							dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
							nPatternValue = 1 - nPatternValue;
						}
					}
					else if ( ( Type == c_nFlipHorizontalOut || Type == c_nFlipHorizontalIn ) && 
							(m_dFrame <= 0.2 || m_dFrame > 0.4 && m_dFrame <= 0.6 || m_dFrame > 0.8 ) ||
							( Type == c_nFlipHorizontalOutBackward || Type == c_nFlipHorizontalInBackward ) &&
							(m_dFrame > 0.2 && m_dFrame <= 0.4 || m_dFrame > 0.6 && m_dFrame <= 0.8))
		            {
						nSizeXY = nStartSizeY - (int)((nPatternX - nBottomX)*(nStartSizeY - nLevelY)/(double)max(1,nLevelX));

						if ( nPatternY <= (nSizeY - 1 + nSizeXY)/2 && nPatternY >= (nSizeY - 1 - nSizeXY)/2 )
						{
							dSourceY = nTileY * nSizeY + (double)(nSizeY - 1)/max(1,nSizeXY) * (nPatternY - (nSizeY - 1 - nSizeXY)/2.0);
							dSourceX = nTileX * nSizeX + (double)(nPatternX - nBottomX) * (nSizeX - 1)/max(1,nLevelX);
							nPatternValue = 1 - nPatternValue;
						}
					}
				}
			}
			
			if (dSourceX < 0 || dSourceX >= m_first.Width || dSourceY < 0 || dSourceY >= m_first.Height)
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
			else if (Type == c_nFlipVerticalOut || Type == c_nFlipHorizontalOut ||
					 Type == c_nFlipVerticalOutBackward || Type == c_nFlipHorizontalOutBackward )
			{
				if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_first.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_first.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				}
			} else 
			{
				if (nPatternValue == 1)
				{
					m_result.Pixels[nIndex + 2] = m_last.GetBilinear(dSourceX, dSourceY, 2, 0);
					m_result.Pixels[nIndex + 1] = m_last.GetBilinear(dSourceX, dSourceY, 1, 0);
					m_result.Pixels[nIndex + 0] = m_last.GetBilinear(dSourceX, dSourceY, 0, 0);
				}
				else if (nPatternValue == 0)
				{
					m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
				}
			}
		}
	}


#else

	if (Type == c_nFlipVerticalInBackward || Type == c_nFlipVerticalIn || Type == c_nFlipVerticalOutBackward || Type == c_nFlipVerticalOut)
		IPPFlipAwayVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipVerticalIn || Type == c_nFlipVerticalInBackward), (Type == c_nFlipVerticalInBackward/*c_nFlipVerticalIn*/ || Type == c_nFlipVerticalOutBackward/*c_nFlipVerticalOut*/));
	else if (Type == c_nFlipHorizontalInBackward || Type == c_nFlipHorizontalIn || Type == c_nFlipHorizontalOutBackward || Type == c_nFlipHorizontalOut)
		IPPFlipAwayHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipHorizontalIn || Type == c_nFlipHorizontalInBackward), (Type == c_nFlipHorizontalInBackward/*c_nFlipHorizontalIn*/ || Type == c_nFlipHorizontalOutBackward/*c_nFlipHorizontalOut*/));

#endif

	// all ok
	return TRUE;
}
BOOL ImageCompose3::FlipOutIn(int Type)
{
#ifndef NOT_OPTIM
if ( Type == c_nFlipVerticalInBackward || Type == c_nFlipVerticalIn )
    IPPFlipAwayOutInVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipVerticalIn ));
else if ( Type == c_nFlipHorizontalInBackward || Type == c_nFlipHorizontalIn )
    IPPFlipAwayOutInHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipHorizontalIn) );
#endif
	// all ok
	return TRUE;
}
BOOL ImageCompose3::FlipBoxes(int Type)
{
#ifndef NOT_OPTIM
if ( Type == c_nSideTopToBottom || Type == c_nSideBottomToTop )
    IPPFlipBoxesVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSideTopToBottom ));
else if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft )
    IPPFlipBoxesHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nSideLeftToRight) );
#endif
	// all ok
	return TRUE;
}

BOOL ImageCompose3::FlipOutBoxes(int Type)
{
#ifndef NOT_OPTIM
if ( Type == c_nFlipVerticalInBackward || Type == c_nFlipVerticalIn )
    //IPPFlipOutBoxesVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipVerticalIn) );
	IPPFlipOutBoxesHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipVerticalIn) );
else if ( Type == c_nFlipHorizontalInBackward || Type == c_nFlipHorizontalIn )
    IPPFlipOutBoxesVertical(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipHorizontalIn) );
	//IPPFlipOutBoxesHorizontal(m_first, m_last, m_result, m_nTileSizeX, m_nTileSizeY, m_dFrame, (Type == c_nFlipHorizontalIn) );
#endif
	// all ok
	return TRUE;
}
	
// shatter
BOOL ImageCompose3::Shatter(int Type)
{
    int nSizeX = m_nTileSizeX;
    int nSizeY = m_nTileSizeY;
    
    // variables
    int nwRect = 16; // quantity of squares in width
    int nhRect = 12; // quantity of squares in height
	int nW; // width of square
    int nH; // height of square
    if ( nSizeX < nwRect )
	{
		nwRect = nSizeX; nW = 1;
	} else
	{
		nW = (int)(nSizeX/nwRect);
		nwRect = (int)((nSizeX-1)/nW) + 1;
	}
    if ( nSizeY < nhRect )
	{
		nhRect = nSizeY; nH = 1;
	} else
	{
		nH = (int)(nSizeY/nhRect);
		nhRect = (int)((nSizeY-1)/nH) + 1;
	}

	int * nRandomCol = new int[nwRect];
	int * nRandomRow = new int[nhRect];
    int nCol, nRow, nColIndex, nRowIndex, i, j; 

    double  dSpeed;
    int nSourceX, nSourceY, nSourceIndex;
    int nDestX, nDestY, nDestIndex;
	int nSourceTileX, nSourceTileY;
    int nDestTileX, nDestTileY;
    int nFinishX, nFinishY, nX_O, nY_O, nX_O1, nY_O1;

	int nTileX, nTileY;
	int nTilesX = (m_first.Width - 1) / nSizeX + 1;
	int	nTilesY = (m_first.Height - 1) / nSizeY + 1;
    int nImageStep = m_first.Width * 4;
	int nDstTmp, nSrcTmp;
    
	for ( i=0; i<nwRect; i++ )
        nRandomCol[i] = i;
    for ( i=0; i<nhRect; i++ )
        nRandomRow[i] = i;

	std::random_shuffle( &nRandomCol[0], &nRandomCol[nwRect] );
    std::random_shuffle( &nRandomRow[0], &nRandomRow[nhRect] );

    srand(1);

    m_result.CopyFrom(m_last);
	if ( Type == c_nSideLeftBottomToRightTop )
	{
		nFinishX = nSizeX + 50;
		nFinishY = nSizeY + 50;
	} 
	else if ( Type == c_nSideLeftTopToRightBottom )
	{
		nFinishX = nSizeX + 50;
		nFinishY = -nH - 50;
	}
	else if ( Type == c_nSideRightBottomToLeftTop )
	{
		nFinishX = -nW - 50;
		nFinishY = nSizeY + 50;
	}
	else if ( Type == c_nSideRightTopToLeftBottom )
	{
		nFinishX = -nW - 50;
		nFinishY = -nH - 50;
	}
	for (nRowIndex = 0; nRowIndex < nhRect; nRowIndex++)
	{
        nRow = nRandomRow[nRowIndex];
        nY_O = nRow * nH;

        for (nColIndex = 0; nColIndex < nwRect; nColIndex++)
        {
			nCol = nRandomCol[nColIndex];
            nX_O = nCol * nW;
            dSpeed = m_dFrame * ( ((double)rand()/RAND_MAX)/2.0 + 0.75 );
            nY_O1 = (int)(nY_O * (1 - dSpeed) + nFinishY * dSpeed);
            nX_O1 = (int)(nX_O * (1 - dSpeed) + nFinishX * dSpeed);
            
            for ( i=0; i<nH; i++)
			{
				nSourceTileY = i + nY_O;
				nDestTileY = i + nY_O1;
                if ( nSourceTileY >= nSizeY || nDestTileY >= nSizeY || nSourceTileY < 0 || nDestTileY < 0 )
					continue;
				for ( j=0; j<nW; j++)
                {
                    nDestTileX = j + nX_O1;
                    nSourceTileX = j + nX_O;
                                        
                    if ( nSourceTileX >= nSizeX || nDestTileX >= nSizeX || nSourceTileX < 0 || nDestTileX < 0 )
						continue;
					for ( nTileY=0; nTileY<nTilesY; nTileY++ )
					{
						nSourceY = nTileY * nSizeY + nSourceTileY;
						nDestY = nTileY * nSizeY + nDestTileY;

						if ( nDestY >= m_first.Height || nDestY < 0 || nSourceY >= m_first.Height || nSourceY < 0 )
							continue;
						nDstTmp = nImageStep * nDestY;
						nSrcTmp = nImageStep * nSourceY;
						for ( nTileX=0; nTileX<nTilesX; nTileX++ )
						{
							nSourceX = nTileX * nSizeX + nSourceTileX;
							nDestX = nTileX * nSizeX + nDestTileX;
							if ( nDestX >= m_first.Width || nDestX < 0 || nSourceX >= m_first.Width || nSourceX < 0 )
			                    continue;
							nDestIndex = nDstTmp + 4 * nDestX; 
							nSourceIndex = nSrcTmp + 4 * nSourceX; 

							m_result.Pixels[nDestIndex + 2] = (BYTE)(m_result.Pixels[nDestIndex + 2] * m_dFrame + m_first.Pixels[nSourceIndex + 2] * (1 - m_dFrame));
							m_result.Pixels[nDestIndex + 1] = (BYTE)(m_result.Pixels[nDestIndex + 1] * m_dFrame + m_first.Pixels[nSourceIndex + 1] * (1 - m_dFrame));
							m_result.Pixels[nDestIndex + 0] = (BYTE)(m_result.Pixels[nDestIndex + 0] * m_dFrame + m_first.Pixels[nSourceIndex + 0] * (1 - m_dFrame));
					
						}
					}
				}
			}
        }
	}
	delete [] nRandomCol;
	delete [] nRandomRow;
	// all ok
    return TRUE;
}
BOOL ImageCompose3::ShatterRotate(int Type)
{
    int nSizeX = m_nTileSizeX;
    int nSizeY = m_nTileSizeY;

#ifdef NOT_OPTIM
    
    // variables
    int nwRect = 16; // quantity of squares in width
    int nhRect = 12; // quantity of squares in height
	int nW; // width of square
    int nH; // height of square
    
	if ( nSizeX < nwRect )
	{
		nwRect = nSizeX; nW = 1;
	} else
	{
		nW = (int)(nSizeX/nwRect);
		nwRect = (int)((nSizeX-1)/nW) + 1;
	}
    if ( nSizeY < nhRect )
	{
		nhRect = nSizeY; nH = 1;
	} else
	{
		nH = (int)(nSizeY/nhRect);
		nhRect = (int)((nSizeY-1)/nH) + 1;
	}

    int nR = (int)(sqrt(nW*nW + nH*nH) + 1);
    int * nRandomCol = new int[nwRect];
	int * nRandomRow = new int[nhRect];
    int nCol, nRow, nColIndex, nRowIndex, i, j; 

    double dSourceX, dSourceY, dSourceTileX, dSourceTileY;
    int nDestTileX, nDestTileY, nDestX, nDestY, nDestIndex;
    int nFinishX, nFinishY;
	double dX_O, dY_O, dX_O1, dY_O1;
	double dAngle, dSpeed, dCos, dSin;
	int nTileX, nTileY;
	int nTilesX = (m_first.Width - 1) / nSizeX + 1;
	int	nTilesY = (m_first.Height - 1) / nSizeY + 1;
    double dTileC = (nR-1)/2.0;
    double Cx, Cy, dCheckX, dCheckY;     

    for ( i=0; i<nwRect; i++ )
        nRandomCol[i] = i;
    for ( i=0; i<nhRect; i++ )
        nRandomRow[i] = i;

    srand(1);

	std::random_shuffle( &nRandomCol[0], &nRandomCol[nwRect] );
    std::random_shuffle( &nRandomRow[0], &nRandomRow[nhRect] );

	m_result.CopyFrom(m_last);
	if ( Type == c_nSideLeftToRight )
		nFinishX = nSizeX + 20;
	else if ( Type == c_nSideRightToLeft )
		nFinishX = -nW - 20;
	else if ( Type == c_nSideTopToBottom )
		nFinishY = -nH - 20;
	else if ( Type == c_nSideBottomToTop )
		nFinishY = nSizeY + 20;

    Cx = (nW-1)/2.0;
    Cy = (nH-1)/2.0;

    for (nRowIndex = 0; nRowIndex < nhRect; nRowIndex++)
	{
        nRow = nRandomRow[nRowIndex];
        dY_O = nRow * nH + Cy;

		for (nColIndex = 0; nColIndex < nwRect; nColIndex++)
        {
			nCol = nRandomCol[nColIndex];
			dX_O = nCol * nW + Cx;

            dSpeed = m_dFrame * ( ((double)rand()/RAND_MAX)* 0.5 + 1 );
			dAngle = m_dFrame * (double)rand()/RAND_MAX * 3.1415 * 4;
			
            if ( Type == c_nSideRightToLeft || Type == c_nSideTopToBottom )
				dAngle = -dAngle;			

            dCos = cos(dAngle);
            dSin = sin(dAngle);

			if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft)
			{	
				dY_O1 = dY_O;
				dX_O1 = dX_O * (1 - dSpeed) + nFinishX * dSpeed;
			}
			else if ( Type == c_nSideTopToBottom || Type == c_nSideBottomToTop)
			{
				dX_O1 = dX_O;
				dY_O1 = dY_O * (1 - dSpeed) + nFinishY * dSpeed;
			}

			for ( i=0; i<nR; i++) // y coordinate
			{
				dCheckY = i - dTileC;
                for ( j=0; j<nR; j++) // x coordinate
                {
					dCheckX = j - dTileC;

                    dSourceTileX = dCheckX * dCos - dCheckY * dSin; 
                    dSourceTileY = dCheckX * dSin + dCheckY * dCos;
                    if ( fabs(dSourceTileX) <= nW/2.0 && fabs(dSourceTileY) <= nH/2.0)
                    {
                        dSourceTileX += dX_O;
                        dSourceTileY += dY_O;
                        nDestTileY = (int)(dY_O1 + dCheckY);
                        nDestTileX = (int)(dX_O1 + dCheckX);

		                if ( dSourceTileY >= nSizeY || nDestTileY >= nSizeY || dSourceTileY < 0 || nDestTileY < 0 ||
							 dSourceTileX >= nSizeX || nDestTileX >= nSizeX || dSourceTileX < 0 || nDestTileX < 0 )
							continue;
						for ( nTileY=0; nTileY<nTilesY; nTileY++ )
						{
							dSourceY = nTileY * nSizeY + dSourceTileY;
							nDestY = nTileY * nSizeY + nDestTileY;

							if ( nDestY >= m_first.Height || nDestY < 0 || dSourceY >= m_first.Height || dSourceY < 0 )
								continue;
							for ( nTileX=0; nTileX<nTilesX; nTileX++ )
							{
								dSourceX = nTileX * nSizeX + dSourceTileX;
								nDestX = nTileX * nSizeX + nDestTileX;
								if ( nDestX >= m_first.Width || nDestX < 0 || dSourceX >= m_first.Width || dSourceX < 0 )
									continue;
								nDestIndex = 4 * (nDestX + nDestY * m_first.Width); 
								m_result.Pixels[nDestIndex + 2] = (BYTE)(m_result.Pixels[nDestIndex + 2] * m_dFrame + m_first.GetBilinear(dSourceX, dSourceY, 2, 0) * (1 - m_dFrame));
								m_result.Pixels[nDestIndex + 1] = (BYTE)(m_result.Pixels[nDestIndex + 1] * m_dFrame + m_first.GetBilinear(dSourceX, dSourceY, 1, 0) * (1 - m_dFrame));
								m_result.Pixels[nDestIndex + 0] = (BYTE)(m_result.Pixels[nDestIndex + 0] * m_dFrame + m_first.GetBilinear(dSourceX, dSourceY, 0, 0) * (1 - m_dFrame));
							}
						}
					}
				}
			}
        }
	}
	delete [] nRandomCol;
	delete [] nRandomRow;
#else
    
    // variables
    int nwRect = 16; // quantity of squares in width
    int nhRect = 12; // quantity of squares in height

	if ( Type == c_nWhirlWind )
	{
		nwRect /= 2;
		nhRect /= 2;
	}
	int nW; // width of square
    int nH; // height of square
    
	if ( nSizeX < nwRect )
	{
		nwRect = nSizeX; nW = 1;
	} else
	{
		nW = (int)(nSizeX/nwRect);
		nwRect = (int)((nSizeX-1)/nW) + 1;
	}
    if ( nSizeY < nhRect )
	{
		nhRect = nSizeY; nH = 1;
	} else
	{
		nH = (int)(nSizeY/nhRect);
		nhRect = (int)((nSizeY-1)/nH) + 1;
	}

    int nR = (int)(sqrt((double)nW*nW + nH*nH) + 1);
    int * nRandomCol = new int[nwRect];
	int * nRandomRow = new int[nhRect];
    int nCol, nRow, nColIndex, nRowIndex, i, j; 

    double dSourceX, dSourceY, dSourceTileX, dSourceTileY;
    int nDestTileX, nDestTileY, nDestX, nDestY, nDestIndex, nSrcIndex;
    int nFinishX, nFinishY;
	double dX_O, dY_O, dX_O1, dY_O1;
	double dAngle, dSpeed, dCos, dSin;
	int nTileX, nTileY;
	int nTilesX = (m_first.Width - 1) / nSizeX + 1;
	int	nTilesY = (m_first.Height - 1) / nSizeY + 1;
    double dTileC = (nR-1)/2.0;
    double Cx, Cy, dCheckX, dCheckY;     
	int nImageStep = 4 * m_first.Width; 
    double nHalfnW = nW/2.0;
	double nHalfnH = nH/2.0;
	int nDestTmp, nSrcTmp;

	for ( i=0; i<nwRect; i++ )
        nRandomCol[i] = i;
    for ( i=0; i<nhRect; i++ )
        nRandomRow[i] = i;

    srand(1);

	std::random_shuffle( &nRandomCol[0], &nRandomCol[nwRect] );
    std::random_shuffle( &nRandomRow[0], &nRandomRow[nhRect] );

	m_result.CopyFrom(m_last);
	if ( Type == c_nSideLeftToRight )
		nFinishX = nSizeX + 20;
	else if ( Type == c_nSideRightToLeft )
		nFinishX = -nW - 20;
	else if ( Type == c_nSideTopToBottom )
		nFinishY = -nH - 20;
	else if ( Type == c_nSideBottomToTop )
		nFinishY = nSizeY + 20;

    Cx = (nW-1)/2.0;
    Cy = (nH-1)/2.0;

    for (nRowIndex = 0; nRowIndex < nhRect; nRowIndex++)
	{
        nRow = nRandomRow[nRowIndex];
        dY_O = nRow * nH + Cy;

		for (nColIndex = 0; nColIndex < nwRect; nColIndex++)
        {
			nCol = nRandomCol[nColIndex];
			dX_O = nCol * nW + Cx;

            dSpeed = m_dFrame * ( ((double)rand()/RAND_MAX)* 0.5 + 1 );
			dAngle = m_dFrame * (double)rand()/RAND_MAX * 3.1415 * 4;
			
            if ( Type == c_nSideRightToLeft || Type == c_nSideTopToBottom )
				dAngle = -dAngle;	

			if ( Type == c_nWhirlWind)
			{
				dAngle *= 2;
			}


            dCos = cos(dAngle);
            dSin = sin(dAngle);

			if ( Type == c_nSideLeftToRight || Type == c_nSideRightToLeft)
			{	
				dY_O1 = dY_O;
				dX_O1 = dX_O * (1 - dSpeed) + nFinishX * dSpeed;
			}
			else if ( Type == c_nSideTopToBottom || Type == c_nSideBottomToTop)
			{
				dX_O1 = dX_O;
				dY_O1 = dY_O * (1 - dSpeed) + nFinishY * dSpeed;
			}
			else if ( Type == c_nWhirlWind)
			{
				dX_O1 = dX_O;
				dY_O1 = dY_O;
			}

			for ( i=0; i<nR; i++) // y coordinate
			{
				dCheckY = i - dTileC;
                for ( j=0; j<nR; j++) // x coordinate
                {
					dCheckX = j - dTileC;

                    dSourceTileX = dCheckX * dCos - dCheckY * dSin; 
                    dSourceTileY = dCheckX * dSin + dCheckY * dCos;
                    if ( fabs(dSourceTileX) <= nHalfnW && fabs(dSourceTileY) <= nHalfnH)
                    {
                        dSourceTileX += dX_O;
                        dSourceTileY += dY_O;
                        nDestTileY = (int)(dY_O1 + dCheckY);
                        nDestTileX = (int)(dX_O1 + dCheckX);

		                if ( dSourceTileY >= nSizeY || nDestTileY >= nSizeY || dSourceTileY < 0 || nDestTileY < 0 ||
							 dSourceTileX >= nSizeX || nDestTileX >= nSizeX || dSourceTileX < 0 || nDestTileX < 0 )
							continue;
						for ( nTileY=0; nTileY<nTilesY; nTileY++ )
						{
							dSourceY = nTileY * nSizeY + dSourceTileY;
							nDestY = nTileY * nSizeY + nDestTileY;

							if ( nDestY >= m_first.Height || nDestY < 0 || dSourceY >= m_first.Height || dSourceY < 0 )
								continue;

							nDestTmp = nImageStep * nDestY; 
                            nSrcTmp = (int)(dSourceY+0.5) * nImageStep; 
 							for ( nTileX=0; nTileX<nTilesX; nTileX++ )
							{
								dSourceX = nTileX * nSizeX + dSourceTileX;
								nDestX = nTileX * nSizeX + nDestTileX;
								if ( nDestX >= m_first.Width || nDestX < 0 || dSourceX >= m_first.Width || dSourceX < 0 )
									continue;
								
								nDestIndex = nDestTmp + 4 * nDestX; 
                                nSrcIndex = nSrcTmp + 4 * ((int)(dSourceX+0.5)); 

								m_result.Pixels[nDestIndex + 2] = (BYTE)(m_result.Pixels[nDestIndex + 2] * m_dFrame + m_first.Pixels[nSrcIndex + 2] * (1 - m_dFrame));
								m_result.Pixels[nDestIndex + 1] = (BYTE)(m_result.Pixels[nDestIndex + 1] * m_dFrame + m_first.Pixels[nSrcIndex + 1] * (1 - m_dFrame));
								m_result.Pixels[nDestIndex + 0] = (BYTE)(m_result.Pixels[nDestIndex + 0] * m_dFrame + m_first.Pixels[nSrcIndex + 0] * (1 - m_dFrame));
							}
						}
					}
				}
			}
        }
	}
	delete [] nRandomCol;
	delete [] nRandomRow;
				    

#endif

	// all ok
    return TRUE;
}
BOOL ImageCompose3::ShatterIn(int Type)
{
    int nSizeX = m_nTileSizeX;
    int nSizeY = m_nTileSizeY;

    // variables
    int nwRect = 16; // 32; // quantity of squares in width
    int nhRect = 12; // 24; // quantity of squares in height
    int nW; // width of square
    int nH; // height of square

	if (Type == c_nShatterHigh)
	{
		nwRect = 32;
		nhRect = 24;
	}

#ifdef NOT_OPTIM
       
	if ( nSizeX < nwRect )
	{
		nwRect = nSizeX; nW = 1;
	} else
	{
		nW = (int)(nSizeX/nwRect);
		nwRect = (int)((nSizeX-1)/nW) + 1;
	}
    if ( nSizeY < nhRect )
	{
		nhRect = nSizeY; nH = 1;
	} else
	{
		nH = (int)(nSizeY/nhRect);
		nhRect = (int)((nSizeY-1)/nH) + 1;
	}

    double dSizeRect = sqrt(nW*nW + nH*nH);
	int * nRandomCol = new int[nwRect];
	int * nRandomRow = new int[nhRect];
    int nCol, nRow, nColIndex, nRowIndex, i, j; 

    double dSourceX, dSourceY, dSourceTileX, dSourceTileY;
    int nDestTileX, nDestTileY, nDestX, nDestY, nDestIndex;
	double dX_O, dY_O, dX_O1, dY_O1;
	double dSpeed, dAngle, dDist, dCoeff, dTileC;
	int nTileX, nTileY;
	int nTilesX = (m_first.Width - 1) / nSizeX + 1;
	int	nTilesY = (m_first.Height - 1) / nSizeY + 1;
    int nR = 0;
    double dCos, dSin, Cx, Cy, dCheckY, dCheckX; 
    double dDisappear = m_dFrame * m_dFrame * m_dFrame;

	for ( i=0; i<nwRect; i++ )
        nRandomCol[i] = i;
    for ( i=0; i<nhRect; i++ )
        nRandomRow[i] = i;

    srand(1);

	std::random_shuffle( &nRandomCol[0], &nRandomCol[nwRect] );
    std::random_shuffle( &nRandomRow[0], &nRandomRow[nhRect] );

	m_result.CopyFrom(m_last);

    Cx = (nSizeX-1)/2.0;
    Cy = (nSizeY-1)/2.0;

    for (nRowIndex = 0; nRowIndex < nhRect; nRowIndex++)
	{
        nRow = nRandomRow[nRowIndex];
        dY_O = nRow * nH + (nH-1)/2.0;

        for (nColIndex = 0; nColIndex < nwRect; nColIndex++)
        {
			nCol = nRandomCol[nColIndex];
            dX_O = nCol * nW + (nW-1)/2.0;
            
            dSpeed = (double)rand()/RAND_MAX * 3;
            dDist = 1 - m_dFrame * dSpeed;
            if ( dDist < 0.1 )
                continue;
            dCoeff = 1/dDist;
            dAngle = (1 - dDist)* 3.1415 * 2;

            dX_O1 = (dX_O - Cx) * dCoeff + Cx;
            dY_O1 = (dY_O - Cy) * dCoeff + Cy;

            dCos = cos(dAngle)/dCoeff;
            dSin = sin(dAngle)/dCoeff;
			
            nR = (int)(dCoeff * dSizeRect + 1);
            dTileC = (nR-1)/2.0;
			
            for ( i=0; i<nR; i++) // y coordinate
			{
				dCheckY = i - dTileC;
                for ( j=0; j<nR; j++) // x coordinate
                {
                    dCheckX = j - dTileC;
					
                    dSourceTileX = dCheckX * dCos - dCheckY * dSin; 
                    dSourceTileY = dCheckX * dSin + dCheckY * dCos;
					if ( fabs(dSourceTileX) <= nW/2.0 && fabs(dSourceTileY) <= nH/2.0)
                    {
						dSourceTileX += dX_O;
						dSourceTileY += dY_O;
						nDestTileY = (int)(dY_O1 + dCheckY);
						nDestTileX = (int)(dX_O1 + dCheckX);
		                if ( dSourceTileY >= nSizeY || nDestTileY >= nSizeY || dSourceTileY < 0 || nDestTileY < 0 ||
							 dSourceTileX >= nSizeX || nDestTileX >= nSizeX || dSourceTileX < 0 || nDestTileX < 0 )
							continue;
						for ( nTileY=0; nTileY<nTilesY; nTileY++ )
						{
							dSourceY = nTileY * nSizeY + dSourceTileY;
							nDestY = nTileY * nSizeY + nDestTileY;

							if ( nDestY >= m_first.Height || nDestY < 0 || dSourceY >= m_first.Height || dSourceY < 0 )
								continue;
							for ( nTileX=0; nTileX<nTilesX; nTileX++ )
							{
								dSourceX = nTileX * nSizeX + dSourceTileX;
								nDestX = nTileX * nSizeX + nDestTileX;
								if ( nDestX >= m_first.Width || nDestX < 0 || dSourceX >= m_first.Width || dSourceX < 0 )
									continue;
								nDestIndex = 4 * (nDestX + nDestY * m_first.Width); 
								m_result.Pixels[nDestIndex + 2] = (BYTE)(m_result.Pixels[nDestIndex + 2] * dDisappear + m_first.GetBilinear(dSourceX, dSourceY, 2, 0) * (1 - dDisappear));
								m_result.Pixels[nDestIndex + 1] = (BYTE)(m_result.Pixels[nDestIndex + 1] * dDisappear + m_first.GetBilinear(dSourceX, dSourceY, 1, 0) * (1 - dDisappear));
								m_result.Pixels[nDestIndex + 0] = (BYTE)(m_result.Pixels[nDestIndex + 0] * dDisappear + m_first.GetBilinear(dSourceX, dSourceY, 0, 0) * (1 - dDisappear));
							}
						}
					}
				}
			}
        }
	}
	delete [] nRandomCol;
	delete [] nRandomRow;
#else

    if ( nSizeX < nwRect )
    {
        nwRect = nSizeX; nW = 1;
    } else
    {
        nW = (int)(nSizeX/nwRect);
        nwRect = (int)((nSizeX-1)/nW) + 1;
    }
    if ( nSizeY < nhRect )
    {
        nhRect = nSizeY; nH = 1;
    } else
    {
        nH = (int)(nSizeY/nhRect);
        nhRect = (int)((nSizeY-1)/nH) + 1;
    }

    int    nImageStep = 4 * m_first.Width;
    double dSizeRect = sqrt((double)nW*nW + nH*nH);
    int * nRandomCol = new int[nwRect];
    int * nRandomRow = new int[nhRect];
    int nCol, nRow, nColIndex, nRowIndex, i, j; 

    double dSourceX, dSourceY, dSourceTileX, dSourceTileY;
    int nDestTileX, nDestTileY, nDestX, nDestY, nDestIndex,nSrcIndex;
    double dX_O, dY_O, dX_O1, dY_O1;
    double dSpeed, dAngle, dDist, dCoeff, dTileC;
    int nTileX, nTileY;
    int nTilesX = (m_first.Width - 1) / nSizeX + 1;
    int	nTilesY = (m_first.Height - 1) / nSizeY + 1;
    int nR = 0;
    double dCos, dSin, Cx, Cy, dCheckY, dCheckX; 
    double dDisappear = m_dFrame * m_dFrame * m_dFrame;
    double dHalfnH = nH/2.0;
    double dHalfnW = nW/2.0;
	int nDestTmp, nSrcTmp;

    for ( i=0; i<nwRect; i++ )
    nRandomCol[i] = i;
    for ( i=0; i<nhRect; i++ )
    nRandomRow[i] = i;

    srand(1);

    std::random_shuffle( &nRandomCol[0], &nRandomCol[nwRect] );
    std::random_shuffle( &nRandomRow[0], &nRandomRow[nhRect] );

    m_result.CopyFrom(m_last);

    Cx = (nSizeX-1)/2.0;
    Cy = (nSizeY-1)/2.0;

    for (nRowIndex = 0; nRowIndex < nhRect; nRowIndex++)
    {
        nRow = nRandomRow[nRowIndex];
        dY_O = nRow * nH + (nH-1)/2.0;

        for (nColIndex = 0; nColIndex < nwRect; nColIndex++)
        {
            nCol = nRandomCol[nColIndex];
            dX_O = nCol * nW + (nW-1)/2.0;

            dSpeed = (double)rand()/RAND_MAX * 3;
            dDist = 1 - m_dFrame * dSpeed;
            if ( dDist < 0.1 )
                continue;
            dCoeff = 1/dDist;
            dAngle = (1 - dDist)* 3.1415 * 2;

            dX_O1 = (dX_O - Cx) * dCoeff + Cx;
            dY_O1 = (dY_O - Cy) * dCoeff + Cy;

            dCos = cos(dAngle)/dCoeff;
            dSin = sin(dAngle)/dCoeff;

            nR = (int)(dCoeff * dSizeRect + 1);
            dTileC = (nR-1)/2.0;

            for ( i=0; i<nR; i++) // y coordinate
            {
                dCheckY = i - dTileC;
                for ( j=0; j<nR; j++) // x coordinate
                {
                    dCheckX = j - dTileC;

                    dSourceTileX = dCheckX * dCos - dCheckY * dSin; 
                    dSourceTileY = dCheckX * dSin + dCheckY * dCos;
                    if ( fabs(dSourceTileX) <= dHalfnW && fabs(dSourceTileY) <= dHalfnH)
                    {
                        dSourceTileX += dX_O;
                        dSourceTileY += dY_O;
                        nDestTileY = (int)(dY_O1 + dCheckY);
                        nDestTileX = (int)(dX_O1 + dCheckX);
                        if ( dSourceTileY >= nSizeY || nDestTileY >= nSizeY || dSourceTileY < 0 || nDestTileY < 0 ||
                            dSourceTileX >= nSizeX || nDestTileX >= nSizeX || dSourceTileX < 0 || nDestTileX < 0 )
                            continue;
                        for ( nTileY=0; nTileY<nTilesY; nTileY++ )
                        {
                            dSourceY = nTileY * nSizeY + dSourceTileY;
                            nDestY = nTileY * nSizeY + nDestTileY;

                            if ( nDestY >= m_first.Height || nDestY < 0 || dSourceY >= m_first.Height || dSourceY < 0 )
                                continue;
                            nDestTmp = nImageStep * nDestY; 
                            nSrcTmp = (int)(dSourceY+0.5) * nImageStep; 
                            for ( nTileX=0; nTileX<nTilesX; nTileX++ )
                            {
                                dSourceX = nTileX * nSizeX + dSourceTileX;
                                nDestX = nTileX * nSizeX + nDestTileX;
                                if ( nDestX >= m_first.Width || nDestX < 0 || dSourceX >= m_first.Width || dSourceX < 0 )
                                    continue;
                                nDestIndex = nDestTmp + 4 * nDestX; 
                                nSrcIndex = nSrcTmp + 4 * ((int)(dSourceX+0.5)); 
                                m_result.Pixels[nDestIndex + 2] = (BYTE)(m_result.Pixels[nDestIndex + 2] * dDisappear + m_first.Pixels[nSrcIndex + 2] * (1 - dDisappear));
                                m_result.Pixels[nDestIndex + 1] = (BYTE)(m_result.Pixels[nDestIndex + 1] * dDisappear + m_first.Pixels[nSrcIndex + 1] * (1 - dDisappear));
                                m_result.Pixels[nDestIndex + 0] = (BYTE)(m_result.Pixels[nDestIndex + 0] * dDisappear + m_first.Pixels[nSrcIndex + 0] * (1 - dDisappear));
                            }
                        }
                    }
                }
            }
        }
    }
    delete [] nRandomCol;
    delete [] nRandomRow;
#endif
	// all ok
    return TRUE;
}
	
// graduate

//BOOL ImageCompose3::GraduateWipeSide(int Type)
//{
//	// compute tile size
//	BOOL bIsMODY2 = ((m_result.Height % m_nTileSizeY) == 1);
//	BOOL bIsMODX2 = ((m_result.Width % m_nTileSizeX) == 1);
//	int nSizeX = m_nTileSizeX;
//	int nSizeY = m_nTileSizeY;
//
//	int nSizeY2 = m_nTileSizeY;
//	if (bIsMODY2)
//		++nSizeY2;
//
//	int nSizeX2 = m_nTileSizeX;
//	if (bIsMODX2)
//		++nSizeX2;
//
//	// variables
//	int nX, nY, nIndex;
//	int nPatternX, nPatternY, nPatternValue;
//	int nLevelX = (int)( (nSizeX - 1)*m_dFrame );
//	int nLevelY = (int)( (nSizeY - 1)*m_dFrame );
//	int nLevelTile = (int)((m_nTilesX*m_nTilesY)*m_dFrame);
//	int nTileX, nTileY, nTileIndex, nTileOldIndex;
//	double dTileLevel;
//	
//	// compute starting indexex
//	nIndex = 0;
//	nTileIndex = 0;
//	nTileOldIndex = -1;
//
//	int nSizeYMem = nSizeY;
//	int nSizeXMem = nSizeX;
//
//	// combine two images
//	for (nY = 0; nY < m_first.Height; ++nY)
//	{
//		// compute pattern Y coordinate
//		nPatternY = nY % nSizeY;
//		nTileY = nY / m_nTileSizeY;
//
//		if (bIsMODY2 && nY >= ((m_nTilesY - 1)*nSizeY))
//		{
//			nSizeYMem = nSizeY2;
//			nPatternY = nY % nSizeY;
//			nTileY = nY / nSizeY;
//			if (nY == (m_first.Height - 1))
//			{
//				nPatternY = nSizeY;
//				--nTileY;
//			}
//		}
//
//		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
//		{
//			// compute pattern X coordinate
//			nPatternX = nX % nSizeX;
//			nTileX = nX / m_nTileSizeX;
//
//			nSizeXMem = nSizeX;
//			if (bIsMODX2 && nX >= ((m_nTilesX - 1)*nSizeX))
//			{
//				nSizeXMem = nSizeX2;
//				nPatternX = nX % nSizeX;
//				nTileX = nX / nSizeX;
//				if (nX == (m_first.Width - 1))
//				{
//					nPatternX = nSizeX;
//					--nTileX;
//				}
//			}
//
//			nTileIndex = nTileX + m_nTilesX*nTileY;
//
//			// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
//			nPatternValue = 0;
//
//			// check whether we reach new tile
//			if (nTileIndex != nTileOldIndex)
//			{
//				dTileLevel = m_graduate.GetLevel(nTileIndex, m_dFrame);
//				nTileOldIndex = nTileIndex;
//
//				nLevelX = (int)(nSizeXMem*dTileLevel);
//				nLevelY = (int)(nSizeYMem*dTileLevel);
//			}
//
//			// compute color value
//			if (Type == c_nSideLeftToRight)
//			{
//				if (nPatternX < nLevelX)
//					nPatternValue = 1;
//			}
//			else if (Type == c_nSideRightToLeft)
//			{
//				if (nPatternX > (nSizeXMem - 1) - nLevelX)
//					nPatternValue = 1;
//			}
//			else if (Type == c_nSideBottomToTop)
//			{
//				if (nPatternY < nLevelY)
//					nPatternValue = 1;
//			}
//			else if (Type == c_nSideTopToBottom)
//			{
//				if (nPatternY > (nSizeYMem - 1) - nLevelY)
//					nPatternValue = 1;
//			}
//			else if (Type == c_nSideLeftTopToRightBottom)
//			{
//				if (nPatternX < nLevelX && nPatternY > (nSizeY - 1) - nLevelY)
//					nPatternValue = 1;
//			}
//			else if (Type == c_nSideLeftBottomToRightTop)
//			{
//				if (nPatternX < nLevelX && nPatternY < nLevelY)
//					nPatternValue = 1;
//			}
//			else if (Type == c_nSideRightBottomToLeftTop)
//			{
//				if (nPatternX > (nSizeXMem - 1) - nLevelX && nPatternY < nLevelY)
//					nPatternValue = 1;
//			}
//			else if (Type == c_nSideRightTopToLeftBottom)
//			{
//				if (nPatternX > (nSizeXMem - 1) - nLevelX && nPatternY > (nSizeY - 1) - nLevelY)
//					nPatternValue = 1;
//			}
//
//			// combine colors
//			if (nPatternValue == 0)
//			{
//				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
//				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
//				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
//			}
//			else
//			{
//				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
//				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
//				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
//			}
//		}
//	}
//
//	// all ok
//	return TRUE;
//}
//
//BOOL ImageCompose3::GraduateWipeClock(int Type)
//{
//	// compute tile size
//	BOOL bIsMODY2 = ((m_result.Height % m_nTileSizeY) == 1);
//	BOOL bIsMODX2 = ((m_result.Width % m_nTileSizeX) == 1);
//	int nSizeX = m_nTileSizeX;
//	int nSizeY = m_nTileSizeY;
//
//	int nSizeY2 = m_nTileSizeY;
//	if (bIsMODY2)
//		++nSizeY2;
//
//	int nSizeX2 = m_nTileSizeX;
//	if (bIsMODX2)
//		++nSizeX2;
//
//	// variables
//	int nX, nY, nIndex, nSectorIndex;
//	int nPatternX, nPatternY, nPatternValue;
//	int nParts = 1;
//	double dSector = 3.1415*2;
//	double dAngle = 3.1415*m_dFrame*((Type == c_nBottomToTopOpposite || Type == c_nTopToBottomOpposite || 
//									  Type == c_nLeftToRightOpposite || Type == c_nRightToLeftOpposite ) ? 1 : 2);
//	double dStartAngle = 3.1415 * 0.5;
//	double dR, dA;
//	int nTileX, nTileY, nTileIndex, nTileOldIndex;
//	double dTileLevel;
//
//	// compute starting index
//	nIndex = 0;
//	nTileIndex = 0;
//	nTileOldIndex = -1;
//
//	if ( Type == c_n2AngleClockwise  || Type == c_n2AngleCounterClockwise  )
//		nParts = 2;
//	else if ( Type == c_n3AngleClockwise  || Type == c_n3AngleCounterClockwise  )
//		nParts = 3;
//
//	dAngle /= nParts;
//	dSector /= nParts;
//
//	// combine two images
//	for (nY = 0; nY < m_first.Height; ++nY)
//	{
//		// compute pattern Y coordinate
//		nPatternY = nY % nSizeY;
//		nTileY = nY / nSizeY;
//		
//		if (bIsMODY2 && nY >= nSizeY2)
//		{
//			nPatternY = nY % nSizeY2;
//			nTileY = nY / nSizeY2;
//		}
//
//		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
//		{bCurWipeSide = FALSE; 
//			// compute pattern X coordinate
//			nPatternX = nX % nSizeX;
//			nTileX = nX / m_nTileSizeX;
//
//			if (bIsMODX2 && nX >= nSizeX2)
//			{
//				nPatternX = nX % nSizeX2;
//				nTileX = nX / nSizeX2;
//			}
//
//			nTileIndex = nTileX + m_nTilesX*nTileY;
//
//			// reset pattern value
//			nPatternValue = 0;
//
//			// check whether we reach new tile
//			if (nTileIndex != nTileOldIndex)
//			{
//				dTileLevel = m_graduate.GetLevel(nTileIndex, m_dFrame);
//				nTileOldIndex = nTileIndex;
//
//				dAngle = 3.1415*dTileLevel*((Type == c_nBottomToTopOpposite || Type == c_nTopToBottomOpposite || 
//									  Type == c_nLeftToRightOpposite || Type == c_nRightToLeftOpposite ) ? 1 : 2);
//			}
//
//			// compute angles
//			if (Type == c_nBottomToTopClockwise || Type == c_nBottomToTopCounterClockwise || Type == c_nBottomToTopOpposite)
//				dStartAngle = -3.1415 * 0.5;
//			else if (Type == c_nLeftToRightClockwise || Type == c_nLeftToRightCounterClockwise || Type == c_nLeftToRightOpposite)
//				dStartAngle = 3.1415 ;
//			else if (Type == c_nRightToLeftClockwise || Type == c_nRightToLeftCounterClockwise || Type == c_nRightToLeftOpposite)
//				dStartAngle = 0.;
//			
//			// compute pixel radius and angle
//			GetRadiusAndAngle(nPatternX, nPatternY, 0.5*(nSizeX - 1), 0.5*(nSizeY - 1), dR, dA);
//				
//			dA -= dStartAngle;
//			
//			if ( Type == c_nBottomToTopClockwise || Type == c_nTopToBottomClockwise ||
//				 Type == c_nLeftToRightClockwise || Type == c_nRightToLeftClockwise ||
//				 Type == c_n3AngleClockwise  || Type == c_n2AngleClockwise )
//				 dA *= -1;
//			
//			if ( dA < 0 ) dA += 3.1415 * 2;
//				
//			for ( nSectorIndex=0; nSectorIndex<nParts; ++nSectorIndex )
//				if ( dA-nSectorIndex*dSector > 0 && dA-nSectorIndex*dSector < dAngle )
//				{	
//					nPatternValue = 1;
//					break;
//				}
//			if ( dA > (3.1415 * 2 - dAngle) && ( Type == c_nBottomToTopOpposite || 
//												  Type == c_nTopToBottomOpposite ||
//												  Type == c_nLeftToRightOpposite ||
//												  Type == c_nRightToLeftOpposite ) )
//				nPatternValue = 1;
//
//			// combine colors
//			if (nPatternValue == 0)
//			{
//				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
//				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
//				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
//			}
//			else
//			{
//				m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
//				m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
//				m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
//			}
//		}
//	}
//
//	// all ok
//	return TRUE;
//}
//
BOOL ImageCompose3::GraduateWipeSide(int Type)
{
	return Graduate(Type, TRUE);
}
BOOL ImageCompose3::GraduateWipeClock(int Type)
{
	return Graduate(Type, FALSE);
}
BOOL ImageCompose3::Graduate(int Type, BOOL bWipeSide)
{
	int nWideFade = 0;
	int nCurType = Type;
	BOOL bCurWipeSide = bWipeSide;

	switch(Type)
	{
	case c_nGraduateSideLeftToRightSmooth:       nCurType = c_nGraduateSideLeftToRight;       nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateSideRightToLeftSmooth:       nCurType = c_nGraduateSideRightToLeft;       nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateSideTopToBottomSmooth:       nCurType = c_nGraduateSideTopToBottom;       nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateSideBottomToTopSmooth:       nCurType = c_nGraduateSideBottomToTop;       nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateCornerLeftTopSmooth:         nCurType = c_nGraduateCornerLeftTop;         nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateCornerLeftBottomSmooth:      nCurType = c_nGraduateCornerLeftBottom;      nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateCornerRightBottomSmooth:     nCurType = c_nGraduateCornerRightBottom;     nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateCornerRightTopSmooth:        nCurType = c_nGraduateCornerRightTop;        nWideFade = 30; bCurWipeSide =  TRUE; break;
	case c_nGraduateClockBottomClockwiseSmooth:  nCurType = c_nGraduateClockBottomClockwise;  nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockBottomReverseSmooth:    nCurType = c_nGraduateClockBottomReverse;    nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockBottomOppositeSmooth:   nCurType = c_nGraduateClockBottomOpposite;   nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockTopClockwiseSmooth:     nCurType = c_nGraduateClockTopClockwise;     nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockTopReverseSmooth:       nCurType = c_nGraduateClockTopReverse;       nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockTopOppositeSmooth:      nCurType = c_nGraduateClockTopOpposite;      nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockLeftClockwiseSmooth:    nCurType = c_nGraduateClockLeftClockwise;    nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockLeftReverseSmooth:      nCurType = c_nGraduateClockLeftReverse;      nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockLeftOppositeSmooth:     nCurType = c_nGraduateClockLeftOpposite;     nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockRightClockwiseSmooth:   nCurType = c_nGraduateClockRightClockwise;   nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockRightReverseSmooth:	 nCurType = c_nGraduateClockRightReverse;     nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClockRightOppositeSmooth:    nCurType = c_nGraduateClockRightOpposite;    nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClock2AnglesClockwiseSmooth: nCurType = c_nGraduateClock2AnglesClockwise; nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClock2AnglesReverseSmooth:   nCurType = c_nGraduateClock2AnglesReverse;   nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClock3AnglesClockwiseSmooth: nCurType = c_nGraduateClock3AnglesClockwise; nWideFade = 40; bCurWipeSide = FALSE; break;
	case c_nGraduateClock3AnglesReverseSmooth:   nCurType = c_nGraduateClock3AnglesReverse;   nWideFade = 40; bCurWipeSide = FALSE; break;
	}
	return IPPMaskGraduate(m_first, m_last, m_result, m_nTilesX, m_nTilesY, nCurType, m_dFrame, &m_maskGraduate, nWideFade, bCurWipeSide);
}
BOOL ImageCompose3::WipePathCommon(int nTileCountX, int nTileCountY, GraphicsPath* path)
{
#define   MASK_SIZE     100

	if (nTileCountY<1 || nTileCountX<1 || !path)
		return FALSE;
	Matrix decM;
	decM.Translate(-MASK_SIZE/2, -MASK_SIZE/2);

	GraphicsPath* decPath = path->Clone();

	decPath->Transform(&decM);

	decM.Reset();
	decM.Scale(0.9,0.9);
	decPath->Transform(&decM);

	decM.Reset();
	decM.Translate(MASK_SIZE/2, MASK_SIZE/2);
	decPath->Transform(&decM);

	Bitmap bmp(MASK_SIZE,MASK_SIZE,PixelFormat24bppRGB);
	Graphics gr(&bmp);
	gr.Clear(Color(0,0,0));

	SolidBrush brsh(Color(0,255,0));
	gr.FillPath(&brsh, path);
	SolidBrush brsh2(Color(255,0,0));
	gr.FillPath(&brsh2, decPath);

	delete decPath;

	BitmapData oBitmapData;

	if( Ok != bmp.LockBits(NULL, ImageLockModeRead, PixelFormat24bppRGB, 
		&oBitmapData) || !oBitmapData.Scan0)
		return FALSE;

	BYTE* maskPixels = (BYTE*)oBitmapData.Scan0;
	int stride = oBitmapData.Stride;

	double tmpVal = 5.0*(1.0-m_dFrame)/*/2.0*/;

	double  pathRectProportions[4];
	pathRectProportions[0] = MASK_SIZE*(0.5-tmpVal);// left
	pathRectProportions[1] = MASK_SIZE*(0.5+tmpVal);// top
	pathRectProportions[2] = MASK_SIZE*(0.5+tmpVal);// right
	pathRectProportions[3] = MASK_SIZE*(0.5-tmpVal);// bottom

	double  pathRectProportionsWidth = pathRectProportions[2]-pathRectProportions[0];//right-left
	double  pathRectProportionsHeight = pathRectProportions[1]-pathRectProportions[3];//top-bottom

	int     TileXSizePix = m_first.Width/nTileCountX;
	int     TileYSizePix = m_first.Height/nTileCountY;

	int		nX, nY, nIndex;
	nIndex = 0;
	// combine two images
	for (nY = 0; nY < m_first.Height; ++nY)
	{
		// compute pattern Y coordinate
		double PatternY = pathRectProportions[3]+(nY % TileYSizePix)*pathRectProportionsHeight/TileYSizePix;

		for (nX = 0; nX < m_first.Width; ++nX, nIndex += 4)
		{
			// compute pattern X coordinate
			double PatternX = pathRectProportions[0]+(nX % TileXSizePix)*pathRectProportionsWidth/TileXSizePix;

			Color tmpCol;
			int  nTempIndex = (((int)PatternY)*stride)+(((int)PatternX)*3);
			// combine colors
			if (PatternX>0 && PatternX<MASK_SIZE && PatternY>0 && PatternY<MASK_SIZE)
			{
				if (maskPixels[nTempIndex+2]==255 && maskPixels[nTempIndex+1]==0)
				{
					m_result.Pixels[nIndex + 2] = m_last.Pixels[nIndex + 2];
					m_result.Pixels[nIndex + 1] = m_last.Pixels[nIndex + 1];
					m_result.Pixels[nIndex + 0] = m_last.Pixels[nIndex + 0];
				}
				else
					if (maskPixels[nTempIndex]==0 && maskPixels[nTempIndex+1]==255)
					{
						m_result.Pixels[nIndex + 2] = (m_last.Pixels[nIndex + 2]+m_first.Pixels[nIndex + 2])/2;
						m_result.Pixels[nIndex + 1] = (m_last.Pixels[nIndex + 1]+m_first.Pixels[nIndex + 1])/2;
						m_result.Pixels[nIndex + 0] = (m_last.Pixels[nIndex + 0]+m_first.Pixels[nIndex + 0])/2;
					}
					else
					{
						m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
						m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
						m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
					}
			}
			else
			{
				m_result.Pixels[nIndex + 2] = m_first.Pixels[nIndex + 2];
				m_result.Pixels[nIndex + 1] = m_first.Pixels[nIndex + 1];
				m_result.Pixels[nIndex + 0] = m_first.Pixels[nIndex + 0];
			}
		}
	}

	bmp.UnlockBits(&oBitmapData);

	return TRUE;
}


BOOL ImageCompose3::Mask(int Type)
{
	int nFadeWide = 0;
	if ( ( c_nBowTileSmoothHorizontal <= Type && c_nZigZagSmoothVertical >= Type) ||
				(c_nWipeSmoothBoxIn <= Type && c_nWipeSmoothDiamondIn  >= Type) || c_nWipeSmoothCircleIn == Type ||
		      ( c_nCircleCornerSmoothLeftTop <= Type && c_nCircleCornerSmoothRightBottom >= Type ))
	{
		nFadeWide = 40;
	}
	else if ( c_nWipeSmoothCircle <= Type && c_nWipeSmoothWideKeyHoles >= Type)
	{
		nFadeWide = 30;
	}
	else if ( c_nWipeSmoothNarrowStar <= Type && c_nWipeSmoothNarrowFlowers >= Type)
	{
		nFadeWide = 10;
	}
	else 
	{
		nFadeWide = 0;
	}
	IPPMask(m_first, m_last, m_result, m_dFrame, Type, &m_mask, nFadeWide);
	return TRUE;
}

BOOL ImageCompose3::Tetris(int Type)
{
	IPPTetris(m_first, m_last, m_result, m_dFrame, Type, &m_utility);
	return TRUE;
}

BOOL ImageCompose3::ShotChange(int Type)
{
	BYTE nBkColor = 0;
	BYTE nFilmColorMid = 32;
	BYTE nFilmColorBorder = 96;
	BYTE nBlendColor = 128;
	if( (Type >= c_nShotChangeGHorizontalRToL    && Type <= c_nShotChangeGVerticalBToU) ||
		(Type >= c_nShotChangeGHorizontalRToLNoZ && Type <= c_nShotChangeGVerticalBToUNoZ))
	{
		nBkColor = 0;
		nFilmColorMid = 32;
		nFilmColorBorder = 96;
		nBlendColor = 128;
	}
	else
	{
		nBkColor = 220;
		nFilmColorMid = 0;
		nFilmColorBorder = 32;
		nBlendColor = 128;
	}
	if ( c_nShotChangeGHorizontalLToR == Type || c_nShotChangeGHorizontalRToL == Type ||
		 c_nShotChangeBHorizontalLToR == Type || c_nShotChangeBHorizontalRToL == Type)
		IPPShotChangeHorizontal(m_first, m_last, m_result, m_dFrame, Type, &m_utility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
	else if ( c_nShotChangeGVerticalBToU == Type || c_nShotChangeGVerticalUToB == Type ||
		      c_nShotChangeBVerticalBToU == Type || c_nShotChangeBVerticalUToB == Type)
		IPPShotChangeVertical(m_first, m_last, m_result, m_dFrame, Type, &m_utility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
	else if ( c_nShotChangeGHorizontalRToLNoZ == Type || c_nShotChangeGHorizontalLToRNoZ == Type ||
		      c_nShotChangeBHorizontalRToLNoZ == Type || c_nShotChangeBHorizontalLToRNoZ == Type)
		IPPShotChangeHorizontalWithoutZoom(m_first, m_last, m_result, m_dFrame, Type, &m_utility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
	else if ( c_nShotChangeGVerticalBToUNoZ == Type || c_nShotChangeGVerticalUToBNoZ == Type ||
		      c_nShotChangeBVerticalBToUNoZ == Type || c_nShotChangeBVerticalUToBNoZ == Type)
		IPPShotChangeVerticalWithoutZoom(m_first, m_last, m_result, m_dFrame, Type, &m_utility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
	return TRUE;
}

BOOL ImageCompose3::BlackWhiteLines(int Type)
{
	if ( c_nBlackWhiteLines == Type )
		IPPBlackWhiteLines(m_first, m_last, m_result, m_dFrame, FALSE);
	if ( c_nBlackWhiteLinesInverse == Type )
		IPPBlackWhiteLines(m_first, m_last, m_result, m_dFrame, TRUE);
	return TRUE;
}

BOOL ImageCompose3::MovieMaker(int Type)
{
	switch( Type )
	{
	case c_nEye: 
		IPPEye(m_first, m_last, m_result, m_dFrame, 0);
		break;
	case c_nSmoothEye: 
		IPPEye(m_first, m_last, m_result, m_dFrame, 8);
		break;
	case c_nHeart: 
		IPPHeart(m_first, m_last, m_result, m_dFrame, 0);
		break;
	case c_nSmoothHeart: 
		IPPHeart(m_first, m_last, m_result, m_dFrame, 8);
		break;
	case c_nPixelate: 
		IPPPixelate(m_first, m_last, m_result, m_dFrame);
		break;
	case c_nRollRight: 
		IPPRoll(m_first, m_last, m_result, m_dFrame, 0);
		break;
	case c_nRollLeft: 
		IPPRoll(m_first, m_last, m_result, m_dFrame, 1);
		break;
	case c_nSlideUp: 
		IPPSlide(m_first, m_last, m_result, m_dFrame, 0);
		break;
	case c_nSlideRight:	
		IPPSlide(m_first, m_last, m_result, m_dFrame, 1);
		break;
	case c_nSlideDown: 
		IPPSlide(m_first, m_last, m_result, m_dFrame, 2);
		break;
	case c_nSlideLeft: 
		IPPSlide(m_first, m_last, m_result, m_dFrame, 3);
		break;
	case c_nSlideCenterUp: 
		IPPSlideCenter(m_first, m_last, m_result, m_dFrame, 0);
		break;
	case c_nSlideCenterRight: 
		IPPSlideCenter(m_first, m_last, m_result, m_dFrame, 1);
		break;
	case c_nSlideCenterDown: 
		IPPSlideCenter(m_first, m_last, m_result, m_dFrame, 2);
		break;
	case c_nSlideCenterLeft: 
		IPPSlideCenter(m_first, m_last, m_result, m_dFrame, 3);
		break;
	case c_nShrinkInVertical: 
		IPPShrink(m_first, m_last, m_result, m_dFrame, 0);
		break;
	case c_nShrinkInHorizontal:	
		IPPShrink(m_first, m_last, m_result, m_dFrame, 1);
		break;
	case c_nWave:
		IPPSimpleWave(m_first, m_last, m_result, m_dFrame, 0);
		break;
	case c_nSmoothWave:
		IPPSimpleWave(m_first, m_last, m_result, m_dFrame, 8);
		break;
	}
	return TRUE;
}

BOOL ImageCompose3::Spiral(int Type)
{
	int nDirection = 0;
	switch(Type)
	{
	case c_nSpiralTopLeft: 
		nDirection = 0;
		break;
	case c_nSpiralTopRight: 
		nDirection = 1;
		break;
	case c_nSpiralBottomRight: 
		nDirection = 2;
		break;
	case c_nSpiralBottomLeft: 
		nDirection = 3;
		break;
	case c_nSpiralCenterIn: 
		nDirection = 4;
		break;
	case c_nSpiralReverseCenterIn: 
		nDirection = 5;
		break;
	case c_nSpiralCenterOut: 
		nDirection = 6;
		break;
	case c_nSpiralReverseCenterOut: 
		nDirection = 7;
		break;
	}
	IPPSpiral(m_first, m_last, m_result, m_dFrame, nDirection);
	return TRUE;
}

BOOL ImageCompose3::PinwheelCurved(int Type)
{
	int nFadeWide = 0;
	if ( c_nPinwheelCurvedSN12ASmooth == Type || c_nPinwheelPropeller12Smooth == Type ||
		 c_nPinwheelCurvedSW12ASmooth == Type || c_nPinwheelCurvedLN12ASmooth == Type)
	{
		nFadeWide = 160;
	}
	else if( c_nPinwheelCurvedSN7ASmooth == Type || c_nPinwheelPropeller7Smooth == Type ||
		     c_nPinwheelCurvedSW7ASmooth == Type || c_nPinwheelCurvedLN7ASmooth == Type)
	{
		nFadeWide = 80;
	}
	else if ( c_nPinwheelCurvedSN5ASmooth == Type || c_nPinwheelPropeller4Smooth == Type ||
		      c_nPinwheelCurvedSW5ASmooth == Type || c_nPinwheelCurvedLN5ASmooth == Type)
	{
		nFadeWide = 40;
	}
	else 
	{
		nFadeWide = 0;
	}
	IPPPinwheelCurved(m_first, m_last, m_result, m_dFrame, Type, &m_maskPinwheelCurved, nFadeWide);
	return TRUE;
}

BOOL ImageCompose3::Puzzle(int nType)
{
	int nXCount   = 1; 
	int	nYCount   = 1;
	int nFlyCount = 1;
	int nCorners  = 0;
	BOOL bPuzzle        = TRUE;
	BOOL nShuffleType   = 0;
	BOOL bCollapse      = FALSE;

	switch(nType)
	{
	case c_nPuzzle4x4:             nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
	case c_nPuzzle7x7:             nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
	case c_nPuzzle10x10:           nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
	case c_nPuzzle4x4Collapse:     nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
	case c_nPuzzle7x7Collapse:     nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
	case c_nPuzzle10x10Collapse:   nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
	case c_nTile4x4Cons:           nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 0; nCorners = 6; bCollapse = FALSE; break;
	case c_nTile4x4Rand:           nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
	case c_nTile7x7Cons:           nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 0; nCorners = 6; bCollapse = FALSE; break;
	case c_nTile7x7Rand:           nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
	case c_nTile10x10Cons:         nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 0; nCorners = 6; bCollapse = FALSE; break;
	case c_nTile10x10Rand:         nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
	case c_nTile4x4ConsCollapse:   nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 2; nCorners = 5; bCollapse =  TRUE; break;
	case c_nTile4x4RandCollapse:   nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
	case c_nTile7x7ConsCollapse:   nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 2; nCorners = 5; bCollapse =  TRUE; break;
	case c_nTile7x7RandCollapse:   nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
	case c_nTile10x10ConsCollapse: nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 2; nCorners = 5; bCollapse =  TRUE; break;
	case c_nTile10x10RandCollapse: nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
	}
	return IPPPuzzle(m_first, m_last, m_result, m_dFrame, nType, &m_puzzleUtility, nXCount, nYCount, nFlyCount, nCorners, bPuzzle, nShuffleType, bCollapse);
}

BOOL ImageCompose3::Tile(int nType)
{
	int nFigureType = 0;
	double dSideFactor = 0;

	int nFlyCount      = 1;
	int nStartType     = 0;
	int nShuffleType   = 0;
	BOOL bCollapse     = FALSE;

	switch(nType)
	{
	case c_nSmallHexagons:                nFigureType =  6; dSideFactor = 15.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
	case c_nLargeHexagons:                nFigureType =  6; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
	case c_nSmallHexagonsCollapse:        nFigureType =  6; dSideFactor = 15.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse =  TRUE; break;
	case c_nLargeHexagonsCollapse:        nFigureType =  6; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse =  TRUE; break;

	case c_nSmallRhombuses:               nFigureType = 41; dSideFactor = 12.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
	case c_nLargeRhombuses:               nFigureType = 41; dSideFactor =  8.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
	case c_nSmallRhombusesCollapse:       nFigureType = 41; dSideFactor = 12.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse =  TRUE; break;
	case c_nLargeRhombusesCollapse:       nFigureType = 41; dSideFactor =  8.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse =  TRUE; break;

	case c_nSmallBricks:                  nFigureType = 42; dSideFactor = 10.0; nShuffleType = 2; nFlyCount = 15; nStartType = 5; bCollapse = FALSE; break;
	case c_nLargeBricks:                  nFigureType = 42; dSideFactor =  7.0; nShuffleType = 2; nFlyCount = 10; nStartType = 5; bCollapse = FALSE; break;
	case c_nSmallBricks2:                 nFigureType = 42; dSideFactor = 10.0; nShuffleType = 3; nFlyCount = 15; nStartType = 5; bCollapse = FALSE; break;
	case c_nLargeBricks2:                 nFigureType = 42; dSideFactor =  7.0; nShuffleType = 3; nFlyCount = 10; nStartType = 5; bCollapse = FALSE; break;
	case c_nSmallBricks3:                 nFigureType = 42; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
	case c_nLargeBricks3:                 nFigureType = 42; dSideFactor =  7.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
	case c_nSmallBricksCollapse:          nFigureType = 42; dSideFactor = 10.0; nShuffleType = 0; nFlyCount = 15; nStartType = 6; bCollapse =  TRUE; break;
	case c_nLargeBricksCollapse:          nFigureType = 42; dSideFactor =  7.0; nShuffleType = 0; nFlyCount = 10; nStartType = 6; bCollapse =  TRUE; break;

	case c_nSmallTriangles:               nFigureType =  3; dSideFactor = 10.0; nShuffleType = 2; nFlyCount = 15; nStartType = 0; bCollapse = FALSE; break;
	case c_nLargeTriangles:               nFigureType =  3; dSideFactor =  7.0; nShuffleType = 2; nFlyCount = 10; nStartType = 0; bCollapse = FALSE; break;
	case c_nSmallTriangles2:              nFigureType =  3; dSideFactor = 10.0; nShuffleType = 3; nFlyCount = 15; nStartType = 7; bCollapse = FALSE; break;
	case c_nLargeTriangles2:              nFigureType =  3; dSideFactor =  7.0; nShuffleType = 3; nFlyCount = 10; nStartType = 7; bCollapse = FALSE; break;
	case c_nSmallTriangles3:              nFigureType =  3; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
	case c_nLargeTriangles3:              nFigureType =  3; dSideFactor =  7.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
	case c_nSmallTrianglesCollapse:       nFigureType =  3; dSideFactor = 10.0; nShuffleType = 0; nFlyCount = 15; nStartType = 6; bCollapse =  TRUE; break;
	case c_nLargeTrianglesCollapse:       nFigureType =  3; dSideFactor =  7.0; nShuffleType = 0; nFlyCount = 10; nStartType = 6; bCollapse =  TRUE; break;


	case c_nSmallInclinedBricks:          nFigureType = 43; dSideFactor =  9.0; nShuffleType = 2; nFlyCount = 15; nStartType = 5; bCollapse = FALSE; break;
	case c_nLargeInclinedBricks:          nFigureType = 43; dSideFactor =  6.0; nShuffleType = 2; nFlyCount = 10; nStartType = 5; bCollapse = FALSE; break;
	case c_nSmallInclinedBricks2:         nFigureType = 43; dSideFactor =  9.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
	case c_nLargeInclinedBricks2:         nFigureType = 43; dSideFactor =  6.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
	case c_nSmallInclinedBricksCollapse:  nFigureType = 43; dSideFactor =  9.0; nShuffleType = 0; nFlyCount = 15; nStartType = 6; bCollapse =  TRUE; break;
	case c_nLargeInclinedBricksCollapse:  nFigureType = 43; dSideFactor =  6.0; nShuffleType = 0; nFlyCount = 10; nStartType = 6; bCollapse =  TRUE; break;
	}
	return IPPTiles(m_first, m_last, m_result, nFigureType, m_dFrame, dSideFactor, nType, &m_puzzleUtility, nFlyCount, nShuffleType, nStartType, bCollapse);
}
