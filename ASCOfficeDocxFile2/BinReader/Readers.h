/*
 * (c) Copyright Ascensio System SIA 2010-2018
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef READERS
#define READERS

#include "FileWriter.h"
#include "ReaderClasses.h"

#include "../BinWriter/BinReaderWriterDefines.h"
#include "../../XlsxSerializerCom/Writer/BinaryReader.h"

#include "../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Document.h"
#include "../../Common/DocxFormat/Source/DocxFormat/FontTable.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Numbering.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Comments.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Styles.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Footnote.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Endnote.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Settings/Settings.h"

#include "../DocWrapper/XlsxSerializer.h"

#include "../../DesktopEditor/common/ASCVariant.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include "../../DesktopEditor/common/Directory.h"

namespace BinDocxRW {

enum ETblStyleOverrideType
{
	tblstyleoverridetypeBand1Horz  =  0,
	tblstyleoverridetypeBand1Vert  =  1,
	tblstyleoverridetypeBand2Horz  =  2,
	tblstyleoverridetypeBand2Vert  =  3,
	tblstyleoverridetypeFirstCol   =  4,
	tblstyleoverridetypeFirstRow   =  5,
	tblstyleoverridetypeLastCol    =  6,
	tblstyleoverridetypeLastRow    =  7,
	tblstyleoverridetypeNeCell     =  8,
	tblstyleoverridetypeNwCell     =  9,
	tblstyleoverridetypeSeCell     = 10,
	tblstyleoverridetypeSwCell     = 11,
	tblstyleoverridetypeWholeTable = 12
};

struct RowHeight
{
	unsigned char	HRule	= 0;
	long			nHeight	= 0;
};

class SdtWraper
{
public:

	OOX::Logic::CSdt m_oSdt;
	rPr* m_oEndPr;
	rPr* m_oRPr;
	int m_nType;
	SdtWraper(int nType)
	{
		m_nType = nType;
		m_oEndPr = NULL;
		m_oRPr = NULL;
	}
	~SdtWraper()
	{
		RELEASEOBJECT(m_oEndPr)
		RELEASEOBJECT(m_oRPr)
	}
};

#define READ1_DEF(stLen, res, fReadFunction, arg) {\
	long read1defCurPos = 0;\
	while(read1defCurPos < stLen)\
	{\
		BYTE read1defType = m_oBufferedStream.GetUChar();\
		long read1defLength =  m_oBufferedStream.GetLong();\
		res = fReadFunction(read1defType, read1defLength, arg);\
		if(res == c_oSerConstants::ReadUnknown)\
		{\
			m_oBufferedStream.GetPointer(read1defLength);\
			res = c_oSerConstants::ReadOk;\
		}\
		else if(res != c_oSerConstants::ReadOk)\
			break;\
		read1defCurPos += read1defLength + 5;\
	}\
}
#define READ2_DEF(stLen, res, fReadFunction, arg) {\
	long read2defCurPos = 0;\
	while(read2defCurPos < stLen)\
	{\
		BYTE read2defType = m_oBufferedStream.GetUChar();\
		long read2defLenType =  m_oBufferedStream.GetUChar();\
		int read2defCurPosShift = 2;\
		int read2defRealLen;\
		switch(read2defLenType)\
		{\
		case c_oSerPropLenType::Null:		read2defRealLen = 0;break;\
		case c_oSerPropLenType::Byte:		read2defRealLen = 1;break;\
		case c_oSerPropLenType::Short:		read2defRealLen = 2;break;\
		case c_oSerPropLenType::Three:		read2defRealLen = 3;break;\
		case c_oSerPropLenType::Long:\
		case c_oSerPropLenType::Double:		read2defRealLen = 4;break;\
		case c_oSerPropLenType::Variable:\
			read2defRealLen = m_oBufferedStream.GetLong();\
			read2defCurPosShift += 4;\
			break;\
		case c_oSerPropLenType::Double64:	read2defRealLen = 8;break;\
		case c_oSerPropLenType::Long64:		read2defRealLen = 8;break;\
		default:res = c_oSerConstants::ErrorUnknown;break;\
		}\
		if(res == c_oSerConstants::ReadOk)\
			res = fReadFunction(read2defType, read2defRealLen, arg);\
		if(res == c_oSerConstants::ReadUnknown)\
		{\
			m_oBufferedStream.GetPointer(read2defRealLen);\
			res = c_oSerConstants::ReadOk;\
		}\
		else if(res != c_oSerConstants::ReadOk)\
			break;\
		read2defCurPos += read2defRealLen + read2defCurPosShift;\
	}\
}
#define READ_TABLE_DEF(res, fReadFunction, arg) {\
	res = m_oBufferedStream.Peek(4) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;\
	if (c_oSerConstants::ReadOk == res) {\
		long readtabledefLen = m_oBufferedStream.GetLong();\
		res = m_oBufferedStream.Peek(readtabledefLen) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;\
		if (c_oSerConstants::ReadOk == res) {\
			READ1_DEF(readtabledefLen, res, fReadFunction, arg);\
		}\
	}\
}
#define READ1_TRACKREV(type, length, poResult) \
	if(c_oSerProp_RevisionType::Author == type)\
	{\
		poResult->Author = m_oBufferedStream.GetString3(length);\
	}\
	else if(c_oSerProp_RevisionType::Date == type)\
	{\
		poResult->Date = m_oBufferedStream.GetString3(length);\
	}\
	else if(c_oSerProp_RevisionType::Id == type)\
	{\
		poResult->Id = new long(m_oBufferedStream.GetLong());\
    }\
    else if(c_oSerProp_RevisionType::UserId == type)\
    {\
        poResult->UserId = m_oBufferedStream.GetString3(length);\
    }

	class Binary_CommonReader
	{
	protected:
		NSBinPptxRW::CBinaryFileReader& m_oBufferedStream;
	public:
		Binary_CommonReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream):m_oBufferedStream(poBufferedStream)
		{
		}
	};
class Binary_CommonReader2 : public Binary_CommonReader
{
public:
	Binary_CommonReader2(NSBinPptxRW::CBinaryFileReader& poBufferedStream):Binary_CommonReader(poBufferedStream)
	{
	}
	docRGB ReadColor()
	{
		docRGB oRGB;
		oRGB.R = m_oBufferedStream.GetUChar();
		oRGB.G = m_oBufferedStream.GetUChar();
		oRGB.B = m_oBufferedStream.GetUChar();
		return oRGB;
	}
	void ReadThemeColor(int length, CThemeColor& oCThemeColor)
	{
		int res = c_oSerConstants::ReadOk;
		READ2_DEF(length, res, this->_ReadThemeColor, &oCThemeColor);
	}
	int _ReadThemeColor(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CThemeColor* pCThemeColor = static_cast<CThemeColor*>(poResult);
		switch(type)
		{
		case c_oSer_ColorThemeType::Auto:
			pCThemeColor->Auto = true;
			break;
		case c_oSer_ColorThemeType::Color:
			pCThemeColor->bColor = true;
			pCThemeColor->Color = m_oBufferedStream.GetUChar();
			break;
		case c_oSer_ColorThemeType::Tint:
			pCThemeColor->bTint = true;
			pCThemeColor->Tint = m_oBufferedStream.GetUChar();
			break;
		case c_oSer_ColorThemeType::Shade:
			pCThemeColor->bShade = true;
			pCThemeColor->Shade = m_oBufferedStream.GetUChar();
			break;
		default:
			res = c_oSerConstants::ReadUnknown;
			break;
		}
		return res;
	}
	template<typename T> int ReadTrackRevision(long length, T* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadTrackRevisionInner, poResult);
		return res;
	}
	int ReadShd(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		Shd* pShd = static_cast<Shd*>(poResult);
		switch(type)
		{
		case c_oSerShdType::Value:
			pShd->bValue = true;
			pShd->Value = m_oBufferedStream.GetUChar();
			break;
		case c_oSerShdType::Color:
			pShd->bColor = true;
			pShd->Color = ReadColor();
			break;
		case c_oSerShdType::ColorTheme:
			pShd->bThemeColor = true;
			ReadThemeColor(length, pShd->ThemeColor);
			break;
		default:
			res = c_oSerConstants::ReadUnknown;
			break;
		}
		return res;
	};
private:
	template<typename T> int ReadTrackRevisionInner(BYTE type, long length, T* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_TRACKREV(type, length, poResult)
		else
		 res = c_oSerConstants::ReadUnknown;
		return res;
	}
};
class Binary_VbaProjectTableReader : public Binary_CommonReader
{
	Writers::FileWriter&	m_oFileWriter;

public:

	Binary_VbaProjectTableReader (NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter)
			: Binary_CommonReader(poBufferedStream), m_oFileWriter(oFileWriter)
	{
	}

    int Read ();
};

class Binary_HdrFtrTableReader : public Binary_CommonReader
{
	Writers::FileWriter&	m_oFileWriter;
	CComments*				m_pComments;

	int						nCurType;
	int						nCurHeaderType;
public:
	Writers::HeaderFooterWriter& m_oHeaderFooterWriter;
	
	Binary_HdrFtrTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, CComments* pComments);
    int Read();
    int ReadHdrFtrContent	(BYTE type, long length, void* poResult);
    int ReadHdrFtrFEO		(BYTE type, long length, void* poResult);
    int ReadHdrFtrItem		(BYTE type, long length, void* poResult);
	int ReadHdrFtrItemContent(BYTE type, long length, void* poResult);
};
class Binary_rPrReader : public Binary_CommonReader
{
protected:
	Binary_CommonReader2 oBinary_CommonReader2;
	Writers::FileWriter& m_oFileWriter;
public:
	Binary_rPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter) : Binary_CommonReader(poBufferedStream), oBinary_CommonReader2(poBufferedStream), m_oFileWriter(oFileWriter)
	{
	}
	int Read(long stLen, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		READ2_DEF(stLen, res, this->ReadContent, poResult);
		return res;
	};
	int ReadContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		rPr* orPr = static_cast<rPr*>(poResult);
		switch(type)
		{
		case c_oSerProp_rPrType::Bold:
			{
				BYTE Bold = m_oBufferedStream.GetUChar();
				orPr->bBold = true;
				orPr->Bold = (0 != Bold);
				break;
			}
		case c_oSerProp_rPrType::Italic:
			{
				BYTE Italic = m_oBufferedStream.GetUChar();
				orPr->bItalic = true;
				orPr->Italic = (0 != Italic);
				break;
			}
		case c_oSerProp_rPrType::Underline:
			{
				BYTE Underline = m_oBufferedStream.GetUChar();
				orPr->bUnderline = true;
				orPr->Underline = (0 != Underline);
				break;
			}
		case c_oSerProp_rPrType::Strikeout:
			{
				BYTE Strikeout = m_oBufferedStream.GetUChar();
				orPr->bStrikeout = true;
				orPr->Strikeout = (0 != Strikeout);
				break;
			}
		case c_oSerProp_rPrType::FontAscii:
			{
                std::wstring sFontName(m_oBufferedStream.GetString3(length));
				sFontName = XmlUtils::EncodeXmlString(sFontName);
                if(!sFontName.empty())
				{
					orPr->bFontAscii = true;
					orPr->FontAscii = sFontName;
				}
				break;
			}
		case c_oSerProp_rPrType::FontHAnsi:
			{
                std::wstring sFontName(m_oBufferedStream.GetString3(length));
				sFontName = XmlUtils::EncodeXmlString(sFontName);
                if(!sFontName.empty())
				{
					orPr->bFontHAnsi = true;
					orPr->FontHAnsi = sFontName;
				}
				break;
			}
		case c_oSerProp_rPrType::FontCS:
			{
                std::wstring sFontName(m_oBufferedStream.GetString3(length));
				sFontName = XmlUtils::EncodeXmlString(sFontName);
                if(!sFontName.empty())
				{
					orPr->bFontCS = true;
					orPr->FontCS = sFontName;
				}
				break;
			}
		case c_oSerProp_rPrType::FontAE:
			{
                std::wstring sFontName(m_oBufferedStream.GetString3(length));
				sFontName = XmlUtils::EncodeXmlString(sFontName);
                if(!sFontName.empty())
				{
					orPr->bFontAE = true;
					orPr->FontAE = sFontName;
				}
				break;
			}
		case c_oSerProp_rPrType::FontSize:
			{
				long nFontSize = m_oBufferedStream.GetLong();
				orPr->bFontSize = true;
				orPr->FontSize = nFontSize;
				break;
			}
		case c_oSerProp_rPrType::Color:
			{
				orPr->bColor = true;
				orPr->Color = oBinary_CommonReader2.ReadColor();
				break;
			}
		case c_oSerProp_rPrType::VertAlign:
			{
				BYTE VertAlign = m_oBufferedStream.GetUChar();
				orPr->bVertAlign = true;
				orPr->VertAlign = VertAlign;
				break;
			}
		case c_oSerProp_rPrType::HighLight:
			{
				orPr->bHighLight = true;
				orPr->HighLight = oBinary_CommonReader2.ReadColor();
				break;
			}
		case c_oSerProp_rPrType::HighLightTyped:
			{
				BYTE byteHighLightTyped = m_oBufferedStream.GetUChar();
				switch(byteHighLightTyped)
				{
				case c_oSer_ColorType::None:orPr->bHighLight = false;break;
				}
				break;
			}
		case c_oSerProp_rPrType::Shd:
			{
				Shd oShd;
				READ2_DEF(length, res, oBinary_CommonReader2.ReadShd, &oShd);
				orPr->bShd = true;
				orPr->Shd = oShd.ToString();
				break;
			}
		case c_oSerProp_rPrType::RStyle:
			{
                std::wstring sRStyle(m_oBufferedStream.GetString3(length));
				orPr->bRStyle = true;
				orPr->RStyle = sRStyle;
			}
			break;
		case c_oSerProp_rPrType::Spacing:
			{
				orPr->bSpacing = true;
				orPr->Spacing = m_oBufferedStream.GetDouble();
			}
			break;
		case c_oSerProp_rPrType::DStrikeout:
			{
				orPr->bDStrikeout = true;
				orPr->DStrikeout = (0 != m_oBufferedStream.GetUChar());
			}
			break;
		case c_oSerProp_rPrType::Caps:
			{
				orPr->bCaps = true;
				orPr->Caps = (0 != m_oBufferedStream.GetUChar());
			}
			break;
		case c_oSerProp_rPrType::SmallCaps:
			{
				orPr->bSmallCaps = true;
				orPr->SmallCaps = (0 != m_oBufferedStream.GetUChar());
			}
			break;
		case c_oSerProp_rPrType::Position:
			{
				orPr->bPosition = true;
				orPr->Position = m_oBufferedStream.GetDouble();
			}
			break;
		case c_oSerProp_rPrType::FontHint:
			{
				orPr->bFontHint = true;
				orPr->FontHint = m_oBufferedStream.GetUChar();
			}
			break;
		case c_oSerProp_rPrType::BoldCs:
			{
				orPr->bBoldCs = true;
				orPr->BoldCs = m_oBufferedStream.GetBool();
			}
			break;
		case c_oSerProp_rPrType::ItalicCs:
			{
				orPr->bItalicCs = true;
				orPr->ItalicCs = m_oBufferedStream.GetBool();
			}
			break;
		case c_oSerProp_rPrType::FontSizeCs:
			{
				orPr->bFontSizeCs = true;
				orPr->FontSizeCs = m_oBufferedStream.GetLong();
			}
			break;
		case c_oSerProp_rPrType::Cs:
			{
				orPr->bCs = true;
				orPr->Cs = m_oBufferedStream.GetBool();
			}
			break;
		case c_oSerProp_rPrType::Rtl:
			{
				orPr->bRtl = true;
				orPr->Rtl = m_oBufferedStream.GetBool();
			}
			break;
		case c_oSerProp_rPrType::Lang:
			{
				orPr->bLang = true;
				orPr->Lang = m_oBufferedStream.GetString3(length);
			}
			break;
		case c_oSerProp_rPrType::LangBidi:
			{
				orPr->bLangBidi = true;
				orPr->LangBidi = m_oBufferedStream.GetString3(length);
			}
			break;
		case c_oSerProp_rPrType::LangEA:
			{
				orPr->bLangEA = true;
				orPr->LangEA = m_oBufferedStream.GetString3(length);
			}
			break;
		case c_oSerProp_rPrType::ColorTheme:
			{
				orPr->bThemeColor = true;
				oBinary_CommonReader2.ReadThemeColor(length, orPr->ThemeColor);
			}
			break;
		case c_oSerProp_rPrType::Vanish:
			{
				orPr->bVanish = true;
				orPr->Vanish = m_oBufferedStream.GetBool();
			}
			break;
		case c_oSerProp_rPrType::TextOutline:
			{
				if (length > 0)
				{
					long nCurPos = m_oBufferedStream.GetPos();
                    std::wstring strXml;
                    HRESULT hRes = m_oFileWriter.m_pDrawingConverter->GetRecordXml(nCurPos, length, XMLWRITER_RECORD_TYPE_TEXT_OUTLINE, XMLWRITER_DOC_TYPE_WORDART, strXml);
                    if (S_OK == hRes)
                        orPr->Outline = strXml;
					m_oBufferedStream.Seek(nCurPos + length);
				}
			}
			break;
		case c_oSerProp_rPrType::TextFill:
			{
				if (length > 0)
				{
					long nCurPos = m_oBufferedStream.GetPos();
                    std::wstring strXml;
                    HRESULT hRes = m_oFileWriter.m_pDrawingConverter->GetRecordXml(nCurPos, length, XMLWRITER_RECORD_TYPE_TEXT_FILL, XMLWRITER_DOC_TYPE_WORDART, strXml);
                    if (S_OK == hRes)
                        orPr->Fill = L"<w14:textFill>" + strXml + L"</w14:textFill>";
					m_oBufferedStream.Seek(nCurPos + length);
				}
			}
			break;
		case c_oSerProp_rPrType::Del:
			{
				TrackRevision oDel;
				oBinary_CommonReader2.ReadTrackRevision(length, &oDel);
				orPr->Del = oDel.ToString(L"w:del");
			}
			break;
		case c_oSerProp_rPrType::Ins:
			{
				TrackRevision oIns;
				oBinary_CommonReader2.ReadTrackRevision(length, &oIns);
				orPr->Ins = oIns.ToString(L"w:ins");
			}
			break;
		case c_oSerProp_rPrType::MoveFrom:
			{
				TrackRevision oMoveFrom;
				oBinary_CommonReader2.ReadTrackRevision(length, &oMoveFrom);
				orPr->MoveFrom = oMoveFrom.ToString(L"w:moveFrom");
			}
			break;
		case c_oSerProp_rPrType::MoveTo:
			{
				TrackRevision oMoveTo;
				oBinary_CommonReader2.ReadTrackRevision(length, &oMoveTo);
				orPr->MoveTo = oMoveTo.ToString(L"w:moveTo");
			}
			break;
		case c_oSerProp_rPrType::rPrChange:
			{
				TrackRevision oRPrChange;
				READ1_DEF(length, res, this->ReadrPrChange, &oRPrChange);
				orPr->rPrChange = oRPrChange.ToString(L"w:rPrChange");
			}
			break;
		default:
			res = c_oSerConstants::ReadUnknown;
			break;
		}
		return res;
	}
	int ReadrPrChange(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pTrackRevision)
		else if(c_oSerProp_RevisionType::rPrChange == type)
		{
			pTrackRevision->RPr = new rPr(m_oFileWriter.m_oFontTableWriter.m_mapFonts);
			res = Read(length, pTrackRevision->RPr);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
};
class Binary_pPrReader : public Binary_CommonReader
{
private:
	Writers::FontTableWriter& m_oFontTableWriter;
public:
	Binary_CommonReader2		oBinary_CommonReader2;
	Binary_rPrReader			oBinary_rPrReader;
	Binary_HdrFtrTableReader	oBinary_HdrFtrTableReader;

	Writers::FileWriter&		m_oFileWriter;
	bool						bDoNotWriteNullProp;
	long						m_nCurNumId;
	long						m_nCurLvl;

	Binary_pPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter):
		m_oFontTableWriter(oFileWriter.m_oFontTableWriter), Binary_CommonReader(poBufferedStream), oBinary_CommonReader2(poBufferedStream), oBinary_rPrReader(poBufferedStream, oFileWriter), oBinary_HdrFtrTableReader(poBufferedStream, oFileWriter, oFileWriter.m_pComments), m_oFileWriter(oFileWriter)
	{
		bDoNotWriteNullProp = false;
		m_nCurNumId = -1;
		m_nCurLvl = -1;
	}
	int Read(long stLen, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		READ2_DEF(stLen, res, this->ReadContent, poResult);
		return res;
	};
	int ReadContent( BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		switch(type)
		{
		case c_oSerProp_pPrType::ContextualSpacing:
			{
				BYTE contextualSpacing = m_oBufferedStream.GetUChar();
				if(0 != contextualSpacing)
                    pCStringWriter->WriteString(std::wstring(L"<w:contextualSpacing w:val=\"true\"/>"));
				else if(false == bDoNotWriteNullProp)
                    pCStringWriter->WriteString(std::wstring(L"<w:contextualSpacing w:val=\"false\"/>"));
            }break;
		case c_oSerProp_pPrType::Ind:
			{
				XmlUtils::CStringWriter oTempWriter;
				READ2_DEF(length, res, this->ReadInd, &oTempWriter);
				if(oTempWriter.GetCurSize() > 0)
				{
                    pCStringWriter->WriteString(std::wstring(L"<w:ind"));
					pCStringWriter->Write(oTempWriter);
                    pCStringWriter->WriteString(std::wstring(L"/>"));
				}
				break;
			}
		case c_oSerProp_pPrType::Jc:
			{
				BYTE jc = m_oBufferedStream.GetUChar();
				switch(jc)
				{
                case align_Right: pCStringWriter->WriteString(std::wstring(L"<w:jc w:val=\"right\" />"));break;
                case align_Left: pCStringWriter->WriteString(std::wstring(L"<w:jc w:val=\"left\" />"));break;
                case align_Center: pCStringWriter->WriteString(std::wstring(L"<w:jc w:val=\"center\" />"));break;
                case align_Justify: pCStringWriter->WriteString(std::wstring(L"<w:jc w:val=\"both\" />"));break;
				}				
            }break;
		case c_oSerProp_pPrType::KeepLines:
			{
				BYTE KeepLines = m_oBufferedStream.GetUChar();
				if(0 != KeepLines)
                    pCStringWriter->WriteString(std::wstring(L"<w:keepLines/>"));
				else if(false == bDoNotWriteNullProp)
                    pCStringWriter->WriteString(std::wstring(L"<w:keepLines w:val=\"false\"/>"));
            }break;
		case c_oSerProp_pPrType::KeepNext:
			{
				BYTE KeepNext = m_oBufferedStream.GetUChar();
				if(0 != KeepNext)
                    pCStringWriter->WriteString(std::wstring(L"<w:keepNext/>"));
				else if(false == bDoNotWriteNullProp)
                    pCStringWriter->WriteString(std::wstring(L"<w:keepNext w:val=\"false\"/>"));
            }break;
		case c_oSerProp_pPrType::PageBreakBefore:
			{
				BYTE pageBreakBefore = m_oBufferedStream.GetUChar();
				if(0 != pageBreakBefore)
                    pCStringWriter->WriteString(std::wstring(L"<w:pageBreakBefore/>"));
				else if(false == bDoNotWriteNullProp)
                    pCStringWriter->WriteString(std::wstring(L"<w:pageBreakBefore w:val=\"false\"/>"));
				break;
			}
		case c_oSerProp_pPrType::Spacing:
			{
				Spacing oSpacing;
				READ2_DEF(length, res, this->ReadSpacing, &oSpacing);
				if(oSpacing.bLine || oSpacing.bAfter || oSpacing.bAfterAuto || oSpacing.bBefore || oSpacing.bBeforeAuto)
				{
                    pCStringWriter->WriteString(std::wstring(L"<w:spacing"));
					BYTE bLineRule = linerule_Auto;
					//проверяется bLine, а не bLineRule чтобы всегда писать LineRule, если есть w:line
					if(oSpacing.bLine)
					{
						if(oSpacing.bLineRule)
							bLineRule = oSpacing.LineRule;
                        std::wstring sLineRule;
						switch(oSpacing.LineRule)
						{
							case linerule_AtLeast:sLineRule = _T(" w:lineRule=\"atLeast\"");break;
							case linerule_Exact:sLineRule = _T(" w:lineRule=\"exact\"");break;
							default:sLineRule = _T(" w:lineRule=\"auto\"");break;
						}
						pCStringWriter->WriteString(sLineRule);
					}
					if(oSpacing.bLine)
					{
                        std::wstring sLine;
						if(linerule_Auto == bLineRule)
						{
							long nLine = SerializeCommon::Round(oSpacing.Line * 240);
                            sLine = L" w:line=\"" + std::to_wstring(nLine) + L"\"";
						}
						else
						{
							long nLine = SerializeCommon::Round( g_dKoef_mm_to_twips * oSpacing.Line);
                            sLine = L" w:line=\"" + std::to_wstring(nLine) + L"\"";
						}
						pCStringWriter->WriteString(sLine);
					}
					if(oSpacing.bAfter)
					{
						long After = SerializeCommon::Round( g_dKoef_mm_to_twips * oSpacing.After);
                        std::wstring sAfter = L" w:after=\"" + std::to_wstring(After) + L"\"";
						pCStringWriter->WriteString(sAfter);
					}
					if(oSpacing.bAfterAuto)
					{
						if(true == oSpacing.AfterAuto)
                            pCStringWriter->WriteString(std::wstring(L" w:afterAutospacing=\"1\""));
						else
                            pCStringWriter->WriteString(std::wstring(L" w:afterAutospacing=\"0\""));
					}
					if(oSpacing.bBefore)
					{
						long Before = SerializeCommon::Round( g_dKoef_mm_to_twips * oSpacing.Before);
                        std::wstring sBefore = L" w:before=\"" + std::to_wstring(Before) + L"\"";
						pCStringWriter->WriteString(sBefore);
					}
					if(oSpacing.bBeforeAuto)
					{
						if(true == oSpacing.BeforeAuto)
                            pCStringWriter->WriteString(std::wstring(L" w:beforeAutospacing=\"1\""));
						else
                            pCStringWriter->WriteString(std::wstring(L" w:beforeAutospacing=\"0\""));
					}
                    pCStringWriter->WriteString(std::wstring(L"/>"));
				}
				break;
			}
		case c_oSerProp_pPrType::Shd:
			{
				Shd oShd;
				READ2_DEF(length, res, oBinary_CommonReader2.ReadShd, &oShd);
				pCStringWriter->WriteString(oShd.ToString());
				break;
			}
		case c_oSerProp_pPrType::WidowControl:
			{
				BYTE WidowControl = m_oBufferedStream.GetUChar();
				if(0 != WidowControl)
				{
					if(false == bDoNotWriteNullProp)
                        pCStringWriter->WriteString(std::wstring(L"<w:widowControl/>"));
				}
				else
                    pCStringWriter->WriteString(std::wstring(L"<w:widowControl w:val=\"off\" />"));
				break;
			}
		case c_oSerProp_pPrType::Tab:
			{
				Tabs oTabs;
				READ2_DEF(length, res, this->ReadTabs, &oTabs);
				size_t nLen = oTabs.m_aTabs.size();
				if(nLen > 0)
				{
                    pCStringWriter->WriteString(std::wstring(L"<w:tabs>"));
					for(size_t i = 0; i < nLen; ++i)
					{
						Tab& oTab = oTabs.m_aTabs[i];
						long nTab = SerializeCommon::Round( g_dKoef_mm_to_twips * oTab.Pos);
                        std::wstring sVal;
						switch(oTab.Val)
						{
                            case g_tabtype_right:   sVal = L"right";   break;
                            case g_tabtype_center:  sVal = L"center";  break;
                            case g_tabtype_clear:   sVal = L"clear";   break;
                            default:                sVal = L"left";    break;
						}
						pCStringWriter->WriteString(L"<w:tab w:val=\"" + sVal + L"\" w:pos=\"" + std::to_wstring(nTab) + L"\"");
						if (oTab.bLeader)
						{
							std::wstring sLeader;
							switch(oTab.Leader)
							{
							case SimpleTypes::tabtlcDot        : sLeader = (L"dot");break;
							case SimpleTypes::tabtlcHeavy      : sLeader = (L"heavy");break;
							case SimpleTypes::tabtlcHyphen     : sLeader = (L"hyphen");break;
							case SimpleTypes::tabtlcMiddleDot  : sLeader = (L"middleDot");break;
							case SimpleTypes::tabtlcNone       : sLeader = (L"none");break;
							case SimpleTypes::tabtlcUnderscore : sLeader = (L"underscore");break;
							default                : sLeader = (L"none");break;
							}
							pCStringWriter->WriteString(L" w:leader=\"" + sLeader + L"\"");
						}
						pCStringWriter->WriteString(L"/>");
					}
                    pCStringWriter->WriteString(std::wstring(L"</w:tabs>"));
				}				
            }break;
		case c_oSerProp_pPrType::ParaStyle:
			{
                std::wstring sStyleName(m_oBufferedStream.GetString3(length));
				sStyleName = XmlUtils::EncodeXmlString(sStyleName);
                pCStringWriter->WriteString(L"<w:pStyle w:val=\"" + sStyleName + L"\" />");
            }break;
		case c_oSerProp_pPrType::numPr:
            {
                pCStringWriter->WriteString(std::wstring(L"<w:numPr>"));
				READ2_DEF(length, res, this->ReadNumPr, poResult);
                pCStringWriter->WriteString(std::wstring(L"</w:numPr>"));
            }break;
		case c_oSerProp_pPrType::pPr_rPr:
			{
				rPr orPr(m_oFontTableWriter.m_mapFonts);
				res = oBinary_rPrReader.Read(length, &orPr);
				if(orPr.IsNoEmpty())
				orPr.Write(pCStringWriter);
				
			}break;
		case c_oSerProp_pPrType::pBdr:
			{
				docBorders odocBorders;
				READ1_DEF(length, res, this->ReadBorders, &odocBorders);
                if(false == odocBorders.IsEmpty())
				{
                    pCStringWriter->WriteString(std::wstring(_T("<w:pBdr>")));
					odocBorders.Write(pCStringWriter, false);
                    pCStringWriter->WriteString(std::wstring(_T("</w:pBdr>")));
				}				
            }break;
		case c_oSerProp_pPrType::FramePr:
			{
				CFramePr oFramePr;
				READ2_DEF(length, res, this->ReadFramePr, &oFramePr);
                if(false == oFramePr.IsEmpty())
					oFramePr.Write(*pCStringWriter);				
            }break;
		case c_oSerProp_pPrType::pPrChange:
			{
				TrackRevision oPPrChange;
				READ1_DEF(length, res, this->ReadPPrChange, &oPPrChange);
				oPPrChange.Write(pCStringWriter, _T("w:pPrChange"));				
            }break;
        case c_oSerProp_pPrType::SectPr:
			{
				SectPr oSectPr;
				READ1_DEF(length, res, this->Read_SecPr, &oSectPr);
                pCStringWriter->WriteString(std::wstring(_T("<w:sectPr>")));
				pCStringWriter->WriteString(oSectPr.Write());
                pCStringWriter->WriteString(std::wstring(_T("</w:sectPr>")));
            }break;
		case c_oSerProp_pPrType::outlineLvl:
			{
				long outlineLvl = m_oBufferedStream.GetLong();
				pCStringWriter->WriteString(L"<w:outlineLvl w:val=\"" + std::to_wstring(outlineLvl) + L"\"/>");
			}break;
		default:
			res = c_oSerConstants::ReadUnknown;
			break;
		}
		return res;
	};
	int ReadPPrChange(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pPPrChange = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pPPrChange)
		else if(c_oSerProp_RevisionType::pPrChange == type)
		{
			pPPrChange->PPr = new XmlUtils::CStringWriter();
			res = Read(length, pPPrChange->PPr);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadInd(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		switch(type)
		{
		case c_oSerProp_pPrType::Ind_Left:
			{
				double dIndLeft = m_oBufferedStream.GetDouble();
				long nIndLeft = SerializeCommon::Round(dIndLeft * g_dKoef_mm_to_twips);

                pCStringWriter->WriteString(L" w:left=\"" + std::to_wstring(nIndLeft) + L"\"");
				break;
			}
		case c_oSerProp_pPrType::Ind_Right:
			{
				double dIndRight = m_oBufferedStream.GetDouble();
				long nIndRight = SerializeCommon::Round(dIndRight * g_dKoef_mm_to_twips);

                pCStringWriter->WriteString(L" w:right=\"" + std::to_wstring(nIndRight) + L"\"");
				break;
			}
		case c_oSerProp_pPrType::Ind_FirstLine:
			{
				double dIndFirstLine = m_oBufferedStream.GetDouble();
				long nIndFirstLine = SerializeCommon::Round(dIndFirstLine * g_dKoef_mm_to_twips);
                std::wstring sIndFirstLine;
				if(nIndFirstLine > 0)
                    sIndFirstLine = L" w:firstLine =\"" + std::to_wstring(nIndFirstLine) + L"\"";
				else
                    sIndFirstLine = L" w:hanging=\"" + std::to_wstring(-nIndFirstLine) + L"\"";
				pCStringWriter->WriteString(sIndFirstLine);
				break;
			}
		default:
			res = c_oSerConstants::ReadUnknown;
			break;
		}
		return res;
    }
	int ReadSpacing(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		Spacing* pSpacing = static_cast<Spacing*>(poResult);
		switch(type)
		{
		case c_oSerProp_pPrType::Spacing_Line:
			pSpacing->bLine = true;
			pSpacing->Line = m_oBufferedStream.GetDouble();
			break;
		case c_oSerProp_pPrType::Spacing_LineRule:
			pSpacing->bLineRule = true;
			pSpacing->LineRule = m_oBufferedStream.GetUChar();
			break;
		case c_oSerProp_pPrType::Spacing_Before:
			pSpacing->bBefore = true;
			pSpacing->Before = m_oBufferedStream.GetDouble();
			break;
		case c_oSerProp_pPrType::Spacing_After:
			pSpacing->bAfter = true;
			pSpacing->After = m_oBufferedStream.GetDouble();
			break;
		case c_oSerProp_pPrType::Spacing_BeforeAuto:
			pSpacing->bBeforeAuto = true;
			pSpacing->BeforeAuto = (0 != m_oBufferedStream.GetUChar()) ? true : false;
			break;
		case c_oSerProp_pPrType::Spacing_AfterAuto:
			pSpacing->bAfterAuto = true;
			pSpacing->AfterAuto = (0 != m_oBufferedStream.GetUChar()) ? true : false;
			break;
		default:
			res = c_oSerConstants::ReadUnknown;
			break;
		}
		return res;
    }
	int ReadTabs(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		Tabs* poTabs = static_cast<Tabs*>(poResult);
		if(c_oSerProp_pPrType::Tab_Item == type)
		{
			Tab oTabItem;
			READ2_DEF(length, res, this->ReadTabItem, &oTabItem);
			poTabs->m_aTabs.push_back(oTabItem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadTabItem(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		Tab* poTabItem = static_cast<Tab*>(poResult);
		if(c_oSerProp_pPrType::Tab_Item_Val == type)
			poTabItem->Val = m_oBufferedStream.GetUChar();
		else if(c_oSerProp_pPrType::Tab_Item_Pos == type)
			poTabItem->Pos = m_oBufferedStream.GetDouble();
		else if(c_oSerProp_pPrType::Tab_Item_Leader == type)
		{
			poTabItem->bLeader = true;
			poTabItem->Leader = m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadNumPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if(c_oSerProp_pPrType::numPr_lvl == type)
		{
			long nLvl = m_oBufferedStream.GetLong();
			m_nCurLvl = nLvl;

            pCStringWriter->WriteString(L"<w:ilvl w:val=\"" + std::to_wstring(nLvl) + L"\"/>");
		}
		else if(c_oSerProp_pPrType::numPr_id == type)
		{
			long nnumId = m_oBufferedStream.GetLong();
			m_nCurNumId = nnumId;

            pCStringWriter->WriteString(L"<w:numId w:val=\"" + std::to_wstring(nnumId) + L"\"/>");
		}
		else if(c_oSerProp_pPrType::numPr_Ins == type)
		{
			TrackRevision Ins;
			oBinary_CommonReader2.ReadTrackRevision(length, &Ins);
			Ins.Write(pCStringWriter, _T("w:ins"));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadBorders(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docBorders* pdocBorders = static_cast<docBorders*>(poResult);
		if( c_oSerBordersType::left == type )
		{
			pdocBorders->bLeft = true;
			READ2_DEF(length, res, this->ReadBorder, &pdocBorders->oLeft);
		}
		else if( c_oSerBordersType::top == type )
		{
			pdocBorders->bTop = true;
			READ2_DEF(length, res, this->ReadBorder, &pdocBorders->oTop);
		}
		else if( c_oSerBordersType::right == type )
		{
			pdocBorders->bRight = true;
			READ2_DEF(length, res, this->ReadBorder, &pdocBorders->oRight);
		}
		else if( c_oSerBordersType::bottom == type )
		{
			pdocBorders->bBottom = true;
			READ2_DEF(length, res, this->ReadBorder, &pdocBorders->oBottom);
		}
		else if( c_oSerBordersType::insideV == type )
		{
			pdocBorders->bInsideV = true;
			READ2_DEF(length, res, this->ReadBorder, &pdocBorders->oInsideV);
		}
		else if( c_oSerBordersType::insideH == type )
		{
			pdocBorders->bInsideH = true;
			READ2_DEF(length, res, this->ReadBorder, &pdocBorders->oInsideH);
		}
		else if( c_oSerBordersType::between == type )
		{
			pdocBorders->bBetween = true;
			READ2_DEF(length, res, this->ReadBorder, &pdocBorders->oBetween);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadBorder(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docBorder* odocBorder = static_cast<docBorder*>(poResult);
		if( c_oSerBorderType::Color == type )
		{
			odocBorder->bColor = true;
			odocBorder->Color = oBinary_CommonReader2.ReadColor();
		}
		else if( c_oSerBorderType::Space == type )
		{
			odocBorder->bSpace = true;
			odocBorder->Space = m_oBufferedStream.GetDouble();
		}
		else if( c_oSerBorderType::Size == type )
		{
			odocBorder->bSize = true;
			odocBorder->Size = m_oBufferedStream.GetDouble();
		}
		else if( c_oSerBorderType::Value == type )
		{
			odocBorder->bValue = true;
			odocBorder->Value = m_oBufferedStream.GetUChar();
		}
		else if( c_oSerBorderType::ColorTheme == type )
		{
			odocBorder->bThemeColor = true;
			oBinary_CommonReader2.ReadThemeColor(length, odocBorder->ThemeColor);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadFramePr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CFramePr* oFramePr = static_cast<CFramePr*>(poResult);
		if( c_oSer_FramePrType::DropCap == type )
		{
			oFramePr->bDropCap = true;
			oFramePr->DropCap = m_oBufferedStream.GetUChar();
		}
		else if( c_oSer_FramePrType::H == type )
		{
			oFramePr->bH = true;
			oFramePr->H = m_oBufferedStream.GetLong();
		}
		else if( c_oSer_FramePrType::HAnchor == type )
		{
			oFramePr->bHAnchor = true;
			oFramePr->HAnchor = m_oBufferedStream.GetUChar();
		}
		else if( c_oSer_FramePrType::HRule == type )
		{
			oFramePr->bHRule = true;
			oFramePr->HRule = m_oBufferedStream.GetUChar();
		}
		else if( c_oSer_FramePrType::HSpace == type )
		{
			oFramePr->bHSpace = true;
			oFramePr->HSpace = m_oBufferedStream.GetLong();
		}
		else if( c_oSer_FramePrType::Lines == type )
		{
			oFramePr->bLines = true;
			oFramePr->Lines = m_oBufferedStream.GetLong();
		}
		else if( c_oSer_FramePrType::VAnchor == type )
		{
			oFramePr->bVAnchor = true;
			oFramePr->VAnchor = m_oBufferedStream.GetUChar();
		}
		else if( c_oSer_FramePrType::VSpace == type )
		{
			oFramePr->bVSpace = true;
			oFramePr->VSpace = m_oBufferedStream.GetLong();
		}
		else if( c_oSer_FramePrType::W == type )
		{
			oFramePr->bW = true;
			oFramePr->W = m_oBufferedStream.GetLong();
		}
		else if( c_oSer_FramePrType::Wrap == type )
		{
			oFramePr->bWrap = true;
			oFramePr->Wrap = m_oBufferedStream.GetUChar();
		}
		else if( c_oSer_FramePrType::X == type )
		{
			oFramePr->bX = true;
			oFramePr->X = m_oBufferedStream.GetLong();
		}
		else if( c_oSer_FramePrType::XAlign == type )
		{
			oFramePr->bXAlign = true;
			oFramePr->XAlign = m_oBufferedStream.GetUChar();
		}
		else if( c_oSer_FramePrType::Y == type )
		{
			oFramePr->bY = true;
			oFramePr->Y = m_oBufferedStream.GetLong();
		}
		else if( c_oSer_FramePrType::YAlign == type )
		{
			oFramePr->bYAlign = true;
			oFramePr->YAlign = m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
    int Read_SecPr(BYTE type, long length, void* poResult)
    {
		SectPr* pSectPr = static_cast<SectPr*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerProp_secPrType::pgSz == type )
		{
			READ2_DEF(length, res, this->Read_pgSz, poResult);
		}
		else if( c_oSerProp_secPrType::pgMar == type )
		{
			READ2_DEF(length, res, this->Read_pgMar, poResult);
		}
		else if( c_oSerProp_secPrType::setting == type )
		{
			READ2_DEF(length, res, this->Read_pgSetting, poResult);
		}
		else if( c_oSerProp_secPrType::headers == type )
		{
			READ1_DEF(length, res, this->Read_pgHeader, poResult);
		}
		else if( c_oSerProp_secPrType::footers == type )
		{
			READ1_DEF(length, res, this->Read_pgFooter, poResult);
		}
		else if( c_oSerProp_secPrType::pageNumType == type )
		{
			READ1_DEF(length, res, this->Read_pageNumType, poResult);
		}
		else if( c_oSerProp_secPrType::sectPrChange == type )
		{
			TrackRevision sectPrChange;
			READ1_DEF(length, res, this->ReadSectPrChange, &sectPrChange);
			pSectPr->sectPrChange = sectPrChange.ToString(_T("w:sectPrChange"));
		}
		else if( c_oSerProp_secPrType::cols == type )
		{
			OOX::Logic::CColumns oCols;
			READ1_DEF(length, res, this->ReadCols, &oCols);
			pSectPr->cols = oCols.toXML();
		}
		else if( c_oSerProp_secPrType::pgBorders == type )
		{
			OOX::Logic::CPageBorders pgBorders;
			READ1_DEF(length, res, this->ReadPageBorders, &pgBorders);
			pSectPr->pgBorders = pgBorders.toXML();
		}
		else if( c_oSerProp_secPrType::footnotePr == type )
		{
			OOX::Logic::CFtnProps oFtnProps;
			READ1_DEF(length, res, this->ReadFootnotePr, &oFtnProps);
			pSectPr->footnotePr = oFtnProps.toXML();
		}
		else if( c_oSerProp_secPrType::endnotePr == type )
		{
			OOX::Logic::CEdnProps oEdnProps;
			READ1_DEF(length, res, this->ReadEndnotePr, &oEdnProps);
			pSectPr->endnotePr = oEdnProps.toXML();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadFootnotePr(BYTE type, long length, void* poResult)
	{
		OOX::Logic::CFtnProps* pFtnProps = static_cast<OOX::Logic::CFtnProps*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerNotes::PrFmt == type )
		{
			pFtnProps->m_oNumFmt.Init();
			READ1_DEF(length, res, this->ReadNumFmt, pFtnProps->m_oNumFmt.GetPointer());
		}
		else if( c_oSerNotes::PrRestart == type )
		{
			pFtnProps->m_oNumRestart.Init();
			pFtnProps->m_oNumRestart->m_oVal.Init();
			pFtnProps->m_oNumRestart->m_oVal->SetValue((SimpleTypes::ERestartNumber)m_oBufferedStream.GetUChar());
		}
		else if( c_oSerNotes::PrStart == type )
		{
			pFtnProps->m_oNumStart.Init();
			pFtnProps->m_oNumStart->m_oVal.Init();
			pFtnProps->m_oNumStart->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if( c_oSerNotes::PrFntPos == type )
		{
			pFtnProps->m_oPos.Init();
			pFtnProps->m_oPos->m_oVal.Init();
			pFtnProps->m_oPos->m_oVal->SetValue((SimpleTypes::EFtnPos)m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadEndnotePr(BYTE type, long length, void* poResult)
	{
		OOX::Logic::CEdnProps* pEdnProps = static_cast<OOX::Logic::CEdnProps*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerNotes::PrFmt == type )
		{
			pEdnProps->m_oNumFmt.Init();
			READ1_DEF(length, res, this->ReadNumFmt, pEdnProps->m_oNumFmt.GetPointer());
		}
		else if( c_oSerNotes::PrRestart == type )
		{
			pEdnProps->m_oNumRestart.Init();
			pEdnProps->m_oNumRestart->m_oVal.Init();
			pEdnProps->m_oNumRestart->m_oVal->SetValue((SimpleTypes::ERestartNumber)m_oBufferedStream.GetUChar());
		}
		else if( c_oSerNotes::PrStart == type )
		{
			pEdnProps->m_oNumStart.Init();
			pEdnProps->m_oNumStart->m_oVal.Init();
			pEdnProps->m_oNumStart->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if( c_oSerNotes::PrEndPos == type )
		{
			pEdnProps->m_oPos.Init();
			pEdnProps->m_oPos->m_oVal.Init();
			pEdnProps->m_oPos->m_oVal->SetValue((SimpleTypes::EEdnPos)m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadNumFmt(BYTE type, long length, void* poResult)
	{
		ComplexTypes::Word::CNumFmt* pNumFmt = static_cast<ComplexTypes::Word::CNumFmt*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerNumTypes::NumFmtVal == type )
		{
			pNumFmt->m_oVal.Init();
			pNumFmt->m_oVal->SetValue((SimpleTypes::ENumberFormat)m_oBufferedStream.GetUChar());
		}
		else if( c_oSerNumTypes::NumFmtFormat == type )
		{
			pNumFmt->m_sFormat.Init();
            pNumFmt->m_sFormat->append(m_oBufferedStream.GetString3(length));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSectPrChange(BYTE type, long length, void* poResult)
	{
		TrackRevision* sectPrChange = static_cast<TrackRevision*>(poResult);
		int res = c_oSerConstants::ReadOk;
		READ1_TRACKREV(type, length, sectPrChange)
		else if( c_oSerProp_RevisionType::sectPrChange == type )
		{
			sectPrChange->sectPr = new SectPr();
			READ1_DEF(length, res, this->Read_SecPr, sectPrChange->sectPr);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int Read_pgSz(BYTE type, long length, void* poResult)
	{
		SectPr* pSectPr = static_cast<SectPr*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSer_pgSzType::Orientation == type )
		{
			pSectPr->cOrientation = m_oBufferedStream.GetUChar();
		}
		else if( c_oSer_pgSzType::W == type )
		{
			pSectPr->W = m_oBufferedStream.GetDouble();
		}
		else if( c_oSer_pgSzType::H == type )
		{
			pSectPr->H = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int Read_pgMar(BYTE type, long length, void* poResult)
	{
		SectPr* pSectPr = static_cast<SectPr*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSer_pgMarType::Left == type )
		{
			pSectPr->Left = m_oBufferedStream.GetDouble();
		}
		else if( c_oSer_pgMarType::Top == type )
		{
			pSectPr->Top = m_oBufferedStream.GetDouble();
		}
		else if( c_oSer_pgMarType::Right == type )
		{
			pSectPr->Right = m_oBufferedStream.GetDouble();
		}
		else if( c_oSer_pgMarType::Bottom == type )
		{
			pSectPr->Bottom = m_oBufferedStream.GetDouble();
		}
		else if( c_oSer_pgMarType::Header == type )
		{
			pSectPr->bHeader = true;
			pSectPr->Header = m_oBufferedStream.GetDouble();
		}
		else if( c_oSer_pgMarType::Footer == type )
		{
			pSectPr->bFooter = true;
			pSectPr->Footer = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int Read_pgSetting(BYTE type, long length, void* poResult)
	{
		SectPr* pSectPr = static_cast<SectPr*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerProp_secPrSettingsType::titlePg == type )
		{
			pSectPr->bTitlePg = true;
			pSectPr->TitlePg = m_oBufferedStream.GetBool();
		}
		else if( c_oSerProp_secPrSettingsType::EvenAndOddHeaders == type )
		{
			pSectPr->bEvenAndOddHeaders = true;
			pSectPr->EvenAndOddHeaders = m_oBufferedStream.GetBool();
		}
		else if( c_oSerProp_secPrSettingsType::SectionType == type )
		{
			pSectPr->bSectionType = true;
			pSectPr->SectionType = m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int Read_pgHeader(BYTE type, long length, void* poResult)
	{
		SectPr* pSectPr = static_cast<SectPr*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerProp_secPrType::hdrftrelem == type )
		{
			int nHdrFtrIndex = m_oBufferedStream.GetLong();
			if(nHdrFtrIndex >= 0 && nHdrFtrIndex <= (int)m_oFileWriter.m_oHeaderFooterWriter.m_aHeaders.size())
			{
				Writers::HdrFtrItem* pHdrFtrItem = m_oFileWriter.m_oHeaderFooterWriter.m_aHeaders[nHdrFtrIndex];
				pHdrFtrItem->m_sFilename;
                std::wstring sType;
				if(SimpleTypes::hdrftrFirst == pHdrFtrItem->eType)
					sType = _T("first");
				else if(SimpleTypes::hdrftrEven == pHdrFtrItem->eType)
					sType = _T("even");
				else
					sType = _T("default");
				pSectPr->sHeaderFooterReference += _T("<w:headerReference w:type=\"") + sType +_T("\" r:id=\"") + pHdrFtrItem->rId + _T("\"/>");
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int Read_pgFooter(BYTE type, long length, void* poResult)
	{
		SectPr* pSectPr = static_cast<SectPr*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerProp_secPrType::hdrftrelem == type )
		{
			int nHdrFtrIndex = m_oBufferedStream.GetLong();
			if(nHdrFtrIndex >= 0 && nHdrFtrIndex <= (int)oBinary_HdrFtrTableReader.m_oHeaderFooterWriter.m_aFooters.size())
			{
				Writers::HdrFtrItem* pHdrFtrItem = oBinary_HdrFtrTableReader.m_oHeaderFooterWriter.m_aFooters[nHdrFtrIndex];
				pHdrFtrItem->m_sFilename;
                std::wstring sType;
				if(SimpleTypes::hdrftrFirst == pHdrFtrItem->eType)
					sType = _T("first");
				else if(SimpleTypes::hdrftrEven == pHdrFtrItem->eType)
					sType = _T("even");
				else
					sType = _T("default");
				pSectPr->sHeaderFooterReference += _T("<w:footerReference w:type=\"") + sType +_T("\" r:id=\"") + pHdrFtrItem->rId + _T("\"/>");
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int Read_pageNumType(BYTE type, long length, void* poResult)
	{
		SectPr* pSectPr = static_cast<SectPr*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerProp_secPrPageNumType::start == type )
		{
			pSectPr->bPageNumStart = true;
			pSectPr->PageNumStart = m_oBufferedStream.GetLong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadCols(BYTE type, long length, void* poResult)
	{
		OOX::Logic::CColumns* pCols = static_cast<OOX::Logic::CColumns*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerProp_Columns::EqualWidth == type )
		{
			pCols->m_oEqualWidth.Init();
			pCols->m_oEqualWidth->FromBool(m_oBufferedStream.GetBool());
		}
		else if( c_oSerProp_Columns::Num == type )
		{
			pCols->m_oNum.Init();
			pCols->m_oNum->SetValue(m_oBufferedStream.GetLong());
		}
		else if( c_oSerProp_Columns::Sep == type )
		{
			pCols->m_oSep.Init();
			pCols->m_oSep->FromBool(m_oBufferedStream.GetBool());
		}
		else if( c_oSerProp_Columns::Space == type )
		{
			pCols->m_oSpace.Init();
			pCols->m_oSpace->FromTwips(m_oBufferedStream.GetLong());
		}
		else if( c_oSerProp_Columns::Column == type )
		{
			ComplexTypes::Word::CColumn* pCol = new ComplexTypes::Word::CColumn();
			READ1_DEF(length, res, this->ReadCol, pCol);
			pCols->m_arrColumns.push_back(pCol);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadCol(BYTE type, long length, void* poResult)
	{
		ComplexTypes::Word::CColumn* pCol = static_cast<ComplexTypes::Word::CColumn*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerProp_Columns::ColumnSpace == type )
		{
			pCol->m_oSpace.Init();
			pCol->m_oSpace->FromTwips(m_oBufferedStream.GetLong());
		}
		else if( c_oSerProp_Columns::ColumnW == type )
		{
			pCol->m_oW.Init();
			pCol->m_oW->FromTwips(m_oBufferedStream.GetLong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadPageBorders(BYTE type, long length, void* poResult)
	{
		OOX::Logic::CPageBorders* pPageBorders = static_cast<OOX::Logic::CPageBorders*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerPageBorders::Display == type )
		{
			pPageBorders->m_oDisplay.Init();
			pPageBorders->m_oDisplay->SetValue((SimpleTypes::EPageBorderDisplay)m_oBufferedStream.GetChar());
		}
		else if( c_oSerPageBorders::OffsetFrom == type )
		{
			pPageBorders->m_oOffsetFrom.Init();
			pPageBorders->m_oOffsetFrom->SetValue((SimpleTypes::EPageBorderOffset)m_oBufferedStream.GetChar());
		}
		else if( c_oSerPageBorders::ZOrder == type )
		{
			pPageBorders->m_oZOrder.Init();
			pPageBorders->m_oZOrder->SetValue((SimpleTypes::EPageBorderZOrder)m_oBufferedStream.GetChar());
		}
		else if( c_oSerPageBorders::Bottom == type )
		{
			pPageBorders->m_oBottom.Init();
			READ2_DEF(length, res, this->ReadPageBorder, pPageBorders->m_oBottom.GetPointer());
		}
		else if( c_oSerPageBorders::Left == type )
		{
			pPageBorders->m_oLeft.Init();
			READ2_DEF(length, res, this->ReadPageBorder, pPageBorders->m_oLeft.GetPointer());
		}
		else if( c_oSerPageBorders::Right == type )
		{
			pPageBorders->m_oRight.Init();
			READ2_DEF(length, res, this->ReadPageBorder, pPageBorders->m_oRight.GetPointer());
		}
		else if( c_oSerPageBorders::Top == type )
		{
			pPageBorders->m_oTop.Init();
			READ2_DEF(length, res, this->ReadPageBorder, pPageBorders->m_oTop.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadPageBorder(BYTE type, long length, void* poResult)
	{
		ComplexTypes::Word::CPageBorder* pPageBorder = static_cast<ComplexTypes::Word::CPageBorder*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerPageBorders::Color == type )
		{
			docRGB color = oBinary_CommonReader2.ReadColor();
			pPageBorder->m_oColor.Init();
			pPageBorder->m_oColor->SetValue(SimpleTypes::hexcolorRGB);
			pPageBorder->m_oColor->Set_R(color.R);
			pPageBorder->m_oColor->Set_G(color.G);
			pPageBorder->m_oColor->Set_B(color.B);
		}
		else if( c_oSerPageBorders::ColorTheme == type )
		{
			CThemeColor themeColor;
			oBinary_CommonReader2.ReadThemeColor(length, themeColor);
			if(themeColor.Auto)
			{
				pPageBorder->m_oColor.Init();
				pPageBorder->m_oColor->SetValue(SimpleTypes::hexcolorAuto);
			}
			if(themeColor.bColor)
			{
				pPageBorder->m_oThemeColor.Init();
				pPageBorder->m_oThemeColor->SetValue((SimpleTypes::EThemeColor)themeColor.Color);
			}
			if(themeColor.bShade)
			{
				pPageBorder->m_oThemeShade.Init();
				pPageBorder->m_oThemeShade->SetValue(themeColor.Shade);
			}
			if(themeColor.bTint)
			{
				pPageBorder->m_oThemeTint.Init();
				pPageBorder->m_oThemeTint->SetValue(themeColor.Tint);
			}

		}
		else if( c_oSerPageBorders::Space == type )
		{
			pPageBorder->m_oSpace.Init();
			pPageBorder->m_oSpace->SetValue(m_oBufferedStream.GetLong());
		}
		else if( c_oSerPageBorders::Sz == type )
		{
			pPageBorder->m_oSz.Init();
			pPageBorder->m_oSz->SetValue(m_oBufferedStream.GetLong());
		}
		else if( c_oSerPageBorders::Val == type )
		{
			pPageBorder->m_oVal.Init();
			pPageBorder->m_oVal->SetValue((SimpleTypes::EBorder)m_oBufferedStream.GetLong());
		}
		else if( c_oSerPageBorders::Frame == type )
		{
			pPageBorder->m_oFrame.Init();
			pPageBorder->m_oFrame->FromBool(m_oBufferedStream.GetBool());
		}
		else if( c_oSerPageBorders::Shadow == type )
		{
			pPageBorder->m_oShadow.Init();
			pPageBorder->m_oShadow->FromBool(m_oBufferedStream.GetBool());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
};
class Binary_tblPrReader : public Binary_CommonReader
{
protected:
	Binary_pPrReader oBinary_pPrReader;
	Binary_CommonReader2 oBinary_CommonReader2;
public:
    std::wstring m_sCurTableShd;
	std::vector<double> m_aCurTblGrid;
	bool bCellShd;
public:
	Binary_tblPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter):Binary_CommonReader(poBufferedStream),oBinary_CommonReader2(poBufferedStream),oBinary_pPrReader(poBufferedStream, oFileWriter)
	{
	}
	int Read_tblPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CWiterTblPr* pWiterTblPr = static_cast<CWiterTblPr*>(poResult);
		if( c_oSerProp_tblPrType::RowBandSize == type )
		{
			long nRowBandSize = m_oBufferedStream.GetLong();
            pWiterTblPr->RowBandSize = L"<w:tblStyleRowBandSize w:val=\"" + std::to_wstring(nRowBandSize) + L"\"/>";
		}
		else if( c_oSerProp_tblPrType::ColBandSize == type )
		{
			long nColBandSize = m_oBufferedStream.GetLong();
            pWiterTblPr->ColBandSize = L"<w:tblStyleColBandSize w:val=\"" + std::to_wstring(nColBandSize) + L"\"/>";
		}
		else if( c_oSerProp_tblPrType::Jc == type )
		{
			BYTE jc = m_oBufferedStream.GetUChar();
			switch(jc)
			{
                case align_Right:   pWiterTblPr->Jc = std::wstring(_T("<w:jc w:val=\"right\" />")); break;
                case align_Center:  pWiterTblPr->Jc = std::wstring(_T("<w:jc w:val=\"center\" />"));break;
                case align_Justify: pWiterTblPr->Jc = std::wstring(_T("<w:jc w:val=\"both\" />"));  break;
                case align_Left:    break;
            }
		}
		else if( c_oSerProp_tblPrType::TableInd == type )
		{
			double dInd = m_oBufferedStream.GetDouble();
			long nInd = SerializeCommon::Round( g_dKoef_mm_to_twips * dInd);
            pWiterTblPr->TableInd = L"<w:tblInd w:w=\"" + std::to_wstring(nInd) + L"\" w:type=\"dxa\"/>";
		}
		else if( c_oSerProp_tblPrType::TableW == type )
		{
			docW odocW;
			READ2_DEF(length, res, this->ReadW, &odocW);
            pWiterTblPr->TableW = odocW.Write(std::wstring(_T("w:tblW")));
		}
		else if( c_oSerProp_tblPrType::TableCellMar == type )
		{
			XmlUtils::CStringWriter oTempWriter;
			READ1_DEF(length, res, this->ReadCellMargins, &oTempWriter);
			if(oTempWriter.GetCurSize() > 0)
			{
                pWiterTblPr->TableCellMar += L"<w:tblCellMar>";
                pWiterTblPr->TableCellMar += oTempWriter.GetData();
                pWiterTblPr->TableCellMar += L"</w:tblCellMar>";
			}
		}
		else if( c_oSerProp_tblPrType::TableBorders == type )
		{
			docBorders odocBorders;
			READ1_DEF(length, res, oBinary_pPrReader.ReadBorders, &odocBorders);
            if(false == odocBorders.IsEmpty())
			{
				XmlUtils::CStringWriter oTempWriter; 
				odocBorders.Write(&oTempWriter, false);
                pWiterTblPr->TableBorders += L"<w:tblBorders>";
                pWiterTblPr->TableBorders += oTempWriter.GetData();
                pWiterTblPr->TableBorders += L"</w:tblBorders>";
			}
		}
		else if( c_oSerProp_tblPrType::Shd == type )
		{
			Shd oShd;
			READ2_DEF(length, res, oBinary_CommonReader2.ReadShd, &oShd);
			pWiterTblPr->Shd = oShd.ToString();
			m_sCurTableShd = pWiterTblPr->Shd;
		}
		else if( c_oSerProp_tblPrType::tblpPr == type )
		{
			XmlUtils::CStringWriter oTempWriter;
			READ2_DEF(length, res, this->Read_tblpPr, &oTempWriter);
            pWiterTblPr->tblpPr += L"<w:tblpPr w:vertAnchor=\"page\" w:horzAnchor=\"page\"";
            pWiterTblPr->tblpPr += oTempWriter.GetData();
            pWiterTblPr->tblpPr += L"/>";
		}
		else if( c_oSerProp_tblPrType::tblpPr2 == type )
		{
			XmlUtils::CStringWriter oTempWriter;
			READ2_DEF(length, res, this->Read_tblpPr2, &oTempWriter);
            pWiterTblPr->tblpPr += L"<w:tblpPr";
            pWiterTblPr->tblpPr += oTempWriter.GetData();
            pWiterTblPr->tblpPr += L"/>";
		}
		else if( c_oSerProp_tblPrType::Style == type )
		{
            std::wstring Name(m_oBufferedStream.GetString3(length));
			Name = XmlUtils::EncodeXmlString(Name);
            pWiterTblPr->Style = L"<w:tblStyle w:val=\"" + Name + L"\"/>";
		}
		else if( c_oSerProp_tblPrType::Look == type )
		{
			//<w:tblLook  w:val="" w:firstRow="true" w:lastRow="true" w:noHBand="true" w:noVBand="true" /> 
			long nLook = m_oBufferedStream.GetLong();
			int nFC = (0 == (nLook & 0x0080)) ? 0 : 1;
			int nFR = (0 == (nLook & 0x0020)) ? 0 : 1;
			int nLC = (0 == (nLook & 0x0100)) ? 0 : 1;
			int nLR = (0 == (nLook & 0x0040)) ? 0 : 1;
			int nBH = (0 == (nLook & 0x0200)) ? 0 : 1;
			int nBV = (0 == (nLook & 0x0400)) ? 0 : 1;
            pWiterTblPr->Look = L"<w:tblLook w:val=\"" + XmlUtils::IntToString(nLook, L"%04X")
                            +   L"\" w:firstRow=\""     + std::to_wstring(nFR) + L"\" w:lastRow=\""    + std::to_wstring(nLR)
                            +   L"\" w:firstColumn=\""  + std::to_wstring(nFC) + L"\" w:lastColumn=\"" + std::to_wstring(nLC)
                            +   L"\" w:noHBand=\""      + std::to_wstring(nBH) + L"\" w:noVBand=\""    + std::to_wstring(nBV) + L"\"/>";
		}
		else if( c_oSerProp_tblPrType::Layout == type )
		{
			long nLayout = m_oBufferedStream.GetUChar();
            std::wstring sLayout;
			switch(nLayout)
			{
				case 1: sLayout = _T("autofit");break;
				case 2: sLayout = _T("fixed");break;
			}
            if(false == sLayout.empty())
                pWiterTblPr->Layout = L"<w:tblLayout w:type=\"" + sLayout + L"\"/>";
		}
		else if( c_oSerProp_tblPrType::tblPrChange == type )
		{
			TrackRevision tblPrChange;
			READ1_DEF(length, res, this->ReadTblPrChange, &tblPrChange);
			pWiterTblPr->tblPrChange = tblPrChange.ToString(_T("w:tblPrChange"));
		}
		else if( c_oSerProp_tblPrType::TableCellSpacing == type )
		{
			double dSpacing = m_oBufferedStream.GetDouble();
			dSpacing /=2;
			long nSpacing = SerializeCommon::Round( g_dKoef_mm_to_twips * dSpacing);
            pWiterTblPr->TableCellSpacing = L"<w:tblCellSpacing w:w=\"" + std::to_wstring(nSpacing) + L"\" w:type=\"dxa\"/>";
		}
		else if( c_oSerProp_tblPrType::tblCaption == type )
		{
			pWiterTblPr->Caption = m_oBufferedStream.GetString3(length);
		}
		else if( c_oSerProp_tblPrType::tblDescription == type )
		{
			pWiterTblPr->Description = m_oBufferedStream.GetString3(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadW(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docW* odocW = static_cast<docW*>(poResult);
		if( c_oSerWidthType::Type == type )
		{
			odocW->bType = true;
			odocW->Type = m_oBufferedStream.GetUChar();
		}
		else if( c_oSerWidthType::W == type )
		{
			odocW->bW = true;
			odocW->W = m_oBufferedStream.GetDouble();
		}
		else if( c_oSerWidthType::WDocx == type )
		{
			odocW->bWDocx = true;
			odocW->WDocx = m_oBufferedStream.GetLong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadCellMargins(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerMarginsType::left == type )
		{
			docW oLeft;
			READ2_DEF(length, res, this->ReadW, &oLeft);
            oLeft.Write(*pCStringWriter, std::wstring(_T("w:left")));
		}
		else if( c_oSerMarginsType::top == type )
		{
			docW oTop;
			READ2_DEF(length, res, this->ReadW, &oTop);
            oTop.Write(*pCStringWriter, std::wstring(_T("w:top")));
		}
		else if( c_oSerMarginsType::right == type )
		{
			docW oRight;
			READ2_DEF(length, res, this->ReadW, &oRight);
            oRight.Write(*pCStringWriter, std::wstring(_T("w:right")));
		}
		else if( c_oSerMarginsType::bottom == type )
		{
			docW oBottom;
			READ2_DEF(length, res, this->ReadW, &oBottom);
            oBottom.Write(*pCStringWriter, std::wstring(_T("w:bottom")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int Read_tblpPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSer_tblpPrType::X == type )
		{
			double dX = m_oBufferedStream.GetDouble();
			long nX = SerializeCommon::Round( g_dKoef_mm_to_twips * dX);
            pCStringWriter->WriteString(L" w:tblpX=\"" + std::to_wstring(nX) + L"\"");
		}
		else if( c_oSer_tblpPrType::Y == type )
		{
			double dY = m_oBufferedStream.GetDouble();
			long nY = SerializeCommon::Round( g_dKoef_mm_to_twips * dY);

            pCStringWriter->WriteString(L" w:tblpY=\"" + std::to_wstring(nY) + L"\"");
		}
		else if( c_oSer_tblpPrType::Paddings == type )
		{
			PaddingsToWrite oPaddings;
			READ2_DEF(length, res, this->ReadPaddings, &oPaddings);
			if(oPaddings.bLeft)
			{
				double dLeft = oPaddings.Left;
				long nLeft = SerializeCommon::Round( g_dKoef_mm_to_twips * dLeft);

                pCStringWriter->WriteString(L" w:leftFromText=\"" + std::to_wstring(nLeft) + L"\"");
			}
			if(oPaddings.bTop)
			{
				double dTop = oPaddings.Top;
				long nTop = SerializeCommon::Round( g_dKoef_mm_to_twips * dTop);

                pCStringWriter->WriteString(L" w:topFromText=\"" + std::to_wstring(nTop) + L"\"");
			}
			if(oPaddings.bRight)
			{
				double dRight = oPaddings.Right;
				long nRight = SerializeCommon::Round( g_dKoef_mm_to_twips * dRight);

                pCStringWriter->WriteString(L" w:rightFromText=\"" + std::to_wstring(nRight) + L"\"");
			}
			if(oPaddings.bBottom)
			{
				double dBottom = oPaddings.Bottom;
				long nBottom = SerializeCommon::Round( g_dKoef_mm_to_twips * dBottom);

                pCStringWriter->WriteString(L" w:bottomFromText=\"" + std::to_wstring(nBottom) + L"\"");
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int Read_tblpPr2(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSer_tblpPrType2::HorzAnchor == type )
		{
            std::wstring sXml;
			switch(m_oBufferedStream.GetUChar())
			{
            case 0:sXml += (_T(" w:horzAnchor=\"margin\""));break;
            case 1:sXml += (_T(" w:horzAnchor=\"page\""));break;
            case 2:sXml += (_T(" w:horzAnchor=\"text\""));break;
            default:sXml += (_T(" w:horzAnchor=\"text\""));break;
			}
			pCStringWriter->WriteString(sXml);
		}
		else if( c_oSer_tblpPrType2::TblpX == type )
		{
			double dX = m_oBufferedStream.GetDouble();
			long nX = SerializeCommon::Round( g_dKoef_mm_to_twips * dX);

            pCStringWriter->WriteString(L" w:tblpX=\"" + std::to_wstring(nX) + L"\"");
		}
		else if( c_oSer_tblpPrType2::TblpXSpec == type )
		{
            std::wstring sXml;
			switch(m_oBufferedStream.GetUChar())
			{
                case 0:     sXml += (_T(" w:tblpXSpec=\"center\""));    break;
                case 1:     sXml += (_T(" w:tblpXSpec=\"inside\""));    break;
                case 2:     sXml += (_T(" w:tblpXSpec=\"left\""));      break;
                case 3:     sXml += (_T(" w:tblpXSpec=\"outside\""));   break;
                case 4:     sXml += (_T(" w:tblpXSpec=\"right\""));     break;
                default:    sXml += (_T(" w:tblpXSpec=\"left\""));      break;
			}
			pCStringWriter->WriteString(sXml);
		}
		else if( c_oSer_tblpPrType2::VertAnchor == type )
		{
            std::wstring sXml;
			switch(m_oBufferedStream.GetUChar())
			{
            case 0: sXml += (_T(" w:vertAnchor=\"margin\""));break;
            case 1: sXml += (_T(" w:vertAnchor=\"page\""));  break;
            case 2: sXml += (_T(" w:vertAnchor=\"text\""));  break;
            default:sXml += (_T(" w:vertAnchor=\"text\"")); break;
			}
			pCStringWriter->WriteString(sXml);
		}
		else if( c_oSer_tblpPrType2::TblpY == type )
		{
			double dY = m_oBufferedStream.GetDouble();
			long nY = SerializeCommon::Round( g_dKoef_mm_to_twips * dY);

            pCStringWriter->WriteString(L" w:tblpY=\"" + std::to_wstring(nY) + L"\"");
		}
		else if( c_oSer_tblpPrType2::TblpYSpec == type )
		{
            std::wstring sXml;
			switch(m_oBufferedStream.GetUChar())
			{
            case 0:     sXml += (_T(" w:tblpYSpec=\"bottom\"")); break;
            case 1:     sXml += (_T(" w:tblpYSpec=\"center\"")); break;
            case 2:     sXml += (_T(" w:tblpYSpec=\"inline\"")); break;
            case 3:     sXml += (_T(" w:tblpYSpec=\"inside\"")); break;
            case 4:     sXml += (_T(" w:tblpYSpec=\"outside\""));break;
            case 5:     sXml += (_T(" w:tblpYSpec=\"top\""));    break;
            default:    sXml += (_T(" w:tblpYSpec=\"top\""));    break;
			}
			pCStringWriter->WriteString(sXml);
		}
		else if( c_oSer_tblpPrType2::Paddings == type )
		{
			READ2_DEF(length, res, this->ReadPaddings2, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int Read_RowPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerProp_rowPrType::CantSplit == type )
		{
			BYTE CantSplit = m_oBufferedStream.GetUChar();
			if(0 != CantSplit)
                pCStringWriter->WriteString(std::wstring(_T("<w:cantSplit />")));
			else
                pCStringWriter->WriteString(std::wstring(_T("<w:cantSplit w:val=\"false\"/>")));
		}
		else if( c_oSerProp_rowPrType::After == type )
		{
			rowPrAfterBefore orowPrAfterBefore(_T("After"));
			READ2_DEF(length, res, this->ReadAfter, &orowPrAfterBefore);
			if(true == orowPrAfterBefore.bGridAfter && orowPrAfterBefore.nGridAfter > 0 && false == orowPrAfterBefore.oAfterWidth.bW)
			{
				//ищем по tblGrid
				long nGridLength = (long)m_aCurTblGrid.size();
				if(orowPrAfterBefore.nGridAfter < nGridLength)
				{
					double nSumW = 0;
					for(int i = 0; i < orowPrAfterBefore.nGridAfter; i++)
					{
						nSumW += m_aCurTblGrid[nGridLength - i - 1];
					}
					orowPrAfterBefore.oAfterWidth.bW = true;
					orowPrAfterBefore.oAfterWidth.W = nSumW;
				}
			}
			orowPrAfterBefore.Write(*pCStringWriter);
		}
		else if( c_oSerProp_rowPrType::Before == type )
		{
			rowPrAfterBefore orowPrAfterBefore(_T("Before"));
			READ2_DEF(length, res, this->ReadBefore, &orowPrAfterBefore);
			if(true == orowPrAfterBefore.bGridAfter && orowPrAfterBefore.nGridAfter > 0 && false == orowPrAfterBefore.oAfterWidth.bW)
			{
				//ищем по tblGrid
				if(orowPrAfterBefore.nGridAfter < m_aCurTblGrid.size())
				{
					double nSumW = 0;
					for(int i = 0; i < orowPrAfterBefore.nGridAfter; i++)
					{
						nSumW += m_aCurTblGrid[i];
					}
					orowPrAfterBefore.oAfterWidth.bW = true;
					orowPrAfterBefore.oAfterWidth.W = nSumW;
				}
			}
			orowPrAfterBefore.Write(*pCStringWriter);
		}
		else if( c_oSerProp_rowPrType::Jc == type )
		{
			BYTE jc = m_oBufferedStream.GetUChar();
			switch(jc)
			{
            case align_Right: pCStringWriter->WriteString(std::wstring(_T("<w:jc w:val=\"right\" />")));break;
            case align_Left: pCStringWriter->WriteString(std::wstring(_T("<w:jc w:val=\"left\" />")));break;
            case align_Center: pCStringWriter->WriteString(std::wstring(_T("<w:jc w:val=\"center\" />")));break;
            case align_Justify: pCStringWriter->WriteString(std::wstring(_T("<w:jc w:val=\"both\" />")));break;
			}
		}
		else if( c_oSerProp_rowPrType::TableCellSpacing == type )
		{
			double dSpacing = m_oBufferedStream.GetDouble();
			dSpacing /=2;
			long nSpacing = SerializeCommon::Round( g_dKoef_mm_to_twips * dSpacing);

            pCStringWriter->WriteString(L"<w:tblCellSpacing w:w=\"" + std::to_wstring(nSpacing) + L"\" w:type=\"dxa\"/>");
		}
		else if( c_oSerProp_rowPrType::Height == type )
		{
			RowHeight val;
			READ2_DEF(length, res, this->ReadHeight, &val);
           
			pCStringWriter->WriteString(L"<w:trHeight w:val=\"" + std::to_wstring(val.nHeight) + L"\"");
			
			switch (val.HRule)
			{
				case 1: pCStringWriter->WriteString(L" w:hRule=\"auto\"");	break;
				case 2: pCStringWriter->WriteString(L" w:hRule=\"exact\"");	break;
			}
			pCStringWriter->WriteString(L"/>");
		}
		else if( c_oSerProp_rowPrType::TableHeader == type )
		{
			BYTE tblHeader = m_oBufferedStream.GetUChar();
			if(0 != tblHeader)
                pCStringWriter->WriteString(std::wstring(_T("<w:tblHeader />")));
			else
                pCStringWriter->WriteString(std::wstring(_T("<w:tblHeader w:val=\"false\"/>")));
		}
		else if( c_oSerProp_rowPrType::Del == type )
		{
			TrackRevision Del;
			oBinary_CommonReader2.ReadTrackRevision(length, &Del);
			Del.Write(pCStringWriter, _T("w:del"));
		}
		else if( c_oSerProp_rowPrType::Ins == type )
		{
			TrackRevision Ins;
			oBinary_CommonReader2.ReadTrackRevision(length, &Ins);
			Ins.Write(pCStringWriter, _T("w:ins"));
		}
		else if( c_oSerProp_rowPrType::trPrChange == type )
		{
			TrackRevision trPrChange;
			READ1_DEF(length, res, this->ReadTrPrChange, &trPrChange);
			trPrChange.Write(pCStringWriter, _T("w:trPrChange"));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadAfter(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		rowPrAfterBefore* orowPrAfterBefore = static_cast<rowPrAfterBefore*>(poResult);
		if( c_oSerProp_rowPrType::GridAfter == type )
		{
			orowPrAfterBefore->bGridAfter = true;
			orowPrAfterBefore->nGridAfter = m_oBufferedStream.GetLong();
		}
		else if( c_oSerProp_rowPrType::WAfter == type )
		{
			READ2_DEF(length, res, this->ReadW, &orowPrAfterBefore->oAfterWidth);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadBefore(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		rowPrAfterBefore* orowPrAfterBefore = static_cast<rowPrAfterBefore*>(poResult);
		if( c_oSerProp_rowPrType::GridBefore == type )
		{
			orowPrAfterBefore->bGridAfter = true;
			orowPrAfterBefore->nGridAfter = m_oBufferedStream.GetLong();
		}
		else if( c_oSerProp_rowPrType::WBefore == type )
		{
			READ2_DEF(length, res, this->ReadW, &orowPrAfterBefore->oAfterWidth);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadHeight(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;

		RowHeight* pHeight = static_cast<RowHeight*>(poResult);
		
		if( c_oSerProp_rowPrType::Height_Rule == type )
		{
			pHeight->HRule = m_oBufferedStream.GetUChar();
		}
		else if( c_oSerProp_rowPrType::Height_Value == type )
		{
			double dHeight = m_oBufferedStream.GetDouble();
			pHeight->nHeight = SerializeCommon::Round( g_dKoef_mm_to_twips * dHeight);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int Read_CellPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerProp_cellPrType::GridSpan == type )
		{
			long nGridSpan = m_oBufferedStream.GetLong();
			if(nGridSpan > 1)
			{
                pCStringWriter->WriteString(L"<w:gridSpan w:val=\"" + std::to_wstring(nGridSpan) + L"\"/>");
			}
		}
		else if( c_oSerProp_cellPrType::Shd == type )
		{
			bCellShd = true;
			Shd oShd;
			READ2_DEF(length, res, oBinary_CommonReader2.ReadShd, &oShd);
			pCStringWriter->WriteString(oShd.ToString());
		}
		else if( c_oSerProp_cellPrType::TableCellBorders == type )
		{
			docBorders odocBorders;
			READ1_DEF(length, res, oBinary_pPrReader.ReadBorders, &odocBorders);
            if(false == odocBorders.IsEmpty())
			{
                pCStringWriter->WriteString(std::wstring(_T("<w:tcBorders>")));
				odocBorders.Write(pCStringWriter, true);
                pCStringWriter->WriteString(std::wstring(_T("</w:tcBorders>")));
			}
		}
		else if( c_oSerProp_cellPrType::CellMar == type )
		{
			XmlUtils::CStringWriter oTempWriter; 
			READ1_DEF(length, res, this->ReadCellMargins, &oTempWriter);
			if(oTempWriter.GetCurSize() > 0)
			{
                pCStringWriter->WriteString(std::wstring(_T("<w:tcMar>")));
				pCStringWriter->Write(oTempWriter);
                pCStringWriter->WriteString(std::wstring(_T("</w:tcMar>")));
			}
		}
		else if( c_oSerProp_cellPrType::TableCellW == type )
		{
			docW oW;
			READ2_DEF(length, res, this->ReadW, &oW);
            oW.Write(*pCStringWriter, std::wstring(_T("w:tcW")));
		}
		else if( c_oSerProp_cellPrType::VAlign == type )
		{
			BYTE VAlign = m_oBufferedStream.GetUChar();
			switch(VAlign)
			{
            case vertalignjc_Top:pCStringWriter->WriteString(std::wstring(_T("<w:vAlign w:val=\"top\" />")));break;
            case vertalignjc_Center:pCStringWriter->WriteString(std::wstring(_T("<w:vAlign w:val=\"center\" />")));break;
            case vertalignjc_Bottom:pCStringWriter->WriteString(std::wstring(_T("<w:vAlign w:val=\"bottom\" />")));break;
			}
		}
		else if( c_oSerProp_cellPrType::VMerge == type )
		{
			BYTE VMerge = m_oBufferedStream.GetUChar();
			switch(VMerge)
			{
            case vmerge_Restart:pCStringWriter->WriteString(std::wstring(_T("<w:vMerge w:val=\"restart\" />")));break;
            case vmerge_Continue:pCStringWriter->WriteString(std::wstring(_T("<w:vMerge w:val=\"continue\" />")));break;
			}
		}
		else if( c_oSerProp_cellPrType::CellDel == type )
		{
			TrackRevision Del;
			oBinary_CommonReader2.ReadTrackRevision(length, &Del);
			Del.Write(pCStringWriter, _T("w:del"));
		}
		else if( c_oSerProp_cellPrType::CellIns == type )
		{
			TrackRevision Ins;
			oBinary_CommonReader2.ReadTrackRevision(length, &Ins);
			Ins.Write(pCStringWriter, _T("w:ins"));
		}
		else if( c_oSerProp_cellPrType::CellMerge == type )
		{
			TrackRevision cellMerge;
			READ1_DEF(length, res, this->ReadCellMerge, &cellMerge);
			cellMerge.Write(pCStringWriter, _T("w:cellMerge"));
		}
		else if( c_oSerProp_cellPrType::tcPrChange == type )
		{
			TrackRevision tcPrChange;
			READ1_DEF(length, res, this->ReadTcPrChange, &tcPrChange);
			tcPrChange.Write(pCStringWriter, _T("w:tcPrChange"));
		}
		else if( c_oSerProp_cellPrType::textDirection == type )
		{
			SimpleTypes::CTextDirection<> oTextDirection;
			oTextDirection.SetValue((SimpleTypes::ETextDirection)m_oBufferedStream.GetUChar());
            pCStringWriter->WriteString(std::wstring(_T("<w:textDirection w:val=\"")));
			pCStringWriter->WriteString(oTextDirection.ToString());
            pCStringWriter->WriteString(std::wstring(_T("\" />")));
		}
		else if( c_oSerProp_cellPrType::hideMark == type )
		{
			bool hideMark = m_oBufferedStream.GetBool();
			if(hideMark)
                pCStringWriter->WriteString(std::wstring(_T("<w:hideMark />")));
			else
                pCStringWriter->WriteString(std::wstring(_T("<w:hideMark w:val=\"false\"/>")));
		}
		else if( c_oSerProp_cellPrType::noWrap == type )
		{
			bool noWrap = m_oBufferedStream.GetBool();
			if(noWrap)
                pCStringWriter->WriteString(std::wstring(_T("<w:noWrap />")));
			else
                pCStringWriter->WriteString(std::wstring(_T("<w:noWrap w:val=\"false\"/>")));
		}
		else if( c_oSerProp_cellPrType::tcFitText == type )
		{
			bool tcFitText = m_oBufferedStream.GetBool();
			if(tcFitText)
                pCStringWriter->WriteString(std::wstring(_T("<w:tcFitText />")));
			else
                pCStringWriter->WriteString(std::wstring(_T("<w:tcFitText w:val=\"false\"/>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadPaddings(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		PaddingsToWrite* oPaddings = static_cast<PaddingsToWrite*>(poResult);
		if (c_oSerPaddingType::left == type)
		{
			oPaddings->bLeft = true;
			oPaddings->Left = m_oBufferedStream.GetDouble();
		}
		else if (c_oSerPaddingType::top == type)
		{
			oPaddings->bTop = true;
			oPaddings->Top = m_oBufferedStream.GetDouble();
		}
		else if (c_oSerPaddingType::right == type)
		{
			oPaddings->bRight = true;
			oPaddings->Right = m_oBufferedStream.GetDouble();
		}
		else if (c_oSerPaddingType::bottom == type)
		{
			oPaddings->bBottom = true;
			oPaddings->Bottom = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadPaddings2(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if (c_oSerPaddingType::left == type)
		{
			double dLeft = m_oBufferedStream.GetDouble();
			long nLeft = SerializeCommon::Round( g_dKoef_mm_to_twips * dLeft);

            pCStringWriter->WriteString(L" w:leftFromText=\"" + std::to_wstring(nLeft) + L"\"");
		}
		else if (c_oSerPaddingType::top == type)
		{
			double dTop = m_oBufferedStream.GetDouble();
			long nTop = SerializeCommon::Round( g_dKoef_mm_to_twips * dTop);

            pCStringWriter->WriteString(L" w:topFromText=\"" + std::to_wstring(nTop) + L"\"");
		}
		else if (c_oSerPaddingType::right == type)
		{
			double dRight = m_oBufferedStream.GetDouble();
			long nRight = SerializeCommon::Round( g_dKoef_mm_to_twips * dRight);

            pCStringWriter->WriteString(L" w:rightFromText=\"" + std::to_wstring(nRight) + L"\"");
		}
		else if (c_oSerPaddingType::bottom == type)
		{
			double dBottom = m_oBufferedStream.GetDouble();
			long nBottom = SerializeCommon::Round( g_dKoef_mm_to_twips * dBottom);

            pCStringWriter->WriteString(L" w:bottomFromText=\"" + std::to_wstring(nBottom) + L"\"");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadCellMerge(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pTrackRevision)
		else if (c_oSerProp_RevisionType::VMerge == type)
		{
			pTrackRevision->vMerge = new long(m_oBufferedStream.GetLong());
		}
		else if (c_oSerProp_RevisionType::VMergeOrigin == type)
		{
			pTrackRevision->vMergeOrigin = new long(m_oBufferedStream.GetLong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadTblPrChange(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pTrackRevision)
		else if (c_oSerProp_RevisionType::tblPrChange == type)
		{
			pTrackRevision->tblPr = new CWiterTblPr();
			READ1_DEF(length, res, this->Read_tblPr, pTrackRevision->tblPr);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadTrPrChange(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pTrackRevision)
		else if (c_oSerProp_RevisionType::tblPrChange == type)
		{
			pTrackRevision->trPr = new XmlUtils::CStringWriter();
			READ2_DEF(length, res, this->Read_RowPr, pTrackRevision->trPr);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadTcPrChange(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pTrackRevision)
		else if (c_oSerProp_RevisionType::tcPrChange == type)
		{
			pTrackRevision->tcPr = new XmlUtils::CStringWriter();
			READ2_DEF(length, res, this->Read_CellPr, pTrackRevision->tcPr);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
};
class Binary_NumberingTableReader : public Binary_CommonReader
{
	Binary_pPrReader oBinary_pPrReader;
	Binary_rPrReader oBinary_rPrReader;
	Writers::NumberingWriter& oNumberingWriters;
	Writers::FontTableWriter& m_oFontTableWriter;
	std::vector<docNum*> m_aDocNums;
	std::vector<docANum*> m_aDocANums;
	std::map<int, int> m_mapANumToNum;
public:
	Binary_NumberingTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter) :Binary_CommonReader(poBufferedStream), oNumberingWriters(oFileWriter.m_oNumberingWriter), m_oFontTableWriter(oFileWriter.m_oFontTableWriter), oBinary_pPrReader(poBufferedStream, oFileWriter), oBinary_rPrReader(poBufferedStream, oFileWriter)
	{
	}
	int Read()
	{
		int res = c_oSerConstants::ReadOk;
		READ_TABLE_DEF(res, this->ReadNumberingContent, NULL);
		for(size_t i = 0; i < m_aDocANums.size(); ++i)
		{
			docANum* pdocANum = m_aDocANums[i];
			pdocANum->Write(oNumberingWriters.m_oANum);
			delete m_aDocANums[i];
		}
		m_aDocANums.clear();
		for(size_t i = 0; i < m_aDocNums.size(); ++i)
		{
			m_aDocNums[i]->Write(oNumberingWriters.m_oNumList);
			delete m_aDocNums[i];
		}
		m_aDocNums.clear();
		return res;
	};
	int ReadNumberingContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerNumTypes::AbstractNums == type )
		{
			READ1_DEF(length, res, this->ReadAbstractNums, poResult);
		}
		else if ( c_oSerNumTypes::Nums == type )
		{
			READ1_DEF(length, res, this->ReadNums, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadNums(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerNumTypes::Num == type )
		{
			docNum* pdocNum = new docNum();
			READ2_DEF(length, res, this->ReadNum, pdocNum);
			if(pdocNum->bAId && pdocNum->bId)
				m_mapANumToNum[pdocNum->AId] = pdocNum->Id;
			m_aDocNums.push_back(pdocNum);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadNum(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docNum* odocNum = static_cast<docNum*>(poResult);
		if ( c_oSerNumTypes::Num_ANumId == type )
		{
			odocNum->bAId = true;
			odocNum->AId = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerNumTypes::Num_NumId == type )
		{
			odocNum->bId = true;
			odocNum->Id = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerNumTypes::Num_LvlOverride == type )
		{
			docLvlOverride* lvlOverride = new docLvlOverride();
			READ1_DEF(length, res, this->ReadLvlOverride, lvlOverride);
			odocNum->LvlOverrides.push_back(lvlOverride);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadLvlOverride(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docLvlOverride* lvlOverride = static_cast<docLvlOverride*>(poResult);
		if (c_oSerNumTypes::ILvl == type)
		{
			lvlOverride->bILvl = true;
			lvlOverride->ILvl = m_oBufferedStream.GetLong();
		}
		else if (c_oSerNumTypes::StartOverride == type)
		{
			lvlOverride->bStartOverride = true;
			lvlOverride->StartOverride = m_oBufferedStream.GetLong();
		}
		else if (c_oSerNumTypes::Lvl == type)
		{
			lvlOverride->Lvl = new docLvl();
			READ2_DEF(length, res, this->ReadLevel, lvlOverride->Lvl);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadAbstractNums(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerNumTypes::AbstractNum == type )
		{
			docANum* pdocANum = new docANum();
			READ1_DEF(length, res, this->ReadAbstractNum, pdocANum);
			m_aDocANums.push_back(pdocANum);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadAbstractNum(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docANum* odocANum = static_cast<docANum*>(poResult);
		if ( c_oSerNumTypes::AbstractNum_Lvls == type )
		{
			READ1_DEF(length, res, this->ReadLevels, poResult);
		}
		else if ( c_oSerNumTypes::AbstractNum_Id == type )
		{
			odocANum->bId = true;
			odocANum->Id = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerNumTypes::NumStyleLink == type )
			odocANum->NumStyleLink = m_oBufferedStream.GetString3(length);
		else if ( c_oSerNumTypes::StyleLink == type )
			odocANum->StyleLink = m_oBufferedStream.GetString3(length);
		//else if ( c_oSerNumTypes::AbstractNum_Type == type )
		//{
		//	oNewNum.Type = this.stream.GetUChar();
		//}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadLevels(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docANum* odocANum = static_cast<docANum*>(poResult);
		if ( c_oSerNumTypes::Lvl == type )
		{
			docLvl* odocLvl = new docLvl();
			odocLvl->ILvl = odocANum->Lvls.size();
			READ2_DEF(length, res, this->ReadLevel, odocLvl);
			odocANum->Lvls.push_back(odocLvl);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadLevel(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docLvl* odocLvl = static_cast<docLvl*>(poResult);
		if ( c_oSerNumTypes::lvl_Format == type )
		{
			odocLvl->bFormat = true;
			odocLvl->Format = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerNumTypes::lvl_Jc == type )
		{
			odocLvl->bJc = true;
			odocLvl->Jc = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerNumTypes::lvl_LvlText == type )
		{
			odocLvl->bText = true;
			READ1_DEF(length, res, this->ReadLevelText, poResult);
		}
		else if ( c_oSerNumTypes::lvl_Restart == type )
		{
			odocLvl->bRestart = true;
			odocLvl->Restart = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerNumTypes::lvl_Start == type )
		{
			odocLvl->bStart = true;
			odocLvl->Start = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerNumTypes::lvl_Suff == type )
		{
			odocLvl->bSuff = true;
			odocLvl->Suff = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerNumTypes::lvl_PStyle == type )
		{
			odocLvl->bPStyle = true;
			odocLvl->PStyle = m_oBufferedStream.GetString3(length);
		}
		else if ( c_oSerNumTypes::lvl_ParaPr == type )
		{
			odocLvl->bParaPr = true;
            odocLvl->ParaPr.WriteString(std::wstring(_T("<w:pPr>")));
			res = oBinary_pPrReader.Read(length, &odocLvl->ParaPr);
            odocLvl->ParaPr.WriteString(std::wstring(_T("</w:pPr>")));
		}
		else if ( c_oSerNumTypes::lvl_TextPr == type )
		{
			odocLvl->bTextPr = true;
			rPr orPr(m_oFontTableWriter.m_mapFonts);
			res = oBinary_rPrReader.Read(length, &orPr);
			if(orPr.IsNoEmpty())
				orPr.Write(&odocLvl->TextPr);
		}
		else if ( c_oSerNumTypes::ILvl == type )
		{
			odocLvl->bILvl = true;
			odocLvl->ILvl = m_oBufferedStream.GetLong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadLevelText(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docLvl* odocLvl = static_cast<docLvl*>(poResult);
		if ( c_oSerNumTypes::lvl_LvlTextItem == type )
		{
			docLvlText* odocLvlText = new docLvlText();
			READ1_DEF(length, res, this->ReadLevelTextItem, odocLvlText);
			odocLvl->Text.push_back(odocLvlText);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadLevelTextItem(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docLvlText* odocLvlText = static_cast<docLvlText*>(poResult);
		if ( c_oSerNumTypes::lvl_LvlTextItemText == type )
		{
            std::wstring sText(m_oBufferedStream.GetString3(length));
			odocLvlText->bText = true;
			odocLvlText->Text = sText;
		}
		else if ( c_oSerNumTypes::lvl_LvlTextItemNum == type )
		{
			odocLvlText->bNumber = true;
			odocLvlText->Number = m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
};
class BinaryStyleTableReader : public Binary_CommonReader
{
	Binary_pPrReader			oBinary_pPrReader;
	Binary_rPrReader			oBinary_rPrReader;
	Binary_tblPrReader			oBinary_tblPrReader;

	Writers::StylesWriter&		m_oStylesWriter;
	Writers::FontTableWriter&	m_oFontTableWriter;
public:
	BinaryStyleTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter) :Binary_CommonReader(poBufferedStream), m_oStylesWriter(oFileWriter.m_oStylesWriter), m_oFontTableWriter(oFileWriter.m_oFontTableWriter), oBinary_pPrReader(poBufferedStream, oFileWriter), oBinary_rPrReader(poBufferedStream, oFileWriter), oBinary_tblPrReader(poBufferedStream, oFileWriter)
	{
	}
	int Read()
	{
		int res = c_oSerConstants::ReadOk;
		READ_TABLE_DEF(res, this->ReadStyleTableContent, NULL);
		return res;
	};
	int ReadStyleTableContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if(c_oSer_st::Styles == type)
		{
			READ1_DEF(length, res, this->ReadStyle, poResult);
		}
		else if(c_oSer_st::DefpPr == type)
		{
            m_oStylesWriter.m_pPrDefault.WriteString(std::wstring(_T("<w:pPr>")));
			bool bOldVal = oBinary_pPrReader.bDoNotWriteNullProp;
			oBinary_pPrReader.bDoNotWriteNullProp = true;
			res = oBinary_pPrReader.Read(length, &m_oStylesWriter.m_pPrDefault);
			oBinary_pPrReader.bDoNotWriteNullProp = bOldVal;
            m_oStylesWriter.m_pPrDefault.WriteString(std::wstring(_T("</w:pPr>")));
		}
		else if(c_oSer_st::DefrPr == type)
		{
			rPr oNew_rPr(m_oFontTableWriter.m_mapFonts);
			oNew_rPr.bDoNotWriteNullProp = true;
			res = oBinary_rPrReader.Read(length, &oNew_rPr);
			if(oNew_rPr.IsNoEmpty())
				oNew_rPr.Write(&m_oStylesWriter.m_rPrDefault);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if(c_oSer_sts::Style == type)
		{
			docStyle odocStyle;
			READ1_DEF(length, res, this->ReadStyleContent, &odocStyle);
			if(m_oStylesWriter.m_nVersion < 2)
			{
				odocStyle.bqFormat = true;
				odocStyle.qFormat = true;
			}
			odocStyle.Write(&m_oStylesWriter.m_Styles);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadStyleContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docStyle* odocStyle = static_cast<docStyle*>(poResult);
		if(c_oSer_sts::Style_Name == type)
		{
            std::wstring Name(m_oBufferedStream.GetString3(length));
			Name = XmlUtils::EncodeXmlString(Name);
			odocStyle->Name = Name;
		}
		else if(c_oSer_sts::Style_Id == type)
		{
            std::wstring Id(m_oBufferedStream.GetString3(length));
			Id = XmlUtils::EncodeXmlString(Id);
			odocStyle->Id = Id;
		}
		else if(c_oSer_sts::Style_Type == type)
		{
			odocStyle->byteType = m_oBufferedStream.GetUChar();
		}
		else if(c_oSer_sts::Style_Default == type)
		{
			odocStyle->bDefault = (0 != m_oBufferedStream.GetUChar());
		}
		else if(c_oSer_sts::Style_BasedOn == type)
		{
            std::wstring BasedOn(m_oBufferedStream.GetString3(length));
			BasedOn = XmlUtils::EncodeXmlString(BasedOn);
			odocStyle->BasedOn = BasedOn;
		}
		else if(c_oSer_sts::Style_Next == type)
		{
            std::wstring NextId(m_oBufferedStream.GetString3(length));
			NextId = XmlUtils::EncodeXmlString(NextId);
			odocStyle->NextId = NextId;
		}
		else if(c_oSer_sts::Style_Link == type)
		{
			odocStyle->Link = m_oBufferedStream.GetString3(length);
		}
		else if(c_oSer_sts::Style_qFormat == type)
		{
			odocStyle->bqFormat = true;
			odocStyle->qFormat = (0 != m_oBufferedStream.GetUChar()) ? true : false;
		}
		else if(c_oSer_sts::Style_uiPriority == type)
		{
			odocStyle->buiPriority = true;
			odocStyle->uiPriority = m_oBufferedStream.GetLong();
		}
		else if(c_oSer_sts::Style_hidden == type)
		{
			odocStyle->bhidden = true;
			odocStyle->hidden = (0 != m_oBufferedStream.GetUChar()) ? true : false;
		}
		else if(c_oSer_sts::Style_semiHidden == type)
		{
			odocStyle->bsemiHidden = true;
			odocStyle->semiHidden = (0 != m_oBufferedStream.GetUChar()) ? true : false;
		}
		else if(c_oSer_sts::Style_unhideWhenUsed == type)
		{
			odocStyle->bunhideWhenUsed = true;
			odocStyle->unhideWhenUsed = (0 != m_oBufferedStream.GetUChar()) ? true : false;
		}
		else if(c_oSer_sts::Style_TextPr == type)
		{
			rPr oNew_rPr(m_oFontTableWriter.m_mapFonts);
			res = oBinary_rPrReader.Read(length, &oNew_rPr);
			if(oNew_rPr.IsNoEmpty())
			{
				XmlUtils::CStringWriter oTempWriter;
				oNew_rPr.Write(&oTempWriter);
				odocStyle->TextPr = oTempWriter.GetData();
			}
		}
		else if(c_oSer_sts::Style_ParaPr == type)
		{
			XmlUtils::CStringWriter oTempWriter;
			oBinary_pPrReader.m_nCurNumId = -1;
			oBinary_pPrReader.m_nCurLvl = -1;
			res = oBinary_pPrReader.Read(length, &oTempWriter);
			odocStyle->ParaPr = oTempWriter.GetData();
		}
		else if(c_oSer_sts::Style_TablePr == type)
		{
			CWiterTblPr oWiterTblPr;
			READ1_DEF(length, res, oBinary_tblPrReader.Read_tblPr, &oWiterTblPr);
			odocStyle->TablePr = oWiterTblPr.Write();
		}
		else if(c_oSer_sts::Style_RowPr == type)
		{
			XmlUtils::CStringWriter oTempWriter;
			READ2_DEF(length, res, oBinary_tblPrReader.Read_RowPr, &oTempWriter);
            std::wstring sRowPr = oTempWriter.GetData();
			odocStyle->RowPr = sRowPr;
		}
		else if(c_oSer_sts::Style_CellPr == type)
		{
			XmlUtils::CStringWriter oTempWriter;
			READ2_DEF(length, res, oBinary_tblPrReader.Read_CellPr, &oTempWriter);
            std::wstring sCellPr = oTempWriter.GetData();
			odocStyle->CellPr = sCellPr;
		}
		else if(c_oSer_sts::Style_TblStylePr == type)
		{
			READ1_DEF(length, res, this->ReadTblStylePr, odocStyle);
		}
		else if(c_oSer_sts::Style_CustomStyle == type)
		{
			odocStyle->bCustom = true;
			odocStyle->Custom = m_oBufferedStream.GetBool();
		}
		else if(c_oSer_sts::Style_Aliases == type)
		{
			odocStyle->Aliases = m_oBufferedStream.GetString3(length);
		}
		else if(c_oSer_sts::Style_AutoRedefine == type)
		{
			odocStyle->bautoRedefine = true;
			odocStyle->autoRedefine = m_oBufferedStream.GetBool();
		}
		else if(c_oSer_sts::Style_Locked == type)
		{
			odocStyle->blocked = true;
			odocStyle->locked = m_oBufferedStream.GetBool();
		}
		else if(c_oSer_sts::Style_Personal == type)
		{
			odocStyle->bpersonal = true;
			odocStyle->personal = m_oBufferedStream.GetBool();
		}
		else if(c_oSer_sts::Style_PersonalCompose == type)
		{
			odocStyle->bpersonalCompose = true;
			odocStyle->personalCompose = m_oBufferedStream.GetBool();
		}
		else if(c_oSer_sts::Style_PersonalReply == type)
		{
			odocStyle->bpersonalReply = true;
			odocStyle->personalReply = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadTblStylePr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docStyle* odocStyle = static_cast<docStyle*>(poResult);
		if(c_oSerProp_tblStylePrType::TblStylePr == type)
		{
			tblStylePr otblStylePr;
			READ1_DEF(length, res, this->ReadTblStyleProperty, &otblStylePr);
			if(otblStylePr.bType && otblStylePr.Writer.GetCurSize() > 0)
			{
				XmlUtils::CStringWriter oCStringWriter;
				switch(otblStylePr.Type)
				{
                case ETblStyleOverrideType::tblstyleoverridetypeBand1Horz: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"band1Horz\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeBand1Vert: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"band1Vert\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeBand2Horz: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"band2Horz\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeBand2Vert: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"band2Vert\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeFirstCol: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"firstCol\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeFirstRow: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"firstRow\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeLastCol: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"lastCol\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeLastRow: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"lastRow\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeNeCell: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"neCell\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeNwCell: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"nwCell\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeSeCell: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"seCell\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeSwCell: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"swCell\">")));break;
                case ETblStyleOverrideType::tblstyleoverridetypeWholeTable: oCStringWriter.WriteString(std::wstring(_T("<w:tblStylePr w:type=\"wholeTable\">")));break;
				}
				oCStringWriter.Write(otblStylePr.Writer);
                oCStringWriter.WriteString(std::wstring(_T("</w:tblStylePr>")));
				odocStyle->TblStylePr.push_back(oCStringWriter.GetData());
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadTblStyleProperty(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		tblStylePr* ptblStylePr = static_cast<tblStylePr*>(poResult);
		if(c_oSerProp_tblStylePrType::Type == type)
		{
			ptblStylePr->bType = true;
			ptblStylePr->Type = m_oBufferedStream.GetUChar();

		}
		else if(c_oSerProp_tblStylePrType::RunPr == type)
		{
			rPr oNew_rPr(m_oFontTableWriter.m_mapFonts);
			res = oBinary_rPrReader.Read(length, &oNew_rPr);
			if(oNew_rPr.IsNoEmpty())
			{
				oNew_rPr.Write(&ptblStylePr->Writer);
			}
		}
		else if(c_oSerProp_tblStylePrType::ParPr == type)
		{
			XmlUtils::CStringWriter oTempWriter;
			res = oBinary_pPrReader.Read(length, &oTempWriter);
			if(oTempWriter.GetCurSize() > 0)
			{
                ptblStylePr->Writer.WriteString(std::wstring(_T("<w:pPr>")));
				ptblStylePr->Writer.Write(oTempWriter);
                ptblStylePr->Writer.WriteString(std::wstring(_T("</w:pPr>")));
			}
		}
		else if(c_oSerProp_tblStylePrType::TblPr == type)
		{
			CWiterTblPr oWiterTblPr;
			READ1_DEF(length, res, oBinary_tblPrReader.Read_tblPr, &oWiterTblPr);

            if(false == oWiterTblPr.IsEmpty())
				ptblStylePr->Writer.WriteString(oWiterTblPr.Write());
		}
		else if(c_oSerProp_tblStylePrType::TrPr == type)
		{
			XmlUtils::CStringWriter oTempWriter;
			READ2_DEF(length, res, oBinary_tblPrReader.Read_RowPr, &oTempWriter);

            if(oTempWriter.GetCurSize() > 0)
			{
                ptblStylePr->Writer.WriteString(std::wstring(_T("<w:trPr>")));
				ptblStylePr->Writer.Write(oTempWriter);
                ptblStylePr->Writer.WriteString(std::wstring(_T("</w:trPr>")));
			}
		}
		else if(c_oSerProp_tblStylePrType::TcPr == type)
		{
			XmlUtils::CStringWriter oTempWriter;
			READ2_DEF(length, res, oBinary_tblPrReader.Read_CellPr, &oTempWriter);
			if(oTempWriter.GetCurSize() > 0)
			{
                ptblStylePr->Writer.WriteString(std::wstring(_T("<w:tcPr>")));
				ptblStylePr->Writer.Write(oTempWriter);
                ptblStylePr->Writer.WriteString(std::wstring(_T("</w:tcPr>")));
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
};
class Binary_OtherTableReader : public Binary_CommonReader
{
	Writers::FileWriter& m_oFileWriter;
    std::wstring m_sFileInDir;
public:
    Binary_OtherTableReader(std::wstring sFileInDir, NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter) 
		: m_sFileInDir(sFileInDir), Binary_CommonReader(poBufferedStream), m_oFileWriter(oFileWriter)
	{
	}
	int Read()
	{
		int res = c_oSerConstants::ReadOk;
		READ_TABLE_DEF(res, this->ReadOtherContent, NULL);
		return res;
	}
	int ReadOtherContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerOtherTableTypes::ImageMap == type )
		{
			READ1_DEF(length, res, this->ReadImageMapContent, NULL);
		}
		else if(c_oSerOtherTableTypes::DocxTheme == type)
		{
			smart_ptr<PPTX::Theme> pTheme = new PPTX::Theme(NULL);
			pTheme->fromPPTY(&m_oBufferedStream);
			NSBinPptxRW::CXmlWriter xmlWriter;
			pTheme->toXmlWriter(&xmlWriter);
			m_oFileWriter.m_oTheme.m_sContent = xmlWriter.GetXmlString();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadImageMapContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerOtherTableTypes::ImageMap_Src == type )
		{
            std::wstring sImage(m_oBufferedStream.GetString3(length));
            std::wstring sFilePath;
			bool bDeleteFile = false;
			NSFile::CFileBinary oFile;
            if(0 == sImage.find(_T("data:")))
			{
				if(oFile.CreateTempFile())
					SerializeCommon::convertBase64ToImage(oFile, sImage);
			}
            else if(0 == sImage.find(_T("http:")) || 0 == sImage.find(_T("https:")) || 0 == sImage.find(_T("ftp:")) || 0 == sImage.find(_T("www")))
			{
				//url
				sFilePath = SerializeCommon::DownloadImage(sImage);
				bDeleteFile = true;
			}
			else
			{
				//local
                sFilePath = m_sFileInDir + _T("media") + FILE_SEPARATOR_STR + sImage;
			}

			//Проверяем что файл существует
			FILE* pFileNative = oFile.GetFileNative();
			if(NULL != pFileNative)
			{
				m_oFileWriter.m_oMediaWriter.AddImage2(pFileNative);
			}
			else if(NSFile::CFileBinary::Exists(sFilePath))
			{
				m_oFileWriter.m_oMediaWriter.AddImage(sFilePath);
				if(bDeleteFile)
					NSFile::CFileBinary::Remove(sFilePath);
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
};
class Binary_CommentsTableReader : public Binary_CommonReader
{
public:
	CComments m_oComments;
public:
	Binary_CommentsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter):Binary_CommonReader(poBufferedStream)
	{
	}
	int Read()
	{
		int res = c_oSerConstants::ReadOk;
		READ_TABLE_DEF(res, this->ReadComments, NULL);
		return res;
	};
	int ReadComments(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_CommentsType::Comment == type )
		{
			CComment* pComment = new CComment(m_oComments.m_oParaIdCounter, m_oComments.m_oFormatIdCounter);
			READ1_DEF(length, res, this->ReadCommentContent, pComment);
			if(pComment->bIdOpen && NULL == m_oComments.get(pComment->IdOpen))
				m_oComments.add(pComment);
			else
				RELEASEOBJECT(pComment);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadCommentContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CComment* pComment = static_cast<CComment*>(poResult);
		if ( c_oSer_CommentsType::Id == type )
		{
			pComment->bIdOpen = true;
			pComment->IdOpen = m_oBufferedStream.GetLong();
		}
		else if ( c_oSer_CommentsType::UserName == type )
		{
            std::wstring UserName(m_oBufferedStream.GetString3(length));
			pComment->UserName = UserName;
		}
		else if ( c_oSer_CommentsType::UserId == type )
		{
            std::wstring UserId(m_oBufferedStream.GetString3(length));
			pComment->UserId = UserId;
		}
		else if ( c_oSer_CommentsType::Date == type )
		{
            std::wstring Date(m_oBufferedStream.GetString3(length));
			pComment->Date = Date;
		}
		else if ( c_oSer_CommentsType::OOData == type )
		{
			std::wstring Data(m_oBufferedStream.GetString3(length));
			pComment->OOData = Data;
		}
		else if ( c_oSer_CommentsType::Text == type )
		{
            std::wstring Text(m_oBufferedStream.GetString3(length));
			pComment->Text = Text;
		}
		else if ( c_oSer_CommentsType::Solved == type )
		{
			pComment->bSolved = true;
			pComment->Solved = m_oBufferedStream.GetBool();
		}
		else if ( c_oSer_CommentsType::Replies == type )
		{
			READ1_DEF(length, res, this->ReadReplies, &pComment->replies);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadReplies(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		std::vector<CComment*>* paComments = static_cast<std::vector<CComment*>*>(poResult);
		if ( c_oSer_CommentsType::Comment == type )
		{
			CComment* pNewComment = new CComment(m_oComments.m_oParaIdCounter, m_oComments.m_oFormatIdCounter);
			READ1_DEF(length, res, this->ReadCommentContent, pNewComment);
			paComments->push_back(pNewComment);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
};
class Binary_SettingsTableReader : public Binary_CommonReader
{
	Binary_pPrReader m_oBinary_pPrReader;
	Writers::SettingWriter& m_oSettingWriter;
	Writers::FileWriter& m_oFileWriter;
public:
	Binary_SettingsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter):
		Binary_CommonReader(poBufferedStream),m_oSettingWriter(oFileWriter.m_oSettingWriter),m_oFileWriter(oFileWriter),m_oBinary_pPrReader(poBufferedStream, oFileWriter)
	{
	}
	int Read()
	{
		int res = c_oSerConstants::ReadOk;
		READ_TABLE_DEF(res, this->ReadSettings, NULL);
		return res;
	};
	int ReadSettings(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_SettingsType::ClrSchemeMapping == type )
		{
			int aSchemeMapping[] = { 0, 1, 2, 3, 4, 5, 10, 11, 8, 9, 6, 7 };
			READ2_DEF(length, res, this->ReadClrSchemeMapping, aSchemeMapping);
            std::wstring sSchemeMapping = _T("<w:clrSchemeMapping");
			for(int i = 0; i < 12; ++i)
			{
				switch(i)
				{
                case 0: sSchemeMapping += (_T(" w:accent1"));break;
                case 1: sSchemeMapping += (_T(" w:accent2"));break;
                case 2: sSchemeMapping += (_T(" w:accent3"));break;
                case 3: sSchemeMapping += (_T(" w:accent4"));break;
                case 4: sSchemeMapping += (_T(" w:accent5"));break;
                case 5: sSchemeMapping += (_T(" w:accent6"));break;
                case 6: sSchemeMapping += (_T(" w:bg1"));break;
                case 7: sSchemeMapping += (_T(" w:bg2"));break;
                case 8: sSchemeMapping += (_T(" w:followedHyperlink"));break;
                case 9: sSchemeMapping += (_T(" w:hyperlink"));break;
                case 10: sSchemeMapping += (_T(" w:t1"));break;
                case 11: sSchemeMapping += (_T(" w:t2"));break;
				}
				switch(aSchemeMapping[i])
				{
                case 0: sSchemeMapping += (_T("=\"accent1\""));break;
                case 1: sSchemeMapping += (_T("=\"accent2\""));break;
                case 2: sSchemeMapping += (_T("=\"accent3\""));break;
                case 3: sSchemeMapping += (_T("=\"accent4\""));break;
                case 4: sSchemeMapping += (_T("=\"accent5\""));break;
                case 5: sSchemeMapping += (_T("=\"accent6\""));break;
                case 6: sSchemeMapping += (_T("=\"dark1\""));break;
                case 7: sSchemeMapping += (_T("=\"dark2\""));break;
                case 8: sSchemeMapping += (_T("=\"followedHyperlink\""));break;
                case 9: sSchemeMapping += (_T("=\"hyperlink\""));break;
                case 10: sSchemeMapping += (_T("=\"light1\""));break;
                case 11: sSchemeMapping += (_T("=\"light2\""));break;
				}
			}
            sSchemeMapping += (_T("/>"));
			m_oSettingWriter.AddSetting(sSchemeMapping);
			m_oFileWriter.m_pDrawingConverter->LoadClrMap(sSchemeMapping);
		}
		else if ( c_oSer_SettingsType::DefaultTabStop == type )
		{
			double dDefTabStop = m_oBufferedStream.GetDouble();
			long nDefTabStop = SerializeCommon::Round(dDefTabStop * g_dKoef_mm_to_twips);
            std::wstring sXml;

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<w:defaultTabStop w:val=\"" + std::to_wstring(nDefTabStop) + L"\"/>");
		}
		else if ( c_oSer_SettingsType::MathPr == type )
		{	
			m_oFileWriter.m_oSettingWriter.AddSetting(_T("<m:mathPr>"));
			READ1_DEF(length, res, this->ReadMathPr, poResult);
			m_oFileWriter.m_oSettingWriter.AddSetting(_T("</m:mathPr>"));
		}
		else if ( c_oSer_SettingsType::TrackRevisions == type )
		{
			bool bTrackRevisions = m_oBufferedStream.GetBool();
			if (bTrackRevisions)
			{
				m_oFileWriter.m_oSettingWriter.AddSetting(L"<w:trackRevisions/>");
			}
			else
			{
				m_oFileWriter.m_oSettingWriter.AddSetting(L"<w:trackRevisions w:val=\"false\"/>");
			}
		}
		else if( c_oSer_SettingsType::FootnotePr == type )
		{
			OOX::Settings::CFtnDocProps oFtnProps;
			READ1_DEF(length, res, this->ReadFootnotePr, &oFtnProps);
			m_oFileWriter.m_oSettingWriter.AddSetting(oFtnProps.toXML());
		}
		else if( c_oSer_SettingsType::EndnotePr == type )
		{
			OOX::Settings::CEdnDocProps oEdnProps;
			READ1_DEF(length, res, this->ReadEndnotePr, &oEdnProps);
			m_oFileWriter.m_oSettingWriter.AddSetting(oEdnProps.toXML());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};	
	int ReadFootnotePr(BYTE type, long length, void* poResult)
	{
		OOX::Settings::CFtnDocProps* pFtnProps = static_cast<OOX::Settings::CFtnDocProps*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerNotes::PrFmt == type )
		{
			pFtnProps->m_oNumFmt.Init();
			READ1_DEF(length, res, m_oBinary_pPrReader.ReadNumFmt, pFtnProps->m_oNumFmt.GetPointer());
		}
		else if( c_oSerNotes::PrRestart == type )
		{
			pFtnProps->m_oNumRestart.Init();
			pFtnProps->m_oNumRestart->m_oVal.Init();
			pFtnProps->m_oNumRestart->m_oVal->SetValue((SimpleTypes::ERestartNumber)m_oBufferedStream.GetUChar());
		}
		else if( c_oSerNotes::PrStart == type )
		{
			pFtnProps->m_oNumStart.Init();
			pFtnProps->m_oNumStart->m_oVal.Init();
			pFtnProps->m_oNumStart->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if( c_oSerNotes::PrFntPos == type )
		{
			pFtnProps->m_oPos.Init();
			pFtnProps->m_oPos->m_oVal.Init();
			pFtnProps->m_oPos->m_oVal->SetValue((SimpleTypes::EFtnPos)m_oBufferedStream.GetUChar());
		}
		else if( c_oSerNotes::PrRef == type )
		{
			OOX::CFtnEdnSepRef* pRef = new OOX::CFtnEdnSepRef();
			pRef->m_eType = OOX::et_w_footnote;
			pRef->m_oId.Init();
			pRef->m_oId->SetValue(m_oBufferedStream.GetLong());
			pFtnProps->m_arrFootnote.push_back(pRef);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadEndnotePr(BYTE type, long length, void* poResult)
	{
		OOX::Settings::CEdnDocProps* pEdnProps = static_cast<OOX::Settings::CEdnDocProps*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if( c_oSerNotes::PrFmt == type )
		{
			pEdnProps->m_oNumFmt.Init();
			READ1_DEF(length, res, m_oBinary_pPrReader.ReadNumFmt, pEdnProps->m_oNumFmt.GetPointer());
		}
		else if( c_oSerNotes::PrRestart == type )
		{
			pEdnProps->m_oNumRestart.Init();
			pEdnProps->m_oNumRestart->m_oVal.Init();
			pEdnProps->m_oNumRestart->m_oVal->SetValue((SimpleTypes::ERestartNumber)m_oBufferedStream.GetUChar());
		}
		else if( c_oSerNotes::PrStart == type )
		{
			pEdnProps->m_oNumStart.Init();
			pEdnProps->m_oNumStart->m_oVal.Init();
			pEdnProps->m_oNumStart->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if( c_oSerNotes::PrEndPos == type )
		{
			pEdnProps->m_oPos.Init();
			pEdnProps->m_oPos->m_oVal.Init();
			pEdnProps->m_oPos->m_oVal->SetValue((SimpleTypes::EEdnPos)m_oBufferedStream.GetUChar());
		}
		else if( c_oSerNotes::PrRef == type )
		{
			OOX::CFtnEdnSepRef* pRef = new OOX::CFtnEdnSepRef();
			pRef->m_eType = OOX::et_w_endnote;
			pRef->m_oId.Init();
			pRef->m_oId->SetValue(m_oBufferedStream.GetLong());
			pEdnProps->m_arrEndnote.push_back(pRef);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int ReadMathPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_MathPrType::BrkBin == type )
		{
			READ2_DEF(length, res, this->ReadMathBrkBin, poResult);
		}
		else if ( c_oSer_MathPrType::BrkBinSub == type )
		{
			READ2_DEF(length, res, this->ReadMathBrkBinSub, poResult);
		}
		else if ( c_oSer_MathPrType::DefJc == type )
		{
			READ2_DEF(length, res, this->ReadMathDefJc, poResult);
		}
		else if ( c_oSer_MathPrType::DispDef == type )
		{
			READ2_DEF(length, res, this->ReadMathDispDef, poResult);
		}
		else if ( c_oSer_MathPrType::InterSp == type )
		{
			READ2_DEF(length, res, this->ReadMathInterSp, poResult);
		}
		else if ( c_oSer_MathPrType::IntLim == type )
		{
			READ2_DEF(length, res, this->ReadMathIntLim, poResult);
		}
		else if ( c_oSer_MathPrType::IntraSp == type )
		{
			READ2_DEF(length, res, this->ReadMathIntraSp, poResult);
		}
		else if ( c_oSer_MathPrType::LMargin == type )
		{
			READ2_DEF(length, res, this->ReadMathLMargin, poResult);
		}
		else if ( c_oSer_MathPrType::MathFont == type )
		{
			READ2_DEF(length, res, this->ReadMathMathFont, poResult);
		}
		else if ( c_oSer_MathPrType::NaryLim == type )
		{
			READ2_DEF(length, res, this->ReadMathNaryLim, poResult);
		}
		else if ( c_oSer_MathPrType::PostSp == type )
		{
			READ2_DEF(length, res, this->ReadMathPostSp, poResult);
		}
		else if ( c_oSer_MathPrType::PreSp == type )
		{
			READ2_DEF(length, res, this->ReadMathPreSp, poResult);
		}
		else if ( c_oSer_MathPrType::RMargin == type )
		{
			READ2_DEF(length, res, this->ReadMathRMargin, poResult);
		}
		else if ( c_oSer_MathPrType::SmallFrac == type )
		{
			READ2_DEF(length, res, this->ReadMathSmallFrac, poResult);
		}
		else if ( c_oSer_MathPrType::WrapIndent == type )
		{
			READ2_DEF(length, res, this->ReadMathWrapIndent, poResult);
		}
		else if ( c_oSer_MathPrType::WrapRight == type )
		{
			READ2_DEF(length, res, this->ReadMathWrapRight, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathBrkBin(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE BrkBin;
            std::wstring sBrkBin (_T("repeat"));
			BrkBin = m_oBufferedStream.GetUChar();			
			switch(BrkBin)
			{
				case 0: sBrkBin = _T("after");break;
				case 1: sBrkBin = _T("before");break;
				case 2: sBrkBin = _T("repeat");break;
			}			

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:brkBin m:val=\"" + sBrkBin + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathBrkBinSub(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE BrkBinSub;
            std::wstring sBrkBinSub (_T("--"));
			BrkBinSub = m_oBufferedStream.GetUChar();			
			switch(BrkBinSub)
			{
				case 0: sBrkBinSub = _T("+-");break;
				case 1: sBrkBinSub = _T("-+");break;
				case 2: sBrkBinSub = _T("--");break;
			}			
            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:brkBinSub m:val=\"" + sBrkBinSub + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathDefJc(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE DefJc;
            std::wstring sDefJc (_T("centerGroup"));
			DefJc = m_oBufferedStream.GetUChar();			
			switch(DefJc)
			{
				case 0: sDefJc = _T("center");break;
				case 1: sDefJc = _T("centerGroup");break;
				case 2: sDefJc = _T("left");break;
				case 3: sDefJc = _T("right");break;
			}			
            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:defJc m:val=\"" + sDefJc + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathDispDef(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:dispDef");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			m_oFileWriter.m_oSettingWriter.AddSetting(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathInterSp(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:interSp m:val=\"" + std::to_wstring(lVal) + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathIntLim(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE IntLim;
            std::wstring sIntLim (_T("subSup"));
			IntLim = m_oBufferedStream.GetUChar();			
			switch(IntLim)
			{
				case 0: sIntLim = _T("subSup");break;
				case 1: sIntLim = _T("undOvr");break;
			}			
            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:intLim m:val=\"" + sIntLim + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathIntraSp(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:intraSp m:val=\"" + std::to_wstring(lVal) + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathLMargin(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:lMargin m:val=\"" + std::to_wstring(lVal) + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathMathFont(BYTE type, long length, void* poResult)//ReadString
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
                std::wstring sFontName(m_oBufferedStream.GetString3(length));
				sFontName = XmlUtils::EncodeXmlString(sFontName);

                std::wstring sVal;
                sVal += (_T("<m:mathFont m:val=\""));
                sVal += (sFontName);
                sVal += (_T("\" />"));
				m_oFileWriter.m_oSettingWriter.AddSetting(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	
	int ReadMathNaryLim(BYTE type, long length, void* poResult)//Readbyte
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE NaryLim;
            std::wstring sNaryLim (_T("subSup"));
			NaryLim = m_oBufferedStream.GetUChar();			
			switch(NaryLim)
			{
				case 0: sNaryLim = _T("subSup");break;
				case 1: sNaryLim = _T("undOvr");break;
			}			
            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:naryLim m:val=\"" + sNaryLim + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}		
	int ReadMathPostSp(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:postSp m:val=\"" + std::to_wstring(lVal) + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathPreSp(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:preSp m:val=\"" + std::to_wstring(lVal)+ L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathRMargin(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:rMargin m:val=\"" + std::to_wstring(lVal) + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathSmallFrac(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:smallFrac m:val=");
			if (bVal)
				sVal += _T("\"true\" />");
			else
				sVal += _T("\"false\" />");
			m_oFileWriter.m_oSettingWriter.AddSetting(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathWrapIndent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());

            m_oFileWriter.m_oSettingWriter.AddSetting(L"<m:wrapIndent m:val=\"" + std::to_wstring(lVal) + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathWrapRight(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:wrapRight m:val=");
			if (bVal)
				sVal += _T("\"true\" />");
			else
				sVal += _T("\"false\" />");
			m_oFileWriter.m_oSettingWriter.AddSetting(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadClrSchemeMapping(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		int *paSchemeMapping = static_cast<int*>(poResult);
		if ( c_oSer_ClrSchemeMappingType::Accent1 <= type && type <= c_oSer_ClrSchemeMappingType::T2)
		{
			paSchemeMapping[type] = m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
};
class Binary_DocumentTableReader : public Binary_CommonReader
{
private:
    Binary_CommonReader2            oBinary_CommonReader2;

    Writers::FileWriter&            m_oFileWriter;
    Writers::FontTableWriter&       m_oFontTableWriter;
    Binary_pPrReader                oBinary_pPrReader;
    Binary_rPrReader                oBinary_rPrReader;
    Binary_tblPrReader              oBinary_tblPrReader;
    XmlUtils::CStringWriter*        m_pCurWriter;
    rPr                             m_oCur_rPr;
    rPr                             m_oMath_rPr;
    XmlUtils::CStringWriter         m_oCur_pPr;
    BYTE                            m_byteLastElemType;
    CComments*                      m_pComments;
public:
    Writers::ContentWriter&         m_oDocumentWriter;
    Writers::MediaWriter&           m_oMediaWriter;

    Binary_DocumentTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, Writers::ContentWriter& oDocumentWriter, CComments* pComments)
            : Binary_CommonReader(poBufferedStream)
            , m_oDocumentWriter(oDocumentWriter)
            , m_oFileWriter(oFileWriter)
            , m_oMediaWriter(oFileWriter.m_oMediaWriter)
            , m_oFontTableWriter(oFileWriter.m_oFontTableWriter)
            , oBinary_CommonReader2(poBufferedStream)
            , oBinary_pPrReader(poBufferedStream, oFileWriter)
            , oBinary_rPrReader(poBufferedStream, oFileWriter)
            , oBinary_tblPrReader(poBufferedStream, oFileWriter)
            , m_oCur_rPr(m_oFontTableWriter.m_mapFonts)
            , m_oMath_rPr(m_oFontTableWriter.m_mapFonts)
            , m_pComments(pComments)
	{
		m_byteLastElemType = c_oSerParType::Content;
		m_pCurWriter = NULL;
	}
	~Binary_DocumentTableReader()
	{
	}
	int Read()
	{
		int res = c_oSerConstants::ReadOk;
		READ_TABLE_DEF(res, this->ReadDocumentContent, NULL);
		return res;
    }
	XmlUtils::CStringWriter& GetRunStringWriter()
	{
		if(NULL != m_pCurWriter)
			return *m_pCurWriter;
		else
			return m_oDocumentWriter.m_oContent;
	}
	int ReadDocumentContentOut(long length)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadDocumentContent, NULL);
		return res;
	}
	int ReadDocumentContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerParType::Par == type )
		{
			m_byteLastElemType = c_oSerParType::Par;
			m_oCur_pPr.ClearNoAttack();

            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:p>")));
			READ1_DEF(length, res, this->ReadParagraph, NULL);
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</w:p>")));
		}
		else if(c_oSerParType::Table == type)
		{
			m_byteLastElemType = c_oSerParType::Table;
        //сбрасываем Shd
            oBinary_tblPrReader.m_sCurTableShd.clear();
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:tbl>")));
			READ1_DEF(length, res, this->ReadDocTable, &m_oDocumentWriter.m_oContent);
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</w:tbl>")));
		//сбрасываем Shd
            oBinary_tblPrReader.m_sCurTableShd.clear();
		}
		else if(c_oSerParType::Sdt == type)
		{
			SdtWraper oSdt(0);
			READ1_DEF(length, res, this->ReadSdt, &oSdt);
		}
		else if ( c_oSerParType::sectPr == type )
		{
			SectPr oSectPr;
			READ1_DEF(length, res, this->Read_SecPr, &oSectPr);
			m_oDocumentWriter.m_oSecPr.WriteString(oSectPr.Write());
			if(oSectPr.bEvenAndOddHeaders && oSectPr.EvenAndOddHeaders)
				m_oFileWriter.m_oSettingWriter.AddSetting(_T("<w:evenAndOddHeaders/>"));
		}
        else if ( c_oSerParType::Background == type )
        {
            Background oBackground;
			READ2_DEF(length, res, this->Read_Background, &oBackground);
            m_oDocumentWriter.m_oBackground.WriteString(oBackground.Write());
        }
		else if ( c_oSerParType::BookmarkStart == type )
		{
			OOX::Logic::CBookmarkStart oBookmarkStart;
			READ1_DEF(length, res, this->ReadBookmarkStart, &oBookmarkStart);
			m_oDocumentWriter.m_oContent.WriteString(oBookmarkStart.toXML());
		}
		else if ( c_oSerParType::BookmarkEnd == type )
		{
			OOX::Logic::CBookmarkEnd oBookmarkEnd;
			READ1_DEF(length, res, this->ReadBookmarkEnd, &oBookmarkEnd);
			m_oDocumentWriter.m_oContent.WriteString(oBookmarkEnd.toXML());
		}
		else if(c_oSerParType::JsaProject == type)
		{
			BYTE* pData = m_oBufferedStream.GetPointer(length);
			OOX::CPath sJsaProject = OOX::FileTypes::JsaProject.DefaultFileName();
			std::wstring filePath = m_oFileWriter.m_oDocumentWriter.m_sDir + FILE_SEPARATOR_STR + L"word"+ FILE_SEPARATOR_STR + sJsaProject.GetPath();

			NSFile::CFileBinary oFile;
			oFile.CreateFileW(filePath);
			oFile.WriteFile(pData, length);
			oFile.CloseFile();

			long lId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::JsaProject.RelationType(), sJsaProject.GetPath(), L"", &lId);
			m_oFileWriter.m_pDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(sJsaProject.GetExtention(false));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadParagraph(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerParType::pPr == type )
		{
			res = oBinary_pPrReader.Read(length, &m_oCur_pPr);
			if(m_oCur_pPr.GetCurSize() > 0)
			{
                m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:pPr>")));
				m_oDocumentWriter.m_oContent.Write(m_oCur_pPr);
                m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</w:pPr>")));
			}
		}
		else if ( c_oSerParType::Content == type )
		{
			READ1_DEF(length, res, this->ReadParagraphContent, NULL);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadParagraphContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerParType::Run == type )
		{
			m_oCur_rPr.Reset();
            GetRunStringWriter().WriteString(std::wstring(_T("<w:r>")));
			READ1_DEF(length, res, this->ReadRun, NULL);
            GetRunStringWriter().WriteString(std::wstring(_T("</w:r>")));
		}
		else if ( c_oSerParType::CommentStart == type )
		{
			long nId = 0;
			READ1_DEF(length, res, this->ReadComment, &nId);
			if(NULL != m_pComments)
			{
				CComment* pComment = m_pComments->get(nId);
				if(NULL != pComment)
				{
					int nNewId = m_pComments->getNextId(pComment->getCount());
					pComment->setFormatStart(nNewId);
                    GetRunStringWriter().WriteString(pComment->writeRef(std::wstring(_T("")), std::wstring(_T("w:commentRangeStart")), std::wstring(_T(""))));
				}
			}
		}
		else if ( c_oSerParType::CommentEnd == type )
		{
			long nId = 0;
			READ1_DEF(length, res, this->ReadComment, &nId);
			if(NULL != m_pComments)
			{
				CComment* pComment = m_pComments->get(nId);
				if(NULL != pComment && pComment->bIdFormat)
                    GetRunStringWriter().WriteString(pComment->writeRef(std::wstring(_T("")), std::wstring(_T("w:commentRangeEnd")), std::wstring(_T(""))));
			}
		}
		else if ( c_oSerParType::OMathPara == type )
		{
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<m:oMathPara>")));
			READ1_DEF(length, res, this->ReadMathOMathPara, poResult);
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</m:oMathPara>")));
		}
		else if ( c_oSerParType::OMath == type )
		{
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<m:oMath>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</m:oMath>")));
		}
		else if ( c_oSerParType::Hyperlink == type )
		{
			CHyperlink oHyperlink;
			READ1_DEF(length, res, this->ReadHyperlink, &oHyperlink);
			oHyperlink.Write(GetRunStringWriter());
		}
		else if ( c_oSerParType::FldSimple == type )
		{
			CFldSimple oFldSimple;
			READ1_DEF(length, res, this->ReadFldSimple, &oFldSimple);
			oFldSimple.Write(GetRunStringWriter());
		}
		else if ( c_oSerParType::Ins == type )
		{
			TrackRevision oTrackRevision;
			READ1_DEF(length, res, this->ReadDelIns, &oTrackRevision);
			oTrackRevision.Write(&GetRunStringWriter(), _T("w:ins"));
		}
		else if ( c_oSerParType::Del == type )
		{
			TrackRevision oTrackRevision;
			READ1_DEF(length, res, this->ReadDelIns, &oTrackRevision);
			oTrackRevision.Write(&GetRunStringWriter(), _T("w:del"));
		}
		else if ( c_oSerParType::MoveFrom == type )
		{
			TrackRevision oTrackRevision;
			READ1_DEF(length, res, this->ReadDelIns, &oTrackRevision);
			oTrackRevision.Write(&GetRunStringWriter(), _T("w:moveFrom"));
		}
		else if ( c_oSerParType::MoveTo == type )
		{
			TrackRevision oTrackRevision;
			READ1_DEF(length, res, this->ReadDelIns, &oTrackRevision);
			oTrackRevision.Write(&GetRunStringWriter(), _T("w:moveTo"));
		}
		else if ( c_oSerParType::MoveFromRangeStart == type )
		{
			OOX::Logic::CMoveFromRangeStart oMoveFromRangeStart;
			READ1_DEF(length, res, this->ReadMoveFromRangeStart, &oMoveFromRangeStart);
			GetRunStringWriter().WriteString(oMoveFromRangeStart.toXML());
		}
		else if ( c_oSerParType::MoveFromRangeEnd == type )
		{
			OOX::Logic::CMoveFromRangeEnd oMoveToRangeEnd;
			READ1_DEF(length, res, this->ReadMoveFromRangeEnd, &oMoveToRangeEnd);
			GetRunStringWriter().WriteString(oMoveToRangeEnd.toXML());
		}
		else if ( c_oSerParType::MoveToRangeStart == type )
		{
			OOX::Logic::CMoveToRangeStart oMoveToRangeStart;
			READ1_DEF(length, res, this->ReadMoveToRangeStart, &oMoveToRangeStart);
			GetRunStringWriter().WriteString(oMoveToRangeStart.toXML());
		}
		else if ( c_oSerParType::MoveToRangeEnd == type )
		{
			OOX::Logic::CMoveToRangeEnd oMoveToRangeEnd;
			READ1_DEF(length, res, this->ReadMoveToRangeEnd, &oMoveToRangeEnd);
			GetRunStringWriter().WriteString(oMoveToRangeEnd.toXML());
		}
		else if(c_oSerParType::Sdt == type)
		{
			SdtWraper oSdt(1);
			READ1_DEF(length, res, this->ReadSdt, &oSdt);
		}
		else if ( c_oSerParType::BookmarkStart == type )
		{
			OOX::Logic::CBookmarkStart oBookmarkStart;
			READ1_DEF(length, res, this->ReadBookmarkStart, &oBookmarkStart);
			m_oDocumentWriter.m_oContent.WriteString(oBookmarkStart.toXML());
		}
		else if ( c_oSerParType::BookmarkEnd == type )
		{
			OOX::Logic::CBookmarkEnd oBookmarkEnd;
			READ1_DEF(length, res, this->ReadBookmarkEnd, &oBookmarkEnd);
			m_oDocumentWriter.m_oContent.WriteString(oBookmarkEnd.toXML());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadDelIns(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pTrackRevision)
		else if ( c_oSerProp_RevisionType::Content == type )
		{
			pTrackRevision->content = new XmlUtils::CStringWriter();
			XmlUtils::CStringWriter* pPrevWriter = m_pCurWriter;
			m_pCurWriter = pTrackRevision->content;
			READ1_DEF(length, res, this->ReadParagraphContent, NULL);
			m_pCurWriter = pPrevWriter;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMoveFromRangeStart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CMoveFromRangeStart* pMoveFromRangeStart = static_cast<OOX::Logic::CMoveFromRangeStart*>(poResult);
		if (c_oSerMoveRange::Author == type)
		{
			pMoveFromRangeStart->m_sAuthor.Init();
			pMoveFromRangeStart->m_sAuthor->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerMoveRange::ColFirst == type)
		{
			pMoveFromRangeStart->m_oColFirst.Init();
			pMoveFromRangeStart->m_oColFirst->SetValue(m_oBufferedStream.GetLong());
		}
		else if (c_oSerMoveRange::ColLast == type)
		{
			pMoveFromRangeStart->m_oColLast.Init();
			pMoveFromRangeStart->m_oColLast->SetValue(m_oBufferedStream.GetLong());
		}
		else if (c_oSerMoveRange::Date == type)
        {
            std::wstring strValue = m_oBufferedStream.GetString3(length);

			pMoveFromRangeStart->m_oDate.Init();
            pMoveFromRangeStart->m_oDate->SetValue(strValue);
		}
		else if (c_oSerMoveRange::DisplacedByCustomXml == type)
		{
			pMoveFromRangeStart->m_oDisplacedByCustomXml.Init();
			pMoveFromRangeStart->m_oDisplacedByCustomXml->SetValue((SimpleTypes::EDisplacedByCustomXml)m_oBufferedStream.GetUChar());
		}
		else if (c_oSerMoveRange::Id == type)
		{
			pMoveFromRangeStart->m_oId.Init();
			pMoveFromRangeStart->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else if (c_oSerMoveRange::Name == type)
		{
			pMoveFromRangeStart->m_sName.Init();
			pMoveFromRangeStart->m_sName->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerMoveRange::UserId == type)
		{
			pMoveFromRangeStart->m_sUserId.Init();
			pMoveFromRangeStart->m_sUserId->append(m_oBufferedStream.GetString3(length));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMoveToRangeStart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CMoveToRangeStart* pMoveToRangeStart = static_cast<OOX::Logic::CMoveToRangeStart*>(poResult);
		if (c_oSerMoveRange::Author == type)
		{
			pMoveToRangeStart->m_sAuthor.Init();
			pMoveToRangeStart->m_sAuthor->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerMoveRange::ColFirst == type)
		{
			pMoveToRangeStart->m_oColFirst.Init();
			pMoveToRangeStart->m_oColFirst->SetValue(m_oBufferedStream.GetLong());
		}
		else if (c_oSerMoveRange::ColLast == type)
		{
			pMoveToRangeStart->m_oColLast.Init();
			pMoveToRangeStart->m_oColLast->SetValue(m_oBufferedStream.GetLong());
		}
		else if (c_oSerMoveRange::Date == type)
        {
            std::wstring strValue = m_oBufferedStream.GetString3(length);

			pMoveToRangeStart->m_oDate.Init();
            pMoveToRangeStart->m_oDate->SetValue(strValue);
		}
		else if (c_oSerMoveRange::DisplacedByCustomXml == type)
		{
			pMoveToRangeStart->m_oDisplacedByCustomXml.Init();
			pMoveToRangeStart->m_oDisplacedByCustomXml->SetValue((SimpleTypes::EDisplacedByCustomXml)m_oBufferedStream.GetUChar());
		}
		else if (c_oSerMoveRange::Id == type)
		{
			pMoveToRangeStart->m_oId.Init();
			pMoveToRangeStart->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else if (c_oSerMoveRange::Name == type)
		{
			pMoveToRangeStart->m_sName.Init();
			pMoveToRangeStart->m_sName->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerMoveRange::UserId == type)
		{
			pMoveToRangeStart->m_sUserId.Init();
			pMoveToRangeStart->m_sUserId->append(m_oBufferedStream.GetString3(length));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMoveFromRangeEnd(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CMoveFromRangeEnd* pMoveFromRangeEnd = static_cast<OOX::Logic::CMoveFromRangeEnd*>(poResult);
		if (c_oSerMoveRange::DisplacedByCustomXml == type)
		{
			pMoveFromRangeEnd->m_oDisplacedByCustomXml.Init();
			pMoveFromRangeEnd->m_oDisplacedByCustomXml->SetValue((SimpleTypes::EDisplacedByCustomXml)m_oBufferedStream.GetUChar());
		}
		else if (c_oSerMoveRange::Id == type)
		{
			pMoveFromRangeEnd->m_oId.Init();
			pMoveFromRangeEnd->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMoveToRangeEnd(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CMoveToRangeEnd* pMoveToRangeEnd = static_cast<OOX::Logic::CMoveToRangeEnd*>(poResult);
		if (c_oSerMoveRange::DisplacedByCustomXml == type)
		{
			pMoveToRangeEnd->m_oDisplacedByCustomXml.Init();
			pMoveToRangeEnd->m_oDisplacedByCustomXml->SetValue((SimpleTypes::EDisplacedByCustomXml)m_oBufferedStream.GetUChar());
		}
		else if (c_oSerMoveRange::Id == type)
		{
			pMoveToRangeEnd->m_oId.Init();
			pMoveToRangeEnd->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadFldChar(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CFldChar* pFldChar = static_cast<OOX::Logic::CFldChar*>(poResult);
		if ( c_oSer_FldSimpleType::CharType == type )
		{
			pFldChar->m_oFldCharType.Init();
			pFldChar->m_oFldCharType->SetValue((SimpleTypes::EFldCharType)m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadFldSimple(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CFldSimple* pFldSimple = static_cast<CFldSimple*>(poResult);
		if ( c_oSer_FldSimpleType::Instr == type )
			pFldSimple->sInstr = m_oBufferedStream.GetString3(length);
		else if ( c_oSer_FldSimpleType::Content == type )
		{
			XmlUtils::CStringWriter* pPrevWriter = m_pCurWriter;
			m_pCurWriter = &pFldSimple->writer;
			READ1_DEF(length, res, this->ReadParagraphContent, NULL);
			m_pCurWriter = pPrevWriter;
		}
		else if ( c_oSer_FldSimpleType::FFData == type )
		{
			OOX::Logic::CFFData oFFData;
			READ1_DEF(length, res, this->ReadFFData, &oFFData);
			pFldSimple->writer.WriteString(oFFData.toXML());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadFFData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CFFData* pFFData = static_cast<OOX::Logic::CFFData*>(poResult);
		if ( c_oSerFFData::CalcOnExit == type )
		{
			pFFData->m_oCalcOnExit.Init();
			pFFData->m_oCalcOnExit->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else if ( c_oSerFFData::CheckBox == type )
		{
			pFFData->m_oCheckBox.Init();
			READ1_DEF(length, res, this->ReadFFCheckBox, pFFData->m_oCheckBox.GetPointer());
		}
		else if ( c_oSerFFData::DDList == type )
		{
			pFFData->m_oDDList.Init();
			READ1_DEF(length, res, this->ReadDDList, pFFData->m_oDDList.GetPointer());
		}
		else if ( c_oSerFFData::Enabled == type )
		{
			pFFData->m_oEnabled.Init();
			pFFData->m_oEnabled->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else if ( c_oSerFFData::EntryMacro == type )
		{
			std::wstring sVal = m_oBufferedStream.GetString3(length);
			pFFData->m_oEntryMacro.Init();
			pFFData->m_oEntryMacro->m_oVal.Init();
			pFFData->m_oEntryMacro->m_oVal->SetValue(sVal);
		}
		else if ( c_oSerFFData::ExitMacro == type )
		{
			std::wstring sVal = m_oBufferedStream.GetString3(length);
			pFFData->m_oExitMacro.Init();
			pFFData->m_oExitMacro->m_oVal.Init();
			pFFData->m_oExitMacro->m_oVal->SetValue(sVal);
		}
		else if ( c_oSerFFData::HelpText == type )
		{
			pFFData->m_oHelpText.Init();
			READ1_DEF(length, res, this->ReadFFHelpText, pFFData->m_oHelpText.GetPointer());
		}
		else if ( c_oSerFFData::Label == type )
		{
			pFFData->m_oLabel.Init();
			pFFData->m_oLabel->m_oVal.Init();
			pFFData->m_oLabel->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerFFData::Name == type )
		{
			std::wstring sVal = m_oBufferedStream.GetString3(length);
			pFFData->m_oName.Init();
			pFFData->m_oName->m_oVal.Init();
			pFFData->m_oName->m_oVal->SetValue(sVal);
		}
		else if ( c_oSerFFData::StatusText == type )
		{
			pFFData->m_oStatusText.Init();
			READ1_DEF(length, res, this->ReadFFStatusText, pFFData->m_oStatusText.GetPointer());
		}
		else if ( c_oSerFFData::TabIndex == type )
		{
			pFFData->m_oTabIndex.Init();
			pFFData->m_oTabIndex->m_oVal.Init();
			pFFData->m_oTabIndex->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerFFData::TextInput == type )
		{
			pFFData->m_oTextInput.Init();
			READ1_DEF(length, res, this->ReadTextInput, pFFData->m_oTextInput.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadFFCheckBox(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CFFCheckBox* pFFCheckBox = static_cast<OOX::Logic::CFFCheckBox*>(poResult);
		if ( c_oSerFFData::CBChecked == type )
		{
			pFFCheckBox->m_oChecked.Init();
			pFFCheckBox->m_oChecked->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else if ( c_oSerFFData::CBDefault == type )
		{
			pFFCheckBox->m_oDefault.Init();
			pFFCheckBox->m_oDefault->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else if ( c_oSerFFData::CBSize == type )
		{
			pFFCheckBox->m_oSize.Init();
			pFFCheckBox->m_oSize->m_oVal.Init();
			pFFCheckBox->m_oSize->m_oVal->FromHps(m_oBufferedStream.GetULong());
		}
		else if ( c_oSerFFData::CBSizeAuto == type )
		{
			pFFCheckBox->m_oSizeAuto.Init();
			pFFCheckBox->m_oSizeAuto->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadDDList(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CFFDDList* pDDList = static_cast<OOX::Logic::CFFDDList*>(poResult);
		if ( c_oSerFFData::DLDefault == type )
		{
			pDDList->m_oDefault.Init();
			pDDList->m_oDefault->m_oVal.Init();
			pDDList->m_oDefault->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerFFData::DLResult == type )
		{
			pDDList->m_oResult.Init();
			pDDList->m_oResult->m_oVal.Init();
			pDDList->m_oResult->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerFFData::DLListEntry == type )
		{
			ComplexTypes::Word::String* pVal = new ComplexTypes::Word::String();
			pVal->m_sVal.Init();
			pVal->m_sVal->append(m_oBufferedStream.GetString3(length));
			pDDList->m_arrListEntry.push_back(pVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadFFHelpText(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		ComplexTypes::Word::CFFHelpText* pHelpText = static_cast<ComplexTypes::Word::CFFHelpText*>(poResult);
		if ( c_oSerFFData::HTType == type )
		{
			pHelpText->m_oType.Init();
			pHelpText->m_oType->SetValue((SimpleTypes::EInfoTextType)m_oBufferedStream.GetUChar());
		}
		else if ( c_oSerFFData::HTVal == type )
		{
			std::wstring sVal = m_oBufferedStream.GetString3(length);
			pHelpText->m_oVal.Init();
			pHelpText->m_oVal->SetValue(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadFFStatusText(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		ComplexTypes::Word::CFFStatusText* pStatusText = static_cast<ComplexTypes::Word::CFFStatusText*>(poResult);
		if ( c_oSerFFData::HTType == type )
		{
			pStatusText->m_oType.Init();
			pStatusText->m_oType->SetValue((SimpleTypes::EInfoTextType)m_oBufferedStream.GetUChar());
		}
		else if ( c_oSerFFData::HTVal == type )
		{
			std::wstring sVal = m_oBufferedStream.GetString3(length);
			pStatusText->m_oVal.Init();
			pStatusText->m_oVal->SetValue(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadTextInput(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CFFTextInput* pTextInput = static_cast<OOX::Logic::CFFTextInput*>(poResult);
		if ( c_oSerFFData::TIDefault == type )
		{
			pTextInput->m_oDefault.Init();
			pTextInput->m_oDefault->m_sVal.Init();
			pTextInput->m_oDefault->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if ( c_oSerFFData::TIFormat == type )
		{
			pTextInput->m_oFormat.Init();
			pTextInput->m_oFormat->m_sVal.Init();
			pTextInput->m_oFormat->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if ( c_oSerFFData::TIMaxLength == type )
		{
			pTextInput->m_oMaxLength.Init();
			pTextInput->m_oMaxLength->m_oVal.Init();
			pTextInput->m_oMaxLength->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerFFData::TIType == type )
		{
			pTextInput->m_oType.Init();
			pTextInput->m_oType->m_oVal.Init();
			pTextInput->m_oType->m_oVal->SetValue((SimpleTypes::EFFTextType)m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadBookmarkStart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CBookmarkStart* pBookmarkStart = static_cast<OOX::Logic::CBookmarkStart*>(poResult);
		if ( c_oSerBookmark::Id == type )
		{
			pBookmarkStart->m_oId.Init();
			pBookmarkStart->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerBookmark::Name == type )
		{
			pBookmarkStart->m_sName.Init();
			pBookmarkStart->m_sName->append(m_oBufferedStream.GetString3(length));
		}
		else if ( c_oSerBookmark::DisplacedByCustomXml == type )
		{
			pBookmarkStart->m_oDisplacedByCustomXml.Init();
			pBookmarkStart->m_oDisplacedByCustomXml->SetValue((SimpleTypes::EDisplacedByCustomXml)m_oBufferedStream.GetUChar());
		}
		else if ( c_oSerBookmark::ColFirst == type )
		{
			pBookmarkStart->m_oColFirst.Init();
			pBookmarkStart->m_oColFirst->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerBookmark::ColLast == type )
		{
			pBookmarkStart->m_oColLast.Init();
			pBookmarkStart->m_oColLast->SetValue(m_oBufferedStream.GetLong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadBookmarkEnd(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Logic::CBookmarkEnd* pBookmarkEnd = static_cast<OOX::Logic::CBookmarkEnd*>(poResult);
		if ( c_oSerBookmark::Id == type )
		{
			pBookmarkEnd->m_oId.Init();
			pBookmarkEnd->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerBookmark::DisplacedByCustomXml == type )
		{
			pBookmarkEnd->m_oDisplacedByCustomXml.Init();
			pBookmarkEnd->m_oDisplacedByCustomXml->SetValue((SimpleTypes::EDisplacedByCustomXml)m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int ReadHyperlink(BYTE type, long length, void* poResult)
	{		
		int res = c_oSerConstants::ReadOk;
		CHyperlink* pHyperlink = static_cast<CHyperlink*>(poResult);
		if ( c_oSer_HyperlinkType::Link == type )
			pHyperlink->sLink = m_oBufferedStream.GetString3(length);
		else if ( c_oSer_HyperlinkType::Anchor == type )
			pHyperlink->sAnchor = m_oBufferedStream.GetString3(length);
		else if ( c_oSer_HyperlinkType::Tooltip == type )
			pHyperlink->sTooltip = m_oBufferedStream.GetString3(length);
		else if ( c_oSer_HyperlinkType::History == type )
		{
			pHyperlink->bHistory = true;
			pHyperlink->History = m_oBufferedStream.GetBool();
		}
		else if ( c_oSer_HyperlinkType::DocLocation == type )
			pHyperlink->sDocLocation = m_oBufferedStream.GetString3(length);
		else if ( c_oSer_HyperlinkType::TgtFrame == type )
			pHyperlink->sTgtFrame = m_oBufferedStream.GetString3(length);
		else if ( c_oSer_HyperlinkType::Content == type )
		{
			XmlUtils::CStringWriter* pPrevWriter = m_pCurWriter;
			m_pCurWriter = &pHyperlink->writer;
			READ1_DEF(length, res, this->ReadParagraphContent, NULL);
			long rId;
            std::wstring sHref = XmlUtils::EncodeXmlString(pHyperlink->sLink);
            m_oFileWriter.m_pDrawingConverter->WriteRels(std::wstring(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink")), sHref, std::wstring(_T("External")), &rId);

            pHyperlink->rId = L"rId" + std::to_wstring(rId);
			m_pCurWriter = pPrevWriter;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathLong(BYTE type, long length, void* poResult)
	{		
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
			LONG lVal = m_oBufferedStream.GetLong();
		else
			res = c_oSerConstants::ReadUnknown;

		return res;
	}
	int ReadMathArgOut(long length)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadMathArg, NULL);
		return res;
	}
	int ReadMathArg(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Acc == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:acc>")));
			READ1_DEF(length, res, this->ReadMathAcc, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:acc>")));
		}
		else if ( c_oSer_OMathContentType::ArgPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:argPr>")));
			READ1_DEF(length, res, this->ReadMathArgPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:argPr>")));
		}
		else if ( c_oSer_OMathContentType::Bar == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:bar>")));
			READ1_DEF(length, res, this->ReadMathBar, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:bar>")));
		}
		else if ( c_oSer_OMathContentType::BorderBox == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:borderBox>")));
			READ1_DEF(length, res, this->ReadMathBorderBox, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:borderBox>")));
		}
		else if ( c_oSer_OMathContentType::Box == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:box>")));
			READ1_DEF(length, res, this->ReadMathBox, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:box>")));
		}
		else if ( c_oSer_OMathContentType::CtrlPr == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathContentType::Delimiter == type )
		{	
            GetRunStringWriter().WriteString(std::wstring(_T("<m:d>")));
			READ1_DEF(length, res, this->ReadMathDelimiter, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:d>")));
		}
		else if ( c_oSer_OMathContentType::EqArr == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:eqArr>")));
			READ1_DEF(length, res, this->ReadMathEqArr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:eqArr>")));
		}
		else if ( c_oSer_OMathContentType::Fraction == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:f>")));
			READ1_DEF(length, res, this->ReadMathFraction, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:f>")));
		}
		else if ( c_oSer_OMathContentType::Func == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:func>")));
			READ1_DEF(length, res, this->ReadMathFunc, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:func>")));
		}
		else if ( c_oSer_OMathContentType::GroupChr == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:groupChr>")));
			READ1_DEF(length, res, this->ReadMathGroupChr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:groupChr>")));
		}
		else if ( c_oSer_OMathContentType::LimLow == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:limLow>")));
			READ1_DEF(length, res, this->ReadMathLimLow, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:limLow>")));
		}
		else if ( c_oSer_OMathContentType::LimUpp == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:limUpp>")));
			READ1_DEF(length, res, this->ReadMathLimUpp, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:limUpp>")));
		}
		else if ( c_oSer_OMathContentType::Matrix == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:m>")));
			READ1_DEF(length, res, this->ReadMathMatrix, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:m>")));
		}
		else if ( c_oSer_OMathContentType::Nary == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:nary>")));
			READ1_DEF(length, res, this->ReadMathNary, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:nary>")));
		}
		else if ( c_oSer_OMathContentType::OMath == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:oMath>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:oMath>")));
		}
		else if ( c_oSer_OMathContentType::OMathPara == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:oMathPara>")));
			READ1_DEF(length, res, this->ReadMathOMathPara, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:oMathPara>")));
		}
		else if ( c_oSer_OMathContentType::Phant == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:phant>")));
			READ1_DEF(length, res, this->ReadMathPhant, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:phant>")));
		}
		else if ( c_oSer_OMathContentType::MRun == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:r>")));
			READ1_DEF(length, res, this->ReadMathMRun, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:r>")));
		}
		else if ( c_oSer_OMathContentType::Rad == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:rad>")));
			READ1_DEF(length, res, this->ReadMathRad, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:rad>")));
		}
		else if ( c_oSer_OMathContentType::SPre == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sPre>")));
			READ1_DEF(length, res, this->ReadMathSPre, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sPre>")));
		}
		else if ( c_oSer_OMathContentType::SSub == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sSub>")));
			READ1_DEF(length, res, this->ReadMathSSub, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sSub>")));
		}
		else if ( c_oSer_OMathContentType::SSubSup == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sSubSup>")));
			READ1_DEF(length, res, this->ReadMathSSubSup, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sSubSup>")));
		}
		else if ( c_oSer_OMathContentType::SSup == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sSup>")));
			READ1_DEF(length, res, this->ReadMathSSup, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sSup>")));
		}
		else if ( c_oSer_OMathContentType::BookmarkStart == type )
		{
			OOX::Logic::CBookmarkStart oBookmarkStart;
			READ1_DEF(length, res, this->ReadBookmarkStart, &oBookmarkStart);
			GetRunStringWriter().WriteString(oBookmarkStart.toXML());
		}
		else if ( c_oSer_OMathContentType::BookmarkEnd == type )
		{
			OOX::Logic::CBookmarkEnd oBookmarkEnd;
			READ1_DEF(length, res, this->ReadBookmarkEnd, &oBookmarkEnd);
			GetRunStringWriter().WriteString(oBookmarkEnd.toXML());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathAcc(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::AccPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:accPr>")));
			READ1_DEF(length, res, this->ReadMathAccPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:accPr>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int ReadMathAccPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Chr == type )
		{
			READ2_DEF(length, res, this->ReadMathChr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathAln(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:aln");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathAlnScr(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:alnScr");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathArgPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::ArgSz == type )
		{
			READ2_DEF(length, res, this->ReadMathArgSz, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathArgSz(BYTE type, long length, void* poResult)//GetLong
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:argSz"));
			if (lVal)
			{
                sVal += L" m:val=\"" + std::to_wstring(lVal) + L"\"";
			}
            sVal += (_T(" />"));
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBar(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::BarPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:barPr>")));
			READ1_DEF(length, res, this->ReadMathBarPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:barPr>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBarPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::Pos == type )
		{		
			READ2_DEF(length, res, this->ReadMathPos, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBaseJc(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE BaseJc;
            std::wstring sBaseJc (_T("top"));
			BaseJc = m_oBufferedStream.GetUChar();			
			switch(BaseJc)
			{
				case 0: sBaseJc = _T("bottom");break;
				case 1: sBaseJc = _T("center");break;
				case 2: sBaseJc = _T("inline");break;
				case 3: sBaseJc = _T("inside");break;
				case 4: sBaseJc = _T("outside");break;
                case 5: sBaseJc = _T("top");    break;
			}			
            GetRunStringWriter().WriteString(L"<m:baseJc m:val=\"" + sBaseJc + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBegChr(BYTE type, long length, void* poResult) //ReadString
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
                std::wstring sChr = GetMathText (length);
                std::wstring sVal;
                sVal += (_T("<m:begChr m:val=\""));
                sVal += (sChr);
                sVal += (_T("\" />"));

				GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBorderBox(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::BorderBoxPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:borderBoxPr>")));
			READ1_DEF(length, res, this->ReadMathBorderBoxPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:borderBoxPr>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{		
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBorderBoxPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{			
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::HideBot == type )
		{
			READ2_DEF(length, res, this->ReadMathHideBot, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::HideLeft == type )
		{		
			READ2_DEF(length, res, this->ReadMathHideLeft, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::HideRight == type )
		{		
			READ2_DEF(length, res, this->ReadMathHideRight, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::HideTop == type )
		{		
			READ2_DEF(length, res, this->ReadMathHideTop, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::StrikeBLTR == type )
		{		
			READ2_DEF(length, res, this->ReadMathStrikeBLTR, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::StrikeH == type )
		{		
			READ2_DEF(length, res, this->ReadMathStrikeH, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::StrikeTLBR == type )
		{		
			READ2_DEF(length, res, this->ReadMathStrikeTLBR, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::StrikeV == type )
		{		
			READ2_DEF(length, res, this->ReadMathStrikeV, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBox(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::BoxPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:boxPr>")));
			READ1_DEF(length, res, this->ReadMathBoxPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:boxPr>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{		
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBoxPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Aln == type )
		{
			READ2_DEF(length, res, this->ReadMathAln, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Brk == type )
		{
			READ2_DEF(length, res, this->ReadMathBrk, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{		
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::Diff == type )
		{		
			READ2_DEF(length, res, this->ReadMathDiff, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::NoBreak == type )
		{		
			READ2_DEF(length, res, this->ReadMathNoBreak, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::OpEmu == type )
		{		
			READ2_DEF(length, res, this->ReadMathOpEmu, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathBrk(BYTE type, long length, void* poResult)//GetLong
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::AlnAt == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:brk"));
			if (lVal)
			{
                sVal += L" m:alnAt=\"" + std::to_wstring(lVal)+ L"\"";
			}
            sVal += (_T(" />"));
			GetRunStringWriter().WriteString(sVal);
		}
		else if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
            bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal(_T("<m:brk/>"));
				GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathCGp(BYTE type, long length, void* poResult)//GetLong
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:cGp"));
			if (lVal)
			{
                sVal += L" m:val=\"" + std::to_wstring(lVal) + L"\"";
			}
            sVal += (_T("/>"));
            GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int ReadMathCGpRule(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:cGpRule"));
			if (lVal)
			{
                sVal += L" m:val=\"" + std::to_wstring(lVal)+ L"\"";
			}
            sVal += (_T(" />"));
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int ReadMathChr(BYTE type, long length, void* poResult)//ReadString
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
                std::wstring sChr = GetMathText (length);
                std::wstring sVal;
                sVal += (_T("<m:chr m:val=\""));
                sVal += (sChr);
                sVal += (_T("\" />"));
				GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathCount(BYTE type, long length, void* poResult)//GetLong
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:count"));
			if (lVal)
			{
                sVal += L" m:val=\"" + std::to_wstring(lVal) + L"\"";
			}
            sVal += (_T(" />"));
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathCSp(BYTE type, long length, void* poResult)//GetLong
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:cSp"));
			if (lVal)
			{
                sVal += L" m:val=\"" + std::to_wstring(lVal)+ L"\"";
			}
            sVal += (_T(" />"));
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathCtrlPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerRunType::rPr == type )
		{
			m_oMath_rPr.Reset();
			res = oBinary_rPrReader.Read(length, &m_oMath_rPr);
			if(m_oMath_rPr.IsNoEmpty())
			m_oMath_rPr.Write(&GetRunStringWriter());
		}
		else if ( c_oSerRunType::arPr == type )
		{
			PPTX::Logic::RunProperties rPr;
			m_oBufferedStream.Skip(1);//skip type
			rPr.fromPPTY(&m_oBufferedStream);
			rPr.m_name = L"a:rPr";
			//todo use one writer
			NSBinPptxRW::CXmlWriter oWriter;
			rPr.toXmlWriter(&oWriter);
			GetRunStringWriter().WriteString(oWriter.GetXmlString());
		}
		else if ( c_oSerRunType::del == type )
		{
			TrackRevision oRPrChange;
			READ1_DEF(length, res, this->ReadMathCtrlPrDelIns, &oRPrChange);
			oRPrChange.Write(&GetRunStringWriter(), _T("w:del"));
		}
		else if ( c_oSerRunType::ins == type )
		{
			TrackRevision oRPrChange;
			READ1_DEF(length, res, this->ReadMathCtrlPrDelIns, &oRPrChange);
			oRPrChange.Write(&GetRunStringWriter(), _T("w:ins"));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathCtrlPrDelIns(BYTE type, long length, void* poResult)
	{
		return oBinary_rPrReader.ReadrPrChange(type, length, poResult);
	}

	int ReadMathDelimiter(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::DelimiterPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:dPr>")));
			READ1_DEF(length, res, this->ReadMathDelimiterPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:dPr>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{		
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathDegHide(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:degHide");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathDiff(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:diff");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathDelimiterPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Column == type )
		{
			READ2_DEF(length, res, this->ReadMathLong, poResult);
		}	
		else if ( c_oSer_OMathBottomNodesType::BegChr == type )
		{
			READ2_DEF(length, res, this->ReadMathBegChr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::EndChr == type )
		{
			READ2_DEF(length, res, this->ReadMathEndChr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Grow == type )
		{
			READ2_DEF(length, res, this->ReadMathGrow, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::SepChr == type )
		{
			READ2_DEF(length, res, this->ReadMathSepChr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Shp == type )
		{
			READ2_DEF(length, res, this->ReadMathShp, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathEndChr(BYTE type, long length, void* poResult)//ReadString
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
                std::wstring sChr = GetMathText (length);
                std::wstring sVal;
                sVal += (_T("<m:endChr m:val=\""));
                sVal += (sChr);
                sVal += (_T("\" />"));
				GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathEqArr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::EqArrPr == type )
		{
			GetRunStringWriter().WriteString(std::wstring(_T("<m:eqArrPr>")));
			READ1_DEF(length, res, this->ReadMathEqArrPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:eqArrPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathEqArrPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Row == type )
		{
			READ2_DEF(length, res, this->ReadMathLong, poResult);
		}	
		else if ( c_oSer_OMathBottomNodesType::BaseJc == type )
		{
			READ2_DEF(length, res, this->ReadMathBaseJc, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::MaxDist == type )
		{
			READ2_DEF(length, res, this->ReadMathMaxDist, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::ObjDist == type )
		{
			READ2_DEF(length, res, this->ReadMathObjDist, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::RSp == type )
		{
			READ2_DEF(length, res, this->ReadMathRSp, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::RSpRule == type )
		{
			READ2_DEF(length, res, this->ReadMathRSpRule, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathFraction(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Den == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:den>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:den>")));
		}
		else if ( c_oSer_OMathContentType::FPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:fPr>")));
			READ1_DEF(length, res, this->ReadMathFPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:fPr>")));
		}
		else if ( c_oSer_OMathContentType::Num == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:num>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:num>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathFPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::Type == type )
		{
			READ2_DEF(length, res, this->ReadMathType, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathFunc(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::FName == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:fName>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:fName>")));
		}
		else if ( c_oSer_OMathContentType::FuncPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:funcPr>")));
			READ1_DEF(length, res, this->ReadMathFuncPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:funcPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathFuncPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathGroupChr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::GroupChrPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:groupChrPr>")));
			READ1_DEF(length, res, this->ReadMathGroupChrPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:groupChrPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathGroupChrPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Chr == type )
		{
			READ2_DEF(length, res, this->ReadMathChr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::Pos == type )
		{
			READ2_DEF(length, res, this->ReadMathPos, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::VertJc == type )
		{
			READ2_DEF(length, res, this->ReadMathVertJc, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathGrow(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:grow");
			if (!bVal)
				sVal += _T(" m:val=\"false\" />");
			else
				sVal += _T(" m:val=\"true\" />");

			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathHideBot(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:hideBot");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathHideLeft(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:hideLeft");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathHideRight(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:hideRight");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathHideTop(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:hideTop");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}		
	int ReadMathMJc(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE Jc;
            std::wstring sJc (_T("centerGroup"));
			Jc = m_oBufferedStream.GetUChar();			
			switch(Jc)
			{
				case 0:	sJc = _T("center");break;
				case 1:	sJc = _T("centerGroup");break;
				case 2:	sJc = _T("left");break;
				case 3:	sJc = _T("right");break;
			}			
            GetRunStringWriter().WriteString(L"<m:jc m:val=\"" + sJc + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathLimLoc(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE LimLoc;
            std::wstring sLimLoc (_T("subSup"));
			LimLoc = m_oBufferedStream.GetUChar();	
			switch(LimLoc)
			{
				case 0: sLimLoc = _T("subSup");break;
				case 1: sLimLoc = _T("undOvr");break;
			}		
            GetRunStringWriter().WriteString(L"<m:limLoc m:val=\"" + sLimLoc + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int ReadMathLimLow(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::Lim == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:lim>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:lim>")));
		}
		else if ( c_oSer_OMathContentType::LimLowPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:limLowPr>")));
			READ1_DEF(length, res, this->ReadMathLimLowPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:limLowPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathLimLowPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathLimUpp(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::Lim == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:lim>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:lim>")));
		}
		else if ( c_oSer_OMathContentType::LimUppPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:limUppPr>")));
			READ1_DEF(length, res, this->ReadMathLimUppPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:limUppPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathLimUppPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathLit(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:lit");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int ReadMathMatrix(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::MPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:mPr>")));
			READ1_DEF(length, res, this->ReadMathMPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:mPr>")));
		}
		else if ( c_oSer_OMathContentType::Mr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:mr>")));
			READ1_DEF(length, res, this->ReadMathMr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:mr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMaxDist(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:maxDist");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMc(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::McPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:mcPr>")));
			READ1_DEF(length, res, this->ReadMathMcPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:mcPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMcJc(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE McJc;
            std::wstring sMcJc (_T("center"));
			McJc = m_oBufferedStream.GetUChar();			
			switch(McJc)
			{
                case 0: sMcJc = _T("center");   break;
                case 1: sMcJc = _T("inside");   break;
                case 2: sMcJc = _T("left");     break;
                case 3: sMcJc = _T("outside");  break;
                case 4: sMcJc = _T("right");    break;
			}			
            GetRunStringWriter().WriteString(L"<m:mcJc m:val=\"" + sMcJc + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMcPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Count == type )
		{
			READ2_DEF(length, res, this->ReadMathCount, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::McJc == type )
		{
			READ2_DEF(length, res, this->ReadMathMcJc, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMcs(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Mc == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:mc>")));
			READ1_DEF(length, res, this->ReadMathMc, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:mc>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Row == type )
		{
			READ2_DEF(length, res, this->ReadMathLong, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Column == type )
		{
			READ2_DEF(length, res, this->ReadMathLong, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::BaseJc == type )
		{
			READ2_DEF(length, res, this->ReadMathBaseJc, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CGp == type )
		{
			READ2_DEF(length, res, this->ReadMathCGp, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CGpRule == type )
		{
			READ2_DEF(length, res, this->ReadMathCGpRule, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CSp == type )
		{
			READ2_DEF(length, res, this->ReadMathCSp, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::Mcs == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:mcs>")));
			READ1_DEF(length, res, this->ReadMathMcs, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:mcs>")));
		}
		else if ( c_oSer_OMathBottomNodesType::PlcHide == type )
		{
			READ2_DEF(length, res, this->ReadMathPlcHide, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::RSp == type )
		{
			READ2_DEF(length, res, this->ReadMathRSp, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::RSpRule == type )
		{
			READ2_DEF(length, res, this->ReadMathRSpRule, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathNary(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::NaryPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:naryPr>")));
			READ1_DEF(length, res, this->ReadMathNaryPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:naryPr>")));
		}
		else if ( c_oSer_OMathContentType::Sub == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sub>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sub>")));
		}
		else if ( c_oSer_OMathContentType::Sup == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sup>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sup>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathNaryPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Chr == type )
		{
			READ2_DEF(length, res, this->ReadMathChr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::Grow == type )
		{
			READ2_DEF(length, res, this->ReadMathGrow, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::LimLoc == type )
		{
			READ2_DEF(length, res, this->ReadMathLimLoc, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::SubHide == type )
		{
			READ2_DEF(length, res, this->ReadMathSubHide, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::SupHide == type )
		{
			READ2_DEF(length, res, this->ReadMathSupHide, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathNoBreak(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:noBreak");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathNor(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:nor");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathObjDist(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:objDist");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathOMathParaOut(long length)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadMathOMathPara, NULL);
		return res;
	}
	int ReadMathOMathPara(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::OMath == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:oMath>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:oMath>")));
		}
		else if ( c_oSer_OMathContentType::OMathParaPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:oMathParaPr>")));
			READ1_DEF(length, res, this->ReadMathOMathParaPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:oMathParaPr>")));
		}
		else if ( c_oSer_OMathContentType::Run == type )
		{
			m_oCur_rPr.Reset();
            GetRunStringWriter().WriteString(std::wstring(_T("<w:r>")));
			READ1_DEF(length, res, this->ReadRun, NULL);
            GetRunStringWriter().WriteString(std::wstring(_T("</w:r>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathOMathParaPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::MJc == type )
		{
			READ2_DEF(length, res, this->ReadMathMJc, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathOpEmu(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:opEmu");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathPhant(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::PhantPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:phantPr>")));
			READ1_DEF(length, res, this->ReadMathPhantPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:phantPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathPhantPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::Show == type )
		{
			READ2_DEF(length, res, this->ReadMathShow, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Transp == type )
		{
			READ2_DEF(length, res, this->ReadMathTransp, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::ZeroAsc == type )
		{
			READ2_DEF(length, res, this->ReadMathZeroAsc, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::ZeroDesc == type )
		{
			READ2_DEF(length, res, this->ReadMathZeroDesc, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::ZeroWid == type )
		{
			READ2_DEF(length, res, this->ReadMathZeroWid, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathPlcHide(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:plcHide");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE Pos;
            std::wstring sPos(_T("bot"));
			Pos = m_oBufferedStream.GetUChar();
			
			switch(Pos)
			{
				case 0: sPos = _T("bot");break;
				case 1: sPos = _T("top");break;
			}
			
            GetRunStringWriter().WriteString(L"<m:pos m:val=\"" + sPos + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
    std::wstring GetMathText (long length)
	{
        std::wstring strVal(m_oBufferedStream.GetString3(length));
		return XmlUtils::EncodeXmlString(strVal, true);
	}
	int ReadMathText(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
            std::wstring sText = GetMathText (length);
			GetRunStringWriter().WriteString(sText);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMRun(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::MText == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:t>")));

			READ2_DEF(length, res, this->ReadMathText, poResult);

            GetRunStringWriter().WriteString(std::wstring(_T("</m:t>")));
		}
		else if ( c_oSer_OMathContentType::RPr == type )
		{
			m_oMath_rPr.Reset();
			res = oBinary_rPrReader.Read(length, &m_oMath_rPr);
			if(m_oMath_rPr.IsNoEmpty())
				m_oMath_rPr.Write(&GetRunStringWriter());
		}
		else if ( c_oSer_OMathContentType::MRPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:rPr>")));
			READ1_DEF(length, res, this->ReadMathMRPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:rPr>")));
		}
		else if ( c_oSer_OMathContentType::ARPr == type )
		{
			PPTX::Logic::RunProperties rPr;
			m_oBufferedStream.Skip(1);//skip type
			rPr.fromPPTY(&m_oBufferedStream);
			rPr.m_name = L"a:rPr";
			//todo use one writer
			NSBinPptxRW::CXmlWriter oWriter;
			rPr.toXmlWriter(&oWriter);
			GetRunStringWriter().WriteString(oWriter.GetXmlString());
		}
		else if (c_oSer_OMathContentType::pagebreak == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"page\"/>")));
		}
		else if (c_oSer_OMathContentType::linebreak == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:br />")));
		}
		else if (c_oSer_OMathContentType::columnbreak == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"column\"/>")));
		}
		else if (c_oSer_OMathContentType::Del == type)
		{
			TrackRevision oTrackRevision;
			READ1_DEF(length, res, this->ReadMathInsDel, &oTrackRevision);
			oTrackRevision.Write(&GetRunStringWriter(), _T("w:del"));
		}
		else if (c_oSer_OMathContentType::Ins == type)
		{
			TrackRevision oTrackRevision;
			READ1_DEF(length, res, this->ReadMathInsDel, &oTrackRevision);
			oTrackRevision.Write(&GetRunStringWriter(), _T("w:ins"));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathInsDel(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		READ1_TRACKREV(type, length, pTrackRevision)
		else if(c_oSerProp_RevisionType::ContentRun == type)
		{
			pTrackRevision->contentRun = new XmlUtils::CStringWriter();
			XmlUtils::CStringWriter* pPrevWriter = m_pCurWriter;
			m_pCurWriter = pTrackRevision->contentRun;
			READ1_DEF(length, res, this->ReadMathMRun, NULL);
			m_pCurWriter = pPrevWriter;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadMathRad(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::Deg== type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:deg>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:deg>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else if ( c_oSer_OMathContentType::RadPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:radPr>")));
			READ1_DEF(length, res, this->ReadMathRadPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:radPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathRadPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else if ( c_oSer_OMathBottomNodesType::DegHide == type )
		{
			READ2_DEF(length, res, this->ReadMathDegHide, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathMRPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::Aln == type )
		{
			READ2_DEF(length, res, this->ReadMathAln, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Brk == type )
		{
			READ2_DEF(length, res, this->ReadMathBrk, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Lit == type )
		{
			READ2_DEF(length, res, this->ReadMathLit, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Nor == type )
		{
			READ2_DEF(length, res, this->ReadMathNor, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Scr == type )
		{
			READ2_DEF(length, res, this->ReadMathScr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::Sty == type )
		{
			READ2_DEF(length, res, this->ReadMathSty, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathRSp(BYTE type, long length, void* poResult)//GetLong
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:rSp"));
			if (lVal)
			{
                sVal += L" m:val=\"" + std::to_wstring(lVal) + L"\"";
			}
            sVal += (_T("/>"));
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathRSpRule(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			LONG lVal;
			lVal = m_oBufferedStream.GetLong();
            std::wstring sVal(_T("<m:rSpRule"));
			if (lVal)
			{
                sVal += L" m:val=\"" + std::to_wstring(lVal)+ L"\"";
			}
            sVal += (_T("/>"));
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathScr(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE Scr;
            std::wstring sScr (_T("roman"));
			Scr = m_oBufferedStream.GetUChar();			
			switch(Scr)
			{
				case 0: sScr = _T("double-struck");break;
                case 1: sScr = _T("fraktur");   break;
                case 2: sScr = _T("monospace"); break;
                case 3: sScr = _T("roman");     break;
				case 4: sScr = _T("sans-serif");break;
                case 5: sScr = _T("script");    break;
			}			
            GetRunStringWriter().WriteString(L"<m:scr m:val=\"" + sScr + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSepChr(BYTE type, long length, void* poResult)//ReadString
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
                std::wstring sChr = GetMathText (length);
                std::wstring sVal;
                sVal += (_T("<m:sepChr m:val=\""));
                sVal += (sChr);
                sVal += (_T("\" />"));
				GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathShow(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:show");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathShp(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE Shp;
            std::wstring sShp (_T("centered"));
			Shp = m_oBufferedStream.GetUChar();			
			switch(Shp)
			{
				case 0: sShp = _T("centered");break;
				case 1: sShp = _T("match");break;
			}			
            GetRunStringWriter().WriteString(L"<m:shp m:val=\"" + sShp + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSPre(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::SPrePr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sPrePr>")));
			READ1_DEF(length, res, this->ReadMathSPrePr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sPrePr>")));
		}
		else if ( c_oSer_OMathContentType::Sub == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sub>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sub>")));
		}
		else if ( c_oSer_OMathContentType::Sup == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sup>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sup>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSPrePr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSSub(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::SSubPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sSubPr>")));
			READ1_DEF(length, res, this->ReadMathSSubPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sSubPr>")));
		}
		else if ( c_oSer_OMathContentType::Sub == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sub>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sub>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSSubPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSSubSup(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::SSubSupPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sSubSupPr>")));
			READ1_DEF(length, res, this->ReadMathSSubSupPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sSubSupPr>")));
		}
		else if ( c_oSer_OMathContentType::Sub == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sub>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sub>")));
		}
		else if ( c_oSer_OMathContentType::Sup == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sup>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sup>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSSubSupPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::AlnScr == type )
		{
			READ2_DEF(length, res, this->ReadMathAlnScr, poResult);
		}
		else if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSSup(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathContentType::SSupPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sSupPr>")));
			READ1_DEF(length, res, this->ReadMathSSupPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sSupPr>")));
		}
		else if ( c_oSer_OMathContentType::Sup == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:sup>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:sup>")));
		}
		else if ( c_oSer_OMathContentType::Element == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:e>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:e>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSSupPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesType::CtrlPr == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:ctrlPr>")));
			READ1_DEF(length, res, this->ReadMathCtrlPr, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:ctrlPr>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathStrikeBLTR(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:strikeBLTR");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathStrikeH(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:strikeH");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathStrikeTLBR(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:strikeTLBR");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathStrikeV(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:strikeV");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSty(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE Sty;
            std::wstring sSty (_T("i"));
			Sty = m_oBufferedStream.GetUChar();	
			switch(Sty)
			{
				case 0: sSty = _T("b");break;
				case 1: sSty = _T("bi");break;
				case 2: sSty = _T("i");break;
				case 3: sSty = _T("p");break;
			}			
            GetRunStringWriter().WriteString(L"<m:sty m:val=\"" + sSty + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSubHide(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:subHide");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathSupHide(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:supHide");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathTransp(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:transp");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathType(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE Type;
            std::wstring sType (_T("bar"));
			Type = m_oBufferedStream.GetUChar();			
			switch(Type)
			{
				case 0: sType = _T("bar");break;
				case 1: sType = _T("lin");break;
				case 2: sType = _T("noBar");break;
				case 3: sType = _T("skw");break;
			}			
            GetRunStringWriter().WriteString(L"<m:type m:val=\"" + sType + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathVertJc(BYTE type, long length, void* poResult)//GetUChar
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			BYTE VertJc;
            std::wstring sVertJc (_T("bot"));
			VertJc = m_oBufferedStream.GetUChar();			
			switch(VertJc)
			{
				case 0: sVertJc = _T("bot");break;
				case 1: sVertJc = _T("top");break;
			}			
            GetRunStringWriter().WriteString(L"<m:vertJc m:val=\"" + sVertJc + L"\"/>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathZeroAsc(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:zeroAsc");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathZeroDesc(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:zaroDesc");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadMathZeroWid(BYTE type, long length, void* poResult)//GetBool
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSer_OMathBottomNodesValType::Val == type )
		{
			bool bVal = m_oBufferedStream.GetBool();
            std::wstring sVal = _T("<m:zeroWid");
			if (bVal)
				sVal += _T(" m:val=\"true\" />");
			else
				sVal += _T(" m:val=\"false\" />");
			GetRunStringWriter().WriteString(sVal);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadRun(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerRunType::rPr == type )
		{
			res = oBinary_rPrReader.Read(length, &m_oCur_rPr);
			if(m_oCur_rPr.IsNoEmpty())
				m_oCur_rPr.Write(&GetRunStringWriter());
		}
		else if ( c_oSerRunType::Content == type )
		{
			READ1_DEF(length, res, this->ReadRunContent, NULL);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadRunContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if (c_oSerRunType::run == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:t xml:space=\"preserve\">")));
            std::wstring sText(m_oBufferedStream.GetString3(length));
			sText = XmlUtils::EncodeXmlString(sText);
			GetRunStringWriter().WriteString(sText);
            GetRunStringWriter().WriteString(std::wstring(_T("</w:t>")));
		}
		else if (c_oSerRunType::delText == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:delText xml:space=\"preserve\">")));
            std::wstring sText(m_oBufferedStream.GetString3(length));
			sText = XmlUtils::EncodeXmlString(sText);
			GetRunStringWriter().WriteString(sText);
            GetRunStringWriter().WriteString(std::wstring(_T("</w:delText>")));
		}
		else if (c_oSerRunType::tab == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:tab/>")));
		}
		else if (c_oSerRunType::cr == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:cr/>")));
		}
		else if (c_oSerRunType::noBreakHyphen == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:noBreakHyphen/>")));
		}
		else if (c_oSerRunType::softHyphen == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:softHyphen/>")));
		}
		else if (c_oSerRunType::pagenum == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"begin\"/></w:r><w:r>")));
			if(m_oCur_rPr.IsNoEmpty())
				m_oCur_rPr.Write(&GetRunStringWriter());
            GetRunStringWriter().WriteString(std::wstring(_T("<w:instrText xml:space=\"preserve\">PAGE \\* MERGEFORMAT</w:instrText></w:r><w:r>")));
			if(m_oCur_rPr.IsNoEmpty())
				m_oCur_rPr.Write(&GetRunStringWriter());
            GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"separate\"/></w:r><w:r>")));
			if(m_oCur_rPr.IsNoEmpty())
				m_oCur_rPr.Write(&GetRunStringWriter());
            GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"end\"/>")));
		}
		else if (c_oSerRunType::pagebreak == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"page\"/>")));
		}
		else if (c_oSerRunType::linebreak == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:br />")));
		}
		else if (c_oSerRunType::columnbreak == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"column\"/>")));
		}
		else if(c_oSerRunType::image == type)
		{
			docImg odocImg(m_oFileWriter.getNextDocPr());
			READ2_DEF(length, res, this->ReadImage, &odocImg);
			if(odocImg.MediaId >= 0 && odocImg.MediaId < m_oMediaWriter.nImageCount)
			{
                std::wstring sNewImgName = m_oMediaWriter.m_aImageNames[odocImg.MediaId];
                std::wstring sNewImgRel = _T("media/") + sNewImgName;

                sNewImgRel = XmlUtils::EncodeXmlString(sNewImgRel);
				long rId;
                m_oFileWriter.m_pDrawingConverter->WriteRels(std::wstring(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image")), sNewImgRel, std::wstring(), &rId);
                odocImg.srId = L"rId" + std::to_wstring(rId);
				//odocImg.srId = m_oMediaWriter.m_poDocumentRelsWriter->AddRels(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"), sNewImgRel, false);
				//odocImg.srId = m_oMediaWriter.m_aImageRels[odocImg.MediaId];
                if(!odocImg.srId.empty())
				{
					odocImg.Write(&GetRunStringWriter());
				}
			}
		}
		else if(c_oSerRunType::pptxDrawing == type)
		{
			CDrawingProperty oCDrawingProperty(m_oFileWriter.getNextDocPr());
			READ2_DEF(length, res, this->ReadPptxDrawing, &oCDrawingProperty);

			if(oCDrawingProperty.IsChart())
			{
				GetRunStringWriter().WriteString(oCDrawingProperty.Write());
			}
			else if(oCDrawingProperty.bDataPos && oCDrawingProperty.bDataLength)
			{
                std::wstring sDrawingProperty = oCDrawingProperty.Write();
                if(false == sDrawingProperty.empty())
				{
					ReadDrawing(oCDrawingProperty);
				}
			}
		}
		else if(c_oSerRunType::table == type)
		{
			//сбрасываем Shd
            oBinary_tblPrReader.m_sCurTableShd.clear();
			//todo
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</w:p>")));
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:tbl>")));
			READ1_DEF(length, res, this->ReadDocTable, &m_oDocumentWriter.m_oContent);
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</w:tbl>")));
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:p>")));

            if(m_oCur_pPr.GetCurSize() > 0)
			{
                m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:pPr>")));
				m_oDocumentWriter.m_oContent.Write(m_oCur_pPr);
                m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</w:pPr>")));
			}
			//сбрасываем Shd
            oBinary_tblPrReader.m_sCurTableShd.clear();
		}
		else if(c_oSerRunType::fldstart_deprecated == type)
		{
            std::wstring sField(m_oBufferedStream.GetString3(length));
			sField = XmlUtils::EncodeXmlString(sField);
            GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"begin\"/></w:r><w:r>")));
			if(m_oCur_rPr.IsNoEmpty())
				m_oCur_rPr.Write(&GetRunStringWriter());
            GetRunStringWriter().WriteString(std::wstring(_T("<w:instrText xml:space=\"preserve\">")));
			GetRunStringWriter().WriteString(sField);
            GetRunStringWriter().WriteString(std::wstring(_T("</w:instrText></w:r><w:r>")));
			if(m_oCur_rPr.IsNoEmpty())
				m_oCur_rPr.Write(&GetRunStringWriter());
            GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"separate\"/>")));
		}
		else if(c_oSerRunType::fldend_deprecated == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"end\"/>")));
		}
		else if ( c_oSerRunType::CommentReference == type )
		{
			long nId = 0;
			READ1_DEF(length, res, this->ReadComment, &nId);
			if(NULL != m_pComments)
			{
				CComment* pComment = m_pComments->get(nId);
				if(NULL != pComment && pComment->bIdFormat)
				{
                    GetRunStringWriter().WriteString(pComment->writeRef(std::wstring(_T("")), std::wstring(_T("w:commentReference")), std::wstring(_T(""))));
				}
			}
		}
		else if ( c_oSerRunType::object == type)
		{
			READ1_DEF(length, res, this->ReadObject, poResult);
		}
		else if ( c_oSerRunType::separator == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:separator/>")));
		}
		else if ( c_oSerRunType::continuationSeparator == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:continuationSeparator/>")));
		}
		else if ( c_oSerRunType::footnoteRef == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:footnoteRef/>")));
		}
		else if ( c_oSerRunType::endnoteRef == type)
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<w:endnoteRef/>")));
		}
		else if ( c_oSerRunType::footnoteReference == type)
		{
			OOX::Logic::CFootnoteReference oFootnoteRef;
			READ1_DEF(length, res, this->ReadFootnoteRef, &oFootnoteRef);
			GetRunStringWriter().WriteString(oFootnoteRef.toXML());
		}
		else if ( c_oSerRunType::endnoteReference == type)
		{
			OOX::Logic::CEndnoteReference oEndnoteRef;
			READ1_DEF(length, res, this->ReadEndnoteRef, &oEndnoteRef);
			GetRunStringWriter().WriteString(oEndnoteRef.toXML());
		}
		else if ( c_oSerRunType::fldChar == type)
		{
			OOX::Logic::CFldChar oFldChar;
			READ1_DEF(length, res, this->ReadFldChar, &oFldChar);
			GetRunStringWriter().WriteString(oFldChar.toXML());
		}
		else if ( c_oSerRunType::instrText == type)
		{
			GetRunStringWriter().WriteString(std::wstring(_T("<w:instrText xml:space=\"preserve\">")));
			std::wstring sText(m_oBufferedStream.GetString3(length));
			sText = XmlUtils::EncodeXmlString(sText);
			GetRunStringWriter().WriteString(sText);
			GetRunStringWriter().WriteString(std::wstring(_T("</w:instrText>")));
		}
		else if ( c_oSerRunType::delInstrText == type)
		{
			GetRunStringWriter().WriteString(std::wstring(_T("<w:delInstrText xml:space=\"preserve\">")));
			std::wstring sText(m_oBufferedStream.GetString3(length));
			sText = XmlUtils::EncodeXmlString(sText);
			GetRunStringWriter().WriteString(sText);
			GetRunStringWriter().WriteString(std::wstring(_T("</w:delInstrText>")));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadFootnoteRef(BYTE type, long length, void* poResult)
	{
		OOX::Logic::CFootnoteReference* pFootnoteRef = static_cast<OOX::Logic::CFootnoteReference*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if (c_oSerNotes::RefCustomMarkFollows == type)
		{
			pFootnoteRef->m_oCustomMarkFollows.Init();
			pFootnoteRef->m_oCustomMarkFollows->FromBool(m_oBufferedStream.GetBool());
		}
		else if (c_oSerNotes::RefId == type)
		{
			pFootnoteRef->m_oId.Init();
			pFootnoteRef->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadEndnoteRef(BYTE type, long length, void* poResult)
	{
		OOX::Logic::CEndnoteReference* pEndnoteRef = static_cast<OOX::Logic::CEndnoteReference*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if (c_oSerNotes::RefCustomMarkFollows == type)
		{
			pEndnoteRef->m_oCustomMarkFollows.Init();
			pEndnoteRef->m_oCustomMarkFollows->FromBool(m_oBufferedStream.GetBool());
		}
		else if (c_oSerNotes::RefId == type)
		{
			pEndnoteRef->m_oId.Init();
			pEndnoteRef->m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	void ReadDrawing(CDrawingProperty &oCDrawingProperty)
	{
        std::wstring sDrawingProperty = oCDrawingProperty.Write();
        if(false == sDrawingProperty.empty())
		{
			long nCurPos = m_oBufferedStream.GetPos();
            std::wstring sDrawingXml;
            m_oFileWriter.m_pDrawingConverter->SaveObjectEx(oCDrawingProperty.DataPos, oCDrawingProperty.DataLength, sDrawingProperty, XMLWRITER_DOC_TYPE_DOCX, sDrawingXml);
			m_oBufferedStream.Seek(nCurPos);

            if( false == sDrawingXml.empty())
			{
                GetRunStringWriter().WriteString(sDrawingXml);
			}
		}
	}
	int ReadObject(BYTE type, long length, void* poResult)
	{
		CDrawingProperty oCDrawingProperty(m_oFileWriter.getNextDocPr());
		
		int res = c_oSerConstants::ReadOk;
		if( c_oSerParType::OMath == type )
		{
            GetRunStringWriter().WriteString(std::wstring(_T("<m:oMath>")));
			READ1_DEF(length, res, this->ReadMathArg, poResult);
            GetRunStringWriter().WriteString(std::wstring(_T("</m:oMath>")));
		}
		else if(c_oSerRunType::pptxDrawing == type) 
		{
			READ2_DEF(length, res, this->ReadPptxDrawing, &oCDrawingProperty);

            if(oCDrawingProperty.bDataPos && oCDrawingProperty.bDataLength)
            {
				ReadDrawing(oCDrawingProperty);
            }
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadComment(BYTE type, long length, void* poResult)
	{
		long* pVal = static_cast<long*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if (c_oSer_CommentsType::Id == type)
			*pVal = m_oBufferedStream.GetLong();
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadDocTable(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerDocTableType::tblPr == type )
		{
			CWiterTblPr oWiterTblPr;
			READ1_DEF(length, res, oBinary_tblPrReader.Read_tblPr, &oWiterTblPr);
			pCStringWriter->WriteString(oWiterTblPr.Write());
		}
		else if( c_oSerDocTableType::tblGrid == type )
		{
			oBinary_tblPrReader.m_aCurTblGrid.clear();
            pCStringWriter->WriteString(std::wstring(_T("<w:tblGrid>")));
			READ2_DEF(length, res, this->Read_tblGrid, poResult);
            pCStringWriter->WriteString(std::wstring(_T("</w:tblGrid>")));
		}
		else if( c_oSerDocTableType::Content == type )
		{
			READ1_DEF(length, res, this->Read_TableContent, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
};
	int Read_tblGrid(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerDocTableType::tblGrid_Item == type )
		{
			double dgridCol = m_oBufferedStream.GetDouble();
			oBinary_tblPrReader.m_aCurTblGrid.push_back(dgridCol);
			long ngridCol = SerializeCommon::Round( g_dKoef_mm_to_twips * dgridCol);

            pCStringWriter->WriteString(L"<w:gridCol w:w=\"" + std::to_wstring(ngridCol)+ L"\"/>");
		}
		else if( c_oSerDocTableType::tblGridChange == type )
		{
			TrackRevision oTrackRevision;
			READ1_DEF(length, res, this->Read_tblGridChange, &oTrackRevision);
			oTrackRevision.Write(pCStringWriter, _T("w:tblGridChange"));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int Read_tblGridChange(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
		if(c_oSerProp_RevisionType::Id == type)
		{
			pTrackRevision->Id = new long(m_oBufferedStream.GetLong());
		}
		else if(c_oSerProp_RevisionType::tblGridChange == type)
		{
			oBinary_tblPrReader.m_aCurTblGrid.clear();
			pTrackRevision->tblGridChange = new XmlUtils::CStringWriter();
			READ2_DEF(length, res, this->Read_tblGrid, pTrackRevision->tblGridChange);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int Read_TableContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerDocTableType::Row == type )
		{
            pCStringWriter->WriteString(std::wstring(_T("<w:tr>")));
			READ1_DEF(length, res, this->Read_Row, poResult);
            pCStringWriter->WriteString(std::wstring(_T("</w:tr>")));
		}
		else if(c_oSerDocTableType::Sdt == type)
		{
			SdtWraper oSdt(2);
			READ1_DEF(length, res, this->ReadSdt, &oSdt);
		}
		else if (c_oSerDocTableType::BookmarkStart == type)
		{
			OOX::Logic::CBookmarkStart oBookmarkStart;
			READ1_DEF(length, res, this->ReadBookmarkStart, &oBookmarkStart);
			pCStringWriter->WriteString(oBookmarkStart.toXML());
		}
		else if (c_oSerDocTableType::BookmarkEnd == type)
		{
			OOX::Logic::CBookmarkEnd oBookmarkEnd;
			READ1_DEF(length, res, this->ReadBookmarkEnd, &oBookmarkEnd);
			pCStringWriter->WriteString(oBookmarkEnd.toXML());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int Read_Row(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerDocTableType::Row_Pr == type )
		{
            pCStringWriter->WriteString(std::wstring(_T("<w:trPr>")));
			READ2_DEF(length, res, oBinary_tblPrReader.Read_RowPr, pCStringWriter);
            pCStringWriter->WriteString(std::wstring(_T("</w:trPr>")));
		}
		else if( c_oSerDocTableType::Row_Content == type )
		{
			READ1_DEF(length, res, this->ReadRowContent, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	};
	int ReadRowContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerDocTableType::Cell == type )
		{
            pCStringWriter->WriteString(std::wstring(_T("<w:tc>")));
			READ1_DEF(length, res, this->ReadCell, poResult);
            pCStringWriter->WriteString(std::wstring(_T("</w:tc>")));
		}
		else if(c_oSerDocTableType::Sdt == type)
		{
			SdtWraper oSdt(3);
			READ1_DEF(length, res, this->ReadSdt, &oSdt);
		}
		else if (c_oSerDocTableType::BookmarkStart == type)
		{
			OOX::Logic::CBookmarkStart oBookmarkStart;
			READ1_DEF(length, res, this->ReadBookmarkStart, &oBookmarkStart);
			pCStringWriter->WriteString(oBookmarkStart.toXML());
		}
		else if (c_oSerDocTableType::BookmarkEnd == type)
		{
			OOX::Logic::CBookmarkEnd oBookmarkEnd;
			READ1_DEF(length, res, this->ReadBookmarkEnd, &oBookmarkEnd);
			pCStringWriter->WriteString(oBookmarkEnd.toXML());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadCell(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		XmlUtils::CStringWriter* pCStringWriter = static_cast<XmlUtils::CStringWriter*>(poResult);
		if( c_oSerDocTableType::Cell_Pr == type )
		{
            pCStringWriter->WriteString(std::wstring(_T("<w:tcPr>")));
			oBinary_tblPrReader.bCellShd = false;
			READ2_DEF(length, res, oBinary_tblPrReader.Read_CellPr, pCStringWriter);
            if(false == oBinary_tblPrReader.bCellShd && !oBinary_tblPrReader.m_sCurTableShd.empty())
			{
				pCStringWriter->WriteString(oBinary_tblPrReader.m_sCurTableShd);
			}
			oBinary_tblPrReader.bCellShd = false;
            pCStringWriter->WriteString(std::wstring(_T("</w:tcPr>")));
		}
		else if( c_oSerDocTableType::Cell_Content == type )
		{
			Binary_DocumentTableReader oBinary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, m_oDocumentWriter, m_pComments);
			READ1_DEF(length, res, this->ReadCellContent, &oBinary_DocumentTableReader);
			//Потому что если перед </tc> не идет <p>, то документ считается невалидным
			if(c_oSerParType::Par != oBinary_DocumentTableReader.m_byteLastElemType)
			{
                m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:p />")));
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
    }
	int ReadCellContent(BYTE type, long length, void* poResult)
	{
		Binary_DocumentTableReader* pBinary_DocumentTableReader = static_cast<Binary_DocumentTableReader*>(poResult);
		return pBinary_DocumentTableReader->ReadDocumentContent(type, length, NULL);
	}
	int ReadImage(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		docImg* odocImg = static_cast<docImg*>(poResult);
		if ( c_oSerImageType::MediaId == type )
		{
			odocImg->bMediaId = true;
			odocImg->MediaId = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerImageType::Type == type )
		{
			odocImg->bType = true;
			odocImg->Type = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerImageType::Width == type )
		{
			odocImg->bWidth = true;
			odocImg->Width = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType::Height == type )
		{
			odocImg->bHeight = true;
			odocImg->Height = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType::X == type )
		{
			odocImg->bX = true;
			odocImg->X = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType::Y == type )
		{
			odocImg->bY = true;
			odocImg->Y = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType::Padding == type )
		{
			odocImg->bPaddings = true;
			READ2_DEF(length, res, oBinary_tblPrReader.ReadPaddings, &odocImg->Paddings);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
    int Read_Background(BYTE type, long length, void* poResult)
    {
		m_oFileWriter.m_oSettingWriter.AddSetting(L"<w:displayBackgroundShape/>");
		
		int res = c_oSerConstants::ReadOk;
        Background* pBackground = static_cast<Background*>(poResult);

        if( c_oSerBackgroundType::Color == type )
        {
            pBackground->bColor = true;
            pBackground->Color = oBinary_CommonReader2.ReadColor();
        }
        else if( c_oSerBackgroundType::ColorTheme == type )
        {
            pBackground->bThemeColor = true;
            oBinary_CommonReader2.ReadThemeColor(length, pBackground->ThemeColor);
        }
        else if( c_oSerBackgroundType::pptxDrawing == type )
        {
			CDrawingProperty oCDrawingProperty(m_oFileWriter.getNextDocPr());
			READ2_DEF(length, res, this->ReadPptxDrawing, &oCDrawingProperty);

			if (oCDrawingProperty.bDataPos && oCDrawingProperty.bDataLength)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				pBackground->sObject = m_oFileWriter.m_pDrawingConverter->SaveObjectBackground(oCDrawingProperty.DataPos, oCDrawingProperty.DataLength);
				m_oBufferedStream.Seek(nCurPos);
			}
        }
        else
            res = c_oSerConstants::ReadUnknown;
        return res;
    }

	int ReadPptxDrawing(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
		if ( c_oSerImageType2::Type == type )
		{
			pDrawingProperty->bType = true;
			pDrawingProperty->Type = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerImageType2::PptxData == type )
		{
			pDrawingProperty->bDataPos = true;
			pDrawingProperty->bDataLength = true;
			pDrawingProperty->DataPos = m_oBufferedStream.GetPos();
			pDrawingProperty->DataLength = length;
		//сейчас пропуская, потому что перед чтение этого поля надо собрать остальные данные
			res = c_oSerConstants::ReadUnknown;
		}
		else if ( c_oSerImageType2::Chart2 == type )
		{
			if(false == m_oFileWriter.m_bSaveChartAsImg)
			{
				OOX::CPath pathChartsDir = m_oFileWriter.m_oChartWriter.m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"charts";
				OOX::CSystemUtility::CreateDirectories(pathChartsDir.GetPath());
				
				OOX::CPath pathChartsRelsDir = pathChartsDir.GetPath() + FILE_SEPARATOR_STR +  L"_rels";                
				OOX::CSystemUtility::CreateDirectories(pathChartsRelsDir.GetPath());

				OOX::CPath pathChartsWorksheetDir = m_oFileWriter.m_oChartWriter.m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"embeddings";
				OOX::CSystemUtility::CreateDirectories(pathChartsWorksheetDir.GetPath());

                int nativeDocumentType = m_oFileWriter.m_pDrawingConverter->m_pImageManager->m_nDocumentType;

                m_oFileWriter.m_pDrawingConverter->m_pImageManager->m_nDocumentType = XMLWRITER_DOC_TYPE_XLSX;
				m_oFileWriter.m_pDrawingConverter->SetDstContentRels();
				
				std::wstring sThemePath = m_oFileWriter.m_sThemePath;

				size_t nPos = sThemePath.rfind(FILE_SEPARATOR_STR);
				if (std::wstring::npos != nPos)
				{
					sThemePath = sThemePath.substr(0, nPos);
				}
					
				BinXlsxRW::SaveParams			oSaveParams(sThemePath, m_oFileWriter.m_pDrawingConverter->GetContentTypes());
				BinXlsxRW::BinaryChartReader	oBinaryChartReader(m_oBufferedStream, oSaveParams, m_oFileWriter.m_pDrawingConverter);
				
				OOX::Spreadsheet::CChartSpace* pChartSpace = new OOX::Spreadsheet::CChartSpace(NULL);
				oBinaryChartReader.ReadCT_ChartSpace(length, &pChartSpace->m_oChartSpace);

				//save xlsx
				_INT32 nChartCount = m_oFileWriter.m_pDrawingConverter->GetDocumentChartsCount();
				_INT32 nChartIndex = nChartCount + 1;
				m_oFileWriter.m_pDrawingConverter->SetDocumentChartsCount(nChartCount + 1);
				std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring(nChartIndex) + L".xlsx";
				std::wstring sXlsxPath = pathChartsWorksheetDir.GetPath() + FILE_SEPARATOR_STR + sXlsxFilename;
				BinXlsxRW::CXlsxSerializer oXlsxSerializer;
				if (oXlsxSerializer.writeChartXlsx(sXlsxPath, *pChartSpace))
				{
					std::wstring sChartsWorksheetRelsName = L"../embeddings/" + sXlsxFilename;
					long rIdXlsx;
					std::wstring bstrChartsWorksheetRelType = OOX::FileTypes::MicrosoftOfficeExcelWorksheet.RelationType();

					m_oFileWriter.m_pDrawingConverter->WriteRels(bstrChartsWorksheetRelType, sChartsWorksheetRelsName, std::wstring(), &rIdXlsx);
					m_oFileWriter.m_pDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(L"xlsx");

					pChartSpace->m_oChartSpace.m_externalData = new OOX::Spreadsheet::CT_ExternalData();
					pChartSpace->m_oChartSpace.m_externalData->m_id = new std::wstring();
					pChartSpace->m_oChartSpace.m_externalData->m_id->append(L"rId");
					pChartSpace->m_oChartSpace.m_externalData->m_id->append(std::to_wstring(rIdXlsx));
					pChartSpace->m_oChartSpace.m_externalData->m_autoUpdate = new OOX::Spreadsheet::CT_Boolean();
					pChartSpace->m_oChartSpace.m_externalData->m_autoUpdate->m_val = new bool(false);
				}

			//save chart.xml
				NSStringUtils::CStringBuilder sw;
				pChartSpace->toXML(sw);
			
				std::wstring sFilename;
				std::wstring sRelsName;
                std::wstring sContent = sw.GetData();
                
				m_oFileWriter.m_oChartWriter.AddChart(sContent, sRelsName, sFilename, nChartIndex);

                OOX::CPath pathChartsRels =  pathChartsRelsDir.GetPath() + FILE_SEPARATOR_STR + sFilename + L".rels";
				m_oFileWriter.m_pDrawingConverter->SaveDstContentRels(pathChartsRels.GetPath());

				long rIdChart;
                std::wstring bstrChartRelType = OOX::FileTypes::Chart.RelationType();
				
				m_oFileWriter.m_pDrawingConverter->WriteRels(bstrChartRelType, sRelsName, std::wstring(), &rIdChart);
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml", L"/word/charts", sFilename);

                pDrawingProperty->sChartRels = L"rId" + std::to_wstring( rIdChart);

                m_oFileWriter.m_pDrawingConverter->m_pImageManager->m_nDocumentType = nativeDocumentType;
			}
			else
				res = c_oSerConstants::ReadUnknown;
		}
		else if ( c_oSerImageType2::ChartImg == type )
		{
			if(true == m_oFileWriter.m_bSaveChartAsImg)
			{
				pDrawingProperty->bDataPos = true;
				pDrawingProperty->bDataLength = true;
				pDrawingProperty->DataPos = m_oBufferedStream.GetPos();
				pDrawingProperty->DataLength = length;
		//сейчас пропуская, потому что перед чтение этого поля надо собрать остальные данные
			}
			res = c_oSerConstants::ReadUnknown;
		}
		else if ( c_oSerImageType2::BehindDoc == type )
		{
			pDrawingProperty->bBehindDoc = true;
			pDrawingProperty->BehindDoc = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerImageType2::DistL == type )
		{
			pDrawingProperty->bDistL = true;
			pDrawingProperty->DistL = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType2::DistT == type )
		{
			pDrawingProperty->bDistT = true;
			pDrawingProperty->DistT = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType2::DistR == type )
		{
			pDrawingProperty->bDistR = true;
			pDrawingProperty->DistR = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType2::DistB == type )
		{
			pDrawingProperty->bDistB = true;
			pDrawingProperty->DistB = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerImageType2::LayoutInCell == type )
		{
			pDrawingProperty->bLayoutInCell = true;
			pDrawingProperty->LayoutInCell = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerImageType2::RelativeHeight == type )
		{
			pDrawingProperty->bRelativeHeight = true;
			pDrawingProperty->RelativeHeight = m_oBufferedStream.GetULong();
		}
		else if ( c_oSerImageType2::BSimplePos == type )
		{
			pDrawingProperty->bBSimplePos = true;
			pDrawingProperty->BSimplePos = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerImageType2::EffectExtent == type )
		{
			READ2_DEF(length, res, this->ReadEffectExtent, poResult);
		}
		else if ( c_oSerImageType2::Extent == type )
		{
			READ2_DEF(length, res, this->ReadExtent, poResult);
		}
		else if ( c_oSerImageType2::PositionH == type )
		{
			READ2_DEF(length, res, this->ReadPositionH, poResult);
		}
		else if ( c_oSerImageType2::PositionV == type )
		{
			READ2_DEF(length, res, this->ReadPositionV, poResult);
		}
		else if ( c_oSerImageType2::SimplePos == type )
		{
			READ2_DEF(length, res, this->ReadSimplePos, poResult);
		}
		else if ( c_oSerImageType2::SizeRelH == type )
		{
			OOX::Drawing::CSizeRelH oSizeRelH;
			READ2_DEF(length, res, this->ReadSizeRelH, &oSizeRelH);
			pDrawingProperty->sSizeRelH = oSizeRelH.toXML();
		}
		else if ( c_oSerImageType2::SizeRelV == type )
		{
			OOX::Drawing::CSizeRelV oSizeRelV;
			READ2_DEF(length, res, this->ReadSizeRelV, &oSizeRelV);
			pDrawingProperty->sSizeRelV = oSizeRelV.toXML();
		}
		else if ( c_oSerImageType2::WrapNone == type )
		{
			pDrawingProperty->bDrawingPropertyWrap = true;
			pDrawingProperty->DrawingPropertyWrap.bWrappingType = true;
			pDrawingProperty->DrawingPropertyWrap.WrappingType = type;
		}
		else if ( c_oSerImageType2::WrapSquare == type )
		{
			pDrawingProperty->bDrawingPropertyWrap = true;
			pDrawingProperty->DrawingPropertyWrap.bWrappingType = true;
			pDrawingProperty->DrawingPropertyWrap.WrappingType = type;
			READ2_DEF(length, res, this->ReadEmptyWrap, poResult);
		}
		else if ( c_oSerImageType2::WrapThrough == type )
		{
			pDrawingProperty->bDrawingPropertyWrap = true;
			pDrawingProperty->DrawingPropertyWrap.bWrappingType = true;
			pDrawingProperty->DrawingPropertyWrap.WrappingType = type;
			READ2_DEF(length, res, this->ReadWrapThroughTight, &pDrawingProperty->DrawingPropertyWrap);
		}
		else if ( c_oSerImageType2::WrapTight == type )
		{
			pDrawingProperty->bDrawingPropertyWrap = true;
			pDrawingProperty->DrawingPropertyWrap.bWrappingType = true;
			pDrawingProperty->DrawingPropertyWrap.WrappingType = type;
			READ2_DEF(length, res, this->ReadWrapThroughTight, &pDrawingProperty->DrawingPropertyWrap);
		}
		else if ( c_oSerImageType2::WrapTopAndBottom == type )
		{
			pDrawingProperty->bDrawingPropertyWrap = true;
			pDrawingProperty->DrawingPropertyWrap.bWrappingType = true;
			pDrawingProperty->DrawingPropertyWrap.WrappingType = type;
			READ2_DEF(length, res, this->ReadEmptyWrap, poResult);
		}
		else if ( c_oSerImageType2::GraphicFramePr == type )
		{
			PPTX::Logic::CNvGraphicFramePr oGraphicFramePr(L"wp");
			READ2_DEF(length, res, this->ReadCNvGraphicFramePr, &oGraphicFramePr);
			pDrawingProperty->sGraphicFramePr = oGraphicFramePr.toXML();
		}
		else if ( c_oSerImageType2::DocPr == type )
		{
			PPTX::Logic::CNvPr pNonVisualDrawingProps(L"wp");
			READ1_DEF(length, res, this->ReadDocPr, &pNonVisualDrawingProps);
			
			pNonVisualDrawingProps.id = pDrawingProperty->m_nDocPr;
			pDrawingProperty->sDocPr = pNonVisualDrawingProps.toXML2(L"wp:docPr");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadEmbedded(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		
		CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
		
		if ( c_oSerEmbedded::Type == type )
		{
			pDrawingProperty->nObjectType = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerEmbedded::Program == type )
		{
			pDrawingProperty->sObjectProgram = m_oBufferedStream.GetString2();
		}
		else if ( c_oSerEmbedded::Data == type )
		{
			pDrawingProperty->bObject = true;
			long pos  = m_oBufferedStream.GetPos();
			
			if (pDrawingProperty->nObjectType == 1)
			{
			}
			else if (pDrawingProperty->nObjectType == 2)
			{

				BinXlsxRW::CXlsxSerializer oXlsxSerializer;
				oXlsxSerializer.setDrawingConverter(m_oFileWriter.m_pDrawingConverter);

				std::wstring strDstEmbedded = m_oBufferedStream.m_pRels->m_pManager->GetDstMedia();
				int nPos = (int)strDstEmbedded.rfind(wchar_t('m'));
				if (-1 != nPos)
					strDstEmbedded = strDstEmbedded.substr(0, nPos);

				strDstEmbedded += L"embeddings";
				NSDirectory::CreateDirectory(strDstEmbedded);

				std::wstring strDstEmbeddedTemp = strDstEmbedded + FILE_SEPARATOR_STR + L"Temp";
				NSDirectory::CreateDirectory(strDstEmbeddedTemp);

				int id = m_oFileWriter.m_oChartWriter.nEmbeddedCount++;

				std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring( id + 1) + L".xlsx";
				BinXlsxRW::SaveParams oSaveParams(m_oFileWriter.m_sThemePath, m_oFileWriter.m_pDrawingConverter->GetContentTypes());//???
				
				OOX::Spreadsheet::CXlsx oXlsx;

				BinXlsxRW::BinaryFileReader embeddedReader;				
				embeddedReader.ReadMainTable(oXlsx, m_oBufferedStream, L"", strDstEmbeddedTemp, oSaveParams, m_oFileWriter.m_pDrawingConverter);

				oXlsx.PrepareToWrite();

				oXlsx.Write(strDstEmbeddedTemp, *oSaveParams.pContentTypes);

				COfficeUtils oOfficeUtils(NULL);
				oOfficeUtils.CompressFileOrDirectory(strDstEmbeddedTemp, strDstEmbedded + FILE_SEPARATOR_STR + sXlsxFilename, true);

				std::wstring sEmbWorksheetRelsName = L"embeddings/" + sXlsxFilename;
                std::wstring bstrEmbWorksheetRelType = OOX::FileTypes::MicrosoftOfficeExcelWorksheet.RelationType();
                m_oFileWriter.m_pDrawingConverter->WriteRels(bstrEmbWorksheetRelType, sEmbWorksheetRelsName, std::wstring(), &pDrawingProperty->nObjectId);

				NSDirectory::DeleteDirectory(strDstEmbeddedTemp);
			}
			m_oBufferedStream.Seek( pos + length); 
		}		
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadCNvGraphicFramePr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		
		PPTX::Logic::CNvGraphicFramePr * pLocking = static_cast<PPTX::Logic::CNvGraphicFramePr*>(poResult);
		
		if ( c_oSerGraphicFramePr::NoChangeAspect == type )
		{
			pLocking->noChangeAspect = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerGraphicFramePr::NoDrilldown == type )
		{
			pLocking->noDrilldown = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerGraphicFramePr::NoGrp == type )
		{
			pLocking->noGrp = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerGraphicFramePr::NoMove == type )
		{
			pLocking->noMove = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerGraphicFramePr::NoResize == type )
		{
			pLocking->noResize = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerGraphicFramePr::NoSelect == type )
		{
			pLocking->noSelect = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadDocPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		
		PPTX::Logic::CNvPr* pNonVisualDrawingProps = static_cast<PPTX::Logic::CNvPr*>(poResult);
		
		if ( c_oSerDocPr::Id == type )
		{
			pNonVisualDrawingProps->id = m_oBufferedStream.GetLong();
		}
		else if ( c_oSerDocPr::Name == type )
		{
            pNonVisualDrawingProps->name = m_oBufferedStream.GetString3(length);
		}
		else if ( c_oSerDocPr::Hidden == type )
		{
			pNonVisualDrawingProps->hidden = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerDocPr::Title == type )
		{
            pNonVisualDrawingProps->title = m_oBufferedStream.GetString3(length);
		}
		else if ( c_oSerDocPr::Descr == type )
		{
            pNonVisualDrawingProps->descr = m_oBufferedStream.GetString3(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadEffectExtent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
		if ( c_oSerEffectExtent::Left == type )
		{
			pDrawingProperty->bEffectExtentL = true;
			pDrawingProperty->EffectExtentL = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerEffectExtent::Top == type )
		{
			pDrawingProperty->bEffectExtentT = true;
			pDrawingProperty->EffectExtentT = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerEffectExtent::Right == type )
		{
			pDrawingProperty->bEffectExtentR = true;
			pDrawingProperty->EffectExtentR = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerEffectExtent::Bottom == type )
		{
			pDrawingProperty->bEffectExtentB = true;
			pDrawingProperty->EffectExtentB = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadExtent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
		if ( c_oSerExtent::Cx == type )
		{
			pDrawingProperty->bWidth = true;
			pDrawingProperty->Width = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerExtent::Cy == type )
		{
			pDrawingProperty->bHeight = true;
			pDrawingProperty->Height = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadPositionH(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
		if ( c_oSerPosHV::RelativeFrom == type )
		{
			pDrawingProperty->bPositionHRelativeFrom = true;
			pDrawingProperty->PositionHRelativeFrom = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerPosHV::Align == type )
		{
			pDrawingProperty->bPositionHAlign = true;
			pDrawingProperty->PositionHAlign = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerPosHV::PosOffset == type )
		{
			pDrawingProperty->bPositionHPosOffset = true;
			pDrawingProperty->PositionHPosOffset = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerPosHV::PctOffset == type )
		{
			pDrawingProperty->bPositionHPctOffset = true;
			pDrawingProperty->PositionHPctOffset = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadPositionV(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
		if ( c_oSerPosHV::RelativeFrom == type )
		{
			pDrawingProperty->bPositionVRelativeFrom = true;
			pDrawingProperty->PositionVRelativeFrom = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerPosHV::Align == type )
		{
			pDrawingProperty->bPositionVAlign = true;
			pDrawingProperty->PositionVAlign = m_oBufferedStream.GetUChar();
		}
		else if ( c_oSerPosHV::PosOffset == type )
		{
			pDrawingProperty->bPositionVPosOffset = true;
			pDrawingProperty->PositionVPosOffset = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerPosHV::PctOffset == type )
		{
			pDrawingProperty->bPositionVPctOffset = true;
			pDrawingProperty->PositionVPctOffset = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSimplePos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
		if ( c_oSerSimplePos::X == type )
		{
			pDrawingProperty->bSimplePosX = true;
			pDrawingProperty->SimplePosX = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerSimplePos::Y == type )
		{
			pDrawingProperty->bSimplePosY = true;
			pDrawingProperty->SimplePosY = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSizeRelH(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Drawing::CSizeRelH* pSizeRelH = static_cast<OOX::Drawing::CSizeRelH*>(poResult);
		if ( c_oSerSizeRelHV::RelativeFrom == type )
		{
			pSizeRelH->m_oRelativeFrom.Init();
			pSizeRelH->m_oRelativeFrom->SetValue((SimpleTypes::ESizeRelFromH)m_oBufferedStream.GetUChar());
		}
		else if ( c_oSerSizeRelHV::Pct == type )
		{
			pSizeRelH->m_oPctWidth.Init();
			pSizeRelH->m_oPctWidth->SetValue(m_oBufferedStream.GetDouble());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSizeRelV(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Drawing::CSizeRelV* pSizeRelV = static_cast<OOX::Drawing::CSizeRelV*>(poResult);
		if ( c_oSerSizeRelHV::RelativeFrom == type )
		{
			pSizeRelV->m_oRelativeFrom.Init();
			pSizeRelV->m_oRelativeFrom->SetValue((SimpleTypes::ESizeRelFromV)m_oBufferedStream.GetUChar());
		}
		else if ( c_oSerSizeRelHV::Pct == type )
		{
			pSizeRelV->m_oPctHeight.Init();
			pSizeRelV->m_oPctHeight->SetValue(m_oBufferedStream.GetDouble());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadWrapThroughTight(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerWrapThroughTight::WrapPolygon == type )
		{
			READ2_DEF(length, res, this->ReadWrapPolygon, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadWrapPolygon(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingPropertyWrap* pDrawingPropertyWrap = static_cast<CDrawingPropertyWrap*>(poResult);
		if ( c_oSerWrapPolygon::Edited == type )
		{
			pDrawingPropertyWrap->bEdited = true;
			pDrawingPropertyWrap->Edited = m_oBufferedStream.GetBool();
		}
		else if ( c_oSerWrapPolygon::Start == type )
		{
			pDrawingPropertyWrap->bStart = true;
			READ2_DEF(length, res, this->ReadPolygonPoint, &pDrawingPropertyWrap->Start);
		}
		else if ( c_oSerWrapPolygon::ALineTo == type )
		{
			READ2_DEF(length, res, this->ReadLineTo, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadLineTo(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingPropertyWrap* pDrawingPropertyWrap = static_cast<CDrawingPropertyWrap*>(poResult);
		if ( c_oSerWrapPolygon::LineTo == type )
		{
			CDrawingPropertyWrapPoint* pWrapPoint = new CDrawingPropertyWrapPoint();
			READ2_DEF(length, res, this->ReadPolygonPoint, pWrapPoint);
			pDrawingPropertyWrap->Points.push_back(pWrapPoint);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadPolygonPoint(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDrawingPropertyWrapPoint* pWrapPoint = static_cast<CDrawingPropertyWrapPoint*>(poResult);
		if ( c_oSerPoint2D::X == type )
		{
			pWrapPoint->bX = true;
			pWrapPoint->X = m_oBufferedStream.GetDouble();
		}
		else if ( c_oSerPoint2D::Y == type )
		{
			pWrapPoint->bY = true;
			pWrapPoint->Y = m_oBufferedStream.GetDouble();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadEmptyWrap(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int Read_SecPr(BYTE type, long length, void* poResult)
	{
		return oBinary_pPrReader.Read_SecPr(type, length, poResult);
	}
	int ReadSdt(BYTE type, long length, void* poResult)
	{
		int res = 0;
		SdtWraper* pSdt = static_cast<SdtWraper*>(poResult);
		if (c_oSerSdt::Pr == type)
		{
			pSdt->m_oSdt.m_oSdtPr.Init();
			READ1_DEF(length, res, this->ReadSdtPr, pSdt);
		}
		else if (c_oSerSdt::EndPr == type)
		{
			pSdt->m_oEndPr = new rPr(m_oFontTableWriter.m_mapFonts);
			res = oBinary_rPrReader.Read(length, pSdt->m_oEndPr);
		}
		else if (c_oSerSdt::Content == type)
		{
			m_oDocumentWriter.m_oContent.WriteString(L"<w:sdt>");
			if ( pSdt->m_oSdt.m_oSdtPr.IsInit() )
			{
				m_oDocumentWriter.m_oContent.WriteString(pSdt->m_oSdt.m_oSdtPr->toXMLStart());
				if (NULL != pSdt->m_oRPr)
				{
					pSdt->m_oRPr->Write(&m_oDocumentWriter.m_oContent);
				}
				m_oDocumentWriter.m_oContent.WriteString(pSdt->m_oSdt.m_oSdtPr->toXMLEnd());
			}
			if (NULL != pSdt->m_oEndPr)
			{
				m_oDocumentWriter.m_oContent.WriteString(L"<w:sdtEndPr>");
				pSdt->m_oEndPr->Write(&m_oDocumentWriter.m_oContent);
				m_oDocumentWriter.m_oContent.WriteString(L"</w:sdtEndPr>");
			}

			m_oDocumentWriter.m_oContent.WriteString(L"<w:sdtContent>");
			switch(pSdt->m_nType)
			{
			case 0:
				{
				READ1_DEF(length, res, this->ReadDocumentContent, this);
				}
				break;
			case 1:
				{
				READ1_DEF(length, res, this->ReadParagraphContent, this);
				}
				break;
			case 2:
				{
				READ1_DEF(length, res, this->Read_TableContent, &m_oDocumentWriter.m_oContent);
				}
				break;
			case 3:
				{
				READ1_DEF(length, res, this->ReadRowContent, &m_oDocumentWriter.m_oContent);
				}
				break;
			}
			m_oDocumentWriter.m_oContent.WriteString(L"</w:sdtContent>");
			m_oDocumentWriter.m_oContent.WriteString(L"</w:sdt>");
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSdtPr(BYTE type, long length, void* poResult)
	{
		int res = 0;
		SdtWraper* pSdtWraper = static_cast<SdtWraper*>(poResult);
		OOX::Logic::CSdtPr* pSdtPr = pSdtWraper->m_oSdt.m_oSdtPr.GetPointer();
		if (c_oSerSdt::Type == type)
		{
			pSdtPr->m_eType = (OOX::Logic::ESdtType)m_oBufferedStream.GetUChar();
		}
		else if (c_oSerSdt::Alias == type)
		{
			pSdtPr->m_oAlias.Init();
			pSdtPr->m_oAlias->m_sVal.Init();
			pSdtPr->m_oAlias->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::ComboBox == type)
		{
			pSdtPr->m_oComboBox.Init();
			READ1_DEF(length, res, this->ReadSdtComboBox, pSdtPr->m_oComboBox.GetPointer());
		}
		else if (c_oSerSdt::DataBinding == type)
		{
			pSdtPr->m_oDataBinding.Init();
			READ1_DEF(length, res, this->ReadSdtPrDataBinding, pSdtPr->m_oDataBinding.GetPointer());
		}
		else if (c_oSerSdt::PrDate == type)
		{
			pSdtPr->m_oDate.Init();
			READ1_DEF(length, res, this->ReadSdtPrDate, pSdtPr->m_oDate.GetPointer());
		}
		else if (c_oSerSdt::DocPartList == type)
		{
			pSdtPr->m_oDocPartList.Init();
			READ1_DEF(length, res, this->ReadDocPartList, pSdtPr->m_oDocPartList.GetPointer());
		}
		else if (c_oSerSdt::DocPartObj == type)
		{
			pSdtPr->m_oDocPartObj.Init();
			READ1_DEF(length, res, this->ReadDocPartList, pSdtPr->m_oDocPartObj.GetPointer());
		}
		else if (c_oSerSdt::DropDownList == type)
		{
			pSdtPr->m_oDropDownList.Init();
			READ1_DEF(length, res, this->ReadDropDownList, pSdtPr->m_oDropDownList.GetPointer());
		}
		else if (c_oSerSdt::Id == type)
		{
			pSdtPr->m_oId.Init();
			pSdtPr->m_oId->m_oVal.Init();
			pSdtPr->m_oId->m_oVal->SetValue(m_oBufferedStream.GetULong());
		}
		else if (c_oSerSdt::Label == type)
		{
			pSdtPr->m_oLabel.Init();
			pSdtPr->m_oLabel->m_oVal.Init();
			pSdtPr->m_oLabel->m_oVal->SetValue(m_oBufferedStream.GetULong());
		}
		else if (c_oSerSdt::Lock == type)
		{
			pSdtPr->m_oLock.Init();
			pSdtPr->m_oLock->m_oVal.SetValue((SimpleTypes::ELock)m_oBufferedStream.GetUChar());
		}
		else if (c_oSerSdt::PlaceHolder == type)
		{
			pSdtPr->m_oPlaceHolder.Init();
			pSdtPr->m_oPlaceHolder->m_oDocPart.Init();
			pSdtPr->m_oPlaceHolder->m_oDocPart->m_sVal.Init();
			pSdtPr->m_oPlaceHolder->m_oDocPart->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::RPr == type)
		{
			pSdtWraper->m_oRPr = new rPr(m_oFontTableWriter.m_mapFonts);
			res = oBinary_rPrReader.Read(length, pSdtWraper->m_oRPr);
		}
		else if (c_oSerSdt::ShowingPlcHdr == type)
		{
			pSdtPr->m_oShowingPlcHdr.Init();
			pSdtPr->m_oShowingPlcHdr->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else if (c_oSerSdt::TabIndex == type)
		{
			pSdtPr->m_oTabIndex.Init();
			pSdtPr->m_oTabIndex->m_oVal.Init();
			pSdtPr->m_oTabIndex->m_oVal->SetValue(m_oBufferedStream.GetULong());
		}
		else if (c_oSerSdt::Tag == type)
		{
			pSdtPr->m_oTag.Init();
			pSdtPr->m_oTag->m_sVal.Init();
			pSdtPr->m_oTag->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::Temporary == type)
		{
			pSdtPr->m_oTemporary.Init();
			pSdtPr->m_oTemporary->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else if (c_oSerSdt::MultiLine == type)
		{
			pSdtPr->m_oText.Init();
			pSdtPr->m_oText->m_oMultiLine.Init();
			pSdtPr->m_oText->m_oMultiLine->FromBool(m_oBufferedStream.GetBool());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSdtComboBox(BYTE type, long length, void* poResult)
	{
		int res = 0;
		OOX::Logic::CSdtComboBox* pSdtComboBox = static_cast<OOX::Logic::CSdtComboBox*>(poResult);
		if (c_oSerSdt::LastValue == type)
		{
			pSdtComboBox->m_sLastValue.Init();
			pSdtComboBox->m_sLastValue->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::SdtListItem == type)
		{
			ComplexTypes::Word::CSdtListItem* pSdtListItem = new ComplexTypes::Word::CSdtListItem();
			READ1_DEF(length, res, this->ReadSdtListItem, pSdtListItem);
			pSdtComboBox->m_arrListItem.push_back(pSdtListItem);		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSdtListItem(BYTE type, long length, void* poResult)
	{
		int res = 0;
		ComplexTypes::Word::CSdtListItem* pSdtListItem = static_cast<ComplexTypes::Word::CSdtListItem*>(poResult);
		if (c_oSerSdt::DisplayText == type)
		{
			pSdtListItem->m_sDisplayText.Init();
			pSdtListItem->m_sDisplayText->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::Value == type)
		{
			pSdtListItem->m_sValue.Init();
			pSdtListItem->m_sValue->append(m_oBufferedStream.GetString3(length));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSdtPrDataBinding(BYTE type, long length, void* poResult)
	{
		int res = 0;
		ComplexTypes::Word::CDataBinding* pDataBinding = static_cast<ComplexTypes::Word::CDataBinding*>(poResult);
		if (c_oSerSdt::PrefixMappings == type)
		{
			pDataBinding->m_sPrefixMappings.Init();
			pDataBinding->m_sPrefixMappings->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::StoreItemID == type)
		{
			pDataBinding->m_sStoreItemID.Init();
			pDataBinding->m_sStoreItemID->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::XPath == type)
		{
			pDataBinding->m_sXPath.Init();
			pDataBinding->m_sXPath->append(m_oBufferedStream.GetString3(length));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadSdtPrDate(BYTE type, long length, void* poResult)
	{
		int res = 0;
		OOX::Logic::CDate* pDate = static_cast<OOX::Logic::CDate*>(poResult);
		if (c_oSerSdt::FullDate == type)
		{
            std::wstring sVal = m_oBufferedStream.GetString3(length);

            pDate->m_oFullDate.Init();
            pDate->m_oFullDate->SetValue(sVal);
		}
		else if (c_oSerSdt::Calendar == type)
		{
			pDate->m_oCalendar.Init();
			pDate->m_oCalendar->m_oVal.SetValue((SimpleTypes::ECalendarType)m_oBufferedStream.GetUChar());
		}
		else if (c_oSerSdt::DateFormat == type)
		{
			pDate->m_oDateFormat.Init();
			pDate->m_oDateFormat->m_sVal.Init();
			pDate->m_oDateFormat->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::Lid == type)
		{
			pDate->m_oLid.Init();
			pDate->m_oLid->m_oVal.Init();
            std::wstring sVal = m_oBufferedStream.GetString3(length);
            pDate->m_oLid->m_oVal->SetValue(sVal);
		}
		else if (c_oSerSdt::StoreMappedDataAs == type)
		{
			pDate->m_oStoreMappedDateAs.Init();
			pDate->m_oStoreMappedDateAs->m_oVal.SetValue((SimpleTypes::ESdtDateMappingType)m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadDocPartList(BYTE type, long length, void* poResult)
	{
		int res = 0;
		OOX::Logic::CSdtDocPart* pDocPart = static_cast<OOX::Logic::CSdtDocPart*>(poResult);
		if (c_oSerSdt::DocPartCategory == type)
		{
			pDocPart->m_oDocPartCategory.Init();
			pDocPart->m_oDocPartCategory->m_sVal.Init();
			pDocPart->m_oDocPartCategory->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::DocPartGallery == type)
		{
			pDocPart->m_oDocPartGallery.Init();
			pDocPart->m_oDocPartGallery->m_sVal.Init();
			pDocPart->m_oDocPartGallery->m_sVal->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::DocPartUnique == type)
		{
			pDocPart->m_oDocPartUnique.Init();
			pDocPart->m_oDocPartUnique->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadDropDownList(BYTE type, long length, void* poResult)
	{
		int res = 0;
		OOX::Logic::CSdtDropDownList* pDropDownList = static_cast<OOX::Logic::CSdtDropDownList*>(poResult);
		if (c_oSerSdt::LastValue == type)
		{
			pDropDownList->m_sLastValue.Init();
			pDropDownList->m_sLastValue->append(m_oBufferedStream.GetString3(length));
		}
		else if (c_oSerSdt::SdtListItem == type)
		{
			ComplexTypes::Word::CSdtListItem* pSdtListItem = new ComplexTypes::Word::CSdtListItem();
			READ1_DEF(length, res, this->ReadSdtListItem, pSdtListItem);
			pDropDownList->m_arrListItem.push_back(pSdtListItem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
};
class Binary_NotesTableReader : public Binary_CommonReader
{
	Writers::FileWriter&					m_oFileWriter;
	CComments*								m_pComments;
	bool									m_bIsFootnote;
	nullable<SimpleTypes::CDecimalNumber<>>	m_oId;
	nullable<SimpleTypes::CFtnEdn<>>		m_oType;
public:
	Binary_NotesTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, CComments* pComments, bool bIsFootnote):
		Binary_CommonReader(poBufferedStream),m_oFileWriter(oFileWriter),m_pComments(pComments),m_bIsFootnote(bIsFootnote)
	{
	}
	int Read()
	{
		m_oFileWriter.m_pDrawingConverter->SetDstContentRels();
        std::wstring sFilename;
		Writers::ContentWriter* pContentWriter = NULL;
		if(m_bIsFootnote)
		{
			sFilename = m_oFileWriter.m_oFootnotesWriter.getFilename();
			pContentWriter = &m_oFileWriter.m_oFootnotesWriter.m_oNotesWriter;
		}
		else
		{
			sFilename = m_oFileWriter.m_oEndnotesWriter.getFilename();
			pContentWriter = &m_oFileWriter.m_oEndnotesWriter.m_oNotesWriter;
		}
		Binary_DocumentTableReader oBinary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, *pContentWriter, m_pComments);

		int res = c_oSerConstants::ReadOk;
		READ_TABLE_DEF(res, this->ReadNotes, &oBinary_DocumentTableReader);

		OOX::CPath fileRelsPath = m_oFileWriter.m_oDocumentWriter.m_sDir +	FILE_SEPARATOR_STR + L"word" +
																			FILE_SEPARATOR_STR + L"_rels"+
																			FILE_SEPARATOR_STR + sFilename + L".rels";

		m_oFileWriter.m_pDrawingConverter->SaveDstContentRels(fileRelsPath.GetPath());
		return res;
	}
	int ReadNotes(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerNotes::Note == type )
		{
			m_oType.reset();
			m_oId.reset();
			READ1_DEF(length, res, this->ReadNote, poResult);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadNote(BYTE type, long length, void* poResult)
	{
		Binary_DocumentTableReader* pBinary_DocumentTableReader = static_cast<Binary_DocumentTableReader*>(poResult);
		int res = c_oSerConstants::ReadOk;
		if ( c_oSerNotes::NoteType == type )
		{
			m_oType.Init();
			m_oType->SetValue((SimpleTypes::EFtnEdn)m_oBufferedStream.GetUChar());
		}
		else if ( c_oSerNotes::NoteId == type )
		{
			m_oId.Init();
			m_oId->SetValue(m_oBufferedStream.GetLong());
		}
		else if ( c_oSerNotes::NoteContent == type )
		{
			XmlUtils::CStringWriter& writer = pBinary_DocumentTableReader->m_oDocumentWriter.m_oContent;
			if(m_bIsFootnote)
			{
				writer.WriteString(L"<w:footnote");
			}
			else
			{
				writer.WriteString(L"<w:endnote");
			}
			if(m_oType.IsInit())
			{
				writer.WriteString(L" w:type=\"");
				writer.WriteString(m_oType->ToString());
				writer.WriteString(L"\"");
			}
			if(m_oId.IsInit())
			{
				writer.WriteString(L" w:id=\"");
				writer.WriteString(m_oId->ToString());
				writer.WriteString(L"\"");
			}
			writer.WriteString(L">");
			READ1_DEF(length, res, this->ReadNoteContent, poResult);
			if(m_bIsFootnote)
			{
				writer.WriteString(L"</w:footnote>");
			}
			else
			{
				writer.WriteString(L"</w:endnote>");
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int ReadNoteContent(BYTE type, long length, void* poResult)
	{
		Binary_DocumentTableReader* pBinary_DocumentTableReader = static_cast<Binary_DocumentTableReader*>(poResult);
		return pBinary_DocumentTableReader->ReadDocumentContent(type, length, NULL);
	};
};
class BinaryFileReader
{
private:
	NSBinPptxRW::CBinaryFileReader& m_oBufferedStream;
	Writers::FileWriter&			m_oFileWriter;
    std::wstring					m_sFileInDir;
public: 
		BinaryFileReader(std::wstring& sFileInDir, NSBinPptxRW::CBinaryFileReader& oBufferedStream, Writers::FileWriter& oFileWriter) : 
			m_sFileInDir(sFileInDir), m_oBufferedStream(oBufferedStream), m_oFileWriter(oFileWriter)
		{
		}
		int ReadFile()
		{
			return ReadMainTable();
		}
		int ReadMainTable()
		{
			m_oBufferedStream.m_nDocumentType = XMLWRITER_DOC_TYPE_DOCX;

			long res = c_oSerConstants::ReadOk;

			res = m_oBufferedStream.Peek(1) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
			if(c_oSerConstants::ReadOk != res)
				return res;
			long nOtherOffset		= -1;
			long nStyleOffset		= -1;
			long nSettingsOffset	= -1;
			long nDocumentOffset	= -1;
			long nCommentsOffset	= -1;

			std::vector<BYTE> aTypes;
			std::vector<long> aOffBits;
			BYTE mtLen = m_oBufferedStream.GetUChar();
			
			for(int i = 0; i < mtLen; ++i)
			{
				//mtItem
				res = m_oBufferedStream.Peek(5) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
				if(c_oSerConstants::ReadOk != res)
					return res;
				BYTE mtiType = m_oBufferedStream.GetUChar();
				long mtiOffBits = m_oBufferedStream.GetLong();

				if(c_oSerTableTypes::Other == mtiType)
				{
					nOtherOffset = mtiOffBits;
				}
				else if(c_oSerTableTypes::Style == mtiType)
				{
					nStyleOffset = mtiOffBits;
				}
				else if(c_oSerTableTypes::Settings == mtiType)
				{
					nSettingsOffset = mtiOffBits;
				}
				else if(c_oSerTableTypes::Document == mtiType)
				{
					nDocumentOffset = mtiOffBits;
				}
				else if(c_oSerTableTypes::Comments == mtiType)
				{
					nCommentsOffset = mtiOffBits;
				}
				else
				{
					aTypes.push_back(mtiType);
					aOffBits.push_back(mtiOffBits);
				}
			}
			if(-1 != nOtherOffset)
			{
				int nOldPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Seek(nOtherOffset);
				res = Binary_OtherTableReader(m_sFileInDir, m_oBufferedStream, m_oFileWriter).Read();
				if(c_oSerConstants::ReadOk != res)
					return res;
			}
			if(-1 != nSettingsOffset)
			{
				int nOldPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Seek(nSettingsOffset);
				res = Binary_SettingsTableReader(m_oBufferedStream, m_oFileWriter).Read();
				if(c_oSerConstants::ReadOk != res)
					return res;
			}
			else
			{
				m_oFileWriter.m_oSettingWriter.AddSetting(L"<w:defaultTabStop w:val=\"708\"/>");
                std::wstring sClrMap(L"<w:clrSchemeMapping w:bg1=\"light1\" w:t1=\"dark1\" w:bg2=\"light2\" w:t2=\"dark2\" w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:hyperlink=\"hyperlink\" w:followedHyperlink=\"followedHyperlink\"/>");
				m_oFileWriter.m_oSettingWriter.AddSetting(sClrMap);
				m_oFileWriter.m_pDrawingConverter->LoadClrMap(sClrMap);
			}
			BinaryStyleTableReader oBinaryStyleTableReader(m_oBufferedStream, m_oFileWriter);
			if(-1 != nStyleOffset)
			{
				int nOldPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Seek(nStyleOffset);
				res = oBinaryStyleTableReader.Read();
				if(c_oSerConstants::ReadOk != res)
					return res;
			}
			Binary_CommentsTableReader oBinary_CommentsTableReader(m_oBufferedStream, m_oFileWriter);
			if(-1 != nCommentsOffset)
			{
				int nOldPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Seek(nCommentsOffset);
				res = oBinary_CommentsTableReader.Read();
				m_oFileWriter.m_pComments = &oBinary_CommentsTableReader.m_oComments;
				if(c_oSerConstants::ReadOk != res)
					return res;
			}
			
			for(size_t i = 0; i < aTypes.size(); ++i)
			{
				BYTE mtiType = aTypes[i];
				long mtiOffBits = aOffBits[i];

				m_oBufferedStream.Seek(mtiOffBits);
				switch(mtiType)
				{
					//case c_oSerTableTypes::Signature:break;
					//case c_oSerTableTypes::Info:break;
					//case c_oSerTableTypes::Style:
					//	res = BinaryStyleTableReader(m_oBufferedStream, m_oFileWriter).Read();
					//	break;
					//case c_oSerTableTypes::Document:
					//	res = Binary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, m_oFileWriter.m_oDocumentWriter).Read();
					//	break;
					case c_oSerTableTypes::HdrFtr:
						res = Binary_HdrFtrTableReader(m_oBufferedStream, m_oFileWriter, m_oFileWriter.m_pComments).Read();
						break;
					case c_oSerTableTypes::Numbering:
						res = Binary_NumberingTableReader(m_oBufferedStream, m_oFileWriter).Read();
						break;
					case c_oSerTableTypes::Footnotes:
						res = Binary_NotesTableReader(m_oBufferedStream, m_oFileWriter, m_oFileWriter.m_pComments, true).Read();
						break;
					case c_oSerTableTypes::Endnotes:
						res = Binary_NotesTableReader(m_oBufferedStream, m_oFileWriter, m_oFileWriter.m_pComments, false).Read();
						break;
					case c_oSerTableTypes::VbaProject:
						res = Binary_VbaProjectTableReader(m_oBufferedStream, m_oFileWriter).Read();
						break;

					//Comments должны читаться раньше чем c_oSerTableTypes::Document
					//case c_oSerTableTypes::Comments
					//	res = oBinary_CommentsTableReader.Read();
					//	break;
					//case c_oSerTableTypes::Other:
					//	res = Binary_OtherTableReader(m_sFileInDir, m_oBufferedStream, m_oFileWriter).Read();
					//	break;
				}
				if(c_oSerConstants::ReadOk != res)
					return res;
			}
			if(-1 != nDocumentOffset)
			{
				m_oBufferedStream.Seek(nDocumentOffset);

				m_oFileWriter.m_pDrawingConverter->SetDstContentRels();
				long stamdartRId;
               
				m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles",		L"styles.xml",		L"", &stamdartRId);
                m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings",	L"settings.xml",	L"", &stamdartRId);
                m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings",L"webSettings.xml", L"", &stamdartRId);
                m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable",	L"fontTable.xml",	L"", &stamdartRId);
                m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme",		L"theme/theme1.xml",L"", &stamdartRId);
               
				if (m_oFileWriter.m_pVbaProject.IsInit())
				{
					//m_oFileWriter.m_pVbaProject->write();
					m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2006/relationships/vbaProject",		L"vbaProject.bin", L"", &stamdartRId);
					
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.ms-word.document.macroEnabled.main+xml",	L"/word",		L"document.xml");
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.ms-office.vbaProject",					L"/word",		L"vbaProject.bin");
				}
				else
				{
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml",	L"/word",		L"document.xml");
				}
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",			L"/word",		L"styles.xml");
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",			L"/word",		L"settings.xml");
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",		L"/word",		L"webSettings.xml");
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",		L"/word",		L"fontTable.xml");
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.theme+xml",								L"/word/theme", L"theme1.xml");

				if(false == m_oFileWriter.m_oNumberingWriter.IsEmpty())
				{
					long rId;
                    m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering", L"numbering.xml", std::wstring(), &rId);
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml", L"/word", L"numbering.xml");
				}
                if(false == m_oFileWriter.m_oFootnotesWriter.IsEmpty())
				{
					long rId;
                    m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes", L"footnotes.xml", std::wstring(), &rId);
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml", L"/word", L"footnotes.xml");
				}
                if(false == m_oFileWriter.m_oEndnotesWriter.IsEmpty())
				{
					long rId;
                    m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes", L"endnotes.xml", std::wstring(), &rId);
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml", L"/word", L"endnotes.xml");
				}
				for(size_t i = 0; i < m_oFileWriter.m_oHeaderFooterWriter.m_aHeaders.size(); ++i)
				{
					Writers::HdrFtrItem* pHeader = m_oFileWriter.m_oHeaderFooterWriter.m_aHeaders[i];
					if(false == pHeader->IsEmpty())
					{
						long rId;
                        m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header", pHeader->m_sFilename, std::wstring(), &rId);
                        pHeader->rId = L"rId" + std::to_wstring( rId );
						
						m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml", L"/word", pHeader->m_sFilename);
					}
				}
				for(size_t i = 0; i < m_oFileWriter.m_oHeaderFooterWriter.m_aFooters.size(); ++i)
				{
					Writers::HdrFtrItem* pFooter = m_oFileWriter.m_oHeaderFooterWriter.m_aFooters[i];
					if(false == pFooter->IsEmpty())
					{
						long rId;
                        m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer", pFooter->m_sFilename, std::wstring(), &rId);
                        pFooter->rId = L"rId" + std::to_wstring( rId );
						
						m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml", L"/word", pFooter->m_sFilename);
					}
				}
				res = Binary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, m_oFileWriter.m_oDocumentWriter, &oBinary_CommentsTableReader.m_oComments).Read();

                OOX::CPath fileRelsPath = m_oFileWriter.m_oDocumentWriter.m_sDir	+ FILE_SEPARATOR_STR + L"word"
																					+ FILE_SEPARATOR_STR + L"_rels"
																					+ FILE_SEPARATOR_STR + L"document.xml.rels";

                CComments& oComments= oBinary_CommentsTableReader.m_oComments;
				Writers::CommentsWriter& oCommentsWriter = m_oFileWriter.m_oCommentsWriter;
                
				std::wstring sContent	= oComments.writeContent();
                std::wstring sContentEx = oComments.writeContentExt();	//важно чтобы writeContentExt вызывался после writeContent
                std::wstring sPeople	= oComments.writePeople();

				oCommentsWriter.setElements(sContent, sContentEx, sPeople);
                
				if(false == oCommentsWriter.m_sComment.empty())
				{
					long rId;
                    m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments", L"comments.xml", std::wstring(), &rId);
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml", L"/word", L"comments.xml");
				}
                if(false == oCommentsWriter.m_sCommentExt.empty())
				{
					long rId;
                    m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2011/relationships/commentsExtended", L"commentsExtended.xml", std::wstring(), &rId);
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtended+xml", L"/word", L"commentsExtended.xml");
				}
                if(false == oCommentsWriter.m_sPeople.empty())
				{
					long rId;
                    m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2011/relationships/people", L"people.xml", std::wstring(), &rId);
					m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.people+xml", L"/word", L"people.xml");
				}

                m_oFileWriter.m_pDrawingConverter->SaveDstContentRels(fileRelsPath.GetPath());
				if(c_oSerConstants::ReadOk != res)
					return res;
			}
			return res;
		}
};
}
#endif	// #ifndef READERS
