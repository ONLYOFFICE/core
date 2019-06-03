/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _METAFILE_COMMON_METAFILERENDERER_H
#define _METAFILE_COMMON_METAFILERENDERER_H

#include "../../../graphics/IRenderer.h"
#include "../../../graphics/structures.h"
#include "../../../graphics/Image.h"
#include "../../../raster/ImageFileFormatChecker.h"
#include "../../../raster/BgraFrame.h"
#include "../../../graphics/AggPlusEnums.h"

#include "IOutputDevice.h"
#include "MetaFile.h"
#include "MetaFileTypes.h"
#include "MetaFileObjects.h"

#include <cmath>

#ifndef M_PI
	#define M_PI 3.1415926
#endif

namespace MetaFile
{
	class CEmfClip;

	class CMetaFileRenderer : public IOutputDevice
	{
	public:
		CMetaFileRenderer(IMetaFileBase *pFile, IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
		{
			m_pFile = pFile;

			m_dX = dX;
			m_dY = dY;
			m_dW = dWidth;
			m_dH = dHeight;

			m_pRenderer = NULL;

			if (!pRenderer)
				return;

			m_pRenderer = pRenderer;

			TRect* pBounds = m_pFile->GetDCBounds();
			int nL = pBounds->nLeft;
			int nR = pBounds->nRight;
			int nT = pBounds->nTop;
			int nB = pBounds->nBottom;

			m_dScaleX = (nR - nL <= 0) ? 1 : m_dW / (double)(nR - nL);
			m_dScaleY = (nB - nT <= 0) ? 1 : m_dH / (double)(nB - nT);

			m_bStartedPath = false;

			//int alpha = 0xff;
			//m_pRenderer->put_BrushAlpha1(alpha);
			//m_pRenderer->put_BrushType(c_BrushTypeSolid);
			//m_pRenderer->put_BrushColor1(0xffffff);

			//m_pRenderer->BeginCommand(c_nPathType);
			//m_pRenderer->PathCommandStart();
			//m_pRenderer->PathCommandMoveTo(pBounds->nLeft	, pBounds->nTop);
			//m_pRenderer->PathCommandLineTo(pBounds->nRight	, pBounds->nTop);
			//m_pRenderer->PathCommandLineTo(pBounds->nRight	, pBounds->nBottom);
			//m_pRenderer->PathCommandLineTo(pBounds->nLeft	, pBounds->nBottom);
			//m_pRenderer->PathCommandLineTo(pBounds->nLeft	, pBounds->nTop);
			//m_pRenderer->PathCommandClose();
			//m_pRenderer->DrawPath(c_nWindingFillMode);
			//m_pRenderer->EndCommand(c_nPathType);
			//m_pRenderer->PathCommandEnd();
		}
		~CMetaFileRenderer()
		{
		}

		void Begin()
		{
			if (m_pFile)
			{
				TRect* pBounds = m_pFile->GetDCBounds();
				int nL = pBounds->nLeft;
				int nR = pBounds->nRight;
				int nT = pBounds->nTop;
				int nB = pBounds->nBottom;

				m_dScaleX = (nR - nL <= 0) ? 1 : m_dW / (double)(nR - nL);
				m_dScaleY = (nB - nT <= 0) ? 1 : m_dH / (double)(nB - nT);
			}
		}
		void End()
		{
			CheckEndPath();
		}

		void DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight)
		{
			if (!pBuffer || 0 == unWidth || 0 == unHeight)
				return;

			CheckEndPath();

			UpdateTransform();
			UpdateClip();

			Aggplus::CImage oImage;
			BYTE* pBufferPtr = new BYTE[4 * unWidth * unHeight];
			oImage.Create(pBufferPtr, unWidth, unHeight, 4 * unWidth);

			for (int nIndex = 0, nSize = 4 * unWidth * unHeight; nIndex < nSize; nIndex += 4)
			{
				pBufferPtr[0] = (unsigned char)pBuffer[nIndex + 0];
				pBufferPtr[1] = (unsigned char)pBuffer[nIndex + 1];
				pBufferPtr[2] = (unsigned char)pBuffer[nIndex + 2];
				pBufferPtr[3] = (unsigned char)pBuffer[nIndex + 3];
				pBufferPtr += 4;
			}

			TPointD oTL = TranslatePoint(dX, dY);
			TPointD oBR = TranslatePoint(dX + dW, dY + dH);

			double dImageX = oTL.x;
			double dImageY = oTL.y;
			double dImageW = oBR.x - oTL.x;
			double dImageH = oBR.y - oTL.y;

			if (dImageH < 0 || dImageW < 0)
			{
				double dM11, dM12, dM21, dM22, dMx, dMy;
				m_pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dMx, &dMy);

				double dKx = 1, dKy = 1, dShiftKoefX = 0, dShiftKoefY = 0;
				if (dImageW < 0)
				{
					dKx = -1;
					dShiftKoefX = 2 * dImageX + dImageW;

					dImageW = -dImageW;
					dImageX -= dImageW;
				}

				if (dImageH < 0)
				{
					dKy = -1;
					dShiftKoefY = 2 * dImageY + dImageH;

					dImageH = -dImageH;
					dImageY -= dImageH;
				}

				m_pRenderer->SetTransform(dKx * dM11, dKx * dM12, dKy * dM21, dKy * dM22, dShiftKoefX * dM11 + dShiftKoefY * dM21 + dMx, dShiftKoefX * dM12 + dShiftKoefY * dM22 + dMy);
			}

