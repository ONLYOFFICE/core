#include <stddef.h>
#include "Object.h"
#include "Stream.h"
#include "GState.h"
#include "OutputDevice.h"

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// OutputDev
	//-------------------------------------------------------------------------------------------------------------------------------

	void OutputDev::SetDefaultCTM(double *pCTM)
	{
		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrDefaultCTM[nIndex] = pCTM[nIndex];
		}

		double dDet = 1 / (m_arrDefaultCTM[0] * m_arrDefaultCTM[3] - m_arrDefaultCTM[1] * m_arrDefaultCTM[2]);

		m_arrDefaultInvCTM[0] =  m_arrDefaultCTM[3] * dDet;
		m_arrDefaultInvCTM[1] = -m_arrDefaultCTM[1] * dDet;
		m_arrDefaultInvCTM[2] = -m_arrDefaultCTM[2] * dDet;
		m_arrDefaultInvCTM[3] =  m_arrDefaultCTM[0] * dDet;
		m_arrDefaultInvCTM[4] = (m_arrDefaultCTM[2] * m_arrDefaultCTM[5] - m_arrDefaultCTM[3] * m_arrDefaultCTM[4]) * dDet;
		m_arrDefaultInvCTM[5] = (m_arrDefaultCTM[1] * m_arrDefaultCTM[4] - m_arrDefaultCTM[0] * m_arrDefaultCTM[5]) * dDet;
	}

	void OutputDev::ConvertDeviceToUser(double dDevX, double dDevY, double *pdUserX, double *pdUserY)
	{
		*pdUserX = m_arrDefaultInvCTM[0] * dDevX + m_arrDefaultInvCTM[2] * dDevY + m_arrDefaultInvCTM[4];
		*pdUserY = m_arrDefaultInvCTM[1] * dDevX + m_arrDefaultInvCTM[3] * dDevY + m_arrDefaultInvCTM[5];
	}

	void OutputDev::ConvertUserToDevice(double dUserX, double dUserY, int *pnDevX, int *pnDevY)
	{
		*pnDevX = (int)(m_arrDefaultCTM[0] * dUserX + m_arrDefaultCTM[2] * dUserY + m_arrDefaultCTM[4] + 0.5);
		*pnDevY = (int)(m_arrDefaultCTM[1] * dUserX + m_arrDefaultCTM[3] * dUserY + m_arrDefaultCTM[5] + 0.5);
	}

	void OutputDev::UpdateAll(GrState *pGState)
	{
		UpdateLineDash(pGState);
		UpdateFlatness(pGState);
		UpdateLineJoin(pGState);
		UpdateLineCap(pGState);
		UpdateMiterLimit(pGState);
		UpdateLineWidth(pGState);
		UpdateStrokeAdjust(pGState);
		UpdateFillColorSpace(pGState);
		UpdateFillColor(pGState);
		UpdateStrokeColorSpace(pGState);
		UpdateStrokeColor(pGState);
		UpdateBlendMode(pGState);
		UpdateFillOpacity(pGState);
		UpdateStrokeOpacity(pGState);
		UpdateFillOverprint(pGState);
		UpdateStrokeOverprint(pGState);
		UpdateTransfer(pGState);
		UpdateFont(pGState);
	}

	bool OutputDev::BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pnUnicode, int nUnLen)
	{
		return false;
	}

	void OutputDev::DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, bool bInvert, bool bInlineImage)
	{
		if (bInlineImage)
		{
			pStream->Reset();
			int nSize = nHeight * ((nWidth + 7) / 8);
			for (int nIndex = 0; nIndex < nSize; ++nIndex)
				pStream->GetChar();
			pStream->Close();
		}
	}

	void OutputDev::DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pnMaskColors, bool bInlineImage)
	{
		if (bInlineImage)
		{
			pStream->Reset();
			int nSize = nHeight * ((nWidth * pColorMap->GetComponentsCount() * pColorMap->GetBitsPerComponent() + 7) / 8);
			for (int nIndex = 0; nIndex < nSize; ++nIndex)
				pStream->GetChar();
			pStream->Close();
		}
	}

	void OutputDev::DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, bool bMaskInvert)
	{
		DrawImage(pGState, pRef, pStream, nWidth, nHeight, pColorMap, NULL, false);
	}

	void OutputDev::DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap)
	{
		DrawImage(pGState, pRef, pStream, nWidth, nHeight, pColorMap, NULL, false);
	}

#if OPI_SUPPORT
	void OutputDev::OpiBegin(GrState *pGState, Dict *pOpiDict) 
	{
	}
	void OutputDev::OpiEnd  (GrState *pGState, Dict *pOpiDict) 
	{
	}
#endif
}