#include "CEmfInterpretatorSvg.h"

#include "../../Common/MetaFileUtils.h"

#include "../../../../graphics/Image.h"

#include "../../../BgraFrame.h"

#include <algorithm>
#include <cmath>

#ifndef MININT8
#define MAXUINT8    ((unsigned char)~((unsigned char)0))
#define MAXINT8     ((char)(MAXUINT8 >> 1))
#define MININT8     ((char)~MAXINT8)
#endif

#define MAXTRANSFORMSCALE 100.
#define MINTRANSFORMSCALE 0.00000001

namespace MetaFile
{               
	CEmfInterpretatorSvg::CEmfInterpretatorSvg(CEmfParserBase* pParser, double dWidth, double dHeight)
	    : m_pParser(pParser), m_unNumberDefs(0), m_oScale(1, 1)
	{
		SetSize(dWidth, dHeight);
	}

	CEmfInterpretatorSvg::~CEmfInterpretatorSvg()
	{

	}

	void CEmfInterpretatorSvg::CreateConditional(IMetaFileBase *pParser)
	{
		m_oSecondConditional.m_pParser = dynamic_cast<CEmfParserBase*>(pParser);
	}

	void CEmfInterpretatorSvg::ChangeConditional()
	{
		if (NULL == m_oSecondConditional.m_pParser)
			return;

		std::swap(m_wsLastClipId, m_oSecondConditional.m_wsLastClipId);
		std::swap(m_pParser, m_oSecondConditional.m_pParser);
	}

	InterpretatorType CEmfInterpretatorSvg::GetType() const
	{
		return InterpretatorType::Svg;
	}

	void CEmfInterpretatorSvg::SetSize(double dWidth, double dHeight)
	{
		m_oSizeWindow.x = dWidth;
		m_oSizeWindow.y = dHeight;
	}

	void CEmfInterpretatorSvg::GetSize(double &dWidth, double &dHeight)
	{
		dWidth  = m_oSizeWindow.x;
		dHeight = m_oSizeWindow.y;
	}