			m_pRenderer->DrawImage(&oImage, dImageX, dImageY, dImageW, dImageH);
		}
		void DrawString(std::wstring& wsText, unsigned int unCharsCount, double _dX, double _dY, double* pDx, int iGraphicsMode)
		{
			CheckEndPath();

			IFont* pFont = m_pFile->GetFont();
			if (!pFont)
				return;

			UpdateClip();
			UpdateTransform(iGraphicsMode);

			int lLogicalFontHeight = pFont->GetHeight();
			if (lLogicalFontHeight < 0)
				lLogicalFontHeight = -lLogicalFontHeight;
			if (lLogicalFontHeight < 0.01)
				lLogicalFontHeight = 18;

			double dFontHeight = fabs(lLogicalFontHeight * m_dScaleY / 25.4 * 72);

			std::wstring wsFaceName = pFont->GetFaceName();
			m_pRenderer->put_FontName(wsFaceName);
			m_pRenderer->put_FontSize(dFontHeight);

			int lStyle = 0;
			if (pFont->GetWeight() > 550)
				lStyle |= 0x01;
			if (pFont->IsItalic())
				lStyle |= 0x02;
			if (pFont->IsUnderline())
				lStyle |= (1 << 2);
			if (pFont->IsStrikeOut())
				lStyle |= (1 << 7);

			m_pRenderer->put_FontStyle(lStyle);

			double dTheta = -((((double)pFont->GetEscapement()) / 10) * M_PI / 180);

			double dCosTheta = (float)cos(dTheta);
			double dSinTheta = (float)sin(dTheta);

			float fL = 0, fT = 0, fW = 0, fH = 0;
			float fUndX1 = 0, fUndY1 = 0, fUndX2 = 0, fUndY2 = 0, fUndSize = 1;

			double dFontCharSpace = m_pFile->GetCharSpace() * m_dScaleX * m_pFile->GetPixelWidth();
			m_pRenderer->put_FontCharSpace(dFontCharSpace);
			CFontManager* pFontManager = m_pFile->GetFontManager();
			if (pFontManager)
			{
				pFontManager->LoadFontByName(wsFaceName, dFontHeight, lStyle, 72, 72);
				pFontManager->SetCharSpacing(dFontCharSpace * 72 / 25.4);

                double dMmToPt = 25.4 / 72;

                double dFHeight = dFontHeight;
                double dFDescent = dFontHeight;
                if (pFontManager->m_pFont)
                {
                    dFHeight  *= pFontManager->m_pFont->GetHeight() / pFontManager->m_pFont->m_lUnits_Per_Em * dMmToPt;
                    dFDescent *= pFontManager->m_pFont->GetDescender() / pFontManager->m_pFont->m_lUnits_Per_Em * dMmToPt;
                }
                double dFAscent  = dFHeight - std::abs(dFDescent);

				if (NULL != pDx && unCharsCount > 1)
				{
					// Тогда мы складываем все pDx кроме последнего символа, последний считаем отдельно
					double dTempTextW = 0;
					for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount - 1; unCharIndex++)
					{
						dTempTextW += pDx[unCharIndex];
					}
					dTempTextW *= m_dScaleX;

					std::wstring wsTempText;
					wsTempText += wsText.at(wsText.length() - 1);
					//wsTempText += wsText.at(unCharsCount - 1);

					pFontManager->LoadString1(wsTempText, 0, 0);
					TBBox oBox = pFontManager->MeasureString2();
					dTempTextW += dMmToPt * (oBox.fMaxX - oBox.fMinX);

					fL = 0;
					fW = (float)dTempTextW;
				}
				else
				{
					pFontManager->LoadString1(wsText, 0, 0);
					TBBox oBox = pFontManager->MeasureString2();
					fL = (float)dMmToPt * (oBox.fMinX);
					fW = (float)dMmToPt * (oBox.fMaxX - oBox.fMinX);
				}

				// Просчитаем положение подчеркивания
				pFontManager->GetUnderline(&fUndX1, &fUndY1, &fUndX2, &fUndY2, &fUndSize);
				fUndY1   *= (float)dMmToPt;
				fUndY2   *= (float)dMmToPt;
				fUndSize *= (float)dMmToPt / 2;

				fUndX1 = fL;
				fUndX2 = fL + fW;


				fT = (float)-dFAscent;
				fH = (float)dFHeight;
			}

