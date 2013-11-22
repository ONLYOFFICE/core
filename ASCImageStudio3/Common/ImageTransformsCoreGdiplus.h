#pragma once

#include <ImageTransformsCoreConstants.h>
#include <PaintStruct.h>

namespace ImageStudio
{
	namespace Transforms
	{
		namespace Core
		{
			namespace GDIPLUS
			{
				int GDIPLUS_GetPathPoints(Gdiplus::GraphicsPath& oPath, Gdiplus::PointF*& pPoints, int& nCount)
				{
					pPoints = NULL;

					nCount = oPath.GetPointCount();

					if (nCount > 0)
					{
						pPoints = new Gdiplus::PointF[nCount];

						if (NULL == pPoints)
							return c_nErrorNoMemory;
						
						oPath.GetPathPoints(pPoints, nCount);
					}

					return c_nErrorNone;
				}
				int GDIPLUS_GetPathPointsBeginEnd(Gdiplus::GraphicsPath& oPath, Gdiplus::PointF& oPointsBegin, Gdiplus::PointF& oPointsEnd)
				{
					Gdiplus::PointF* pPoints = NULL;
					int nCount = 0;

					if (c_nErrorNone != GDIPLUS_GetPathPoints(oPath, pPoints, nCount))
						return c_nErrorPathIsEmpty;

					oPointsBegin = *pPoints;
					oPointsEnd = *(pPoints + nCount - 1);

					if (NULL != pPoints)
						delete[] pPoints;

					return c_nErrorNone;
				}
				int GDIPLUS_GetSectorPoints(float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle, float fRatio, Gdiplus::PointF*& pPoints, int& nCount)
				{
					/* @desc:
					int GDIPLUS_GetSectorPoints(float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle, float fRatio, Gdiplus::PointF*& pPoints, int& nCount)
					{
						подсчитывает границу сектора, заданного как кусок эллипса (Left, Top, Width, Height),
						ограниченного углами (StartAngle, SweepAngle) и радиусами (радиус эллипса; радиус эллипса, умноженный на Ratio)
						и сохраняет результат в виде полилайна (pPoints, nCount)
						предполагается, что параметры Width и Height >= 0 
					}
					*/

					// очищаем результат
					nCount = 0;
					pPoints = NULL;

					Gdiplus::REAL fLeftInt, fTopInt, fWidthInt, fHeightInt;
					Gdiplus::REAL fLeftOut, fTopOut, fWidthOut, fHeightOut;

					// подсчитываем внешний и внутренний прямоугольники для сектора
					if (fRatio <= 1.0f)
					{
						fLeftOut = fLeft;
						fTopOut = fTop;
						fWidthOut = fWidth;
						fHeightOut = fHeight;

						fLeftInt = fLeft + 0.5f*(fWidthOut - fWidthOut*fRatio);
						fTopInt = fTop + 0.5f*(fHeightOut - fHeightOut*fRatio);
						fWidthInt = fWidthOut*fRatio;
						fHeightInt = fHeightOut*fRatio;
					}
					else
					{
						fLeftInt = fLeft;
						fTopInt = fTop;
						fWidthInt = fWidth;
						fHeightInt = fHeight;

						fLeftOut = fLeft + 0.5f*(fWidthInt - fWidthInt*fRatio);
						fTopOut = fTopInt + 0.5f*(fHeightInt - fHeightInt*fRatio);
						fWidthOut = fWidthInt*fRatio;
						fHeightOut = fHeightInt*fRatio;
					}

					// создаем gdiplus фигуры для подсчета отрисовки
					Gdiplus::GraphicsPath oPathArc1; oPathArc1.AddArc(fLeftOut, fTopOut, fWidthOut - 1, fHeightOut - 1, fStartAngle, fSweepAngle);
					Gdiplus::GraphicsPath oPathArc2; oPathArc2.AddArc(fLeftInt, fTopInt, fWidthInt - 1, fHeightInt - 1, fStartAngle, fSweepAngle); oPathArc2.Reverse();

					oPathArc1.Flatten();
					oPathArc2.Flatten();

					Gdiplus::PointF* pPointsArc1 = NULL;
					Gdiplus::PointF* pPointsArc2 = NULL;
					int nPointsArc1Count = 0;
					int nPointsArc2Count = 0;

					int nResult = c_nErrorNone;

					// достаем из path'ов координаты arc'ов
					if (c_nErrorNone == GDIPLUS_GetPathPoints(oPathArc1, pPointsArc1, nPointsArc1Count) && 
						c_nErrorNone == GDIPLUS_GetPathPoints(oPathArc2, pPointsArc2, nPointsArc2Count) &&
						nPointsArc1Count > 0 && nPointsArc2Count > 0)
					{
						// allocat'им память под результирующий полилайн
						nCount = nPointsArc1Count + nPointsArc2Count;
						pPoints = new Gdiplus::PointF[nCount];

						if (NULL == pPoints)
							nResult = c_nErrorNoMemory;
						else
						{
							Gdiplus::PointF* pPointsArc1Temp = pPointsArc1;
							Gdiplus::PointF* pPointsArc2Temp = pPointsArc2;
							Gdiplus::PointF* pPointsSectorTemp = pPoints;

							// считаем точки полигона сектора
							for (int index = 0; index < nPointsArc1Count; ++index, ++pPointsSectorTemp, ++pPointsArc1Temp)
								*pPointsSectorTemp = *pPointsArc1Temp;
							for (int index = 0; index < nPointsArc2Count; ++index, ++pPointsSectorTemp, ++pPointsArc2Temp)
								*pPointsSectorTemp = *pPointsArc2Temp;
						}
					}

					// освобождаем ненужную память
					if (NULL != pPointsArc1)
						delete[] pPointsArc1;
					if (NULL != pPointsArc2)
						delete[] pPointsArc2;

					return nResult;
				}
				
