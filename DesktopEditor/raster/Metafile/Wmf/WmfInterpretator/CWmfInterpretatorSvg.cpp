#include "CWmfInterpretatorSvg.h"

#include "../../../../graphics/Image.h"

#include "../../../BgraFrame.h"

#include <cmath>

namespace MetaFile
{
	CWmfInterpretatorSvg::CWmfInterpretatorSvg(CWmfParserBase *pParser, unsigned int unWidth, unsigned int unHeight)
	    : m_pParser(pParser), m_unNumberDefs(0), m_dScale(1)
	{
		SetSize(unWidth, unHeight);
	}

	CWmfInterpretatorSvg::~CWmfInterpretatorSvg()
	{

	}

	InterpretatorType CWmfInterpretatorSvg::GetType() const
	{
		return InterpretatorType::Svg;
	}

	void CWmfInterpretatorSvg::SetSize(unsigned int unWidth, unsigned int unHeight)
	{
		m_oSizeWindow.x = unWidth;
		m_oSizeWindow.y = unHeight;
	}

	void CWmfInterpretatorSvg::GetSize(unsigned int &unWidth, unsigned int &unHeight)
	{
		unWidth  = m_oSizeWindow.x;
		unHeight = m_oSizeWindow.y;
	}

	void CWmfInterpretatorSvg::UpdateSize()
	{
		if (0 != m_oSizeWindow.x && 0 == m_oSizeWindow.y)
			m_oSizeWindow.y = m_oSizeWindow.x * (m_oViewport.GetHeight() / m_oViewport.GetWidth());
		else if (0 == m_oSizeWindow.x && 0 != m_oSizeWindow.y)
		    m_oSizeWindow.x = m_oSizeWindow.y * (m_oViewport.GetWidth() / m_oViewport.GetHeight());
	}

	void CWmfInterpretatorSvg::HANDLE_META_HEADER(const TWmfPlaceable& oPlaceable, const TWmfHeader& oHeader)
	{
		m_oXmlWriter.WriteNodeBegin(L"svg", true);
		m_oXmlWriter.WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
		m_oXmlWriter.WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");

		m_dScale = 1;

		if (oPlaceable.Inch != 0)
			m_dScale = 96.f / oPlaceable.Inch;

		TRect *pBounds = m_pParser->GetDCBounds();

		m_oViewport.dLeft	= pBounds->nLeft;
		m_oViewport.dTop	= pBounds->nTop;
		m_oViewport.dRight	= pBounds->nRight;
		m_oViewport.dBottom = pBounds->nBottom;

		UpdateSize();

		if (m_oViewport.GetWidth() != 0)
			m_oXmlWriter.WriteAttribute(L"width", ConvertToWString(m_oViewport.GetWidth()));

		if (m_oViewport.GetHeight() != 0)
			m_oXmlWriter.WriteAttribute(L"height", ConvertToWString(m_oViewport.GetHeight()));

		double dXScale = 1, dYScale = 1, dXTranslate = 0, dYTranslate = 0;

		if (0 != m_oSizeWindow.x)
		{
			dXScale = m_oSizeWindow.x / m_oViewport.GetWidth();
			dXTranslate = (m_oViewport.GetWidth()) / 2 * std::abs(dXScale - 1);

			if (dXScale < 1)
				dXTranslate = -dXTranslate;
		}

		if (0 != m_oSizeWindow.y)
		{
			dYScale = m_oSizeWindow.y / m_oViewport.GetHeight();
			dYTranslate = (m_oViewport.GetHeight()) / 2 * std::abs(dYScale - 1);

			if (dYScale < 1)
				dYTranslate = -dYTranslate;
		}

		if (1 != dXScale || 1 != dYScale)
			m_oXmlWriter.WriteAttribute(L"transform", L"matrix(" + std::to_wstring(dXScale) + L",0,0," + std::to_wstring(dYScale) + L',' + ConvertToWString(dXTranslate) + L',' + ConvertToWString(dYTranslate) + L')');

		m_oXmlWriter.WriteNodeEnd(L"svg", true, false);
	}

