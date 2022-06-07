#include "CWmfInterpretatorSvg.h"

#include "../Image.h"

namespace MetaFile
{
        CWmfInterpretatorSvg::CWmfInterpretatorSvg(const wchar_t *wsFilePath, CWmfParserBase *pParser, unsigned int unWidth, unsigned int unHeight)
                : m_wsSvgFilePath(wsFilePath), m_pParser(pParser)
        {
                SetSize(unWidth, unHeight);
        }

        CWmfInterpretatorSvg::CWmfInterpretatorSvg(const CWmfInterpretatorSvg &oInterpretator)
                : m_wsSvgFilePath(oInterpretator.m_wsSvgFilePath), m_pParser(NULL)
        {

        }

        CWmfInterpretatorSvg::~CWmfInterpretatorSvg()
        {

        }

        InterpretatorType CWmfInterpretatorSvg::GetType() const
        {
                return InterpretatorType::Svg;
        }

        void CWmfInterpretatorSvg::SetOutputDevice(const wchar_t *wsFilePath)
        {
                m_wsSvgFilePath = wsFilePath;
        }

        void CWmfInterpretatorSvg::SetSize(unsigned int unWidth, unsigned int unHeight)
        {
                m_oSizeWindow.cx = unWidth;
                m_oSizeWindow.cy = unHeight;
        }

        void CWmfInterpretatorSvg::HANDLE_META_HEADER(const TWmfPlaceable& oPlaceable, const TWmfHeader& oHeader)
        {
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
                //конец Wmf файла
        }