				int GDIPLUS_DrawLine(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fX1, float fY1, float fX2, float fY2)
				{
					/* @desc:
					int GDIPLUS_DrawLine(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fX1, float fY1, float fX2, float fY2)
					{
						отрисовывает линию с координатами (X1,Y1 - X2,Y2) в указанном (pGraphics) указанным стилем (Gdiplus::Pen* pPen)
					}
					*/

					if (NULL == pGraphics || NULL == pPen)
						return c_nErrorInvalidPointer;

					pGraphics->DrawLine(pPen, fX1, fY1, fX2, fY2);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawRectangle(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_DrawRectangle(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывает границу прямоугольника с координатами (Left, Top, Width, Height) 
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pPen)
						return c_nErrorInvalidPointer;

					pGraphics->DrawRectangle(pPen, fLeft, fTop, fWidth, fHeight);

					return c_nErrorNone;
				}
				int GDIPLUS_FillRectangle(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_FillRectangle(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывает "залитый" прямоугольник с координатами (Left, Top, Width, Height) 
						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pBrush)
						return c_nErrorInvalidPointer;

					pGraphics->FillRectangle(pBrush, fLeft, fTop, fWidth, fHeight);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawEllipse(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_DrawEllipse(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывает границу эллипса, вписанного в прямоугольник с координатами (Left, Top, Width, Height) 
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pPen)
						return c_nErrorInvalidPointer;

					pGraphics->DrawEllipse(pPen, fLeft, fTop, fWidth, fHeight);

					return c_nErrorNone;
				}
				int GDIPLUS_FillEllipse(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_FillEllipse(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывает "залитый" эллипс, вписанный в прямоугольник с координатами (Left, Top, Width, Height) 
						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pBrush)
						return c_nErrorInvalidPointer;

					pGraphics->FillEllipse(pBrush, fLeft, fTop, fWidth, fHeight);

					return c_nErrorNone;
				}
				int GDIPLUS_FillInverseRectangle(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::RectF& oRectImage, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_FillInverseRectangle(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::RectF& oRectImage, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывает "залитую" внешность прямоугольника с координатами (Left, Top, Width, Height) 

						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)

						в качестве объемлющего прямоугольника используется (oRectImage)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pBrush)
						return c_nErrorInvalidPointer;

					// создаем регион для заливки
					Gdiplus::RectF oRectClip(fLeft, fTop, fWidth, fHeight);
					Gdiplus::Region oRegion(oRectImage); 
					
					oRegion.Xor(oRectClip);

					pGraphics->FillRegion(pBrush, &oRegion);

					return c_nErrorNone;
				}
				int GDIPLUS_FillInverseEllipse(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::RectF& oRectImage, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_FillInverseEllipse(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::RectF& oRectImage, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывает "залитую" внешность эллипса, заданного описанным около него прямоугольником с координатами (Left, Top, Width, Height)

						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)

						в качестве объемлющего прямоугольника используется (oRectImage)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pBrush)
						return c_nErrorInvalidPointer;

					// создаем регион для заливки
					Gdiplus::GraphicsPath oPath; 

					oPath.AddEllipse(fLeft, fTop, fWidth, fHeight);

					oPath.AddRectangle(oRectImage);

					pGraphics->FillPath(pBrush, &oPath);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawPolyline(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, Gdiplus::PointF* pPoints, int nCount)
				{
					/* @desc:
					int GDIPLUS_DrawPolyline(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, Gdiplus::PointF* pPoints, int nCount)
					{
						отрисовывает ломаную линию с координатами точек (pPoints, nCount) 
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)
					}
					*/

					if (NULL == pGraphics || NULL == pPen)
						return c_nErrorInvalidPointer;

					if (NULL == pPoints || nCount < 2)
						return c_nErrorNone;

					pGraphics->DrawLines(pPen, pPoints, nCount);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawPolygon(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, Gdiplus::PointF* pPoints, int nCount)
				{
					/* @desc:
					int GDIPLUS_DrawPolygon(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, Gdiplus::PointF* pPoints, int nCount)
					{
						отрисовывает замкнутую ломаную линию с координатами точек (pPoints, nCount) 
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)
					}
					*/

					if (NULL == pGraphics || NULL == pPen)
						return c_nErrorInvalidPointer;

					if (NULL == pPoints || nCount < 3)
						return c_nErrorNone;

					pGraphics->DrawPolygon(pPen, pPoints, nCount);

					return c_nErrorNone;
				}
				int GDIPLUS_FillPolygon(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::PointF* pPoints, int nCount)
				{
					/* @desc:
					int GDIPLUS_FillPolygon(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::PointF* pPoints, int nCount)
					{
						отрисовывает "залитую" замкнутую ломаную линию с координатами точек (pPoints, nCount) 
						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)
					}
					*/

					if (NULL == pGraphics || NULL == pBrush)
						return c_nErrorInvalidPointer;

					if (NULL == pPoints || nCount < 3)
						return c_nErrorNone;

					pGraphics->FillPolygon(pBrush, pPoints, nCount);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawPie(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
				{
					/* @desc:
					int GDIPLUS_DrawPie(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
					{
						отрисовывает границу pie'я, заданного как кусок эллипса (Left, Top, Width, Height), 
						ограниченный углами (StartAngle, SweepAngle),
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pPen)
						return c_nErrorInvalidPointer;

					pGraphics->DrawPie(pPen, fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle);

					return c_nErrorNone;
				}
				int GDIPLUS_FillPie(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
				{
					/* @desc:
					int GDIPLUS_FillPie(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
					{
						отрисовывает внутренность pie'я, заданного как кусок эллипса (Left, Top, Width, Height), 
						ограниченный углами (StartAngle, SweepAngle),
						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pBrush)
						return c_nErrorInvalidPointer;

					pGraphics->FillPie(pBrush, fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawSector(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle, float fRatio)
				{
					/* @desc:
					int GDIPLUS_DrawSector(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle, float fRatio)
					{
						отрисовывает границу сектора, заданного как кусок эллипса (Left, Top, Width, Height),
						ограниченный углами (StartAngle, SweepAngle) и радиусами (радиус эллипса; радиус эллипса, умноженный на Ratio)
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)
						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pPen)
						return c_nErrorInvalidPointer;

					Gdiplus::PointF* pPoints = NULL;
					int nCount = 0;

					// подсчитываем координаты сектора
					if (c_nErrorNone != GDIPLUS_GetSectorPoints(fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle, fRatio, pPoints, nCount))
						return c_nErrorNone;

					// отрисовываем сектор
					int nResult = GDIPLUS_DrawPolygon(pGraphics, pPen, pPoints, nCount);

					// удаляем память
					if (NULL != pPoints)
						delete[] pPoints;

					return nResult;
				}
				int GDIPLUS_FillSector(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle, float fRatio)
				{
					/* @desc:
					int GDIPLUS_FillSector(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fLeft, float fTop, float fWidth, float fHeight, float fStartAngle, float fSweepAngle, float fRatio)
					{
						отрисовывает "залитый" сектор, заданный как кусок эллипса (Left, Top, Width, Height),
						ограниченный углами (StartAngle, SweepAngle) и радиусами (радиус эллипса; радиус эллипса, умноженный на Ratio)
						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)
						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pBrush)
						return c_nErrorInvalidPointer;

					Gdiplus::PointF* pPoints = NULL;
					int nCount = 0;

					// подсчитываем координаты сектора
					if (c_nErrorNone != GDIPLUS_GetSectorPoints(fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle, fRatio, pPoints, nCount))
						return c_nErrorNone;

					// отрисовываем сектор
					int nResult = GDIPLUS_FillPolygon(pGraphics, pBrush, pPoints, nCount);

					// удаляем память
					if (NULL != pPoints)
						delete[] pPoints;

					return nResult;
				}
				int GDIPLUS_DrawSimplePie(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fCenterX, float fCenterY, float fRadius, float fStartAngle, float fSweepAngle)
				{
					/* @desc:
					int GDIPLUS_DrawSimplePie(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fCenterX, float fCenterY, float fRadius, float fStartAngle, float fSweepAngle)
					{
						отрисовывает границу pie'я, заданного как кусок окружности (CenterX, CenterY, Radius), 
						ограниченный углами (StartAngle, SweepAngle),
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)
					}
					*/

					float fLeft = fCenterX - fRadius;
					float fTop = fCenterY - fRadius;
					float fWidth = 2*fRadius;
					float fHeight = 2*fRadius;

					return GDIPLUS_DrawPie(pGraphics, pPen, fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle);
				}
				int GDIPLUS_FillSimplePie(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fCenterX, float fCenterY, float fRadius, float fStartAngle, float fSweepAngle)
				{
					/* @desc:
					int GDIPLUS_FillSimplePie(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fCenterX, float fCenterY, float fRadius, float fStartAngle, float fSweepAngle)
					{
						отрисовывает внутренность pie'я, заданного как кусок окружности (CenterX, CenterY, Radius), 
						ограниченный углами (StartAngle, SweepAngle),
						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)
					}
					*/

					float fLeft = fCenterX - fRadius;
					float fTop = fCenterY - fRadius;
					float fWidth = 2*fRadius;
					float fHeight = 2*fRadius;

					return GDIPLUS_FillPie(pGraphics, pBrush, fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle);
				}
				int GDIPLUS_DrawSimpleSector(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fCenterX, float fCenterY, float fRadius1, float fRadius2, float fStartAngle, float fSweepAngle)
				{
					/* @desc:
					int GDIPLUS_DrawSimpleSector(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, float fCenterX, float fCenterY, float fRadius1, float fRadius2, float fStartAngle, float fSweepAngle)
					{
						отрисовывает границу сектора, заданного как кусок окружности с центром в точке (CenterX, CenterY), 
						ограниченный углами (StartAngle, SweepAngle) и радиусами (Radius1, Radius2),
						в указанном (pGraphics) и указанным стилем (Gdiplus::Pen* pPen)
					}
					*/

					float fLeft = fCenterX - fRadius1;
					float fTop = fCenterY - fRadius1;
					float fWidth = 2*fRadius1;
					float fHeight = 2*fRadius1;

					return GDIPLUS_DrawSector(pGraphics, pPen, fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle, fRadius2/fRadius1);
				}
				int GDIPLUS_FillSimpleSector(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fCenterX, float fCenterY, float fRadius1, float fRadius2, float fStartAngle, float fSweepAngle)
				{
					/* @desc:
					int GDIPLUS_FillSimpleSector(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, float fCenterX, float fCenterY, float fRadius1, float fRadius2, float fStartAngle, float fSweepAngle)
					{
						отрисовывает "залитый" сектор, заданного как кусок окружности с центром в точке (CenterX, CenterY), 
						ограниченный углами (StartAngle, SweepAngle) и радиусами (Radius1, Radius2),
						в указанном (pGraphics) и указанным стилем (Gdiplus::Brush* pBrush)
					}
					*/

					float fLeft = fCenterX - fRadius1;
					float fTop = fCenterY - fRadius1;
					float fWidth = 2*fRadius1;
					float fHeight = 2*fRadius1;

					return GDIPLUS_FillSector(pGraphics, pBrush, fLeft, fTop, fWidth, fHeight, fStartAngle, fSweepAngle, fRadius2/fRadius1);
				}

				int GDIPLUS_DrawText(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::Font* pFont, Gdiplus::StringFormat* pStringFormat, const CString& strText, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_DrawText(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::Font* pFont, Gdiplus::StringFormat* pStringFormat, const CString& strText, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывается текст (strText) с указанными настройками

						шрифт - pFont
						формат отрисовки текста - pFormat (опциональное значение, может быть равно NULL)
						тип заливки - pBrush
						прямоугольник, в котором будет производиться отрисовка - (Left, Top, Right, Bottom)

						в указанном (pGraphics)
					}
					*/

					if (NULL == pGraphics || NULL == pBrush || NULL == pFont)
						return c_nErrorInvalidPointer;

					int nTextLength = strText.GetLength();
					if (nTextLength < 1)
						return c_nErrorNone;

					BSTR bstrText = strText.AllocSysString();

					// вычисляем прямоугольник, в котором будем отрисовывать текст
					Gdiplus::RectF oRectText(fLeft, fTop, fWidth, fHeight);

					if (NULL != pStringFormat)
						pGraphics->DrawString(bstrText, nTextLength, pFont, oRectText, pStringFormat, pBrush);
					else
					{
						Gdiplus::StringFormat oStringFormat;

						oStringFormat.SetAlignment(StringAlignmentNear);
						oStringFormat.SetLineAlignment(StringAlignmentNear);

						pGraphics->DrawString(bstrText, nTextLength, pFont, oRectText, &oStringFormat, pBrush);
					}

					SysFreeString(bstrText);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawTextScale(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::Font* pFont, Gdiplus::StringFormat* pStringFormat, const CString& strText, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_DrawTextScale(Gdiplus::Graphics* pGraphics, Gdiplus::Brush* pBrush, Gdiplus::Font* pFont, Gdiplus::StringFormat* pStringFormat, const CString& strText, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывается текст (strText) с указанными настройками

						шрифт - pFont
						формат отрисовки текста - pFormat (опциональное значение, может быть равно NULL)
						тип заливки - pBrush
						прямоугольник, в котором будет производиться отрисовка - (dLeft, dTop, dRight, dBottom); при этом текст растянется в этот прямоугольник

						в указанном (pGraphics)
					}
					*/

					if (NULL == pGraphics || NULL == pBrush || NULL == pFont)
						return c_nErrorInvalidPointer;

					int nTextLength = strText.GetLength();
					if (nTextLength < 1)
						return c_nErrorNone;

					BSTR bstrText = strText.AllocSysString();

					// вычисляем прямоугольник, описанный около текста без учета растяжения
					Gdiplus::PointF oPointCorner(0, 0);
					Gdiplus::RectF oRectTextNormal;
					pGraphics->MeasureString(bstrText, nTextLength, pFont, oPointCorner, &oRectTextNormal);

					// вычисляем прямоугольник, в котором будем отрисовывать текст
					Gdiplus::RectF oRectTextScaled(fLeft, fTop, fWidth, fHeight);

					// запоминаем текущее состояние pGraphics, чтобы после трансформаций и отрисовок вернуть state назадз
					Gdiplus::GraphicsState oState = pGraphics->Save();

					// выполняем преобразование координат для растяжения текста
					pGraphics->TranslateTransform(oRectTextScaled.X, oRectTextScaled.Y);
					pGraphics->ScaleTransform(oRectTextScaled.Width/oRectTextNormal.Width, oRectTextScaled.Height/oRectTextNormal.Height);

					Gdiplus::StringFormat oStringFormat;
					oStringFormat.SetAlignment(StringAlignmentCenter);
					oStringFormat.SetLineAlignment(StringAlignmentCenter);

					// отрисовываем текст
					if (NULL != pStringFormat)
						pGraphics->DrawString(bstrText, nTextLength, pFont, oRectTextNormal, pStringFormat, pBrush);
					else
					{
						Gdiplus::StringFormat oStringFormat;

						oStringFormat.SetAlignment(StringAlignmentNear);
						oStringFormat.SetLineAlignment(StringAlignmentNear);

						pGraphics->DrawString(bstrText, nTextLength, pFont, oRectTextNormal, &oStringFormat, pBrush);
					}

					// удаляем ненужную память
					SysFreeString(bstrText);

					// возвращаем старое состояние pGraphics
					pGraphics->Restore(oState);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawImage(Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImage, float fLeft, float fTop, float fWidth, float fHeight)
				{
					/* @desc:
					int GDIPLUS_DrawImage(Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImage, float fLeft, float fTop, float fWidth, float fHeight)
					{
						отрисовывает картинку (pImage) с координатами (Left, Top, Width, Height) 
						в указанном (pGraphics)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics || NULL == pImage)
						return c_nErrorInvalidPointer;

					pGraphics->DrawImage(pImage, fLeft, fTop, fWidth, fHeight);

					return c_nErrorNone;
				}

/* TODO
[id(401)] HRESULT DrawImage([in] double Left, [in] double Top, [in, satype("BYTE")] SAFEARRAY** Image);
[id(403)] HRESULT DrawImageScale([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in, satype("BYTE")] SAFEARRAY** Image);
[id(405)] HRESULT DrawImageColorKeyRange([in] long Left, [in] long Top, [in, satype("BYTE")] SAFEARRAY** Image, [in] long Color1, [in] long Color2);
[id(407)] HRESULT DrawImageColorKeyThreshold([in] long Left, [in] long Top, [in, satype("BYTE")] SAFEARRAY** Image, [in] long Color, [in] long Threshold);
*/
				
				int GDIPLUS_DrawLine(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dX1, double dY1, double dX2, double dY2)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetPenAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Pen* pPen = oSettings.GetPen().GetPen();

					return GDIPLUS_DrawLine(pGraphics, pPen, (float)dX1, (float)dY1, (float)dX2, (float)dY2);
				}
				int GDIPLUS_DrawRectangle(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetPenAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Pen* pPen = oSettings.GetPen().GetPen();

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					return GDIPLUS_DrawRectangle(pGraphics, pPen, fLeft, fTop, fWidth, fHeight);
				}
				int GDIPLUS_FillRectangle(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetBrushAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					Gdiplus::Brush* pBrush = NULL;

					if (oSettings.GetBrush().GetRectable())
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oSettings.GetBrush().GetRect());
					else
					{
						Gdiplus::RectF oRectGradient(fLeft, fTop, fWidth, fHeight);

						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);
					}

					return GDIPLUS_FillRectangle(pGraphics, pBrush, fLeft, fTop, fWidth, fHeight);
				}
				int GDIPLUS_DrawEllipse(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetPenAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Pen* pPen = oSettings.GetPen().GetPen();

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					return GDIPLUS_DrawEllipse(pGraphics, pPen, fLeft, fTop, fWidth, fHeight);
				}
				int GDIPLUS_FillEllipse(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetBrushAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					Gdiplus::Brush* pBrush = NULL;

					if (oSettings.GetBrush().GetRectable())
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oSettings.GetBrush().GetRect());
					else
					{
						Gdiplus::RectF oRectGradient(fLeft, fTop, fWidth, fHeight);

						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);
					}

					return GDIPLUS_FillEllipse(pGraphics, pBrush, fLeft, fTop, fWidth, fHeight);
				}
				int GDIPLUS_FillInverseRectangle(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::RectF& oRectImage, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetBrushAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					Gdiplus::Brush* pBrush = NULL;

					if (oSettings.GetBrush().GetRectable())
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oSettings.GetBrush().GetRect());
					else
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectImage);

					return GDIPLUS_FillInverseRectangle(pGraphics, pBrush, oRectImage, fLeft, fTop, fWidth, fHeight);
				}
				int GDIPLUS_FillInverseEllipse(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::RectF& oRectImage, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetBrushAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					Gdiplus::Brush* pBrush = NULL;

					if (oSettings.GetBrush().GetRectable())
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oSettings.GetBrush().GetRect());
					else
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectImage);

