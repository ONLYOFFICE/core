#include "CInterpretatorSvgBase.h"
#include "../../../BgraFrame.h"

#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdint.h>

#ifndef MININT32
#define MAXUINT32   ((uint32_t)~((uint32_t)0))
#define MAXINT32    ((int32_t)(MAXUINT32 >> 1))
#define MININT32    ((int32_t)~MAXINT32)
#endif

namespace MetaFile
{
    #define MAXTRANSFORMSCALE 100.
    #define MINTRANSFORMSCALE 0.00000001

	const std::map<int, std::wstring> mHatchStyles =
	{
	    { HS_HORIZONTAL,       L"HORIZONTAL"},
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
	    : m_oSizeWindow(dWidth, dHeight), m_unNumberDefs(0), m_pParser(pParser), m_pXmlWriter(new XmlUtils::CXmlWriter()),
	      m_bExternXmlWriter(false), m_bUpdatedClip(true), m_eShapeRendering(EShapeRendering::Auto)
	{}

	CInterpretatorSvgBase::~CInterpretatorSvgBase()
	{
		if (!m_bExternXmlWriter)
			RELEASEOBJECT(m_pXmlWriter);
	}

	void CInterpretatorSvgBase::SetSize(double dWidth, double dHeight)
	{
		m_oSizeWindow.X = dWidth;
		m_oSizeWindow.Y = dHeight;
	}

	void CInterpretatorSvgBase::GetSize(double &dWidth, double &dHeight)
	{
		dWidth  = m_oSizeWindow.X;
		dHeight = m_oSizeWindow.Y;
	}

	void CInterpretatorSvgBase::UpdateSize()
	{
		if (Equals(0., m_oSizeWindow.X) && !Equals(0., m_oViewport.GetWidth()))
			m_oSizeWindow.X =  m_oViewport.GetWidth();

		if (Equals(0., m_oSizeWindow.Y) && !Equals(0., m_oViewport.GetHeight()))
			m_oSizeWindow.Y =  m_oViewport.GetHeight();

		if (0 != m_oSizeWindow.X && 0 == m_oSizeWindow.Y)
			m_oSizeWindow.Y = m_oSizeWindow.X * (m_oViewport.GetHeight() / m_oViewport.GetWidth());
		else if (0 == m_oSizeWindow.X && 0 != m_oSizeWindow.Y)
			m_oSizeWindow.X = m_oSizeWindow.Y * (m_oViewport.GetWidth() / m_oViewport.GetHeight());
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

	void CInterpretatorSvgBase::SetShapeRendering(EShapeRendering eShapeRenderingType)
	{
		m_eShapeRendering = eShapeRenderingType;
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

		if (oNewClipRect.Left > oNewClipRect.Right)
			std::swap(oNewClipRect.Left, oNewClipRect.Right);

		if (oNewClipRect.Top > oNewClipRect.Bottom)
			std::swap(oNewClipRect.Top, oNewClipRect.Bottom);

		wsNewSvg.erase(unFirstPos, unSecondPos - unFirstPos);

		std::wstring wsClip = L"x=\"" + ConvertToWString(oRect.Left) + L"\" y=\"" + ConvertToWString(oRect.Top) + L"\" " +
		                      L"width=\"" + ConvertToWString(oRect.Right - oRect.Left) + L"\" height=\"" + ConvertToWString(oRect.Bottom - oRect.Top) + L"\" " +
		                      L"viewBox=\"" + ConvertToWString(oNewClipRect.Left) + L' ' + ConvertToWString(oNewClipRect.Top) + L' ' + ConvertToWString(oNewClipRect.Right - oNewClipRect.Left) + L' ' + ConvertToWString(oNewClipRect.Bottom - oNewClipRect.Top) + L'\"';

		wsNewSvg.insert(unFirstPos, wsClip);

		NodeAttributes arNodeAttributes;

		AddTransform(arNodeAttributes, pTransform);

		if (!arNodeAttributes.empty())
			WriteNodeBegin(L"g", arNodeAttributes);

		m_pXmlWriter->WriteString(wsNewSvg);

		if (!arNodeAttributes.empty())
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

	void CInterpretatorSvgBase::WriteText(const std::wstring &wsText, const TPointD &oCoord, const TRectL &oBounds, const TPointD &oScale, const std::vector<double>& arDx)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetFont())
			return;

		AddClip();

		NodeAttributes arNodeAttributes;

		double dXCoord = oCoord.X;
		double dYCoord = oCoord.Y;

		if (m_pParser->GetTextAlign() & TA_UPDATECP && (0. == oCoord.X && 0. == oCoord.Y))
		{
			dXCoord = m_pParser->GetCurPos().X;
			dYCoord = m_pParser->GetCurPos().Y;
		}

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		bool bWriteG = false;

		if (OPAQUE == m_pParser->GetTextBgMode())
		{
			std::wstring wsFillRect = CalculateColor(m_pParser->GetTextBgColor(), 255);

			WriteNodeBegin(L"g", {});
			bWriteG = true;

			NodeAttributes arRectAttributes{{L"x",      ConvertToWString(oBounds.Left)},
			                                {L"y",      ConvertToWString(oBounds.Top)},
			                                {L"width",  ConvertToWString(oBounds.Right - oBounds.Left)},
			                                {L"height", ConvertToWString(oBounds.Bottom - oBounds.Top)},
			                                {L"fill",   wsFillRect},
			                                {L"stroke", L"none"}};

			AddShapeRendering(arRectAttributes);

			WriteNode(L"rect", arNodeAttributes);
		}

		int nColor = m_pParser->GetTextColor();

		if (0 != nColor)
			arNodeAttributes.Add(L"fill", CalculateColor(nColor, 255));

		const IFont *pFont = m_pParser->GetFont();

		double dFontHeight = std::fabs(pFont->GetHeight());

		if (dFontHeight < 0.01)
			dFontHeight = DEFAULT_FONT_SIZE;

		arNodeAttributes.Add(L"font-size", dFontHeight);

		NSStringUtils::CStringBuilder oFontName;
		oFontName.WriteEncodeXmlString(pFont->GetFaceName());

		if (0 != oFontName.GetSize())
		{
			#ifndef BUILDING_WASM_MODULE
			NSFonts::CFontSelectFormat oFormat;
			oFormat.wsName = new std::wstring(pFont->GetFaceName());

			NSFonts::CFontInfo *pFontInfo = m_pParser->GetFontManager()->GetFontInfoByParams(oFormat);

			if (NULL != pFontInfo && !StringEquals(*oFormat.wsName, pFontInfo->m_wsFontName))
			{
				oFontName.Clear();
				oFontName.WriteEncodeXmlString(L"\'");
				oFontName.WriteEncodeXmlString(*oFormat.wsName);
				oFontName.WriteEncodeXmlString(L"\',\'");
				oFontName.WriteEncodeXmlString(pFontInfo->m_wsFontName);
				oFontName.WriteEncodeXmlString(L"\'");
			}
			#endif
			arNodeAttributes.Add(L"font-family", oFontName.GetData());
		}

		if (pFont->GetWeight() > 550)
			arNodeAttributes.Add(L"font-weight", L"bold");

		if (pFont->IsItalic())
			arNodeAttributes.Add(L"font-style", L"italic");

		if (pFont->IsUnderline() && pFont->IsStrikeOut())
			arNodeAttributes.Add(L"text-decoration", L"underline line-through");
		else if (pFont->IsUnderline())
			arNodeAttributes.Add(L"text-decoration", L"underline");
		else if (pFont->IsStrikeOut())
			arNodeAttributes.Add(L"text-decoration", L"line-through");

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
			arNodeAttributes.Add(L"dominant-baseline", L"auto");

			if (ulVTextAlign != VTA_BOTTOM)
				ulTextAlign -= TA_BOTTOM;
		}
		else if (ulVTextAlign == VTA_CENTER)
		{
			arNodeAttributes.Add(L"dominant-baseline", L"middle");
		}
		else // if (ulTextAlign & TA_TOP)
		{
			dYCoord += dFontHeight;
		}

		if (ulTextAlign == TA_RIGHT)
		{
			if (arDx.empty())
				arNodeAttributes.Add(L"text-anchor", L"end");
			else
				dXCoord -= std::accumulate(arDx.begin(), arDx.end(), 0.0);
		}
		else if (ulTextAlign == TA_CENTER)
		{
			if (arDx.empty())
				arNodeAttributes.Add(L"text-anchor", L"middle");
			else
				dXCoord -= std::accumulate(arDx.begin(), arDx.end(), 0.0) / 2;
		}
		else  //if (ulTextAlign & TA_LEFT)
		{
			// Ничего не делаем
		}

		if (oScale.Y < -0.00001) //TODO::Тоже нужно и для dXScale
		{
			dYCoord += dFontHeight;

			oTransform.Dy += (2 * dYCoord - dFontHeight) * oTransform.M22;

			oTransform.M22 = fabs(oTransform.M22);
		}

		if (0 != pFont->GetEscapement())
		{
			double dEscapement = pFont->GetEscapement() / -10;

			if (m_pParser->GetTransform().M22 < 0)
				dEscapement = -dEscapement;

			double dSin = std::sin(dEscapement * M_PI / 180.);

			dYCoord -= dFontHeight * dSin;

			if (oScale.Y < -0.00001)
				dXCoord -= dFontHeight * dSin;

			arNodeAttributes.Add(L"transform", L"rotate(" + ConvertToWString(dEscapement) + L' ' + ConvertToWString(dXCoord) + L' ' + ConvertToWString(dYCoord) + L')');

			if (oScale.Y > 0.00001)
				dXCoord -= dFontHeight * dSin;
		}

		AddTransform(arNodeAttributes, &oTransform);

		arNodeAttributes.Add(L"xml:space", L"preserve");

		size_t unPosLineBreak = wsText.find(L"\n");

		std::wstring wsXCoord;

		if (arDx.empty() || arDx.size() < wsText.length())
			wsXCoord = ConvertToWString(dXCoord);
		else
		{
			std::vector<double> arXCoords(wsText.length());

			arXCoords[0] = dXCoord;

			for (unsigned int unIndex = 1; unIndex < wsText.length(); ++unIndex)
				arXCoords[unIndex] = arDx[unIndex - 1] + arXCoords[unIndex - 1];

			wsXCoord = ConvertToWString(arXCoords);
		}

		if (std::wstring::npos == unPosLineBreak)
		{
			arNodeAttributes.Add(L"x", wsXCoord);
			arNodeAttributes.Add(L"y", dYCoord);

			WriteNode(L"text", arNodeAttributes, StringNormalization(wsText));
		}
		else
		{
			size_t unStart = 0;
			double dYNewCoord = dYCoord;

			WriteNodeBegin(L"text", arNodeAttributes);

			do
			{
				WriteNode(L"tspan", {{L"x", wsXCoord},
				                     {L"y", ConvertToWString(dYNewCoord)}}, StringNormalization(wsText.substr(unStart, unPosLineBreak - unStart)));

				dYNewCoord += dFontHeight * 1.6;
				unStart = wsText.find_first_not_of(L"\n", unPosLineBreak);
				unPosLineBreak = wsText.find(L"\n", unStart);
			}
			while(unStart != std::wstring::npos);

			WriteNodeEnd(L"text");
		}

		if (bWriteG)
			m_pXmlWriter->WriteNodeEnd(L"g");
	}