        void CWmfInterpretatorSvg::HANDLE_META_ARC(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
        {
                TRectD oNewRect;

                oNewRect.dLeft   = TranslateX(shLeft);
                oNewRect.dTop    = TranslateX(shTop);
                oNewRect.dRight  = TranslateX(shRight);
                oNewRect.dBottom = TranslateX(shBottom);

                double dXRadius = std::fabs(oNewRect.dRight - oNewRect.dLeft) / 2;
                double dYRadius = std::fabs(oNewRect.dBottom - oNewRect.dTop) / 2;

                std::wstring wsValue = L"M " + std::to_wstring(TranslateX(shXStartArc)) + L' ' + std::to_wstring(TranslateY(shYStartArc));

                wsValue += L" A " + std::to_wstring(dXRadius) + L' ' +
                                    std::to_wstring(dYRadius) + L' ' +
                                    L"0 0 1 " +
//                                    ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
//                                    ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
                                    std::to_wstring(TranslateX(shXEndArc)) + L' ' +
                                    std::to_wstring(TranslateY(shYEndArc));

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(oNewRect);

                WriteNode(L"path" , arAttributes);
        }

        void CWmfInterpretatorSvg::HANDLE_META_CHORD(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft)
        {
                //TODO:: реализовать
        }

        void CWmfInterpretatorSvg::HANDLE_META_ELLIPSE(short shBottom, short shRight, short shTop, short shLeft)
        {
                TRectD oNewRect;

                oNewRect.dLeft   = TranslateX(shLeft);
                oNewRect.dTop    = TranslateX(shTop);
                oNewRect.dRight  = TranslateX(shRight);
                oNewRect.dBottom = TranslateX(shBottom);

                NodeAttributes arAttributes = {{L"cx", std::to_wstring((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
                                               {L"cy", std::to_wstring((oNewRect.dTop    + oNewRect.dBottom) / 2)},
                                               {L"rx", std::to_wstring((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
                                               {L"ry", std::to_wstring((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
                AddStroke(arAttributes);
                AddFill(arAttributes);

                UpdateTransform(oNewRect);

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
                //TODO:: реализовать
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

                TPointD oPoint(TranslateX(shX), TranslateY(shY));

                NodeAttributes arAttributes = {{L"x1", std::to_wstring(oCurPos.x)},
                                               {L"y1", std::to_wstring(oCurPos.y)},
                                               {L"x2", std::to_wstring(TranslateX(shX))},
                                               {L"y2", std::to_wstring(TranslateY(shY))}};

                AddStroke(arAttributes);

                UpdateTransform(oPoint.x, oPoint.y);

                WriteNode(L"line", arAttributes);
        }

        void CWmfInterpretatorSvg::HANDLE_META_PAINTREGION(unsigned short ushRegionIndex)
        {
                //TODO:: реализовать
        }

        void CWmfInterpretatorSvg::HANDLE_META_PATBLT(unsigned int unRasterOperation, short shH, short shW, short shY, short shX)
        {
                //TODO:: реализовать
        }

        void CWmfInterpretatorSvg::HANDLE_META_PIE(short shXRadial1, short shYRadial1, short shXRadial2, short shYRadial2, short shB, short shR, short shT, short shL)
        {
                //TODO:: реализовать
        }

        void CWmfInterpretatorSvg::HANDLE_META_POLYLINE(const std::vector<TWmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TWmfPointS& oPoint : arPoints)
                     wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateX(oPoint.y)) + L' ';

                NodeAttributes arAttributes = {{L"points", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arPoints);

                WriteNode(L"polyline", arAttributes);
        }

        void CWmfInterpretatorSvg::HANDLE_META_POLYGON(const std::vector<TWmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TWmfPointS& oPoint : arPoints)
                        wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y)) + L' ';

                NodeAttributes arAttributes = {{L"points", wsValue}};

                AddStroke(arAttributes);
                AddFill(arAttributes);

                UpdateTransform(arPoints);

                WriteNode(L"polygon", arAttributes);
        }

        void CWmfInterpretatorSvg::HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TWmfPointS>> &arPolygons)
        {
                for (const std::vector<TWmfPointS>& arPoints : arPolygons)
                        HANDLE_META_POLYGON(arPoints);
        }

        void CWmfInterpretatorSvg::HANDLE_META_RECTANGLE(short shB, short shR, short shT, short shL)
        {
                TRectD oNewRect;

                oNewRect.dLeft   = TranslateX(shL);
                oNewRect.dTop    = TranslateX(shT);
                oNewRect.dRight  = TranslateX(shR);
                oNewRect.dBottom = TranslateX(shB);

                NodeAttributes arAttributes = {{L"x", std::to_wstring(oNewRect.dLeft)},
                                               {L"y", std::to_wstring(oNewRect.dTop)},
                                               {L"width", std::to_wstring(oNewRect.dRight - oNewRect.dLeft)},
                                               {L"height", std::to_wstring(oNewRect.dBottom - oNewRect.dTop)}};

                AddStroke(arAttributes);
                AddFill(arAttributes);

                UpdateTransform(oNewRect);

                WriteNode(L"rect", arAttributes);
        }

        void CWmfInterpretatorSvg::HANDLE_META_ROUNDRECT(short shH, short shW, short shB, short shR, short shT, short shL)
        {
                TRectD oNewRect;

                oNewRect.dLeft   = TranslateX(shL);
                oNewRect.dTop    = TranslateX(shT);
                oNewRect.dRight  = TranslateX(shR);
                oNewRect.dBottom = TranslateX(shB);


                NodeAttributes arAttributes = {{L"x", std::to_wstring(oNewRect.dLeft)},
                                               {L"y", std::to_wstring(oNewRect.dTop)},
                                               {L"width", std::to_wstring(oNewRect.dRight - oNewRect.dLeft)},
                                               {L"height", std::to_wstring(oNewRect.dBottom - oNewRect.dTop)},
                                               {L"rx", std::to_wstring(TranslateX(shW))},
                                               {L"ry", std::to_wstring(TranslateY(shH))}};

                AddStroke(arAttributes);
                AddFill(arAttributes);

                UpdateTransform(oNewRect);

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

        }

        void CWmfInterpretatorSvg::HANDLE_META_SELECTOBJECT(unsigned short ushIndex)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_SELECTPALETTE(unsigned short ushIndex)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_EXCLUDECLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_INTERSECTCLIPRECT(short shLeft, short shTop, short shRight, short shBottom)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_MOVETO(short shX, short shY)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_OFFSETCLIPRGN(short shOffsetX, short shOffsetY)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_OFFSETVIEWPORTORG(short shXOffset, short shYOffset)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_OFFSETWINDOWORG(short shXOffset, short shYOffset)
        {

        }

        void CWmfInterpretatorSvg::HANDLE_META_RESTOREDC()
        {

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

        void CWmfInterpretatorSvg::Begin()
        {
                m_oXmlWriter.WriteNodeBegin(L"svg", true);
                m_oXmlWriter.WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
                m_oXmlWriter.WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");
                m_oXmlWriter.WriteNodeEnd(L"svg", true, false);
        }

        void CWmfInterpretatorSvg::End()
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

                                m_oViewport.dX += m_oViewport.dWidth;
                        }

                        if (m_pParser->IsWindowFlippedY())
                        {
                                nFlipY = -1;
                                bFlipped = true;

                                m_oViewport.dY += m_oViewport.dHeight;
                        }

                        if (nFlipX < 0 || nFlipY < 0 || bFlipped)
                                wsXml.insert(5, L"transform=\"scale(" + std::to_wstring(nFlipX) + L' ' + std::to_wstring(nFlipY) + L")\" ");
                }

//                if (m_oViewport.dX < 0 || m_oViewport.dY < 0)
                wsXml.insert(5, L"viewBox=\"" + std::to_wstring(m_oViewport.dX) + L' ' + std::to_wstring(m_oViewport.dY) + L' ' + std::to_wstring(m_oViewport.dWidth) + L' ' + std::to_wstring(m_oViewport.dHeight) + L"\" ");

                if (0 != m_oSizeWindow.cx && 0 != m_oSizeWindow.cy)
                        wsXml.insert(5, L"width=\"" + std::to_wstring(m_oSizeWindow.cx) + L"\" height=\"" + std::to_wstring(m_oSizeWindow.cy) + L"\" ");

                m_oXmlWriter.SetXmlString(wsXml);

                m_oXmlWriter.SaveToFile((!m_wsSvgFilePath.empty()) ? m_wsSvgFilePath : L"temp.svg");
        }

        void CWmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight)
        {
                if (0 == unWidth || 0 == unHeight)
                        return;

                Aggplus::CImage oImage;
                BYTE* pBufferPtr = new BYTE[4 * unWidth * unHeight];
                oImage.Create(pBufferPtr, unWidth, unHeight, 4 * unWidth);

		for (int nIndex = 0, nSize = 4 * unWidth * unHeight; nIndex < nSize; nIndex += 4)
		{
			pBufferPtr[0] = (BYTE)pBuffer[nIndex + 0];
			pBufferPtr[1] = (BYTE)pBuffer[nIndex + 1];
			pBufferPtr[2] = (BYTE)pBuffer[nIndex + 2];
			pBufferPtr[3] = (BYTE)pBuffer[nIndex + 3];
			pBufferPtr += 4;
		}

		oImage.SaveFile(L"temp.png", 4);

		NSFile::CFileBinary oFile;

                BYTE* pImageBytes = NULL;
                DWORD ulImageSize;

                oFile.ReadAllBytes(L"temp.png", &pImageBytes, ulImageSize);

                if (0 == ulImageSize)
                        return;

                int nSize = NSBase64::Base64EncodeGetRequiredLength(ulImageSize);
                unsigned char* ucValue = new unsigned char[nSize];

                if (NULL == ucValue)
                        return;

                NSBase64::Base64Encode(pImageBytes, ulImageSize, ucValue, &nSize);
                std::wstring wsValue(ucValue, ucValue + nSize);

                NodeAttributes arAttributes = {{L"x", std::to_wstring(TranslateX(dX))},
                                               {L"y", std::to_wstring(TranslateY(dY))},
                                               {L"width", std::to_wstring(TranslateX(dW))},
                                               {L"height", std::to_wstring(TranslateY(dH))},
                                               {L"xlink:href", L"data:image/png;base64," + wsValue}};;

                UpdateTransform(dX, dY);

                WriteNode(L"image", arAttributes);

                NSFile::CFileBinary::Remove(L"temp.png");

                delete [] ucValue;

                if (NULL != pImageBytes)
                        delete [] pImageBytes;
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

        void CWmfInterpretatorSvg::WriteText(const std::wstring &wsText, double dX, double dY, const TWmfRect& oBounds)
        {
                NodeAttributes arNodeAttributes;

                std::wstring wsXCoord = std::to_wstring(TranslateX(dX));
                std::wstring wsYCoord = std::to_wstring(TranslateY(dY));

                if (NULL != m_pParser && NULL != m_pParser->GetFont())
                {
                        if (OPAQUE == m_pParser->GetTextBgMode() && !oBounds.Empty())
                        {
                                std::wstring wsFillRect = L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor()) + L", 255)";

                                WriteNode(L"rect", {{L"x",      std::to_wstring((oBounds.Left))},
                                                    {L"y",      std::to_wstring((oBounds.Top))},
                                                    {L"width",  std::to_wstring((oBounds.Right - oBounds.Left))},
                                                    {L"height", std::to_wstring((oBounds.Bottom - oBounds.Top))},
                                                    {L"fill", wsFillRect},
                                                    {L"stroke", L"none"}});
                        }

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

                        if (pFont->IsUnderline())
                                arNodeAttributes.push_back({L"text-decoration", L"underline"});

                        if (pFont->IsStrikeOut())
                                arNodeAttributes.push_back({L"text-decoration", L"line-through"});

                        //TODO:: разобраться для корректной работы
//                        double dFontCharSpace = TranslateX(pFont->GetCharSet());

//                        if (dFontCharSpace > 1)
//                                arNodeAttributes.push_back({L"letter-spacing", std::to_wstring(dFontCharSpace)});

                        if (0 != pFont->GetEscapement())
                                arNodeAttributes.push_back({L"transform", L"rotate(" + std::to_wstring(pFont->GetEscapement() / -10) + L' ' + wsXCoord + L' ' + wsYCoord + L')'});
                }

                arNodeAttributes.push_back({L"x", wsXCoord});
                arNodeAttributes.push_back({L"y", wsYCoord});

                UpdateTransform(TranslateX(dX), TranslateX(dY));

                WriteNode(L"text", arNodeAttributes, StringNormalization(wsText));
        }

        void CWmfInterpretatorSvg::AddStroke(NodeAttributes &arAttributes)
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

        void CWmfInterpretatorSvg::AddFill(NodeAttributes &arAttributes)
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

        void CWmfInterpretatorSvg::UpdateTransform(double dX, double dY)
        {
                if (dX < m_oViewport.dX)
                        m_oViewport.dX = dX;

                if (dX > m_oViewport.dX + m_oViewport.dWidth)
                        m_oViewport.dWidth = dX - m_oViewport.dX;

                if (dY < m_oViewport.dY)
                        m_oViewport.dY = dY;

                if (dY > m_oViewport.dY + m_oViewport.dHeight)
                        m_oViewport.dHeight = dY - m_oViewport.dY;
        }

        void CWmfInterpretatorSvg::UpdateTransform(const TRectD &oRect)
        {
                UpdateTransform(oRect.dLeft, oRect.dTop);
                UpdateTransform(oRect.dRight, oRect.dBottom);
        }

        void CWmfInterpretatorSvg::UpdateTransform(const std::vector<TWmfPointS> &arPoints)
        {
                short shMinX = 0, shMinY = 0;
                for (const TWmfPointS& oPoint : arPoints)
                {
                        if (oPoint.x < shMinX) shMinX = oPoint.x;
                        if (oPoint.y < shMinY) shMinY = oPoint.y;
                }

                if (0 != shMinX || 0 != shMinY)
                        UpdateTransform(shMinX, shMinY);
        }

        double CWmfInterpretatorSvg::TranslateX(double dX)
        {
                if (NULL != m_pParser && NULL != m_pParser->GetTransform())
                        return dX * m_pParser->GetTransform()->M11;

                return  dX;
        }

        double CWmfInterpretatorSvg::TranslateY(double dY)
        {
                if (NULL != m_pParser && NULL != m_pParser->GetTransform())
                        return dY * m_pParser->GetTransform()->M22;

                return dY;
        }

        TPointD CWmfInterpretatorSvg::TranslatePoint(const TPointD &oPoint)
        {
                TPointD oNewPoint;

                oNewPoint.x = TranslateX(oPoint.x);
                oNewPoint.y = TranslateY(oPoint.y);

                return oNewPoint;
        }

        TRectD CWmfInterpretatorSvg::TranslateRect(const TEmfRectL &oRect)
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

        TPointD CWmfInterpretatorSvg::GetCutPos()
        {
                if (NULL != m_pParser)
                         return TranslatePoint(m_pParser->GetCurPos());

                TPointD oCurPos;

                oCurPos.x = m_oViewport.dX;
                oCurPos.y = m_oViewport.dY;

                return oCurPos;
        }
}
