#include "CWmfInterpretatorSvg.h"

#include "../../../../graphics/Image.h"

#include "../../../BgraFrame.h"

#include <cmath>

namespace MetaFile
{
	CWmfInterpretatorSvg::CWmfInterpretatorSvg(CWmfParserBase *pParser, unsigned int unWidth, unsigned int unHeight)
	    : CInterpretatorSvgBase(pParser, unWidth, unHeight)
	{}

	CWmfInterpretatorSvg::~CWmfInterpretatorSvg()
	{

	}

	InterpretatorType CWmfInterpretatorSvg::GetType() const
	{
		return InterpretatorType::Svg;
	}

	void CWmfInterpretatorSvg::HANDLE_META_HEADER(const TWmfPlaceable& oPlaceable, const TWmfHeader& oHeader)
	{
		m_pXmlWriter->WriteNodeBegin(L"svg", true);
		m_pXmlWriter->WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
		m_pXmlWriter->WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");

		const TRectL& oBounds{m_pParser->GetDCBounds()};

		m_oViewport.dLeft   = std::min(oBounds.Left, oBounds.Right );
		m_oViewport.dTop    = std::min(oBounds.Top,  oBounds.Bottom);
		m_oViewport.dRight  = std::max(oBounds.Left, oBounds.Right );
		m_oViewport.dBottom = std::max(oBounds.Top,  oBounds.Bottom);

		UpdateSize();

		double dXScale = 1., dYScale = 1.;

		if (0 != m_oSizeWindow.X)
			dXScale = m_oSizeWindow.X / m_oViewport.GetWidth();

		if (0 != m_oSizeWindow.Y)
			dYScale = m_oSizeWindow.Y / m_oViewport.GetHeight();

		if (m_oViewport.GetWidth() != 0)
			m_pXmlWriter->WriteAttribute(L"width", ConvertToWString(m_oViewport.GetWidth() * dXScale));

		if (m_oViewport.GetHeight() != 0)
			m_pXmlWriter->WriteAttribute(L"height", ConvertToWString(m_oViewport.GetHeight() * dYScale));

		m_pXmlWriter->WriteNodeEnd(L"svg", true, false);

		if (!Equals(1., dXScale) || !Equals(1., dYScale))
		{
			m_pXmlWriter->WriteNodeBegin(L"g", true);

			m_pXmlWriter->WriteAttribute(L"transform", L"scale(" + ConvertToWString(dXScale) + L',' + ConvertToWString(dYScale) + L')');

			m_pXmlWriter->WriteNodeEnd(L"g", true, false);
		}
	}

	void CWmfInterpretatorSvg::HANDLE_META_EOF()
	{
		CloseClip();

		if (!m_wsDefs.empty())
			m_pXmlWriter->WriteString(L"<defs>" + m_wsDefs + L"</defs>");

		if (!Equals(m_oSizeWindow.X, m_oViewport.GetWidth()) || !Equals(m_oSizeWindow.Y, m_oViewport.GetHeight()))
			m_pXmlWriter->WriteNodeEnd(L"g", false, false);

		m_pXmlWriter->WriteNodeEnd(L"svg", false, false);
	}

