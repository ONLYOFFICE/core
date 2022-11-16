#include "CEmfInterpretatorArray.h"

namespace MetaFile
{
	CEmfInterpretatorArray::CEmfInterpretatorArray(){}

	CEmfInterpretatorArray::CEmfInterpretatorArray(const CEmfInterpretatorArray &oInterpretator)
		: m_arInterpretators(oInterpretator.m_arInterpretators){}

	CEmfInterpretatorArray::~CEmfInterpretatorArray()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			delete pInterpretator;

		m_arInterpretators.clear();
	}

	void CEmfInterpretatorArray::AddEmfInterpretator(const wchar_t *wsFilepath)
	{
		if (wsFilepath == NULL)
			return;

		CEmfInterpretator *pEmfInterpretator = new CEmfInterpretator(wsFilepath);
		m_arInterpretators.push_back(pEmfInterpretator);
	}

#ifdef METAFILE_SUPPORT_WMF_EMF_XML
	void CEmfInterpretatorArray::AddXmlInterpretator(const wchar_t *wsFilepath)
	{
		if (wsFilepath == NULL)
			return;

		CEmfInterpretatorXml *pXmlInterpretator = new CEmfInterpretatorXml(wsFilepath);
		m_arInterpretators.push_back(pXmlInterpretator);
	}
