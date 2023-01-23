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

#ifdef _DEBUG
#include <iostream>
#define LOGGING(_value) std::wcout << _value << std::endl;
#else
#define LOGGING(_value)
#endif

#define M_MINFONTSIZE 5

namespace MetaFile
{
	struct TRenderConditional
	{
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
			m_pSecondConditional = NULL;

			if (!pRenderer)
				return;

			m_pRenderer = pRenderer;

			UpdateScale();

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

		CMetaFileRenderer(IMetaFileBase *pFile, CMetaFileRenderer* pMetaFileRenderer)
		{
			m_pFile = pFile;

			if (!pMetaFileRenderer)
				return;

			m_dX = pMetaFileRenderer->m_dX;
			m_dY = pMetaFileRenderer->m_dY;
			m_dW = pMetaFileRenderer->m_dW;
			m_dH = pMetaFileRenderer->m_dH;

			m_pRenderer = NULL;
			m_pSecondConditional = NULL;

			m_pRenderer = pMetaFileRenderer->m_pRenderer;

			UpdateScale();

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

		virtual ~CMetaFileRenderer()
		{
			RELEASEOBJECT(m_pSecondConditional)
		}

		void CreateConditional(IMetaFileBase* pFile)
		{
			RELEASEOBJECT(m_pSecondConditional);

			m_pSecondConditional = new TRenderConditional();

			SaveConditional(*m_pSecondConditional);

			m_pSecondConditional->m_pFile = pFile;
		}

		void ChangeConditional()
		{
			if (NULL != m_pSecondConditional)
			{
				TRenderConditional oRenderConditional;
				SaveConditional(oRenderConditional);

				SetConditional(*m_pSecondConditional);

				*m_pSecondConditional = oRenderConditional;
			}
		}

		IMetaFileBase* GetFile() const
		{
			return m_pFile;
		}

		void UpdateScale()
		{
			if (NULL == m_pFile)
				return;

			TRect* pBounds = m_pFile->GetDCBounds();
			int nL = pBounds->nLeft;
			int nR = pBounds->nRight;
			int nT = pBounds->nTop;
			int nB = pBounds->nBottom;

			m_dScaleX = m_dW / std::fabs((double)(nR - nL));
			m_dScaleY = m_dH / std::fabs((double)(nB - nT));
		}

		void Begin()
		{
			UpdateScale();
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
			oImage.Create(pBuffer, unWidth, unHeight, 4 * unWidth, true);

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
		void DrawDriverString(const std::wstring& wsString, const std::vector<TPointD>& arPoints)
		{
			IFont *pFont = m_pFile->GetFont();

			if (NULL == pFont)
				return;

			UpdateTransform();
			UpdateClip();

			double dFontScale = 1.;
			double dLogicalFontHeight = std::fabs(pFont->GetHeight());

			double dM11, dM12, dM21, dM22, dRx, dRy;

			if (dLogicalFontHeight < M_MINFONTSIZE)
			{
				m_pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);
				dFontScale = dM22;
				dLogicalFontHeight *= dFontScale;
				m_pRenderer->SetTransform(dM11 / std::fabs(dM11), dM12, dM21, dM22 / std::fabs(dM22), dRx, dRy);
			}

			m_pRenderer->put_FontName(pFont->GetFaceName());
			m_pRenderer->put_FontSize(fabs(dLogicalFontHeight * m_dScaleX / 25.4 * 72));

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

			m_pRenderer->put_BrushType(c_BrushTypeSolid);
			m_pRenderer->put_BrushColor1(m_pFile->GetTextColor());
			m_pRenderer->put_BrushAlpha1(255);

			m_pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);
			m_pRenderer->ResetTransform();

			std::vector<TPointD> arGlyphPoint(arPoints.size());

			for (unsigned int unIndex = 0; unIndex < arPoints.size(); ++unIndex)
			{
				arGlyphPoint[unIndex].x = (arPoints[unIndex].x * dM11) * m_dScaleX * dFontScale + dRx;
				arGlyphPoint[unIndex].y = (arPoints[unIndex].y * dM22) * m_dScaleY * dFontScale + dRy;
			}

			for (unsigned int unIndex = 0; unIndex < std::min(arPoints.size(), wsString.length()); ++unIndex)
				m_pRenderer->CommandDrawTextCHAR(wsString[unIndex], arGlyphPoint[unIndex].x, arGlyphPoint[unIndex].y, 0, 0);

		}