					return GDIPLUS_FillInverseEllipse(pGraphics, pBrush, oRectImage, fLeft, fTop, fWidth, fHeight);
				}
				int GDIPLUS_DrawPolyline(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::PointF* pPoints, int nCount)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetPenAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Pen* pPen = oSettings.GetPen().GetPen();

					return GDIPLUS_DrawPolyline(pGraphics, pPen, pPoints, nCount);
				}
				int GDIPLUS_DrawPolygon(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::PointF* pPoints, int nCount)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetPenAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Pen* pPen = oSettings.GetPen().GetPen();

					return GDIPLUS_DrawPolygon(pGraphics, pPen, pPoints, nCount);
				}
				int GDIPLUS_FillPolygon(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::PointF* pPoints, int nCount)
				{
					if (NULL == pPoints || nCount < 1)
						return c_nErrorNone;

					if (oSettings.GetBrushAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Brush* pBrush = NULL;

					//if (oSettings.GetBrush().GetRectable())
					//	pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oSettings.GetBrush().GetRect());
					//else
					{
						float fLeft = pPoints[0].X;
						float fTop = pPoints[0].Y;
						float fRight = pPoints[0].X;
						float fBottom = pPoints[0].Y;

						// находим объемлющий прямоугольник для polygon (только для градиентных brush'ей) 
						//if (Painter::IsBrushTypeGradient(oSettings.GetBrush().GetType()))
						{
							for (int index = 1; index < nCount; ++index)
							{
								fLeft = min(fLeft, pPoints[index].X);
								fTop = min(fTop, pPoints[index].Y);
								fRight = max(fRight, pPoints[index].X);
								fBottom = max(fBottom, pPoints[index].Y);
							}
						}

						Gdiplus::RectF oRectGradient(fLeft, fTop, fRight - fLeft, fBottom - fTop);

						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);
					}

					return GDIPLUS_FillPolygon(pGraphics, pBrush, pPoints, nCount);
				}
				int GDIPLUS_DrawPie(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetPenAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Pen* pPen = oSettings.GetPen().GetPen();

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					return GDIPLUS_DrawPie(pGraphics, pPen, fLeft, fTop, fWidth, fHeight, (float)dStartAngle, (float)dSweepAngle);
				}
				int GDIPLUS_FillPie(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetBrushAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					Gdiplus::Brush* pBrush = NULL;

					if (oSettings.GetBrush().GetRectable())
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oSettings.GetBrush().GetRect());
					else
					{
						Gdiplus::RectF oRectGradient(fLeft, fTop, fWidth, fHeight);

						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);
					}

					return GDIPLUS_FillPie(pGraphics, pBrush, fLeft, fTop, fWidth, fHeight, (float)dStartAngle, (float)dSweepAngle);
				}
				int GDIPLUS_DrawSector(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetPenAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					Gdiplus::Pen* pPen = oSettings.GetPen().GetPen();

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					return GDIPLUS_DrawSector(pGraphics, pPen, fLeft, fTop, fWidth, fHeight, (float)dStartAngle, (float)dSweepAngle, (float)dRatio);
				}
				int GDIPLUS_FillSector(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					if (oSettings.GetBrushAntiAlias())
						pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
					else
						pGraphics->SetSmoothingMode(SmoothingModeNone);

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					Gdiplus::Brush* pBrush = NULL;

					if (oSettings.GetBrush().GetRectable())
						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oSettings.GetBrush().GetRect());
					else
					{
						Gdiplus::RectF oRectGradient(fLeft, fTop, fWidth, fHeight);

						if (dRatio > 1.0)
						{
							oRectGradient.X = (float)(fLeft + 0.5f*oRectGradient.Width - 0.5*dRatio*oRectGradient.Width);
							oRectGradient.Y = (float)(fTop + 0.5f*oRectGradient.Height - 0.5*dRatio*oRectGradient.Height);
							oRectGradient.Width = (float)(dRatio*oRectGradient.Width);
							oRectGradient.Height = (float)(dRatio*oRectGradient.Height);
						}

						pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);
					}

					return GDIPLUS_FillSector(pGraphics, pBrush, fLeft, fTop, fWidth, fHeight, (float)dStartAngle, (float)dSweepAngle, (float)dRatio);
				}
				int GDIPLUS_DrawSimplePie(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius;
					double dTop = dCenterY - dRadius;
					double dRight = dCenterX + dRadius;
					double dBottom = dCenterY + dRadius;

					return GDIPLUS_DrawPie(pGraphics, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int GDIPLUS_FillSimplePie(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius;
					double dTop = dCenterY - dRadius;
					double dRight = dCenterX + dRadius;
					double dBottom = dCenterY + dRadius;

					return GDIPLUS_FillPie(pGraphics, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int GDIPLUS_DrawSimpleSector(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius1, double dRadius2, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius1;
					double dTop = dCenterY - dRadius1;
					double dRight = dCenterX + dRadius1;
					double dBottom = dCenterY + dRadius1;

					return GDIPLUS_DrawSector(pGraphics, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRadius2/max(0.0001, dRadius1));
				}
				int GDIPLUS_FillSimpleSector(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius1, double dRadius2, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius1;
					double dTop = dCenterY - dRadius1;
					double dRight = dCenterX + dRadius1;
					double dBottom = dCenterY + dRadius1;

					return GDIPLUS_FillSector(pGraphics, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRadius2/max(0.0001, dRadius1));
				}
				int GDIPLUS_DrawText(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					int nTextLength = strText.GetLength();
					if (nTextLength < 1)
						return c_nErrorNone;

					TextRenderingHint nOldHint = pGraphics->GetTextRenderingHint();
					if (oSettings.GetFormat().GetAntiAliasText())
						pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
					else
						pGraphics->SetTextRenderingHint(TextRenderingHintSystemDefault);

					BSTR bstrText = strText.AllocSysString();

					// подсчитываем размер текста на экране для того, чтобы сделать корректную градиентную заливку
					Gdiplus::RectF oRectGradient; oSettings.GetBrush().GetTextRect(bstrText, pGraphics, oSettings.GetFont().GetFont(), (float)dLeft, (float)dTop, &oRectGradient);

					// создаем соответствующий brush
					Gdiplus::Brush* pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);

					// рисуем текст в заданной позиции
					Gdiplus::PointF oPointText((float)dLeft, (float)dTop);
					pGraphics->DrawString(bstrText, nTextLength, oSettings.GetFont().GetFont(), oPointText, pBrush);

					SysFreeString(bstrText);

					pGraphics->SetTextRenderingHint(nOldHint);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawText(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					int nTextLength = strText.GetLength();
					if (nTextLength < 1)
						return c_nErrorNone;

					TextRenderingHint nOldHint = pGraphics->GetTextRenderingHint();
					if (oSettings.GetFormat().GetAntiAliasText())
						pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
					else
						pGraphics->SetTextRenderingHint(TextRenderingHintSystemDefault);

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					Gdiplus::RectF oRectGradient(fLeft, fTop, fWidth, fHeight);

					Gdiplus::Brush* pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);

					int nResult = GDIPLUS_DrawText(pGraphics, pBrush, oSettings.GetFont().GetFont(), oSettings.GetFormat().GetStringFormat(), strText, fLeft, fTop, fWidth, fHeight);

					pGraphics->SetTextRenderingHint(nOldHint);

					return nResult;
				}
				int GDIPLUS_DrawTextScale(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dRight, double dBottom)
				{
					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					int nTextLength = strText.GetLength();
					if (nTextLength < 1)
						return c_nErrorNone;

					TextRenderingHint nOldHint = pGraphics->GetTextRenderingHint();
					if (oSettings.GetFormat().GetAntiAliasText())
						pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
					else
						pGraphics->SetTextRenderingHint(TextRenderingHintSystemDefault);

					BSTR bstrText = strText.AllocSysString();

					float fLeft = (float)min(dLeft, dRight);
					float fTop = (float)min(dTop, dBottom);
					float fWidth = (float)fabs(dLeft - dRight);
					float fHeight = (float)fabs(dTop - dBottom);

					// вычисляем прямоугольник, описанный около текста без учета растяжения
					Gdiplus::PointF oPointCorner(0, 0);
					Gdiplus::RectF oRectTextNormal;
					pGraphics->MeasureString(bstrText, nTextLength, oSettings.GetFont().GetFont(), oPointCorner, &oRectTextNormal);

					// вычисляем прямоугольник, в котором будем отрисовывать текст
					Gdiplus::RectF oRectTextScaled(fLeft, fTop, fWidth, fHeight);

					// запоминаем текущее состояние pGraphics, чтобы после трансформаций и отрисовок вернуть state назадз
					Gdiplus::GraphicsState oState = pGraphics->Save();

					// выполняем преобразование координат для растяжения текста
					pGraphics->TranslateTransform(oRectTextScaled.X, oRectTextScaled.Y);
					pGraphics->ScaleTransform(oRectTextScaled.Width/oRectTextNormal.Width, oRectTextScaled.Height/oRectTextNormal.Height);

					// подсчитываем размер текста на экране для того, чтобы сделать корректную градиентную заливку
					Gdiplus::RectF oRectGradient; oSettings.GetBrush().GetTextRect(bstrText, pGraphics, oSettings.GetFont().GetFont(), fLeft, fTop, &oRectGradient);

					// создаем соответствующий brush
					oRectGradient.X = 0;
					oRectGradient.Y = 0;
					Gdiplus::Brush* pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);

					// отрисовываем текст
					pGraphics->DrawString(bstrText, nTextLength, oSettings.GetFont().GetFont(), oRectTextNormal, oSettings.GetFormat().GetStringFormat(), pBrush);

					// удаляем ненужную память
					SysFreeString(bstrText);

					// возвращаем старое состояние pGraphics
					pGraphics->Restore(oState);

					pGraphics->SetTextRenderingHint(nOldHint);

					return c_nErrorNone;
				}
				int GDIPLUS_DrawCredits(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, float fLeft, float fTop, float fWidth, float fHeight, double dCompleteness, int nType)
				{
					/* @desc:
					int GDIPLUS_DrawCredits(Gdiplus::Graphics* pGraphics, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, float fLeft, float fTop, float fWidth, float fHeight, double dCompleteness, int nType)
					{
						отрисовывает двигающийся текст (strText) в заданном месте (fLeft, fTop, fWidth, fHeight) указанным типом (nType) и стилем (oSettings) 
						в указанном (pGraphics)

						предполагается, что параметры Width и Height >= 0 
					}
					*/

					if (NULL == pGraphics)
						return c_nErrorInvalidPointer;

					// быстрый случай - ничего делать не надо...
					if (dCompleteness < 0)
						return c_nErrorNone;

					int nTextLength = strText.GetLength();
					if (nTextLength < 1)
						return c_nErrorNone;

					TextRenderingHint nOldHint = pGraphics->GetTextRenderingHint();
					if (oSettings.GetFormat().GetAntiAliasText())
						pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
					else
						pGraphics->SetTextRenderingHint(TextRenderingHintSystemDefault);

					BSTR bstrText = strText.AllocSysString();

					// подсчитываем размер тексте, чтобы правильно его спозиционировать в указанном прямоугольнике
					Gdiplus::PointF oPointStart(0, 0);
					Gdiplus::RectF oRectText(0, 0, 0, 0);
					pGraphics->MeasureString(bstrText, nTextLength, oSettings.GetFont().GetFont(), oPointStart, &oRectText);

					double dAlphaKoefficient = 1.0;

					// compute advanced colors and positions
					if (nType == c_nCreditsHMWBottom || nType == c_nCreditsHMWCenter || nType == c_nCreditsHMWTop || nType == c_nCreditsVMWLeft || nType == c_nCreditsVMWCenter || nType == c_nCreditsVMWRight)
					{
						if (dCompleteness < 0.25)
							dCompleteness = 2*dCompleteness;
						else if (dCompleteness > 0.75)
							dCompleteness = 0.5 + 2*(dCompleteness - 0.75);
						else
							dCompleteness = 0.5;
					}
					else if (nType == c_nCreditsHMHBottom || nType == c_nCreditsHMHCenter || nType == c_nCreditsHMHTop || nType == c_nCreditsVMHLeft || nType == c_nCreditsVMHCenter || nType == c_nCreditsVMHRight)
					{
						if (dCompleteness > 0.5)
						{
							if (dCompleteness > 0.75)
								dAlphaKoefficient = (1.0 - dCompleteness)/0.25;

							dCompleteness = 0.5;
						}
					}
					else if (nType == c_nCreditsHMSBottom || nType == c_nCreditsHMSCenter || nType == c_nCreditsHMSTop || nType == c_nCreditsVMSLeft || nType == c_nCreditsVMSCenter || nType == c_nCreditsVMSRight)
					{
						dCompleteness *= 0.5;
					}
					else if (nType == c_nCreditsHHMBottom || nType == c_nCreditsHHMCenter || nType == c_nCreditsHHMTop || nType == c_nCreditsVHMLeft || nType == c_nCreditsVHMCenter || nType == c_nCreditsVHMRight)
					{
						if (dCompleteness < 0.5)
						{
							if (dCompleteness < 0.25)
								dAlphaKoefficient = dCompleteness/0.25;

							dCompleteness = 0.5;
						}
					}

					Gdiplus::PointF oPointText;

					// подсчитываем место, в котором будем отрисовывать текст
					if (nType == c_nCreditsHMMBottom || nType == c_nCreditsHMWBottom || nType == c_nCreditsHMHBottom || nType == c_nCreditsHMSBottom || nType == c_nCreditsHHMBottom)
					{
						oPointText.X = fLeft + (REAL)((fWidth + 1)*(1.0 - dCompleteness) + dCompleteness*(-oRectText.GetRight() - 1));
						oPointText.Y = fTop + (REAL)(fHeight - 1 - oRectText.Height);
					}
					if (nType == c_nCreditsHMMCenter || nType == c_nCreditsHMWCenter || nType == c_nCreditsHMHCenter || nType == c_nCreditsHMSCenter || nType == c_nCreditsHHMCenter)
					{
						oPointText.X = fLeft + (REAL)((fWidth + 1)*(1.0 - dCompleteness) + dCompleteness*(-oRectText.GetRight() - 1));
						oPointText.Y = fTop + (REAL)((0.5f*fHeight) - 0.5*oRectText.Height);
					}
					if (nType == c_nCreditsHMMTop || nType == c_nCreditsHMWTop || nType == c_nCreditsHMHTop || nType == c_nCreditsHMSTop || nType == c_nCreditsHHMTop)
					{
						oPointText.X = fLeft + (REAL)((fWidth + 1)*(1.0 - dCompleteness) + dCompleteness*(-oRectText.GetRight() - 1));
						oPointText.Y = fTop + (REAL)1.0f;
					}
					if (nType == c_nCreditsVMMLeft || nType == c_nCreditsVMWLeft || nType == c_nCreditsVMHLeft || nType == c_nCreditsVMSLeft || nType == c_nCreditsVHMLeft)
					{
						oPointText.X = fLeft + (REAL)1.0f;
						oPointText.Y = fTop + (REAL)((fHeight + 1)*(1.0 - dCompleteness) + dCompleteness*(-oRectText.GetBottom() - 1));
					}
					if (nType == c_nCreditsVMMCenter || nType == c_nCreditsVMWCenter || nType == c_nCreditsVMHCenter || nType == c_nCreditsVMSCenter || nType == c_nCreditsVHMCenter)
					{
						oPointText.X = fLeft + (REAL)(0.5*(fWidth - 1) - 0.5*oRectText.Width);
						oPointText.Y = fTop + (REAL)((fHeight + 1)*(1.0 - dCompleteness) + dCompleteness*(-oRectText.GetBottom() - 1));
					}
					if (nType == c_nCreditsVMMRight || nType == c_nCreditsVMWRight || nType == c_nCreditsVMHRight || nType == c_nCreditsVMSRight || nType == c_nCreditsVHMRight)
					{
						oPointText.X = fLeft + (REAL)(fWidth - 1 - oRectText.Width - 1);
						oPointText.Y = fTop + (REAL)((fHeight + 1)*(1.0 - dCompleteness) + dCompleteness*(-oRectText.GetBottom() - 1));
					}

					// делаем текст полупрозрачным, если это нужно
					oSettings.GetBrush().SetAlpha1((long)(oSettings.GetBrush().GetAlpha1()*dAlphaKoefficient));
					oSettings.GetBrush().SetAlpha2((long)(oSettings.GetBrush().GetAlpha2()*dAlphaKoefficient));
					oSettings.GetBrush().SetTextureAlpha((long)(oSettings.GetBrush().GetTextureAlpha()*dAlphaKoefficient));

					// создаем соответствующий brush
					Gdiplus::RectF oRectGradient(fLeft, fTop, fWidth, fHeight);
					Gdiplus::Brush* pBrush = oSettings.GetBrush().GetBrush(&oSettings.GetTextureManager(), &oRectGradient);

					// отрисовываем текст в указанной позиции
					pGraphics->DrawString(bstrText, nTextLength, oSettings.GetFont().GetFont(), oPointText, pBrush);

					SysFreeString(bstrText);

					pGraphics->SetTextRenderingHint(nOldHint);

					return c_nErrorNone;
				}
			}
		}
	}
}