#endif

	void CEmfInterpretatorArray::AddRenderInterpretator(IOutputDevice *pIOutputDevice)
	{
		if (pIOutputDevice == NULL)
			return;

		CEmfInterpretatorRender *pRenderInterpretator = new CEmfInterpretatorRender(pIOutputDevice);
		m_arInterpretators.push_back(pRenderInterpretator);
	}

	void CEmfInterpretatorArray::CreateConditional(IMetaFileBase *pMetafileBase)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->CreateConditional(pMetafileBase);
	}

	void CEmfInterpretatorArray::ChangeConditional()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->ChangeConditional();
	}

	InterpretatorType CEmfInterpretatorArray::GetType() const
	{
		return InterpretatorType::Array;
	}

	void CEmfInterpretatorArray::Begin()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->Begin();
	}

	void CEmfInterpretatorArray::End()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->End();
	}

	void CEmfInterpretatorArray::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->DrawBitmap(dX, dY, dW, dH, pBuffer, unWidth, unHeight);
	}

	void CEmfInterpretatorArray::DrawString(std::wstring &wsText, unsigned int unCharsCount, double dX, double dY, double *pDx,
											int iGraphicsMode, double dXScale, double dYScale)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->DrawString(wsText, unCharsCount, dX, dY, pDx, iGraphicsMode, dXScale, dYScale);
	}

	void CEmfInterpretatorArray::DrawDriverString(const std::wstring& wsString, const std::vector<TPointD>& arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->DrawDriverString(wsString, arPoints);
	}

	void CEmfInterpretatorArray::StartPath()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->StartPath();
	}

	void CEmfInterpretatorArray::MoveTo(double dX, double dY)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->MoveTo(dX, dY);
	}

	void CEmfInterpretatorArray::LineTo(double dX, double dY)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->LineTo(dX, dY);
	}

	void CEmfInterpretatorArray::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
	}

	void CEmfInterpretatorArray::ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->ArcTo(dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
	}

	void CEmfInterpretatorArray::ClosePath()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->ClosePath();
	}

	void CEmfInterpretatorArray::DrawPath(int nType)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->DrawPath(nType);
	}

	void CEmfInterpretatorArray::EndPath()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->EndPath();
	}

	void CEmfInterpretatorArray::ResetClip()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->ResetClip();
	}

	void CEmfInterpretatorArray::IntersectClip(const TRectD &oClip)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->IntersectClip(oClip);
	}

	void CEmfInterpretatorArray::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->ExcludeClip(oClip, oBB);
	}

	void CEmfInterpretatorArray::PathClip(IPath* pPath, int nClipMode, TXForm* pTransform)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->PathClip(pPath, nClipMode, pTransform);
	}

	void CEmfInterpretatorArray::StartClipPath(unsigned int unMode, int nFillMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->StartClipPath(unMode, nFillMode);
	}

	void CEmfInterpretatorArray::EndClipPath(unsigned int unMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->EndClipPath(unMode);
	}

	void CEmfInterpretatorArray::UpdateDC()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->UpdateDC();
	}

	void CEmfInterpretatorArray::SetTransform(double &dM11, double &dM12, double &dM21, double &dM22, double &dX, double &dY)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->SetTransform(dM11, dM12, dM21, dM22, dX, dY);
	}

	void CEmfInterpretatorArray::GetTransform(double *pdM11, double *pdM12, double *pdM21, double *pdM22, double *pdX, double *pdY)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->GetTransform(pdM11, pdM12, pdM21, pdM22, pdX, pdY);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_HEADER(oTEmfHeader);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ALPHABLEND(oTEmfAlphaBlend, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_STRETCHDIBITS(oTEmfStretchDIBITS, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_BITBLT(oTEmfBitBlt, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETDIBITSTODEVICE(oTEmfSetDiBitsToDevice, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_STRETCHBLT(oTEmfStretchBLT, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_EOF()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_EOF();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SAVEDC()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SAVEDC();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_RESTOREDC(nIndexDC);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_MODIFYWORLDTRANSFORM(oXForm, unMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETWORLDTRANSFORM(oXForm);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_CREATEBRUSHINDIRECT(unBrushIndex, pBrush);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETTEXTCOLOR(oColor);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SELECTOBJECT(unObjectIndex);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_EXTCREATEFONTINDIRECTW(unIndex, oLogFont);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETTEXTALIGN(unAlign);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETBKMODE(unBgMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_DELETEOBJECT(unObjectIndex);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETMITERLIMIT(unMeterLimit);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen, const std::vector<unsigned int>& arUnused)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_EXTCREATEPEN(unPenIndex, pPen, arUnused);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_CREATEPEN(unPenIndex, unWidthX, pPen);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETPOLYFILLMODE(unFillMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_BEGINPATH()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_BEGINPATH();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ENDPATH()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ENDPATH();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_CLOSEFIGURE()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_CLOSEFIGURE();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_FLATTENPATH()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_FLATTENPATH();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_WIDENPATH()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_WIDENPATH();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ABORTPATH()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ABORTPATH();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_MOVETOEX(const TEmfPointL &oPoint)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_MOVETOEX(oPoint);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETARCDIRECTION(unDirection);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_FILLPATH(oBounds);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETMAPMODE(unMapMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETWINDOWORGEX(oOrigin);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETWINDOWEXTEX(oExtent);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SCALEWINDOWEXTEX(nXNum, nXDenom, nYNum, nYDenom);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETVIEWPORTORGEX(oOrigin);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETVIEWPORTEXTEX(oExtent);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SCALEVIEWPORTEXTEX(nXNum, nXDenom, nYNum, nYDenom);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETSTRETCHBLTMODE(unStretchMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETICMMODE(unICMMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_CREATEMONOBRUSH(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_CREATEMONOBRUSH(unBrushIndex, oDibBrush, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(unBrushIndex, oDibBrush, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SELECTCLIPPATH(unRegionMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETBKCOLOR(oColor);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_EXCLUDECLIPRECT(oClip);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_EXTSELECTCLIPRGN(unRgnDataSize, unRegionMode, oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETMETARGN()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETMETARGN();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETROP2(unRop2Mode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_CREATEPALETTE(unPaletteIndex, oEmfLogPalette);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SELECTPALETTE(unPaletteIndex);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_REALIZEPALETTE()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_REALIZEPALETTE();
	}

	void CEmfInterpretatorArray::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_INTERSECTCLIPRECT(oClip);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETLAYOUT(unLayoutMode);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETBRUSHORGEX(oOrigin);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ARC(oBox, oStart, oEnd);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ARCTO(oBox, oStart, oEnd);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_CHORD(oBox, oStart, oEnd);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ELLIPSE(oBox);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_EXTTEXTOUTA(oTEmfExtTextoutA);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_EXTTEXTOUTW(oTEmfExtTextoutW);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_LINETO(oPoint);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_PIE(oBox, oStart ,oEnd);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYBEZIER(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYBEZIER(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYBEZIERTO(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYBEZIERTO(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYDRAW(oBounds, arPoints, unCount, pAbTypes);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYDRAW(oBounds, arPoints, unCount, pAbTypes);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYGON(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYGON(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYLINE(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYLINE(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYLINETO(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYLINETO(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYPOLYGON(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYPOLYGON(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYPOLYLINE(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_POLYPOLYLINE(oBounds, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_RECTANGLE(oBox);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_ROUNDRECT(oBox, oCorner);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SETPIXELV(oPoint, oColor);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_SMALLTEXTOUT(oText);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_STROKEANDFILLPATH(oBounds);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_STROKEPATH(oBounds);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_UNKNOWN(CDataStream &oDataStream)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_UNKNOWN(oDataStream);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_FILLRGN(oBounds, unIhBrush, oRegionDataHeader, arRects);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_PAINTRGN(const TEmfRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_PAINTRGN(oBounds, oRegionDataHeader, arRects);
	}

	void CEmfInterpretatorArray::HANDLE_EMR_FRAMERGN(const TEmfRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMR_FRAMERGN(oBounds, unIhBrush, nWidth, nHeight, oRegionDataHeader, arRects);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_OFFSETCLIP(double dX, double dY)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_OFFSETCLIP(dX, dY);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_RESETCLIP()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_RESETCLIP();
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_SETCLIPPATH(short unShFlags, const CEmfPlusPath *pPath)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_SETCLIPPATH(unShFlags, pPath);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_SETCLIPRECT(short shCM, const TEmfPlusRectF &oRect)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_SETCLIPRECT(shCM, oRect);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_SETCLIPREGION(short shObjectIndex, short shCM, const CEmfPlusRegion *pRegion)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_SETCLIPREGION(shObjectIndex, shCM, pRegion);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_COMMENT(CDataStream& oStream, unsigned int unSize)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_COMMENT(oStream, unSize);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_ENDOFFILE()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_ENDOFFILE();
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_GETDC()
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_GETDC();
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_HEADER(unsigned int unEmfPlusFlags, unsigned int m_unLogicalDpiX, unsigned int m_unLogicalDpiY)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_HEADER(unEmfPlusFlags, m_unLogicalDpiX, m_unLogicalDpiY);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_CLEAR(const TEmfPlusARGB &oARGB)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_CLEAR(oARGB);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWARC(BYTE chOgjectIndex, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWARC(chOgjectIndex, dStartAngle, dSweepAngle, oRect);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWBEZIERS(short shOgjectIndex, const std::vector<TEmfPlusPointF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWBEZIERS(shOgjectIndex, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWCLOSEDCURVE(short shOgjectIndex, double dTension, const std::vector<TEmfPlusPointF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWCLOSEDCURVE(shOgjectIndex, dTension, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWCURVE(short shOgjectIndex, double dTension, unsigned int unOffset, unsigned int unNumSegments, const std::vector<TEmfPlusPointF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWCURVE(shOgjectIndex, dTension, unOffset, unNumSegments, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWDRIVERSTRING(short shOgjectIndex, unsigned int unBrushId, unsigned int unDriverStringOptionsFlags, unsigned int unMatrixPresent, TXForm *pMatrix, const std::wstring &wsString, const std::vector<TPointD> &arGlyphPos)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWDRIVERSTRING(shOgjectIndex, unBrushId, unDriverStringOptionsFlags, unMatrixPresent, pMatrix, wsString, arGlyphPos);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWELLIPSE(short shOgjectIndex, const TEmfPlusRectF &oRect)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWELLIPSE(shOgjectIndex, oRect);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWIMAGE(short shOgjectIndex, unsigned int, const TEmfPlusRectF &, const TEmfPlusRectF &)
	{

	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWIMAGEPOINTS(short shOgjectIndex, unsigned int, const TEmfPlusRectF &, const TEmfPlusRectF &)
	{

	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWLINES(short shOgjectIndex, const std::vector<TEmfPlusPointF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWLINES(shOgjectIndex, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWPATH(short shOgjectIndex, unsigned int unPenId, const CEmfPath* pPath)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWPATH(shOgjectIndex, unPenId, pPath);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWPIE(short shOgjectIndex, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWPIE(shOgjectIndex, dStartAngle, dSweepAngle, oRect);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWRECTS(short shOgjectIndex, const std::vector<TEmfPlusRectF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWRECTS(shOgjectIndex, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_DRAWSTRING(short shOgjectIndex, unsigned int unBrushId, unsigned int unFormatID, const std::wstring &wsString, const TEmfPlusRectF &oRect)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_DRAWSTRING(shOgjectIndex, unBrushId, unFormatID, wsString, oRect);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_FILLCLOSEDCURVE(unsigned int unBrushId, double dTension, const std::vector<TEmfPlusRectF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_FILLCLOSEDCURVE(unBrushId, dTension, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_FILLELLIPSE(unsigned int unBrushId, const TEmfPlusRectF &oRect)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_FILLELLIPSE(unBrushId, oRect);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_FILLPATH(short shOgjectIndex, unsigned int unBrushId, const CEmfPlusPath *pPath)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_FILLPATH(shOgjectIndex, unBrushId, pPath);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_FILLPIE(unsigned int unBrushId, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_FILLPIE(unBrushId, dStartAngle, dSweepAngle, oRect);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_FILLPOLYGON(unsigned int unBrushId, const std::vector<TEmfPlusPointF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_FILLPOLYGON(unBrushId, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_FILLRECTS(unsigned int unBrushId, const std::vector<TEmfPlusRectF> &arPoints)
	{
		for (CEmfInterpretatorBase* pInterpretator : m_arInterpretators)
			pInterpretator->HANDLE_EMFPLUS_FILLRECTS(unBrushId, arPoints);
	}

	void CEmfInterpretatorArray::HANDLE_EMFPLUS_FILLREGION(short shOgjectIndex, unsigned int unBrushId)
	{

	}
}