		void DrawString(std::wstring& wsText, unsigned int unCharsCount, double _dX, double _dY, double* pDx, int iGraphicsMode, double dXScale, double dYScale)
		{
			CheckEndPath();
			IFont* pFont = m_pFile->GetFont();
			if (!pFont)
				return;

			UpdateTransform(iGraphicsMode);
			UpdateClip();

			double dLogicalFontHeight = std::fabs(pFont->GetHeight());

			double dM11, dM12, dM21, dM22, dRx, dRy;
			double dFontScale = 1.;

			if (dLogicalFontHeight < M_MINFONTSIZE)
			{
				m_pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);
				dFontScale = dM22;
				dLogicalFontHeight *= dFontScale;
				m_pRenderer->SetTransform(dM11 / std::fabs(dM11), dM12, dM21, dM22 / std::fabs(dM22), dRx, dRy);
			}

			double dFontHeight = fabs(dLogicalFontHeight * m_dScaleY / 25.4 * 72);

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

			double dTheta = ((((double)pFont->GetEscapement()) / 10) * M_PI / 180);

			double dCosTheta = cosf(dTheta);
			double dSinTheta = sinf(dTheta);

			if (dYScale > 0)
				dSinTheta = -dSinTheta;

			double dFontCharSpace = m_pFile->GetCharSpace() * m_dScaleX * m_pFile->GetPixelWidth() * dFontScale;
			m_pRenderer->put_FontCharSpace(dFontCharSpace);

			float fL = 0, fT = 0, fW = 0, fH = 0;
			float fUndX1 = 0, fUndY1 = 0, fUndX2 = 0, fUndY2 = 0, fUndSize = 1;

			NSFonts::IFontManager* pFontManager = m_pFile->GetFontManager();

			std::vector<std::wstring> arSplitString;

			if (wsText.find(L"\n") != std::wstring::npos)
			{
				size_t unStart;
				size_t unEnd = 0;

				while ((unStart = wsText.find_first_not_of(L"\n", unEnd)) != std::wstring::npos)
				{
					unEnd = wsText.find(L"\n", unStart);
					arSplitString.push_back(wsText.substr(unStart, unEnd - unStart));
				}
			}
			else
				arSplitString = {wsText};

			double dSkipY = 0;

