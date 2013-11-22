#pragma once

#include <ImagePaintComposeUtility.h>

// TODO: 
// все функции в данном файле не провер€ют указатели и какие-либо параметры на валидность
// соответственно, использовать с осторожностью

#pragma warning(disable: 4244)

namespace ImageStudio
{
	namespace Paint
	{
		namespace Compose
		{
			int AlphaBlend( BYTE* pResult, BYTE* pDst, BYTE* pSrc, int nPixels, double dAlpha )
			{
				dAlpha = min(max(0, dAlpha), 1);

				DWORD nSrcAlpha = DWORD(dAlpha * 255 + 0.5);
				DWORD nDstAlpha = 255 - nSrcAlpha;

				nSrcAlpha *= 0x8081;
				nDstAlpha *= 0x8081;

				for( int i = 0; i < nPixels; ++i, pSrc += 4, pDst += 4, pResult += 4 )
				{
					pResult[0] = (pSrc[0] * nSrcAlpha + pDst[0] * nDstAlpha + (128 * 0x8081)) >> 23;
					pResult[1] = (pSrc[1] * nSrcAlpha + pDst[1] * nDstAlpha + (128 * 0x8081)) >> 23;
					pResult[2] = (pSrc[2] * nSrcAlpha + pDst[2] * nDstAlpha + (128 * 0x8081)) >> 23;
				}

				//for( int i = 0; i < nPixels; ++i, pSrc += 4, pDst += 4, pResult += 4 )
				//{
				//	DWORD a1 = pDst[3] * nDstAlpha;
				//	DWORD a2 = pSrc[3] * nSrcAlpha;
				//	
				//	pResult[0] = BYTE((pSrc[0] * a2 + pDst[0] * a1 + 32513) / 65025);
				//	pResult[1] = BYTE((pSrc[1] * a2 + pDst[1] * a1 + 32513) / 65025);
				//	pResult[2] = BYTE((pSrc[2] * a2 + pDst[2] * a1 + 32513) / 65025);
				//	pResult[3] = BYTE((a2 + a1 + 128) * 0x8081 >> 23);
				//}

				return 0;
			}

			
			namespace IPP
			{
				namespace Constants
				{
					//const int c_nTwirlStepCount = 5;
					//const double c_dTwirlStepBeginFrame[5] = {0.0,0.3,0.5,0.7,1.0};

					//const int c_nSphereStepCount = 5;
					//const double c_dSphereStepBeginFrame[5] = {0.0,0.3,0.5,0.7,1.0};

					//const int c_nRippleStepCount = 4;
					//const double c_dRippleStepBeginFrame[4] = {0.0,0.2,0.8,1.0};

					//const int c_nWaveStepCount = 4;
					//const double c_dWaveStepBeginFrame[4] = {0.0,0.2,0.8,1.0};

					//const int c_nGaussianBlurStepCount = 5;
					//const double c_dGaussianBlurStepBeginFrame[5] = {0.0,0.3,0.5,0.7,1.0};

					//const int c_nNoiseStepCount = 5;
					//const double c_dNoiseStepBeginFrame[5] = {0.0,0.4,0.5,0.6,1.0};
				}

				BOOL IPP_ResizeCenter( BYTE* pBGRASource, BYTE* pBGRAResult, int nWidth, int nHeight, double dXFactor, double dYFactor, double dXCenter, double dYCenter)
				{
					if( abs(dXFactor) < 0.0001 || abs(dYFactor) < 0.0001 )
						return TRUE;

					int offset = 4 * nWidth * (nHeight - 1);

					Gdiplus::Bitmap oSourceImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBGRASource + offset );
					if( Gdiplus::Ok != oSourceImage.GetLastStatus() )
						return FALSE;

					Gdiplus::Bitmap oResultImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBGRAResult + offset );
					if( Gdiplus::Ok != oResultImage.GetLastStatus() )
						return FALSE;
					
					Gdiplus::Graphics oRender( &oResultImage );
					if( Gdiplus::Ok != oRender.GetLastStatus() )
						return FALSE;
					
					oRender.SetInterpolationMode( Gdiplus::InterpolationModeBilinear );
					
					double dRectWidth  = nWidth  * dXFactor;
					double dRectHeight = nHeight * dYFactor;
					
					Gdiplus::RectF oDstRect( 
						float(dXCenter * (1 - dXFactor)),  
						float(dYCenter * (1 - dYFactor)), 
						float(dRectWidth), 
						float(dRectHeight)
					);

					oRender.DrawImage( &oSourceImage, oDstRect, 0, 0, nWidth, nHeight, Gdiplus::UnitPixel ); 
					
					return TRUE;
				}

				BOOL IPP_Rotate(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn, int nFirstAngle, BOOL bTransparent)
				{
					BYTE *pBuffer = new BYTE [4 * nWidth * nHeight];
					if( !pBuffer )
						return FALSE;

					double dInFrame = bIn ? dFrame : 1 - dFrame;
					double dAngle   = dInFrame * nFirstAngle;

					nSizeX = (nSizeX < 2) ? nWidth  : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiRect roiTileRect = {0, 0, nSizeX, nSizeY};
					IppiSize roiSize     = {nWidth,nHeight};
					
					int nHTileCount = nWidth  / nSizeX;
					int nVTileCount = nHeight / nSizeY;

					IppiSize roiRightBorderTile  = {nWidth % nSizeX, nSizeY};
					IppiSize roiBottomBorderTile = {nSizeX, nHeight % nSizeY};

					double dKoef = bIn ? 1 - dFrame : dFrame;
					LPBYTE pIn   = bIn ? pBGRASource2 : pBGRASource1;
					LPBYTE pOut  = bIn ? pBGRASource1 : pBGRASource2;
					
					memcpy( pBuffer, pIn, nWidth * nHeight * 4 );
					
					int roiScaleSizeW = max(1, nSizeX * dKoef);
					int roiScaleSizeH = max(1, nSizeY * dKoef);
					
					IppiSize roiScaleSize = {roiScaleSizeW, roiScaleSizeH};
					IppiRect roiScaleRect = {0, 0, roiScaleSizeW, roiScaleSizeH};
					
					Ipp8u *pScaleBuffer = new Ipp8u[roiScaleSize.width * roiScaleSize.height * 4];
					if( !pScaleBuffer )
					{
						delete [] pBuffer;
						return FALSE;
					}

					double xShift = (1 - dKoef) * nSizeX / 2.0;
					double yShift = (1 - dKoef) * nSizeY / 2.0;
					
					ippiAddRotateShift( roiScaleSize.width / 2.0, roiScaleSize.height / 2.0, dAngle, &xShift, &yShift);

					IppiRect srcROI = {0, 0, nSizeX, nSizeY};
					
					if( 0 == roiRightBorderTile.width )
					{
						for( int j = 0; j < nVTileCount; ++j, srcROI.y += nSizeY )
						{
							srcROI.x = 0;

							for( int i = 0; i < nHTileCount; ++i, srcROI.x += nSizeX )
							{
								ippiResize_8u_C4R(
									pOut, roiSize, 4*nWidth, srcROI,
									pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
									dKoef, dKoef, IPPI_INTER_LINEAR);

								ippiRotate_8u_C4R(
									pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiScaleRect,
									pBuffer + 4*(srcROI.x+nWidth*srcROI.y), 4*nWidth, roiTileRect,
									dAngle, xShift, yShift, IPPI_INTER_LINEAR );
							}
						}
					}
					else
					{
						IppiRect srcROIRightBorder = {nSizeX*nHTileCount,0,roiRightBorderTile.width,nSizeY};
						IppiRect roiRightBorderScaleRect = {0,0,roiRightBorderTile.width*dKoef,nSizeY*dKoef};
						IppiRect roiRightBorderTileRect = {0,0,roiRightBorderTile.width,nSizeY};

						double xRBShift = (1 - dKoef) * roiRightBorderTile.width / 2.0;
						double yRBShift = (1 - dKoef) * nSizeY / 2.0;
						
						ippiAddRotateShift( roiRightBorderScaleRect.width / 2.0, roiScaleSize.height / 2.0,	dAngle, &xRBShift, &yRBShift);

						for( int j = 0; j < nVTileCount; ++j, srcROI.y += nSizeY, srcROIRightBorder.y = srcROI.y )
						{
							srcROI.x = 0;
							for( int i = 0; i < nHTileCount; ++i, srcROI.x += nSizeX )
							{
								ippiResize_8u_C4R(
									pOut, roiSize, 4*nWidth, srcROI,
									pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
									dKoef, dKoef, IPPI_INTER_LINEAR);


								IppStatus iStatus = ippiRotate_8u_C4R(
									pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiScaleRect,
									pBuffer + 4*(srcROI.x+nWidth*srcROI.y), 4*nWidth, roiTileRect,
									dAngle, xShift, yShift, IPPI_INTER_LINEAR );
							}

							ippiResize_8u_C4R(
								pOut, roiSize, 4*nWidth, srcROIRightBorder,
								pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
								dKoef, dKoef, IPPI_INTER_LINEAR);

							IppStatus iStatus = ippiRotate_8u_C4R(
								pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiRightBorderScaleRect,
								pBuffer + 4*(srcROI.x+nWidth*srcROI.y), 4*nWidth, roiRightBorderTileRect,
								dAngle, xRBShift, yRBShift, IPPI_INTER_LINEAR );
						}
					}

					if( 0 != roiBottomBorderTile.height )
					{
						IppiRect srcROIBottomBorder = {0,nSizeY*nVTileCount,nSizeX,roiBottomBorderTile.height};
						IppiRect roiBottomBorderScaleRect = {0,0,nSizeX*dKoef,roiBottomBorderTile.height*dKoef};
						IppiRect roiBottomBorderTileRect = {0,0,nSizeX,roiBottomBorderTile.height};

						double xBBShift = (1 - dKoef) * nSizeX / 2.0;
						double yBBShift = (1 - dKoef) * roiBottomBorderTile.height / 2.0;
						
						ippiAddRotateShift( roiScaleSize.width / 2.0, roiBottomBorderScaleRect.height / 2.0, dAngle, &xBBShift, &yBBShift);

						for( int i = 0; i < nHTileCount; ++i, srcROIBottomBorder.x += nSizeX )
						{
							ippiResize_8u_C4R(
								pOut, roiSize, 4*nWidth, srcROIBottomBorder,
								pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
								dKoef, dKoef, IPPI_INTER_LINEAR);

							ippiRotate_8u_C4R(
								pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiBottomBorderScaleRect,
								pBuffer + 4*(srcROIBottomBorder.x+nWidth*srcROIBottomBorder.y), 4*nWidth, roiBottomBorderTileRect,
								dAngle, xBBShift, yBBShift, IPPI_INTER_LINEAR );
						}
						
						if( 0 != roiRightBorderTile.width )
						{
							srcROIBottomBorder.width = roiRightBorderTile.width;
							roiBottomBorderTileRect.width = roiRightBorderTile.width;
							roiBottomBorderScaleRect.width = roiRightBorderTile.width*dKoef;

							double xBBShift = (1 - dKoef) * roiRightBorderTile.width  / 2.0;
							double yBBShift = (1 - dKoef) * roiBottomBorderTile.height / 2.0;
							
							ippiAddRotateShift( roiBottomBorderScaleRect.width / 2.0, roiBottomBorderScaleRect.height / 2.0, dAngle, &xBBShift, &yBBShift);

							ippiResize_8u_C4R(
								pOut, roiSize, 4*nWidth, srcROIBottomBorder,
								pScaleBuffer, 4*roiScaleSize.width, roiScaleSize,
								dKoef, dKoef, IPPI_INTER_LINEAR);

							ippiRotate_8u_C4R(
								pScaleBuffer,roiScaleSize, 4*roiScaleSize.width, roiBottomBorderScaleRect,
								pBuffer + 4*(srcROIBottomBorder.x+nWidth*srcROIBottomBorder.y), 4*nWidth, roiBottomBorderTileRect,
								dAngle, xBBShift, yBBShift, IPPI_INTER_LINEAR );
						}
					}

					AlphaBlend( pBGRAResult, pBuffer, bIn ? pBGRASource2 : pBGRASource1, nWidth * nHeight, bTransparent ? (bIn ? dFrame : 1 - dFrame) : 0 );

					delete []pBuffer;
					delete []pScaleBuffer;
					return TRUE;
				}