			TPointD oTextPoint = TranslatePoint(_dX, _dY);
			double dX = oTextPoint.x;
			double dY = oTextPoint.y;

			// Найдем начальную точку текста
			unsigned int ulTextAlign = m_pFile->GetTextAlign();
			if (ulTextAlign & TA_BASELINE)
			{
				// Ничего не делаем
			}
			else if (ulTextAlign & TA_BOTTOM)
			{
				float fTemp = -(-fT + fH);

				dX += -fTemp * dSinTheta;
				dY +=  fTemp * dCosTheta;
			}
			else // if (ulTextAlign & TA_TOP)
			{
				float fTemp = -fT;

				dX += -fTemp * dSinTheta;
				dY +=  fTemp * dCosTheta;
			}

			if (ulTextAlign & TA_CENTER)
			{
				dX += -fW / 2 * dCosTheta;
				dY += -fW / 2 * dSinTheta;
			}
			else if (ulTextAlign & TA_RIGHT)
			{
				dX += -fW * dCosTheta;
				dY += -fW * dSinTheta;
			}
			else //if (ulTextAlign & TA_LEFT)
			{
				// Ничего не делаем
			}

			if (pFont->IsUnderline())
			{
				fUndX1 += (float)dX;
				fUndX2 += (float)dX;
				fUndY1 += (float)dY;
				fUndY2 += (float)dY;
			}

			bool bChangeCTM = false;

			if (iGraphicsMode == GM_COMPATIBLE)
			{
				// В данной ситуации матрица преобразования должна быть диагональной, в ней могут быть только отражения,
				// которые в данном случае нужно проправить

				double dM11, dM12, dM21, dM22, dRx, dRy;
				m_pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

				double dShiftX = 0;
				double dShiftY = 0;

				// Нам нужно знать в следствие чего происходит флип, из-за Window или Viewport
				if (dM11 < -0.00001)
				{
					dX -= fabs(fW);

					if (m_pFile->IsWindowFlippedX())
					{
						dShiftX = (2 * dX + fabs(fW)) * dM11;
					}
					else
					{
						dShiftX = (2 * dX - fabs(fW)) * dM11;
					}
				}

				if (dM22 < - 0.00001)
				{
					dY -= fabs(fH);
					if (m_pFile->IsWindowFlippedY())
					{
						dShiftY = (2 * dY + fabs(fH)) * dM22;
					}
					else
					{
						dShiftY = (2 * dY - fabs(fH)) * dM22;
					}
				}

				m_pRenderer->ResetTransform();
				m_pRenderer->SetTransform(fabs(dM11), 0, 0, fabs(dM22), dShiftX + dRx, dShiftY + dRy);

				bChangeCTM = true;
			}

			if (0 != pFont->GetEscapement())
			{
				// TODO: тут реализован только параметр shEscapement, еще нужно реализовать параметр Orientation
				m_pRenderer->SetTransform(dCosTheta, dSinTheta, -dSinTheta, dCosTheta, dX - dX * dCosTheta + dY * dSinTheta, dY - dX * dSinTheta - dY * dCosTheta);
				bChangeCTM = true;
			}