			for (const std::wstring& wsString : arSplitString)
			{
				if (NULL == pFontManager)
				{
					if (NULL != pDx && wsString.length())
					{
						// Тогда мы складываем все pDx кроме последнего символа, последний считаем отдельно
						double dTempTextW = 0;
						for (unsigned int unCharIndex = 0; unCharIndex < wsString.length() - 1; unCharIndex++)
						{
							dTempTextW += pDx[unCharIndex];
						}

						dTempTextW += dFontHeight * wsString.length();

						fW = (float)dTempTextW;
					}
					else
					{
						fW = (float)(dFontHeight * wsString.length());
					}

					fH = dFontHeight * 1.2;
				}
				else
				{
					pFontManager->LoadFontByName(wsFaceName, dFontHeight, lStyle, 72, 72);
					pFontManager->SetCharSpacing(dFontCharSpace * 72 / 25.4);

					double dMmToPt = 25.4 / 72;

					double dFHeight = dFontHeight;
					double dFDescent = dFontHeight;

					NSFonts::IFontFile* pFontFile = pFontManager->GetFile();

					if (pFontFile)
					{
						dFHeight  *= pFontFile->GetHeight() / pFontFile->Units_Per_Em() * dMmToPt;
						dFDescent *= pFontFile->GetDescender() / pFontFile->Units_Per_Em() * dMmToPt;
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
						wsTempText += wsString.at(wsString.length() - 1);
						//wsTempText += wsText.at(unCharsCount - 1);

						pFontManager->LoadString1(wsTempText, 0, 0);
						TBBox oBox = pFontManager->MeasureString2();
						dTempTextW += dMmToPt * (oBox.fMaxX - oBox.fMinX);

						fL = 0;
						fW = (float)dTempTextW;
					}
					else
					{
						pFontManager->LoadString1(wsString, 0, 0);
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

				TPointD oTextPoint = TranslatePoint(_dX * dFontScale, _dY * dFontScale);
				double dX = oTextPoint.x;
				double dY = oTextPoint.y + dSkipY;

				dSkipY += dLogicalFontHeight * m_dScaleY * 1.5;

				// Найдем начальную точку текста
				unsigned int ulTextAlign = m_pFile->GetTextAlign() & TA_MASK;

				unsigned int ulVTextAlign = m_pFile->GetTextAlign() >> 8;

				if (ulTextAlign & TA_UPDATECP)
				{
					ulTextAlign -= TA_UPDATECP;
				}
				if (ulTextAlign & TA_BASELINE)
				{
					ulTextAlign -= TA_BASELINE;
					// Ничего не делаем
				}
				else if (ulTextAlign & TA_BOTTOM || ulVTextAlign == VTA_BOTTOM)
				{
					float fTemp = -(fH + fT / 2);

					dX += -fTemp * dSinTheta;
					dY +=  fTemp * dCosTheta;

					if (ulVTextAlign != VTA_BOTTOM)
						ulTextAlign -= TA_BOTTOM;
				}
				else if (ulVTextAlign == VTA_CENTER)
				{
					float fTemp = (fH + fT / 2) / 2;

					dX += -fTemp * dSinTheta;
					dY +=  fTemp * dCosTheta;
				}
				else // if (ulTextAlign & TA_TOP)
				{
					float fTemp = -fT;

					dX += -fTemp * dSinTheta;
					dY +=  fTemp * dCosTheta;
				}

				if (ulTextAlign == TA_RIGHT)
				{
					dX += -fW * dCosTheta;
					dY += -fW * dSinTheta;
				}
				else if (ulTextAlign == TA_CENTER)
				{
					dX += -fW / 2 * dCosTheta;
					dY += -fW / 2 * dSinTheta;
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
					double dShiftX = 0;
					double dShiftY = 0;

					m_pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);
					if (dXScale < -0.00001)
					{
						dX += fabs(fW);

						if (m_pFile->IsWindowFlippedX())
						{
							dShiftX = (2 * dX - fabs(fW)) * dM11;
						}
						else
						{
							dShiftX = (2 * dX + fabs(fW)) * dM11;
						}

						dM11 = fabs(dM11);
					}

					if (dYScale < -0.00001)
					{
						dY += fabs(fH);

						dShiftY = (2 * dY - fabs(fH)) * dM22;

						dM22 = fabs(dM22);
					}

					m_pRenderer->ResetTransform();
					m_pRenderer->SetTransform(dM11, dM12, dM21, dM22, dShiftX + dRx, dShiftY + dRy);

					bChangeCTM = true;
				}

				if (0 != pFont->GetEscapement())
				{
					// TODO: тут реализован только параметр shEscapement, еще нужно реализовать параметр Orientation
					m_pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

					double dOldX = dX;

					dX = dX * dCosTheta + dY * dSinTheta;
					dY = dY * dCosTheta - dOldX * dSinTheta;

					m_pRenderer->ResetTransform();
					m_pRenderer->SetTransform(dCosTheta * dM11, dSinTheta * dM22,
											  -dSinTheta * dM11, dCosTheta * dM22,
											  dRx, dRy);

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
					m_pRenderer->CommandDrawText(wsString, dX, dY, 0, 0);
				}
				else
				{
					unsigned int unUnicodeLen = 0;
					unsigned int* pUnicode = NSStringExt::CConverter::GetUtf32FromUnicode(wsString, unUnicodeLen);
					if (pUnicode && unUnicodeLen)
					{
						double dOffset = 0;
						double dKoefX = m_dScaleX * dFontScale;
						for (unsigned int unCharIndex = 0; unCharIndex < unUnicodeLen; unCharIndex++)
						{
							m_pRenderer->CommandDrawTextCHAR(pUnicode[unCharIndex], dX + dOffset, dY, 0, 0);
							dOffset += (pDx[unCharIndex] * dKoefX);
						}

						delete[] pUnicode;
					}
				}

				if (bChangeCTM)
					m_pRenderer->ResetTransform();
			}
		}
		void StartPath()
		{
			CheckEndPath();

			UpdateTransform();
			UpdateClip();

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
				m_pRenderer->DrawPath(1);
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
		void IntersectClip(const TRectD& oClip)
		{
			m_pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);

			m_pRenderer->BeginCommand(c_nClipType);
			m_pRenderer->BeginCommand(c_nPathType);
			m_pRenderer->PathCommandStart();

			TPointD oTL = TranslatePoint(oClip.dLeft, oClip.dTop);
			TPointD oBR = TranslatePoint(oClip.dRight, oClip.dBottom);

			m_pRenderer->PathCommandMoveTo(oTL.x, oTL.y);
			m_pRenderer->PathCommandLineTo(oTL.x, oBR.y);
			m_pRenderer->PathCommandLineTo(oBR.x, oBR.y);
			m_pRenderer->PathCommandLineTo(oBR.x, oTL.y);
			m_pRenderer->PathCommandLineTo(oTL.x, oTL.y);

			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->EndCommand(c_nClipType);
			m_pRenderer->PathCommandEnd();
		}
		void ExcludeClip(const TRectD& oClip, const TRectD& oBB)
		{
			StartClipPath(RGN_AND, ALTERNATE);

			MoveTo(oClip.dLeft,  oClip.dTop);
			LineTo(oClip.dRight, oClip.dTop);
			LineTo(oClip.dRight, oClip.dBottom);
			LineTo(oClip.dLeft,  oClip.dBottom);
			ClosePath();

			MoveTo(oBB.dLeft,  oBB.dTop);
			LineTo(oBB.dRight, oBB.dTop);
			LineTo(oBB.dRight, oBB.dBottom);
			LineTo(oBB.dLeft,  oBB.dBottom);
			ClosePath();

			EndClipPath(RGN_AND);
		}
		void PathClip(IPath *pPath, int nClipMode, TXForm* pTransform = NULL)
		{
			double dM11, dM12, dM21, dM22, dX, dY;

			if (NULL != pTransform)
			{
				GetTransform(&dM11, &dM12, &dM21, &dM22, &dX, &dY);
				SetTransform(pTransform->M11, pTransform->M12, pTransform->M21, pTransform->M22, pTransform->Dx, pTransform->Dy);
			}

			pPath->Draw(this, false, false, nClipMode);

			if (NULL != pTransform)
				SetTransform(dM11, dM12, dM21, dM22, dX, dY);
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

		void SaveConditional(TRenderConditional& oRenderConditional)
		{
			oRenderConditional.m_pFile	   = m_pFile;
			oRenderConditional.m_lDrawPathType = m_lDrawPathType;
			oRenderConditional.m_dX		   = m_dX;
			oRenderConditional.m_dY		   = m_dY;
			oRenderConditional.m_dW		   = m_dW;
			oRenderConditional.m_dH		   = m_dH;
			oRenderConditional.m_dScaleX	   = m_dScaleX;
			oRenderConditional.m_dScaleY	   = m_dScaleY;
			oRenderConditional.m_bStartedPath  = m_bStartedPath;
		}

		void SetConditional(const TRenderConditional& oRenderConditional)
		{
			m_pFile		= oRenderConditional.m_pFile;
			m_lDrawPathType = oRenderConditional.m_lDrawPathType;
			m_dX		= oRenderConditional.m_dX;
			m_dY		= oRenderConditional.m_dY;
			m_dW		= oRenderConditional.m_dW;
			m_dH		= oRenderConditional.m_dH;
			m_dScaleX	= oRenderConditional.m_dScaleX;
			m_dScaleY	= oRenderConditional.m_dScaleY;
			m_bStartedPath  = oRenderConditional.m_bStartedPath;
		}

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
			else if (BS_PATTERN == unBrushStyle)
			{
				m_pRenderer->put_BrushType(c_BrushTypePattern);
				m_pRenderer->put_BrushTextureMode(c_BrushTextureModeTileCenter);
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

					case HS_05Percent:	wsBrushType = L"pct5"; break;
					case HS_10Percent:	wsBrushType = L"pct10"; break;
					case HS_20Percent:	wsBrushType = L"pct20"; break;
					case HS_25Percent:	wsBrushType = L"pct25"; break;
					case HS_30Percent:	wsBrushType = L"pct30"; break;
					case HS_40Percent:	wsBrushType = L"pct40"; break;
					case HS_50Percent:	wsBrushType = L"pct50"; break;
					case HS_60Percent:	wsBrushType = L"pct60"; break;
					case HS_70Percent:	wsBrushType = L"pct70"; break;
					case HS_75Percent:	wsBrushType = L"pct75"; break;
					case HS_80Percent:	wsBrushType = L"pct80"; break;
					case HS_90Percent:	wsBrushType = L"pct90"; break;

					case HS_LTDOWNWARDDIAG:	wsBrushType = L"ltDnDiag"; break;
					case HS_LTUPWARDDIAG:	wsBrushType = L"ltUpDiag"; break;
					case HS_DNDOWNWARDDIAG:	wsBrushType = L"dkDnDiag"; break;
					case HS_DNUPWARDDIAG:	wsBrushType = L"dkUpDiag"; break;
					case HS_WDOWNWARDDIAG:	wsBrushType = L"wdDnDiag"; break;
					case HS_WUPWARDDIAG:	wsBrushType = L"wdUpDiag"; break;

					case HS_LTVERTICAL:		wsBrushType = L"ltVert"; break;
					case HS_LTHORIZONTAL:	wsBrushType = L"ltHorz"; break;
					case HS_NVERTICAL:		wsBrushType = L"narVert"; break;
					case HS_NHORIZONTAL:	wsBrushType = L"narHorz"; break;
					case HS_DNVERTICAL:		wsBrushType = L"dkVert"; break;
					case HS_DNHORIZONTAL:	wsBrushType = L"dkHorz"; break;

					case HS_DASHDOWNWARDDIAG:	wsBrushType = L"dashDnDiag"; break;
					case HS_DASHUPWARDDIAG:		wsBrushType = L"dashUpDiag"; break;
					case HS_DASHHORIZONTAL:		wsBrushType = L"dashHorz"; break;
					case HS_DASHVERTICAL:		wsBrushType = L"dashVert"; break;

					case HS_SMALLCONFETTI:		wsBrushType = L"smConfetti"; break;
					case HS_LARGECONFETTI:		wsBrushType = L"lgConfetti"; break;
					case HS_ZIGZAG:				wsBrushType = L"zigZag"; break;
					case HS_WAVE:				wsBrushType = L"wave"; break;
					case HS_DIAGBRICK:			wsBrushType = L"diagBrick"; break;
					case HS_HORIZBRICK:			wsBrushType = L"horzBrick"; break;
					case HS_WEAVE:				wsBrushType = L"weave"; break;
					case HS_PLAID:				wsBrushType = L"plaid"; break;
					case HS_DIVOT:				wsBrushType = L"divot"; break;
					case HS_DOTGRID:			wsBrushType = L"dotGrid"; break;
					case HS_DOTDIAMOND:			wsBrushType = L"dotDmnd"; break;
					case HS_SHINGLE:			wsBrushType = L"shingle"; break;
					case HS_TRELLIS:			wsBrushType = L"trellis"; break;
					case HS_SPHERE:				wsBrushType = L"sphere"; break;
					case HS_SGRID:				wsBrushType = L"smGrid"; break;
					case HS_SCHECHERBOARD:		wsBrushType = L"smCheck"; break;
					case HS_LCHECHERBOARD:		wsBrushType = L"lgCheck"; break;
					case HS_OUTLINEDDIAMOND:	wsBrushType = L"openDmnd"; break;
					case HS_SOLIDDIAMOND:		wsBrushType = L"solidDmnd"; break;

					default: break;
				}

				// TODO: Непонятно почему, но в Hatch все цвета идут не как RGB, а как BGR
				if (TRANSPARENT == m_pFile->GetTextBgMode())
					m_pRenderer->put_BrushAlpha2(0);
				else
				{
					m_pRenderer->put_BrushAlpha2(255);
					m_pRenderer->put_BrushColor2(m_pFile->GetTextBgColor());
				}

				m_pRenderer->put_BrushTexturePath(wsBrushType);
				m_pRenderer->put_BrushAlpha1(255);
				m_pRenderer->put_BrushColor1(pBrush->GetColor());
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

				double dX, dY, dWidth, dHeight;

				pBrush->GetBounds(dX, dY, dWidth, dHeight);

				m_pRenderer->BrushBounds(dX, dY, dWidth, dHeight);

				m_pRenderer->put_BrushLinearAngle(pBrush->GetStyleEx());

				long Colors[2];
				Colors[0] = pBrush->GetColor()  + (pBrush->GetAlpha()  << 24);
				Colors[1] = pBrush->GetColor2() + (pBrush->GetAlpha2() << 24);
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
				Colors[0] = pBrush->GetColor()  + (pBrush->GetAlpha()  << 24);
				Colors[1] = pBrush->GetColor2() + (pBrush->GetAlpha2() << 24);
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

			TXForm oMatrix;

			oMatrix.Copy(m_pFile->GetTransform(iGraphicsMode));

			if (std::fabs(oMatrix.M11) > 100. || std::fabs(oMatrix.M22) > 100.)
			{
				oMatrix.M11 /= std::fabs(oMatrix.M11);
				oMatrix.M22 /= std::fabs(oMatrix.M22);
			}

			m_pRenderer->ResetTransform();
			m_pRenderer->SetTransform(oMatrix.M11, oMatrix.M12 * dKoefY / dKoefX, oMatrix.M21 * dKoefX / dKoefY, oMatrix.M22, oMatrix.Dx * dKoefX, oMatrix.Dy * dKoefY);
		}
		bool UpdatePen()
		{
			IPen* pPen = m_pFile->GetPen();
			if (!pPen)
				return false;

			unsigned int unMetaPenStyle = pPen->GetStyle();

			unsigned int ulPenStyle     = unMetaPenStyle & PS_STYLE_MASK;

			if (PS_NULL == ulPenStyle)
				return false;

			unsigned int ulPenType      = unMetaPenStyle & PS_TYPE_MASK;
			unsigned int ulPenStartCap  = unMetaPenStyle & PS_STARTCAP_MASK;
			unsigned int ulPenEndCap    = unMetaPenStyle & PS_ENDCAP_MASK;
			unsigned int ulPenJoin      = unMetaPenStyle & PS_JOIN_MASK;

			BYTE nStartCapStyle = 0;
			if (PS_STARTCAP_ROUND == ulPenStartCap)
				nStartCapStyle = Aggplus::LineCapRound;
			else if (PS_STARTCAP_SQUARE == ulPenStartCap)
				nStartCapStyle = Aggplus::LineCapSquare;
			else if (PS_STARTCAP_FLAT == ulPenStartCap)
				nStartCapStyle = Aggplus::LineCapFlat;

			BYTE nEndCapStyle = 0;
			if (PS_ENDCAP_ROUND == ulPenEndCap)
				nEndCapStyle = Aggplus::LineCapRound;
			else if (PS_ENDCAP_SQUARE == ulPenEndCap)
				nEndCapStyle = Aggplus::LineCapSquare;
			else if (PS_ENDCAP_FLAT == ulPenEndCap)
				nEndCapStyle = Aggplus::LineCapFlat;

			if (0 == nStartCapStyle)
				nStartCapStyle = nEndCapStyle;

			BYTE nJoinStyle = 0;
			if (PS_JOIN_ROUND == ulPenJoin)
				nJoinStyle = Aggplus::LineJoinRound;
			else if (PS_JOIN_BEVEL == ulPenJoin)
				nJoinStyle = Aggplus::LineJoinBevel;
			else if (PS_JOIN_MITER == ulPenJoin)
				nJoinStyle = Aggplus::LineJoinMiter;

			double dWidth = pPen->GetWidth();

			if (0 == dWidth || (1 == dWidth && PS_COSMETIC == ulPenType))
			{
				double dRendererDpiX;
				m_pRenderer->get_DpiX(&dRendererDpiX);

				dWidth = 25.4 / dRendererDpiX;

				if (PS_COSMETIC == ulPenType)
					dWidth /= m_pFile->GetTransform()->M11 / (m_pFile->GetDpi() / 96.);

				nStartCapStyle = nEndCapStyle = Aggplus::LineCapFlat;
				nJoinStyle = Aggplus::LineJoinMiter;
			}
			else
				dWidth *= m_dScaleX;

			double dMiterLimit = (0 != pPen->GetMiterLimit()) ? pPen->GetMiterLimit() : m_pFile->GetMiterLimit() * m_dScaleX;

			BYTE nDashStyle = Aggplus::DashStyleSolid;

			double *pDataDash;
			unsigned int unSizeDash;

			pPen->GetDashData(pDataDash, unSizeDash);

			if (NULL != pDataDash && 0 != unSizeDash)
			{
				//на данный момент производьный стиль не отрисовывается,
				//поэтому замещает по возможно его на стандартный
				m_pRenderer->put_PenDashOffset(pPen->GetDashOffset());

				std::vector<double> arDashData;

				for (unsigned int unIndex = 0; unIndex < unSizeDash; ++unIndex)
					arDashData.push_back(pDataDash[unIndex] * dWidth);

				m_pRenderer->PenDashPattern(arDashData.data(), unSizeDash);
				nDashStyle = Aggplus::DashStyleCustom;
			}
			else if (PS_SOLID != ulPenStyle)
			{
				std::vector<double> arDashPattern;

				double dPixWidth = 0;

				if (0 == pPen->GetWidth() || (1 == pPen->GetWidth() && PS_COSMETIC == ulPenType))
				{
					dPixWidth = dWidth;
				}
				else
				{
					dPixWidth = dWidth * 25.4 / 72.;

					if (1 == pPen->GetWidth() && PS_COSMETIC == ulPenType)
						dPixWidth /= m_pFile->GetTransform()->M11 / (m_pFile->GetDpi() / 96.);
				}

				switch (ulPenStyle)
				{
					case PS_DASH:
					{
						arDashPattern.push_back(9 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);

						break;
					}
					case PS_DOT:
					{
						arDashPattern.push_back(3 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);

						break;
					}
					case PS_DASHDOT:
					{
						arDashPattern.push_back(9 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);

						break;
					}
					case PS_DASHDOTDOT:
					{
						arDashPattern.push_back(9 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);
						arDashPattern.push_back(3 * dPixWidth);

						break;
					}
				}

				if (!arDashPattern.empty())
				{
					m_pRenderer->PenDashPattern(arDashPattern.data(), arDashPattern.size());
					nDashStyle = Aggplus::DashStyleCustom;
					nStartCapStyle = nEndCapStyle = Aggplus::LineCapFlat;
					nJoinStyle = Aggplus::LineJoinMiter;
				}
			}

			m_pRenderer->put_PenDashStyle(nDashStyle);
			m_pRenderer->put_PenLineJoin(nJoinStyle);
			m_pRenderer->put_PenLineStartCap(nStartCapStyle);
			m_pRenderer->put_PenLineEndCap(nEndCapStyle);
			m_pRenderer->put_PenColor(pPen->GetColor());
			m_pRenderer->put_PenSize(dWidth);
			m_pRenderer->put_PenAlpha(pPen->GetAlpha());
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

		TRenderConditional *m_pSecondConditional;
	};
}
#endif // _METAFILE_COMMON_METAFILERENDERER_H
