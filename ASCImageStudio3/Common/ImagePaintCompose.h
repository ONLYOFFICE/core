#pragma once

#include <ImagePaintComposeBGRA.h>

namespace ImageStudio
{
	namespace Paint
	{
		namespace Compose
		{
			class CAlphaComposer
			{
			public:
				
				CAlphaComposer()
				{
					m_pTableR = NULL;
					m_pTableG = NULL;
					m_pTableB = NULL;

					m_pBGRACopy = NULL;
				}
				virtual ~CAlphaComposer()
				{
					TransformEnd();
				}
				
				BOOL Transform(BYTE* pBGRA, int nWidth, int nHeight, int nColor, int nAlpha, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness)
				{
					if (!TransformBegin(pBGRA, nWidth, nHeight, nColor, nAlpha, nTileSizeX, nTileSizeY))
						return FALSE;

					// теперь, в зависимости от типа смешения применяем соответствующий эффект
					if (nType >= Constants::c_nAlphaComposeWipeDoorVerticalIn && nType <= Constants::c_nAlphaComposeWipeDoorDiagonalRotated)
						return WipeDoor(dCompleteness, nType);

					if (nType >= Constants::c_nAlphaComposeWipeSideTypeLeftToRight && nType <= Constants::c_nAlphaComposeWipeSideTypeRightTopToLeftBottom)
						return WipeSide(dCompleteness, nType);

					else if (nType >= Constants::c_nAlphaComposeWipeCenterTypeBoxIn && nType <= Constants::c_nAlphaComposeWipeCenterTypeCrossOut)
						return WipeCenter(dCompleteness, nType);

					else if (nType >= Constants::c_nAlphaComposeWipeStripsTypeVertical && nType <= Constants::c_nAlphaComposeWipeStripsTypeHorizontal)
						return WipeStrips(dCompleteness, nType);

					else if (nType >= Constants::c_nAlphaComposeMosaicTypeSpiralIn && nType <= Constants::c_nAlphaComposeMosaicTypeChessBottomToTop)
						return WipeMosaic(dCompleteness, nType);

					else if (nType >= Constants::c_nAlphaComposePushDoorVerticalIn && nType <= Constants::c_nAlphaComposePushDoorHorizontalOut)
						return PushDoor(dCompleteness, nType);

					else if (nType >= Constants::c_nAlphaComposePushSideTypeLeftToRight && nType <= Constants::c_nAlphaComposePushSideTypeBottomToTop)
						return PushSide(dCompleteness, nType);

					else if (nType >= Constants::c_nAlphaComposePushStripsTypeVertical && nType <= Constants::c_nAlphaComposePushStripsTypeHorizontal)
						return PushStrips(dCompleteness, nType);

					TransformEnd();

					return FALSE;
				}
				
			protected:

				BOOL CreateTransformTable(int nColor, int nAlpha, BYTE*& pTable)
				{
					/* @desc: 
					BOOL CreateTransformTable(int nColor, int nAlpha, BYTE*& pTable)
					{
						создает таблицу преобразования цветов для смешивания с заданным цветом (nColor) и прозрачностью (nAlpha)

						nColor - цвет, с которым будет происходить смешение; изменяется в пределах от 0 до 255
						nAlpha - прозрачность, с которой будут смешиваться цвета; изменяется в пределах от 0 до 255
					}
					*/

					pTable = new BYTE[256];

					if (NULL == pTable)
						return FALSE;

					int nNewValue = 0;
					BYTE* pTableValue = pTable;

					double dAlpha1 = nAlpha/255.0;
					double dAlpha2 = 1.0 - dAlpha1;
					double dColor = dAlpha1*nColor;

					for (int nIndex = 0; nIndex < 256; ++nIndex, ++pTableValue)
					{
						nNewValue = (int)(dAlpha2*nIndex + dColor);

						if (nNewValue < 0)
							*pTableValue = 0;
						else if (nNewValue > 255)
							*pTableValue = 255;
						else
							*pTableValue = (BYTE)nNewValue;
					}

					return TRUE;
				}
				BOOL CreateBackup()
				{
					if (NULL == m_pBGRA)
						return FALSE;

					if (NULL != m_pBGRACopy)
					{
						delete[] m_pBGRACopy;
						m_pBGRACopy = NULL;
					}

					m_pBGRACopy = new BYTE[4*m_nWidth*m_nHeight];

					if (NULL == m_pBGRACopy)
						return FALSE;


					memcpy(m_pBGRACopy, m_pBGRA, 4*m_nWidth*m_nHeight);

					return TRUE;
				}
				BOOL TransformBegin(BYTE* pBGRA, int nWidth, int nHeight, int nColor, int nAlpha, int nTileSizeX, int nTileSizeY)
				{
					BOOL bSuccess = TRUE;

					// вначале сохраняем указатели на память
					m_pBGRA = pBGRA;
					m_nWidth = nWidth;
					m_nHeight = nHeight;

					// подсчитываем размер тайла, в котором будет применяться эффект
					m_nTileSizeX = (nTileSizeX < 2) ? m_nWidth : nTileSizeX;
					m_nTileSizeY = (nTileSizeY < 2) ? m_nHeight : nTileSizeY;

					// создаем Таблицы, для быстрого смешения цветов
					if (!CreateTransformTable(GetRValue(nColor), nAlpha, m_pTableR))
						bSuccess = FALSE;
					if (!CreateTransformTable(GetGValue(nColor), nAlpha, m_pTableG))
						bSuccess = FALSE;
					if (!CreateTransformTable(GetBValue(nColor), nAlpha, m_pTableB))
						bSuccess = FALSE;

					// если что не так - удаляем все нафиг
					if (!bSuccess)
						TransformEnd();

					return bSuccess;
				}
				BOOL TransformEnd()
				{
					if (NULL != m_pTableR)
					{
						delete[] m_pTableR;
						m_pTableR = NULL;
					}
					if (NULL != m_pTableG)
					{
						delete[] m_pTableG;
						m_pTableG = NULL;
					}
					if (NULL != m_pTableB)
					{
						delete[] m_pTableB;
						m_pTableB = NULL;
					}
					if (NULL != m_pBGRACopy)
					{
						delete[] m_pBGRACopy;
						m_pBGRACopy = NULL;
					}

					return TRUE;
				}
				
