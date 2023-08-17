#include "CInterpretatorSvgBase.h"
#include "../../../BgraFrame.h"

#include <algorithm>
#include <numeric>
#include <cmath>

#ifndef MININT32
#define MAXUINT32   ((UINT32)~((UINT32)0))
#define MAXINT32    ((INT32)(MAXUINT32 >> 1))
#define MININT32    ((INT32)~MAXINT32)
#endif

namespace MetaFile
{
    #define MAXTRANSFORMSCALE 100.
    #define MINTRANSFORMSCALE 0.00000001

	const std::map<int, std::wstring> mHatchStyles =
	{
	    { HS_HORIZONTAL,      L"HORIZONTAL"},
	    { HS_VERTICAL,         L"VERTICAL"},
	    { HS_FDIAGONAL,        L"FDIAGONAL"},
	    { HS_BDIAGONAL,        L"BDIAGONAL"},
	    { HS_CROSS,            L"CROSS"},
	    { HS_DIAGCROSS,        L"DIAGCROSS"},
	    { HS_05Percent,        L"05PERCENT"},
	    { HS_10Percent,        L"10PERCENT"},
	    { HS_20Percent,        L"20PERCENT"},
	    { HS_25Percent,        L"25PERCENT"},
	    { HS_30Percent,        L"30PERCENT"},
	    { HS_40Percent,        L"40PERCENT"},
	    { HS_50Percent,        L"50PERCENT"},
	    { HS_60Percent,        L"60PERCENT"},
	    { HS_70Percent,        L"70PERCENT"},
	    { HS_75Percent,        L"75PERCENT"},
	    { HS_80Percent,        L"80PERCENT"},
	    { HS_90Percent,        L"90PERCENT"},
	    { HS_LTDOWNWARDDIAG,   L"LTDOWNWARDDIAG"},
	    { HS_LTUPWARDDIAG,     L"LTUPWARDDIAG"},
	    { HS_DNDOWNWARDDIAG,   L"DNDOWNWARDDIAG"},
	    { HS_DNUPWARDDIAG,     L"DNUPWARDDIAG"},
	    { HS_WDOWNWARDDIAG,    L"WDOWNWARDDIAG"},
	    { HS_WUPWARDDIAG,      L"WUPWARDDIAG"},
	    { HS_LTVERTICAL,       L"LTVERTICAL"},
	    { HS_LTHORIZONTAL,     L"LTHORIZONTAL"},
	    { HS_NVERTICAL,        L"NVERTICAL"},
	    { HS_NHORIZONTAL,      L"NHORIZONTAL"},
	    { HS_DNVERTICAL,       L"DNVERTICAL"},
	    { HS_DNHORIZONTAL,     L"DNHORIZONTAL"},
	    { HS_DASHDOWNWARDDIAG, L"DASHDOWNWARDDIAG"},
	    { HS_DASHUPWARDDIAG,   L"DASHUPWARDDIAG"},
	    { HS_DASHHORIZONTAL,   L"DASHHORIZONTAL"},
	    { HS_DASHVERTICAL,     L"DASHVERTICAL"},
	    { HS_SMALLCONFETTI,    L"SMALLCONFETTI"},
	    { HS_LARGECONFETTI,    L"LARGECONFETTI"},
	    { HS_ZIGZAG,           L"ZIGZAG"},
	    { HS_WAVE,             L"WAVE"},
	    { HS_DIAGBRICK,        L"DIAGBRICK"},
	    { HS_HORIZBRICK,       L"HORIZBRICK"},
	    { HS_WEAVE,            L"WEAVE"},
	    { HS_PLAID,            L"PLAID"},
	    { HS_DIVOT,            L"DIVOT"},
	    { HS_DOTGRID,          L"DOTGRID"},
	    { HS_DOTDIAMOND,       L"DOTDIAMOND"},
	    { HS_SHINGLE,          L"SHINGLE"},
	    { HS_TRELLIS,          L"TRELLIS"},
	    { HS_SPHERE,           L"SPHERE"},
	    { HS_SGRID,            L"SGRID"},
	    { HS_SCHECHERBOARD,    L"SCHECHERBOARD"},
	    { HS_LCHECHERBOARD,    L"LCHECHERBOARD"},
	    { HS_OUTLINEDDIAMOND,  L"OUTLINEDDIAMOND"},
	    { HS_SOLIDDIAMOND,     L"SOLIDDIAMOND"}
	};

	CInterpretatorSvgBase::CInterpretatorSvgBase(IMetaFileBase *pParser, double dWidth, double dHeight)
	    : m_oSizeWindow(dWidth, dHeight), m_unNumberDefs(0), m_pParser(pParser), m_pXmlWriter(new XmlUtils::CXmlWriter()), m_bExternXmlWriter(false)
	{}

	CInterpretatorSvgBase::~CInterpretatorSvgBase()
	{
		if (!m_bExternXmlWriter)
			RELEASEOBJECT(m_pXmlWriter);
	}

	void CInterpretatorSvgBase::SetSize(double dWidth, double dHeight)
	{
		m_oSizeWindow.x = dWidth;
		m_oSizeWindow.y = dHeight;
	}

	void CInterpretatorSvgBase::GetSize(double &dWidth, double &dHeight)
	{
		dWidth  = m_oSizeWindow.x;
		dHeight = m_oSizeWindow.y;
	}

	void CInterpretatorSvgBase::UpdateSize()
	{
		if (0 != m_oSizeWindow.x && 0 == m_oSizeWindow.y)
			m_oSizeWindow.y = m_oSizeWindow.x * (m_oViewport.GetHeight() / m_oViewport.GetWidth());
		else if (0 == m_oSizeWindow.x && 0 != m_oSizeWindow.y)
			m_oSizeWindow.x = m_oSizeWindow.y * (m_oViewport.GetWidth() / m_oViewport.GetHeight());
	}

	void CInterpretatorSvgBase::SetXmlWriter(XmlUtils::CXmlWriter *pXmlWriter)
	{
		if (NULL != pXmlWriter)
		{
			if (!m_bExternXmlWriter)
				RELEASEOBJECT(m_pXmlWriter);

			m_pXmlWriter = pXmlWriter;
			m_bExternXmlWriter = true;
		}
	}

	XmlUtils::CXmlWriter *CInterpretatorSvgBase::GetXmlWriter()
	{
		return m_pXmlWriter;
	}

	std::wstring CInterpretatorSvgBase::GetFile()
	{
		return m_pXmlWriter->GetXmlString();
	}

	void CInterpretatorSvgBase::IncludeSvg(const std::wstring &wsSvg, const TRectD &oRect, const TRectD &oClipRect, TXForm *pTransform)
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

		m_pXmlWriter->WriteString(wsNewSvg);

