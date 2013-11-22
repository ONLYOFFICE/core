#pragma once

#pragma comment(lib, "ippcorel.lib")
#pragma comment(lib, "ippiemerged.lib")
#pragma comment(lib, "ippimerged.lib")
#pragma comment(lib, "ippsemerged.lib")
#pragma comment(lib, "ippsmerged.lib")
#pragma comment(lib, "ippcvemerged.lib")
#pragma comment(lib, "ippcvmerged.lib")

#include <ippcore.h>
#include <ippdefs.h>
#include <ippi.h>
#include <ippcv.h>
#include <algorithm>

#include <ImagePaintComposeConstants.h>

#pragma warning(disable: 4244)

namespace ImageStudio
{
	namespace Paint
	{
		namespace Compose
		{
			inline int AlphaBlend( int dst, int src, int alpha )
			{
				return ((src - dst) * alpha >> 8) + dst;
			}


			
			namespace Utility
			{
				class CGraduate
				{
				public:
					
					CGraduate()
					{
						m_nSize = 0;
						m_pGraduation = NULL;
					}
					virtual ~CGraduate()
					{
						Clear();
					}
					
					BOOL Clear()
					{
						if (m_pGraduation)
							delete[] m_pGraduation;

						m_pGraduation = NULL;
						m_nSize = 0;

						return TRUE;
					}
					BOOL Create(int nSize)
					{
						if (nSize < 1)
							return Clear();
						if (nSize == m_nSize)
							return TRUE;

						m_nSize = nSize;
						m_pGraduation = new int[m_nSize];

						if (NULL == m_pGraduation)
						{
							Clear();
							return FALSE;
						}

						for (int index = 0; index < m_nSize; ++index)
							m_pGraduation[index] = index;

						CRandom rnd( 1 );

						for (int index = 0; index < m_nSize; ++index)
						{
							int nSwapIndex = rnd.Rand( m_nSize );
							int nSwapValue = m_pGraduation[nSwapIndex];

							m_pGraduation[nSwapIndex] = m_pGraduation[index];
							m_pGraduation[index] = nSwapValue;
						}

						return TRUE;
					}
					double GetLevel(int nIndex, double dFrame)
					{
						if (nIndex < 0 || nIndex >= m_nSize)
							return 0;
						if (dFrame <= 0)
							return 0;
						if (dFrame >= 1)
							return 1;

						double dLevel = m_nSize*dFrame;

						if (dLevel <= m_pGraduation[nIndex])
							return 0;
						if (dLevel >= m_pGraduation[nIndex] + 1)
							return 1;
						return dLevel - m_pGraduation[nIndex];
					}
				protected:
					
					int m_nSize;
					int* m_pGraduation; 
				};
				class CTetrisFigure
				{
				public:
					CTetrisFigure()
					{
						m_nFigHeight = 0;
						m_nFigWidth = 0;
						m_nPicHeight = 0;
						m_nPicWidth = 0;
						m_nType = 0;
						m_nXCoorSource = 0;
						m_nYCoorSource = 0;
					}
					CTetrisFigure(int nFigWidth, int nFigHeight, int nType, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
					{
						m_nFigHeight = nFigHeight;
						m_nFigWidth = nFigWidth;
						m_nType = nType;
						m_nPicHeight = nPicHeight;
						m_nPicWidth = nPicWidth;
						m_pBGRASource = pBGRASource;
						m_nXCoorSource = nXCoorSource;
						m_nYCoorSource = nYCoorSource;
					}
					~CTetrisFigure()
					{
					}

				private:

					void DrawAlphaBlendingLine(BYTE* pBGRAResult, BYTE* pBGRASource, int nColor, int nNumLine, double dEffect)
					{
						int nAlpha = 7 * (1 << nNumLine) & 0xff;
						nColor = nColor * nAlpha;
						nAlpha = 255 - nAlpha;

						if(1 == dEffect)
						{
							pBGRAResult[0] = (nColor + pBGRASource[0] *nAlpha) * 0x8081U >> (7 + 16);
							pBGRAResult[1] = (nColor + pBGRASource[1] *nAlpha) * 0x8081U >> (7 + 16);
							pBGRAResult[2] = (nColor + pBGRASource[2] *nAlpha) * 0x8081U >> (7 + 16);
						}
						else
						{
							double factor = 1 - dEffect;
							pBGRAResult[0] = dEffect * ((nColor + pBGRASource[0] *nAlpha) * 0x8081U >> (7 + 16)) + pBGRASource[0] * factor;
							pBGRAResult[1] = dEffect * ((nColor + pBGRASource[1] *nAlpha) * 0x8081U >> (7 + 16)) + pBGRASource[1] * factor;
							pBGRAResult[2] = dEffect * ((nColor + pBGRASource[2] *nAlpha) * 0x8081U >> (7 + 16)) + pBGRASource[2] * factor;

						}
						pBGRAResult[3] = pBGRASource[3];
					}

				public:

					void SetValue(int nFigWidth, int nFigHeight, int nType, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
					{
						m_nFigHeight = nFigHeight;
						m_nFigWidth = nFigWidth;
						m_nType = nType;
						m_nPicHeight = nPicHeight;
						m_nPicWidth = nPicWidth;
						m_pBGRASource = pBGRASource;
						m_nXCoorSource = nXCoorSource;
						m_nYCoorSource = nYCoorSource;
					}

					BOOL Cpy(BYTE* pBGRAResult, int nXCoorResult, int nYCoorResult, int nWidth, int nHeight, double dEffect)
					{
						if(!pBGRAResult || nWidth != m_nPicWidth || nHeight != m_nPicHeight)
							return FALSE;
						
						BYTE* pSource = m_pBGRASource;
						BYTE* pResultBuffer = new BYTE[ 4 * nWidth * ( 2 * nHeight)];
						BYTE* pResultBufferCurent = pResultBuffer;
						pResultBufferCurent += 4 * nWidth * nHeight;
						BYTE* pResultBufferMiddle = pResultBufferCurent;
						BYTE* pResult = pBGRAResult;
						
						IppiSize roiSize = { nWidth, nHeight};
						
						::memcpy(pResultBufferMiddle, pResult, 4 * nWidth * nHeight);
						//ippiCopy_8u_C4R( pResult, 4 * nWidth, pResultBufferMiddle, 4 * nWidth, roiSize);
						
						pSource             += m_nYCoorSource * nWidth * 4;
						pResultBufferCurent += nYCoorResult * nWidth * 4;
						
						if (1 == m_nType)
						{
							for(int nYIndex = nYCoorResult; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
							{
								pSource             += m_nXCoorSource*4;
								pResultBufferCurent += nXCoorResult*4;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min(nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if( nYCoorResult + 5 >= nYIndex &&
									  ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult)) && 
									    0 != dEffect)
									{
										DrawAlphaBlendingLine(pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
									}
									else if( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)) &&
										     0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
										   ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= min(nWidth, nXCoorResult + m_nFigWidth) - (min(nHeight, nYCoorResult + m_nFigHeight) - nYIndex)) &&
										     0 != dEffect) 
									{
										DrawAlphaBlendingLine(pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)) &&
										    0 != dEffect)
									{
										DrawAlphaBlendingLine(pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy(pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
						
							}
						}
						else if(2 == m_nType)
						{
							int nYIndex = nYCoorResult;
							
							for(; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight/2); nYIndex++)
							{
								pSource             += m_nXCoorSource * 4;
								pResultBufferCurent += nXCoorResult   * 4;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if( nYCoorResult + 5 >= nYIndex &&
									  ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult)) &&
									    0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
									}
									else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( min(nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)) &&
										     0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if( min( nHeight, nYCoorResult + m_nFigHeight / 2) - 5 <= nYIndex &&
										   ( ( nXIndex <= nXCoorResult +     m_nFigWidth / 3 - ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex) && nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex)) ||
										     ( nXIndex >= nXCoorResult + 2 * m_nFigWidth / 3 + ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex))) &&
										     0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight/2) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( nXIndex - nXCoorResult)) &&
										     0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy(pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
						
							}
							for(; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
							{
								pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 3);
								pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 3);

								int nXIndex          =       nXCoorResult   + m_nFigWidth / 3;