				BOOL WipeDoor(double dCompleteness, int nType)
				{
					int nLevelX = (int)((m_nTileSizeX - 1)*dCompleteness);
					int nLevelY = (int)((m_nTileSizeY - 1)*dCompleteness);
					int nLevel1X = (int)(0.5*(m_nTileSizeX - 1)*dCompleteness);
					int nLevel1Y = (int)(0.5*(m_nTileSizeY - 1)*dCompleteness);
					int nLevel2X = (int)(0.5*(m_nTileSizeX - 1)*(1 - dCompleteness));
					int nLevel2Y = (int)(0.5*(m_nTileSizeY - 1)*(1 - dCompleteness));

					int nX, nY, nIndex = 0;
					int nPatternX, nPatternY, nPatternValue;

					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = nY % m_nTileSizeY;

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = nX % m_nTileSizeX;

							nPatternValue = 0; // 0 - означает значение из source картинки - его менять не надо

							if (nType == Constants::c_nAlphaComposeWipeDoorVerticalIn)
							{
								if (nPatternY <= nLevel1Y || nPatternY >= (m_nTileSizeY - 1) - nLevel1Y)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeDoorVerticalOut)
							{
								if (nPatternY >= nLevel2Y && nPatternY <= (m_nTileSizeY - 1) - nLevel2Y)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeDoorHorizontalIn)
							{
								if (nPatternX <= nLevel1X || nPatternX >= (m_nTileSizeX - 1) - nLevel1X)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeDoorHorizontalOut)
							{
								if (nPatternX >= nLevel2X && nPatternX <= (m_nTileSizeX - 1) - nLevel2X)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeDoorDiagonal)
							{
								if ((nPatternY <= nLevelY && nPatternX <= nLevelX) || (nPatternY >= (m_nTileSizeY - 1) - nLevelY && nPatternX >= (m_nTileSizeX - 1) - nLevelX))
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeDoorDiagonalRotated)
							{
								if ((nPatternY >= (m_nTileSizeY - 1) - nLevelY && nPatternX <= nLevelX) || (nPatternY <= nLevelY && nPatternX >= (m_nTileSizeX - 1) - nLevelX))
									nPatternValue = 1;
							}

							if (nPatternValue == 1)
							{
								m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRA[nIndex + 0]];
								m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRA[nIndex + 1]];
								m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRA[nIndex + 2]];
							}
						}
					}

					return TRUE;
				}
				BOOL WipeSide(double dCompleteness, int nType)
				{
					int nLevelX = (int)((m_nTileSizeX - 1)*dCompleteness);
					int nLevelY = (int)((m_nTileSizeY - 1)*dCompleteness);

					int nX, nY, nIndex = 0;
					int nPatternX, nPatternY, nPatternValue;
					
					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = nY % m_nTileSizeY;

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = nX % m_nTileSizeX;

							nPatternValue = 0; // 0 - означает значение из source картинки - его менять не надо

							if (nType == Constants::c_nAlphaComposeWipeSideTypeLeftToRight)
							{
								if (nPatternX <= nLevelX)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeSideTypeRightToLeft)
							{
								if (nPatternX >= (m_nTileSizeX - 1) - nLevelX)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeSideTypeBottomToTop)
							{
								if (nPatternY <= nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeSideTypeTopToBottom)
							{
								if (nPatternY >= (m_nTileSizeY - 1) - nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeSideTypeLeftTopToRightBottom)
							{
								if (nPatternX <= nLevelX && nPatternY >= (m_nTileSizeY - 1) - nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeSideTypeLeftBottomToRightTop)
							{
								if (nPatternX <= nLevelX && nPatternY <= nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeSideTypeRightBottomToLeftTop)
							{
								if (nPatternX >= (m_nTileSizeX - 1) - nLevelX && nPatternY <= nLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeSideTypeRightTopToLeftBottom)
							{
								if (nPatternX >= (m_nTileSizeX - 1) - nLevelX && nPatternY >= (m_nTileSizeY - 1) - nLevelY)
									nPatternValue = 1;
							}
							
							if (nPatternValue == 1)
							{
								m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRA[nIndex + 0]];
								m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRA[nIndex + 1]];
								m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRA[nIndex + 2]];
							}
						}
					}

					return TRUE;
				}
				BOOL WipeCenter(double dCompleteness, int nType)
				{
					double dCenterX = 0.5*(m_nTileSizeX - 1);
					double dCenterY = 0.5*(m_nTileSizeY - 1);

					double dLevelX = 0.5*m_nTileSizeX*dCompleteness;
					double dLevelY = 0.5*m_nTileSizeY*dCompleteness;

					double dRadius;
					double dRadiusMin = 0.25*(m_nTileSizeX*m_nTileSizeX + m_nTileSizeY*m_nTileSizeY)*dCompleteness;
					double dRadiusMax = 0.25*(m_nTileSizeX*m_nTileSizeX + m_nTileSizeY*m_nTileSizeY)*(1 - dCompleteness);

					double dDiamond;
					double dDiamondMin = 0.5*(m_nTileSizeX + m_nTileSizeY)*dCompleteness;
					double dDiamondMax = 0.5*(m_nTileSizeX + m_nTileSizeY)*(1 - dCompleteness);

					double dCross1X = 0.5*(m_nTileSizeX - 1)*dCompleteness;
					double dCross1Y = 0.5*(m_nTileSizeY - 1)*dCompleteness;
					double dCross2X = 0.5*(m_nTileSizeX - 1)*(1 - dCompleteness);
					double dCross2Y = 0.5*(m_nTileSizeY - 1)*(1 - dCompleteness);

					int nX, nY, nIndex = 0;
					int nPatternX, nPatternY, nPatternValue;

					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = nY % m_nTileSizeY;

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = nX % m_nTileSizeX;

							nPatternValue = 0; // 0 - означает значение из source картинки - его менять не надо

							if (nType == Constants::c_nAlphaComposeWipeCenterTypeBoxIn)
							{
								if (nPatternX < dLevelX || nPatternX > (m_nTileSizeX - 1) - dLevelX || nPatternY < dLevelY || nPatternY > (m_nTileSizeY - 1) - dLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeCenterTypeBoxOut)
							{
								if (nPatternX > dCenterX - dLevelX && nPatternX < dCenterX + dLevelX && 
									nPatternY > dCenterY - dLevelY && nPatternY < dCenterY + dLevelY)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeCenterTypeCircleIn)
							{
								dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
								if (dRadius > dRadiusMax)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeCenterTypeCircleOut)
							{
								dRadius = (nPatternX - dCenterX)*(nPatternX - dCenterX) + (nPatternY - dCenterY)*(nPatternY - dCenterY);
								if (dRadius < dRadiusMin)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeCenterTypeDiamondIn)
							{
								dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
								if (dDiamond > dDiamondMax)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeCenterTypeDiamondOut)
							{
								dDiamond = fabs(nPatternX - dCenterX) + fabs(nPatternY - dCenterY);
								if (dDiamond < dDiamondMin)
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeCenterTypeCrossIn)
							{
								if ((nPatternY <= dCross1Y && nPatternX <= dCross1X) ||
									(nPatternY <= dCross1Y && nPatternX >= (m_nTileSizeX - 1) - dCross1X) ||
									(nPatternY >= (m_nTileSizeY - 1) - dCross1Y && nPatternX >= (m_nTileSizeX - 1) - dCross1X) ||
									(nPatternY >= (m_nTileSizeY - 1) - dCross1Y && nPatternX <= dCross1X))
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeCenterTypeCrossOut)
							{
								if ((nPatternY >= dCross2Y && nPatternY <= (m_nTileSizeY - 1) - dCross2Y) ||
									(nPatternX >= dCross2X && nPatternX <= (m_nTileSizeX - 1) - dCross2X))
									nPatternValue = 1;
							}

							if (nPatternValue == 1)
							{
								m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRA[nIndex + 0]];
								m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRA[nIndex + 1]];
								m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRA[nIndex + 2]];
							}
						}
					}

					// all ok
					return TRUE;
				}
				BOOL WipeStrips(double dCompleteness, int nType)
				{
					double dCenterX = 0.5*(m_nTileSizeX - 1);
					double dCenterY = 0.5*(m_nTileSizeY - 1);
					double dLevelX = m_nTileSizeX*dCompleteness;
					double dLevelY = m_nTileSizeY*dCompleteness;

					int nX, nY, nIndex = 0;
					int nPatternX, nPatternY, nPatternValue;

					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = nY % m_nTileSizeY;

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = nX % m_nTileSizeX;

							nPatternValue = 0; // 0 - означает значение из source картинки - его менять не надо

							if (nType == Constants::c_nAlphaComposeWipeStripsTypeVertical)
							{
								if ((nPatternX < dCenterX && nPatternY < dLevelY) ||
									(nPatternX >= dCenterX && nPatternY > (m_nTileSizeY - 1) - dLevelY))
									nPatternValue = 1;
							}
							else if (nType == Constants::c_nAlphaComposeWipeStripsTypeHorizontal)
							{
								if ((nPatternY < dCenterY && nPatternX < dLevelX) ||
									(nPatternY >= dCenterY && nPatternX > (m_nTileSizeX - 1) - dLevelX))
									nPatternValue = 1;
							}

							if (nPatternValue == 1)
							{
								m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRA[nIndex + 0]];
								m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRA[nIndex + 1]];
								m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRA[nIndex + 2]];
							}
						}
					}

					return TRUE;
				}
				BOOL WipeMosaic(double dCompleteness, int nType)
				{
					int nLevel = (int)((8*8 - 1)*dCompleteness);

					int nX, nY, nIndex = 0;
					int nPatternX, nPatternY, nPatternValue;

					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = (int)(8.0*(nY % m_nTileSizeY)/(double)(m_nTileSizeY - 1));
						nPatternY = min(nPatternY, 7);

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = (int)(8.0*(nX % m_nTileSizeX)/(double)(m_nTileSizeX - 1));
							nPatternX = min(nPatternX, 7);

							nPatternValue = 0;

							if (nType == Constants::c_nAlphaComposeMosaicTypeSpiralIn)				nPatternValue = Constants::c_byMosaicSpiralIn				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeSpiralOut)				nPatternValue = Constants::c_byMosaicSpiralOut				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeStrips)				nPatternValue = Constants::c_byMosaicStrips					[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeDissolve)				nPatternValue = Constants::c_byMosaicDissolve				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeLeftTopToRightBottom)	nPatternValue = Constants::c_byMosaicDiagonalLeftTop		[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeRightTopToLeftBottom)	nPatternValue = Constants::c_byMosaicDiagonalRightTop		[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeRightBottomToLeftTop)	nPatternValue = Constants::c_byMosaicDiagonalRightBottom	[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeLeftBottomToRightTop)	nPatternValue = Constants::c_byMosaicDiagonalLeftBottom		[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeWallLeftToRight)		nPatternValue = Constants::c_byMosaicWallLeft				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeWallRightToLeft)		nPatternValue = Constants::c_byMosaicWallRight				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeWallTopToBottom)		nPatternValue = Constants::c_byMosaicWallTop				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeWallBottomToTop)		nPatternValue = Constants::c_byMosaicWallBottom				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeChessLeftToRight)		nPatternValue = Constants::c_byMosaicChessLeft				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeChessRightToLeft)		nPatternValue = Constants::c_byMosaicChessRight				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeChessTopToBottom)		nPatternValue = Constants::c_byMosaicChessTop				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeChessBottomToTop)		nPatternValue = Constants::c_byMosaicChessBottom			[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeClockwise)				nPatternValue = Constants::c_byMosaicClockwise				[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeCounterClockwise)		nPatternValue = Constants::c_byMosaicCounterClockwise		[nPatternY][nPatternX];
							if (nType == Constants::c_nAlphaComposeMosaicTypeRandom)				nPatternValue = Constants::c_byMosaicRandom					[nPatternY][nPatternX];

							if (nPatternValue < nLevel)
							{
								m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRA[nIndex + 0]];
								m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRA[nIndex + 1]];
								m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRA[nIndex + 2]];
							}
						}
					}

					return TRUE;
				}
				BOOL PushDoor(double dCompleteness, int nType)
				{
					if (!CreateBackup())
						return FALSE;

					int nLevelX = (int)((m_nTileSizeX - 1)*dCompleteness);
					int nLevelY = (int)((m_nTileSizeY - 1)*dCompleteness);
					int nLevel1X = (int)(0.5*(m_nTileSizeX - 1)*dCompleteness);
					int nLevel1Y = (int)(0.5*(m_nTileSizeY - 1)*dCompleteness);
					int nLevel2X = (int)(0.5*(m_nTileSizeX - 1)*(1 - dCompleteness));
					int nLevel2Y = (int)(0.5*(m_nTileSizeY - 1)*(1 - dCompleteness));

					int nX, nY, nIndex = 0;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;

					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = nY % m_nTileSizeY;

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = nX % m_nTileSizeX;

							nPatternValue = 0;

							nSourceX = nX;
							nSourceY = nY;

							if (nType == Constants::c_nAlphaComposePushDoorVerticalIn)
							{
								if (nPatternY <= nLevel1Y)
								{
									nSourceY = nY + (m_nTileSizeY - 1)/2 - nLevel1Y;
									nPatternValue = 1;
								}
								else if (nPatternY <= (m_nTileSizeY - 1)/2)
								{
									nSourceY = nY - nLevel1Y;
									nPatternValue = 0;
								}
								else if (nPatternY >= (m_nTileSizeY - 1) - nLevel1Y)
								{
									nSourceY = nY + nLevel1Y - (m_nTileSizeY - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternY >= (m_nTileSizeY - 1)/2)
								{
									nSourceY = nY + nLevel1Y;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nAlphaComposePushDoorVerticalOut)
							{
								if (nPatternY <= nLevel2Y)
								{
									nSourceY = nY + (m_nTileSizeY - 1)/2 - nLevel2Y;
									nPatternValue = 0;
								}
								else if (nPatternY >= (m_nTileSizeY - 1) - nLevel2Y)
								{
									nSourceY = nY + nLevel2Y - (m_nTileSizeY - 1)/2;
									nPatternValue = 0;
								}
								else if (nPatternY >= nLevel2Y && nPatternY <= (m_nTileSizeY - 1)/2)
								{
									nSourceY = nY - nLevel2Y;
									nPatternValue = 1;
								}
								else if (nPatternY <= (m_nTileSizeY - 1) - nLevel2Y && nPatternY >= (m_nTileSizeY - 1)/2)
								{
									nSourceY = nY + nLevel2Y;
									nPatternValue = 1;
								}
							}
							else if (nType == Constants::c_nAlphaComposePushDoorHorizontalIn)
							{
								if (nPatternX <= nLevel1X)
								{
									nSourceX = nX + (m_nTileSizeX - 1)/2 - nLevel1X;
									nPatternValue = 1;
								}
								else if (nPatternX <= (m_nTileSizeX - 1)/2)
								{
									nSourceX = nX - nLevel1X;
									nPatternValue = 0;
								}
								else if (nPatternX >= (m_nTileSizeX - 1) - nLevel1X)
								{
									nSourceX = nX + nLevel1X - (m_nTileSizeX - 1)/2;
									nPatternValue = 1;
								}
								else if (nPatternX >= (m_nTileSizeX - 1)/2)
								{
									nSourceX = nX + nLevel1X;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nAlphaComposePushDoorHorizontalOut)
							{
								if (nPatternX <= nLevel2X)
								{
									nSourceX = nX + (m_nTileSizeX - 1)/2 - nLevel2X;
									nPatternValue = 0;
								}
								else if (nPatternX >= (m_nTileSizeX - 1) - nLevel2X)
								{
									nSourceX = nX + nLevel2X - (m_nTileSizeX - 1)/2;
									nPatternValue = 0;
								}
								else if (nPatternX >= nLevel2X && nPatternX <= (m_nTileSizeX - 1)/2)
								{
									nSourceX = nX - nLevel2X;
									nPatternValue = 1;
								}
								else if (nPatternX <= (m_nTileSizeX - 1) - nLevel2X && nPatternX >= (m_nTileSizeX - 1)/2)
								{
									nSourceX = nX + nLevel2X;
									nPatternValue = 1;
								}
							}

							if (nSourceX >= 0 && nSourceX < m_nWidth && nSourceY >= 0 && nSourceY < m_nHeight)
							{
								nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

								if (nPatternValue == 0)
								{
									m_pBGRA[nIndex + 0] = m_pBGRACopy[nSourceIndex + 0];
									m_pBGRA[nIndex + 1] = m_pBGRACopy[nSourceIndex + 1];
									m_pBGRA[nIndex + 2] = m_pBGRACopy[nSourceIndex + 2];
								}
								else
								{
									m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRACopy[nSourceIndex + 0]];
									m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRACopy[nSourceIndex + 1]];
									m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRACopy[nSourceIndex + 2]];
								}
							}
						}
					}

					return TRUE;
				}
				BOOL PushSide(double dCompleteness, int nType)
				{
					if (!CreateBackup())
						return FALSE;

					int nLevelX = (int)((m_nTileSizeX - 1)*dCompleteness);
					int nLevelY = (int)((m_nTileSizeY - 1)*dCompleteness);

					int nX, nY, nIndex = 0;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;

					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = nY % m_nTileSizeY;

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = nX % m_nTileSizeX;

							nPatternValue = 0;

							nSourceX = nX;
							nSourceY = nY;

							if (nType == Constants::c_nAlphaComposePushSideTypeLeftToRight)
							{
								if (nPatternX <= nLevelX)
								{
									nSourceX = nX + (m_nTileSizeX - 1) - nLevelX;
									nPatternValue = 1;
								}
								else
								{
									nSourceX = nX - nLevelX;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nAlphaComposePushSideTypeRightToLeft)
							{
								if (nPatternX >= (m_nTileSizeX - 1) - nLevelX)
								{
									nSourceX = nX + nLevelX - (m_nTileSizeX - 1);
									nPatternValue = 1;
								}
								else
								{
									nSourceX = nX + nLevelX;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nAlphaComposePushSideTypeBottomToTop)
							{
								if (nPatternY <= nLevelY)
								{
									nSourceY = nY + (m_nTileSizeY - 1) - nLevelY;
									nPatternValue = 1;
								}
								else
								{
									nSourceY = nY - nLevelY;
									nPatternValue = 0;
								}
							}
							else if (nType == Constants::c_nAlphaComposePushSideTypeTopToBottom)
							{
								if (nPatternY >= (m_nTileSizeY - 1) - nLevelY)
								{
									nSourceY = nY + nLevelY - (m_nTileSizeY - 1);
									nPatternValue = 1;
								}
								else
								{
									nSourceY = nY + nLevelY;
									nPatternValue = 0;
								}
							}

							if (nSourceX >= 0 && nSourceX < m_nWidth && nSourceY >= 0 && nSourceY < m_nHeight)
							{
								nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

								if (nPatternValue == 0)
								{
									m_pBGRA[nIndex + 0] = m_pBGRACopy[nSourceIndex + 0];
									m_pBGRA[nIndex + 1] = m_pBGRACopy[nSourceIndex + 1];
									m_pBGRA[nIndex + 2] = m_pBGRACopy[nSourceIndex + 2];
								}
								else
								{
									m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRACopy[nSourceIndex + 0]];
									m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRACopy[nSourceIndex + 1]];
									m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRACopy[nSourceIndex + 2]];
								}
							}
						}
					}

					return TRUE;
				}
				BOOL PushStrips(double dCompleteness, int nType)
				{
					if (!CreateBackup())
						return FALSE;

					int nCenterX = (m_nTileSizeX - 1)/2;
					int nCenterY = (m_nTileSizeY - 1)/2;
					int nLevelX = (int)((m_nTileSizeX - 1)*dCompleteness);
					int nLevelY = (int)((m_nTileSizeY - 1)*dCompleteness);

					int nX, nY, nIndex = 0;
					int nSourceX, nSourceY, nSourceIndex;
					int nPatternX, nPatternY, nPatternValue;

					for (nY = 0; nY < m_nHeight; ++nY)
					{
						nPatternY = nY % m_nTileSizeY;

						for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
						{
							nPatternX = nX % m_nTileSizeX;

							nPatternValue = 0;

							nSourceX = nX;
							nSourceY = nY;

							if (nType == Constants::c_nAlphaComposePushStripsTypeVertical)
							{
								if (nPatternX <= nCenterX)
								{
									if (nPatternY <= nLevelY)
									{
										nSourceY = nY + (m_nTileSizeY - 1) - nLevelY;
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
									if (nPatternY >= (m_nTileSizeY - 1) - nLevelY)
									{
										nSourceY = nY + nLevelY - (m_nTileSizeY - 1);
										nPatternValue = 1;
									}
									else
									{
										nSourceY = nY + nLevelY;
										nPatternValue = 0;
									}
								}
							}
							else if (nType == Constants::c_nAlphaComposePushStripsTypeHorizontal)
							{
								if (nPatternY <= nCenterY)
								{
									if (nPatternX <= nLevelX)
									{
										nSourceX = nX + (m_nTileSizeX - 1) - nLevelX;
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
									if (nPatternX >= (m_nTileSizeX - 1) - nLevelX)
									{
										nSourceX = nX + nLevelX - (m_nTileSizeX - 1);
										nPatternValue = 1;
									}
									else
									{
										nSourceX = nX + nLevelX;
										nPatternValue = 0;
									}
								}
							}

							if (nSourceX >= 0 && nSourceX < m_nWidth && nSourceY >= 0 && nSourceY < m_nHeight)
							{
								nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

								if (nPatternValue == 0)
								{
									m_pBGRA[nIndex + 0] = m_pBGRACopy[nSourceIndex + 0];
									m_pBGRA[nIndex + 1] = m_pBGRACopy[nSourceIndex + 1];
									m_pBGRA[nIndex + 2] = m_pBGRACopy[nSourceIndex + 2];
								}
								else
								{
									m_pBGRA[nIndex + 0] = m_pTableB[m_pBGRACopy[nSourceIndex + 0]];
									m_pBGRA[nIndex + 1] = m_pTableG[m_pBGRACopy[nSourceIndex + 1]];
									m_pBGRA[nIndex + 2] = m_pTableR[m_pBGRACopy[nSourceIndex + 2]];
								}
							}
						}
					}

					return TRUE;
				}
				
			protected:
				
				// таблицы для быстрого смешения цветов (для каждой цветовой компоненты - своя таблицы)
				BYTE* m_pTableR;
				BYTE* m_pTableG;
				BYTE* m_pTableB;

				// картинка, для обработки
				BYTE* m_pBGRA; // сохраненный указатель - не создаем его и не удаляем
				int m_nWidth;
				int m_nHeight;

				// настройки трансформа
				int m_nTileSizeX;
				int m_nTileSizeY;
				
				// копия картинки, пришедшей на вход
				BYTE* m_pBGRACopy;
			};
			class CImageComposer
			{
			public:
				DWORD m_dwBaseRnd;

			public:

				CImageComposer()
				{
					m_dwBaseRnd = ::rand();
				}
				virtual ~CImageComposer()
				{
				}
				
				BOOL Transform( BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness )
				{
					m_oUtility      .m_dwBaseRnd = m_dwBaseRnd;
					m_oMask         .m_dwBaseRnd = m_dwBaseRnd;
					m_oPuzzleUtility.m_dwBaseRnd = m_dwBaseRnd;
					m_oMaskGraduate .m_dwBaseRnd = m_dwBaseRnd;
					
					if (nWidth < 1 || nHeight < 1)
						return FALSE;
					
					if (NULL == pBGRASource1 || NULL == pBGRASource2 || NULL == pBGRAResult)
						return FALSE;

					// если размеры тайла не указаны - подбираем лучшее значение для данного эффекта
					if (nTileSizeX <= 0 || nTileSizeY <= 0)
						ComputeDefaultTileSize(nType, nWidth, nHeight, nTileSizeX, nTileSizeY);

					if (nType == Constants::c_nSimpleCopyFirst || nType == Constants::c_nSimpleCopyLast)
						return BGRA::BGRA_SimpleCopy(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nFadeSolid && nType <= Constants::c_nFadePatternCross3)
						return BGRA::BGRA_Fade(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nMosaicSpiralIn && nType <= Constants::c_nMosaicChessBottomToTop)
						return BGRA::BGRA_Mosaic(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nDiffuseLeftToRight && nType <= Constants::c_nDiffuseBottomToTop)
						return BGRA::BGRA_Diffuse(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nRotateSingleIn && nType <= Constants::c_nRotateTwiceOutClockwise)
						return BGRA::BGRA_Rotate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nWipeDoorVerticalIn && nType <= Constants::c_nWipeDoorDiagonalRotated)
						return BGRA::BGRA_WipeDoor(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nWipeStripsVertical && nType <= Constants::c_nWipeStripsHorizontal)
						return BGRA::BGRA_WipeStrips(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nWipeSideLeftToRight && nType <= Constants::c_nWipeCornerRightTop)
						return BGRA::BGRA_WipeSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nWipeCenterBoxIn && nType <= Constants::c_nWipeCenterCrossOut)
						return BGRA::BGRA_WipeCenter(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSlideDoorVerticalIn && nType <= Constants::c_nSlideDoorDiagonalRotated)
						return BGRA::BGRA_SlideDoor(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);
					
					if (nType >= Constants::c_nSlideStripsVertical && nType <= Constants::c_nSlideStripsHorizontal)
						return BGRA::BGRA_SlideStrips(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSlideSideLeftToRight && nType <= Constants::c_nSlideCornerRightTop)
						return BGRA::BGRA_SlideSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSlideCenterCrossIn && nType <= Constants::c_nSlideCenterCrossOut)
						return BGRA::BGRA_SlideCenter(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nPushDoorVerticalIn && nType <= Constants::c_nPushDoorHorizontalOut)
						return BGRA::BGRA_PushDoor(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nPushStripsVertical && nType <= Constants::c_nPushStripsHorizontal)
						return BGRA::BGRA_PushStrips(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nPushSideLeftToRight && nType <= Constants::c_nPushSideBottomToTop)
						return BGRA::BGRA_PushSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nStretchDoorVerticalIn && nType <= Constants::c_nStretchDoorHorizontalOut)
						return BGRA::BGRA_StretchDoor(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nStretchStripsVertical && nType <= Constants::c_nStretchStripsHorizontal)
						return BGRA::BGRA_StretchStrips(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nStretchSideLeftToRight && nType <= Constants::c_nStretchCornerRightTop)
						return BGRA::BGRA_StretchSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nStretchCenterBoxIn && nType <= Constants::c_nStretchCenterBoxOut)
						return BGRA::BGRA_StretchCenter(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nStretchWipeSideLeftToRightIn && nType <= Constants::c_nStretchWipeSideBottomToTopOut)
						return BGRA::BGRA_StretchWipeSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSwingSideLeftIn && nType <= Constants::c_nSwingSideBottomOut)
						return BGRA::BGRA_SwingSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSwingSideChangeLeft && nType <= Constants::c_nSwingSideChangeBottom)
						return BGRA::BGRA_SwingSideChange(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSwingDoorsVerticalIn	&& nType <= Constants::c_nSwingDoorsHorizontalOut)
						return BGRA::BGRA_SwingDoors(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nSwingDoorsChangeVertical	|| nType == Constants::c_nSwingDoorsChangeHorizontal)
						return BGRA::BGRA_SwingDoorsChange(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nPageTurnLeftTop && nType <= Constants::c_nPageTurnRightTop)
						return BGRA::BGRA_PageTurnSimple(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nPageTurnLeftTopWhiteBack3d && nType <= Constants::c_nPageTurnRightTopWhiteBack3d /*|| nType == Constants::c_nPageTurnLeftTopShadow3d*/)
						return BGRA::BGRA_PageTurnWhiteBack(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nPageTurnLeftTop3d && nType <= Constants::c_nPageTurnRightTop3d)
						return BGRA::BGRA_PageTurnWhiteBack(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if ( nType >= Constants::c_nPageTurnAlbumLeftTop && nType <= Constants::c_nPageTurnAlbumLeftBottomCorners4 )
						return BGRA::BGRA_PageTurnAlbum(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);
					
					if (nType == Constants::c_nFlipOverVerticalBack ||
						nType == Constants::c_nFlipOverVertical	||
						nType == Constants::c_nFlipOverHorizontalBack ||
						nType == Constants::c_nFlipOverHorizontal)
						return BGRA::BGRA_FlipOver(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nFlipAwayVerticalInBack && nType <= Constants::c_nFlipAwayHorizontalOut)
						return BGRA::BGRA_FlipAway(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSlideChangeLeftToRight && nType <= Constants::c_nSlideChangeBottomToTop)
						return BGRA::BGRA_SlideChange(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSlideLinesLeftToRight && nType <= Constants::c_nSlideLinesBottomToTop)
						return BGRA::BGRA_SlideBoxes(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nWipeCheckerLeftToRight && nType <= Constants::c_nWipeCheckerBottomToTop)
						return BGRA::BGRA_WipeChecker(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nClockBottomClockwise && nType <= Constants::c_nClock12AnglesReverse)
						return BGRA::BGRA_WipeClock(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nWipeRandomStripsVertical && nType <= Constants::c_nWipeRandomStripsHorizontal)
						return BGRA::BGRA_WipeStripsRandom(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oUtility);

					if (nType >= Constants::c_nShatterCornerRightBottom && nType <= Constants::c_nShatterCornerLeftBottom)
						return BGRA::BGRA_ShatterCorner(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nShatterSideLeftToRight && nType <= Constants::c_nShatterSideBottomToTop || nType == Constants::c_nWhirlWind)
						return BGRA::BGRA_ShatterSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nShatterCenterInsideHigh && nType <= Constants::c_nShatterCenterInsideLow)
						return BGRA::BGRA_ShatterCenter(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if ((nType >= Constants::c_nGraduateSideLeftToRight && nType <= Constants::c_nGraduateCornerRightTop) || (Constants::c_nGraduateClockBottomClockwise <= nType && Constants::c_nGraduateClock3AnglesReverseSmooth >= nType))
						return BGRA::BGRA_Graduate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oMaskGraduate);

					if (nType >= Constants::c_nTwirlClockwise && nType <= Constants::c_nTwirlReverseDrain)
						return BGRA::BGRA_Twirl(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSphereInside && nType <= Constants::c_nSphereOutside)
						return BGRA::BGRA_Sphere(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nTwistLeftToRight && nType <= Constants::c_nTwistRightBottom)
						return BGRA::BGRA_Twist(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nRippleOutside && nType <= Constants::c_nRippleInside)
						return BGRA::BGRA_Ripple(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nWaveHorizontal && nType <= Constants::c_nWaveVertical)
						return BGRA::BGRA_Wave(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nEnhancedBurningThreshold && nType <= Constants::c_nEnhancedBurningFire)
						return BGRA::BGRA_Burning(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oUtility);

					if (nType >= Constants::c_nFadeSideLeftToRight && nType <= Constants::c_nFadeSideBottomToTop)
						return BGRA::BGRA_WipeFadeSide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nClockCornerLeftTop && nType <= Constants::c_nClockCornerReverseRightBottom)
						return BGRA::BGRA_WipeClockCorner(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nFlipFlyVerticalIn ||
						nType == Constants::c_nFlipFlyVerticalOut ||
						nType == Constants::c_nFlipFlyHorizontalIn ||
						nType == Constants::c_nFlipFlyHorizontalOut)
						return BGRA::BGRA_FlipOutIn(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nStretchFoldLeftToRight && nType <= Constants::c_nStretchFoldBottomToTop)
						return BGRA::BGRA_Fold(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nEnhancedGaussianBlur)
						return BGRA::BGRA_GaussianBlur(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nEnhancedColorNoise && nType <= Constants::c_nEnhancedFlashDark)
						return BGRA::BGRA_Noise(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nFlipBoxesLeftToRight && nType <= Constants::c_nFlipBoxesBottomToTop)
						return BGRA::BGRA_FlipBoxes(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nFlipOutBoxesVertical || 
						nType == Constants::c_nFlipOutBoxesVerticalBack ||
						nType == Constants::c_nFlipOutBoxesHorizontal ||
						nType == Constants::c_nFlipOutBoxesHorizontalBack)
						return BGRA::BGRA_FlipOutBoxes(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nTetris && nType <= Constants::c_nTetrisCollapse)
						return BGRA::BGRA_Tetris(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oUtility);

					if (nType <= Constants::c_nShotChangeBVerticalBToUNoZ && nType >= Constants::c_nShotChangeGHorizontalRToL)
						return BGRA::BGRA_ShotChange(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oUtility);

					if (nType == Constants::c_nBlackWhiteLines || nType == Constants::c_nBlackWhiteLinesInverse)
						return BGRA::BGRA_BlackWhiteLines(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if( (nType >= Constants::c_nBowTileHorizontal  && nType <= Constants::c_nWipeDiamonds) ||
						(nType >= Constants::c_nPinwheelCurvedSN5A && nType <= Constants::c_nPinwheelPropeller12Smooth) ||
						(nType == Constants::c_nEnhancedAlphaNoise || nType == Constants::c_nEnhancedShutter) )
						return BGRA::BGRA_Mask(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oMask);

					if (nType == Constants::c_nWave || nType == Constants::c_nSmoothWave )
						return BGRA::BGRA_SimpleWave(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nEye || nType == Constants::c_nSmoothEye)
						return BGRA::BGRA_Eye(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nHeart || nType == Constants::c_nSmoothHeart)
						return BGRA::BGRA_Heart(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nPixelate)
						return BGRA::BGRA_Pixelate(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nRollRight || nType == Constants::c_nRollLeft)
						return BGRA::BGRA_Roll(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSlideUp && nType <= Constants::c_nSlideLeft)
						return BGRA::BGRA_Slide(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSlideCenterUp && nType <= Constants::c_nSlideCenterLeft)
						return BGRA::BGRA_SlideCenter2(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType == Constants::c_nShrinkInVertical || nType == Constants::c_nShrinkInHorizontal)
						return BGRA::BGRA_Shrink(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if (nType >= Constants::c_nSpiralTopLeft && nType <= Constants::c_nSpiralReverseCenterOut)
						return BGRA::BGRA_Spiral(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness);

					if ( nType >= Constants::c_nPuzzle4x4 && nType <= Constants::c_nTile10x10RandCollapse )
						return BGRA::BGRA_Puzzle(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oPuzzleUtility);

					if (nType >= Constants::c_nSmallHexagons && nType <= Constants::c_nLargeInclinedBricksCollapse )
						return BGRA::BGRA_Tiles(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness, &m_oPuzzleUtility);

					// по умолчанию применяем простой fade
					return BGRA::BGRA_Fade(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, Constants::c_nFadeSolid, dCompleteness);

					/*
					switch (effectCategory)
					{
						default:  Fade             (effectType); break; //
						case 1:   Mosaic           (effectType); break; //
						case 2:   Diffuse          (effectType); break; //
						case 3:   Rotate           (effectType); break; //
						case 4:   WipeDoor         (effectType); break; //
						case 5:   WipeStrips       (effectType); break; //
						case 6:   WipeSide         (effectType); break; //
						case 7:   WipeCenter       (effectType); break; //
						case 8:   SlideDoor        (effectType); break; //
						case 9:   SlideStrips      (effectType); break; //
						case 10:  SlideSide        (effectType); break; //
						case 11:  SlideCenter      (effectType); break; //
						case 12:  PushDoor         (effectType); break; //
						case 13:  PushStrips       (effectType); break; //
						case 14:  PushSide         (effectType); break; //
						case 15:  StretchDoor      (effectType); break; //
						case 16:  StretchStrips    (effectType); break; //
						case 17:  StretchSide      (effectType); break; //
						case 18:  StretchCenter    (effectType); break; //
						case 19:  StretchWipeSide  (effectType); break; //
						case 20:  SwingSide        (effectType); break; //   
						case 21:  SwingSideChange  (effectType); break; //
						case 22:  SwingDoors       (effectType); break; //
						case 23:  SwingDoorsChange (effectType); break; //
						case 24:  PageTurn         (effectType); break; //
						case 25:  FlipOver         (effectType); break; //
						case 26:  FlipAway         (effectType); break; //
						case 27:  SlideChange      (effectType); break; //
						case 28:  SlideBoxes       (effectType); break; //  
						case 29:  WipeChecker      (effectType); break; //
						case 30:  WipeClock        (effectType); break; //
						case 31:  WipeStripsRandom (effectType); break; //
						case 32:  Shatter          (effectType); break; //
						case 33:  ShatterRotate    (effectType); break; /
						case 34:  ShatterIn        (effectType); break; //
						case 35:  GraduateWipeSide (effectType); break; //
						case 36:  GraduateWipeClock(effectType); break; //
						case 37:  Twirl		       (effectType); break; //
						case 38:  Sphere	       (effectType); break; //
						case 39:  Twist			   (effectType); break; //
						case 40:  Ripple		   (effectType); break; //
						case 41:  Wave			   (effectType); break; //
						case 42:  Burning		   (effectType); break; //
						case 43:  WipeFadeSide	   (effectType); break; //
						case 44:  WipeCircleCorner (effectType); break; //
						case 45:  WipeClockCorner  (effectType); break; //
						case 46:  FlipOutIn        (effectType); break; //
						case 47:  Fold			   (effectType); break; //
						case 48:  GaussianBlur	   (effectType); break; //
						case 49:  Noise			   (effectType); break; //
						case 50:  FlipBoxes		   (effectType); break; //
						case 51:  FlipOutBoxes	   (effectType); break; //
					}

					*/
				}
				
			protected:

				void ComputeDefaultTileSize(int nType, int nWidth, int nHeight, int& nTileSizeX, int& nTileSizeY)
				{
					// TODO: подбираем размер тайла в зависимости от эффекта!

					if( nType >= Constants::c_nFadePatternDots && nType <= Constants::c_nFadePatternCross )
					{
						int nTileSize = 8;

						nTileSizeX = nTileSize;
						nTileSizeY = nTileSize;
						return;
					}

					if( nType >= Constants::c_nFadePatternDots2 && nType <= Constants::c_nFadePatternCross2 )
					{
						int nTileSize = max(16, max(nWidth, nHeight) / 20);

						nTileSizeX = nTileSize;
						nTileSizeY = nTileSize;
						return;
					}

					if( nType >= Constants::c_nFadePatternDots3 && nType <= Constants::c_nFadePatternCross3 )
					{
						int nTileSize = max(20, max(nWidth, nHeight) / 10);

						nTileSizeX = nTileSize;
						nTileSizeY = nTileSize;
						return;
					}

					if( nType >= Constants::c_nMosaicDissolve && nType <= Constants::c_nMosaicRandom )
					{
						nTileSizeX = (nTileSizeX < 1) ? 8 : nTileSizeX;
						nTileSizeY = (nTileSizeY < 1) ? 8 : nTileSizeY;
						return;
					}

					if ((nType >= Constants::c_nSlideSideLeftToRight && nType <= Constants::c_nSlideCornerRightTop) ||
						(nType >= Constants::c_nFlipBoxesLeftToRight && nType <= Constants::c_nFlipBoxesBottomToTop))
					{
						nTileSizeX = (nTileSizeX < 1) ? (max(2, nWidth/8)) : nTileSizeX;
						nTileSizeY = (nTileSizeY < 1) ? (max(2, nHeight/8)) : nTileSizeY;
						return;
					}

					if ((nType >= Constants::c_nWipeDoorVerticalIn && nType <= Constants::c_nWipeDoorDiagonalRotated) ||
						(nType >= Constants::c_nWipeStripsVertical && nType <= Constants::c_nWipeStripsHorizontal) ||
						(nType >= Constants::c_nWipeSideLeftToRight && nType <= Constants::c_nWipeCornerRightTop) ||
						(nType >= Constants::c_nWipeCheckerLeftToRight && nType <= Constants::c_nWipeCheckerBottomToTop) ||
						(nType >= Constants::c_nGraduateSideLeftToRight && nType <= Constants::c_nGraduateCornerRightTop) ||
						(nType >= Constants::c_nStretchFoldLeftToRight && nType <= Constants::c_nStretchFoldBottomToTop) ||
						(nType >= Constants::c_nFlipOutBoxesVertical && nType <= Constants::c_nFlipOutBoxesHorizontalBack))
					{
						nTileSizeX = (nTileSizeX < 1) ? (max(2, nWidth/4)) : nTileSizeX;
						nTileSizeY = (nTileSizeY < 1) ? (max(2, nHeight/4)) : nTileSizeY;
						return;
					}

					if ((nType >= Constants::c_nSlideStripsVertical && nType <= Constants::c_nSlideStripsHorizontal) ||
						(nType >= Constants::c_nGraduateClockBottomClockwise && nType <= Constants::c_nGraduateClock3AnglesReverse))
					{
						nTileSizeX = (nTileSizeX < 1) ? (max(2, nWidth/2)) : nTileSizeX;
						nTileSizeY = (nTileSizeY < 1) ? (max(2, nHeight/2)) : nTileSizeY;
						return;
					}

					if (nType == Constants::c_nWipeSmoothCircles ||
						nType == Constants::c_nWipeSmoothDiamonds ||
						nType == Constants::c_nWipeSmoothWideStars ||
						nType == Constants::c_nWipeSmoothNarrowStars ||
						nType == Constants::c_nWipeSmoothWideFlowers ||
						nType == Constants::c_nWipeSmoothNarrowFlowers ||
						nType == Constants::c_nWipeSmoothWideKeyHoles ||
						nType == Constants::c_nWipeSmoothNarrowKeyHoles)
					{
						nTileSizeX = nWidth/5;
						nTileSizeY = nHeight/5;
						return;
					}

					nTileSizeX = (nTileSizeX < 1) ? nWidth : nTileSizeX;
					nTileSizeY = (nTileSizeY < 1) ? nHeight : nTileSizeY;
				}
				
			protected:

				//Utility::CGraduate m_oGraduate;
				Utility::CUtility m_oUtility;	
				Utility::CMask m_oMask;
				Utility::CPuzzleUtility m_oPuzzleUtility;
				Utility::CMaskGraduate m_oMaskGraduate;
			};
		}
	}
}
