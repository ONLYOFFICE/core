#pragma once

#include <ImageTransformsCoreWrappers.h>

#include <ImagePaintComposeConstants.h>
#include <ImagePaintComposeIPP.h>
#include <algorithm>

#include <GeometryEx.h>


// TODO: 
// все функции в данном файле не проверяют указатели и какие-либо параметры на валидность
// соответственно, использовать с осторожностью

namespace ImageStudio
{
	namespace Paint
	{
		namespace Compose
		{
			namespace BGRA
			{
				BOOL BGRA_SimpleCopy(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nSimpleCopyFirst)
						memcpy(pBGRAResult, pBGRASource1, 4*nWidth*nHeight);
					else if (nType == Constants::c_nSimpleCopyLast)
						memcpy(pBGRAResult, pBGRASource2, 4*nWidth*nHeight);

					return TRUE;
				}

				BOOL BGRA_Fade(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// combine two images
					switch( nType )
					{
					case Constants::c_nFadeZoomIn:
						IPP::IPP_FadeZoom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, TRUE, FALSE);
						break;
					case Constants::c_nFadeZoomOut:
						IPP::IPP_FadeZoom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, FALSE, TRUE);
						break;
					case Constants::c_nFadeZoomBoth:
						IPP::IPP_FadeZoom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, TRUE, TRUE);
						break;
					case Constants::c_nFadeSolid:
						AlphaBlend( pBGRAResult, pBGRASource1, pBGRASource2, nWidth*nHeight, dCompleteness );
						break;

					default:
						const BYTE* pPattern = NULL;
						
						if( nType >= Constants::c_nFadePatternDots2 && nType <= Constants::c_nFadePatternCross2 )
							nType -= 30;
						else
						if( nType >= Constants::c_nFadePatternDots3 && nType <= Constants::c_nFadePatternCross3 )
							nType -= 60;

						switch( nType )
						{
						case Constants::c_nFadePatternDots:                pPattern = &Constants::c_byPatternDots[0][0]; break;
						case Constants::c_nFadePatternDotsDense:           pPattern = &Constants::c_byPatternDotsDense[0][0]; break;
						case Constants::c_nFadePatternGrid:                pPattern = &Constants::c_byPatternGrid[0][0]; break;
						case Constants::c_nFadePatternGridRotated:         pPattern = &Constants::c_byPatternGridRotated[0][0]; break;
						case Constants::c_nFadePatternLineVertical:        pPattern = &Constants::c_byPatternLineVertical[0][0]; break;
						case Constants::c_nFadePatternLineHorizontal:      pPattern = &Constants::c_byPatternLineHorizontal[0][0]; break;
						case Constants::c_nFadePatternLineDiagonal:        pPattern = &Constants::c_byPatternLineDiagonal[0][0]; break;
						case Constants::c_nFadePatternLineDiagonalRotated: pPattern = &Constants::c_byPatternLineDiagonalRotated[0][0]; break;
						case Constants::c_nFadePatternChess:               pPattern = &Constants::c_byPatternChessBoard[0][0]; break;
						case Constants::c_nFadePatternDashes:              pPattern = &Constants::c_byPatternDashes[0][0]; break;
						case Constants::c_nFadePatternSpiner:              pPattern = &Constants::c_byPatternSpiner[0][0]; break;
						case Constants::c_nFadePatternHeart:	           pPattern = &Constants::c_byPatternHeart[0][0]; break;
						case Constants::c_nFadePatternCross:	           pPattern = &Constants::c_byPatternCross[0][0]; break;
						case Constants::c_nFadePatternThatches:            pPattern = &Constants::c_byPatternThatches[0][0]; break;
						}

						if( !pPattern )
						{
							AlphaBlend( pBGRAResult, pBGRASource1, pBGRASource2, nWidth*nHeight, dCompleteness );
							break;
						}

						double dTileFactorX = 1.0 / nTileSizeX;
						double dTileFactorY = 1.0 / nTileSizeY;

						int nAlpha = int(dCompleteness * 256 + 0.5);

						BYTE* pResult = pBGRAResult;
						BYTE* pDst = pBGRASource1;
						BYTE* pSrc = pBGRASource2;

						int nTileY = 0;
						for( int nY = 0; nY < nHeight; ++nY )
						{
							// compute pattern Y coordinate
							if( ++nTileY >= nTileSizeY )
								nTileY = 0;

							int nPatternY1 = int((nTileY * Constants::c_nPatternSize * 256) * dTileFactorY);
							int nFactorY   = nPatternY1 & 255; nPatternY1 >>= 8;
							int nPatternY2 = (nPatternY1 < Constants::c_nPatternSize - 1) ? nPatternY1 + 1 : 0;

							int nTileX = 0;
							for( int nX = 0; nX < nWidth; ++nX, pResult += 4, pDst += 4, pSrc += 4 )
							{
								// compute pattern X coordinate
								if( ++nTileX >= nTileSizeX )
									nTileX = 0;

								int nPatternX1 = int((nTileX * Constants::c_nPatternSize * 256) * dTileFactorX);
								int nFactorX   = nPatternX1 & 255; nPatternX1 >>= 8;
								int nPatternX2 = (nPatternX1 < Constants::c_nPatternSize - 1) ? nPatternX1 + 1 : 0;

								// compute pattern value
								int nP11 = pPattern[nPatternY1 * Constants::c_nPatternSize + nPatternX1] * 256;
								int nP12 = pPattern[nPatternY1 * Constants::c_nPatternSize + nPatternX2] * 256;
								int nP21 = pPattern[nPatternY2 * Constants::c_nPatternSize + nPatternX1] * 256;
								int nP22 = pPattern[nPatternY2 * Constants::c_nPatternSize + nPatternX2] * 256;

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
								pResult[3] = pDst[3];
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_Burning(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CUtility* pUtility)
				{
					if (nType == Constants::c_nEnhancedBurningThreshold)
						return IPP::IPP_Burning(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, pUtility);

					if (nType == Constants::c_nEnhancedBurningFade)
						return IPP::IPP_BurningFade(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, pUtility);

					if (nType == Constants::c_nEnhancedBurningFire)
						return IPP::IPP_BurningFire(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, pUtility);

					return FALSE;
				}
				BOOL BGRA_GaussianBlur(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					//return IPP::IPP_GaussianBlur(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness);
					return IPP::IPP_GaussianBlur2(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness);
				}
				BOOL BGRA_Noise(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nEnhancedColorNoise)
						return IPP::IPP_Noise(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, FALSE);

					if (nType == Constants::c_nEnhancedGrayscaleNoise)
						return IPP::IPP_Noise(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, TRUE);

					if (nType == Constants::c_nEnhancedFlashLight)
						return IPP::IPP_Flash(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, RGB(255, 255, 255));

					if (nType == Constants::c_nEnhancedFlashDark)
						return IPP::IPP_Flash(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, RGB(0, 0, 0));

					return FALSE;
				}

				BOOL BGRA_Tetris(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CUtility* pUtility)
				{
					if (Constants::c_nTetris == nType)
						return IPP::IPP_Tetris(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, pUtility, FALSE);
					if (Constants::c_nTetrisCollapse == nType)
						return IPP::IPP_Tetris(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, pUtility, TRUE);
					return FALSE;
				}

				BOOL BGRA_Puzzle(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CPuzzleUtility* pPuzzleUtility)
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
					case Constants::c_nPuzzle4x4:             nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
					case Constants::c_nPuzzle7x7:             nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
					case Constants::c_nPuzzle10x10:           nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
					case Constants::c_nPuzzle4x4Collapse:     nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
					case Constants::c_nPuzzle7x7Collapse:     nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
					case Constants::c_nPuzzle10x10Collapse:   nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle =  TRUE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
					case Constants::c_nTile4x4Cons:           nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 0; nCorners = 6; bCollapse = FALSE; break;
					case Constants::c_nTile4x4Rand:           nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
					case Constants::c_nTile7x7Cons:           nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 0; nCorners = 6; bCollapse = FALSE; break;
					case Constants::c_nTile7x7Rand:           nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
					case Constants::c_nTile10x10Cons:         nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 0; nCorners = 6; bCollapse = FALSE; break;
					case Constants::c_nTile10x10Rand:         nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse = FALSE; break;
					case Constants::c_nTile4x4ConsCollapse:   nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 2; nCorners = 5; bCollapse =  TRUE; break;
					case Constants::c_nTile4x4RandCollapse:   nXCount =  4; nYCount =  4; nFlyCount =  5; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
					case Constants::c_nTile7x7ConsCollapse:   nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 2; nCorners = 5; bCollapse =  TRUE; break;
					case Constants::c_nTile7x7RandCollapse:   nXCount =  7; nYCount =  7; nFlyCount = 10; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
					case Constants::c_nTile10x10ConsCollapse: nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 2; nCorners = 5; bCollapse =  TRUE; break;
					case Constants::c_nTile10x10RandCollapse: nXCount = 10; nYCount = 10; nFlyCount = 15; bPuzzle = FALSE; nShuffleType = 1; nCorners = 4; bCollapse =  TRUE; break;
					}
					return IPP::IPP_Puzzle(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, pPuzzleUtility, nXCount, nYCount, nFlyCount, nCorners, bPuzzle, nShuffleType, bCollapse);
				}

				BOOL BGRA_Tiles(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight,  int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CPuzzleUtility* pPuzzleUtility)
				{
					int nFigureType = 0;
					double dSideFactor = 0;

					int nFlyCount      = 1;
					int nStartType     = 0;
					int nShuffleType   = 0;
					BOOL bCollapse     = FALSE;

					switch(nType)
					{
					case Constants::c_nSmallHexagons:                nFigureType =  6; dSideFactor = 15.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nLargeHexagons:                nFigureType =  6; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nSmallHexagonsCollapse:        nFigureType =  6; dSideFactor = 15.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse =  TRUE; break;
					case Constants::c_nLargeHexagonsCollapse:        nFigureType =  6; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse =  TRUE; break;

					case Constants::c_nSmallRhombuses:               nFigureType = 41; dSideFactor = 12.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nLargeRhombuses:               nFigureType = 41; dSideFactor =  8.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nSmallRhombusesCollapse:       nFigureType = 41; dSideFactor = 12.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse =  TRUE; break;
					case Constants::c_nLargeRhombusesCollapse:       nFigureType = 41; dSideFactor =  8.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse =  TRUE; break;

					case Constants::c_nSmallBricks:                  nFigureType = 42; dSideFactor = 10.0; nShuffleType = 2; nFlyCount = 15; nStartType = 5; bCollapse = FALSE; break;
					case Constants::c_nLargeBricks:                  nFigureType = 42; dSideFactor =  7.0; nShuffleType = 2; nFlyCount = 10; nStartType = 5; bCollapse = FALSE; break;
					case Constants::c_nSmallBricks2:                 nFigureType = 42; dSideFactor = 10.0; nShuffleType = 3; nFlyCount = 15; nStartType = 5; bCollapse = FALSE; break;
					case Constants::c_nLargeBricks2:                 nFigureType = 42; dSideFactor =  7.0; nShuffleType = 3; nFlyCount = 10; nStartType = 5; bCollapse = FALSE; break;
					case Constants::c_nSmallBricks3:                 nFigureType = 42; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nLargeBricks3:                 nFigureType = 42; dSideFactor =  7.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nSmallBricksCollapse:          nFigureType = 42; dSideFactor = 10.0; nShuffleType = 0; nFlyCount = 15; nStartType = 6; bCollapse =  TRUE; break;
					case Constants::c_nLargeBricksCollapse:          nFigureType = 42; dSideFactor =  7.0; nShuffleType = 0; nFlyCount = 10; nStartType = 6; bCollapse =  TRUE; break;

					case Constants::c_nSmallTriangles:               nFigureType =  3; dSideFactor = 10.0; nShuffleType = 2; nFlyCount = 15; nStartType = 0; bCollapse = FALSE; break;
					case Constants::c_nLargeTriangles:               nFigureType =  3; dSideFactor =  7.0; nShuffleType = 2; nFlyCount = 10; nStartType = 0; bCollapse = FALSE; break;
					case Constants::c_nSmallTriangles2:              nFigureType =  3; dSideFactor = 10.0; nShuffleType = 3; nFlyCount = 15; nStartType = 7; bCollapse = FALSE; break;
					case Constants::c_nLargeTriangles2:              nFigureType =  3; dSideFactor =  7.0; nShuffleType = 3; nFlyCount = 10; nStartType = 7; bCollapse = FALSE; break;
					case Constants::c_nSmallTriangles3:              nFigureType =  3; dSideFactor = 10.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nLargeTriangles3:              nFigureType =  3; dSideFactor =  7.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nSmallTrianglesCollapse:       nFigureType =  3; dSideFactor = 10.0; nShuffleType = 0; nFlyCount = 15; nStartType = 6; bCollapse =  TRUE; break;
					case Constants::c_nLargeTrianglesCollapse:       nFigureType =  3; dSideFactor =  7.0; nShuffleType = 0; nFlyCount = 10; nStartType = 6; bCollapse =  TRUE; break;


					case Constants::c_nSmallInclinedBricks:          nFigureType = 43; dSideFactor =  9.0; nShuffleType = 2; nFlyCount = 15; nStartType = 5; bCollapse = FALSE; break;
					case Constants::c_nLargeInclinedBricks:          nFigureType = 43; dSideFactor =  6.0; nShuffleType = 2; nFlyCount = 10; nStartType = 5; bCollapse = FALSE; break;
					case Constants::c_nSmallInclinedBricks2:         nFigureType = 43; dSideFactor =  9.0; nShuffleType = 1; nFlyCount = 15; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nLargeInclinedBricks2:         nFigureType = 43; dSideFactor =  6.0; nShuffleType = 1; nFlyCount = 10; nStartType = 4; bCollapse = FALSE; break;
					case Constants::c_nSmallInclinedBricksCollapse:  nFigureType = 43; dSideFactor =  9.0; nShuffleType = 0; nFlyCount = 15; nStartType = 6; bCollapse =  TRUE; break;
					case Constants::c_nLargeInclinedBricksCollapse:  nFigureType = 43; dSideFactor =  6.0; nShuffleType = 0; nFlyCount = 10; nStartType = 6; bCollapse =  TRUE; break;
					}
					return IPP::IPP_Tiles(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nFigureType, dCompleteness, dSideFactor, nType, pPuzzleUtility, nFlyCount, nShuffleType, nStartType, bCollapse);
				}
				BOOL BGRA_ShotChange(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CUtility* pUtility)
				{
					
					BYTE nBkColor = 0;
					BYTE nFilmColorMid = 32;
					BYTE nFilmColorBorder = 96;
					BYTE nBlendColor = 128;
					if( (nType >= Constants::c_nShotChangeGHorizontalRToL && nType <= Constants::c_nShotChangeGVerticalBToU) ||
						(nType >= Constants::c_nShotChangeGHorizontalRToLNoZ && nType <= Constants::c_nShotChangeGVerticalBToUNoZ))
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
					if(Constants::c_nShotChangeGHorizontalLToR == nType || Constants::c_nShotChangeBHorizontalLToR == nType)
						return IPP::IPP_ShotChangeHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, FALSE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					if(Constants::c_nShotChangeGHorizontalRToL == nType || Constants::c_nShotChangeBHorizontalRToL == nType)
						return IPP::IPP_ShotChangeHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, TRUE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					if(Constants::c_nShotChangeGVerticalBToU == nType || Constants::c_nShotChangeBVerticalBToU == nType)
						return IPP::IPP_ShotChangeVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, TRUE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					if(Constants::c_nShotChangeGVerticalUToB == nType || Constants::c_nShotChangeBVerticalUToB == nType)
						return IPP::IPP_ShotChangeVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, FALSE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					if(Constants::c_nShotChangeGHorizontalRToLNoZ == nType || Constants::c_nShotChangeBHorizontalRToLNoZ == nType)
						return IPP::IPP_ShotChangeHorizontalWithoutZoom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, TRUE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					if(Constants::c_nShotChangeGHorizontalLToRNoZ == nType || Constants::c_nShotChangeBHorizontalLToRNoZ == nType)
						return IPP::IPP_ShotChangeHorizontalWithoutZoom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, FALSE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					if(Constants::c_nShotChangeGVerticalBToUNoZ == nType || Constants::c_nShotChangeBVerticalBToUNoZ == nType)
						return IPP::IPP_ShotChangeVerticalWithoutZoom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, TRUE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					if(Constants::c_nShotChangeGVerticalUToBNoZ == nType || Constants::c_nShotChangeBVerticalUToBNoZ == nType)
						return IPP::IPP_ShotChangeVerticalWithoutZoom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, FALSE, pUtility, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					return FALSE;
				}

