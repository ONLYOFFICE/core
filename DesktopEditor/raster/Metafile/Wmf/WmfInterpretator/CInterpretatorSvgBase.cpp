#include "CInterpretatorSvgBase.h"

#include "../../../BgraFrame.h"

namespace MetaFile
{
    #define MAXTRANSFORMSCALE 100.
    #define MINTRANSFORMSCALE 0.00000001

	CInterpretatorSvgBase::CInterpretatorSvgBase(IMetaFileBase *pParser, double dWidth, double dHeight)
	    : m_oSizeWindow(dWidth, dHeight), m_oScale(1, 1), m_unNumberDefs(0), m_pParser(pParser)
	{}

	CInterpretatorSvgBase::~CInterpretatorSvgBase()
	{}

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
			m_oXmlWriter = *pXmlWriter;
	}

	XmlUtils::CXmlWriter *CInterpretatorSvgBase::GetXmlWriter()
	{
		return &m_oXmlWriter;
	}

	std::wstring CInterpretatorSvgBase::GetFile()
	{
		return m_oXmlWriter.GetXmlString();
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

		m_oXmlWriter.WriteString(wsNewSvg);

		WriteNodeEnd(L"g");
	}

	void CInterpretatorSvgBase::WriteNode(const std::wstring &wsNodeName, const NodeAttributes &arAttributes, const std::wstring &wsValueNode)
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
			m_oXmlWriter.WriteString (wsValueNode);
			m_oXmlWriter.WriteNodeEnd(wsNodeName, false, false);
		}
	}

	void CInterpretatorSvgBase::WriteNodeBegin(const std::wstring &wsNodeName, const NodeAttributes &arAttributes)
	{
		m_oXmlWriter.WriteNodeBegin(wsNodeName, !arAttributes.empty());

		if (!arAttributes.empty())
		{
			for (const NodeAttribute& oAttribute : arAttributes)
				m_oXmlWriter.WriteAttribute(oAttribute.first, oAttribute.second);

			m_oXmlWriter.WriteNodeEnd(wsNodeName, true, false);
		}
	}

	void CInterpretatorSvgBase::WriteNodeEnd(const std::wstring &wsNodeName)
	{
		m_oXmlWriter.WriteNodeEnd(wsNodeName, false, false);
	}

	void CInterpretatorSvgBase::WriteText(const std::wstring& wsText, const TPointD& oCoord, const TRect& oBounds, const TPointD& oScale)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetFont())
			return;

		NodeAttributes arNodeAttributes;

		double dXCoord = oCoord.x;
		double dYCoord = oCoord.y;

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

	void CInterpretatorSvgBase::AddStroke(NodeAttributes &arAttributes) const
	{
		if (NULL == m_pParser)
			return;

		IPen *pPen = m_pParser->GetPen();

		if (NULL == pPen || PS_NULL == pPen->GetStyle())
			return;

		if (pPen->GetAlpha() != 255)
			arAttributes.push_back({L"stroke-opacity" , ConvertToWString(pPen->GetAlpha() / 255., 3)});

		arAttributes.push_back({L"stroke", L"rgb(" + INTCOLOR_TO_RGB(pPen->GetColor()) + L')'});

		double dStrokeWidth = std::fabs(pPen->GetWidth());

		if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (pPen->GetStyle() & PS_TYPE_MASK)))
			dStrokeWidth = 1. / std::fabs(m_pParser->GetTransform()->M11);

		arAttributes.push_back({L"stroke-width", ConvertToWString(dStrokeWidth)});

		unsigned int unMetaPenStyle = pPen->GetStyle();
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

	void CInterpretatorSvgBase::AddTransform(NodeAttributes &arAttributes, TXForm *pTransform) const
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

	void CInterpretatorSvgBase::AddClip(NodeAttributes &arAttributes)
	{
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

	std::wstring CInterpretatorSvgBase::CreateDibPatternStyle(IBrush *pBrush)
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
}
