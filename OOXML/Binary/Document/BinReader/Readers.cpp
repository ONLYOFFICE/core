/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "Readers.h"
#include "ReaderClasses.h"

#include "../BinWriter/BinReaderWriterDefines.h"
#include "../../Sheets/Writer/BinaryReader.h"

#include "../../../PPTXFormat/App.h"
#include "../../../PPTXFormat/Core.h"
#include "../../../PPTXFormat/Logic/HeadingVariant.h"

#include "../../../DocxFormat/Docx.h"
#include "../../../DocxFormat/Document.h"
#include "../../../DocxFormat/FontTable.h"
#include "../../../DocxFormat/Numbering.h"
#include "../../../DocxFormat/Comments.h"
#include "../../../DocxFormat/Styles.h"
#include "../../../DocxFormat/Footnote.h"
#include "../../../DocxFormat/Endnote.h"
#include "../../../DocxFormat/Settings/Settings.h"
#include "../../../DocxFormat/App.h"
#include "../../../DocxFormat/Core.h"
#include "../../../DocxFormat/CustomXml.h"
#include "../../../DocxFormat/Math/oMathContent.h"

#include "../DocWrapper/XlsxSerializer.h"

#include "../../../../DesktopEditor/common/ASCVariant.h"
#include "../../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../DesktopEditor/raster/ImageFileFormatChecker.h"