	void CEmfInterpretatorSvg::UpdateSize()
	{
		if (0 != m_oSizeWindow.x && 0 == m_oSizeWindow.y)
			m_oSizeWindow.y = m_oSizeWindow.x * (m_oViewport.GetHeight() / m_oViewport.GetWidth());
		else if (0 == m_oSizeWindow.x && 0 != m_oSizeWindow.y)
			m_oSizeWindow.x = m_oSizeWindow.y * (m_oViewport.GetWidth() / m_oViewport.GetHeight());
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
	{
		m_oViewport.dLeft   = oTEmfHeader.oFramePx.lLeft;
		m_oViewport.dTop    = oTEmfHeader.oFramePx.lTop;
		m_oViewport.dRight  = oTEmfHeader.oFramePx.lRight;
		m_oViewport.dBottom = oTEmfHeader.oFramePx.lBottom;

		m_oScale.dX = (double)oTEmfHeader.oMillimeters.cx / (double)oTEmfHeader.oDevice.cx;
		m_oScale.dY = (double)oTEmfHeader.oMillimeters.cy / (double)oTEmfHeader.oDevice.cy;

		m_oXmlWriter.WriteNodeBegin(L"svg", true);
		m_oXmlWriter.WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
		m_oXmlWriter.WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");

		UpdateSize();

		if (m_oViewport.GetWidth() != 0)
			m_oXmlWriter.WriteAttribute(L"width", ConvertToWString(m_oViewport.GetWidth()));

		if (m_oViewport.GetHeight() != 0)
			m_oXmlWriter.WriteAttribute(L"height", ConvertToWString(m_oViewport.GetHeight()));

		double dXScale = 1, dYScale = 1, dXTranslate = 0, dYTranslate = 0;

		if (0 != m_oSizeWindow.x)
		{
			dXScale = m_oSizeWindow.x / m_oViewport.GetWidth();
			dXTranslate = m_oViewport.GetWidth() / 2 * std::abs(dXScale - 1);

			if (dXScale < 1)
				dXTranslate = -dXTranslate;
		}

		if (0 != m_oSizeWindow.y)
		{
			dYScale = m_oSizeWindow.y / m_oViewport.GetHeight();
			dYTranslate = m_oViewport.GetHeight() / 2 * std::abs(dYScale - 1);

			if (dYScale < 1)
				dYTranslate = -dYTranslate;
		}

		if (1 != dXScale || 1 != dYScale)
			m_oXmlWriter.WriteAttribute(L"transform", L"matrix(" + std::to_wstring(dXScale) + L",0,0," + std::to_wstring(dYScale) + L',' + ConvertToWString(dXTranslate) + L',' + ConvertToWString(dYTranslate) + L')');

		m_oXmlWriter.WriteNodeEnd(L"svg", true, false);
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
		m_oXmlWriter.WriteString(m_wsDefs);
		m_oXmlWriter.WriteNodeEnd(L"svg", false, false);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SAVEDC()
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		ResetClip();
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
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath();

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
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

	void CEmfInterpretatorSvg::HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
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
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
	{
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{
		ResetClip();
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
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
	{
		double dXRadius = unRadius;
		double dYRadius = unRadius;

		double dXCenter = oCenter.x;
		double dYCenter = oCenter.y;

		double dStartX = dXCenter + dXRadius  * cos((dStartAngle) * M_PI / 180);
		double dStartY = dYCenter + dYRadius  * sin((dStartAngle) * M_PI / 180);

		double dEndX = dXCenter + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = dYCenter + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		std::wstring wsValue = L"M " + ConvertToWString(dStartX) + L' ' + ConvertToWString(dStartY);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
				ConvertToWString(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				ConvertToWString(dEndX) + L' ' +
				ConvertToWString(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetDC()->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
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

		double dEndX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		TEmfPointL oStartPoint;

		dStartAngle *= -M_PI / 180;

		double dWidth  = oBox.lRight  - oBox.lLeft;
		double dHeight = oBox.lBottom - oBox.lTop;

		double dTan = atan2( sin( dStartAngle ) / dHeight / 2,  cos( dStartAngle ) / dWidth / 2 );

		oStartPoint.x = oBox.lLeft + dWidth / 2.0 + dWidth / 2 * cos(dTan);
		oStartPoint.y = oBox.lTop + dHeight / 2.0 - dHeight / 2 * sin(dTan);

		std::wstring wsValue = L"M " + ConvertToWString(oStartPoint.x) + L' ' + ConvertToWString(oStartPoint.y);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
				ConvertToWString(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				ConvertToWString(dEndX) + L' ' +
				ConvertToWString(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetDC()->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
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

		std::wstring wsValue = L"M " + ConvertToWString(dStartX) + L' ' + ConvertToWString(dStartY);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
				ConvertToWString(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				ConvertToWString(dEndX) + L' ' +
				ConvertToWString(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetDC()->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		//TODO:: реализовать
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
									   {L"cy", ConvertToWString((oNewRect.dTop    + oNewRect.dBottom) / 2)},
									   {L"rx", ConvertToWString((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
									   {L"ry", ConvertToWString((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"ellipse", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutA.aEmrText.OutputString, oTEmfExtTextoutA.aEmrText.Chars);

		WriteText(wsText, oTEmfExtTextoutA.aEmrText.Reference.x, oTEmfExtTextoutA.aEmrText.Reference.y, oTEmfExtTextoutA.Bounds, oTEmfExtTextoutA.exScale, oTEmfExtTextoutA.eyScale);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutW.wEmrText.OutputString, oTEmfExtTextoutW.wEmrText.Chars);

		WriteText(wsText, oTEmfExtTextoutW.wEmrText.Reference.x, oTEmfExtTextoutW.wEmrText.Reference.y, oTEmfExtTextoutW.Bounds, oTEmfExtTextoutW.exScale, oTEmfExtTextoutW.eyScale);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
	{
		TPointD oCurPos = GetCutPos();

		NodeAttributes arAttributes = {{L"x1", ConvertToWString(oCurPos.x)},
									   {L"y1", ConvertToWString(oCurPos.y)},
									   {L"x2", ConvertToWString(oPoint.x)},
									   {L"y2", ConvertToWString(oPoint.y)}};

		AddStroke(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"line", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		short shCenterX = (oBox.lLeft + oBox.lRight) / 2;
		short shCenterY = (oBox.lTop + oBox.lBottom) / 2;

		short shRadiusX = std::abs(oBox.lRight - oBox.lLeft) / 2;
		short shRadiusY = std::abs(oBox.lBottom - oBox.lTop) / 2;

		std::wstring wsPath = L'M' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L' ' +
		                      L'L' + ConvertToWString(oStart.x)+ L' ' + ConvertToWString(oStart.y)+ L' ' +
		                      L'A' + ConvertToWString(shRadiusX) + L' ' + ConvertToWString(shRadiusY) + L" 0, 0, 0, " + ConvertToWString(oEnd.x) + L' ' + ConvertToWString(oEnd.y) + L' ' +
		                      L'L' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L" Z";

		NodeAttributes arAttributes = {{L"d", wsPath}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
			return;

		std::wstring wsValue;

		BYTE oLastType = 0x00;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			if (0x02 == pAbTypes[unIndex])
			{
				if (oLastType != 0x02)
				{
					wsValue += L" L ";
					oLastType = 0x02;
				}
				wsValue += ConvertToWString(arPoints[unIndex].x) + L',' + ConvertToWString(arPoints[unIndex].y) + L' ';
			}
			else if (0x04 == pAbTypes[unIndex] && unIndex + 2 < unCount)
			{
				if (oLastType != 0x02)
				{
					wsValue += L" C ";
					oLastType = 0x04;
				}
				wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L',' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
							ConvertToWString(arPoints[unIndex + 1].x) + L',' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
							ConvertToWString(arPoints[unIndex + 2].x) + L',' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
			{
				wsValue += L" M " + ConvertToWString(arPoints[unIndex].x) + L' ' + ConvertToWString(arPoints[unIndex].y) + L' ';
				oLastType = 0x06;
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
			return;

		std::wstring wsValue;

		BYTE oLastType = 0x00;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			if (0x02 == pAbTypes[unIndex])
			{
				if (oLastType != 0x02)
				{
					wsValue += L" L ";
					oLastType = 0x02;
				}
				wsValue += ConvertToWString(arPoints[unIndex].x) + L',' + ConvertToWString(arPoints[unIndex].y) + L' ';
			}
			else if (0x04 == pAbTypes[unIndex] && unIndex + 2 < unCount)
			{
				if (oLastType != 0x02)
				{
					wsValue += L" C ";
					oLastType = 0x04;
				}
				wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L',' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
							ConvertToWString(arPoints[unIndex + 1].x) + L',' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
							ConvertToWString(arPoints[unIndex + 2].x) + L',' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
			{
				wsValue += L" M " + ConvertToWString(arPoints[unIndex].x) + L' ' + ConvertToWString(arPoints[unIndex].y) + L' ';
				oLastType = 0x06;
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polygon", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polygon", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		wsValue.pop_back();

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = ConvertToWString(oCurPos.x) + L',' + ConvertToWString(oCurPos.y);

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += L' ' + ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y);

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = ConvertToWString(oCurPos.x) + L',' + ConvertToWString(oCurPos.y);

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += L' ' + ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y);

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointL>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';

			for (const TEmfPointL& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointS>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';

			for (const TEmfPointS& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointL>& oPolyline : arPolygons)
		{
			if (oPolyline.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';

			for (const TEmfPointL& oPoint : oPolyline)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointS>& oPolyline : arPolygons)
		{
			if (oPolyline.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';

			for (const TEmfPointS& oPoint : oPolyline)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.dLeft)},
									   {L"y",		ConvertToWString(oNewRect.dTop)},
									   {L"width",	ConvertToWString(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height",	ConvertToWString(oNewRect.dBottom - oNewRect.dTop)}};

		AddStroke(arAttributes);
		AddFill(arAttributes, oNewRect.dRight - oNewRect.dLeft, oNewRect.dBottom - oNewRect.dTop);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);


		WriteNode(L"rect", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.dLeft)},
									   {L"y",		ConvertToWString(oNewRect.dTop)},
									   {L"width",	ConvertToWString(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height",	ConvertToWString(oNewRect.dBottom - oNewRect.dTop)},
									   {L"rx",		ConvertToWString(oCorner.cx)},
									   {L"ry",		ConvertToWString(oCorner.cy)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"rect", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath();

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath();

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_UNKNOWN(CDataStream &oDataStream)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || arRects.empty())
			return;

		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfRectL& oRect : arRects)
		{
			oTempRect = TranslateRect(oRect);

			wsValue +=	L"M " + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			            L"L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                    ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_PAINTRGN(const TEmfRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || arRects.empty())
			return;

		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfRectL& oRect : arRects)
		{
			oTempRect = TranslateRect(oRect);

			wsValue +=	L"M " + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			            L"L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                    ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FRAMERGN(const TEmfRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_OFFSETCLIP(double dX, double dY)
	{
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_RESETCLIP()
	{
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPPATH(short unShFlags, const CEmfPlusPath *pPath)
	{
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPRECT(short shCM, const TEmfPlusRectF &oRect)
	{
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPREGION(short shObjectIndex, short shCM, const CEmfPlusRegion *pRegion)
	{
		ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_CLEAR(const TEmfPlusARGB &oARGB)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWARC(BYTE chOgjectIndex, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.GetRectD();

		if (NULL != m_pParser && m_pParser->GetDC()->GetFinalTransform(GM_ADVANCED)->M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.dRight - oNewRect.dLeft)) / 2;
		double dYRadius = std::fabs((oNewRect.dBottom - oNewRect.dTop)) / 2;

		std::wstring wsValue = L"M " + ConvertToWString(oNewRect.dLeft) + L' ' + ConvertToWString(oNewRect.dTop);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
		        ConvertToWString(dYRadius) + L' ' +
		        L"0 " +
		        ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
		        ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
		        ConvertToWString(oNewRect.dRight) + L' ' +
		        ConvertToWString(oNewRect.dBottom);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetDC()->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWBEZIERS(short shOgjectIndex, const std::vector<TEmfPlusPointF> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].X) + L' ' + ConvertToWString(arPoints[0].Y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].X)     + L' ' + ConvertToWString(arPoints[unIndex].Y)     + L' ' +
			            ConvertToWString(arPoints[unIndex + 1].X) + L' ' + ConvertToWString(arPoints[unIndex + 1].Y) + L' ' +
			            ConvertToWString(arPoints[unIndex + 2].X) + L' ' + ConvertToWString(arPoints[unIndex + 2].Y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWCLOSEDCURVE(short shOgjectIndex, double dTension, const std::vector<TEmfPlusPointF> &arPoints)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWCURVE(short shOgjectIndex, double dTension, unsigned int unOffset, unsigned int unNumSegments, const std::vector<TEmfPlusPointF> &arPoints)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWDRIVERSTRING(short shOgjectIndex, unsigned int unBrushId, unsigned int unDriverStringOptionsFlags, unsigned int unMatrixPresent, TXForm *pMatrix, const std::wstring &wsString, const std::vector<TPointD> &arGlyphPos)
	{
		if (NULL == m_pParser || wsString.size() != arGlyphPos.size())
			return;

		IFont *pFont = m_pParser->GetFont();

		if (NULL == pFont)
			return;

		NodeAttributes arNodeAttributes;

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		TEmfColor oColor = m_pParser->GetDC()->GetTextColor();

		if (0 != oColor.r || 0 != oColor.g || 0 != oColor.b)
			arNodeAttributes.push_back({L"fill", L"rgb(" + ConvertToWString(oColor.r, 0) + L", " + ConvertToWString(oColor.g, 0) + L", " + ConvertToWString(oColor.b, 0) + L')'});

		double dFontHeight = std::fabs(pFont->GetHeight());

		if (dFontHeight < 0.01)
			dFontHeight = 18;

		arNodeAttributes.push_back({L"font-size", ConvertToWString(dFontHeight)});

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

		std::wstring wsValue;

		for (unsigned int unIndex = 0; unIndex < arGlyphPos.size(); ++unIndex)
			wsValue += L"<tspan x=\"" + ConvertToWString(arGlyphPos[unIndex].x) + L"\" y=\"" + ConvertToWString(arGlyphPos[unIndex].y) + L"\">" + StringNormalization(std::wstring(1, wsString[unIndex])) + L"</tspan>";

		AddTransform(arNodeAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"text", arNodeAttributes, wsValue);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWELLIPSE(short shOgjectIndex, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.GetRectD();

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.dTop    + oNewRect.dBottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"ellipse", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWIMAGE(short shOgjectIndex, unsigned int, const TEmfPlusRectF &, const TEmfPlusRectF &)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWIMAGEPOINTS(short shOgjectIndex, unsigned int, const TEmfPlusRectF &, const TEmfPlusRectF &)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWLINES(short shOgjectIndex, const std::vector<TEmfPlusPointF> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPlusPointF& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWPATH(short shOgjectIndex, unsigned int unPenId, const CEmfPath* pPath)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath(pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWPIE(short shOgjectIndex, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		//TODO: Реализовать при встрече
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWRECTS(short shOgjectIndex, const std::vector<TEmfPlusRectF> &arRects)
	{
		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfPlusRectF& oRect : arRects)
		{
			oTempRect = oRect.GetRectD();

			wsValue +=	L"M "  + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) +
			            L" L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                     ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWSTRING(short shOgjectIndex, unsigned int unBrushId, unsigned int unFormatID, const std::wstring &wsString, const TEmfPlusRectF &oRect)
	{
		WriteText(wsString, oRect.dX, oRect.dY, oRect.GetRectL(), m_pParser->GetTransform()->M11, m_pParser->GetTransform()->M22);
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLCLOSEDCURVE(unsigned int unBrushId, double dTension, const std::vector<TEmfPlusRectF> &arPoints)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLELLIPSE(unsigned int unBrushId, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.GetRectD();

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.dTop    + oNewRect.dBottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.dBottom - oNewRect.dTop)    / 2)}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"ellipse", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLPATH(short shOgjectIndex, unsigned int unBrushId, const CEmfPlusPath *pPath)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath(pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		TRectD oPathRect = pPath->ConvertToRect();

		AddFill(arAttributes, oPathRect.dRight - oPathRect.dLeft, oPathRect.dBottom - oPathRect.dTop);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLPIE(unsigned int unBrushId, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLPOLYGON(unsigned int unBrushId, const std::vector<TEmfPlusPointF> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPlusPointF& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polygon", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLRECTS(unsigned int unBrushId, const std::vector<TEmfPlusRectF> &arRects)
	{
		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfPlusRectF& oRect : arRects)
		{
			oTempRect = oRect.GetRectD();

			m_pParser->GetTransform()->Apply(oTempRect.dLeft,  oTempRect.dTop);
			m_pParser->GetTransform()->Apply(oTempRect.dRight, oTempRect.dBottom);

			wsValue +=	L"M "  + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) +
			            L" L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                     ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLREGION(short shOgjectIndex, unsigned int unBrushId)
	{

	}

	void CEmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		if (NULL == pBuffer || 0 == dW || 0 == dH || 0 == unWidth || 0 == unHeight)
			return;

		if (1 == unWidth && 1 == unHeight)
		{
			NodeAttributes arAttributes = {{L"x",      ConvertToWString(dX)},
			                               {L"y",      ConvertToWString(dY)},
			                               {L"width",  ConvertToWString(dW)},
			                               {L"height", ConvertToWString(dH)},
			                               {L"fill", L"rgb(" + std::to_wstring(pBuffer[2]) + L',' + std::to_wstring(pBuffer[1]) + L',' + std::to_wstring(pBuffer[0]) + L',' + std::to_wstring(pBuffer[3]) + L')'}};

			AddTransform(arAttributes);

			WriteNode(L"rect", arAttributes);

			return;
		}

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
			int nImageSize = NSBase64::Base64EncodeGetRequiredLength(nNewSize);
			unsigned char* ucValue = new unsigned char[nImageSize];

			if (NULL == ucValue)
				return;

			NSBase64::Base64Encode(pNewBuffer, nNewSize, ucValue, &nImageSize);
			std::wstring wsValue(ucValue, ucValue + nImageSize);

			RELEASEARRAYOBJECTS(ucValue);

			NodeAttributes arAttributes = {{L"x",      ConvertToWString(dX)},
			                               {L"y",      ConvertToWString(dY)},
			                               {L"width",  ConvertToWString(dW)},
			                               {L"height", ConvertToWString(dH)},
			                               {L"xlink:href", L"data:image/png;base64," + wsValue}};

			AddTransform(arAttributes);

			NodeAttributes arGAttributes;
			AddClip(arGAttributes);

			if (!m_wsLastClipId.empty())
				WriteNodeBegin(L"g", arGAttributes);

			WriteNode(L"image", arAttributes);

			if (!m_wsLastClipId.empty())
				WriteNodeEnd(L"g");
		}

		if (NULL != pNewBuffer)
			delete [] pNewBuffer;
	}

	void CEmfInterpretatorSvg::ResetClip()
	{
		m_wsLastClipId.clear();
	}

	void CEmfInterpretatorSvg::IntersectClip(const TRectD &oClip)
	{
		m_wsLastClipId = L"INTERSECTCLIP_" + ConvertToWString(++m_unNumberDefs, 0);

		TXForm *pTransform = m_pParser->GetTransform();

		m_wsDefs += L"<clipPath id=\"" + m_wsLastClipId + L"\">" +
		            L"<rect x=\"" + ConvertToWString(oClip.dLeft * pTransform->M11, 0) + L"\" y=\"" + ConvertToWString(oClip.dTop * pTransform->M22, 0) + L"\" width=\"" + ConvertToWString((oClip.dRight - oClip.dLeft) * pTransform->M11, 0) + L"\" height=\"" + ConvertToWString((oClip.dBottom - oClip.dTop) * pTransform->M22, 0) + L"\"/>" +
		            L"</clipPath>";
	}

	void CEmfInterpretatorSvg::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		m_wsLastClipId = L"EXCLUDECLIP_" + ConvertToWString(++m_unNumberDefs, 0);

		TXForm *pTransform = m_pParser->GetTransform();

		m_wsDefs += L"<clipPath id=\"" + m_wsLastClipId + L"\">" +
		            L"<path d=\"M" + ConvertToWString(oBB.dLeft * pTransform->M11) + L' ' + ConvertToWString(oBB.dTop * pTransform->M22) + L", L" + ConvertToWString(oBB.dRight * pTransform->M11) + L' ' + ConvertToWString(oBB.dTop * pTransform->M11) + L", " +
		            ConvertToWString(oBB.dRight * pTransform->M11) + L' ' + ConvertToWString(oBB.dBottom * pTransform->M22) + L", " + ConvertToWString(oBB.dLeft * pTransform->M11) + L' ' + ConvertToWString(oBB.dBottom * pTransform->M22) + L", M" +
		            ConvertToWString(oClip.dLeft * pTransform->M11) + L' ' + ConvertToWString(oClip.dTop * pTransform->M22) + L", L" + ConvertToWString(oClip.dRight * pTransform->M11) + L' ' + ConvertToWString(oClip.dTop * pTransform->M22) + L", " +
		            ConvertToWString(oClip.dRight * pTransform->M11) + L' ' + ConvertToWString(oClip.dBottom * pTransform->M22) + L", " + ConvertToWString(oClip.dLeft * pTransform->M11) + L' ' + ConvertToWString(oClip.dLeft * pTransform->M22) + L"\" clip-rule=\"evenodd\"/>" +
		            L"</clipPath>";
	}

	void CEmfInterpretatorSvg::PathClip(IPath *pPath, int nClipMode, TXForm *pTransform)
	{
		if (NULL == pPath || nClipMode != CombineModeIntersect)
			return;

		m_wsLastClipId = L"PATHCLIP_" + ConvertToWString(++m_unNumberDefs, 0);

		CEmfPath *pEmfPath = dynamic_cast<CEmfPath*>(pPath);

		if (NULL == pEmfPath)
			return;

		std::wstring wsPath = CreatePath(pEmfPath, pTransform);

		if (wsPath.empty())
			return;

		m_wsDefs += L"<clipPath id=\"" + m_wsLastClipId + L"\"><path d=\"" + wsPath + L"\" clip-rule=\"evenodd\"/></clipPath>";
	}

	void CEmfInterpretatorSvg::SetXmlWriter(XmlUtils::CXmlWriter *pXmlWriter)
	{
		if (NULL != pXmlWriter)
			m_oXmlWriter = *pXmlWriter;
	}

	XmlUtils::CXmlWriter *CEmfInterpretatorSvg::GetXmlWriter()
	{
		return &m_oXmlWriter;
	}

	std::wstring CEmfInterpretatorSvg::GetFile()
	{
		return m_oXmlWriter.GetXmlString();
	}

	void CEmfInterpretatorSvg::IncludeSvg(const std::wstring &wsSvg, const TRectD& oRect, const TRectD& oClipRect, TXForm *pTransform)
	{
		if (wsSvg.empty())
			return;

		std::wstring wsNewSvg = wsSvg;

		size_t unFirstPos = 83;
		size_t unSecondPos = wsSvg.find(L'>', unFirstPos);

		if (std::wstring::npos == unSecondPos)
			return;

		TRectD oNewClipRect(oClipRect);

		if (oNewClipRect.dLeft > oNewClipRect.dRight)
			std::swap(oNewClipRect.dLeft, oNewClipRect.dRight);

		if (oNewClipRect.dTop > oNewClipRect.dBottom)
			std::swap(oNewClipRect.dTop, oNewClipRect.dBottom);

		NodeAttributes arNodeAttributes;

		AddTransform(arNodeAttributes, pTransform);

		WriteNodeBegin(L"g", arNodeAttributes);

		wsNewSvg.erase(unFirstPos, unSecondPos - unFirstPos);

		std::wstring wsClip = L"x=\"" + ConvertToWString(oRect.dLeft) + L"\" y=\"" + ConvertToWString(oRect.dTop) + L"\" " +
		                      L"width=\"" + ConvertToWString(oRect.dRight - oRect.dLeft) + L"\" height=\"" + ConvertToWString(oRect.dBottom - oRect.dTop) + L"\" " +
		                      L"viewBox=\"" + ConvertToWString(oNewClipRect.dLeft) + L' ' + ConvertToWString(oNewClipRect.dTop) + L' ' + ConvertToWString(oNewClipRect.dRight - oNewClipRect.dLeft) + L' ' + ConvertToWString(oNewClipRect.dBottom - oNewClipRect.dTop) + L'\"';

		wsNewSvg.insert(unFirstPos, wsClip);

		m_oXmlWriter.WriteString(wsNewSvg);

		WriteNodeEnd(L"g");
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

	void CEmfInterpretatorSvg::WriteNodeBegin(const std::wstring &wsNodeName, const NodeAttributes &arAttributes)
	{
		m_oXmlWriter.WriteNodeBegin(wsNodeName, !arAttributes.empty());

		if (!arAttributes.empty())
		{
			for (const NodeAttribute& oAttribute : arAttributes)
				m_oXmlWriter.WriteAttribute(oAttribute.first, oAttribute.second);

			m_oXmlWriter.WriteNodeEnd(wsNodeName, true, false);
		}
	}

	void CEmfInterpretatorSvg::WriteNodeEnd(const std::wstring &wsNodeName)
	{
		m_oXmlWriter.WriteNodeEnd(wsNodeName, false, false);
	}

	void CEmfInterpretatorSvg::WriteText(const std::wstring &wsText, double dX, double dY, const TEmfRectL& oBounds, double dXScale, double dYScale)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetFont())
			return;

		NodeAttributes arNodeAttributes;

		double dXCoord = dX;
		double dYCoord = dY;

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		bool bWriteG = false;

		if (OPAQUE == m_pParser->GetTextBgMode())
		{
			std::wstring wsFillRect = L"rgb(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor()) + L')';

			NodeAttributes arGAttributes;
			AddClip(arGAttributes);

			WriteNodeBegin(L"g", arGAttributes);
			bWriteG = true;

			WriteNode(L"rect", {{L"x",      ConvertToWString(oBounds.lLeft)},
			                    {L"y",      ConvertToWString(oBounds.lTop)},
			                    {L"width",  ConvertToWString(oBounds.lRight - oBounds.lLeft)},
			                    {L"height", ConvertToWString(oBounds.lBottom - oBounds.lTop)},
			                    {L"fill",   wsFillRect},
			                    {L"stroke", L"none"}});
		}

		TEmfColor oColor = m_pParser->GetDC()->GetTextColor();

		if (0 != oColor.r || 0 != oColor.g || 0 != oColor.b)
			arNodeAttributes.push_back({L"fill", L"rgb(" + ConvertToWString(oColor.r, 0) + L", " + ConvertToWString(oColor.g, 0) + L", " + ConvertToWString(oColor.b, 0) + L')'});

		IFont *pFont = m_pParser->GetFont();

		double dFontHeight = std::fabs(pFont->GetHeight());

		if (dFontHeight < 0.01)
			dFontHeight = 18;

		arNodeAttributes.push_back({L"font-size", ConvertToWString(dFontHeight)});

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
		//                        double dFontCharSpace = pFont->GetCharSet();

		//                        if (dFontCharSpace > 1)
		//                                arNodeAttributes.push_back({L"letter-spacing", ConvertToWString(dFontCharSpace)});

		unsigned int ulTextAlign  = m_pParser->GetTextAlign() & TA_MASK;
		unsigned int ulVTextAlign = m_pParser->GetTextAlign() >> 8;

		if (ulTextAlign & TA_BASELINE)
		{
			ulTextAlign -= TA_BASELINE;
			// Ничего не делаем
		}
		else if (ulTextAlign & TA_BOTTOM || ulVTextAlign == VTA_BOTTOM)
		{
			arNodeAttributes.push_back({L"dominant-baseline", L"auto"});

			if (ulVTextAlign != VTA_BOTTOM)
				ulTextAlign -= TA_BOTTOM;
		}
		else if (ulVTextAlign == VTA_CENTER)
		{
			arNodeAttributes.push_back({L"dominant-baseline", L"middle"});
		}
		else // if (ulTextAlign & TA_TOP)
		{
			arNodeAttributes.push_back({L"dominant-baseline", L"hanging"});
		}

		if (ulTextAlign == TA_RIGHT)
		{
			arNodeAttributes.push_back({L"text-anchor", L"end"});
		}
		else if (ulTextAlign == TA_CENTER)
		{
			arNodeAttributes.push_back({L"text-anchor", L"middle"});
		}
		else //if (ulTextAlign & TA_LEFT)
		{
			// Ничего не делаем
		}

		if (dYScale < -0.00001) //TODO::Тоже нужно и для dXScale
		{
			dYCoord += dFontHeight;

			oTransform.Dy += (2 * dYCoord - dFontHeight) * oTransform.M22;

			oTransform.M22 = fabs(oTransform.M22);
		}

		if (0 != pFont->GetEscapement())
		{
			double dEscapement = pFont->GetEscapement() / -10;

			if (m_pParser->GetTransform()->M22 < 0)
				dEscapement = -dEscapement;

			arNodeAttributes.push_back({L"transform", L"rotate(" + ConvertToWString(dEscapement) + L' ' + ConvertToWString(dXCoord) + L' ' + ConvertToWString(dYCoord) + L')'});
		}

		AddTransform(arNodeAttributes, &oTransform);

		arNodeAttributes.push_back({L"xml:space", L"preserve"});

		if (!m_wsLastClipId.empty() && !bWriteG)
		{
			NodeAttributes arGAttributes;
			AddClip(arGAttributes);
			WriteNodeBegin(L"g", arGAttributes);
		}

		size_t unPosLineBreak = wsText.find(L"\n");

		if (std::wstring::npos == unPosLineBreak)
		{
			arNodeAttributes.push_back({L"x", ConvertToWString(dXCoord)});
			arNodeAttributes.push_back({L"y", ConvertToWString(dYCoord)});

			WriteNode(L"text", arNodeAttributes, StringNormalization(wsText));
		}
		else
		{
			size_t unStart = 0;
			double dYNewCoord = dYCoord;

			WriteNodeBegin(L"text", arNodeAttributes);

			do
			{
				std::wstring wsTemp = StringNormalization(wsText.substr(unStart, unPosLineBreak - unStart));

				WriteNode(L"tspan", {{L"x", ConvertToWString(dXCoord)},
				                     {L"y", ConvertToWString(dYNewCoord)}}, StringNormalization(wsText.substr(unStart, unPosLineBreak - unStart)));

				dYNewCoord += dFontHeight * 1.6;
				unStart = wsText.find_first_not_of(L"\n", unPosLineBreak);
				unPosLineBreak = wsText.find(L"\n", unStart);
			}
			while(unStart != std::wstring::npos);

			WriteNodeEnd(L"text");
		}

		if (bWriteG || !m_wsLastClipId.empty())
			m_oXmlWriter.WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::AddStroke(NodeAttributes &arAttributes)
	{
		if (NULL == m_pParser)
			return;

		IPen *pPen = m_pParser->GetPen();

		if (NULL == pPen || PS_NULL == pPen->GetStyle())
			return;

		if (pPen->GetAlpha() != 255)
			arAttributes.push_back({L"stroke-opacity" , ConvertToWString(pPen->GetAlpha() / 255., 3)});

		arAttributes.push_back({L"stroke", L"rgb(" + INTCOLOR_TO_RGB(m_pParser->GetPen()->GetColor()) + L')'});

		double dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());

		if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK)))
			dStrokeWidth = 1. / std::fabs(m_pParser->GetTransform()->M11);

		arAttributes.push_back({L"stroke-width", ConvertToWString(dStrokeWidth)});

		unsigned int unMetaPenStyle = m_pParser->GetPen()->GetStyle();
		//			unsigned int ulPenType      = unMetaPenStyle & PS_TYPE_MASK;
		unsigned int ulPenStyle     = unMetaPenStyle & PS_STYLE_MASK;

		double* arDatas = NULL;
		unsigned int unDataSize = 0;

		pPen->GetDashData(arDatas, unDataSize);

		if (NULL != arDatas && 0 != unDataSize)
		{
			std::wstring wsDashArray;

			for (unsigned int unIndex = 0; unIndex < unDataSize; ++unIndex)
				wsDashArray += ConvertToWString(dStrokeWidth * arDatas[unIndex]) + L' ';

			wsDashArray.pop_back();

			arAttributes.push_back({L"stroke-dasharray", wsDashArray});

			ulPenStyle = PS_USERSTYLE;
		}
		else if (PS_DASH == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2)});
		else if (PS_DOT == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth)});
		else if (PS_DASHDOT == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2)});
		else if (PS_DASHDOTDOT == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2)});
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

	void CEmfInterpretatorSvg::AddFill(NodeAttributes &arAttributes, double dWidth, double dHeight)
	{
		if (NULL == m_pParser)
		{
			arAttributes.push_back({L"fill", L"none"});
			return;
		}

		IBrush *pBrush = NULL;

		if (NULL != m_pParser->GetPen())
		{
			CEmfPlusPen *pPen = dynamic_cast<CEmfPlusPen*>(m_pParser->GetPen());

			if (NULL != pPen)
				pBrush = pPen->Brush;
		}

		if (NULL == pBrush)
			pBrush = m_pParser->GetBrush();

		if (NULL == pBrush || BS_NULL == pBrush->GetStyle())
		{
			arAttributes.push_back({L"fill", L"none"});
			return;
		}

		switch (pBrush->GetStyle())
		{
			case BS_SOLID:
			{
				arAttributes.push_back({L"fill", L"rgb(" + INTCOLOR_TO_RGB(pBrush->GetColor()) + L")"});

				if (pBrush->GetAlpha() != 255)
					arAttributes.push_back({L"fill-opacity" , ConvertToWString(pBrush->GetAlpha() / 255., 3)});

				return;
			}
			case BS_HATCHED:
			{
				const std::wstring wsStyleId = CreateHatchStyle(pBrush->GetHatch(), dWidth, dHeight);

				if (!wsStyleId.empty())
				{
					arAttributes.push_back({L"fill", L"url(#" + wsStyleId + L")"});
					return;
				}

				break;
			}
			case BS_DIBPATTERN:
			{
				const std::wstring wsStyleId = CreateDibPatternStyle(pBrush);

				if (!wsStyleId.empty())
				{
					arAttributes.push_back({L"fill", L"url(#" + wsStyleId + L")"});
					return;
				}
			}
			case BS_PATTERN:
			{
				const std::wstring wsStyleId = CreatePatternStyle(pBrush);

				if (!wsStyleId.empty())
				{
					arAttributes.push_back({L"fill", L"url(#" + wsStyleId + L")"});
					return;
				}
			}
			case BS_LINEARGRADIENT:
			case BS_RECTGRADIENT:
			case BS_PATHGRADIENT:
			case BS_RADIALGRADIENT:
			case BS_AXIALGRADIENT:
			{
				const std::wstring wsStyleId = CreateGradient(pBrush);

				if (!wsStyleId.empty())
				{
					arAttributes.push_back({L"fill", L"url(#" + wsStyleId + L")"});
					return;
				}
			}
			default:
				arAttributes.push_back({L"fill", L"none"});
		}
	}

	void CEmfInterpretatorSvg::AddTransform(NodeAttributes &arAttributes, TXForm* pTransform)
	{
		if (NULL == m_pParser)
			return;

		TXForm oOldTransform;

		if (NULL != pTransform)
			oOldTransform.Copy(pTransform);
		else
			oOldTransform.Copy(m_pParser->GetTransform());

		if (std::fabs(oOldTransform.M11) > MAXTRANSFORMSCALE || std::fabs(oOldTransform.M22) > MAXTRANSFORMSCALE)
		{
			oOldTransform.M11 /= std::fabs(oOldTransform.M11);
			oOldTransform.M22 /= std::fabs(oOldTransform.M22);
		}

		bool bScale = false, bTranslate = false;

		if (oOldTransform.M11 != 1 || oOldTransform.M22 != 1)
			bScale = true;

		if (oOldTransform.Dx != 0 || oOldTransform.Dy != 0)
			bTranslate = true;

		NodeAttribute *pFoundTransform = NULL;

		for (NodeAttribute& oNode : arAttributes)
		{
			if (L"transform" == oNode.first)
			{
				pFoundTransform = &oNode;
				break;
			}
		}

		std::wstring wsValue;

		if (bScale && !bTranslate)
		{
			wsValue = L"scale(" +	ConvertToWString(oOldTransform.M11) + L',' + ConvertToWString(oOldTransform.M22) + L')';
		}
		else if (bTranslate && !bScale)
		{
			wsValue = L"translate(" + ConvertToWString(oOldTransform.Dx) + L',' + ConvertToWString(oOldTransform.Dy) + L')';
		}
		else if (bScale && bTranslate)
		{
			wsValue = L"matrix(" +	ConvertToWString(oOldTransform.M11) + L',' +
			                        ConvertToWString(oOldTransform.M12) + L',' +
			                        ConvertToWString(oOldTransform.M21) + L',' +
			                        ConvertToWString(oOldTransform.M22) + L',' +
			                        ConvertToWString(oOldTransform.Dx) + L',' + ConvertToWString(oOldTransform.Dy) + L')';
		}
		else return;

		if (NULL != pFoundTransform)
			pFoundTransform->second.insert(0, wsValue + L' ');
		else
			arAttributes.push_back({L"transform", wsValue});
	}

	void CEmfInterpretatorSvg::AddClip(NodeAttributes &arAttributes)
	{
		if (NULL == m_pParser)
			return;

		if (m_wsLastClipId.empty())
			UpdateClip();

		if (!m_wsLastClipId.empty())
			arAttributes.push_back({L"clip-path", L"url(#" + m_wsLastClipId + L')'});
	}

	void CEmfInterpretatorSvg::AddNoneFill(NodeAttributes &arAttributes)
	{
		arAttributes.push_back({L"fill", L"none"});
	}

	TRectD CEmfInterpretatorSvg::TranslateRect(const TEmfRectL &oRect)
	{
		TRectD oNewRect;

		oNewRect.dLeft   = oRect.lLeft;
		oNewRect.dTop    = oRect.lTop;
		oNewRect.dRight  = oRect.lRight;
		oNewRect.dBottom = oRect.lBottom;

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

	TPointD CEmfInterpretatorSvg::GetCutPos() const
	{
		if (NULL != m_pParser)
			return m_pParser->GetCurPos();

		TPointD oCurPos;

		oCurPos.x = m_oViewport.dLeft;
		oCurPos.y = m_oViewport.dTop;

		return oCurPos;
	}

	std::wstring CEmfInterpretatorSvg::CreatePath(const CEmfPath* pPath, const TXForm* pTransform) const
	{
		if (NULL == m_pParser)
			return std::wstring();

		const CEmfPath* pNewPath = ((pPath != NULL) ? pPath : m_pParser->GetPath());

		if (NULL == pNewPath || pNewPath->m_pCommands.empty())
			return std::wstring();

		std::wstring wsValue;
		TEmfPointD *pFirstPoint = NULL;
		BYTE oLastType = 0x00;

		TXForm oTransform;

		if (NULL != pTransform)
			oTransform.Copy(pTransform);

		TPointD oPoint;

		for (const CEmfPathCommandBase* pCommand : pNewPath->m_pCommands)
		{
			switch ((unsigned int)pCommand->GetType())
			{
				case EMF_PATHCOMMAND_MOVETO:
				{
					CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;

					oPoint.x = pMoveTo->x;
					oPoint.y = pMoveTo->y;

					oTransform.Apply(oPoint.x, oPoint.y);

					wsValue += L"M " + ConvertToWString(oPoint.x) + L',' +  ConvertToWString(oPoint.y) + L' ';

					RELEASEOBJECT(pFirstPoint);
					pFirstPoint = new TEmfPointD();
					pFirstPoint->x = oPoint.x;
					pFirstPoint->y = oPoint.y;

					oLastType = EMF_PATHCOMMAND_MOVETO;

					break;
				}
				case EMF_PATHCOMMAND_LINETO:
				{
					CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;

					if (EMF_PATHCOMMAND_LINETO != oLastType)
					{
						oLastType = EMF_PATHCOMMAND_LINETO;
						wsValue += L"L ";
					}

					oPoint.x = pLineTo->x;
					oPoint.y = pLineTo->y;

					oTransform.Apply(oPoint.x, oPoint.y);

					wsValue += ConvertToWString(oPoint.x) + L',' +  ConvertToWString(oPoint.y) + L' ';

					if (NULL == pFirstPoint)
					{
						pFirstPoint = new TEmfPointD;
						pFirstPoint->x = oPoint.x;
						pFirstPoint->y = oPoint.y;
					}

					break;
				}
				case EMF_PATHCOMMAND_CURVETO:
				{
					CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;

					if (EMF_PATHCOMMAND_CURVETO != oLastType)
					{
						oLastType = EMF_PATHCOMMAND_CURVETO;
						wsValue += L"C ";
					}

					TPointD oPoint2(pCurveTo->x2, pCurveTo->y2), oPoint3(pCurveTo->xE, pCurveTo->yE);

					oPoint.x = pCurveTo->x1;
					oPoint.y = pCurveTo->y1;

					oTransform.Apply(oPoint.x,  oPoint.y);
					oTransform.Apply(oPoint2.x, oPoint2.y);
					oTransform.Apply(oPoint3.x, oPoint3.y);

					wsValue +=	ConvertToWString(oPoint.x)  + L',' + ConvertToWString(oPoint.y)  + L' ' +
					            ConvertToWString(oPoint2.x) + L',' + ConvertToWString(oPoint2.y) + L' ' +
					            ConvertToWString(oPoint3.x) + L',' + ConvertToWString(oPoint3.y) + L' ';

					if (NULL == pFirstPoint)
					{
						pFirstPoint = new TEmfPointD;
						pFirstPoint->x = oPoint.x;
						pFirstPoint->y = oPoint.y;
					}

					break;
				}
				case EMF_PATHCOMMAND_ARCTO:
				{
					CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;

					TPointD oPoint2(pArcTo->right, pArcTo->bottom);

					oPoint.x = pArcTo->left;
					oPoint.y = pArcTo->top;

					oTransform.Apply(oPoint.x,  oPoint.y);
					oTransform.Apply(oPoint2.x, oPoint2.y);

					double dXRadius = std::fabs((oPoint2.x - oPoint.x)) / 2;
					double dYRadius = std::fabs((oPoint2.y - oPoint.y)) / 2;

					double dEndX = (oPoint2.x + oPoint.x)  / 2 + dXRadius  * cos((pArcTo->sweep) * M_PI / 180);
					double dEndY = (oPoint2.y + oPoint2.y) / 2 + dYRadius  * sin((pArcTo->sweep) * M_PI / 180);

					wsValue += L"A " + ConvertToWString(dXRadius) + L' ' +
							ConvertToWString(dYRadius) + L' ' +
							L"0 " +
							((std::fabs(pArcTo->sweep - pArcTo->start) <= 180) ? L"0" : L"1") + L' ' +
							((std::fabs(pArcTo->sweep - pArcTo->start) <= 180) ? L"1" : L"0") + L' ' +
							ConvertToWString(dEndX) + L' ' +
							ConvertToWString(dEndY) + L' ';


					if (NULL == pFirstPoint)
					{
						pFirstPoint = new TEmfPointD;
						pFirstPoint->x = oPoint.x;
						pFirstPoint->y = oPoint.y;
					}

					oLastType = EMF_PATHCOMMAND_ARCTO;

					break;
				}
				case EMF_PATHCOMMAND_CLOSE:
				{
					if (NULL != pFirstPoint)
					{
						wsValue += L"L " + ConvertToWString(pFirstPoint->x) + L',' + ConvertToWString(pFirstPoint->y) + L' ';
						oLastType = EMF_PATHCOMMAND_CLOSE;
						RELEASEOBJECT(pFirstPoint);
					}
					break;
				}
			}
		}

		if (!wsValue.empty() && wsValue[0] != L'M')
			wsValue.insert(0, L"M " + ConvertToWString(m_pParser->GetCurPos().x) + L',' + ConvertToWString(m_pParser->GetCurPos().y) + L' ');

		RELEASEOBJECT(pFirstPoint);

		return wsValue;
	}

	std::wstring CEmfInterpretatorSvg::CreateHatchStyle(unsigned int unHatchStyle, double dWidth, double dHeight)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetBrush() || NULL == m_pParser->GetPen())
			return std::wstring();

		double dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());

		if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK)))
			dStrokeWidth = 1. / m_pParser->GetTransform()->M11;

		std::wstring wsStrokeWidth = ConvertToWString(dStrokeWidth);
		std::wstring wsValue  = ConvertToWString(dStrokeWidth * 8., 6);
		std::wstring wsValueW = ((0 != dWidth)  ? ConvertToWString((dStrokeWidth * 8.) / dWidth,  6) : L"1");
		std::wstring wsValueH = ((0 != dHeight) ? ConvertToWString((dStrokeWidth * 8.) / dHeight, 6) : L"1");

		std::wstring wsStrokeColor = L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetBrush()->GetColor()) + L"," + ConvertToWString(m_pParser->GetBrush()->GetAlpha(), 0) + L")";
		std::wstring wsBgColor;

		if (TRANSPARENT != m_pParser->GetTextBgMode())
			wsBgColor += L"rgb(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor())+ L")";

		switch(unHatchStyle)
		{
			case HS_HORIZONTAL:
			{
				std::wstring wsId = L"HORIZONTAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_VERTICAL:
			{
				std::wstring wsId = L"VERTICAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"100%\" height=\"100%\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y1=\"0\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";
				return wsId;
			}
			case HS_FDIAGONAL:
			{
				std::wstring wsId = L"FDIAGONAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_BDIAGONAL:
			{
				std::wstring wsId = L"BDIAGONAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_CROSS:
			{
				std::wstring wsId = L"CROSS_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_DIAGCROSS:
			{
				std::wstring wsId = L"DIAGCROSS" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}

			case HS_05Percent:
			{
				std::wstring wsId = L"05PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_10Percent:
			{
				std::wstring wsId = L"10PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_20Percent:
			{
				std::wstring wsId = L"20PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_25Percent:
			{
				std::wstring wsId = L"25PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_30Percent:
			{
				std::wstring wsId = L"30PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_40Percent:
			{
				std::wstring wsId = L"40PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_50Percent:
			{
				std::wstring wsId = L"50PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_60Percent:
			{
				std::wstring wsId = L"60PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"

				            L"</pattern> ";

				return wsId;
			}
			case HS_70Percent:
			{
				std::wstring wsId = L"70PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_75Percent:
			{
				std::wstring wsId = L"75PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_80Percent:
			{
				std::wstring wsId = L"80PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_90Percent:
			{
				std::wstring wsId = L"90PERCENT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * -0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * -0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.5)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * -0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.5)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * -0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"
				            L"</pattern> ";

				return wsId;
			}

			case HS_LTDOWNWARDDIAG:
			{
				std::wstring wsId = L"LTDOWNWARDDIAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_LTUPWARDDIAG:
			{
				std::wstring wsId = L"LTUPWARDDIAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DNDOWNWARDDIAG:
			{
				std::wstring wsId = L"DNDOWNWARDDIAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DNUPWARDDIAG:
			{
				std::wstring wsId = L"DNUPWARDDIAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_WDOWNWARDDIAG:
			{
				std::wstring wsId = L"WDOWNWARDDIAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_WUPWARDDIAG:
			{
				std::wstring wsId = L"BDIAGONAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}

			case HS_LTVERTICAL:
			{
				std::wstring wsId = L"LTVERTICAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";
				return wsId;
			}
			case HS_LTHORIZONTAL:
			{
				std::wstring wsId = L"LTHORIZONTAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_NVERTICAL:
			{
				std::wstring wsId = L"NVERTICAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";
				return wsId;
			}
			case HS_NHORIZONTAL:
			{
				std::wstring wsId = L"NHORIZONTAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_DNVERTICAL:
			{
				std::wstring wsId = L"DNVERTICAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				wsStrokeWidth = ConvertToWString(dStrokeWidth * 2);

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";
				return wsId;
			}
			case HS_DNHORIZONTAL:
			{
				std::wstring wsId = L"DNHORIZONTAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				wsStrokeWidth = ConvertToWString(dStrokeWidth * 2);

				m_wsDefs += L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"0\" y1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}

			case HS_DASHDOWNWARDDIAG:
			{
				std::wstring wsId = L"DASHDOWNWARDDIAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">>";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DASHUPWARDDIAG:
			{
				std::wstring wsId = L"DASHUPWARDDIAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">>";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DASHHORIZONTAL:
			{
				std::wstring wsId = L"DASHHORIZONTAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">>";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_DASHVERTICAL:
			{
				std::wstring wsId = L"DASHVERTICAL_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}

			case HS_SMALLCONFETTI:
			{
				std::wstring wsId = L"SMALLCONFETTI_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_LARGECONFETTI:
			{
				std::wstring wsId = L"LARGECONFETTI_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				wsStrokeWidth = ConvertToWString(dStrokeWidth * 2);

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_ZIGZAG:
			{
				std::wstring wsId = L"ZIGZAG_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_WAVE:
			{
				std::wstring wsId = L"WAVE_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DIAGBRICK:
			{
				std::wstring wsId = L"DIAGBRICK_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>"  +
				            L"</pattern> ";

				return wsId;
			}
			case HS_HORIZBRICK:
			{
				std::wstring wsId = L"HORIZBRICK_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_WEAVE:
			{
				std::wstring wsId = L"WEAVE_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0)  + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5)  + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_PLAID:
			{
				std::wstring wsId = L"PLAID_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +


				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DIVOT:
			{
				std::wstring wsId = L"DIVOT_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DOTGRID:
			{
				std::wstring wsId = L"DOTGRID_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_DOTDIAMOND:
			{
				std::wstring wsId = L"DOTDIAMOND_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_SHINGLE:
			{
				std::wstring wsId = L"SHINGLE_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_TRELLIS:
			{
				std::wstring wsId = L"TRELLIS_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_SPHERE:
			{
				std::wstring wsId = L"SPHERE_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_SGRID:
			{
				std::wstring wsId = L"SGRID_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8)   + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8)   + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8)   + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8)   + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_SCHECHERBOARD:
			{
				std::wstring wsId = L"SCHECHERBOARD_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				wsStrokeWidth = ConvertToWString(dStrokeWidth * 2);

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"</pattern> ";

				return wsId;
			}
			case HS_LCHECHERBOARD:
			{
				std::wstring wsId = L"LCHECHERBOARD_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				wsStrokeWidth = ConvertToWString(dStrokeWidth * 4);

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_OUTLINEDDIAMOND:
			{
				std::wstring wsId = L"OUTLINEDDIAMOND_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +

				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 8.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 7.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}
			case HS_SOLIDDIAMOND:
			{
				std::wstring wsId = L"SOLIDDIAMOND_" + ConvertToWString(++m_unNumberDefs, 0);

				m_wsDefs += L"<pattern id=\"" + wsId + L"\" " +
				            L"width=\"" + wsValueW + L"\" height=\"" + wsValueH + L"\" patternUnits=\"objectBoundingBox\" shape-rendering=\"crispEdges\">";

				if (!wsBgColor.empty())
					m_wsDefs += L"<rect x=\"0\" y=\"0\" width=\"" + wsValue + L"\" height=\"" + wsValue + L"\" fill=\"" + wsBgColor + L"\"/>";

				m_wsDefs += L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 0.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 1.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 2.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 0.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 7.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 3.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 1.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 6.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 4.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 2.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 5.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 5.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"<line x1=\"" + ConvertToWString(dStrokeWidth * 3.0) + L"\" y1=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" x2=\"" + ConvertToWString(dStrokeWidth * 4.0) + L"\" y2=\"" + ConvertToWString(dStrokeWidth * 6.5) + L"\" stroke=\"" + wsStrokeColor + L"\" stroke-width=\"" + wsStrokeWidth + L"\"/>" +
				            L"</pattern> ";

				return wsId;
			}

			default: return std::wstring();

		}

		return std::wstring();
	}

	std::wstring CEmfInterpretatorSvg::CreateDibPatternStyle(IBrush *pBrush)
	{
		if (NULL == m_pParser || NULL == pBrush || NULL == m_pParser->GetPen())
			return std::wstring();

		BYTE* pBuffer = NULL;
		unsigned int unWidth = 0, unHeight = 0;

		pBrush->GetDibPattern(&pBuffer, unWidth, unHeight);

		if (NULL == pBuffer || 0 == unWidth || 0 == unHeight)
			return std::wstring();

		CBgraFrame oFrame;
		oFrame.put_Data(pBuffer);
		oFrame.put_Width(unWidth);
		oFrame.put_Height(unHeight);
		oFrame.put_Stride(4 * unWidth);

		BYTE *pTempBuffer = NULL;
		int nTempSize;
		std::wstring wsStyleId;

		oFrame.Encode(pTempBuffer, nTempSize, 4);
		oFrame.put_Data(NULL);

		if (NULL == pTempBuffer || 0 == nTempSize)
			return std::wstring();

		char* pImageData = NULL;
		int nImageSize = 0;

		NSFile::CBase64Converter::Encode(pTempBuffer, nTempSize, pImageData, nImageSize, NSBase64::B64_BASE64_FLAG_NOCRLF);

		delete [] pTempBuffer;

		if (NULL == pImageData || 0 == nImageSize)
			return std::wstring();

		wsStyleId += L"DIBPATTERN_" + ConvertToWString(++m_unNumberDefs, 0);

		std::wstring wsImageDataW = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pImageData, (LONG)nImageSize);

		double dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());

		if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK)))
			dStrokeWidth = 1. / m_pParser->GetTransform()->M11;

		std::wstring wsWidth  = ConvertToWString(dStrokeWidth * 10 * unHeight / unWidth);
		std::wstring wsHeight = ConvertToWString(dStrokeWidth * 10 * unWidth  / unHeight);

		m_wsDefs += L"<pattern id=\"" + wsStyleId + L"\" " +
		            L"width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\" patternUnits=\"userSpaceOnUse\">" +
		            L"<image xlink:href=\"data:image/png;base64," + wsImageDataW + L"\" x=\"0\" y=\"0\" width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\"/>" +
		            L"</pattern> ";

		delete [] pImageData;

		return wsStyleId;
	}

	std::wstring CEmfInterpretatorSvg::CreatePatternStyle(IBrush *pBrush)
	{
		if (NULL == m_pParser || NULL == pBrush)
			return std::wstring();

		std::wstring wsPatterPath = pBrush->GetDibPatterPath();

		if (wsPatterPath.empty())
			return std::wstring();

		CBgraFrame oFrame;

		oFrame.OpenFile(wsPatterPath);

		BYTE *pTempBuffer = NULL;
		int nTempSize;
		std::wstring wsStyleId;

		oFrame.Encode(pTempBuffer, nTempSize, 4);

		if (NULL == pTempBuffer || 0 == nTempSize)
			return std::wstring();

		char* pImageData = NULL;
		int nImageSize = 0;

		NSFile::CBase64Converter::Encode(pTempBuffer, nTempSize, pImageData, nImageSize, NSBase64::B64_BASE64_FLAG_NOCRLF);

		delete [] pTempBuffer;

		if (NULL == pImageData || 0 == nImageSize)
			return std::wstring();

		wsStyleId += L"PATTERN_" + ConvertToWString(++m_unNumberDefs, 0);

		std::wstring wsImageDataW = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pImageData, (LONG)nImageSize);

		double dStrokeWidth = 1. / m_pParser->GetTransform()->M11;

		std::wstring wsWidth  = ConvertToWString(oFrame.get_Width()  / m_pParser->GetTransform()->M11);
		std::wstring wsHeight = ConvertToWString(oFrame.get_Height() / m_pParser->GetTransform()->M22);

		m_wsDefs += L"<pattern id=\"" + wsStyleId + L"\" " +
		            L"width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\" patternUnits=\"userSpaceOnUse\">" +
		            L"<image xlink:href=\"data:image/png;base64," + wsImageDataW + L"\" x=\"0\" y=\"0\" width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\"/>" +
		            L"</pattern> ";

		delete [] pImageData;

		return wsStyleId;
	}

	std::wstring CEmfInterpretatorSvg::CreateGradient(IBrush *pBrush)
	{
		if (pBrush == NULL)
			return std::wstring();

		std::wstring wsStyleId;

		if (BS_LINEARGRADIENT	== pBrush->GetStyle() ||
		    BS_RECTGRADIENT		== pBrush->GetStyle() ||
		    BS_PATHGRADIENT     == pBrush->GetStyle())
		{
			wsStyleId = L"LINEARGRADIENT_" + ConvertToWString(++m_unNumberDefs, 0);

			m_wsDefs += L"<linearGradient id=\"" + wsStyleId + L"\">" +
			            L"<stop offset=\"0%\" stop-color=\"rgb(" + INTCOLOR_TO_RGB(pBrush->GetColor()) + L")\"/>" +
			            L"<stop offset=\"100%\" stop-color=\"rgb(" + INTCOLOR_TO_RGB(pBrush->GetColor2()) + L")\"/>" +
			            L"</linearGradient>";

			return wsStyleId;
		}
		else if (BS_RADIALGRADIENT == pBrush->GetStyle() ||
		         BS_AXIALGRADIENT  == pBrush->GetStyle())
		{
			wsStyleId = L"RADIALGRADIENT_" + ConvertToWString(++m_unNumberDefs, 0);

			double dX = MININT8, dY = MININT8;

			pBrush->GetCenterPoint(dX, dY);

			std::wstring wsIndlude;

			if (dX != MININT8 || dY != MININT8)
			{
				double dLeft, dTop, dWidth, dHeight;

				pBrush->GetBounds(dLeft, dTop, dWidth, dHeight);

				dX = ((dX - dLeft) / dWidth);
				dY = ((dY - dTop)  / dHeight);

				wsIndlude = L" cx=\"" + ConvertToWString(dX) + L"\" cy=\"" + ConvertToWString(dY) + L"\" r=\"1\"";
			}

			m_wsDefs += L"<radialGradient id=\"" + wsStyleId + L"\"" + wsIndlude + L">" +
			            L"<stop offset=\"0%\" stop-color=\"rgb(" + INTCOLOR_TO_RGB(pBrush->GetColor()) + L")\"/>" +
			            L"<stop offset=\"100%\" stop-color=\"rgb(" + INTCOLOR_TO_RGB(pBrush->GetColor2()) + L")\"/>" +
			            L"</radialGradient>";

			return wsStyleId;
		}

		return std::wstring();
	}

	void CEmfInterpretatorSvg::UpdateClip()
	{
		IClip* pClip = m_pParser->GetClip();

		if (NULL != pClip)
			pClip->ClipOnRenderer(this);
	}
}