	void CWmfInterpretatorSvg::HANDLE_META_ARC(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
	{
		TRectD oNewRect;

		oNewRect.Left   = shLeft;
		oNewRect.Top    = shTop;
		oNewRect.Right  = shRight;
		oNewRect.Bottom = shBottom;

		double dXRadius = std::fabs(oNewRect.Right - oNewRect.Left) / 2;
		double dYRadius = std::fabs(oNewRect.Bottom - oNewRect.Top) / 2;

		double dStartAngle = GetEllipseAngle(oNewRect.Left, oNewRect.Top, oNewRect.Right, oNewRect.Bottom, shXStartArc, shYStartArc) / 180. * M_PI;
		double dEndAngle   = GetEllipseAngle(oNewRect.Left, oNewRect.Top, oNewRect.Right, oNewRect.Bottom, shXEndArc, shYEndArc) / 180. * M_PI;

		double dX1 = std::cos(dStartAngle) * dXRadius;
		double dY1 = std::sin(dStartAngle) * dXRadius;
		double dX2 = std::cos(dEndAngle) * dXRadius;
		double dY2 = std::sin(dEndAngle) * dYRadius;

		std::wstring wsValue = L"M " + ConvertToWString(dX1 + (shRight + shLeft) / 2.) + L' ' + ConvertToWString(dY1 + (shBottom + shTop) / 2.);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
		           ConvertToWString(dYRadius) + L' ' +
		           L"0 0 0 " +
		           //                                    ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
		           //                                    ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
		           ConvertToWString(dX2 + (shRight + shLeft) / 2.) + L' ' +
		           ConvertToWString(dY2 + (shBottom + shTop) / 2.);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path" , arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_CHORD(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_ELLIPSE(short shBottom, short shRight, short shTop, short shLeft)
	{
		TRectD oNewRect;

		oNewRect.Left   = shLeft;
		oNewRect.Top    = shTop;
		oNewRect.Right  = shRight;
		oNewRect.Bottom = shBottom;

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.Left   + oNewRect.Right)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.Top    + oNewRect.Bottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.Right  - oNewRect.Left)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.Bottom - oNewRect.Top)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"ellipse", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_EXTTEXTOUT(short shY, short shX, short shStringLength, unsigned short ushFwOptions, const TRectL &oRectangle, unsigned char *pString, short *pDx)
	{
		const IFont* pFont = NULL;

		if (NULL != m_pParser)
			pFont = m_pParser->GetFont();

		const std::wstring wsText{ConvertToUnicode(pString, (long)shStringLength, (NULL != pFont) ? pFont->GetCharSet() : DEFAULT_CHARSET)};

		TPointD oScale(1, 1);

		if (NULL != m_pParser)
		{
			oScale.X = m_pParser->IsWindowFlippedX() ? -1 : 1;
			oScale.Y = m_pParser->IsWindowFlippedY() ? -1 : 1;
		}

		std::vector<double> arDx(0);

		if (NULL != pDx && shStringLength == wsText.length())
			arDx = std::vector<double>(pDx, pDx + wsText.length());

		WriteText(wsText, TPointD(shX, shY), oRectangle, oScale, arDx);
	}

