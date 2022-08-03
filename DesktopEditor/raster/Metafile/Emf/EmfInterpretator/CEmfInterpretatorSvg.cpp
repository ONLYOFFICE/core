#include "CEmfInterpretatorSvg.h"

#include "../../Common/MetaFileUtils.h"

#include "../Image.h"

#include "../../../BgraFrame.h"

namespace MetaFile
{               
	CEmfInterpretatorSvg::CEmfInterpretatorSvg(const wchar_t *wsFilePath, CEmfParserBase* pParser, unsigned int unWidth, unsigned int unHeight)
		: m_wsSvgFilePath(wsFilePath), m_pParser(pParser)
	{
		SetSize(unWidth, unHeight);
	}

	CEmfInterpretatorSvg::CEmfInterpretatorSvg(const CEmfInterpretatorSvg &oInterpretator)
		: m_wsSvgFilePath(oInterpretator.m_wsSvgFilePath), m_pParser(NULL)
	{
	}

	CEmfInterpretatorSvg::~CEmfInterpretatorSvg()
	{

	}

	InterpretatorType CEmfInterpretatorSvg::GetType() const
	{
		return InterpretatorType::Svg;
	}

	void CEmfInterpretatorSvg::SetOutputDevice(const wchar_t *wsFilePath)
	{
		m_wsSvgFilePath = wsFilePath;
	}