			// Для начала нарисуем фон текста
			if (OPAQUE == m_pFile->GetTextBgMode())
			{
				m_pRenderer->put_BrushType(c_BrushTypeSolid);
				m_pRenderer->put_BrushAlpha1(255);
				m_pRenderer->put_BrushColor1(m_pFile->GetTextBgColor());

				m_pRenderer->BeginCommand(c_nPathType);
				m_pRenderer->PathCommandStart();
				m_pRenderer->PathCommandMoveTo(dX + fL, dY + fT);
				m_pRenderer->PathCommandLineTo(dX + fL + fW, dY + fT);
				m_pRenderer->PathCommandLineTo(dX + fL + fW, dY + fT + fH);
				m_pRenderer->PathCommandLineTo(dX + fL, dY + fT + fH);
				m_pRenderer->PathCommandClose();
				m_pRenderer->DrawPath(c_nWindingFillMode);
				m_pRenderer->EndCommand(c_nPathType);
				m_pRenderer->PathCommandEnd();
			}

			// Нарисуем подчеркивание 
			if (pFont->IsUnderline())
			{
				m_pRenderer->put_PenSize((double)fUndSize);
				m_pRenderer->put_PenLineEndCap(0);
				m_pRenderer->put_PenLineStartCap(0);

				m_pRenderer->BeginCommand(c_nPathType);
				m_pRenderer->PathCommandStart();
				m_pRenderer->PathCommandMoveTo(fUndX1, fUndY1);
				m_pRenderer->PathCommandLineTo(fUndX2, fUndY2);
				m_pRenderer->DrawPath(c_nStroke);
				m_pRenderer->EndCommand(c_nPathType);
				m_pRenderer->PathCommandEnd();
			}

			// Установим цвет текста
			m_pRenderer->put_BrushType(c_BrushTypeSolid);
			m_pRenderer->put_BrushColor1(m_pFile->GetTextColor());
			m_pRenderer->put_BrushAlpha1(255);

			// Рисуем сам текст

			if (NULL == pDx)
			{
                m_pRenderer->CommandDrawText(wsText, dX, dY, 0, 0);
			}
			else
			{
				unsigned int unUnicodeLen = 0;
				unsigned int* pUnicode = NSStringExt::CConverter::GetUtf32FromUnicode(wsText, unUnicodeLen);
				if (pUnicode && unUnicodeLen)
				{
					double dOffset = 0;
					double dKoefX = m_dScaleX;
					for (unsigned int unCharIndex = 0; unCharIndex < unUnicodeLen; unCharIndex++)
					{
						std::wstring wsChar = NSStringExt::CConverter::GetUnicodeFromUTF32(&*(pUnicode + unCharIndex), 1);
						m_pRenderer->CommandDrawText(wsChar, dX + dOffset, dY, 0, 0);
						dOffset += (pDx[unCharIndex] * dKoefX);
					}

					delete[] pUnicode;
				}
			}
			

