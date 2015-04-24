#ifndef _RENDERER_OUPUT_EMF_H
#define _RENDERER_OUPUT_EMF_H


#include "../../../graphics/IRenderer.h"
#include "../../../graphics/structures.h"
#include "../../../graphics/Image.h"
#include "../../../raster/ImageFileFormatChecker.h"
#include "../../../raster/BgraFrame.h"
#include "../../../graphics/AggPlusEnums.h"

#include "EmfOutputDevice.h"
#include "EmfFile.h"

namespace MetaFile
{
	class CEmfRendererOutput : public CEmfOutputDevice
	{

	public:
		CEmfRendererOutput(CEmfFile *pFile, IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
		{
			m_pEmfFile = pFile;

			m_dX = dX;
			m_dY = dY;
			m_dW = dWidth;
			m_dH = dHeight;

			m_pRenderer = NULL;

			if (!pRenderer)
				return;

			m_pRenderer = pRenderer;

			TEmfRectL* pBounds = m_pEmfFile->GetDCBounds();
			int lL = pBounds->lLeft;
			int lR = pBounds->lRight;
			int lT = pBounds->lTop;
			int lB = pBounds->lBottom;

			m_dScaleX = (lR - lL <= 0) ? 1 : m_dW / (double)(lR - lL);
			m_dScaleY = (lB - lT <= 0) ? 1 : m_dH / (double)(lB - lT);

			m_bStartedPath = false;
		}
		~CEmfRendererOutput()
		{
		}

		void Begin()
		{
		}
		void End()
		{
			CheckEndPath();
		}

		void DrawBitmap(int lX, int lY, int lW, int lH, BYTE* pBuffer, unsigned int ulWidth, unsigned int ulHeight)
		{
			CheckEndPath();

			UpdateTransform();
			UpdateClip();

			Aggplus::CImage oImage;
			BYTE* pBufferPtr = new BYTE[4 * ulWidth * ulHeight];
			oImage.Create(pBufferPtr, ulWidth, ulHeight, 4 * ulWidth);

			for (int nIndex = 0, nSize = 4 * ulWidth * ulHeight; nIndex < nSize; nIndex += 4)
			{
				pBufferPtr[0] = (unsigned char)pBuffer[nIndex + 0];
				pBufferPtr[1] = (unsigned char)pBuffer[nIndex + 1];
				pBufferPtr[2] = (unsigned char)pBuffer[nIndex + 2];
				pBufferPtr[3] = (unsigned char)pBuffer[nIndex + 3];
				pBufferPtr += 4;
			}

			TEmfPointD oTL = TranslatePoint(lX, lY);
			TEmfPointD oBR = TranslatePoint(lX + lW, lY + lH);
			m_pRenderer->DrawImage(&oImage, oTL.x, oTL.y, oBR.x - oTL.x, oBR.y - oTL.y);
		}
		void DrawText(const wchar_t* wsText, unsigned int ulCharsCount, int lX, int lY)
		{
			CheckEndPath();

			UpdateTransform();
			UpdateClip();

			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return;

			CEmfLogFont* pFont = pDC->GetFont();
			if (!pFont)
				return;

			TEmfLogFont* pLogFont = &pFont->LogFontEx.LogFont;

			int lLogicalFontHeight = pLogFont->Height;
			if (lLogicalFontHeight < 0)
				lLogicalFontHeight = -lLogicalFontHeight;
			if (lLogicalFontHeight < 0.01)
				lLogicalFontHeight = 18;

			double dFontHeight = lLogicalFontHeight * m_dScaleY * pDC->GetPixelHeight() / 25.4 * 72;

			std::wstring wsFaceName((const wchar_t*)pLogFont->FaceName);
			m_pRenderer->put_FontName(wsFaceName);
			m_pRenderer->put_FontSize(dFontHeight);

			int lStyle = 0;
			if (pLogFont->Weight > 550)
				lStyle |= 0x01;
			if (pLogFont->Italic)
				lStyle |= 0x02;
			if (pLogFont->Underline)
				lStyle |= (1 << 2);
			if (pLogFont->StrikOut)
				lStyle |= (1 << 7);

			m_pRenderer->put_FontStyle(lStyle);

			TEmfColor oTextColor = pDC->GetTextColor();

			// Установим цвет текста
			m_pRenderer->put_BrushType(c_BrushTypeSolid);
			m_pRenderer->put_BrushColor1(METAFILE_RGBA(oTextColor.r, oTextColor.g, oTextColor.b));
			m_pRenderer->put_BrushAlpha1(255);

			double dTheta = -((((double)pLogFont->Escapement) / 10) * M_PI / 180);

			double dCosTheta = (float)cos(dTheta);
			double dSinTheta = (float)sin(dTheta);

			float fL = 0, fT = 0, fW = 0, fH = 0;
			float fUndX1 = 0, fUndY1 = 0, fUndX2 = 0, fUndY2 = 0, fUndSize = 1;
			CFontManager* pFontManager = m_pEmfFile->m_pFontManager;
			if (pFontManager)
			{
				pFontManager->LoadFontByName(wsFaceName, dFontHeight, lStyle, 72, 72);
				pFontManager->LoadString1(wsText, 0, 0);
				TBBox oBox = pFontManager->MeasureString2();
				fL = oBox.fMinX;
				fT = oBox.fMinY;
				fW = oBox.fMaxX - oBox.fMinX;
				fH = oBox.fMaxY - oBox.fMinY;
				pFontManager->GetUnderline(&fUndX1, &fUndY1, &fUndX2, &fUndY2, &fUndSize);

				double fKoef = 25.4 / 72;

				fL *= (float)fKoef;
				fT *= (float)fKoef;
				fW *= (float)fKoef;
				fH *= (float)fKoef;

				fUndX1 *= (float)fKoef; fUndY1 *= (float)fKoef;
				fUndX2 *= (float)fKoef; fUndY2 *= (float)fKoef;
				fUndSize *= (float)fKoef / 2;
			}

			TEmfPointD oTextPoint = TranslatePoint(lX, lY);
			double dX = oTextPoint.x;
			double dY = oTextPoint.y;

			// Найдем начальную точку текста
			unsigned int ulTextAlign = pDC->GetTextAlign();
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

			if (pLogFont->Underline)
			{
				fUndX1 += (float)dX;
				fUndX2 += (float)dX;
				fUndY1 += (float)dY;
				fUndY2 += (float)dY;
			}

			bool bChangeCTM = false;
			if (0 != pLogFont->Escapement)
			{
				// TODO: тут реализован только параметр shEscapement, еще нужно реализовать параметр Orientation
				m_pRenderer->SetTransform(dCosTheta, dSinTheta, -dSinTheta, dCosTheta, dX - dX * dCosTheta + dY * dSinTheta, dY - dX * dSinTheta - dY * dCosTheta);
				bChangeCTM = true;
			}

			// Для начала нарисуем фон текста
			if (OPAQUE == pDC->GetBgMode())
			{
				TEmfColor oBgColor = pDC->GetBgColor();
				m_pRenderer->put_BrushType(c_BrushTypeSolid);
				m_pRenderer->put_BrushColor1(255);
				m_pRenderer->put_BrushAlpha1(METAFILE_RGBA(oBgColor.r, oBgColor.g, oBgColor.b));

				m_pRenderer->BeginCommand(c_nPathType);
				m_pRenderer->PathCommandStart();
				m_pRenderer->PathCommandMoveTo(dX + fL, dY + fT);
				m_pRenderer->PathCommandLineTo(dX + fL + fW, dY + fT);
				m_pRenderer->PathCommandLineTo(dX + fL + fW, dY + fT + fH);
				m_pRenderer->PathCommandLineTo(dX + fL, dY + fT + fH);
				m_pRenderer->PathCommandClose();
				m_pRenderer->DrawPath(c_nWindingFillMode);
				m_pRenderer->EndCommand(c_nPathType);
				m_pRenderer->PathCommandStart();
			}

			// Нарисуем подчеркивание 
			if (pLogFont->Underline)
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

			// Рисуем сам текст
			m_pRenderer->CommandDrawText(wsText, dX, dY, 0, 0, 0);

			if (bChangeCTM)
				m_pRenderer->ResetTransform();
		}
		void StartPath()
		{
			CheckEndPath();

			UpdateTransform();
			UpdateClip();

			m_lDrawPathType = -1;
			if (true == UpdateBrush())
			{
				CEmfDC* pDC = m_pEmfFile->GetDC();
				if (ALTERNATE == pDC->GetFillMode())
					m_lDrawPathType = c_nEvenOddFillMode;
				else// if (WINDING == pDC->GetFillMode())
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
		void MoveTo(int lX, int lY)
		{
			CheckStartPath(true);
			TEmfPointD oPoint = TranslatePoint(lX, lY);
			m_pRenderer->PathCommandMoveTo(oPoint.x, oPoint.y);
		}
		void LineTo(int lX, int lY)
		{
			CheckStartPath(false);
			TEmfPointD oPoint = TranslatePoint(lX, lY);
			m_pRenderer->PathCommandLineTo(oPoint.x, oPoint.y);
		}
		void CurveTo(int lX1, int lY1, int lX2, int lY2, int lXe, int lYe)
		{
			CheckStartPath(false);

			TEmfPointD oPoint1 = TranslatePoint(lX1, lY1);
			TEmfPointD oPoint2 = TranslatePoint(lX2, lY2);
			TEmfPointD oPointE = TranslatePoint(lXe, lYe);
			m_pRenderer->PathCommandCurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
		}
		void ArcTo(int lLeft, int lTop, int lRight, int lBottom, double dStart, double dSweep)
		{
			CheckStartPath(false);

			TEmfPointD oTL = TranslatePoint(lLeft, lTop);
			TEmfPointD oBR = TranslatePoint(lRight, lBottom);
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

				int m_lEndType = -1;

				if (bStroke && (m_lDrawPathType & c_nStroke))
					m_lEndType = c_nStroke;
				
				if (bFill)
				{
					if (m_lDrawPathType & c_nWindingFillMode)
						m_lEndType = (-1 == m_lDrawPathType ? c_nWindingFillMode : m_lDrawPathType | c_nWindingFillMode);
					else if (m_lDrawPathType & c_nEvenOddFillMode)
						m_lEndType = (-1 == m_lDrawPathType ? c_nEvenOddFillMode : m_lDrawPathType | c_nEvenOddFillMode);
				}

				if (-1 != m_lEndType)
					m_pRenderer->DrawPath(m_lEndType);
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
		void IntersectClip(int lLeft, int lTop, int lRight, int lBottom)
		{
			m_pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);
			
			m_pRenderer->BeginCommand(c_nClipType);
			m_pRenderer->BeginCommand(c_nPathType);
			m_pRenderer->PathCommandStart();

			TEmfPointD oTL = TranslatePoint(lLeft, lTop);
			TEmfPointD oBR = TranslatePoint(lRight, lBottom);

			m_pRenderer->PathCommandMoveTo(oTL.x, oTL.y);
			m_pRenderer->PathCommandLineTo(oTL.x, oBR.y);
			m_pRenderer->PathCommandLineTo(oBR.x, oBR.y);
			m_pRenderer->PathCommandLineTo(oBR.x, oTL.y);
			m_pRenderer->PathCommandLineTo(oTL.x, oTL.y);

			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->EndCommand(c_nClipType);
			m_pRenderer->PathCommandEnd();
		}
		void StartClipPath(unsigned int unMode)
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

			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return;

			unsigned int unFillMode = pDC->GetFillMode();
			if (ALTERNATE == unFillMode)
				unClipMode |= c_nClipRegionTypeEvenOdd;
			else //if (WINDING == unFillMode)
				unClipMode |= c_nClipRegionTypeWinding;

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

	private:

		void CheckStartPath(bool bMoveTo)
		{
			if (!m_bStartedPath)
			{
				StartPath();

				if (!bMoveTo)
				{
					CEmfDC* pDC = m_pEmfFile->GetDC();
					if (!pDC)
						return;

					MoveTo(pDC->GetCurPos().x, pDC->GetCurPos().y);
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

		TEmfPointD TranslatePoint(int lX, int lY)
		{
			double dX = m_pEmfFile->TranslateX(lX);
			double dY = m_pEmfFile->TranslateY(lY);

			// Координаты приходят уже с примененной матрицей. Поэтому сначала мы умножаем на матрицу преобразования, 
			// вычитаем начальные координаты и умножаем на обратную матрицу преобразования.
			TEmfRectL* pBounds = m_pEmfFile->GetDCBounds();
			double dT = pBounds->lTop;
			double dL = pBounds->lLeft;

			TEmfXForm* pInverse = m_pEmfFile->GetDC()->GetInverseTransform();
			TEmfXForm* pTransform = m_pEmfFile->GetDC()->GetTransform();
			pTransform->Apply(dX, dY);
			dX -= dL;
			dY -= dT;
			pInverse->Apply(dX, dY);

			TEmfPointD oPoint;
			oPoint.x = m_dScaleX * dX + m_dX;
			oPoint.y = m_dScaleY * dY + m_dY;
			return oPoint;
		}

		bool UpdateBrush()
		{
			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return false;

			CEmfLogBrushEx* pBrush = pDC->GetBrush();
			if (!pBrush)
				return false;

			int lColor = METAFILE_RGBA(pBrush->Color.r, pBrush->Color.g, pBrush->Color.b);

			if (BS_NULL == pBrush->BrushStyle)
				return false;			
			else if (BS_DIBPATTERN == pBrush->BrushStyle)
			{
				m_pRenderer->put_BrushType(c_BrushTypeTexture);
				m_pRenderer->put_BrushTextureMode(c_BrushTextureModeTile);
				m_pRenderer->put_BrushTexturePath(pBrush->DibPatternPath);
			}
			else //if (BS_SOLID == pBrush->BrushStyle)
			{
				m_pRenderer->put_BrushColor1(lColor);
				m_pRenderer->put_BrushAlpha1(pBrush->BrushAlpha);
				m_pRenderer->put_BrushType(c_BrushTypeSolid);
			}

			return true;
		}
		void UpdateTransform()
		{
			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return;

			double dKoefX = m_dScaleX;
			double dKoefY = m_dScaleY;

			TEmfXForm* pMatrix = pDC->GetTransform();
			m_pRenderer->ResetTransform();
			m_pRenderer->SetTransform(pMatrix->M11, pMatrix->M12 * dKoefY / dKoefX, pMatrix->M21 * dKoefX / dKoefY, pMatrix->M22, pMatrix->Dx * dKoefX, pMatrix->Dy * dKoefY);
		}
		bool UpdatePen()
		{
			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return false;

			CEmfLogPen* pPen = pDC->GetPen();
			if (!pPen)
				return false;

			int lColor = METAFILE_RGBA(pPen->Color.r, pPen->Color.g, pPen->Color.b);

			// TODO: dWidth зависит еще от флага PS_GEOMETRIC в стиле карандаша
			double dWidth = pPen->Width * m_dScaleX * pDC->GetPixelWidth();
			if (dWidth <= 0.01)
				dWidth = 0;

			unsigned int ulPenType   = pPen->PenStyle & PS_TYPE_MASK;
			unsigned int ulPenEndCap = pPen->PenStyle & PS_ENDCAP_MASK;
			unsigned int ulPenJoin   = pPen->PenStyle & PS_JOIN_MASK;
			unsigned int ulPenStyle  = pPen->PenStyle & PS_STYLE_MASK;

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

			double dMiterLimit = pDC->GetMiterLimit() * m_dScaleX * pDC->GetPixelWidth();

			// TODO: Некоторые типы пена невозможно реализовать с текущим интерфейсом рендерера, поэтому мы делаем его пока PS_SOLID.
			// TODO: Реализовать PS_USERSTYLE
			BYTE nDashStyle;
			if (PS_ALTERNATE == ulPenStyle || PS_USERSTYLE == ulPenStyle || PS_INSIDEFRAME == ulPenStyle)
				nDashStyle = (BYTE)PS_SOLID;
			else if (PS_NULL != ulPenStyle)
				nDashStyle = (BYTE)ulPenStyle;

			m_pRenderer->put_PenDashStyle(nDashStyle);
			m_pRenderer->put_PenLineJoin(nJoinStyle);
			m_pRenderer->put_PenLineStartCap(nCapStyle);
			m_pRenderer->put_PenLineEndCap(nCapStyle);
			m_pRenderer->put_PenColor(lColor);
			m_pRenderer->put_PenSize(dWidth);
			m_pRenderer->put_PenAlpha(255);
			m_pRenderer->put_PenMiterLimit(dMiterLimit);

			// TO DO: С текущим интерфейсом AVSRenderer, остальные случаи ushROPMode
			//        реализовать невозможно. Потому что данный параметр нужно протаскивать
			//        как параметр Pen'a, и тот кто рисует сам должен разруливать все случаи.

			switch (pDC->GetRop2Mode())
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
			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return false;

			CEmfClip* pClip = pDC->GetClip();
			if (!pClip)
				return false;

			pClip->ClipOnRenderer(this);

			return true;
		}

	private:

		IRenderer* m_pRenderer;
		int        m_lDrawPathType;
		double     m_dX;      // Координаты левого верхнего угла
		double     m_dY;      //
		double     m_dW;      // 
		double     m_dH;      // 
		double     m_dScaleX; // Коэффициенты сжатия/растяжения, чтобы 
		double     m_dScaleY; // результирующая картинка была нужных размеров.
		CEmfFile*  m_pEmfFile;
		bool       m_bStartedPath;
	};
}
#endif // _RENDERER_OUPUT_EMF_H