	void CWmfInterpretatorSvg::HANDLE_META_BITBLT(const TWmfBitBlt &oWmfBitBlt, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CWmfInterpretatorSvg::HANDLE_META_DIBBITBLT(const TWmfBitBlt &oWmfBitBlt, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CWmfInterpretatorSvg::HANDLE_META_DIBSTRETCHBLT(const TWmfStretchBlt &oWmfStretchBlt, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CWmfInterpretatorSvg::HANDLE_META_SETDIBTODEV(const TWmfSetDibToDev &oWmfSetDibToDev, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CWmfInterpretatorSvg::HANDLE_META_STRETCHBLT(const TWmfStretchBlt &oWmfStretchBlt, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CWmfInterpretatorSvg::HANDLE_META_STRETCHDIB(const TWmfStretchDib &oWmfStretchDib, CDataStream &oDataStream)
	{
		//запись реализована в DrawBitmap
	}

	void CWmfInterpretatorSvg::HANDLE_META_EOF()
	{
		m_oXmlWriter.WriteString(m_wsDefs);
		m_oXmlWriter.WriteNodeEnd(L"svg", false, false);
	}

	void CWmfInterpretatorSvg::HANDLE_META_ARC(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
	{
		TRectD oNewRect;

		oNewRect.dLeft   = shLeft;
		oNewRect.dTop    = shTop;
		oNewRect.dRight  = shRight;
		oNewRect.dBottom = shBottom;

		double dXRadius = std::fabs(oNewRect.dRight - oNewRect.dLeft) / 2;
		double dYRadius = std::fabs(oNewRect.dBottom - oNewRect.dTop) / 2;

		std::wstring wsValue = L"M " + ConvertToWString(shXStartArc) + L' ' + ConvertToWString(shYStartArc);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
				ConvertToWString(dYRadius) + L' ' +
				L"0 0 1 " +
				//                                    ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				//                                    ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				ConvertToWString(shXEndArc) + L' ' +
				ConvertToWString(shYEndArc);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"path" , arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_CHORD(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_ELLIPSE(short shBottom, short shRight, short shTop, short shLeft)
	{
		TRectD oNewRect;

		oNewRect.dLeft   = shLeft;
		oNewRect.dTop    = shTop;
		oNewRect.dRight  = shRight;
		oNewRect.dBottom = shBottom;

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
									   {L"cy", ConvertToWString((oNewRect.dTop    + oNewRect.dBottom) / 2)},
									   {L"rx", ConvertToWString((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
									   {L"ry", ConvertToWString((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"ellipse", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_EXTTEXTOUT(short shY, short shX, short shStringLength, unsigned short ushFwOptions, const TWmfRect &oRectangle, unsigned char *pString, short *pDx)
	{
		IFont* pFont = NULL;

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

		WriteText(wsText, shX, shY, oRectangle);
	}

	void CWmfInterpretatorSvg::HANDLE_META_FILLREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex)
	{
		CWmfRegion *pRegion = m_pParser->GetDC()->GetRegion();

		if (NULL == pRegion)
			return;

		std::wstring wsValue;

		for (unsigned int unScanIndex = 0; unScanIndex < pRegion->ScanCount; ++unScanIndex)
		{
			TWmfScanObject *pScanObject = &pRegion->aScans[unScanIndex];

			if (pScanObject->Count == 0) continue;

			for (unsigned int unIndex = 0; unIndex < pScanObject->Count >> 1; ++unIndex)
			{
				wsValue += L"M " + ConvertToWString(std::max((short)pScanObject->ScanLines[unIndex].Left, (short)pRegion->BoundingRectangle.Left)) + L',' +  ConvertToWString(std::max((short)pScanObject->Top, (short)pRegion->BoundingRectangle.Top)) + L' ' +
				           ConvertToWString(std::min((short)pScanObject->ScanLines[unIndex].Right, (short)pRegion->BoundingRectangle.Right)) + L',' + ConvertToWString(std::max((short)pScanObject->Top, (short)pRegion->BoundingRectangle.Top)) + L' ' +
				           ConvertToWString(std::min((short)pScanObject->ScanLines[unIndex].Right, (short)pRegion->BoundingRectangle.Right)) + L',' + ConvertToWString(std::min((short)pScanObject->Bottom, (short)pRegion->BoundingRectangle.Bottom)) + L' ' +
				           ConvertToWString(std::max((short)pScanObject->ScanLines[unIndex].Left, (short)pRegion->BoundingRectangle.Left)) + L',' + ConvertToWString(std::min((short)pScanObject->Bottom, (short)pRegion->BoundingRectangle.Bottom)) + L' ' +
				           ConvertToWString(std::max((short)pScanObject->ScanLines[unIndex].Left, (short)pRegion->BoundingRectangle.Left)) + L',' + ConvertToWString(std::max((short)pScanObject->Top, (short)pRegion->BoundingRectangle.Top)) + L' ';
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

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

		NodeAttributes arAttributes = {{L"x1", ConvertToWString(oCurPos.x)},
									   {L"y1", ConvertToWString(oCurPos.y)},
									   {L"x2", ConvertToWString(shX)},
									   {L"y2", ConvertToWString(shY)}};

		AddStroke(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"line", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_PAINTREGION(unsigned short ushRegionIndex)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_PATBLT(unsigned int unRasterOperation, short shH, short shW, short shY, short shX)
	{
		TRectD oNewRect;

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(shX)},
		                               {L"y",		ConvertToWString(shY)},
		                               {L"width",	ConvertToWString(shW)},
		                               {L"height",	ConvertToWString(shH)}};

		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

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
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"path", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_POLYLINE(const std::vector<TWmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TWmfPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"polyline", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_POLYGON(const std::vector<TWmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TWmfPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"polygon", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TWmfPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TWmfPointS>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';

			for (const TWmfPointS& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_RECTANGLE(short shB, short shR, short shT, short shL)
	{
		TRectD oNewRect;

		oNewRect.dLeft   = shL;
		oNewRect.dTop    = shT;
		oNewRect.dRight  = shR;
		oNewRect.dBottom = shB;

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.dLeft)},
									   {L"y",		ConvertToWString(oNewRect.dTop)},
									   {L"width",	ConvertToWString(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height",	ConvertToWString(oNewRect.dBottom - oNewRect.dTop)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"rect", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_ROUNDRECT(short shH, short shW, short shB, short shR, short shT, short shL)
	{
		TRectD oNewRect;

		oNewRect.dLeft   = shL;
		oNewRect.dTop    = shT;
		oNewRect.dRight  = shR;
		oNewRect.dBottom = shB;


		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.dLeft)},
									   {L"y",		ConvertToWString(oNewRect.dTop)},
									   {L"width",	ConvertToWString(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height",	ConvertToWString(oNewRect.dBottom - oNewRect.dTop)},
									   {L"rx",		ConvertToWString(shW)},
									   {L"ry",		ConvertToWString(shH)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip(arAttributes);

		WriteNode(L"rect", arAttributes);
	}

	void CWmfInterpretatorSvg::HANDLE_META_SETPIXEL(const TWmfColor &oColor, short shY, short shX)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_TEXTOUT(short shStringLength, unsigned char *pString, short shX, short shY)
	{
		IFont* pFont = NULL;

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

		WriteText(wsText, shX, shY);
	}

	void CWmfInterpretatorSvg::HANDLE_META_CREATEBRUSHINDIRECT(const TWmfLogBrush &oBrush)
	{
	}

	void CWmfInterpretatorSvg::HANDLE_META_CREATEFONTINDIRECT(const CWmfFont &oFont)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_CREATEPALETTE(const CWmfPalette &oPalette)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_CREATEPATTERNBRUSH(const CWmfBrush &oBrush, CDataStream &oDataStream)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_CREATEPENINDIRECT(const CWmfPen &oPen)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_CREATEREGION(const CWmfRegion &oRegion)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_DELETEOBJECT(unsigned short ushIndex)
	{
	}

	void CWmfInterpretatorSvg::HANDLE_META_DIBCREATEPATTERNBRUSH(unsigned short ushStyle, unsigned short ushColorUsage, const CWmfBrush &oBrush, CDataStream &oDataStream)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SELECTCLIPREGION(unsigned short ushIndex)
	{
		//TODO:: реализовать
	}

	void CWmfInterpretatorSvg::HANDLE_META_SELECTOBJECT(unsigned short ushIndex)
	{
	}

	void CWmfInterpretatorSvg::HANDLE_META_SELECTPALETTE(unsigned short ushIndex)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_EXCLUDECLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
	{
		ResetClip();
	}

	void CWmfInterpretatorSvg::HANDLE_META_INTERSECTCLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
	{
		ResetClip();
	}

	void CWmfInterpretatorSvg::HANDLE_META_MOVETO(short shX, short shY)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_OFFSETCLIPRGN(short shOffsetX, short shOffsetY)
	{
		ResetClip();
	}

	void CWmfInterpretatorSvg::HANDLE_META_OFFSETVIEWPORTORG(short shXOffset, short shYOffset)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_OFFSETWINDOWORG(short shXOffset, short shYOffset)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_RESTOREDC()
	{
		ResetClip();
	}

	void CWmfInterpretatorSvg::HANDLE_META_SAVEDC()
	{
	}

	void CWmfInterpretatorSvg::HANDLE_META_SCALEVIEWPORTEXT(short yDenom, short yNum, short xDenom, short xNum)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SCALEWINDOWEXT(short yDenom, short yNum, short xDenom, short xNum)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETBKCOLOR(const TWmfColor &oColor)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETBKMODE(unsigned short ushMode)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETLAYOUT(unsigned short ushLayout)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETMAPMODE(unsigned short ushMapMode)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETPOLYFILLMODE(unsigned short ushMode)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETROP2(unsigned short ushMode)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETSTRETCHBLTMODE(unsigned short ushMode)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETTEXTALIGN(unsigned short ushTextAlign)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETTEXTCHAREXTRA(unsigned short ushCharSpacing)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETTEXTCOLOR(const TWmfColor &oColor)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETTEXTJUSTIFICATION(unsigned short ushBreakCount, unsigned short ushBreakExtra)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETVIEWPORTEXT(short shX, short shY)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETVIEWPORTORG(short shX, short shY)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETWINDOWEXT(short shX, short shY)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_SETWINDOWORG(short shX, short shY)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_ESCAPE(unsigned short ushEscapeFunction, unsigned short ushByteCount)
	{

	}

	void CWmfInterpretatorSvg::HANDLE_META_UNKNOWN(CDataStream &oDataStream)
	{

	}

	void CWmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight)
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
			AddClip(arAttributes);

			WriteNode(L"image", arAttributes);
		}

		if (NULL != pNewBuffer)
			delete [] pNewBuffer;
	}

	void CWmfInterpretatorSvg::ResetClip()
	{
		m_wsLastClipId.clear();
	}

	void CWmfInterpretatorSvg::IntersectClip(const TRectD& oClip)
	{
		m_wsLastClipId = L"INTERSECTCLIP_" + ConvertToWString(++m_unNumberDefs, 0);

		m_wsDefs += L"<clipPath id=\"" + m_wsLastClipId + L"\">" +
		            L"<rect x=\"" + ConvertToWString(oClip.dLeft, 0) + L"\" y=\"" + ConvertToWString(oClip.dTop, 0) + L"\" width=\"" + ConvertToWString(oClip.dRight - oClip.dLeft, 0) + L"\" height=\"" + ConvertToWString(oClip.dBottom - oClip.dTop, 0) + L"\"/>" +
		            L"</clipPath>";
	}

	void CWmfInterpretatorSvg::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		m_wsLastClipId = L"EXCLUDECLIP_" + ConvertToWString(++m_unNumberDefs, 0);

		m_wsDefs += L"<clipPath id=\"" + m_wsLastClipId + L"\">" +
		            L"<path d=\"M" + ConvertToWString(oBB.dLeft) + L' ' + ConvertToWString(oBB.dTop) + L", L" + ConvertToWString(oBB.dRight) + L' ' + ConvertToWString(oBB.dTop) + L", " +
		            ConvertToWString(oBB.dRight) + L' ' + ConvertToWString(oBB.dBottom) + L", " + ConvertToWString(oBB.dLeft) + L' ' + ConvertToWString(oBB.dBottom) + L", M" +
		            ConvertToWString(oClip.dLeft) + L' ' + ConvertToWString(oClip.dTop) + L", L" + ConvertToWString(oClip.dRight) + L' ' + ConvertToWString(oClip.dTop) + L", " +
		            ConvertToWString(oClip.dRight) + L' ' + ConvertToWString(oClip.dBottom) + L", " + ConvertToWString(oClip.dLeft) + L' ' + ConvertToWString(oClip.dLeft) + L"\" clip-rule=\"evenodd\"/>" +
		            L"</clipPath>";
	}

	void CWmfInterpretatorSvg::PathClip(IPath *pPath, int nClipMode, TXForm *pTransform)
	{
	}

	void CWmfInterpretatorSvg::SetXmlWriter(XmlUtils::CXmlWriter *pXmlWriter)
	{
		if (NULL != pXmlWriter)
			m_oXmlWriter = *pXmlWriter;
	}

	XmlUtils::CXmlWriter *CWmfInterpretatorSvg::GetXmlWriter()
	{
		return &m_oXmlWriter;
	}

	std::wstring CWmfInterpretatorSvg::GetFile()
	{
		return m_oXmlWriter.GetXmlString();
	}

	void CWmfInterpretatorSvg::IncludeSvg(const std::wstring &wsSvg, const TRectD &oRect, const TRectD &oClipRect, TXForm *pTransform)
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

	void CWmfInterpretatorSvg::WriteNode(const std::wstring &wsNodeName, const NodeAttributes &arAttributes, const std::wstring &wsValueNode)
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

	void CWmfInterpretatorSvg::WriteNodeBegin(const std::wstring &wsNodeName, const NodeAttributes &arAttributes)
	{
		m_oXmlWriter.WriteNodeBegin(wsNodeName, !arAttributes.empty());

		if (!arAttributes.empty())
		{
			for (const NodeAttribute& oAttribute : arAttributes)
				m_oXmlWriter.WriteAttribute(oAttribute.first, oAttribute.second);

			m_oXmlWriter.WriteNodeEnd(wsNodeName, true, false);
		}
	}

	void CWmfInterpretatorSvg::WriteNodeEnd(const std::wstring &wsNodeName)
	{
		m_oXmlWriter.WriteNodeEnd(wsNodeName, false, false);
	}

	void CWmfInterpretatorSvg::WriteText(const std::wstring &wsText, double dX, double dY, const TWmfRect& oBounds)
	{
		NodeAttributes arNodeAttributes;

		double dXCoord = dX;
		double dYCoord = dY;

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		if (NULL != m_pParser && NULL != m_pParser->GetFont())
		{
			TWmfColor oColor = m_pParser->GetDC()->GetTextColor();

			if (0 != oColor.r || 0 != oColor.g || 0 != oColor.b)
				arNodeAttributes.push_back({L"fill", L"rgba(" + ConvertToWString(oColor.r, 0) + L", " + ConvertToWString(oColor.g, 0) + L", " + ConvertToWString(oColor.b, 0) + L", 255)"});

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

			unsigned int ulTextAlign = m_pParser->GetTextAlign();
			if (ulTextAlign & TA_BASELINE)
			{
				// Ничего не делаем
			}
			else if (ulTextAlign & TA_BOTTOM)
			{
				arNodeAttributes.push_back({L"dominant-baseline", L"auto"});
			}
			else // if (ulTextAlign & TA_TOP)
			{
				arNodeAttributes.push_back({L"dominant-baseline", L"hanging"});
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
			{
				double dEscapement = pFont->GetEscapement() / -10;

				if (m_pParser->GetTransform()->M22 < 0)
					dEscapement = -dEscapement;

				arNodeAttributes.push_back({L"transform", L"rotate(" + ConvertToWString(dEscapement) + L' ' + ConvertToWString(dXCoord) + L' ' + ConvertToWString(dYCoord) + L')'});
			}

			if (m_pParser->IsWindowFlippedX())
			{
				dXCoord = -dXCoord;
				oTransform.M11 = -oTransform.M11;
			}

			if (m_pParser->IsWindowFlippedY())
			{
				dYCoord = -dYCoord;
				oTransform.M22 = -oTransform.M22;
			}
		}

		AddTransform(arNodeAttributes, &oTransform);
		AddClip(arNodeAttributes);

		arNodeAttributes.push_back({L"x", ConvertToWString(dXCoord)});
		arNodeAttributes.push_back({L"y", ConvertToWString(dYCoord)});

		WriteNode(L"text", arNodeAttributes, StringNormalization(wsText));
	}

	void CWmfInterpretatorSvg::AddStroke(NodeAttributes &arAttributes)
	{
		if (NULL == m_pParser)
			return;

		IPen* pPen = m_pParser->GetPen();

		if (NULL == pPen || PS_NULL == pPen->GetStyle())
			return;

		if (pPen->GetAlpha() != 255)
			arAttributes.push_back({L"stroke-opacity" , ConvertToWString(pPen->GetAlpha() / 255., 3)});

		arAttributes.push_back({L"stroke", L"rgb(" + INTCOLOR_TO_RGB(pPen->GetColor()) + L')'});

		double dStrokeWidth = std::fabs(m_pParser->GetPen()->GetWidth());

		if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK)))
			dStrokeWidth = 1. / std::fabs(m_pParser->GetTransform()->M11);

		arAttributes.push_back({L"stroke-width", ConvertToWString(dStrokeWidth)});

		unsigned int unMetaPenStyle = pPen->GetStyle();

		//			unsigned int ulPenType      = unMetaPenStyle & PS_TYPE_MASK;
		unsigned int ulPenStyle     = unMetaPenStyle & PS_STYLE_MASK;

		if (PS_SOLID == ulPenStyle)
		{
			unsigned int ulPenStartCap  = unMetaPenStyle & PS_STARTCAP_MASK;
			unsigned int ulPenEndCap    = unMetaPenStyle & PS_ENDCAP_MASK;
			unsigned int ulPenJoin      = unMetaPenStyle & PS_JOIN_MASK;

			// svg не поддерживает разные стили для сторон линии
			if (PS_STARTCAP_ROUND == ulPenStartCap)
				arAttributes.push_back({L"stroke-linecap", L"round"});
			else if (PS_STARTCAP_SQUARE == ulPenStartCap)
				arAttributes.push_back({L"stroke-linecap", L"square"});
			else if (PS_STARTCAP_FLAT == ulPenStartCap)
				arAttributes.push_back({L"stroke-linecap", L"butt"});
			else
				arAttributes.push_back({L"stroke-linecap", L"round"});

			if (PS_JOIN_MITER == ulPenJoin)
				arAttributes.push_back({L"stroke-linejoin", L"miter"});
			else if (PS_JOIN_BEVEL == ulPenJoin)
				arAttributes.push_back({L"stroke-linejoin", L"bevel"});
			else if (PS_JOIN_ROUND == ulPenJoin)
				arAttributes.push_back({L"stroke-linejoin", L"round"});
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

	void CWmfInterpretatorSvg::AddFill(NodeAttributes &arAttributes, double dWidth, double dHeight)
	{
		if (NULL == m_pParser)
		{
			AddNoneFill(arAttributes);
			return;
		}

		IBrush *pBrush = m_pParser->GetBrush();

		if (NULL == pBrush || BS_NULL == pBrush->GetStyle())
		{
			AddNoneFill(arAttributes);
			return;
		}

		if (pBrush->GetAlpha() != 255)
			arAttributes.push_back({L"fill-opacity" , ConvertToWString(pBrush->GetAlpha() / 255., 3)});

		switch (pBrush->GetStyle())
		{
			case BS_SOLID:
			{
				arAttributes.push_back({L"fill", L"rgba(" + INTCOLOR_TO_RGB(pBrush->GetColor()) + L"," + ConvertToWString(pBrush->GetAlpha(), 0) + L")"});
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

				break;
			}
			default: break;
		}

		AddNoneFill(arAttributes);
	}

	void CWmfInterpretatorSvg::AddTransform(NodeAttributes &arAttributes, TXForm *pTransform)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetBrush())
			return;

		TXForm oOldTransform;

		if (NULL != pTransform)
			oOldTransform.Copy(pTransform);
		else
			oOldTransform.Copy(m_pParser->GetTransform());

		if (std::fabs(oOldTransform.M11) > 100. || std::fabs(oOldTransform.M22) > 100.)
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

	void CWmfInterpretatorSvg::AddClip(NodeAttributes &arAttributes)
	{
		if (NULL == m_pParser)
			return;

		if (m_wsLastClipId.empty())
			UpdateClip();

		if (!m_wsLastClipId.empty())
			arAttributes.push_back({L"clip-path", L"url(#" + m_wsLastClipId + L')'});
	}

	void CWmfInterpretatorSvg::AddNoneFill(NodeAttributes &arAttributes)
	{
		arAttributes.push_back({L"fill", L"none"});
	}

	TRectD CWmfInterpretatorSvg::TranslateRect(const TWmfRect &oRect)
	{
		TRectD oNewRect;

		oNewRect.dLeft   = oRect.Left;
		oNewRect.dTop    = oRect.Top;
		oNewRect.dRight  = oRect.Right;
		oNewRect.dBottom = oRect.Bottom;

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

	TPointD CWmfInterpretatorSvg::GetCutPos()
	{
		if (NULL != m_pParser)
			return m_pParser->GetCurPos();

		TPointD oCurPos;

		oCurPos.x = m_oViewport.dLeft;
		oCurPos.y = m_oViewport.dRight;

		return oCurPos;
	}

	std::wstring CWmfInterpretatorSvg::CreateHatchStyle(unsigned int unHatchStyle, double dWidth, double dHeight)
	{
		if (NULL == m_pParser || NULL == m_pParser->GetBrush() || NULL == m_pParser->GetPen())
			return std::wstring();

		double dStrokeWidth = m_pParser->GetPen()->GetWidth();

		if (0.0 == dStrokeWidth || (1.0 == dStrokeWidth && PS_COSMETIC == (m_pParser->GetPen()->GetStyle() & PS_TYPE_MASK)))
			dStrokeWidth = 1. / m_pParser->GetTransform()->M11;

		std::wstring wsStrokeWidth = ConvertToWString(dStrokeWidth);
		std::wstring wsValue  = ConvertToWString(dStrokeWidth * 8., 6);
		std::wstring wsValueW = ((0 != dWidth)  ? ConvertToWString((dStrokeWidth * 8.) / dWidth,  6) : L"1");
		std::wstring wsValueH = ((0 != dHeight) ? ConvertToWString((dStrokeWidth * 8.) / dHeight, 6) : L"1");

		std::wstring wsStrokeColor = L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetBrush()->GetColor()) + L')';
		std::wstring wsBgColor;

		if (TRANSPARENT != m_pParser->GetTextBgMode())
			wsBgColor = L"rgb(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor()) + L')';

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

	std::wstring CWmfInterpretatorSvg::CreateDibPatternStyle(IBrush *pBrush)
	{
		if (NULL == m_pParser || NULL == pBrush || NULL == m_pParser->GetPen())
			return std::wstring();

		BYTE* pBuffer = NULL;
		unsigned int unWidth = 0, unHeight = 0;

		pBrush->GetDibPattern(&pBuffer, unWidth, unHeight);

		if (NULL == pBuffer || 0 == unWidth || 0 == unHeight)
			return std::wstring();

		char* pImageData = NULL;
		int nImageSize = 0;
		std::wstring wsStyleId;

		NSFile::CBase64Converter::Encode(pBuffer, unWidth * 4 * unHeight, pImageData, nImageSize, NSBase64::B64_BASE64_FLAG_NOCRLF);

		if (NULL != pImageData)
		{
			wsStyleId += L"DIBPATTERN_" + ConvertToWString(++m_unNumberDefs, 0);

			std::wstring wsImageDataW = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pImageData, (LONG)nImageSize);

			double dStrokeWidth = std::abs(m_pParser->GetPen()->GetWidth());

			if (PS_COSMETIC == m_pParser->GetPen())
			{
				dStrokeWidth = 1 / std::abs(m_pParser->GetPixelHeight());
			}
			else
			{
				double dMinStrokeWidth = 1 / std::abs(m_pParser->GetPixelHeight());

				if (dStrokeWidth < dMinStrokeWidth)
					dStrokeWidth = dMinStrokeWidth;
			}

			std::wstring wsWidth  = ConvertToWString(dStrokeWidth * 10 * unHeight / unWidth);
			std::wstring wsHeight = ConvertToWString(dStrokeWidth * 10 * unWidth  / unHeight);

			m_wsDefs += L"<pattern id=\"" + wsStyleId + L"\" " +
						L"width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\" patternUnits=\"userSpaceOnUse\">" +
						L"<image xlink:href=\"data:image/png;base64," + wsImageDataW + L"\" x=\"0\" y=\"0\" width=\"" + wsWidth + L"\" height=\"" + wsHeight + L"\"/>" +
						L"</pattern> ";

			delete [] pImageData;
		}

		return wsStyleId;
	}

	void CWmfInterpretatorSvg::UpdateClip()
	{
		IClip* pClip = m_pParser->GetClip();

		if (NULL != pClip)
			pClip->ClipOnRenderer(this);
	}
}