			if (bChangeCTM)
				m_pRenderer->ResetTransform();
		}
		void StartPath()
		{
			CheckEndPath();

			UpdateClip();
			UpdateTransform();

			m_lDrawPathType = -1;
			if (true == UpdateBrush())
			{
				unsigned int unFillMode = m_pFile->GetFillMode();
				if (ALTERNATE == unFillMode)
					m_lDrawPathType = c_nEvenOddFillMode;
				else// if (WINDING == unFillMode)
					m_lDrawPathType = c_nWindingFillMode;
			}

			if (true == UpdatePen())
			{
				if (-1 == m_lDrawPathType)
					m_lDrawPathType = c_nStroke;
				else
					m_lDrawPathType |= c_nStroke;
			}

			m_pRenderer->BeginCommand(c_nPathType);
			m_pRenderer->PathCommandStart();

			m_bStartedPath = true;
		}
		void MoveTo(double dX, double dY)
		{
			CheckStartPath(true);
			TPointD oPoint = TranslatePoint(dX, dY);
			m_pRenderer->PathCommandMoveTo(oPoint.x, oPoint.y);
		}
		void LineTo(double dX, double dY)
		{
			CheckStartPath(false);
			TPointD oPoint = TranslatePoint(dX, dY);
			m_pRenderer->PathCommandLineTo(oPoint.x, oPoint.y);
		}
		void CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe)
		{
			CheckStartPath(false);

			TPointD oPoint1 = TranslatePoint(dX1, dY1);
			TPointD oPoint2 = TranslatePoint(dX2, dY2);
			TPointD oPointE = TranslatePoint(dXe, dYe);
			m_pRenderer->PathCommandCurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
		}
		void ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStart, double dSweep)
		{
			CheckStartPath(false);

			TPointD oTL = TranslatePoint(dLeft, dTop);
			TPointD oBR = TranslatePoint(dRight, dBottom);
			m_pRenderer->PathCommandArcTo(oTL.x, oTL.y, oBR.x - oTL.x, oBR.y - oTL.y, dStart, dSweep);
		}
		void ClosePath()
		{
			CheckStartPath(false);

			m_pRenderer->PathCommandClose();
		}
		void DrawPath(int lType = 0)
		{
			if (lType <= 0)
			{
				if (-1 != m_lDrawPathType)
					m_pRenderer->DrawPath(m_lDrawPathType);
			}
			else if (-1 != m_lDrawPathType)
			{
				bool bStroke = lType & 1 ? true : false;
				bool bFill   = lType & 2 ? true : false;

				int nEndType = -1;

				if (bStroke && (m_lDrawPathType & c_nStroke))
					nEndType = c_nStroke;

				if (bFill)
				{
					if (m_lDrawPathType & c_nWindingFillMode)
						nEndType = (-1 == nEndType ? c_nWindingFillMode : nEndType | c_nWindingFillMode);
					else if (m_lDrawPathType & c_nEvenOddFillMode)
						nEndType = (-1 == nEndType ? c_nEvenOddFillMode : nEndType | c_nEvenOddFillMode);
				}

				if (-1 != nEndType)
					m_pRenderer->DrawPath(nEndType);
			}
		}
		void EndPath()
		{
			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->PathCommandEnd();

			m_bStartedPath = false;
		}
		void UpdateDC()
		{
			CheckEndPath();
		}
		void ResetClip()
		{
			m_pRenderer->BeginCommand(c_nResetClipType);
			m_pRenderer->EndCommand(c_nResetClipType);
		}
		void IntersectClip(double dLeft, double dTop, double dRight, double dBottom)
		{
			m_pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);

			m_pRenderer->BeginCommand(c_nClipType);
			m_pRenderer->BeginCommand(c_nPathType);
			m_pRenderer->PathCommandStart();

			TPointD oTL = TranslatePoint(dLeft, dTop);
			TPointD oBR = TranslatePoint(dRight, dBottom);

			m_pRenderer->PathCommandMoveTo(oTL.x, oTL.y);
			m_pRenderer->PathCommandLineTo(oTL.x, oBR.y);
			m_pRenderer->PathCommandLineTo(oBR.x, oBR.y);
			m_pRenderer->PathCommandLineTo(oBR.x, oTL.y);
			m_pRenderer->PathCommandLineTo(oTL.x, oTL.y);

			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->EndCommand(c_nClipType);
			m_pRenderer->PathCommandEnd();
		}
		void StartClipPath(unsigned int unMode, int nFillMode = -1)
		{
			CheckEndPath();

			m_bStartedPath = true;

			unsigned int unClipMode = -1;
			switch (unMode)
			{
				case RGN_AND: unClipMode = c_nClipRegionIntersect; break;
				case RGN_OR: unClipMode = c_nClipRegionUnion; break;
				case RGN_XOR: unClipMode = c_nClipRegionXor; break;
				case RGN_DIFF: unClipMode = c_nClipRegionDiff; break;
				default: unClipMode = c_nClipRegionIntersect; break;
			}

			unsigned int unFillMode = -1 == nFillMode ? m_pFile->GetFillMode() : nFillMode;

			if (ALTERNATE == unFillMode)
				unClipMode |= c_nClipRegionTypeEvenOdd;
			else //if (WINDING == unFillMode)
				unClipMode |= c_nClipRegionTypeWinding;

			if (RGN_COPY == unMode)
				ResetClip();

			m_pRenderer->put_ClipMode(unClipMode);
			m_pRenderer->BeginCommand(c_nClipType);
			m_pRenderer->BeginCommand(c_nPathType);
			m_pRenderer->PathCommandStart();
		}
		void EndClipPath(unsigned int unMode)
		{
			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->EndCommand(c_nClipType);
			m_pRenderer->PathCommandEnd();

			m_bStartedPath = false;
		}		
		void SetTransform(double& dM11, double& dM12, double& dM21, double& dM22, double& dX, double& dY)
		{
			double dKoefX = m_dScaleX;
			double dKoefY = m_dScaleY;

			m_pRenderer->ResetTransform();
			m_pRenderer->SetTransform(dM11, dM12 * dKoefY / dKoefX, dM21 * dKoefX / dKoefY, dM22, dX * dKoefX, dY * dKoefY);
		}
		void GetTransform(double* pdM11, double* pdM12, double* pdM21, double* pdM22, double* pdX, double* pdY)
		{
			double dKoefX = m_dScaleX;
			double dKoefY = m_dScaleY;

			m_pRenderer->GetTransform(pdM11, pdM12, pdM21, pdM22, pdX, pdY);
			*pdM12 *= dKoefX / dKoefY;
			*pdM21 *= dKoefY / dKoefX;
			*pdX /= dKoefX;
			*pdY /= dKoefY;
		}

	private:

		void CheckStartPath(bool bMoveTo)
		{
			if (!m_bStartedPath)
			{
				StartPath();

				if (!bMoveTo)
				{
					TPointD oCurPos = m_pFile->GetCurPos();
					MoveTo(oCurPos.x, oCurPos.y);
				}
			}
		}
		void CheckEndPath()
		{
			if (m_bStartedPath)
			{
				DrawPath();
				EndPath();
			}
		}

		TPointD TranslatePoint(double dX, double dY)
		{
			TPointD oPoint;
			oPoint.x = m_dScaleX * dX + m_dX;
			oPoint.y = m_dScaleY * dY + m_dY;
			return oPoint;
		}
		bool UpdateBrush()
		{
			IBrush* pBrush = m_pFile->GetBrush();
			if (!pBrush)
				return false;

			unsigned int unBrushStyle = pBrush->GetStyle();
			if (BS_NULL == unBrushStyle)
				return false;
			else if (BS_DIBPATTERN == unBrushStyle)
			{
				m_pRenderer->put_BrushType(c_BrushTypeTexture);
				m_pRenderer->put_BrushTextureMode(c_BrushTextureModeTile);
				m_pRenderer->put_BrushTexturePath(pBrush->GetDibPatterPath());
			}
			else if (BS_HATCHED == unBrushStyle)
			{
				m_pRenderer->put_BrushType(c_BrushTypeHatch1);
				std::wstring wsBrushType = L"horz";

				switch (pBrush->GetHatch())
				{
				case HS_HORIZONTAL: wsBrushType = L"horz"; break;
				case HS_VERTICAL:   wsBrushType = L"vert"; break;
				case HS_FDIAGONAL:  wsBrushType = L"dnDiag"; break;
				case HS_BDIAGONAL:  wsBrushType = L"upDiag"; break;
				case HS_CROSS:      wsBrushType = L"cross"; break;
				case HS_DIAGCROSS:  wsBrushType = L"diagCross"; break;
				}

				// TODO: Непонятно почему, но в Hatch все цвета идут не как RGB, а как BGR
				if (TRANSPARENT == m_pFile->GetTextBgMode())
					m_pRenderer->put_BrushAlpha2(0);
				else
				{
					m_pRenderer->put_BrushAlpha2(255);

					TColor oBgColor(m_pFile->GetTextBgColor());
					m_pRenderer->put_BrushColor2(oBgColor.ToInt());
				}

				TColor oFgColor(pBrush->GetColor());
				m_pRenderer->put_BrushTexturePath(wsBrushType);
				m_pRenderer->put_BrushAlpha1(255);
				m_pRenderer->put_BrushColor1(oFgColor.ToInt());
			}
			else if (	BS_LINEARGRADIENT	== unBrushStyle ||
						BS_RECTGRADIENT		== unBrushStyle ||
						BS_PATHGRADIENT		== unBrushStyle
					)
			{
				m_pRenderer->put_BrushType(c_BrushTypePathGradient1);

				m_pRenderer->put_BrushColor1(pBrush->GetColor());
				m_pRenderer->put_BrushColor2(pBrush->GetColor2());
				m_pRenderer->put_BrushAlpha1(pBrush->GetAlpha());
				m_pRenderer->put_BrushAlpha2(pBrush->GetAlpha2());

				m_pRenderer->put_BrushLinearAngle(pBrush->GetStyleEx());

				long Colors[2];
				Colors[0] = (pBrush->GetColor()<<8) + pBrush->GetAlpha();
				Colors[1] = (pBrush->GetColor2()<<8) + pBrush->GetAlpha2();
				double Position[2] = {0, 1};

				m_pRenderer->put_BrushGradientColors(Colors,Position,2);

			}
			else if (	BS_RADIALGRADIENT	== unBrushStyle ||
						BS_AXIALGRADIENT	== unBrushStyle 
					)
			{
				m_pRenderer->put_BrushType(c_BrushTypePathGradient2);

				m_pRenderer->put_BrushColor1(pBrush->GetColor());
				m_pRenderer->put_BrushColor2(pBrush->GetColor2());
				m_pRenderer->put_BrushAlpha1(pBrush->GetAlpha());
				m_pRenderer->put_BrushAlpha2(pBrush->GetAlpha2());

				long Colors[2];
				Colors[0] = (pBrush->GetColor()<<8) + pBrush->GetAlpha();
				Colors[1] = (pBrush->GetColor2()<<8) + pBrush->GetAlpha2();
				double Position[2] = {0, 1};
				
				m_pRenderer->put_BrushGradientColors(Colors,Position,2);
			}

			else //if (BS_SOLID == unBrushStyle)
			{
				m_pRenderer->put_BrushType(c_BrushTypeSolid);
				m_pRenderer->put_BrushColor1(pBrush->GetColor());
				m_pRenderer->put_BrushAlpha1(pBrush->GetAlpha());
			}

			return true;
		}
		void UpdateTransform(int iGraphicsMode = GM_ADVANCED)
		{
			double dKoefX = m_dScaleX;
			double dKoefY = m_dScaleY;

			TEmfXForm* pMatrix = m_pFile->GetTransform(iGraphicsMode);
			m_pRenderer->ResetTransform();
			m_pRenderer->SetTransform(pMatrix->M11, pMatrix->M12 * dKoefY / dKoefX, pMatrix->M21 * dKoefX / dKoefY, pMatrix->M22, pMatrix->Dx * dKoefX, pMatrix->Dy * dKoefY);
		}
		bool UpdatePen()
		{
			IPen* pPen = m_pFile->GetPen();
			if (!pPen)
				return false;

			int nColor = pPen->GetColor();

			// TODO: dWidth зависит еще от флага PS_GEOMETRIC в стиле карандаша
			double dWidth = pPen->GetWidth() * m_dScaleX;
			if (dWidth <= 0.01)
				dWidth = 0;

			unsigned int unMetaPenStyle = pPen->GetStyle();
			unsigned int ulPenType   = unMetaPenStyle & PS_TYPE_MASK;
			unsigned int ulPenEndCap = unMetaPenStyle & PS_ENDCAP_MASK;
			unsigned int ulPenJoin   = unMetaPenStyle & PS_JOIN_MASK;
			unsigned int ulPenStyle  = unMetaPenStyle & PS_STYLE_MASK;

			BYTE nCapStyle = 0;
			if (PS_ENDCAP_ROUND == ulPenEndCap)
				nCapStyle = Aggplus::LineCapRound;
			else if (PS_ENDCAP_SQUARE == ulPenEndCap)
				nCapStyle = Aggplus::LineCapSquare;
			else if (PS_ENDCAP_FLAT == ulPenEndCap)
				nCapStyle = Aggplus::LineCapFlat;

			BYTE nJoinStyle = 0;
			if (PS_JOIN_ROUND == ulPenJoin)
				nJoinStyle = Aggplus::LineJoinRound;
			else if (PS_JOIN_BEVEL == ulPenJoin)
				nJoinStyle = Aggplus::LineJoinBevel;
			else if (PS_JOIN_MITER == ulPenJoin)
				nJoinStyle = Aggplus::LineJoinMiter;

			double dMiterLimit = m_pFile->GetMiterLimit() * m_dScaleX;

			// TODO: Реализовать PS_USERSTYLE
			BYTE nDashStyle = Aggplus::DashStyleSolid;;

			// В WinGDI все карандаши толщиной больше 1px рисуются в стиле PS_SOLID
			if (1 >= pPen->GetWidth() && PS_SOLID != ulPenStyle)
			{
				// TODO: Ранее здесь специально ставилась толщина 0, что любой рендерер должен
				//       воспринимать как толщину в 1px. Но сейчас это не работает в графическом ренедерере,
				//       поэтому временно это убрано.
				//       Толщиной в 1px - именно так рисуется в винде любая пунктирная линия в метафайле.

				//dWidth = 0; // Специальное значение для 1pх карандаша

				double dDpiX;
				m_pRenderer->get_DpiX(&dDpiX);
				double dPixelW = dDpiX > 1 ? 25.4 / dDpiX : 25.4 / 72;

				double dDashOff = 0;
				double* pDashPattern = NULL;
				int nDashLen = 0;

				switch (ulPenStyle)
				{
				case PS_DASH:
				{
					dDashOff = 0 * dPixelW;
					nDashLen = 2;
					pDashPattern = new double[2];
					if (pDashPattern)
					{
						pDashPattern[0] = 18 * dPixelW;
						pDashPattern[1] = 3 * dPixelW;
					}
					break;
				}
				case PS_DOT:
				{
					dDashOff = 4 * dPixelW;
					nDashLen = 2;
					pDashPattern = new double[2];
					if (pDashPattern)
					{
						pDashPattern[0] = 3 * dPixelW;
						pDashPattern[1] = 3 * dPixelW;
					}
					break;
				}
				case PS_DASHDOT:
				{
					dDashOff = 22 * dPixelW;
					nDashLen = 4;
					pDashPattern = new double[4];
					if (pDashPattern)
					{
						pDashPattern[0] = 9 * dPixelW;
						pDashPattern[1] = 6 * dPixelW;
						pDashPattern[2] = 3 * dPixelW;
						pDashPattern[3] = 6 * dPixelW;
					}
					break;
				}
				case PS_DASHDOTDOT:
				{
					dDashOff = 22 * dPixelW;
					nDashLen = 6;
					pDashPattern = new double[6];
					if (pDashPattern)
					{
						pDashPattern[0] = 9 * dPixelW;
						pDashPattern[1] = 3 * dPixelW;
						pDashPattern[2] = 3 * dPixelW;
						pDashPattern[3] = 3 * dPixelW;
						pDashPattern[4] = 3 * dPixelW;
						pDashPattern[5] = 3 * dPixelW;
					}
					break;
				}
				}

				if (NULL != pDashPattern)
				{
					m_pRenderer->put_PenDashOffset(dDashOff);
					m_pRenderer->PenDashPattern(pDashPattern, nDashLen);
					nDashStyle = Aggplus::DashStyleCustom;
					delete[] pDashPattern;
				}
			}

			m_pRenderer->put_PenDashStyle(nDashStyle);
			m_pRenderer->put_PenLineJoin(nJoinStyle);
			m_pRenderer->put_PenLineStartCap(nCapStyle);
			m_pRenderer->put_PenLineEndCap(nCapStyle);
			m_pRenderer->put_PenColor(nColor);
			m_pRenderer->put_PenSize(dWidth);
			m_pRenderer->put_PenAlpha(255);
			m_pRenderer->put_PenMiterLimit(dMiterLimit);

			// TO DO: С текущим интерфейсом AVSRenderer, остальные случаи ushROPMode
			//        реализовать невозможно. Потому что данный параметр нужно протаскивать
			//        как параметр Pen'a, и тот кто рисует сам должен разруливать все случаи.

			switch (m_pFile->GetRop2Mode())
			{
				case R2_BLACK:   m_pRenderer->put_PenColor(METAFILE_RGBA(0, 0, 0)); break;
				case R2_NOP:     m_pRenderer->put_PenAlpha(0); break;
				case R2_COPYPEN: break;
				case R2_WHITE:   m_pRenderer->put_PenColor(METAFILE_RGBA(255, 255, 255)); break;
			}

			if (PS_NULL == ulPenStyle)
				return false;

			return true;
		}
		bool UpdateClip()
		{			
			IClip* pClip = m_pFile->GetClip();
			if (!pClip)
				return false;

			pClip->ClipOnRenderer(this);

			return true;
		}

	private:

		IRenderer*     m_pRenderer;
		IMetaFileBase* m_pFile;
		int            m_lDrawPathType;
		double         m_dX;      // Координаты левого верхнего угла
		double         m_dY;      //
		double         m_dW;      // 
		double         m_dH;      // 
		double         m_dScaleX; // Коэффициенты сжатия/растяжения, чтобы 
		double         m_dScaleY; // результирующая картинка была нужных размеров.
		bool           m_bStartedPath;
	};
}
#endif // _METAFILE_COMMON_METAFILERENDERER_H