				BOOL BGRA_SimpleWave(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{					
					int nWideFade = Constants::c_nSmoothWave == nType ? 40 : 0;

					int nRadius = nWidth / 2;
					int nSqrRadius = nRadius * nRadius;
					
					int nAmplit  = int(nRadius * (1 - sqrt( 0.75 )));
					int nMidLine = int(dCompleteness * (nAmplit + nHeight) - nAmplit);
					int nY = nMidLine + nRadius;

					int nX[4] = { 0, nWidth / 2, nWidth, 3 * nWidth / 2};
					for ( int nCounter = 0 ; nCounter < 4; nCounter++)
					{
						nX[nCounter] += int(dCompleteness * nWidth * -2);
						
						while ( nX[nCounter] < nWidth / -2 )
							nX[nCounter] += 2 * nWidth;
						
						while ( nX[nCounter] > 3 * nWidth / 2)
							nX[nCounter] -= 2 * nWidth;
					}
					
					int nIndex = 0;
					for(int nYIndex = 0; nYIndex < nHeight; nYIndex++ )
					{
						for(int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex += 4 )
						{
							double dPatterValue = 0;
							int nCurSector = 0;
							
							for( int nCounter = 0 ; nCounter < 4; nCounter++)
							{
								if ( nXIndex > nX[nCounter & 3] && nXIndex <= nX[ (nCounter + 1 ) & 3])
								{
									nCurSector = nCounter;
									break;
								}
							}

							double dRadius1 = sqrt(double((nXIndex - nX[nCurSector]) * (nXIndex - nX[nCurSector]) + (nYIndex - nY) * (nYIndex - nY)));
							double dRadius2 = sqrt(double((nXIndex - nX[( nCurSector + 1 ) & 3]) * (nXIndex - nX[( nCurSector + 1 ) & 3]) + (nYIndex - nY) * (nYIndex - nY)));

							double dRadius3 = sqrt(double((nXIndex - nX[nCurSector]) * (nXIndex - nX[nCurSector]) + (nYIndex - nY - nWideFade) * (nYIndex - nY - nWideFade)));
							double dRadius4 = sqrt(double((nXIndex - nX[( nCurSector + 1 ) & 3]) * (nXIndex - nX[( nCurSector + 1 ) & 3]) + (nYIndex - nY - nWideFade) * (nYIndex - nY - nWideFade)));
							if ( dRadius1 > nRadius && dRadius2 > nRadius && nYIndex <= nY)
							{
								dPatterValue = 1;
							}
							if ( 0 == dPatterValue )
							{
								if ( dRadius3 > nRadius && dRadius4 > nRadius && nYIndex <= nY + nWideFade)
									dPatterValue = min(dRadius3 - nRadius, dRadius4 - nRadius) / nWideFade;
							}
							
							pBGRAResult[nIndex + 2] = BYTE(pBGRASource1[nIndex + 2] * (1 - dPatterValue) + pBGRASource2[nIndex + 2] * dPatterValue);
							pBGRAResult[nIndex + 1] = BYTE(pBGRASource1[nIndex + 1] * (1 - dPatterValue) + pBGRASource2[nIndex + 1] * dPatterValue);
							pBGRAResult[nIndex + 0] = BYTE(pBGRASource1[nIndex + 0] * (1 - dPatterValue) + pBGRASource2[nIndex + 0] * dPatterValue);
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

				BOOL BGRA_BlackWhiteLines(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if(Constants::c_nBlackWhiteLines == nType)
						return IPP::IPP_BlackWhiteLines(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, FALSE);
					if(Constants::c_nBlackWhiteLinesInverse == nType)
						return IPP::IPP_BlackWhiteLines(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, TRUE);
					return FALSE;
				}

				BOOL BGRA_Eye(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if ( Constants::c_nEye == nType )
						return IPP::IPP_Eye(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, 0);
					else
						return IPP::IPP_Eye(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, 40);
				}

				BOOL BGRA_Heart(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if ( Constants::c_nSmoothHeart == nType )
						return IPP::IPP_Heart(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, 40);
					else
						return IPP::IPP_Heart(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, 0);
				}

				BOOL BGRA_Mask(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CMask* pMask)
				{
					int nFadeWide = 0;
					if ( Constants::c_nPinwheelCurvedSN12ASmooth == nType || Constants::c_nPinwheelPropeller12Smooth == nType ||
						 Constants::c_nPinwheelCurvedSW12ASmooth == nType || Constants::c_nPinwheelCurvedLN12ASmooth == nType)
					{
						nFadeWide = 160;
					}
					else if( Constants::c_nPinwheelCurvedSN7ASmooth == nType || Constants::c_nPinwheelPropeller7Smooth == nType ||
						     Constants::c_nPinwheelCurvedSW7ASmooth == nType || Constants::c_nPinwheelCurvedLN7ASmooth == nType)
					{
						nFadeWide = 80;
					}
					else if ( ( Constants::c_nBowTileSmoothHorizontal <= nType && Constants::c_nZigZagSmoothVertical >= nType) ||
							  Constants::c_nWipeSmoothCircleIn == nType ||
							  ( Constants::c_nWipeSmoothBoxIn <= nType && Constants::c_nWipeSmoothDiamondIn >= nType) ||
							  ( Constants::c_nCircleCornerSmoothLeftTop <= nType && Constants::c_nCircleCornerSmoothRightBottom >= nType )||
							  Constants::c_nPinwheelCurvedSN5ASmooth == nType || Constants::c_nPinwheelPropeller4Smooth == nType ||
						      Constants::c_nPinwheelCurvedSW5ASmooth == nType || Constants::c_nPinwheelCurvedLN5ASmooth == nType)
					{
						nFadeWide = 40;
					}
					else if ( Constants::c_nWipeSmoothCircle <= nType && Constants::c_nWipeSmoothWideKeyHoles >= nType)
					{
						nFadeWide = 30;
					}
					else if ( Constants::c_nWipeSmoothNarrowStar <= nType && Constants::c_nWipeSmoothNarrowFlowers >= nType)
					{
						nFadeWide = 10;
					}
					else if ( Constants::c_nEnhancedShutter == nType )
					{
						nFadeWide = 10;
					}
					else 
					{
						nFadeWide = 0;
					}
					return IPP::IPP_Mask(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nType, *pMask, nFadeWide);
				}
				BOOL BGRA_Pixelate(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					return IPP::IPP_Pixelate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness);
				}

				BOOL BGRA_Roll(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					int nDirection = Constants::c_nRollLeft == nType ? 1 : 0;

					return IPP::IPP_Roll(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nDirection);
				}

				BOOL BGRA_Slide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					int nDirection = 0;

					switch( nType )
					{
					case Constants::c_nSlideUp:    nDirection = 0; break;
					case Constants::c_nSlideRight: nDirection = 1; break;
					case Constants::c_nSlideDown:  nDirection = 2; break;
					case Constants::c_nSlideLeft:  nDirection = 3; break;
					default:
						return FALSE;
					}

					return IPP::IPP_Slide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nDirection);
				}

				BOOL BGRA_SlideCenter2(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					int nDirection = 0;

					switch( nType )
					{
					case Constants::c_nSlideCenterUp:    nDirection = 0; break;
					case Constants::c_nSlideCenterRight: nDirection = 1; break;
					case Constants::c_nSlideCenterDown:  nDirection = 2; break;
					case Constants::c_nSlideCenterLeft:  nDirection = 3; break;
					default: 
						return FALSE;
					}

					return IPP::IPP_SlideCenter( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nDirection );
				}

				BOOL BGRA_Shrink(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					int nDirection = Constants::c_nShrinkInHorizontal == nType ? 1 : 0;

					return IPP::IPP_Shrink(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nDirection);
				}

				BOOL BGRA_Spiral(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					int nDirection = 0;

					switch( nType )
					{
					case Constants::c_nSpiralTopLeft:          nDirection = 0; break;
					case Constants::c_nSpiralTopRight:         nDirection = 1; break;
					case Constants::c_nSpiralBottomRight:      nDirection = 2; break;
					case Constants::c_nSpiralBottomLeft:       nDirection = 3; break;
					case Constants::c_nSpiralCenterIn:         nDirection = 4; break;
					case Constants::c_nSpiralReverseCenterIn:  nDirection = 5; break;
					case Constants::c_nSpiralCenterOut:        nDirection = 6; break;
					case Constants::c_nSpiralReverseCenterOut: nDirection = 7; break;
					default:
						return FALSE;
					}

					return IPP::IPP_Spiral( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, nDirection );
				}

				BOOL BGRA_Mosaic(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// check for patterns
					const BYTE* pPattern = NULL;
					switch( nType )
					{
					case Constants::c_nMosaicSpiralIn:         pPattern = &Constants::c_byMosaicSpiralIn           [0][0]; break;
					case Constants::c_nMosaicSpiralOut:        pPattern = &Constants::c_byMosaicSpiralOut          [0][0]; break;
					case Constants::c_nMosaicStrips:           pPattern = &Constants::c_byMosaicStrips             [0][0]; break;
					case Constants::c_nMosaicDissolve:         pPattern = &Constants::c_byMosaicDissolve           [0][0]; break;
					case Constants::c_nMosaicLeftTop:          pPattern = &Constants::c_byMosaicDiagonalLeftTop    [0][0]; break;
					case Constants::c_nMosaicRightTop:         pPattern = &Constants::c_byMosaicDiagonalRightTop   [0][0]; break;
					case Constants::c_nMosaicRightBottom:      pPattern = &Constants::c_byMosaicDiagonalRightBottom[0][0]; break;
					case Constants::c_nMosaicLeftBottom:       pPattern = &Constants::c_byMosaicDiagonalLeftBottom [0][0]; break;
					case Constants::c_nMosaicWallLeftToRight:  pPattern = &Constants::c_byMosaicWallLeft           [0][0]; break;
					case Constants::c_nMosaicWallRightToLeft:  pPattern = &Constants::c_byMosaicWallRight          [0][0]; break;
					case Constants::c_nMosaicWallTopToBottom:  pPattern = &Constants::c_byMosaicWallTop            [0][0]; break;
					case Constants::c_nMosaicWallBottomToTop:  pPattern = &Constants::c_byMosaicWallBottom         [0][0]; break;
					case Constants::c_nMosaicChessLeftToRight: pPattern = &Constants::c_byMosaicChessLeft          [0][0]; break;
					case Constants::c_nMosaicChessRightToLeft: pPattern = &Constants::c_byMosaicChessRight         [0][0]; break;
					case Constants::c_nMosaicChessTopToBottom: pPattern = &Constants::c_byMosaicChessTop           [0][0]; break;
					case Constants::c_nMosaicChessBottomToTop: pPattern = &Constants::c_byMosaicChessBottom        [0][0]; break;
					case Constants::c_nMosaicClockwise:        pPattern = &Constants::c_byMosaicClockwise          [0][0]; break;
					case Constants::c_nMosaicCounterClockwise: pPattern = &Constants::c_byMosaicCounterClockwise   [0][0]; break;
					case Constants::c_nMosaicRandom:           pPattern = &Constants::c_byMosaicRandom             [0][0]; break;
					}

					if( !pPattern )
						return FALSE;

					// combine two images
					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					double dFactorX = 1.0 / nTileSizeX;
					double dFactorY = 1.0 / nTileSizeY;

					int nLevel = int((8 * 8) * dCompleteness + 0.5);

					int nTileY = 0;
					for( int nY = nHeight; nY > 0; --nY, ++nTileY )
					{
						// compute pattern Y coordinate
						if( nTileY >= nTileSizeY )
							nTileY = 0;
						
						int nPatternY = int(int(nTileY * Constants::c_nPatternSize) * dFactorY);

						int nTileX = 0;
						for( int nX = nWidth; nX > 0; --nX, ++nTileX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( nTileX >= nTileSizeX )
								nTileX = 0;
							
							int nPatternX = int(int(nTileX * Constants::c_nPatternSize) * dFactorX);

							// compute pattern value - the level of transition
							int nPatternValue = pPattern[nPatternY * Constants::c_nPatternSize + nPatternX];

							// combine colors
							pResult[0] = nPatternValue < nLevel ? pSource2[0] : pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_Diffuse(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					const int c_nLimit = 127; // 2^n - 1

					ImageStudio::CRandom oRandom( ::rand() ); // встроенный генератор псевдослучайных чисел

					int nLevelX = int(nTileSizeX * dCompleteness);
					int nLevelY = int(nTileSizeY * dCompleteness);

					if( nLevelX < 1 ) nLevelX = 1;
					if( nLevelY < 1 ) nLevelY = 1;
					
					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					double dFactorX1 = c_nLimit * (dCompleteness - 1) / nLevelX;
					double dFactorX2 = c_nLimit * dCompleteness / (nLevelX - nTileSizeX);
					double dFactorY1 = c_nLimit * (dCompleteness - 1) / nLevelY;
					double dFactorY2 = c_nLimit * dCompleteness / (nLevelY - nTileSizeY);

					// combine two images
					int nPatternY = 0;
					for( int nY = nHeight; nY > 0; --nY, ++nPatternY )
					{
						// compute pattern Y coordinate
						if( nPatternY >= nTileSizeY )
							nPatternY = 0;

						int nPatternX = 0;
						for( int nX = nWidth; nX > 0; --nX, ++nPatternX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( nPatternX >= nTileSizeX )
								nPatternX = 0;

							// compute pattern value - the level of transition
							int nPatternValue = oRandom.Rand() & c_nLimit;

							// compute color value
							switch( nType )
							{
							case Constants::c_nDiffuseLeftToRight:
								if (nPatternX <= nLevelX)
								{
									double dPosLevel = nPatternX * dFactorX1 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource1[0] : pSource2[0];
								}
								else
								{
									double dPosLevel = (nTileSizeX - nPatternX) * dFactorX2 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource2[0] : pSource1[0];
								}
								break;
							
							case Constants::c_nDiffuseRightToLeft:
								if( (nTileSizeX - nPatternX) <= nLevelX)
								{
									double dPosLevel = (nTileSizeX - nPatternX) * dFactorX1 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource1[0] : pSource2[0];
								}

								else
								{
									double dPosLevel = nPatternX * dFactorX2 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource2[0] : pSource1[0];
								}
								break;
							
							case Constants::c_nDiffuseBottomToTop:
								if( nPatternY <= nLevelY )
								{
									double dPosLevel = nPatternY * dFactorY1 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource1[0] : pSource2[0];
								}
								else
								{
									double dPosLevel = (nTileSizeY - nPatternY) * dFactorY2 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource2[0] : pSource1[0];
								}
								break;

							case Constants::c_nDiffuseTopToBottom:
								if( (nTileSizeY - nPatternY) <= nLevelY )
								{
									double dPosLevel = (nTileSizeY - nPatternY) * dFactorY1 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource1[0] : pSource2[0];
								}
								else
								{
									double dPosLevel = nPatternY * dFactorY2 + c_nLimit;

									pResult[0] = nPatternValue > dPosLevel ? pSource2[0] : pSource1[0];
								}
								break;
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_Rotate(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
				int nFirstAngle = -360;
				
				if ( nType == Constants::c_nRotateTwiceOutClockwise )
				{
					nFirstAngle = 720;
					return IPP::IPP_Rotate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, FALSE, nFirstAngle, FALSE);
				}
				
				if ( (nType == Constants::c_nRotateTwiceIn) || (nType == Constants::c_nRotateTwiceOut) || (nType == Constants::c_nRotateTransparentTwiceIn) || (nType == Constants::c_nRotateTransparentTwiceOut))
					nFirstAngle = -720;
				if ( (nType == Constants::c_nRotateHalfIn) || (nType == Constants::c_nRotateHalfOut) || (nType == Constants::c_nRotateTransparentHalfIn) || (nType == Constants::c_nRotateTransparentHalfOut))
					nFirstAngle = -90;
				if ( Constants::c_nRotateSingleIn <= nType && Constants::c_nRotateHalfOut >= nType)
					return IPP::IPP_Rotate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, ((nType == Constants::c_nRotateSingleIn) || (nType == Constants::c_nRotateTwiceIn) || (Constants::c_nRotateHalfIn == nType)), nFirstAngle, FALSE);
				else
					return IPP::IPP_Rotate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, ((nType == Constants::c_nRotateTransparentSingleIn) || (nType == Constants::c_nRotateTransparentTwiceIn) || (Constants::c_nRotateTransparentHalfIn == nType)), nFirstAngle, TRUE);
				}
				BOOL BGRA_WipeDoor(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nLevelX  = int(nTileSizeX * dCompleteness);
					int nLevelY  = int(nTileSizeY * dCompleteness);
					int nLevel1X = int(nTileSizeX * dCompleteness / 2);
					int nLevel1Y = int(nTileSizeY * dCompleteness / 2);
					int nLevel2X = int(nTileSizeX * (1 - dCompleteness) / 2);
					int nLevel2Y = int(nTileSizeY * (1 - dCompleteness) / 2);

					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					// combine two images
					int nPatternY = 0;
					for( int nY = nHeight; nY > 0; --nY, ++nPatternY )
					{
						// compute pattern Y coordinate
						if( nPatternY >= nTileSizeY )
							nPatternY = 0;

						int nPatternX = 0;
						for( int nX = nWidth; nX > 0; --nX, ++nPatternX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( nPatternX >= nTileSizeX )
								nPatternX = 0;

							// compute color value
							switch( nType )
							{
							case Constants::c_nWipeDoorVerticalIn:
								if( (nPatternY <= nLevel1Y) || (nPatternY >= nTileSizeY - nLevel1Y) )
								{
									pResult[0] = pSource2[0];
									continue;
								}
								break;
							case Constants::c_nWipeDoorVerticalOut:
								if( (nPatternY >= nLevel2Y) && (nPatternY <= nTileSizeY - nLevel2Y) )
								{
									pResult[0] = pSource2[0];
									continue;
								}
								break;
							case Constants::c_nWipeDoorHorizontalIn:
								if( (nPatternX <= nLevel1X) || (nPatternX >= nTileSizeX - nLevel1X) )
								{
									pResult[0] = pSource2[0];
									continue;
								}
								break;
							case Constants::c_nWipeDoorHorizontalOut:
								if( (nPatternX >= nLevel2X) && (nPatternX <= nTileSizeX - nLevel2X) )
								{
									pResult[0] = pSource2[0];
									continue;
								}
								break;
							case Constants::c_nWipeDoorDiagonal:
								if( ((nPatternY <= nLevelY) && (nPatternX <= nLevelX)) || 
									((nPatternY >= nTileSizeY - nLevelY) && (nPatternX >= nTileSizeX - nLevelX)) )
								{
									pResult[0] = pSource2[0];
									continue;
								}
								break;
							case Constants::c_nWipeDoorDiagonalRotated:
								if( ((nPatternY >= nTileSizeY - nLevelY) && (nPatternX <= nLevelX)) || 
									((nPatternY <= nLevelY) && (nPatternX >= nTileSizeX - nLevelX)) )
								{
									pResult[0] = pSource2[0];
									continue;
								}
								break;
							}

							pResult[0] = pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_WipeStrips(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					double dCenterX = nTileSizeX / 2.0;
					double dCenterY = nTileSizeY / 2.0;
					double dLevelX  = nTileSizeX*dCompleteness;
					double dLevelY  = nTileSizeY*dCompleteness;
					
					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					// combine two images
					int nPatternY = 0;
					for( int nY = nHeight; nY > 0; --nY, ++nPatternY )
					{
						// compute pattern Y coordinate
						if( nPatternY >= nTileSizeY )
							nPatternY = 0;

						int nPatternX = 0;
						for( int nX = nWidth; nX > 0; --nX, ++nPatternX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( nPatternX >= nTileSizeX )
								nPatternX = 0;

							// compute color value
							if (nType == Constants::c_nWipeStripsVertical)
							{
								if ((nPatternX <  dCenterX && nPatternY < dLevelY) ||
									(nPatternX >= dCenterX && nPatternY > nTileSizeY - dLevelY))
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else //if (nType == Constants::c_nWipeStripsHorizontal)
							{
								if ((nPatternY <  dCenterY && nPatternX < dLevelX) ||
									(nPatternY >= dCenterY && nPatternX > nTileSizeX - dLevelX))
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}

							pResult[0] = pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_WipeSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					
					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					// combine two images
					int nPatternY = -1;
					for( int nY = nHeight; nY > 0; --nY)
					{
						// compute pattern Y coordinate
						if( ++nPatternY >= nTileSizeY )
							nPatternY = 0;

						int nPatternX = -1;
						for( int nX = nWidth; nX > 0; --nX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( ++nPatternX >= nTileSizeX )
								nPatternX = 0;

							if (nType == Constants::c_nWipeSideLeftToRight)
							{
								if (nPatternX <= nLevelX)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeSideRightToLeft)
							{
								if (nPatternX >= (nTileSizeX - 1) - nLevelX)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeSideBottomToTop)
							{
								if (nPatternY <= nLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeSideTopToBottom)
							{
								if (nPatternY >= (nTileSizeY - 1) - nLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCornerLeftTop)
							{
								if (nPatternX <= nLevelX && nPatternY >= (nTileSizeY - 1) - nLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCornerLeftBottom)
							{
								if (nPatternX <= nLevelX && nPatternY <= nLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCornerRightBottom)
							{
								if (nPatternX >= (nTileSizeX - 1) - nLevelX && nPatternY <= nLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCornerRightTop)
							{
								if (nPatternX >= (nTileSizeX - 1) - nLevelX && nPatternY >= (nTileSizeY - 1) - nLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}

							pResult[0] = pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}

				BOOL BGRA_WipeCenter(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					double dCenterX = 0.5*(nTileSizeX - 1);
					double dCenterY = 0.5*(nTileSizeY - 1);

					double dLevelX = 0.5*nTileSizeX*dCompleteness;
					double dLevelY = 0.5*nTileSizeY*dCompleteness;

					double dRadiusMin = 0.25*(nTileSizeX*nTileSizeX + nTileSizeY*nTileSizeY)*dCompleteness;
					double dRadiusMax = 0.25*(nTileSizeX*nTileSizeX + nTileSizeY*nTileSizeY)*(1.0 - dCompleteness);

					double dDiamondMin = 0.5*(nTileSizeX + nTileSizeY)*dCompleteness;
					double dDiamondMax = 0.5*(nTileSizeX + nTileSizeY)*(1.0 - dCompleteness);

					double dCross1X = 0.5*(nTileSizeX - 1)*dCompleteness;
					double dCross1Y = 0.5*(nTileSizeY - 1)*dCompleteness;
					double dCross2X = 0.5*(nTileSizeX - 1)*(1.0 - dCompleteness);
					double dCross2Y = 0.5*(nTileSizeY - 1)*(1.0 - dCompleteness);
					
					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					// combine two images
					int nPatternY = -1;
					for( int nY = nHeight; nY > 0; --nY )
					{
						// compute pattern Y coordinate
						if( ++nPatternY >= nTileSizeY )
							nPatternY = 0;

						int nPatternX = -1;
						for( int nX = nWidth; nX > 0; --nX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( ++nPatternX >= nTileSizeX )
								nPatternX = 0;

							// compute color value
							if (nType == Constants::c_nWipeCenterBoxIn)
							{
								if (nPatternX < dLevelX || nPatternX > (nTileSizeX - 1) - dLevelX || nPatternY < dLevelY || nPatternY > (nTileSizeY - 1) - dLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCenterBoxOut)
							{
								if (nPatternX > dCenterX - dLevelX && nPatternX < dCenterX + dLevelX && 
									nPatternY > dCenterY - dLevelY && nPatternY < dCenterY + dLevelY)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCenterCircleIn)
							{
								double dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
								if (dRadius > dRadiusMax)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCenterCircleOut)
							{
								double dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
								if (dRadius < dRadiusMin)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCenterDiamondIn)
							{
								double dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
								if (dDiamond > dDiamondMax)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCenterDiamondOut)
							{
								double dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
								if (dDiamond < dDiamondMin)
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCenterCrossIn)
							{
								if ((nPatternY <= dCross1Y && nPatternX <= dCross1X) ||
									(nPatternY <= dCross1Y && nPatternX >= (nTileSizeX - 1) - dCross1X) ||
									(nPatternY >= (nTileSizeY - 1) - dCross1Y && nPatternX >= (nTileSizeX - 1) - dCross1X) ||
									(nPatternY >= (nTileSizeY - 1) - dCross1Y && nPatternX <= dCross1X))
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCenterCrossOut)
							{
								if ((nPatternY >= dCross2Y && nPatternY <= (nTileSizeY - 1) - dCross2Y) ||
									(nPatternX >= dCross2X && nPatternX <= (nTileSizeX - 1) - dCross2X))
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}

							pResult[0] = pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_WipeChecker(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					
					int nHalfTileX = (nTileSizeX - 1) / 2;
					int nHalfTileY = (nTileSizeY - 1) / 2;

					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					// combine two images
					int nPatternY = -1;
					for( int nY = nHeight; nY > 0; --nY )
					{
						// compute pattern Y coordinate
						if( ++nPatternY >= nTileSizeY )
							nPatternY = 0;

						int nPatternX = -1;
						for( int nX = nWidth; nX > 0; --nX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( ++nPatternX >= nTileSizeX )
								nPatternX = 0;

							// compute color value
							if (nType == Constants::c_nWipeCheckerLeftToRight)
							{
								int nTemp = nPatternX;
								if( nPatternY >= nHalfTileY )
									nTemp += (nTemp <= nHalfTileX) ? nHalfTileX : -nHalfTileX;
								
								if( nTemp <= nLevelX  )
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCheckerRightToLeft)
							{
								int nTemp = nPatternX;
								if( nPatternY >= nHalfTileY )
									nTemp += (nTemp <= nHalfTileX) ? nHalfTileX : -nHalfTileX;
								
								if( nTemp >= (nTileSizeX - 1) - nLevelX )
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCheckerBottomToTop)
							{
								int nTemp = nPatternY;
								if( nPatternX >= nHalfTileX )
									nTemp += (nTemp <= nHalfTileY) ? nHalfTileY : -nHalfTileY;

								if( nTemp <= nLevelY )
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
							else if (nType == Constants::c_nWipeCheckerTopToBottom)
							{
								int nTemp = nPatternY;
								if( nPatternX >= nHalfTileX )
									nTemp += (nTemp <= nHalfTileY) ? nHalfTileY : -nHalfTileY;

								if( nTemp >= (nTileSizeY - 1) - nLevelY )
								{
									pResult[0] = pSource2[0];
									continue;
								}
							}
					
							pResult[0] = pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_SlideDoor(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// compute tile size
					int nTile = 8;

					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					int nLevel1X = (int)( 0.5*(nTileSizeX - 1)*dCompleteness );
					int nLevel1Y = (int)( 0.5*(nTileSizeY - 1)*dCompleteness );
					int nLevel2X = (int)( 0.5*(nTileSizeX - 1)*(1.0 - dCompleteness) );
					int nLevel2Y = (int)( 0.5*(nTileSizeY - 1)*(1.0 - dCompleteness) );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nSlideDoorVerticalIn)
							{
								if (nPatternY <= nLevel1Y)
								{
									nSourceY = nY + (nTileSizeY - 1)/2 - nLevel1Y;
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevel1Y)
								{
									nSourceY = nY + nLevel1Y - (nTileSizeY - 1)/2;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideDoorVerticalOut)
							{
								if (nPatternY >= nLevel2Y && nPatternY <= (nTileSizeY - 1)/2)
								{
									nSourceY = nY - nLevel2Y;
									nPatternValue = 1;
								}
								else if (nPatternY <= (nTileSizeY - 1) - nLevel2Y && nPatternY >= (nTileSizeY - 1)/2)
								{
									nSourceY = nY + nLevel2Y;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideDoorHorizontalIn)
							{
								if (nPatternX <= nLevel1X)
								{
									nSourceX = nX + (nTileSizeX - 1)/2 - nLevel1X;
									nPatternValue = 1;
								}
								else if (nPatternX >= (nTileSizeX - 1) - nLevel1X)
								{
									nSourceX = nX + nLevel1X - (nTileSizeX - 1)/2;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideDoorHorizontalOut)
							{
								if (nPatternX >= nLevel2X && nPatternX <= (nTileSizeX - 1)/2)
								{
									nSourceX = nX - nLevel2X;
									nPatternValue = 1;
								}
								else if (nPatternX <= (nTileSizeX - 1) - nLevel2X && nPatternX >= (nTileSizeX - 1)/2)
								{
									nSourceX = nX + nLevel2X;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideDoorDiagonal)
							{
								if (nPatternY <= nLevelY && nPatternX <= nLevelX && nPatternY >= (nTileSizeY - 1) - nLevelY && nPatternX >= (nTileSizeX - 1) - nLevelX)
								{
									if ( (nX/nTile % 2 && !(nY/nTile % 2)) || (!(nX/nTile % 2) && nY/nTile % 2) )
									{
										nSourceY = nY + (nTileSizeY - 1) - nLevelY;
										nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									}
									else
									{
										nSourceY = nY + nLevelY - (nTileSizeY - 1);
										nSourceX = nX + nLevelX - (nTileSizeX - 1);
									}
									
									nPatternValue = 1;
								}
								else if (nPatternY <= nLevelY && nPatternX <= nLevelX)
								{
									nSourceY = nY + (nTileSizeY - 1) - nLevelY;
									nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevelY && nPatternX >= (nTileSizeX - 1) - nLevelX)
								{
									nSourceY = nY + nLevelY - (nTileSizeY - 1);
									nSourceX = nX + nLevelX - (nTileSizeX - 1);
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideDoorDiagonalRotated)
							{
								if (nPatternY >= (nTileSizeY - 1) - nLevelY && nPatternX <= nLevelX && nPatternY <= nLevelY && nPatternX >= (nTileSizeX - 1) - nLevelX)
								{
									if ( (nX/nTile % 2 && !(nY/nTile % 2)) || (!(nX/nTile % 2) && nY/nTile % 2) )
									{
										nSourceY = nY + nLevelY - (nTileSizeY - 1);
										nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									}
									else
									{
										nSourceX = nX + nLevelX - (nTileSizeX - 1);
										nSourceY = nY + (nTileSizeY - 1) - nLevelY;
									}
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevelY && nPatternX <= nLevelX)
								{
									nSourceY = nY + nLevelY - (nTileSizeY - 1);
									nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									nPatternValue = 1;
								}
								else if (nPatternY <= nLevelY && nPatternX >= (nTileSizeX - 1) - nLevelX)
								{
									nSourceX = nX + nLevelX - (nTileSizeX - 1);
									nSourceY = nY + (nTileSizeY - 1) - nLevelY;
									nPatternValue = 1;
								}
							}

							// combine colors
							if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);

								pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_SlideStrips(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nCenterX = (nTileSizeX - 1)/2;
					int nCenterY = (nTileSizeY - 1)/2;
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nSlideStripsVertical)
							{
								if (nPatternX <= nCenterX)
								{
									if (nPatternY <= nLevelY)
									{
										nSourceY = nY + (nTileSizeY - 1) - nLevelY;
										nPatternValue = 1;
									}
								}
								else
								{
									if (nPatternY >= (nTileSizeY - 1) - nLevelY)
									{
										nSourceY = nY + nLevelY - (nTileSizeY - 1);
										nPatternValue = 1;
									}
								}
							}
							else if (nType == Constants::c_nSlideStripsHorizontal)
							{
								if (nPatternY <= nCenterY)
								{
									if (nPatternX <= nLevelX)
									{
										nSourceX = nX + (nTileSizeX - 1) - nLevelX;
										nPatternValue = 1;
									}
								}
								else
								{
									if (nPatternX >= (nTileSizeX - 1) - nLevelX)
									{
										nSourceX = nX + nLevelX - (nTileSizeX - 1);
										nPatternValue = 1;
									}
								}
							}

							// combine colors
							if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);

								pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
							}
						}
					}

					// all ok
					return TRUE;
				}

				BOOL BGRA_SlideSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nSlideSideLeftToRight)
							{
								if (nPatternX <= nLevelX)
								{
									nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideSideRightToLeft)
							{
								if (nPatternX >= (nTileSizeX - 1) - nLevelX)
								{
									nSourceX = nX + nLevelX - (nTileSizeX - 1);
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideSideBottomToTop)
							{
								if (nPatternY <= nLevelY)
								{
									nSourceY = nY + (nTileSizeY - 1) - nLevelY;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideSideTopToBottom)
							{
								if (nPatternY >= (nTileSizeY - 1) - nLevelY)
								{
									nSourceY = nY + nLevelY - (nTileSizeY - 1);
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideCornerLeftTop)
							{
								if (nPatternX <= nLevelX && nPatternY >= (nTileSizeY - 1) - nLevelY)
								{
									nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									nSourceY = nY + nLevelY - (nTileSizeY - 1);
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideCornerLeftBottom)
							{
								if (nPatternX <= nLevelX && nPatternY <= nLevelY)
								{
									nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									nSourceY = nY + (nTileSizeY - 1) - nLevelY;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideCornerRightBottom)
							{
								if (nPatternX >= (nTileSizeX - 1) - nLevelX && nPatternY <= nLevelY)
								{
									nSourceX = nX + nLevelX - (nTileSizeX - 1);
									nSourceY = nY + (nTileSizeY - 1) - nLevelY;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideCornerRightTop)
							{
								if (nPatternX >= (nTileSizeX - 1) - nLevelX && nPatternY >= (nTileSizeY - 1) - nLevelY)
								{
									nSourceX = nX + nLevelX - (nTileSizeX - 1);
									nSourceY = nY + nLevelY - (nTileSizeY - 1);
									nPatternValue = 1;
								}
							}

							// combine colors
							if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);

								pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_SlideCenter(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nLevel1X = (int)( 0.5*(nTileSizeX - 1)*dCompleteness );
					int nLevel1Y = (int)( 0.5*(nTileSizeY - 1)*dCompleteness );
					int nLevel2X = (int)( 0.5*(nTileSizeX - 1)*(1.0 - dCompleteness) );
					int nLevel2Y = (int)( 0.5*(nTileSizeY - 1)*(1.0 - dCompleteness) );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nSlideCenterCrossIn)
							{
								if (nPatternY <= nLevel1Y && nPatternX <= nLevel1X)
								{
									nSourceX = nX + (nTileSizeX - 1)/2 - nLevel1X;
									nSourceY = nY + (nTileSizeY - 1)/2 - nLevel1Y;
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevel1Y && nPatternX <= nLevel1X)
								{
									nSourceX = nX + (nTileSizeX - 1)/2 - nLevel1X;
									nSourceY = nY + nLevel1Y - (nTileSizeY - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevel1Y && nPatternX >= (nTileSizeX - 1) - nLevel1X)
								{
									nSourceX = nX + nLevel1X - (nTileSizeX - 1)/2;
									nSourceY = nY + nLevel1Y - (nTileSizeY - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternY <= nLevel1Y && nPatternX >= (nTileSizeX - 1) - nLevel1X)
								{
									nSourceX = nX + nLevel1X - (nTileSizeX - 1)/2;
									nSourceY = nY + (nTileSizeY - 1)/2 - nLevel1Y;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideCenterCrossOut)
							{
								if (nPatternY <= nLevel2Y && nPatternX <= nLevel2X)
								{
									nSourceX = nX + (nTileSizeX - 1)/2 - nLevel2X;
									nSourceY = nY + (nTileSizeY - 1)/2 - nLevel2Y;
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevel2Y && nPatternX <= nLevel2X)
								{
									nSourceX = nX + (nTileSizeX - 1)/2 - nLevel2X;
									nSourceY = nY + nLevel2Y - (nTileSizeY - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevel2Y && nPatternX >= (nTileSizeX - 1) - nLevel2X)
								{
									nSourceX = nX + nLevel2X - (nTileSizeX - 1)/2;
									nSourceY = nY + nLevel2Y - (nTileSizeY - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternY <= nLevel2Y && nPatternX >= (nTileSizeX - 1) - nLevel2X)
								{
									nSourceX = nX + nLevel2X - (nTileSizeX - 1)/2;
									nSourceY = nY + (nTileSizeY - 1)/2 - nLevel2Y;
									nPatternValue = 1;
								}
							}

							// combine colors
							if (nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);

								if ((nType == Constants::c_nSlideCenterCrossIn && nPatternValue == 1) || 
									(nType == Constants::c_nSlideCenterCrossOut && nPatternValue == 0))
								{
									pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
								}
								else if ((nType == Constants::c_nSlideCenterCrossIn && nPatternValue == 0) || 
										 (nType == Constants::c_nSlideCenterCrossOut && nPatternValue == 1))
								{
									pBGRAResult[nIndex + 3] = pBGRASource1[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource1[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource1[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource1[nSourceIndex + 0];
								}
							}
						}
					}

					// all ok
					return TRUE;
				}

				BOOL BGRA_PushDoor(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// compute tile size
					int nTile = 8;

					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					int nLevel1X = (int)( 0.5*(nTileSizeX - 1)*dCompleteness );
					int nLevel1Y = (int)( 0.5*(nTileSizeY - 1)*dCompleteness );
					int nLevel2X = (int)( 0.5*(nTileSizeX - 1)*(1.0 - dCompleteness) );
					int nLevel2Y = (int)( 0.5*(nTileSizeY - 1)*(1.0 - dCompleteness) );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nPushDoorVerticalIn)
							{
								if (nPatternY <= nLevel1Y)
								{
									nSourceY = nY + (nTileSizeY - 1)/2 - nLevel1Y;
									nPatternValue = 1;
								}
								else if (nPatternY <= (nTileSizeY - 1)/2)
								{
									nSourceY = nY - nLevel1Y;
									nPatternValue = 0;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevel1Y)
								{
									nSourceY = nY + nLevel1Y - (nTileSizeY - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternY >= (nTileSizeY - 1)/2)
								{
									nSourceY = nY + nLevel1Y;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nPushDoorVerticalOut)
							{
								if (nPatternY <= nLevel2Y)
								{
									nSourceY = nY + (nTileSizeY - 1)/2 - nLevel2Y;
									nPatternValue = 0;
								}
								else if (nPatternY >= (nTileSizeY - 1) - nLevel2Y)
								{
									nSourceY = nY + nLevel2Y - (nTileSizeY - 1)/2;
									nPatternValue = 0;
								}
								else if (nPatternY >= nLevel2Y && nPatternY <= (nTileSizeY - 1)/2)
								{
									nSourceY = nY - nLevel2Y;
									nPatternValue = 1;
								}
								else if (nPatternY <= (nTileSizeY - 1) - nLevel2Y && nPatternY >= (nTileSizeY - 1)/2)
								{
									nSourceY = nY + nLevel2Y;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nPushDoorHorizontalIn)
							{
								if (nPatternX <= nLevel1X)
								{
									nSourceX = nX + (nTileSizeX - 1)/2 - nLevel1X;
									nPatternValue = 1;
								}
								else if (nPatternX <= (nTileSizeX - 1)/2)
								{
									nSourceX = nX - nLevel1X;
									nPatternValue = 0;
								}
								else if (nPatternX >= (nTileSizeX - 1) - nLevel1X)
								{
									nSourceX = nX + nLevel1X - (nTileSizeX - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternX >= (nTileSizeX - 1)/2)
								{
									nSourceX = nX + nLevel1X;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nPushDoorHorizontalOut)
							{
								if (nPatternX <= nLevel2X)
								{
									nSourceX = nX + (nTileSizeX - 1)/2 - nLevel2X;
									nPatternValue = 0;
								}
								else if (nPatternX >= (nTileSizeX - 1) - nLevel2X)
								{
									nSourceX = nX + nLevel2X - (nTileSizeX - 1)/2;
									nPatternValue = 0;
								}
								else if (nPatternX >= nLevel2X && nPatternX <= (nTileSizeX - 1)/2)
								{
									nSourceX = nX - nLevel2X;
									nPatternValue = 1;
								}
								else if (nPatternX <= (nTileSizeX - 1) - nLevel2X && nPatternX >= (nTileSizeX - 1)/2)
								{
									nSourceX = nX + nLevel2X;
									nPatternValue = 1;
								}
							}

							// combine colors
							if (nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);

								if (nPatternValue == 0)
								{
									pBGRAResult[nIndex + 3] = pBGRASource1[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource1[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource1[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource1[nSourceIndex + 0];
								}
								else
								{
									pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
								}
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_PushStrips(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nCenterX = (nTileSizeX - 1)/2;
					int nCenterY = (nTileSizeY - 1)/2;
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nPushStripsVertical)
							{
								if (nPatternX <= nCenterX)
								{
									if (nPatternY <= nLevelY)
									{
										nSourceY = nY + (nTileSizeY - 1) - nLevelY;
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
									if (nPatternY >= (nTileSizeY - 1) - nLevelY)
									{
										nSourceY = nY + nLevelY - (nTileSizeY - 1);
										nPatternValue = 1;
									}
									else
									{
										nSourceY = nY + nLevelY;
										nPatternValue = 0;
									}
								}
							}
							else if (nType == Constants::c_nPushStripsHorizontal)
							{
								if (nPatternY <= nCenterY)
								{
									if (nPatternX <= nLevelX)
									{
										nSourceX = nX + (nTileSizeX - 1) - nLevelX;
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
									if (nPatternX >= (nTileSizeX - 1) - nLevelX)
									{
										nSourceX = nX + nLevelX - (nTileSizeX - 1);
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
							if (nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);

								if (nPatternValue == 0)
								{
									pBGRAResult[nIndex + 3] = pBGRASource1[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource1[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource1[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource1[nSourceIndex + 0];
								}
								else
								{
									pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
								}
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_PushSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nPushSideLeftToRight)
							{
								if (nPatternX <= nLevelX)
								{
									nSourceX = nX + (nTileSizeX - 1) - nLevelX;
									nPatternValue = 1;
								}
								else
								{
									nSourceX = nX - nLevelX;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nPushSideRightToLeft)
							{
								if (nPatternX >= (nTileSizeX - 1) - nLevelX)
								{
									nSourceX = nX + nLevelX - (nTileSizeX - 1);
									nPatternValue = 1;
								}
								else
								{
									nSourceX = nX + nLevelX;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nPushSideBottomToTop)
							{
								if (nPatternY <= nLevelY)
								{
									nSourceY = nY + (nTileSizeY - 1) - nLevelY;
									nPatternValue = 1;
								}
								else
								{
									nSourceY = nY - nLevelY;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nPushSideTopToBottom)
							{
								if (nPatternY >= (nTileSizeY - 1) - nLevelY)
								{
									nSourceY = nY + nLevelY - (nTileSizeY - 1);
									nPatternValue = 1;
								}
								else
								{
									nSourceY = nY + nLevelY;
									nPatternValue = 0;
								}
							}

							// combine colors
							if (nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);

								if (nPatternValue == 0)
								{
									pBGRAResult[nIndex + 3] = pBGRASource1[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource1[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource1[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource1[nSourceIndex + 0];
								}
								else
								{
									pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
								}
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_StretchDoor(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nStretchDoorVerticalIn || nType == Constants::c_nStretchDoorVerticalOut)
						return IPP::IPP_StretchDoorVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nStretchDoorVerticalIn));

					if (nType == Constants::c_nStretchDoorHorizontalIn || nType == Constants::c_nStretchDoorHorizontalOut)
						return IPP::IPP_StretchDoorHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nStretchDoorHorizontalIn));

					// all ok
					return TRUE;
				}
				BOOL BGRA_StretchStrips(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nStretchStripsVertical)
						return IPP::IPP_StretchStripsVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);

					if (nType == Constants::c_nStretchStripsHorizontal)
						return IPP::IPP_StretchStripsHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);

					// all ok
					return TRUE;
				}
				BOOL BGRA_StretchSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nStretchSideLeftToRight)
						return IPP::IPP_StretchSideHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, TRUE);

					if (nType == Constants::c_nStretchSideRightToLeft)
						return IPP::IPP_StretchSideHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, FALSE);

					if (nType == Constants::c_nStretchSideTopToBottom)
						return IPP::IPP_StretchSideVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, TRUE);

					if (nType == Constants::c_nStretchSideBottomToTop)
						return IPP::IPP_StretchSideVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, FALSE);

					if (nType == Constants::c_nStretchCornerLeftTop)
						return IPP::IPP_StretchSideLTToRB(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);

					if (nType == Constants::c_nStretchCornerLeftBottom)
						return IPP::IPP_StretchSideLBToRT(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);

					if (nType == Constants::c_nStretchCornerRightBottom)
						return IPP::IPP_StretchSideRBToLT(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);

					if (nType == Constants::c_nStretchCornerRightTop)
						return IPP::IPP_StretchSideRTToLB(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);
					
					// all ok
					return TRUE;
				}
				BOOL BGRA_StretchCenter(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					return IPP::IPP_StretchCenter(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nStretchCenterBoxIn));

					// all ok
					return TRUE;
				}
				BOOL BGRA_StretchWipeSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nStretchWipeSideLeftToRightIn || 
						nType == Constants::c_nStretchWipeSideLeftToRightOut || 
						nType == Constants::c_nStretchWipeSideRightToLeftIn || 
						nType == Constants::c_nStretchWipeSideRightToLeftOut)

						return IPP::IPP_StretchWipeSideHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, 
							((nType == Constants::c_nStretchWipeSideLeftToRightIn) || (nType == Constants::c_nStretchWipeSideRightToLeftIn)),
							((nType == Constants::c_nStretchWipeSideLeftToRightIn) || (nType == Constants::c_nStretchWipeSideLeftToRightOut)));

					if (nType == Constants::c_nStretchWipeSideTopToBottomIn  ||
						nType == Constants::c_nStretchWipeSideTopToBottomOut ||
						nType == Constants::c_nStretchWipeSideBottomToTopIn ||
						nType == Constants::c_nStretchWipeSideBottomToTopOut)

						return IPP::IPP_StretchWipeSideVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, 
							((nType == Constants::c_nStretchWipeSideBottomToTopIn) || (nType == Constants::c_nStretchWipeSideTopToBottomIn)),
							((nType == Constants::c_nStretchWipeSideTopToBottomIn) || (nType == Constants::c_nStretchWipeSideTopToBottomOut)));

					// all ok
					return TRUE;
				}

				BOOL BGRA_SwingSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nSwingSideTopOut || 
						nType == Constants::c_nSwingSideBottomOut || 
						nType == Constants::c_nSwingSideTopIn || 
						nType == Constants::c_nSwingSideBottomIn)
						return IPP::IPP_SwingSideHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, 
							(nType == Constants::c_nSwingSideTopOut || nType == Constants::c_nSwingSideTopIn), 
							(nType == Constants::c_nSwingSideTopIn || nType == Constants::c_nSwingSideBottomIn));

					if (nType == Constants::c_nSwingSideLeftOut || 
						nType == Constants::c_nSwingSideLeftIn || 
						nType == Constants::c_nSwingSideRightOut || 
						nType == Constants::c_nSwingSideRightIn)
						return IPP::IPP_SwingSideVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, 
							(nType == Constants::c_nSwingSideLeftIn || nType == Constants::c_nSwingSideLeftOut), 
							(nType == Constants::c_nSwingSideLeftIn || nType == Constants::c_nSwingSideRightIn));

					// all ok
					return TRUE;

				}
				BOOL BGRA_SwingSideChange(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nSwingSideChangeBottom|| 
						nType == Constants::c_nSwingSideChangeTop)
						return IPP::IPP_SwingSideChangeHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nSwingSideChangeTop));

					if (nType == Constants::c_nSwingSideChangeRight|| 
						nType == Constants::c_nSwingSideChangeLeft)
						return IPP::IPP_SwingSideChangeVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nSwingSideChangeLeft));

					// all ok
					return TRUE;
				}
				BOOL BGRA_SwingDoors(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nSwingDoorsVerticalOut || 
						nType == Constants::c_nSwingDoorsVerticalIn)
						return IPP::IPP_SwingDoorsVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nSwingDoorsVerticalIn));

					if (nType == Constants::c_nSwingDoorsHorizontalOut || 
						nType == Constants::c_nSwingDoorsHorizontalIn)
						return IPP::IPP_SwingDoorsHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nSwingDoorsHorizontalIn));

					// all ok
					return TRUE;

				}
				BOOL BGRA_SwingDoorsChange(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nSwingDoorsChangeVertical)
						return IPP::IPP_SwingDoorsChangeVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);

					if (nType == Constants::c_nSwingDoorsChangeHorizontal)
						return IPP::IPP_SwingDoorsChangeHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness);

					// all ok
					return TRUE;
				}

				BOOL BGRA_PageTurnSimple(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
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
							if (nType == Constants::c_nPageTurnLeftBottom)
							{
								if ( nPatternX + nPatternY <= nLevel ) 
									nPatternValue = 1;
								else if ( nPatternX <= nLevel && nPatternY <= nLevel)
								{
									nSourceX = nTileX * nTileSizeX + nLevel - nPatternY;
									nSourceY = nTileY * nTileSizeY + nLevel - nPatternX;
								}
							}
							else if (nType == Constants::c_nPageTurnLeftTop)
							{
								if ( nPatternY - nPatternX >= nTileSizeY - 1 - nLevel ) 
									nPatternValue = 1;
								else if ( nPatternX <= nLevel && nPatternY >= nTileSizeY - 1 - nLevel)
								{
									nSourceX = nTileX * nTileSizeX + nPatternY - (nTileSizeY - 1 - nLevel);
									nSourceY = nTileY * nTileSizeY + nPatternX + (nTileSizeY - 1 - nLevel);
								}
							}
							else if (nType == Constants::c_nPageTurnRightBottom)
							{
								if ( nPatternX - nPatternY >= nTileSizeX - 1 - nLevel ) 
									nPatternValue = 1;
								else if ( nPatternX >= nTileSizeX - 1 - nLevel && nPatternY <= nLevel)
								{
									nSourceX = nTileX * nTileSizeX + nPatternY + (nTileSizeX - 1 - nLevel);
									nSourceY = nTileY * nTileSizeY + nPatternX - (nTileSizeX - 1 - nLevel);
								}
							}
							else if (nType == Constants::c_nPageTurnRightTop)
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
								pBGRAResult[nIndex + 3] = pBGRASource2[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
							}
							else
							{
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);
								
								pBGRAResult[nIndex + 3] = pBGRASource1[nSourceIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nSourceIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nSourceIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nSourceIndex + 0];
							}
						}
					}

					return TRUE;
				}
				BOOL BGRA_PageTurn(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					//int nLevelY = (int)((nTileSizeY - 1)*dCompleteness );
					//int nLevelX = (int)((nTileSizeX - 1)*dCompleteness );

					//double  quadr[8];
					//ImageStudio::Transforms::NonLinearTransforms::BilinearTransformation*   trns = NULL;
					//if (nType == Constants::c_nPageTurnLeftTop)
					//{
					//	quadr[0] = 0;
					//	quadr[1] = nHeight-nLevelY;
					//	quadr[2] = nLevelX;
					//	quadr[3] = nHeight-nLevelY;
					//	quadr[4] = nLevelX;
					//	quadr[5] = nHeight;
					//	quadr[6] = 5*nLevelX/6;
					//	quadr[7] = nHeight-5*nLevelY/6;
					//	
					//	trns = new ImageStudio::Transforms::NonLinearTransforms::BilinearTransformation(0,nHeight-nLevelY, 
					//		nLevelX, nHeight,quadr);
					//}else
					//	if (nType == Constants::c_nPageTurnLeftBottom)
					//	{
					//		quadr[0] = 5*nLevelX/6;
					//		quadr[1] = 5*nLevelY/6;
					//		quadr[2] = nLevelX;
					//		quadr[3] = 0;
					//		quadr[4] = nLevelX;
					//		quadr[5] = nLevelY;
					//		quadr[6] = 0;
					//		quadr[7] = nLevelY;

					//		trns = new ImageStudio::Transforms::NonLinearTransforms::BilinearTransformation(0,0, 
					//			nLevelX, nLevelY,quadr);
					//	}else
					//		if (nType == Constants::c_nPageTurnRightTop)
					//		{
					//			quadr[0] = nWidth-nLevelX;
					//			quadr[1] = nHeight-nLevelY;
					//			quadr[2] = nWidth;
					//			quadr[3] = nHeight-nLevelY;
					//			quadr[4] = nWidth-5*nLevelX/6;
					//			quadr[5] = nHeight-5*nLevelY/6;
					//			quadr[6] = nWidth-nLevelX;
					//			quadr[7] = nHeight;
					//			trns = new ImageStudio::Transforms::NonLinearTransforms::BilinearTransformation(nWidth-nLevelX,nHeight-nLevelY, 
					//				nWidth, nHeight,quadr);
					//		}else
					//			if (nType == Constants::c_nPageTurnRightBottom)
					//			{
					//				quadr[0] = nWidth-nLevelX;
					//				quadr[1] = 0;
					//				quadr[2] = nWidth-5*nLevelX/6;
					//				quadr[3] = 5*nLevelY/6;
					//				quadr[4] = nWidth;
					//				quadr[5] = nLevelY;
					//				quadr[6] = nWidth-nLevelX;
					//				quadr[7] = nLevelY;
					//				trns = new ImageStudio::Transforms::NonLinearTransforms::BilinearTransformation(nWidth-nLevelX,0, 
					//					nWidth, nLevelY,quadr);
					//			}

					//	memcpy(pBGRAResult,pBGRASource2,4*nWidth*nHeight);

					//	if (!trns || !trns->is_valid())
					//	{
					//		if (trns)
					//			delete trns;
					//		return TRUE;
					//	}
					//	// combine two images
					//	for (int nY = 0; nY < nHeight; ++nY)
					//	{
					//		for (int nX = 0; nX < nWidth; ++nX)
					//		{
					//			BYTE tmpR = pBGRASource1[0 + 4*(nX + nY*nWidth)];
					//			BYTE tmpG = pBGRASource1[1 + 4*(nX + nY*nWidth)];
					//			BYTE tmpB = pBGRASource1[2 + 4*(nX + nY*nWidth)];

					//			double pX = nX;
					//			double pY = nY;
					//			if (nType == Constants::c_nPageTurnRightTop && nX>(nWidth-nLevelX) && nY>(nHeight-nLevelY))
					//				trns->transform(&pX, &pY);
					//			if (nType == Constants::c_nPageTurnLeftTop && nX<nLevelX && nY>(nHeight-nLevelY))
					//				trns->transform(&pX, &pY);
					//			if (nType == Constants::c_nPageTurnRightBottom && nX>(nWidth-nLevelX) && nY<nLevelY)
					//				trns->transform(&pX, &pY);
					//			if (nType == Constants::c_nPageTurnLeftBottom && nX<nLevelX && nY<nLevelY)
					//				trns->transform(&pX, &pY);
					//			int    iX = (int)pX;
					//			int    iY = (int)pY;

					//			if (iX<0 || iY<0 || iX >nWidth || iY>nHeight)
					//				continue;


					//			pBGRAResult[2 + 4*(iX + iY*nWidth)] = tmpB;
					//			pBGRAResult[1 + 4*(iX + iY*nWidth)] = tmpG;
					//			pBGRAResult[0 + 4*(iX + iY*nWidth)] = tmpR;
					//		}
					//	}
					//	delete trns;

					::memcpy( pBGRAResult, pBGRASource2, nWidth * nHeight * 4 );

					CBuffer<Ipp32f> oMapX( nWidth * nHeight );
					CBuffer<Ipp32f> oMapY( nWidth * nHeight );

					if( oMapX.IsEmpty() || oMapY.IsEmpty() )
						return FALSE;

					// угол наклона изгиба относительно оси X
					const double dsin = sin(IPP_PI180 * 30);
					const double dcos = cos(IPP_PI180 * 30);

					// основные константы
					const double omega    = IPP_PI2 + 40 * IPP_PI180;
					const double radius   = nWidth / 3.0;
					const double factor   = 1.0 / radius;
					const double distance = dCompleteness * (radius + sqrt( double(nWidth * nWidth + nHeight * nHeight)));
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
					case Constants::c_nPageTurnLeftTop3d:     nInc =  1; nStep = -2 * nWidth; nDist = nWidth * (nHeight - 1); nDX = 0;          nDY = nHeight - 1; nFX =  1; nFY = -1; break;
					case Constants::c_nPageTurnLeftBottom3d:  nInc =  1; nStep =  0;          nDist = 0;                      nDX = 0;          nDY = 0;           nFX =  1; nFY =  1; break;
					case Constants::c_nPageTurnRightBottom3d: nInc = -1; nStep =  2 * nWidth; nDist = nWidth - 1;             nDX = nWidth - 1; nDY = 0;           nFX = -1; nFY =  1; break;
					case Constants::c_nPageTurnRightTop3d:    nInc = -1; nStep =  0;          nDist = nWidth *  nHeight - 1;  nDX = nWidth - 1; nDY = nHeight - 1; nFX = -1; nFY = -1; break;
					}

					Ipp32f* pMapX = oMapX.GetPtr() + nDist;
					Ipp32f* pMapY = oMapY.GetPtr() + nDist;

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

					ippiRemap_8u_C4R( pBGRASource1, SrcSize, 4*nWidth, SrcRect, 
						oMapX.GetPtr(), sizeof(Ipp32f)*nWidth, 
						oMapY.GetPtr(), sizeof(Ipp32f)*nWidth, 
						pBGRAResult, 4*nWidth, SrcSize, IPPI_INTER_LINEAR);


					return TRUE;
				}

				BOOL BGRA_PageTurnWhiteBack(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					BOOL bWhiteBack;
					if ( Constants::c_nPageTurnLeftTopWhiteBack3d <= nType && Constants::c_nPageTurnRightTopWhiteBack3d >= nType )
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

					if ( Constants::c_nPageTurnLeftTopWhiteBack3d == nType || Constants::c_nPageTurnLeftTop3d == nType )
					{
						nInc =  1; nStep = -2 * nWidth; nDist = nWidth * (nHeight - 1); nDX = 0;          nDY = nHeight - 1; nFX =  1; nFY = -1; nTypeNoBack = Constants::c_nPageTurnLeftTop3d;
					}
					else if ( Constants::c_nPageTurnLeftBottomWhiteBack3d == nType || Constants::c_nPageTurnLeftBottom3d == nType)
					{
						nInc =  1; nStep =  0;          nDist = 0;                      nDX = 0;          nDY = 0;           nFX =  1; nFY =  1; nTypeNoBack = Constants::c_nPageTurnLeftBottom3d;
					}
					else if ( Constants::c_nPageTurnRightBottomWhiteBack3d == nType || Constants::c_nPageTurnRightBottom3d == nType)
					{
						nInc = -1; nStep =  2 * nWidth; nDist = nWidth - 1;             nDX = nWidth - 1; nDY = 0;           nFX = -1; nFY =  1; nTypeNoBack = Constants::c_nPageTurnRightBottom3d;
					}
					else if ( Constants::c_nPageTurnRightTopWhiteBack3d == nType || Constants::c_nPageTurnRightTop3d == nType)
					{
						nInc = -1; nStep =  0;          nDist = nWidth *  nHeight - 1;  nDX = nWidth - 1; nDY = nHeight - 1; nFX = -1; nFY = -1; nTypeNoBack = Constants::c_nPageTurnRightTop3d;
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
								if( bWhiteBack )
								{
									BYTE cBorderColor = 128;
									BYTE cMiddleColor = 255;
									double dMidLine = dCurveLine / 2;
									double dKoef;
									if ( dX <= dMidLine)
										dKoef = 1 - dX / dMidLine;
									else
										dKoef = (dX - dMidLine) / (dMidLine);
									pBuffer[nIndex + 0] = BYTE(cBorderColor * dKoef + (1 - dKoef) * cMiddleColor);
									pBuffer[nIndex + 1] = BYTE(cBorderColor * dKoef + (1 - dKoef) * cMiddleColor);
									pBuffer[nIndex + 2] = BYTE(cBorderColor * dKoef + (1 - dKoef) * cMiddleColor);
									pBuffer[nIndex + 3] = BYTE(cBorderColor * dKoef + (1 - dKoef) * cMiddleColor);
									continue;
								}
								//else
								//{
								//	//BYTE cMiddleColor = 255;
								//	//double dMidLine = distance - radius * omega;
								//	//double dKoef;
								//	//if ( dX <= dMidLine)
								//	//{
								//	//	dKoef = 1 - dX / dMidLine;
								//	//	//dKoef = sqrt(dKoef);
								//	//}
								//	//else
								//	//{
								//	//	dKoef = (dX - dMidLine) / (dCurveLine - dMidLine);
								//	//	//dKoef *= dKoef;
								//	//}
								//	////dKoef = sqrt(dKoef);
								//	//pBuffer[nIndex + 0] = cMiddleColor + dKoef * (pBGRASource1[nIndex + 0] - cMiddleColor);
								//	//pBuffer[nIndex + 1] = cMiddleColor + dKoef * (pBGRASource1[nIndex + 1] - cMiddleColor);
								//	//pBuffer[nIndex + 2] = cMiddleColor + dKoef * (pBGRASource1[nIndex + 2] - cMiddleColor);
								//	//pBuffer[nIndex + 3] = cMiddleColor + dKoef * (pBGRASource1[nIndex + 3] - cMiddleColor);
								//	//continue;
								//	BYTE cMiddleColor = 255;
								//	double dLightFactor = 0.1 * sqrt(double(nWidth * nWidth + nHeight * nHeight));
								//	double dMidLine = distance - radius * omega;
								//	double dKoef;
								//	if ( dX <= dMidLine && dX >= dMidLine - dLightFactor)
								//	{
								//		dKoef = (dMidLine - dX) / dLightFactor;
								//		//dKoef = sqrt(dKoef);
								//	}
								//	else if (dX >= dMidLine && dX <= dMidLine + dLightFactor)
								//	{
								//		dKoef = (dX - dMidLine) / dLightFactor;
								//		//dKoef *= dKoef;
								//	}
								//	else
								//		dKoef = 1;
								//	dKoef = sqrt(dKoef * dKoef * dKoef);
								//	//dKoef = sqrt(dKoef);
								//	double dFactor = 1;
								//	pBuffer[nIndex + 0] = BYTE(pBGRASource1[nIndex + 0] * dKoef + (1 - dKoef) * (dFactor * 255 + pBGRASource1[nIndex + 0]) / (dFactor + 1));
								//	pBuffer[nIndex + 1] = BYTE(pBGRASource1[nIndex + 1] * dKoef + (1 - dKoef) * (dFactor * 255 + pBGRASource1[nIndex + 1]) / (dFactor + 1));
								//	pBuffer[nIndex + 2] = BYTE(pBGRASource1[nIndex + 2] * dKoef + (1 - dKoef) * (dFactor * 255 + pBGRASource1[nIndex + 2]) / (dFactor + 1));
								//	pBuffer[nIndex + 3] = BYTE(pBGRASource1[nIndex + 3] * dKoef + (1 - dKoef) * (dFactor * 255 + pBGRASource1[nIndex + 3]) / (dFactor + 1));
								//	continue;
								//}
							}
							else if( dX < dFadeLine + distance  )
							{
									BYTE cBlackColor = 0;
									double dKoef = 1 - (distance + dFadeLine  - dX) / (dFadeLine + radius * IPP_PI2);
									dKoef = sqrt(dKoef);
									
									pBuffer[nIndex + 0] = BYTE(cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 0]);
									pBuffer[nIndex + 1] = BYTE(cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 1]);
									pBuffer[nIndex + 2] = BYTE(cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 2]);
									pBuffer[nIndex + 3] = BYTE(cBlackColor * (1 - dKoef) + dKoef * pBGRASource1[nIndex + 3]);
									continue;
							}



						}
					}

					BGRA_PageTurn(pBuffer, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nTypeNoBack, dCompleteness);

					delete []pBuffer;
					return TRUE;
				}
				BOOL BGRA_LoadFotoCorners( Gdiplus::Bitmap* pCornerImage[4], int nType )
				{
					for( int i = 0; i < 4; i++ )
					{
						pCornerImage[i] = NULL;
					}
					
					static const LPCTSTR szNames[4][4] = { 
						_T("#213"), _T("#214"), _T("#215"), _T("#216"), 
						_T("#217"), _T("#218"), _T("#219"), _T("#220"),
						_T("#221"), _T("#222"), _T("#223"), _T("#224"),
						_T("#225"), _T("#226"), _T("#227"), _T("#228"),
					};
						
					HMODULE hModule = ::GetModuleHandle( _T("AVSImageStudio3.dll") );

					const int nWidth  = 128;
					const int nHeight = 128;

					for( int i = 0; i < 4; i++ )
					{
						HRSRC hResHandle = ::FindResource( hModule, szNames[nType][i], _T("RawImage") );
						if( !hResHandle )
							continue;

						HGLOBAL hResMem = ::LoadResource( hModule, hResHandle );
						if( !hResMem )
							continue;

						BYTE* pResData = (BYTE*)::LockResource( hResMem );
						if( !pResData )
							continue;

						Gdiplus::Bitmap* pImage = new Gdiplus::Bitmap( nWidth, nHeight, PixelFormat32bppARGB );
						if( !pImage )
							continue;

						if( pImage->GetLastStatus() != Gdiplus::Ok )
						{
							delete pImage;
							continue;
						}

						Gdiplus::BitmapData data;
						if( Gdiplus::Ok != pImage->LockBits( NULL, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data ) )
						{
							delete pImage;
							continue;
						}

						BYTE* pDst = (BYTE*)data.Scan0;
						BYTE* pSrc = pResData;

						for( int nY = 0; nY < nHeight; ++nY, pDst += data.Stride, pSrc += nWidth * 4 )
						{
							memcpy( pDst, pSrc, nWidth * 4 );
						}

						if( Gdiplus::Ok != pImage->UnlockBits( &data ) )
						{
							delete pImage;
							continue;
						}

						pCornerImage[i]	= pImage;
					}

					return (pCornerImage[0] && pCornerImage[1] && pCornerImage[2] && pCornerImage[3]);
				}
				BOOL BGRA_DrawAlbumList(BYTE* pResult, int nWidth, int nHeight, double dAngle, double dResizeFactor, double dXShift, double dYShift, int nWide, BOOL bCorners, double dFinalAngle, int nTypeCorner )
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
					if( oImage.GetLastStatus() != Gdiplus::Ok )
						return FALSE;

					Gdiplus::Graphics oRender( &oImage );
					if( oRender.GetLastStatus() != Gdiplus::Ok )
						return FALSE;
					
					oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
					oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
					oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
					
					Gdiplus::Pen oPen( Gdiplus::Color( 0, 0, 0 ), 3 );
					
					for( int nIndex = 0; nIndex < nWide; nIndex++ )
					{
						for( int nPoint = 0; nPoint < 4; nPoint++ )
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
						
						for( int nPoint = 0; nPoint < 4; nPoint++ )
						{
							int nTopColor = int(255 * (nIndex / 5.) + 128 * (1 - nIndex / 5.));
							int nBottomColor = int(255 * (nIndex / 5.) + 128 * (1 - nIndex / 5.));
							
							if( nIndex == 0 )
								oPen.SetColor(Gdiplus::Color(0,0,0));
							else
								oPen.SetColor(Gdiplus::Color(255,255,255));
							
							if( !bCorners )
								oRender.DrawLine( &oPen, float(dX[nPoint % 4]), float(dY[nPoint % 4]), float(dX[(nPoint + 1) % 4]), float(dY[(nPoint + 1) % 4]));
						}
					}

					oPen.SetColor( Gdiplus::Color( 0, 0, 0 ) );
					oPen.SetWidth( 1 );

					//прорисовка углов

					if( bCorners )
					{
						Gdiplus::Bitmap* pCorner[4];

						nTypeCorner -= 1;
						if( nTypeCorner < 0 || nTypeCorner > 3 )
						{
							nTypeCorner = 0;
						}

						if( BGRA_LoadFotoCorners( pCorner, nTypeCorner ) )
						{
							double dFactorX = nWidth  / 640.0 * dKoef;
							double dFactorY = nHeight / 480.0 * dKoef;

							double dx = nWidth  * (1 - dKoef) / 2;
							double dy = nHeight * (1 - dKoef) / 2;
							double dw = 18 * dFactorX;
							double dh = 18 * dFactorY;

							double corners[4][2] = { 
								dx - dw,          dy - dh, 
								nWidth - dx + dw, dy - dh, 
								nWidth - dx + dw, nHeight - dy + dh, 
								dx - dw,          nHeight - dy + dh
							};

							double dSizeX = dFactorX * 128;
							double dSizeY = dFactorY * 128;

							Gdiplus::PointF points[4] = {
								Gdiplus::PointF( float(corners[0][0]), float(corners[0][1]) ),
								Gdiplus::PointF( float(corners[1][0] - dSizeX), float(corners[1][1]) ),
								Gdiplus::PointF( float(corners[2][0] - dSizeX), float(corners[2][1] - dSizeY) ),
								Gdiplus::PointF( float(corners[3][0]), float(corners[3][1] - dSizeY) ),
							};

							for( int i = 0; i < 4; i++ )
							{
								Gdiplus::RectF rect( points[i].X, points[i].Y, float(dSizeX), float(dSizeY) );

								Gdiplus::Status st = oRender.DrawImage( pCorner[i], rect );
								
								delete pCorner[i];
							}
						}
						else
						{
							double dCornerWidth  = nWidth  * dKoef * 0.1 /*0.075*/;
							double dCornerHeight = nHeight * dKoef * 0.1 /*0.075*/;

							Gdiplus::PointF arrPoints[3];
							
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
								case 1: nX =  1; nY = -1; break;
								case 2: nX =  1; nY =  1; break;
								case 3: nX = -1; nY =  1; break;
								}

								arrPoints[0].X = float(dX1[nIndex] + nX * dCornerWidth / 2);
								arrPoints[0].Y = float(dY1[nIndex] + nY * dCornerHeight / 2);

								arrPoints[1].Y = float(arrPoints[0].Y - nY * 2 * dSinStep * dCornerWidth);
								arrPoints[2].Y = float(arrPoints[0].Y - nY * 2 * dCosStep * dCornerHeight);
								arrPoints[1].X = float(arrPoints[0].X - nX * 2 * dCosStep * dCornerWidth);
								arrPoints[2].X = float(arrPoints[0].X - nX * 2 * dSinStep * dCornerHeight);

								Gdiplus::PointF oPoint1( arrPoints[0].X, arrPoints[0].Y );
								Gdiplus::PointF oPoint2( arrPoints[1].X, arrPoints[2].Y );

								Gdiplus::LinearGradientBrush oBrush( oPoint1, oPoint2, Gdiplus::Color(clBkColor, clBkColor, clBkColor), Gdiplus::Color(clBkColor, clBkColor, clBkColor));
								oRender.FillPolygon( &oBrush, arrPoints, 3 );

								//точки полигона на котором у нас градиент будет
								Gdiplus::PointF arrBrushPoints[4];
								double dGradientFactor = 0.5;

								double dGradientAngle = atan(double( dCornerHeight / dCornerWidth ));
								double dGradientStep = dGradientFactor * dCornerWidth * sin( dGradientAngle ) * 2;
								double dXGrStep = dGradientStep * sin( dGradientAngle );
								double dYGrStep = dGradientStep * cos( dGradientAngle );
								
								arrBrushPoints[0].X = arrPoints[1].X;
								arrBrushPoints[0].Y = arrPoints[1].Y;
								arrBrushPoints[1].X = arrPoints[2].X;
								arrBrushPoints[1].Y = arrPoints[2].Y;

								arrBrushPoints[2].X = float(dX[nIndex]);
								arrBrushPoints[2].Y = float(dY[nIndex]);

								Gdiplus::GraphicsPath oPath;
								oPath.AddPolygon( arrBrushPoints, 3 );
								
								Gdiplus::PathGradientBrush oBrush1( &oPath );
								oBrush1.SetCenterPoint( Gdiplus::PointF( (arrBrushPoints[0].X + arrBrushPoints[1].X) / 2, (arrBrushPoints[0].Y + arrBrushPoints[1].Y) / 2));
								oBrush1.SetCenterColor( Gdiplus::Color( 255, 255, 255));
								
								int nPathCount = 1;
								Gdiplus::Color clSurrond( 200, 200, 200 );
								
								oBrush1.SetSurroundColors( &clSurrond, &nPathCount );

								oRender.FillPolygon( &oBrush1, arrBrushPoints, 3 );

								oRender.DrawLine( &oPen, arrPoints[1].X, arrPoints[1].Y, arrPoints[2].X, arrPoints[2].Y );
							}
						}
					}

					return TRUE;

				}
				BOOL BGRA_PageTurnAlbum(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					double dFrame = dCompleteness;
					double dResizeFactor = 0.6;
					double dFirstAngleFactor =  -2;
					double dLastAngleFactor = 1;
					BYTE clBkColor = 220;
					Ipp8u backColor[4] = { clBkColor, clBkColor, clBkColor, 255 };
					BOOL bCorners = 0;
//					int nBorder  = 10;
					int nBorder  = int(0.0125 * sqrt( double(nHeight * nHeight + nWidth * nWidth)));
					int nCurType = 0;

					int nTypeCorner = 0;
					
					switch( nType )
					{
					case Constants::c_nPageTurnAlbumLeftTop:             nCurType = Constants::c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 0; break;
					case Constants::c_nPageTurnAlbumLeftTopCorners1:     nCurType = Constants::c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 1; break;
					case Constants::c_nPageTurnAlbumLeftTopCorners2:     nCurType = Constants::c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 2; break;
					case Constants::c_nPageTurnAlbumLeftTopCorners3:     nCurType = Constants::c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 3; break;
					case Constants::c_nPageTurnAlbumLeftTopCorners4:     nCurType = Constants::c_nPageTurnLeftTopWhiteBack3d; nTypeCorner = 4; break;

					case Constants::c_nPageTurnAlbumRightTop:            nCurType = Constants::c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 0; break;
					case Constants::c_nPageTurnAlbumRightTopCorners1:    nCurType = Constants::c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 1; break;
					case Constants::c_nPageTurnAlbumRightTopCorners2:    nCurType = Constants::c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 2; break;
					case Constants::c_nPageTurnAlbumRightTopCorners3:    nCurType = Constants::c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 3; break;
					case Constants::c_nPageTurnAlbumRightTopCorners4:    nCurType = Constants::c_nPageTurnRightTopWhiteBack3d; nTypeCorner = 4; break;

					case Constants::c_nPageTurnAlbumRightBottom:         nCurType = Constants::c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 0; break;
					case Constants::c_nPageTurnAlbumRightBottomCorners1: nCurType = Constants::c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 1; break;
					case Constants::c_nPageTurnAlbumRightBottomCorners2: nCurType = Constants::c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 2; break;
					case Constants::c_nPageTurnAlbumRightBottomCorners3: nCurType = Constants::c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 3; break;
					case Constants::c_nPageTurnAlbumRightBottomCorners4: nCurType = Constants::c_nPageTurnRightBottomWhiteBack3d; nTypeCorner = 4; break;

					case Constants::c_nPageTurnAlbumLeftBottom:          nCurType = Constants::c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 0; break;
					case Constants::c_nPageTurnAlbumLeftBottomCorners1:  nCurType = Constants::c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 1; break;
					case Constants::c_nPageTurnAlbumLeftBottomCorners2:  nCurType = Constants::c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 2; break;
					case Constants::c_nPageTurnAlbumLeftBottomCorners3:  nCurType = Constants::c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 3; break;
					case Constants::c_nPageTurnAlbumLeftBottomCorners4:  nCurType = Constants::c_nPageTurnLeftBottomWhiteBack3d; nTypeCorner = 4; break;
					}

					if ( Constants::c_nPageTurnAlbumLeftTop <= nType && Constants::c_nPageTurnAlbumLeftBottom >= nType )
					{
						bCorners = FALSE;
					}
					else 
					{
						bCorners = TRUE;

						dFirstAngleFactor = 0;
						dLastAngleFactor = 0;
					}

					if ( dFrame < 1. / 3. )
					{
						double dAngle = dFirstAngleFactor * dFrame * 3;
						double dKoef = 1 * (1 - dFrame * 3) + dResizeFactor * dFrame * 3;
						double dXShift = nWidth * (1 - dKoef) / 2, dYShift = nHeight * (1 - dKoef) / 2;
						IppiSize roiSize = { nWidth, nHeight};
						BYTE* pBuffer = new BYTE[ 4 * nWidth * nHeight ];
						ippiSet_8u_C4R(backColor, pBuffer, 4 * nWidth, roiSize);


						IppiRect sourceRect = {0, 0, int(nWidth * dKoef), int(nHeight * dKoef)};
						IppiRect resultRect = {0, 0, nWidth, nHeight};
						IPP::IPP_ResizeCenter(pBGRASource1, pBGRASource1, nWidth, nHeight, dKoef, dKoef, 0, nHeight);
						ippiRotate_8u_C4R(pBGRASource1, roiSize, 4 * nWidth, sourceRect, pBuffer, 4 * nWidth, resultRect, dAngle, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
						
						BGRA_DrawAlbumList(pBuffer, nWidth, nHeight, dAngle, dKoef, dXShift, dYShift, nBorder, bCorners, dFirstAngleFactor, nTypeCorner);
						::memcpy(pBGRAResult, pBuffer, 4 * nWidth * nHeight);
						delete []pBuffer;
					}

					else if ( dFrame < 2. / 3. )
					{
						BYTE* pBuffer1 = new BYTE[ 4 * nWidth * nHeight ];
						BYTE* pBuffer2 = new BYTE[ 4 * nWidth * nHeight ];

						double dXShift = nWidth / 2 * (1 - dResizeFactor), dYShift = nHeight / 2 * (1 - dResizeFactor);
						
						IppiSize roiSize = { nWidth, nHeight};
						IppiRect sourceRect = {0, 0, int(nWidth * dResizeFactor), int(nHeight * dResizeFactor)};
						IppiRect resultRect = {0, 0, nWidth, nHeight};
						
						double dKoef = dResizeFactor;

						ippiSet_8u_C4R(backColor, pBuffer1, 4 * nWidth, roiSize);
						ippiSet_8u_C4R(backColor, pBuffer2, 4 * nWidth, roiSize);

						IPP::IPP_ResizeCenter(pBGRASource1, pBGRASource1, nWidth, nHeight, dResizeFactor, dResizeFactor, 0, nHeight);
						ippiRotate_8u_C4R(pBGRASource1, roiSize, 4 * nWidth, sourceRect, pBuffer1, 4 * nWidth, resultRect, dFirstAngleFactor, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
						IPP::IPP_ResizeCenter(pBGRASource2, pBGRASource2, nWidth, nHeight, dResizeFactor, dResizeFactor, 0, nHeight);
						ippiRotate_8u_C4R(pBGRASource2, roiSize, 4 * nWidth, sourceRect, pBuffer2, 4 * nWidth, resultRect, dLastAngleFactor, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

						BGRA_DrawAlbumList(pBuffer1, nWidth, nHeight, dFirstAngleFactor, dResizeFactor, dXShift, dYShift, nBorder, bCorners, dFirstAngleFactor, nTypeCorner);
						BGRA_DrawAlbumList(pBuffer2, nWidth, nHeight, dLastAngleFactor, dResizeFactor, dXShift, dYShift, nBorder, bCorners, dLastAngleFactor, nTypeCorner);

						BGRA_PageTurnWhiteBack(pBuffer1 , pBuffer2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nCurType, dFrame * 3 - 1);
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
						IppiRect sourceRect = {0, 0, int(nWidth * dKoef), int(nHeight * dKoef)};
						IppiRect resultRect = {0, 0, nWidth, nHeight};

						ippiSet_8u_C4R(backColor, pBuffer, 4 * nWidth, roiSize);

						IPP::IPP_ResizeCenter(pBGRASource2, pBGRASource2, nWidth, nHeight, dKoef, dKoef, 0, nHeight);
						ippiRotate_8u_C4R(pBGRASource2, roiSize, 4 * nWidth, sourceRect, pBuffer, 4 * nWidth, resultRect, dAngle, dXShift, dYShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

						BGRA_DrawAlbumList(pBuffer, nWidth, nHeight, dAngle, dKoef, dXShift, dYShift, nBorder, bCorners, dLastAngleFactor, nTypeCorner);

						::memcpy(pBGRAResult, pBuffer, 4 * nWidth * nHeight);
						delete []pBuffer;
					}
					return TRUE;
				}
				BOOL BGRA_FlipOver(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nFlipOverVerticalBack || nType == Constants::c_nFlipOverVertical)
						return IPP::IPP_FlipOverVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipOverVertical));

					if (nType == Constants::c_nFlipOverHorizontalBack || nType == Constants::c_nFlipOverHorizontal)
						return IPP::IPP_FlipOverHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipOverHorizontal));

					// all ok
					return TRUE;
				}				
				BOOL BGRA_FlipAway(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nFlipAwayVerticalInBack || 
						nType == Constants::c_nFlipAwayVerticalIn || 
						nType == Constants::c_nFlipAwayVerticalOutBack || 
						nType == Constants::c_nFlipAwayVerticalOut)

						return IPP::IPP_FlipAwayVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, 
							(nType == Constants::c_nFlipAwayVerticalIn || nType == Constants::c_nFlipAwayVerticalInBack), 
							(nType == Constants::c_nFlipAwayVerticalIn || nType == Constants::c_nFlipAwayVerticalOut));

					if (nType == Constants::c_nFlipAwayHorizontalInBack|| 
						nType == Constants::c_nFlipAwayHorizontalIn || 
						nType == Constants::c_nFlipAwayHorizontalOutBack || 
						nType == Constants::c_nFlipAwayHorizontalOut)

						return IPP::IPP_FlipAwayHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, 
							(nType == Constants::c_nFlipAwayHorizontalIn || nType == Constants::c_nFlipAwayHorizontalInBack), 
							(nType == Constants::c_nFlipAwayHorizontalIn || nType == Constants::c_nFlipAwayHorizontalOut));

					// all ok
					return TRUE;
				}
				BOOL BGRA_SlideChange(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nX, nY, nIndex;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternValue;
					int nLevelX1 = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY1 = (int)( (nTileSizeY - 1)*dCompleteness );
					int nLevelX2 = (int)( (nTileSizeX - 1)*(1-dCompleteness) );
					int nLevelY2 = (int)( (nTileSizeY - 1)*(1-dCompleteness) );
					
					// compute starting index
					nIndex = 0;

					// combine two images
					int nPatternY = -1;
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						if( ++nPatternY >= nTileSizeY )
							 nPatternY = 0;

						int nPatternX = -1;
						for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// compute pattern X coordinate
							if( ++nPatternX >= nTileSizeX )
								 nPatternX = 0;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;

							// compute color value
							if (nType == Constants::c_nSlideChangeLeftToRight)
							{
								if (nPatternX <= nLevelX1)
								{
									nSourceX += ( nLevelX1 <= 0.5*(nTileSizeX-1) ) ? nLevelX1 : nLevelX2;
									nPatternValue = 1;
								}
								else
								{
									nSourceX -= ( nLevelX1 <= 0.5*(nTileSizeX-1) ) ? nLevelX1 : nLevelX2;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nSlideChangeRightToLeft)
							{

								if (nPatternX <= nLevelX2)
								{
									nSourceX += ( nLevelX1 <= 0.5*(nTileSizeX-1) ) ? nLevelX1 : nLevelX2;
									nPatternValue = 0;
								}
								else
								{
									nSourceX -= ( nLevelX1 <= 0.5*(nTileSizeX-1) ) ? nLevelX1 : nLevelX2;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nSlideChangeBottomToTop)
							{
								if (nPatternY <= nLevelY1)
								{
									nSourceY += ( nLevelY1 <= 0.5*(nTileSizeY-1) ) ? nLevelY1 : nLevelY2;
									nPatternValue = 1;
								}
								else
								{
									nSourceY -= ( nLevelY1 <= 0.5*(nTileSizeY-1) ) ? nLevelY1 : nLevelY2;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nSlideChangeTopToBottom)
							{
								if (nPatternY <= nLevelY2)
								{
									nSourceY += ( nLevelY1 <= 0.5*(nTileSizeY-1) ) ? nLevelY1 : nLevelY2;
									nPatternValue = 0;
								}
								else
								{
									nSourceY -= ( nLevelY1 <= 0.5*(nTileSizeY-1) ) ? nLevelY1 : nLevelY2;
									nPatternValue = 1;
								}
							}

							// combine colors
							if (nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pBGRAResult[nIndex + 3] = pBGRASource1[nIndex + 3];
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								// compute source pixel index
								nSourceIndex = 4*(nSourceX + nSourceY*nWidth);
								
								if (nPatternValue == 0)
								{
									pBGRAResult[nIndex + 3] = pBGRASource1[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource1[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource1[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource1[nSourceIndex + 0];
								}
								else
								{
									pBGRAResult[nIndex + 3] = pBGRASource2[nSourceIndex + 3];
									pBGRAResult[nIndex + 2] = pBGRASource2[nSourceIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource2[nSourceIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource2[nSourceIndex + 0];
								}
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_SlideBoxes(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
				    
					// variables
					int nX, nY, nIndex, nBoxIndex;
					int nSourceX, nSourceY;
					int nTileX, nTileY;
					int nPatternX, nPatternY, nPatternValue;
					double dBoxWidth = 0.05;
					int nFilledBoxes = ( dCompleteness <= 0.5 )? 0 : ((int)((dCompleteness-0.5)/(dBoxWidth/2) + 1));
					dCompleteness -= dBoxWidth*nFilledBoxes;
					int nBoxes = (int)(dCompleteness/dBoxWidth);
					// compute starting index
					nIndex = 0;

					DWORD* pDst  = (DWORD*)pBGRAResult;
					DWORD* pSrc1 = (DWORD*)pBGRASource1;
					DWORD* pSrc2 = (DWORD*)pBGRASource2;

					// combine two images
					for (nY = 0; nY < nHeight; ++nY)
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;
						nTileY = nY / nTileSizeY;
				        
						for (nX = 0; nX < nWidth; ++nX, ++nIndex)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;
							nTileX = nX / nTileSizeX;
				            
							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;
				            
							// compute default source pixel coordinates
							nSourceX = nX;
							nSourceY = nY;
				            
							// compute color value
							if (nType == Constants::c_nSlideLinesLeftToRight)
							{
								if ( nFilledBoxes > 0 && nPatternX >= (int)((nTileSizeX - 1)*(1 - dBoxWidth*nFilledBoxes)) )				
									nPatternValue = 1;

								for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
								{
									if ( nPatternX <= (int)(2*(nTileSizeX-1)*(dCompleteness - dBoxWidth*nBoxIndex)) && nPatternX >= (int)(2*(nTileSizeX-1)*(dCompleteness - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
									{
										nSourceX = nTileX * nTileSizeX + (int)(nPatternX + (nTileSizeX - 1)*(1 - 2*dCompleteness + dBoxWidth*(nBoxIndex - nFilledBoxes)));
										nPatternValue = 1;
									}
								}
							}
							else if (nType == Constants::c_nSlideLinesRightToLeft)
							{
								if ( nFilledBoxes > 0 && nPatternX <= (int)((nTileSizeX - 1)*dBoxWidth*nFilledBoxes) )
									nPatternValue = 1;

								for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
								{
									if ( nPatternX >= nTileSizeX - 1 - (int)(2*(nTileSizeX-1)*(dCompleteness - dBoxWidth*nBoxIndex)) && nPatternX <= nTileSizeX - 1 - (int)(2*(nTileSizeX-1)*(dCompleteness - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
									{
										nSourceX = nTileX * nTileSizeX + (int)(nPatternX - (nTileSizeX - 1)*(1 - 2*dCompleteness + dBoxWidth*(nBoxIndex - nFilledBoxes)));
										nPatternValue = 1;
									}
								}
							}
							if (nType == Constants::c_nSlideLinesBottomToTop)
							{
								if ( nFilledBoxes > 0 && nPatternY >= (int)((nTileSizeY- 1)*(1 - dBoxWidth*nFilledBoxes)) )
									nPatternValue = 1;

								for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
								{
									if ( nPatternY <= (int)(2*(nTileSizeY-1)*(dCompleteness - dBoxWidth*nBoxIndex)) && nPatternY >= (int)(2*(nTileSizeY-1)*(dCompleteness - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
									{
										nSourceY = nTileY * nTileSizeY + (int)(nPatternY + (nTileSizeY - 1)*(1 - 2*dCompleteness + dBoxWidth*(nBoxIndex - nFilledBoxes)));
										nPatternValue = 1;
									}
								}
							}
							if (nType == Constants::c_nSlideLinesTopToBottom)
							{
								if ( nFilledBoxes > 0 && nPatternY <= (int)((nTileSizeY - 1)*dBoxWidth*nFilledBoxes) )
									nPatternValue = 1;

								for ( nBoxIndex=0; nBoxIndex<=nBoxes; nBoxIndex++ )
								{
									if ( nPatternY >= nTileSizeY - 1 - (int)(2*(nTileSizeY-1)*(dCompleteness - dBoxWidth*nBoxIndex)) && nPatternY <= nTileSizeY - 1 - (int)(2*(nTileSizeY-1)*(dCompleteness - dBoxWidth/2 - dBoxWidth*nBoxIndex)) )
									{
										nSourceY = nTileY * nTileSizeY + (int)(nPatternY - (nTileSizeY - 1)*(1 - 2*dCompleteness + dBoxWidth*(nBoxIndex - nFilledBoxes)));
										nPatternValue = 1;
									}
								}
							}
				            
							// combine colors
							if (nPatternValue == 0 || nSourceX < 0 || nSourceX >= nWidth || nSourceY < 0 || nSourceY >= nHeight)
							{
								pDst[nIndex] = pSrc1[nIndex];
							}
							else
							{
								pDst[nIndex] = pSrc2[nSourceY * nWidth + nSourceX];
							}
						}
					}
				    
					// all ok
					return TRUE;
				}
				BOOL BGRA_WipeClock(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					int nAngleFactor = 2;
					if( nType == Constants::c_nClockBottomOpposite || 
						nType == Constants::c_nClockTopOpposite || 
						nType == Constants::c_nClockLeftOpposite || 
						nType == Constants::c_nClockRightOpposite )
					{
						nAngleFactor = 1;
					}

					int nParts = 1;
					if( nType == Constants::c_nClock2AnglesClockwise || 
						nType == Constants::c_nClock2AnglesReverse )
						nParts = 2;
					else 
					if( nType == Constants::c_nClock3AnglesClockwise || 
						nType == Constants::c_nClock3AnglesReverse )
						nParts = 3;
					else 
					if( nType == Constants::c_nClock4AnglesClockwise || 
						nType == Constants::c_nClock4AnglesReverse )
						nParts = 4;
					else 
					if( nType == Constants::c_nClock6AnglesClockwise || 
						nType == Constants::c_nClock6AnglesReverse )
						nParts = 6;
					else 
					if( nType == Constants::c_nClock8AnglesClockwise || 
						nType == Constants::c_nClock8AnglesReverse )
						nParts = 8;
					else 
					if( nType == Constants::c_nClock12AnglesClockwise || 
						nType == Constants::c_nClock12AnglesReverse )
						nParts = 12;

					double dAngle = IPP_PI * dCompleteness * nAngleFactor / nParts;
					double dSector = IPP_2PI / nParts; // always greater zero

					double dStartAngle = IPP_PI2;

					if( nType == Constants::c_nClockBottomClockwise || 
						nType == Constants::c_nClockBottomReverse || 
						nType == Constants::c_nClockBottomOpposite )
					{
						dStartAngle = -IPP_PI2;
					}
					else 
					if( nType == Constants::c_nClockLeftClockwise || 
						nType == Constants::c_nClockLeftReverse || 
						nType == Constants::c_nClockLeftOpposite )
					{
						dStartAngle = IPP_PI;
					}
					else 
					if( nType == Constants::c_nClockRightClockwise || 
						nType == Constants::c_nClockRightReverse || 
						nType == Constants::c_nClockRightOpposite )
					{
						dStartAngle = 0;
					}

					double dTileCenterX = (nTileSizeX - 1) / 2.0;
					double dTileCenterY = (nTileSizeY - 1) / 2.0;

					int nSignAngle = 1;
					if( nType == Constants::c_nClockBottomClockwise || 
						nType == Constants::c_nClockTopClockwise ||
						nType == Constants::c_nClockLeftClockwise || 
						nType == Constants::c_nClockRightClockwise ||
						nType == Constants::c_nClock3AnglesClockwise || 
						nType == Constants::c_nClock2AnglesClockwise ||
						nType == Constants::c_nClock4AnglesClockwise || 
						nType == Constants::c_nClock6AnglesClockwise ||
						nType == Constants::c_nClock8AnglesClockwise || 
						nType == Constants::c_nClock12AnglesClockwise )
					{
						nSignAngle = -1;
					}

					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;

					// combine two images
					int nPatternY = 0;
					for( int nY = 0; nY < nHeight; ++nY, ++nPatternY )
					{
						// compute pattern Y coordinate
						if( nPatternY >= nTileSizeY )
							nPatternY = 0;

						int nPatternX = 0;
						for( int nX = 0; nX < nWidth; ++nX, ++nPatternX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( nPatternX >= nTileSizeX )
								nPatternX = 0;

							double dA = ::atan2( nPatternY - dTileCenterY, nPatternX - dTileCenterX );

							dA -= dStartAngle;
							dA *= nSignAngle;
							
							if( dA < 0 )
								dA += IPP_2PI;
								
							int nPatternValue = 0;

							if( (nAngleFactor == 1) && (IPP_2PI - dA <= dAngle) )
							{
								nPatternValue = 1;
							}
							else
							{
								for( int i = 0; i < nParts; ++i, dA -= dSector )
								{
									if( dA < 0 )
										break;

									if( dA <= dAngle )
									{	
										nPatternValue = 1;
										break;
									}
								}
							}

							// combine colors
							pResult[0] = nPatternValue ? pSource2[0] : pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}

				BOOL BGRA_WipeStripsRandom(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CUtility *pUtility)
				{
					DWORD* pDst  = (DWORD*)pBGRAResult;
					DWORD* pSrc1 = (DWORD*)pBGRASource1;
					DWORD* pSrc2 = (DWORD*)pBGRASource2;
					
					int nFrame = int(dCompleteness * 100.0);
					if( Constants::c_nWipeRandomStripsHorizontal == nType )
					{
						pUtility->GenerateWipeStrips(nHeight);
						for( int nYIndex = 0; nYIndex < nHeight; ++nYIndex )
						{
							for( int nXIndex = 0; nXIndex < nWidth; ++nXIndex, ++pDst, ++pSrc1, ++pSrc2 )
							{
								if (pUtility->g_ppWipeRandomStrips[nYIndex][nFrame] >= nFrame)
								{
									pDst[0] = pSrc1[0];
								}
								else
								{
									pDst[0] = pSrc2[0];
								}
							}
						}
					}
					else if( Constants::c_nWipeRandomStripsVertical == nType )
					{
						pUtility->GenerateWipeStrips(nWidth);
						for( int nYIndex = 0; nYIndex < nHeight; ++nYIndex )
						{
							for( int nXIndex = 0; nXIndex < nWidth; ++nXIndex, ++pDst, ++pSrc1, ++pSrc2 )
							{
								if (pUtility->g_ppWipeRandomStrips[nXIndex][nFrame] >= nFrame)
								{
									pDst[0] = pSrc1[0];
								}
								else
								{
									pDst[0] = pSrc2[0];
								}
							}
						}
					}
					
					return TRUE;
				}

				BOOL BGRA_ShatterCorner(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nwRect = 16; // quantity of squares in width
					int nhRect = 12; // quantity of squares in height
					int nW; // width of square
					int nH; // height of square

					if (nTileSizeX < nwRect)
					{
						nwRect = nTileSizeX;
						nW = 1;
					}
					else
					{
						nW = (int)(nTileSizeX/nwRect);
						nwRect = (int)((nTileSizeX-1)/nW) + 1;
					}

					if (nTileSizeY < nhRect)
					{
						nhRect = nTileSizeY;
						nH = 1;
					}
					else
					{
						nH = (int)(nTileSizeY/nhRect);
						nhRect = (int)((nTileSizeY-1)/nH) + 1;
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
					int nTilesX = (nWidth - 1) / nTileSizeX + 1;
					int	nTilesY = (nHeight - 1) / nTileSizeY + 1;
					int nImageStep = nWidth * 4;
					int nDstTmp, nSrcTmp;
				    
					for ( i=0; i<nwRect; i++ )
						nRandomCol[i] = i;
					for ( i=0; i<nhRect; i++ )
						nRandomRow[i] = i;

					std::random_shuffle( &nRandomCol[0], &nRandomCol[nwRect] );
					std::random_shuffle( &nRandomRow[0], &nRandomRow[nhRect] );

					srand(1);

					memcpy(pBGRAResult, pBGRASource2, 4*nWidth*nHeight);

					if (nType == Constants::c_nShatterCornerRightTop)
					{
						nFinishX = nTileSizeX + 50;
						nFinishY = nTileSizeY + 50;
					} 
					else if (nType == Constants::c_nShatterCornerRightBottom)
					{
						nFinishX = nTileSizeX + 50;
						nFinishY = -nH - 50;
					}
					else if (nType == Constants::c_nShatterCornerLeftTop)
					{
						nFinishX = -nW - 50;
						nFinishY = nTileSizeY + 50;
					}
					else if (nType == Constants::c_nShatterCornerLeftBottom)
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
							dSpeed = dCompleteness * ( ((double)rand()/RAND_MAX)/2.0 + 0.75 );
							nY_O1 = (int)(nY_O * (1 - dSpeed) + nFinishY * dSpeed);
							nX_O1 = (int)(nX_O * (1 - dSpeed) + nFinishX * dSpeed);
				            
							for ( i=0; i<nH; i++)
							{
								nSourceTileY = i + nY_O;
								nDestTileY = i + nY_O1;
								
								if ( nSourceTileY >= nTileSizeY || nDestTileY >= nTileSizeY || nSourceTileY < 0 || nDestTileY < 0 )
									continue;

								for ( j=0; j<nW; j++)
								{
									nDestTileX = j + nX_O1;
									nSourceTileX = j + nX_O;
				                                        
									if ( nSourceTileX >= nTileSizeX || nDestTileX >= nTileSizeX || nSourceTileX < 0 || nDestTileX < 0 )
										continue;

									for ( nTileY=0; nTileY<nTilesY; nTileY++ )
									{
										nSourceY = nTileY * nTileSizeY + nSourceTileY;
										nDestY = nTileY * nTileSizeY + nDestTileY;

										if ( nDestY >= nHeight || nDestY < 0 || nSourceY >= nHeight || nSourceY < 0 )
											continue;

										nDstTmp = nImageStep * nDestY;
										nSrcTmp = nImageStep * nSourceY;

										for (nTileX = 0; nTileX < nTilesX; nTileX++ )
										{
											nSourceX = nTileX * nTileSizeX + nSourceTileX;
											nDestX = nTileX * nTileSizeX + nDestTileX;

											if ( nDestX >= nWidth || nDestX < 0 || nSourceX >= nWidth || nSourceX < 0 )
												continue;
											
											nDestIndex = nDstTmp + 4 * nDestX; 
											nSourceIndex = nSrcTmp + 4 * nSourceX; 

											pBGRAResult[nDestIndex + 0] = (BYTE)((pBGRAResult[nDestIndex + 0] - pBGRASource1[nSourceIndex + 0]) * dCompleteness) + pBGRASource1[nSourceIndex + 0];
											pBGRAResult[nDestIndex + 1] = (BYTE)((pBGRAResult[nDestIndex + 1] - pBGRASource1[nSourceIndex + 1]) * dCompleteness) + pBGRASource1[nSourceIndex + 1];
											pBGRAResult[nDestIndex + 2] = (BYTE)((pBGRAResult[nDestIndex + 2] - pBGRASource1[nSourceIndex + 2]) * dCompleteness) + pBGRASource1[nSourceIndex + 2];
											pBGRAResult[nDestIndex + 3] = min(pBGRASource1[nSourceIndex + 3], pBGRASource2[nSourceIndex + 3]);
									
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



				BOOL BGRA_ShatterSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// копируем source2 в результ
					memcpy( pBGRAResult, pBGRASource2, nWidth * nHeight * 4 );

					// расчитываем и выравниваем размер тайлов
					int nNumHorTiles = 16; // quantity of squares in width  // nwRect
					int nNumVerTiles = 12; // quantity of squares in height // nhRect
					int nTileWidth   = 1;  // width of square  // nW
					int nTileHeight  = 1;  // height of square // nH
				    
					if( nTileSizeX < nNumHorTiles )
					{
						nNumHorTiles = nTileSizeX; 
					} 
					else
					{
						nTileWidth   = nTileSizeX / nNumHorTiles;
						nNumHorTiles = (nTileSizeX - 1) / nTileWidth + 1;
					}
					
					if( nTileSizeY < nNumVerTiles )
					{
						nNumVerTiles = nTileSizeY; 
					}
					else
					{
						nTileHeight  = nTileSizeY / nNumVerTiles;
						nNumVerTiles = (nTileSizeY - 1) / nTileHeight + 1;
					}

					// перемешываем тайлы
					int* pShuffleArray = new int[nNumHorTiles + nNumVerTiles];
					if( !pShuffleArray )
						return FALSE;

					int* pShuffleColumns = pShuffleArray;
					int* pShuffleRows    = pShuffleArray + nNumHorTiles;
					int  nCountTiles     = 0;
					
					for( nCountTiles = 0; nCountTiles < nNumHorTiles; nCountTiles++ )
					{
						pShuffleColumns[nCountTiles] = nCountTiles;
					}

					for( nCountTiles = 0; nCountTiles < nNumVerTiles; nCountTiles++ )
					{
						pShuffleRows[nCountTiles] = nCountTiles;
					}

					srand( 1000 );

					std::random_shuffle( pShuffleColumns, pShuffleColumns + nNumHorTiles );
					std::random_shuffle( pShuffleRows, pShuffleRows + nNumVerTiles );
					
					// расчитываем дополнительные параметры
					int alpha   = int(256 * (1-dCompleteness) + 0.5);
					int nRadius = int(sqrt( double(nTileWidth * nTileWidth + nTileHeight * nTileHeight) )) + 1;

					int nTilesX = (nWidth - 1)  / nTileSizeX + 1;
					int	nTilesY = (nHeight - 1) / nTileSizeY + 1;

					int nFinishX = 0;
					int nFinishY = 0;

					switch( nType )
					{
					case Constants::c_nShatterSideLeftToRight: nFinishX =  nTileSizeX  + 20; break;
					case Constants::c_nShatterSideRightToLeft: nFinishX = -nTileWidth  - 20; break;
					case Constants::c_nShatterSideTopToBottom: nFinishY = -nTileHeight - 20; break;
					case Constants::c_nShatterSideBottomToTop: nFinishY =  nTileSizeY  + 20; break;
					}

					for( int nRowIndex = nNumVerTiles - 1; nRowIndex >= 0; --nRowIndex )
					{
						double dTileCenterY = (pShuffleRows[nRowIndex] + 0.5) * nTileHeight;

						for( int nColIndex = nNumHorTiles - 1; nColIndex >= 0; --nColIndex )
						{
							double dTileCenterX = (pShuffleColumns[nColIndex] + 0.5) * nTileWidth;

							double dSpeed = dCompleteness * (::rand() / double(RAND_MAX) * 0.5 + 1);
							double dAngle = dCompleteness * (::rand() / double(RAND_MAX) * IPP_PI * 4);
							
							if( nType == Constants::c_nShatterSideRightToLeft || 
								nType == Constants::c_nShatterSideTopToBottom )
							{
								dAngle = -dAngle;
							}

							if( nType == Constants::c_nWhirlWind)
							{
								dAngle *= 2;
							}

							double dCos = cos(dAngle);
							double dSin = sin(dAngle);

							double dTileX = dTileCenterX;
							double dTileY = dTileCenterY;

							if( nType == Constants::c_nShatterSideLeftToRight || 
								nType == Constants::c_nShatterSideRightToLeft )
							{	
								dTileX *= 1 - dSpeed;
								dTileX += nFinishX * dSpeed;
							}
							else 
							if( nType == Constants::c_nShatterSideTopToBottom || 
								nType == Constants::c_nShatterSideBottomToTop )
							{
								dTileY *= 1 - dSpeed;
								dTileY += nFinishY * dSpeed;
							}

							for( int i = 0; i < nRadius; i++ ) // y coordinate
							{
								double dCheckY = i - nRadius * 0.5;
								
								for( int j = 0; j < nRadius; j++ ) // x coordinate
								{
									double dCheckX = j - nRadius * 0.5;

									double dSourceTileX = dCheckX * dCos - dCheckY * dSin; 
									double dSourceTileY = dCheckX * dSin + dCheckY * dCos;

									if( fabs(dSourceTileX) > nTileWidth * 0.5 )
										continue;

									if( fabs(dSourceTileY) > nTileHeight * 0.5 )
										continue;

									dSourceTileX += dTileCenterX;
									dSourceTileY += dTileCenterY;

									int nDestTileX = (int)(dTileX + dCheckX);
									int nDestTileY = (int)(dTileY + dCheckY);

									if ( dSourceTileY >= nTileSizeY || nDestTileY >= nTileSizeY || dSourceTileY < 0 || nDestTileY < 0 ||
										 dSourceTileX >= nTileSizeX || nDestTileX >= nTileSizeX || dSourceTileX < 0 || nDestTileX < 0 )
										continue;

									for( int nTileY = 0; nTileY < nTilesY; nTileY++ )
									{
										double dSourceY = nTileY * nTileSizeY + dSourceTileY;
										int    nDestY   = nTileY * nTileSizeY + nDestTileY;

										if( nDestY >= nHeight || nDestY < 0 || dSourceY >= nHeight || dSourceY < 0 )
											continue;

										int nDestTmp = nDestY * nWidth * 4;
										int nSrcTmp = (int)(dSourceY) * nWidth * 4; 

										for( int nTileX = 0; nTileX < nTilesX; nTileX++ )
										{
											double dSourceX = nTileX * nTileSizeX + dSourceTileX;
											int    nDestX   = nTileX * nTileSizeX + nDestTileX;

											if( nDestX >= nWidth || nDestX < 0 || dSourceX >= nWidth || dSourceX < 0 )
												continue;
											
											int nDstDisp = nDestTmp + nDestX * 4;
											int nSrcDisp = nSrcTmp  + int(dSourceX) * 4;

											pBGRAResult[nDstDisp + 0] = AlphaBlend( pBGRAResult[nDstDisp + 0], pBGRASource1[nSrcDisp + 0], alpha );
											pBGRAResult[nDstDisp + 1] = AlphaBlend( pBGRAResult[nDstDisp + 1], pBGRASource1[nSrcDisp + 1], alpha );
											pBGRAResult[nDstDisp + 2] = AlphaBlend( pBGRAResult[nDstDisp + 2], pBGRASource1[nSrcDisp + 2], alpha );
											pBGRAResult[nDstDisp + 3] = min(pBGRASource1[nSrcDisp + 3], pBGRASource2[nSrcDisp + 3]);
										}
									}
								}
							}
						}
					}
					
					delete [] pShuffleArray;

					// all ok
					return TRUE;
				}
				BOOL BGRA_ShatterCenter(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int nNumHorRects = 16; // quantity of squares in width  // nwRect
					int nNumVerRects = 12; // quantity of squares in height // nhRect
					int nRectWidth;  // width of square  // nW
					int nRectHeight; // height of square // nH

					if (nType == Constants::c_nShatterCenterInsideHigh)
					{
						nNumHorRects = 32;
						nNumVerRects = 24;
					}

					// копируем source1 на result
					memcpy( pBGRAResult, pBGRASource2, 4*nWidth*nHeight );

					// выравниваем размеры прямоугольников
					if( nTileSizeX < nNumHorRects )
					{
						nNumHorRects = nTileSizeX;
						nRectWidth   = 1;
					}
					else
					{
						nRectWidth   = nTileSizeX / nNumHorRects;
						nNumHorRects = (nTileSizeX - 1) / nRectWidth + 1;
					}
					if( nTileSizeY < nNumVerRects )
					{
						nNumVerRects = nTileSizeY;
						nRectHeight  = 1;
					}
					else
					{
						nRectHeight  = nTileSizeY / nNumVerRects;
						nNumVerRects = (nTileSizeY - 1) / nRectHeight + 1;
					}

					// перемешываем прямоугольники
					int* pShuffleArray = new int[nNumHorRects + nNumVerRects + 2];
					if( !pShuffleArray )
						return FALSE;

					int* pShuffleColumns = pShuffleArray;
					int* pShuffleRows    = pShuffleArray + nNumHorRects + 1;
					int  nCountRects     = 0;
					
					for( nCountRects = 0; nCountRects <= nNumHorRects; nCountRects++ )
					{
						pShuffleColumns[nCountRects] = nCountRects;
					}

					for( nCountRects = 0; nCountRects <= nNumVerRects; nCountRects++ )
					{
						pShuffleRows[nCountRects] = nCountRects;
					}

					srand( 1 );

					std::random_shuffle( pShuffleColumns, pShuffleColumns + nNumHorRects );
					std::random_shuffle( pShuffleRows, pShuffleRows + nNumVerRects );
					
					// расчитываем дополнительные параметры
					double dSizeRect       = sqrt( double(nRectWidth * nRectWidth + nRectHeight * nRectHeight) );
					double dHalfRectHeight = nRectHeight / 2.0;
					double dHalfRectWidth  = nRectWidth  / 2.0;
					
					double Cx = (nTileSizeX) / 2.0;
					double Cy = (nTileSizeY) / 2.0;

					//double dDisappear = dCompleteness * dCompleteness * dCompleteness;
					int alpha = int(256 * (1 - dCompleteness * dCompleteness * dCompleteness) + 0.5);

					// основной расчёт
					for( int nRowIndex = nNumVerRects; nRowIndex >= 0 ; --nRowIndex )
					{
						double dRectCenterY = (nRectHeight * (pShuffleRows[nRowIndex] + 0.5));

						for( int nColIndex = nNumHorRects; nColIndex >= 0; --nColIndex )
						{
							double dRectCenterX = (nRectWidth * (pShuffleColumns[nColIndex] + 0.5));
							
							double dDist = 1 - dCompleteness * (rand() / double(RAND_MAX) * 2.5 + 0.5); // 1 - dCompleteness * dSpeed
							if( dDist < 0.1 ) // ближе чем можем увидеть
								continue;

							// новые координаты прямоугольника
							double dNewRectCenterX = (dRectCenterX - Cx) / dDist + Cx;
							double dNewRectCenterY = (dRectCenterY - Cy) / dDist + Cy;

							// расчитываем новые размеры прямоугольника
							int    nRadius = int(dSizeRect / dDist + 1);
							double dTileC  = (nRadius) / 2.0;

							// отсекаем прямоугольники, которые не попадают в область вывода
							if( dNewRectCenterX - dTileC < 0 &&
								dNewRectCenterX + dTileC < 0 )
								continue;

							if( dNewRectCenterX - dTileC >= nWidth &&
								dNewRectCenterX + dTileC >= nWidth )
								continue;

							if( dNewRectCenterY - dTileC < 0 &&
								dNewRectCenterY + dTileC < 0 )
								continue;

							if( dNewRectCenterY - dTileC >= nHeight &&
								dNewRectCenterY + dTileC >= nHeight )
								continue;

							// расчитываем угол поворота
							double dAngle = (1 - dDist) * IPP_2PI;
							double dCos   = cos(dAngle) * dDist;
							double dSin   = sin(dAngle) * dDist;

							// отрисовка прямоугольника
							for( int i = 0; i < nRadius; i++ ) // y coordinate
							{
								double dCheckY = i - dTileC;

								if( dNewRectCenterY + dCheckY < 0 ||
									dNewRectCenterY + dCheckY >= nTileSizeY )
									continue;
								
								for( int j = 0; j < nRadius; j++ ) // x coordinate
								{
									double dCheckX = j - dTileC;

									if( dNewRectCenterX + dCheckX < 0 ||
										dNewRectCenterX + dCheckX >= nTileSizeX )
										continue;

									double dSourceTileX = dCheckX * dCos - dCheckY * dSin; 
									double dSourceTileY = dCheckX * dSin + dCheckY * dCos;

									if( fabs(dSourceTileX) <= dHalfRectWidth && fabs(dSourceTileY) <= dHalfRectHeight)
									{
										dSourceTileX += dRectCenterX;
										dSourceTileY += dRectCenterY;

										if( dSourceTileY >= nTileSizeY || dSourceTileY < 0 )
											continue;
										
										if(	dSourceTileX >= nTileSizeX || dSourceTileX < 0 )
											continue;

										int nSrcDisp = (nWidth * int(dSourceTileY) + int(dSourceTileX)) * 4;
										int nDstDisp = (nWidth * int(dNewRectCenterY + dCheckY) + int(dNewRectCenterX + dCheckX)) * 4;

										pBGRAResult[nDstDisp + 0] = AlphaBlend( pBGRAResult[nDstDisp + 0], pBGRASource1[nSrcDisp + 0], alpha );
										pBGRAResult[nDstDisp + 1] = AlphaBlend( pBGRAResult[nDstDisp + 1], pBGRASource1[nSrcDisp + 1], alpha );
										pBGRAResult[nDstDisp + 2] = AlphaBlend( pBGRAResult[nDstDisp + 2], pBGRASource1[nSrcDisp + 2], alpha );
										pBGRAResult[nDstDisp + 3] = min(pBGRASource1[nSrcDisp + 3], pBGRASource2[nSrcDisp + 3]);
									}
								}
							}
						}
					}

					delete [] pShuffleArray;
					
					// all ok
					return TRUE;
				}

				BOOL BGRA_Graduate(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CMaskGraduate* pMaskGraduate)
				{
					int nTilesX = (nWidth  + nTileSizeX - 1) / nTileSizeX;
					int nTilesY = (nHeight + nTileSizeY - 1) / nTileSizeY;
					
					int nWideFade = 0;
					int nCurType = nType;
					if ( Constants::c_nGraduateSideLeftToRightSmooth <= nType && Constants::c_nGraduateClock3AnglesReverseSmooth >= nType )
						nWideFade = 0;
					switch(nType)
					{
					case Constants::c_nGraduateSideLeftToRightSmooth:       nCurType = Constants::c_nGraduateSideLeftToRight;       nWideFade = 5; break;
					case Constants::c_nGraduateSideRightToLeftSmooth:       nCurType = Constants::c_nGraduateSideRightToLeft;       nWideFade = 5; break;
					case Constants::c_nGraduateSideTopToBottomSmooth:       nCurType = Constants::c_nGraduateSideTopToBottom;       nWideFade = 5; break;
					case Constants::c_nGraduateSideBottomToTopSmooth:       nCurType = Constants::c_nGraduateSideBottomToTop;       nWideFade = 5; break;
					case Constants::c_nGraduateCornerLeftTopSmooth:         nCurType = Constants::c_nGraduateCornerLeftTop;         nWideFade = 5; break;
					case Constants::c_nGraduateCornerLeftBottomSmooth:      nCurType = Constants::c_nGraduateCornerLeftBottom;      nWideFade = 5; break;
					case Constants::c_nGraduateCornerRightBottomSmooth:     nCurType = Constants::c_nGraduateCornerRightBottom;     nWideFade = 5; break;
					case Constants::c_nGraduateCornerRightTopSmooth:        nCurType = Constants::c_nGraduateCornerRightTop;        nWideFade = 5; break;
					case Constants::c_nGraduateClockBottomClockwiseSmooth:  nCurType = Constants::c_nGraduateClockBottomClockwise;  nWideFade = 5; break;
					case Constants::c_nGraduateClockBottomReverseSmooth:    nCurType = Constants::c_nGraduateClockBottomReverse;    nWideFade = 5; break;
					case Constants::c_nGraduateClockBottomOppositeSmooth:   nCurType = Constants::c_nGraduateClockBottomOpposite;   nWideFade = 5; break;
					case Constants::c_nGraduateClockTopClockwiseSmooth:     nCurType = Constants::c_nGraduateClockTopClockwise;     nWideFade = 5; break;
					case Constants::c_nGraduateClockTopReverseSmooth:       nCurType = Constants::c_nGraduateClockTopReverse;       nWideFade = 5; break;
					case Constants::c_nGraduateClockTopOppositeSmooth:      nCurType = Constants::c_nGraduateClockTopOpposite;      nWideFade = 5; break;
					case Constants::c_nGraduateClockLeftClockwiseSmooth:    nCurType = Constants::c_nGraduateClockLeftClockwise;    nWideFade = 5; break;
					case Constants::c_nGraduateClockLeftReverseSmooth:      nCurType = Constants::c_nGraduateClockLeftReverse;      nWideFade = 5; break;
					case Constants::c_nGraduateClockLeftOppositeSmooth:     nCurType = Constants::c_nGraduateClockLeftOpposite;     nWideFade = 5; break;
					case Constants::c_nGraduateClockRightClockwiseSmooth:   nCurType = Constants::c_nGraduateClockRightClockwise;   nWideFade = 5; break;
					case Constants::c_nGraduateClockRightReverseSmooth:	    nCurType = Constants::c_nGraduateClockRightReverse;     nWideFade = 5; break;
					case Constants::c_nGraduateClockRightOppositeSmooth:    nCurType = Constants::c_nGraduateClockRightOpposite;    nWideFade = 5; break;
					case Constants::c_nGraduateClock2AnglesClockwiseSmooth: nCurType = Constants::c_nGraduateClock2AnglesClockwise; nWideFade = 5; break;
					case Constants::c_nGraduateClock2AnglesReverseSmooth:   nCurType = Constants::c_nGraduateClock2AnglesReverse;   nWideFade = 5; break;
					case Constants::c_nGraduateClock3AnglesClockwiseSmooth: nCurType = Constants::c_nGraduateClock3AnglesClockwise; nWideFade = 5; break;
					case Constants::c_nGraduateClock3AnglesReverseSmooth:   nCurType = Constants::c_nGraduateClock3AnglesReverse;   nWideFade = 5; break;
					}
					return IPP::IPP_MaskGraduate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTilesX, nTilesY, nCurType, dCompleteness, pMaskGraduate, nWideFade);
				}
				BOOL BGRA_GraduateWipeSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nTilesX, int nTilesY, int nType, double dCompleteness, Utility::CGraduate* pGraduate)
				{
					// variables
					int nX, nY, nIndex;
					int nPatternX, nPatternY, nPatternValue;
					int nLevelX = (int)( (nTileSizeX - 1)*dCompleteness );
					int nLevelY = (int)( (nTileSizeY - 1)*dCompleteness );
					int nLevelTile = (int)((nTilesX*nTilesY)*dCompleteness);
					int nTileX, nTileY, nTileIndex, nTileOldIndex;
					double dTileLevel;
					
					// compute starting indexex
					nIndex = 0;
					nTileIndex = 0;
					nTileOldIndex = -1;

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
							nTileIndex = nTileX + nTilesX*nTileY;

							// compute pattern value: if 0 - pixel is first image pixel, last image pixel otherwise
							nPatternValue = 0;

							// check whether we reach new tile
							if (nTileIndex != nTileOldIndex)
							{
								dTileLevel = pGraduate->GetLevel(nTileIndex, dCompleteness);
								nTileOldIndex = nTileIndex;

								nLevelX = (int)(nTileSizeX*dTileLevel);
								nLevelY = (int)(nTileSizeY*dTileLevel);
							}

							// compute color value
							if (nType == Constants::c_nGraduateSideLeftToRight)
							{
								if (nPatternX < nLevelX)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nGraduateSideRightToLeft)
							{
								if (nPatternX > (nTileSizeX - 1) - nLevelX)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nGraduateSideBottomToTop)
							{
								if (nPatternY < nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nGraduateSideTopToBottom)
							{
								if (nPatternY > (nTileSizeY - 1) - nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nGraduateCornerLeftTop)
							{
								if (nPatternX < nLevelX && nPatternY > (nTileSizeY - 1) - nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nGraduateCornerLeftBottom)
							{
								if (nPatternX < nLevelX && nPatternY < nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nGraduateCornerRightBottom)
							{
								if (nPatternX > (nTileSizeX - 1) - nLevelX && nPatternY < nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nGraduateCornerRightTop)
							{
								if (nPatternX > (nTileSizeX - 1) - nLevelX && nPatternY > (nTileSizeY - 1) - nLevelY)
									nPatternValue = 1;
							}

							// combine colors
							if (nPatternValue == 0)
							{
								pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
							}
							else
							{
								pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
								pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
								pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_GraduateWipeClock(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nTilesX, int nTilesY, int nType, double dCompleteness, Utility::CGraduate* pGraduate)
				{
					double dStartAngle = IPP_PI2;
					
					if( nType == Constants::c_nGraduateClockBottomClockwise || 
						nType == Constants::c_nGraduateClockBottomReverse || 
						nType == Constants::c_nGraduateClockBottomOpposite )
					{
						dStartAngle = -IPP_PI2;
					}
					else 
					if( nType == Constants::c_nGraduateClockLeftClockwise || 
						nType == Constants::c_nGraduateClockLeftReverse || 
						nType == Constants::c_nGraduateClockLeftOpposite )
					{
						dStartAngle = IPP_PI;
					}
					else 
					if( nType == Constants::c_nGraduateClockRightClockwise || 
						nType == Constants::c_nGraduateClockRightReverse || 
						nType == Constants::c_nGraduateClockRightOpposite )
					{
						dStartAngle = 0;
					}

					int nAngleFactor = 2;
					if( nType == Constants::c_nGraduateClockBottomOpposite ||
						nType == Constants::c_nGraduateClockTopOpposite || 
						nType == Constants::c_nGraduateClockLeftOpposite || 
						nType == Constants::c_nGraduateClockRightOpposite )
					{
						nAngleFactor = 1;
					}

					int nParts = 1;
					if( nType == Constants::c_nGraduateClock2AnglesClockwise || 
						nType == Constants::c_nGraduateClock2AnglesReverse )
						nParts = 2;
					else 
					if( nType == Constants::c_nGraduateClock3AnglesClockwise || 
						nType == Constants::c_nGraduateClock3AnglesReverse )
						nParts = 3;

					double dAngle  = IPP_PI * dCompleteness * nAngleFactor / nParts;
					double dSector = IPP_2PI / nParts;

					double dTileCenterX = (nTileSizeX - 1) / 2.0;
					double dTileCenterY = (nTileSizeY - 1) / 2.0;

					int nTileOldIndex = -1;
					int nSignAngle = 1;
					
					if( nType == Constants::c_nGraduateClockBottomClockwise || 
						nType == Constants::c_nGraduateClockTopClockwise ||
						nType == Constants::c_nGraduateClockLeftClockwise || 
						nType == Constants::c_nGraduateClockRightClockwise ||
						nType == Constants::c_nGraduateClock3AnglesClockwise || 
						nType == Constants::c_nGraduateClock2AnglesClockwise )
					{
						nSignAngle = -1;
					}

					DWORD* pResult  = (DWORD*)pBGRAResult;
					DWORD* pSource1 = (DWORD*)pBGRASource1;
					DWORD* pSource2 = (DWORD*)pBGRASource2;
					
					// combine two images
					int nTileY = 0;
					int nPatternY = 0;
					
					for( int nY = nHeight; nY > 0; --nY, ++nPatternY )
					{
						// compute pattern Y coordinate
						if( nPatternY >= nTileSizeY )
						{
							nPatternY = 0;
							nTileY += 1;
						}

						int nTileX = 0;
						int nPatternX = 0;
						
						for( int nX = nWidth; nX > 0; --nX, ++nPatternX, ++pResult, ++pSource1, ++pSource2 )
						{
							// compute pattern X coordinate
							if( nPatternX >= nTileSizeX )
							{
								nPatternX = 0;
								nTileX += 1;
							}
							
							int nTileIndex = nTileX + nTilesX * nTileY;

							// check whether we reach new tile
							if( nTileIndex != nTileOldIndex )
							{
								nTileOldIndex = nTileIndex;

								dAngle = IPP_PI * nAngleFactor * pGraduate->GetLevel( nTileIndex, dCompleteness );
							}

							double dA = ::atan2( nPatternY - dTileCenterY, nPatternX - dTileCenterX );

							dA -= dStartAngle;
							dA *= nSignAngle;
							
							if( dA < 0 )
								dA += IPP_2PI;
								
							// reset pattern value
							int nPatternValue = 0;

							if ( (dA > IPP_2PI - dAngle) && (nAngleFactor == 1) ) 
							{
								nPatternValue = 1;
							}
							else
							{
								for( int nSectorIndex = nParts; nSectorIndex > 0; --nSectorIndex, dA -= dSector )
								{
									if( dA > 0 && dA < dAngle )
									{	
										nPatternValue = 1;
										break;
									}
								}
							}

							// combine colors
							pResult[0] = nPatternValue ? pSource2[0] : pSource1[0];
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_Twirl(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness )
				{
					BOOL bIsClockwise = TRUE;
					int  nZoom = 0;
					BOOL bDrain = FALSE;

					switch (nType)
					{
					case Constants::c_nTwirlClockwise:
						 bIsClockwise = TRUE;
						 nZoom = 0;
						 break;
					case Constants::c_nTwirlClockwiseZoomOut:
						 bIsClockwise = TRUE;
						 nZoom = 1;
						 break;
					case Constants::c_nTwirlClockwiseZoomIn:
						 bIsClockwise = TRUE;
						 nZoom = 2;
						 break;
					case Constants::c_nTwirlReverse:
						 bIsClockwise = FALSE;
						 nZoom = 0;
						 break;
					case Constants::c_nTwirlReverseZoomOut:
						 bIsClockwise = FALSE;
						 nZoom = 1;
						 break;
					case Constants::c_nTwirlReverseZoomIn:
						 bIsClockwise = FALSE;
						 nZoom = 2;
						 break;
					case Constants::c_nTwirlDrain:
						 bDrain = TRUE; 
						 bIsClockwise = TRUE;
						 break;
					case Constants::c_nTwirlReverseDrain:
						 bDrain = TRUE; 
						 bIsClockwise = FALSE;
						 break;
					}

					if( bDrain )
						return IPP::IPP_TwirlDrain( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, bIsClockwise );
					else
						return IPP::IPP_Twirl( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, bIsClockwise, nZoom );
				}
				BOOL BGRA_Sphere(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					return IPP::IPP_Sphere(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, (Constants::c_nSphereInside == nType));
				}
				BOOL BGRA_Twist(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					int dir = 0; 
					int forward = -1;

					switch( nType )
					{
					case Constants::c_nTwistLeftToRight: dir = 1; forward = 1; break;
					case Constants::c_nTwistRightToLeft: dir = 1; forward = 0; break;
					case Constants::c_nTwistTopToBottom: dir = 0; forward = 0; break;
					case Constants::c_nTwistBottomToTop: dir = 0; forward = 1; break;
					case Constants::c_nTwistLeftTop:     dir = 0; break;
					case Constants::c_nTwistLeftBottom:  dir = 1; break;
					case Constants::c_nTwistRightTop:    dir = 2; break;
					case Constants::c_nTwistRightBottom: dir = 3; break;
					default: 
						return FALSE;
					}

					if( forward < 0 )
						return IPP::IPP_TwistDiagonal( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, dir );
					else
						return IPP::IPP_TwistLinear( pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, dir, forward );
				}
				BOOL BGRA_Ripple(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					return IPP::IPP_Ripple(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, (Constants::c_nRippleOutside == nType));
				}
				BOOL BGRA_Wave(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					return IPP::IPP_Wave(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, dCompleteness, (Constants::c_nWaveHorizontal == nType));
				}
				BOOL BGRA_WipeFadeSide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int		nX, nY, nIndex;
					int		nPatternX, nPatternY, nPatternValue;
					int		nLevelLast, nLevelFirst;
					double  dLevel = dCompleteness * 1.25;
					int		nAddFadeLevel = 0;	
					int		nFadeSize = (nType == Constants::c_nFadeSideLeftToRight || nType == Constants::c_nFadeSideRightToLeft) ? (nTileSizeX)/4 : (nTileSizeY)/4;
					double	dAlfa;
					int		nCheckPoint = 0;
					int		nWidthInBytes = nWidth * 4;

					if (dLevel <= 0.25)
					{
						if (nType == Constants::c_nFadeSideLeftToRight || nType == Constants::c_nFadeSideRightToLeft)
						{
							nLevelFirst = (int)(dLevel * (nTileSizeX-1));
							nLevelLast = 0;
							nAddFadeLevel = nFadeSize - nLevelFirst;
						}
						else if (nType == Constants::c_nFadeSideBottomToTop || nType == Constants::c_nFadeSideTopToBottom)
						{
							nLevelFirst = (int)(dLevel * (nTileSizeY-1));
							nLevelLast = 0;
							nAddFadeLevel = nFadeSize - nLevelFirst;
						}
					}		
					else if (dLevel >= 1.)
					{
						if (nType == Constants::c_nFadeSideLeftToRight || nType == Constants::c_nFadeSideRightToLeft)
						{
							nLevelFirst = nTileSizeX-1;
							nLevelLast = (int)((dLevel-0.25) * (nTileSizeX-1));
						}
						else if (nType == Constants::c_nFadeSideBottomToTop || nType == Constants::c_nFadeSideTopToBottom)
						{
							nLevelFirst = nTileSizeY-1;
							nLevelLast = (int)((dLevel-0.25) * (nTileSizeY-1));
						}
					}
					else
					{
						if (nType == Constants::c_nFadeSideLeftToRight || nType == Constants::c_nFadeSideRightToLeft)
						{
							nLevelFirst = (int)(dLevel * (nTileSizeX-1));
							nLevelLast = nLevelFirst - nFadeSize;
						}
						else if (nType == Constants::c_nFadeSideBottomToTop || nType == Constants::c_nFadeSideTopToBottom)
						{
							nLevelFirst = (int)(dLevel * (nTileSizeY-1));
							nLevelLast = nLevelFirst - nFadeSize;
						}
					}

					// compute starting index
					nIndex = 0;
					if (nType == Constants::c_nFadeSideBottomToTop || nType == Constants::c_nFadeSideTopToBottom)
					{
						// combine two images
						for (nY = 0; nY < nHeight; ++nY)
						{
							// compute pattern Y coordinate
							nPatternY = nY % nTileSizeY;
							nPatternValue = 0;
						
							nCheckPoint = (nType == Constants::c_nFadeSideBottomToTop) ? nPatternY : (nTileSizeY-1 - nPatternY);
							if (nCheckPoint < nLevelLast)
								nPatternValue = 1;
							else if (nCheckPoint <= nLevelFirst)
							{
								nPatternValue = 2;
								dAlfa = (double)(nCheckPoint - nLevelLast + nAddFadeLevel)/nFadeSize;
							}
							for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
							{
								// combine colors
								if (nPatternValue == 0)
								{
									pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
								}
								else if (nPatternValue == 1)
								{
									pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
								} 
								else
								{
									pBGRAResult[nIndex + 2] = (BYTE)(pBGRASource1[nIndex + 2]*dAlfa + pBGRASource2[nIndex + 2]*(1-dAlfa));
									pBGRAResult[nIndex + 1] = (BYTE)(pBGRASource1[nIndex + 1]*dAlfa + pBGRASource2[nIndex + 1]*(1-dAlfa));
									pBGRAResult[nIndex + 0] = (BYTE)(pBGRASource1[nIndex + 0]*dAlfa + pBGRASource2[nIndex + 0]*(1-dAlfa));
								}
							}
						}
					}
					else if (nType == Constants::c_nFadeSideLeftToRight || nType == Constants::c_nFadeSideRightToLeft)
					{
						for (nX = 0; nX < nWidth; ++nX)
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;
							nPatternValue = 0;

							// compute color value
							nCheckPoint = (nType == Constants::c_nFadeSideLeftToRight) ? nPatternX : (nTileSizeX-1 - nPatternX);	
							if (nCheckPoint < nLevelLast)
								nPatternValue = 1;
							else if (nCheckPoint <= nLevelFirst)
							{
								nPatternValue = 2;
								dAlfa = (double)(nCheckPoint - nLevelLast + nAddFadeLevel)/nFadeSize;
							}

							nIndex = nX*4;
							for (nY = 0; nY < nHeight; ++nY)
							{
								// combine colors
								if (nPatternValue == 0)
								{
									pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
								}
								else if (nPatternValue == 1)
								{
									pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
									pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
									pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
								} 
								else
								{
									pBGRAResult[nIndex + 2] = (BYTE)(pBGRASource1[nIndex + 2]*dAlfa + pBGRASource2[nIndex + 2]*(1-dAlfa));
									pBGRAResult[nIndex + 1] = (BYTE)(pBGRASource1[nIndex + 1]*dAlfa + pBGRASource2[nIndex + 1]*(1-dAlfa));
									pBGRAResult[nIndex + 0] = (BYTE)(pBGRASource1[nIndex + 0]*dAlfa + pBGRASource2[nIndex + 0]*(1-dAlfa));
								}
								nIndex += nWidthInBytes;
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_WipeClockCorner(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					// variables
					int		nPatternX, nPatternY;
					double  dMaxTan;
					double  dTan;
					int		nCenterY = 0;
					int		nCenterX = 0;

					if( nType == Constants::c_nClockCornerReverseLeftBottom || 
						nType == Constants::c_nClockCornerLeftTop ||
						nType == Constants::c_nClockCornerRightBottom || 
						nType == Constants::c_nClockCornerReverseRightTop )
					{
						dMaxTan = tan(IPP_PI2 * (1 - dCompleteness));
					}
					else
					{
						dMaxTan = tan(IPP_PI2 * dCompleteness);
					}

					
					if( nType == Constants::c_nClockCornerLeftBottom || 
						nType == Constants::c_nClockCornerReverseLeftBottom )
					{
						nCenterY = 0;
						nCenterX = 0;
					}
					else 
					if( nType == Constants::c_nClockCornerLeftTop || 
						nType == Constants::c_nClockCornerReverseLeftTop )
					{
						nCenterX = 0;
						nCenterY = nTileSizeY - 1;
					}
					else 
					if( nType == Constants::c_nClockCornerRightBottom || 
						nType == Constants::c_nClockCornerReverseRightBottom )
					{
						nCenterX = nTileSizeX - 1;
						nCenterY = 0;
					}
					else 
					if( nType == Constants::c_nClockCornerRightTop || 
						nType == Constants::c_nClockCornerReverseRightTop )
					{
						nCenterX = nTileSizeX - 1;	
						nCenterY = nTileSizeY - 1;
					}

					BOOL bCaseValue = TRUE;
					double dSign = 1;
					
					if (nType == Constants::c_nClockCornerLeftBottom || 
						nType == Constants::c_nClockCornerReverseLeftTop ||
						nType == Constants::c_nClockCornerReverseRightBottom || 
						nType == Constants::c_nClockCornerRightTop)
					{
						bCaseValue = FALSE;
						dSign = -1;
					}

					dMaxTan *= dSign;

					DWORD* pDst  = (DWORD*)pBGRAResult;
					DWORD* pSrc1 = (DWORD*)pBGRASource1;
					DWORD* pSrc2 = (DWORD*)pBGRASource2;

					// combine two images
					for( int nY = 0; nY < nHeight; ++nY )
					{
						// compute pattern Y coordinate
						nPatternY = nY % nTileSizeY;

						for( int nX = 0; nX < nWidth; ++nX, ++pDst, ++pSrc1, ++pSrc2 )
						{
							// compute pattern X coordinate
							nPatternX = nX % nTileSizeX;

							// reset pattern value
							BOOL bPatternValue = FALSE;

							// compute color value
							if( nPatternX != nCenterX )
							{
								dTan = fabs((double)(nPatternY - nCenterY)/(nPatternX - nCenterX));
								if ( dSign * dTan > dMaxTan )
									bPatternValue = 1;
							}
							else
							{
								bPatternValue = bCaseValue;
							}
							
							// combine colors
							pDst[0] = bPatternValue ? pSrc2[0] : pSrc1[0];
						}
					}

					// all ok
					return TRUE;
				}
				BOOL BGRA_FlipOutIn(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nFlipFlyVerticalOut || nType == Constants::c_nFlipFlyVerticalIn)
						return IPP::IPP_FlipAwayOutInVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipFlyVerticalIn));
					
					if (nType == Constants::c_nFlipFlyHorizontalOut || nType == Constants::c_nFlipFlyHorizontalIn)
						return IPP::IPP_FlipAwayOutInHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipFlyHorizontalIn));

					return FALSE;
				}
				BOOL BGRA_Fold(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nStretchFoldTopToBottom || nType == Constants::c_nStretchFoldBottomToTop)
						return IPP::IPP_FoldVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nStretchFoldBottomToTop));
					
					if (nType == Constants::c_nStretchFoldLeftToRight || nType == Constants::c_nStretchFoldRightToLeft)
						return IPP::IPP_FoldHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nStretchFoldLeftToRight));

					return FALSE;
				}
				BOOL BGRA_FlipBoxes(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nFlipBoxesTopToBottom || nType == Constants::c_nFlipBoxesBottomToTop)
						return IPP::IPP_FlipBoxesVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipBoxesTopToBottom));

					if ( nType == Constants::c_nFlipBoxesLeftToRight || nType == Constants::c_nFlipBoxesRightToLeft)
						return IPP::IPP_FlipBoxesHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipBoxesLeftToRight));

					return FALSE;
				}
				BOOL BGRA_FlipOutBoxes(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (nType == Constants::c_nFlipOutBoxesVerticalBack || nType == Constants::c_nFlipOutBoxesVertical)
						return IPP::IPP_FlipOutBoxesVertical(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipOutBoxesVertical));

					if (nType == Constants::c_nFlipOutBoxesHorizontalBack || nType == Constants::c_nFlipOutBoxesHorizontal)
						return IPP::IPP_FlipOutBoxesHorizontal(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, dCompleteness, (nType == Constants::c_nFlipOutBoxesHorizontal));

					return FALSE;
				}


				//BOOL BGRA_WipePathCommon(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileCountX, int nTileCountY, GraphicsPath* path,double dCompleteness)
				//{
				//	const int c_nMaskSize = 100;
				//	
				//	if (nTileCountY<1 || nTileCountX<1 || !path)
				//		return FALSE;
				//	Matrix decM;
				//	decM.Translate(-c_nMaskSize / 2, -c_nMaskSize / 2);

				//	GraphicsPath* decPath = path->Clone();

				//	decPath->Transform(&decM);

				//	decM.Reset();
				//	decM.Scale(0.9f,0.9f);
				//	decPath->Transform(&decM);

				//	decM.Reset();
				//	decM.Translate(c_nMaskSize / 2, c_nMaskSize / 2);
				//	decPath->Transform(&decM);

				//	Bitmap bmp( c_nMaskSize, c_nMaskSize, PixelFormat24bppRGB );
				//	Graphics gr(&bmp);
				//	gr.Clear(Color(0,0,0));

				//	SolidBrush brsh(Color(0,255,0));
				//	gr.FillPath(&brsh, path);
				//	SolidBrush brsh2(Color(255,0,0));
				//	gr.FillPath(&brsh2, decPath);

				//	delete decPath;

				//	BitmapData oBitmapData;

				//	if( Ok != bmp.LockBits(NULL, ImageLockModeRead, PixelFormat24bppRGB, 
				//		&oBitmapData) || !oBitmapData.Scan0)
				//		return FALSE;

				//	BYTE* maskPixels = (BYTE*)oBitmapData.Scan0;
				//	int stride = oBitmapData.Stride;

				//	double tmpVal = 5.0*(1.0-dCompleteness)/*/2.0*/;

				//	double  pathRectProportions[4];
				//	pathRectProportions[0] = c_nMaskSize*(0.5-tmpVal);// left
				//	pathRectProportions[1] = c_nMaskSize*(0.5+tmpVal);// top
				//	pathRectProportions[2] = c_nMaskSize*(0.5+tmpVal);// right
				//	pathRectProportions[3] = c_nMaskSize*(0.5-tmpVal);// bottom

				//	double  pathRectProportionsWidth = pathRectProportions[2]-pathRectProportions[0];//right-left
				//	double  pathRectProportionsHeight = pathRectProportions[1]-pathRectProportions[3];//top-bottom

				//	int     TileXSizePix = nWidth/nTileCountX;
				//	int     TileYSizePix = nHeight/nTileCountY;

				//	int		nX, nY, nIndex;
				//	nIndex = 0;
				//	// combine two images
				//	for (nY = 0; nY < nHeight; ++nY)
				//	{
				//		// compute pattern Y coordinate
				//		double PatternY = pathRectProportions[3]+(nY % TileYSizePix)*pathRectProportionsHeight/TileYSizePix;

				//		for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
				//		{
				//			// compute pattern X coordinate
				//			double PatternX = pathRectProportions[0]+(nX % TileXSizePix)*pathRectProportionsWidth/TileXSizePix;

				//			Color tmpCol;
				//			int  nTempIndex = (((int)PatternY)*stride)+(((int)PatternX)*3);
				//			// combine colors
				//			if (PatternX>0 && PatternX<c_nMaskSize && PatternY>0 && PatternY<c_nMaskSize)
				//			{
				//				if (maskPixels[nTempIndex+2]==255 && maskPixels[nTempIndex+1]==0)
				//				{
				//					pBGRAResult[nIndex + 2] = pBGRASource2[nIndex + 2];
				//					pBGRAResult[nIndex + 1] = pBGRASource2[nIndex + 1];
				//					pBGRAResult[nIndex + 0] = pBGRASource2[nIndex + 0];
				//				}
				//				else
				//					if (maskPixels[nTempIndex]==0 && maskPixels[nTempIndex+1]==255)
				//					{
				//						pBGRAResult[nIndex + 2] = (pBGRASource2[nIndex + 2]+pBGRASource1[nIndex + 2])/2;
				//						pBGRAResult[nIndex + 1] = (pBGRASource2[nIndex + 1]+pBGRASource1[nIndex + 1])/2;
				//						pBGRAResult[nIndex + 0] = (pBGRASource2[nIndex + 0]+pBGRASource1[nIndex + 0])/2;
				//					}
				//					else
				//					{
				//						pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
				//						pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
				//						pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
				//					}
				//			}
				//			else
				//			{
				//				pBGRAResult[nIndex + 2] = pBGRASource1[nIndex + 2];
				//				pBGRAResult[nIndex + 1] = pBGRASource1[nIndex + 1];
				//				pBGRAResult[nIndex + 0] = pBGRASource1[nIndex + 0];
				//			}
				//		}
				//	}

				//	bmp.UnlockBits(&oBitmapData);

				//	return TRUE;
				//}

				//BOOL BGRA_DrawSmoothCircle( BYTE* pResult, BYTE* pSource, int nWidth, int nHeight, int nStride, double dCenterX, double dCenterY, double dSqrRadiusIn, double dSqrRadiusOut )
				//{
				//	double dFactor = 256 / (dSqrRadiusOut - dSqrRadiusIn);

				//	nStride -= nWidth * 4;

				//	for( int nRow = 0; nRow < nHeight; ++nRow, pResult += nStride, pSource += nStride )
				//	{
				//		double dY = (nRow - dCenterY) * (nRow - dCenterY);

				//		for( int nPos = 0; nPos < nWidth; ++nPos, pResult += 4, pSource += 4 )
				//		{
				//			double dot = (nPos - dCenterX) * (nPos - dCenterX) + dY;
				//			
				//			if( dot <= dSqrRadiusIn )
				//			{
				//				*((int*)pResult) = *((int*)pSource);
				//				continue;
				//			}

				//			if( dot <= dSqrRadiusOut )
				//			{
				//				int alpha = int((dot - dSqrRadiusIn) * dFactor + 0.5);
				//				pResult[0] = AlphaBlend( pSource[0], pResult[0], alpha );
				//				pResult[1] = AlphaBlend( pSource[1], pResult[1], alpha );
				//				pResult[2] = AlphaBlend( pSource[2], pResult[2], alpha );
				//				pResult[3] = AlphaBlend( pSource[3], pResult[3], alpha );
				//			}
				//		}
				//	}
				//	
				//	return TRUE;
				//}				
				//BOOL BGRA_DrawSmoothDiamond( BYTE* pResult, BYTE* pSource, int nWidth, int nHeight, int nStride, double dCenterX, double dCenterY, double dW, double dH )
				//{
				//	const double c_dFactor = 1.21;

				//	Geometry::CDot oCenter( dCenterX, dCenterY );


				//	Geometry::CTriangle oPolyIn(
				//		oCenter.x - dW, 
				//		oCenter.y, 
				//		oCenter.x, 
				//		oCenter.y - dH, 
				//		oCenter.x + dW, 
				//		oCenter.y
				//	);
				//	Geometry::CTriangle oPolyOut(
				//		oCenter.x - dW * c_dFactor,
				//		oCenter.y, 
				//		oCenter.x, 
				//		oCenter.y - dH * c_dFactor, 
				//		oCenter.x + dW * c_dFactor, 
				//		oCenter.y
				//	);
				//	
				//	Geometry::CDot oPixel( 0, 0 );

				//	nStride -= nWidth * 4;

				//	int nRow = min(int(oCenter.y), nHeight);
				//	for( int i = 0; i < 2; i++ )
				//	{
				//		for( ; nRow > 0; --nRow, oPixel.y += 1, pResult += nStride, pSource += nStride )
				//		{
				//			oPixel.x = 0;
				//			for( int nPos = nWidth; nPos > 0; --nPos, oPixel.x += 1, pResult += 4, pSource += 4 )
				//			{
				//				if( !oPolyOut.IsInnerPoint( oPixel ) )
				//					continue;

				//				if( oPolyIn.IsInnerPoint( oPixel ) )
				//				{
				//					*((int*)pResult) = *((int*)pSource);
				//					continue;
				//				}

				//				Geometry::CDot* p1 = NULL;
				//				Geometry::CDot* p2 = NULL;
				//				
				//				if( oPixel.x <= oCenter.x )
				//				{
				//					p1 = &oPolyIn. v1;
				//					p2 = &oPolyOut.v1;
				//				}
				//				else
				//				{
				//					p1 = &oPolyIn .v3;
				//					p2 = &oPolyOut.v3;
				//				}

				//				Geometry::CDot oInPt;
				//				Geometry::CDot oOutPt;

				//				oInPt .IntersectLines( *p1, oPolyIn.v2,  oPixel, oCenter );
				//				oOutPt.IntersectLines( *p2, oPolyOut.v2, oPixel, oCenter );

				//				oOutPt.x -= oInPt.x;
				//				oOutPt.y -= oInPt.y;
				//				oInPt.x   = oPixel.x - oInPt.x;
				//				oInPt.y   = oPixel.y - oInPt.y;
				//				
				//				int alpha = int(oInPt.SqrLength() / oOutPt.SqrLength() * 256 + 0.5);

				//				pResult[0] = AlphaBlend( pSource[0], pResult[0], alpha );
				//				pResult[1] = AlphaBlend( pSource[1], pResult[1], alpha );
				//				pResult[2] = AlphaBlend( pSource[2], pResult[2], alpha );
				//				pResult[3] = AlphaBlend( pSource[3], pResult[3], alpha );
				//			}
				//		}

				//		nRow = min(int(nHeight - oCenter.y), nHeight);

				//		oPolyIn.v2.y = oCenter.y + dH;
				//		oPolyIn.bClockWise = FALSE;
				//		
				//		oPolyOut.v2.y = oCenter.y + dH * c_dFactor;
				//		oPolyOut.bClockWise = FALSE;
				//	}

				//	return TRUE;
				//}				
				//BOOL BGRA_WipePathConcrete(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, Utility::CUtility* pUtility)
				//{
				//	double dSqrRadiusIn  = 0;
				//	double dSqrRadiusOut = 0;

				//	GraphicsPath curPath;
				//	switch (nType)
				//	{
				//	case Constants::c_nWipeSmoothCircle: 
				//		::memcpy( pBGRAResult, pBGRASource1, nWidth*nHeight*4 );

				//		dSqrRadiusIn  = (nWidth * nWidth + nHeight * nHeight) * dCompleteness * dCompleteness / 4;
				//		dSqrRadiusOut = dSqrRadiusIn * 1.44;

				//		return BGRA_DrawSmoothCircle( pBGRAResult, pBGRASource2, nWidth, nHeight, nWidth*4, nWidth / 2.0, nHeight / 2.0, dSqrRadiusIn, dSqrRadiusOut );
				//	
				//	case Constants::c_nWipeSmoothCircles: 
				//		::memcpy( pBGRAResult, pBGRASource1, nWidth*nHeight*4 );
				//		
				//		if( TRUE )
				//		{
				//			double dDiameter = max(nWidth, nHeight) / 4.5;
				//			dSqrRadiusIn  = 2 * dDiameter * dDiameter * dCompleteness * dCompleteness / 4;
				//			dSqrRadiusOut = dSqrRadiusIn * 1.44;
				//			double dMaxRadius = sqrt( dSqrRadiusOut );

				//			int nHorCircles = int(nWidth  / dDiameter) + 1;
				//			int nVerCircles = int(nHeight / dDiameter) + 1;
				//			double dStartX = (nWidth  - nHorCircles * dDiameter) / 2;
				//			double dStartY = (nHeight - nVerCircles * dDiameter) / 2;
				//			double dCenterX = dStartX + dDiameter / 2;
				//			double dCenterY = dStartY + dDiameter / 2;

				//			for( int nRow = 0; nRow < nVerCircles; ++nRow )
				//			{
				//				double dY = dCenterY + nRow * dDiameter;

				//				int y1 = int(dY - dMaxRadius) - 1;
				//				int y2 = int(dY + dMaxRadius) + 1;
				//				
				//				if( y1 < 0 ) y1 = 0;
				//				else if( y1 >= nHeight ) y1 = nHeight - 1;

				//				if( y2 < 0 ) y2 = 0;
				//				else if( y2 >= nHeight ) y2 = nHeight - 1;

				//				int height = y2 - y1 + 1;
				//				
				//				dY -= y1;

				//				for( int nPos = 0; nPos < nHorCircles; ++nPos )
				//				{
				//					double dX = dCenterX + nPos * dDiameter;

				//					int x1 = int(dX - dMaxRadius) - 1;
				//					int x2 = int(dX + dMaxRadius) + 1;

				//					if( x1 < 0 ) x1 = 0;
				//					else if( x1 >= nWidth ) x1 = nWidth - 1;

				//					if( x2 < 0 ) x2 = 0;
				//					else if( x2 >= nWidth ) x2 = nWidth - 1;

				//					int width = x2 - x1 + 1;
				//					int offset = (y1 * nWidth + x1) * 4;

				//					dX -= x1;

				//					BGRA_DrawSmoothCircle( pBGRAResult + offset, pBGRASource2 + offset, width, height, nWidth*4, dX, dY, dSqrRadiusIn, dSqrRadiusOut );
				//				}
				//			}

				//			return TRUE;
				//		}
				//		break;
				//	
				//	case Constants::c_nWipeSmoothDiamond:
				//		::memcpy( pBGRAResult, pBGRASource1, nWidth*nHeight*4 );

				//		if( TRUE )
				//		{
				//			double dW = (nHeight * 4 / 3.0 + nWidth)  / 2 * dCompleteness;
				//			double dH = (nWidth  * 3 / 4.0 + nHeight) / 2 * dCompleteness;
				//			
				//			return BGRA_DrawSmoothDiamond( pBGRAResult, pBGRASource2, nWidth, nHeight, nWidth * 4, nWidth / 2.0, nHeight / 2.0, dW, dH );
				//		}
				//	
				//	case Constants::c_nWipeSmoothDiamonds:
				//		::memcpy( pBGRAResult, pBGRASource1, nWidth*nHeight*4 );
				//		
				//		if( TRUE )
				//		{
				//			const int nHorDiamonds = 5;
				//			const int nVerDiamonds = 5;
				//			const double dPartWidth  = double(nWidth)  / nHorDiamonds / 2;
				//			const double dPartHeight = double(nHeight) / nVerDiamonds / 2;
				//			const double dW = 2 * dPartWidth  * dCompleteness;
				//			const double dH = 2 * dPartHeight * dCompleteness;

				//			double dCenterX = dPartWidth;
				//			double dCenterY = dPartHeight;

				//			for( int nRow = 0; nRow < nVerDiamonds; ++nRow )
				//			{
				//				double dY = dCenterY + (nRow * 2) * dPartHeight;

				//				int y1 = int(dY - dPartHeight) - 1;
				//				int y2 = int(dY + dPartHeight) + 1;
				//				
				//				if( y1 < 0 ) y1 = 0;
				//				else if( y1 >= nHeight ) y1 = nHeight - 1;

				//				if( y2 < 0 ) y2 = 0;
				//				else if( y2 >= nHeight ) y2 = nHeight - 1;

				//				int height = y2 - y1 + 1;
				//				
				//				dY -= y1;

				//				for( int nPos = 0; nPos < nHorDiamonds; ++nPos )
				//				{
				//					double dX = dCenterX + (nPos * 2) * dPartWidth;

				//					int x1 = int(dX - dPartWidth) - 1;
				//					int x2 = int(dX + dPartWidth) + 1;

				//					if( x1 < 0 ) x1 = 0;
				//					else if( x1 >= nWidth ) x1 = nWidth - 1;

				//					if( x2 < 0 ) x2 = 0;
				//					else if( x2 >= nWidth ) x2 = nWidth - 1;

				//					int width = x2 - x1 + 1;
				//					int offset = (y1 * nWidth + x1) * 4;

				//					dX -= x1;

				//					BGRA_DrawSmoothDiamond( pBGRAResult + offset, pBGRASource2 + offset, width, height, nWidth*4, dX, dY, dW, dH );
				//				}
				//			}

				//			return TRUE;
				//		}
				//		break;
				//	case Constants::c_nWipeSmoothStar:
				//	case Constants::c_nWipeSmoothStars:
				//		{
				//			const int start_angle = 90;
				//			const int anglesCnt = 5;
				//			const int radius		= 45;
				//			const int inner_radius  = 15;
				//			const int center_x		= 50;
				//			const int center_y		= 50;

				//			PointF* points = new PointF[anglesCnt*2];

				//			int index = 0;

				//			for (int a = start_angle; a < 360 + start_angle; a += 180/anglesCnt)
				//			{
				//				if (index >= anglesCnt*2) 
				//					break;
				//				points[index] = PointF(
				//					float(center_x + (int)(((index % 2 == 0) ? radius : inner_radius) * cos(IPP_PI180 * a))),
				//					float(center_y + (int)(((index % 2 == 0) ? radius : inner_radius) * sin(IPP_PI180 * a))));
				//				index++;
				//			}

				//			curPath.AddLines(points,anglesCnt*2);

				//			curPath.CloseFigure();

				//			delete[] points;
				//		}
				//		break;
				//	case Constants::c_nWipeSmoothKeyHole:
				//	case Constants::c_nWipeSmoothKeyHoles:
				//		curPath.AddLine(30, 10, 40, 45);
				//		curPath.AddBezier(40,45, 20, 90, 80,90, 60,45);
				//		curPath.AddLine(60, 45, 70, 10);
				//		curPath.CloseFigure();
				//		break;
				//	default:
				//		return FALSE;
				//	}
				//	
				//	BGRA_WipePathCommon(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY,&curPath,dCompleteness);
				//	return TRUE;
				//}
			}
		}
	}
}