				BOOL IPP_StretchDoorVertical(BYTE* pBGRASource1,BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;
					IppiSize roiSize = {nWidth,nHeight};

					Ipp8u *pA = NULL;
					Ipp8u *pB = NULL;
					Ipp8u *pResult = pBGRAResult;
					double dKoef = dFrame;

					if (bIn)
					{
						pA = pBGRASource2;
						pB = pBGRASource1;
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
						dKoef = 1.0- dFrame;
					}
					
					dKoef = floor(dKoef*nSizeY)/nSizeY;

					int nAScaledHeight = nSizeY*dKoef;
					int nBScaledHeight = nSizeY-(int)(nAScaledHeight/2)*2;
					double dKoefB = (double)(nBScaledHeight) / nSizeY;
					int nVTileCount = nHeight / nSizeY;

					int nBottomBorder = nHeight % nSizeY;
					IppiRect srcRectA1 = {0,0,nWidth,nSizeY/2};
					IppiRect srcRectB = {0,0,nWidth,nSizeY};
					IppiRect srcRectA2 = {0,nSizeY/2,nWidth,nSizeY - srcRectA1.height};
					IppiSize roiAScaleSize = {nWidth,nAScaledHeight/2};
					IppiSize roiBScaleSize = {nWidth,nBScaledHeight};


					Ipp8u *pTemp = pBGRAResult;
					for (int i=0;i<nVTileCount;i++)
					{
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRectA1,
							pResult, 4*nWidth, roiAScaleSize,
							1.0, dKoef, IPPI_INTER_LINEAR);
						srcRectA1.y += nSizeY;
						pTemp += 4*nWidth*roiAScaleSize.height;

						ippiResize_8u_C4R(
							pB, roiSize, 4*nWidth, srcRectB,
							pTemp, 4*nWidth, roiBScaleSize,
							1.0, /*1.0-dKoef*/dKoefB, IPPI_INTER_LINEAR);
						srcRectB.y += srcRectB.height;
						pTemp += 4*nWidth*roiBScaleSize.height;

						ippiResize_8u_C4R(
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
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRectA1,
							pResult, 4*nWidth, roiAScaleSize,
							1.0, dKoef, IPPI_INTER_LINEAR);
						pResult += 4*nWidth*roiAScaleSize.height;

						ippiResize_8u_C4R(
							pB, roiSize, 4*nWidth, srcRectB,
							pResult, 4*nWidth, roiBScaleSize,
							1.0, /*1.0-dKoef*/dKoefB, IPPI_INTER_LINEAR);
						pResult += 4*nWidth*roiBScaleSize.height;

						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRectA2,
							pResult, 4*nWidth, roiAScaleSize,
							1.0, dKoef, IPPI_INTER_LINEAR);			
					}
					return TRUE;
				}

				BOOL IPP_StretchDoorHorizontal(BYTE* pBGRASource1,BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;
					IppiSize roiSize = {nWidth,nHeight};

					Ipp8u *pA = NULL;
					Ipp8u *pB = NULL;
					Ipp8u *pResult = pBGRAResult;
					double dKoef = dFrame;

					if (bIn)
					{
						pA = pBGRASource2;
						pB = pBGRASource1;
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
						dKoef = 1.0- dFrame;
					}

					dKoef = floor(dKoef*nSizeX)/nSizeX;
					
					int nAScaledWidth = nSizeX*dKoef;
					int nBScaledWidth = nSizeX-(int)(nAScaledWidth/2)*2;
					int nHTileCount = nWidth / nSizeX;

					int nRightBorder = nWidth % nSizeX;
					IppiRect srcRectA1 = {0,0,nSizeX/2,nHeight};
					IppiRect srcRectB = {0,0,nSizeX,nHeight};
					IppiRect srcRectA2 = {nSizeX/2,0,nSizeX - nSizeX/2,nHeight};
					IppiSize roiAScaleSize = {nAScaledWidth/2,nHeight};
					IppiSize roiBScaleSize = {nBScaledWidth,nHeight};
					
					Ipp8u *pTemp = pBGRAResult;
					for (int i=0;i<nHTileCount;i++)
					{
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRectA1,
							pResult, 4*nWidth, roiAScaleSize,
							dKoef, 1.0, IPPI_INTER_LINEAR);
						srcRectA1.x += nSizeX;
						pTemp += 4*roiAScaleSize.width;

						ippiResize_8u_C4R(
							pB, roiSize, 4*nWidth, srcRectB,
							pTemp, 4*nWidth, roiBScaleSize,
							1.0-dKoef, 1.0, IPPI_INTER_LINEAR);
						srcRectB.x += srcRectB.width;
						pTemp += 4*roiBScaleSize.width;

						ippiResize_8u_C4R(
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
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRectA1,
							pResult, 4*nWidth, roiAScaleSize,
							dKoef, 1.0, IPPI_INTER_LINEAR);
						pResult += 4*roiAScaleSize.width;

						ippiResize_8u_C4R(
							pB, roiSize, 4*nWidth, srcRectB,
							pResult, 4*nWidth, roiBScaleSize,
							1.0-dKoef, 1.0, IPPI_INTER_LINEAR);
						pResult += 4*roiBScaleSize.width;

						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRectA2,
							pResult, 4*nWidth, roiAScaleSize,
							dKoef, 1.0, IPPI_INTER_LINEAR);			
					}
					return TRUE;
				}


				BOOL IPP_StretchStripsVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiSize roiSize = {nWidth,nHeight};
					nSizeX += nSizeX%2;

					int nHTileCount = nWidth / nSizeX;
					int nVTileCount = nHeight / nSizeY;

					Ipp8u *pA = pBGRASource1;
					Ipp8u *pB = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					for (int j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + 4*nWidth*j*nSizeY;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									pResult, 4*nWidth, roiAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	
							srcRect.x += srcRect.width;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*(nWidth * roiBScaleSize.height + srcRect.width)), 
									4*nWidth, roiAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	

							srcRect.x -= srcRect.width;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*nWidth*roiAScaleSize.height),
									4*nWidth, roiBScaleSize,
									1.0, dKoef1, IPPI_INTER_LINEAR);	
							srcRect.x += srcRect.width;
							ippiResize_8u_C4R(
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
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									pResult, 4*nWidth, roiRBorderAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	
							srcRBorderRect.x += srcRBorderRect.width;
							
							srcRBorderRect.width = nRBorder/2 + nRBorder%2;
							roiRBorderAScaleSize.width =nRBorder/2 + nRBorder%2;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*(nWidth * roiRBorderBScaleSize.height + nRBorder/2)), 
									4*nWidth, roiRBorderAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	
							roiRBorderAScaleSize.width =nRBorder/2;
							srcRBorderRect.width = nRBorder/2;
							

							srcRBorderRect.x = srcRect.x;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*nWidth*roiRBorderAScaleSize.height),
									4*nWidth, roiRBorderBScaleSize,
									1.0, dKoef1, IPPI_INTER_LINEAR);	
							srcRBorderRect.x += srcRBorderRect.width;

							srcRBorderRect.width = nRBorder/2 + nRBorder%2;
							roiRBorderBScaleSize.width +=nRBorder/2 + nRBorder%2;
							ippiResize_8u_C4R(
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
						pResult = pBGRAResult + 4*nWidth*(nHeight-nBBorder);
						roiBScaleSize.height = nBBorder*dFrame;
						roiAScaleSize.height = nBBorder - roiBScaleSize.height;
						srcRect.x = 0;
						srcRect.y = nHeight-nBBorder; 
						srcRect.height = nBBorder;
						dKoef2 = (float)roiAScaleSize.height/nBBorder;
						dKoef1 = (float)roiBScaleSize.height/nBBorder;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									pResult, 4*nWidth, roiAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	
							srcRect.x += srcRect.width;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*(nWidth * roiBScaleSize.height + srcRect.width)), 
									4*nWidth, roiAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	

							srcRect.x -= srcRect.width;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*nWidth*roiAScaleSize.height),
									4*nWidth, roiBScaleSize,
									1.0, dKoef1, IPPI_INTER_LINEAR);	
							srcRect.x += srcRect.width;
							ippiResize_8u_C4R(
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


							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									pResult, 4*nWidth, roiRBorderAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	
							srcRBorderRect.x += srcRBorderRect.width;

							srcRBorderRect.width = nRBorder/2 + nRBorder%2;
							roiRBorderAScaleSize.width =nRBorder/2 + nRBorder%2;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*(nWidth * roiRBorderBScaleSize.height + nRBorder/2)), 
									4*nWidth, roiRBorderAScaleSize,
									1.0, dKoef2, IPPI_INTER_LINEAR);	
							srcRBorderRect.width = nRBorder/2;
							roiRBorderAScaleSize.width =nRBorder/2;
							

							srcRBorderRect.x = srcRect.x;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*nWidth*roiRBorderAScaleSize.height),
									4*nWidth, roiRBorderBScaleSize,
									1.0, dKoef1, IPPI_INTER_LINEAR);	
							srcRBorderRect.x += srcRBorderRect.width;

							srcRBorderRect.width = nRBorder/2 + nRBorder%2;
							roiRBorderBScaleSize.width =nRBorder/2 + nRBorder%2;
							ippiResize_8u_C4R(
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
				BOOL IPP_StretchStripsHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					nSizeY += nSizeY%2;
					IppiSize roiSize = {nWidth,nHeight};

					int nHTileCount = nWidth / nSizeX;
					int nVTileCount = nHeight / nSizeY;

					Ipp8u *pA = pBGRASource1;
					Ipp8u *pB = pBGRASource2;

					IppiRect srcRect = {0,0,nSizeX,nSizeY/2};
					IppiSize roiBScaleSize = {nSizeX*dFrame,nSizeY/2};
					IppiSize roiAScaleSize = {nSizeX - roiBScaleSize.width,nSizeY/2};

					int nBBorder = nHeight % nSizeY;
					int nRBorder = nWidth % nSizeX;

					IppiSize roiRBorderBScaleSize = {nRBorder*dFrame,nSizeY/2};
					IppiSize roiRBorderAScaleSize = {nRBorder - roiRBorderBScaleSize.width,nSizeY/2};
					IppiRect srcRBorderRect = {0,0,nRBorder,nSizeY/2};
					double dKoef2 = (float)roiAScaleSize.width/nSizeX;
					double dKoef1 = (float)roiBScaleSize.width/nSizeX;
					double dKoef2r = (float)roiRBorderAScaleSize.width/max(1, nRBorder);
					double dKoef1r = (float)roiRBorderBScaleSize.width/max(1, nRBorder);

					Ipp8u *pResult = pBGRAResult;
					for (int j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + 4*nWidth*j*nSizeY;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									pResult, 4*nWidth, roiAScaleSize,
									dKoef2, 1.0, IPPI_INTER_LINEAR);	
							srcRect.y += srcRect.height;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*(nWidth * srcRect.height + roiBScaleSize.width)), 
									4*nWidth, roiAScaleSize,
									dKoef2, 1.0, IPPI_INTER_LINEAR);	

							srcRect.y -= srcRect.height;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*roiAScaleSize.width),
									4*nWidth, roiBScaleSize,
									dKoef1, 1.0, IPPI_INTER_LINEAR);	
							srcRect.y += srcRect.height;
							ippiResize_8u_C4R(
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
							srcRBorderRect.x = srcRect.x;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									pResult, 4*nWidth, roiRBorderAScaleSize,
									dKoef2r, 1.0,  IPPI_INTER_LINEAR);	
							srcRBorderRect.y += srcRBorderRect.height;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*(nWidth * srcRBorderRect.height + roiRBorderBScaleSize.width)), 
									4*nWidth, roiRBorderAScaleSize,
									dKoef2r, 1.0, IPPI_INTER_LINEAR);	
							

							srcRBorderRect.y -= srcRBorderRect.height;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*roiRBorderAScaleSize.width),
									4*nWidth, roiRBorderBScaleSize,
									dKoef1r, 1.0, IPPI_INTER_LINEAR);	
							srcRBorderRect.y += srcRBorderRect.height;
							ippiResize_8u_C4R(
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
						pResult = pBGRAResult + 4*nWidth*nVTileCount*nSizeY;
						roiBScaleSize.height = nBBorder/2;
						roiAScaleSize.height = nBBorder/2;
						srcRect.x = 0;
						srcRect.y = nVTileCount*nSizeY; 
						srcRect.height = nBBorder/2;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									pResult, 4*nWidth, roiAScaleSize,
									dKoef2, 1.0, IPPI_INTER_LINEAR);	
							srcRect.y += srcRect.height;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*(nWidth * srcRect.height + roiBScaleSize.width)), 
									4*nWidth, roiAScaleSize,
									dKoef2, 1.0, IPPI_INTER_LINEAR);	

							srcRect.y -= srcRect.height;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRect,
									(Ipp8u*)(pResult + 4*roiAScaleSize.width),
									4*nWidth, roiBScaleSize,
									dKoef1, 1.0, IPPI_INTER_LINEAR);	
							srcRect.y += srcRect.height;
							ippiResize_8u_C4R(
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

							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									pResult, 4*nWidth, roiRBorderAScaleSize,
									dKoef2r, 1.0, IPPI_INTER_LINEAR);	
							srcRBorderRect.y += srcRBorderRect.height;
							ippiResize_8u_C4R(
									pA, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*(nWidth * srcRBorderRect.height + roiRBorderBScaleSize.width)), 
									4*nWidth, roiRBorderAScaleSize,
									dKoef2r, 1.0, IPPI_INTER_LINEAR);	
							

							srcRBorderRect.y -= srcRBorderRect.height;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*roiRBorderAScaleSize.width),
									4*nWidth, roiRBorderBScaleSize,
									dKoef1r, 1.0, IPPI_INTER_LINEAR);	
							srcRBorderRect.y += srcRBorderRect.height;
							ippiResize_8u_C4R(
									pB, roiSize, 4*nWidth, srcRBorderRect,
									(Ipp8u*)(pResult + 4*nWidth*srcRBorderRect.height), 
									4*nWidth, roiRBorderBScaleSize,
									dKoef1r, 1.0, IPPI_INTER_LINEAR);	
						}
					}
					return TRUE;
				}
				BOOL IPP_StretchSideHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bLtoR)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiSize roiSize = {nWidth,nHeight};

					int nHTileCount = nWidth / nSizeX;

					Ipp8u *pA = pBGRASource1;
					Ipp8u *pB = pBGRASource2;
					double dKoefA = 1.0 - dFrame;
					double dKoefB = dFrame;
					if (bLtoR)
					{
						pA = pBGRASource2;
						pB = pBGRASource1;
						dKoefA = dFrame;
						dKoefB = 1.0 - dFrame;
					}


					IppiRect srcRect = {0,0,nSizeX,nHeight};
					IppiSize roiAScaleSize = {nSizeX*dKoefA,nHeight};
					IppiSize roiBScaleSize = {nSizeX - roiAScaleSize.width,nHeight};


					Ipp8u *pResult = pBGRAResult;
					for (int i=0;i<nHTileCount;i++)
					{
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRect,
							pResult, 4*nWidth, roiAScaleSize,
							dKoefA, 1.0, IPPI_INTER_LINEAR);	

						pResult += 4*roiAScaleSize.width;

						ippiResize_8u_C4R(
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
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRect,
							pResult, 4*nWidth, roiAScaleSize,
							dKoefA, 1.0, IPPI_INTER_LINEAR);	

						pResult += 4*roiAScaleSize.width;

						ippiResize_8u_C4R(
							pB, roiSize, 4*nWidth, srcRect,
							pResult, 4*nWidth, roiBScaleSize,
							dKoefB, 1.0, IPPI_INTER_LINEAR);	
					}
					return TRUE;
				}

				BOOL IPP_StretchSideVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bTtoB)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiSize roiSize = {nWidth,nHeight};

					int nVTileCount = nHeight / nSizeY;

					Ipp8u *pA = pBGRASource2;
					Ipp8u *pB = pBGRASource1;
					double dKoefA = dFrame;
					double dKoefB = 1.0 - dFrame;
					if (bTtoB)
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
						dKoefA = 1.0 - dFrame;
						dKoefB = dFrame;
					}


					IppiRect srcRect = {0,0,nWidth,nSizeY};
					IppiSize roiAScaleSize = {nWidth,nSizeY*dKoefA};
					IppiSize roiBScaleSize = {nWidth,nSizeY - roiAScaleSize.height};
					dKoefB = (double)(nSizeY - roiAScaleSize.height) / nSizeY;

					Ipp8u *pResult = pBGRAResult;
					for (int i=0;i<nVTileCount;i++)
					{
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRect,
							pResult, 4*nWidth, roiAScaleSize,
							1.0, dKoefA, IPPI_INTER_LINEAR);	

						pResult += 4*nWidth*roiAScaleSize.height;

						ippiResize_8u_C4R(
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
						ippiResize_8u_C4R(
							pA, roiSize, 4*nWidth, srcRect,
							pResult, 4*nWidth, roiAScaleSize,
							1.0, dKoefA, IPPI_INTER_LINEAR);	

						pResult += 4*nWidth*roiAScaleSize.height;

						ippiResize_8u_C4R(
							pB, roiSize, 4*nWidth, srcRect,
							pResult, 4*nWidth, roiBScaleSize,
							1.0, dKoefB, IPPI_INTER_LINEAR);	
					}
					return TRUE;
				}


				BOOL IPP_StretchSideLTToRB(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiSize roiSize = {nWidth,nHeight};

					int nHTileCount = nWidth / nSizeX;
					int nVTileCount = nHeight / nSizeY;
					memcpy(pBGRAResult,pBGRASource1,4*nWidth*nHeight);

					IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
					IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
					IppiRect srcRect = {0,0,nSizeX,nSizeY};

					IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
					IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

					Ipp8u *pResult = pBGRAResult;
					for (int j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + 4*nWidth*(nSizeY*(j+1) - roiScaleSize.height);
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRect,
									pResult, 4*nWidth, roiScaleSize,
									dFrame, dFrame, IPPI_INTER_LINEAR);	
							pResult += 4*nSizeX;
							srcRect.x += srcRect.width;
						}
						if (0!=srcRectRightBorder.width)
						{
							srcRectRightBorder.x = srcRect.x;
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
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
						pResult = pBGRAResult + 4*nWidth*(nSizeY*nVTileCount + srcRectBottomBorder.height - roiBottomBorderScaleSize.height);
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectBottomBorder,
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
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
									pResult, 4*nWidth, roiRightBorderScaleSize,
									dFrame, dFrame, IPPI_INTER_LINEAR);	
						}
					}
					return TRUE;
				}


				BOOL IPP_StretchSideLBToRT(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiSize roiSize = {nWidth,nHeight};

					int nHTileCount = nWidth / nSizeX;
					int nVTileCount = nHeight / nSizeY;
					memcpy(pBGRAResult,pBGRASource1,4*nWidth*nHeight);

					IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
					IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
					IppiRect srcRect = {0,0,nSizeX,nSizeY};

					IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
					IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

					Ipp8u *pResult = pBGRAResult;
					for (int j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + 4*nSizeY*nWidth*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRect,
									pResult, 4*nWidth, roiScaleSize,
									dFrame, dFrame, IPPI_INTER_LINEAR);	
							pResult += 4*nSizeX;
							srcRect.x += srcRect.width;
						}
						if (0!=srcRectRightBorder.width)
						{
							srcRectRightBorder.x = srcRect.x;
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
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
						pResult = pBGRAResult + 4*nSizeY*nWidth*nVTileCount;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectBottomBorder,
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
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
									pResult, 4*nWidth, roiRightBorderScaleSize,
									dFrame, dFrame, IPPI_INTER_LINEAR);	
						}
					}
					return TRUE;
				}

				BOOL IPP_StretchSideRBToLT(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiSize roiSize = {nWidth,nHeight};

					int nHTileCount = nWidth / nSizeX;
					int nVTileCount = nHeight / nSizeY;
					memcpy(pBGRAResult,pBGRASource1,4*nWidth*nHeight);

					IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
					IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
					IppiRect srcRect = {0,0,nSizeX,nSizeY};

					IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
					IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

					Ipp8u *pResult = pBGRAResult;

					for (int j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + 4*(nSizeY*nWidth*j + nSizeX-roiScaleSize.width);
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRect,
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
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
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
						pResult = pBGRAResult + 4*(nSizeY*nWidth*nVTileCount + nSizeX - roiBottomBorderScaleSize.width);
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectBottomBorder,
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

							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
									pResult, 4*nWidth, roiRightBorderScaleSize,
									dFrame, dFrame, IPPI_INTER_LINEAR);	
						}
					}
					return TRUE;
				}
				BOOL IPP_StretchSideRTToLB(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					IppiSize roiSize = {nWidth,nHeight};

					int nHTileCount = nWidth / nSizeX;
					int nVTileCount = nHeight / nSizeY;
					memcpy(pBGRAResult,pBGRASource1,4*nWidth*nHeight);

					IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
					IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
					IppiRect srcRect = {0,0,nSizeX,nSizeY};

					IppiSize roiScaleSize = {nSizeX*dFrame,nSizeY*dFrame};
					IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dFrame,nSizeY*dFrame};

					Ipp8u *pResult = pBGRAResult;
					for (int j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + 4*(nWidth*(nSizeY*(j+1) - roiScaleSize.height)
							+ (nSizeX-roiScaleSize.width));
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRect,
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
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
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
						pResult = pBGRAResult + 4*(nWidth*(nSizeY*nVTileCount + srcRectBottomBorder.height - roiBottomBorderScaleSize.height)
							+ (nSizeX - roiBottomBorderScaleSize.width));
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectBottomBorder,
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

							ippiResize_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcRectRightBorder,
									pResult, 4*nWidth, roiRightBorderScaleSize,
									dFrame, dFrame, IPPI_INTER_LINEAR);	
						}
					}
					return TRUE;
				}

				BOOL IPP_StretchCenter(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
				{
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
						pA = pBGRASource1;
						pB = pBGRASource2;
						dKoef = 1.0 - dFrame;
					}
					else
					{
						pA = pBGRASource2;
						pB = pBGRASource1;		
					}

					IppiRect srcRect = {0,0,nSizeX,nSizeY};
					IppiSize roiTileSize = {nSizeX,nSizeY};
					IppiSize roiScaleSize = {nSizeX*dKoef,nSizeY*dKoef};

					IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
					IppiSize roiRightBorderScaleSize = {srcRectRightBorder.width*dKoef,nSizeY*dKoef};

					IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};
					IppiSize roiBottomBorderScaleSize = {nSizeX*dKoef,srcRectBottomBorder.height*dKoef};

					Ipp8u *pResult = pBGRAResult;
					memcpy(pResult,pB,4*nWidth*nHeight);


					for (int j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + 4*(nWidth*(nSizeY*j + (nSizeY - roiScaleSize.height)/2) + 
							(nSizeX - roiScaleSize.width)/2);
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(
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
							ippiResize_8u_C4R(pA, roiSize, 4*nWidth, srcRectRightBorder,
									pResult, 4*nWidth, roiRightBorderScaleSize,
									dKoef, dKoef, IPPI_INTER_LINEAR);	
							srcRectRightBorder.y += nSizeY;
						}
						srcRect.x = 0;
						srcRect.y += nSizeY;
					}
					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + 4*(nWidth*(nSizeY*nVTileCount + (srcRectBottomBorder.height - roiBottomBorderScaleSize.height)/2) + 
							(nSizeX - roiScaleSize.width)/2);
						for (int i=0;i<nHTileCount;i++)
						{
							ippiResize_8u_C4R(
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
							ippiResize_8u_C4R(pA, roiSize, 4*nWidth, srcRectRightBorder,
									pResult, 4*nWidth, roiRightBorderScaleSize,
									dKoef, dKoef, IPPI_INTER_LINEAR);	
						}
					}
					return TRUE;
				}

				BOOL IPP_StretchWipeSideHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn, BOOL bLtoR)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					int nHTileCount = nWidth / nSizeX;

					Ipp8u *pResult = pBGRAResult;

					Ipp8u *pA = NULL;
					Ipp8u *pB = NULL;
					double dKoef = floor((1.0 - dFrame)*nSizeX)/nSizeX;
					if (bIn)
					{
						dKoef = dFrame;
						pA = pBGRASource2;
						pB = pBGRASource1;
						if (!bLtoR)
							pResult += (4*(nSizeX - (int)(nSizeX*dKoef)));
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
						if (bLtoR)
							pResult += (4*(nSizeX - (int)(nSizeX*dKoef)));
					}

					IppiSize roiSize = {nWidth,nHeight};
					IppiRect srcRect = {0,0,nSizeX,nHeight};
					IppiSize roiScaleSize = {nSizeX*dKoef,nHeight};
					int nRightBorder = nWidth % nSizeX;

					memcpy(pBGRAResult,pB,4*nWidth*nHeight);

					for (int i=0;i<nHTileCount;i++)
					{
						ippiResize_8u_C4R(
								pA, roiSize, 4*nWidth, srcRect,
								pResult, 4*nWidth, roiScaleSize,
								dKoef, 1.0, IPPI_INTER_LINEAR);	
						srcRect.x += srcRect.width;		
						pResult += 4*nSizeX;
					}

					if (0!=nRightBorder)
					{
						dKoef = floor(dKoef*nRightBorder)/nRightBorder;
						pResult = pBGRAResult + 4*nSizeX*nHTileCount;
						if (((bIn)&&(!bLtoR))||((!bIn)&&(bLtoR)))
						{
							pResult += 4*(nRightBorder - (int)(nRightBorder*dKoef));
						}

						srcRect.width = nRightBorder;
						roiScaleSize.width = nRightBorder*dKoef;
						ippiResize_8u_C4R(
								pA, roiSize, 4*nWidth, srcRect,
								pResult, 4*nWidth, roiScaleSize,
								dKoef, 1.0, IPPI_INTER_LINEAR);	
					}
					return TRUE;
				}

				BOOL IPP_StretchWipeSideVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn, BOOL bBtoT)
				{
					nSizeX = (nSizeX < 2) ? nWidth : nSizeX;
					nSizeY = (nSizeY < 2) ? nHeight : nSizeY;

					int nVTileCount = nHeight / nSizeY;

					Ipp8u *pResult = pBGRAResult;

					Ipp8u *pA = NULL;
					Ipp8u *pB = NULL;	
					double dKoef = floor((1.0 - dFrame)*nSizeY)/nSizeY;
					if (bIn)
					{
						dKoef = floor(dFrame*nSizeY)/nSizeY;
						pA = pBGRASource2;
						pB = pBGRASource1;
						if (bBtoT)
							pResult += (4*nWidth*(nSizeY - (int)(nSizeY*dKoef)));
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
						if (!bBtoT)
							pResult += (4*nWidth*(nSizeY - (int)(nSizeY*dKoef)));
					}

					IppiSize roiSize = {nWidth,nHeight};
					IppiRect srcRect = {0,0,nWidth,nSizeY};
					IppiSize roiScaleSize = {nWidth,nSizeY*dKoef};
					int nBottomBorder = nHeight % nSizeY;

					memcpy(pBGRAResult,pB,4*nWidth*nHeight);

					for (int i=0;i<nVTileCount;i++)
					{
						ippiResize_8u_C4R(
								pA, roiSize, 4*nWidth, srcRect,
								pResult, 4*nWidth, roiScaleSize,
								1.0, dKoef, IPPI_INTER_LINEAR);	
						srcRect.y += srcRect.height;		
						pResult += 4*nSizeY*nWidth;
					}

					if (0!=nBottomBorder)
					{
						dKoef = floor(dKoef*nBottomBorder)/nBottomBorder;
						pResult = pBGRAResult + 4*nWidth*(nHeight - nBottomBorder);
						if (((bIn)&&(bBtoT))||((!bIn)&&(!bBtoT)))
						{
							pResult += (4*nWidth*(nBottomBorder - (int)(nBottomBorder*dKoef)));
						}

						srcRect.height = nBottomBorder;
						roiScaleSize.height = dKoef*nBottomBorder;
						ippiResize_8u_C4R(
								pA, roiSize, 4*nWidth, srcRect,
								pResult, 4*nWidth, roiScaleSize,
								 1.0, dKoef, IPPI_INTER_LINEAR);	
					}
					return TRUE;
				}

				BOOL IPP_TransformTwirl( BYTE* pSourcePixels, BYTE* pResultPixels, int nWidth, int nHeight, double dFrame, double dAngle, double dDegree, double dZoom, DWORD* pBackColor )
				{
					CBuffer<Ipp32f> oMapX( nWidth*nHeight );
					CBuffer<Ipp32f> oMapY( nWidth*nHeight );

					int nSizeTable = nWidth + nHeight;
					CBuffer<double> oHelpTable( (nSizeTable + 1) * 2 );

					if( oMapX.IsEmpty() || oMapY.IsEmpty() || oHelpTable.IsEmpty() )
					{
						::memcpy( pResultPixels, pSourcePixels, nWidth * nHeight * 4 );
						return FALSE;
					}
					
					dAngle *= dFrame * IPP_PI180;
					
					// расчитываем таблицу дл€ ускорени€ расчЄтов
					double* pTable = oHelpTable.GetPtr();
					double dFactor = 1.0 / nSizeTable;
					
					for( int i = 0; i <= nSizeTable; ++i, pTable += 2 )
					{
						double ang = dAngle * pow( 1 - sqrt(i * dFactor), dDegree );
						
						pTable[0] = sin( ang ) * dZoom;
						pTable[1] = cos( ang ) * dZoom;
					}

					pTable = oHelpTable.GetPtr();
					dFactor = nSizeTable * 4.0 / (nWidth * nWidth + nHeight * nHeight);

					int nHalfWidth  = nWidth / 2;
					int nHalfHeight = nHeight / 2;
					int nVMax = nHeight - nHalfHeight;
					int nHMax = nWidth  - nHalfWidth;

					Ipp32f* pxMapCur = oMapX.GetPtr();
					Ipp32f* pyMapCur = oMapY.GetPtr();
					
					for( int j = -nHalfHeight; j < nVMax; ++j )
					{
						for( int i = -nHalfWidth; i < nHMax; ++i, ++pxMapCur, ++pyMapCur )
						{
							double dR = int(i * i + j * j) * dFactor;
							int nIndex = int(dR);
							double* pItem = pTable + nIndex * 2;
							
							double dSin = pItem[0];
							double dCos = pItem[1];
							
							// интерпол€ци€
							if( nIndex < nSizeTable )
							{
								dR -= nIndex;
								dSin += (pItem[2] - dSin) * dR;
								dCos += (pItem[3] - dCos) * dR;
							}
							
							*pxMapCur = float((i * dCos - j * dSin) + nHalfWidth);
							*pyMapCur = float((i * dSin + j * dCos) + nHalfHeight);
						}
					}

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};

					if( pBackColor )
					{
						Ipp8u backColor[4] = {
							GetBValue(*pBackColor), 
							GetGValue(*pBackColor), 
							GetRValue(*pBackColor), 
							255
						};
						
						ippiSet_8u_C4R( backColor, pResultPixels, 4*nWidth, SrcSize );
					}

					ippiRemap_8u_C4R( pSourcePixels, SrcSize, 4*nWidth, SrcRect, 
						oMapX.GetPtr(), sizeof(Ipp32f)*nWidth, 
						oMapY.GetPtr(), sizeof(Ipp32f)*nWidth, 
						pResultPixels, 4*nWidth, SrcSize, IPPI_INTER_LINEAR);

					return TRUE;
				}

				BOOL IPP_Twirl( BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bClockwise, int nZoom )
				{
					const double c_dStepBegin = 0.3;
					const double c_dStepEnd   = 0.7;

					int nSize = nWidth * nHeight;

					CBuffer<BYTE> oBuffer(nSize * 4);
					if( oBuffer.IsEmpty() )
						return FALSE;

					AlphaBlend( oBuffer, pBGRASource1, pBGRASource2, nSize, (dFrame - c_dStepBegin) / (c_dStepEnd - c_dStepBegin) );

					double dFrameIn = 1 - abs(dFrame * 2 - 1);
					double dZoom = 1.0;

					switch( nZoom )
					{
					case 1: dZoom = 1.0 / max(0.00001, 1.0 - dFrameIn); break;
					case 2:	dZoom = 1.0 - dFrameIn; break;
					}

					DWORD dwBackColor = 0;
					
					return IPP_TransformTwirl( oBuffer, pBGRAResult, nWidth, nHeight, dFrameIn, (bClockwise ? 720 : -720), 2, dZoom, &dwBackColor );
				}

				BOOL IPP_TwirlDrain( BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bClockwise )
				{
					double dAngle = bClockwise ? 360 : -360;
					dAngle += 3 * dAngle * dFrame * dFrame;

					double dZoom = 1.0 / max(0.00001, 1 - dFrame);
						
					::memcpy( pBGRAResult, pBGRASource2, 4 * nWidth * nHeight);
					
					IPP_TransformTwirl( pBGRASource1, pBGRAResult, nWidth, nHeight, dFrame, dAngle, 2, dZoom, NULL );
					
					return TRUE;
				}

				BOOL IPP_TransformSphere(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long CenterX, long CenterY,  long RadiusX, long RadiusY, double Degree, DWORD dwBackColor)
				{
					int nRadiusX = max(1, RadiusX*Frame);
					int nRadiusY = max(1, RadiusY*Frame);
					int nRmax    = max(1, nRadiusX*nRadiusY);

					CBuffer<Ipp8u>  oBuffer( nWidth * nHeight * 4 );
					CBuffer<Ipp32f> oMapX( nWidth * nHeight );
					CBuffer<Ipp32f> oMapY( nWidth * nHeight );
					CBuffer<double> oOffset( nRmax );

					if( oBuffer.IsEmpty() || oMapX.IsEmpty() || oMapY.IsEmpty() || oOffset.IsEmpty() )
						return FALSE;

					double* pOffset = oOffset.GetPtr();
					
					pOffset[0] = 0;
					for( int i = 1; i < nRmax; i++ )
					{
						pOffset[i] = pow( i / double(nRmax), Degree );
					}
					
					double dFactorX = 1.0 / (nRadiusX * nRadiusX);
					double dFactorY = 1.0 / (nRadiusY * nRadiusY);

					Ipp32f* pxMapCur = oMapX.GetPtr();
					Ipp32f* pyMapCur = oMapY.GetPtr();
					
					for( int nRow = 0; nRow < nHeight; ++nRow )
					{
						int nY = nRow - CenterY;

						for( int nPos = 0; nPos < nWidth; ++nPos )
						{
							int nX = nPos - CenterX;
							double dR = nX * nX * dFactorX + nY * nY * dFactorY;
							if( dR < 1 )
							{
								int index = int(sqrt( dR ) * nRadiusX * nRadiusY);
								
								*pxMapCur++ = nX * pOffset[index] + CenterX;
								*pyMapCur++ = nY * pOffset[index] + CenterY;
							}
							else
							{
								*pxMapCur++ = nPos;
								*pyMapCur++ = nRow;
							}
						}
					}

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};

					Ipp8u backColor[4] = { 
						GetBValue( dwBackColor ), 
						GetGValue( dwBackColor ), 
						GetRValue( dwBackColor ), 
						255
					};
					
					Ipp8u* pBuffer = oBuffer.GetPtr();

					ippiSet_8u_C4R( backColor, pBuffer, nWidth * 4, SrcSize );

					ippiRemap_8u_C4R( pPixels, SrcSize, nWidth * 4, SrcRect, 
						oMapX.GetPtr(), sizeof(Ipp32f) * nWidth, 
						oMapY.GetPtr(), sizeof(Ipp32f) * nWidth, 
						pBuffer, nWidth * 4, SrcSize, IPPI_INTER_LINEAR);

					memcpy( pPixels, pBuffer, nWidth * nHeight * 4 );
					
					return TRUE;
				}

				BOOL IPP_Sphere(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bIn)
				{
					const double c_dStepBegin = 0.3;
					const double c_dStepEnd   = 0.7;

					int nSize = nWidth * nHeight;

					AlphaBlend( pBGRAResult, pBGRASource1, pBGRASource2, nSize, (dFrame - c_dStepBegin) / (c_dStepEnd - c_dStepBegin) );

					double dFrameIn = 1 - abs(dFrame * 2 - 1);
					double dDegree  = bIn ? -0.6 : 3;

					int nSphereWidth  = bIn ? nWidth  / 2 : max(nWidth, nHeight);
					int nSphereHeight = bIn ? nHeight / 2 : max(nWidth, nHeight);

					return IPP_TransformSphere(pBGRAResult, nWidth, nHeight, dFrameIn, nWidth/2, nHeight/2, nSphereWidth, nSphereHeight, dDegree, 0);
				}


				BOOL IPP_SwingSideVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bLeft, BOOL bIn)
				{
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
						pA = pBGRASource2;
						pB = pBGRASource1;		
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memcpy(pResult,pB,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}

					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
														srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}
					return TRUE;
				}


				BOOL IPP_SwingSideHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bTop, BOOL bIn)
				{
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
						pA = pBGRASource2;
						pB = pBGRASource1;		
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;
					memcpy(pResult,pB,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}
					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
														srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}
					return TRUE;

				}

				BOOL IPP_SwingSideChangeVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bLeft)
				{
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
						pA = pBGRASource1;
						dKoef = 1.0 - 2*dFrame;
					}
					else
					{
						pA = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memset(pResult,0,nIppStep*nHeight);

					int j = 0;
					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}

					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
														srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}
					
					return TRUE;
				}

				BOOL IPP_SwingSideChangeHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bTop)
				{
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
						pA = pBGRASource1;
						dKoef = 1.0 - 2*dFrame;
					}
					else
					{
						pA = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;
					memset(pResult,0,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}
					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
														srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}
					return TRUE;

				}

				BOOL IPP_SwingDoorsHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
				{
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
						pA = pBGRASource2;
						pB = pBGRASource1;		
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memcpy(pResult,pB,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
							pResult += (nTileHStep - nTileHHalfStep);
							pSource += (nTileHStep - nTileHHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsR, IPPI_INTER_LINEAR );
							pResult += nTileHHalfStep;
							pSource += nTileHHalfStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							if ( srcRectRightBorder.width <= srcRect.width )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsL, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectRightBorder.width - srcRect.width;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
															nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
								pResult += nTileHHalfStep;
								pSource += nTileHHalfStep;
								
								srcRectRightBorder.width = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsR, IPPI_INTER_LINEAR );
								srcRectRightBorder.width = rest + srcRect.width;
							}
							roiSize.width = srcRect.width;
						}
					}
					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
							pResult += (nTileHStep - nTileHHalfStep);
							pSource += (nTileHStep - nTileHHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
							pResult += nTileHHalfStep;
							pSource += nTileHHalfStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							if ( srcRectRightBorder.width <= srcRect.width )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectBottomBorder.width - srcRect.width;
								srcRectBottomBorder.width = srcRect.width;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
								pResult += nTileHHalfStep;
								pSource += nTileHHalfStep;
								
								srcRectBottomBorder.width = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
							}
						}
					}
					return TRUE;
				}


				BOOL IPP_SwingDoorsVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
				{
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
						pA = pBGRASource2;
						pB = pBGRASource1;		
					}
					else
					{
						pA = pBGRASource1;
						pB = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memcpy(pResult,pB,4*nWidth*nHeight);

					int j = 0;

					for (j=0;j<nHTileCount;j++)
					{
						pResult = pBGRAResult + nTileHStep*j;
						pSource = pA + nTileHStep*j;
						for (int i=0;i<nVTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
							pResult += (nTileVStep - nTileVHalfStep);
							pSource += (nTileVStep - nTileVHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsB, IPPI_INTER_LINEAR );
							pResult += nTileVHalfStep;
							pSource += nTileVHalfStep;
						}
						if (0!=srcRectBottomBorder.height)
						{
							roiSize.height = srcRectBottomBorder.height;
							if ( srcRectBottomBorder.height <= srcRect.height )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsT, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectBottomBorder.height - srcRect.height;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
															nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
								pResult += nTileVHalfStep;
								pSource += nTileVHalfStep;
								
								srcRectBottomBorder.height = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsB, IPPI_INTER_LINEAR );
								srcRectBottomBorder.height = rest + srcRect.height;
							}
							roiSize.height = srcRect.height;
						}
					}
					if (0!=srcRectRightBorder.width)
					{
						pResult = pBGRAResult + nTileHStep*j;
						pSource = pA + nTileHStep*j;
						roiSize.width = srcRectRightBorder.width;

						for (int i=0;i<nVTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
							pResult += (nTileVStep - nTileVHalfStep);
							pSource += (nTileVStep - nTileVHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
							pResult += nTileVHalfStep;
							pSource += nTileVHalfStep;
						}
						if (0!=srcRectBottomBorder.height)
						{
							roiSize.height = srcRectRightBorder.height = srcRectBottomBorder.height;
							if ( srcRectBottomBorder.height <= srcRect.height )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectRightBorder.height - srcRect.height;
								srcRectRightBorder.height = srcRect.height;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
								pResult += nTileVHalfStep;
								pSource += nTileVHalfStep;
								
								srcRectRightBorder.height = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
							}
						}
					}
					return TRUE;
				}
				BOOL IPP_SwingDoorsChangeHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
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
						pA = pBGRASource1;
						dKoef = 1.0 - 2*dFrame;
					}
					else
					{
						pA = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memset(pResult,0,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
							pResult += (nTileHStep - nTileHHalfStep);
							pSource += (nTileHStep - nTileHHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsR, IPPI_INTER_LINEAR );
							pResult += nTileHHalfStep;
							pSource += nTileHHalfStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							if ( srcRectRightBorder.width <= srcRect.width )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsL, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectRightBorder.width - srcRect.width;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
															nIppStep, srcRect, coeffsL, IPPI_INTER_LINEAR );
								pResult += nTileHHalfStep;
								pSource += nTileHHalfStep;
								
								srcRectRightBorder.width = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsR, IPPI_INTER_LINEAR );
								srcRectRightBorder.width = rest + srcRect.width;
							}
							roiSize.width = srcRect.width;
						}
					}
					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
							pResult += (nTileHStep - nTileHHalfStep);
							pSource += (nTileHStep - nTileHHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
														nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
							pResult += nTileHHalfStep;
							pSource += nTileHHalfStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							if ( srcRectRightBorder.width <= srcRect.width )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectBottomBorder.width - srcRect.width;
								srcRectBottomBorder.width = srcRect.width;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsL, IPPI_INTER_LINEAR );
								pResult += nTileHHalfStep;
								pSource += nTileHHalfStep;
								
								srcRectBottomBorder.width = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsR, IPPI_INTER_LINEAR );
							}
						}
					}
					return TRUE;
				}
				BOOL IPP_SwingDoorsChangeVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame)
				{
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
						pA = pBGRASource1;
						dKoef = 1.0 - 2*dFrame;
					}
					else
					{
						pA = pBGRASource2;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memset(pResult,0,4*nWidth*nHeight);

					int j = 0;

					for (j=0;j<nHTileCount;j++)
					{
						pResult = pBGRAResult + nTileHStep*j;
						pSource = pA + nTileHStep*j;
						for (int i=0;i<nVTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
							pResult += (nTileVStep - nTileVHalfStep);
							pSource += (nTileVStep - nTileVHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
														nIppStep, srcRect, coeffsB, IPPI_INTER_LINEAR );
							pResult += nTileVHalfStep;
							pSource += nTileVHalfStep;
						}
						if (0!=srcRectBottomBorder.height)
						{
							roiSize.height = srcRectBottomBorder.height;
							if ( srcRectBottomBorder.height <= srcRect.height )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsT, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectBottomBorder.height - srcRect.height;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
															nIppStep, srcRect, coeffsT, IPPI_INTER_LINEAR );
								pResult += nTileVHalfStep;
								pSource += nTileVHalfStep;
								
								srcRectBottomBorder.height = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
															nIppStep, srcRectBottomBorder, coeffsB, IPPI_INTER_LINEAR );
								srcRectBottomBorder.height = rest + srcRect.height;
							}
							roiSize.height = srcRect.height;
						}
					}
					if (0!=srcRectRightBorder.width)
					{
						pResult = pBGRAResult + nTileHStep*j;
						pSource = pA + nTileHStep*j;
						roiSize.width = srcRectRightBorder.width;

						for (int i=0;i<nVTileCount;i++)
						{
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
							pResult += (nTileVStep - nTileVHalfStep);
							pSource += (nTileVStep - nTileVHalfStep);
							ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
														nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
							pResult += nTileVHalfStep;
							pSource += nTileVHalfStep;
						}
						if (0!=srcRectBottomBorder.height)
						{
							roiSize.height = srcRectRightBorder.height = srcRectBottomBorder.height;
							if ( srcRectBottomBorder.height <= srcRect.height )
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
							else
							{
								int rest = srcRectRightBorder.height - srcRect.height;
								srcRectRightBorder.height = srcRect.height;
								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsT, IPPI_INTER_LINEAR );
								pResult += nTileVHalfStep;
								pSource += nTileVHalfStep;
								
								srcRectRightBorder.height = rest;

								ippiWarpPerspective_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
															nIppStep, srcRectRightBorder, coeffsB, IPPI_INTER_LINEAR );
							}
						}
					}
					return TRUE;
				}

				BOOL IPP_FlipOverHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
				{
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
						pA = pBGRASource1;
					}
					else
					{
						pA = pBGRASource2;
						dKoef = 1.0 - dFrame;
					}

					IppiRect srcRect = {0,0,nSizeX,nSizeY};

					IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
					IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

					double	quadDst[4][2];
					double  coeffs[2][4];

					coeffs[0][0] = coeffs[0][1] = coeffs[0][2] = coeffs[0][3] = 0;
					coeffs[1][0] = coeffs[1][1] = coeffs[1][2] = coeffs[1][3] = 0;

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
				    
					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memset(pResult,0,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
								nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );

							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
								nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );        
							roiSize.width = nSizeX;
						}
					}

					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
								srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );

							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
								nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}

					return TRUE;
				}
				BOOL IPP_FlipOverVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn)
				{
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
						pA = pBGRASource1;
					}
					else
					{
						pA = pBGRASource2;
						dKoef = 1.0 - dFrame;
					}

					IppiRect srcRect = {0,0,nSizeX,nSizeY};

					IppiRect srcRectRightBorder = {0,0,nWidth % nSizeX,nSizeY};
					IppiRect srcRectBottomBorder = {0,0,nSizeX,nHeight % nSizeY};

					double	quadDst[4][2];
					double  coeffs[2][4];

					coeffs[0][0] = coeffs[0][1] = coeffs[0][2] = coeffs[0][3] = 0;
					coeffs[1][0] = coeffs[1][1] = coeffs[1][2] = coeffs[1][3] = 0;

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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memset(pResult,0,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
								nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
								nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}

					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
								srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
								nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}

					return TRUE;
				}

				BOOL IPP_FlipAwayHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY, double dFrame, BOOL bIn, BOOL bForward )
				{
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
						pA = pBGRASource2;
						pB = pBGRASource1;
						dFrame = 1 - dFrame;
					}
					else
					{
						pB = pBGRASource2;
						pA = pBGRASource1;
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

					coeffs[0][0] = coeffs[0][1] = coeffs[0][2] = coeffs[0][3] = 0;
					coeffs[1][0] = coeffs[1][1] = coeffs[1][2] = coeffs[1][3] = 0;


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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memcpy(pResult,pB,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
								nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
								nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}

					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
								srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
								nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}
					return TRUE;
				}

				BOOL IPP_FlipAwayVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bIn, BOOL bForward )
				{
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
						pA = pBGRASource2;
						pB = pBGRASource1;
						dFrame = 1 - dFrame;
					}
					else
					{
						pB = pBGRASource2;
						pA = pBGRASource1;
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

					coeffs[0][0] = coeffs[0][1] = coeffs[0][2] = coeffs[0][3] = 0;
					coeffs[1][0] = coeffs[1][1] = coeffs[1][2] = coeffs[1][3] = 0;

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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memcpy(pResult,pB,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							IppStatus ippst = ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
								nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );

							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
								nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}
					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
								srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
								nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}
					return TRUE;
				}

				BOOL IPP_TwistLinear( BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bHorDir, BOOL bForward, DWORD dwColorBGRA = 0xff000000 )
				{
					IppiSize roiSize = {nWidth, nHeight};
					IppiRect srcROI  = {0, 0, nWidth, nHeight};

					Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
					Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
					
					int nSizeTable = bHorDir ? nWidth : nHeight;
					Ipp64f* pHelp = new Ipp64f[nSizeTable];

					if (pxMap && pyMap && pHelp)
					{
						Ipp8u backColor[4]; //= {0, 0, 0, 255};
						*((DWORD*)(&backColor[0])) = dwColorBGRA;
						ippiSet_8u_C4R( backColor, pBGRAResult, 4*nWidth, roiSize );
						
						double fT      = ((bForward ? dFrame : 1 - dFrame) * 3 - 1) * nSizeTable;
						double dFactor = IPP_PI2 / nSizeTable;

						for( int i = 0; i < nSizeTable; i++ )
						{
							double dPhi = sin( (i - fT) * dFactor );
							if( dPhi < 0 )
							{
								if( dPhi > -0.00001 ) 
									dPhi = -0.00001;
							}
							else
							{
								if( dPhi < 0.00001 )
									dPhi = 0.00001;
							}
							
							pHelp[i] = 1 / dPhi;
						}

						int nHalfWidth  = nWidth / 2;
						int nHalfHeight = nHeight / 2;
						int nHMax = nWidth  - nHalfWidth;
						int nVMax = nHeight - nHalfHeight;

						// перва€ часть отрисовки
						if( fT < nSizeTable - 1 )
						{
							Ipp32f* pxMapCur = pxMap;
							Ipp32f* pyMapCur = pyMap;
							
							if( bHorDir )
							{
								if( fT < 0 )
								{
									int nHMax = fT + nWidth;
									for( int j = -nHalfHeight; j < nVMax; j++ )
									{
										for( int i = 0; i < nHMax; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j * pHelp[i] + nHalfHeight);
										}
										for( int i = nHMax; i < nWidth; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j + nHalfHeight);
										}
									}
								}
								else
								{
									int nHMin = fT;
									for( int j = -nHalfHeight; j < nVMax; j++ )
									{
										for( int i = 0; i < nHMin; i++ )
										{
											*pxMapCur++ = -1.0f;
											*pyMapCur++ = -1.0f;
										}
										for( int i = nHMin; i < nWidth; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j * pHelp[i] + nHalfHeight);
										}
									}
								}
							}
							else
							{
								if( fT < 0 )
								{
									int nVMax = fT + nHeight;
									for( int j = 0; j < nVMax; j++ )
									{
										for( int i = -nHalfWidth; i < nHMax; i++ )
										{
											*pxMapCur++ = float(i * pHelp[j] + nHalfWidth);
											*pyMapCur++ = float(j);
										}
									}
									for( int j = nVMax; j < nHeight; j++ )
									{
										for( int i = 0; i < nWidth; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j);
										}
									}
								}
								else
								{
									int nVMin = fT;
									for( int j = 0; j < nVMin; j++ )
									{
										for( int i = 0; i < nWidth; i++ )
										{
											*pxMapCur++ = -1.0f;
											*pyMapCur++ = -1.0f;
										}
									}
									for( int j = nVMin; j < nHeight; j++ )
									{
										for( int i = -nHalfWidth; i < nHMax; i++ )
										{
											*pxMapCur++ = float(i * pHelp[j] + nHalfWidth);
											*pyMapCur++ = float(j);
										}
									}
								}
							}
							
							ippiRemap_8u_C4R( bForward ? pBGRASource1 : pBGRASource2, 
								roiSize, 4*nWidth, srcROI, 
								pxMap, sizeof(Ipp32f)*nWidth, 
								pyMap, sizeof(Ipp32f)*nWidth, 
								pBGRAResult, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
						}

						// втора€ часть отрисовки
						if( fT > 0 )
						{
							for( int i = 0; i < nSizeTable; i++ )
							{
								pHelp[i] *= -1;
							}
							
							Ipp32f* pxMapCur = pxMap;
							Ipp32f* pyMapCur = pyMap;
							
							if( bHorDir )
							{
								if( fT > nWidth )
								{
									int nHMax = fT - nWidth;
									for( int j = -nHalfHeight; j < nVMax; j++ )
									{
										for( int i = 0; i < nHMax; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j + nHalfHeight);
										}
										for( int i = nHMax; i < nWidth; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j * pHelp[i] + nHalfHeight);
										}
									}
								}
								else
								{
									int nHMin = fT;				
									for( int j = -nHalfHeight; j < nVMax; j++ )
									{
										for( int i = 0; i < nHMin; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j * pHelp[i] + nHalfHeight);
										}
										for( int i = nHMin; i < nWidth; i++ )
										{
											*pxMapCur++ = -1.0f;
											*pyMapCur++ = -1.0f;
										}
									}
								}
							}
							else
							{
								if( fT > nHeight )
								{
									int nVMax = fT - nHeight;
									for( int j = 0; j < nVMax; j++ )
									{
										for( int i = 0; i < nWidth; i++ )
										{
											*pxMapCur++ = float(i);
											*pyMapCur++ = float(j);
										}
									}
									for( int j = nVMax; j < nHeight; j++ )
									{
										for( int i = -nHalfWidth; i < nHMax; i++ )
										{
											*pxMapCur++ = float(i * pHelp[j] + nHalfWidth);
											*pyMapCur++ = float(j);
										}
									}
								}
								else
								{
									int nVMin = fT;				
									for( int j = 0; j < nVMin; j++ )
									{
										for( int i = -nHalfWidth; i < nHMax; i++ )
										{
											*pxMapCur++ = float(i * pHelp[j] + nHalfWidth);
											*pyMapCur++ = float(j);
										}
									}
									for( int j = nVMin; j < nHeight; j++ )
									{
										for( int i = 0; i < nWidth; i++ )
										{
											*pxMapCur++ = -1.0f;
											*pyMapCur++ = -1.0f;
										}
									}
								}
							}

							ippiRemap_8u_C4R( bForward ? pBGRASource2 : pBGRASource1,
								roiSize, 4*nWidth, srcROI, 
								pxMap, sizeof(Ipp32f)*nWidth, 
								pyMap, sizeof(Ipp32f)*nWidth, 
								pBGRAResult, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
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

				BOOL IPP_TwistDiagonal( BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nDirection, DWORD dwColorBGRA = 0xff000000 )
				{
					IppiSize roiSize = {nWidth, nHeight};
					IppiRect srcROI  = {0, 0, nWidth, nHeight};

					double dRadius = sqrt( double(nWidth * nWidth + nHeight * nHeight) );
					int nSizeTable = int(dRadius);
					Ipp64f* pTable = new Ipp64f[nSizeTable + 1];
					
					Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
					Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];

					if (pxMap && pyMap && pTable)
					{
						Ipp8u backColor[4]; //= {0, 0, 0, 255};
						*((DWORD*)(&backColor[0])) = dwColorBGRA;
						ippiSet_8u_C4R(backColor, pBGRAResult, 4*nWidth, roiSize);

						double fCosPhi = nWidth  / dRadius;
						double fSinPhi = nHeight / dRadius;
						double dFactor = IPP_PI / dRadius;
						double fT      = (3.0 * dFrame - 1.0) * (nWidth + nHeight) / 2; //nHeight;
						
						// расчитываем таблицу дл€ ускорени€ расчЄтов
						for( int i = 0; i <= nSizeTable; i++ )
						{
							double temp = sin( (i - fT) * dFactor );
							if( temp < 0 )
							{
								if( temp > -0.0001 )
									temp = -0.0001;
							}
							else
							{
								if( temp < 0.0001 )
									temp = 0.0001;
							}
							pTable[i] = 1 / temp;
						}
						
						double dMin = fT + 2;
						double dMax = fT + dRadius / 2;

						Ipp32f* pxMapCur = pxMap;
						Ipp32f* pyMapCur = pyMap;
						
						switch( nDirection )
						{
						case 0:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * (nHeight - j);
								double dCosY = fCosPhi * (nHeight - j);
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * i;
									if( dX < dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if( dX > dMax )
									{
										*pxMapCur++ = float(i);
										*pyMapCur++ = float(j);
										continue;
									}
									
									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}
									
									dY *= dCosY - fSinPhi * i;

									*pxMapCur++ =            fCosPhi * dX - fSinPhi * dY;
									*pyMapCur++ = nHeight - (fSinPhi * dX + fCosPhi * dY);
								}
							}
							break;
						case 1:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * j;
								double dCosY = fCosPhi * j;
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * i;
									if( dX < dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if( dX > dMax )
									{
										*pxMapCur++ = float(i);
										*pyMapCur++ = float(j);
										continue;
									}

									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}

									dY *= dCosY - fSinPhi * i;

									*pxMapCur++ = fCosPhi * dX - fSinPhi * dY;
									*pyMapCur++ = fSinPhi * dX + fCosPhi * dY;
								}
							}
							break;
						case 2:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * (nHeight - j);
								double dCosY = fCosPhi * (nHeight - j);
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * (nWidth - i);
									if( dX < dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if( dX > dMax )
									{
										*pxMapCur++ = i;
										*pyMapCur++ = j;
										continue;
									}
									
									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}
									
									dY *= dCosY - fSinPhi * (nWidth - i);

									*pxMapCur++ = nWidth  - (fCosPhi * dX - fSinPhi * dY);
									*pyMapCur++ = nHeight - (fSinPhi * dX + fCosPhi * dY);
								}
							}
							break;
						case 3:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * j;
								double dCosY = fCosPhi * j;
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * (nWidth - i);
									if( dX < dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if (dX > dMax )
									{
										*pxMapCur++ = float(i);
										*pyMapCur++ = float(j);
										continue;
									}
									
									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}
									
									dY *= dCosY - fSinPhi * (nWidth - i);

									*pxMapCur++ = nWidth - (fCosPhi * dX - fSinPhi * dY);
									*pyMapCur++ =           fSinPhi * dX + fCosPhi * dY;
								}
							}
							break;
						}
						ippiRemap_8u_C4R(pBGRASource1, roiSize, 4*nWidth, srcROI, 
							pxMap, sizeof(Ipp32f)*nWidth, 
							pyMap, sizeof(Ipp32f)*nWidth, 
							pBGRAResult, 4*nWidth, roiSize, IPPI_INTER_LINEAR);

						//второй рисунок
						for( int i = 0; i < nSizeTable; i++ )
						{
							pTable[i] *= -1;
						}

						dMin = fT - 2;
						dMax = fT - dRadius / 2;

						pxMapCur = pxMap;
						pyMapCur = pyMap;
						
						switch(nDirection)
						{
						case 0:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * (nHeight - j);
								double dCosY = fCosPhi * (nHeight - j);
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * i;
									if( dX > dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if( dX < dMax )
									{
										*pxMapCur++ = float(i);
										*pyMapCur++ = float(j);
										continue;
									}
									
									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}

									dY *= dCosY - fSinPhi * i;

									*pxMapCur++ =            fCosPhi * dX - fSinPhi * dY;
									*pyMapCur++ = nHeight - (fSinPhi * dX + fCosPhi * dY);
								}
							}
							break;
						case 1:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * j;
								double dCosY = fCosPhi * j;
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * i;
									if( dX > dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if( dX < dMax )
									{
										*pxMapCur++ = float(i);
										*pyMapCur++ = float(j);
										continue;
									}
									
									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}
									
									dY *= dCosY - fSinPhi * i;

									*pxMapCur++ = fCosPhi * dX - fSinPhi * dY;
									*pyMapCur++ = fSinPhi * dX + fCosPhi * dY;
								}
							}
							break;
						case 2:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * (nHeight - j);
								double dCosY = fCosPhi * (nHeight - j);
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * (nWidth - i);
									if( dX > dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if( dX < dMax )
									{
										*pxMapCur++ = float(i);
										*pyMapCur++ = float(j);
										continue;
									}
									
									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}
									
									dY *= dCosY - fSinPhi * (nWidth - i);

									*pxMapCur++ = nWidth  - (fCosPhi * dX - fSinPhi * dY);
									*pyMapCur++ = nHeight - (fSinPhi * dX + fCosPhi * dY);
								}
							}
							break;
						case 3:
							for( int j = 0; j < nHeight; j++ )
							{
								double dSinY = fSinPhi * j;
								double dCosY = fCosPhi * j;
								
								for( int i = 0; i < nWidth; i++ )
								{
									double dX = dSinY + fCosPhi * (nWidth - i);
									if( dX > dMin )
									{
										*pxMapCur++ = -1;
										*pyMapCur++ = -1;
										continue;
									}
									if( dX < dMax )
									{
										*pxMapCur++ = float(i);
										*pyMapCur++ = float(j);
										continue;
									}
									
									int nIndex = int(dX);
									double dY = pTable[nIndex];
									
									// интерпол€ци€
									if( nIndex < nSizeTable )
									{
										dY += (pTable[nIndex + 1] - dY) * (dX - nIndex);
									}
									
									dY *= dCosY - fSinPhi * (nWidth - i);

									*pxMapCur++ = nWidth - (fCosPhi * dX - fSinPhi * dY);
									*pyMapCur++ =           fSinPhi * dX + fCosPhi * dY;
								}
							}
							break;
						}
						ippiRemap_8u_C4R(pBGRASource2, roiSize, 4*nWidth, srcROI, 
							pxMap, sizeof(Ipp32f)*nWidth, 
							pyMap, sizeof(Ipp32f)*nWidth, 
							pBGRAResult, 4*nWidth, roiSize, IPPI_INTER_LINEAR);
					}

					if (pyMap)
						delete []pyMap;
					if (pxMap)
						delete []pxMap;
					if (pTable)
						delete []pTable;

					return TRUE;
				}

				BOOL IPP_Ripple(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bIsOut)
				{
					CBuffer<float> oMapX( nWidth );
					CBuffer<float> oMapY( nWidth );
					CBuffer<BYTE> oBuffer( nWidth * nHeight * 4 );

					int nSizeHelp = max(nWidth, nHeight);
					CBuffer<double> oHelp(nSizeHelp * 2);

					if( oMapX.IsEmpty() || oMapY.IsEmpty() || oBuffer.IsEmpty() || oHelp.IsEmpty() )
					{
						return FALSE;
					}

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize DstSize = {nWidth, 1};


					const double c_dStepBegin = 0.2;
					const double c_dStepEnd   = 0.8;

					AlphaBlend( oBuffer.GetPtr(), pBGRASource1, pBGRASource2, nWidth * nHeight, (dFrame - c_dStepBegin) / (c_dStepEnd - c_dStepBegin) );
					
					double dHypot  = nWidth * nWidth + nHeight * nHeight;
					double dAmpl   = sqrt( dHypot ) * 2 * (dFrame > 0.5 ? 1 - dFrame : dFrame);
					double dOmega  = IPP_2PI * 7.5;
					double dOffset = IPP_2PI * 2 * dFrame * (bIsOut ? 1 : -1);
					
					double* pItem = oHelp.GetPtr();
					for( int i = 0; i < nSizeHelp; ++i, pItem += 2 )
					{
						int temp = i - nSizeHelp / 2;
						pItem[0] = temp * temp / dHypot;
						pItem[1] = (temp - 1) * (temp - 1) / dHypot;
					}

					int right  = nWidth  - 1;
					int bottom = nHeight - 1;

					double* pBaseI = oHelp.GetPtr() + (nSizeHelp / 2 - nWidth  / 2) * 2;
					double* pBaseJ = oHelp.GetPtr() + (nSizeHelp / 2 - nHeight / 2) * 2;

					double* pJ = pBaseJ;
					for( int j = 0; j <= bottom; ++j, pJ += 2, pBGRAResult += nWidth * 4 )
					{
						float* pMapX = oMapX.GetPtr();
						float* pMapY = oMapY.GetPtr();

						double* pI = pBaseI;
						for( int i = 0; i <= right; ++i, pI += 2, ++pMapX, ++pMapY )
						{
							double dTemp = sin( dOmega * sqrt(pI[0] + pJ[0]) - dOffset);
							
							double dx = abs( i + dAmpl * (dTemp - sin( dOmega * sqrt( pI[1] + pJ[0] ) - dOffset)));
							double dy = abs( j + dAmpl * (dTemp - sin( dOmega * sqrt( pI[0] + pJ[1] ) - dOffset)));

							if( dx > right )
								dx = (right * 2) - dx;

							if( dy > bottom )
								dy = (bottom * 2) - dy;

							pMapX[0] = float(dx);
							pMapY[0] = float(dy);
						}

						ippiRemap_8u_C4R( oBuffer.GetPtr(), SrcSize, nWidth * 4, SrcRect, 
							oMapX.GetPtr(), sizeof(Ipp32f) * nWidth, 
							oMapY.GetPtr(), sizeof(Ipp32f) * nWidth, 
							pBGRAResult, nWidth * 4, DstSize, IPPI_INTER_LINEAR );

					}

					return TRUE;
				}
				BOOL IPP_Wave(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bIsHorizontal)
				{
					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};

					Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
					Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
					Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];

					if (pxMap && pyMap && pBuffer)
					{
						const double c_dStepBegin = 0.2;
						const double c_dStepEnd   = 0.8;

						AlphaBlend( pBuffer, pBGRASource1, pBGRASource2, nWidth * nHeight, (dFrame - c_dStepBegin) / (c_dStepEnd - c_dStepBegin) );
						
						Ipp32f* pxMapCur = pxMap;
						Ipp32f* pyMapCur = pyMap;

						double ampl   = 0.1 * (dFrame > 0.5 ? 1 - dFrame : dFrame);
						double offset = 6 * IPP_PI * dFrame;
						double omega  = 10 * IPP_PI;

						if (bIsHorizontal)
						{
							ampl  *= nHeight - 1;
							omega /= nWidth;

							float fMaxOffset = float(nHeight - 2 - ampl);
							
							for( int nPos = 0; nPos < nWidth; nPos++ )
							{
								*pyMapCur++ = float(ampl * sin( omega * nPos + offset ));
								*pxMapCur++ = nPos;
							}
							
							for( int nRow = 1; nRow < nHeight; ++nRow )
							{
								for( int nPos = 0; nPos < nWidth; ++nPos, ++pxMapCur, ++pyMapCur )
								{
									*pxMapCur = nPos;
									*pyMapCur = pyMapCur[-nWidth] + 1;

									if( pyMapCur[-nWidth] < ampl )
									{
										pyMapCur[-nWidth] += ampl;
										pyMapCur[-nWidth] *= 0.5f;
									}

									if( pyMapCur[0] > fMaxOffset )
									{
										pyMapCur[0] -= 0.5f;
									}
								}
							}
						}
						else
						{
							ampl  *= nWidth - 1;
							omega /= nHeight;

							float fMaxOffset = float(nWidth - 2 - ampl);

							for( int nRow = 0; nRow < nHeight; ++nRow )
							{
								*pxMapCur++ = float(ampl * sin( omega * nRow + offset ));
								*pyMapCur++ = nRow;
								
								for( int nPos = 1; nPos < nWidth; ++nPos, ++pxMapCur, ++pyMapCur )
								{
									*pxMapCur = pxMapCur[-1] + 1;
									*pyMapCur = nRow;

									if( pxMapCur[-1] < ampl )
									{
										pxMapCur[-1] += ampl;
										pxMapCur[-1] *= 0.5f;
									}
									
									if( pxMapCur[0] > fMaxOffset )
									{
										pxMapCur[0] -= 0.5f;
									}
								}
								pxMapCur[-1] = nWidth - 1.5f; // коррекци€ правой границы
							}
						}
						
						ippiRemap_8u_C4R(pBuffer, SrcSize, 4*nWidth, SrcRect, 
							pxMap, sizeof(Ipp32f)*nWidth, 
							pyMap, sizeof(Ipp32f)*nWidth, 
							pBGRAResult, 4*nWidth, SrcSize, IPPI_INTER_LINEAR);
					}

					if (pyMap)
						delete []pyMap;
					if (pxMap)
						delete []pxMap;
					if (pBuffer)
						delete []pBuffer;
					
					return TRUE;
				}

				BOOL IPP_FadeZoom(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bZoomIn, BOOL bZoomOut)
				{
					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					
					Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
					Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
					Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];

					int nHalfWidth = nWidth/2;
					int nHalfHeight = nHeight/2;
					int nVMax = nHeight - nHalfHeight;
					int nHMax = nWidth - nHalfWidth;
					
					if (pxMap && pyMap && pBuffer)
					{
						Ipp8u* pFirst  = pBGRASource1;
						Ipp8u* pSecond = pBGRASource2;

						if( bZoomIn )
						{
							double dZoom = 1.0 / (dFrame * 6 + 1);

							Ipp32f* pxMapCur = pxMap;
							Ipp32f* pyMapCur = pyMap;
							
							for( int j = -nHalfHeight; j < nVMax; j++ )
							{
								for( int i = -nHalfWidth; i < nHMax; i++ )
								{
									*pxMapCur++ = float(nHalfWidth  + i * dZoom);
									*pyMapCur++ = float(nHalfHeight + j * dZoom);
								}
							}

							ippiRemap_8u_C4R(pBGRASource1, SrcSize, 4*nWidth, SrcRect, 
								pxMap, sizeof(Ipp32f)*nWidth, 
								pyMap, sizeof(Ipp32f)*nWidth, 
								pBuffer, 4*nWidth, SrcSize, IPPI_INTER_LINEAR);

							pFirst = pBuffer;
						}

						if( bZoomOut )
						{
							double dZoom = 1.0 / ((1 - dFrame) * 6 + 1);

							Ipp32f* pxMapCur = pxMap;
							Ipp32f* pyMapCur = pyMap;
							
							for( int j = -nHalfHeight; j < nVMax; j++ )
							{
								for( int i = -nHalfWidth; i < nHMax; i++ )
								{
									*pxMapCur++ = float(nHalfWidth  + i * dZoom);
									*pyMapCur++ = float(nHalfHeight + j * dZoom);
								}
							}

							ippiRemap_8u_C4R(pBGRASource2, SrcSize, 4*nWidth, SrcRect, 
								pxMap, sizeof(Ipp32f)*nWidth, 
								pyMap, sizeof(Ipp32f)*nWidth, 
								pBGRAResult, 4*nWidth, SrcSize, IPPI_INTER_LINEAR);

							pSecond = pBGRAResult;
						}

						AlphaBlend( pBGRAResult, pFirst, pSecond, nWidth * nHeight, dFrame );
					}
					
					if (pyMap)
						delete []pyMap;
					if (pxMap)
						delete []pxMap;
					if (pBuffer)
						delete []pBuffer;
					
					return TRUE;

				}

				BOOL IPP_Burning(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, Utility::CUtility* pUtility)
				{
					if( !pUtility->GenerateBurningMask( nWidth, nHeight ) )
						return FALSE;

					int nFrame = int(dFrame * 256.0);

					BYTE*  pMask = pUtility->g_pBurningMask;
					DWORD* pSrc1 = (DWORD*)pBGRASource1;
					DWORD* pSrc2 = (DWORD*)pBGRASource2;
					DWORD* pDst  = (DWORD*)pBGRAResult;

					int nSize = nWidth * nHeight;
					for( int i = 0; i < nSize; ++i, ++pMask, ++pDst, ++pSrc1, ++pSrc2 )
					{
						pDst[0] = (pMask[0] >= nFrame) ? pSrc1[0] : pSrc2[0];
					}

					return TRUE;
				}

				BOOL IPP_BurningFade(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, Utility::CUtility* pUtility)
				{
					if( !pUtility->GenerateBurningMask( nWidth, nHeight) )
						return FALSE;
					
					int nFrame = int(dFrame * 255);
					int nLTTreshold = nFrame;
					int nGTTreshold = min(nFrame + 25, 255);

					double factor = 255.0 / (nGTTreshold - nLTTreshold + 1);
					double offset = 0.5 - nLTTreshold * factor;

					DWORD Table[512];

					for( int i = 0; i < 256; i++ )
					{
						DWORD alpha1 = DWORD(i * factor + offset);
						DWORD alpha2 = 255 - alpha1;

						alpha1 *= 0x8081;
						alpha2 *= 0x8081;

						Table[i * 2 + 0] = alpha1;
						Table[i * 2 + 1] = alpha2;
					}

					BYTE* pMask = pUtility->g_pBurningMask;
					BYTE* pSrc1 = pBGRASource1;
					BYTE* pSrc2 = pBGRASource2;
					BYTE* pDst  = pBGRAResult;

					int nSize = nWidth * nHeight;
					for( int i = 0; i < nSize; ++i, ++pMask, pSrc1 += 4, pSrc2 += 4, pDst += 4 )
					{
						int msk = pMask[0];
						if( msk < nLTTreshold )
						{
							*((DWORD*)pDst) = *((DWORD*)pSrc2);
							continue;
						}

						if( msk > nGTTreshold )
						{
							*((DWORD*)pDst) = *((DWORD*)pSrc1);
							continue;
						}

						DWORD alpha1 = Table[msk * 2 + 0];
						DWORD alpha2 = Table[msk * 2 + 1];

						pDst[0] = (pSrc1[0] * alpha1 + pSrc2[0] * alpha2 + (128 * 0x8081)) >> 23;
						pDst[1] = (pSrc1[1] * alpha1 + pSrc2[1] * alpha2 + (128 * 0x8081)) >> 23;
						pDst[2] = (pSrc1[2] * alpha1 + pSrc2[2] * alpha2 + (128 * 0x8081)) >> 23;
					}

					return TRUE;
				}

				BOOL IPP_BurningFire(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, Utility::CUtility* pUtility)
				{
					static const BYTE palette[3][256] = {
						// blue
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x03, 0x07, 0x0b, 0x0f, 0x13, 0x17, 0x1b, 0x1f, 0x23, 0x27, 0x2b, 0x2f, 0x33, 0x37, 0x3b, 0x3f, 
						0x43, 0x47, 0x4b, 0x4f, 0x53, 0x57, 0x5b, 0x5f, 0x63, 0x67, 0x6b, 0x6f, 0x73, 0x77, 0x7b, 0x7f, 
						0x83, 0x87, 0x8b, 0x8f, 0x93, 0x97, 0x9b, 0x9f, 0xa3, 0xa7, 0xab, 0xaf, 0xb3, 0xb7, 0xbb, 0xbf, 
						0xc3, 0xc7, 0xcb, 0xcf, 0xd3, 0xd7, 0xdb, 0xdf, 0xe3, 0xe7, 0xeb, 0xef, 0xf3, 0xf7, 0xfb, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						// green
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x03, 0x07, 0x0b, 0x0f, 0x13, 0x17, 0x1b, 0x1f, 0x23, 0x27, 0x2b, 0x2f, 0x33, 0x37, 0x3b, 0x3f, 
						0x43, 0x47, 0x4b, 0x4f, 0x53, 0x57, 0x5b, 0x5f, 0x63, 0x67, 0x6b, 0x6f, 0x73, 0x77, 0x7b, 0x7f, 
						0x83, 0x87, 0x8b, 0x8f, 0x93, 0x97, 0x9b, 0x9f, 0xa3, 0xa7, 0xab, 0xaf, 0xb3, 0xb7, 0xbb, 0xbf, 
						0xc3, 0xc7, 0xcb, 0xcf, 0xd3, 0xd7, 0xdb, 0xdf, 0xe3, 0xe7, 0xeb, 0xef, 0xf3, 0xf7, 0xfb, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						// red
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x03, 0x07, 0x0b, 0x0f, 0x13, 0x17, 0x1b, 0x1f, 0x23, 0x27, 0x2b, 0x2f, 0x33, 0x37, 0x3b, 0x3f, 
						0x43, 0x47, 0x4b, 0x4f, 0x53, 0x57, 0x5b, 0x5f, 0x63, 0x67, 0x6b, 0x6f, 0x73, 0x77, 0x7b, 0x7f, 
						0x83, 0x87, 0x8b, 0x8f, 0x93, 0x97, 0x9b, 0x9f, 0xa3, 0xa7, 0xab, 0xaf, 0xb3, 0xb7, 0xbb, 0xbf, 
						0xc3, 0xc7, 0xcb, 0xcf, 0xd3, 0xd7, 0xdb, 0xdf, 0xe3, 0xe7, 0xeb, 0xef, 0xf3, 0xf7, 0xfb, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 

					};
					
					if( !pUtility->GenerateBurningMask( nWidth, nHeight ) )
						return FALSE;
					
					int nFrame = int(dFrame * 254.0);
					int nLTTreshold = nFrame;
					int nGTTreshold = min(nFrame + 20, 255);

					BYTE table[256];

					int nLength = nGTTreshold - nLTTreshold;

					for( int i = 0; i < 256; i++ )
					{
						if( i < nLTTreshold || i > nGTTreshold )
						{
							table[i] = 0;
							continue;
						}

						table[i] = 255 - abs((i - nLTTreshold) * 2 * 255 / nLength - 255);
					}

					BYTE* pDst  = pBGRAResult;
					BYTE* pSrc1 = pBGRASource1;
					BYTE* pSrc2 = pBGRASource2;
					BYTE* pMask = pUtility->g_pBurningMask;

					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, ++pMask, pDst += 4, pSrc1 += 4, pSrc2 += 4 )
					{
						int msk = pMask[0];
						
						if( msk < nLTTreshold )
						{
							*((DWORD*)pDst) = *((DWORD*)pSrc2);
							continue;
						}

						if( msk > nGTTreshold )
						{
							*((DWORD*)pDst) = *((DWORD*)pSrc1);
							continue;
						}

						msk = table[msk];

						pDst[0] = palette[0][msk];
						pDst[1] = palette[1][msk];
						pDst[2] = palette[2][msk];
						pDst[3] = 255;
					}

					return TRUE;
				}

				BOOL IPP_FlipAwayOutInHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY, double dFrame, BOOL bForward )
				{
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
						pA = pBGRASource2;
						dFrame = 2 - 2*dFrame;
						bForward = !bForward;
					}
					else
					{
						pA = pBGRASource1;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memset(pResult,0,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
								nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
								nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}

					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
								srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
								nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}

					return TRUE;
				}

				BOOL IPP_FlipAwayOutInVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bForward )
				{
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
						pA = pBGRASource2;
						dFrame = 2 - 2*dFrame;
						bForward = !bForward;
					}
					else
					{
						pA = pBGRASource1;
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

					Ipp8u *pResult = pBGRAResult;
					Ipp8u *pSource = pA;

					memset(pResult,0,nIppStep*nHeight);

					int j = 0;

					for (j=0;j<nVTileCount;j++)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						for (int i=0;i<nHTileCount;i++)
						{
							IppStatus ippst = ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRect, pResult,
								nIppStep, srcRect, coeffs, IPPI_INTER_LINEAR );

							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectRightBorder, pResult,
								nIppStep, srcRectRightBorder, coeffs, IPPI_INTER_LINEAR );
							roiSize.width = nSizeX;
						}
					}
					if (0!=srcRectBottomBorder.height)
					{
						pResult = pBGRAResult + nTileVStep*j;
						pSource = pA + nTileVStep*j;
						roiSize.height = srcRectBottomBorder.height;

						for (int i=0;i<nHTileCount;i++)
						{
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult, nIppStep,
								srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR );
							pResult += nTileHStep;
							pSource += nTileHStep;
						}
						if (0!=srcRectRightBorder.width)
						{
							roiSize.width = srcRectBottomBorder.width = srcRectRightBorder.width;
							ippiWarpBilinear_8u_C4R( pSource, roiSize, nIppStep, srcRectBottomBorder, pResult,
								nIppStep, srcRectBottomBorder, coeffs, IPPI_INTER_LINEAR); 
						}
					}
					return TRUE;
				}

				BOOL IPP_FoldVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bTop )
				{
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
					Ipp8u *  pDst = pBGRAResult;
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
							memcpy(pBGRAResult, pBGRASource2, nTileHStep);	
							memcpy(pBGRAResult+nTileHStep, pBGRASource1+nTileHStep, nIppStep*nHeight - nTileHStep);
						}
						else if ( nParts == 2*(nHTileCount-1) || nParts == 2*(nHTileCount-1)-1)
						{
							if ( nParts == 2*(nHTileCount-1))
								memcpy(pBGRAResult, pBGRASource2, nIppStep*nHeight);
							else
							{
								if ( nBottomBorder>1 )
								{
									int nBytes = nIppStep*nHeight - nBottomBorder*nIppStep;
									memcpy(pBGRAResult, pBGRASource2, nBytes);
									memcpy(pBGRAResult+nBytes, pBGRASource1+nBytes, nIppStep*nHeight - nBytes );
								}
								else
								{	
									int nBytes = nIppStep*nHeight - nTileHStep;
									memcpy(pBGRAResult, pBGRASource2, nBytes);
									memcpy(pBGRAResult+nBytes, pBGRASource1+nBytes, nTileHStep );
								}
							}
							roiSrcRect.y = nSizeY * (nHTileCount-2);
							pDst += (roiSrcRect.y + nSizeY)*nIppStep;
							if ( nBottomBorder > 1 )
							{
								if ( nParts == 2*(nHTileCount-1)-1 )
								{
									Ipp8u * pTmp = new	Ipp8u[ nTileHStep ];	
									ippiResize_8u_C4R(pBGRASource1, roiSize, nIppStep, roiSrcRect, pTmp, nIppStep, roiTileSize,
										1, dKoef, IPPI_INTER_LINEAR);
									roiTileSize.height = roiResizedSize.height;
									ippiMirror_8u_C4IR( pTmp, nIppStep, roiTileSize, ippAxsHorizontal );
									roiTileSize.height = ( roiResizedSize.height < nBottomBorder ) ? roiResizedSize.height : nBottomBorder;
									ippiCopy_8u_C4R( pTmp, nIppStep, pDst, nIppStep, roiTileSize );
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
									memcpy(pBGRAResult, pBGRASource2, nBytes);
									memcpy(pBGRAResult+nBytes, pBGRASource1+nBytes, nIppStep*nHeight - nBytes);
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
							memcpy(pBGRAResult, pBGRASource1, nBytes);
							memcpy(pBGRAResult + nBytes, pBGRASource2 + nBytes, nTileHStep);	
							roiSrcRect.y = nHeight - nSizeY;
							pDst += roiSrcRect.y * nIppStep;
						}
						else if ( nParts == 2*(nHTileCount-1) || nParts == 2*(nHTileCount-1)-1)
						{
							if ( nParts == 2*(nHTileCount-1))
								memcpy(pBGRAResult, pBGRASource2, nIppStep*nHeight);
							else
							{
								if ( nBottomBorder>1 )
								{
									int nBytes = nBottomBorder*nIppStep;
									memcpy(pBGRAResult, pBGRASource1, nBytes);
									memcpy(pBGRAResult+nBytes, pBGRASource2+nBytes, nIppStep*nHeight - nBytes );
								}
								else
								{	
									int nBytes = nTileHStep;
									memcpy(pBGRAResult, pBGRASource1, nTileHStep);
									memcpy(pBGRAResult+nTileHStep, pBGRASource2+nTileHStep, nIppStep*nHeight - nTileHStep );
								}
							}
							roiSrcRect.y = nHeight - nSizeY*(nHTileCount-1);
							if ( nBottomBorder > 1 )
							{
								if ( nParts == 2*(nHTileCount-1)-1 )
								{
									Ipp8u * pTmp = new	Ipp8u[ nTileHStep ];	
									int nTmpShift;
									ippiResize_8u_C4R(pBGRASource1, roiSize, nIppStep, roiSrcRect, pTmp, nIppStep, roiTileSize,
										1, dKoef, IPPI_INTER_LINEAR);
									roiTileSize.height = roiResizedSize.height;
									ippiMirror_8u_C4IR( pTmp, nIppStep, roiTileSize, ippAxsHorizontal );
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
									ippiCopy_8u_C4R( pTmp+nTmpShift, nIppStep, pDst+nDstShift, nIppStep, roiTileSize );
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
									memcpy(pBGRAResult+nBytes, pBGRASource2+nBytes, nIppStep*nHeight - nBytes);
									memcpy(pBGRAResult, pBGRASource1, nBytes);
									roiSrcRect.y = nHeight - nSizeY*i;
									pDst += (roiSrcRect.y - nSizeY)*nIppStep;
									break;
								}
							}
					}
					
					ippiResize_8u_C4R(pBGRASource1, roiSize, nIppStep, roiSrcRect, pDst+nDstShift, nIppStep, roiResizedSize,
    									1, dKoef, IPPI_INTER_LINEAR);
					if ( nParts != 0 )
						ippiMirror_8u_C4IR( pDst + nDstShift, nIppStep, roiResizedSize, ippAxsHorizontal );

					return TRUE;
				}



				BOOL IPP_FoldHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY,	double dFrame, BOOL bLeft )
				{
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
					Ipp8u *  pDst = pBGRAResult;
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
							ippiCopy_8u_C4R( pBGRASource2, nIppStep, pBGRAResult, nIppStep, roiTileSize );
							roiSize.width -= nSizeX;
							ippiCopy_8u_C4R( pBGRASource1 + nTileWStep, nIppStep, pBGRAResult + nTileWStep, nIppStep, roiSize );
							roiSize.width += nSizeX;
						}
						else if ( nParts == 2*(nWTileCount-1) || nParts == 2*(nWTileCount-1)-1)
						{
							if ( nParts == 2*(nWTileCount-1))
								memcpy(pBGRAResult, pBGRASource2, nIppStep*nHeight);
							else
							{
								if ( nRightBorder>1 )
								{
									int nBytes = (nWidth - nRightBorder)*4;
									roiSize.width = (nWidth - nRightBorder);
									ippiCopy_8u_C4R( pBGRASource2, nIppStep, pBGRAResult, nIppStep, roiSize );				
									roiSize.width = nRightBorder;
									ippiCopy_8u_C4R( pBGRASource1 + nBytes, nIppStep, pBGRAResult + nBytes, nIppStep, roiSize );
									roiSize.width = nWidth;
								}
								else
								{	
									roiSize.width = (nWidth - nSizeX);
									int nBytes = roiSize.width*4;
									ippiCopy_8u_C4R( pBGRASource2, nIppStep, pBGRAResult, nIppStep, roiSize );				
									ippiCopy_8u_C4R( pBGRASource1 + nBytes, nIppStep, pBGRAResult + nBytes, nIppStep, roiTileSize );
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
									ippiResize_8u_C4R(pBGRASource1, roiSize, nIppStep, roiSrcRect, pTmp, nTileWStep, roiTileSize,
														dKoef, 1, IPPI_INTER_LINEAR);
									roiTileSize.width = roiResizedSize.width;
									ippiMirror_8u_C4IR( pTmp, nTileWStep, roiTileSize, ippAxsVertical );
									roiTileSize.width = ( roiResizedSize.width < nRightBorder ) ? roiResizedSize.width : nRightBorder;
									ippiCopy_8u_C4R( pTmp, nTileWStep, pDst, nIppStep, roiTileSize );
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
									ippiCopy_8u_C4R( pBGRASource2, nIppStep, pBGRAResult, nIppStep, roiSize );
									roiSize.width = nWidth - roiSize.width;
									ippiCopy_8u_C4R( pBGRASource1 + nBytes, nIppStep, pBGRAResult + nBytes, nIppStep, roiSize );
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
							ippiCopy_8u_C4R( pBGRASource1, nIppStep, pBGRAResult, nIppStep, roiSize );
							ippiCopy_8u_C4R( pBGRASource2 + nBytes, nIppStep, pBGRAResult + nBytes, nIppStep, roiTileSize );
							roiSize.width = nWidth;
							roiSrcRect.x = nWidth - nSizeX;
							pDst += roiSrcRect.x * 4;
						}
						else if ( nParts == 2*(nWTileCount-1) || nParts == 2*(nWTileCount-1)-1)
						{
							if ( nParts == 2*(nWTileCount-1))
								memcpy(pBGRAResult, pBGRASource2, nIppStep*nHeight);
							else
							{
								if ( nRightBorder>1 )
								{
									roiSize.width = nRightBorder;
									int nBytes = nRightBorder*4;
									ippiCopy_8u_C4R( pBGRASource1, nIppStep, pBGRAResult, nIppStep, roiSize );				
									roiSize.width = nWidth - nRightBorder;
									ippiCopy_8u_C4R( pBGRASource2 + nBytes, nIppStep, pBGRAResult + nBytes, nIppStep, roiSize );
									roiSize.width = nWidth;
								}
								else
								{	
									ippiCopy_8u_C4R( pBGRASource1, nIppStep, pBGRAResult, nIppStep, roiTileSize );				
									roiSize.width = (nWidth - nSizeX);
									ippiCopy_8u_C4R( pBGRASource2 + nTileWStep, nIppStep, pBGRAResult + nTileWStep, nIppStep, roiSize );
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
									ippiResize_8u_C4R(pBGRASource1, roiSize, nIppStep, roiSrcRect, pTmp, nTileWStep, roiTileSize,
										dKoef, 1, IPPI_INTER_LINEAR);
									roiTileSize.width = roiResizedSize.width;
									ippiMirror_8u_C4IR( pTmp, nTileWStep, roiTileSize, ippAxsVertical );
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
									ippiCopy_8u_C4R( pTmp+nTmpShift, nTileWStep, pDst+nDstShift, nIppStep, roiTileSize );
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
									ippiCopy_8u_C4R( pBGRASource1, nIppStep, pBGRAResult, nIppStep, roiSize );
									roiSize.width = nWidth - roiSize.width;
									ippiCopy_8u_C4R( pBGRASource2 + nBytes, nIppStep, pBGRAResult + nBytes, nIppStep, roiSize );
									roiSize.width = nWidth;
									roiSrcRect.x = nWidth - nSizeX*i;
									pDst += (roiSrcRect.x - nSizeX)*4;
									break;
								}
							}
					}
					ippiResize_8u_C4R(pBGRASource1, roiSize, nIppStep, roiSrcRect, pDst+nDstShift, nIppStep, roiResizedSize,
										dKoef, 1, IPPI_INTER_LINEAR);
					if ( nParts != 0 )
						ippiMirror_8u_C4IR( pDst + nDstShift, nIppStep, roiResizedSize, ippAxsVertical );

					return TRUE;
				}

				//BOOL IPP_GaussianBlur(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame)
				//{
				//	IppiSize roiSize = {nWidth,nHeight};

				//	int nGaussianBlurStep=1;
				//	for (;nGaussianBlurStep < Constants::c_nGaussianBlurStepCount; nGaussianBlurStep++)
				//	{
				//		if (dFrame < Constants::c_dGaussianBlurStepBeginFrame[nGaussianBlurStep])
				//			break;
				//	}

				//	if (Constants::c_nGaussianBlurStepCount <= nGaussianBlurStep)
				//		nGaussianBlurStep = Constants::c_nGaussianBlurStepCount - 1;
				//	else if (1 >= nGaussianBlurStep)
				//		nGaussianBlurStep = 1;

				//	double dFrameIn;
				//	switch (nGaussianBlurStep)
				//	{
				//	case 1:
				//		memcpy(pBGRAResult,pBGRASource1,4*nWidth*nHeight);
				//		dFrameIn = dFrame/(Constants::c_dGaussianBlurStepBeginFrame[2] - Constants::c_dGaussianBlurStepBeginFrame[0]);
				//		break;
				//	case 2:
				//		{
				//			dFrameIn = dFrame/(Constants::c_dGaussianBlurStepBeginFrame[2] - Constants::c_dGaussianBlurStepBeginFrame[0]);
				//			Ipp8u alpha = 0.5*(dFrame - Constants::c_dGaussianBlurStepBeginFrame[1])/(Constants::c_dGaussianBlurStepBeginFrame[2] - Constants::c_dGaussianBlurStepBeginFrame[1]) * 255;
				//			ippiAlphaCompC_8u_C4R( 
				//				pBGRASource2, 4*nWidth, alpha,
				//				pBGRASource1, 4*nWidth, 255, 
				//				pBGRAResult, 4*nWidth, roiSize, ippAlphaOver);
				//		}
				//		break;
				//	case 3:
				//		{			
				//			dFrameIn = 1.0 - (dFrame - Constants::c_dGaussianBlurStepBeginFrame[2])/(Constants::c_dGaussianBlurStepBeginFrame[4] - Constants::c_dGaussianBlurStepBeginFrame[2]);
				//			Ipp8u alpha = 0.5*(1.0 - (dFrame - Constants::c_dGaussianBlurStepBeginFrame[2])/(Constants::c_dGaussianBlurStepBeginFrame[3] - Constants::c_dGaussianBlurStepBeginFrame[2])) * 255;
				//			ippiAlphaCompC_8u_C4R( 
				//				pBGRASource1, 4*nWidth, alpha,
				//				pBGRASource2, 4*nWidth, 255, 
				//				pBGRAResult, 4*nWidth, roiSize, ippAlphaOver);
				//		}
				//		break;
				//	case 4:
				//		dFrameIn = 1.0 - (dFrame - Constants::c_dGaussianBlurStepBeginFrame[2])/(Constants::c_dGaussianBlurStepBeginFrame[4] - Constants::c_dGaussianBlurStepBeginFrame[2]);
				//		memcpy(pBGRAResult,pBGRASource2,4*nWidth*nHeight);
				//		break;
				//	}

				//	int nSize = dFrameIn*nWidth/10.0;
				//	int nCount = 2*nSize+1;
				//	
				//	Ipp32s* pKernel = new Ipp32s[nCount];

				//	IppiSize roiBufferExtVSize;
				//	roiBufferExtVSize.width = nWidth;
				//	roiBufferExtVSize.height = nHeight + 2*nSize;
				//	Ipp8u* pBufferExtV = new Ipp8u[4*roiBufferExtVSize.width*roiBufferExtVSize.height];

				//	IppiSize roiBufferExtHSize;
				//	roiBufferExtHSize.width	= nWidth + 2*nSize;
				//	roiBufferExtHSize.height = nHeight;
				//	Ipp8u* pBufferExtH = new Ipp8u[4*roiBufferExtHSize.width*roiBufferExtHSize.height];

				//	if (pKernel && pBufferExtV && pBufferExtH)
				//	{
				//		for (int i = 0; i < nCount; ++i)
				//			pKernel[i] = 1;

				//		ippiCopyReplicateBorder_8u_C4R(pBGRAResult, 4*nWidth, roiSize, pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, nSize, 0);
				//		ippiFilterColumn_8u_AC4R((Ipp8u*)(pBufferExtV + nSize*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, pBGRAResult, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);

				//		ippiCopyReplicateBorder_8u_C4R(pBGRAResult, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, nSize);
				//		ippiFilterRow_8u_C4R((Ipp8u*)(pBufferExtH + 4*nSize), 4*roiBufferExtHSize.width, pBGRAResult, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);
				//		
				//		ippiCopyReplicateBorder_8u_C4R(pBGRAResult, 4*nWidth, roiSize,  pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, nSize, 0);
				//		ippiFilterColumn_8u_AC4R((Ipp8u*)(pBufferExtV + nSize*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, pBGRAResult, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);
				//		
				//		ippiCopyReplicateBorder_8u_C4R(pBGRAResult, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, nSize);
				//		ippiFilterRow_8u_C4R((Ipp8u*)(pBufferExtH + 4*nSize), 4*roiBufferExtHSize.width, pBGRAResult, 4*nWidth, roiSize, pKernel, nCount, nSize, nCount);
				//	}
				//	
				//	if (pBufferExtH)
				//		delete[] pBufferExtH;
				//	if (pBufferExtV)
				//		delete[] pBufferExtV;
				//	if (pKernel)
				//		delete[] pKernel;

				//	return TRUE;
				//}
				
				void IPP_BGRA_GaussianBlurHorizontal( Ipp16u* pMapBGR, Ipp8u* pDstBGRA, int nWidth, int nHeight, int nDstStride, int nMapStride, int nBlurSize )
				{
					Ipp32u nFactor = nBlurSize * 2 + 1;
					nFactor *= nFactor;

					for( ; nHeight > 0; --nHeight, pDstBGRA += nDstStride, pMapBGR += nMapStride )
					{
						Ipp8u*  pDst = pDstBGRA;
						Ipp16u* pMap = pMapBGR;

						// подсчЄт начальной суммы
						Ipp32u nB = Ipp32u(pMap[0] * (nBlurSize + 1));
						Ipp32u nG = Ipp32u(pMap[1] * (nBlurSize + 1));
						Ipp32u nR = Ipp32u(pMap[2] * (nBlurSize + 1));

						int nLength = nBlurSize;
						for( ; nLength > 0; --nLength )
						{
							pMap += 3;
							nB += pMap[0];
							nG += pMap[1];
							nR += pMap[2];
						}

						// расчЄт новых пикселов в строке
						pMap = pMapBGR;
						nLength = nWidth;
						
						for( ;; )
						{
							pDst[0] = BYTE(nB / nFactor);
							pDst[1] = BYTE(nG / nFactor);
							pDst[2] = BYTE(nR / nFactor);

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}
							pDst += 4; 

							// вычитаем крайнее левое значение
							int index = 3 * ((nLength > nWidth - nBlurSize) ? nWidth - nLength : nBlurSize);
							nB -= pMap[0 - index];
							nG -= pMap[1 - index];
							nR -= pMap[2 - index];

							// переходим на следующий пиксел
							--nLength;
							pMap += 3;
							
							// прибавл€ем следующее крайнее правое значение
							index = 3 * (nLength > nBlurSize ? nBlurSize : nLength - 1);
							nB += pMap[0 + index];
							nG += pMap[1 + index];
							nR += pMap[2 + index];
						}
					}
				}
				void IPP_BGRA_GaussianBlurVertical(Ipp8u* pSrcBGRA, Ipp16u* pMapBGR, int nWidth, int nHeight, int nSrcStride, int nMapStride, int nBlurSize )
				{
					for( ; nWidth > 0; --nWidth, pSrcBGRA += 4, pMapBGR += 3 )
					{
						Ipp8u*  pSrc = pSrcBGRA;
						Ipp16u* pMap = pMapBGR;

						// подсчЄт начальной суммы
						Ipp16u nB = Ipp16u(pSrc[0] * (nBlurSize + 1));
						Ipp16u nG = Ipp16u(pSrc[1] * (nBlurSize + 1));
						Ipp16u nR = Ipp16u(pSrc[2] * (nBlurSize + 1));

						int nLength = nBlurSize;
						while( nLength-- > 0 )
						{
							pSrc += nSrcStride;
							nB += pSrc[0];
							nG += pSrc[1];
							nR += pSrc[2];
						}

						// расчЄт новых пикселов в строке
						pSrc = pSrcBGRA;
						nLength = nHeight;
						
						for( ;; )
						{
							pMap[0] = nB;
							pMap[1] = nG;
							pMap[2] = nR;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}
							pMap += nMapStride;

							// вычитаем крайнее верхнее значение
							int index = nSrcStride * ((nLength > nHeight - nBlurSize) ? nHeight - nLength : nBlurSize);
							nB -= pSrc[0 - index];
							nG -= pSrc[1 - index];
							nR -= pSrc[2 - index];

							// переходим на следующий пиксел
							--nLength;
							pSrc += nSrcStride;
							
							// прибавл€ем следующее крайнее нижнее значение
							index = nSrcStride * (nLength > nBlurSize ? nBlurSize : nLength - 1);
							nB += pSrc[0 + index];
							nG += pSrc[1 + index];
							nR += pSrc[2 + index];
						}
					}
				}
				BOOL IPP_GaussianBlur2(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame)
				{
					// смешиваем картинки согласно таблице интервалов
					int nSize = nWidth * nHeight;

					const double c_dStepBegin = 0.3;
					const double c_dStepEnd   = 0.7;

					AlphaBlend( pBGRAResult, pBGRASource1, pBGRASource2, nSize, (dFrame - c_dStepBegin)/(c_dStepEnd - c_dStepBegin) );

					// определ€ем размер блюра
					double dBlurSize = min(nWidth, nHeight) / 4.0;
					if( dBlurSize > 256 ) 
						dBlurSize = 256;

					int nBlurSize = Round( dBlurSize * (1 - abs(dFrame * 2 - 1)) );
					if( nBlurSize < 1 )
						return TRUE;
					
					// создаЄм вспомогательный массив дл€ ускоренного расчЄта блюра
					Ipp16u* pMap = new Ipp16u[nSize * 3];
					if( !pMap )
						return TRUE;

					// собственно сам расчЄт эффекта
					int nImgStride = nWidth * 4;
					int nMapStride = nWidth * 3;

					// первый цикл
					IPP_BGRA_GaussianBlurVertical( pBGRAResult, pMap, nWidth, nHeight, nImgStride, nMapStride, nBlurSize );
					IPP_BGRA_GaussianBlurHorizontal( pMap, pBGRAResult, nWidth, nHeight, nImgStride, nMapStride, nBlurSize );

					// второй цикл
					IPP_BGRA_GaussianBlurVertical( pBGRAResult, pMap, nWidth, nHeight, nImgStride, nMapStride, nBlurSize );
					IPP_BGRA_GaussianBlurHorizontal( pMap, pBGRAResult, nWidth, nHeight, nImgStride, nMapStride, nBlurSize );

					delete [] pMap;
					
					return TRUE;
				}

				BOOL IPP_Noise(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bGrayscale)
				{
					int nSize = nWidth * nHeight;

					const double c_dStepBegin = 0.3;
					const double c_dStepEnd   = 0.7;

					AlphaBlend( pBGRAResult, pBGRASource1, pBGRASource2, nSize, (dFrame - c_dStepBegin)/(c_dStepEnd - c_dStepBegin) );

					int nMax = Round( 256 * (1 - abs(dFrame * 2 - 1)) );
					if( nMax < 1 )
						return TRUE;

					CRandom rnd( GetTickCount() );
					
					if( bGrayscale )
					{
						for( int i = 0; i < nSize; ++i, pBGRAResult += 4 )
						{
							int num = rnd.Rand( nMax );
							
							int b = num + pBGRAResult[0];
							pBGRAResult[0] = min(b, 255);

							int g = num + pBGRAResult[1];
							pBGRAResult[1] = min(g, 255);

							int r = num + pBGRAResult[2];
							pBGRAResult[2] = min(r, 255);
						}
					}
					else
					{
						for( int i = 0; i < nSize; ++i, pBGRAResult += 4 )
						{
							int b = rnd.Rand( nMax ) + pBGRAResult[0];
							pBGRAResult[0] = min(b, 255);

							int g = rnd.Rand( nMax ) + pBGRAResult[1];
							pBGRAResult[1] = min(g, 255);

							int r = rnd.Rand( nMax ) + pBGRAResult[2];
							pBGRAResult[2] = min(r, 255);
						}
					}

					return TRUE;
				}
					
				BOOL IPP_FlipBoxesVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY, double dFrame, BOOL bTop )
				{
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
					Ipp8u *  pDst = pBGRAResult;
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
							pSrc = pBGRASource1;
							if ( i == (nHTileCount - 1) && nBottomBorder > 1)
								roiTileSize.height = nBottomBorder;
							ippiCopy_8u_C4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
						}
						else if ( dFrame > dPrevFlips+dFlipTime )
						{
							pSrc = pBGRASource2;
							if ( i == (nHTileCount - 1) && nBottomBorder > 1)
								roiTileSize.height = nBottomBorder;
							ippiCopy_8u_C4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
						}
						else 
						{
							dKoeff = (dFrame - dPrevFlips)/dFlipTime;
							if ( dKoeff > 0.5 )
							{
								pSrc = pBGRASource2;
								dKoeff = 2 * (dKoeff - 0.5);
							}
							else
							{
								pSrc = pBGRASource1;
								dKoeff = 2 * (0.5 - dKoeff);
							}
							if ( dKoeff > 0.0001 )
							{
								if ( i == (nHTileCount - 1) && nBottomBorder > 1)
								{	
									roiSrcRect.height =  roiTileSize.height = nBottomBorder;
									ippiResizeCenter_8u_C4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
														nIppStep, roiTileSize, 1, dKoeff, xCenter, nBottomBorder*0.5, IPPI_INTER_LINEAR);
								}
								else
									ippiResizeCenter_8u_C4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
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

				BOOL IPP_FlipBoxesHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY, double dFrame, BOOL bLeft )
				{
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
					Ipp8u *  pDst = pBGRAResult;
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
							pSrc = pBGRASource1;
							if ( i == (nWTileCount - 1) && nRightBorder > 1)
								roiTileSize.width = nRightBorder;
							ippiCopy_8u_C4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
						}
						else if ( dFrame > dPrevFlips+dFlipTime )
						{
							pSrc = pBGRASource2;
							if ( i == (nWTileCount - 1) && nRightBorder > 1)
								roiTileSize.width = nRightBorder;
							ippiCopy_8u_C4R( pSrc + nDstShift, nIppStep, pDst + nDstShift, nIppStep, roiTileSize );			
						}
						else 
						{
							dKoeff = (dFrame - dPrevFlips)/dFlipTime;
							if ( dKoeff > 0.5 )
							{
								pSrc = pBGRASource2;
								dKoeff = 2 * (dKoeff - 0.5);
							}
							else
							{
								pSrc = pBGRASource1;
								dKoeff = 2 * (0.5 - dKoeff);
							}
							if ( dKoeff > 0.0001 )
							{
								if ( i == (nWTileCount - 1) && nRightBorder > 1)
								{	
									roiSrcRect.width =  roiTileSize.width = nRightBorder;
									ippiResizeCenter_8u_C4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
														nIppStep, roiTileSize, dKoeff, 1, nRightBorder*0.5, yCenter, IPPI_INTER_LINEAR);
								}
								else
									ippiResizeCenter_8u_C4R( pSrc + nDstShift, roiTileSize, nIppStep, roiSrcRect, pDst + nDstShift,
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

				BOOL IPP_FlipOutBoxesVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY, double dFrame, BOOL bIn)
				{
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
					
					memset(pBGRAResult,0,nIppStep*nHeight);

					if (dFrame <= 0.5)
					{
						pA = pBGRASource1;
						dFrame = 1.0 - dFrame;
					}
					else
					{
						pA = pBGRASource2;
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
						pDst = pBGRAResult;
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
							ippiResize_8u_C4R( pA + nSrcShift, roiTileSize, nIppStep, srcRect, pDst + nDstShift,
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
						ippiWarpBilinearQuad_8u_C4R( pDst, roiSize, nIppStep, srcRect, quadSrc, pBGRAResult,
													nIppStep, srcRect, quadDst, IPPI_INTER_LINEAR );

						delete [] pDst;	
					}

					return TRUE;
				}
				BOOL IPP_FlipOutBoxesHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nSizeX, int nSizeY, double dFrame, BOOL bIn)
				{
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
					
					memset(pBGRAResult,0,nIppStep*nHeight);

					if (dFrame < 0.5)
					{
						pA = pBGRASource1;
						dFrame = 1.0 - dFrame;
					}
					else
					{
						pA = pBGRASource2;
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
						pDst = pBGRAResult;
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
							ippiResize_8u_C4R( pA + nSrcShift, roiTileSize, nIppStep, srcRect, pDst + nDstShift,
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

						ippiWarpBilinearQuad_8u_C4R( pDst, roiSize, nIppStep, srcRect, quadSrc, pBGRAResult,
													nIppStep, srcRect, quadDst, IPPI_INTER_LINEAR );

						delete [] pDst;	
					}

					return TRUE;
				}


				BOOL IPP_Flash(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int Color)
				{
					BYTE TableB[256];
					BYTE TableG[256];
					BYTE TableR[256];
					
					int nB = GetBValue(Color);
					int nG = GetGValue(Color);
					int nR = GetRValue(Color);

					int alpha = Round((1 - abs(dFrame * 2 - 1)) * 255);

					nB = nB * alpha + 128; 
					nG = nG * alpha + 128; 
					nR = nR * alpha + 128; 

					alpha = 255 - alpha;

					for( int i = 0; i < 256; i++ )
					{
						TableB[i] = (i * alpha + nB) / 255;
						TableG[i] = (i * alpha + nG) / 255;
						TableR[i] = (i * alpha + nR) / 255;
					}

					BYTE* pSrc  = dFrame < 0.5 ? pBGRASource1 : pBGRASource2;
					BYTE* pDst  = pBGRAResult;
					int   nSize = nHeight * nWidth;

					for( int i = 0; i < nSize; ++i, pSrc += 4, pDst += 4 )
					{
						pDst[0] = TableB[pSrc[0]];
						pDst[1] = TableG[pSrc[1]];
						pDst[2] = TableR[pSrc[2]];
						pDst[3] = pSrc[3];
					}
					
					return TRUE;
				}

				BOOL IPP_CpyBar(BYTE* pBGRAResult, BYTE* pBGRASource, int nWidth, int nHeight, int nXCoorSource, int nYCoorSource, int nWidthBar, int nHeightBar, int nXCoorRes, int nYCoorRes)
				{
					if(!pBGRASource || !pBGRAResult)
						return FALSE;

					BYTE* ps = pBGRASource;
					BYTE* pr = pBGRAResult;
					ps += nYCoorSource * nWidth * 4;
					pr += nYCoorRes * nWidth * 4;
					for(int nYIndex = nYCoorRes; nYIndex < min(nHeight, nYCoorRes + nHeightBar); nYIndex++)
					{
						ps += nXCoorSource*4;
						pr += nXCoorRes*4;
						int nXIndex = nXCoorRes;
						for(; nXIndex < min(nWidth, nXCoorRes + nWidthBar); nXIndex++)
						{
							memcpy(pr, ps, 3);
							pr += 4; ps += 4;
						}
						for(; nXIndex < nWidth; nXIndex++)
						{
							pr += 4; ps += 4;
						}
						
					}
					return TRUE;

				}

				BOOL IPP_Tetris(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, Utility::CUtility* pUtility, BOOL bCollapse)
				{
					if( bCollapse )
					{
						Exchange( &pBGRASource1, &pBGRASource2 );
					}
					
					pUtility->GenerateRand(4);

					::memcpy( pBGRAResult, pBGRASource1, nWidth * nHeight * 4 );
					
					if(0 == pUtility->g_nRand)
					{
						ImageStudio::Paint::Compose::Utility::CTetrisFigure pFigures[5];
						
						int nBottomLineFigures[6];
						
						for(int i = 0; i < 6; i++)
						{
							nBottomLineFigures[i] = (int)(6 * nHeight * (dFrame - i / 6.0));
						}
						
						pFigures[0].SetValue( 3 * nWidth / 5,     nHeight / 2, 2, pBGRASource2, nWidth, nHeight, 2 * nWidth / 5,	              0);
						pFigures[1].SetValue( 2 * nWidth / 5, 3 * nHeight / 4, 6, pBGRASource2, nWidth, nHeight,              0,	              0);					
						pFigures[2].SetValue( 2 * nWidth / 5,     nHeight / 2, 1, pBGRASource2, nWidth, nHeight,     nWidth / 5,     nHeight / 4);					
						pFigures[3].SetValue( 2 * nWidth / 5, 3 * nHeight / 4, 4, pBGRASource2, nWidth, nHeight, 3 * nWidth / 5,     nHeight / 4);					
						pFigures[4].SetValue( 4 * nWidth / 5,     nHeight / 4, 1, pBGRASource2, nWidth, nHeight,              0,	3 * nHeight / 4);					
						
						if (!bCollapse)
						{
							pUtility->RunTetris(pFigures, 6, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
						else
						{
							pUtility->CollapseTetris(pFigures, 6, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
					}
					else if(1 == pUtility->g_nRand)
					{
						ImageStudio::Paint::Compose::Utility::CTetrisFigure pFigures[9];
						
						int nBottomLineFigures[10];
						
						for(int i = 0; i < 10; i++)
						{
							nBottomLineFigures[i] = (int)(10 * nHeight * (dFrame - i / 10.0));
						}
						
						pFigures[0].SetValue( 2 * nWidth / 6, 2 * nHeight / 6,	1, pBGRASource2, nWidth, nHeight, 4*nWidth/6,	0);					
						pFigures[1].SetValue(     nWidth / 6, 4 * nHeight / 6,	1, pBGRASource2, nWidth, nHeight, 0,				0);					
						pFigures[2].SetValue( 2 * nWidth / 6, 3 * nHeight / 6,	6, pBGRASource2, nWidth, nHeight, 2*nWidth/6,	0);					
						pFigures[3].SetValue( 2 * nWidth / 6, 3 * nHeight / 6,	3, pBGRASource2, nWidth, nHeight, 3*nWidth/6,	nHeight/6);					
						pFigures[4].SetValue( 2 * nWidth / 6, 3 * nHeight / 6,	4, pBGRASource2, nWidth, nHeight, 4*nWidth/6,	2*nHeight/6);					
						pFigures[5].SetValue(     nWidth / 6, 4 * nHeight / 6,	1, pBGRASource2, nWidth, nHeight, nWidth/6,		0);					
						pFigures[6].SetValue( 3 * nWidth / 6, 2 * nHeight / 6,	5, pBGRASource2, nWidth, nHeight, nWidth/6,		3*nHeight/6);					
						pFigures[7].SetValue( 3 * nWidth / 6, 2 * nHeight / 6,	5, pBGRASource2, nWidth, nHeight, 3*nWidth/6,	4*nHeight/6);					
						pFigures[8].SetValue( 3 * nWidth / 6, 2 * nHeight / 6,	7, pBGRASource2, nWidth, nHeight, 0,				4*nHeight/6);					
						
						if( !bCollapse )
						{
							pUtility->RunTetris(pFigures, 10, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
						else
						{
							pUtility->CollapseTetris(pFigures, 10, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
					}
					else if(2 == pUtility->g_nRand)
					{
						ImageStudio::Paint::Compose::Utility::CTetrisFigure pFigures[9];
						
						int nBottomLineFigures[10];
						
						for( int i = 0; i < 10; i++ )
						{
							nBottomLineFigures[i] = (int)(10 * nHeight * (dFrame - i / 10.0));
						}
						
						pFigures[0].SetValue(3*nWidth/6,	2*nHeight/6,	2, pBGRASource2, nWidth, nHeight, 0,				0);					
						pFigures[1].SetValue(2*nWidth/6,	2*nHeight/6,	1, pBGRASource2, nWidth, nHeight, 3*nWidth/6,	0);					
						pFigures[2].SetValue(2*nWidth/6,	3*nHeight/6,	3, pBGRASource2, nWidth, nHeight, 0,				nHeight/6);					
						pFigures[3].SetValue(3*nWidth/6,	2*nHeight/6,	7, pBGRASource2, nWidth, nHeight, 2*nWidth/6,	nHeight/6);					
						pFigures[4].SetValue(nWidth/6,		4*nHeight/6,	1, pBGRASource2, nWidth, nHeight, 5*nWidth/6,	0);					
						pFigures[5].SetValue(3*nWidth/6,	2*nHeight/6,	2, pBGRASource2, nWidth, nHeight, nWidth/6,		3*nHeight/6);					
						pFigures[6].SetValue(3*nWidth/6,	2*nHeight/6,	5, pBGRASource2, nWidth, nHeight, 3*nWidth/6,	3*nHeight/6);					
						pFigures[7].SetValue(4*nWidth/6,	nHeight/6,		1, pBGRASource2, nWidth, nHeight, 2*nWidth/6,	5*nHeight/6);					
						pFigures[8].SetValue(2*nWidth/6,	2*nHeight/6,	1, pBGRASource2, nWidth, nHeight, 0,				4*nHeight/6);					
						
						if( !bCollapse )
						{
							pUtility->RunTetris(pFigures, 10, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
						else
						{
							pUtility->CollapseTetris(pFigures, 10, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
					}
					else if(3 == pUtility->g_nRand)
					{
						ImageStudio::Paint::Compose::Utility::CTetrisFigure pFigures[4];
						
						int nBottomLineFigures[5];
						
						for( int i = 0; i < 5; i++ )
						{
							nBottomLineFigures[i] = (int)(5 * nHeight * (dFrame - i / 5.0));
						}
						
						pFigures[0].SetValue(nWidth,		nHeight/4,		1, pBGRASource2, nWidth, nHeight, 0,				0);					
						pFigures[1].SetValue(3*nWidth/4,	2*nHeight/4,	2, pBGRASource2, nWidth, nHeight, 0,				nHeight/4);					
						pFigures[2].SetValue(2*nWidth/4,	3*nHeight/4,	4, pBGRASource2, nWidth, nHeight, 2*nWidth/4,	nHeight/4);					
						pFigures[3].SetValue(3*nWidth/4,	2*nHeight/4,	7, pBGRASource2, nWidth, nHeight, 0,				2*nHeight/4);					
						
						if( !bCollapse )
						{
							pUtility->RunTetris(pFigures, 5, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
						else
						{
							pUtility->CollapseTetris(pFigures, 5, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
					}
					else if(4 == pUtility->g_nRand || !(4 == pUtility->g_nRand))
					{
						ImageStudio::Paint::Compose::Utility::CTetrisFigure pFigures[6];
						
						int nBottomLineFigures[6];
						
						for(int i = 0; i < 6; i++)
						{
							nBottomLineFigures[i] = (int)(6 * nHeight * (dFrame - i / 6.0));
						}
						
						pFigures[0].SetValue(2*nWidth/4,	2*nHeight/5,	1, pBGRASource2, nWidth, nHeight, 2*nWidth/4,	0);					
						pFigures[1].SetValue(2*nWidth/4,	3*nHeight/5,	6, pBGRASource2, nWidth, nHeight, 0,				0);					
						pFigures[2].SetValue(2*nWidth/4,	3*nHeight/5,	3, pBGRASource2, nWidth, nHeight, nWidth/4,		nHeight/5);					
						pFigures[3].SetValue(2*nWidth/4,	3*nHeight/5,	4, pBGRASource2, nWidth, nHeight, 2*nWidth/4,	2*nHeight/5);					
						pFigures[4].SetValue(3*nWidth/4,	2*nHeight/5,	7, pBGRASource2, nWidth, nHeight, 0,				3*nHeight/5);					
						
						if( !bCollapse )
						{
							pUtility->RunTetris(pFigures, 6, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
						else
						{
							pUtility->CollapseTetris(pFigures, 6, dFrame, pBGRAResult, nWidth, nHeight, 0, nBottomLineFigures);
						}
					}

					return TRUE;
				}

				BOOL IPP_ShotChangeHorizontal(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nType, BOOL bInverse, Utility::CUtility* pUtility, BYTE nBkColor,	BYTE nFilmColorMid,	BYTE nFilmColorBorder, BYTE nBlendColor)
				{
					
					dFrame = min(max(dFrame, 0.0), 1.0);
					const double dPercent  = 9. / 35;

					pUtility->GenerateShot(nWidth, nHeight, 0, nType, bInverse, TRUE, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					
					IppiSize roiSize = {nWidth,nHeight};
					IppiRect srcRect = {0,0,nWidth,nHeight};
					IppiSize roiScaleSize = {nWidth,nHeight};
					
					BYTE *pResult = pBGRAResult;

					if(dFrame < 1./3.)
					{						
						double dKoef = 1 - dFrame*3;
						double dFactor = (1./(1 - dPercent) * dKoef + 1 *(1-dKoef));
						double dFactor2 = ((1 - dPercent) * (1 - dKoef) + 1 * dKoef);
						IPP_ResizeCenter(pUtility->g_pShotSource1, pResult, nWidth, nHeight, dFactor, dFactor, nWidth / 2, nHeight / 2);
						IPP_ResizeCenter(pBGRASource1, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
						
					}
					else if(dFrame < 2./3.)
					{
						BYTE* pBuffer1 = new BYTE[4 * nWidth * nHeight];
						BYTE* pBuffer2 = new BYTE[4 * nWidth * nHeight];

						::memcpy(pBuffer1, pUtility->g_pShotSource1, 4 * nWidth * nHeight);
						IPP_ResizeCenter(pBGRASource1, pBuffer1, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);

						::memcpy(pBuffer2, pUtility->g_pShotSource2, 4 * nWidth * nHeight);
						IPP_ResizeCenter(pBGRASource2, pBuffer2, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);
						if( !bInverse )
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
						IPP_ResizeCenter(pUtility->g_pShotSource2, pResult, nWidth, nHeight, dFactor, dFactor, nWidth / 2, nHeight / 2);
						IPP_ResizeCenter(pBGRASource2, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
					}
									
					return TRUE;
				}

				BOOL IPP_ShotChangeVertical(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nType, BOOL bInverse, Utility::CUtility* pUtility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
				{					
					dFrame = min(max(dFrame, 0.0), 1.0);
					const double dPercent = 9. / 35;

					pUtility->GenerateShot(nWidth, nHeight, 1, nType, bInverse, TRUE, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);
					
					IppiSize roiSize = {nWidth,nHeight};
					IppiRect srcRect = {0,0,nWidth,nHeight};
					IppiSize roiScaleSize = {nWidth,nHeight};
					
					Ipp8u *pResult = pBGRAResult;

					if(dFrame < 1./3.)
					{						
						double dKoef = 1 - dFrame*3;
						double dFactor1 = (1./(1- dPercent) * dKoef + 1 *(1-dKoef));
						double dFactor2 = 1 * dKoef + (1 - dPercent) * (1 - dKoef);

						IPP_ResizeCenter(pUtility->g_pShotSource1, pResult, nWidth, nHeight, dFactor1, dFactor1, nWidth / 2, nHeight / 2);
						IPP_ResizeCenter(pBGRASource1, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
					}
					else if(dFrame < 2./3.)
					{
						BYTE* pBuffer1 = new BYTE[4 * nWidth * nHeight];
						BYTE* pBuffer2 = new BYTE[4 * nWidth * nHeight];

						::memcpy(pBuffer1, pUtility->g_pShotSource1, 4 * nWidth * nHeight);
						IPP_ResizeCenter(pBGRASource1, pBuffer1, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);

						::memcpy(pBuffer2, pUtility->g_pShotSource2, 4 * nWidth * nHeight);
						IPP_ResizeCenter(pBGRASource2, pBuffer2, nWidth, nHeight, 1 - dPercent, 1 - dPercent, nWidth / 2, nHeight / 2);

						if(bInverse)
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
						IPP_ResizeCenter(pUtility->g_pShotSource2, pResult, nWidth, nHeight, dFactor1, dFactor1, nWidth / 2, nHeight / 2);
						IPP_ResizeCenter(pBGRASource2, pResult, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
					}
					return TRUE;
				}

				BOOL IPP_ShotChangeHorizontalWithoutZoom(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nType, BOOL bInverse, Utility::CUtility* pUtility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
				{
					dFrame = min(max(dFrame, 0.0), 1.0);

					BYTE *pS1 = pBGRASource1;
					BYTE *pS2 = pBGRASource2;

					IppiSize roiSize = {nWidth,nHeight};

					const double dPercent  = 9. / 35;
					const double dXFactor = 0.01;

					pUtility->GenerateShot(nWidth, nHeight, 0, nType, bInverse, FALSE, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);

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
						if(!bInverse)
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


				BOOL IPP_ShotChangeVerticalWithoutZoom(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nType, BOOL bInverse, Utility::CUtility* pUtility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
				{
					dFrame = min(max(dFrame, 0.0), 1.0);

					BYTE *pS1 = pBGRASource1;
					BYTE *pS2 = pBGRASource2;

					IppiSize roiSize = {nWidth,nHeight};

					const double dPercent  = 9. / 35;
					const double dYFactor = 0.01;

					pUtility->GenerateShot(nWidth, nHeight, 1, nType, bInverse, FALSE, dPercent, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor);

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
						if(bInverse)
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


				BOOL IPP_BlackWhiteLines(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, BOOL bInverse)
				{
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
								oBlackBrush->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
								oWhiteBrush->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
								
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
								oBlackBrush->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
								oWhiteBrush->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
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

				BOOL IPP_Spiral(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nDirection)
				{
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
							dStartAngle -= 180 / IPP_PI * atan(double(nHeight * 1. / nWidth));
							nStep = nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * IPP_PI180));
						}
						else
						{
							dStartAngle -= 90 + 180 / IPP_PI * atan(double(nWidth * 1. / nHeight));
							nStep = -nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * IPP_PI180));
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
							dStartAngle -= 180 / IPP_PI * atan(double(nHeight * 1. / nWidth));
							nStep = nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * IPP_PI180));
						}
						else
						{
							dStartAngle -= 90 + 180 / IPP_PI * atan(double(nWidth * 1. / nHeight));
							nStep = -nWidth / 2 * 360 / (dStartAngle * cos(dStartAngle * IPP_PI180));
						}
						dAngle = dStartAngle * dFrame;
					}
					else
					{
						dAngle = dStartAngle * (1. - dFrame);
					}
					nX = nStep * dAngle / 360 * cos(dAngle * IPP_PI180);
					nY = nStep * dAngle / 360 * sin(dAngle * IPP_PI180);
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
					ippiResize_8u_C4R(
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
						ippiAlphaCompC_8u_C4R(pBuffer, 4 * nWidth, 255 * dBlendKoef, pSource1, 4*nWidth, 255 * ( 1 - dBlendKoef ), pBGRAResult, 4*nWidth, roiSize, ippAlphaPlus);
					}
					else
					{
						::memcpy(pBGRAResult, pBuffer, 4 * nWidth * nHeight);
					}

					delete []pBuffer2;
					delete []pBuffer;
					return TRUE;
				}
				BOOL IPP_Mask(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nType, Utility::CMask& oMaskManager, int nFadeWide)
				{
					oMaskManager.GenerateMask( nWidth, nHeight, nType );

					BYTE* pMask = oMaskManager.m_pMask;
					if( !pMask )
						return FALSE;

					double dFactor = 255.0 / (nFadeWide ? nFadeWide : 1);
					int nFrame = int((255 + nFadeWide) * dFrame + 0.5);
					
					DWORD Table[512];
					for( int i = 0; i < 256; i++ )
					{
						DWORD alpha2 = DWORD((nFrame - i) * dFactor);
						DWORD alpha1 = 255 - alpha2;

						alpha1 *= 0x8081;
						alpha2 *= 0x8081;

						Table[i * 2 + 0] = alpha1;
						Table[i * 2 + 1] = alpha2;
					}

					int nSize = nWidth * nHeight;
					for( int i = 0; i < nSize; ++i, ++pMask, pBGRASource1 += 4, pBGRASource2 += 4, pBGRAResult += 4 )
					{
						int mask = pMask[0];
						
						if( nFrame < mask )
						{
							*((DWORD*)pBGRAResult) = *((DWORD*)pBGRASource1);
							continue;
						}
						
						if( nFrame - nFadeWide >= mask )
						{
							*((DWORD*)pBGRAResult) = *((DWORD*)pBGRASource2);
							continue;
						}

						DWORD alpha1 = Table[mask * 2 + 0];
						DWORD alpha2 = Table[mask * 2 + 1];
						
						pBGRAResult[0] = (pBGRASource1[0] * alpha1 + pBGRASource2[0] * alpha2 + (128 * 0x8081)) >> 23;
						pBGRAResult[1] = (pBGRASource1[1] * alpha1 + pBGRASource2[1] * alpha2 + (128 * 0x8081)) >> 23;
						pBGRAResult[2] = (pBGRASource1[2] * alpha1 + pBGRASource2[2] * alpha2 + (128 * 0x8081)) >> 23;
					}

					return TRUE;
				}

				BOOL IPP_Eye(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nWideFade)
				{
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
				}

				BOOL IPP_Heart(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nWideFade)
				{

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
				}

				BOOL IPP_Pixelate(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame)
				{
					int nSize = nWidth * nHeight;

					const double c_dStepBegin = 0.3;
					const double c_dStepEnd   = 0.7;

					AlphaBlend( pBGRAResult, pBGRASource1, pBGRASource2, nSize, (dFrame - c_dStepBegin)/(c_dStepEnd - c_dStepBegin) );

					int nBarSize = Round( (1 - abs(dFrame * 2 - 1)) * pow( nWidth, 0.64 ) ) + 1;
					if( nBarSize <= 1 )
						return TRUE;

					int nBaseX = (nWidth  / 2) % nBarSize;
					int nBaseY = (nHeight / 2) % nBarSize;

					if( nBaseX ) nBaseX -= nBarSize;
					if( nBaseY ) nBaseY -= nBarSize;

					int nStride = nWidth * 4;

					int nPosY = 0;
					int nMaxY = nBaseY + nBarSize;

					for( int nBarY = nBaseY; nBarY < nHeight; nBarY += nBarSize, nPosY = nMaxY, nMaxY += nBarSize )
					{
						if( nMaxY > nHeight )
							nMaxY = nHeight;

						BYTE* pBarPixels = pBGRAResult + nPosY * nStride;

						int nPosX = 0;
						int nMaxX = nBaseX + nBarSize;

						for( int nBarX = nBaseX; nBarX < nWidth; nBarX += nBarSize, nPosX = nMaxX, nMaxX += nBarSize )
						{
							if( nMaxX > nWidth )
								nMaxX = nWidth;
							
							BYTE* pSrc = pBarPixels + nPosX * 4;

							double dSumR = 0;
							double dSumG = 0;
							double dSumB = 0;

							for( int nY = 0; nY < nMaxY - nPosY; ++nY, pSrc += nStride )
							{
								for( int nX = 0; nX < nMaxX - nPosX; ++nX )
								{
									dSumB += pSrc[nX * 4 + 0];
									dSumG += pSrc[nX * 4 + 1];
									dSumR += pSrc[nX * 4 + 2];
								}
							}

							double dFactor = 1.0 / ((nMaxX - nPosX) * (nMaxY - nPosY));
							int nB = int(dSumB * dFactor + 0.5);
							int nG = int(dSumG * dFactor + 0.5);
							int nR = int(dSumR * dFactor + 0.5);

							BYTE* pDst = pBarPixels + nPosX * 4;

							for( int nY = 0; nY < nMaxY - nPosY; ++nY, pDst += nStride )
							{
								for( int nX = 0; nX < nMaxX - nPosX; ++nX )
								{
									pDst[nX * 4 + 0] = nB;
									pDst[nX * 4 + 1] = nG;
									pDst[nX * 4 + 2] = nR;
								}
							}
						}
					}

					return TRUE;
				}

				BOOL IPP_Roll(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nDirection)
				{
					IppiSize roiSize = {nWidth, nHeight};
					IppiRect roiRect = { 0, 0, nWidth, nHeight};
					dFrame = min(max(dFrame, 0.0), 1.0);
					BYTE* pSource1 = pBGRASource1;
					BYTE* pSource2 = pBGRASource2;
					BYTE* pResult = pBGRAResult;
					::memcpy(pResult, pSource2, 4 * nWidth * nHeight);
					if( 1 == nDirection )
					{
						ippiRotate_8u_C4R(
								pSource1, roiSize, 4 * nWidth, roiRect,
								pResult, 4*nWidth, roiRect,
								- 90 * dFrame, 0, 0, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
					}
					else
					{
						ippiRotateCenter_8u_C4R(
								pSource1, roiSize, 4 * nWidth, roiRect,
								pResult, 4*nWidth, roiRect,
								90 * (dFrame), nWidth, 0, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
					}
					return TRUE;
				}

				BOOL IPP_Slide(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nDirection)
				{
					dFrame = min(max(dFrame, 0), 1);

					::memcpy( pBGRAResult, pBGRASource2, nWidth * nHeight * 4 );

					if( 0 == nDirection )
					{
						int nMidLine = nHeight * (1 - dFrame);
						
						::memcpy( pBGRAResult + 4 * (nHeight - nMidLine) * nWidth, pBGRASource1, nMidLine * nWidth * 4);
					}
					else if (1 == nDirection)
					{
						int nMidLine = nWidth * dFrame;
						IppiSize roiSizeR = {nWidth - nMidLine,nHeight};
						
						ippiCopy_8u_C4R( pBGRASource1, nWidth * 4, pBGRAResult + nMidLine * 4, nWidth * 4, roiSizeR);
					}
					else if( 2 == nDirection )
					{
						int nMidLine = nHeight * (1 - dFrame);
						
						::memcpy(pBGRAResult, pBGRASource1 + (nHeight - nMidLine) * nWidth * 4, nMidLine * nWidth * 4);
					} 
					else if( 3 == nDirection)
					{
						int nMidLine = nWidth * dFrame;
						IppiSize roiSizeR = { nWidth - nMidLine , nHeight};
						
						ippiCopy_8u_C4R(pBGRASource1 + nMidLine * 4, nWidth * 4, pBGRAResult, nWidth * 4, roiSizeR);

					}
					return TRUE;
				}

				BOOL IPP_SlideCenter(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nDirection)
				{
					dFrame = min(max(dFrame, 0), 1);

					::memcpy( pBGRAResult, pBGRASource2, 4 * nWidth * nHeight );

					float fScale = float(1 - dFrame * 0.9);
					if( fScale < 0.0001 )
						return TRUE;

					float fHalfWidth  = float(nWidth  / 2.0);
					float fHalfHeight = float(nHeight / 2.0);

					Gdiplus::PointF oCenter;

					switch( nDirection )
					{
					case 0:	oCenter.Y = -fHalfHeight; break; // top
					case 1:	oCenter.X =  fHalfWidth ; break; // right
					case 2:	oCenter.Y =  fHalfHeight; break; // bottom
					case 3:	oCenter.X = -fHalfWidth ; break; // left
					default: 
						return FALSE;
					}

					float factor = float(dFrame * (1 + fScale * dFrame));
					oCenter.X *= factor; 
					oCenter.Y *= factor;

					int offset = 4 * nWidth * (nHeight - 1);

					Gdiplus::Bitmap oSourceImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBGRASource1 + offset );
					if( Gdiplus::Ok != oSourceImage.GetLastStatus() )
						return FALSE;

					Gdiplus::Bitmap oResultImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBGRAResult + offset );
					if( Gdiplus::Ok != oResultImage.GetLastStatus() )
						return FALSE;
					
					Gdiplus::Graphics oRender( &oResultImage );
					if( Gdiplus::Ok != oRender.GetLastStatus() )
						return FALSE;

					oRender.SetInterpolationMode( Gdiplus::InterpolationModeBilinear );

					oRender.TranslateTransform( -fHalfWidth, -fHalfHeight, MatrixOrderAppend );
					oRender.ScaleTransform    (  fScale,      fScale,      MatrixOrderAppend );
					oRender.TranslateTransform(  fHalfWidth,  fHalfHeight, MatrixOrderAppend );
					oRender.TranslateTransform(  oCenter.X,   oCenter.Y,   MatrixOrderAppend );

					oRender.DrawImage( &oSourceImage, 0, 0 ); 

					return TRUE;
				}

				BOOL IPP_Shrink(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nDirection)
				{
					dFrame = min(max(dFrame, 0.0), 1.0) * 2;
					BYTE* pSource1 = pBGRASource1;
					BYTE* pSource2 = pBGRASource2;
					BYTE* pResult = pBGRAResult;

					const double dXFactor = 0.03;
					const double dYFactor = 0.03;

					IppiSize roiSize = {nWidth, nHeight};
					IppiRect roiRect = {0, 0, nWidth, nHeight};

					BYTE* pBuffer = new BYTE[4 * nWidth * nHeight];
					::memcpy(pBuffer, pSource2, 4 * nWidth * nHeight);
					if(dFrame < 1)
					{

						if( 0 == nDirection )
						{
							ippiResizeCenter_8u_C4R(
								pSource1, roiSize, 4*nWidth, roiRect,
								pBuffer, 4*nWidth, roiSize,
								(1 - dFrame) + dXFactor * dFrame, 1, nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
						}
						else
						{
							ippiResizeCenter_8u_C4R(
								pSource1, roiSize, 4*nWidth, roiRect,
								pBuffer, 4*nWidth, roiSize,
								1, (1 - dFrame) + dYFactor * dFrame, nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
						}
						::memcpy(pResult, pBuffer, 4 * nWidth * nHeight);
					}
					else
					{
						if( 0 == nDirection )
						{
							ippiResizeCenter_8u_C4R(
								pSource1, roiSize, 4*nWidth, roiRect,
								pBuffer, 4*nWidth, roiSize,
								dXFactor, (1 - (dFrame - 1)) + dYFactor * (dFrame - 1), nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
						}
						else
						{
							ippiResizeCenter_8u_C4R(
								pSource1, roiSize, 4*nWidth, roiRect,
								pBuffer, 4*nWidth, roiSize,
								(1 - (dFrame - 1)) + dXFactor * (dFrame - 1), dYFactor, nWidth / 2, nHeight / 2,IPPI_INTER_LINEAR);
						}

						CAlphaTable table;
						table.CalculateEx( dFrame - 1 );

						for(int nYIndex = 0; nYIndex < nHeight; ++nYIndex)
						{
							for(int nXIndex = 0; nXIndex < nWidth; ++nXIndex, pResult += 4, pSource2 += 4, pBuffer += 4)
							{
								pResult[0] = table.Blend( pBuffer[0], pSource2[0] );
								pResult[1] = table.Blend( pBuffer[1], pSource2[1] );
								pResult[2] = table.Blend( pBuffer[2], pSource2[2] );
								pResult[3] = pSource2[2];
							}
						}
						
						pBuffer -= 4 * nWidth * nHeight;
					}
					delete [] pBuffer;
					return TRUE;
				}

				BOOL IPP_Puzzle(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, double dFrame, int nType, Utility::CPuzzleUtility* pUtility, int nXCount, const int nYCount, const int nFlyTime, int nCorners, BOOL bPuzzle, int nShuffleType, BOOL bCollapse)
				{
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

					ImageStudio::Paint::Compose::Utility::CPuzzleFigure *pFigures = new ImageStudio::Paint::Compose::Utility::CPuzzleFigure[nCount];
					
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
					// —оздаем различные графические инструменты дл€ отрисовки
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

				BOOL IPP_MaskGraduate(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTilesX, int nTilesY, int nType, double dFrame, Utility::CMaskGraduate* oMaskManager, int nFadeWide)
				{
					oMaskManager->GenerateMask( nWidth, nHeight, nType);

					Ipp16u* pMask = oMaskManager->m_pMask;
					if( !pMask )
						return FALSE;

					nFadeWide = int(nFadeWide / 255.0 * 65535.0 + 0.5);

					double dFactor = 65535.0 / (nFadeWide ? nFadeWide : 1);
					int nFrame = int((65535 + nFadeWide) * dFrame + 0.5);

					int nSize = nWidth * nHeight;
					for( int i = 0; i < nSize; ++i, ++pMask, pBGRASource1 += 4, pBGRASource2 += 4, pBGRAResult += 4 )
					{
						int mask = pMask[0];
						
						if( nFrame < mask )
						{
							*((DWORD*)pBGRAResult) = *((DWORD*)pBGRASource1);
							continue;
						}
						
						if( nFrame - nFadeWide >= mask )
						{
							*((DWORD*)pBGRAResult) = *((DWORD*)pBGRASource2);
							continue;
						}

						DWORD alpha = DWORD((nFrame - mask) * dFactor);

						pBGRAResult[0] = (pBGRASource2[0] * alpha + pBGRASource1[0] * (65535 - alpha) + 128) / 65535;
						pBGRAResult[1] = (pBGRASource2[1] * alpha + pBGRASource1[1] * (65535 - alpha) + 128) / 65535;
						pBGRAResult[2] = (pBGRASource2[2] * alpha + pBGRASource1[2] * (65535 - alpha) + 128) / 65535;
					}

					return TRUE;
				}

				BOOL IPP_Tiles(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nFigureType, double dFrame, double dSideFactor, int nType, Utility::CPuzzleUtility* pUtility, int nFlyTime, int nShuffleType, int nStartType, BOOL bCollapse)
				{
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
					// —оздаем различные графические инструменты дл€ отрисовки
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
					// —оздаем список кусочков
					//

					int nXCount = 0;
					int nYCount = 0;

					//
					// –асчет числа кусочков
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

					ImageStudio::Paint::Compose::Utility::CFigure *pFigures = new ImageStudio::Paint::Compose::Utility::CFigure[nCount];

					pUtility->GenerateRandomShuffle(nXCount, nYCount, nShuffleType, nType);
					pUtility->GenerateCorners(nWidth, nHeight, nCount, nStartType, nType);

					//
					// «аполн€ем данные о кусочках
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
					// ќбработка движени€ кусочков
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

			}
		}
	}
}

#pragma warning(default: 4244)
