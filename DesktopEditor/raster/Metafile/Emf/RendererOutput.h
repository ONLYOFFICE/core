#ifndef _RENDERER_OUPUT_EMF_H
#define _RENDERER_OUPUT_EMF_H


#include "../../../graphics/IRenderer.h"
#include "../../../graphics/structures.h"
#include "../../../graphics/Image.h"
#include "../../../raster/ImageFileFormatChecker.h"
#include "../../../raster/BgraFrame.h"

#include "EmfOutputDevice.h"
#include "EmfFile.h"

namespace Metafile
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
		}
		~CEmfRendererOutput()
		{
		}

		void Begin()
		{
		}
		void End()
		{
		}

		void DrawBitmap(long lX, long lY, long lW, long lH, BYTE* pBuffer, unsigned long ulWidth, unsigned long ulHeight)
		{
			UpdateTransform();

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

			double dX = TransX(lX);
			double dY = TransX(lY);

			double dX1 = TransX(lX + lW);
			double dY1 = TransY(lY + lH);

			m_pRenderer->DrawImage(&oImage, dX, dY, dX1 - dX, dY1 - dY);
		}
		void DrawText(const wchar_t* wsText, unsigned long ulCharsCount, long lX, long lY)
		{
			UpdateTransform();

			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return;

			CEmfLogFont* pFont = pDC->GetFont();
			if (!pFont)
				return;

			TEmfLogFont* pLogFont = &pFont->LogFontEx.LogFont;

			TEmfRectL* pBounds = &m_pEmfFile->m_oHeader.oBounds;
			TEmfRectL* pFrame  = &m_pEmfFile->m_oHeader.oFrame;

			double dFontHeight = TransY(std::abs(pLogFont->Height)) / 25.4 * 72;
			std::wstring wsFaceName((const wchar_t*)pLogFont->FaceName);
			m_pRenderer->put_FontName(wsFaceName);
			m_pRenderer->put_FontSize(dFontHeight);

			long lStyle = 0;
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

			double dX = TransX(lX);
			double dY = TransX(lY);

			// Найдем начальную точку текста
			unsigned long ulTextAlign = pDC->GetTextAlign();
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
			UpdateTransform();
			UpdateBrush();

			m_pRenderer->BeginCommand(c_nPathType);
			m_pRenderer->PathCommandStart();
		}
		void MoveTo(long lX, long lY)
		{
			double dX = TransX(lX);
			double dY = TransY(lY);
			m_pRenderer->PathCommandMoveTo(dX, dY);
		}
		void LineTo(long lX, long lY)
		{
			double dX = TransX(lX);
			double dY = TransY(lY);
			m_pRenderer->PathCommandLineTo(dX, dY);
		}
		void ClosePath()
		{
			m_pRenderer->PathCommandClose();
		}
		void DrawPath()
		{
			m_pRenderer->DrawPath(c_nWindingFillMode);
		}
		void EndPath()
		{
			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->PathCommandEnd();
		}

	private:

		double TransX(long lX)
		{
			long lL = m_pEmfFile->m_oHeader.oBounds.lLeft;
			long lR = m_pEmfFile->m_oHeader.oBounds.lRight;

			if (lR - lL <= 0)
				return 0;

			return m_dW * (double)(lX - lL) / (double)(lR - lL);
		}
		double TransY(long lY)
		{
			long lT = m_pEmfFile->m_oHeader.oBounds.lTop;
			long lB = m_pEmfFile->m_oHeader.oBounds.lBottom;

			if (lB - lT <= 0)
				return 0;

			return m_dH * (double)(lY - lT) / (double)(lB - lT);
		}

		bool UpdateBrush()
		{
			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return false;

			CEmfLogBrushEx* pBrush = pDC->GetBrush();
			if (!pBrush)
				return false;

			long lColor = METAFILE_RGBA(pBrush->Color.r, pBrush->Color.g, pBrush->Color.b);

			if (BS_NULL == pBrush->BrushStyle)
				return false;
			else //if (BS_SOLID == pBrush->BrushStyle)
			{
				m_pRenderer->put_BrushColor1(lColor);
				m_pRenderer->put_BrushAlpha1(255);
				m_pRenderer->put_BrushType(c_BrushTypeSolid);
			}

			return true;
		}
		void UpdateTransform()
		{
			CEmfDC* pDC = m_pEmfFile->GetDC();
			if (!pDC)
				return;

			long lL = m_pEmfFile->m_oHeader.oBounds.lLeft;
			long lR = m_pEmfFile->m_oHeader.oBounds.lRight;
			long lT = m_pEmfFile->m_oHeader.oBounds.lTop;
			long lB = m_pEmfFile->m_oHeader.oBounds.lBottom;

			if (lB - lT <= 0 || lR - lL <= 0)
				return;

			double dKoefX = m_dW / (double)(lR - lL);
			double dKoefY = m_dW / (double)(lR - lL);

			TEmfXForm* pMatrix = pDC->GetTransform();

			m_pRenderer->ResetTransform();
			m_pRenderer->SetTransform(pMatrix->M11, pMatrix->M12 * dKoefY / dKoefX, pMatrix->M21 * dKoefX / dKoefY, pMatrix->M22, pMatrix->Dx * dKoefX, pMatrix->Dy * dKoefY);
		}

	private:

		IRenderer*              m_pRenderer;

		double                  m_dDpiX;
		double                  m_dDpiY;

		long                    m_lDrawPathType;

		double                  m_dX;        // Координаты левого верхнего угла
		double                  m_dY;        //
		double                  m_dW;   // Коэффициенты сжатия/растяжения, чтобы 
		double                  m_dH;   // результирующая картинка была нужных размеров.
		CEmfFile*               m_pEmfFile;
	};
}
#endif // _RENDERER_OUPUT_EMF_H