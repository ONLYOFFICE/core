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

		void Draw_Bitmap(long lX, long lY, long lW, long lH, BYTE* pBuffer, unsigned long ulWidth, unsigned long ulHeight)
		{
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

	private:

		IRenderer*              m_pRenderer;
		NSStructures::CPen      m_oPen;
		NSStructures::CBrush    m_oBrush;
		NSStructures::CFont     m_oFont;

		double                  m_dDpiX;
		double                  m_dDpiY;

		double                  m_dX;        // Координаты левого верхнего угла
		double                  m_dY;        //
		double                  m_dW;   // Коэффициенты сжатия/растяжения, чтобы 
		double                  m_dH;   // результирующая картинка была нужных размеров.
		CEmfFile*               m_pEmfFile;
	};
}
#endif // _RENDERER_OUPUT_EMF_H