	void CInterpretatorSvgBase::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight, unsigned int unBlendMode)
	{
		if (NULL == pBuffer || Equals(0., dW) || Equals(0., dH) || 0 == unWidth || 0 == unHeight)
			return;

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		if (dW < 0 || dH < 0)
		{
			double dKx = 1, dKy = 1, dShiftKoefX = 0, dShiftKoefY = 0;
			if (dW < 0)
			{
				dKx = -1;
				dShiftKoefX = 2 * dX + dW;

				dW = -dW;
				dX -= dW;
			}

			if (dH < 0)
			{
				dKy = -1;
				dShiftKoefY = 2 * dY + dH;

				dH = -dH;
				dY -= dH;
			}

			oTransform.Dx  += dShiftKoefX * oTransform.M11 + dShiftKoefY * oTransform.M21;
			oTransform.Dy  += dShiftKoefX * oTransform.M12 + dShiftKoefY * oTransform.M22;
			oTransform.M11 *= dKx;
			oTransform.M12 *= dKx;
			oTransform.M21 *= dKy;
			oTransform.M22 *= dKy;
		}

		if (1 == unWidth && 1 == unHeight)
		{
			NodeAttributes arAttributes = {{L"x",      ConvertToWString(dX)},
			                               {L"y",      ConvertToWString(dY)},
			                               {L"width",  ConvertToWString(dW)},
			                               {L"height", ConvertToWString(dH)},
			                               {L"fill",   CalculateColor(pBuffer[2], pBuffer[1], pBuffer[0], 255)}};

			AddShapeRendering(arAttributes);
			AddTransform(arAttributes, &oTransform);
			AddClip();

			WriteNode(L"rect", arAttributes);

			return;
		}

		CBgraFrame  oFrame;

		oFrame.put_Data(pBuffer);
		oFrame.put_Width(unWidth);
		oFrame.put_Height(unHeight);

		BYTE* pNewBuffer = NULL;
		int nNewSize = 0;

		if (!oFrame.Encode(pNewBuffer, nNewSize, 4))
		{
			oFrame.put_Data(NULL);
			return;
		}

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

			AddTransform(arAttributes, &oTransform);
			AddClip();

			WriteNode(L"image", arAttributes);
		}

		if (NULL != pNewBuffer)
			delete [] pNewBuffer;
	}

	void CInterpretatorSvgBase::ResetClip()
	{
		CloseClip();
		m_oClip.Reset();
		m_bUpdatedClip = false;
	}

	void CInterpretatorSvgBase::IntersectClip(const TRectD &oClip)
	{
		TRectD oUpdatedClip{oClip};

		NormalizeRect(oUpdatedClip);

		m_pParser->GetTransform().Apply(oUpdatedClip.Left,  oUpdatedClip.Top);
		m_pParser->GetTransform().Apply(oUpdatedClip.Right, oUpdatedClip.Bottom);

		const std::wstring wsId    = L"INTERSECTCLIP_" + ConvertToWString(++m_unNumberDefs, 0);
		const std::wstring wsValue = L"<rect x=\"" + ConvertToWString(oUpdatedClip.Left, 0) +
		                             L"\" y=\"" + ConvertToWString(oUpdatedClip.Top, 0) +
		                             L"\" width=\"" + ConvertToWString(oUpdatedClip.Right - oUpdatedClip.Left, 0) +
		                             L"\" height=\"" + ConvertToWString(oUpdatedClip.Bottom - oUpdatedClip.Top, 0) + L"\"/>";

		m_oClip.AddClipValue(wsId, wsValue);
	}

	void CInterpretatorSvgBase::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		const TXForm &oTransform{m_pParser->GetTransform()};

		TRectD oUpdatedClip{oClip};
		NormalizeRect(oUpdatedClip);

		oTransform.Apply(oUpdatedClip.Left, oUpdatedClip.Top);
		oTransform.Apply(oUpdatedClip.Right, oUpdatedClip.Bottom);

		TRectD oBBRect{oBB};
		NormalizeRect(oBBRect);

		oTransform.Apply(oBBRect.Left, oBBRect.Top);
		oTransform.Apply(oBBRect.Right, oBBRect.Bottom);

		const std::wstring wsId    = L"EXCLUDECLIP_" + ConvertToWString(++m_unNumberDefs, 0);
		const std::wstring wsValue = L"<path d=\"M" + ConvertToWString(oBBRect.Left) + L' ' + ConvertToWString(oBBRect.Top) + L", L" + ConvertToWString(oBBRect.Right) + L' ' + ConvertToWString(oBBRect.Top) + L", " +
		                             ConvertToWString(oBBRect.Right) + L' ' + ConvertToWString(oBBRect.Bottom) + L", " + ConvertToWString(oBBRect.Left) + L' ' + ConvertToWString(oBBRect.Bottom) + L", Z M" +
		                             ConvertToWString(oUpdatedClip.Left) + L' ' + ConvertToWString(oUpdatedClip.Top) + L", L" + ConvertToWString(oUpdatedClip.Right) + L' ' + ConvertToWString(oUpdatedClip.Top) + L", " +
		                             ConvertToWString(oUpdatedClip.Right) + L' ' + ConvertToWString(oUpdatedClip.Bottom) + L", " + ConvertToWString(oUpdatedClip.Left) + L' ' + ConvertToWString(oUpdatedClip.Bottom) + L" Z\" clip-rule=\"evenodd\"/>";

		m_oClip.AddClipValue(wsId, wsValue);
	}
	
	void CInterpretatorSvgBase::PathClip(const CPath &oPath, int nClipMode, TXForm *pTransform)
	{
		std::wstring wsPath = CreatePath(oPath, pTransform);

		if (wsPath.empty())
			return;

		const std::wstring wsClipId = L"PATHCLIP_" + ConvertToWString(++m_unNumberDefs, 0);
		const std::wstring wsValue  = L"<path d=\"" + wsPath + L"\"/>";

		m_oClip.AddClipValue(wsClipId, wsValue, nClipMode);
	}

	void CInterpretatorSvgBase::AddStroke(NodeAttributes &arAttributes) const
	{
		if (NULL == m_pParser)
			return;

		const IPen *pPen = m_pParser->GetPen();

		if (NULL == pPen || PS_NULL == pPen->GetStyle())
			return;

		switch (m_pParser->GetRop2Mode())
		{
			case R2_BLACK:   arAttributes.Add(L"stroke", L"rgb(0, 0, 0)"); break;
			case R2_NOP:     return;
			case R2_WHITE:   arAttributes.Add(L"stroke", L"rgb(255, 255, 255)"); break;
			default: arAttributes.Add(L"stroke", CalculateColor(pPen->GetColor(), pPen->GetAlpha())); break;
		}

		unsigned int unMetaPenStyle = pPen->GetStyle();
		unsigned int ulPenStyle     = unMetaPenStyle & PS_STYLE_MASK;
		unsigned int ulPenStartCap  = unMetaPenStyle & PS_STARTCAP_MASK;
		unsigned int ulPenEndCap    = unMetaPenStyle & PS_ENDCAP_MASK;
		unsigned int ulPenJoin      = unMetaPenStyle & PS_JOIN_MASK;

		// svg не поддерживает разные стили для разных сторон линии
		std::wstring wsLineCap, wsLineJoin;

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
		
		if (PS_JOIN_MITER == ulPenJoin)
			wsLineJoin = L"miter";
		else if (PS_JOIN_BEVEL == ulPenJoin)
			wsLineJoin = L"bevel";
		else if (PS_JOIN_ROUND == ulPenJoin)
			wsLineJoin = L"round";

		double dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());

		if (Equals(0., dStrokeWidth))
		{
			dStrokeWidth = 1.;
			arAttributes.Add(L"vector-effect", L"non-scaling-stroke");
		}

		arAttributes.Add(L"stroke-width",      dStrokeWidth);
		arAttributes.Add(L"stroke-miterlimit", pPen->GetMiterLimit());
		arAttributes.Add(L"stroke-linecap",    wsLineCap);
		arAttributes.Add(L"stroke-linejoin",   wsLineJoin);

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

			arAttributes.Add(L"stroke-dasharray", wsDashArray);
		}
		else if (PS_DASH == ulPenStyle)
			arAttributes.Add(L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2));
		else if (PS_DOT == ulPenStyle)
			arAttributes.Add(L"stroke-dasharray", ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth));
		else if (PS_DASHDOT == ulPenStyle)
			arAttributes.Add(L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2));
		else if (PS_DASHDOTDOT == ulPenStyle)
			arAttributes.Add(L"stroke-dasharray", ConvertToWString(dStrokeWidth * 4) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2) + L' ' + ConvertToWString(dStrokeWidth) + L' ' + ConvertToWString(dStrokeWidth * 2));
	}

	void CInterpretatorSvgBase::AddFill(NodeAttributes &arAttributes, double dWidth, double dHeight)
	{
		if (NULL == m_pParser)
		{
			arAttributes.Add(L"fill", L"none");
			return;
		}

		const IBrush *pBrush = NULL;

		if (NULL != m_pParser->GetPen())
		{
			const CEmfPlusPen *pPen = dynamic_cast<const CEmfPlusPen*>(m_pParser->GetPen());

			if (NULL != pPen)
				pBrush = pPen->pBrush;
		}

		if (NULL == pBrush)
			pBrush = m_pParser->GetBrush();

		if (NULL == pBrush || BS_NULL == pBrush->GetStyle())
		{
			arAttributes.Add(L"fill", L"none");
			return;
		}

		switch (pBrush->GetStyle())
		{
			case BS_SOLID:
			{
				arAttributes.Add(L"fill", CalculateColor(pBrush->GetColor(), pBrush->GetAlpha()));
				return;
			}
			case BS_HATCHED:
			{
				const std::wstring wsStyleId = CreateHatchStyle(pBrush->GetHatch(), dWidth, dHeight);

				if (!wsStyleId.empty())
				{
					arAttributes.Add(L"fill", L"url(#" + wsStyleId + L")");
					return;
				}

				break;
			}
			case BS_DIBPATTERN:
			{
				const std::wstring wsStyleId = CreateDibPatternStyle(pBrush);

				if (!wsStyleId.empty())
				{
					arAttributes.Add(L"fill", L"url(#" + wsStyleId + L")");
					return;
				}
			}
			case BS_PATTERN:
			{
				const std::wstring wsStyleId = CreatePatternStyle(pBrush);

				if (!wsStyleId.empty())
				{
					arAttributes.Add(L"fill", L"url(#" + wsStyleId + L")");
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
					arAttributes.Add(L"fill", L"url(#" + wsStyleId + L")");
					return;
				}
			}
			default:
				arAttributes.Add(L"fill", L"none");
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
			wsValue = L"scale(" + ConvertToWString(oOldTransform.M11) + L',' + ConvertToWString(oOldTransform.M22) + L')';
		}
		else if (bTranslate && !bScale)
		{
			wsValue = L"translate(" + ConvertToWString(oOldTransform.Dx) + L',' + ConvertToWString(oOldTransform.Dy) + L')';
		}
		else if (bScale && bTranslate)
		{
			wsValue = L"matrix(" + ConvertToWString(oOldTransform.M11) + L',' +
			                       ConvertToWString(oOldTransform.M12) + L',' +
			                       ConvertToWString(oOldTransform.M21) + L',' +
			                       ConvertToWString(oOldTransform.M22) + L',' +
			                       ConvertToWString(oOldTransform.Dx)  + L',' + ConvertToWString(oOldTransform.Dy) + L')';
		}
		else return;

		if (NULL != pFoundTransform)
			pFoundTransform->second.insert(0, wsValue + L' ');
		else
			arAttributes.Add(L"transform", wsValue);
	}

	void CInterpretatorSvgBase::AddClip()
	{
		if (m_bUpdatedClip || !OpenClip())
			return;

		m_bUpdatedClip = true;
	}

	bool CInterpretatorSvgBase::OpenClip()
	{
		CloseClip();
		
		const CClip *pClip = m_pParser->GetClip();

		if (NULL == pClip || pClip->Empty()) 
			return false;

		pClip->DrawOnRenderer(this);

		WriteNodeBegin(L"g", {{L"clip-path", L"url(#" + m_oClip.GetClipId() + L')'}});
		m_oClip.BeginClip();

		return true;
	}
	
	void CInterpretatorSvgBase::CloseClip()
	{
		if (!m_oClip.StartedClip())
			return;

		WriteNodeEnd(L"g");
		m_oClip.CloseClip();
		m_wsDefs += m_oClip.GetClip();
	}

	void CInterpretatorSvgBase::AddNoneFill(NodeAttributes &arAttributes) const
	{
		arAttributes.Add(L"fill", L"none");
	}

	void CInterpretatorSvgBase::AddShapeRendering(NodeAttributes& arAttributes) const
	{
		switch (m_eShapeRendering)
		{
			case EShapeRendering::OptimizeSpeed:
			{
				arAttributes.Add(L"shape-rendering", L"optimizeSpeed");
				break;
			}
			case EShapeRendering::CrispEdges:
			{
				arAttributes.Add(L"shape-rendering", L"crispEdges");
				break;
			}
			case EShapeRendering::GeometricPrecision:
			{
				arAttributes.Add(L"shape-rendering", L"geometricPrecision");
				break;
			}
			default:
				break;
		}
	}

	TPointD GetFirstPoint(const CPathCommandBase* pPathCommand)
	{
		if (NULL == pPathCommand)
			return {0., 0.};

		switch (pPathCommand->GetType())
		{
			case EPathCommandType::PATH_COMMAND_MOVETO:
			{
				CPathCommandMoveTo* pMoveTo{(CPathCommandMoveTo*)pPathCommand};
				return {pMoveTo->GetX(), pMoveTo->GetY()};
			}
			case EPathCommandType::PATH_COMMAND_LINETO:
			{
				CPathCommandLineTo* pLineTo{(CPathCommandLineTo*)pPathCommand};
				return {pLineTo->GetX(), pLineTo->GetY()};
			}
			default:
				return {0., 0.};
		}
	}

	void CInterpretatorSvgBase::AddLineCaps(NodeAttributes& arAttributes, const CPath* pMainPath)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetPen() || NULL == pMainPath)
			return;

		const IPen *pPen = m_pParser->GetPen();
		const std::vector<CPathCommandBase *> arCommands{pMainPath->GetCommands()};
		const CLineCapData* pStartLineCap = dynamic_cast<const CLineCapData*>(pPen->GetStartLineCap());

		if (NULL != pStartLineCap)
		{
			double dAngle = 0.;

			if (arCommands.size() > 1)
			{
				const TPointD oFirstPoint {GetFirstPoint(arCommands[0])};
				const TPointD oSecondPoint{GetFirstPoint(arCommands[1])};

				dAngle = (atan2((oSecondPoint.Y - oFirstPoint.Y), (oSecondPoint.X - oFirstPoint.X))) / M_PI * 180. + 90.;
			}

			const std::wstring wsStartLineCapId{CreateLineCap(pStartLineCap, dAngle)};

			if (!wsStartLineCapId.empty())
				arAttributes.Add(L"marker-start", L"url(#" + wsStartLineCapId + L')');
		}

		const CLineCapData* pEndLineCap = dynamic_cast<const CLineCapData*>(pPen->GetEndLineCap());

		if (NULL != pEndLineCap)
		{
			double dAngle = 0.;

			if (arCommands.size() > 1)
			{
				const TPointD oFirstPoint {GetFirstPoint(arCommands[arCommands.size() - 1])};
				const TPointD oSecondPoint{GetFirstPoint(arCommands[arCommands.size() - 2])};

				dAngle = (atan2((oSecondPoint.Y - oFirstPoint.Y), (oSecondPoint.X - oFirstPoint.X))) / M_PI * 180. + 90.;
			}

			const std::wstring wsEndLineCapId{CreateLineCap(pEndLineCap, dAngle)};

			if (!wsEndLineCapId.empty())
				arAttributes.Add(L"marker-end", L"url(#" + wsEndLineCapId + L')');
		}
	}

	TPointD CInterpretatorSvgBase::GetCutPos() const
	{
		if (NULL != m_pParser)
			return m_pParser->GetCurPos();

		return TPointD(m_oViewport.dLeft, m_oViewport.dRight);
	}

	std::wstring CInterpretatorSvgBase::CreatePath(const CPath& oPath, const TXForm *pTransform) const
	{
		if (NULL == m_pParser || oPath.Empty())
			return std::wstring();

		std::wstring wsValue, wsMoveValue;
		BYTE oLastType = 0x00;

		TXForm oTransform;

		if (NULL != pTransform)
			oTransform.Copy(pTransform);

		for (const CPathCommandBase* pCommand : oPath.GetCommands())
		{
			if (PATH_COMMAND_MOVETO != pCommand->GetType() && !wsMoveValue.empty())
			{
				wsValue += wsMoveValue;
				wsMoveValue.clear();
			}
			switch ((unsigned int)pCommand->GetType())
			{
				case PATH_COMMAND_MOVETO:
				{
					CPathCommandMoveTo* pMoveTo = (CPathCommandMoveTo*)pCommand;

					TPointD oPoint(pMoveTo->GetX(), pMoveTo->GetY());
					oTransform.Apply(oPoint.X, oPoint.Y);

					wsMoveValue = L"M " + ConvertToWString(oPoint.X) + L',' +  ConvertToWString(oPoint.Y) + L' ';

					oLastType = PATH_COMMAND_MOVETO;

					break;
				}
				case PATH_COMMAND_LINETO:
				{
					CPathCommandLineTo* pLineTo = (CPathCommandLineTo*)pCommand;

					TPointD oPoint(pLineTo->GetX(), pLineTo->GetY());
					oTransform.Apply(oPoint.X, oPoint.Y);

					if (PATH_COMMAND_LINETO != oLastType)
					{
						oLastType = PATH_COMMAND_LINETO;
						wsValue += L"L ";
					}

					wsValue += ConvertToWString(oPoint.X) + L',' +  ConvertToWString(oPoint.Y) + L' ';

					break;
				}
				case PATH_COMMAND_CURVETO:
				{
					CPathCommandCurveTo* pCurveTo = (CPathCommandCurveTo*)pCommand;

					if (PATH_COMMAND_CURVETO != oLastType)
					{
						oLastType = PATH_COMMAND_CURVETO;
						wsValue += L"C ";
					}

					TPointD oPoint1(pCurveTo->GetX1(), pCurveTo->GetY1()), oPoint2(pCurveTo->GetX2(), pCurveTo->GetY2()), oPointE(pCurveTo->GetXE(), pCurveTo->GetYE());
					oTransform.Apply(oPoint1.X, oPoint1.Y);
					oTransform.Apply(oPoint2.X, oPoint2.Y);
					oTransform.Apply(oPointE.X, oPointE.Y);

					wsValue += ConvertToWString(oPoint1.X) + L',' + ConvertToWString(oPoint1.Y)  + L' ' +
					           ConvertToWString(oPoint2.X) + L',' + ConvertToWString(oPoint2.Y) + L' ' +
					           ConvertToWString(oPointE.X) + L',' + ConvertToWString(oPointE.Y) + L' ';

					break;
				}
				case PATH_COMMAND_ARCTO:
				{
					CPathCommandArcTo* pArcTo = (CPathCommandArcTo*)pCommand;

					TPointD oPoint1(pArcTo->GetLeft(), pArcTo->GetTop()), oPoint2(pArcTo->GetRight(), pArcTo->GetBottom());

					oTransform.Apply(oPoint1.X, oPoint1.Y);
					oTransform.Apply(oPoint2.X, oPoint2.Y);

					double dXRadius = std::fabs(oPoint2.X - oPoint1.X) / 2;
					double dYRadius = std::fabs(oPoint2.Y - oPoint1.X) / 2;

					double dEndX = (oPoint2.X + oPoint1.X) / 2 + dXRadius  * cos((pArcTo->GetSweepAngle()) * M_PI / 180);
					double dEndY = (oPoint2.Y + oPoint1.X) / 2 + dYRadius  * sin((pArcTo->GetSweepAngle()) * M_PI / 180);

					wsValue += L"A " + ConvertToWString(dXRadius) + L' ' +
					           ConvertToWString(dYRadius) + L' ' +
					           L"0 " +
					           ((std::fabs(pArcTo->GetSweepAngle() - pArcTo->GetStartAngle()) <= 180) ? L"0" : L"1") + L' ' +
					           ((std::fabs(pArcTo->GetSweepAngle() - pArcTo->GetStartAngle()) <= 180) ? L"1" : L"0") + L' ' +
					           ConvertToWString(dEndX) + L' ' +
					           ConvertToWString(dEndY) + L' ';

					oLastType = PATH_COMMAND_ARCTO;

					break;
				}
				case PATH_COMMAND_CLOSE:
				{
					wsValue += L"Z ";
					oLastType = PATH_COMMAND_CLOSE;
					break;
				}
			}
		}

		if (!wsValue.empty() && wsValue[0] != L'M')
			wsValue.insert(0, L"M " + ConvertToWString(m_pParser->GetCurPos().X) + L',' + ConvertToWString(m_pParser->GetCurPos().Y) + L' ');

		return wsValue;
	}

	std::wstring CInterpretatorSvgBase::CreateHatchStyle(unsigned int unHatchStyle, double dWidth, double dHeight)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetBrush())
			return std::wstring();

		double dStrokeWidth  = 1. / m_pParser->GetTransform().M11;

		if (NULL != m_pParser->GetPen())
		{
			dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());
	
			if (Equals(0, dStrokeWidth) || PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK))
				dStrokeWidth = 1;
		}

		std::wstring wsStrokeColor = CalculateColor(m_pParser->GetBrush()->GetColor(), m_pParser->GetBrush()->GetAlpha());

		CHatchGenerator oHatchGenerator;

		oHatchGenerator.SetSize(dWidth, dHeight);
		oHatchGenerator.SetStyleId(unHatchStyle, ++m_unNumberDefs);
		oHatchGenerator.SetStroke(dStrokeWidth, m_pParser->GetBrush()->GetColor(), m_pParser->GetBrush()->GetAlpha());

		if (TRANSPARENT != m_pParser->GetTextBgMode())
			oHatchGenerator.SetBackground(m_pParser->GetTextBgColor());

		if (oHatchGenerator.GenerateHatch())
		{
			m_wsDefs += oHatchGenerator.GetPattern();

			return oHatchGenerator.GetPatternId();
		}

		--m_unNumberDefs;
		return std::wstring();
	}

	std::wstring CInterpretatorSvgBase::CreateDibPatternStyle(const IBrush *pBrush)
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

		double dStrokeWidth  = 1. / m_pParser->GetTransform().M11;

		if (NULL != m_pParser->GetPen())
		{
			dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());
	
			if (Equals(0, dStrokeWidth) || PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK))
				dStrokeWidth = 1;
		}

		std::wstring wsWidth  = ConvertToWString(dStrokeWidth * unWidth);
		std::wstring wsHeight = ConvertToWString(dStrokeWidth * unHeight);

		m_wsDefs += L"<pattern id=\"" + wsStyleId + L"\" " +
		            L"width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\" patternUnits=\"userSpaceOnUse\">" +
		            L"<image xlink:href=\"data:image/png;base64," + wsImageDataW + L"\" x=\"0\" y=\"0\" width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\"/>" +
		            L"</pattern> ";

		delete [] pImageData;

		return wsStyleId;
	}

	std::wstring CInterpretatorSvgBase::CreatePatternStyle(const IBrush *pBrush)
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

	std::wstring CInterpretatorSvgBase::CreateGradient(const IBrush* pBrush)
	{
		if (pBrush == NULL)
			return std::wstring();

		std::wstring wsStyleId;

		if (BS_LINEARGRADIENT == pBrush->GetStyle() ||
		    BS_RECTGRADIENT   == pBrush->GetStyle() ||
		    BS_PATHGRADIENT   == pBrush->GetStyle())
		{
			wsStyleId = L"LINEARGRADIENT_" + ConvertToWString(++m_unNumberDefs, 0);

			m_wsDefs += L"<linearGradient id=\"" + wsStyleId + L"\">";

			std::vector<long> arColors;
			std::vector<double> arPositions;
			pBrush->GetGradientColors(arColors, arPositions);

			for (unsigned int unIndex = 0; unIndex < arColors.size(); ++unIndex)
				m_wsDefs += L"<stop offset=\"" + ConvertToWString(arPositions[unIndex], 2) +
				            L"\" stop-color=\"" + CalculateColor(arColors[unIndex]) +  L"\"/>";

			m_wsDefs += L"</linearGradient>";

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
			            L"<stop offset=\"0%\" stop-color=\""   + CalculateColor(pBrush->GetColor(),  pBrush->GetAlpha() ) + L"\"/>" +
			            L"<stop offset=\"100%\" stop-color=\"" + CalculateColor(pBrush->GetColor2(), pBrush->GetAlpha2()) + L"\"/>" +
			            L"</radialGradient>";

			return wsStyleId;
		}

		return std::wstring();
	}

	std::wstring CInterpretatorSvgBase::CreateLineCap(const CLineCapData* pLineCap, const double& dAngle)
	{
		if (NULL == pLineCap)
			return std::wstring();

		CustomLineCapDataType enLineCapDataType = pLineCap->GetType();

		if (CustomLineCapDataType::CustomLineCapDataTypeDefault == enLineCapDataType)
		{
			const TEmfPlusCustomLineCapData* pCustomLineCap = dynamic_cast<const TEmfPlusCustomLineCapData*>(pLineCap);

			if (NULL == pCustomLineCap)
				return std::wstring();

			const std::wstring wsPath = CreatePath(*pCustomLineCap->pPath);

			if (wsPath.empty())
				return std::wstring();

			const std::wstring wsMarkerId {L"CUSTOM_MARKER_" + std::to_wstring(++m_unNumberDefs)};
			const TRectD       oPathBounds{pCustomLineCap->pPath->GetBounds()};
			const double       dWidth     {std::abs(oPathBounds.Right  - oPathBounds.Left)};
			const double       dHeight    {std::abs(oPathBounds.Bottom - oPathBounds.Top)};
			const std::wstring wsWidth    {ConvertToWString(dWidth)};
			const std::wstring wsHeight   {ConvertToWString(dHeight)};
			const std::wstring wsViewBox  {L"viewBox=\"" + ConvertToWString(oPathBounds.Left) + L' ' + ConvertToWString(oPathBounds.Top) +
			                               L' ' + wsWidth + L' ' + wsHeight + L'\"'};

			const double dRefY = -dHeight * (std::min)(3. / dWidth, 3. / dHeight);

			m_wsDefs += L"<marker id=\"" + wsMarkerId + L"\" " + wsViewBox + L" markerWidth=\"" +
			            wsWidth + L"\" markerHeight=\"" + wsHeight + L"\" refY=\"" + ConvertToWString(dRefY) +
			            L"\" orient=\"" + std::to_wstring(dAngle) + L"\"><path d=\"" + wsPath + L"\"/></marker>";

			return wsMarkerId;
		}

		return std::wstring();
	}

	CHatchGenerator::CHatchGenerator()
		: m_nHatchStyle(-1), m_unNumber(0), m_chStrokeAlpha(255), m_nBackgroundColor(-1), m_chBackgroundAlpha(255)
	{}

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
		m_dStrokeWidth  = dWidth;
		m_nStrokeColor  = nColor;
		m_chStrokeAlpha = chAlpha;
	}

	void CHatchGenerator::SetBackground(int nColor, unsigned char chAlpha)
	{
		m_nBackgroundColor  = nColor;
		m_chBackgroundAlpha = chAlpha;
	}

	bool CHatchGenerator::GenerateHatch()
	{
		if (-1 == m_nHatchStyle)
			return false;

		if (!GenerateStartPattern())
			return false;

		GenerateBackground();

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

	std::wstring CHatchGenerator::GetPatternId() const
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
		if (0 == m_chStrokeAlpha)
			return;

		m_oStringBuilder.WriteNodeBegin(L"line", true);
		m_oStringBuilder.WriteAttribute(L"x1", ConvertToWString(m_dStrokeWidth * oPoint1.X));
		m_oStringBuilder.WriteAttribute(L"y1", ConvertToWString(m_dStrokeWidth * oPoint1.Y));
		m_oStringBuilder.WriteAttribute(L"x2", ConvertToWString(m_dStrokeWidth * oPoint2.X));
		m_oStringBuilder.WriteAttribute(L"y2", ConvertToWString(m_dStrokeWidth * oPoint2.Y));
		m_oStringBuilder.WriteAttribute(L"stroke", CalculateColor(m_nStrokeColor, m_chStrokeAlpha));
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
		if (0 == m_chStrokeAlpha)
			return;

		m_oStringBuilder.WriteNodeBegin(L"line", true);
		m_oStringBuilder.WriteAttribute(L"x1", ConvertToWString(m_dStrokeWidth * oPoint.X));
		m_oStringBuilder.WriteAttribute(L"y1", ConvertToWString(m_dStrokeWidth * oPoint.Y));
		m_oStringBuilder.WriteAttribute(L"x2", ConvertToWString(m_dStrokeWidth * (oPoint.X + 1)));
		m_oStringBuilder.WriteAttribute(L"y2", ConvertToWString(m_dStrokeWidth * oPoint.Y));
		m_oStringBuilder.WriteAttribute(L"stroke", CalculateColor(m_nStrokeColor, m_chStrokeAlpha));
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

	void CHatchGenerator::GenerateBackground()
	{
		if (0 > m_nBackgroundColor || 0 == m_chBackgroundAlpha)
			return;

		m_oStringBuilder.WriteNodeBegin(L"rect", true);
		m_oStringBuilder.WriteAttribute(L"x", L"0");
		m_oStringBuilder.WriteAttribute(L"y", L"0");
		m_oStringBuilder.WriteAttribute(L"width",  ConvertToWString(m_dStrokeWidth * 8));
		m_oStringBuilder.WriteAttribute(L"height", ConvertToWString(m_dStrokeWidth * 8));
		m_oStringBuilder.WriteAttribute(L"fill", CalculateColor(m_nBackgroundColor, m_chBackgroundAlpha));
		m_oStringBuilder.WriteNodeEnd(L"rect", true, true);
	}

	void CHatchGenerator::GenerateEndPattern()
	{
		m_oStringBuilder.WriteNodeEnd(L"pattern");
	}

	CSvgClip::CSvgClip()
		: m_bStartClip(false)
	{}

	void CSvgClip::Reset()
	{
		m_arValues.clear();
	}

	void CSvgClip::BeginClip()
	{
		m_bStartClip = true;
	}

	void CSvgClip::CloseClip()
	{
		m_bStartClip = false;
	}

	bool CSvgClip::StartedClip() const
	{
		return m_bStartClip;
	}

	bool CSvgClip::Empty() const
	{
		return m_arValues.empty();
	}

	void CSvgClip::AddClipValue(const std::wstring &wsId, const std::wstring &wsValue, int nClipMode)
	{
		if (RGN_COPY == nClipMode)
			m_arValues.clear();

		m_arValues.push_back({wsId, wsValue, nClipMode});
	}

	std::wstring CSvgClip::GetClip() const
	{
		if (m_arValues.empty())
			return std::wstring();

		std::wstring wsClip;

		for (const TClipValue& oClipValue : m_arValues)
		{
			switch (oClipValue.m_nClipMode)
			{
			case RGN_AND:
			{
				size_t unPosition = wsClip.find(L">", wsClip.rfind(L"<clipPath"));

				if (std::wstring::npos != unPosition)
					wsClip.insert(unPosition, L" clip-path=\"url(#" + oClipValue.m_wsId + L")\"");

				wsClip += L"<clipPath id=\"" + oClipValue.m_wsId + L"\">" + oClipValue.m_wsValue + L"</clipPath>";
				break;
			}
			case RGN_OR:
			{
				if (wsClip.empty())
					wsClip += L"<clipPath id=\"" + oClipValue.m_wsId + L"\">" + oClipValue.m_wsValue + L"</clipPath>";
				else
					wsClip.insert(wsClip.length() - 11, oClipValue.m_wsValue);
				break;
			}
			case RGN_XOR:
			{
				size_t unPosition = wsClip.find(L">", wsClip.rfind(L"<clipPath"));

				if (std::wstring::npos != unPosition)
					wsClip.insert(unPosition, L" clip-rule=\"evenodd\"");

				if (wsClip.empty())
					wsClip += L"<clipPath id=\"" + oClipValue.m_wsId + L"\">" + oClipValue.m_wsValue + L"</clipPath>";
				else
					wsClip.insert(wsClip.length() - 11, oClipValue.m_wsValue);
				break;
			}
			case RGN_COPY:
			{
				wsClip = L"<clipPath id=\"" + oClipValue.m_wsId + L"\">" + oClipValue.m_wsValue + L"</clipPath>";
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

		return m_arValues.front().m_wsId;
	}

	std::wstring CalculateColor(unsigned int unColor, BYTE uchAlpha)
	{
		if (0 == uchAlpha)
			return L"none";

		if (255 == uchAlpha)
			return L"rgb(" + INTCOLOR_TO_RGB(unColor) + L')';

		return L"rgba(" + INTCOLOR_TO_RGB(unColor) + L", " + ConvertToWString((double)uchAlpha / 255., 3) + L')';
	}

	std::wstring CalculateColor(BYTE uchRed, BYTE uchGreen, BYTE uchBlue, BYTE uchAlpha)
	{
		if (0 == uchAlpha)
			return L"none";

		if (255 == uchAlpha)
			return L"rgb(" + std::to_wstring(uchRed) + L", " + std::to_wstring(uchGreen) + L", " + std::to_wstring(uchBlue) + L')';

		return L"rgba(" + std::to_wstring(uchRed) + L", " + std::to_wstring(uchGreen) + L", " + std::to_wstring(uchBlue) + L", " + ConvertToWString((double)uchAlpha / 255., 3) + L')';
	}

	std::wstring CalculateColor(unsigned int unColor)
	{
		BYTE chAlpha = unColor >> 24;

		return CalculateColor(unColor, chAlpha);
	}

	void NormalizeRect(TRectD& oRect)
	{
		if (oRect.Right < oRect.Left)
			std::swap(oRect.Right, oRect.Left);

		if (oRect.Bottom < oRect.Top)
			std::swap(oRect.Bottom, oRect.Top);
	}

	TRectD TranslateRect(const TRectL& oRect)
	{
		TRectD oNewRect(oRect.Left, oRect.Top, oRect.Right, oRect.Bottom);

		NormalizeRect(oNewRect);

		return oNewRect;
	}
}