	void CEmfInterpretatorSvg::SetSize(unsigned int unWidth, unsigned int unHeight)
	{
		m_oSizeWindow.cx = unWidth;
		m_oSizeWindow.cy = unHeight;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
	{
		m_oViewport.dLeft   = 0;
		m_oViewport.dTop    = 0;
		m_oViewport.dRight  = oTEmfHeader.oDevice.cx;
		m_oViewport.dBottom =  oTEmfHeader.oDevice.cy;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EOF()
	{
		//TODO: конец EMF файла
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SAVEDC()
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen, const std::vector<unsigned int> &arUnused)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_BEGINPATH()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ENDPATH()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CLOSEFIGURE()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FLATTENPATH()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_WIDENPATH()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ABORTPATH()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_MOVETOEX(const TEmfPointL &oPoint)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CREATEMONOBRUSH(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETMETARGN()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_REALIZEPALETTE()
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
	{
		ArcTo(oCenter.x - unRadius, oCenter.y - unRadius, oCenter.x + unRadius, oCenter.y + unRadius, dStartAngle, dSweepAngle);

		double dFistAngle = dStartAngle;
		double dSecondAngle = dSweepAngle;

		if (NULL != m_pParser && m_pParser->GetDC()->GetFinalTransform(GM_ADVANCED)->M22 < 0)
		{
			dFistAngle *= -1;
			dSecondAngle *= -1;
		}

		double dXRadius = TranslateX(unRadius);
		double dYRadius = TranslateY(unRadius);

		double dXCenter = TranslateX(oCenter.x);
		double dYCenter = TranslateY(oCenter.y);

		double dStartX = dXCenter + dXRadius  * cos((dStartAngle) * M_PI / 180);
		double dStartY = dYCenter + dYRadius  * sin((dStartAngle) * M_PI / 180);

		double dEndX = dXCenter + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = dYCenter + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		std::wstring wsValue = L"M " + std::to_wstring(dStartX) + L' ' + std::to_wstring(dStartY);

		wsValue += L" A " + std::to_wstring(dXRadius) + L' ' +
				std::to_wstring(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				std::to_wstring(dEndX) + L' ' +
				std::to_wstring(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue},
									   {L"fill", L"none"}};

		AddStroke(arAttributes);

		UpdateTransform(dStartX, dStartY);
		UpdateTransform(dEndX, dEndY);

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		TRectD oNewRect = TranslateRect(oBox);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y);

		if (NULL != m_pParser && m_pParser->GetDC()->GetFinalTransform(GM_ADVANCED)->M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.dRight - oNewRect.dLeft)) / 2;
		double dYRadius = std::fabs((oNewRect.dBottom - oNewRect.dTop)) / 2;

		double dStartX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dStartAngle) * M_PI / 180);
		double dStartY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dStartAngle) * M_PI / 180);

		double dEndX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		std::wstring wsValue = L"M " + std::to_wstring(dStartX) + L' ' + std::to_wstring(dStartY);

		wsValue += L" A " + std::to_wstring(dXRadius) + L' ' +
				std::to_wstring(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				std::to_wstring(dEndX) + L' ' +
				std::to_wstring(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue},
									   {L"fill", L"none"}};

		AddStroke(arAttributes);

		UpdateTransform(oNewRect);

		WriteNode(L"path" , arAttributes);

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		TRectD oNewRect = TranslateRect(oBox);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y);

		if (NULL != m_pParser && m_pParser->GetDC()->GetFinalTransform(GM_ADVANCED)->M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.dRight - oNewRect.dLeft)) / 2;
		double dYRadius = std::fabs((oNewRect.dBottom - oNewRect.dTop)) / 2;

		double dStartX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dStartAngle) * M_PI / 180);
		double dStartY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dStartAngle) * M_PI / 180);

		double dEndX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		std::wstring wsValue = L"M " + std::to_wstring(dStartX) + L' ' + std::to_wstring(dStartY);

		wsValue += L" A " + std::to_wstring(dXRadius) + L' ' +
				std::to_wstring(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				std::to_wstring(dEndX) + L' ' +
				std::to_wstring(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue},
									   {L"fill", L"none"}};

		AddStroke(arAttributes);

		UpdateTransform(oNewRect);

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		//TODO:: реализовать
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"cx", std::to_wstring((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
									   {L"cy", std::to_wstring((oNewRect.dTop    + oNewRect.dBottom) / 2)},
									   {L"rx", std::to_wstring((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
									   {L"ry", std::to_wstring((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(oNewRect);

		WriteNode(L"ellipse", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutA.aEmrText.OutputString, oTEmfExtTextoutA.aEmrText.Chars);

		WriteText(wsText, oTEmfExtTextoutA.aEmrText.Reference.x, oTEmfExtTextoutA.aEmrText.Reference.y, oTEmfExtTextoutA.Bounds);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutW.wEmrText.OutputString, oTEmfExtTextoutW.wEmrText.Chars);

		WriteText(wsText, oTEmfExtTextoutW.wEmrText.Reference.x, oTEmfExtTextoutW.wEmrText.Reference.y, oTEmfExtTextoutW.Bounds);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
	{
		TPointD oCurPos = GetCutPos();

		NodeAttributes arAttributes = {{L"x1", std::to_wstring(oCurPos.x)},
									   {L"y1", std::to_wstring(oCurPos.y)},
									   {L"x2", std::to_wstring(TranslateX(oPoint.x))},
									   {L"y2", std::to_wstring(TranslateY(oPoint.y))}};

		AddStroke(arAttributes);

		UpdateTransform(oPoint.x, oPoint.y);

		WriteNode(L"line", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + std::to_wstring(arPoints[0].x) + L' ' + std::to_wstring(arPoints[0].y) + L' ';

		for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
			wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + std::to_wstring(TranslateX(arPoints[0].x)) + L' ' + std::to_wstring(TranslateY(arPoints[0].y)) + L' ';

		for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
			wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + std::to_wstring(TranslateX(arPoints[0].x)) + L' ' + std::to_wstring(TranslateY(arPoints[0].y)) + L' ';

		for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
			wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + std::to_wstring(TranslateX(arPoints[0].x)) + L' ' + std::to_wstring(TranslateY(arPoints[0].y)) + L' ';

		for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
			wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
					std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
			return;

		std::wstring wsValue;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			if (0x02 == pAbTypes[unIndex])
				wsValue += L" L " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
			else if (0x04 == pAbTypes[unIndex] && unIndex + 3 < unCount)
			{
				wsValue += L" C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
						std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
						std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
				wsValue += L" M " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints, unCount);

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
			return;

		std::wstring wsValue;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			if (0x02 == pAbTypes[unIndex])
				wsValue += L" L " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
			else if (0x04 == pAbTypes[unIndex] && unIndex + 3 < unCount)
			{
				wsValue += L" C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
						std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
						std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
				wsValue += L" M " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints, unCount);

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y)) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"polygon", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateX(oPoint.y)) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"polygon", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateX(oPoint.y)) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue},
									   {L"fill", L"none"}};

		AddStroke(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y)) + L' ';

		wsValue.pop_back();

		NodeAttributes arAttributes = {{L"points", wsValue},
									   {L"fill", L"none"}};

		AddStroke(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = std::to_wstring(oCurPos.x) + L',' + std::to_wstring(oCurPos.y);

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += L' ' + std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y));

		NodeAttributes arAttributes = {{L"points", wsValue},
									   {L"fill", L"none"}};

		AddStroke(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = std::to_wstring(oCurPos.x) + L',' + std::to_wstring(oCurPos.y);

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += L' ' + std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y));

		NodeAttributes arAttributes = {{L"points", wsValue},
									   {L"fill", L"none"}};

		AddStroke(arAttributes);

		UpdateTransform(arPoints);

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		for (const std::vector<TEmfPointL>& arPolygonPoints : arPoints)
			HANDLE_EMR_POLYGON(oBounds, arPolygonPoints);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		for (const std::vector<TEmfPointS>& arPolygonPoints : arPoints)
			HANDLE_EMR_POLYGON(oBounds, arPolygonPoints);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		for (const std::vector<TEmfPointL>& arPolygonPoints : arPoints)
			HANDLE_EMR_POLYLINE(oBounds, arPolygonPoints);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		for (const std::vector<TEmfPointS>& arPolygonPoints : arPoints)
			HANDLE_EMR_POLYLINE(oBounds, arPolygonPoints);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x", std::to_wstring(oNewRect.dLeft)},
									   {L"y", std::to_wstring(oNewRect.dTop)},
									   {L"width", std::to_wstring(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height", std::to_wstring(oNewRect.dBottom - oNewRect.dTop)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(oNewRect);

		WriteNode(L"rect", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x", std::to_wstring(oNewRect.dLeft)},
									   {L"y", std::to_wstring(oNewRect.dTop)},
									   {L"width", std::to_wstring(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height", std::to_wstring(oNewRect.dBottom - oNewRect.dTop)},
									   {L"rx", std::to_wstring(TranslateX(oCorner.cx))},
									   {L"ry", std::to_wstring(TranslateY(oCorner.cy))}};

		AddStroke(arAttributes);
		AddFill(arAttributes);

		UpdateTransform(oNewRect);

		WriteNode(L"rect", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_UNKNOWN(CDataStream &oDataStream)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{

	}

	void CEmfInterpretatorSvg::Begin()
	{
		m_oXmlWriter.WriteNodeBegin(L"svg", true);
		m_oXmlWriter.WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
		m_oXmlWriter.WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");
		m_oXmlWriter.WriteNodeEnd(L"svg", true, false);
	}

	void CEmfInterpretatorSvg::End()
	{
		m_oXmlWriter.WriteNodeEnd(L"svg", false, false);

		std::wstring wsXml = m_oXmlWriter.GetXmlString();

		bool bFlipped = false;

		if (NULL != m_pParser)
		{
			int nFlipX = 1;
			int nFlipY = 1;

			if (m_pParser->IsWindowFlippedX())
			{
				nFlipX = -1;
				bFlipped = true;
			}

			if (m_pParser->IsWindowFlippedY())
			{
				nFlipY = -1;
				bFlipped = true;
			}

			if (nFlipX < 0 || nFlipY < 0 || bFlipped)
				wsXml.insert(5, L"transform=\"scale(" + std::to_wstring(nFlipX) + L' ' + std::to_wstring(nFlipY) + L")\" ");
		}

		//                if (m_oViewport.dX < 0 || m_oViewport.dY < 0)
		if (!m_oViewport.Empty())
			wsXml.insert(5, L"viewBox=\"" + std::to_wstring(m_oViewport.dLeft) + L' ' + std::to_wstring(m_oViewport.dTop) + L' ' + std::to_wstring(m_oViewport.GetWidth()) + L' ' + std::to_wstring(m_oViewport.GetHeight()) + L"\" ");

		if (0 != m_oSizeWindow.cx && 0 != m_oSizeWindow.cy)
			wsXml.insert(5, L"width=\"" + std::to_wstring(m_oSizeWindow.cx) + L"\" height=\"" + std::to_wstring(m_oSizeWindow.cy) + L"\" ");

		m_oXmlWriter.SetXmlString(wsXml);

		m_oXmlWriter.SaveToFile((!m_wsSvgFilePath.empty()) ? m_wsSvgFilePath : L"temp.svg");
	}

	void CEmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		if (0 == unWidth || 0 == unHeight)
			return;

		CBgraFrame  oFrame;

		oFrame.put_Data(pBuffer);
		oFrame.put_Width(unWidth);
		oFrame.put_Height(unHeight);

		BYTE* pNewBuffer = NULL;
		int nNewSize = 0;

		oFrame.Encode(pNewBuffer, nNewSize, 4);
		oFrame.put_Data(NULL);

		if (0 < nNewSize)
		{
			char* pImageData = NULL;
			int nImageSize = 0;

			NSFile::CBase64Converter::Encode(pNewBuffer, nNewSize, pImageData, nImageSize, NSBase64::B64_BASE64_FLAG_NOCRLF);

			if (NULL == pImageData)
				return;

			double dNewX = TranslateX(dX);
			double dNewY = TranslateY(dY);

			std::wstring sImageDataW = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pImageData, (LONG)nImageSize);

			NodeAttributes arAttributes = {{L"x", std::to_wstring(dNewX)},
										   {L"y", std::to_wstring(dNewY)},
										   {L"width", std::to_wstring(TranslateX(dW))},
										   {L"height", std::to_wstring(TranslateY(dH))},
										   {L"xlink:href", L"data:image/png;base64," + sImageDataW}};

			UpdateTransform(dNewX, dNewY);

			WriteNode(L"image", arAttributes);

			RELEASEARRAYOBJECTS(pImageData);
		}

		if (NULL != pNewBuffer)
			delete [] pNewBuffer;
	}

	void CEmfInterpretatorSvg::WriteNode(const std::wstring &wsNodeName, const NodeAttributes &arAttributes, const std::wstring &wsValueNode)
	{
		m_oXmlWriter.WriteNodeBegin(wsNodeName, true);

		for (const NodeAttribute& oAttribute : arAttributes)
			m_oXmlWriter.WriteAttribute(oAttribute.first, oAttribute.second);

		if (wsValueNode.empty())
		{
			m_oXmlWriter.WriteNodeEnd(wsNodeName, true, true);
		}
		else
		{
			m_oXmlWriter.WriteNodeEnd(wsNodeName, true, false);

			m_oXmlWriter.WriteString(wsValueNode);

			m_oXmlWriter.WriteNodeEnd(wsNodeName, false, false);
		}
	}

	void CEmfInterpretatorSvg::WriteText(const std::wstring &wsText, double dX, double dY, const TEmfRectL& oBounds)
	{
		NodeAttributes arNodeAttributes;

		double dXCoord = TranslateX(dX);
		double dYCoord = TranslateY(dY);

		if (NULL != m_pParser && NULL != m_pParser->GetFont())
		{
			if (OPAQUE == m_pParser->GetTextBgMode())
			{
				std::wstring wsFillRect = L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor()) + L", 255)";

				WriteNode(L"rect", {{L"x",      std::to_wstring((oBounds.lLeft))},
									{L"y",      std::to_wstring((oBounds.lTop))},
									{L"width",  std::to_wstring((oBounds.lRight - oBounds.lLeft))},
									{L"height", std::to_wstring((oBounds.lBottom - oBounds.lTop))},
									{L"fill", wsFillRect},
									{L"stroke", L"none"}});
			}


			TEmfColor oColor = m_pParser->GetDC()->GetTextColor();

			if (0 != oColor.r || 0 != oColor.g || 0 != oColor.b)
				arNodeAttributes.push_back({L"fill", L"rgba(" + std::to_wstring((int)oColor.r) + L", " + std::to_wstring((int)oColor.g) + L", " + std::to_wstring((int)oColor.b) + L", 255)"});

			IFont *pFont = m_pParser->GetFont();

			double dFontHeight = std::fabs(TranslateY(pFont->GetHeight()));

			if (dFontHeight < 0.01)
				dFontHeight = 18;

			arNodeAttributes.push_back({L"font-size", std::to_wstring(dFontHeight)});

			std::wstring wsFaceName = pFont->GetFaceName();

			if (!wsFaceName.empty())
				arNodeAttributes.push_back({L"font-family", wsFaceName});

			if (pFont->GetWeight() > 550)
				arNodeAttributes.push_back({L"font-weight", L"bold"});

			if (pFont->IsItalic())
				arNodeAttributes.push_back({L"font-style", L"italic"});


			if (pFont->IsUnderline() && pFont->IsStrikeOut())
				arNodeAttributes.push_back({L"text-decoration", L"underline line-through"});
			else if (pFont->IsUnderline())
				arNodeAttributes.push_back({L"text-decoration", L"underline"});
			else if (pFont->IsStrikeOut())
				arNodeAttributes.push_back({L"text-decoration", L"line-through"});

			//TODO:: разобраться для корректной работы
			//                        double dFontCharSpace = TranslateX(pFont->GetCharSet());

			//                        if (dFontCharSpace > 1)
			//                                arNodeAttributes.push_back({L"letter-spacing", std::to_wstring(dFontCharSpace)});

			unsigned int ulTextAlign = m_pParser->GetTextAlign();
			if (ulTextAlign & TA_BASELINE)
			{
				// Ничего не делаем
			}
			else if (ulTextAlign & TA_BOTTOM)
			{
				arNodeAttributes.push_back({L"alignment-baseline", L"bottom"});
			}
			else // if (ulTextAlign & TA_TOP)
			{
				arNodeAttributes.push_back({L"alignment-baseline", L"top"});
			}

			if (ulTextAlign & TA_CENTER)
			{
				arNodeAttributes.push_back({L"text-anchor", L"middle"});
			}
			else if (ulTextAlign & TA_RIGHT)
			{
				arNodeAttributes.push_back({L"text-anchor", L"end"});
			}
			else //if (ulTextAlign & TA_LEFT)
			{
				// Ничего не делаем
			}

			if (0 != pFont->GetEscapement())
				arNodeAttributes.push_back({L"transform", L"rotate(" + std::to_wstring(pFont->GetEscapement() / -10) + L' ' + std::to_wstring(dXCoord) + L' ' + std::to_wstring(dYCoord) + L')'});

			UpdateTransform(TranslateRect(oBounds));
		}
		else
			UpdateTransform(TranslateX(dX), TranslateX(dY));

		arNodeAttributes.push_back({L"x", std::to_wstring(dXCoord)});
		arNodeAttributes.push_back({L"y", std::to_wstring(dYCoord)});

		WriteNode(L"text", arNodeAttributes, StringNormalization(wsText));
	}

	void CEmfInterpretatorSvg::AddStroke(NodeAttributes &arAttributes)
	{
		if (NULL != m_pParser && NULL != m_pParser->GetPen())
		{
			arAttributes.push_back({L"stroke", L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetPen()->GetColor()) + L"," + std::to_wstring(m_pParser->GetPen()->GetAlpha()) + L")"});

			double dStrokeWidth = TranslateY(m_pParser->GetPen()->GetWidth());

			if (dStrokeWidth < 1)
				dStrokeWidth = 1;

			if (dStrokeWidth > 0)
				arAttributes.push_back({L"stroke-width", std::to_wstring(dStrokeWidth)});

			unsigned int unMetaPenStyle = m_pParser->GetPen()->GetStyle();
			//			unsigned int ulPenType      = unMetaPenStyle & PS_TYPE_MASK;
			unsigned int ulPenStyle     = unMetaPenStyle & PS_STYLE_MASK;

			if (PS_DASH == ulPenStyle)
				arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth * 4) + L' ' + std::to_wstring(dStrokeWidth * 2)});
			else if (PS_DOT == ulPenStyle)
				arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth)});
			else if (PS_DASHDOT == ulPenStyle)
				arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth * 4) + L' ' + std::to_wstring(dStrokeWidth * 2) + L' ' + std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth * 2)});
			else if (PS_DASHDOTDOT == ulPenStyle)
				arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth * 4) + L' ' + std::to_wstring(dStrokeWidth * 2) + L' ' + std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth * 2) + L' ' + std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth * 2)});
			else
			{
				unsigned int ulPenStartCap  = unMetaPenStyle & PS_STARTCAP_MASK;
				unsigned int ulPenEndCap    = unMetaPenStyle & PS_ENDCAP_MASK;
				unsigned int ulPenJoin      = unMetaPenStyle & PS_JOIN_MASK;

				// svg не поддерживает разные стили для сторон линии
				if (PS_STARTCAP_FLAT == ulPenStartCap || PS_ENDCAP_FLAT == ulPenEndCap)
					arAttributes.push_back({L"stroke-linecap", L"butt"});
				else if (PS_STARTCAP_SQUARE == ulPenStartCap || PS_ENDCAP_SQUARE == ulPenEndCap)
					arAttributes.push_back({L"stroke-linecap", L"square"});
				else if (PS_STARTCAP_ROUND == ulPenStartCap || PS_ENDCAP_ROUND == ulPenEndCap)
					arAttributes.push_back({L"stroke-linecap", L"round"});

				if (PS_JOIN_MITER == ulPenJoin)
					arAttributes.push_back({L"stroke-linejoin", L"miter"});
				else if (PS_JOIN_BEVEL == ulPenJoin)
					arAttributes.push_back({L"stroke-linejoin", L"bevel"});
				else if (PS_JOIN_ROUND == ulPenJoin)
					arAttributes.push_back({L"stroke-linejoin", L"round"});
			}
		}
		else arAttributes.push_back({L"stroke", L"black"});
	}

	void CEmfInterpretatorSvg::AddFill(NodeAttributes &arAttributes)
	{
		if (NULL != m_pParser && NULL != m_pParser->GetBrush())
		{
			if (BS_SOLID == m_pParser->GetBrush()->GetStyle())
				arAttributes.push_back({L"fill", L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetBrush()->GetColor()) + L"," + std::to_wstring(m_pParser->GetBrush()->GetAlpha()) + L")"});
			else
				arAttributes.push_back({L"fill", L"none"});
		}
		else arAttributes.push_back({L"fill", L"none"});
	}

	void CEmfInterpretatorSvg::UpdateTransform(double dX, double dY)
	{
		if (dX < m_oViewport.dLeft)
			m_oViewport.dLeft = dX;

		if (dX > m_oViewport.dRight)
			m_oViewport.dRight = dX;

		if (dY < m_oViewport.dTop)
			m_oViewport.dTop = dY;

		if (dY > m_oViewport.dBottom)
			m_oViewport.dBottom = dY;
	}

	void CEmfInterpretatorSvg::UpdateTransform(const TRectD &oRect)
	{
		UpdateTransform(oRect.dLeft, oRect.dTop);
		UpdateTransform(oRect.dRight, oRect.dBottom);
	}

	void CEmfInterpretatorSvg::UpdateTransform(const std::vector<TEmfPointL> &arPoints, const NodeAttributes& arAttributes)
	{
		short shMinX = SHRT_MAX, shMinY = SHRT_MAX;
		short shMaxX = SHRT_MIN, shMaxY = SHRT_MIN;

		for (const TEmfPointL& oPoint : arPoints)
		{
			if (oPoint.x < shMinX) shMinX = oPoint.x;
			if (oPoint.x > shMaxX) shMaxX = oPoint.x;

			if (oPoint.y < shMinY) shMinY = oPoint.y;
			if (oPoint.y > shMaxY) shMaxY = oPoint.y;
		}

		double dShift = 0;

		if (!arAttributes.empty())
			for (const NodeAttribute& oAttribute : arAttributes)
				if (L"stroke-width" == oAttribute.first)
					dShift = std::stod(oAttribute.second) / 2;

		if (SHRT_MAX != shMinX && SHRT_MAX != shMinY)
			UpdateTransform(TranslateX(shMinX) - dShift, TranslateY(shMinY) - dShift);

		if (SHRT_MIN != shMaxX && SHRT_MIN != shMaxY)
			UpdateTransform(TranslateX(shMaxX) + dShift, TranslateY(shMaxY) + dShift);
	}

	void CEmfInterpretatorSvg::UpdateTransform(const std::vector<TEmfPointS> &arPoints, const NodeAttributes& arAttributes)
	{
		short shMinX = SHRT_MAX, shMinY = SHRT_MAX;
		short shMaxX = SHRT_MIN, shMaxY = SHRT_MIN;

		for (const TEmfPointS& oPoint : arPoints)
		{
			if (oPoint.x < shMinX) shMinX = oPoint.x;
			if (oPoint.x > shMaxX) shMaxX = oPoint.x;

			if (oPoint.y < shMinY) shMinY = oPoint.y;
			if (oPoint.y > shMaxY) shMaxY = oPoint.y;
		}

		double dShift = 0;

		if (!arAttributes.empty())
			for (const NodeAttribute& oAttribute : arAttributes)
				if (L"stroke-width" == oAttribute.first)
					dShift = std::stod(oAttribute.second) / 2;

		if (SHRT_MAX != shMinX && SHRT_MAX != shMinY)
			UpdateTransform(TranslateX(shMinX) - dShift, TranslateY(shMinY) - dShift);

		if (SHRT_MIN != shMaxX && SHRT_MIN != shMaxY)
			UpdateTransform(TranslateX(shMaxX) + dShift, TranslateY(shMaxY) + dShift);
	}

	void CEmfInterpretatorSvg::UpdateTransform(TEmfPointL *arPoints, unsigned int unCount)
	{
		if (NULL == arPoints || 0 == unCount)
			return;

		double dMinX = 0, dMinY = 0;
		for (unsigned int unIndex = 0; unIndex < unCount; ++unCount)
		{
			if (arPoints[unIndex].x < dMinX) dMinX = arPoints[unIndex].x;
			if (arPoints[unIndex].y < dMinY) dMinY = arPoints[unIndex].y;
		}

		if (0 != dMinX || 0 != dMinY)
			UpdateTransform(dMinX, dMinY);
	}

	void CEmfInterpretatorSvg::UpdateTransform(TEmfPointS *arPoints, unsigned int unCount)
	{
		if (NULL == arPoints || 0 == unCount)
			return;

		short shMinX = 0, shMinY = 0;
		for (unsigned int unIndex = 0; unIndex < unCount; ++unCount)
		{
			if (arPoints[unIndex].x < shMinX) shMinX = arPoints[unIndex].x;
			if (arPoints[unIndex].y < shMinY) shMinY = arPoints[unIndex].y;
		}

		if (0 != shMinX || 0 != shMinY)
			UpdateTransform(shMinX, shMinY);
	}

	double CEmfInterpretatorSvg::TranslateX(double dX)
	{
		if (NULL != m_pParser && NULL != m_pParser->GetTransform())
			return dX * m_pParser->GetTransform()->M11;

		return  dX;
	}

	double CEmfInterpretatorSvg::TranslateY(double dY)
	{
		if (NULL != m_pParser && NULL != m_pParser->GetTransform())
			return dY * m_pParser->GetTransform()->M22;

		return dY;
	}

	TPointD CEmfInterpretatorSvg::TranslatePoint(const TPointD &oPoint)
	{
		TPointD oNewPoint;

		oNewPoint.x = TranslateX(oPoint.x);
		oNewPoint.y = TranslateY(oPoint.y);

		return oNewPoint;
	}

	TRectD CEmfInterpretatorSvg::TranslateRect(const TEmfRectL &oRect)
	{
		TRectD oNewRect;

		oNewRect.dLeft   = TranslateX(oRect.lLeft);
		oNewRect.dTop    = TranslateY(oRect.lTop);
		oNewRect.dRight  = TranslateX(oRect.lRight);
		oNewRect.dBottom = TranslateY(oRect.lBottom);

		if (oNewRect.dRight < oNewRect.dLeft)
		{
			double dTempValue = oNewRect.dLeft;
			oNewRect.dLeft    = oNewRect.dRight;
			oNewRect.dRight   = dTempValue;
		}

		if (oNewRect.dBottom < oNewRect.dTop)
		{
			double dTempValue = oNewRect.dTop;
			oNewRect.dTop     = oNewRect.dBottom;
			oNewRect.dBottom  = dTempValue;
		}

		return oNewRect;
	}

	TPointD CEmfInterpretatorSvg::GetCutPos()
	{
		if (NULL != m_pParser)
			return TranslatePoint(m_pParser->GetCurPos());

		TPointD oCurPos;

		oCurPos.x = m_oViewport.dLeft;
		oCurPos.y = m_oViewport.dTop;

		return oCurPos;
	}
}