	void CWmfInterpretatorSvg::HANDLE_META_FILLREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex)
	{
		const CWmfRegion *pRegion = dynamic_cast<const CWmfRegion*>(m_pParser->GetRegion());

		if (NULL == pRegion)
			return;

		std::wstring wsValue;

		for (unsigned int unScanIndex = 0; unScanIndex < pRegion->shScanCount; ++unScanIndex)
		{
			TWmfScanObject *pScanObject = &pRegion->pScans[unScanIndex];

			if (pScanObject->ushCount == 0) continue;

			for (unsigned int unIndex = 0; unIndex < pScanObject->ushCount >> 1; ++unIndex)
			{
				wsValue += L"M " + ConvertToWString(std::max((short)pScanObject->pScanLines[unIndex].ushLeft, (short)pRegion->oBoundingRectangle.Left)) + L',' +  ConvertToWString(std::max((short)pScanObject->ushTop, (short)pRegion->oBoundingRectangle.Top)) + L' ' +
				           ConvertToWString(std::min((short)pScanObject->pScanLines[unIndex].ushRight, (short)pRegion->oBoundingRectangle.Right)) + L',' + ConvertToWString(std::max((short)pScanObject->ushTop, (short)pRegion->oBoundingRectangle.Top)) + L' ' +
				           ConvertToWString(std::min((short)pScanObject->pScanLines[unIndex].ushRight, (short)pRegion->oBoundingRectangle.Right)) + L',' + ConvertToWString(std::min((short)pScanObject->ushBottom, (short)pRegion->oBoundingRectangle.Bottom)) + L' ' +
				           ConvertToWString(std::max((short)pScanObject->pScanLines[unIndex].ushLeft, (short)pRegion->oBoundingRectangle.Left)) + L',' + ConvertToWString(std::min((short)pScanObject->ushBottom, (short)pRegion->oBoundingRectangle.Bottom)) + L' ' +
				           ConvertToWString(std::max((short)pScanObject->pScanLines[unIndex].ushLeft, (short)pRegion->oBoundingRectangle.Left)) + L',' + ConvertToWString(std::max((short)pScanObject->ushTop, (short)pRegion->oBoundingRectangle.Top)) + L' ';
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_FRAMEREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex, short shHeight, short shWidth)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_INVERTREGION(unsigned short ushRegionIndex)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_LINETO(short shY, short shX)
	{
		TPointD oCurPos = GetCutPos();

		NodeAttributes arAttributes = {{L"x1", ConvertToWString(oCurPos.X)},
		                               {L"y1", ConvertToWString(oCurPos.Y)},
		                               {L"x2", ConvertToWString(shX)},
		                               {L"y2", ConvertToWString(shY)}};

		AddStroke(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"line", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_PAINTREGION(unsigned short ushRegionIndex)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_PATBLT(unsigned int unRasterOperation, short shH, short shW, short shY, short shX)
	{
		TRectD oNewRect;

		NodeAttributes arAttributes = {{L"x",      ConvertToWString(shX)},
		                               {L"y",      ConvertToWString(shY)},
		                               {L"width",  ConvertToWString(shW)},
		                               {L"height", ConvertToWString(shH)}};

		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"rect", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_PIE(short shXRadial1, short shYRadial1, short shXRadial2, short shYRadial2, short shB, short shR, short shT, short shL)
	{
		short shCenterX = (shL + shR) / 2;
		short shCenterY = (shT + shB) / 2;

		short shRadiusX = (shR - shL) / 2;
		short shRadiusY = (shB - shT) / 2;

		std::wstring wsPath = L'M' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L' ' +
		                      L'L' + ConvertToWString(shXRadial1)+ L' ' + ConvertToWString(shYRadial1)+ L' ' +
		                      L'A' + ConvertToWString(shRadiusX) + L' ' + ConvertToWString(shRadiusY) + L" 0, 0, 0, " + ConvertToWString(shXRadial2) + L' ' + ConvertToWString(shYRadial2) + L' ' +
		                      L'L' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L" Z";


		NodeAttributes arAttributes = {{L"d", wsPath}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_POLYLINE(const std::vector<TPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polyline", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_POLYGON(const std::vector<TPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polygon", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TPointS>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].X) + L',' +  ConvertToWString(oPolygon[0].Y) + L' ';

			for (const TPointS& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].X) + L',' +  ConvertToWString(oPolygon[0].Y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		arAttributes.Add(L"fill-rule", L"evenodd");

		WriteNode(L"path", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_RECTANGLE(short shB, short shR, short shT, short shL)
	{
		TRectD oNewRect = TranslateRect({shL, shT, shR, shB});

		NodeAttributes arAttributes = {{L"x",      ConvertToWString(oNewRect.Left)},
		                               {L"y",      ConvertToWString(oNewRect.Top)},
		                               {L"width",  ConvertToWString(oNewRect.Right - oNewRect.Left)},
		                               {L"height", ConvertToWString(oNewRect.Bottom - oNewRect.Top)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"rect", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_ROUNDRECT(short shH, short shW, short shB, short shR, short shT, short shL)
	{
		TRectD oNewRect = TranslateRect({shL, shT, shR, shB});

		NodeAttributes arAttributes = {{L"x",      ConvertToWString(oNewRect.Left)},
		                               {L"y",      ConvertToWString(oNewRect.Top)},
		                               {L"width",  ConvertToWString(oNewRect.Right - oNewRect.Left)},
		                               {L"height", ConvertToWString(oNewRect.Bottom - oNewRect.Top)},
		                               {L"rx",     ConvertToWString((double)shW / 2.)},
		                               {L"ry",     ConvertToWString((double)shH / 2.)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddShapeRendering(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"rect", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_SETPIXEL(const TRGBA &oColor, short shY, short shX)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_TEXTOUT(short shStringLength, unsigned char *pString, short shX, short shY)
	{
		const IFont* pFont = NULL;

		if (NULL != m_pParser)
			pFont = m_pParser->GetFont();

		NSStringExt::CConverter::ESingleByteEncoding eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT;;
		if (pFont)
		{
			// Соответствие Charset -> Codepage: http://support.microsoft.com/kb/165478
			// http://msdn.microsoft.com/en-us/library/cc194829.aspx
			//  Charset Name       Charset Value(hex)  Codepage number
			//  ------------------------------------------------------
			//
			//  DEFAULT_CHARSET           1 (x01)
			//  SYMBOL_CHARSET            2 (x02)
			//  OEM_CHARSET             255 (xFF)
			//  ANSI_CHARSET              0 (x00)            1252
			//  RUSSIAN_CHARSET         204 (xCC)            1251
			//  EASTEUROPE_CHARSET      238 (xEE)            1250
			//  GREEK_CHARSET           161 (xA1)            1253
			//  TURKISH_CHARSET         162 (xA2)            1254
			//  BALTIC_CHARSET          186 (xBA)            1257
			//  HEBREW_CHARSET          177 (xB1)            1255
			//  ARABIC _CHARSET         178 (xB2)            1256
			//  SHIFTJIS_CHARSET        128 (x80)             932
			//  HANGEUL_CHARSET         129 (x81)             949
			//  GB2313_CHARSET          134 (x86)             936
			//  CHINESEBIG5_CHARSET     136 (x88)             950
			//  THAI_CHARSET            222 (xDE)             874
			//  JOHAB_CHARSET	        130 (x82)            1361
			//  VIETNAMESE_CHARSET      163 (xA3)            1258

			switch (pFont->GetCharSet())
			{
			default:
			case DEFAULT_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT; break;
			case SYMBOL_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT; break;
			case ANSI_CHARSET:          eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1252; break;
			case RUSSIAN_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1251; break;
			case EASTEUROPE_CHARSET:    eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1250; break;
			case GREEK_CHARSET:         eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1253; break;
			case TURKISH_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1254; break;
			case BALTIC_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1257; break;
			case HEBREW_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1255; break;
			case ARABIC_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1256; break;
			case SHIFTJIS_CHARSET:      eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP932; break;
			case HANGEUL_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP949; break;
			case 134/*GB2313_CHARSET*/: eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP936; break;
			case CHINESEBIG5_CHARSET:   eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP950; break;
			case THAI_CHARSET:          eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP874; break;
			case JOHAB_CHARSET:         eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1361; break;
			case VIETNAMESE_CHARSET:    eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1258; break;
			}
		}

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromSingleByteString((const unsigned char*)pString, (long)shStringLength, eCharSet);

		WriteText(wsText, TPointD(shX, shY));
	}

	void CWmfInterpretatorSvg::HANDLE_META_EXCLUDECLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
	{
		m_bUpdatedClip = false;
	}

	void CWmfInterpretatorSvg::HANDLE_META_INTERSECTCLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
	{
		m_bUpdatedClip = false;
	}

	void CWmfInterpretatorSvg::HANDLE_META_RESTOREDC()
	{
		m_bUpdatedClip = false;
	}

	void CWmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		CInterpretatorSvgBase::DrawBitmap(dX, dY, dW, dH, pBuffer, unWidth, unHeight);
	}

	void CWmfInterpretatorSvg::ResetClip()
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CWmfInterpretatorSvg::IntersectClip(const TRectD &oClip)
	{
		CInterpretatorSvgBase::IntersectClip(oClip);
	}

	void CWmfInterpretatorSvg::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		CInterpretatorSvgBase::ExcludeClip(oClip, oBB);
	}
}