		WriteNodeEnd(L"g");
	}

	void CInterpretatorSvgBase::WriteNode(const std::wstring &wsNodeName, const NodeAttributes &arAttributes, const std::wstring &wsValueNode)
	{
		m_pXmlWriter->WriteNodeBegin(wsNodeName, true);

		for (const NodeAttribute& oAttribute : arAttributes)
			m_pXmlWriter->WriteAttribute(oAttribute.first, oAttribute.second);

		if (wsValueNode.empty())
		{
			m_pXmlWriter->WriteNodeEnd(wsNodeName, true, true);
		}
		else
		{
			m_pXmlWriter->WriteNodeEnd(wsNodeName, true, false);
			m_pXmlWriter->WriteString (wsValueNode);
			m_pXmlWriter->WriteNodeEnd(wsNodeName, false, false);
		}
	}

	void CInterpretatorSvgBase::WriteNodeBegin(const std::wstring &wsNodeName, const NodeAttributes &arAttributes)
	{
		m_pXmlWriter->WriteNodeBegin(wsNodeName, !arAttributes.empty());

		if (!arAttributes.empty())
		{
			for (const NodeAttribute& oAttribute : arAttributes)
				m_pXmlWriter->WriteAttribute(oAttribute.first, oAttribute.second);

			m_pXmlWriter->WriteNodeEnd(wsNodeName, true, false);
		}
	}

	void CInterpretatorSvgBase::WriteNodeEnd(const std::wstring &wsNodeName)
	{
		m_pXmlWriter->WriteNodeEnd(wsNodeName, false, false);
	}

	static void EraseWords(std::wstring& wsString, const std::vector<std::wstring>& arWords)
	{
		size_t unBegin = 0;
		for (const std::wstring& wsWord : arWords)
		{
			while (std::wstring::npos != (unBegin = wsString.find(wsWord)))
				wsString.erase(unBegin, wsWord.length());
		}
	}

	void CInterpretatorSvgBase::WriteText(const std::wstring &wsText, const TPointD &oCoord, const TRect &oBounds, const TPointD &oScale, const std::vector<double>& arDx)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetFont())
			return;

		const std::wstring wsNormalizedText = StringNormalization(wsText);

		if (wsNormalizedText.empty())
			return;

		AddClip();

		NodeAttributes arNodeAttributes;

		double dXCoord = oCoord.x;
		double dYCoord = oCoord.y;

		if (m_pParser->GetTextAlign() & TA_UPDATECP && (0. == oCoord.x && 0. == oCoord.y))
		{
			dXCoord = m_pParser->GetCurPos().x;
			dYCoord = m_pParser->GetCurPos().y;
		}

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		bool bWriteG = false;

		if (OPAQUE == m_pParser->GetTextBgMode())
		{
			std::wstring wsFillRect = L"rgb(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor()) + L')';

			WriteNodeBegin(L"g", {});
			bWriteG = true;

			WriteNode(L"rect", {{L"x",      ConvertToWString(oBounds.nLeft)},
			                    {L"y",      ConvertToWString(oBounds.nTop)},
			                    {L"width",  ConvertToWString(oBounds.nRight - oBounds.nLeft)},
			                    {L"height", ConvertToWString(oBounds.nBottom - oBounds.nTop)},
			                    {L"fill",   wsFillRect},
			                    {L"stroke", L"none"}});
		}

		int nColor = m_pParser->GetTextColor();

		if (0 != nColor)
			arNodeAttributes.push_back({L"fill", L"rgb(" + INTCOLOR_TO_RGB(nColor) + L')'});

		IFont *pFont = m_pParser->GetFont();

		if (NULL == pFont)
			return;

		double dFontHeight = std::fabs(pFont->GetHeight());

		if (dFontHeight < 0.01)
			dFontHeight = 18;

		arNodeAttributes.push_back({L"font-size", ConvertToWString(dFontHeight)});

		std::wstring wsFontName = pFont->GetFaceName();

		if (!wsFontName.empty())
		{
			NSFonts::CFontSelectFormat oFormat;
			oFormat.wsName = new std::wstring(pFont->GetFaceName());

			NSFonts::CFontInfo *pFontInfo = m_pParser->GetFontManager()->GetFontInfoByParams(oFormat);

			if (NULL != pFontInfo && !StringEquals(wsFontName, pFontInfo->m_wsFontName))
				wsFontName = L"&apos;" + wsFontName + L"&apos;, &apos;" + pFontInfo->m_wsFontName + L"&apos;";
		}

		if (!wsFontName.empty())
			arNodeAttributes.push_back({L"font-family", wsFontName});

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
			dYCoord += dFontHeight;
		}

		if (ulTextAlign == TA_RIGHT)
		{
			if (arDx.empty())
				arNodeAttributes.push_back({L"text-anchor", L"end"});
			else
				dXCoord -= std::accumulate(arDx.begin(), arDx.end(), 0.0);
		}
		else if (ulTextAlign == TA_CENTER)
		{
			if (arDx.empty())
				arNodeAttributes.push_back({L"text-anchor", L"middle"});
			else
				dXCoord -= std::accumulate(arDx.begin(), arDx.end(), 0.0) / 2;
		}
		else  //if (ulTextAlign & TA_LEFT)
		{
			// Ничего не делаем
		}

		if (oScale.y < -0.00001) //TODO::Тоже нужно и для dXScale
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

			double dSin = std::sin(dEscapement * M_PI / 180.);

			dYCoord -= dFontHeight * dSin;

			if (oScale.y < -0.00001)
				dXCoord -= dFontHeight * dSin;

			arNodeAttributes.push_back({L"transform", L"rotate(" + ConvertToWString(dEscapement) + L' ' + ConvertToWString(dXCoord) + L' ' + ConvertToWString(dYCoord) + L')'});

			if (oScale.y > 0.00001)
				dXCoord -= dFontHeight * dSin;
		}

		AddTransform(arNodeAttributes, &oTransform);

		arNodeAttributes.push_back({L"xml:space", L"preserve"});

		size_t unPosLineBreak = wsNormalizedText.find(L"\n");

		std::wstring wsXCoord;

		if (arDx.empty() || arDx.size() < wsNormalizedText.length())
			wsXCoord = ConvertToWString(dXCoord);
		else
		{
			std::vector<double> arXCoords(wsNormalizedText.length());

			arXCoords[0] = dXCoord;

			for (unsigned int unIndex = 1; unIndex < wsNormalizedText.length(); ++unIndex)
				arXCoords[unIndex] = arDx[unIndex - 1] + arXCoords[unIndex - 1];

			wsXCoord = ConvertToWString(arXCoords);
		}

		if (std::wstring::npos == unPosLineBreak)
		{
			arNodeAttributes.push_back({L"x", wsXCoord});
			arNodeAttributes.push_back({L"y", ConvertToWString(dYCoord)});

			WriteNode(L"text", arNodeAttributes, wsNormalizedText);
		}
		else
		{
			size_t unStart = 0;
			double dYNewCoord = dYCoord;

			WriteNodeBegin(L"text", arNodeAttributes);

			do
			{
				WriteNode(L"tspan", {{L"x", wsXCoord},
									 {L"y", ConvertToWString(dYNewCoord)}}, wsNormalizedText.substr(unStart, unPosLineBreak - unStart));

				dYNewCoord += dFontHeight * 1.6;
				unStart = wsNormalizedText.find_first_not_of(L"\n", unPosLineBreak);
				unPosLineBreak = wsNormalizedText.find(L"\n", unStart);
			}
			while(unStart != std::wstring::npos);

			WriteNodeEnd(L"text");
		}

		if (bWriteG)
			m_pXmlWriter->WriteNodeEnd(L"g");
	}

	void CInterpretatorSvgBase::ResetClip()
	{
		if (m_oClip.StartClip() && !m_oClip.EndClip())
		{
			WriteNodeEnd(L"g");
			m_oClip.CloseClip();
		}

		m_oClip.Reset();
	}

	void CInterpretatorSvgBase::IntersectClip(const TRectD &oClip)
	{
		TXForm *pTransform = m_pParser->GetTransform();

		ResetClip();

		const std::wstring wsId    = L"INTERSECTCLIP_" + ConvertToWString(++m_unNumberDefs, 0);
		const std::wstring wsValue = L"<rect x=\"" + ConvertToWString(oClip.dLeft * pTransform->M11, 0) + L"\" y=\"" + ConvertToWString(oClip.dTop * pTransform->M22, 0) + L"\" width=\"" + ConvertToWString((oClip.dRight - oClip.dLeft) * pTransform->M11, 0) + L"\" height=\"" + ConvertToWString((oClip.dBottom - oClip.dTop) * pTransform->M22, 0) + L"\"/>";

		m_oClip.AddClipValue(wsId, wsValue);
	}

	void CInterpretatorSvgBase::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		TXForm *pTransform = m_pParser->GetTransform();

		const std::wstring wsId    = L"EXCLUDECLIP_" + ConvertToWString(++m_unNumberDefs, 0);
		const std::wstring wsValue = L"<path d=\"M" + ConvertToWString(oBB.dLeft * pTransform->M11) + L' ' + ConvertToWString(oBB.dTop * pTransform->M22) + L", L" + ConvertToWString(oBB.dRight * pTransform->M11) + L' ' + ConvertToWString(oBB.dTop * pTransform->M11) + L", " +
		                             ConvertToWString(oBB.dRight * pTransform->M11) + L' ' + ConvertToWString(oBB.dBottom * pTransform->M22) + L", " + ConvertToWString(oBB.dLeft * pTransform->M11) + L' ' + ConvertToWString(oBB.dBottom * pTransform->M22) + L", M" +
		                             ConvertToWString(oClip.dLeft * pTransform->M11) + L' ' + ConvertToWString(oClip.dTop * pTransform->M22) + L", L" + ConvertToWString(oClip.dRight * pTransform->M11) + L' ' + ConvertToWString(oClip.dTop * pTransform->M22) + L", " +
		                             ConvertToWString(oClip.dRight * pTransform->M11) + L' ' + ConvertToWString(oClip.dBottom * pTransform->M22) + L", " + ConvertToWString(oClip.dLeft * pTransform->M11) + L' ' + ConvertToWString(oClip.dLeft * pTransform->M22) + L"\" clip-rule=\"evenodd\"/>";

		m_oClip.AddClipValue(wsId, wsValue);
	}

	void CInterpretatorSvgBase::AddStroke(NodeAttributes &arAttributes) const
	{
		if (NULL == m_pParser)
			return;

		IPen *pPen = m_pParser->GetPen();

		if (NULL == pPen || PS_NULL == pPen->GetStyle())
			return;

		switch (m_pParser->GetRop2Mode())
		{
			case R2_BLACK:   arAttributes.push_back({L"stroke", L"rgb(0, 0, 0)"}); break;
			case R2_NOP:     return;
			case R2_WHITE:   arAttributes.push_back({L"stroke", L"rgb(255, 255, 255)"}); break;
			default: arAttributes.push_back({L"stroke", L"rgb(" + INTCOLOR_TO_RGB(pPen->GetColor()) + L')'}); break;
		}

		double dStrokeWidth = std::fabs(pPen->GetWidth());

		if (0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (pPen->GetStyle() & PS_TYPE_MASK)))
		{
			double dScale = m_pParser->GetDpi() / 96.;

			if (0 != m_oViewport.GetWidth() && 0 != m_oSizeWindow.x)
				dScale *= m_oViewport.GetWidth() / m_oSizeWindow.x;

			dStrokeWidth = dScale / std::fabs(m_pParser->GetTransform()->M11);
		}

		arAttributes.push_back({L"stroke-width", ConvertToWString(dStrokeWidth)});

		if (pPen->GetAlpha() != 255)
			arAttributes.push_back({L"stroke-opacity" , ConvertToWString(pPen->GetAlpha() / 255., 3)});

		arAttributes.push_back({L"stroke-miterlimit", ConvertToWString(pPen->GetMiterLimit())});

		unsigned int unMetaPenStyle = pPen->GetStyle();
		unsigned int ulPenStyle     = unMetaPenStyle & PS_STYLE_MASK;
		unsigned int ulPenStartCap  = unMetaPenStyle & PS_STARTCAP_MASK;
		unsigned int ulPenEndCap    = unMetaPenStyle & PS_ENDCAP_MASK;
		unsigned int ulPenJoin      = unMetaPenStyle & PS_JOIN_MASK;

		// svg не поддерживает разные стили для разных сторон линии
		std::wstring wsLineCap;

		if (PS_ENDCAP_ROUND == ulPenEndCap)
			wsLineCap = L"round";
		else if (PS_ENDCAP_SQUARE == ulPenEndCap)
			wsLineCap = L"square";
		else if (PS_ENDCAP_FLAT == ulPenEndCap)
			wsLineCap = L"butt";

		if (PS_STARTCAP_FLAT == ulPenStartCap)
			wsLineCap = L"butt";
		else if (PS_STARTCAP_SQUARE == ulPenStartCap)
			wsLineCap = L"square";
		else if (PS_STARTCAP_ROUND == ulPenStartCap)
			wsLineCap = L"round";

		arAttributes.push_back({L"stroke-linecap", wsLineCap});

		if (PS_JOIN_MITER == ulPenJoin)
			arAttributes.push_back({L"stroke-linejoin", L"miter"});
		else if (PS_JOIN_BEVEL == ulPenJoin)
			arAttributes.push_back({L"stroke-linejoin", L"bevel"});
		else if (PS_JOIN_ROUND == ulPenJoin)
			arAttributes.push_back({L"stroke-linejoin", L"round"});

		double* arDatas = NULL;
		unsigned int unDataSize = 0;

		pPen->GetDashData(arDatas, unDataSize);

		if (NULL != arDatas && 0 != unDataSize)
		{
			std::wstring wsDashArray;

			for (unsigned int unIndex = 0; unIndex < unDataSize; ++unIndex)
			{
				if (PS_STARTCAP_ROUND == ulPenStartCap)
					wsDashArray += ConvertToWString(dStrokeWidth * (arDatas[unIndex] - ((0 == unIndex % 2) ? 1 : -1))) + L' ';
				else
					wsDashArray += ConvertToWString(dStrokeWidth * arDatas[unIndex]) + L' ';
			}
			wsDashArray.pop_back();

			arAttributes.push_back({L"stroke-dasharray", wsDashArray});
		}
		else if (PS_DASH == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2)});
		else if (PS_DOT == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth)});
		else if (PS_DASHDOT == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2)});
		else if (PS_DASHDOTDOT == ulPenStyle)
			arAttributes.push_back({L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2)});
	}

	void CInterpretatorSvgBase::AddFill(NodeAttributes &arAttributes, double dWidth, double dHeight)
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

		unsigned int unBrushStyle = pBrush->GetStyle();

		if (NULL == pBrush || BS_NULL == unBrushStyle)
		{
			arAttributes.push_back({L"fill", L"none"});
			return;
		}

		switch (unBrushStyle)
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

	void CInterpretatorSvgBase::AddTransform(NodeAttributes &arAttributes, TXForm *pTransform) const
	{
		if (NULL == m_pParser)
			return;

		TXForm oOldTransform;

		if (NULL != pTransform)
			oOldTransform.Copy(pTransform);
		else
			oOldTransform.Copy(m_pParser->GetTransform());

//		if (std::fabs(oOldTransform.M11) > MAXTRANSFORMSCALE || std::fabs(oOldTransform.M22) > MAXTRANSFORMSCALE)
//		{
//			oOldTransform.M11 /= std::fabs(oOldTransform.M11);
//			oOldTransform.M22 /= std::fabs(oOldTransform.M22);
//		}

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

	void CInterpretatorSvgBase::AddClip()
	{
		if (NULL == m_pParser)
			return;

		if (!m_oClip.StartClip())
		{
			UpdateClip();

			if (!m_oClip.Empty())
			{
				WriteNodeBegin(L"g", {{L"clip-path", L"url(#" + m_oClip.GetClipId() + L')'}});
				m_oClip.BeginClip();
			}
		}
	}

	void CInterpretatorSvgBase::UpdateClip()
	{
		IClip* pClip = m_pParser->GetClip();

		if (NULL != pClip)
		{
			pClip->ClipOnRenderer((CInterpretatorSvgBase*)this);
			m_wsDefs += m_oClip.GetClip();
		}
		else
			ResetClip();
	}

	void CInterpretatorSvgBase::AddNoneFill(NodeAttributes &arAttributes) const
	{
		arAttributes.push_back({L"fill", L"none"});
	}

	TPointD CInterpretatorSvgBase::GetCutPos() const
	{
		if (NULL != m_pParser)
			return m_pParser->GetCurPos();

		TPointD oCurPos;

		oCurPos.x = m_oViewport.dLeft;
		oCurPos.y = m_oViewport.dRight;

		return oCurPos;
	}

	std::wstring CInterpretatorSvgBase::CreatePath(const IPath *pPath, const TXForm *pTransform)
	{
		return std::wstring();
	}

	std::wstring CInterpretatorSvgBase::CreateHatchStyle(unsigned int unHatchStyle, double dWidth, double dHeight)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetBrush())
			return std::wstring();

		double dStrokeWidth = 1. / m_pParser->GetTransform()->M11;

		if (NULL != m_pParser->GetPen())
		{
			dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());

			if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK)))
				dStrokeWidth = 1. / m_pParser->GetTransform()->M11;
		}

		if (0 != m_oViewport.GetWidth() && 0 != m_oSizeWindow.x)
			dStrokeWidth *= m_oViewport.GetWidth() / m_oSizeWindow.x;

		std::wstring wsStrokeColor = L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetBrush()->GetColor()) + L"," + ConvertToWString(m_pParser->GetBrush()->GetAlpha(), 0) + L")";
		std::wstring wsBgColor;

		if (TRANSPARENT != m_pParser->GetTextBgMode())
			wsBgColor += L"rgb(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor())+ L")";

		CHatchGenerator oHatchGenerator;

		oHatchGenerator.SetSize(dWidth, dHeight);
		oHatchGenerator.SetStyleId(unHatchStyle, ++m_unNumberDefs);
		oHatchGenerator.SetStroke(dStrokeWidth, m_pParser->GetBrush()->GetColor(), m_pParser->GetBrush()->GetAlpha());

		if (TRANSPARENT != m_pParser->GetTextBgMode())
			oHatchGenerator.SetBKColor(m_pParser->GetTextBgColor());

		if (oHatchGenerator.GenerateHatch())
		{
			m_wsDefs += oHatchGenerator.GetPattern();

			return oHatchGenerator.GetPatternId();
		}

		--m_unNumberDefs;
		return std::wstring();
	}

	std::wstring CInterpretatorSvgBase::CreateDibPatternStyle(IBrush *pBrush)
	{
		if (NULL == m_pParser || NULL == pBrush)
			return std::wstring();

		CBgraFrame oFrame;
		unsigned int unWidth = 0, unHeight = 0;

		if (pBrush->GetDibPatterPath().empty())
		{
			BYTE* pBuffer = NULL;

			pBrush->GetDibPattern(&pBuffer, unWidth, unHeight);

			if (NULL == pBuffer || 0 == unWidth || 0 == unHeight)
				return std::wstring();

			oFrame.put_Data(pBuffer);
			oFrame.put_Width(unWidth);
			oFrame.put_Height(unHeight);
			oFrame.put_Stride(4 * unWidth);
		}
		else
		{
			oFrame.OpenFile(pBrush->GetDibPatterPath());
			unWidth  = oFrame.get_Width();
			unHeight = oFrame.get_Height();
		}

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

		double dStrokeWidth = 1. / m_pParser->GetTransform()->M11;

		if (NULL != m_pParser->GetPen())
		{
			dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());

			if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK)))
				dStrokeWidth = 1. / m_pParser->GetTransform()->M11;
		}

		if (0 != m_oViewport.GetWidth() && 0 != m_oSizeWindow.x)
			dStrokeWidth *= m_oViewport.GetWidth() / m_oSizeWindow.x;

		std::wstring wsWidth  = ConvertToWString(dStrokeWidth * unWidth);
		std::wstring wsHeight = ConvertToWString(dStrokeWidth * unHeight);

		m_wsDefs += L"<pattern id=\"" + wsStyleId + L"\" " +
		            L"width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\" patternUnits=\"userSpaceOnUse\">" +
		            L"<image xlink:href=\"data:image/png;base64," + wsImageDataW + L"\" x=\"0\" y=\"0\" width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\"/>" +
		            L"</pattern> ";

		delete [] pImageData;

		return wsStyleId;
	}

	std::wstring CInterpretatorSvgBase::CreatePatternStyle(IBrush *pBrush)
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

		std::wstring wsWidth  = ConvertToWString(oFrame.get_Width());
		std::wstring wsHeight = ConvertToWString(oFrame.get_Height());

		m_wsDefs += L"<pattern id=\"" + wsStyleId + L"\" " +
		            L"width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\">" +
		            L"<image xlink:href=\"data:image/png;base64," + wsImageDataW + L"\" x=\"0\" y=\"0\" width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\"/>" +
		            L"</pattern> ";

		delete [] pImageData;

		return wsStyleId;
	}

	std::wstring CInterpretatorSvgBase::CreateGradient(IBrush *pBrush)
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

			double dX = MININT32, dY = MININT32;

			pBrush->GetCenterPoint(dX, dY);

			std::wstring wsIndlude;

			if (dX != MININT32 || dY != MININT32)
			{
				double dLeft, dTop, dWidth, dHeight;

				pBrush->GetBounds(dLeft, dTop, dWidth, dHeight);

				if (0 != dWidth && 0 != dHeight)
				{
					dX = ((dX - dLeft) / dWidth);
					dY = ((dY - dTop)  / dHeight);

					wsIndlude = L" cx=\"" + ConvertToWString(dX) + L"\" cy=\"" + ConvertToWString(dY) + L"\" r=\"1\"";
				}
			}

			m_wsDefs += L"<radialGradient id=\"" + wsStyleId + L"\"" + wsIndlude + L">" +
			            L"<stop offset=\"0%\" stop-color=\"rgb(" + INTCOLOR_TO_RGB(pBrush->GetColor()) + L")\"/>" +
			            L"<stop offset=\"100%\" stop-color=\"rgb(" + INTCOLOR_TO_RGB(pBrush->GetColor2()) + L")\"/>" +
			            L"</radialGradient>";

			return wsStyleId;
		}

		return std::wstring();
	}

	CHatchGenerator::CHatchGenerator()
	    : m_nHatchStyle(-1), m_unNumber(0), m_nBKColor(-1)
	{
	}

	void CHatchGenerator::SetSize(double dWidth, double dHeight)
	{
		m_dWidth  = dWidth;
		m_dHeight = dHeight;
	}

	void CHatchGenerator::SetStyleId(unsigned int unHatchStyle, unsigned int unNumber)
	{
		m_nHatchStyle = unHatchStyle;
		m_unNumber    = unNumber;
	}

	void CHatchGenerator::SetStroke(double dWidth, int nColor, unsigned char chAlpha)
	{
		m_dStrokeWidth = dWidth;
		m_nStrokeColor = nColor;
		m_chAlpha      = chAlpha;
	}

	void CHatchGenerator::SetBKColor(int nColor)
	{
		m_nBKColor  = nColor;
	}

	bool CHatchGenerator::GenerateHatch()
	{
		if (-1 == m_nHatchStyle)
			return false;

		if (!GenerateStartPattern())
			return false;

		GenerateBK();

		switch(m_nHatchStyle)
		{
			case HS_HORIZONTAL:
			{
				AddLine({0.0, 0.5}, {8.0, 0.5});

				break;
			}
			case HS_VERTICAL:
			{
				AddLine({0.5, 0.0}, {0.5, 8.0});

				break;
			}
			case HS_FDIAGONAL:
			{
				AddPoints({{0.0, 0.5}, {1.0, 1.5}, {2.0, 2.5}, {3.0, 3.5}, {4.0, 4.5}, {5.0, 5.5}, {6.0, 6.5}, {7.0, 7.5}});

				break;
			}
			case HS_BDIAGONAL:
			{
				AddPoints({{7.0, 0.5}, {6.0, 1.5}, {5.0, 2.5}, {4.0, 3.5}, {3.0, 4.5}, {2.0, 5.5}, {1.0, 6.5}, {0.0, 7.5}});

				break;
			}
			case HS_CROSS:
			{
				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{0.5, 0.0}, {0.5, 8.0}}});

				break;
			}
			case HS_DIAGCROSS:
			{
				AddPoints({{0.0, 0.5}, {7.0, 0.5}, {1.0, 1.5}, {6.0, 1.5}, {2.0, 2.5}, {5.0, 2.5}, {5.0, 5.5}, {2.0, 5.5}, {6.0, 6.5}, {1.0, 6.5}, {7.0, 7.5}, {0.0, 7.5}});

				AddLines({{{3.0, 3.5}, {5.0, 3.5}}, {{3.0, 4.5}, {5.0, 4.5}}});

				break;
			}

			case HS_05Percent:
			{
				AddPoints({{0.0, 0.5}, {4.0, 4.5}});

				break;
			}
			case HS_10Percent:
			{
				AddPoints({{0.0, 0.5}, {4.0, 2.5}, {0.0, 4.5}, {4.0, 6.5}});

				break;
			}
			case HS_20Percent:
			{
				AddPoints({{0.0, 0.5}, {4.0, 0.5}, {2.0, 2.5}, {6.0, 2.5}, {0.0, 4.5}, {4.0, 4.5}, {2.0, 6.5}, {6.0, 6.5}});

				break;
			}
			case HS_25Percent:
			{
				AddPoints({{0.0, 0.5}, {4.0, 0.5}, {2.0, 1.5}, {6.0, 1.5}, {0.0, 2.5}, {4.0, 2.5}, {2.0, 3.5}, {6.0, 3.5},
				           {0.0, 4.5}, {4.0, 4.5}, {2.0, 5.5}, {6.0, 5.5}, {0.0, 6.5}, {4.0, 6.5}, {2.0, 7.5}, {6.0, 7.5}});

				break;
			}
			case HS_30Percent:
			{
				AddPoints({{0.0, 0.5}, {2.0, 0.5}, {4.0, 0.5}, {6.0, 0.5}, {1.0, 1.5}, {5.0, 1.5},
				           {0.0, 2.5}, {2.0, 2.5}, {4.0, 2.5}, {6.0, 2.5}, {3.0, 3.5}, {7.0, 3.5},
				           {0.0, 4.5}, {2.0, 4.5}, {4.0, 4.5}, {6.0, 4.5}, {1.0, 5.5}, {5.0, 5.5},
				           {0.0, 6.5}, {2.0, 6.5}, {4.0, 6.5}, {6.0, 6.5}, {3.0, 7.5}, {7.0, 7.5}});

				break;
			}
			case HS_40Percent:
			{
				AddPoints({{0.0, 0.5}, {2.0, 0.5}, {4.0, 0.5}, {6.0, 0.5}, {1.0, 1.5}, {3.0, 1.5}, {5.0, 1.5}, {7.0, 1.5},
				           {0.0, 2.5}, {2.0, 2.5}, {4.0, 2.5}, {6.0, 2.5}, {1.0, 3.5}, {3.0, 3.5},             {7.0, 3.5},
				           {0.0, 4.5}, {2.0, 4.5}, {4.0, 4.5}, {6.0, 4.5}, {1.0, 5.5}, {3.0, 5.5}, {5.0, 5.5}, {7.0, 5.5},
				           {0.0, 6.5}, {2.0, 6.5}, {4.0, 6.5}, {6.0, 6.5},             {3.0, 7.5}, {5.0, 7.5}, {7.0, 7.5}});

				break;
			}
			case HS_50Percent:
			{
				AddPoints({{0.0, 0.5}, {2.0, 0.5}, {4.0, 0.5}, {6.0, 0.5}, {1.0, 1.5}, {3.0, 1.5}, {5.0, 1.5}, {7.0, 1.5},
				           {0.0, 2.5}, {2.0, 2.5}, {4.0, 2.5}, {6.0, 2.5}, {1.0, 3.5}, {3.0, 3.5}, {5.0, 3.5}, {7.0, 3.5},
				           {0.0, 4.5}, {2.0, 4.5}, {4.0, 4.5}, {6.0, 4.5}, {1.0, 5.5}, {3.0, 5.5}, {5.0, 5.5}, {7.0, 5.5},
				           {0.0, 6.5}, {2.0, 6.5}, {4.0, 6.5}, {6.0, 6.5}, {1.0, 7.5}, {3.0, 7.5}, {5.0, 7.5}, {7.0, 7.5}});

				break;
			}
			case HS_60Percent:
			{
				AddLines({{{0.0, 0.5}, {3.0, 0.5}}, {{4.0, 0.5}, {7.0, 0.5}}, {{2.0, 2.5}, {5.0, 2.5}},
				          {{6.0, 2.5}, {8.0, 2.5}}, {{0.0, 4.5}, {3.0, 4.5}}, {{4.0, 4.5}, {7.0, 4.5}},
				          {{2.0, 6.5}, {5.0, 6.5}}, {{6.0, 6.5}, {8.0, 6.5}}});

				AddPoints({{1.0, 1.5}, {3.0, 1.5}, {5.0, 1.5}, {7.0, 1.5}, {0.0, 2.5},
				           {1.0, 3.5}, {3.0, 3.5}, {5.0, 3.5}, {7.0, 3.5},
				           {1.0, 5.5}, {3.0, 5.5}, {5.0, 5.5}, {7.0, 5.5}, {0.0, 6.5},
				           {1.0, 7.5}, {3.0, 7.5}, {5.0, 7.5}, {7.0, 7.5}});

				break;
			}
			case HS_70Percent:
			{
				AddLines({{{1.0, 0.5}, {4.0, 0.5}}, {{5.0, 0.5}, {8.0, 0.5}}, {{0.0, 1.5}, {2.0, 1.5}}, {{3.0, 1.5}, {6.0, 1.5}},
				          {{1.0, 2.5}, {4.0, 2.5}}, {{5.0, 2.5}, {8.0, 2.5}}, {{0.0, 3.5}, {2.0, 3.5}}, {{3.0, 3.5}, {6.0, 3.5}},
				          {{1.0, 4.5}, {4.0, 4.5}}, {{5.0, 4.5}, {8.0, 4.5}}, {{0.0, 5.5}, {2.0, 5.5}}, {{3.0, 5.5}, {6.0, 5.5}},
				          {{1.0, 6.5}, {4.0, 6.5}}, {{5.0, 6.5}, {8.0, 6.5}}, {{0.0, 7.5}, {2.0, 7.5}}, {{3.0, 7.5}, {6.0, 7.5}}});

				AddPoints({{7.0, 1.5}, {7.0, 3.5}, {7.0, 5.5}, {7.0, 7.5}});

				break;
			}
			case HS_75Percent:
			{
				AddLines({{{1.0, 0.5}, {4.0, 0.5}}, {{5.0, 0.5}, {8.0, 0.5}}, {{0.0, 1.5}, {8.0, 1.5}},
				          {{0.0, 2.5}, {2.0, 2.5}}, {{3.0, 2.5}, {6.0, 2.5}}, {{0.0, 3.5}, {8.0, 3.5}},
				          {{1.0, 4.5}, {4.0, 4.5}}, {{5.0, 4.5}, {8.0, 4.5}}, {{0.0, 5.5}, {8.0, 5.5}},
				          {{0.0, 6.5}, {2.0, 6.5}}, {{3.0, 6.5}, {6.0, 6.5}}, {{0.0, 7.5}, {8.0, 7.5}}});

				AddPoints({{7.0, 2.5}, {7.0, 6.5}});


				break;
			}
			case HS_80Percent:
			{
				AddLines({{{0.0, 0.5}, {3.0, 0.5}}, {{4.0, 0.5}, {8.0, 0.5}}, {{0.0, 1.5}, {8.0, 1.5}},
				          {{0.0, 2.5}, {7.0, 2.5}}, {{0.0, 3.5}, {8.0, 3.5}},
				          {{0.0, 4.5}, {3.0, 4.5}}, {{4.0, 4.5}, {8.0, 4.5}}, {{0.0, 5.5}, {8.0, 5.5}},
				          {{0.0, 6.5}, {7.0, 6.5}}, {{0.0, 7.5}, {8.0, 7.5}}});

				break;
			}
			case HS_90Percent:
			{
				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{0.0, 1.5}, {8.0, 1.5}}, {{0.0, 2.5}, {8.0, 2.5}},
				          {{0.0, 3.5}, {4.0, 3.5}}, {{5.0, 3.5}, {8.0, 3.5}}, {{1.0, 7.5}, {8.0, 7.5}},
				          {{0.0, 4.5}, {8.0, 4.5}}, {{0.0, 5.5}, {8.0, 5.5}}, {{0.0, 6.5}, {8.0, 6.5}}});

				break;
			}

			case HS_LTDOWNWARDDIAG:
			{
				AddPoints({{0.0, 0.5}, {4.0, 0.5}, {1.0, 1.5}, {5.0, 1.5}, {2.0, 2.5}, {6.0, 2.5}, {3.0, 3.5}, {7.0, 3.5},
				           {0.0, 4.5}, {4.0, 4.5}, {1.0, 5.5}, {5.0, 5.5}, {2.0, 6.5}, {6.0, 6.5}, {3.0, 7.5}, {7.0, 7.5}});


				break;
			}
			case HS_LTUPWARDDIAG:
			{
				AddPoints({{3.0, 0.5}, {7.0, 0.5}, {2.0, 1.5}, {6.0, 1.5}, {1.0, 2.5}, {5.0, 2.5}, {0.0, 3.5}, {4.0, 3.5},
				           {3.0, 4.5}, {7.0, 4.5}, {2.0, 5.5}, {6.0, 5.5}, {1.0, 6.5}, {5.0, 6.5}, {0.0, 7.5}, {4.0, 7.5}});

				break;
			}
			case HS_DNDOWNWARDDIAG:
			{
				AddPoints({{0.0, 3.5}, {7.0, 3.5}, {0.0, 7.5}, {7.0, 7.5}});

				AddLines({{{0.0, 0.5}, {2.0, 0.5}}, {{4.0, 0.5}, {6.0, 0.5}}, {{1.0, 1.5}, {3.0, 1.5}}, {{5.0, 1.5}, {7.0, 1.5}},
				          {{2.0, 2.5}, {4.0, 2.5}}, {{6.0, 2.5}, {8.0, 2.5}}, {{3.0, 3.5}, {5.0, 3.5}},
				          {{0.0, 4.5}, {2.0, 4.5}}, {{4.0, 4.5}, {6.0, 4.5}}, {{1.0, 5.5}, {3.0, 5.5}}, {{5.0, 5.5}, {7.0, 5.5}},
				          {{2.0, 6.5}, {4.0, 6.5}}, {{6.0, 6.5}, {8.0, 6.5}}, {{3.0, 7.5}, {5.0, 7.5}}});

				break;
			}
			case HS_DNUPWARDDIAG:
			{
				AddPoints({{0.0, 3.5}, {7.0, 3.5}, {0.0, 7.5}, {7.0, 7.5}});

				AddLines({{{2.0, 0.5}, {4.0, 0.5}}, {{6.0, 0.5}, {8.0, 0.5}}, {{1.0, 1.5}, {3.0, 1.5}}, {{5.0, 1.5}, {7.0, 1.5}},
				          {{0.0, 2.5}, {2.0, 2.5}}, {{4.0, 2.5}, {6.0, 2.5}}, {{3.0, 3.5}, {5.0, 3.5}},
				          {{2.0, 4.5}, {4.0, 4.5}}, {{6.0, 4.5}, {8.0, 4.5}}, {{1.0, 5.5}, {3.0, 5.5}}, {{5.0, 5.5}, {7.0, 5.5}},
				          {{0.0, 6.5}, {2.0, 6.5}}, {{4.0, 6.5}, {6.0, 6.5}}, {{3.0, 7.5}, {5.0, 7.5}}});

				break;
			}
			case HS_WDOWNWARDDIAG:
			{
				AddPoints({{7.0, 0.5}, {0.0, 7.5}});

				AddLines({{{0.0, 0.5}, {2.0, 0.5}}, {{0.0, 1.5}, {3.0, 1.5}}, {{1.0, 2.5}, {4.0, 2.5}}, {{2.0, 3.5}, {5.0, 3.5}},
				          {{3.0, 4.5}, {6.0, 4.5}}, {{4.0, 5.5}, {7.0, 5.5}}, {{5.0, 6.5}, {8.0, 6.5}}, {{6.0, 7.5}, {8.0, 7.5}}});

				break;
			}
			case HS_WUPWARDDIAG:
			{
				AddPoints({{0.0, 0.5}, {7.0, 7.5}});

				AddLines({{{6.0, 0.5}, {8.0, 0.5}}, {{5.0, 1.5}, {8.0, 1.5}}, {{4.0, 2.5}, {7.0, 2.5}}, {{3.0, 3.5}, {6.0, 3.5}},
				          {{2.0, 4.5}, {5.0, 4.5}}, {{1.0, 5.5}, {4.0, 5.5}}, {{0.0, 6.5}, {3.0, 6.5}}, {{0.0, 7.5}, {2.0, 7.5}}});

				break;
			}

			case HS_LTVERTICAL:
			{
				AddLines({{{0.5, 0.0}, {0.5, 8.0}}, {{4.5, 0.0}, {4.5, 8.0}}});

				break;
			}
			case HS_LTHORIZONTAL:
			{
				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{0.0, 4.5}, {8.0, 4.5}}});

				break;
			}
			case HS_NVERTICAL:
			{
				AddLines({{{0.5, 0.0}, {0.5, 8.0}}, {{2.5, 0.0}, {2.5, 8.0}}, {{4.5, 0.0}, {4.5, 8.0}}, {{6.5, 0.0}, {6.5, 8.0}}});

				break;
			}
			case HS_NHORIZONTAL:
			{
				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{0.0, 2.5}, {8.0, 2.5}}, {{0.0, 4.5}, {8.0, 4.5}}, {{0.0, 6.5}, {8.0, 6.5}}});

				break;
			}
			case HS_DNVERTICAL:
			{
				m_dStrokeWidth *= 2;

				AddLines({{{0.5, 0.0}, {0.5, 8.0}}, {{4.5, 0.0}, {4.5, 8.0}}});

				m_dStrokeWidth /= 2;

				break;
			}
			case HS_DNHORIZONTAL:
			{
				m_dStrokeWidth *= 2;

				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{0.0, 4.5}, {8.0, 4.5}}});

				m_dStrokeWidth /= 2;

				break;
			}

			case HS_DASHDOWNWARDDIAG:
			{
				AddPoints({{0.0, 2.5}, {4.0, 2.5}, {1.0, 3.5}, {5.0, 3.5}, {2.0, 4.5}, {6.0, 4.5}, {3.0, 5.5}, {7.0, 5.5}});

				break;
			}
			case HS_DASHUPWARDDIAG:
			{
				AddPoints({{3.0, 2.5}, {7.0, 2.5}, {2.0, 3.5}, {6.0, 3.5}, {1.0, 4.5}, {5.0, 4.5}, {0.0, 5.5}, {4.0, 5.5}});

				break;
			}
			case HS_DASHHORIZONTAL:
			{
				AddLines({{{0.0, 0.5}, {4.0, 0.5}}, {{4.0, 4.5}, {8.0, 4.5}}});

				break;
			}
			case HS_DASHVERTICAL:
			{
				AddLines({{{0.5, 0.0}, {0.5, 4.0}}, {{4.5, 4.0}, {4.5, 8.0}}});

				break;
			}

			case HS_SMALLCONFETTI:
			{
				AddPoints({{0.0, 0.5}, {4.0, 1.5}, {1.0, 2.5}, {6.0, 3.5}, {3.0, 4.5}, {7.0, 5.5}, {2.0, 6.5}, {5.0, 7.5}});

				break;
			}
			case HS_LARGECONFETTI:
			{
				m_dStrokeWidth *= 2;

				AddLines({{{2.0, 0.5}, {4.0, 0.5}}, {{6.0, 2.5}, {8.0, 2.5}}, {{3.0, 3.5}, {5.0, 3.5}}, {{0.0, 4.5}, {2.0, 4.5}}, {{4.0, 6.5}, {6.0, 6.5}}});

				m_dStrokeWidth /= 2;

				break;
			}
			case HS_ZIGZAG:
			{
				AddPoints({{0.0, 0.5}, {7.0, 0.5}, {1.0, 1.5}, {6.0, 1.5}, {2.0, 2.5}, {5.0, 2.5},
				           {0.0, 4.5}, {7.0, 4.5}, {1.0, 5.5}, {6.0, 5.5}, {2.0, 6.5}, {5.0, 6.5}});

				AddLines({{{3.0, 3.5}, {5.0, 3.5}}, {{3.0, 7.5}, {5.0, 7.5}}});

				break;
			}
			case HS_WAVE:
			{
				AddPoints({{1.0, 1.5}, {4.0, 1.5}, {7.0, 1.5},
				           {1.0, 5.5}, {4.0, 5.5}, {7.0, 5.5}});

				AddLines({{{5.0, 0.5}, {7.0, 0.5}}, {{2.0, 2.5}, {4.0, 2.5}},
				          {{5.0, 4.5}, {7.0, 4.5}}, {{2.0, 6.5}, {4.0, 6.5}}});

				break;
			}
			case HS_DIAGBRICK:
			{
				AddPoints({{7.0, 0.5}, {6.0, 1.5}, {5.0, 2.5}, {4.0, 3.5}, {2.0, 5.5}, {5.0, 5.5},
				           {1.0, 6.5}, {6.0, 6.5}, {0.0, 7.5}, {7.0, 7.5}});

				AddLine({3.0, 4.5}, {5.0, 4.5});

				break;
			}
			case HS_HORIZBRICK:
			{
				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{0.5, 0.5}, {0.5, 4.5}}, {{0.0, 4.5}, {8.0, 4.5}}, {{4.5, 4.5}, {4.5, 8.0}}});

				break;
			}
			case HS_WEAVE:
			{
				AddPoints({{0.0, 0.5}, {4.0, 0.5}, {1.0, 1.5}, {3.0, 1.5}, {5.0, 1.5},
				           {2.0, 2.5}, {6.0, 2.5}, {1.0, 3.5}, {5.0, 3.5}, {7.0, 3.5},
				           {0.0, 4.5}, {4.0, 4.5}, {1.0, 5.5}, {3.0, 5.5},
				           {2.0, 6.5}, {6.0, 6.5}, {1.0, 7.5}, {5.0, 7.5}, {7.0, 7.5}});

				break;
			}
			case HS_PLAID:
			{
				AddPoints({{0.0, 0.5}, {2.0, 0.5}, {4.0, 0.5}, {6.0, 0.5},
				           {1.0, 1.5}, {3.0, 1.5}, {5.0, 1.5}, {7.0, 1.5},
				           {0.0, 2.5}, {2.0, 2.5}, {4.0, 2.5}, {6.0, 2.5},
				           {1.0, 3.5}, {3.0, 3.5}, {5.0, 3.5}, {7.0, 3.5}});

				AddLines({{{0.0, 4.5}, {4.0, 4.5}}, {{0.0, 5.5}, {4.0, 5.5}}, {{0.0, 6.5}, {4.0, 6.5}}, {{0.0, 7.5}, {4.0, 7.5}}});

				break;
			}
			case HS_DIVOT:
			{
				AddPoints({{3.0, 1.5}, {4.0, 2.5}, {3.0, 3.5},
				           {0.0, 5.5}, {7.0, 6.5}, {0.0, 7.5}});

				break;
			}
			case HS_DOTGRID:
			{
				AddPoints({{0.0, 0.5}, {2.0, 0.5}, {4.0, 0.5}, {6.0, 0.5},
				           {0.0, 2.5}, {0.0, 4.5}, {0.0, 6.5}});

				break;
			}
			case HS_DOTDIAMOND:
			{
				AddPoints({{0.0, 0.5}, {2.0, 2.5}, {6.0, 2.5}, {4.0, 4.5}, {2.0, 6.5}, {6.0, 6.5}});

				break;
			}
			case HS_SHINGLE:
			{
				AddPoints({{0.0, 1.5}, {5.0, 1.5}, {1.0, 2.5}, {4.0, 2.5}, {6.0, 5.5}});

				AddLines({{{6.0, 0.5}, {8.0, 0.5}}, {{2.0, 3.5}, {4.0, 3.5}}, {{4.0, 4.5}, {6.0, 4.5}}, {{7.5, 6.0}, {7.5, 8.0}}});

				break;
			}
			case HS_TRELLIS:
			{
				AddPoints({{0.0, 3.5}, {7.0, 3.5}, {0.0, 7.5}, {7.0, 7.5}});

				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{1.0, 1.5}, {3.0, 1.5}}, {{5.0, 1.5}, {7.0, 1.5}},
				          {{0.0, 2.5}, {8.0, 2.5}}, {{3.0, 3.5}, {5.0, 3.5}},
				          {{0.0, 4.5}, {8.0, 4.5}}, {{1.0, 5.5}, {3.0, 5.5}}, {{5.0, 5.5}, {7.0, 5.5}},
				          {{0.0, 6.5}, {8.0, 6.5}}, {{3.0, 7.5}, {5.0, 7.5}}});

				break;
			}
			case HS_SPHERE:
			{
				AddPoints({{0.0, 1.5}, {4.0, 1.5}, {7.0, 1.5}, {0.0, 2.5}, {0.0, 3.5},
				           {0.0, 5.5}});

				AddLines({{{1.0, 0.5}, {4.0, 0.5}}, {{5.0, 0.5}, {8.0, 0.5}}, {{4.0, 2.5}, {8.0, 2.5}}, {{4.0, 3.5}, {8.0, 3.5}},
				          {{1.0, 4.5}, {4.0, 4.5}}, {{5.0, 4.5}, {8.0, 4.5}}, {{3.0, 5.5}, {5.0, 5.5}},
				          {{0.0, 6.5}, {5.0, 6.5}}, {{0.0, 7.5}, {5.0, 7.5}}});

				break;
			}
			case HS_SGRID:
			{
				AddLines({{{0.0, 0.5}, {8.0, 0.5}}, {{0.0, 4.5}, {8.0, 4.5}},
				          {{0.5, 0.0}, {0.5, 8.0}}, {{4.5, 0.0}, {4.5, 8.0}}});

				break;
			}
			case HS_SCHECHERBOARD:
			{
				m_dStrokeWidth *= 2;

				AddPoints({{0.0, -0.5}, {7.0, -0.5}, {0.0, 3.5}, {7.0, 3.5}, {0.0, 7.5}, {7.0, 7.5}});

				AddLines({{{3.0, -0.5}, {5.0, -0.5}}, {{1.0, 1.5}, {3.0, 1.5}}, {{5.0, 1.5}, {7.0, 1.5}},
				          {{3.0, 3.5}, {5.0, 3.5}}, {{1.0, 5.5}, {3.0, 5.5}}, {{5.0, 5.5}, {7.0, 5.5}},
				          {{3.0, 7.5}, {5.0, 7.5}}});

				m_dStrokeWidth /= 2;

				break;
			}
			case HS_LCHECHERBOARD:
			{
				m_dStrokeWidth *= 4;

				AddLines({{{0.0, 1.0}, {4.0, 1.0}}, {{4.0, 5.0}, {8.0, 5.0}}});

				m_dStrokeWidth /= 4;

				break;
			}
			case HS_OUTLINEDDIAMOND:
			{
				AddPoints({{0.0, 0.5}, {6.0, 0.5}, {1.0, 1.5}, {5.0, 1.5}, {2.0, 2.5}, {4.0, 2.5}, {3.0, 3.5},
				           {4.0, 4.5}, {2.0, 4.5}, {5.0, 5.5}, {1.0, 5.5}, {6.0, 6.5}, {0.0, 6.5}, {7.0, 7.5}});

				break;
			}
			case HS_SOLIDDIAMOND:
			{
				AddPoints({{3.0, 0.5}, {3.0, 6.5}});

				AddLines({{{2.0, 1.5}, {5.0, 1.5}}, {{1.0, 2.5}, {6.0, 2.5}}, {{0.0, 3.5}, {7.0, 3.5}},
				          {{1.0, 4.5}, {6.0, 4.5}}, {{2.0, 5.5}, {5.0, 5.5}}});

				break;
			}

			default: return false;
		}

		GenerateEndPattern();

		return true;
	}

	std::wstring CHatchGenerator::GetPatternId()
	{
		if (mHatchStyles.end() == mHatchStyles.find(m_nHatchStyle))
			return L"";

		return mHatchStyles.at(m_nHatchStyle) + L'_' + ConvertToWString(m_unNumber);
	}

	std::wstring CHatchGenerator::GetPattern()
	{
		return m_oStringBuilder.GetData();
	}

	void CHatchGenerator::AddLine(const TPointD& oPoint1, const TPointD& oPoint2)
	{
		m_oStringBuilder.WriteNodeBegin(L"line", true);
		m_oStringBuilder.WriteAttribute(L"x1", ConvertToWString(m_dStrokeWidth * oPoint1.x));
		m_oStringBuilder.WriteAttribute(L"y1", ConvertToWString(m_dStrokeWidth * oPoint1.y));
		m_oStringBuilder.WriteAttribute(L"x2", ConvertToWString(m_dStrokeWidth * oPoint2.x));
		m_oStringBuilder.WriteAttribute(L"y2", ConvertToWString(m_dStrokeWidth * oPoint2.y));
		m_oStringBuilder.WriteAttribute(L"stroke", L"rgb(" + INTCOLOR_TO_RGB(m_nStrokeColor) + L')');
		m_oStringBuilder.WriteAttribute(L"stroke-width", ConvertToWString(m_dStrokeWidth));
		m_oStringBuilder.WriteNodeEnd(L"line", true, true);
	}

	void CHatchGenerator::AddLines(const std::vector<std::pair<TPointD, TPointD>> &arPoints)
	{
		for (const std::pair<TPointD, TPointD>& oPoints : arPoints)
			AddLine(oPoints.first, oPoints.second);
	}

	void CHatchGenerator::AddPoint(const TPointD& oPoint)
	{
		m_oStringBuilder.WriteNodeBegin(L"line", true);
		m_oStringBuilder.WriteAttribute(L"x1", ConvertToWString(m_dStrokeWidth * oPoint.x));
		m_oStringBuilder.WriteAttribute(L"y1", ConvertToWString(m_dStrokeWidth * oPoint.y));
		m_oStringBuilder.WriteAttribute(L"x2", ConvertToWString(m_dStrokeWidth * (oPoint.x + 1)));
		m_oStringBuilder.WriteAttribute(L"y2", ConvertToWString(m_dStrokeWidth * oPoint.y));
		m_oStringBuilder.WriteAttribute(L"stroke", L"rgb(" + INTCOLOR_TO_RGB(m_nStrokeColor) + L')');
		m_oStringBuilder.WriteAttribute(L"stroke-width", ConvertToWString(m_dStrokeWidth));
		m_oStringBuilder.WriteNodeEnd(L"line", true, true);
	}

	void CHatchGenerator::AddPoints(const std::vector<TPointD> &arPoints)
	{
		for (const TPointD& oPoint : arPoints)
			AddPoint(oPoint);
	}

	bool CHatchGenerator::GenerateStartPattern()
	{
		std::wstring wsPatternId = GetPatternId();

		if (wsPatternId.empty())
			return false;

		m_oStringBuilder.WriteNodeBegin(L"pattern", true);
		m_oStringBuilder.WriteAttribute(L"id", wsPatternId);
		m_oStringBuilder.WriteAttribute(L"width",  ((0 != m_dWidth)  ? ConvertToWString((m_dStrokeWidth * 8.) / m_dWidth)  : L"1"));
		m_oStringBuilder.WriteAttribute(L"height", ((0 != m_dHeight) ? ConvertToWString((m_dStrokeWidth * 8.) / m_dHeight) : L"1"));
		m_oStringBuilder.WriteAttribute(L"patternUnits", L"objectBoundingBox");
		m_oStringBuilder.WriteAttribute(L"shape-rendering", L"crispEdges");
		m_oStringBuilder.WriteNodeEnd(L"pattern", true, false);

		return true;
	}

	void CHatchGenerator::GenerateBK()
	{
		if (0 > m_nBKColor)
			return;

		m_oStringBuilder.WriteNodeBegin(L"rect", true);
		m_oStringBuilder.WriteAttribute(L"x", L"0");
		m_oStringBuilder.WriteAttribute(L"y", L"0");
		m_oStringBuilder.WriteAttribute(L"width",  ConvertToWString(m_dStrokeWidth * 8));
		m_oStringBuilder.WriteAttribute(L"height", ConvertToWString(m_dStrokeWidth * 8));
		m_oStringBuilder.WriteAttribute(L"fill", L"rgb(" + INTCOLOR_TO_RGB(m_nBKColor) + L')');
		m_oStringBuilder.WriteNodeEnd(L"rect", true, true);
	}

	void CHatchGenerator::GenerateEndPattern()
	{
		m_oStringBuilder.WriteNodeEnd(L"pattern");
	}

	CSvgClip::CSvgClip()
        : m_bStartClip(false), m_bEndClip(false)
	{}

	void CSvgClip::Reset()
	{
		if (StartClip() && !EndClip())
			CloseClip();

		m_bStartClip = false;
		m_bEndClip   = false;
		m_arValues.clear();
	}

	void CSvgClip::BeginClip()
	{
		m_bStartClip = true;
	}

	void CSvgClip::CloseClip()
	{
		m_bEndClip = true;
	}

	bool CSvgClip::StartClip() const
	{
		return m_bStartClip;
	}

	bool CSvgClip::EndClip() const
	{
		return m_bEndClip;
	}

	bool CSvgClip::Empty() const
	{
		return m_arValues.empty();
	}

	void CSvgClip::AddClipValue(const std::wstring &wsId, const std::wstring &wsValue, int nClipMode)
	{
		m_arValues.push_back(std::make_tuple(wsId, wsValue, nClipMode));
	}

	std::wstring CSvgClip::GetClip() const
	{
		if (m_arValues.empty())
			return std::wstring();

		std::wstring wsClip;

		std::wstring wsId;

		for (ClipValue::const_iterator oIter = m_arValues.begin(); oIter != m_arValues.end(); ++oIter)
		{
			wsId = std::get<0>(*oIter);

			switch (std::get<2>(*oIter))
			{
			case RGN_AND:
			{
				size_t unPosition = wsClip.find(L">", wsClip.rfind(L"<clipPath"));

				if (std::wstring::npos != unPosition)
					wsClip.insert(unPosition, L" clip-path=\"url(#" + wsId + L")\"");

				wsClip += L"<clipPath id=\"" + wsId + L"\">" + std::get<1>(*oIter) + L"</clipPath>";
				break;
			}
			case RGN_OR:
			{
				if (wsClip.empty())
					wsClip += L"<clipPath id=\"" + wsId + L"\">" + std::get<1>(*oIter) + L"</clipPath>";
				else
					wsClip.insert(wsClip.length() - 11, std::get<1>(*oIter));
				break;
			}
			case RGN_XOR:
			{
				size_t unPosition = wsClip.find(L">", wsClip.rfind(L"<clipPath"));

				if (std::wstring::npos != unPosition)
					wsClip.insert(unPosition, L" clip-rule=\"evenodd\"");

				if (wsClip.empty())
					wsClip += L"<clipPath id=\"" + wsId + L"\">" + std::get<1>(*oIter) + L"</clipPath>";
				else
					wsClip.insert(wsClip.length() - 11, std::get<1>(*oIter));
			}
			case RGN_COPY:
			{
				wsClip = L"<clipPath id=\"" + wsId + L"\">" + std::get<1>(*oIter) + L"</clipPath>";
				break;
			}
			}
		}

		return wsClip;
	}

	std::wstring CSvgClip::GetClipId() const
	{
		if (m_arValues.empty())
			return std::wstring();

		std::wstring wsId = std::get<0>(m_arValues.front());

		for (ClipValue::const_iterator oIter = m_arValues.begin() + 1; oIter != m_arValues.end(); ++oIter)
		{
			if (RGN_COPY == std::get<2>(*oIter))
				wsId = std::get<0>(*oIter);
		}

		return wsId;
	}

}