#define UINT_TO_COMPLEX_BOOL(offset, val) \
	if (0 != ((nFlags >> offset) & 1)) { \
		val.Init(); \
	}

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
	nullable<OOX::Logic::CRunProperty> m_oEndPr;
	nullable<OOX::Logic::CRunProperty> m_oRPr;
	int m_nType;
	
	SdtWraper(int nType) : m_nType(nType) {}
	~SdtWraper() {}
};
#define READ1_DEF(stLen, res, fReadFunction, arg) {\
	long read1defCurPos = 0;\
		while(read1defCurPos < (long)stLen)\
	{\
		BYTE read1defType = 0;\
		if (false == m_oBufferedStream.GetUCharWithResult(&read1defType))\
			break;\
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
		while(read2defCurPos < (long)stLen)\
	{\
		BYTE read2defType = 0;\
		if (false == m_oBufferedStream.GetUCharWithResult(&read2defType))\
			break;\
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
		poResult->Id = new _INT32(m_oBufferedStream.GetLong());\
    }\
    else if(c_oSerProp_RevisionType::UserId == type)\
    {\
        poResult->UserId = m_oBufferedStream.GetString3(length);\
    }
#define READ1_TRACKREV_2(type, length, poResult) \
	if(c_oSerProp_RevisionType::Author == type)\
	{\
		poResult->m_sAuthor = m_oBufferedStream.GetString3(length);\
	}\
	else if(c_oSerProp_RevisionType::Date == type)\
	{\
		poResult->m_oDate.Init(); poResult->m_oDate->SetValue(m_oBufferedStream.GetString3(length));\
	}\
	else if(c_oSerProp_RevisionType::Id == type)\
	{\
		poResult->m_oId.Init(); poResult->m_oId->SetValue( m_oBufferedStream.GetLong());\
    }\
    else if(c_oSerProp_RevisionType::UserId == type)\
    {\
        poResult->m_sUserId = m_oBufferedStream.GetString3(length);\
    }

Binary_CommonReader::Binary_CommonReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream):m_oBufferedStream(poBufferedStream)
{
}
Binary_CommonReader2::Binary_CommonReader2(NSBinPptxRW::CBinaryFileReader& poBufferedStream):Binary_CommonReader(poBufferedStream)
{
}
docRGB Binary_CommonReader2::ReadColor()
{
	docRGB oRGB;
	oRGB.R = m_oBufferedStream.GetUChar();
	oRGB.G = m_oBufferedStream.GetUChar();
	oRGB.B = m_oBufferedStream.GetUChar();
	return oRGB;
}
void Binary_CommonReader2::ReadHexColor(SimpleTypes::CHexColor *pColor)
{
	if (!pColor)
	{
		m_oBufferedStream.Skip(3);
		return;
	}

	pColor->SetValue(SimpleTypes::hexcolorRGB);

	pColor->Set_R(m_oBufferedStream.GetUChar());
	pColor->Set_G(m_oBufferedStream.GetUChar());
	pColor->Set_B(m_oBufferedStream.GetUChar());
}
void Binary_CommonReader2::ReadThemeColor(int length, CThemeColor& oCThemeColor)
{
	int res = c_oSerConstants::ReadOk;
	READ2_DEF(length, res, this->ReadThemeColorContent, &oCThemeColor);
}
int Binary_CommonReader2::ReadThemeColorContent(BYTE type, long length, void* poResult)
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
template<typename T> int Binary_CommonReader2::ReadTrackRevision(long length, T* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadTrackRevisionInner, poResult);
	return res;
}
template<typename T> int Binary_CommonReader2::ReadTrackRevision2(long length, T* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadTrackRevisionInner2, poResult);
	return res;
}
int Binary_CommonReader2::ReadShdComplexType(BYTE type, long length, void* poResult)
{
	if (length < 0)
		return 0;

	int res = c_oSerConstants::ReadOk;
	ComplexTypes::Word::CShading* pShd = static_cast<ComplexTypes::Word::CShading*>(poResult);

	if (!pShd)
	{
		m_oBufferedStream.Skip(length);
		return 0;
	}
	switch (type)
	{
		case c_oSerShdType::Value:
		{
			pShd->m_oVal.Init();
			pShd->m_oVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerShdType::Color:
		{
			pShd->m_oColor.Init();
			ReadHexColor(pShd->m_oColor.GetPointer());
		}break;
		case c_oSerShdType::ColorTheme:
		{
			CThemeColor themeColor;
			ReadThemeColor(length, themeColor);
			themeColor.ToCThemeColor(pShd->m_oColor, pShd->m_oThemeColor, pShd->m_oThemeTint, pShd->m_oThemeShade);
		}break;
		case c_oSerShdType::Fill:
		{
			pShd->m_oFill.Init();
			ReadHexColor(pShd->m_oFill.GetPointer());
		}break;
		case c_oSerShdType::FillTheme:
		{
			CThemeColor themeColor;
			ReadThemeColor(length, themeColor);
			themeColor.ToCThemeColor(pShd->m_oFill, pShd->m_oThemeFill, pShd->m_oThemeFillTint, pShd->m_oThemeFillShade);
		}break;
		default:
		{
			res = c_oSerConstants::ReadUnknown;
		}break;
	}
	return res;
}
template<typename T> int Binary_CommonReader2::ReadTrackRevisionInner(BYTE type, long length, T* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ1_TRACKREV(type, length, poResult)
	else
	 res = c_oSerConstants::ReadUnknown;
	return res;
}
template<typename T> int Binary_CommonReader2::ReadTrackRevisionInner2(BYTE type, long length, T* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ1_TRACKREV_2(type, length, poResult)
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
Binary_HdrFtrTableReader::Binary_HdrFtrTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter)
	:
	Binary_CommonReader(poBufferedStream),
	m_oFileWriter(oFileWriter),
	m_oHeaderFooterWriter(oFileWriter.get_headers_footers_writer())
{

}
int Binary_HdrFtrTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadHdrFtrContent, NULL);
	return res;
}
int Binary_HdrFtrTableReader::ReadHdrFtrContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSerHdrFtrTypes::Header == type || c_oSerHdrFtrTypes::Footer == type )
	{
		nCurType = type;
		READ1_DEF(length, res, this->ReadHdrFtrFEO, poResult);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_HdrFtrTableReader::ReadHdrFtrFEO(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSerHdrFtrTypes::HdrFtr_First == type || c_oSerHdrFtrTypes::HdrFtr_Even == type || c_oSerHdrFtrTypes::HdrFtr_Odd == type )
	{
		nCurHeaderType = type;
		READ1_DEF(length, res, this->ReadHdrFtrItem, poResult);

		if (BinDocxRW::g_nCurFormatVersion < 5 && SimpleTypes::hdrftrEven == type)
		{
			m_oFileWriter.m_pCurrentSettings->m_oEvenAndOddHeaders.Init();
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int Binary_HdrFtrTableReader::ReadHdrFtrItem(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSerHdrFtrTypes::HdrFtr_Content == type )
	{
		Writers::HdrFtrItem* poHdrFtrItem = NULL;
		switch(nCurHeaderType)
		{
		case c_oSerHdrFtrTypes::HdrFtr_First:poHdrFtrItem = new Writers::HdrFtrItem(SimpleTypes::hdrftrFirst);break;
		case c_oSerHdrFtrTypes::HdrFtr_Even:poHdrFtrItem = new Writers::HdrFtrItem(SimpleTypes::hdrftrEven);break;
		case c_oSerHdrFtrTypes::HdrFtr_Odd:poHdrFtrItem = new Writers::HdrFtrItem(SimpleTypes::hdrftrDefault);break;
		}
		if(NULL != poHdrFtrItem)
		{
			if(nCurType == c_oSerHdrFtrTypes::Header)
			{
				m_oHeaderFooterWriter.m_aHeaders.push_back(poHdrFtrItem);
                poHdrFtrItem->m_sFilename = L"header" + std::to_wstring((int)m_oHeaderFooterWriter.m_aHeaders.size()) + L".xml";
			}
			else
			{
				m_oHeaderFooterWriter.m_aFooters.push_back(poHdrFtrItem);
                poHdrFtrItem->m_sFilename = L"footer" + std::to_wstring((int)m_oHeaderFooterWriter.m_aFooters.size()) + L".xml";
			}
			m_oFileWriter.m_pDrawingConverter->SetDstContentRels();
			
			Binary_DocumentTableReader oBinary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, poHdrFtrItem->Header, false);
			READ1_DEF(length, res, this->ReadHdrFtrItemContent, &oBinary_DocumentTableReader);

            OOX::CPath fileRelsPath = m_oFileWriter.get_document_writer().m_sDir +	FILE_SEPARATOR_STR + L"word" + 
												 (m_oFileWriter.m_bGlossaryMode ?	FILE_SEPARATOR_STR + std::wstring(L"glossary") : L"") +
																					FILE_SEPARATOR_STR + L"_rels"+ 
																					FILE_SEPARATOR_STR + poHdrFtrItem->m_sFilename + L".rels";

            m_oFileWriter.m_pDrawingConverter->SaveDstContentRels(fileRelsPath.GetPath());
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_HdrFtrTableReader::ReadHdrFtrItemContent(BYTE type, long length, void* poResult)
{
	Binary_DocumentTableReader* pBinary_DocumentTableReader = static_cast<Binary_DocumentTableReader*>(poResult);
	return pBinary_DocumentTableReader->ReadDocumentContent(type, length, NULL);
}


Binary_rPrReader::Binary_rPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter)
	: Binary_CommonReader(poBufferedStream), oBinary_CommonReader2(poBufferedStream), m_oFileWriter(oFileWriter)
	, m_mapFonts(oFileWriter.get_font_table_writer().m_mapFonts)
{
}
int Binary_rPrReader::Read(long stLen, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ2_DEF(stLen, res, this->ReadContent, poResult);
	return res;
};
int Binary_rPrReader::ReadContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CRunProperty* pRPr = static_cast<OOX::Logic::CRunProperty*>(poResult);
	
	switch(type)
	{
		case c_oSerProp_rPrType::Bold:
		{
			pRPr->m_oBold.Init();  pRPr->m_oBold->m_oVal.FromBool(0 != m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::Italic:
		{
			pRPr->m_oItalic.Init();  pRPr->m_oItalic->m_oVal.FromBool(0 != m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::Underline:
		{
			pRPr->m_oU.Init();  pRPr->m_oU->m_oVal.Init();  pRPr->m_oU->m_oVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::Strikeout:
		{
			pRPr->m_oStrike.Init(); pRPr->m_oStrike->m_oVal.FromBool(0 != m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::FontAscii:
		{
			std::wstring sFontName = XmlUtils::EncodeXmlString(m_oBufferedStream.GetString3(length));
            if(!sFontName.empty())
			{
				m_mapFonts[sFontName] = 1;
				if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
				pRPr->m_oRFonts->m_sAscii = sFontName;
			}
		}break;
		case c_oSerProp_rPrType::FontAsciiTheme:
		{
			if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
			pRPr->m_oRFonts->m_oAsciiTheme.Init();
			pRPr->m_oRFonts->m_oAsciiTheme->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::FontHAnsi:
		{
			std::wstring sFontName = XmlUtils::EncodeXmlString(m_oBufferedStream.GetString3(length));
			if (!sFontName.empty())
			{
				m_mapFonts[sFontName] = 1;
				if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
				pRPr->m_oRFonts->m_sHAnsi = sFontName;
			}
		}break;
		case c_oSerProp_rPrType::FontHAnsiTheme:
		{
			if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
			pRPr->m_oRFonts->m_oHAnsiTheme.Init();
			pRPr->m_oRFonts->m_oHAnsiTheme->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::FontCS:
		{
			std::wstring sFontName = XmlUtils::EncodeXmlString(m_oBufferedStream.GetString3(length));
			if (!sFontName.empty())
			{
				m_mapFonts[sFontName] = 1;
				if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
				pRPr->m_oRFonts->m_sCs = sFontName;
			}
		}break;
		case c_oSerProp_rPrType::FontCSTheme:
		{
			if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
			pRPr->m_oRFonts->m_oCsTheme.Init();
			pRPr->m_oRFonts->m_oCsTheme->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::FontAE:
		{
			std::wstring sFontName = XmlUtils::EncodeXmlString(m_oBufferedStream.GetString3(length));
			if (!sFontName.empty())
			{
				m_mapFonts[sFontName] = 1;
				if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
				pRPr->m_oRFonts->m_sEastAsia = sFontName;
			}
		}break;
		case c_oSerProp_rPrType::FontAETheme:
		{
			if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
			pRPr->m_oRFonts->m_oEastAsiaTheme.Init();
			pRPr->m_oRFonts->m_oEastAsiaTheme->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::FontSize:
		{
			pRPr->m_oSz.Init(); pRPr->m_oSz->m_oVal.Init();
			pRPr->m_oSz->m_oVal->FromHps(m_oBufferedStream.GetLong());
		}break;
		case c_oSerProp_rPrType::Color:
		{
			docRGB color = oBinary_CommonReader2.ReadColor();			
			pRPr->m_oColor.Init(); pRPr->m_oColor->m_oVal.Init(); 
			pRPr->m_oColor->m_oVal->SetValue(SimpleTypes::hexcolorRGB);
			pRPr->m_oColor->m_oVal->Set_R(color.R); pRPr->m_oColor->m_oVal->Set_G(color.G); pRPr->m_oColor->m_oVal->Set_B(color.B);
		}break;
		case c_oSerProp_rPrType::VertAlign:
		{
			pRPr->m_oVertAlign.Init();
			pRPr->m_oVertAlign->m_oVal.Init(); pRPr->m_oVertAlign->m_oVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::HighLight:
		{
			docRGB H = oBinary_CommonReader2.ReadColor();
			std::wstring sColor;
			if (0x00 == H.R && 0x00 == H.G && 0x00 == H.B)
				sColor = _T("black");
			else if (0x00 == H.R && 0x00 == H.G && 0xFF == H.B)
				sColor = _T("blue");
			else if (0x00 == H.R && 0xFF == H.G && 0xFF == H.B)
				sColor = _T("cyan");
			else if (0x00 == H.R && 0x00 == H.G && 0x8B == H.B)
				sColor = _T("darkBlue");
			else if (0x00 == H.R && 0x8B == H.G && 0x8B == H.B)
				sColor = _T("darkCyan");
			else if (0xA9 == H.R && 0xA9 == H.G && 0xA9 == H.B)
				sColor = _T("darkGray");
			else if (0x00 == H.R && 0x64 == H.G && 0x00 == H.B)
				sColor = _T("darkGreen");
			else if (0x80 == H.R && 0x00 == H.G && 0x80 == H.B)
				sColor = _T("darkMagenta");
			else if (0x8B == H.R && 0x00 == H.G && 0x00 == H.B)
				sColor = _T("darkRed");
			else if (0x80 == H.R && 0x80 == H.G && 0x00 == H.B)
				sColor = _T("darkYellow");
			else if (0x00 == H.R && 0xFF == H.G && 0x00 == H.B)
				sColor = _T("green");
			else if (0xD3 == H.R && 0xD3 == H.G && 0xD3 == H.B)
				sColor = _T("lightGray");
			else if (0xFF == H.R && 0x00 == H.G && 0xFF == H.B)
				sColor = _T("magenta");
			else if (0xFF == H.R && 0x00 == H.G && 0x00 == H.B)
				sColor = _T("red");
			else if (0xFF == H.R && 0xFF == H.G && 0xFF == H.B)
				sColor = _T("white");
			else if (0xFF == H.R && 0xFF == H.G && 0x00 == H.B)
				sColor = _T("yellow");

			pRPr->m_oHighlight.Init(); pRPr->m_oHighlight->m_oVal.Init();
			pRPr->m_oHighlight->m_oVal->FromString(sColor);
		}break;
		case c_oSerProp_rPrType::HighLightTyped:
		{
			BYTE byteHighLightTyped = m_oBufferedStream.GetUChar();
			
			pRPr->m_oHighlight.Init(); pRPr->m_oHighlight->m_oVal.Init();
			pRPr->m_oHighlight->m_oVal->SetValue(SimpleTypes::highlightcolorNone);
		}break;
		case c_oSerProp_rPrType::Shd:
		{
			pRPr->m_oShd.Init();
			READ2_DEF(length, res, oBinary_CommonReader2.ReadShdComplexType, pRPr->m_oShd.GetPointer());			
		}break;
		case c_oSerProp_rPrType::RStyle:
		{
			pRPr->m_oRStyle.Init();
			pRPr->m_oRStyle->m_sVal = m_oBufferedStream.GetString3(length);
		}break;
		case c_oSerProp_rPrType::Spacing:
		{
			pRPr->m_oSpacing.Init();
			pRPr->m_oSpacing->m_oVal.Init(); pRPr->m_oSpacing->m_oVal->FromTwips(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
		}break;
		case c_oSerProp_rPrType::SpacingTwips:
		{
			pRPr->m_oSpacing.Init();
			pRPr->m_oSpacing->m_oVal.Init(); pRPr->m_oSpacing->m_oVal->FromTwips(m_oBufferedStream.GetLong());
		}break;
		case c_oSerProp_rPrType::DStrikeout:
		{
			pRPr->m_oDStrike.Init();
			pRPr->m_oDStrike->m_oVal.SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::Caps:
		{
			pRPr->m_oCaps.Init();
			pRPr->m_oCaps->m_oVal.FromBool(0 != m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::SmallCaps:
		{
			pRPr->m_oSmallCaps.Init();
			pRPr->m_oSmallCaps->m_oVal.FromBool(0 != m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::Position:
		{
			pRPr->m_oPosition.Init();
			pRPr->m_oPosition->m_oVal.Init();
			pRPr->m_oPosition->m_oVal->FromMm(m_oBufferedStream.GetDouble());
		}break;
		case c_oSerProp_rPrType::PositionHps:
		{
			pRPr->m_oPosition.Init();
			pRPr->m_oPosition->m_oVal.Init();
			pRPr->m_oPosition->m_oVal->FromHps(m_oBufferedStream.GetLong());
		}break;
		case c_oSerProp_rPrType::FontHint:
		{
			if (!pRPr->m_oRFonts.IsInit()) pRPr->m_oRFonts.Init();
			pRPr->m_oRFonts->m_oHint.Init();
			pRPr->m_oRFonts->m_oHint->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::BoldCs:
		{
			pRPr->m_oBoldCs.Init();
			pRPr->m_oBoldCs->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}break;
		case c_oSerProp_rPrType::ItalicCs:
		{
			pRPr->m_oItalicCs.Init();
			pRPr->m_oItalicCs->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}break;
		case c_oSerProp_rPrType::FontSizeCs:
		{
			pRPr->m_oSzCs.Init(); pRPr->m_oSzCs->m_oVal.Init();
			pRPr->m_oSzCs->m_oVal->FromHps(m_oBufferedStream.GetLong());
		}break;
		case c_oSerProp_rPrType::Cs:
		{
			pRPr->m_oCs.Init(); 
			pRPr->m_oCs->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}break;
		case c_oSerProp_rPrType::Rtl:
		{
			pRPr->m_oRtL.Init();
			pRPr->m_oRtL->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}break;
		case c_oSerProp_rPrType::Lang:
		{
			if (!pRPr->m_oLang.IsInit()) pRPr->m_oLang.Init();
			pRPr->m_oLang->m_oVal = m_oBufferedStream.GetString3(length);
		}break;
		case c_oSerProp_rPrType::LangBidi:
		{
			if (!pRPr->m_oLang.IsInit()) pRPr->m_oLang.Init();
			pRPr->m_oLang->m_oBidi = m_oBufferedStream.GetString3(length);
		}break;
		case c_oSerProp_rPrType::LangEA:
		{
			if (!pRPr->m_oLang.IsInit()) pRPr->m_oLang.Init();
			pRPr->m_oLang->m_oEastAsia = m_oBufferedStream.GetString3(length);
		}break;		
		case c_oSerProp_rPrType::ColorTheme:
		{
			if (!pRPr->m_oColor.IsInit()) pRPr->m_oColor.Init();
			CThemeColor ThemeColor;
			oBinary_CommonReader2.ReadThemeColor(length, ThemeColor);
			ThemeColor.ToCThemeColor(pRPr->m_oColor->m_oVal, pRPr->m_oColor->m_oThemeColor, pRPr->m_oColor->m_oThemeTint, pRPr->m_oColor->m_oThemeShade);
		}break;
		case c_oSerProp_rPrType::Vanish:
		{
			pRPr->m_oVanish.Init();
			pRPr->m_oVanish->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}break;
		case c_oSerProp_rPrType::TextOutline:
		{
			if (length > 0)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Skip(1);//skip type
				pRPr->m_oTextOutline.Init();
				pRPr->m_oTextOutline->fromPPTY(&m_oBufferedStream);
				m_oBufferedStream.Seek(nCurPos + length);
			}
		}break;
		case c_oSerProp_rPrType::TextFill:
		{
			if (length > 0)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Skip(1);//skip type
				pRPr->m_oTextFill.fromPPTY(&m_oBufferedStream);
				m_oBufferedStream.Seek(nCurPos + length);
			}
		}break;
		case c_oSerProp_rPrType::Scene3d:
		{
			if (length > 0)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Skip(1);//skip type
				pRPr->m_oScene3d.Init();
				pRPr->m_oScene3d->fromPPTY(&m_oBufferedStream);
				m_oBufferedStream.Seek(nCurPos + length);
			}
		}break;
		case c_oSerProp_rPrType::Props3d:
		{
			if (length > 0)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Skip(1);//skip type
				pRPr->m_oProps3d.Init();
				pRPr->m_oProps3d->fromPPTY(&m_oBufferedStream);
				m_oBufferedStream.Seek(nCurPos + length);
			}
		}break;
		case c_oSerProp_rPrType::ShadowExt:
		{
			if (length > 0)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Skip(1);//skip type
				pRPr->m_oShadowExt.Init();
				pRPr->m_oShadowExt->fromPPTY(&m_oBufferedStream);
				m_oBufferedStream.Seek(nCurPos + length);
			}
		}break;
		case c_oSerProp_rPrType::Glow:
		{
			if (length > 0)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Skip(1);//skip type
				pRPr->m_oGlow.Init();
				pRPr->m_oGlow->fromPPTY(&m_oBufferedStream);
				m_oBufferedStream.Seek(nCurPos + length);
			}
		}break;
		case c_oSerProp_rPrType::Reflection:
		{
			if (length > 0)
			{
				long nCurPos = m_oBufferedStream.GetPos();
				m_oBufferedStream.Skip(1);//skip type
				pRPr->m_oReflection.Init();
				pRPr->m_oReflection->fromPPTY(&m_oBufferedStream);
				m_oBufferedStream.Seek(nCurPos + length);
			}
		}break;
		case c_oSerProp_rPrType::CntxtAlts:
		{
			pRPr->m_oCntxtAlts.Init();
			pRPr->m_oCntxtAlts->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}break;	
		case c_oSerProp_rPrType::NumForm:
		{
			pRPr->m_oNumForm.Init(); pRPr->m_oNumForm->m_oVal.Init();
			pRPr->m_oNumForm->m_oVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::NumSpacing:
		{
			pRPr->m_oNumSpacing.Init(); pRPr->m_oNumSpacing->m_oVal.Init();
			pRPr->m_oNumSpacing->m_oVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::Ligatures:
		{
			pRPr->m_oLigatures.Init(); pRPr->m_oLigatures->m_oVal.Init();
			pRPr->m_oLigatures->m_oVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}break;
		case c_oSerProp_rPrType::Del:
		{
			pRPr->m_oDel.Init();
			oBinary_CommonReader2.ReadTrackRevision2(length, pRPr->m_oDel.GetPointer());
		}break;
		case c_oSerProp_rPrType::Ins:
		{
			pRPr->m_oIns.Init();
			oBinary_CommonReader2.ReadTrackRevision2(length, pRPr->m_oIns.GetPointer());
		}break;
		case c_oSerProp_rPrType::MoveFrom:
		{
			pRPr->m_oMoveFrom.Init();
			oBinary_CommonReader2.ReadTrackRevision2(length, pRPr->m_oMoveFrom.GetPointer());
		}break;
		case c_oSerProp_rPrType::MoveTo:
		{
			pRPr->m_oMoveTo.Init();
			oBinary_CommonReader2.ReadTrackRevision2(length, pRPr->m_oMoveTo.GetPointer());
		}break;
		case c_oSerProp_rPrType::rPrChange:
		{
			pRPr->m_oRPrChange.Init();
			READ1_DEF(length, res, this->ReadrPrChange2, pRPr->m_oRPrChange.GetPointer());
		}break;		
		case c_oSerProp_rPrType::CompressText:
		{
			pRPr->m_oW.Init();
			pRPr->m_oW->m_oVal.Init(); pRPr->m_oW->m_oVal->SetValue(m_oBufferedStream.GetLong());
		}break;
		case c_oSerProp_rPrType::SnapToGrid:
		{
			pRPr->m_oSnapToGrid.Init();
			pRPr->m_oSnapToGrid->m_oVal.FromBool(m_oBufferedStream.GetBool());
		}break;	
	default:
		res = c_oSerConstants::ReadUnknown;
		break;
	}
	return res;
}

int Binary_rPrReader::ReadrPrChange2(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CRPrChange* pRPrChange = static_cast<OOX::Logic::CRPrChange*>(poResult);

	READ1_TRACKREV_2(type, length, pRPrChange)
	else if (c_oSerProp_RevisionType::rPrChange == type)
	{
		pRPrChange->m_pRunPr.Init(); 
		pRPrChange->m_pRunPr->m_bRPRChange = true;
		res = Read(length, pRPrChange->m_pRunPr.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
		return res;
};

Binary_pPrReader::Binary_pPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter)
	:
	m_oFontTableWriter(oFileWriter.get_font_table_writer()), 
	Binary_CommonReader(poBufferedStream), 
	oBinary_CommonReader2(poBufferedStream), 
	oBinary_rPrReader(poBufferedStream, oFileWriter),
	oBinary_HdrFtrTableReader(poBufferedStream, oFileWriter), 
	m_oFileWriter(oFileWriter)
{
	bDoNotWriteNullProp = false;
	m_nCurNumId = -1;
	m_nCurLvl = -1;
}
int Binary_pPrReader::Read(long stLen, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ2_DEF(stLen, res, this->ReadContent, poResult);
	return res;
};
int Binary_pPrReader::ReadContent( BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
	switch(type)
	{
	case c_oSerProp_pPrType::ContextualSpacing:
		{
			BYTE contextualSpacing = m_oBufferedStream.GetUChar();
			if(0 != contextualSpacing)
                pCStringWriter->WriteString(std::wstring(L"<w:contextualSpacing/>"));
			else if(false == bDoNotWriteNullProp)
                pCStringWriter->WriteString(std::wstring(L"<w:contextualSpacing w:val=\"0\"/>"));
        }break;
	case c_oSerProp_pPrType::Ind:
		{
			NSStringUtils::CStringBuilder oTempWriter;
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
                pCStringWriter->WriteString(std::wstring(L"<w:keepLines w:val=\"0\"/>"));
        }break;
	case c_oSerProp_pPrType::KeepNext:
		{
			BYTE KeepNext = m_oBufferedStream.GetUChar();
			if(0 != KeepNext)
                pCStringWriter->WriteString(std::wstring(L"<w:keepNext/>"));
			else if(false == bDoNotWriteNullProp)
                pCStringWriter->WriteString(std::wstring(L"<w:keepNext w:val=\"0\"/>"));
        }break;
	case c_oSerProp_pPrType::PageBreakBefore:
		{
			BYTE pageBreakBefore = m_oBufferedStream.GetUChar();
			if(0 != pageBreakBefore)
                pCStringWriter->WriteString(std::wstring(L"<w:pageBreakBefore/>"));
			else if(false == bDoNotWriteNullProp)
                pCStringWriter->WriteString(std::wstring(L"<w:pageBreakBefore w:val=\"0\"/>"));
			break;
		}
	case c_oSerProp_pPrType::Spacing:
		{
			Spacing oSpacing;
			READ2_DEF(length, res, this->ReadSpacing, &oSpacing);
			if(oSpacing.bLine || oSpacing.bLineTwips || oSpacing.bAfter || oSpacing.bAfterAuto || oSpacing.bBefore || oSpacing.bBeforeAuto)
			{
                pCStringWriter->WriteString(std::wstring(L"<w:spacing"));

				if (oSpacing.bBefore)
				{
					std::wstring sBefore = L" w:before=\"" + std::to_wstring(oSpacing.Before) + L"\"";
					pCStringWriter->WriteString(sBefore);
				}
				if (oSpacing.bBeforeAuto)
				{
					if (true == oSpacing.BeforeAuto)
						pCStringWriter->WriteString(std::wstring(L" w:beforeAutospacing=\"1\""));
					else
						pCStringWriter->WriteString(std::wstring(L" w:beforeAutospacing=\"0\""));
				}
				if(oSpacing.bAfter)
				{
					std::wstring sAfter = L" w:after=\"" + std::to_wstring(oSpacing.After) + L"\"";
					pCStringWriter->WriteString(sAfter);
				}
				if(oSpacing.bAfterAuto)
				{
					if(true == oSpacing.AfterAuto)
                        pCStringWriter->WriteString(std::wstring(L" w:afterAutospacing=\"1\""));
					else
                        pCStringWriter->WriteString(std::wstring(L" w:afterAutospacing=\"0\""));
				}
				BYTE bLineRule = linerule_Auto;
				
				std::wstring sLineRule;
				//проверяется bLine, а не bLineRule чтобы всегда писать LineRule, если есть w:line
				if (oSpacing.bLine || oSpacing.bLineTwips)
				{
					if (oSpacing.bLineRule)
						bLineRule = oSpacing.LineRule;
					switch (oSpacing.LineRule)
					{
					case linerule_AtLeast:	sLineRule = _T(" w:lineRule=\"atLeast\""); break;
					case linerule_Exact:	sLineRule = _T(" w:lineRule=\"exact\""); break;
					default:				sLineRule = _T(" w:lineRule=\"auto\""); break;
					}
				}
				if (oSpacing.bLine)
				{
					std::wstring sLine;
					if (linerule_Auto == bLineRule)
					{
						long nLine = SerializeCommon::Round(oSpacing.Line * 240);
						sLine = L" w:line=\"" + std::to_wstring(nLine) + L"\"";
					}
					else
					{
						long nLine = SerializeCommon::Round(g_dKoef_mm_to_twips * oSpacing.Line);
						sLine = L" w:line=\"" + std::to_wstring(nLine) + L"\"";
					}
					pCStringWriter->WriteString(sLine);
				}
				else if (oSpacing.bLineTwips)
				{
					pCStringWriter->WriteString(L" w:line=\"" + std::to_wstring(oSpacing.LineTwips) + L"\"");
				}
				if (false == sLineRule.empty())
					pCStringWriter->WriteString(sLineRule);

				pCStringWriter->WriteString(std::wstring(L"/>"));
			}
			break;
		}
	case c_oSerProp_pPrType::Shd:
		{
			ComplexTypes::Word::CShading oShd;
			READ2_DEF(length, res, oBinary_CommonReader2.ReadShdComplexType, &oShd);

			std::wstring sShd = oShd.ToString();
			if (false == sShd.empty())
				pCStringWriter->WriteString(L"<w:shd " + sShd + L"/>");
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
					pCStringWriter->WriteString(L"<w:tab w:val=\"" + oTab.Val.ToString() + L"\" w:pos=\"" + std::to_wstring(oTab.Pos) + L"\"");
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
			OOX::Logic::CRunProperty oRPr;
			res = oBinary_rPrReader.Read(length, &oRPr);
			if (oRPr.IsNoEmpty())
				pCStringWriter->WriteString(oRPr.toXML());
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
	case c_oSerProp_pPrType::SuppressLineNumbers:
		{
			if(m_oBufferedStream.GetBool())
				pCStringWriter->WriteString(std::wstring(L"<w:suppressLineNumbers/>"));
			else
				pCStringWriter->WriteString(std::wstring(L"<w:suppressLineNumbers w:val=\"0\"/>"));
		}break;
	default:
		res = c_oSerConstants::ReadUnknown;
		break;
	}
	return res;
};
int Binary_pPrReader::ReadPPrChange(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	TrackRevision* pPPrChange = static_cast<TrackRevision*>(poResult);
	READ1_TRACKREV(type, length, pPPrChange)
	else if(c_oSerProp_RevisionType::pPrChange == type)
	{
		pPPrChange->PPr = new NSStringUtils::CStringBuilder();
		res = Read(length, pPPrChange->PPr);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_pPrReader::ReadInd(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
	switch(type)
	{
	case c_oSerProp_pPrType::Ind_Left:
		{
			double dIndLeft = m_oBufferedStream.GetDouble();
			long nIndLeft = SerializeCommon::Round(dIndLeft * g_dKoef_mm_to_twips);

            pCStringWriter->WriteString(L" w:left=\"" + std::to_wstring(nIndLeft) + L"\"");
			break;
		}
	case c_oSerProp_pPrType::Ind_LeftTwips:
		{
			pCStringWriter->WriteString(L" w:left=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
			break;
		}
	case c_oSerProp_pPrType::Ind_Right:
		{
			double dIndRight = m_oBufferedStream.GetDouble();
			long nIndRight = SerializeCommon::Round(dIndRight * g_dKoef_mm_to_twips);

            pCStringWriter->WriteString(L" w:right=\"" + std::to_wstring(nIndRight) + L"\"");
			break;
		}
	case c_oSerProp_pPrType::Ind_RightTwips:
		{
			pCStringWriter->WriteString(L" w:right=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
			break;
		}
	case c_oSerProp_pPrType::Ind_FirstLine:
		{
			double dIndFirstLine = m_oBufferedStream.GetDouble();
			long nIndFirstLine = SerializeCommon::Round(dIndFirstLine * g_dKoef_mm_to_twips);
            std::wstring sIndFirstLine;
			if(nIndFirstLine >= 0)
                sIndFirstLine = L" w:firstLine=\"" + std::to_wstring(nIndFirstLine) + L"\"";
			else
                sIndFirstLine = L" w:hanging=\"" + std::to_wstring(-nIndFirstLine) + L"\"";
			pCStringWriter->WriteString(sIndFirstLine);
			break;
		}
	case c_oSerProp_pPrType::Ind_FirstLineTwips:
		{
			long nIndFirstLine = m_oBufferedStream.GetLong();
			std::wstring sIndFirstLine;
			if(nIndFirstLine >= 0)
				sIndFirstLine = L" w:firstLine=\"" + std::to_wstring(nIndFirstLine) + L"\"";
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
int Binary_pPrReader::ReadSpacing(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	Spacing* pSpacing = static_cast<Spacing*>(poResult);
	switch(type)
	{
	case c_oSerProp_pPrType::Spacing_Line:
		pSpacing->bLine = true;
		pSpacing->Line = m_oBufferedStream.GetDouble();
		break;
	case c_oSerProp_pPrType::Spacing_LineTwips:
		pSpacing->bLineTwips = true;
		pSpacing->LineTwips = m_oBufferedStream.GetLong();
		break;
	case c_oSerProp_pPrType::Spacing_LineRule:
		pSpacing->bLineRule = true;
		pSpacing->LineRule = m_oBufferedStream.GetUChar();
		break;
	case c_oSerProp_pPrType::Spacing_Before:
		pSpacing->bBefore = true;
		pSpacing->Before = SerializeCommon::Round( g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
		break;
	case c_oSerProp_pPrType::Spacing_BeforeTwips:
		pSpacing->bBefore = true;
		pSpacing->Before = m_oBufferedStream.GetLong();
		break;
	case c_oSerProp_pPrType::Spacing_After:
		pSpacing->bAfter = true;
		pSpacing->After = SerializeCommon::Round( g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
		break;
	case c_oSerProp_pPrType::Spacing_AfterTwips:
		pSpacing->bAfter = true;
		pSpacing->After = m_oBufferedStream.GetLong();
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
int Binary_pPrReader::ReadTabs(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::ReadTabItem(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	Tab* poTabItem = static_cast<Tab*>(poResult);
	if(c_oSerProp_pPrType::Tab_Item_Val == type)
		poTabItem->Val.SetValue((SimpleTypes::ETabJc)m_oBufferedStream.GetUChar());
	else if(c_oSerProp_pPrType::Tab_Item_Val_deprecated == type)
	{
		switch(m_oBufferedStream.GetUChar())
		{
		case 1:	poTabItem->Val.SetValue(SimpleTypes::tabjcRight);break;
		case 2:	poTabItem->Val.SetValue(SimpleTypes::tabjcCenter);break;
		case 3:	poTabItem->Val.SetValue(SimpleTypes::tabjcClear);break;
		default:poTabItem->Val.SetValue(SimpleTypes::tabjcLeft);break;
		}
	}
	else if(c_oSerProp_pPrType::Tab_Item_Pos == type)
		poTabItem->Pos = SerializeCommon::Round( g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	else if(c_oSerProp_pPrType::Tab_Item_PosTwips == type)
		poTabItem->Pos = m_oBufferedStream.GetLong();
	else if(c_oSerProp_pPrType::Tab_Item_Leader == type)
	{
		poTabItem->bLeader = true;
		poTabItem->Leader = m_oBufferedStream.GetUChar();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_pPrReader::ReadNumPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
int Binary_pPrReader::ReadBorders(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::ReadBorder(BYTE type, long length, void* poResult)
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
		odocBorder->Space = SerializeCommon::Round(g_dKoef_mm_to_pt * m_oBufferedStream.GetDouble());
	}
	else if( c_oSerBorderType::SpacePoint == type )
	{
		odocBorder->bSpace = true;
		odocBorder->Space = m_oBufferedStream.GetLong();
	}
	else if( c_oSerBorderType::Size == type )
	{
		odocBorder->bSize = true;
		odocBorder->Size = SerializeCommon::Round(g_dKoef_mm_to_eightpoint * m_oBufferedStream.GetDouble());
	}
	else if( c_oSerBorderType::Size8Point == type )
	{
		odocBorder->bSize = true;
		odocBorder->Size = m_oBufferedStream.GetLong();
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
int Binary_pPrReader::ReadBorder2(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	ComplexTypes::Word::CBorder* pBorder = static_cast<ComplexTypes::Word::CBorder*>(poResult);
	if( c_oSerBorderType::Color == type )
	{
		pBorder->m_oColor.Init();
		pBorder->m_oColor->SetValue(SimpleTypes::hexcolorRGB);
		oBinary_CommonReader2.ReadHexColor(pBorder->m_oColor.GetPointer());
	}
	else if( c_oSerBorderType::Space == type )
	{
		pBorder->m_oSpace.Init();
		pBorder->m_oSpace->SetValue(SerializeCommon::Round(g_dKoef_mm_to_pt * m_oBufferedStream.GetDouble()));
	}
	else if( c_oSerBorderType::SpacePoint == type )
	{
		pBorder->m_oSpace.Init();
		pBorder->m_oSpace->SetValue(m_oBufferedStream.GetLong());
	}
	else if( c_oSerBorderType::Size == type )
	{
		pBorder->m_oSz.Init();
		pBorder->m_oSz->SetValue(SerializeCommon::Round(g_dKoef_mm_to_eightpoint * m_oBufferedStream.GetDouble()));
	}
	else if( c_oSerBorderType::Size8Point == type )
	{
		pBorder->m_oSz.Init();
		pBorder->m_oSz->SetValue(m_oBufferedStream.GetLong());
	}
	else if( c_oSerBorderType::Value == type )
	{
		pBorder->m_oVal.Init();
		if(border_Single == m_oBufferedStream.GetUChar())
			pBorder->m_oVal->SetValue(SimpleTypes::bordervalueSingle);
		else
			pBorder->m_oVal->SetValue(SimpleTypes::bordervalueNone);
	}
	else if( c_oSerBorderType::ColorTheme == type )
	{
		CThemeColor ThemeColor;
		oBinary_CommonReader2.ReadThemeColor(length, ThemeColor);
		ThemeColor.ToCThemeColor(pBorder->m_oColor, pBorder->m_oThemeColor, pBorder->m_oThemeTint, pBorder->m_oThemeShade);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_pPrReader::ReadFramePr(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::Read_SecPr(BYTE type, long length, void* poResult)
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
	else if( c_oSerProp_secPrType::lnNumType == type )
	{
		ComplexTypes::Word::CLineNumber lineNumber;
		READ1_DEF(length, res, this->Read_lineNumType, &lineNumber);
		pSectPr->lineNum = L"<w:lnNumType " + lineNumber.ToString() + L"/>";
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
	else if( c_oSerProp_secPrType::rtlGutter == type )
	{
		pSectPr->bRtlGutter = true;
		pSectPr->RtlGutter = m_oBufferedStream.GetBool();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_pPrReader::ReadFootnotePr(BYTE type, long length, void* poResult)
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
		pFtnProps->m_oNumStart->m_oVal = m_oBufferedStream.GetLong();
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
int Binary_pPrReader::ReadEndnotePr(BYTE type, long length, void* poResult)
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
		pEdnProps->m_oNumStart->m_oVal = m_oBufferedStream.GetLong();
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
int Binary_pPrReader::ReadNumFmt(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::ReadSectPrChange(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::Read_pgSz(BYTE type, long length, void* poResult)
{
	SectPr* pSectPr = static_cast<SectPr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSer_pgSzType::Orientation == type )
	{
		pSectPr->bOrientation = true;
		pSectPr->cOrientation = m_oBufferedStream.GetUChar();
	}
	else if( c_oSer_pgSzType::W == type )
	{
		pSectPr->bW = true;
		pSectPr->W = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgSzType::WTwips == type )
	{
		pSectPr->bW = true;
		pSectPr->W = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_pgSzType::H == type )
	{
		pSectPr->bH = true;
		pSectPr->H = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgSzType::HTwips == type )
	{
		pSectPr->bH = true;
		pSectPr->H = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_pPrReader::Read_pgMar(BYTE type, long length, void* poResult)
{
	SectPr* pSectPr = static_cast<SectPr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSer_pgMarType::Left == type )
	{
		pSectPr->bLeft = true;
		pSectPr->Left = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgMarType::LeftTwips == type )
	{
		pSectPr->bLeft = true;
		pSectPr->Left = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_pgMarType::Top == type )
	{
		pSectPr->bTop = true;
		pSectPr->Top = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgMarType::TopTwips == type )
	{
		pSectPr->bTop = true;
		pSectPr->Top = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_pgMarType::Right == type )
	{
		pSectPr->bRight = true;
		pSectPr->Right = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgMarType::RightTwips == type )
	{
		pSectPr->bRight = true;
		pSectPr->Right = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_pgMarType::Bottom == type )
	{
		pSectPr->bBottom = true;
		pSectPr->Bottom = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgMarType::BottomTwips == type )
	{
		pSectPr->bBottom = true;
		pSectPr->Bottom = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_pgMarType::Header == type )
	{
		pSectPr->bHeader = true;
		pSectPr->Header = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgMarType::HeaderTwips == type )
	{
		pSectPr->bHeader = true;
		pSectPr->Header = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_pgMarType::Footer == type )
	{
		pSectPr->bFooter = true;
		pSectPr->Footer = SerializeCommon::Round(g_dKoef_mm_to_twips * m_oBufferedStream.GetDouble());
	}
	else if( c_oSer_pgMarType::FooterTwips == type )
	{
		pSectPr->bFooter = true;
		pSectPr->Footer = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_pgMarType::GutterTwips == type )
	{
		pSectPr->bGutter = true;
		pSectPr->Gutter = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_pPrReader::Read_pgSetting(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::Read_pgHeader(BYTE type, long length, void* poResult)
{
	SectPr* pSectPr = static_cast<SectPr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSerProp_secPrType::hdrftrelem == type )
	{
		int nHdrFtrIndex = m_oBufferedStream.GetLong();
		if(nHdrFtrIndex >= 0 && nHdrFtrIndex < (int)m_oFileWriter.get_headers_footers_writer().m_aHeaders.size())
		{
			Writers::HdrFtrItem* pHdrFtrItem = m_oFileWriter.get_headers_footers_writer().m_aHeaders[nHdrFtrIndex];

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
int Binary_pPrReader::Read_pgFooter(BYTE type, long length, void* poResult)
{
	SectPr* pSectPr = static_cast<SectPr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSerProp_secPrType::hdrftrelem == type )
	{
		int nHdrFtrIndex = m_oBufferedStream.GetLong();
		if(nHdrFtrIndex >= 0 && nHdrFtrIndex <= (int)oBinary_HdrFtrTableReader.m_oHeaderFooterWriter.m_aFooters.size())
		{
			Writers::HdrFtrItem* pHdrFtrItem = oBinary_HdrFtrTableReader.m_oHeaderFooterWriter.m_aFooters[nHdrFtrIndex];

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
int Binary_pPrReader::Read_pageNumType(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::Read_lineNumType(BYTE type, long length, void* poResult)
{
	ComplexTypes::Word::CLineNumber* pLineNumber = static_cast<ComplexTypes::Word::CLineNumber*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSerProp_secPrLineNumType::CountBy == type )
	{
		pLineNumber->m_oCountBy.Init();
		pLineNumber->m_oCountBy->SetValue(m_oBufferedStream.GetLong());
	}
	else if( c_oSerProp_secPrLineNumType::Distance == type )
	{
		pLineNumber->m_oDistance.Init();
		pLineNumber->m_oDistance->FromTwips(m_oBufferedStream.GetLong());
	}
	else if( c_oSerProp_secPrLineNumType::Restart == type )
	{
		pLineNumber->m_oRestart.Init();
		pLineNumber->m_oRestart->SetValue((SimpleTypes::ELineNumberRestart)m_oBufferedStream.GetUChar());
	}
	else if( c_oSerProp_secPrLineNumType::Start == type )
	{
		pLineNumber->m_oStart.Init();
		pLineNumber->m_oStart->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_pPrReader::ReadCols(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::ReadCol(BYTE type, long length, void* poResult)
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
int Binary_pPrReader::ReadPageBorders(BYTE type, long length, void* poResult)
{
	OOX::Logic::CPageBorders* pPageBorders = static_cast<OOX::Logic::CPageBorders*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSerPageBorders::Display == type )
	{
		pPageBorders->m_oDisplay.Init();
		pPageBorders->m_oDisplay->SetValueFromByte(m_oBufferedStream.GetChar());
	}
	else if( c_oSerPageBorders::OffsetFrom == type )
	{
		pPageBorders->m_oOffsetFrom.Init();
		pPageBorders->m_oOffsetFrom->SetValueFromByte(m_oBufferedStream.GetChar());
	}
	else if( c_oSerPageBorders::ZOrder == type )
	{
		pPageBorders->m_oZOrder.Init();
		pPageBorders->m_oZOrder->SetValueFromByte(m_oBufferedStream.GetChar());
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
int Binary_pPrReader::ReadPageBorder(BYTE type, long length, void* poResult)
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


Binary_tblPrReader::Binary_tblPrReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter):Binary_CommonReader(poBufferedStream),oBinary_CommonReader2(poBufferedStream),oBinary_pPrReader(poBufferedStream, oFileWriter)
{
}
int Binary_tblPrReader::Read_tblPr(BYTE type, long length, void* poResult)
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
	else if( c_oSerProp_tblPrType::TableIndTwips == type )
	{
		pWiterTblPr->TableInd = L"<w:tblInd w:w=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\" w:type=\"dxa\"/>";
	}
	else if( c_oSerProp_tblPrType::TableW == type )
	{
		docW odocW;
		READ2_DEF(length, res, this->ReadW, &odocW);
        pWiterTblPr->TableW = odocW.Write(std::wstring(_T("w:tblW")));
	}
	else if( c_oSerProp_tblPrType::TableCellMar == type )
	{
		NSStringUtils::CStringBuilder oTempWriter;
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
			NSStringUtils::CStringBuilder oTempWriter;
			odocBorders.Write(&oTempWriter, false);
            pWiterTblPr->TableBorders += L"<w:tblBorders>";
            pWiterTblPr->TableBorders += oTempWriter.GetData();
            pWiterTblPr->TableBorders += L"</w:tblBorders>";
		}
	}
	else if( c_oSerProp_tblPrType::Shd == type )
	{
		ComplexTypes::Word::CShading oShd;
		READ2_DEF(length, res, oBinary_CommonReader2.ReadShdComplexType, &oShd);

		std::wstring sShd = oShd.ToString();
		if (false == sShd.empty())
		{
			pWiterTblPr->Shd = L"<w:shd " + sShd + L"/>";
		}
	}
	else if( c_oSerProp_tblPrType::tblpPr == type )
	{
		NSStringUtils::CStringBuilder oTempWriter;
		READ2_DEF(length, res, this->Read_tblpPr, &oTempWriter);
        pWiterTblPr->tblpPr += L"<w:tblpPr w:vertAnchor=\"page\" w:horzAnchor=\"page\"";
        pWiterTblPr->tblpPr += oTempWriter.GetData();
        pWiterTblPr->tblpPr += L"/>";
	}
	else if( c_oSerProp_tblPrType::tblpPr2 == type )
	{
		NSStringUtils::CStringBuilder oTempWriter;
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
		_INT32 nLook = m_oBufferedStream.GetLong();
		int nFC = (0 == (nLook & 0x0080)) ? 0 : 1;
		int nFR = (0 == (nLook & 0x0020)) ? 0 : 1;
		int nLC = (0 == (nLook & 0x0100)) ? 0 : 1;
		int nLR = (0 == (nLook & 0x0040)) ? 0 : 1;
		int nBH = (0 == (nLook & 0x0200)) ? 0 : 1;
		int nBV = (0 == (nLook & 0x0400)) ? 0 : 1;
        pWiterTblPr->Look = L"<w:tblLook w:val=\"" + XmlUtils::ToString(nLook, L"%04X")
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
	else if( c_oSerProp_tblPrType::TableCellSpacingTwips == type )
	{
		pWiterTblPr->TableCellSpacing = L"<w:tblCellSpacing w:w=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\" w:type=\"dxa\"/>";
	}
	else if( c_oSerProp_tblPrType::tblCaption == type )
	{
		pWiterTblPr->Caption = m_oBufferedStream.GetString3(length);
	}
	else if( c_oSerProp_tblPrType::tblDescription == type )
	{
		pWiterTblPr->Description = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerProp_tblPrType::tblOverlap == type)
	{
		BYTE jc = m_oBufferedStream.GetUChar();
		switch (jc)
		{
			case 1: pWiterTblPr->Overlap = std::wstring(_T("<w:tblOverlap w:val=\"never\" />")); break;
			case 2: pWiterTblPr->Overlap = std::wstring(_T("<w:tblOverlap w:val=\"overlap\" />")); break;
		}

	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int Binary_tblPrReader::ReadW(BYTE type, long length, void* poResult)
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
int Binary_tblPrReader::ReadCellMargins(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
int Binary_tblPrReader::Read_tblpPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
int Binary_tblPrReader::Read_tblpPr2(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
	else if( c_oSer_tblpPrType2::TblpXTwips == type )
	{
		pCStringWriter->WriteString(L" w:tblpX=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
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
	else if( c_oSer_tblpPrType2::TblpYTwips == type )
	{
		pCStringWriter->WriteString(L" w:tblpY=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
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
int Binary_tblPrReader::Read_RowPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
			if(orowPrAfterBefore.nGridAfter < (long)m_aCurTblGrid.size())
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
	else if( c_oSerProp_rowPrType::TableCellSpacingTwips == type )
	{
		pCStringWriter->WriteString(L"<w:tblCellSpacing w:w=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\" w:type=\"dxa\"/>");
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
int Binary_tblPrReader::ReadAfter(BYTE type, long length, void* poResult)
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
int Binary_tblPrReader::ReadBefore(BYTE type, long length, void* poResult)
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
int Binary_tblPrReader::ReadHeight(BYTE type, long length, void* poResult)
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
	else if( c_oSerProp_rowPrType::Height_ValueTwips == type )
	{
		pHeight->nHeight = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int Binary_tblPrReader::Read_CellPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
		ComplexTypes::Word::CShading oShd;
		READ2_DEF(length, res, oBinary_CommonReader2.ReadShdComplexType, &oShd);
		
		std::wstring sShd = oShd.ToString();
		if (false == sShd.empty())
			pCStringWriter->WriteString(L"<w:shd " + sShd + L"/>");
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
		NSStringUtils::CStringBuilder oTempWriter;
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
	else if( c_oSerProp_cellPrType::HMerge == type )
	{
		BYTE HMerge = m_oBufferedStream.GetUChar();
		switch(HMerge)
		{
		case vmerge_Restart:pCStringWriter->WriteString(std::wstring(_T("<w:hMerge w:val=\"restart\" />")));break;
		case vmerge_Continue:pCStringWriter->WriteString(std::wstring(_T("<w:hMerge w:val=\"continue\" />")));break;
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
		SimpleTypes::CTextDirection oTextDirection;
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
int Binary_tblPrReader::ReadPaddings(BYTE type, long length, void* poResult)
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
int Binary_tblPrReader::ReadPaddings2(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
	if (c_oSerPaddingType::left == type)
	{
		double dLeft = m_oBufferedStream.GetDouble();
		long nLeft = SerializeCommon::Round( g_dKoef_mm_to_twips * dLeft);

        pCStringWriter->WriteString(L" w:leftFromText=\"" + std::to_wstring(nLeft) + L"\"");
	}
	else if (c_oSerPaddingType::leftTwips == type)
	{
		pCStringWriter->WriteString(L" w:leftFromText=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
	}
	else if (c_oSerPaddingType::top == type)
	{
		double dTop = m_oBufferedStream.GetDouble();
		long nTop = SerializeCommon::Round( g_dKoef_mm_to_twips * dTop);

        pCStringWriter->WriteString(L" w:topFromText=\"" + std::to_wstring(nTop) + L"\"");
	}
	else if (c_oSerPaddingType::topTwips == type)
	{
		pCStringWriter->WriteString(L" w:topFromText=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
	}
	else if (c_oSerPaddingType::right == type)
	{
		double dRight = m_oBufferedStream.GetDouble();
		long nRight = SerializeCommon::Round( g_dKoef_mm_to_twips * dRight);

        pCStringWriter->WriteString(L" w:rightFromText=\"" + std::to_wstring(nRight) + L"\"");
	}
	else if (c_oSerPaddingType::rightTwips == type)
	{
		pCStringWriter->WriteString(L" w:rightFromText=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
	}
	else if (c_oSerPaddingType::bottom == type)
	{
		double dBottom = m_oBufferedStream.GetDouble();
		long nBottom = SerializeCommon::Round( g_dKoef_mm_to_twips * dBottom);

        pCStringWriter->WriteString(L" w:bottomFromText=\"" + std::to_wstring(nBottom) + L"\"");
	}
	else if (c_oSerPaddingType::bottomTwips == type)
	{
		pCStringWriter->WriteString(L" w:bottomFromText=\"" + std::to_wstring(m_oBufferedStream.GetLong()) + L"\"");
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_tblPrReader::ReadCellMerge(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
	READ1_TRACKREV(type, length, pTrackRevision)
	else if (c_oSerProp_RevisionType::VMerge == type)
	{
		pTrackRevision->vMerge = new _INT32(m_oBufferedStream.GetLong());
	}
	else if (c_oSerProp_RevisionType::VMergeOrigin == type)
	{
		pTrackRevision->vMergeOrigin = new _INT32(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_tblPrReader::ReadTblPrChange(BYTE type, long length, void* poResult)
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
int Binary_tblPrReader::ReadTrPrChange(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
	READ1_TRACKREV(type, length, pTrackRevision)
	else if (c_oSerProp_RevisionType::trPrChange == type)
	{
		pTrackRevision->trPr = new NSStringUtils::CStringBuilder();
		READ2_DEF(length, res, this->Read_RowPr, pTrackRevision->trPr);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_tblPrReader::ReadTcPrChange(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
	READ1_TRACKREV(type, length, pTrackRevision)
	else if (c_oSerProp_RevisionType::tcPrChange == type)
	{
		pTrackRevision->tcPr = new NSStringUtils::CStringBuilder();
		READ2_DEF(length, res, this->Read_CellPr, pTrackRevision->tcPr);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

Binary_NumberingTableReader::Binary_NumberingTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter)
	:
	Binary_CommonReader(poBufferedStream), 
	oNumberingWriters(oFileWriter.get_numbering_writer()),
	m_oFontTableWriter(oFileWriter.get_font_table_writer()),
	oBinary_pPrReader(poBufferedStream, oFileWriter), 
	oBinary_rPrReader(poBufferedStream, oFileWriter)
{
}
int Binary_NumberingTableReader::Read()
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
int Binary_NumberingTableReader::ReadNumberingContent(BYTE type, long length, void* poResult)
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
int Binary_NumberingTableReader::ReadNums(BYTE type, long length, void* poResult)
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
int Binary_NumberingTableReader::ReadNum(BYTE type, long length, void* poResult)
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
int Binary_NumberingTableReader::ReadLvlOverride(BYTE type, long length, void* poResult)
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
int Binary_NumberingTableReader::ReadAbstractNums(BYTE type, long length, void* poResult)
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
int Binary_NumberingTableReader::ReadAbstractNum(BYTE type, long length, void* poResult)
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
int Binary_NumberingTableReader::ReadLevels(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	docANum* odocANum = static_cast<docANum*>(poResult);
	if ( c_oSerNumTypes::Lvl == type )
	{
		docLvl* odocLvl = new docLvl();
		odocLvl->ILvl = (long)odocANum->Lvls.size();
		READ2_DEF(length, res, this->ReadLevel, odocLvl);
		odocANum->Lvls.push_back(odocLvl);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_NumberingTableReader::ReadLevel(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	docLvl* odocLvl = static_cast<docLvl*>(poResult);
	if ( c_oSerNumTypes::lvl_Format == type )
	{
		odocLvl->bFormat = true;
		odocLvl->Format = m_oBufferedStream.GetLong();
	}
	else if ( c_oSerNumTypes::lvl_NumFmt == type )
	{
		ComplexTypes::Word::CNumFmt oNumFmt;
		READ1_DEF(length, res, oBinary_pPrReader.ReadNumFmt, &oNumFmt);
		odocLvl->sFormat = L"<w:numFmt " + oNumFmt.ToString() + L"/>";
	}
	else if ( c_oSerNumTypes::lvl_Jc_deprecated == type )
	{
		odocLvl->bJc = true;
		odocLvl->Jc = m_oBufferedStream.GetUChar();
	}
	else if ( c_oSerNumTypes::lvl_Jc == type )
	{
		ComplexTypes::Word::CJc oJc;
		oJc.m_oVal.Init();
		oJc.m_oVal->SetValue((SimpleTypes::EJc)m_oBufferedStream.GetUChar());
		odocLvl->sJc = L"<w:lvlJc " + oJc.ToString() + L"/>";
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
		
		OOX::Logic::CRunProperty orPr;
		res = oBinary_rPrReader.Read(length, &orPr);
		if(orPr.IsNoEmpty())
			odocLvl->TextPr.WriteString(orPr.toXML());
	}
	else if ( c_oSerNumTypes::ILvl == type )
	{
		odocLvl->bILvl = true;
		odocLvl->ILvl = m_oBufferedStream.GetLong();
	}
	else if ( c_oSerNumTypes::Tentative == type )
	{
		odocLvl->bTentative = true;
		odocLvl->Tentative = m_oBufferedStream.GetBool();
	}
	else if ( c_oSerNumTypes::Tplc == type )
	{
		odocLvl->bTplc = true;
		odocLvl->Tplc = m_oBufferedStream.GetULong();
	}
	else if ( c_oSerNumTypes::IsLgl == type )
	{
		odocLvl->bIsLgl = true;
		odocLvl->IsLgl = m_oBufferedStream.GetBool();
	}
	else if ( c_oSerNumTypes::LvlLegacy == type )
	{
		odocLvl->bLvlLegacy = true;
		READ1_DEF(length, res, this->ReadLvlLegacy, odocLvl);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_NumberingTableReader::ReadLvlLegacy(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	docLvl* odocLvl = static_cast<docLvl*>(poResult);
	if ( c_oSerNumTypes::Legacy == type )
	{
		odocLvl->bLegacy = true;
		odocLvl->Legacy = m_oBufferedStream.GetBool();
	}
	else if ( c_oSerNumTypes::LegacyIndent == type )
	{
		odocLvl->bLegacyIndent = true;
		odocLvl->LegacyIndent = m_oBufferedStream.GetLong();
	}
	else if ( c_oSerNumTypes::LegacySpace == type )
	{
		odocLvl->bLegacySpace = true;
		odocLvl->LegacySpace = m_oBufferedStream.GetULong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_NumberingTableReader::ReadLevelText(BYTE type, long length, void* poResult)
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
int Binary_NumberingTableReader::ReadLevelTextItem(BYTE type, long length, void* poResult)
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


BinaryStyleTableReader::BinaryStyleTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter) 
	:
	Binary_CommonReader(poBufferedStream),
	m_oStylesWriter(oFileWriter.get_style_writers()), 
	m_oFontTableWriter(oFileWriter.get_font_table_writer()), 
	oBinary_pPrReader(poBufferedStream, oFileWriter), 
	oBinary_rPrReader(poBufferedStream, oFileWriter), 
	oBinary_tblPrReader(poBufferedStream, oFileWriter)
{
}
int BinaryStyleTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadStyleTableContent, NULL);
	return res;
};
int BinaryStyleTableReader::ReadStyleTableContent(BYTE type, long length, void* poResult)
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
		//OOX::CDocDefaults docDefault;
		OOX::Logic::CRunProperty oRunPr;
		
		res = oBinary_rPrReader.Read(length, &oRunPr);
		if (oRunPr.IsNoEmpty())
		{
			m_oStylesWriter.m_rPrDefault.WriteString(oRunPr.toXML());
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadStyle(BYTE type, long length, void* poResult)
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
int BinaryStyleTableReader::ReadStyleContent(BYTE type, long length, void* poResult)
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
		odocStyle->bDefault = true;
		odocStyle->Default = m_oBufferedStream.GetBool();
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
		OOX::Logic::CRunProperty newRPr;
		res = oBinary_rPrReader.Read(length, &newRPr);
		if(newRPr.IsNoEmpty())
		{
			odocStyle->TextPr = newRPr.toXML();
		}
	}
	else if(c_oSer_sts::Style_ParaPr == type)
	{
		NSStringUtils::CStringBuilder oTempWriter;
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
		NSStringUtils::CStringBuilder oTempWriter;
		READ2_DEF(length, res, oBinary_tblPrReader.Read_RowPr, &oTempWriter);
        std::wstring sRowPr = oTempWriter.GetData();
		odocStyle->RowPr = sRowPr;
	}
	else if(c_oSer_sts::Style_CellPr == type)
	{
		NSStringUtils::CStringBuilder oTempWriter;
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
int BinaryStyleTableReader::ReadTblStylePr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	docStyle* odocStyle = static_cast<docStyle*>(poResult);
	if(c_oSerProp_tblStylePrType::TblStylePr == type)
	{
		tblStylePr otblStylePr;
		READ1_DEF(length, res, this->ReadTblStyleProperty, &otblStylePr);
		if(otblStylePr.bType && otblStylePr.Writer.GetCurSize() > 0)
		{
			NSStringUtils::CStringBuilder oCStringWriter;
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
int BinaryStyleTableReader::ReadTblStyleProperty(BYTE type, long length, void* poResult)
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
		OOX::Logic::CRunProperty newRPr;

		res = oBinary_rPrReader.Read(length, &newRPr);
		if(newRPr.IsNoEmpty())
		{
			ptblStylePr->Writer.WriteString(newRPr.toXML());
		}
	}
	else if(c_oSerProp_tblStylePrType::ParPr == type)
	{
		NSStringUtils::CStringBuilder oTempWriter;
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
		NSStringUtils::CStringBuilder oTempWriter;
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
		NSStringUtils::CStringBuilder oTempWriter;
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


Binary_OtherTableReader::Binary_OtherTableReader(std::wstring sFileInDir, NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter) 
	: m_sFileInDir(sFileInDir), Binary_CommonReader(poBufferedStream), m_oFileWriter(oFileWriter)
{
}
int Binary_OtherTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadOtherContent, NULL);
	return res;
}
int Binary_OtherTableReader::ReadOtherContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	// not using now
	//if ( c_oSerOtherTableTypes::ImageMap == type )
	//{
	//	READ1_DEF(length, res, this->ReadImageMapContent, NULL);
	//}
	//else 
	if(c_oSerOtherTableTypes::DocxTheme == type)
	{
		smart_ptr<PPTX::Theme> pTheme = new PPTX::Theme(NULL);
		try
		{
			pTheme->fromPPTY(&m_oBufferedStream);
		}
		catch(...)
		{
			//todooo в отдельный лог
		}
		if (false == m_oFileWriter.m_bGlossaryMode)
		{
			NSBinPptxRW::CXmlWriter xmlWriter;
			pTheme->toXmlWriter(&xmlWriter);
			m_oFileWriter.m_oTheme.m_sContent = xmlWriter.GetXmlString();

			if ((m_oFileWriter.m_pDrawingConverter) && (m_oFileWriter.m_pDrawingConverter->m_pTheme))
			{
				*m_oFileWriter.m_pDrawingConverter->m_pTheme = pTheme;
			}
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
Binary_CustomsTableReader::Binary_CustomsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter)
	: Binary_CommonReader(poBufferedStream), m_oFileWriter(oFileWriter)
{
}
int Binary_CustomsTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadCustom, NULL);

	return res;
}
int Binary_CustomsTableReader::ReadCustom(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;

	if (c_oSerCustoms::Custom == type)
	{
		OOX::CCustomXMLProps oCustomXmlProps(NULL);

		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadCustomContent, &oCustomXmlProps);

		if (false == oCustomXmlProps.m_oCustomXmlContentA.empty())
		{
			m_oFileWriter.m_oCustomXmlWriter.WriteCustomA(oCustomXmlProps.toXML(), oCustomXmlProps.m_oCustomXmlContentA, m_oFileWriter.m_bGlossaryMode);
		}
		else if (false == oCustomXmlProps.m_oCustomXmlContent.empty())
		{
			m_oFileWriter.m_oCustomXmlWriter.WriteCustom(oCustomXmlProps.toXML(), oCustomXmlProps.m_oCustomXmlContent, m_oFileWriter.m_bGlossaryMode);
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_CustomsTableReader::ReadCustomContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::CCustomXMLProps* pCustomXMLProps = static_cast<OOX::CCustomXMLProps*>(poResult);

	if (c_oSerCustoms::Uri == type)
	{
		if (false == pCustomXMLProps->m_oShemaRefs.IsInit())
			pCustomXMLProps->m_oShemaRefs.Init();

		pCustomXMLProps->m_oShemaRefs->m_arrItems.push_back(new OOX::CCustomXMLProps::CShemaRef());
		pCustomXMLProps->m_oShemaRefs->m_arrItems.back()->m_sUri = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerCustoms::ItemId == type)
	{
		pCustomXMLProps->m_oItemID.FromString(m_oBufferedStream.GetString3(length));
	}
	else if (c_oSerCustoms::Content == type)
	{
		pCustomXMLProps->m_oCustomXmlContent = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerCustoms::ContentA == type)
	{
		pCustomXMLProps->m_oCustomXmlContentA = m_oBufferedStream.GetString2A();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
Binary_CommentsTableReader::Binary_CommentsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter) 
	: Binary_CommonReader(poBufferedStream), m_oFileWriter(oFileWriter)
{
	if (!m_oFileWriter.m_pComments)
		m_oFileWriter.m_pComments = &m_oComments;
}
int Binary_CommentsTableReader::Read()
{
	m_oFileWriter.m_pDrawingConverter->SetDstContentRels();
    
	Writers::ContentWriter oContentWriter;
	Binary_DocumentTableReader oBinary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, oContentWriter, false);

	oBinary_DocumentTableReader.m_bUsedParaIdCounter = true;

	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadComments, &oBinary_DocumentTableReader);

	OOX::CPath fileRelsPath = m_oFileWriter.get_document_writer().m_sDir
		+ FILE_SEPARATOR_STR + L"word" 
		+ (m_oFileWriter.m_bGlossaryMode ? (FILE_SEPARATOR_STR + std::wstring(L"glossary")) : L"")
		+ FILE_SEPARATOR_STR + L"_rels"
		+ FILE_SEPARATOR_STR 
		+ m_oFileWriter.get_comments_writer().getFilename()
		+ L".rels";

	m_oFileWriter.m_pDrawingConverter->SaveDstContentRels(fileRelsPath.GetPath());
	
	return res;
}
int Binary_CommentsTableReader::ReadComments(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_CommentsType::Comment == type )
	{
		CComment* pComment = new CComment(m_oComments.m_oParaIdCounter, m_oComments.m_oFormatIdCounter);
		pComment->pBinary_DocumentTableReader = poResult;
		
		READ1_DEF(length, res, this->ReadCommentContent, pComment);

		if(pComment->bIdOpen && NULL == m_oComments.get(pComment->IdOpen))
			m_oComments.add(pComment);
		else
			RELEASEOBJECT(pComment);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_CommentsTableReader::ReadCommentContent(BYTE type, long length, void* poResult)
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
	else if ( c_oSer_CommentsType::Initials == type )
	{
		pComment->Initials = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_CommentsType::UserId == type )
	{
        std::wstring UserId(m_oBufferedStream.GetString3(length));
		pComment->UserId = UserId;
	}
	else if ( c_oSer_CommentsType::ProviderId == type )
	{
		std::wstring ProviderId(m_oBufferedStream.GetString3(length));
		pComment->ProviderId = ProviderId;
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
	else if ( c_oSer_CommentsType::DateUtc == type )
	{
		std::wstring Data(m_oBufferedStream.GetString3(length));
		pComment->DateUtc = Data;
	}
	else if ( c_oSer_CommentsType::UserData == type )
	{
		std::wstring Data(m_oBufferedStream.GetString3(length));
		pComment->UserData = Data;
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
	else if ( c_oSer_CommentsType::DurableId == type )
	{
		pComment->bDurableId = true;
		pComment->DurableId = m_oBufferedStream.GetULong();
	}
	else if ( c_oSer_CommentsType::CommentContent == type )
	{
		READ1_DEF(length, res, this->ReadCommentContentExt, pComment->pBinary_DocumentTableReader);

		Binary_DocumentTableReader* doc_reader = (Binary_DocumentTableReader*)pComment->pBinary_DocumentTableReader;
		
		pComment->sContent = doc_reader->m_oDocumentWriter.m_oContent.GetData();
		doc_reader->m_oDocumentWriter.m_oContent.Clear();

		_INT32 nId = m_oFileWriter.m_pComments->m_oParaIdCounter.getCurrentId();
		pComment->sParaId = XmlUtils::ToString(nId, L"%08X");

	}
	else if ( c_oSer_CommentsType::Replies == type )
	{
		READ1_DEF(length, res, this->ReadReplies, pComment);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_CommentsTableReader::ReadCommentContentExt(BYTE type, long length, void* poResult)
{
	Binary_DocumentTableReader* pBinary_DocumentTableReader = static_cast<Binary_DocumentTableReader*>(poResult);

	if (!pBinary_DocumentTableReader) return c_oSerConstants::ReadOk;

	return pBinary_DocumentTableReader->ReadDocumentContent(type, length, NULL);
}
int Binary_CommentsTableReader::ReadReplies(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;

	CComment* pCommentParent = static_cast<CComment*>(poResult);
	
	if ( c_oSer_CommentsType::Comment == type )
	{
		CComment* pNewComment = new CComment(m_oComments.m_oParaIdCounter, m_oComments.m_oFormatIdCounter);
		pNewComment->pBinary_DocumentTableReader = pCommentParent->pBinary_DocumentTableReader;

		READ1_DEF(length, res, this->ReadCommentContent, pNewComment);

		pNewComment->sParaIdParent = pCommentParent->sParaId;
		pCommentParent->replies.push_back(pNewComment);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}


Binary_SettingsTableReader::Binary_SettingsTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, OOX::CSettingsCustom* pSettingsCustom)
	:
	Binary_CommonReader(poBufferedStream), 
	m_oSettingWriter(oFileWriter.get_settings_writer()),
	m_oFileWriter(oFileWriter),
	m_oBinary_pPrReader(poBufferedStream, oFileWriter),
	m_oBinary_rPrReader(poBufferedStream, oFileWriter),
	m_pSettingsCustom(pSettingsCustom)
{
}
int Binary_SettingsTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;

	READ_TABLE_DEF(res, this->ReadSettings, m_oFileWriter.m_pCurrentSettings);
	
	return res;
};
int Binary_SettingsTableReader::ReadSettings(BYTE type, long length, void* poResult)
{
	OOX::CSettings *pSettings = static_cast<OOX::CSettings*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_SettingsType::ClrSchemeMapping == type )
	{
		pSettings->m_oClrSchemeMapping.Init();
		pSettings->m_oClrSchemeMapping->m_name = L"w:clrSchemeMapping";

		int aSchemeMapping[] = { 0, 1, 2, 3, 4, 5, 10, 11, 8, 9, 6, 7 };
		READ2_DEF(length, res, this->ReadClrSchemeMapping, aSchemeMapping);

		for (BYTE i = 0; i < 12; ++i)
		{
			std::wstring val;
			switch (i)
			{
				case 0: val = L"accent1";	break;
				case 1: val = L"accent2";	break;
				case 2: val = L"accent3";	break;
				case 3: val = L"accent4";	break;
				case 4: val = L"accent5";	break;
				case 5: val = L"accent6";	break;
				case 6: val = L"bg1";	break;
				case 7: val = L"bg2";	break;
				case 8: val = L"folHlink";	break;
				case 9: val = L"hlink";	break;
				case 10: val = L"tx1";	break;
				case 11: val = L"tx2";	break;
			}
			PPTX::Limit::ColorSchemeIndex index;
			index.SetBYTECode(aSchemeMapping[i] > 5 ? aSchemeMapping[i] + 2 : aSchemeMapping[i]);

			pSettings->m_oClrSchemeMapping->Insert(val, index);
		}
		smart_ptr<PPTX::Logic::ClrMap> pClrMap = pSettings->m_oClrSchemeMapping.GetPointer();
		pClrMap.AddRef();
		*m_oFileWriter.m_pDrawingConverter->m_pClrMap = pClrMap;
	}
	else if ( c_oSer_SettingsType::DefaultTabStop == type )
	{
		pSettings->m_oDefaultTabStop.Init();
		pSettings->m_oDefaultTabStop->m_oVal.Init();
		pSettings->m_oDefaultTabStop->m_oVal->FromMm(m_oBufferedStream.GetDouble());
	}
	else if ( c_oSer_SettingsType::DefaultTabStopTwips == type )
	{
		pSettings->m_oDefaultTabStop.Init();
		pSettings->m_oDefaultTabStop->m_oVal.Init();
		pSettings->m_oDefaultTabStop->m_oVal->SetValue(m_oBufferedStream.GetLong());
	}
	else if ( c_oSer_SettingsType::MathPr == type )
	{	
		pSettings->m_oMathPr.Init();
		READ1_DEF(length, res, this->ReadMathPr, pSettings->m_oMathPr.GetPointer());
	}
	else if ( c_oSer_SettingsType::TrackRevisions == type )
	{
		pSettings->m_oTrackRevisions.Init();
		pSettings->m_oTrackRevisions->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if( c_oSer_SettingsType::FootnotePr == type )
	{
		pSettings->m_oFootnotePr.Init();
		READ1_DEF(length, res, this->ReadFootnotePr, pSettings->m_oFootnotePr.GetPointer());
	}
	else if( c_oSer_SettingsType::EndnotePr == type )
	{
		pSettings->m_oEndnotePr.Init();
		READ1_DEF(length, res, this->ReadEndnotePr, pSettings->m_oEndnotePr.GetPointer());
	}
	else if( c_oSer_SettingsType::DecimalSymbol == type )
	{
		pSettings->m_oDecimalSymbol.Init();
		pSettings->m_oDecimalSymbol->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if( c_oSer_SettingsType::ListSeparator == type )
	{
		pSettings->m_oListSeparator.Init();
		pSettings->m_oListSeparator->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if( c_oSer_SettingsType::GutterAtTop == type )
	{
		pSettings->m_oGutterAtTop.Init();
		pSettings->m_oGutterAtTop->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if( c_oSer_SettingsType::MirrorMargins == type )
	{
		pSettings->m_oMirrorMargins.Init();
		pSettings->m_oMirrorMargins->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if( c_oSer_SettingsType::PrintTwoOnOne == type )
	{
		pSettings->m_oPrintTwoOnOne.Init();
		pSettings->m_oPrintTwoOnOne->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if( c_oSer_SettingsType::BookFoldPrinting == type )
	{
		pSettings->m_oBookFoldPrinting.Init();
		pSettings->m_oBookFoldPrinting->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if( c_oSer_SettingsType::BookFoldPrintingSheets == type )
	{
		pSettings->m_oBookFoldPrintingSheets.Init();
		pSettings->m_oBookFoldPrintingSheets->m_oVal = m_oBufferedStream.GetLong();
	}
	else if( c_oSer_SettingsType::BookFoldRevPrinting == type )
	{
		pSettings->m_oBookFoldRevPrinting.Init();
		pSettings->m_oBookFoldRevPrinting->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if( c_oSer_SettingsType::SdtGlobalColor == type )
	{
		OOX::Logic::CRunProperty oRPr;
		res = m_oBinary_rPrReader.Read(length, &oRPr);
		
		m_pSettingsCustom->m_oSdtGlobalColor = oRPr.m_oColor;
	}
	else if( c_oSer_SettingsType::SdtGlobalShowHighlight == type )
	{
		m_pSettingsCustom->m_oSdtGlobalShowHighlight.Init();
		m_pSettingsCustom->m_oSdtGlobalShowHighlight->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if( c_oSer_SettingsType::SpecialFormsHighlight == type )
	{
		OOX::Logic::CRunProperty oRPr;
		res = m_oBinary_rPrReader.Read(length, &oRPr);
		
		m_pSettingsCustom->m_oSpecialFormsHighlight = oRPr.m_oColor;
	}
	else if( c_oSer_SettingsType::Compat == type )
	{
		pSettings->m_oCompat.Init();
		READ1_DEF(length, res, this->ReadCompat, pSettings->m_oCompat.GetPointer());
	}
	else if (c_oSer_SettingsType::DocumentProtection == type)
	{
		pSettings->m_oDocumentProtection.Init();
		READ1_DEF(length, res, this->ReadDocProtect, pSettings->m_oDocumentProtection.GetPointer());
	}
	else if (c_oSer_SettingsType::WriteProtection == type)
	{
		pSettings->m_oWriteProtection.Init();
		READ1_DEF(length, res, this->ReadWriteProtect, pSettings->m_oWriteProtection.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int Binary_SettingsTableReader::ReadCompat(BYTE type, long length, void* poResult)
{
	OOX::Settings::CCompat* pCompat = static_cast<OOX::Settings::CCompat*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSerCompat::CompatSetting == type )
	{
		OOX::Settings::CCompatSetting* pCCompatSetting = new OOX::Settings::CCompatSetting();
		READ1_DEF(length, res, this->ReadCompatSetting, pCCompatSetting);
		pCompat->m_arrCompatSettings.push_back(pCCompatSetting);
	}
	else if( c_oSerCompat::Flags1 == type )
	{
		_UINT32 nFlags = m_oBufferedStream.GetULong();
		UINT_TO_COMPLEX_BOOL(0, pCompat->m_oUseSingleBorderforContiguousCells);
		UINT_TO_COMPLEX_BOOL(1, pCompat->m_oWpJustification);
		UINT_TO_COMPLEX_BOOL(2, pCompat->m_oNoTabHangInd);
		UINT_TO_COMPLEX_BOOL(3, pCompat->m_oNoLeading);
		UINT_TO_COMPLEX_BOOL(4, pCompat->m_oSpaceForUL);
		UINT_TO_COMPLEX_BOOL(5, pCompat->m_oNoColumnBalance);
		UINT_TO_COMPLEX_BOOL(6, pCompat->m_oBalanceSingleByteDoubleByteWidth);
		UINT_TO_COMPLEX_BOOL(7, pCompat->m_oNoExtraLineSpacing);
		UINT_TO_COMPLEX_BOOL(8, pCompat->m_oDoNotLeaveBackslashAlone);
		UINT_TO_COMPLEX_BOOL(9, pCompat->m_oUlTrailSpace);
		UINT_TO_COMPLEX_BOOL(10, pCompat->m_oDoNotExpandShiftReturn);
		UINT_TO_COMPLEX_BOOL(11, pCompat->m_oSpacingInWholePoints);
		UINT_TO_COMPLEX_BOOL(12, pCompat->m_oLineWrapLikeWord6);
		UINT_TO_COMPLEX_BOOL(13, pCompat->m_oPrintBodyTextBeforeHeader);
		UINT_TO_COMPLEX_BOOL(14, pCompat->m_oPrintColBlack);
		UINT_TO_COMPLEX_BOOL(15, pCompat->m_oWpSpaceWidth);
		UINT_TO_COMPLEX_BOOL(16, pCompat->m_oShowBreaksInFrames);
		UINT_TO_COMPLEX_BOOL(17, pCompat->m_oSubFontBySize);
		UINT_TO_COMPLEX_BOOL(18, pCompat->m_oSuppressBottomSpacing);
		UINT_TO_COMPLEX_BOOL(19, pCompat->m_oSuppressTopSpacing);
		UINT_TO_COMPLEX_BOOL(20, pCompat->m_oSuppressSpacingAtTopOfPage);
		UINT_TO_COMPLEX_BOOL(21, pCompat->m_oSuppressTopSpacingWP);
		UINT_TO_COMPLEX_BOOL(22, pCompat->m_oSuppressSpBfAfterPgBrk);
		UINT_TO_COMPLEX_BOOL(23, pCompat->m_oSwapBordersFacingPages);
		UINT_TO_COMPLEX_BOOL(24, pCompat->m_oConvMailMergeEsc);
		UINT_TO_COMPLEX_BOOL(25, pCompat->m_oTruncateFontHeightsLikeWP6);
		UINT_TO_COMPLEX_BOOL(26, pCompat->m_oMwSmallCaps);
		UINT_TO_COMPLEX_BOOL(27, pCompat->m_oUsePrinterMetrics);
		UINT_TO_COMPLEX_BOOL(28, pCompat->m_oDoNotSuppressParagraphBorders);
		UINT_TO_COMPLEX_BOOL(29, pCompat->m_oWrapTrailSpaces);
		UINT_TO_COMPLEX_BOOL(30, pCompat->m_oFootnoteLayoutLikeWW8);
		UINT_TO_COMPLEX_BOOL(31, pCompat->m_oShapeLayoutLikeWW8);
	}
	else if( c_oSerCompat::Flags2 == type )
	{
		_UINT32 nFlags = m_oBufferedStream.GetULong();
		UINT_TO_COMPLEX_BOOL(0, pCompat->m_oAlignTablesRowByRow);
		UINT_TO_COMPLEX_BOOL(1, pCompat->m_oForgetLastTabAlignment);
		UINT_TO_COMPLEX_BOOL(2, pCompat->m_oAdjustLineHeightInTable);
		UINT_TO_COMPLEX_BOOL(3, pCompat->m_oAutoSpaceLikeWord95);
		UINT_TO_COMPLEX_BOOL(4, pCompat->m_oNoSpaceRaiseLower);
		UINT_TO_COMPLEX_BOOL(5, pCompat->m_oDoNotUseHTMLParagraphAutoSpacing);
		UINT_TO_COMPLEX_BOOL(6, pCompat->m_oLayoutRawTableWidth);
		UINT_TO_COMPLEX_BOOL(7, pCompat->m_oLayoutTableRowsApart);
		UINT_TO_COMPLEX_BOOL(8, pCompat->m_oUseWord97LineBreakRules);
		UINT_TO_COMPLEX_BOOL(9, pCompat->m_oDoNotBreakWrappedTables);
		UINT_TO_COMPLEX_BOOL(10, pCompat->m_oDoNotSnapToGridInCell);
		UINT_TO_COMPLEX_BOOL(11, pCompat->m_oSelectFldWithFirstOrLastChar);
		UINT_TO_COMPLEX_BOOL(12, pCompat->m_oApplyBreakingRules);
		UINT_TO_COMPLEX_BOOL(13, pCompat->m_oDoNotWrapTextWithPunct);
		UINT_TO_COMPLEX_BOOL(14, pCompat->m_oDoNotUseEastAsianBreakRules);
		UINT_TO_COMPLEX_BOOL(15, pCompat->m_oUseWord2002TableStyleRules);
		UINT_TO_COMPLEX_BOOL(16, pCompat->m_oGrowAutofit);
		UINT_TO_COMPLEX_BOOL(17, pCompat->m_oUseFELayout);
		UINT_TO_COMPLEX_BOOL(18, pCompat->m_oUseNormalStyleForList);
		UINT_TO_COMPLEX_BOOL(19, pCompat->m_oDoNotUseIndentAsNumberingTabStop);
		UINT_TO_COMPLEX_BOOL(20, pCompat->m_oUseAltKinsokuLineBreakRules);
		UINT_TO_COMPLEX_BOOL(21, pCompat->m_oAllowSpaceOfSameStyleInTable);
		UINT_TO_COMPLEX_BOOL(22, pCompat->m_oDoNotSuppressIndentation);
		UINT_TO_COMPLEX_BOOL(23, pCompat->m_oDoNotAutofitConstrainedTables);
		UINT_TO_COMPLEX_BOOL(24, pCompat->m_oAutofitToFirstFixedWidthCell);
		UINT_TO_COMPLEX_BOOL(25, pCompat->m_oUnderlineTabInNumList);
		UINT_TO_COMPLEX_BOOL(26, pCompat->m_oDisplayHangulFixedWidth);
		UINT_TO_COMPLEX_BOOL(27, pCompat->m_oSplitPgBreakAndParaMark);
		UINT_TO_COMPLEX_BOOL(28, pCompat->m_oDoNotVertAlignCellWithSp);
		UINT_TO_COMPLEX_BOOL(29, pCompat->m_oDoNotBreakConstrainedForcedTable);
		UINT_TO_COMPLEX_BOOL(30, pCompat->m_oDoNotVertAlignInTxbx);
		UINT_TO_COMPLEX_BOOL(31, pCompat->m_oUseAnsiKerningPairs);
	}
	else if( c_oSerCompat::Flags3 == type )
	{
		_UINT32 nFlags = m_oBufferedStream.GetULong();
		UINT_TO_COMPLEX_BOOL(0, pCompat->m_oCachedColBalance);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadCompatSetting(BYTE type, long length, void* poResult)
{
	OOX::Settings::CCompatSetting* pCompatSetting = static_cast<OOX::Settings::CCompatSetting*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if( c_oSerCompat::CompatName == type )
	{
		pCompatSetting->m_sName = m_oBufferedStream.GetString3(length);
	}
	else if( c_oSerCompat::CompatUri == type )
	{
		pCompatSetting->m_sUri = m_oBufferedStream.GetString3(length);
	}
	else if( c_oSerCompat::CompatValue == type )
	{
		pCompatSetting->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadDocProtect(BYTE type, long length, void* poResult)
{
	OOX::Settings::CDocProtect* pDocProtect = static_cast<OOX::Settings::CDocProtect*>(poResult);
	int res = c_oSerConstants::ReadOk;
	
	if (c_oDocProtect::AlgIdExt == type)
	{
		pDocProtect->m_oAlgIdExt = m_oBufferedStream.GetString3(length);
	}
	else if (c_oDocProtect::AlgIdExtSource == type)
	{
		pDocProtect->m_oAlgIdExtSource = m_oBufferedStream.GetString3(length);
	}
	else if (c_oDocProtect::AlgorithmName == type)
	{
		pDocProtect->m_oAlgorithmName.Init();  
		pDocProtect->m_oAlgorithmName->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oDocProtect::CryptAlgorithmClass == type)
	{
		pDocProtect->m_oCryptAlgorithmClass.Init();
		pDocProtect->m_oCryptAlgorithmClass->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oDocProtect::CryptAlgorithmSid == type)
	{
		pDocProtect->m_oCryptAlgorithmSid = m_oBufferedStream.GetLong();
	}
	else if (c_oDocProtect::CryptAlgorithmType == type)
	{
		pDocProtect->m_oCryptAlgorithmType.Init();
		pDocProtect->m_oCryptAlgorithmType->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oDocProtect::CryptProvider == type)
	{
		pDocProtect->m_oCryptProvider = m_oBufferedStream.GetString3(length);
	}
	else if (c_oDocProtect::CryptProviderType == type)
	{
		pDocProtect->m_oCryptProviderType.Init();
		pDocProtect->m_oCryptProviderType->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oDocProtect::CryptProviderTypeExt == type)
	{
		pDocProtect->m_oCryptProviderTypeExt = m_oBufferedStream.GetString3(length);
	}
	else if (c_oDocProtect::CryptProviderTypeExtSource == type)
	{
		pDocProtect->m_oCryptProviderTypeExtSource = m_oBufferedStream.GetString3(length);
	}
	else if (c_oDocProtect::Edit == type)
	{
		pDocProtect->m_oEdit.Init();
		pDocProtect->m_oEdit->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oDocProtect::Enforcment == type)
	{
		pDocProtect->m_oEnforcment = m_oBufferedStream.GetUChar() != 0;
	}
	else if (c_oDocProtect::Formatting == type)
	{
		pDocProtect->m_oFormatting = m_oBufferedStream.GetUChar() != 0;
	}
	else if (c_oDocProtect::HashValue == type)
	{
		pDocProtect->m_sHashValue = m_oBufferedStream.GetString3(length);
	}
	else if (c_oDocProtect::SaltValue == type)
	{
		pDocProtect->m_sSaltValue = m_oBufferedStream.GetString3(length);
	}
	else if (c_oDocProtect::SpinCount == type)
	{
		pDocProtect->m_oSpinCount = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;

}
int Binary_SettingsTableReader::ReadWriteProtect(BYTE type, long length, void* poResult)
{
	OOX::Settings::CWriteProtection* pWriteProtect = static_cast<OOX::Settings::CWriteProtection*>(poResult);
	int res = c_oSerConstants::ReadOk;

	if (c_oWriteProtect::AlgIdExt == type)
	{
		pWriteProtect->m_oAlgIdExt = m_oBufferedStream.GetString3(length);
	}
	else if (c_oWriteProtect::AlgIdExtSource == type)
	{
		pWriteProtect->m_oAlgIdExtSource = m_oBufferedStream.GetString3(length);
	}
	else if (c_oWriteProtect::AlgorithmName == type)
	{
		pWriteProtect->m_oAlgorithmName.Init();
		pWriteProtect->m_oAlgorithmName->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oWriteProtect::CryptAlgorithmClass == type)
	{
		pWriteProtect->m_oCryptAlgorithmClass.Init();
		pWriteProtect->m_oCryptAlgorithmClass->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oWriteProtect::CryptAlgorithmSid == type)
	{
		pWriteProtect->m_oCryptAlgorithmSid = m_oBufferedStream.GetLong();
	}
	else if (c_oWriteProtect::CryptAlgorithmType == type)
	{
		pWriteProtect->m_oCryptAlgorithmType.Init();
		pWriteProtect->m_oCryptAlgorithmType->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oWriteProtect::CryptProvider == type)
	{
		pWriteProtect->m_oCryptProvider = m_oBufferedStream.GetString3(length);
	}
	else if (c_oWriteProtect::CryptProviderType == type)
	{
		pWriteProtect->m_oCryptProviderType.Init();
		pWriteProtect->m_oCryptProviderType->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oWriteProtect::CryptProviderTypeExt == type)
	{
		pWriteProtect->m_oCryptProviderTypeExt = m_oBufferedStream.GetString3(length);
	}
	else if (c_oWriteProtect::CryptProviderTypeExtSource == type)
	{
		pWriteProtect->m_oCryptProviderTypeExtSource = m_oBufferedStream.GetString3(length);
	}
	else if (c_oWriteProtect::Recommended == type)
	{
		pWriteProtect->m_oRecommended = m_oBufferedStream.GetUChar() != 0;
	}
	else if (c_oWriteProtect::HashValue == type)
	{
		pWriteProtect->m_sHashValue = m_oBufferedStream.GetString3(length);
	}
	else if (c_oWriteProtect::SaltValue == type)
	{
		pWriteProtect->m_sSaltValue = m_oBufferedStream.GetString3(length);
	}
	else if (c_oWriteProtect::SpinCount == type)
	{
		pWriteProtect->m_oSpinCount = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;

}
int Binary_SettingsTableReader::ReadFootnotePr(BYTE type, long length, void* poResult)
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
		pFtnProps->m_oNumStart->m_oVal = m_oBufferedStream.GetLong();
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
int Binary_SettingsTableReader::ReadEndnotePr(BYTE type, long length, void* poResult)
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
		pEdnProps->m_oNumStart->m_oVal = m_oBufferedStream.GetLong();
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

int Binary_SettingsTableReader::ReadMathPr(BYTE type, long length, void* poResult)
{
	OOX::Logic::CMathPr* pMathPr = static_cast<OOX::Logic::CMathPr*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_MathPrType::BrkBin == type )
	{
		pMathPr->m_oBrkBin.Init();
		READ2_DEF(length, res, this->ReadMathBrkBin, pMathPr->m_oBrkBin.GetPointer());
	}
	else if ( c_oSer_MathPrType::BrkBinSub == type )
	{
		pMathPr->m_oBrkBinSub.Init();
		READ2_DEF(length, res, this->ReadMathBrkBinSub, pMathPr->m_oBrkBinSub.GetPointer());
	}
	else if ( c_oSer_MathPrType::DefJc == type )
	{
		pMathPr->m_oDefJc.Init();
		READ2_DEF(length, res, this->ReadMathDefJc, pMathPr->m_oDefJc.GetPointer());
	}
	else if ( c_oSer_MathPrType::DispDef == type )
	{
		pMathPr->m_oDispDef.Init();
		READ2_DEF(length, res, this->ReadMathDispDef, pMathPr->m_oDispDef.GetPointer());
	}
	else if ( c_oSer_MathPrType::InterSp == type )
	{
		pMathPr->m_oInterSp.Init();
		READ2_DEF(length, res, this->ReadMathInterSp, pMathPr->m_oInterSp.GetPointer());
	}
	else if ( c_oSer_MathPrType::IntLim == type )
	{
		pMathPr->m_oIntLim.Init();
		READ2_DEF(length, res, this->ReadMathIntLim, pMathPr->m_oIntLim.GetPointer());
	}
	else if ( c_oSer_MathPrType::IntraSp == type )
	{
		pMathPr->m_oIntraSp.Init();
		READ2_DEF(length, res, this->ReadMathIntraSp, pMathPr->m_oIntraSp.GetPointer());
	}
	else if ( c_oSer_MathPrType::LMargin == type )
	{
		pMathPr->m_oLMargin.Init();
		READ2_DEF(length, res, this->ReadMathLMargin, pMathPr->m_oLMargin.GetPointer());
	}
	else if ( c_oSer_MathPrType::MathFont == type )
	{
		pMathPr->m_oMathFont.Init();
		READ2_DEF(length, res, this->ReadMathMathFont, pMathPr->m_oMathFont.GetPointer());
	}
	else if ( c_oSer_MathPrType::NaryLim == type )
	{
		pMathPr->m_oNaryLim.Init();
		READ2_DEF(length, res, this->ReadMathNaryLim, pMathPr->m_oNaryLim.GetPointer());
	}
	else if ( c_oSer_MathPrType::PostSp == type )
	{
		pMathPr->m_oPostSp.Init();
		READ2_DEF(length, res, this->ReadMathPostSp, pMathPr->m_oPostSp.GetPointer());
	}
	else if ( c_oSer_MathPrType::PreSp == type )
	{
		pMathPr->m_oPreSp.Init();
		READ2_DEF(length, res, this->ReadMathPreSp, pMathPr->m_oPreSp.GetPointer());
	}
	else if ( c_oSer_MathPrType::RMargin == type )
	{
		pMathPr->m_oRMargin.Init();
		READ2_DEF(length, res, this->ReadMathRMargin, pMathPr->m_oRMargin.GetPointer());
	}
	else if ( c_oSer_MathPrType::SmallFrac == type )
	{
		pMathPr->m_oSmallFrac.Init();
		READ2_DEF(length, res, this->ReadMathSmallFrac, pMathPr->m_oSmallFrac.GetPointer());
	}
	else if ( c_oSer_MathPrType::WrapIndent == type )
	{
		pMathPr->m_oWrapIndent.Init();
		READ2_DEF(length, res, this->ReadMathWrapIndent, pMathPr->m_oWrapIndent.GetPointer());
	}
	else if ( c_oSer_MathPrType::WrapRight == type )
	{
		pMathPr->m_oWrapRight.Init();
		READ2_DEF(length, res, this->ReadMathWrapRight, pMathPr->m_oWrapRight.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathBrkBin(BYTE type, long length, void* poResult)//GetUChar
{
	OOX::Logic::CBrkBin* item = static_cast<OOX::Logic::CBrkBin*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if (c_oSer_OMathBottomNodesValType::Val == type)
	{
		item->m_val.Init();
		item->m_val->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathBrkBinSub(BYTE type, long length, void* poResult)//GetUChar
{
	OOX::Logic::CBrkBinSub* item = static_cast<OOX::Logic::CBrkBinSub*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if (c_oSer_OMathBottomNodesValType::Val == type)
	{
		item->m_val.Init();
		item->m_val->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadMathDefJc(BYTE type, long length, void* poResult)//GetUChar
{
	OOX::Logic::CDefJc* item = static_cast<OOX::Logic::CDefJc*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if (c_oSer_OMathBottomNodesValType::Val == type)
	{
		item->m_val.Init();
		item->m_val->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadMathDispDef(BYTE type, long length, void* poResult)//GetBool
{
	OOX::Logic::CDispDef* item = static_cast<OOX::Logic::CDispDef*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val.Init();
		item->m_val->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathInterSp(BYTE type, long length, void* poResult)//GetUChar
{
	OOX::Logic::CInterSp* item = static_cast<OOX::Logic::CInterSp*>(poResult);
	item->m_val.Init();

	int res = c_oSerConstants::ReadOk;
	if (c_oSer_OMathBottomNodesValType::Val == type)
	{
		item->m_val->FromMm(m_oBufferedStream.GetDouble());
	}
	else if (c_oSer_OMathBottomNodesValType::ValTwips == type)
	{
		item->m_val->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadMathIntLim(BYTE type, long length, void* poResult)//GetUChar
{
	OOX::Logic::CIntLim* item = static_cast<OOX::Logic::CIntLim*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val.Init();
		item->m_val->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadMathIntraSp(BYTE type, long length, void* poResult)//GetUChar
{
	OOX::Logic::CIntraSp* item = static_cast<OOX::Logic::CIntraSp*>(poResult);
	item->m_val.Init();

	int res = c_oSerConstants::ReadOk;
	if (c_oSer_OMathBottomNodesValType::Val == type)
	{
		item->m_val->FromMm(m_oBufferedStream.GetDouble());
		//LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());
	}
	else if (c_oSer_OMathBottomNodesValType::ValTwips == type)
	{
		item->m_val->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadMathLMargin(BYTE type, long length, void* poResult)//GetUChar
{
	OOX::Logic::CLMargin* item = static_cast<OOX::Logic::CLMargin*>(poResult);
	item->m_val.Init();

	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val->FromMm(m_oBufferedStream.GetDouble());
		//LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());
	}
	else if (c_oSer_OMathBottomNodesValType::ValTwips == type)
	{
		item->m_val->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathMathFont(BYTE type, long length, void* poResult)//ReadString
{
	OOX::Logic::CMathFont* item = static_cast<OOX::Logic::CMathFont*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val = m_oBufferedStream.GetString3(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int Binary_SettingsTableReader::ReadMathNaryLim(BYTE type, long length, void* poResult)//Readbyte
{
	OOX::Logic::CNaryLim* item = static_cast<OOX::Logic::CNaryLim*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val.Init();
		item->m_val->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}		
int Binary_SettingsTableReader::ReadMathPostSp(BYTE type, long length, void* poResult)
{
	OOX::Logic::CPostSp* item = static_cast<OOX::Logic::CPostSp*>(poResult);
	item->m_val.Init();

	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val->FromMm(m_oBufferedStream.GetDouble());
		//LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());
	}
	else if ( c_oSer_OMathBottomNodesValType::ValTwips == type )
	{
		item->m_val->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathPreSp(BYTE type, long length, void* poResult)
{
	OOX::Logic::CPreSp* item = static_cast<OOX::Logic::CPreSp*>(poResult);
	item->m_val.Init();

	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val->FromMm(m_oBufferedStream.GetDouble());
		//LONG lVal =  (LONG)Mm_To_Dx(m_oBufferedStream.GetDouble());
	}
	else if ( c_oSer_OMathBottomNodesValType::ValTwips == type )
	{
		item->m_val->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathRMargin(BYTE type, long length, void* poResult)
{
	OOX::Logic::CRMargin* item = static_cast<OOX::Logic::CRMargin*>(poResult);
	item->m_val.Init();
	
	int res = c_oSerConstants::ReadOk;	

	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val->FromMm(m_oBufferedStream.GetDouble());
	}
	else if ( c_oSer_OMathBottomNodesValType::ValTwips == type )
	{
		item->m_val->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathSmallFrac(BYTE type, long length, void* poResult)//GetBool
{
	OOX::Logic::CSmallFrac* item = static_cast<OOX::Logic::CSmallFrac*>(poResult);
	int res = c_oSerConstants::ReadOk;
	
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val.Init();
		item->m_val->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathWrapIndent(BYTE type, long length, void* poResult)
{
	OOX::Logic::CWrapIndent* item = static_cast<OOX::Logic::CWrapIndent*>(poResult);
	int res = c_oSerConstants::ReadOk;
	
	item->m_val.Init();
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val->FromMm(m_oBufferedStream.GetDouble());
	}
	else if ( c_oSer_OMathBottomNodesValType::ValTwips == type )
	{
		item->m_val->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_SettingsTableReader::ReadMathWrapRight(BYTE type, long length, void* poResult)//GetBool
{
	OOX::Logic::CWrapRight* item = static_cast<OOX::Logic::CWrapRight*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		item->m_val.Init();
		item->m_val->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_SettingsTableReader::ReadClrSchemeMapping(BYTE type, long length, void* poResult)
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


Binary_DocumentTableReader::Binary_DocumentTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, Writers::ContentWriter& oDocumentWriter, bool bOFormRead)
        : Binary_CommonReader(poBufferedStream)
        , m_oDocumentWriter(oDocumentWriter)
        , m_oFileWriter(oFileWriter)
        , m_oMediaWriter(oFileWriter.m_oMediaWriter)
        , m_oFontTableWriter(oFileWriter.get_font_table_writer())
        , oBinary_CommonReader2(poBufferedStream)
        , oBinary_pPrReader(poBufferedStream, oFileWriter)
        , oBinary_rPrReader(poBufferedStream, oFileWriter)
        , oBinary_tblPrReader(poBufferedStream, oFileWriter)
		, m_bOFormRead(bOFormRead)
{
	m_bUsedParaIdCounter = false;
	m_byteLastElemType = c_oSerParType::Content;
	m_pCurWriter = NULL;
}
Binary_DocumentTableReader::~Binary_DocumentTableReader()
{
}
int Binary_DocumentTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadDocumentContent, NULL);
	return res;
}
NSStringUtils::CStringBuilder& Binary_DocumentTableReader::GetRunStringWriter()
{
	if(NULL != m_pCurWriter)
		return *m_pCurWriter;
	else
		return m_oDocumentWriter.m_oContent;
}
int Binary_DocumentTableReader::ReadDocumentContentOut(long length)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadDocumentContent, NULL);
	return res;
}
int Binary_DocumentTableReader::ReadDocumentContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSerParType::Par == type )
	{
		m_byteLastElemType = c_oSerParType::Par;
		m_oCur_pPr.ClearNoAttack();

		if (m_bUsedParaIdCounter && m_oFileWriter.m_pComments)
		{
			_INT32 nId = m_oFileWriter.m_pComments->m_oParaIdCounter.getNextId();
			std::wstring sParaId = XmlUtils::ToString(nId, L"%08X");

			m_oDocumentWriter.m_oContent.WriteString(L"<w:p w14:paraId=\"" + sParaId + L"\" w14:textId=\"" + sParaId + L"\">");
		}
		else
		{
			m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"<w:p>"));
		}
		READ1_DEF(length, res, this->ReadParagraph, NULL);
        m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"</w:p>"));
	}
	else if (c_oSerParType::CommentStart == type)
	{
		long nId = 0;
		READ1_DEF(length, res, this->ReadComment, &nId);
		if (NULL != m_oFileWriter.m_pComments)
		{
			CComment* pComment = m_oFileWriter.m_pComments->get(nId);
			if (NULL != pComment)
			{
				int nNewId = m_oFileWriter.m_pComments->getNextId(pComment->getCount());
				pComment->setFormatStart(nNewId);
				GetRunStringWriter().WriteString(pComment->writeRef(std::wstring(_T("")), std::wstring(_T("w:commentRangeStart")), std::wstring(_T(""))));
			}
		}
	}
	else if (c_oSerParType::CommentEnd == type)
	{
		long nId = 0;
		READ1_DEF(length, res, this->ReadComment, &nId);
		if (NULL != m_oFileWriter.m_pComments)
		{
			CComment* pComment = m_oFileWriter.m_pComments->get(nId);
			if (NULL != pComment && pComment->bIdFormat)
				GetRunStringWriter().WriteString(pComment->writeRef(std::wstring(_T("")), std::wstring(_T("w:commentRangeEnd")), std::wstring(_T(""))));
		}
	}
	else if(c_oSerParType::Table == type)
	{
		m_byteLastElemType = c_oSerParType::Table;
        m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:tbl>")));
		READ1_DEF(length, res, this->ReadDocTable, &m_oDocumentWriter.m_oContent);
        m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("</w:tbl>")));
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
		
		if (oSectPr.bEvenAndOddHeaders && oSectPr.EvenAndOddHeaders)
			m_oFileWriter.m_pCurrentSettings->m_oEvenAndOddHeaders.Init();
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
	else if ( c_oSerParType::MoveFromRangeStart == type )
	{
		OOX::Logic::CMoveFromRangeStart oMoveFromRangeStart;
		READ1_DEF(length, res, this->ReadMoveFromRangeStart, &oMoveFromRangeStart);
		m_oDocumentWriter.m_oContent.WriteString(oMoveFromRangeStart.toXML());
	}
	else if ( c_oSerParType::MoveFromRangeEnd == type )
	{
		OOX::Logic::CMoveFromRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveFromRangeEnd, &oMoveToRangeEnd);
		m_oDocumentWriter.m_oContent.WriteString(oMoveToRangeEnd.toXML());
	}
	else if ( c_oSerParType::MoveToRangeStart == type )
	{
		OOX::Logic::CMoveToRangeStart oMoveToRangeStart;
		READ1_DEF(length, res, this->ReadMoveToRangeStart, &oMoveToRangeStart);
		m_oDocumentWriter.m_oContent.WriteString(oMoveToRangeStart.toXML());
	}
	else if ( c_oSerParType::MoveToRangeEnd == type )
	{
		OOX::Logic::CMoveToRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveToRangeEnd, &oMoveToRangeEnd);
		m_oDocumentWriter.m_oContent.WriteString(oMoveToRangeEnd.toXML());
	}
	else if (c_oSerParType::DocParts == type)
	{
		m_oDocumentWriter.m_oContent.WriteString(L"<w:docParts>");
		OOX::Logic::CDocParts oDocParts;
		READ1_DEF(length, res, this->ReadDocParts, &oDocParts);
		m_oDocumentWriter.m_oContent.WriteString(L"</w:docParts>");
	}
	else if(c_oSerParType::JsaProject == type)
	{
		BYTE* pData = m_oBufferedStream.GetPointer(length);
		OOX::CPath sJsaProject = OOX::FileTypes::JsaProject.DefaultFileName();
		std::wstring filePath = m_oFileWriter.get_document_writer().m_sDir + FILE_SEPARATOR_STR + L"word"+ FILE_SEPARATOR_STR + sJsaProject.GetPath();

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(filePath);
		oFile.WriteFile(pData, length);
		oFile.CloseFile();

		unsigned int lId = 0;
		m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::JsaProject.RelationType(), sJsaProject.GetPath(), L"", &lId);
		m_oFileWriter.m_pDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(sJsaProject.GetExtention(false));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadDocParts(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CDocParts* pDocParts = static_cast<OOX::Logic::CDocParts*>(poResult);
	
	if (c_oSerGlossary::DocPart == type)
	{
		m_oDocumentWriter.m_oContent.WriteString(L"<w:docPart>");
		pDocParts->m_arrItems.push_back(new OOX::Logic::CDocPart());
		READ1_DEF(length, res, this->ReadDocPart, pDocParts->m_arrItems.back());
		m_oDocumentWriter.m_oContent.WriteString(L"</w:docPart>");
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadDocPart(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CDocPart* pDocPart = static_cast<OOX::Logic::CDocPart*>(poResult);

	if (c_oSerGlossary::DocPartPr == type)
	{
		pDocPart->m_oDocPartPr = new OOX::Logic::CDocPartPr();
		READ1_DEF(length, res, this->ReadDocPartPr, pDocPart->m_oDocPartPr.GetPointer());
		
		m_oDocumentWriter.m_oContent.WriteString(pDocPart->m_oDocPartPr->toXML());
	}
	else if (c_oSerGlossary::DocPartBody == type)
	{
		m_oDocumentWriter.m_oContent.WriteString(L"<w:docPartBody>");
		READ1_DEF(length, res, this->ReadDocumentContent, this);
		m_oDocumentWriter.m_oContent.WriteString(L"</w:docPartBody>");
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int Binary_DocumentTableReader::ReadDocPartPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	
	OOX::Logic::CDocPartPr* pDocPartPr = static_cast<OOX::Logic::CDocPartPr*>(poResult);

	if (c_oSerGlossary::Name == type)
	{
		pDocPartPr->m_oName.Init();
		pDocPartPr->m_oName->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerGlossary::Style == type)
	{
		pDocPartPr->m_oStyle.Init();
		pDocPartPr->m_oStyle->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerGlossary::Guid == type)
	{
		pDocPartPr->m_oGuid.Init();
		pDocPartPr->m_oGuid->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerGlossary::Description == type)
	{
		pDocPartPr->m_oDescription.Init();
		pDocPartPr->m_oDescription->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerGlossary::CategoryName == type)
	{
		if (false == pDocPartPr->m_oCategory.IsInit()) pDocPartPr->m_oCategory.Init();
		pDocPartPr->m_oCategory->m_oName.Init();
		pDocPartPr->m_oCategory->m_oName->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerGlossary::CategoryGallery == type)
	{
		if (false == pDocPartPr->m_oCategory.IsInit()) pDocPartPr->m_oCategory.Init();
		pDocPartPr->m_oCategory->m_oGallery.Init();
		pDocPartPr->m_oCategory->m_oGallery->m_oVal.Init();
		pDocPartPr->m_oCategory->m_oGallery->m_oVal->SetValue((SimpleTypes::EDocPartGallery)m_oBufferedStream.GetUChar());
	}
	else if (c_oSerGlossary::Types == type)
	{
		pDocPartPr->m_oTypes = new OOX::Logic::CDocPartTypes();
		READ1_DEF(length, res, this->ReadDocPartTypes, pDocPartPr->m_oTypes.GetPointer());
	}
	else if (c_oSerGlossary::Behaviors == type)
	{
		pDocPartPr->m_oBehaviors = new OOX::Logic::CDocPartBehaviors();
		READ1_DEF(length, res, this->ReadDocPartBehaviors, pDocPartPr->m_oBehaviors.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadDocPartBehaviors(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CDocPartBehaviors* pDocPartBehaviors = static_cast<OOX::Logic::CDocPartBehaviors*>(poResult);

	if (c_oSerGlossary::Behavior == type)
	{
		ComplexTypes::Word::CDocPartBehavior *behavior = new ComplexTypes::Word::CDocPartBehavior();
		pDocPartBehaviors->m_arrItems.push_back(behavior);
		behavior->m_oVal.Init();
		behavior->m_oVal->SetValue((SimpleTypes::EDocPartBehavior)m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadDocPartTypes(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CDocPartTypes* pDocPartTypes = static_cast<OOX::Logic::CDocPartTypes*>(poResult);

	if (c_oSerGlossary::Type == type)
	{
		ComplexTypes::Word::String *type = new ComplexTypes::Word::String();
		pDocPartTypes->m_arrItems.push_back(type);
		type->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int Binary_DocumentTableReader::ReadParagraph(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadParagraphContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSerParType::Run == type )
	{
		m_oCur_rPr.Clear();
        GetRunStringWriter().WriteString(std::wstring(_T("<w:r>")));
		READ1_DEF(length, res, this->ReadRun, NULL);
        GetRunStringWriter().WriteString(std::wstring(_T("</w:r>")));
	}
	else if ( c_oSerParType::CommentStart == type )
	{
		long nId = 0;
		READ1_DEF(length, res, this->ReadComment, &nId);
		if(NULL != m_oFileWriter.m_pComments)
		{
			CComment* pComment = m_oFileWriter.m_pComments->get(nId);
			if(NULL != pComment)
			{
				int nNewId = m_oFileWriter.m_pComments->getNextId(pComment->getCount());
				pComment->setFormatStart(nNewId);
                GetRunStringWriter().WriteString(pComment->writeRef(std::wstring(_T("")), std::wstring(_T("w:commentRangeStart")), std::wstring(_T(""))));
			}
		}
	}
	else if ( c_oSerParType::CommentEnd == type )
	{
		long nId = 0;
		READ1_DEF(length, res, this->ReadComment, &nId);
		if(NULL != m_oFileWriter.m_pComments)
		{
			CComment* pComment = m_oFileWriter.m_pComments->get(nId);
			if(NULL != pComment && pComment->bIdFormat)
                GetRunStringWriter().WriteString(pComment->writeRef(std::wstring(_T("")), std::wstring(_T("w:commentRangeEnd")), std::wstring(_T(""))));
		}
	}
	else if ( c_oSerParType::OMathPara == type )
	{
        GetRunStringWriter().WriteString(std::wstring(_T("<m:oMathPara>")));
		READ1_DEF(length, res, this->ReadMathOMathPara, poResult);
        GetRunStringWriter().WriteString(std::wstring(_T("</m:oMathPara>")));
	}
	else if ( c_oSerParType::OMath == type )
	{
        GetRunStringWriter().WriteString(std::wstring(_T("<m:oMath>")));
		READ1_DEF(length, res, this->ReadMathArg, poResult);
        GetRunStringWriter().WriteString(std::wstring(_T("</m:oMath>")));
	}
	else if ( c_oSerParType::MRun == type )
	{
		GetRunStringWriter().WriteString(std::wstring(_T("<m:r>")));
		READ1_DEF(length, res, this->ReadMathMRun, poResult);
		GetRunStringWriter().WriteString(std::wstring(_T("</m:r>")));
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
		GetRunStringWriter().WriteString(oBookmarkStart.toXML());
	}
	else if ( c_oSerParType::BookmarkEnd == type )
	{
		OOX::Logic::CBookmarkEnd oBookmarkEnd;
		READ1_DEF(length, res, this->ReadBookmarkEnd, &oBookmarkEnd);
		GetRunStringWriter().WriteString(oBookmarkEnd.toXML());
	}
	else if (c_oSerParType::PermStart == type)
	{
		OOX::Logic::CPermStart oPerm;
		READ1_DEF(length, res, this->ReadPermStart, &oPerm);
		GetRunStringWriter().WriteString(oPerm.toXML());
	}
	else if (c_oSerParType::PermEnd == type)
	{
		OOX::Logic::CPermEnd oPerm;
		READ1_DEF(length, res, this->ReadPermEnd, &oPerm);
		GetRunStringWriter().WriteString(oPerm.toXML());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadDelIns(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
	READ1_TRACKREV(type, length, pTrackRevision)
	else if ( c_oSerProp_RevisionType::Content == type )
	{
		pTrackRevision->content = new NSStringUtils::CStringBuilder();
		NSStringUtils::CStringBuilder* pPrevWriter = m_pCurWriter;
		m_pCurWriter = pTrackRevision->content;
		READ1_DEF(length, res, this->ReadParagraphContent, NULL);
		m_pCurWriter = pPrevWriter;
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMoveFromRangeStart(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMoveToRangeStart(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMoveFromRangeEnd(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMoveToRangeEnd(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadFldChar(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CFldChar* pFldChar = static_cast<OOX::Logic::CFldChar*>(poResult);
	if ( c_oSer_FldSimpleType::CharType == type )
	{
		pFldChar->m_oFldCharType.Init();
		pFldChar->m_oFldCharType->SetValue((SimpleTypes::EFldCharType)m_oBufferedStream.GetUChar());
	}
	else if (c_oSer_FldSimpleType::FFData == type)
	{
		pFldChar->m_oFFData.Init();
		READ1_DEF(length, res, this->ReadFFData, pFldChar->m_oFFData.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadFldSimple(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	CFldSimple* pFldSimple = static_cast<CFldSimple*>(poResult);
	if ( c_oSer_FldSimpleType::Instr == type )
		pFldSimple->sInstr = m_oBufferedStream.GetString3(length);
	else if ( c_oSer_FldSimpleType::Content == type )
	{
		NSStringUtils::CStringBuilder* pPrevWriter = m_pCurWriter;
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
int Binary_DocumentTableReader::ReadFFData(BYTE type, long length, void* poResult)
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
		pFFData->m_oLabel->m_oVal = m_oBufferedStream.GetLong();
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
int Binary_DocumentTableReader::ReadFFCheckBox(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadDDList(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CFFDDList* pDDList = static_cast<OOX::Logic::CFFDDList*>(poResult);
	if ( c_oSerFFData::DLDefault == type )
	{
		pDDList->m_oDefault.Init();
		pDDList->m_oDefault->m_oVal = m_oBufferedStream.GetLong();
	}
	else if ( c_oSerFFData::DLResult == type )
	{
		pDDList->m_oResult.Init();
		pDDList->m_oResult->m_oVal = m_oBufferedStream.GetLong();
	}
	else if ( c_oSerFFData::DLListEntry == type )
	{
		ComplexTypes::Word::String* pVal = new ComplexTypes::Word::String();
		pVal->m_sVal = m_oBufferedStream.GetString3(length);
		
		pDDList->m_arrListEntry.push_back(pVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadFFHelpText(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadFFStatusText(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadTextInput(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CFFTextInput* pTextInput = static_cast<OOX::Logic::CFFTextInput*>(poResult);
	if ( c_oSerFFData::TIDefault == type )
	{
		pTextInput->m_oDefault.Init();
		pTextInput->m_oDefault->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSerFFData::TIFormat == type )
	{
		pTextInput->m_oFormat.Init();
		pTextInput->m_oFormat->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSerFFData::TIMaxLength == type )
	{
		pTextInput->m_oMaxLength.Init();
		pTextInput->m_oMaxLength->m_oVal = m_oBufferedStream.GetLong();
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
int Binary_DocumentTableReader::ReadBookmarkStart(BYTE type, long length, void* poResult)
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
		pBookmarkStart->m_oDisplacedByCustomXml->SetValueFromByte(m_oBufferedStream.GetUChar());
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
int Binary_DocumentTableReader::ReadBookmarkEnd(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadPermStart(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CPermStart* pPerm = static_cast<OOX::Logic::CPermStart*>(poResult);
	if (c_oSerPermission::Id == type)
	{
		pPerm->m_sId.Init();
		*pPerm->m_sId = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerPermission::DisplacedByCustomXml == type)
	{
		pPerm->m_oDisplacedByCustomXml.Init();
		pPerm->m_oDisplacedByCustomXml->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else if (c_oSerPermission::ColFirst == type)
	{
		pPerm->m_oColFirst.Init();
		pPerm->m_oColFirst->SetValue(m_oBufferedStream.GetLong());
	}
	else if (c_oSerPermission::ColLast == type)
	{
		pPerm->m_oColLast.Init();
		pPerm->m_oColLast->SetValue(m_oBufferedStream.GetLong());
	}
	else if (c_oSerPermission::Ed == type)
	{
		pPerm->m_sId.Init();
		*pPerm->m_sId = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerPermission::EdGroup == type)
	{
		pPerm->m_oEdGrp.Init();
		pPerm->m_oEdGrp->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	
	return res;
}
int Binary_DocumentTableReader::ReadPermEnd(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Logic::CPermEnd* pPerm = static_cast<OOX::Logic::CPermEnd*>(poResult);
	if (c_oSerPermission::Id == type)
	{
		pPerm->m_sId.Init();
		*pPerm->m_sId = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerPermission::DisplacedByCustomXml == type)
	{
		pPerm->m_oDisplacedByCustomXml.Init();
		pPerm->m_oDisplacedByCustomXml->SetValueFromByte(m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadHyperlink(BYTE type, long length, void* poResult)
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
		NSStringUtils::CStringBuilder* pPrevWriter = m_pCurWriter;
		m_pCurWriter = &pHyperlink->writer;
		READ1_DEF(length, res, this->ReadParagraphContent, NULL);
		if (!pHyperlink->sLink.empty())
		{
			unsigned int rId;
			std::wstring sHref = XmlUtils::EncodeXmlString(pHyperlink->sLink);
			m_oFileWriter.m_pDrawingConverter->WriteRels(std::wstring(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink")), sHref, std::wstring(_T("External")), &rId);
			pHyperlink->rId = L"rId" + std::to_wstring(rId);
		}
		m_pCurWriter = pPrevWriter;
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathLong(BYTE type, long length, void* poResult)
{		
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
		LONG lVal = m_oBufferedStream.GetLong();
	else
		res = c_oSerConstants::ReadUnknown;

	return res;
}
int Binary_DocumentTableReader::ReadMathArgOut(long length)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadMathArg, NULL);
	return res;
}
int Binary_DocumentTableReader::ReadMathArg(BYTE type, long length, void* poResult)
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
	else if ( c_oSer_OMathContentType::Del == type )
	{
		TrackRevision oTrackRevision;
		READ1_DEF(length, res, this->ReadDelIns, &oTrackRevision);
		oTrackRevision.Write(&GetRunStringWriter(), _T("w:del"));
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
	else if ( c_oSer_OMathContentType::Ins == type )
	{
		TrackRevision oTrackRevision;
		READ1_DEF(length, res, this->ReadDelIns, &oTrackRevision);
		oTrackRevision.Write(&GetRunStringWriter(), _T("w:ins"));
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
	else if ( c_oSer_OMathContentType::MoveFromRangeStart == type )
	{
		OOX::Logic::CMoveFromRangeStart oMoveFromRangeStart;
		READ1_DEF(length, res, this->ReadMoveFromRangeStart, &oMoveFromRangeStart);
		GetRunStringWriter().WriteString(oMoveFromRangeStart.toXML());
	}
	else if ( c_oSer_OMathContentType::MoveFromRangeEnd == type )
	{
		OOX::Logic::CMoveFromRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveFromRangeEnd, &oMoveToRangeEnd);
		GetRunStringWriter().WriteString(oMoveToRangeEnd.toXML());
	}
	else if ( c_oSer_OMathContentType::MoveToRangeStart == type )
	{
		OOX::Logic::CMoveToRangeStart oMoveToRangeStart;
		READ1_DEF(length, res, this->ReadMoveToRangeStart, &oMoveToRangeStart);
		GetRunStringWriter().WriteString(oMoveToRangeStart.toXML());
	}
	else if ( c_oSer_OMathContentType::MoveToRangeEnd == type )
	{
		OOX::Logic::CMoveToRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveToRangeEnd, &oMoveToRangeEnd);
		GetRunStringWriter().WriteString(oMoveToRangeEnd.toXML());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathAcc(BYTE type, long length, void* poResult)
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

int Binary_DocumentTableReader::ReadMathAccPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathAln(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:aln");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathAlnScr(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:alnScr");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathArgPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathArgSz(BYTE type, long length, void* poResult)//GetLong
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
int Binary_DocumentTableReader::ReadMathBar(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathBarPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathBaseJc(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathBegChr(BYTE type, long length, void* poResult) //ReadString
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
int Binary_DocumentTableReader::ReadMathBorderBox(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathBorderBoxPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathBox(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathBoxPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathBrk(BYTE type, long length, void* poResult)//GetLong
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
int Binary_DocumentTableReader::ReadMathCGp(BYTE type, long length, void* poResult)//GetLong
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

int Binary_DocumentTableReader::ReadMathCGpRule(BYTE type, long length, void* poResult)//GetUChar
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

int Binary_DocumentTableReader::ReadMathChr(BYTE type, long length, void* poResult)//ReadString
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
int Binary_DocumentTableReader::ReadMathCount(BYTE type, long length, void* poResult)//GetLong
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
int Binary_DocumentTableReader::ReadMathCSp(BYTE type, long length, void* poResult)//GetLong
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
int Binary_DocumentTableReader::ReadMathCtrlPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSerRunType::rPr == type )
	{
		m_oMath_rPr.Clear();
		res = oBinary_rPrReader.Read(length, &m_oMath_rPr);
		
		if(m_oMath_rPr.IsNoEmpty())		
			GetRunStringWriter().WriteString(m_oMath_rPr.toXML());
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
		ComplexTypes::Word::CTrackChange del;
		
		READ1_DEF(length, res, this->ReadMathCtrlPrDelIns, &del);
		GetRunStringWriter().WriteString(del.ValNode(L"w:del"));
	}
	else if ( c_oSerRunType::ins == type )
	{
		ComplexTypes::Word::CTrackChange ins;

		READ1_DEF(length, res, this->ReadMathCtrlPrDelIns, &ins);
		GetRunStringWriter().WriteString(ins.ValNode(L"w:ins"));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathCtrlPrDelIns(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	ComplexTypes::Word::CTrackChange* trackChange = static_cast<ComplexTypes::Word::CTrackChange*>(poResult);

	READ1_TRACKREV_2(type, length, trackChange)
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int Binary_DocumentTableReader::ReadMathDelimiter(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathDegHide(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		//word writes "1"(excel, powerpoint write "on"). "on" is correct for word, libre, onedrive, gdrive
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:degHide");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathDiff(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:diff");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_DocumentTableReader::ReadMathDelimiterPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathEndChr(BYTE type, long length, void* poResult)//ReadString
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
int Binary_DocumentTableReader::ReadMathEqArr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathEqArrPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathFraction(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathFPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathFunc(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathFuncPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathGroupChr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathGroupChrPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathGrow(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:grow");
		if (!bVal)
			sVal += _T(" m:val=\"off\" />");
		else
			sVal += _T(" m:val=\"on\" />");

		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathHideBot(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:hideBot");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathHideLeft(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:hideLeft");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathHideRight(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:hideRight");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathHideTop(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:hideTop");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}		
int Binary_DocumentTableReader::ReadMathMJc(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathLimLoc(BYTE type, long length, void* poResult)//GetUChar
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

int Binary_DocumentTableReader::ReadMathLimLow(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathLimLowPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathLimUpp(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathLimUppPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathLit(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:lit");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}	
int Binary_DocumentTableReader::ReadMathMatrix(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathMaxDist(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:maxDist");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathMc(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathMcJc(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathMcPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathMcs(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathMPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathMr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathNary(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathNaryPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathNoBreak(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:noBreak");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathNor(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:nor");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathObjDist(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:objDist");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathOMathParaOut(long length)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadMathOMathPara, NULL);
	return res;
}
int Binary_DocumentTableReader::ReadMathOMathPara(BYTE type, long length, void* poResult)
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
		m_oCur_rPr.Clear();
        GetRunStringWriter().WriteString(std::wstring(_T("<w:r>")));
		READ1_DEF(length, res, this->ReadRun, NULL);
        GetRunStringWriter().WriteString(std::wstring(_T("</w:r>")));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathOMathParaPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathOpEmu(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:opEmu");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathPhant(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathPhantPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathPlcHide(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:plcHide");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathPos(BYTE type, long length, void* poResult)
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
std::wstring Binary_DocumentTableReader::GetMathText (long length)
{
    std::wstring strVal(m_oBufferedStream.GetString3(length));
	return XmlUtils::EncodeXmlString(strVal, true);
}
int Binary_DocumentTableReader::ReadMathText(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathMRun(BYTE type, long length, void* poResult)
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
		m_oMath_rPr.Clear();
		res = oBinary_rPrReader.Read(length, &m_oMath_rPr);
		
		if (m_oMath_rPr.IsNoEmpty())
			GetRunStringWriter().WriteString(m_oMath_rPr.toXML());
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
int Binary_DocumentTableReader::ReadMathInsDel(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
	READ1_TRACKREV(type, length, pTrackRevision)
	else if(c_oSerProp_RevisionType::ContentRun == type)
	{
		pTrackRevision->contentRun = new NSStringUtils::CStringBuilder();
		NSStringUtils::CStringBuilder* pPrevWriter = m_pCurWriter;
		m_pCurWriter = pTrackRevision->contentRun;
		READ1_DEF(length, res, this->ReadMathMRun, NULL);
		m_pCurWriter = pPrevWriter;
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int Binary_DocumentTableReader::ReadMathRad(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathRadPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathMRPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathRSp(BYTE type, long length, void* poResult)//GetLong
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
int Binary_DocumentTableReader::ReadMathRSpRule(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathScr(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathSepChr(BYTE type, long length, void* poResult)//ReadString
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
int Binary_DocumentTableReader::ReadMathShow(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:show");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathShp(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathSPre(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathSPrePr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathSSub(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathSSubPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathSSubSup(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathSSubSupPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathSSup(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathSSupPr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadMathStrikeBLTR(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:strikeBLTR");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathStrikeH(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:strikeH");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathStrikeTLBR(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:strikeTLBR");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathStrikeV(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:strikeV");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathSty(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathSubHide(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:subHide");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathSupHide(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:supHide");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathTransp(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:transp");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathType(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathVertJc(BYTE type, long length, void* poResult)//GetUChar
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
int Binary_DocumentTableReader::ReadMathZeroAsc(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:zeroAsc");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathZeroDesc(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:zaroDesc");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadMathZeroWid(BYTE type, long length, void* poResult)//GetBool
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSer_OMathBottomNodesValType::Val == type )
	{
		bool bVal = m_oBufferedStream.GetBool();
        std::wstring sVal = _T("<m:zeroWid");
		if (bVal)
			sVal += _T(" m:val=\"on\" />");
		else
			sVal += _T(" m:val=\"off\" />");
		GetRunStringWriter().WriteString(sVal);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadRun(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if ( c_oSerRunType::rPr == type )
	{
		res = oBinary_rPrReader.Read(length, &m_oCur_rPr);
		
		if (m_oCur_rPr.IsNoEmpty())
			GetRunStringWriter().WriteString(m_oCur_rPr.toXML());
	}
	else if ( c_oSerRunType::Content == type )
	{
		READ1_DEF(length, res, this->ReadRunContent, NULL);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadRunContent(BYTE type, long length, void* poResult)
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
		if (m_oCur_rPr.IsNoEmpty())
			GetRunStringWriter().WriteString(m_oCur_rPr.toXML());
        GetRunStringWriter().WriteString(std::wstring(_T("<w:instrText xml:space=\"preserve\">PAGE \\* MERGEFORMAT</w:instrText></w:r><w:r>")));
		if(m_oCur_rPr.IsNoEmpty())
			GetRunStringWriter().WriteString(m_oCur_rPr.toXML());
        GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"separate\"/></w:r><w:r>")));
		if(m_oCur_rPr.IsNoEmpty())
			GetRunStringWriter().WriteString(m_oCur_rPr.toXML());
		GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"end\"/>")));
	}
	else if (c_oSerRunType::pagebreak == type)
	{
		GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"page\" w:clear=\"all\"/>")));
	}
	else if (c_oSerRunType::linebreak == type)
	{
		GetRunStringWriter().WriteString(std::wstring(_T("<w:br/>")));
	}
	else if (c_oSerRunType::linebreakClearAll == type)
	{
		GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"textWrapping\" w:clear=\"all\"/>")));
	}
	else if (c_oSerRunType::linebreakClearLeft == type)
	{
		GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"textWrapping\" w:clear=\"left\"/>")));
	}
	else if (c_oSerRunType::linebreakClearRight == type)
	{
		GetRunStringWriter().WriteString(std::wstring(_T("<w:br w:type=\"textWrapping\" w:clear=\"right\"/>")));
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
			unsigned int rId;
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

		if(oCDrawingProperty.IsGraphicFrameContent())
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
		//todo
        m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"</w:p>"));
        m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"<w:tbl>"));
		READ1_DEF(length, res, this->ReadDocTable, &m_oDocumentWriter.m_oContent);
        m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"</w:tbl>"));

		if (m_bUsedParaIdCounter && m_oFileWriter.m_pComments)
		{
			_INT32 nId = m_oFileWriter.m_pComments->m_oParaIdCounter.getNextId();
			std::wstring sParaId = XmlUtils::ToString(nId, L"%08X");

			m_oDocumentWriter.m_oContent.WriteString(L"<w:p w14:paraId=\"" + sParaId + L"\" w14:textId=\"" + sParaId + L"\">");
		}
		else
		{
			m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"<w:p>"));
		}
        if(m_oCur_pPr.GetCurSize() > 0)
		{
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"<w:pPr>"));
			m_oDocumentWriter.m_oContent.Write(m_oCur_pPr);
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(L"</w:pPr>"));
		}
	}
	else if(c_oSerRunType::fldstart_deprecated == type)
	{
        std::wstring sField(m_oBufferedStream.GetString3(length));
		sField = XmlUtils::EncodeXmlString(sField);
        GetRunStringWriter().WriteString(std::wstring(_T("<w:fldChar w:fldCharType=\"begin\"/></w:r><w:r>")));
		if(m_oCur_rPr.IsNoEmpty())
			GetRunStringWriter().WriteString(m_oCur_rPr.toXML());
		GetRunStringWriter().WriteString(std::wstring(_T("<w:instrText xml:space=\"preserve\">")));
		GetRunStringWriter().WriteString(sField);
        GetRunStringWriter().WriteString(std::wstring(_T("</w:instrText></w:r><w:r>")));
		if(m_oCur_rPr.IsNoEmpty())
			GetRunStringWriter().WriteString(m_oCur_rPr.toXML());
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
		if(NULL != m_oFileWriter.m_pComments)
		{
			CComment* pComment = m_oFileWriter.m_pComments->get(nId);
			if(NULL != pComment) // могут быть и без start/end
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
int Binary_DocumentTableReader::ReadFootnoteRef(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadEndnoteRef(BYTE type, long length, void* poResult)
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
void Binary_DocumentTableReader::ReadDrawing(CDrawingProperty &oCDrawingProperty)
{
    std::wstring sDrawingProperty = oCDrawingProperty.Write();
    if(false == sDrawingProperty.empty())
	{
		long nCurPos = m_oBufferedStream.GetPos();
        std::wstring sDrawingXml;

		int nDocType = m_oFileWriter.m_bGlossaryMode ? XMLWRITER_DOC_TYPE_DOCX_GLOSSARY : XMLWRITER_DOC_TYPE_DOCX;

        m_oFileWriter.m_pDrawingConverter->SaveObjectEx(oCDrawingProperty.DataPos, oCDrawingProperty.DataLength, sDrawingProperty, nDocType, sDrawingXml);
		m_oBufferedStream.Seek(nCurPos);

        if( false == sDrawingXml.empty())
		{
            GetRunStringWriter().WriteString(sDrawingXml);
		}
	}
}
int Binary_DocumentTableReader::ReadObject(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadComment(BYTE type, long length, void* poResult)
{
	long* pVal = static_cast<long*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if (c_oSer_CommentsType::Id == type)
		*pVal = m_oBufferedStream.GetLong();
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadDocTable(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
int Binary_DocumentTableReader::Read_tblGrid(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
	if( c_oSerDocTableType::tblGrid_Item == type )
	{
		double dgridCol = m_oBufferedStream.GetDouble();
		oBinary_tblPrReader.m_aCurTblGrid.push_back(dgridCol);
		long ngridCol = SerializeCommon::Round( g_dKoef_mm_to_twips * dgridCol);

        pCStringWriter->WriteString(L"<w:gridCol w:w=\"" + std::to_wstring(ngridCol)+ L"\"/>");
	}
	else if( c_oSerDocTableType::tblGrid_ItemTwips == type )
	{
		int ngridCol = m_oBufferedStream.GetLong();

		if (ngridCol > 0)
			pCStringWriter->WriteString(L"<w:gridCol w:w=\"" + std::to_wstring(ngridCol)+ L"\"/>");
		else
			pCStringWriter->WriteString(L"<w:gridCol/>");
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
int Binary_DocumentTableReader::Read_tblGridChange(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	TrackRevision* pTrackRevision = static_cast<TrackRevision*>(poResult);
	if(c_oSerProp_RevisionType::Id == type)
	{
		pTrackRevision->Id = new _INT32(m_oBufferedStream.GetLong());
	}
	else if(c_oSerProp_RevisionType::tblGridChange == type)
	{
		oBinary_tblPrReader.m_aCurTblGrid.clear();
		pTrackRevision->tblGridChange = new NSStringUtils::CStringBuilder();
		READ2_DEF(length, res, this->Read_tblGrid, pTrackRevision->tblGridChange);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int Binary_DocumentTableReader::Read_TableContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
	else if ( c_oSerDocTableType::MoveFromRangeStart == type )
	{
		OOX::Logic::CMoveFromRangeStart oMoveFromRangeStart;
		READ1_DEF(length, res, this->ReadMoveFromRangeStart, &oMoveFromRangeStart);
		pCStringWriter->WriteString(oMoveFromRangeStart.toXML());
	}
	else if ( c_oSerDocTableType::MoveFromRangeEnd == type )
	{
		OOX::Logic::CMoveFromRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveFromRangeEnd, &oMoveToRangeEnd);
		pCStringWriter->WriteString(oMoveToRangeEnd.toXML());
	}
	else if ( c_oSerDocTableType::MoveToRangeStart == type )
	{
		OOX::Logic::CMoveToRangeStart oMoveToRangeStart;
		READ1_DEF(length, res, this->ReadMoveToRangeStart, &oMoveToRangeStart);
		pCStringWriter->WriteString(oMoveToRangeStart.toXML());
	}
	else if ( c_oSerDocTableType::MoveToRangeEnd == type )
	{
		OOX::Logic::CMoveToRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveToRangeEnd, &oMoveToRangeEnd);
		pCStringWriter->WriteString(oMoveToRangeEnd.toXML());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int Binary_DocumentTableReader::Read_Row(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
int Binary_DocumentTableReader::ReadRowContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
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
	else if ( c_oSerDocTableType::MoveFromRangeStart == type )
	{
		OOX::Logic::CMoveFromRangeStart oMoveFromRangeStart;
		READ1_DEF(length, res, this->ReadMoveFromRangeStart, &oMoveFromRangeStart);
		pCStringWriter->WriteString(oMoveFromRangeStart.toXML());
	}
	else if ( c_oSerDocTableType::MoveFromRangeEnd == type )
	{
		OOX::Logic::CMoveFromRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveFromRangeEnd, &oMoveToRangeEnd);
		pCStringWriter->WriteString(oMoveToRangeEnd.toXML());
	}
	else if ( c_oSerDocTableType::MoveToRangeStart == type )
	{
		OOX::Logic::CMoveToRangeStart oMoveToRangeStart;
		READ1_DEF(length, res, this->ReadMoveToRangeStart, &oMoveToRangeStart);
		pCStringWriter->WriteString(oMoveToRangeStart.toXML());
	}
	else if ( c_oSerDocTableType::MoveToRangeEnd == type )
	{
		OOX::Logic::CMoveToRangeEnd oMoveToRangeEnd;
		READ1_DEF(length, res, this->ReadMoveToRangeEnd, &oMoveToRangeEnd);
		pCStringWriter->WriteString(oMoveToRangeEnd.toXML());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadCell(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	NSStringUtils::CStringBuilder* pCStringWriter = static_cast<NSStringUtils::CStringBuilder*>(poResult);
	if( c_oSerDocTableType::Cell_Pr == type )
	{
        pCStringWriter->WriteString(std::wstring(_T("<w:tcPr>")));
		READ2_DEF(length, res, oBinary_tblPrReader.Read_CellPr, pCStringWriter);
        pCStringWriter->WriteString(std::wstring(_T("</w:tcPr>")));
	}
	else if( c_oSerDocTableType::Cell_Content == type )
	{
		Binary_DocumentTableReader oBinary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, m_oDocumentWriter, m_bOFormRead);
		READ1_DEF(length, res, this->ReadCellContent, &oBinary_DocumentTableReader);
		//Потому что если перед </tc> не идет <p>, то документ считается невалидным
		if(c_oSerParType::Par != oBinary_DocumentTableReader.m_byteLastElemType)
		{
            m_oDocumentWriter.m_oContent.WriteString(std::wstring(_T("<w:p/>")));
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadCellContent(BYTE type, long length, void* poResult)
{
	Binary_DocumentTableReader* pBinary_DocumentTableReader = static_cast<Binary_DocumentTableReader*>(poResult);
	return pBinary_DocumentTableReader->ReadDocumentContent(type, length, NULL);
}
int Binary_DocumentTableReader::ReadImage(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::Read_Background(BYTE type, long length, void* poResult)
{
	m_oFileWriter.m_pCurrentSettings->m_oDisplayBackgroundShape.Init();

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
			m_oFileWriter.m_pDrawingConverter->m_pReader->m_nDocumentType = m_oFileWriter.m_bGlossaryMode ? XMLWRITER_DOC_TYPE_DOCX_GLOSSARY : XMLWRITER_DOC_TYPE_DOCX;
		
			long nCurPos = m_oBufferedStream.GetPos();
			pBackground->sObject = m_oFileWriter.m_pDrawingConverter->SaveObjectBackground(oCDrawingProperty.DataPos, oCDrawingProperty.DataLength);
			m_oBufferedStream.Seek(nCurPos);
		}
    }
    else
        res = c_oSerConstants::ReadUnknown;
    return res;
}

int Binary_DocumentTableReader::ReadPptxDrawing(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
	if (c_oSerImageType2::Type == type)
	{
		pDrawingProperty->bType = true;
		pDrawingProperty->Type = m_oBufferedStream.GetUChar();
	}
	else if (c_oSerImageType2::PptxData == type)
	{
		pDrawingProperty->bDataPos = true;
		pDrawingProperty->bDataLength = true;
		pDrawingProperty->DataPos = m_oBufferedStream.GetPos();
		pDrawingProperty->DataLength = length;
		//сейчас пропуская, потому что перед чтение этого поля надо собрать остальные данные
		res = c_oSerConstants::ReadUnknown;
	}
	else if (c_oSerImageType2::Chart == type)
	{
		OOX::CPath pathCharts = m_oFileWriter.m_oChartWriter.m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"charts";
		OOX::CSystemUtility::CreateDirectories(pathCharts.GetPath());
		
		m_oBufferedStream.m_pRels->m_pManager->SetDstCharts(pathCharts.GetPath());

		m_oBufferedStream.Seek(m_oBufferedStream.GetPos() - 4); //roll back length

		PPTX::Logic::GraphicFrame graphicFrame; 

		graphicFrame.chartRec.Init();
		graphicFrame.chartRec->fromPPTY(&m_oBufferedStream);

		if (graphicFrame.chartRec->id_data.IsInit())
		{
			pDrawingProperty->sGraphicFrameContent = graphicFrame.toXML2();
		}
	}
	else if (c_oSerImageType2::ChartEx == type)
	{
		OOX::CPath pathCharts = m_oFileWriter.m_oChartWriter.m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"charts";
		OOX::CSystemUtility::CreateDirectories(pathCharts.GetPath());

		m_oBufferedStream.m_pRels->m_pManager->SetDstCharts(pathCharts.GetPath());

		m_oBufferedStream.Seek(m_oBufferedStream.GetPos() - 4); //roll back length

		PPTX::Logic::GraphicFrame graphicFrame;

		graphicFrame.chartRec.Init();
		graphicFrame.chartRec->m_bChartEx = true;
		graphicFrame.chartRec->fromPPTY(&m_oBufferedStream);

		if (graphicFrame.chartRec->id_data.IsInit())
		{
			pDrawingProperty->sGraphicFrameContent = graphicFrame.toXML2();
		}
	}
	else if ( c_oSerImageType2::BehindDoc == type )
	{
		pDrawingProperty->bBehindDoc = true;
		pDrawingProperty->BehindDoc = m_oBufferedStream.GetBool();
	}
	else if ( c_oSerImageType2::DistL == type )
	{
		pDrawingProperty->bDistL = true;
		pDrawingProperty->DistL = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerImageType2::DistLEmu == type )
	{
		pDrawingProperty->bDistL = true;
		pDrawingProperty->DistL = (__int64)m_oBufferedStream.GetULong();
	}
	else if ( c_oSerImageType2::DistT == type )
	{
		pDrawingProperty->bDistT = true;
		pDrawingProperty->DistT = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerImageType2::DistTEmu == type )
	{
		pDrawingProperty->bDistT = true;
		pDrawingProperty->DistT = (__int64)m_oBufferedStream.GetULong();
	}
	else if ( c_oSerImageType2::DistR == type )
	{
		pDrawingProperty->bDistR = true;
		pDrawingProperty->DistR = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerImageType2::DistREmu == type )
	{
		pDrawingProperty->bDistR = true;
		pDrawingProperty->DistR = (__int64)m_oBufferedStream.GetULong();
	}
	else if ( c_oSerImageType2::DistB == type )
	{
		pDrawingProperty->bDistB = true;
		pDrawingProperty->DistB = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerImageType2::DistBEmu == type )
	{
		pDrawingProperty->bDistB = true;
		pDrawingProperty->DistB = (__int64)m_oBufferedStream.GetULong();
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
int Binary_DocumentTableReader::ReadEmbedded(BYTE type, long length, void* poResult)
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
			if (true == NSDirectory::CreateDirectory(strDstEmbeddedTemp))
			{
				std::wstring strDstEmbeddedTempXl = strDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl";
				NSDirectory::CreateDirectory(strDstEmbeddedTempXl);

				std::wstring strDstEmbeddedTempThemePath = strDstEmbeddedTempXl + FILE_SEPARATOR_STR + L"theme";
				std::wstring strDstEmbeddedTempDrawingPath = strDstEmbeddedTempXl + FILE_SEPARATOR_STR + L"drawings";
				std::wstring strDstEmbeddedTempEmbeddingsPath = strDstEmbeddedTempXl + FILE_SEPARATOR_STR + L"embeddings";

				int id = m_oFileWriter.m_oChartWriter.nEmbeddedCount++;

				BinXlsxRW::SaveParams oSaveParams(strDstEmbeddedTempDrawingPath, strDstEmbeddedTempEmbeddingsPath, strDstEmbeddedTempThemePath, m_oFileWriter.m_pDrawingConverter->GetContentTypes(), NULL, true);

				OOX::Spreadsheet::CXlsx oXlsx;

				BinXlsxRW::BinaryFileReader embeddedReader;
				embeddedReader.ReadMainTable(oXlsx, m_oBufferedStream, L"", strDstEmbeddedTemp, oSaveParams, m_oFileWriter.m_pDrawingConverter);

				oXlsx.PrepareToWrite();

				oXlsx.Write(strDstEmbeddedTemp, *oSaveParams.pContentTypes);

				std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring(id + 1) + (oSaveParams.bMacroEnabled ? L".xlsm" : L".xlsx");
				COfficeUtils oOfficeUtils(NULL);
				oOfficeUtils.CompressFileOrDirectory(strDstEmbeddedTemp, strDstEmbedded + FILE_SEPARATOR_STR + sXlsxFilename, true);

				std::wstring sEmbWorksheetRelsName = L"embeddings/" + sXlsxFilename;
				std::wstring sEmbWorksheetRelType = oSaveParams.bMacroEnabled ? OOX::FileTypes::MicrosoftOfficeExcelMacro_EnabledWorksheet.RelationType() :
																				OOX::FileTypes::MicrosoftOfficeExcelWorksheet.RelationType();
				
				m_oFileWriter.m_pDrawingConverter->WriteRels(sEmbWorksheetRelType, sEmbWorksheetRelsName, std::wstring(), &pDrawingProperty->nObjectId);

				NSDirectory::DeleteDirectory(strDstEmbeddedTemp);
			}
		}
		m_oBufferedStream.Seek( pos + length); 
	}		
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadCNvGraphicFramePr(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadDocPr(BYTE type, long length, void* poResult)
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
	else if (c_oSerDocPr::Form == type)
	{
		pNonVisualDrawingProps->form = m_oBufferedStream.GetBool();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadEffectExtent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
	if ( c_oSerEffectExtent::Left == type )
	{
		pDrawingProperty->bEffectExtentL = true;
		pDrawingProperty->EffectExtentL = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerEffectExtent::LeftEmu == type )
	{
		pDrawingProperty->bEffectExtentL = true;
		pDrawingProperty->EffectExtentL = (__int64)m_oBufferedStream.GetLong();
	}
	else if ( c_oSerEffectExtent::Top == type )
	{
		pDrawingProperty->bEffectExtentT = true;
		pDrawingProperty->EffectExtentT = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerEffectExtent::TopEmu == type )
	{
		pDrawingProperty->bEffectExtentT = true;
		pDrawingProperty->EffectExtentT = (__int64)m_oBufferedStream.GetLong();
	}
	else if ( c_oSerEffectExtent::Right == type )
	{
		pDrawingProperty->bEffectExtentR = true;
		pDrawingProperty->EffectExtentR = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerEffectExtent::RightEmu == type )
	{
		pDrawingProperty->bEffectExtentR = true;
		pDrawingProperty->EffectExtentR = (__int64)m_oBufferedStream.GetLong();
	}
	else if ( c_oSerEffectExtent::Bottom == type )
	{
		pDrawingProperty->bEffectExtentB = true;
		pDrawingProperty->EffectExtentB = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerEffectExtent::BottomEmu == type )
	{
		pDrawingProperty->bEffectExtentB = true;
		pDrawingProperty->EffectExtentB = (__int64)m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadExtent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
	if ( c_oSerExtent::Cx == type )
	{
		pDrawingProperty->bWidth = true;
		pDrawingProperty->Width = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerExtent::CxEmu == type )
	{
		pDrawingProperty->bWidth = true;
		pDrawingProperty->Width = (__int64)m_oBufferedStream.GetULong();
	}
	else if ( c_oSerExtent::Cy == type )
	{
		pDrawingProperty->bHeight = true;
		pDrawingProperty->Height = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerExtent::CyEmu == type )
	{
		pDrawingProperty->bHeight = true;
		pDrawingProperty->Height = (__int64)m_oBufferedStream.GetULong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadPositionH(BYTE type, long length, void* poResult)
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
		pDrawingProperty->PositionHPosOffset = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerPosHV::PosOffsetEmu == type )
	{
		pDrawingProperty->bPositionHPosOffset = true;
		pDrawingProperty->PositionHPosOffset = (__int64)m_oBufferedStream.GetLong();
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
int Binary_DocumentTableReader::ReadPositionV(BYTE type, long length, void* poResult)
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
		pDrawingProperty->PositionVPosOffset = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerPosHV::PosOffsetEmu == type )
	{
		pDrawingProperty->bPositionVPosOffset = true;
		pDrawingProperty->PositionVPosOffset = (__int64)m_oBufferedStream.GetLong();
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
int Binary_DocumentTableReader::ReadSimplePos(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	CDrawingProperty* pDrawingProperty = static_cast<CDrawingProperty*>(poResult);
	if ( c_oSerSimplePos::X == type )
	{
		pDrawingProperty->bSimplePosX = true;
		pDrawingProperty->SimplePosX = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerSimplePos::XEmu == type )
	{
		pDrawingProperty->bSimplePosX = true;
		pDrawingProperty->SimplePosX = (__int64)m_oBufferedStream.GetLong();
	}
	else if ( c_oSerSimplePos::Y == type )
	{
		pDrawingProperty->bSimplePosY = true;
		pDrawingProperty->SimplePosY = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerSimplePos::YEmu == type )
	{
		pDrawingProperty->bSimplePosY = true;
		pDrawingProperty->SimplePosY = (__int64)m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSizeRelH(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadSizeRelV(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadWrapThroughTight(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadWrapPolygon(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadLineTo(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadPolygonPoint(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	CDrawingPropertyWrapPoint* pWrapPoint = static_cast<CDrawingPropertyWrapPoint*>(poResult);
	if ( c_oSerPoint2D::X == type )
	{
		pWrapPoint->bX = true;
		pWrapPoint->X = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerPoint2D::XEmu == type )
	{
		pWrapPoint->bX = true;
		pWrapPoint->X = (__int64)m_oBufferedStream.GetLong();
	}
	else if ( c_oSerPoint2D::Y == type )
	{
		pWrapPoint->bY = true;
		pWrapPoint->Y = (__int64)(g_dKoef_mm_to_emu * m_oBufferedStream.GetDouble());
	}
	else if ( c_oSerPoint2D::YEmu == type )
	{
		pWrapPoint->bY = true;
		pWrapPoint->Y = (__int64)m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadEmptyWrap(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::Read_SecPr(BYTE type, long length, void* poResult)
{
	return oBinary_pPrReader.Read_SecPr(type, length, poResult);
}
int Binary_DocumentTableReader::ReadSdt(BYTE type, long length, void* poResult)
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
		pSdt->m_oEndPr.Init();
		res = oBinary_rPrReader.Read(length, pSdt->m_oEndPr.GetPointer());
	}
	else if (c_oSerSdt::Content == type)
	{
		m_oDocumentWriter.m_oContent.WriteString(L"<w:sdt>");
		if ( pSdt->m_oSdt.m_oSdtPr.IsInit() )
		{
			m_oDocumentWriter.m_oContent.WriteString(pSdt->m_oSdt.m_oSdtPr->toXMLStart());
			if (pSdt->m_oRPr.IsInit())
			{
				m_oDocumentWriter.m_oContent.WriteString(pSdt->m_oRPr->toXML());
			}
			m_oDocumentWriter.m_oContent.WriteString(pSdt->m_oSdt.m_oSdtPr->toXMLEnd());
		}
		if (pSdt->m_oEndPr.IsInit())
		{
			m_oDocumentWriter.m_oContent.WriteString(L"<w:sdtEndPr>");
			m_oDocumentWriter.m_oContent.WriteString(pSdt->m_oEndPr->toXML());
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
int Binary_DocumentTableReader::ReadSdtPr(BYTE type, long length, void* poResult)
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
		pSdtPr->m_oAlias->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::Appearance == type)
	{
		pSdtPr->m_oAppearance.Init();
		pSdtPr->m_oAppearance->m_oVal.Init();
		pSdtPr->m_oAppearance->m_oVal->SetValue((SimpleTypes::ESdtAppearance)m_oBufferedStream.GetUChar());
	}
	else if (c_oSerSdt::ComboBox == type)
	{
		pSdtPr->m_oComboBox.Init();
		READ1_DEF(length, res, this->ReadSdtComboBox, pSdtPr->m_oComboBox.GetPointer());
	}
	else if (c_oSerSdt::Color == type)
	{
		OOX::Logic::CRunProperty oRPr;
		res = oBinary_rPrReader.Read(length, &oRPr);
		pSdtPr->m_oColor = oRPr.m_oColor;
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
		pSdtPr->m_oId->m_oVal = m_oBufferedStream.GetULong();
	}
	else if (c_oSerSdt::Label == type)
	{
		pSdtPr->m_oLabel.Init();
		pSdtPr->m_oLabel->m_oVal = m_oBufferedStream.GetULong();
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
		pSdtPr->m_oPlaceHolder->m_oDocPart->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::RPr == type)
	{
		pSdtWraper->m_oRPr.Init();
		res = oBinary_rPrReader.Read(length, pSdtWraper->m_oRPr.GetPointer());
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
		pSdtPr->m_oTag->m_sVal = m_oBufferedStream.GetString3(length);
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
	else if (c_oSerSdt::Checkbox == type)
	{
		pSdtPr->m_oCheckbox.Init();
		READ1_DEF(length, res, this->ReadSdtCheckBox, pSdtPr->m_oCheckbox.GetPointer());
	}
	else if (c_oSerSdt::FormPr == type)
	{
		pSdtPr->m_oFormPr.Init();
		READ1_DEF(length, res, this->ReadSdtFormPr, pSdtPr->m_oFormPr.GetPointer());
	}
	else if (c_oSerSdt::TextFormPr == type)
	{
		pSdtPr->m_oTextFormPr.Init();
		READ1_DEF(length, res, this->ReadSdtTextFormPr, pSdtPr->m_oTextFormPr.GetPointer());
	}
	else if (c_oSerSdt::PictureFormPr == type)
	{
		pSdtPr->m_oPicture.Init();
		READ1_DEF(length, res, this->ReadSdtPicture, pSdtPr->m_oPicture.GetPointer());
	}
	else if (c_oSerSdt::ComplexFormPr == type)
	{
		pSdtPr->m_oComplexFormPr.Init();
		READ1_DEF(length, res, this->ReadSdtComplexFormPr, pSdtPr->m_oComplexFormPr.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtCheckBox(BYTE type, long length, void* poResult)
{
	int res = 0;
	OOX::Logic::CSdtCheckBox* pSdtCheckBox = static_cast<OOX::Logic::CSdtCheckBox*>(poResult);
	if (c_oSerSdt::CheckboxChecked == type)
	{
		pSdtCheckBox->m_oChecked.Init();
		pSdtCheckBox->m_oChecked->m_oVal.FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerSdt::CheckboxCheckedFont == type)
	{
		if(!pSdtCheckBox->m_oCheckedState.IsInit())
			pSdtCheckBox->m_oCheckedState.Init();
		pSdtCheckBox->m_oCheckedState->m_oFont.Init();
		pSdtCheckBox->m_oCheckedState->m_oFont->append(m_oBufferedStream.GetString3(length));
	}
	else if (c_oSerSdt::CheckboxCheckedVal == type)
	{
		if(!pSdtCheckBox->m_oCheckedState.IsInit())
			pSdtCheckBox->m_oCheckedState.Init();
		pSdtCheckBox->m_oCheckedState->m_oVal.Init();
		pSdtCheckBox->m_oCheckedState->m_oVal->SetValue(m_oBufferedStream.GetLong());
	}
	else if (c_oSerSdt::CheckboxUncheckedFont == type)
	{
		if(!pSdtCheckBox->m_oUncheckedState.IsInit())
			pSdtCheckBox->m_oUncheckedState.Init();
		pSdtCheckBox->m_oUncheckedState->m_oFont.Init();
		pSdtCheckBox->m_oUncheckedState->m_oFont->append(m_oBufferedStream.GetString3(length));
	}
	else if (c_oSerSdt::CheckboxUncheckedVal == type)
	{
		if(!pSdtCheckBox->m_oUncheckedState.IsInit())
			pSdtCheckBox->m_oUncheckedState.Init();
		pSdtCheckBox->m_oUncheckedState->m_oVal.Init();
		pSdtCheckBox->m_oUncheckedState->m_oVal->SetValue(m_oBufferedStream.GetLong());
	}
	else if (c_oSerSdt::CheckboxGroupKey == type)
	{
		pSdtCheckBox->m_oGroupKey.Init();
		pSdtCheckBox->m_oGroupKey->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtComboBox(BYTE type, long length, void* poResult)
{
	int res = 0;
	OOX::Logic::CSdtComboBox* pSdtComboBox = static_cast<OOX::Logic::CSdtComboBox*>(poResult);
	if (c_oSerSdt::LastValue == type)
	{
		pSdtComboBox->m_sLastValue.Init();
		pSdtComboBox->m_sLastValue->append(m_oBufferedStream.GetString3(length));
	}
	else if (c_oSerSdt::TextFormPrFormat == type)
	{
		pSdtComboBox->m_oFormat.Init();
		READ1_DEF(length, res, this->ReadSdtTextFormPrFormat, pSdtComboBox->m_oFormat.GetPointer());
	}
	else if (c_oSerSdt::SdtListItem == type)
	{
		ComplexTypes::Word::CSdtListItem* pSdtListItem = new ComplexTypes::Word::CSdtListItem();
		READ1_DEF(length, res, this->ReadSdtListItem, pSdtListItem);
		pSdtComboBox->m_arrListItem.push_back(pSdtListItem);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtListItem(BYTE type, long length, void* poResult)
{
	int res = 0;
	ComplexTypes::Word::CSdtListItem* pSdtListItem = static_cast<ComplexTypes::Word::CSdtListItem*>(poResult);
	if (c_oSerSdt::DisplayText == type)
	{
		pSdtListItem->m_sDisplayText = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::Value == type)
	{
		pSdtListItem->m_sValue = m_oBufferedStream.GetString3(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtPrDataBinding(BYTE type, long length, void* poResult)
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
int Binary_DocumentTableReader::ReadSdtPrDate(BYTE type, long length, void* poResult)
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
		pDate->m_oDateFormat->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::Lid == type)
	{
		pDate->m_oLid.Init();
		pDate->m_oLid->m_oVal = m_oBufferedStream.GetString3(length);
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
int Binary_DocumentTableReader::ReadDocPartList(BYTE type, long length, void* poResult)
{
	int res = 0;
	OOX::Logic::CSdtDocPart* pDocPart = static_cast<OOX::Logic::CSdtDocPart*>(poResult);
	if (c_oSerSdt::DocPartCategory == type)
	{
		pDocPart->m_oDocPartCategory.Init();
		pDocPart->m_oDocPartCategory->m_sVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::DocPartGallery == type)
	{
		pDocPart->m_oDocPartGallery.Init();
		pDocPart->m_oDocPartGallery->m_sVal = m_oBufferedStream.GetString3(length);
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
int Binary_DocumentTableReader::ReadDropDownList(BYTE type, long length, void* poResult)
{
	int res = 0;
	OOX::Logic::CSdtDropDownList* pDropDownList = static_cast<OOX::Logic::CSdtDropDownList*>(poResult);
	if (c_oSerSdt::LastValue == type)
	{
		pDropDownList->m_sLastValue = m_oBufferedStream.GetString3(length);
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
int Binary_DocumentTableReader::ReadSdtPicture(BYTE type, long length, void* poResult)
{
	OOX::Logic::CSdtPicture* pPicture = static_cast<OOX::Logic::CSdtPicture*>(poResult);

	int res = 0;
	if (c_oSerSdt::PictureFormPrScaleFlag == type)
	{
		pPicture->m_oScaleFlag = m_oBufferedStream.GetLong();
	}
	else if (c_oSerSdt::PictureFormPrLockProportions == type)
	{
		pPicture->m_oLockProportions = m_oBufferedStream.GetBool();
	}
	else if (c_oSerSdt::PictureFormPrRespectBorders == type)
	{
		pPicture->m_oRespectBorders = m_oBufferedStream.GetBool();
	}
	else if (c_oSerSdt::PictureFormPrShiftX == type)
	{
		pPicture->m_oShiftX = m_oBufferedStream.GetDoubleReal();
	}
	else if (c_oSerSdt::PictureFormPrShiftY == type)
	{
		pPicture->m_oShiftY = m_oBufferedStream.GetDoubleReal();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtFormPr(BYTE type, long length, void* poResult)
{
	int res = 0;
	OOX::Logic::CFormPr* pFormPr = static_cast<OOX::Logic::CFormPr*>(poResult);
	
	if (c_oSerSdt::FormPrKey == type)
	{
		pFormPr->m_oKey = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::FormPrLabel == type)
	{
		pFormPr->m_oLabel = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::FormPrHelpText == type)
	{
		pFormPr->m_oHelpText = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::FormPrRequired == type)
	{
		pFormPr->m_oRequired = m_oBufferedStream.GetBool();
	}
	else if (c_oSerSdt::FormPrBorder == type)
	{
		pFormPr->m_oBorder.Init();
		READ2_DEF(length, res, oBinary_pPrReader.ReadBorder2, pFormPr->m_oBorder.GetPointer());
	}
	else if (c_oSerSdt::FormPrShd == type)
	{
		pFormPr->m_oShd.Init();
		READ2_DEF(length, res, oBinary_CommonReader2.ReadShdComplexType, pFormPr->m_oShd.GetPointer());
	}
	else if (c_oSerSdt::OformMaster == type)
	{
		std::wstring pathOFormMaster = m_oBufferedStream.GetString3(length);

		if (false == pathOFormMaster.empty() && m_bOFormRead)
		{
			XmlUtils::replace_all(pathOFormMaster, L"\\", L"/");

			m_oFileWriter.m_pDrawingConverter->GetContentTypes()->Registration(L"oform/fieldMaster+xml", L"", pathOFormMaster.substr(3));	// del "../"		

			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(L"https://schemas.onlyoffice.com/relationships/oform-fieldMaster", pathOFormMaster, L"", &rId);

			pFormPr->m_oFieldRid.Init();
			pFormPr->m_oFieldRid->SetValue(L"rId" + std::to_wstring(rId));
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtTextFormPr(BYTE type, long length, void* poResult)
{
	int res = 0;
	OOX::Logic::CTextFormPr* pTextFormPr = static_cast<OOX::Logic::CTextFormPr*>(poResult);
	if (c_oSerSdt::TextFormPrComb == type)
	{
		pTextFormPr->m_oComb.Init();
		READ1_DEF(length, res, this->ReadSdtTextFormPrComb, pTextFormPr->m_oComb.GetPointer());
	}
	else if (c_oSerSdt::TextFormPrMaxCharacters == type)
	{
		pTextFormPr->m_oMaxCharacters.Init();
		pTextFormPr->m_oMaxCharacters->m_oVal = m_oBufferedStream.GetLong();
	}
	else if (c_oSerSdt::TextFormPrCombBorder == type)
	{
		pTextFormPr->m_oCombBorder.Init();
		READ2_DEF(length, res, oBinary_pPrReader.ReadBorder2, pTextFormPr->m_oCombBorder.GetPointer());
	}
	else if (c_oSerSdt::TextFormPrAutoFit == type)
	{
		pTextFormPr->m_oAutoFit = m_oBufferedStream.GetBool();
	}
	else if (c_oSerSdt::TextFormPrMultiLine == type)
	{
		pTextFormPr->m_oMultiLine = m_oBufferedStream.GetBool();
	}
	else if (c_oSerSdt::TextFormPrFormat == type)
	{
		pTextFormPr->m_oFormat.Init();
		READ1_DEF(length, res, this->ReadSdtTextFormPrFormat, pTextFormPr->m_oFormat.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtComplexFormPr(BYTE type, long length, void* poResult)
{
	int res = 0;
	OOX::Logic::CComplexFormPr* pComplexForm = static_cast<OOX::Logic::CComplexFormPr*>(poResult);
	if (c_oSerSdt::ComplexFormPrType == type)
	{
		pComplexForm->m_oType.Init();
		pComplexForm->m_oType->SetValueFromByte(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;

	return res;
}
int Binary_DocumentTableReader::ReadSdtTextFormPrFormat(BYTE type, long length, void* poResult)
{
	int res = 0;
	ComplexTypes::Word::CTextFormFormat* pFormat = static_cast<ComplexTypes::Word::CTextFormFormat*>(poResult);
	if (c_oSerSdt::TextFormPrFormatType == type)
	{
		pFormat->m_oType.Init();
		pFormat->m_oType->SetValueFromByte(m_oBufferedStream.GetChar());
	}
	else if (c_oSerSdt::TextFormPrFormatVal == type)
	{
		pFormat->m_oVal = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::TextFormPrFormatSymbols == type)
	{
		pFormat->m_oSymbols = m_oBufferedStream.GetString3(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int Binary_DocumentTableReader::ReadSdtTextFormPrComb(BYTE type, long length, void* poResult)
{
	int res = 0;
	ComplexTypes::Word::CComb* pComb = static_cast<ComplexTypes::Word::CComb*>(poResult);
	if (c_oSerSdt::TextFormPrCombWidth == type)
	{
		pComb->m_oWidth = m_oBufferedStream.GetLong();
	}
	else if (c_oSerSdt::TextFormPrCombSym == type)
	{
		pComb->m_oSym = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::TextFormPrCombFont == type)
	{
		pComb->m_oFont = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerSdt::TextFormPrCombWRule == type)
	{
		pComb->m_oWRule.Init();
		pComb->m_oWRule->SetValueFromByte(m_oBufferedStream.GetChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}


Binary_NotesTableReader::Binary_NotesTableReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream, Writers::FileWriter& oFileWriter, bool bIsFootnote)
	: 
	Binary_CommonReader(poBufferedStream), 
	m_oFileWriter(oFileWriter), 
	m_bIsFootnote(bIsFootnote)
{
}
int Binary_NotesTableReader::Read()
{
	m_oFileWriter.m_pDrawingConverter->SetDstContentRels();
    
	std::wstring sFilename;
	Writers::ContentWriter* pContentWriter = NULL;
	if(m_bIsFootnote)
	{
		sFilename = m_oFileWriter.get_footnotes_writer().getFilename();
		pContentWriter = &m_oFileWriter.get_footnotes_writer().m_oNotesWriter;
	}
	else
	{
		sFilename = m_oFileWriter.get_endnotes_writer().getFilename();
		pContentWriter = &m_oFileWriter.get_endnotes_writer().m_oNotesWriter;
	}
	Binary_DocumentTableReader oBinary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, *pContentWriter, false);

	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadNotes, &oBinary_DocumentTableReader);

	OOX::CPath fileRelsPath = (m_oFileWriter.get_document_writer().m_sDir)
	+ FILE_SEPARATOR_STR + L"word" 
	+ (m_oFileWriter.m_bGlossaryMode ? FILE_SEPARATOR_STR + std::wstring(L"glossary") : L"")
	+ FILE_SEPARATOR_STR + L"_rels"
	+ FILE_SEPARATOR_STR + sFilename + L".rels";

	m_oFileWriter.m_pDrawingConverter->SaveDstContentRels(fileRelsPath.GetPath());
	return res;
}
int Binary_NotesTableReader::ReadNotes(BYTE type, long length, void* poResult)
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
int Binary_NotesTableReader::ReadNote(BYTE type, long length, void* poResult)
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
		NSStringUtils::CStringBuilder& writer = pBinary_DocumentTableReader->m_oDocumentWriter.m_oContent;
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
int Binary_NotesTableReader::ReadNoteContent(BYTE type, long length, void* poResult)
{
	Binary_DocumentTableReader* pBinary_DocumentTableReader = static_cast<Binary_DocumentTableReader*>(poResult);
	return pBinary_DocumentTableReader->ReadDocumentContent(type, length, NULL);
};


BinaryFileReader::BinaryFileReader(std::wstring& sFileInDir, NSBinPptxRW::CBinaryFileReader& oBufferedStream, Writers::FileWriter& oFileWriter, bool bMacro, bool bOForm)
	: 
	m_sFileInDir(sFileInDir), 
	m_oBufferedStream(oBufferedStream), 
	m_oFileWriter(oFileWriter),
	m_bMacro(bMacro),
	m_bOForm(bOForm)
{
}
int BinaryFileReader::ReadFile()
{
	long res = c_oSerConstants::ReadOk;

	try
	{
		res = ReadMainTable();
	}
	catch(...)
	{
		res = c_oSerConstants::ErrorStream;
	}

	return res;
}
int BinaryFileReader::ReadMainTable()
{
	m_oBufferedStream.m_nDocumentType = m_oFileWriter.m_bGlossaryMode ? XMLWRITER_DOC_TYPE_DOCX_GLOSSARY : XMLWRITER_DOC_TYPE_DOCX;

	long res = c_oSerConstants::ReadOk;

	res = m_oBufferedStream.Peek(1) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
	
	if(c_oSerConstants::ReadOk != res)
		return res;
	
	long nOtherOffset		= -1;
	long nStyleOffset		= -1;
	long nSettingsOffset	= -1;
	long nDocumentOffset	= -1;
	long nCommentsOffset	= -1;
	long nDocumentCommentsOffset	= -1;

	std::vector<BYTE> aTypes;
	std::vector<long> aOffBits;
	BYTE mtLen = m_oBufferedStream.GetUChar();

	OOX::CSettings oSettings(NULL);
	oSettings.SetDefaults();
	m_oFileWriter.m_pCurrentSettings = &oSettings;
	
	for (int i = 0; i < mtLen; ++i)
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
		else if(c_oSerTableTypes::DocumentComments == mtiType)
		{
			nDocumentCommentsOffset = mtiOffBits;
		}
		else
		{
			aTypes.push_back(mtiType);
			aOffBits.push_back(mtiOffBits);
		}
	}
	if (-1 != nOtherOffset)
	{
		int nOldPos = m_oBufferedStream.GetPos();
		m_oBufferedStream.Seek(nOtherOffset);
		res = Binary_OtherTableReader(m_sFileInDir, m_oBufferedStream, m_oFileWriter).Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
	}
	if (-1 != nSettingsOffset)
	{
		OOX::CSettingsCustom oSettingsCustom;

		int nOldPos = m_oBufferedStream.GetPos();
		m_oBufferedStream.Seek(nSettingsOffset);
		res = Binary_SettingsTableReader(m_oBufferedStream, m_oFileWriter, &oSettingsCustom).Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
		
		if (!oSettingsCustom.IsEmpty())
		{
			m_oFileWriter.m_oCustomXmlWriter.WriteCustomSettings(oSettingsCustom.GetSchemaUrl(), oSettingsCustom.toXML(), m_oFileWriter.m_bGlossaryMode);
		}
	}
	else
	{
        std::wstring sClrMap(L"<w:clrSchemeMapping w:bg1=\"light1\" w:t1=\"dark1\" w:bg2=\"light2\" w:t2=\"dark2\" w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:hyperlink=\"hyperlink\" w:followedHyperlink=\"followedHyperlink\"/>");

		m_oFileWriter.m_pDrawingConverter->LoadClrMap(sClrMap);

		oSettings.m_oClrSchemeMapping.Init();
		oSettings.m_oClrSchemeMapping->fromXMLString(sClrMap);
	}
	BinaryStyleTableReader oBinaryStyleTableReader(m_oBufferedStream, m_oFileWriter);
	if (-1 != nStyleOffset)
	{
		int nOldPos = m_oBufferedStream.GetPos();
		m_oBufferedStream.Seek(nStyleOffset);
		res = oBinaryStyleTableReader.Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
	}
	Binary_CommentsTableReader oBinary_CommentsTableReader(m_oBufferedStream, m_oFileWriter);

	if (-1 != nCommentsOffset)
	{		
		int nOldPos = m_oBufferedStream.GetPos();
		m_oBufferedStream.Seek(nCommentsOffset);
		res = oBinary_CommentsTableReader.Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
	}
	Binary_CommentsTableReader oBinary_DocumentCommentsTableReader(m_oBufferedStream, m_oFileWriter);
	if (-1 != nDocumentCommentsOffset)
	{
		int nOldPos = m_oBufferedStream.GetPos();
		m_oBufferedStream.Seek(nDocumentCommentsOffset);
		res = oBinary_DocumentCommentsTableReader.Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
	}
	
	for (size_t i = 0; i < aTypes.size(); ++i)
	{
		BYTE mtiType = aTypes[i];
		long mtiOffBits = aOffBits[i];

		m_oBufferedStream.Seek(mtiOffBits);
		switch (mtiType)
		{
			//case c_oSerTableTypes::Signature:break;
			//case c_oSerTableTypes::Info:break;
			//case c_oSerTableTypes::Style:
			//	res = BinaryStyleTableReader(m_oBufferedStream, m_oFileWriter).Read();
			//	break;
			//case c_oSerTableTypes::Document:
			//	res = Binary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, m_oFileWriter.m_oDocumentWriter).Read();
			//	break;
		case c_oSerTableTypes::App:
		{
			PPTX::App oApp(NULL);
			oApp.fromPPTY(&m_oBufferedStream);
			OOX::CApp* pApp = new OOX::CApp(NULL);
			pApp->FromPptxApp(&oApp);
			pApp->SetRequiredDefaults();
			m_oFileWriter.m_pApp = pApp;
		}
		break;
		case c_oSerTableTypes::Core:
		{
			PPTX::Core oCore(NULL);
			oCore.fromPPTY(&m_oBufferedStream);
			OOX::CCore* pCore = new OOX::CCore(NULL);
			pCore->FromPptxCore(&oCore);
			pCore->SetRequiredDefaults();
			m_oFileWriter.m_pCore = pCore;
		}
		break;
		case c_oSerTableTypes::CustomProperties:
		{
			PPTX::CustomProperties* pCustomProperties = new PPTX::CustomProperties(NULL);
			pCustomProperties->fromPPTY(&m_oBufferedStream);
			m_oFileWriter.m_pCustomProperties = pCustomProperties;
			m_oFileWriter.m_oDocumentRelsWriter.m_bHasCustomProperties = true;
		}break;
		case c_oSerTableTypes::HdrFtr:
		{
			res = Binary_HdrFtrTableReader(m_oBufferedStream, m_oFileWriter).Read();
		}break;
		case c_oSerTableTypes::Numbering:
		{
			res = Binary_NumberingTableReader(m_oBufferedStream, m_oFileWriter).Read();
		}break;
		case c_oSerTableTypes::Footnotes:
		{
			res = Binary_NotesTableReader(m_oBufferedStream, m_oFileWriter, true).Read();
		}break;
		case c_oSerTableTypes::Endnotes:
		{
			res = Binary_NotesTableReader(m_oBufferedStream, m_oFileWriter, false).Read();
		}break;
		case c_oSerTableTypes::VbaProject:
		{
			m_bMacroRead = true;
			m_oBufferedStream.Skip(1); //skip type
			if (m_bMacro)
			{
				m_oFileWriter.m_pVbaProject = new OOX::VbaProject(NULL);
				m_oFileWriter.m_pVbaProject->fromPPTY(&m_oBufferedStream);
			}
			else
				m_oBufferedStream.SkipRecord();
		}break;
		case c_oSerTableTypes::OForm:
		{
			if (m_bOForm)
			{
				_INT32 nDataSize = m_oBufferedStream.GetLong();
				BYTE *pData = new BYTE[nDataSize];
				if (pData)
				{
					m_oBufferedStream.GetArray(pData, nDataSize);
					
					std::wstring pathOFormDst = m_oFileWriter.get_document_writer().m_sDir + FILE_SEPARATOR_STR + L"oform";
					std::wstring sZipOformFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(m_oFileWriter.get_document_writer().m_sDir, L"oform");

					NSFile::CFileBinary zipOform;
					if (zipOform.CreateFile(sZipOformFile) && NSDirectory::CreateDirectory(pathOFormDst))
					{
						zipOform.WriteFile(pData, nDataSize);
						zipOform.CloseFile();

						COfficeUtils oCOfficeUtils(NULL);
						if (S_OK == oCOfficeUtils.ExtractToDirectory(sZipOformFile, pathOFormDst, NULL, 0))
						{
							m_oFileWriter.m_pDrawingConverter->GetContentTypes()->Registration(L"oform/main+xml", L"/oform", L"main.xml");
						}
						NSFile::CFileBinary::Remove(sZipOformFile);
					}
					delete[]pData;
				}
			}
			else
				m_oBufferedStream.SkipRecord();
		}break;
		case c_oSerTableTypes::Glossary:
		{
			OOX::CPath pathGlossary = m_oFileWriter.get_document_writer().m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"glossary";
			OOX::CPath pathGlossaryRels = pathGlossary + FILE_SEPARATOR_STR + L"_rels";

			if (NSDirectory::CreateDirectory(pathGlossary.GetPath()) && NSDirectory::CreateDirectory(pathGlossaryRels.GetPath()))
			{
				m_oFileWriter.m_bGlossaryMode = true;
				ReadMainTable();
				m_oFileWriter.WriteGlossary();
				m_oFileWriter.m_bGlossaryMode = false;

				m_oFileWriter.m_pCurrentSettings = &oSettings;
				
				smart_ptr<PPTX::Logic::ClrMap> pClrMap = m_oFileWriter.m_pCurrentSettings->m_oClrSchemeMapping.GetPointer();
				pClrMap.AddRef();
				*m_oFileWriter.m_pDrawingConverter->m_pClrMap = pClrMap;
			}
			else res = c_oSerConstants::ReadUnknown;
		}break;
		case c_oSerTableTypes::Customs:
		{
			OOX::CPath pathCustomXml = m_oFileWriter.get_document_writer().m_sDir + FILE_SEPARATOR_STR + L"customXml";
			OOX::CPath pathCustomXmlRels = pathCustomXml + FILE_SEPARATOR_STR + L"_rels";
			if (NSDirectory::CreateDirectory(pathCustomXml.GetPath()) && NSDirectory::CreateDirectory(pathCustomXmlRels.GetPath()))
			{
				Binary_CustomsTableReader oBinary_CustomsTableReader(m_oBufferedStream, m_oFileWriter);
				res = oBinary_CustomsTableReader.Read();
			}
		}break;
		}
		if (c_oSerConstants::ReadOk != res)
			return res;
	}
	if (-1 != nDocumentOffset)
	{
		m_oBufferedStream.Seek(nDocumentOffset);

		m_oFileWriter.m_pDrawingConverter->SetDstContentRels();
		unsigned int stamdartRId;
       
		m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles",		L"styles.xml",		L"", &stamdartRId);
        m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings",	L"settings.xml",	L"", &stamdartRId);
        m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings",L"webSettings.xml", L"", &stamdartRId);
        m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable",	L"fontTable.xml",	L"", &stamdartRId);
       
		if (m_oFileWriter.m_bGlossaryMode)
		{
			m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.glossary+xml", L"/word/glossary", L"document.xml");
		}
		else
		{
			m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme", L"theme/theme1.xml", L"", &stamdartRId);
			if (m_oFileWriter.m_pVbaProject.IsInit())
			{
				m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2006/relationships/vbaProject", L"vbaProject.bin", L"", &stamdartRId);

				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.ms-word.document.macroEnabled.main+xml", L"/word", L"document.xml");
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.ms-office.vbaProject", L"/word", L"vbaProject.bin");
			}
			else
			{
				m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml", L"/word", L"document.xml");
			}
		}
		m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",
			L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), L"styles.xml");
		m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",
			L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), L"settings.xml");
		m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",
			L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), L"webSettings.xml");
		m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",
			L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), L"fontTable.xml");
		m_oFileWriter.m_pDrawingConverter->Registration(L"application/vnd.openxmlformats-officedocument.theme+xml",	L"/word/theme", L"theme1.xml");

		if (false == m_oFileWriter.m_bGlossaryMode && false == m_oFileWriter.IsEmptyGlossary())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/glossaryDocument", L"glossary/document.xml", L"", &rId);
		}
		if (false == m_oFileWriter.get_numbering_writer().IsEmpty())
		{
			unsigned int rId;
            m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering", L"numbering.xml", std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(
				L"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml", 
				L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), L"numbering.xml");
		}
        if (false == m_oFileWriter.get_footnotes_writer().IsEmpty())
		{
			unsigned int rId;
            m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes", L"footnotes.xml", std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(
				L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml", 
				L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), L"footnotes.xml");
		}
        if (false == m_oFileWriter.get_endnotes_writer().IsEmpty())
		{
			unsigned int rId;
            m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes", L"endnotes.xml", std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(
				L"application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml",
				L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), L"endnotes.xml");
		}
		for (size_t i = 0; i < m_oFileWriter.get_headers_footers_writer().m_aHeaders.size(); ++i)
		{
			Writers::HdrFtrItem* pHeader = m_oFileWriter.get_headers_footers_writer().m_aHeaders[i];
			if(false == pHeader->IsEmpty())
			{
				unsigned int rId;
                m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header", pHeader->m_sFilename, std::wstring(), &rId);
                pHeader->rId = L"rId" + std::to_wstring( rId );
				
				m_oFileWriter.m_pDrawingConverter->Registration(
					L"application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml", 
					L"/word" + std::wstring(m_oFileWriter.m_bGlossaryMode ? L"/glossary" : L""), pHeader->m_sFilename);
			}
		}
		for (size_t i = 0; i < m_oFileWriter.get_headers_footers_writer().m_aFooters.size(); ++i)
		{
			Writers::HdrFtrItem* pFooter = m_oFileWriter.get_headers_footers_writer().m_aFooters[i];
			if (false == pFooter->IsEmpty())
			{
				unsigned int rId;
                m_oFileWriter.m_pDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer", pFooter->m_sFilename, std::wstring(), &rId);
                pFooter->rId = L"rId" + std::to_wstring( rId );
				
				m_oFileWriter.m_pDrawingConverter->Registration(
					L"application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml", 
					L"/word" + (m_oFileWriter.m_bGlossaryMode ? std::wstring(L"/glossary") : L""), pFooter->m_sFilename);
			}
		}
		for (size_t i = 0; i < m_oFileWriter.m_oCustomXmlWriter.arItems.size(); ++i)
		{
			if (m_oFileWriter.m_oCustomXmlWriter.arItems[i].second != m_oFileWriter.m_bGlossaryMode) continue;

			std::wstring sRelsPath = L"../" + OOX::FileTypes::CustomXml.DefaultDirectory().GetPath() + L"/" + m_oFileWriter.m_oCustomXmlWriter.arItems[i].first;
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::CustomXml.RelationType(), sRelsPath, L"", &rId);
		}

		res = Binary_DocumentTableReader(m_oBufferedStream, m_oFileWriter, m_oFileWriter.get_document_writer(), m_bOForm).Read();

        OOX::CPath fileRelsPath = m_oFileWriter.get_document_writer().m_sDir	+ FILE_SEPARATOR_STR + L"word"
																				+ (m_oFileWriter.m_bGlossaryMode ? (FILE_SEPARATOR_STR + std::wstring(L"glossary")) : L"")
																				+ FILE_SEPARATOR_STR + L"_rels"
																				+ FILE_SEPARATOR_STR + L"document.xml.rels";
//------------------------------------------------------------------------------------------------------------------------
//comments
        CComments& oComments= oBinary_CommentsTableReader.m_oComments;
		Writers::CommentsWriter& oCommentsWriter = m_oFileWriter.get_comments_writer();
        
		std::wstring sContent	= oComments.writeContent();
        std::wstring sContentEx = oComments.writeContentExt();	//важно чтобы writeContentExt вызывался после writeContent
		std::wstring sContentExtensible = oComments.writeContentExtensible();
		std::wstring sContentsId = oComments.writeContentsIds();
        std::wstring sPeople	= oComments.writePeople();

		std::wstring sDocumentContent	= oBinary_DocumentCommentsTableReader.m_oComments.writeContent();
		std::wstring sDocumentContentEx = oBinary_DocumentCommentsTableReader.m_oComments.writeContentExt();	//важно чтобы writeContentExt вызывался после writeContent
		std::wstring sDocumentContentExtensible = oBinary_DocumentCommentsTableReader.m_oComments.writeContentExtensible();
		std::wstring sDocumentContentsId = oBinary_DocumentCommentsTableReader.m_oComments.writeContentsIds();
		std::wstring sDocumentPeople	= oBinary_DocumentCommentsTableReader.m_oComments.writePeople();

		std::wstring sContentUserData = oComments.writeContentUserData();
		sContentUserData += oBinary_DocumentCommentsTableReader.m_oComments.writeContentUserData();

		oCommentsWriter.setElements(sContent, sContentEx, sContentExtensible, sContentsId, sPeople, sDocumentContent, sDocumentContentEx, sDocumentContentExtensible, sDocumentContentsId, sDocumentPeople, sContentUserData);
//------------------------------------------------------------------------------------------------------------------------
//settings
		m_oFileWriter.get_settings_writer().AddSetting(oSettings.toXML());
//------------------------------------------------------------------------------------------------------------------------

		if (false == oCommentsWriter.m_sComment.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::Comments.RelationType(), OOX::FileTypes::Comments.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::Comments.OverrideType(), L"/word", OOX::FileTypes::Comments.DefaultFileName().GetPath());
		}
        if (false == oCommentsWriter.m_sCommentExt.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::CommentsExt.RelationType(), OOX::FileTypes::CommentsExt.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::CommentsExt.OverrideType(), L"/word", OOX::FileTypes::CommentsExt.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sContentExtensible.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::CommentsExtensible.RelationType(), OOX::FileTypes::CommentsExtensible.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::CommentsExtensible.OverrideType(), L"/word", OOX::FileTypes::CommentsExtensible.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sCommentsIds.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::CommentsIds.RelationType(), OOX::FileTypes::CommentsIds.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::CommentsIds.OverrideType(), L"/word", OOX::FileTypes::CommentsIds.DefaultFileName().GetPath());
		}
        if (false == oCommentsWriter.m_sPeople.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::People.RelationType(), OOX::FileTypes::People.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::People.OverrideType(), L"/word", OOX::FileTypes::People.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sDocumentComment.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::DocumentComments.RelationType(), OOX::FileTypes::DocumentComments.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::DocumentComments.OverrideType(), L"/word", OOX::FileTypes::DocumentComments.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sDocumentCommentExt.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::DocumentCommentsExt.RelationType(), OOX::FileTypes::DocumentCommentsExt.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::DocumentCommentsExt.OverrideType(), L"/word", OOX::FileTypes::DocumentCommentsExt.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sDocumentContentExtensible.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::DocumentCommentsExtensible.RelationType(), OOX::FileTypes::DocumentCommentsExtensible.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::DocumentCommentsExtensible.OverrideType(), L"/word", OOX::FileTypes::DocumentCommentsExtensible.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sDocumentCommentsIds.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::DocumentCommentsIds.RelationType(), OOX::FileTypes::DocumentCommentsIds.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::DocumentCommentsIds.OverrideType(), L"/word", OOX::FileTypes::DocumentCommentsIds.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sDocumentPeople.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::DocumentPeople.RelationType(), OOX::FileTypes::DocumentPeople.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::DocumentPeople.OverrideType(), L"/word", OOX::FileTypes::DocumentPeople.DefaultFileName().GetPath());
		}
		if (false == oCommentsWriter.m_sContentUserData.empty())
		{
			unsigned int rId;
			m_oFileWriter.m_pDrawingConverter->WriteRels(OOX::FileTypes::CommentsUserData.RelationType(), OOX::FileTypes::CommentsUserData.DefaultFileName().GetPath(), std::wstring(), &rId);
			m_oFileWriter.m_pDrawingConverter->Registration(OOX::FileTypes::CommentsUserData.OverrideType(), L"/word", OOX::FileTypes::CommentsUserData.DefaultFileName().GetPath());
		}

        m_oFileWriter.m_pDrawingConverter->SaveDstContentRels(fileRelsPath.GetPath());
		if (c_oSerConstants::ReadOk != res)
			return res;
	}
	return res;
}

}