								for(; nXIndex < min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3); nXIndex++)
								{	
									if
										( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - 5 <= nXIndex 
										&& ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex) 
										&&   nYIndex <= min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex)) 
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - (min(nWidth, nXCoorResult + 2*m_nFigWidth/3)- 5), dEffect);
									}
									else if
										( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex 
										&& ( nXIndex >= nXCoorResult + m_nFigWidth / 3 + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex)
										&&   nXIndex <= min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex)) 
										&& 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
									}
									else if
										( nXCoorResult + m_nFigWidth / 3 + 5 >= nXIndex 
										&& ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( nXIndex - ( nXCoorResult + m_nFigWidth / 3)) 
										&&   nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - ( nXCoorResult + m_nFigWidth / 3)))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth/3 + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
						
							}

						}
						else if(3 == m_nType)
						{
							int nYIndex = nYCoorResult;

							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 3); nYIndex++)
							{
								pSource             += m_nXCoorSource * 4;
								pResultBufferCurent += nXCoorResult   * 4;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 2); nXIndex++)
								{	
									if	( nYCoorResult + 5 >= nYIndex 
										&& ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth / 2) - ( nYIndex - nYCoorResult))
										&&   0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
									}
									else if
										( min(nWidth, nXCoorResult + m_nFigWidth / 2) - 5 <= nXIndex &&
										( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - 5), dEffect);
									}
									else if
										( nXCoorResult + 5 >= nXIndex &&
										( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 3) - ( nXIndex - nXCoorResult))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
						
							}
							for(; nYIndex < min(nHeight, nYCoorResult + 2 * m_nFigHeight / 3); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if ( nYCoorResult + m_nFigHeight / 3 + 5 >= nYIndex &&
									   ( nXIndex <= nXCoorResult + m_nFigWidth  - ( nYIndex - nYCoorResult - m_nFigHeight / 3) && nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( nYIndex - nYCoorResult - m_nFigHeight / 3))
									   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 3 + 5 - nYIndex, dEffect);
									}
									else if
										( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
										( nYIndex >= nYCoorResult + m_nFigHeight / 3 + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) &&  nYIndex <= min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine(pResultBufferCurent, pSource, 0, nXIndex - (min(nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if
										( ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - 5 <= nYIndex) &&
										( nXIndex <= nXCoorResult + m_nFigWidth - ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex) && nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex)) 
										&& 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + 2*m_nFigHeight/3) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
						
							}
							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 2); nXIndex++)
								{	
									if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
									  ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= nXCoorResult + m_nFigWidth / 2 - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
									}
									else if( min(nWidth, nXCoorResult + m_nFigWidth / 2) - 5 <= nXIndex &&
										   ( nYIndex >= nYCoorResult + 2 * m_nFigHeight / 3 + ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex) && nYIndex <= nYCoorResult + m_nFigHeight - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth/2) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex &&
										     nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
						
							}

						}
						else if(4 == m_nType)
						{
							int nYIndex = nYCoorResult;

							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 3); nYIndex++)
							{
								pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 2);
								pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 2);

								int nXIndex = nXCoorResult + m_nFigWidth / 2;
								
								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if	( nYCoorResult + 5 >= nYIndex &&
										( nXIndex >= nXCoorResult + m_nFigWidth/2 + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult))
										&& 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
									}
									else if
										( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
										( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if
										( nXCoorResult + m_nFigWidth / 2 + 5 >= nXIndex &&
										( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult - m_nFigWidth / 2) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 3) - ( nXIndex - nXCoorResult - m_nFigWidth / 2))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth/2 + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
						
							}
							for(; nYIndex < min(nHeight, nYCoorResult + 2 * m_nFigHeight / 3); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if( nYCoorResult + m_nFigHeight / 3 + 5 >= nYIndex &&
									  ( nXIndex <= nXCoorResult + m_nFigWidth / 2  - ( nYIndex - nYCoorResult - m_nFigHeight / 3) && nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult - m_nFigHeight / 3))
										&& 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 3 + 5 - nYIndex, dEffect);
									}
									else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - (min(nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if( ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - 5 <= nYIndex) &&
										   ( nXIndex <= nXCoorResult + m_nFigWidth / 2 - ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex) && nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex)) 
										   && 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex &&
										   ( nYIndex >= nYCoorResult + m_nFigHeight / 3 + ( nXIndex - nXCoorResult) &&  nYIndex <= min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - ( nXIndex - nXCoorResult))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
						
							}
							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
							{
								pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 2);
								pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 2);

								int nXIndex = nXCoorResult + m_nFigWidth / 2;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
									  ( nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= nXCoorResult + m_nFigWidth - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
									  && 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( nYCoorResult + m_nFigHeight - 5), dEffect);
									}
									else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
										     nYIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if( nXCoorResult + m_nFigWidth / 2 + 5 >= nXIndex &&
										   ( nYIndex >= nYCoorResult + 2 * m_nFigHeight / 3 + ( nXIndex - nXCoorResult - m_nFigWidth / 2) && nYIndex <= nYCoorResult + m_nFigHeight - ( nXIndex - nXCoorResult - m_nFigWidth / 2))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine(pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth/2 + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
						
							}

						}
						else if(5 == m_nType)
						{
							int nYIndex = nYCoorResult;

							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 2); nYIndex++)
							{
								pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 3);
								pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 3);

								int nXIndex = nXCoorResult + m_nFigWidth / 3;

								for(; nXIndex < min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3); nXIndex++)
								{	
									if( nYCoorResult + 5 >= nYIndex &&
									  ( nXIndex >= nXCoorResult + m_nFigWidth / 3 + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - ( nYIndex - nYCoorResult))
										&& 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
									}
									else if( min(nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - 5 <= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex)) 
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - 5), dEffect);
									}
									else if( nXCoorResult + m_nFigWidth / 3 + 5 >= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult - m_nFigWidth / 3) && nYIndex <= min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - ( nXIndex - nXCoorResult - m_nFigWidth / 3))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth / 3 + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4; pSource += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4; pSource += 4;
								}
						
							}
							for(; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if( nYCoorResult + m_nFigHeight / 2 + 5 >= nYIndex &&
									  ( ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult - m_nFigHeight / 2) && nXIndex <= nXCoorResult + m_nFigWidth / 3 - ( nYIndex - nYCoorResult - m_nFigHeight / 2)) 
									  ||  nXIndex >= nXCoorResult + 2 * m_nFigWidth / 3 + ( nYIndex - nYCoorResult - m_nFigHeight / 2) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult - m_nFigHeight / 2))
										&& 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 2 + 5 - nYIndex, dEffect);
									}
									else if( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
										   ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)) 
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
										   ( nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex &&
										   ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
							}
						
						}

						else if(6 == m_nType)
						{
							int nYIndex = nYCoorResult;

							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 3); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if( nYCoorResult + 5 >= nYIndex &&
									  (nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult))
									  && 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
									}
									else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 3) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if( min( nHeight, nYCoorResult + m_nFigHeight / 3) - 5 <= nYIndex &&
										   ( nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( min( nHeight, nYCoorResult + m_nFigHeight / 3) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nHeight, nYCoorResult + m_nFigHeight / 3) - nYIndex))
										   && 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight / 3) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex && 
										     nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult)
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4; 
									pSource             += 4;
								}
						
							}
							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 2); nXIndex++)
								{	
									if( min( nWidth, nXCoorResult + m_nFigWidth / 2) - 5 <= nXIndex &&
									  ( nYIndex >= nYCoorResult + m_nFigHeight / 3 + ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex))
									  && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - 5), dEffect);
									}
									else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
										   ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth / 2) - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex && 
										     nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
							}
						
						}

						else if(7 == m_nType)
						{
							int nYIndex = nYCoorResult;

							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 2); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 3); nXIndex++)
								{	
									if( nYCoorResult + 5 >= nYIndex &&
									  ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth / 3) - ( nYIndex - nYCoorResult))
									  && 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
									}
									else if( min( nWidth, nXCoorResult + m_nFigWidth / 3) - 5 <= nXIndex &&
										   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth / 3) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( min( nWidth, nXCoorResult + m_nFigWidth / 3) - nXIndex))
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth / 3) - 5), dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex && 
										     nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult)
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
						
							}
							for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
							{
								pSource             += 4 * m_nXCoorSource;
								pResultBufferCurent += 4 * nXCoorResult;

								int nXIndex = nXCoorResult;

								for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
								{	
									if( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
									  ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
									  && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
									}
									else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
										   ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
										   && 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
									}
									else if( nYCoorResult + m_nFigHeight / 2 + 5 >= nYIndex &&
										   ( nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - ( nYCoorResult + m_nFigHeight / 2)) && nXIndex >= nXCoorResult + m_nFigWidth / 3 + ( nYIndex - ( nYCoorResult + m_nFigHeight / 2)))
										   && 0 != dEffect) 
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 2 + 5 - nYIndex, dEffect);
									}
									else if( nXCoorResult + 5 >= nXIndex && 
										     nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)
										   && 0 != dEffect)
									{
										DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
									}
									else
									{
										memcpy( pResultBufferCurent, pSource, 4);
									}
									pResultBufferCurent += 4;
									pSource             += 4;
								}
								for(; nXIndex < nWidth; nXIndex++)
								{
									pResultBufferCurent += 4;
									pSource             += 4;
								}
							}
						
						}

						else 
						{
							return FALSE;
						}
						ippiCopy_8u_C4R(pResultBufferMiddle, 4 * nWidth, pResult, 4 * nWidth, roiSize);

						delete pResultBuffer;

						return TRUE;
					}




				public:

					int m_nFigHeight;
					int m_nFigWidth;
					int m_nPicHeight;
					int m_nPicWidth;
					int m_nType;
					BYTE* m_pBGRASource;
					int m_nXCoorSource;
					int m_nYCoorSource;

				};

				class CUtility
				{
				public:
					DWORD m_dwBaseRnd;

				protected:

					BOOL DrawFilmsPerf(BYTE* pBGRA, int nWidth, int nHeight, int nBlendColor, int nBkColor, float dX, float dY, float dWidth, float dHeight, float dRadius)
					{
						BYTE* pBuffer = pBGRA; 
						pBuffer += 4 * nWidth * (nHeight - 1);
						
						Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBuffer );
						if( Gdiplus::Ok == oImage.GetLastStatus() )
						{
							Gdiplus::Graphics oRender( &oImage );
							
							oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
							oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
							oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
							
							Gdiplus::GraphicsPath oPath;

							oPath.AddLine(dX + dRadius, dY, dX + dWidth - (dRadius * 2), dY);
							oPath.AddArc(dX + dWidth - (dRadius * 2), dY, dRadius * 2, dRadius * 2, 270, 90);
							oPath.AddLine(dX + dWidth, dY + dRadius, dX + dWidth, dY + dHeight - (dRadius * 2));
							oPath.AddArc(dX + dWidth - (dRadius * 2), dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2,0,90);
							oPath.AddLine(dX + dWidth - (dRadius * 2), dY + dHeight, dX + dRadius, dY + dHeight);
							oPath.AddArc(dX, dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2, 90, 90);
							oPath.AddLine(dX, dY + dHeight - (dRadius * 2), dX, dY + dRadius);
							oPath.AddArc(dX, dY, dRadius * 2, dRadius * 2, 180, 90);
							oPath.CloseFigure();

							Gdiplus::PathGradientBrush *oBrush = new Gdiplus::PathGradientBrush(&oPath);
							oBrush->SetCenterPoint(Gdiplus::PointF(dX + dWidth / 2, dY + dHeight / 2));
							oBrush->SetCenterColor(Color(nBkColor, nBkColor, nBkColor));
							Color *oColor = new Color( nBkColor, nBkColor, nBkColor);
							int nColorCount = 1;
							oBrush->SetSurroundColors(oColor, &nColorCount);

							oRender.FillPath(oBrush, &oPath);

							Gdiplus::Pen *oPen = new Gdiplus::Pen(Color( nBlendColor, nBlendColor, nBlendColor), 1);
							oRender.DrawArc(  oPen, dX + dWidth - (dRadius * 2), dY, dRadius * 2, dRadius * 2, 315, 45);
							oRender.DrawLine( oPen, dX + dWidth, dY + dRadius - 1, dX + dWidth, dY + dHeight - dRadius + 1);
							oRender.DrawArc(  oPen, dX + dWidth - (dRadius * 2), dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2,0,90);
							oRender.DrawLine( oPen, dX + dWidth - dRadius + 1, dY + dHeight, dX + dRadius - 1, dY + dHeight);
							oRender.DrawArc(  oPen, dX, dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2, 90, 45);

							delete oColor;
							delete oPen;
							delete oBrush;
						}
						return TRUE;
					}

					BOOL DrawFilmsBorder(BYTE* pBGRA, int nWidth, int nHeight, int nFilmColorMid, int nFilmColorBorder, float dBorderWidth, float dBorderHeight, BOOL bVertical)
					{
						BYTE* pBuffer = pBGRA; 
						pBuffer += 4 * nWidth * (nHeight - 1);
						
						Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBuffer );
						if( Gdiplus::Ok == oImage.GetLastStatus() )
						{
							Gdiplus::Graphics oRender( &oImage );
							
							oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
							oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
							oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

							Gdiplus::Color clMid(nFilmColorMid, nFilmColorMid, nFilmColorMid);
							Gdiplus::Color clBorder(nFilmColorBorder, nFilmColorBorder, nFilmColorBorder);
							
							if ( bVertical )
							{
								Gdiplus::SolidBrush *oSolidBrush = new Gdiplus::SolidBrush(clMid);
								oRender.FillRectangle(oSolidBrush, -1.0, -1.0, float(nWidth + 1.0), float(nHeight + 1.0));

								Gdiplus::LinearGradientBrush* oGradBrush1 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1.0, -1.0), Gdiplus::PointF( dBorderWidth + 1, -1.0), clBorder, clMid);									
								oRender.FillRectangle(oGradBrush1, -1.0, -1.0, dBorderWidth + 1, float(nHeight) + 1);
								
								Gdiplus::LinearGradientBrush* oGradBrush2 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( nWidth + 1, -1.0), Gdiplus::PointF( nWidth - dBorderWidth -2.0, -1.0), clBorder, clMid);									
								oRender.FillRectangle(oGradBrush2, nWidth - dBorderWidth - 1, -1.0, dBorderWidth + 1, float(nHeight) + 1);

								delete oGradBrush1;
								delete oGradBrush2;
								delete oSolidBrush;
							}
							else
							{
								Gdiplus::SolidBrush *oSolidBrush = new Gdiplus::SolidBrush(clMid);
								oRender.FillRectangle(oSolidBrush, -1.0, -1.0, float(nWidth + 1.0), float(nHeight + 1.0));

								Gdiplus::LinearGradientBrush* oGradBrush1 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1.0, -1.0), Gdiplus::PointF( -1.0, dBorderHeight + 1), clBorder, clMid);									
								oRender.FillRectangle(oGradBrush1, -1.0, -1.0, float(nWidth) + 2.0, dBorderHeight + 1);
								
								Gdiplus::LinearGradientBrush* oGradBrush2 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1.0, float(nHeight) + 1.0), Gdiplus::PointF( -1.0, nHeight - dBorderHeight - 1.0), clBorder, clMid);									
								oRender.FillRectangle(oGradBrush2, float(-1.0), float(nHeight - dBorderHeight - 1.0), float(nWidth + 2.0), float(dBorderHeight + 2.0));

								delete oGradBrush1;
								delete oGradBrush2;
								delete oSolidBrush;
							}
						}
						return TRUE;
					}


				public:
					
					CUtility()
					{
						//g_sBurningPalette = 0;
						g_pBurningMask = 0;

						g_nBurningMaskWidth = -1;
						g_nBurningMaskHeight = -1;

						g_nRand = -1;

						//InitBurningPalette();

						g_pShotSource1 = 0;
						g_pShotSource2 = 0;
					    g_nShotWidth = -1;
					    g_nShotHeight = -1;
					    g_nShotType = -1;

						g_ppWipeRandomStrips = 0;
						g_nWipeStripsCount = -1;

						m_dwBaseRnd = ::rand();

					}
					~CUtility()
					{
						//if (g_sBurningPalette)
						//	delete[] g_sBurningPalette;
						if (g_pBurningMask)
							delete[] g_pBurningMask;
						if (g_pShotSource1)
							delete[] g_pShotSource1;
						if (g_pShotSource2)
							delete[] g_pShotSource2;
						for(int nIndex = 0; nIndex < g_nWipeStripsCount; nIndex++ )
						{
							if(g_ppWipeRandomStrips[nIndex])
								delete[] g_ppWipeRandomStrips[nIndex];
						}
						if (g_ppWipeRandomStrips)
							delete g_ppWipeRandomStrips;
					}
					
					//void InitBurningPalette()
					//{
					//	if (!g_sBurningPalette)
					//		g_sBurningPalette = new Ipp32s[768];

					//	int r,g,b;
					//	r = 256+256+255;
					//	g = 256+255;
					//	b = 255;
					//	
					//	for( int i = 255; i > 239; i-- )
					//	{
					//		g_sBurningPalette[i]     = 255;
					//		g_sBurningPalette[i+256] = 255;
					//		g_sBurningPalette[i+512] = 255;
					//	}
					//	
					//	for(int i = 239; i > 15; i--)
					//	{
					//		g_sBurningPalette[i]     = (r > 255) ? 255 : r;
					//		g_sBurningPalette[i+256] = (g > 255) ? 255 : g;
					//		g_sBurningPalette[i+512] = (b > 255) ? 255 : b;
					//		r = (r > 3) ? (r - 4) : 0;
					//		g = (g > 3) ? (g - 4) : 0;
					//		b = (b > 3) ? (b - 4) : 0;
					//	}
					//	for(int i = 15; i >= 0; i--)
					//	{
					//		g_sBurningPalette[i]     = 0;
					//		g_sBurningPalette[i+256] = 0;
					//		g_sBurningPalette[i+512] = 0;
					//	}
					//}
					BOOL GenerateBurningMask( int nWidth, int nHeight )
					{
						if ((nWidth == g_nBurningMaskWidth) && (nHeight == g_nBurningMaskHeight) && (g_pBurningMask))
							return TRUE;

						if (g_pBurningMask)
							delete []g_pBurningMask;

						int nSize = nWidth * nHeight;
						if( nSize <= 0 )
							return FALSE;

						CRandom rnd( m_dwBaseRnd );
						
						g_nBurningMaskWidth  = nWidth;
						g_nBurningMaskHeight = nHeight;

						g_pBurningMask = new BYTE[nSize];
						if( !g_pBurningMask )
							return FALSE;

						ZeroMemory( g_pBurningMask, nSize );

						Ipp16s* pBuffer = new Ipp16s[nSize];
						if( !pBuffer )
						{
							delete [] g_pBurningMask;
							g_pBurningMask = NULL;

							return FALSE;
						}

						ZeroMemory( pBuffer, nSize * sizeof(Ipp16s) );

						IppiSize roiSize = {nWidth, nHeight};

						int nH = 512;
						float fR = (float)nWidth;

						for( int k = 2; k <= 5; k++ )
						{
							if( k != 5 )
								nH /= 2;
							else
								nH = 2;

							fR /= 3;
							int nRadius = int(fR);
							int nSquareRadius = int(fR*fR) + 1;
							int nSign = rnd.Rand( 2 ) ? -nH : nH;
							double dFactorRadius = 1.0 / nSquareRadius;
							
							for( int p = 20 * k; p > 0; p-- )
							{
								int nCenterX = rnd.Rand( nWidth );
								int nCenterY = rnd.Rand( nHeight );
								
								int nX1 = nCenterX - nRadius;
								int nX2 = nCenterX + nRadius;
								int nY1 = nCenterY - nRadius;
								int nY2 = nCenterY + nRadius;

								if( nX1 < 0 )       nX1 = 0;
								if( nX2 >= nWidth)  nX2 = nWidth - 1;
								if( nY1 < 0 )       nY1 = 0;
								if( nY2 >= nHeight) nY2 = nHeight - 1;

								Ipp16s* pItem = pBuffer + nY1 * nWidth + nX1;
								int nStride = nWidth - (nX2 - nX1 + 1);
								
								int dy = nY1 - nCenterY;
								for( int nRow = nY2 - nY1; nRow >= 0; --nRow, pItem += nStride, ++dy )
								{
									int dx = nX1 - nCenterX;
									for( int nPos = nX2 - nX1; nPos >= 0; --nPos, ++pItem, ++dx )
									{
										int dot = dx*dx + dy*dy;
										if( dot <= nSquareRadius )
										{
											double dTemp = (nSquareRadius - dot) * dFactorRadius;
											*pItem += Ipp16s(nSign * dTemp * dTemp);
										}
									}
								}
							}
						}

						Ipp16s nMin, nMax;
						ippiMin_16s_C1R(pBuffer, 2*nWidth, roiSize, &nMin);
						ippiMax_16s_C1R(pBuffer, 2*nWidth, roiSize, &nMax);
						nMax = max( nMax, nMin + 1 );
						
						float   fFactor = 255.0f/float(nMax - nMin);
						Ipp8u*  pMaskItem = g_pBurningMask;
						Ipp16s* pBufItem = pBuffer;
						for( int nPos = nSize; nPos > 0; --nPos, ++pBufItem, ++pMaskItem )
						{
							*pMaskItem = fFactor * (*pBufItem - nMin);
						}

						delete[] pBuffer;

						return TRUE;
					}
					
					void GenerateRand(int nRandMax)
					{
						if (-1 != g_nRand)
							return;

						CRandom rnd( m_dwBaseRnd );

						g_nRand = rnd.Rand( nRandMax + 1 );
					}

					BOOL RunTetris( CTetrisFigure *pList, int nFiguresCount, float dFrame, BYTE *pBGRAResult, int nWidth, int nHeight, int nStep, int *pnBottomLineFigures)
					{
						if( !pList )
							return FALSE;
						
						if( dFrame * nFiguresCount < 1 )
						{
							return pList[0].Cpy( pBGRAResult, pList[0].m_nXCoorSource, nHeight - pnBottomLineFigures[0] + pList[0].m_nYCoorSource, nWidth, nHeight, 1 );
						}
						
						if( nStep + 2 == nFiguresCount )
						{
							for( int i = 0; i <= nFiguresCount; i++ )
							{
								pList[i].Cpy( pBGRAResult, pList[i].m_nXCoorSource, pList[i].m_nYCoorSource, nWidth, nHeight, (1 - dFrame) * nFiguresCount );
							}

							return TRUE;
						}

						pList[nStep].Cpy( pBGRAResult, pList[nStep].m_nXCoorSource, pList[nStep].m_nYCoorSource, nWidth, nHeight, 1);
							
						nStep += 1;

						if( dFrame * nFiguresCount < nStep + 1 )
						{
							pList[nStep].Cpy( pBGRAResult, pList[nStep].m_nXCoorSource, nHeight - pnBottomLineFigures[nStep] + pList[nStep].m_nYCoorSource, nWidth, nHeight, 1);
						}
						else
						{
							RunTetris( pList, nFiguresCount, dFrame, pBGRAResult, nWidth, nHeight, nStep, pnBottomLineFigures );
						}

						return TRUE;

					}

					BOOL CollapseTetris(CTetrisFigure *pList, int nFiguresCount, float dFrame, BYTE *pBGRAResult, int nWidth, int nHeight, int nStep, int *pnBottomLineFigures)
					{
						if(!pList)
							return FALSE;
						
						if( dFrame * nFiguresCount < 1 )
						{
							for( int i = 0; i <= nFiguresCount; i++ )
							{
								pList[i].Cpy( pBGRAResult, pList[i].m_nXCoorSource, pList[i].m_nYCoorSource, nWidth, nHeight, dFrame * nFiguresCount);
							}

							return TRUE;
						}

						nStep += 1;

						if( nStep > nFiguresCount )
							return TRUE;

						if( dFrame * nFiguresCount < nStep + 1 )
						{
							pList[nStep - 1].Cpy( pBGRAResult, pList[nStep - 1].m_nXCoorSource, pList[nStep - 1].m_nYCoorSource + (pList[nStep - 1].m_nYCoorSource + pList[nStep - 1].m_nFigHeight) * (nStep - dFrame * nFiguresCount), nWidth, nHeight, 1);							
							
							for(int i = nStep; i <= nFiguresCount; i++)
							{
								pList[i].Cpy( pBGRAResult, pList[i].m_nXCoorSource, pList[i].m_nYCoorSource, nWidth, nHeight, 1);
							}
						}
						else
						{
							CollapseTetris( pList, nFiguresCount, dFrame, pBGRAResult, nWidth, nHeight, nStep, pnBottomLineFigures );
						}

						return TRUE;
					}

					void GenerateShot(int nWidth, int nHeight, int nDirection, int nType, BOOL bInverse, BOOL bRapprochement, double dPercent, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
					{
						if(g_pShotSource1 && g_pShotSource2 && nType == g_nShotType && g_nShotWidth == nWidth && g_nShotHeight == nHeight)
							return;

						if(g_pShotSource1)
							delete[] g_pShotSource1;
						if(g_pShotSource2)
							delete[] g_pShotSource2;

					    g_nShotWidth  = nWidth;
					    g_nShotHeight = nHeight;
					    g_nShotType   = nType;

						g_pShotSource1 = new BYTE[nWidth*nHeight*4];
						g_pShotSource2 = new BYTE[nWidth*nHeight*4];

						BOOL bVertical = (1 == nDirection? TRUE : FALSE);

						if( 1 == nDirection )
						{
							double dUnit                  = nWidth / 35.;
							const double dWidthPerf       = 2.8 * dUnit;
							const double dHeightPerf      = 1.98 * dUnit;
							const double dRadiusPerf      = 0.5 * dUnit;
							const double dStepPerf        = (4.75 * dUnit < 10? 10 : 4.75 * dUnit);
							const double dBorderWidth     = 1.01 * dUnit;
							const double dMidLineWidth    = 0.4 * dUnit;
							const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;
							
							double dStartY1 = 0;
							double dStartY2 = 0;

							double dAdditional = nHeight;
							if ( bRapprochement )
								dAdditional += -nHeight * dPercent / 2;

							if ( !bInverse )
								dStartY2 = dAdditional;
							else
								dStartY1 = dAdditional;

							const double dFullBorderWidth = dBorderWidth + dWidthPerf + dMidLineWidth;

							DrawFilmsBorder(g_pShotSource1, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, dFullBorderWidth, 0.1 * dFullBorderWidth, bVertical);
							DrawFilmsBorder(g_pShotSource2, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, dFullBorderWidth, 0.1 * dFullBorderWidth, bVertical);

							for ( double dCurY = - 2 * nHeight - dStartY1; dCurY < nHeight + dStepPerf; dCurY += dStepPerf)
							{
								DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, dBorderWidth, dCurY,                       dWidthPerf, dHeightPerf, dRadiusPerf);
								DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, nWidth - dBorderWidth - dWidthPerf, dCurY, dWidthPerf, dHeightPerf, dRadiusPerf);
							}
							for ( double dCurY = - 2 * nHeight - dStartY2; dCurY < nHeight + dStepPerf; dCurY += dStepPerf)
							{
								DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, dBorderWidth, dCurY,                       dWidthPerf, dHeightPerf, dRadiusPerf);
								DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, nWidth - dBorderWidth - dWidthPerf, dCurY, dWidthPerf, dHeightPerf, dRadiusPerf);
							}
							
						}
						else
						{
							double dUnit                  = nHeight / 35.;
							const double dWidthPerf       = 2.8 * dUnit;
							const double dHeightPerf      = 1.98 * dUnit;
							const double dRadiusPerf      = 0.5 * dUnit;
							const double dStepPerf        = (4.75 * dUnit < 10? 10 : 4.75 * dUnit);
							const double dBorderWidth     = 1.01 * dUnit;
							const double dMidLineWidth    = 0.4 * dUnit;
							const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;

							double dStartX1 = 0;
							double dStartX2 = 0;

							double dAdditional = nWidth;
							if ( bRapprochement )
								dAdditional += -nWidth * dPercent / 2;

							if ( !bInverse )
								dStartX2 = dAdditional;
							else
								dStartX1 = dAdditional;

							const double dFullBorderWidth = dBorderWidth + dWidthPerf + dMidLineWidth;

							DrawFilmsBorder(g_pShotSource1, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, 0.1 * dFullBorderWidth, dFullBorderWidth, bVertical);
							DrawFilmsBorder(g_pShotSource2, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, 0.1 * dFullBorderWidth, dFullBorderWidth, bVertical);

							for ( double dCurX = - 2*nWidth - dStartX1; dCurX < nWidth + dStepPerf; dCurX += dStepPerf)
							{
								DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, dCurX, dBorderWidth,                        dHeightPerf, dWidthPerf, dRadiusPerf);
								DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, dCurX, nHeight - dBorderWidth - dWidthPerf, dHeightPerf, dWidthPerf, dRadiusPerf);
							}
							for ( double dCurX = - 2*nWidth - dStartX2; dCurX < nWidth + dStepPerf; dCurX += dStepPerf)
							{
								DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, dCurX, dBorderWidth,                        dHeightPerf, dWidthPerf, dRadiusPerf);
								DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, dCurX, nHeight - dBorderWidth - dWidthPerf, dHeightPerf, dWidthPerf, dRadiusPerf);
							}
						}
						return;

						//if( 1 == nDirection)
						//{

						//	const double dUnit            = nWidth / 35.;
						//	const double dWidthPerf       = 2.8 * dUnit;
						//	const double dHeightPerf      = 1.98 * dUnit;
						//	const double dRadiusPerf      = 0.5 * dUnit;
						//	const double dStepPerf        = 4.75 * dUnit;
						//	const double dBorderWidth     = 1.01 * dUnit;
						//	const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;
						//	
						//	BYTE *pBuffer = g_pShotSource1;

						//	for (int nIndex = 0; nIndex < 2; nIndex++ )
						//	{
						//		if ( 0 == nIndex )
						//			pBuffer = g_pShotSource1;
						//		else
						//			pBuffer = g_pShotSource2;

						//		for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
						//		{
						//			int nAdditional = nHeight;
						//			if ( bRapprochement )
						//			{
						//				nAdditional += - nHeight * dPercent / 2;
						//			}
						//			int nYIndexS1 = nYIndex;
						//			int nYIndexS2 = nYIndex + nAdditional;
						//			if(!bInverse)
						//			{
						//				nYIndexS1 += nAdditional;
						//				nYIndexS2 -= nAdditional;
						//			}
						//			int nIndexS;
						//			if ( 0 == nIndex )
						//				nIndexS = nYIndexS1;
						//			else
						//				nIndexS = nYIndexS2;

						//			for(int nXIndex = 0; nXIndex < nWidth; nXIndex++)
						//			{
						//				//постоянные для расчетов

						//				const int nYStepCount = nIndexS % int(dStepPerf); // ордината нашей точки в пределах одного шага(шаг = ширина дырки + расстояние между дырками)
						//				const int nFirstFocus = dBorderWidth + dRadiusFactor; // ордината центра первого закругления
						//				const int nSecondFocus = dBorderWidth + dWidthPerf - dRadiusFactor; // ордината центра второго закругления
						//				const int nFullBorderWidth = dBorderWidth + dWidthPerf; // суммарная ширина края (ширина края + ширина дырок)
						//				const double dSqrRadiusPerf = dRadiusPerf * dRadiusPerf;
						//				const double dSqrDecRadiusPerf = (dRadiusPerf - 1) * (dRadiusPerf - 1);
						//				const double dSqrIncRadiusPerf = (dRadiusPerf + 1) * (dRadiusPerf + 1);


						//				if( nXIndex <= dBorderWidth || nXIndex > nWidth - dBorderWidth || 
						//					((( nXIndex > dBorderWidth && nXIndex <= nFullBorderWidth) || ( nXIndex >= nWidth - nFullBorderWidth && nXIndex < nWidth - dBorderWidth)) && nYStepCount > int(dHeightPerf) ) ||
						//					/*скругление углов*/ /*слева*/
						//					(nXIndex <= nFirstFocus                                                   && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - nFirstFocus)  * (nXIndex - nFirstFocus)  + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
						//					(nXIndex <= nFirstFocus                                                   && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - nFirstFocus)  * (nXIndex - nFirstFocus)  + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
						//					(nXIndex >= nSecondFocus && nXIndex <= nFullBorderWidth                   && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - nSecondFocus) * (nXIndex - nSecondFocus) + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
						//					(nXIndex >= nSecondFocus && nXIndex <= nFullBorderWidth                   && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - nSecondFocus) * (nXIndex - nSecondFocus) + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
						//					/*справа*/
						//					(nXIndex <= nWidth - nSecondFocus && nXIndex >= nWidth - nFullBorderWidth && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - (nWidth - nSecondFocus)) * (nXIndex - (nWidth - nSecondFocus)) + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
						//					(nXIndex <= nWidth - nSecondFocus && nXIndex >= nWidth - nFullBorderWidth && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - (nWidth - nSecondFocus)) * (nXIndex - (nWidth - nSecondFocus)) + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
						//					(nXIndex >= nWidth - nFirstFocus                                          && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - (nWidth - nFirstFocus))  * (nXIndex - (nWidth - nFirstFocus))  + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
						//					(nXIndex >= nWidth - nFirstFocus                                          && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - (nWidth - nFirstFocus))  * (nXIndex - (nWidth - nFirstFocus))  + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf))
						//				{
						//					for(int i = 0; i < 3; i++)
						//					{
						//						if(nXIndex <= nFullBorderWidth)
						//						{
						//							*pBuffer = nFilmColorBorder * (1 - double(nXIndex) / double (nFullBorderWidth) ) + nFilmColorMid * (double(nXIndex) / double (nFullBorderWidth));
						//						}
						//						else
						//						{
						//							*pBuffer = nFilmColorBorder * double(nXIndex - (nWidth - nFullBorderWidth))/double(nFullBorderWidth) + nFilmColorMid * (1 - double(nXIndex - (nWidth - nFullBorderWidth))/double(nFullBorderWidth));
						//						}
						//						pBuffer++;
						//					}
						//					*pBuffer = 255;
						//					pBuffer++;
						//				}
						//				else if( nXIndex > nFullBorderWidth && nXIndex < nWidth - nFullBorderWidth)
						//				{
						//					for(int i = 0; i < 3; i++)
						//					{
						//						*pBuffer = nFilmColorMid; 
						//						pBuffer++;
						//					}
						//					*pBuffer = 255;
						//					pBuffer++;
						//				}
						//				else
						//				{
						//					const double dLeftFirstSqrRad = ( nXIndex - nFirstFocus) * ( nXIndex - nFirstFocus) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
						//					const double dLeftSecondSqrRad = ( nXIndex - nSecondFocus) * ( nXIndex - nSecondFocus) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
						//					const double dRightFirstSqrRad = ( nXIndex - ( nWidth - nSecondFocus)) * ( nXIndex - ( nWidth - nSecondFocus)) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
						//					const double dRightSecondSqrRad = ( nXIndex - ( nWidth - nFirstFocus)) * ( nXIndex - ( nWidth - nFirstFocus)) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
						//					
						//					if	//слева
						//						( nXIndex >= dBorderWidth &&
						//							(( nYStepCount == 0  && nXIndex <= nSecondFocus && nXIndex >= nFirstFocus) 
						//							 ||
						//							( dLeftFirstSqrRad > dSqrDecRadiusPerf && dLeftFirstSqrRad < dSqrIncRadiusPerf 
						//							  && nXIndex <= nFirstFocus  && nYStepCount <= dRadiusFactor) 
						//							 ||
						//							( dLeftSecondSqrRad > dSqrDecRadiusPerf && dLeftSecondSqrRad < dSqrIncRadiusPerf 
						//							  && nXIndex >= nSecondFocus && nXIndex <= nFullBorderWidth  && nYStepCount <= dRadiusFactor) 
						//							 ||
						//							  (nXIndex > nFullBorderWidth - 1 && nXIndex < nFullBorderWidth + 1 ))											   ||
						//						//справа
						//						  nXIndex <= nWidth - dBorderWidth &&
						//							(( nYStepCount == 0  && nXIndex >= nWidth - nSecondFocus && nXIndex <= nWidth - nFirstFocus ) 
						//							 ||
						//							( dRightFirstSqrRad > dSqrDecRadiusPerf && dRightFirstSqrRad < dSqrIncRadiusPerf 
						//							  && nXIndex >= nWidth - nFullBorderWidth && nXIndex <= nWidth - nSecondFocus && nYStepCount <= dRadiusFactor) 
						//							 ||
						//							( dRightSecondSqrRad > dSqrDecRadiusPerf && dRightSecondSqrRad < dSqrIncRadiusPerf 
						//							 &&  nXIndex >= nWidth - nFirstFocus && nYStepCount <= dRadiusFactor) 
						//							 ||
						//							  (nXIndex > nWidth - dBorderWidth - 1 && nXIndex < nWidth - dBorderWidth + 1 ))
						//						)
						//					{
						//						for(int i = 0; i < 3; i++)
						//						{
						//							*pBuffer = nBlendColor;
						//							*pBuffer++;
						//						}
						//						*pBuffer = 255;
						//						pBuffer++;
						//					}
						//					else
						//					{
						//						for(int i = 0; i < 3; i++)
						//						{
						//							*pBuffer = nBkColor;
						//							*pBuffer++;
						//						}
						//						*pBuffer = 255;
						//						pBuffer++;
						//					}
						//				}
						//			}
						//		}
						//	}
						//}
						//else
						//{

						//	const double dUnit            = nHeight / 35.;
						//	const double dWidthPerf       = 2.8 * dUnit;
						//	const double dHeightPerf      = 1.98 * dUnit;
						//	const double dRadiusPerf      = 0.5 * dUnit;
						//	const double dStepPerf        = 4.75 * dUnit;
						//	const double dBorderWidth     = 1. * dUnit;
						//	const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;

						//	BYTE *pBuffer = g_pShotSource1;

						//	for (int nIndex = 0; nIndex < 2; nIndex++ )
						//	{
						//		if ( 0 == nIndex )
						//			pBuffer = g_pShotSource1;
						//		else
						//			pBuffer = g_pShotSource2;

						//		for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
						//		{
						//			for(int nXIndex = 0; nXIndex < nWidth; nXIndex++)
						//			{
						//				int nAdditional = nWidth;
						//				if ( bRapprochement )
						//				{
						//					nAdditional -= nWidth * dPercent / 2;
						//				}
						//				int nXIndexS2 = nXIndex;
						//				int nXIndexS1 = nXIndex + nAdditional;
						//				if(!bInverse)
						//				{
						//					nXIndexS2 += nAdditional;
						//					nXIndexS1 -= nAdditional;
						//				}
						//				int nIndexS = 0;
						//				if ( 0 == nIndex)
						//					nIndexS = nXIndexS1;
						//				else
						//					nIndexS = nXIndexS2;


						//				//постоянные для расчетов

						//				const int nXStepCount = nIndexS % int(dStepPerf); // ордината нашей точки в пределах одного шага(шаг = ширина дырки + расстояние между дырками)
						//				const int nFirstFocus = dBorderWidth + dRadiusFactor; // ордината центра первого закругления
						//				const int nSecondFocus = dBorderWidth + dWidthPerf - dRadiusFactor; // ордината центра второго закругления
						//				const int nFullBorderWidth = dBorderWidth + dWidthPerf; // суммарная ширина края (ширина края + ширина дырок)
						//				const double dSqrRadiusPerf = dRadiusPerf * dRadiusPerf;
						//				const double dSqrDecRadiusPerf = (dRadiusPerf - 1) * (dRadiusPerf - 1);
						//				const double dSqrIncRadiusPerf = (dRadiusPerf + 1) * (dRadiusPerf + 1);


						//				if( nYIndex <= dBorderWidth || nYIndex >= nHeight - dBorderWidth || 
						//					((( nYIndex > dBorderWidth && nYIndex <= nFullBorderWidth) || ( nYIndex >= nHeight - nFullBorderWidth && nYIndex < nHeight - dBorderWidth)) && nXStepCount > int(dHeightPerf) ) ||
						//					/*скругление углов*/ /*сверху*/
						//					(nYIndex <= nFirstFocus                                                              && nXStepCount <= dRadiusFactor               &&  (nYIndex - nFirstFocus)  * (nYIndex - nFirstFocus)  + (nXStepCount - dRadiusFactor)               * (nXStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
						//					(nYIndex <= nFirstFocus                                                              && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - nFirstFocus)  * (nYIndex - nFirstFocus)  + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
						//					(nYIndex >= nSecondFocus && nYIndex <= dBorderWidth + dWidthPerf                     && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - nSecondFocus) * (nYIndex - nSecondFocus) + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
						//					(nYIndex >= nSecondFocus && nYIndex <= dBorderWidth + dWidthPerf                     && nXStepCount <= dRadiusFactor               &&  (nYIndex - nSecondFocus) * (nYIndex - nSecondFocus) + (nXStepCount - dRadiusFactor)               * (nXStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
						//					/*снизу*/
						//					(nYIndex <= nHeight - nSecondFocus && nYIndex >= nHeight - dBorderWidth - dWidthPerf && nXStepCount <= dRadiusFactor               &&  (nYIndex - (nHeight - nSecondFocus)) * (nYIndex - (nHeight - nSecondFocus)) + (nXStepCount - dRadiusFactor)               * (nXStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
						//					(nYIndex <= nHeight - nSecondFocus && nYIndex >= nHeight - dBorderWidth - dWidthPerf && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - (nHeight - nSecondFocus)) * (nYIndex - (nHeight - nSecondFocus)) + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
						//					(nYIndex >= nHeight - nFirstFocus                                                    && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - (nHeight - nFirstFocus))  * (nYIndex - (nHeight - nFirstFocus))  + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf ) ||
						//					(nYIndex >= nHeight - nFirstFocus                                                    && nXStepCount <= dRadiusFactor               &&  (nYIndex - (nHeight - nFirstFocus))  * (nYIndex - (nHeight - nFirstFocus))  + (nXStepCount - dRadiusFactor)               * (nXStepCount- dRadiusFactor)                > dSqrRadiusPerf))
						//				{
						//					for(int i = 0; i < 3; i++)
						//					{
						//						if( nYIndex <= nFullBorderWidth)
						//						{
						//							*pBuffer = nFilmColorBorder * (1 - nYIndex / (dBorderWidth + dWidthPerf)) + nFilmColorMid * (nYIndex / (dBorderWidth + dWidthPerf));
						//							//double dKoef = double(nYIndex) / double(nFullBorderWidth);
						//							//*pBuffer = nFilmColorBorder * (1 - dKoef ) + nFilmColorMid * dKoef;
						//						}
						//						else
						//						{
						//							*pBuffer = nFilmColorBorder * (nYIndex - (nHeight - (dBorderWidth + dWidthPerf))) / (dBorderWidth + dWidthPerf) + nFilmColorMid * (1 - (nYIndex - (nHeight - (dBorderWidth + dWidthPerf)))/ (dBorderWidth + dWidthPerf));
						//							//double dKoef = double(nYIndex - (nHeight - nFullBorderWidth)) / double(nFullBorderWidth);
						//							//*pBuffer = nFilmColorBorder * dKoef + nFilmColorMid * (1 - dKoef);
						//						}
						//						pBuffer++;
						//					}
						//					*pBuffer = 255;
						//					pBuffer++;
						//				}
						//				else if(nYIndex > nFullBorderWidth && nYIndex < nHeight - nFullBorderWidth)
						//				{
						//					for(int i = 0; i < 3; i++)
						//					{
						//						*pBuffer = nFilmColorMid; 
						//						pBuffer++;
						//					}
						//					*pBuffer = 255;
						//					pBuffer++;
						//				}
						//				else
						//				{
						//					const double dTopFirstSqrRad =     ( nYIndex - nFirstFocus) * ( nYIndex - nFirstFocus) + (nXStepCount - dHeightPerf + dRadiusFactor ) * ( nXStepCount - dHeightPerf + dRadiusFactor);
						//					const double dTopSecondSqrRad =    ( nYIndex - nFirstFocus) * ( nYIndex - nFirstFocus) + (nXStepCount - dRadiusFactor)                * ( nXStepCount - dRadiusFactor);
						//					const double dBottomFirstSqrRad =  ( nYIndex - ( nHeight - nSecondFocus)) * ( nYIndex - ( nHeight - nSecondFocus)) + (nXStepCount - dHeightPerf + dRadiusFactor) * ( nXStepCount - dHeightPerf + dRadiusFactor);
						//					const double dBottomSecondSqrRad = ( nYIndex - ( nHeight - nSecondFocus)) * ( nYIndex - ( nHeight - nSecondFocus)) + (nXStepCount - dRadiusFactor)               * ( nXStepCount - dRadiusFactor);

						//					if( 
						//						nYIndex >= dBorderWidth &&nYIndex <= nHeight - dBorderWidth &&										   
						//					    ((( nXStepCount > dHeightPerf - 1 && nXStepCount < dHeightPerf + 1  && nYIndex > nFirstFocus - 10/*заглушка*/) 
						//					    ||
						//						( dTopFirstSqrRad > dSqrDecRadiusPerf && dTopFirstSqrRad < dSqrIncRadiusPerf 
						//						  && nYIndex < nFirstFocus && nXStepCount >= dHeightPerf - dRadiusFactor) 
						//						||
						//						( dTopSecondSqrRad > dSqrDecRadiusPerf && dTopSecondSqrRad < dSqrIncRadiusPerf 
						//						  && nYIndex < nFirstFocus && nXStepCount <= dRadiusFactor) 
						//						||
						//						  (nYIndex < dBorderWidth + 1 && nYIndex > dBorderWidth - 1) ) 
						//						||
						//						(( nXStepCount > dHeightPerf - 1 && nXStepCount < dHeightPerf + 1  && nYIndex >= nHeight - nSecondFocus ) 
						//						||
						//						( dBottomFirstSqrRad > dSqrDecRadiusPerf && dBottomFirstSqrRad < dSqrIncRadiusPerf 
						//						  && nYIndex >= nHeight - nFullBorderWidth && nYIndex <= nHeight - nSecondFocus && nXStepCount >= dHeightPerf - dRadiusFactor) ||
						//						( dBottomSecondSqrRad > dSqrDecRadiusPerf && dBottomSecondSqrRad < dSqrIncRadiusPerf 
						//						  && nYIndex >= nHeight - nFullBorderWidth && nYIndex <= nHeight - nSecondFocus && nXStepCount <= dRadiusFactor) 
						//						||
						//						  (nYIndex < nHeight - nFullBorderWidth + 1 && nYIndex > nHeight - nFullBorderWidth - 1 ) )))
						//					{
						//						for(int i = 0; i < 3; i++)
						//						{
						//							*pBuffer = nBlendColor;
						//							pBuffer++;
						//						}
						//					}
						//					else
						//					{
						//						for(int i = 0; i < 3; i++)
						//						{
						//							*pBuffer = nBkColor;
						//							pBuffer++;
						//						}
						//					}
						//					*pBuffer = 255;
						//					pBuffer++;
						//				}
						//			}
						//		}
						//	}
						//}
						//return;
					}




					void GenerateWipeStrips(int nCount)
					{
						if ( g_ppWipeRandomStrips && g_nWipeStripsCount == nCount )
							return;
						
						if ( g_ppWipeRandomStrips )
						{
							for(int nIndex = 0; nIndex < g_nWipeStripsCount; nIndex++ )
							{
								if(g_ppWipeRandomStrips[nIndex])
									delete[] g_ppWipeRandomStrips[nIndex];
							}
							if (g_ppWipeRandomStrips)
								delete[] g_ppWipeRandomStrips;
						}

						g_nWipeStripsCount = nCount;

						CRandom rnd( m_dwBaseRnd );
						
						g_ppWipeRandomStrips = new int*[nCount];
						for( int nIndex = 0; nIndex < nCount; nIndex++ )
						{
							g_ppWipeRandomStrips[nIndex] = new int[256];
						}
						for(int nCountIndex = 0; nCountIndex < nCount; nCountIndex++ )
						{
							for(int nTime = 0; nTime < 256; nTime++)
							{
								g_ppWipeRandomStrips[nCountIndex][nTime] = rnd.Rand( 100 );
							}
						}
						return;
					}





				public:
					
					int	g_nBurningMaskWidth;
					int	g_nBurningMaskHeight;
					
					//Ipp32s* g_sBurningPalette;
					Ipp8u* g_pBurningMask;

					int g_nRand;

					BYTE* g_pShotSource1;
					BYTE* g_pShotSource2;
					int g_nShotWidth;
					int g_nShotHeight;
					int g_nShotType;

					int** g_ppWipeRandomStrips;
					int g_nWipeStripsCount;
					
				};


				class CMaskGraduate
				{
				public:
					DWORD m_dwBaseRnd;

				public:

					CMaskGraduate()
					{
						m_nMaskWidth = -1;
						m_nMaskHeight = -1;
						m_nMaskType = -1;
						g_nTilesX = -1;
						g_nTilesY = -1;

						m_pMask = 0;

						g_nSize = 0;
						g_pGraduation = NULL;

						m_dwBaseRnd = ::rand();
					}

					~CMaskGraduate()
					{
						if ( m_pMask )
							delete []m_pMask;

						if ( g_pGraduation )
							delete []g_pGraduation;
					}

					void GenerateMask(int nWidth, int nHeight, int nType)
					{
						if ((nWidth == m_nMaskWidth) && (nHeight == m_nMaskHeight) && (m_pMask) && (nType == m_nMaskType))
							return ;

						if (m_pMask)
							delete []m_pMask;

						int nSize = nWidth * nHeight;
						if( !nSize )
							return;

						m_nMaskType = nType;
						m_nMaskWidth  = nWidth;
						m_nMaskHeight = nHeight;

						if ( Constants::c_nGraduateSideLeftToRight <= nType && Constants::c_nGraduateCornerRightTop >= nType)
						{
							g_nTilesX = 5;
							g_nTilesY = 5;
						}
						else if ( Constants::c_nGraduateClockBottomClockwise <= nType && Constants::c_nGraduateClock3AnglesReverse >= nType )
						{
							g_nTilesX = 2;
							g_nTilesY = 2;
						}
						g_nTileWidth = nWidth / g_nTilesX;
						g_nTileHeight = nHeight / g_nTilesY;

						m_pMask = new Ipp16u[nSize];
						if( !m_pMask )
							return;
						
						::memset( m_pMask, 255, nSize * sizeof(Ipp16u) );
						RandomShuffle(g_nTilesX * g_nTilesY);

						if ( Constants::c_nGraduateSideLeftToRight <= nType && Constants::c_nGraduateCornerRightTop >= nType)
							WipeSide();
						else if ( Constants::c_nGraduateClockBottomClockwise <= nType && Constants::c_nGraduateClock3AnglesReverse >= nType )
							Clock();
						return;
					}


				protected:

					void WipeSide()
					{
						CBuffer<double> oTable( m_nMaskWidth + m_nMaskHeight );
						if( oTable.IsEmpty() )
						{
							::memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight * sizeof(Ipp16u) );
							return;
						}

						double dFactorX = 1.0 / g_nTileWidth;
						double dFactorY = 1.0 / g_nTileHeight;

						double* pTable = oTable.GetPtr();
						
						int nTileX = -1;
						for( int i = 0; i < m_nMaskWidth; ++i, ++pTable )
						{
							if( Constants::c_nGraduateSideTopToBottom == m_nMaskType ||
								Constants::c_nGraduateSideBottomToTop == m_nMaskType )
							{
								*pTable = 0;
							}
							else
							{
								if( ++nTileX >= g_nTileWidth )
									nTileX = 0;

								*pTable = nTileX * dFactorX;
							}
						}

						int nTileY = -1;
						for( int i = 0; i < m_nMaskHeight; ++i, ++pTable )
						{
							if( Constants::c_nGraduateSideLeftToRight == m_nMaskType ||
								Constants::c_nGraduateSideRightToLeft == m_nMaskType )
							{
								*pTable = 0;
							}
							else
							{
								if( ++nTileY >= g_nTileHeight )
									nTileY = 0;

								*pTable = nTileY * dFactorY;
							}
						}

						if( Constants::c_nGraduateSideRightToLeft   == m_nMaskType ||
							Constants::c_nGraduateCornerRightTop    == m_nMaskType ||
							Constants::c_nGraduateCornerRightBottom == m_nMaskType )
						{
							double* pTableX = oTable.GetPtr();

							for( int i = 0; i < m_nMaskWidth; ++i, ++pTableX )
							{
								*pTableX = 1 - *pTableX;
							}
						}

						if( Constants::c_nGraduateSideTopToBottom == m_nMaskType ||
							Constants::c_nGraduateCornerLeftTop   == m_nMaskType ||
							Constants::c_nGraduateCornerRightTop  == m_nMaskType )
						{
							double* pTableY = oTable.GetPtr() + m_nMaskWidth;

							for( int i = 0; i < m_nMaskHeight; ++i, ++pTableY )
							{
								*pTableY = 1 - *pTableY;
							}
						}

						double dFactor = 65535.0 / (g_nTilesX * g_nTilesY);

						Ipp16u* pBuffer = m_pMask;

						double* pTableY = oTable.GetPtr() + m_nMaskWidth;
						for( int nY = 0; nY < m_nMaskHeight; ++nY, ++pTableY )
						{
							int* pStensil = g_pGraduation + nY / g_nTileHeight * g_nTilesX;

							double* pTableX = oTable.GetPtr();
							for( int nX = 0; nX < m_nMaskWidth; ++nX, ++pTableX )
							{								
								int nCurTileX = nX / g_nTileWidth;

								*pBuffer++ = Ipp16u((max(pTableX[0], pTableY[0]) + pStensil[nCurTileX]) * dFactor);
							}
						}
					}

					void Clock()
					{
						Ipp16u* pBuffer = m_pMask;

						int nAnglesCount = 1;
						double dStartAngle = 0;
						
						if( Constants::c_nGraduateClockLeftOpposite  == m_nMaskType || 
							Constants::c_nGraduateClockLeftClockwise == m_nMaskType || 
							Constants::c_nGraduateClockLeftReverse   == m_nMaskType )
						{
							dStartAngle = 180 * IPP_PI180;
						}
						else 
						if( Constants::c_nGraduateClockTopOpposite  == m_nMaskType || 
							Constants::c_nGraduateClockTopClockwise == m_nMaskType || 
							Constants::c_nGraduateClockTopReverse   == m_nMaskType )
						{
							dStartAngle = 270 * IPP_PI180;
						}
						else 
						if( Constants::c_nGraduateClockBottomOpposite  == m_nMaskType || 
							Constants::c_nGraduateClockBottomClockwise == m_nMaskType || 
							Constants::c_nGraduateClockBottomReverse   == m_nMaskType )
						{
							dStartAngle = 90 * IPP_PI180;
						}
						else
						if( Constants::c_nGraduateClock2AnglesClockwise == m_nMaskType || 
							Constants::c_nGraduateClock2AnglesReverse   == m_nMaskType )
						{
							dStartAngle = 270 * IPP_PI180;
							nAnglesCount = 2;
						}
						else 
						if( Constants::c_nGraduateClock3AnglesClockwise == m_nMaskType || 
							Constants::c_nGraduateClock3AnglesReverse   == m_nMaskType )
						{
							dStartAngle = 270 * IPP_PI180;
							nAnglesCount = 3;
						}
						
						double dFactor = 1;
						int nOffset = 0;
						BOOL bReverse = FALSE;
						
						if( Constants::c_nGraduateClockRightOpposite  == m_nMaskType || 
							Constants::c_nGraduateClockLeftOpposite   == m_nMaskType ||
							Constants::c_nGraduateClockTopOpposite    == m_nMaskType || 
							Constants::c_nGraduateClockBottomOpposite == m_nMaskType) 
						{
							bReverse = TRUE;
							nOffset = 0;
							dFactor = 1.0 / IPP_PI;
						}
						else 
						if( Constants::c_nGraduateClockRightClockwise   == m_nMaskType || 
							Constants::c_nGraduateClockLeftClockwise    == m_nMaskType ||
							Constants::c_nGraduateClockTopClockwise     == m_nMaskType || 
							Constants::c_nGraduateClockBottomClockwise  == m_nMaskType ||
							Constants::c_nGraduateClock2AnglesClockwise == m_nMaskType || 
							Constants::c_nGraduateClock3AnglesClockwise == m_nMaskType ) 
						{
							nOffset = 1;
							dFactor = -1.0 / IPP_2PI;
						}
						else 
						if( Constants::c_nGraduateClockRightReverse   == m_nMaskType || 
							Constants::c_nGraduateClockLeftReverse    == m_nMaskType ||
							Constants::c_nGraduateClockTopReverse     == m_nMaskType || 
							Constants::c_nGraduateClockBottomReverse  == m_nMaskType ||
							Constants::c_nGraduateClock2AnglesReverse == m_nMaskType || 
							Constants::c_nGraduateClock3AnglesReverse == m_nMaskType )
						{
							dFactor = 1.0 / IPP_2PI;
						}

						double dValueFactor = 65535.0 / (g_nTilesX * g_nTilesY);

						struct TTile
						{
							int tile;
							float x;
						};

						CBuffer<TTile> table( m_nMaskWidth );
						if( table.IsEmpty() )
						{
							::memset( pBuffer, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						for( int i = 0; i < m_nMaskWidth; ++i )
						{
							table[i].tile = i / g_nTileWidth;
							table[i].x = i - (table[i].tile + 0.5) * g_nTileWidth;
						}

						for( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							int nCurTileY = nY / g_nTileHeight;
							double dY = nY - (nCurTileY + 0.5) * g_nTileHeight;
							
							nCurTileY *= g_nTilesX;

							TTile* pTile = table.GetPtr();

							for( int nX = 0; nX < m_nMaskWidth; ++nX, ++pTile )
							{								
								double dAngle = atan2( dY, double(pTile->x) );
								if( dAngle < 0 )
									dAngle += IPP_2PI;
								
								dAngle += dStartAngle;
								dAngle *= nAnglesCount;
								dAngle -= int(dAngle * (1.0 / IPP_2PI)) * IPP_2PI;

								double dCur = nOffset + dAngle * dFactor;
								if( bReverse )
								{
									if( dAngle > IPP_PI )
										dCur = 2 - dCur;
								}

								*pBuffer++ = Ipp16u((dCur + g_pGraduation[pTile->tile + nCurTileY]) * dValueFactor);
							}
						}
					}


					BOOL RandomShuffle(int nSize)
					{
						
						if (nSize < 1)
							return FALSE;
						if (nSize == g_nSize)
							return TRUE;

						g_nSize = nSize;
						g_pGraduation = new int[g_nSize];

						if (NULL == g_pGraduation)
							return FALSE;

						for (int index = 0; index < g_nSize; ++index)
							g_pGraduation[index] = index;

						//srand( 1 );
						CRandom rnd( m_dwBaseRnd );

						for (int index = 0; index < g_nSize; ++index)
						{
							int nSwapIndex = rnd.Rand( g_nSize );
							int nSwapValue = g_pGraduation[nSwapIndex];

							g_pGraduation[nSwapIndex] = g_pGraduation[index];
							g_pGraduation[index] = nSwapValue;
						}

						return TRUE;
					}


				public:

					Ipp16u* m_pMask;

				protected:

					int	 m_nMaskWidth;
					int	 m_nMaskHeight;

					int  g_nSize;
					int* g_pGraduation; 
					int  m_nMaskType;
					int  g_nTilesX;
					int  g_nTilesY;
					int  g_nTileWidth;
					int  g_nTileHeight;
				};


				class CMask
				{
				public:
					DWORD m_dwBaseRnd;

					CMask()
					{
						m_nMaskWidth = -1;
						m_nMaskHeight = -1;
						m_nMaskType = -1;

						m_pMask = 0;

						m_dwBaseRnd = ::rand();
					}
					~CMask()
					{
						if ( m_pMask )
							delete []m_pMask;
					}

					void GenerateMask(int nWidth, int nHeight, int nType)
					{
						if ((nWidth == m_nMaskWidth) && (nHeight == m_nMaskHeight) && (m_pMask) && (nType == m_nMaskType))
							return ;

						if (m_pMask)
							delete []m_pMask;

						int nSize = nWidth * nHeight;
						if( !nSize )
							return;

						m_nMaskType = nType;
						m_nMaskWidth  = nWidth;
						m_nMaskHeight = nHeight;

						m_pMask = new BYTE[nSize];
						if( !m_pMask )
							return;

						
						ZeroMemory( m_pMask, nSize );

						if ( Constants::c_nBowTileHorizontal == nType || Constants::c_nBowTileSmoothHorizontal == nType ||
							 Constants::c_nBowTileVertical == nType   || Constants::c_nBowTileSmoothVertical == nType)
							BowTile();
						else 
						if ( Constants::c_nDiagonalBoxOut == nType || Constants::c_nDiagonalSmoothBoxOut == nType)
							DiagonalBoxOut();
						else 
						if ( Constants::c_nDiagonalCrossOut == nType || Constants::c_nDiagonalSmoothCrossOut == nType)
							DiagonalCrossOut();
						else 
						if ( Constants::c_nDiagonalDownRight == nType || Constants::c_nDiagonalSmoothDownRight == nType ||
						  	 Constants::c_nDiagonalDownLeft == nType || Constants::c_nDiagonalSmoothDownLeft == nType ||
							 Constants::c_nDiagonalUpRight == nType || Constants::c_nDiagonalSmoothUpRight == nType ||
							 Constants::c_nDiagonalUpLeft == nType || Constants::c_nDiagonalSmoothUpLeft == nType)
							Diagonal();
						else
						if ( Constants::c_nFunIn == nType || Constants::c_nFunSmoothIn == nType)
							FunIn();
						else 
						if ( Constants::c_nFunOut == nType || Constants::c_nFunSmoothOut == nType)
							FunOut();
						else
						if ( Constants::c_nFunUp == nType || Constants::c_nFunSmoothUp == nType)
							FunUp();
						else 
						if ( Constants::c_nFilledVDown == nType || Constants::c_nFilledVSmoothDown == nType ||
							 Constants::c_nFilledVRight == nType || Constants::c_nFilledVSmoothRight == nType || 
							 Constants::c_nFilledVLeft == nType || Constants::c_nFilledVSmoothLeft == nType ||
							 Constants::c_nFilledVUp == nType || Constants::c_nFilledVSmoothUp == nType )
							FilledV();

						else 
						if ( Constants::c_nInsertDownLeft == nType || Constants::c_nInsertSmoothDownLeft == nType ||
							 Constants::c_nInsertDownRight == nType || Constants::c_nInsertSmoothDownRight == nType ||
							 Constants::c_nInsertUpLeft == nType || Constants::c_nInsertSmoothUpLeft == nType ||
							 Constants::c_nInsertUpRight == nType || Constants::c_nInsertSmoothUpRight == nType )
							Insert();
						else
						if ( Constants::c_nRevealUp == nType || Constants::c_nRevealDown == nType || 
							 Constants::c_nRevealLeft == nType || Constants::c_nRevealRight == nType)
							Reveal();
						else 
						if ( Constants::c_nSplitHorizontal == nType || Constants::c_nSplitSmoothHorizontal == nType ||
							 Constants::c_nSplitVertical == nType || Constants::c_nSplitSmoothVertical == nType )
							Split();
						else 
						if ( Constants::c_nSweepIn == nType || Constants::c_nSweepSmoothIn == nType )
							SweepIn();
						else
						if ( Constants::c_nSweepOut == nType || Constants::c_nSweepSmoothOut == nType )
							SweepOut();
						else
						if ( Constants::c_nSweepUp == nType || Constants::c_nSweepSmoothUp == nType )
							SweepUp();
						else
						if ( Constants::c_nZigZagSmoothVertical == nType || Constants::c_nZigZagVertical == nType )
							ZigZagVer();
						else
						if ( Constants::c_nZigZagSmoothHorizontal == nType || Constants::c_nZigZagHorizontal == nType )
							ZigZagHor();
						else
						if ( Constants::c_nPinwheelCurvedSN5A <= nType  && Constants::c_nPinwheelPropeller12Smooth >= nType )
							PinwheelCurved();
						else if ( Constants::c_nWipeSmoothDiamond == nType || Constants::c_nWipeSmoothDiamondIn == nType)
							Diamonds( 1, 1, nType != Constants::c_nWipeSmoothDiamondIn );
						else if ( Constants::c_nWipeSmoothDiamonds == nType || Constants::c_nWipeDiamonds == nType )
							Diamonds( 5, 5, TRUE );
						else if ( Constants::c_nWipeSmoothCircle == nType || Constants::c_nWipeSmoothCircleIn == nType )
							Circles( 1, nType != Constants::c_nWipeSmoothCircleIn );
						else if ( Constants::c_nWipeSmoothCircles == nType || Constants::c_nWipeCircles == nType)
							Circles( 5, TRUE );
						else if ( Constants::c_nWipeSmoothWideStar   == nType || Constants::c_nWipeSmoothNarrowStar   == nType ||
							      Constants::c_nWipeSmoothWideFlower == nType || Constants::c_nWipeSmoothNarrowFlower == nType)
							StarFlower();
						else if ( Constants::c_nWipeSmoothWideStars   == nType || Constants::c_nWipeSmoothNarrowStars   == nType ||
							      Constants::c_nWipeSmoothWideFlowers == nType || Constants::c_nWipeSmoothNarrowFlowers == nType)
							StarsFlowers();
						else if ( Constants::c_nWipeSmoothWideKeyHole == nType || Constants::c_nWipeSmoothNarrowKeyHole == nType)
							KeyHole();
						else if ( Constants::c_nWipeSmoothWideKeyHoles == nType || Constants::c_nWipeSmoothNarrowKeyHoles == nType)
							KeyHoles();
						else if ( Constants::c_nCircleCornerLeftBottom == nType || Constants::c_nCircleCornerSmoothLeftBottom == nType ||
								  Constants::c_nCircleCornerLeftTop == nType || Constants::c_nCircleCornerSmoothLeftTop == nType ||
								  Constants::c_nCircleCornerRightBottom == nType || Constants::c_nCircleCornerSmoothRightBottom == nType ||
								  Constants::c_nCircleCornerRightTop == nType || Constants::c_nCircleCornerSmoothRightTop == nType )
							CircleCorner();

						else if( Constants::c_nBarsHorizontal == nType)
							BarsHorizontal();
						else if( Constants::c_nBarsVertical == nType)
							BarsVertical();
						else if( Constants::c_nWipeSmoothBoxIn == nType || Constants::c_nWipeSmoothBoxOut == nType)
							Box();
						else if( Constants::c_nEnhancedAlphaNoise == nType )
							Noise();
						else if( Constants::c_nEnhancedShutter == nType )
							Shutter();

						return;
					}


				protected:

					void BowTile()
					{
						CBuffer<double> oTable(m_nMaskWidth + m_nMaskHeight);
						if( oTable.IsEmpty() )
						{
							::memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						double dFactorX = 2.0 / m_nMaskWidth;
						double dFactorY = 2.0 / m_nMaskHeight;
						double dSignX = (Constants::c_nBowTileHorizontal == m_nMaskType || Constants::c_nBowTileSmoothHorizontal == m_nMaskType) ? -128 :  128;
						double dSignY = (Constants::c_nBowTileHorizontal == m_nMaskType || Constants::c_nBowTileSmoothHorizontal == m_nMaskType) ?  128 : -128;

						double* pTableY = oTable.GetPtr();
						for( int nX = 0; nX < m_nMaskWidth; ++nX )
						{
							*pTableY++ = abs(nX * dFactorX - 1) * dSignX;
						}
						
						for( int nY = m_nMaskHeight - 1; nY >= 0; --nY )
						{
							*pTableY++ = abs(nY * dFactorY - 1) * dSignY + 127.5;
						}

						double* pLimit = pTableY - m_nMaskHeight;
						Ipp8u* pBuffer = m_pMask;

						for( pTableY -= 1; pTableY >= pLimit; --pTableY )
						{
							for( double* pTableX = oTable.GetPtr(); pTableX < pLimit; ++pTableX )
							{
								*pBuffer++ = BYTE(pTableY[0] + pTableX[0]);
							}
						}
					}

					void DiagonalBoxOut()
					{
						CBuffer<double> oTable(m_nMaskWidth + m_nMaskHeight);
						if( oTable.IsEmpty() )
						{
							::memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						double dFactorX = 2.0 / m_nMaskWidth;
						double dFactorY = 2.0 / m_nMaskHeight;

						double* pTableY = oTable.GetPtr();
						for( int nX = 0; nX < m_nMaskWidth; ++nX )
						{
							*pTableY++ = abs(nX * dFactorX - 1) * 255;
						}
						
						for( int nY = m_nMaskHeight - 1; nY >= 0; --nY )
						{
							*pTableY++ = (1 - abs(nY * dFactorY - 1)) * 255;
						}

						double* pLimit = pTableY - m_nMaskHeight;
						Ipp8u* pBuffer = m_pMask;

						for( pTableY -= 1; pTableY >= pLimit; --pTableY )
						{
							for( double* pTableX = oTable.GetPtr(); pTableX < pLimit; ++pTableX )
							{
								*pBuffer++ = BYTE(abs(pTableY[0] - pTableX[0]));
							}
						}
					}
					void DiagonalCrossOut()
					{
						CBuffer<double> oTable(m_nMaskWidth + m_nMaskHeight);
						if( oTable.IsEmpty() )
						{
							::memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						double dFactorX = 2.0 / m_nMaskWidth;
						double dFactorY = 2.0 / m_nMaskHeight;

						double* pTableY = oTable.GetPtr();
						for( int nX = 0; nX < m_nMaskWidth; ++nX )
						{
							*pTableY++ = abs(nX * dFactorX - 1) * 255;
						}
						
						for( int nY = m_nMaskHeight - 1; nY >= 0; --nY )
						{
							*pTableY++ = abs(nY * dFactorY - 1) * 255;
						}

						double* pLimit = pTableY - m_nMaskHeight;
						Ipp8u* pBuffer = m_pMask;

						for( pTableY -= 1; pTableY >= pLimit; --pTableY )
						{
							for( double* pTableX = oTable.GetPtr(); pTableX < pLimit; ++pTableX )
							{
								*pBuffer++ = BYTE(abs(pTableX[0] - pTableY[0]));
							}
						}
					}
										
					void Diagonal()
					{
						CBuffer<double> oTable(m_nMaskWidth + m_nMaskHeight);
						if( oTable.IsEmpty() )
						{
							::memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						double dOffset  = 255.0 / (m_nMaskHeight + m_nMaskWidth);
						double dFactorX = dOffset;
						double dFactorY = dOffset;

						if( Constants::c_nDiagonalDownRight       == m_nMaskType || 
							Constants::c_nDiagonalSmoothDownRight == m_nMaskType)
						{
							dOffset  *= m_nMaskHeight;
							dFactorX *= 1; 
							dFactorY *= -1;
						}
						else 
						if( Constants::c_nDiagonalDownLeft       == m_nMaskType || 
							Constants::c_nDiagonalSmoothDownLeft == m_nMaskType)
						{
							dOffset  *= m_nMaskWidth + m_nMaskHeight;
							dFactorX *= -1; 
							dFactorY *= -1;
						}
						else 
						if( Constants::c_nDiagonalUpRight       == m_nMaskType || 
							Constants::c_nDiagonalSmoothUpRight == m_nMaskType)
						{
							dOffset  *= 0;
							dFactorX *= 1; 
							dFactorY *= 1;
						}
						else 
						if( Constants::c_nDiagonalUpLeft       == m_nMaskType || 
							Constants::c_nDiagonalSmoothUpLeft == m_nMaskType)
						{
							dOffset  *= m_nMaskWidth;
							dFactorX *= -1; 
							dFactorY *= 1;
						}

						double* pTableY = oTable.GetPtr();
						for( int nX = 0; nX < m_nMaskWidth; ++nX )
						{
							*pTableY++ = nX * dFactorX;
						}
						
						for( int nY = m_nMaskHeight - 1; nY >= 0; --nY )
						{
							*pTableY++ = nY * dFactorY + dOffset;
						}

						double* pLimit = pTableY - m_nMaskHeight;
						Ipp8u* pBuffer = m_pMask;

						for( pTableY -= 1; pTableY >= pLimit; --pTableY )
						{
							for( double* pTableX = oTable.GetPtr(); pTableX < pLimit; ++pTableX )
							{
								*pBuffer++ = BYTE(pTableY[0] + pTableX[0]);
							}
						}
					}

					void FunIn()
					{
						const double c_dFactor = 255 / IPP_PI;

						CBuffer<double> oTable(m_nMaskWidth + m_nMaskHeight);
						if( oTable.IsEmpty() )
						{
							::memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						double dHalfWidth  = m_nMaskWidth  / 2.0;
						double dHalfHeight = m_nMaskHeight / 2.0;

						double* pTableY = oTable.GetPtr();
						for( int nX = 0; nX < m_nMaskWidth; ++nX )
						{
							*pTableY++ = nX - dHalfWidth;
						}
						
						for( int nY = m_nMaskHeight - 1; nY >= 0; --nY )
						{
							*pTableY++ = nY - dHalfHeight * (nY >= dHalfHeight ? 1.5 : 0.5);
						}

						double* pLimit = pTableY - m_nMaskHeight;
						Ipp8u* pBuffer = m_pMask;
						double* pHalfLimit = pLimit + m_nMaskHeight / 2;

						for( pTableY -= 1; pTableY >= pLimit; --pTableY )
						{
							for( double* pTableX = oTable.GetPtr(); pTableX < pLimit; ++pTableX )
							{
								double dAngle = abs(atan2( pTableX[0], pTableY[0] ));
								
								if( pTableY >= pHalfLimit )
									dAngle = IPP_PI - dAngle;
								
								*pBuffer++ = BYTE(dAngle * c_dFactor);
							}
						}
					}

					void FunOut()
					{
						Ipp8u* pBuffer = m_pMask;

						const double dFactor = 255 / IPP_2PI;

						int nHalfWidth  = m_nMaskWidth / 2;
						int nHalfHeight = m_nMaskHeight / 2;

						double offset1 = m_nMaskWidth * 0.75;
						double offset2 = m_nMaskWidth * 0.25;

						for( int nY = 0; nY < m_nMaskHeight; ++nY )
						{
							double dY = nY - nHalfHeight;			

							for( int nX = 0; nX < m_nMaskWidth; ++nX )
							{
								double dAngle;

								if( nX > nHalfWidth )
								{
									dAngle = IPP_PI - atan2( dY, nX - offset1 );
								}
								else
								{
									dAngle = atan2( dY, nX - offset2 );
									if( dAngle < 0 )
										dAngle += IPP_2PI;
								}
								*pBuffer++ = int(dAngle * dFactor);
							}
						}
					}

					void FunUp()
					{
						Ipp8u* pBuffer = m_pMask;

						const double dFactor = 255 / IPP_PI2;

						double dHalfWidth = m_nMaskWidth / 2.0;

						for( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							double y = nY - m_nMaskHeight;

							for( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double dAngle = IPP_PI - abs(atan2( nX - dHalfWidth, y ));
								*pBuffer++ = int(dAngle * dFactor);
							}
						}
					}
					
					void FilledV()
					{
						double dFactorX = 128.0 / m_nMaskWidth;
						double dFactorY = 128.0 / m_nMaskHeight;
						double dOffsetX = 128.0;
						double dOffsetY = 128.0;

						if( Constants::c_nFilledVDown       == m_nMaskType || 
							Constants::c_nFilledVSmoothDown == m_nMaskType )
						{
							dFactorX *=  2; dOffsetX *= -1;
							dFactorY *= -1; dOffsetY *=  1;
						}
						else 
						if( Constants::c_nFilledVRight       == m_nMaskType || 
							Constants::c_nFilledVSmoothRight == m_nMaskType )
						{
							dFactorX *=  1; dOffsetX *=  0;
							dFactorY *=  2; dOffsetY *= -1;
						}
						else 
						if( Constants::c_nFilledVLeft       == m_nMaskType || 
							Constants::c_nFilledVSmoothLeft == m_nMaskType )
						{
							dFactorX *= -1; dOffsetX *=  1;
							dFactorY *=  2; dOffsetY *= -1;
						}
						else 
						if( Constants::c_nFilledVUp       == m_nMaskType || 
							Constants::c_nFilledVSmoothUp == m_nMaskType )
						{
							dFactorX *=  2; dOffsetX *= -1;
							dFactorY *=  1; dOffsetY *=  0;
						}

						Ipp8u* pBuffer = m_pMask;

						for( int nY = 0; nY < m_nMaskHeight; ++nY )
						{
							double dY = abs(nY * dFactorY + dOffsetY);
							
							for( int nX = 0; nX < m_nMaskWidth; ++nX )
							{
								*pBuffer++ = BYTE(abs(nX * dFactorX + dOffsetX) + dY);
							}
						}
					}
					void Insert()
					{
						double dFactorX = 255.0 / m_nMaskWidth;
						double dFactorY = 255.0 / m_nMaskHeight;
						double dOffsetX = 255.0;
						double dOffsetY = 255.0;
						
						if( Constants::c_nInsertDownLeft       == m_nMaskType || 
							Constants::c_nInsertSmoothDownLeft == m_nMaskType )
						{
							dFactorX *= -1; dOffsetX *= 1;
							dFactorY *= -1; dOffsetY *= 1;
						}
						else 
						if( Constants::c_nInsertDownRight       == m_nMaskType || 
							Constants::c_nInsertSmoothDownRight == m_nMaskType )
						{
							dFactorX *=  1; dOffsetX *= 0;
							dFactorY *= -1; dOffsetY *= 1;
						}
						else 
						if( Constants::c_nInsertUpLeft       == m_nMaskType || 
							Constants::c_nInsertSmoothUpLeft == m_nMaskType )
						{
							dFactorX *= -1; dOffsetX *= 1;
							dFactorY *=  1; dOffsetY *= 0;
						}
						else 
						if( Constants::c_nInsertUpRight       == m_nMaskType || 
							Constants::c_nInsertSmoothUpRight == m_nMaskType )
						{
							dFactorX *= 1; dOffsetX *= 0;
							dFactorY *= 1; dOffsetY *= 0;
						}

						Ipp8u* pBuffer = m_pMask;

						for( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							double dY = nY * dFactorY + dOffsetY;

							for( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double dX = nX * dFactorX + dOffsetX;

								*pBuffer++ = BYTE(max(dX, dY));
							}
						}
					}

					void Reveal()
					{
						double dFactor = 255.0;
						double dOffset = 255.0;
						BOOL bHor = TRUE;

						if( Constants::c_nRevealUp == m_nMaskType)
						{
							dFactor /= m_nMaskHeight; dOffset *= 0; bHor = FALSE;
						}
						else if( Constants::c_nRevealDown == m_nMaskType)
						{
							dFactor /= -m_nMaskHeight; dOffset *= 1; bHor = FALSE;
						}
						else if( Constants::c_nRevealLeft == m_nMaskType)
						{
							dFactor /= -m_nMaskWidth; dOffset *= 1;
						}
						else if( Constants::c_nRevealRight == m_nMaskType)
						{
							dFactor /= m_nMaskWidth; dOffset *= 0;
						}

						Ipp8u* pBuffer = m_pMask;

						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								*pBuffer++ = BYTE((bHor ? nX : nY) * dFactor + dOffset);
							}
						}
					}
					void Split()
					{
						BOOL bHor = FALSE;
						double dFactor = 511;
						
						if( Constants::c_nSplitHorizontal == m_nMaskType || 
							Constants::c_nSplitSmoothHorizontal == m_nMaskType )
						{
							dFactor /= m_nMaskHeight; bHor = FALSE;
						}
						else 
						if( Constants::c_nSplitVertical == m_nMaskType || 
							Constants::c_nSplitSmoothVertical == m_nMaskType )
						{
							dFactor /= m_nMaskWidth; bHor = TRUE;
						}

						Ipp8u* pBuffer = m_pMask;

						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								*pBuffer++ = BYTE(abs((bHor ? nX : nY) * dFactor - 255.5));
							}
						}
					}
					void SweepIn()
					{
						double dAngFactor = 255 / (IPP_PI2 - atan( m_nMaskHeight / (2.0 * m_nMaskWidth) ));
						
						double dFactor = m_nMaskHeight / (2.0 * m_nMaskWidth );
						double dOffset = m_nMaskHeight * 0.25;

						Ipp8u* pBuffer = m_pMask;

						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							double dY = nY - dOffset;

							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double dAngle;

								if ( dY > nX * dFactor )
								{
									double y = nX;
									double x = dY;
									
									dAngle = abs(atan2( y, x ));
								}
								else
								{
									double y = nX - m_nMaskWidth;
									double x = dY - dOffset - dOffset;

									dAngle = IPP_PI - abs(atan2( y, x ));
								}

								*pBuffer++ = BYTE(dAngle * dAngFactor);
							}
						}
					}
					void SweepOut()
					{
						Ipp8u* pBuffer = m_pMask;

						const double dFactor = 255 / IPP_PI;
						int nHalfWidth = m_nMaskWidth / 2;
						int nHalfHeight = m_nMaskHeight / 2;

						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double x = nY - nHalfHeight;
								double y = nX;
								
								if( nX > nHalfWidth)
									y -= m_nMaskWidth;

								double dAngle = abs( atan2( y, x ) );
								
								*pBuffer++ = BYTE(dAngle * dFactor);	
							}
						}
					}
					void SweepUp()
					{
						Ipp8u* pBuffer = m_pMask;

						const double dFactor = 255 / IPP_PI2;

						int nHalfWidth = m_nMaskWidth / 2;

						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double dAngle;

								if( nX > nHalfWidth )
								{
									double y = nY;
									double x = nX - m_nMaskWidth;			
									
									dAngle = IPP_PI - abs( atan2( y, x ));
								}
								else
								{
									double y = nY;
									double x = nX;			
									
									dAngle = abs( atan2( y, x ));
								}

								*pBuffer++ = BYTE(dAngle * dFactor);	
							}
						}
					}

					void ZigZagHor()
					{
						Ipp8u* pBuffer = m_pMask;

						const int nZigZagCount = 4;

						double dZigZagWidth  = m_nMaskWidth / (nZigZagCount * 2.0);
						double dZigZagHeight = m_nMaskHeight / 5.0;
						double dTimeUnit = 255.0 / (m_nMaskHeight * 0.6);

						double factor = dZigZagHeight / dZigZagWidth;
						double dAddOffset = (m_nMaskHeight + dZigZagHeight) / 2.0;
						double dSubOffset = (m_nMaskHeight - dZigZagHeight) / 2.0;

						for( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							int part = 0;

							for( int nX = 0; nX < m_nMaskWidth; ++nX, ++part )
							{
								if( part >= dZigZagWidth * 2 )
									part = 0;

								int value = 0;

								if( part < dZigZagWidth )
								{
									double offset = (dZigZagWidth - part) * factor;

									if( nY > dAddOffset - part * factor )
										value = int((nY - dSubOffset - offset) * dTimeUnit);
									else
										value = int((offset - nY + dSubOffset) * dTimeUnit);
								}
								else
								{
									double offset = (dZigZagWidth * 2 - part) * factor;
									
									if(nY > dSubOffset + (part - dZigZagWidth) * factor )
										value = int((nY - dAddOffset + offset) * dTimeUnit);
									else
										value = int((dAddOffset - nY - offset) * dTimeUnit);
								}

								*pBuffer++ = value;
							}
						}

					}

					void ZigZagVer()
					{
						Ipp8u* pBuffer = m_pMask;

						const int nZigZagCount = 4;

						double dZigZagHeight = m_nMaskHeight / (nZigZagCount * 2.0);
						double dZigZagWidth = m_nMaskWidth / 5.0;
						double dTimeUnit = 255 / (m_nMaskWidth * 0.6);

						double factor = dZigZagWidth / dZigZagHeight;
						double dAddOffset = (m_nMaskWidth + dZigZagWidth) / 2.0;
						double dSubOffset = (m_nMaskWidth - dZigZagWidth) / 2.0;
						
						int part = 0;

						for( int nY = 0; nY < m_nMaskHeight; ++nY, ++part )
						{
							if( part >= dZigZagHeight * 2 )
								part = 0;

							for( int nX = 0; nX < m_nMaskWidth; ++nX )
							{
								int value = 0;

								if( part < dZigZagHeight )
								{
									double offset = (dZigZagHeight - part) * factor;

									if( nX > - part * factor + dAddOffset )
										value = int((nX - dSubOffset - offset) * dTimeUnit);
									else
										value = int((dSubOffset - nX + offset) * dTimeUnit);
								}
								else
								{
									double offset = (dZigZagHeight * 2 - part) * factor;

									if( nX > (part - dZigZagHeight) * factor + dSubOffset )
										value = int((nX - dAddOffset + offset) * dTimeUnit);
									else
										value = int((dAddOffset - nX - offset) * dTimeUnit);
								}

								*pBuffer++ = value;
							}
						}
					}

					void PinwheelCurved()
					{
						double dParts = 1;
						double dAmplitFactor = 0.1;
						double dPhaseFactor = 2.0;

						switch( m_nMaskType )
						{
						case Constants::c_nPinwheelCurvedSN5A:        dParts =  5; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelCurvedSN7A:	      dParts =  7; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelCurvedSN12A:       dParts = 12; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelCurvedSW5A:        dParts =  5; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedSW7A:        dParts =  7; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedSW12A:       dParts = 12; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedLN5A:	      dParts =  5; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedLN7A:        dParts =  7; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedLN12A:       dParts = 12; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelPropeller4:        dParts =  4; dAmplitFactor = 0.4; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelPropeller7:	      dParts =  7; dAmplitFactor = 0.4; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelPropeller12:       dParts = 12; dAmplitFactor = 0.4; dPhaseFactor = 10.0; break;

						case Constants::c_nPinwheelCurvedSN5ASmooth:  dParts =  5; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelCurvedSN7ASmooth:  dParts =  7; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelCurvedSN12ASmooth: dParts = 12; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelCurvedSW5ASmooth:  dParts =  5; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedSW7ASmooth:  dParts =  7; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedSW12ASmooth: dParts = 12; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedLN5ASmooth:  dParts =  5; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedLN7ASmooth:  dParts =  7; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelCurvedLN12ASmooth: dParts = 12; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
						case Constants::c_nPinwheelPropeller4Smooth:  dParts =  4; dAmplitFactor = 0.4; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelPropeller7Smooth:  dParts =  7; dAmplitFactor = 0.4; dPhaseFactor = 10.0; break;
						case Constants::c_nPinwheelPropeller12Smooth: dParts = 12; dAmplitFactor = 0.4; dPhaseFactor = 10.0; break;
						}

						// создаём таблицу для ускорения расчётов
						int nSizeTable = int(m_nMaskWidth + m_nMaskHeight);
						double* pTable = new double[(nSizeTable + 1)];
						if( !pTable )
						{
							memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						double dMaxRadius = (m_nMaskWidth * m_nMaskWidth + m_nMaskHeight * m_nMaskHeight) / 4.0;
						double dFactor = dMaxRadius / nSizeTable;

						double dAmplit = sqrt(dMaxRadius) * dAmplitFactor; // * 4;
						if ( ( m_nMaskType >= Constants::c_nPinwheelPropeller4 && m_nMaskType <= Constants::c_nPinwheelPropeller12 ) || 
							 ( m_nMaskType >= Constants::c_nPinwheelPropeller4Smooth && m_nMaskType <= Constants::c_nPinwheelPropeller12Smooth ))
							 dAmplit = dMaxRadius * dAmplitFactor;
						double dPhase  = IPP_2PI / m_nMaskWidth * dPhaseFactor;

						// заполняем таблицу
						pTable[0] = 0;
						for( int i = 1; i <= nSizeTable; i++ )
						{
							double dr = sqrt(i * dFactor);
							pTable[i] = sin( dr * dPhase ) * dAmplit / dr;
						}
						
						// ещё одна вспомагательная таблица
						int nSizeHelp = max(m_nMaskWidth, m_nMaskHeight);
						double* pHelp = new double[nSizeHelp * 2];
						if( !pHelp )
						{
							delete [] pTable;
							memset( m_pMask, 255, m_nMaskWidth * m_nMaskHeight );
							return;
						}

						int nMinSize = nSizeHelp / 2;
						int nMaxSize = nSizeHelp - nMinSize;

						dFactor = nSizeTable / dMaxRadius;

						double* pItem = pHelp;
						for( int i = -nMinSize; i < nMaxSize; ++i, pItem += 2 )
						{
							pItem[1] = i;
							pItem[0] = i * i * dFactor;
						}
						
						Ipp8u* pBuffer = m_pMask;
						int nWidth  = m_nMaskWidth;
						int nHeight = m_nMaskHeight;

						double* pItemY = pHelp + (nMinSize - nHeight / 2) * 2;
						double* pItemX = pHelp + (nMinSize - nWidth  / 2) * 2;

						dParts *= 255 / IPP_2PI;

						for( int nY = nHeight; nY > 0; --nY, pItemY += 2, pItemX -= nWidth * 2 )
						{
							for ( int nX = nWidth; nX > 0; --nX, pItemX += 2 )
							{
								double dot = pItemY[0] + pItemX[0];
								
								int index = int(dot);
								double dr = pTable[index];

								// интерполяция
								if( index < nSizeTable )
								{
									dot -= index;
									dr += (pTable[index + 1] - dr) * dot;
								}

								double x = pItemX[1] - dr * pItemY[1];
								double y = pItemY[1] + dr * pItemX[1];
								
								double dAngle = atan2( y, x );
								if( dAngle < 0 )
									dAngle += IPP_2PI;

								*pBuffer++ = int( dAngle * dParts);
							}
						}

						delete [] pHelp;
						delete [] pTable;
					}

					void Diamonds( int nHorDiamonds, int nVerDiamonds, BOOL bOut )
					{
						Ipp8u* pBuffer = m_pMask;

						double dPartWidth  = double(m_nMaskWidth)  / nHorDiamonds; 
						double dPartHeight = double(m_nMaskHeight) / nVerDiamonds; 
						double dHalfWidth  = dPartWidth / 2;
						double dHalfHeight = dPartHeight / 2;

						double dFactorX = 255.0 / dPartWidth;
						double dFactorY = 255.0 / dPartHeight;

						double dMaxY = dPartHeight;
						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							if( dMaxY < nY )
								dMaxY += dPartHeight;

							double dY = abs( dMaxY - nY - dHalfHeight ) * dFactorY;

							double dMaxX = dPartWidth;
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								if( dMaxX < nX )
									dMaxX += dPartWidth;
								
								int nValue = int(abs( dMaxX - nX - dHalfWidth ) * dFactorX + dY);

								*pBuffer++ = bOut ? nValue : 255 - nValue;
							}
						}
					}
					void Circles( double dMaxCountCircles, BOOL bOut )
					{
						Ipp8u* pBuffer = m_pMask;

						double dPartSize = max(m_nMaskWidth, m_nMaskHeight) / (dMaxCountCircles - 0.001);
						double dHalfSize = dPartSize / 2;
						double dStartX   = (m_nMaskWidth  - (int(m_nMaskWidth  / dPartSize) + 1) * dPartSize) / 2 + dPartSize;
						double dStartY   = (m_nMaskHeight - (int(m_nMaskHeight / dPartSize) + 1) * dPartSize) / 2 + dPartSize;

						double dFactor   = 255 / sqrt(dHalfSize * dHalfSize * 2);

						double dMaxY = dStartY;
						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							if( dMaxY < nY )
								dMaxY += dPartSize;

							double dy = dMaxY - nY - dHalfSize;
							dy *= dy;

							double dMaxX = dStartX;
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								if( dMaxX < nX )
									dMaxX += dPartSize;

								double dx = dMaxX - nX - dHalfSize;

								int nValue = sqrt(dx * dx + dy) * dFactor;

								*pBuffer++ = bOut ? nValue : 255 - nValue;
							}
						}
					}

					void StarFlower()
					{
						int nType = m_nMaskType;
						Ipp8u* pBuffer = m_pMask;

						const int nStarsPoints = 5;
						const int nCountSectors = nStarsPoints * 2;
						
						const double dFactor = ::sin(IPP_PI2 * (nCountSectors - 4) / nCountSectors) / ::sin(IPP_PI2 * (nCountSectors - 8) / nCountSectors); // отношение большого радиуса звузды к малому
						
						const double dSector = IPP_2PI / nCountSectors;
						const double dReSector = nCountSectors / IPP_2PI;

						const double f1 = (dFactor - 1) * nCountSectors / IPP_2PI;
						const double f2 = 1 / sqrt(dFactor * (dFactor - 2 * cos(dSector)) + 1);

						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double y = nY - m_nMaskHeight / 2;
								double x = nX - m_nMaskWidth / 2;			
								
								double dCurRadius = sqrt(x * x + y * y);
								double angle = atan2( y, x ) - IPP_PI2;
								
								if( angle < 0 )
									angle += IPP_2PI;

								int nCurSector = angle * dReSector;
								double nCurAngle  = angle - nCurSector * dSector;
								
								double dSmallRadius;  // малый радиус звезды
								if ( nCurSector & 1 )
								{
									if( Constants::c_nWipeSmoothWideFlower   == nType || 
										Constants::c_nWipeSmoothNarrowFlower == nType )
									{
										dSmallRadius = dCurRadius / (1 + nCurAngle * f1);
									}
									else
									{
										double dValue = 1 / tan( asin( f2 * (dFactor * sin(dSector - nCurAngle) + sin(nCurAngle))));
										dSmallRadius = dCurRadius / (sin(nCurAngle) * dValue + cos(nCurAngle)); 
									}
								}
								else
								{
									if( Constants::c_nWipeSmoothWideFlower   == nType || 
										Constants::c_nWipeSmoothNarrowFlower == nType )
									{
										dSmallRadius = dCurRadius  / (dFactor - nCurAngle * f1); 
									}
									else
									{
										double dValue = 1 / tan(asin( f2 * (dFactor * sin(nCurAngle) + sin(dSector - nCurAngle))));
										dSmallRadius = dCurRadius / (sin(dSector - nCurAngle) * dValue + cos(dSector - nCurAngle)); 
									}
								}
								double dCurTime = 255. * dSmallRadius * 2 / sqrt( double (m_nMaskHeight * m_nMaskHeight + m_nMaskWidth * m_nMaskWidth));

								*pBuffer++ = int(dCurTime) & 255;
							}
						}
					}

					void StarsFlowers()
					{
						int nType = m_nMaskType;
						Ipp8u* pBuffer = m_pMask;

						for ( int nYIndex = 0; nYIndex < m_nMaskHeight; nYIndex++ )
						{
							for ( int nXIndex = 0; nXIndex < m_nMaskWidth; nXIndex++ )
							{
								const int nHorStars = 5;
								const int nVerStars = 5;
								int nPartWidth = m_nMaskWidth / nHorStars; 
								int nPartHeight = m_nMaskHeight / nVerStars; 
								
								int nHorIndex = nXIndex / nPartWidth;
								int nVerIndex = nYIndex / nPartHeight;
								
								const int nStarsPoints = 5;
								const int nCountSectors = nStarsPoints * 2;
								const int nSector = 360 / nCountSectors;
								double dRad = 3.14159265 / 180;
								double dFactor = sin(dRad *  ( 90 - nSector)) / sin(dRad * ( 90 - 2 * nSector)); // отношение большого радиуса звузды к малому
								double dValue = 300;

								for(int nIndex = 0; nIndex < 2; nIndex++ )
								{
									double dY = nYIndex - (nVerIndex + 0.5) * nPartHeight;
									double dX = nXIndex - (nHorIndex + 0.5) * nPartWidth;			
									double dCurRadius = sqrt(dX * dX + dY * dY);
									double dSecY = (dY >= 0 ? 1.0 : -1.0);
									int nAngle = (dCurRadius ? acos( dX / dCurRadius) * dSecY : 0) /dRad;
									nAngle = ( nAngle < 0? 360 + nAngle : nAngle);
								
									nAngle -= 90;
									nAngle = ( nAngle < 0? 360 + nAngle : nAngle);

									int nCurSector = nAngle / nSector;
									int nCurAngle = nAngle % nSector;
									
									double dSmallRadius;  // малый радиус звезды
									if ( nCurSector % 2 )
									{
										if (Constants::c_nWipeSmoothWideFlowers == nType || Constants::c_nWipeSmoothNarrowFlowers == nType)
											dSmallRadius = dCurRadius * nSector / (nSector + nCurAngle * (dFactor - 1)); 
										else
										{
											double dValue = 1 / tan(asin( ( dFactor * sin( dRad * (nSector - nCurAngle)) + sin(dRad * nCurAngle ) )/sqrt( dFactor * dFactor + 1 - 2 * dFactor * cos(dRad * nSector)) ));
											dSmallRadius = dCurRadius / (sin(dRad * nCurAngle) * dValue + cos(dRad * nCurAngle)); 
										}
									}
									else
									{
										if (Constants::c_nWipeSmoothWideFlowers == nType || Constants::c_nWipeSmoothNarrowFlowers == nType)
											dSmallRadius = dCurRadius * nSector / (dFactor * nSector + nCurAngle * (1 - dFactor)); 
										else
										{
											double dValue = 1 / tan(asin( ( dFactor * sin( dRad * (nCurAngle)) + sin(dRad * (nSector - nCurAngle) ) )/sqrt( dFactor * dFactor + 1 - 2 * dFactor * cos(dRad * nSector)) ));
											dSmallRadius = dCurRadius / (sin(dRad * (nSector - nCurAngle)) * dValue + cos(dRad * (nSector - nCurAngle))); 
										}
									}
									double dCurTime = 255. * dSmallRadius * 2 / sqrt( double (nPartHeight * nPartHeight + nPartWidth * nPartWidth));
									
									dValue = (dCurTime < dValue? dCurTime : dValue);
									
									if (nIndex == 0)
									{
										if ( nYIndex % nPartHeight > nPartHeight / 2)
											nVerIndex++;
										else
											nVerIndex--;
										if ( nVerIndex < 0 || nVerIndex >= nVerStars )
											nIndex = 2;
									}
								}
								*pBuffer = int(dValue) % 255;
								pBuffer++;
							}
						}
					}

					void KeyHoles()
					{
						Ipp8u* pBuffer = m_pMask;

						for ( int nYIndex = 0; nYIndex < m_nMaskHeight; nYIndex++ )
						{
							for ( int nXIndex = 0; nXIndex < m_nMaskWidth; nXIndex++ )
							{
								const int nHorHoles = 5;
								const int nVerHoles = 5;
								int nPartWidth = m_nMaskWidth / nHorHoles; 
								int nPartHeight = m_nMaskHeight / nVerHoles; 
								
								int nHorIndex = nXIndex / nPartWidth;
								int nVerIndex = nYIndex / nPartHeight;
								
								double dResValue = 300;

								for(int nIndex =0; nIndex < 2; nIndex++)
								{
									int nX = nXIndex - nHorIndex * nPartWidth;
									int nY = nYIndex - nVerIndex * nPartHeight;
									int nValue;
									int nValue1;
									double dUnit = 255. * 8. / (nPartWidth * 3. );
									if ( nY - 2. * nPartHeight / 3. <= - abs(8. * ( nX - nPartWidth / 2. ) / 3.))
									{
										nValue = (2. * nPartHeight / 3. - nY) * dUnit / 6.;
									}
									else
									{
										int nX1 = nX - nPartWidth / 2.;
										int nY1 = nY - 2. * nPartHeight / 3.;
										int nCurY = nY1 + 12. * abs(nX1) / 3.;
										nValue = nCurY * dUnit / 3.;
									}
									nValue1 = sqrt(double(( nX - nPartWidth / 2. ) * ( nX - nPartWidth / 2. ) + ( nY - 2 * nPartHeight / 3. ) * ( nY - 2. * nPartHeight / 3. ))) * dUnit / 2.;
									if ( nValue1 < nValue || 2. * nPartHeight / 3. < nY)
										nValue = nValue1;

									double dValue = sqrt( nValue / 255.) * 255;
									
									dResValue = (dValue < dResValue? dValue : dResValue);
									
									if (nIndex == 0)
									{
										if ( nYIndex % nPartHeight > nPartHeight / 2)
											nVerIndex++;
										else
											nVerIndex--;
										if ( nVerIndex < 0 || nVerIndex >= nVerHoles )
											nIndex = 2;
									}

								}
								*pBuffer = dResValue ;
								pBuffer++;
							}
						}
					}

					void KeyHole()
					{
						Ipp8u* pBuffer = m_pMask;

						for ( int nYIndex = 0; nYIndex < m_nMaskHeight; nYIndex++ )
						{
							for ( int nXIndex = 0; nXIndex < m_nMaskWidth; nXIndex++ )
							{
								int nValue;
								int nValue1;
								double dUnit = 255. * 8 / (m_nMaskWidth * 3 );
								if ( nYIndex - 2 * m_nMaskHeight / 3 <= - abs(8. * ( nXIndex - m_nMaskWidth / 2 ) / 3.))
								{
									nValue = (2 * m_nMaskHeight / 3 - nYIndex) * dUnit / 6;
								}
								else
								{
									int nX = nXIndex - m_nMaskWidth / 2;
									int nY = nYIndex - 2 * m_nMaskHeight / 3;
									int nCurY = nY + 12 * abs(nX) / 3;
									nValue = nCurY * dUnit / 3;
								}
								nValue1 = sqrt(double(( nXIndex - m_nMaskWidth / 2 ) * ( nXIndex - m_nMaskWidth / 2 ) + ( nYIndex - 2 * m_nMaskHeight / 3 ) * ( nYIndex - 2 * m_nMaskHeight / 3 ))) * dUnit / 2;
								if ( nValue1 < nValue || 2 * m_nMaskHeight / 3 < nYIndex)
									nValue = nValue1;

								double dValue = sqrt(nValue / 255.);
								*pBuffer = dValue * 255;
								pBuffer++;
							}
						}
					}

					void CircleCorner()
					{
						Ipp8u* pBuffer = m_pMask;

						double dW = m_nMaskWidth;
						double dH = m_nMaskHeight;

						double dFactor = 255 / sqrt(dW * dW + dH * dH);
						double dX = 0, dY = 0;

						switch( m_nMaskType )
						{
						case Constants::c_nCircleCornerLeftBottom:
						case Constants::c_nCircleCornerSmoothLeftBottom:  dX = 0;  dY = 0;  break;
						case Constants::c_nCircleCornerLeftTop:
						case Constants::c_nCircleCornerSmoothLeftTop:     dX = 0;  dY = dH; break;
						case Constants::c_nCircleCornerRightBottom:
						case Constants::c_nCircleCornerSmoothRightBottom: dX = dW; dY = 0;  break;
						case Constants::c_nCircleCornerRightTop:
						case Constants::c_nCircleCornerSmoothRightTop:    dX = dW; dY = dH; break;
						}

						for ( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							for ( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double y = nY - dY;
								double x = nX - dX;

								*pBuffer++ = BYTE(dFactor * sqrt( x * x + y * y ));
							}
						}
					}

	
					void BarsHorizontal()
					{
						CRandom rnd( m_dwBaseRnd );

						Ipp8u* pBuffer = m_pMask;

						for( int nY = 0; nY < m_nMaskHeight; ++nY, pBuffer += m_nMaskWidth )
						{
							::memset( pBuffer, rnd.Rand( 255 ), m_nMaskWidth );
						}
					}

					void BarsVertical()
					{
						CRandom rnd( m_dwBaseRnd );

						Ipp8u* pBuffer = m_pMask;

						for( int i = 0; i < m_nMaskWidth; ++i )
						{
							*pBuffer++ = rnd.Rand( 255 );
						}
						
						for( int nY = 1; nY < m_nMaskHeight; ++nY, pBuffer += m_nMaskWidth )
						{
							::memcpy( pBuffer, m_pMask, m_nMaskWidth );
						}
					}


					void Box()
					{
						Ipp8u* pBuffer = m_pMask;

						int nIn = 1;
						if ( Constants::c_nWipeSmoothBoxIn == m_nMaskType )
							nIn = 0;
						
						double dFactorX = 511.0 / m_nMaskWidth ;
						double dFactorY = 511.0 / m_nMaskHeight;

						double dHalfWidth  = m_nMaskWidth / 2.0;
						double dHalfHeight = m_nMaskHeight / 2.0;

						for( int nY = 0; nY < m_nMaskHeight; nY++ )
						{
							double y = dFactorY * abs(nY - dHalfHeight);

							for( int nX = 0; nX < m_nMaskWidth; nX++ )
							{
								double x = dFactorX * abs(nX - dHalfWidth);

								*pBuffer++ = 255 * (1 - nIn) + (2 * nIn - 1) * max(x, y);
							}
						}
					}
					void Noise()
					{
						CRandom rnd( m_dwBaseRnd );

						int nSize = m_nMaskWidth * m_nMaskHeight;
						Ipp8u* pBuffer = m_pMask;

						for( int i = 0; i < nSize; ++i, ++pBuffer )
						{
							pBuffer[0] = rnd.Rand( 256 );
						}
					}

					void Shutter()
					{
						const int c_nPathCount = 10;
						
						double interval = m_nMaskHeight / double(c_nPathCount);
						if( interval < 10 )
							interval = 10;
						
						double factor = 255 / interval;
						Ipp8u* pBuffer = m_pMask;

						double y = 0;
						for( int row = 0; row < m_nMaskHeight; ++row, ++y )
						{
							if( y > interval )
								y = 0;

							memset( pBuffer, 255 - int(y * factor), m_nMaskWidth );
							pBuffer += m_nMaskWidth;
						}
					}
				public:

					int	m_nMaskWidth;
					int	m_nMaskHeight;
					
					Ipp8u* m_pMask;
					int m_nMaskType;

				};

				class CPuzzleFigure
				{
				public:

					CPuzzleFigure()
					{
						m_nFigHeight = 0;
						m_nFigWidth  = 0;
						
						m_nPicHeight = 0;
						m_nPicWidth  = 0;

					    m_nLeftFactor   = 0; 
					    m_nRightFactor  = 0;					
						m_nTopFactor    = 0;					
						m_nBottomFactor = 0;

						m_nXCoorSource = 0;
						m_nYCoorSource = 0;
					}
					CPuzzleFigure(int nFigWidth, int nFigHeight, int nTopFactor, int nRightFactor, int nBottomFactor, int nLeftFactor, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
					{
						m_nFigHeight = nFigHeight;
						m_nFigWidth  = nFigWidth;

					    m_nLeftFactor   = nLeftFactor;
					    m_nRightFactor  = nRightFactor;
					    m_nTopFactor    = nTopFactor;
						m_nBottomFactor = nBottomFactor;
						
						m_nPicHeight = nPicHeight;
						m_nPicWidth  = nPicWidth;
						
						m_pBGRASource = pBGRASource;
						
						m_nXCoorSource = nXCoorSource;
						m_nYCoorSource = nYCoorSource;
					}
					~CPuzzleFigure()
					{
					}

				private:

					void DrawAlphaBlendingLine(BYTE* pBGRAResult, BYTE* pBGRASource, int nColor, int nNumLine, double dEffect)
					{
						int nAlpha = 7 * (1 << nNumLine);

						nColor = nColor * nAlpha + 127;
						nAlpha = 255 - nAlpha;

						if( 1 == dEffect )
						{
							pBGRAResult[0] = (nColor + pBGRASource[0] * nAlpha) * 0x8081U >> (7 + 16);
							pBGRAResult[1] = (nColor + pBGRASource[1] * nAlpha) * 0x8081U >> (7 + 16);
							pBGRAResult[2] = (nColor + pBGRASource[2] * nAlpha) * 0x8081U >> (7 + 16);
						}
						else
						{
							double factor = 1 - dEffect;
							pBGRAResult[0] = BYTE(dEffect * ((nColor + pBGRASource[0] * nAlpha) * 0x8081U >> (7 + 16)) + pBGRASource[0] * factor + 0.5);
							pBGRAResult[1] = BYTE(dEffect * ((nColor + pBGRASource[1] * nAlpha) * 0x8081U >> (7 + 16)) + pBGRASource[0] * factor + 0.5);
							pBGRAResult[2] = BYTE(dEffect * ((nColor + pBGRASource[2] * nAlpha) * 0x8081U >> (7 + 16)) + pBGRASource[0] * factor + 0.5);

						}
						pBGRAResult[3] = 255;
					}

				public:

					void SetValue(int nFigWidth, int nFigHeight, int nTopFactor, int nRightFactor, int nBottomFactor, int nLeftFactor, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
					{
						m_nFigHeight = nFigHeight;
						m_nFigWidth  = nFigWidth;

						m_nLeftFactor   = nLeftFactor;
					    m_nRightFactor  = nRightFactor;
					    m_nTopFactor    = nTopFactor;
						m_nBottomFactor = nBottomFactor;

						m_nPicHeight = nPicHeight;
						m_nPicWidth  = nPicWidth;

						m_pBGRASource = pBGRASource;

						m_nXCoorSource = nXCoorSource;
						m_nYCoorSource = nYCoorSource;

					}

					
					BOOL Cpy(BYTE* pBGRAResult, int nXCoorResult, int nYCoorResult, int nWidth, int nHeight, int nAlpha)
					{

						if(!pBGRAResult || nWidth != m_nPicWidth || nHeight != m_nPicHeight)
							return FALSE;
						
						int nSize = 4 * nHeight * nWidth;

						BYTE* pSource = m_pBGRASource;
						BYTE* pResult = pBGRAResult;
						
						//double dDiagonal = sqrt(double(m_nFigHeight * m_nFigHeight + m_nFigWidth * m_nFigWidth));
						//
						//double dUnit = 1 / (72.0 * sqrt (2.0)) * dDiagonal;
						double dDiagonal = min(m_nFigHeight, m_nFigWidth);
						
						double dUnit = 1 / (72.0) * dDiagonal;
						int nCircleWidth = 26 * dUnit;                  // ширина кружочка, т.е. 2 * радиус - (зазор со средней частью)
						int nRadius      = 15 * dUnit;                  // радиус кружочка
						int nSqrRadius   = nRadius * nRadius;
						int nSqrIncRadius = ( nRadius + 1 ) * ( nRadius + 1 );
						int nSqrDecRadius = ( nRadius - 1 ) * ( nRadius - 1 );

						int nInStep      = 37 * dUnit;

						int nXStartRes = nXCoorResult - nCircleWidth;
						int nYStartRes = nYCoorResult - nCircleWidth;
						int nXStartSur = m_nXCoorSource - nCircleWidth;
						int nYStartSur = m_nYCoorSource - nCircleWidth;

						int nFullHeight = m_nFigHeight + 2 * nCircleWidth; // полная высота кусочка, с учетом кружков
						int nFullWidth  = m_nFigWidth  + 2 * nCircleWidth; // полная ширина кусочка

						int nXSource = m_nXCoorSource - nCircleWidth;
						int nYSource = m_nYCoorSource - nCircleWidth;
						int nXResult = nXCoorResult - nCircleWidth;
						int nYResult = nYCoorResult - nCircleWidth;

						double dStep = 11 * dUnit;
						double dSrez  = sqrt(nSqrRadius - dStep * dStep);
						double dAngle = acos(11 * dUnit / nRadius) * 180 / IPP_PI;


						if ( nXStartRes < 0 )
						{
							nXStartSur += -nXStartRes;
							nXStartRes = 0;
						}
						if (nYStartRes < 0)
						{
							nYStartSur += -nYStartRes;
							nYStartRes = 0;
						}

						for ( int nYIndexResult = nYStartRes, nYIndexSource = nYStartSur; nYIndexResult < min( nHeight, nYResult + nFullHeight); nYIndexResult++, nYIndexSource++)
						{
							for(int nXIndexResult = nXStartRes, nXIndexSource = nXStartSur; nXIndexResult < min( nWidth, nXResult + nFullWidth); nXIndexResult++, nXIndexSource++)
							{
								BOOL bIn = FALSE;
								
								int nYS = nYIndexSource;
								int nXS = nXIndexSource;

								if ( (nYS > nYSource + nCircleWidth && nYS < nYSource + nFullHeight - nCircleWidth &&
									  nXS > nXSource + nCircleWidth && nXS <= nXSource + nFullWidth  - nCircleWidth && 
									  (m_nTopFactor    >= 0 || (m_nTopFactor    == -1 && ( nYS - nYSource - nInStep ) * ( nYS - nYSource - nInStep ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 )                             >= nSqrRadius ))&&
									  (m_nLeftFactor   >= 0 || (m_nLeftFactor   == -1 && ( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nInStep ) * ( nXS - nXSource - nInStep )                           >= nSqrRadius ))&&
									  (m_nRightFactor  >= 0 || (m_nRightFactor  == -1 && ( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nFullWidth + nInStep ) * ( nXS - nXSource - nFullWidth + nInStep ) >= nSqrRadius ))&&
									  (m_nBottomFactor >= 0 || (m_nBottomFactor == -1 && ( nYS - nYSource - nFullHeight + nInStep ) * ( nYS - nYSource - nFullHeight + nInStep ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 ) >= nSqrRadius ))
									  )
									  
									  || 
									 (m_nTopFactor    == 1 && ( nYS - nYSource - nRadius ) * ( nYS - nYSource - nRadius ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 )                             <= nSqrDecRadius )||
									 (m_nLeftFactor   == 1 && ( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nRadius ) * ( nXS - nXSource - nRadius )                           <= nSqrDecRadius )||
									 (m_nRightFactor  == 1 && ( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nFullWidth + nRadius ) * ( nXS - nXSource - nFullWidth + nRadius ) <= nSqrDecRadius )||
									 (m_nBottomFactor == 1 && ( nYS - nYSource - nFullHeight + nRadius ) * ( nYS - nYSource - nFullHeight + nRadius ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 ) <= nSqrDecRadius )
									 )
								{								
									bIn = TRUE;
								}
								int nResultIndex = 4 * nWidth * nYIndexResult + 4 * nXIndexResult;
								int nSourceIndex = 4 * nWidth * nYIndexSource + 4 * nXIndexSource;
								if ( bIn )
								{
									double dKoef = nAlpha / 255.0; 
									pResult[ nResultIndex + 0 ] = pSource[ nSourceIndex + 0 ] * dKoef + ( 1 - dKoef) * pResult[ nResultIndex + 0 ];
									pResult[ nResultIndex + 1 ] = pSource[ nSourceIndex + 1 ] * dKoef + ( 1 - dKoef) * pResult[ nResultIndex + 1 ];
									pResult[ nResultIndex + 2 ] = pSource[ nSourceIndex + 2 ] * dKoef + ( 1 - dKoef) * pResult[ nResultIndex + 2 ];
									pResult[ nResultIndex + 3 ] = pSource[ nSourceIndex + 3 ] * dKoef + ( 1 - dKoef) * pResult[ nResultIndex + 3 ];
								}
						



						//		if ( 1 == m_nTopFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nRadius ) * ( nYS - nYSource - nRadius ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 )));
						//			
						//			if ( nDistance > nRadius && nYS >= nYSource + nCircleWidth && nYS <= nYSource + nCircleWidth + 5 && nXS >= nXSource + nCircleWidth + (5 - (nYSource + nCircleWidth + 5 - nYS)) && nXS <= nXSource + nFullWidth  - nCircleWidth - (5 - (nYSource + nCircleWidth + 5 - nYS)))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, nYSource + nCircleWidth + 5 - nYS, dEffect);
						//			if ( nDistance <= nRadius && nDistance >= nRadius - 5 && nYS <= nYSource + nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, 5 - (nRadius - nDistance), dEffect);
						//		}
						//		else if ( -1 == m_nTopFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nInStep ) * ( nYS - nYSource - nInStep ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 )));
						//			
						//			if ( nDistance <= nRadius + 5 && nDistance >= nRadius && nYS >= nYSource + nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, 5- (nDistance - nRadius), dEffect);
						//			if ( nDistance > nRadius && nYS >= nYSource + nCircleWidth && nYS <= nYSource + nCircleWidth + 5 && nXS >= nXSource + nCircleWidth + (5 - (nYSource + nCircleWidth + 5 - nYS)) && nXS <= nXSource + nFullWidth  - nCircleWidth - (5 - (nYSource + nCircleWidth + 5 - nYS)))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, nYSource + nCircleWidth + 5 - nYS, dEffect);
						//		}
						//		else
						//		{
						//			if ( nYS >= nYSource + nCircleWidth && nYS <= nYSource + nCircleWidth + 5 && nXS >= nXSource + nCircleWidth + (5 - (nYSource + nCircleWidth + 5 - nYS)) && nXS <= nXSource + nFullWidth  - nCircleWidth - (5 - (nYSource + nCircleWidth + 5 - nYS)))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, nYSource + nCircleWidth + 5 - nYS, dEffect);
						//		}

						//		if ( 1 == m_nBottomFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nFullHeight + nRadius ) * ( nYS - nYSource - nFullHeight + nRadius ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 )));
						//			
						//			if ( nDistance > nRadius && nYS <= nYSource + nFullHeight - nCircleWidth && nYS >= nYSource + nFullHeight - nCircleWidth - 5 && nXS >= nXSource + nCircleWidth + (nYSource + nFullHeight - nCircleWidth - nYS) && nXS <= nXSource + nFullWidth  - nCircleWidth - (nYSource + nFullHeight - nCircleWidth - nYS))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, 5 - (nYSource + nFullHeight - nCircleWidth - nYS), dEffect);
						//			if ( nDistance <= nRadius && nDistance >= nRadius - 5 && nYS >= nYSource + nFullHeight - nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, 5 - (nRadius - nDistance), dEffect);
						//		}
						//		else if ( -1 == m_nBottomFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nFullHeight + nInStep ) * ( nYS - nYSource - nFullHeight + nInStep ) + ( nXS - nXSource - nFullWidth / 2 ) * ( nXS - nXSource - nFullWidth / 2 )));

						//			if ( nDistance <= nRadius + 5 && nDistance >= nRadius && nYS <= nYSource + nFullHeight - nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, 5 - (nDistance - nRadius), dEffect);

						//			if ( nDistance > nRadius && nYS <= nYSource + nFullHeight - nCircleWidth && nYS >= nYSource + nFullHeight - nCircleWidth - 5 && nXS >= nXSource + nCircleWidth + (nYSource + nFullHeight - nCircleWidth - nYS) && nXS <= nXSource + nFullWidth  - nCircleWidth - (nYSource + nFullHeight - nCircleWidth - nYS))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, 5 - (nYSource + nFullHeight - nCircleWidth - nYS), dEffect);
						//		}
						//		else
						//		{
						//			if ( nYS <= nYSource + nFullHeight - nCircleWidth && nYS >= nYSource + nFullHeight - nCircleWidth - 5 && nXS >= nXSource + nCircleWidth + (nYSource + nFullHeight - nCircleWidth - nYS) && nXS <= nXSource + nFullWidth  - nCircleWidth - (nYSource + nFullHeight - nCircleWidth - nYS))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, 5 - (nYSource + nFullHeight - nCircleWidth - nYS), dEffect);
						//		}
						//		
						//		if ( 1 == m_nLeftFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nRadius ) * ( nXS - nXSource - nRadius )));
						//		
						//			if ( nDistance > nRadius && nXS >= nXSource + nCircleWidth && nXS <= nXSource + nCircleWidth + 5 && nYS >= nYSource + nCircleWidth + (5 - (nXSource + nCircleWidth + 5 - nXS))&& nYS <= nYSource + nFullHeight - nCircleWidth - (5 - (nXSource + nCircleWidth + 5 - nXS)))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, nXSource + nCircleWidth + 5 - nXS, dEffect);

						//			if ( nDistance <= nRadius && nDistance >= nRadius - 5 && nXS <= nXSource + nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, 5 - (nRadius - nDistance), dEffect);
						//		}
						//		else if ( -1 == m_nLeftFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nInStep ) * ( nXS - nXSource - nInStep )));

						//			if ( nDistance <= nRadius + 5 && nDistance >= nRadius && nXS >= nXSource + nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, 5- (nDistance - nRadius), dEffect);

						//			if ( nDistance > nRadius && nXS >= nXSource + nCircleWidth && nXS <= nXSource + nCircleWidth + 5  && nYS >= nYSource + nCircleWidth + (5 - (nXSource + nCircleWidth + 5 - nXS))&& nYS <= nYSource + nFullHeight - nCircleWidth - (5 - (nXSource + nCircleWidth + 5 - nXS)))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, nXSource + nCircleWidth + 5 - nXS, dEffect);
						//		}
						//		else
						//		{
						//			if ( nXS >= nXSource + nCircleWidth && nXS <= nXSource + nCircleWidth + 5  && nYS >= nYSource + nCircleWidth + (5 - (nXSource + nCircleWidth + 5 - nXS))&& nYS <= nYSource + nFullHeight - nCircleWidth - (5 - (nXSource + nCircleWidth + 5 - nXS)))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 255, nXSource + nCircleWidth + 5 - nXS, dEffect);
						//		}

						//		if ( 1 == m_nRightFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nFullWidth + nRadius ) * ( nXS - nXSource - nFullWidth + nRadius )));
						//			
						//			if ( nDistance > nRadius && nXS <= nXSource + nFullWidth  - nCircleWidth && nXS >= nXSource + nFullWidth  - nCircleWidth - 5 && nYS >= nYSource + nCircleWidth + (nXSource + nFullWidth  - nCircleWidth - nXS)&& nYS <= nYSource + nFullHeight - nCircleWidth - (nXSource + nFullWidth  - nCircleWidth - nXS))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, 5 - (nXSource + nFullWidth  - nCircleWidth - nXS), dEffect);

						//			if ( nDistance <= nRadius && nDistance >= nRadius - 5 && nXS >= nXSource + nFullWidth  - nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, 5 - (nRadius - nDistance), dEffect);
						//		}
						//		else if ( -1 == m_nRightFactor )
						//		{
						//			int nDistance = sqrt(double(( nYS - nYSource - nFullHeight / 2 ) * ( nYS - nYSource - nFullHeight / 2 ) + ( nXS - nXSource - nFullWidth + nInStep ) * ( nXS - nXSource - nFullWidth + nInStep )));
						//			
						//			if ( nDistance <= nRadius + 5 && nDistance >= nRadius && nXS <= nXSource + nFullWidth  - nCircleWidth )
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, 5- (nDistance - nRadius), dEffect);

						//			if ( nDistance > nRadius && nXS <= nXSource + nFullWidth  - nCircleWidth && nXS >= nXSource + nFullWidth  - nCircleWidth - 5 && nYS >= nYSource + nCircleWidth + (nXSource + nFullWidth  - nCircleWidth - nXS)&& nYS <= nYSource + nFullHeight - nCircleWidth - (nXSource + nFullWidth  - nCircleWidth - nXS))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, 5 - (nXSource + nFullWidth  - nCircleWidth - nXS), dEffect);

						//		}
						//		else
						//		{
						//			if ( nXS <= nXSource + nFullWidth  - nCircleWidth && nXS >= nXSource + nFullWidth  - nCircleWidth - 5 && nYS >= nYSource + nCircleWidth + (nXSource + nFullWidth  - nCircleWidth - nXS)&& nYS <= nYSource + nFullHeight - nCircleWidth - (nXSource + nFullWidth  - nCircleWidth - nXS))
						//				DrawAlphaBlendingLine(pResult + nResultIndex, pSource + nSourceIndex, 0, 5 - (nXSource + nFullWidth  - nCircleWidth - nXS), dEffect);
						//		}
							}
						}


						int nXCur = nXResult;
						int nYCur = nYResult;
						BYTE* pBuffer = pResult; 
						pBuffer += 4 * nWidth * (nHeight - 1);
						
						Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBuffer );
						if( Gdiplus::Ok == oImage.GetLastStatus() )
						{
							Gdiplus::Graphics oRender( &oImage );
							
							oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
							oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
							oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
							
							double dAlphaKoef = nAlpha / 255.0;
							int nAlphaBlack = 448 * dAlphaKoef;
							int nAlphaWhite = 448 * dAlphaKoef;
							
							for (int nIndex = 0; nIndex < 4; nIndex++ )
							{
								nAlphaBlack /= 2; 
								if (nIndex == 1)
								{
									nAlphaBlack = 224 * dAlphaKoef;
									nAlphaWhite = 224 * dAlphaKoef;
								}
								Gdiplus::Pen oPen( Color( nAlphaBlack, 0, 0, 0) , 1);
								if ( 0 == m_nRightFactor )
									oRender.DrawLine(&oPen, float(nXResult + nFullWidth  - nCircleWidth - nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex), float(nXResult + nFullWidth  - nCircleWidth - nIndex), nHeight - float(nYResult + nCircleWidth + nIndex));
								else
								{
									if ( 1 == m_nRightFactor)
										oRender.DrawArc(&oPen, float(nXResult + nFullWidth - 2 * nRadius + nIndex), nHeight - float(nYResult + nFullHeight / 2 + nRadius - nIndex), float(2 * (nRadius - nIndex)), float(2 * (nRadius - nIndex)), 180 + dAngle, 360 - 2 * dAngle);
									else if ( -1 == m_nRightFactor)
										oRender.DrawArc(&oPen, float(nXResult + nFullWidth - 2 * nRadius - nIndex - 2 * dStep), nHeight - float(nYResult + nFullHeight / 2 + nRadius + nIndex), float(2 * (nRadius + nIndex)), float(2 * (nRadius + nIndex)), dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
									oRender.DrawLine(&oPen, float(nXResult + nFullWidth  - nCircleWidth - nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex), float(nXResult + nFullWidth  - nCircleWidth - nIndex), nHeight - float(nYResult + nFullHeight / 2 + dSrez +nIndex));
									oRender.DrawLine(&oPen, float(nXResult + nFullWidth  - nCircleWidth - nIndex), nHeight - float(nYResult + nFullHeight / 2 - dSrez - nIndex), float(nXResult + nFullWidth  - nCircleWidth - nIndex), nHeight - float(nYResult + nCircleWidth + nIndex));
								}

								if ( 0 == m_nTopFactor )
									oRender.DrawLine(&oPen, float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nCircleWidth + nIndex), float(nXResult + nFullWidth - nCircleWidth - nIndex), nHeight - float(nYResult + nCircleWidth + nIndex));
								else
								{
									if ( 1 == m_nTopFactor)
										oRender.DrawArc(&oPen, float(nXResult + nFullWidth / 2 - nRadius + nIndex), nHeight - float(nYResult + 2 * nRadius - nIndex), float(2 * (nRadius - nIndex)), float(2 * (nRadius - nIndex)), 270 + dAngle, 360 - 2 * dAngle);
									else if ( -1 == m_nTopFactor)
										oRender.DrawArc(&oPen, float(nXResult + nFullWidth / 2 - nRadius - nIndex), nHeight - float(nYResult + 2 * nRadius + nIndex + 2 * dStep), float(2 * (nRadius + nIndex)), float(2 * (nRadius + nIndex)), 90 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
									oRender.DrawLine(&oPen, float(nXResult + nFullWidth - nCircleWidth - nIndex), nHeight - float(nYResult + nCircleWidth + nIndex), float(nXResult + nFullWidth / 2 + dSrez + nIndex), nHeight - float(nYResult + nCircleWidth + nIndex));
									oRender.DrawLine(&oPen, float(nXResult + nFullWidth / 2 - dSrez - nIndex), nHeight - float(nYResult + nCircleWidth + nIndex), float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nCircleWidth + nIndex));
								}

								nAlphaWhite /= 2;
								oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));

								if ( 0 == m_nLeftFactor )
									oRender.DrawLine(&oPen, float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nCircleWidth + nIndex), float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex));
								else
								{
									if ( 1 == m_nLeftFactor)
										oRender.DrawArc(&oPen, float(nXResult + nIndex), nHeight - float(nYResult + nFullHeight / 2 + nRadius - nIndex), float(2 * (nRadius - nIndex)), float(2 * (nRadius - nIndex)), dAngle, 360 - 2 * dAngle);
									else if ( -1 == m_nLeftFactor)
										oRender.DrawArc(&oPen, float(nXResult - nIndex + 2 * dStep), nHeight - float(nYResult + nFullHeight / 2 + nRadius + nIndex), float(2 * (nRadius + nIndex)), float(2 * (nRadius + nIndex)), 180 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
									oRender.DrawLine(&oPen, float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nCircleWidth + nIndex), float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nFullHeight / 2 - dSrez - nIndex));
									oRender.DrawLine(&oPen, float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nFullHeight / 2 + dSrez + nIndex), float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex));
								}

								if ( 0 == m_nBottomFactor )
									oRender.DrawLine(&oPen, float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex), float(nXResult + nFullWidth - nCircleWidth - nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex));
								else
								{
									if ( 1 == m_nBottomFactor)
										oRender.DrawArc(&oPen, float(nXResult + nFullWidth / 2 - nRadius + nIndex), nHeight - float(nYResult + nFullHeight - nIndex), float(2 * (nRadius - nIndex)), float(2 * (nRadius - nIndex)), 90 + dAngle, 360 - 2 * dAngle);
									else if ( -1 == m_nBottomFactor)
										oRender.DrawArc(&oPen, float(nXResult + nFullWidth / 2 - nRadius - nIndex), nHeight - float(nYResult + nFullHeight + nIndex - 2 * dStep), float(2 * (nRadius + nIndex)), float(2 * (nRadius + nIndex)), 270 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
									oRender.DrawLine(&oPen, float(nXResult + nFullWidth - nCircleWidth - nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex), float(nXResult + nFullWidth / 2 + dSrez + nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex));
									oRender.DrawLine(&oPen, float(nXResult + nFullWidth / 2 - dSrez - nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex), float(nXResult + nCircleWidth + nIndex), nHeight - float(nYResult + nFullHeight - nCircleWidth - nIndex));
								}


							}
						}

						return TRUE;
					}

					BOOL Draw(Gdiplus::Graphics *oMainRender, Gdiplus::TextureBrush *oBrush, double dXCoorResult, double dYCoorResult, int nWidth, int nHeight, double dEffect)
					{
						if(!oMainRender || !oBrush || nWidth != m_nPicWidth || nHeight != m_nPicHeight )
							return FALSE;
						
						int nSize = 4 * nHeight * nWidth;

						
						double dDiagonal = min(m_nFigHeight, m_nFigWidth);
						
						double dUnit = 1 / (72.0) * dDiagonal;
						double dCircleWidth = 26 * dUnit;                  // ширина кружочка, т.е. 2 * радиус - (зазор со средней частью)
						double dRadius      = 15 * dUnit;                  // радиус кружочка
						double dSqrRadius   = dRadius * dRadius; 
						double dDiff = dCircleWidth - dRadius; 

						double dInStep      = 37 * dUnit;

						double dStep = 11 * dUnit;
						double dSrez  = sqrt(dSqrRadius - dStep * dStep);
						double dAngle = acos(11 * dUnit / dRadius) * 180 / IPP_PI;

						double dXSur = m_nXCoorSource;
						double dYSur = m_nYCoorSource;

						double dFullHeight = m_nFigHeight + 2 * dCircleWidth; // полная высота кусочка, с учетом кружков
						double dFullWidth  = m_nFigWidth  + 2 * dCircleWidth; // полная ширина кусочка

						double dXResult = dXCoorResult - dCircleWidth;
						double dYResult = dYCoorResult - dCircleWidth;


						Gdiplus::GraphicsPath oPathAddEllipse;
						if ( 1 == m_nLeftFactor )
							oPathAddEllipse.AddEllipse( float(dXSur - dCircleWidth),                             nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),  float(2 * dRadius), float(2 * dRadius)); // left
						if ( 1 == m_nRightFactor )
							oPathAddEllipse.AddEllipse( float(dXSur + dCircleWidth + m_nFigWidth - 2 * dRadius), nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),  float(2 * dRadius), float(2 * dRadius)); //right
						if ( 1 == m_nBottomFactor )
							oPathAddEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                nHeight - float(dYSur + m_nFigHeight + dCircleWidth), float(2 * dRadius), float(2 * dRadius)); //bottom
						if ( 1 == m_nTopFactor )
							oPathAddEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                nHeight - float(dYSur - dCircleWidth + 2 * dRadius),  float(2 * dRadius), float(2 * dRadius)); //top
						oPathAddEllipse.CloseFigure();

						Gdiplus::GraphicsPath oPathExclEllipse;
						if ( -1 == m_nLeftFactor )
							oPathExclEllipse.AddEllipse( float(dXSur - dCircleWidth + 2 * dDiff),                             nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),             float(2 * dRadius), float(2 * dRadius)); // left
						if ( -1 == m_nRightFactor )
							oPathExclEllipse.AddEllipse( float(dXSur + dCircleWidth + m_nFigWidth - 2 * dRadius - 2 * dDiff), nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),             float(2 * dRadius), float(2 * dRadius)); //right
						if ( -1 == m_nBottomFactor )
							oPathExclEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                            nHeight - float(dYSur + m_nFigHeight + dCircleWidth - 2 * dDiff),float(2 * dRadius), float(2 * dRadius)); //bottom
						if ( -1 == m_nTopFactor )
							oPathExclEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                            nHeight - float(dYSur - dCircleWidth + 2 * dRadius + 2 * dDiff), float(2 * dRadius), float(2 * dRadius)); //top
						oPathExclEllipse.CloseFigure();

						Gdiplus::Region oRegion(&oPathAddEllipse);

						oRegion.Union(Gdiplus::RectF(dXSur, nHeight - (dYSur + m_nFigHeight), m_nFigWidth, m_nFigHeight));
						oRegion.Exclude(&oPathExclEllipse);	
						// сдвигаем регион и кисть, так чтобы они двигались вместе
						oBrush->TranslateTransform(-float(m_nXCoorSource - dXCoorResult), float(m_nYCoorSource - dYCoorResult), MatrixOrderAppend);
						oRegion.Translate(-float(m_nXCoorSource - dXCoorResult), float(m_nYCoorSource - dYCoorResult));
						
						oMainRender->FillRegion(oBrush, &oRegion);
						

						
						int nAlphaBlack = 448 * dEffect;
						int nAlphaWhite = 448 * dEffect;
						
						for (int nIndex = 0; nIndex < 4; nIndex++ )
						{
							nAlphaBlack /= 2; 
							if (nIndex == 1)
							{
								nAlphaBlack = 224 * dEffect;
								nAlphaWhite = 224 * dEffect;
							}
							Gdiplus::Pen oPen( Color( nAlphaBlack, 0, 0, 0) , 1);
							if ( 0 == m_nRightFactor )
								oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
							else
							{
								if ( 1 == m_nRightFactor)
									oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth - 2 * dRadius + nIndex), nHeight - float(dYResult + dFullHeight / 2 + dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), 180 + dAngle, 360 - 2 * dAngle);
								else if ( -1 == m_nRightFactor)
									oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth - 2 * dRadius - nIndex - 2 * dStep), nHeight - float(dYResult + dFullHeight / 2 + dRadius + nIndex), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
								oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight / 2 + dSrez +nIndex));
								oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight / 2 - dSrez - nIndex),    float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
							}

							if ( 0 == m_nTopFactor )
								oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
							else
							{
								if ( 1 == m_nTopFactor)
									oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius + nIndex), nHeight - float(dYResult + 2 * dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), 270 + dAngle, 360 - 2 * dAngle);
								else if ( -1 == m_nTopFactor)
									oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius - nIndex), nHeight - float(dYResult + 2 * dRadius + nIndex + 2 * dStep), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), 90 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
								oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dFullWidth / 2 + dSrez + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
								oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth / 2 - dSrez - nIndex),    nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dCircleWidth + nIndex),           nHeight - float(dYResult + dCircleWidth + nIndex));
							}

							nAlphaWhite /= 2;
							oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));

							if ( 0 == m_nLeftFactor )
								oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
							else
							{
								if ( 1 == m_nLeftFactor)
									oMainRender->DrawArc(&oPen, float(dXResult + nIndex), nHeight - float(dYResult + dFullHeight / 2 + dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), dAngle, 360 - 2 * dAngle);
								else if ( -1 == m_nLeftFactor)
									oMainRender->DrawArc(&oPen, float(dXResult - nIndex + 2 * dStep), nHeight - float(dYResult + dFullHeight / 2 + dRadius + nIndex), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), 180 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
								oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex),            float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight / 2 - dSrez - nIndex));
								oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight / 2 + dSrez + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
							}

							if ( 0 == m_nBottomFactor )
								oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
							else
							{
								if ( 1 == m_nBottomFactor)
									oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius + nIndex), nHeight - float(dYResult + dFullHeight - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), 90 + dAngle, 360 - 2 * dAngle);
								else if ( -1 == m_nBottomFactor)
									oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius - nIndex), nHeight - float(dYResult + dFullHeight + nIndex - 2 * dStep), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), 270 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
								oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth / 2 + dSrez + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
								oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth / 2 - dSrez - nIndex),    nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dCircleWidth + nIndex),           nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
							}


						}

						// сдвигаем регион и кисть в исходное положение
						oRegion.Translate(float(m_nXCoorSource - dXCoorResult),  - float(m_nYCoorSource - dYCoorResult));
						oBrush->TranslateTransform(float(m_nXCoorSource - dXCoorResult),  - float(m_nYCoorSource - dYCoorResult), MatrixOrderAppend);

						return TRUE;
					}



				public:

					int m_nFigHeight;
					int m_nFigWidth;
					int m_nPicHeight;
					int m_nPicWidth;

					int m_nLeftFactor;   // -1, 0, +1 
					int m_nRightFactor;	 // -1 : вогнутая граница
					int m_nTopFactor;    //  0 : ровная граница
					int m_nBottomFactor; // +1 : выпуклая граница

					BYTE* m_pBGRASource;
					int m_nXCoorSource;		// координаты кусочка, который мы вырезаем из картинки
					int m_nYCoorSource;		//

					
				};


				class CPuzzleUtility
				{
				public:
					DWORD m_dwBaseRnd;
					
					CPuzzleUtility()
					{
						g_pShuffle = 0;
						g_nShuffleCount = -1;

						g_pCorners = 0;
						g_nCornersCount = -1;

					    g_pHorizontalFactor = 0;
					    g_pVerticalFactor   = 0;
					    g_nRow = -1;
					    g_nCol = -1;
						g_nTypeShuffle = -1;
						g_nTypeCorners = -1;
						g_nTypeGenerate = -1;

						m_dwBaseRnd = ::rand();
					}
					~CPuzzleUtility()
					{
						if ( g_pShuffle )
							delete []g_pShuffle;

						if ( g_pCorners )
							delete []g_pCorners;

						if ( g_pHorizontalFactor )
							delete []g_pHorizontalFactor;
						if ( g_pVerticalFactor )
							delete []g_pVerticalFactor;
					}
					
					
					void GenerateRandomShuffle(int nXCount, int nYCount, int nShuffle, int nPuzzleType)
					{
						int nCount = nXCount * nYCount;
						if ( g_pShuffle && nCount == g_nShuffleCount && nPuzzleType == g_nTypeShuffle)
							return;

						if ( g_pShuffle )
							delete []g_pShuffle;

						g_nTypeShuffle = nPuzzleType;
						g_nShuffleCount = nCount;
						g_pShuffle = new int[nCount];

						for(int nIndex = 0; nIndex < nCount; nIndex++)
							g_pShuffle[nIndex] = nIndex;

						if ( 0 == nShuffle )
							return;
						else if ( 1 == nShuffle )
						{
							::srand( m_dwBaseRnd );
							std::random_shuffle(g_pShuffle, g_pShuffle + nCount);
							return;
						}
						else if ( 2 == nShuffle )
						{
							for (int nIndex = 0; nIndex < nCount; nIndex++ )
							{
								g_pShuffle[nIndex] = nCount - 1 - nIndex;
							}
							return;
						}
						else if ( 3 == nShuffle )
						{
							int nYLevel = 0;
							for (int nYIndex = 0; nYIndex < nYCount; nYIndex++, nYLevel = (nYLevel + 1) % 2 )
							{					
								for (int nXIndex = 0; nXIndex < nXCount; nXIndex++ )
								{
									int nIndex = nYIndex * nXCount + nXIndex;
									if ( 0 == nYLevel )
										g_pShuffle[nIndex] = nCount - 1 - nIndex;
									else
										g_pShuffle[nIndex] = nCount - 1 - (nYIndex * nXCount + nXCount - 1 - nXIndex);
								}
							}
						}
						else if ( 4 == nShuffle )
						{
							for (int nYIndex = 0; nYIndex < nYCount; nYIndex++ )
							{					
								for (int nXIndex = 0; nXIndex < nXCount; nXIndex++ )
								{
									int nIndex = nYIndex * nXCount + nXIndex;
									if ( nXIndex % 2 == 0 )
									{
										int nCurX = nXIndex / 2;
										g_pShuffle[nIndex] = nCount - 1 - nYIndex * nXCount - nCurX;
									}
									else
									{
										int nCurX = (nXIndex + 1) / 2;
										g_pShuffle[nIndex] = nCount - 1 - (nYIndex * nXCount  + nXCount - 1 - nCurX);
									}
								}
							}
						}
						return;

					}
					void GenerateCorners(int nWidth, int nHeight, int nCount, int nType, int nPuzzleType)
					{
						if ( g_pCorners && nCount == g_nCornersCount && nPuzzleType == g_nTypeCorners && nHeight == g_nPicHeight && nWidth == g_nPicWidth )
							return;

						if ( g_pCorners )
							delete []g_pCorners;

						g_nPicWidth = nWidth;
						g_nPicHeight = nHeight;

						g_nTypeCorners = nPuzzleType;
						g_nCornersCount = nCount;

						g_pCorners = new int[nCount];
						if ( (0 <= nType  && nType <= 3) || 5 == nType  || 6 == nType || 7 == nType)
						{
							for ( int nIndex = 0; nIndex < nCount; nIndex++)
								g_pCorners[nIndex] = nType;
						}
						else if ( 4 == nType )
						{
							CRandom rnd( m_dwBaseRnd );

							for ( int nIndex = 0; nIndex < nCount; nIndex++)
								g_pCorners[nIndex] = rnd.Rand( 4 );
						}
						return;

					}
					void GeneratePuzzle(int nRow, int nCol, int nPuzzleType)
					{
						if ( g_pHorizontalFactor && g_pVerticalFactor && nRow == g_nRow && nCol == g_nCol && nPuzzleType == g_nTypeGenerate)
							return;

						if ( g_pHorizontalFactor )
							delete []g_pHorizontalFactor;
						if ( g_pVerticalFactor )
							delete []g_pVerticalFactor;

						int nSize = nCol * nRow * 2;

						g_nTypeGenerate = nPuzzleType;
						g_nRow = nRow;
						g_nCol = nCol;

						g_pHorizontalFactor = new int[nSize];
						g_pVerticalFactor = new int[nSize];

						CRandom rnd( m_dwBaseRnd );

						for ( int nYIndex = 0, nIndex = 0; nYIndex < nRow; nYIndex++ )
						{
							for ( int nXIndex = 0; nXIndex < nCol; nXIndex++, nIndex += 2 )
							{
								if ( 1 == nRow )
								{
									g_pHorizontalFactor[nIndex + 0] = 0;
									g_pHorizontalFactor[nIndex + 1] = 0;
									continue;
								}
								if ( 0 == nYIndex )
								{
									g_pHorizontalFactor[nIndex + 0] = 0;
									g_pHorizontalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
								}
								else if ( nRow - 1 == nYIndex )
								{
									g_pHorizontalFactor[nIndex + 1] = 0;
									g_pHorizontalFactor[nIndex + 0] = - g_pHorizontalFactor[nIndex - nCol * 2 + 1];
								}
								else
								{
									g_pHorizontalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
									g_pHorizontalFactor[nIndex + 0] = - g_pHorizontalFactor[nIndex - nCol * 2 + 1];
								}
								
								if ( 1 == nCol )
								{
									g_pVerticalFactor[nIndex + 0] = 0;
									g_pVerticalFactor[nIndex + 1] = 0;
									continue;
								}
								if ( 0 == nXIndex )
								{
									g_pVerticalFactor[nIndex + 0] = 0;
									g_pVerticalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
								}
								else if ( nCol - 1 == nXIndex )
								{
									g_pVerticalFactor[nIndex + 1] = 0;
									g_pVerticalFactor[nIndex + 0] = - g_pVerticalFactor[nIndex - 1];
								}
								else
								{
									g_pVerticalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
									g_pVerticalFactor[nIndex + 0] = - g_pVerticalFactor[nIndex - 1];
								}

							}
						}
						return;
					}



					int GetStartX(int nFigWidth, int nFigHeight, int nXResult, int nYResult, int nIndex, int nCurX)
					{
						int nXStart = 0;

						switch(nIndex)
						{
						case 0: nXStart =             - nFigWidth; break;
						case 1: nXStart = g_nPicWidth + nFigWidth; break; 
						case 2: nXStart = g_nPicWidth + nFigWidth; break; 
						case 3: nXStart =             - nFigWidth; break; 
						case 5: nXStart = nXResult;                break;
						case 6: nXStart = nXResult;                break;
						case 7: 
							{
								if (nCurX % 2)
									nXStart = -nFigWidth;
								else
									nXStart = g_nPicWidth + nFigWidth;
							}
						}

						return nXStart;
					}

					int GetStartY(int nFigWidth, int nFigHeight, int nXResult, int nYResult, int nIndex)
					{
						int nYStart = 0;

						switch(nIndex)
						{
						case 0: nYStart =              - nFigHeight; break;
						case 1: nYStart =              - nFigHeight; break; 
						case 2: nYStart = g_nPicHeight + nFigHeight; break; 
						case 3: nYStart = g_nPicHeight + nFigHeight; break; 
						case 5: nYStart =              - nFigHeight; break;
						case 6: nYStart = g_nPicHeight + nFigHeight; break;
						case 7: nYStart =              - nFigHeight; break;
						}

						return nYStart;
					}
				public:
									
					//для перемешивания кусочков пазла
					int *g_pShuffle;
					int g_nShuffleCount;

					//список углов из которых вылетают кусочки
					int *g_pCorners;
					int g_nCornersCount;

					//списки, определяющие как сами кусочки выглядят
					int *g_pHorizontalFactor;
					int *g_pVerticalFactor;
					int g_nRow;
					int g_nCol;
					int g_nTypeShuffle;
					int g_nTypeCorners;
					int g_nTypeGenerate;

				protected:

					int g_nPicWidth;
					int g_nPicHeight;


				};
				class CFigure
				{
				public:

					CFigure()
					{
						m_dBigRadius   = -1;

						m_nPicHeight = -1;
						m_nPicWidth  = -1;

						m_pBGRASource = 0;

						m_dXCoorSource = -1;
						m_dYCoorSource = -1;
						m_arrPoints = 0;

						m_nFigureType = -1;
						m_nPointsCount = -1;
					}
					~CFigure()
					{
						if ( m_arrPoints )
							delete[] m_arrPoints;
					}

				public:

					void SetValue(int nFigureType, double dSide, BYTE* pBGRASource, int nPicWidth, int nPicHeight, double dXCoorSource, double dYCoorSource)
					{

						m_nPicHeight = nPicHeight;
						m_nPicWidth  = nPicWidth;

						m_pBGRASource = pBGRASource;

						m_dXCoorSource = dXCoorSource;
						m_dYCoorSource = dYCoorSource;

						m_arrPoints = 0;
						m_nPointsCount = -1;

						m_nFigureType = nFigureType;

						// далее заполняем дополнительные данные
						if ( 6 == m_nFigureType )
						{
							m_dBigRadius   = dSide;
							m_dShiftX = m_dBigRadius / 2;
							m_dShiftY = sqrt(3.0) * m_dBigRadius / 2;
						}
						else if ( 30 == m_nFigureType || 31 == m_nFigureType)
						{
							m_dBigRadius = sqrt(3.0) * dSide / 3; // радиус описанной окружности
							m_dShiftY = m_dBigRadius / 2;         // радиус вписанной окружности
							m_dShiftX = dSide / 2;
						}
						else if ( 41 == m_nFigureType )
						{
							m_dBigRadius = dSide / sqrt(2.0);
						}
						else if ( 42 == m_nFigureType )
						{
							m_dShiftX = dSide / 2;
							m_dShiftY = dSide * ( 12.0 / 25.0 ) / 2;
						}
						else if ( 43 == m_nFigureType )
						{
							m_dShiftX = dSide / 2;
							m_dShiftY = dSide * ( 12.0 / 25.0 ) / 2;
						}
					}

					BOOL Draw(Gdiplus::Graphics *oMainRender, Gdiplus::TextureBrush *oBrush, double dXCoorResult, double dYCoorResult, int nWidth, int nHeight, double dEffect)
					{
						if(!oMainRender || !oBrush || nWidth != m_nPicWidth || nHeight != m_nPicHeight)
							return FALSE;
						
						int nSize = 4 * nHeight * nWidth;

						CreatePolygon(m_nFigureType, m_dXCoorSource, m_dYCoorSource);
							
						Gdiplus::GraphicsPath oPath;
						oPath.AddPolygon(m_arrPoints, m_nPointsCount);
						oPath.CloseFigure();

						Gdiplus::Region oRegion(&oPath);
						

						// сдвигаем регион и кисть, так чтобы они двигались вместе
						oBrush->TranslateTransform(-float(m_dXCoorSource - dXCoorResult), -float(m_dYCoorSource - dYCoorResult), MatrixOrderAppend);
						oRegion.Translate(-float(m_dXCoorSource - dXCoorResult), -float(m_dYCoorSource - dYCoorResult));
						
						oMainRender->FillRegion(oBrush, &oRegion);

						if (6 == m_nFigureType)
							DrawHexagonBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
						else if ( 30 == m_nFigureType || 31 == m_nFigureType )
							DrawTriangleBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
						else if ( 41 == m_nFigureType )
							DrawRhombusBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
						else if ( 42 == m_nFigureType )
							DrawBrickBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
						else if ( 43 == m_nFigureType )
							DrawInclinedBrickBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);

						// сдвигаем регион и кисть в исходное положение
						oRegion.Translate(float(m_dXCoorSource - dXCoorResult), float(m_dYCoorSource - dYCoorResult));
						oBrush->TranslateTransform(float(m_dXCoorSource - dXCoorResult), float(m_dYCoorSource - dYCoorResult), MatrixOrderAppend);

						return TRUE;
					}

				protected:

					BOOL CreatePolygon(int nNumber, double dX, double dY)
					{
						if (m_arrPoints)
							delete []m_arrPoints;
						if (6 == nNumber)
						{
							m_nPointsCount = 6;
							m_arrPoints = new Gdiplus::PointF[6];
							int nRadius = m_dBigRadius;
							double dXShift = m_dShiftX;
							double dYShift = m_dShiftY;

							m_arrPoints[0].X = dX - dXShift;
							m_arrPoints[0].Y = dY + dYShift;
							m_arrPoints[1].X = dX + dXShift;
							m_arrPoints[1].Y = dY + dYShift;
							m_arrPoints[2].X = dX + nRadius;
							m_arrPoints[2].Y = dY;
							m_arrPoints[3].X = dX + dXShift;
							m_arrPoints[3].Y = dY - dYShift;

							m_arrPoints[4].X = dX - dXShift;
							m_arrPoints[4].Y = dY - dYShift;
							m_arrPoints[5].X = dX - nRadius;
							m_arrPoints[5].Y = dY;

						}

						else if ( 30 == nNumber )
						{
							m_nPointsCount = 3;
							m_arrPoints = new Gdiplus::PointF[3];
							m_arrPoints[0].X = dX - m_dShiftX;
							m_arrPoints[0].Y = dY + m_dShiftY;

							m_arrPoints[1].X = dX;
							m_arrPoints[1].Y = dY - m_dBigRadius;
							
							m_arrPoints[2].X = dX + m_dShiftX;
							m_arrPoints[2].Y = dY + m_dShiftY;
						}
						else if ( 31 == nNumber )
						{
							m_nPointsCount = 3;
							m_arrPoints = new Gdiplus::PointF[3];
							m_arrPoints[0].X = dX;
							m_arrPoints[0].Y = dY + m_dBigRadius;

							m_arrPoints[1].X = dX - m_dShiftX;
							m_arrPoints[1].Y = dY - m_dShiftY;
							
							m_arrPoints[2].X = dX + m_dShiftX;
							m_arrPoints[2].Y = dY - m_dShiftY;
						}
						else if ( 41 == m_nFigureType )
						{
							m_nPointsCount = 4;
							m_arrPoints = new Gdiplus::PointF[4];

							m_arrPoints[0].X = dX - m_dBigRadius;
							m_arrPoints[0].Y = dY;

							m_arrPoints[1].X = dX;
							m_arrPoints[1].Y = dY + m_dBigRadius;
							
							m_arrPoints[2].X = dX + m_dBigRadius;
							m_arrPoints[2].Y = dY;
							
							m_arrPoints[3].X = dX;
							m_arrPoints[3].Y = dY - m_dBigRadius;
						}
						else if ( 42 == m_nFigureType )
						{
							m_nPointsCount = 4;
							m_arrPoints = new Gdiplus::PointF[4];

							m_arrPoints[0].X = dX - m_dShiftX;
							m_arrPoints[0].Y = dY - m_dShiftY;

							m_arrPoints[1].X = dX + m_dShiftX;
							m_arrPoints[1].Y = dY - m_dShiftY;
							
							m_arrPoints[2].X = dX + m_dShiftX;
							m_arrPoints[2].Y = dY + m_dShiftY;
							
							m_arrPoints[3].X = dX - m_dShiftX;
							m_arrPoints[3].Y = dY + m_dShiftY;
						}
						else if ( 43 == m_nFigureType )
						{
							m_nPointsCount = 4;
							m_arrPoints = new Gdiplus::PointF[4];

							double dCos = cos(-IPP_PI4);
							double dSin = sin(-IPP_PI4);
							
							m_arrPoints[3].X = - m_dShiftX * dCos - (- m_dShiftY) * dSin + dX;
							m_arrPoints[3].Y = - m_dShiftX * dSin + (- m_dShiftY) * dCos + dY;

							m_arrPoints[2].X =   m_dShiftX * dCos - (- m_dShiftY) * dSin + dX;
							m_arrPoints[2].Y =   m_dShiftX * dSin + (- m_dShiftY) * dCos + dY;
							
							m_arrPoints[1].X =   m_dShiftX * dCos - (  m_dShiftY) * dSin + dX;
							m_arrPoints[1].Y =   m_dShiftX * dSin + (  m_dShiftY) * dCos + dY;
							
							m_arrPoints[0].X = - m_dShiftX * dCos - (  m_dShiftY) * dSin + dX;
							m_arrPoints[0].Y = - m_dShiftX * dSin + (  m_dShiftY) * dCos + dY;
						}
						return TRUE;
					}



					BOOL DrawHexagonBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
					{
						Gdiplus::Pen oPen(Color(0, 0, 0), 1);
						CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

						int nAlphaBlack = 448 * dEffect;
						int nAlphaWhite = 448 * dEffect;
						
						for ( int nIndex = 0; nIndex < 5; nIndex++ )
						{
							nAlphaBlack /= 2; 
							if (nIndex == 1)
							{
								nAlphaBlack = 224 * dEffect;
								nAlphaWhite = 224 * dEffect;
							}
							nAlphaWhite /= 2;
							//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
							oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
							oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
							//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[4]);
							oMainRender->DrawLine(&oPen, m_arrPoints[4], m_arrPoints[5]);
							oMainRender->DrawLine(&oPen, m_arrPoints[5], m_arrPoints[0]);

							m_arrPoints[0].X++;
							m_arrPoints[0].Y--;

							m_arrPoints[1].X--;
							m_arrPoints[1].Y--;

							m_arrPoints[2].X--;

							m_arrPoints[3].X--;
							m_arrPoints[3].Y++;

							m_arrPoints[4].X++;
							m_arrPoints[4].Y++;

							m_arrPoints[5].X++;

						}
						return TRUE;
					}
					BOOL DrawTriangleBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
					{
						if (30 == m_nFigureType )
						{
							Gdiplus::Pen oPen(Color(0, 0, 0), 1);
							CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

							int nAlphaBlack = 448 * dEffect;
							int nAlphaWhite = 448 * dEffect;
							
							for ( int nIndex = 0; nIndex < 5; nIndex++ )
							{
								nAlphaBlack /= 2; 
								if (nIndex == 1 || nIndex == 2)
								{
									nAlphaBlack = 224 * dEffect;
									nAlphaWhite = 224 * dEffect;
								}
								nAlphaWhite /= 2;
								if(nIndex == 0)
								{
									nAlphaWhite = 128;
									nAlphaBlack = 128;
								}
								oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
								oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
								oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
								oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
								oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[0]);

								m_arrPoints[0].X ++;
								m_arrPoints[0].Y -= float(0.4);

								m_arrPoints[1].Y++;

								m_arrPoints[2].X --;
								m_arrPoints[2].Y -= float(0.4);


							}
						}
						else if ( 31 == m_nFigureType)
						{
							Gdiplus::Pen oPen(Color(0, 0, 0), 1);
							CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

							int nAlphaBlack = 448 * dEffect;
							int nAlphaWhite = 448 * dEffect;
							
							for ( int nIndex = 0; nIndex < 5; nIndex++ )
							{
								nAlphaBlack /= 2; 
								if (nIndex == 1 || nIndex == 2)
								{
									nAlphaBlack = 224 * dEffect;
									nAlphaWhite = 224 * dEffect;
								}
								nAlphaWhite /= 2;
								if(nIndex == 0)
								{
									nAlphaWhite = 128;
									nAlphaBlack = 128;
								}
								oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
								oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
								oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
								oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
								oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[0]);

								m_arrPoints[0].Y--;

								m_arrPoints[1].X ++;
								m_arrPoints[1].Y += float(0.4);

								m_arrPoints[2].X --;
								m_arrPoints[2].Y += float(0.4);


							}
						}
						return TRUE;
					}

					BOOL DrawRhombusBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
					{
						Gdiplus::Pen oPen(Color(0, 0, 0), 1);
						CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

						int nAlphaBlack = 448 * dEffect;
						int nAlphaWhite = 448 * dEffect;
						
						for ( int nIndex = 0; nIndex < 5; nIndex++ )
						{
							nAlphaBlack /= 2; 
							if (nIndex == 1)
							{
								nAlphaBlack = 224 * dEffect;
								nAlphaWhite = 224 * dEffect;
							}
							nAlphaWhite /= 2;
							//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
							oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
							//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
							oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

							m_arrPoints[0].X++;
							m_arrPoints[1].Y--;
							m_arrPoints[2].X--;
							m_arrPoints[3].Y++;

						}
						return TRUE;
					}

					BOOL DrawBrickBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
					{
						Gdiplus::Pen oPen(Color(0, 0, 0), 1);
						CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

						int nAlphaBlack = 448 * dEffect;
						int nAlphaWhite = 448 * dEffect;
						
						for ( int nIndex = 0; nIndex < 5; nIndex++ )
						{
							nAlphaBlack /= 2; 
							if (nIndex == 1)
							{
								nAlphaBlack = 224 * dEffect;
								nAlphaWhite = 224 * dEffect;
							}
							nAlphaWhite /= 2;
							//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
							oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
							//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
							oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

							m_arrPoints[0].X++;
							m_arrPoints[0].Y++;

							m_arrPoints[1].X--;
							m_arrPoints[1].Y++;
							
							m_arrPoints[2].X--;
							m_arrPoints[2].Y--;
							
							m_arrPoints[3].X++;
							m_arrPoints[3].Y--;

						}
						return TRUE;
					}
					BOOL DrawInclinedBrickBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
					{
						Gdiplus::Pen oPen(Color(0, 0, 0), 1);
						CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

						int nAlphaBlack = 448 * dEffect;
						int nAlphaWhite = 448 * dEffect;
						
						for ( int nIndex = 0; nIndex < 5; nIndex++ )
						{
							nAlphaBlack /= 2; 
							if (nIndex == 1)
							{
								nAlphaBlack = 224 * dEffect;
								nAlphaWhite = 224 * dEffect;
							}
							nAlphaWhite /= 2;
							//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
							oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
							//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
							oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

							m_arrPoints[1].X--;
							m_arrPoints[2].Y++;
							m_arrPoints[3].X++;
							m_arrPoints[0].Y--;

						}
						return TRUE;
					}
					
				public:
					double m_dBigRadius;

					int	m_nPicHeight;
					int	m_nPicWidth;

					BYTE* m_pBGRASource;

					double m_dXCoorSource;
					double m_dYCoorSource;

					int m_nFigureType; 
					// 30, 31 - правильный треугольник (смотрящий вниз или вверх соответсвенно)
					// 6 - правильный шестиугольник
					// 41 - квадрат, повернутый на 45 градусов
					// 42 - прямоугольник в форме кирпича(соотношение сторон 25:12)
					// 43 - крипич под углом 45 градусов

				protected:

					double m_dShiftY;  // вспомогательные параметры, обычно используются для нахождения
					double m_dShiftX;  // координат вершин, относительно координат центра

					Gdiplus::PointF* m_arrPoints;
					int              m_nPointsCount;

				};



			}
		}
	}
}

#pragma warning(default: 4244)
