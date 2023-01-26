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
#include "BinWriters.h"

#include "../DocWrapper/FontProcessor.h"
#include "../../../../Common/Base64.h"
#include "../../../../Common/OfficeFileFormatChecker.h"

#include "../../Presentation/FontCutter.h"
#include "../../../PPTXFormat/App.h"
#include "../../../PPTXFormat/Core.h"
#include "../../../PPTXFormat/Logic/HeadingVariant.h"
#include "../../Sheets/Reader/BinaryWriter.h"
#include "BinEquationWriter.h"

#include "../../../../OfficeUtils/src/OfficeUtils.h"
#ifndef _IOS
	#include "../../../../MsBinaryFile/DocFile/Main/DocFormatLib.h"
#endif
#include "../../../../HtmlFile2/htmlfile2.h"
#include "../../../../RtfFile/Format/ConvertationManager.h"

#include "../../../DocxFormat/CustomXml.h"
#include "../../../DocxFormat/Logic/AlternateContent.h"
#include "../../../DocxFormat/Logic/Dir.h"
#include "../../../DocxFormat/Logic/SmartTag.h"
#include "../../../DocxFormat/Logic/Bdo.h"
#include "../../../DocxFormat/Logic/Run.h"
#include "../../../DocxFormat/Logic/RunProperty.h"

#define COMPLEX_BOOL_TO_UINT(offset, val) \
	if(val.IsInit()) { \
		nFlags |= (val->m_oVal.ToBool() ? 1 : 0) << offset; \
	}

namespace BinDocxRW
{
ParamsWriter::ParamsWriter(NSBinPptxRW::CBinaryFileWriter* pCBufferedStream, DocWrapper::FontProcessor* pFontProcessor, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager)
	:
	m_pCBufferedStream(pCBufferedStream),
	m_pFontProcessor(pFontProcessor),
	m_pOfficeDrawingConverter(pOfficeDrawingConverter),
	m_pEmbeddedFontsManager(pEmbeddedFontsManager)
{
	m_pMain		= NULL;
	m_pSettings = NULL;
	m_pTheme	= NULL;
	m_pCurRels	= NULL;
	m_pStyles	= NULL;
	m_pNumbering = NULL;

	m_pEmbeddedStyles = NULL;
	m_pEmbeddedNumbering = NULL;

	m_bLocalStyles = m_bLocalNumbering = false;
}
std::wstring ParamsWriter::AddEmbeddedStyle(const std::wstring & sStyleId)
{
	if (!m_pEmbeddedStyles) return L"";
	if (sStyleId.empty()) return L"";

	std::wstring sNewStyleId;
	std::map<std::wstring, size_t>::iterator pPair = m_pEmbeddedStyles->m_mapStyleNames.find(sStyleId);

	if (pPair != m_pEmbeddedStyles->m_mapStyleNames.end())
	{
		OOX::CStyle* style = m_pEmbeddedStyles->m_arrStyle[pPair->second];

		sNewStyleId = L"EmbeddedStyle" + std::to_wstring(m_pEmbeddedStyles->m_mapEmbeddedStyleNames.size() + 1) + L"_" + sStyleId;

		m_pStyles->m_mapStyleNames.insert(std::make_pair(sNewStyleId, m_pStyles->m_arrStyle.size()));
		m_pStyles->m_arrStyle.push_back(new OOX::CStyle(*style));

		style = m_pStyles->m_arrStyle.back();

		style->m_sStyleId = sNewStyleId;

		if (style->m_oBasedOn.IsInit() && style->m_oBasedOn->m_sVal.IsInit())
			style->m_oBasedOn->m_sVal = AddEmbeddedStyle(*style->m_oBasedOn->m_sVal);

		m_pStyles->m_mapEmbeddedStyleNames.back().insert(std::make_pair(sStyleId, sNewStyleId));
	}
	return sNewStyleId;
}

BinaryCommonWriter::BinaryCommonWriter(ParamsWriter& oParamsWriter) :	m_oStream(*oParamsWriter.m_pCBufferedStream),
													m_pEmbeddedFontsManager(oParamsWriter.m_pEmbeddedFontsManager)
{
	m_oStream.SetRels(oParamsWriter.m_pCurRels);
}
int BinaryCommonWriter::WriteItemStart(BYTE type)
{
	m_oStream.WriteBYTE(type);
	return WriteItemWithLengthStart();
}
void BinaryCommonWriter::WriteItemEnd(int nStart)
{
	WriteItemWithLengthEnd(nStart);
}
int BinaryCommonWriter::WriteItemWithLengthStart()
{
//Запоминаем позицию чтобы в конце записать туда длину
	int nStartPos = m_oStream.GetPosition();
	m_oStream.Skip(4);
	return nStartPos;
}
void BinaryCommonWriter::WriteItemWithLengthEnd(int nStart)
{
//Length
	int nEnd = m_oStream.GetPosition();
	m_oStream.SetPosition(nStart);
	m_oStream.WriteLONG(nEnd - nStart - 4);
	m_oStream.SetPosition(nEnd);
}
void BinaryCommonWriter::WriteBorder(const BYTE & type, const ComplexTypes::Word::CBorder& border)
{
	//if ((border.m_oVal.IsInit()) && (border.m_oVal->GetValue() == SimpleTypes::bordervalueNone)) return;
	int nCurPos = WriteItemStart(type);
	WriteBorder(border);
	WriteItemEnd(nCurPos);
}
void BinaryCommonWriter::WriteBorder(const ComplexTypes::Word::CBorder& border)
{//todooo сделать все типы бордера
	if(border.m_oVal.IsInit())
	{
		if(border.m_oColor.IsInit())
			WriteColor(c_oSerBorderType::Color, border.m_oColor.get());
		WriteThemeColor(c_oSerBorderType::ColorTheme, border.m_oColor, border.m_oThemeColor, border.m_oThemeTint, border.m_oThemeShade);
		if(border.m_oSpace.IsInit())
		{
			m_oStream.WriteBYTE(c_oSerBorderType::SpacePoint);
			m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oStream.WriteLONG(border.m_oSpace->GetValue());
		}
		if(border.m_oSz.IsInit())
		{
			m_oStream.WriteBYTE(c_oSerBorderType::Size8Point);
			m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oStream.WriteLONG(border.m_oSz->GetValue());
		}
	//Val
		m_oStream.WriteBYTE(c_oSerBorderType::Value);
		m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		switch(border.m_oVal.get().GetValue())
		{
		case SimpleTypes::bordervalueNone:
		case SimpleTypes::bordervalueNil:   m_oStream.WriteBYTE(border_None);   break;
		default:                            m_oStream.WriteBYTE(border_Single); break;
		}
	}
}
void BinaryCommonWriter::WriteTblBorders(const OOX::Logic::CTblBorders& Borders)
{
	int nCurPos = 0;
	//Left
	if(Borders.m_oStart.IsInit())
	{
		WriteBorder(c_oSerBordersType::left, Borders.m_oStart.get());
	}
	//Top
	if(Borders.m_oTop.IsInit())
	{
		WriteBorder(c_oSerBordersType::top, Borders.m_oTop.get());
	}
	//Right
	if(Borders.m_oEnd.IsInit())
	{
		WriteBorder(c_oSerBordersType::right, Borders.m_oEnd.get());
	}
	//Bottom
	if(Borders.m_oBottom.IsInit())
	{
		WriteBorder(c_oSerBordersType::bottom, Borders.m_oBottom.get());
	}
	//InsideV
	if(Borders.m_oInsideV.IsInit())
	{
		WriteBorder(c_oSerBordersType::insideV, Borders.m_oInsideV.get());
	}
	//InsideH
	if(Borders.m_oInsideH.IsInit())
	{
		WriteBorder(c_oSerBordersType::insideH, Borders.m_oInsideH.get());
	}
}
void BinaryCommonWriter::WriteTcBorders(const OOX::Logic::CTcBorders& Borders)
{
	int nCurPos = 0;
	//Top
	if(Borders.m_oTop.IsInit())
	{
		WriteBorder(c_oSerBordersType::top, Borders.m_oTop.get());
	}
	//Left
	if(Borders.m_oStart.IsInit())
	{
		WriteBorder(c_oSerBordersType::left, Borders.m_oStart.get());
	}
	//Bottom
	if(Borders.m_oBottom.IsInit())
	{
		WriteBorder(c_oSerBordersType::bottom, Borders.m_oBottom.get());
	}	
	//Right
	if(Borders.m_oEnd.IsInit())
	{
		WriteBorder(c_oSerBordersType::right, Borders.m_oEnd.get());
	}
	//InsideH
	if(Borders.m_oInsideH.IsInit())
	{
		WriteBorder(c_oSerBordersType::insideH, Borders.m_oInsideH.get());
	}	
	//InsideV
	if(Borders.m_oInsideV.IsInit())
	{
		WriteBorder(c_oSerBordersType::insideV, Borders.m_oInsideV.get());
	}
}
void BinaryCommonWriter::WritePBorders(const OOX::Logic::CPBdr& Borders)
{
	int nCurPos = 0;
	//Left
	if(Borders.m_oLeft.IsInit())
	{
		WriteBorder(c_oSerBordersType::left, Borders.m_oLeft.get());
	}
	//Top
	if(Borders.m_oTop.IsInit())
	{
		WriteBorder(c_oSerBordersType::top, Borders.m_oTop.get());
	}
	//Right
	if(Borders.m_oRight.IsInit())
	{
		WriteBorder(c_oSerBordersType::right, Borders.m_oRight.get());
	}
	//Bottom
	if(Borders.m_oBottom.IsInit())
	{
		WriteBorder(c_oSerBordersType::bottom, Borders.m_oBottom.get());
	}
	//Between
	if(Borders.m_oBetween.IsInit())
	{
		WriteBorder(c_oSerBordersType::between, Borders.m_oBetween.get());
	}
}
void BinaryCommonWriter::WriteColor(BYTE type, const SimpleTypes::CHexColor& color)
{
	if(SimpleTypes::hexcolorRGB == color.GetValue())
	{
		m_oStream.WriteBYTE(type);
		m_oStream.WriteBYTE(c_oSerPropLenType::Three);
		m_oStream.WriteBYTE(color.Get_R());
		m_oStream.WriteBYTE(color.Get_G());
		m_oStream.WriteBYTE(color.Get_B());
	}
}
void BinaryCommonWriter::WriteThemeColor(BYTE type, const nullable<SimpleTypes::CHexColor>& oHexColor, const nullable<SimpleTypes::CThemeColor>& oThemeColor,
	const nullable<SimpleTypes::CUcharHexNumber>& oThemeTint, const nullable<SimpleTypes::CUcharHexNumber>& oThemeShade)
{
	if((oHexColor.IsInit() && SimpleTypes::hexcolorAuto == oHexColor->GetValue()) || oThemeColor.IsInit() || oThemeTint.IsInit() || oThemeShade.IsInit())
	{
		m_oStream.WriteBYTE(type);
		m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = WriteItemWithLengthStart();
		if(oHexColor.IsInit() && SimpleTypes::hexcolorAuto == oHexColor->GetValue())
		{
			m_oStream.WriteBYTE(c_oSer_ColorThemeType::Auto);
			m_oStream.WriteBYTE(c_oSerPropLenType::Null);
		}
		if(oThemeColor.IsInit())
		{
			m_oStream.WriteBYTE(c_oSer_ColorThemeType::Color);
			m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oStream.WriteBYTE(oThemeColor->GetValue());
		}
		if(oThemeTint.IsInit())
		{
			m_oStream.WriteBYTE(c_oSer_ColorThemeType::Tint);
			m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oStream.WriteBYTE(oThemeTint->GetValue());
		}
		if(oThemeShade.IsInit())
		{
			m_oStream.WriteBYTE(c_oSer_ColorThemeType::Shade);
			m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oStream.WriteBYTE(oThemeShade->GetValue());
		}
		WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryCommonWriter::WriteShd(const ComplexTypes::Word::CShading& Shd)
{
	//Type
	if (false != Shd.m_oVal.IsInit())
	{
		m_oStream.WriteBYTE(c_oSerShdType::Value);
		m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oStream.WriteBYTE(Shd.m_oVal.get().GetValue()); //Misalignment-footer.doc

	}
	//Color
	if (false != Shd.m_oColor.IsInit())
		WriteColor(c_oSerShdType::Color, Shd.m_oColor.get());
	
	WriteThemeColor(c_oSerShdType::ColorTheme, Shd.m_oColor, Shd.m_oThemeColor, Shd.m_oThemeTint, Shd.m_oThemeShade);

	//Fill
	if (false != Shd.m_oFill.IsInit())
		WriteColor(c_oSerShdType::Fill, Shd.m_oFill.get());

	WriteThemeColor(c_oSerShdType::FillTheme, Shd.m_oFill, Shd.m_oThemeFill, Shd.m_oThemeFillTint, Shd.m_oThemeFillShade);

}
void BinaryCommonWriter::WritePaddings(const nullable<SimpleTypes::CTwipsMeasure>& left, const nullable<SimpleTypes::CTwipsMeasure>& top,
	const nullable<SimpleTypes::CTwipsMeasure>& right, const nullable<SimpleTypes::CTwipsMeasure>& bottom)
{
	//left
	if(left.IsInit())
	{
		m_oStream.WriteBYTE(c_oSerPaddingType::leftTwips);
		m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oStream.WriteLONG(left->ToTwips());
	}
	//top
	if(top.IsInit())
	{
		m_oStream.WriteBYTE(c_oSerPaddingType::topTwips);
		m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oStream.WriteLONG(top->ToTwips());
	}
	//Right
	if(right.IsInit())
	{
		m_oStream.WriteBYTE(c_oSerPaddingType::rightTwips);
		m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oStream.WriteLONG(right->ToTwips());
	}
	//bottom
	if(bottom.IsInit())
	{
		m_oStream.WriteBYTE(c_oSerPaddingType::bottomTwips);
		m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oStream.WriteLONG(bottom->ToTwips());
	}
}
void BinaryCommonWriter::WriteFont(std::wstring sFontName, BYTE bType, DocWrapper::FontProcessor& m_oFontProcessor)
{
	if(!sFontName.empty())
	{
		//Подбор шрифтов
		sFontName = m_oFontProcessor.getFont(sFontName);
		if(NULL != m_pEmbeddedFontsManager)
			m_pEmbeddedFontsManager->CheckFont(sFontName, m_oFontProcessor.getFontManager());

		m_oStream.WriteBYTE(bType);
		m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oStream.WriteStringW(sFontName);
	}
}
void BinaryCommonWriter::WriteBytesArray(BYTE* pData, long nDataSize)
{
	int nCurPos = WriteItemWithLengthStart();
	m_oStream.WriteBYTEArray(pData, nDataSize);
	WriteItemWithLengthEnd(nCurPos);
}
template<typename T> void BinaryCommonWriter::WriteTrackRevision(const T& elem)
{
	int nCurPos = 0;
	if(elem.m_sAuthor.IsInit())
	{
		nCurPos = WriteItemStart(c_oSerProp_RevisionType::Author);
		m_oStream.WriteStringW3(elem.m_sAuthor->c_str());
		WriteItemWithLengthEnd(nCurPos);
	}
	if(elem.m_oDate.IsInit())
	{
		nCurPos = WriteItemStart(c_oSerProp_RevisionType::Date);
		m_oStream.WriteStringW3(elem.m_oDate->ToString());
		WriteItemWithLengthEnd(nCurPos);
	}
	if(elem.m_oId.IsInit())
	{
		nCurPos = WriteItemStart(c_oSerProp_RevisionType::Id);
		m_oStream.WriteLONG(elem.m_oId->GetValue());
		WriteItemWithLengthEnd(nCurPos);
	}
	if(elem.m_sUserId.IsInit())
	{
		nCurPos = WriteItemStart(c_oSerProp_RevisionType::UserId);
		m_oStream.WriteStringW3(elem.m_sUserId->c_str());
		WriteItemWithLengthEnd(nCurPos);
	}
}



BinaryHeaderFooterTableWriter::BinaryHeaderFooterTableWriter(ParamsWriter& oParamsWriter, OOX::IFileContainer* oDocumentRels, std::map<int, bool>* mapIgnoreComments):
																m_oBcw(oParamsWriter), 
																m_oParamsWriter(oParamsWriter), 
																m_pOfficeDrawingConverter(oParamsWriter.m_pOfficeDrawingConverter), 
																m_oDocumentRelsWriter(oDocumentRels), 
																m_mapIgnoreComments(mapIgnoreComments)
{
}
void BinaryHeaderFooterTableWriter::Write()
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	int nCurPos = 0;
	//Header
	if(m_aHeaders.size() > 0)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerHdrFtrTypes::Header);
		WriteHdrFtrContent(m_aHeaders, m_aHeaderTypes, m_aHeaderSectPrs, true);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Footer
	if(m_aFooters.size() > 0)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerHdrFtrTypes::Footer);
		WriteHdrFtrContent(m_aFooters, m_aFooterTypes, m_aFooterSectPrs, false);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryHeaderFooterTableWriter::WriteHdrFtrContent(std::vector<OOX::CHdrFtr*>& aHdrFtrs, std::vector<SimpleTypes::EHdrFtr>& aHdrFtrTypes, std::vector<OOX::Logic::CSectionProperty*>& aHdrSectPrs, bool bHdr)
{
	int nCurPos = 0;
	for(size_t i = 0; i < aHdrFtrs.size(); ++i)
	{
		OOX::CHdrFtr* pHdrFtr = aHdrFtrs[i];
		SimpleTypes::EHdrFtr eType = aHdrFtrTypes[i];
		OOX::Logic::CSectionProperty* pSectPr = aHdrSectPrs[i];
		BYTE byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_Odd;
		switch(eType)
		{
			case SimpleTypes::hdrftrFirst:	byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_First;	break;
			case SimpleTypes::hdrftrEven:	byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_Even;	break;
			default:						byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_Odd;		break;
		}
		nCurPos = m_oBcw.WriteItemStart(byteHdrFtrType);
		WriteHdrFtrItem(pSectPr, pHdrFtr, bHdr);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryHeaderFooterTableWriter::WriteHdrFtrItem(OOX::Logic::CSectionProperty* pSectPr, OOX::CHdrFtr* pHdrFtr, bool bHdr)
{
	int nCurPos = 0;
//Content
	ParamsDocumentWriter oParamsDocumentWriter(pHdrFtr);
	m_oParamsWriter.m_pCurRels = oParamsDocumentWriter.m_pRels;

	BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, oParamsDocumentWriter, m_mapIgnoreComments, NULL);
	
	smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
	m_pOfficeDrawingConverter->SetRels(oParamsDocumentWriter.m_pRels);
	m_pOfficeDrawingConverter->ClearShapeTypes();

	nCurPos = m_oBcw.WriteItemStart(c_oSerHdrFtrTypes::HdrFtr_Content);
	oBinaryDocumentTableWriter.WriteDocumentContent(pHdrFtr->m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);

	m_pOfficeDrawingConverter->SetRels(oldRels);
}

		BinarySigTableWriter::BinarySigTableWriter(ParamsWriter& oParamsWriter):m_oBcw(oParamsWriter)
		{
		}
		void BinarySigTableWriter::Write()
		{
			//Write stVersion
			m_oBcw.m_oStream.WriteBYTE(c_oSerSigTypes::Version);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(g_nFormatVersion);
		}

Binary_rPrWriter::Binary_rPrWriter(ParamsWriter& oParamsWriter) : m_oBcw(oParamsWriter), m_oParamsWriter(oParamsWriter)
{
}
void Binary_rPrWriter::Write_rPr(OOX::Logic::CRunProperty* rPr)
{
	if (!rPr) return;

	int nCurPos = 0;
	//Bold
	if(false != rPr->m_oBold.IsInit())
	{
		bool bold = SimpleTypes::onoffTrue == rPr->m_oBold.get().m_oVal.GetValue();
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Bold);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(bold);
	}
	//Italic
	if(false != rPr->m_oItalic.IsInit())
	{
		bool italic = SimpleTypes::onoffTrue == rPr->m_oItalic.get().m_oVal.GetValue();
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Italic);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(italic);
	}
	//Underline
	if(false != rPr->m_oU.IsInit())
	{
		const ComplexTypes::Word::CUnderline& oU = rPr->m_oU.get();
		if(oU.m_oVal.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Underline);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(oU.m_oVal.get().GetValue());
		}
	}
	//Strikeout
	if(false != rPr->m_oStrike.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Strikeout);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == rPr->m_oStrike.get().m_oVal.GetValue());
	}
	//FontFamily
	if(false != rPr->m_oRFonts.IsInit())
	{
		const ComplexTypes::Word::CFonts& oFont = rPr->m_oRFonts.get();
		if(oFont.m_sAscii.IsInit())
			m_oBcw.WriteFont(oFont.m_sAscii.get(), c_oSerProp_rPrType::FontAscii, *m_oParamsWriter.m_pFontProcessor);
		if(oFont.m_oAsciiTheme.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::FontAsciiTheme);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE((BYTE)oFont.m_oAsciiTheme->GetValue());
		}
		if(oFont.m_sHAnsi.IsInit())
			m_oBcw.WriteFont(oFont.m_sHAnsi.get(), c_oSerProp_rPrType::FontHAnsi, *m_oParamsWriter.m_pFontProcessor);
		if(oFont.m_oHAnsiTheme.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::FontHAnsiTheme);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE((BYTE)oFont.m_oHAnsiTheme->GetValue());
		}
		if(oFont.m_sEastAsia.IsInit())
			m_oBcw.WriteFont(oFont.m_sEastAsia.get(), c_oSerProp_rPrType::FontAE, *m_oParamsWriter.m_pFontProcessor);
		if(oFont.m_oEastAsiaTheme.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::FontAETheme);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE((BYTE)oFont.m_oEastAsiaTheme->GetValue());
		}
		if(oFont.m_sCs.IsInit())
			m_oBcw.WriteFont(oFont.m_sCs.get(), c_oSerProp_rPrType::FontCS, *m_oParamsWriter.m_pFontProcessor);
		if(oFont.m_oCsTheme.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::FontCSTheme);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE((BYTE)oFont.m_oCsTheme->GetValue());
		}
		//Hint
		if(false != oFont.m_oHint.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::FontHint);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE((BYTE)oFont.m_oHint->GetValue());
		}
	}
	//FontSize
	if((false != rPr->m_oSz.IsInit()) && (false != rPr->m_oSz.get().m_oVal.IsInit()))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::FontSize);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(rPr->m_oSz.get().m_oVal.get().ToHps());
	}
	//Color
	if(false != rPr->m_oColor.IsInit())
	{
		if(rPr->m_oColor->m_oVal.IsInit())
			m_oBcw.WriteColor(c_oSerProp_rPrType::Color, rPr->m_oColor->m_oVal.get());

		m_oBcw.WriteThemeColor(c_oSerProp_rPrType::ColorTheme, rPr->m_oColor->m_oVal, rPr->m_oColor->m_oThemeColor, rPr->m_oColor->m_oThemeTint, rPr->m_oColor->m_oThemeShade);
	}
	//VertAlign
	if(false != rPr->m_oVertAlign.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::VertAlign);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(rPr->m_oVertAlign.get().m_oVal.get().GetValue());
	}
	//HighLight
	if (false != rPr->m_oHighlight.IsInit() || false != rPr->m_oShd.IsInit())
	{
		if(false != rPr->m_oHighlight.IsInit())
		{
			const ComplexTypes::Word::CHighlight& oHighlight = rPr->m_oHighlight.get();
			if(oHighlight.m_oVal.IsInit())
			{
				const SimpleTypes::CHighlightColor& oHighlightVal = oHighlight.m_oVal.get();
				if(SimpleTypes::highlightcolorNone == oHighlightVal.GetValue())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::HighLightTyped);
					m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
					m_oBcw.m_oStream.WriteBYTE(c_oSer_ColorType::None);
				}
				else
				{
					SimpleTypes::CHexColor oHexColor(oHighlightVal.Get_R(), oHighlightVal.Get_G(), oHighlightVal.Get_B());
					m_oBcw.WriteColor(c_oSerProp_rPrType::HighLight, oHexColor);
				}
			}
		}
	}
	//Shd
	if (false != rPr->m_oShd.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Shd);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteShd(rPr->m_oShd.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//RStyle
	if ((false != rPr->m_oRStyle.IsInit()) && (rPr->m_oRStyle->m_sVal.IsInit()))
	{
		std::wstring sStyleId = *rPr->m_oRStyle->m_sVal;

		if (m_oParamsWriter.m_pEmbeddedStyles)
		{
			std::map<std::wstring, std::wstring>::iterator pFind = m_oParamsWriter.m_pStyles->m_mapEmbeddedStyleNames.back().find(sStyleId);

			if (pFind == m_oParamsWriter.m_pStyles->m_mapEmbeddedStyleNames.back().end())
			{
				sStyleId = m_oParamsWriter.AddEmbeddedStyle(sStyleId);
			}
			else
			{
				sStyleId = pFind->second;
			}

		}
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::RStyle);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(sStyleId);
	}
	//Spacing
	if(false != rPr->m_oSpacing.IsInit() && false != rPr->m_oSpacing->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::SpacingTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(rPr->m_oSpacing->m_oVal->ToTwips());
	}
	//DStrikeout
	if(false != rPr->m_oDStrike.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::DStrikeout);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oDStrike->m_oVal.ToBool());
	}
	//Caps
	if(false != rPr->m_oCaps.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Caps);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oCaps->m_oVal.ToBool());
	}
	//SmallCaps
	if(false != rPr->m_oSmallCaps.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::SmallCaps);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oSmallCaps->m_oVal.ToBool());
	}
	//Position
	if(false != rPr->m_oPosition.IsInit() && false != rPr->m_oPosition->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::PositionHps);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(rPr->m_oPosition->m_oVal->ToHps());
	}
	//BoldCs
	if(rPr->m_oBoldCs.IsInit())
	{
		bool boldCs = SimpleTypes::onoffTrue == rPr->m_oBoldCs.get().m_oVal.GetValue();
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::BoldCs);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(boldCs);
	}
	//ItalicCs
	if(rPr->m_oItalicCs.IsInit())
	{
		bool italicCs = SimpleTypes::onoffTrue == rPr->m_oItalicCs.get().m_oVal.GetValue();
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::ItalicCs);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(italicCs);
	}
	//FontSizeCs
	if (rPr->m_oSzCs.IsInit() && rPr->m_oSzCs->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::FontSizeCs);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(rPr->m_oSzCs.get().m_oVal.get().ToHps());
	}
	//Cs
	if (false != rPr->m_oCs.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Cs);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oCs->m_oVal.ToBool());
	}
	//Rtl
	if (false != rPr->m_oRtL.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Rtl);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oRtL->m_oVal.ToBool());
	}
	//Lang
	if (false != rPr->m_oLang.IsInit())
	{
		if(rPr->m_oLang->m_oVal.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Lang);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
			m_oBcw.m_oStream.WriteStringW(*rPr->m_oLang->m_oVal);
		}
		if(rPr->m_oLang->m_oBidi.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::LangBidi);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
			m_oBcw.m_oStream.WriteStringW(*rPr->m_oLang->m_oBidi);
		}
		if(rPr->m_oLang->m_oEastAsia.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::LangEA);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
			m_oBcw.m_oStream.WriteStringW(*rPr->m_oLang->m_oEastAsia);
		}
	}
	//Vanish
	if (rPr->m_oVanish.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Vanish);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oVanish->m_oVal.ToBool());
	}
	if (rPr->m_oTextOutline.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::TextOutline);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();

		m_oBcw.m_oStream.WriteRecord2(0, rPr->m_oTextOutline);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oTextFill.getType() != OOX::et_Unknown)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::TextFill);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();

		m_oBcw.m_oStream.WriteRecord1(0, rPr->m_oTextFill);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oDel.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Del);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(rPr->m_oDel.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oIns.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Ins);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(rPr->m_oIns.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oRPrChange.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::rPrChange);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		Write_rPrChange(rPr->m_oRPrChange.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(rPr->m_oMoveFrom.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::MoveFrom);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(rPr->m_oMoveFrom.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oMoveTo.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::MoveTo);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(rPr->m_oMoveTo.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oW.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::CompressText);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(rPr->m_oW->m_oVal->GetValue());
	}
	if (rPr->m_oSnapToGrid.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::SnapToGrid);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oSnapToGrid->m_oVal.ToBool());
	}
	if (rPr->m_oLigatures.IsInit() && rPr->m_oLigatures->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Ligatures);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)rPr->m_oLigatures->m_oVal->GetValue());
	}
	if (rPr->m_oNumSpacing.IsInit() && rPr->m_oNumSpacing->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::NumSpacing);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)rPr->m_oNumSpacing->m_oVal->GetValue());
	}
	if (rPr->m_oNumForm.IsInit() && rPr->m_oNumForm->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::NumForm);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)rPr->m_oNumForm->m_oVal->GetValue());
	}
	if (rPr->m_oCntxtAlts.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::CntxtAlts);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr->m_oCntxtAlts->m_oVal.ToBool());
	}	
	if (rPr->m_oShadowExt.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::ShadowExt);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();

		m_oBcw.m_oStream.WriteRecord1(0, *rPr->m_oShadowExt);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oReflection.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Reflection);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();

		m_oBcw.m_oStream.WriteRecord1(0, *rPr->m_oReflection);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oGlow.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Glow);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();

		m_oBcw.m_oStream.WriteRecord1(0, *rPr->m_oGlow);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oProps3d.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Props3d);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();

		m_oBcw.m_oStream.WriteRecord1(0, *rPr->m_oProps3d);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (rPr->m_oScene3d.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rPrType::Scene3d);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();

		m_oBcw.m_oStream.WriteRecord1(0, *rPr->m_oScene3d);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_rPrWriter::Write_rPrChange(const OOX::Logic::CRPrChange& rPrChange)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(rPrChange);
	if(rPrChange.m_pRunPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::rPrChange);
		Write_rPr(rPrChange.m_pRunPr.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}


Binary_pPrWriter::Binary_pPrWriter(ParamsWriter& oParamsWriter, BinaryHeaderFooterTableWriter* oBinaryHeaderFooterTableWriter):
		m_oParamsWriter(oParamsWriter),
		m_oBcw(oParamsWriter), 
		brPrs(oParamsWriter), 
		m_oBinaryHeaderFooterTableWriter(oBinaryHeaderFooterTableWriter)
{
}
void Binary_pPrWriter::Write_pPr(const OOX::Logic::CParagraphProperty& pPr)
{
	int nCurPos = 0;
	//Стили надо писать первыми, потому что применение стиля при открытии уничтажаются настройки параграфа

	std::wstring sStyleId;
	if (false != pPr.m_oPStyle.IsInit())
	{
		sStyleId = pPr.m_oPStyle.get().ToString2();

		if (m_oParamsWriter.m_pEmbeddedStyles)
		{
			std::map<std::wstring, std::wstring>::iterator pFind = m_oParamsWriter.m_pStyles->m_mapEmbeddedStyleNames.back().find(sStyleId);

			if (pFind == m_oParamsWriter.m_pStyles->m_mapEmbeddedStyleNames.back().end())
			{
				sStyleId = m_oParamsWriter.AddEmbeddedStyle(sStyleId);
			}
			else
			{
				sStyleId = pFind->second;
			}

		}
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::ParaStyle);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(sStyleId);

	} 
//KeepNext
	if (false != pPr.m_oKeepNext.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::KeepNext);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == pPr.m_oKeepNext.get().m_oVal.GetValue());
	}
//KeepLines
	if (false != pPr.m_oKeepLines.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::KeepLines);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == pPr.m_oKeepLines.get().m_oVal.GetValue());
	}
//PageBreakBefore
	if (false != pPr.m_oPageBreakBefore.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::PageBreakBefore);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == pPr.m_oPageBreakBefore.get().m_oVal.GetValue());
	}
//FramePr
	if (pPr.m_oFramePr.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::FramePr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteFramePr(pPr.m_oFramePr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//WidowControl
	if (false != pPr.m_oWidowControl.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::WidowControl);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == pPr.m_oWidowControl.get().m_oVal.GetValue());
	}
	//Списки надо писать после стилей, т.к. при открытии в методах добавления списка проверяются стили
	//Списки могут быть заданы с стилях.Это надо учитывать.
//NumPr
	if(pPr.m_oNumPr.IsInit() && (pPr.m_oNumPr->m_oNumID.IsInit() || pPr.m_oNumPr->m_oIlvl.IsInit()))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::numPr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteNumPr(pPr.m_oNumPr.get(), pPr);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (pPr.m_oSuppressLineNumbers.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::SuppressLineNumbers);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pPr.m_oSuppressLineNumbers->m_oVal.ToBool());
	}
//pBdr
	if (false != pPr.m_oPBdr.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::pBdr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WritePBorders(pPr.m_oPBdr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Shd
	if (false != pPr.m_oShd.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Shd);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteShd(pPr.m_oShd.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Tabs
	if (false != pPr.m_oTabs.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Tab);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteTabs(pPr.m_oTabs.get(), pPr.m_oInd);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Spacing
	if (false != pPr.m_oSpacing.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Spacing);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteSpacing(pPr.m_oSpacing.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Ind
	if(false != pPr.m_oInd.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Ind);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteInd(pPr.m_oInd.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//ContextualSpacing
	if (false != pPr.m_oContextualSpacing.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::ContextualSpacing);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == pPr.m_oContextualSpacing.get().m_oVal.GetValue());
	}
//Jc
	if(false != pPr.m_oJc.IsInit())
	{
		const ComplexTypes::Word::CJc& oJc = pPr.m_oJc.get();
		if(oJc.m_oVal.IsInit())
		{
			const SimpleTypes::CJc& oEJc = oJc.m_oVal.get();
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Jc);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			switch(oEJc.GetValue())
			{
			case SimpleTypes::jcCenter: m_oBcw.m_oStream.WriteBYTE(align_Center);break;
			case SimpleTypes::jcStart:
			case SimpleTypes::jcLeft: m_oBcw.m_oStream.WriteBYTE(align_Left);break;
			case SimpleTypes::jcEnd:
			case SimpleTypes::jcRight: m_oBcw.m_oStream.WriteBYTE(align_Right);break;
			case SimpleTypes::jcBoth:
			case SimpleTypes::jcThaiDistribute:
			case SimpleTypes::jcDistribute: m_oBcw.m_oStream.WriteBYTE(align_Justify);break;
			default: m_oBcw.m_oStream.WriteBYTE(align_Left);break;
			}
		}
	}
	if (pPr.m_oOutlineLvl.IsInit() && pPr.m_oOutlineLvl->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::outlineLvl);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*pPr.m_oOutlineLvl->m_oVal);
	}
//pPr_rPr
	if(false != pPr.m_oRPr.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::pPr_rPr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		brPrs.Write_rPr(pPr.m_oRPr.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(pPr.m_oPPrChange.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::pPrChange);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WritePPrChange(pPr.m_oPPrChange.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//SectPr
	if(NULL != m_oBinaryHeaderFooterTableWriter && pPr.m_oSectPr.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::SectPr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos2 = m_oBcw.WriteItemWithLengthStart();
		WriteSectPr(pPr.m_oSectPr.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos2);
	}
}
void Binary_pPrWriter::WritePPrChange(const OOX::Logic::CPPrChange& pPrChange)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(pPrChange);
	if(pPrChange.m_pParPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::pPrChange);
		Write_pPr(pPrChange.m_pParPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_pPrWriter::WriteInd(const ComplexTypes::Word::CInd& Ind)
{
//Left
	if(false != Ind.m_oStart.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Ind_LeftTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(Ind.m_oStart->ToTwips());
	}
//Right
	if(false != Ind.m_oEnd.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Ind_RightTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(Ind.m_oEnd->ToTwips());
	}
//FirstLine
	if(false != Ind.m_oFirstLine.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Ind_FirstLineTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(Ind.m_oFirstLine->ToTwips());
	}
	else if(false != Ind.m_oHanging.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Ind_FirstLineTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG( -1 * Ind.m_oHanging->ToTwips());
	}
}
void Binary_pPrWriter::WriteSpacing(const ComplexTypes::Word::CSpacing& Spacing)
{
//LineRule
	SimpleTypes::ELineSpacingRule eLineSpacingRule = SimpleTypes::linespacingruleAuto;
	if(false != Spacing.m_oLineRule.IsInit())
		eLineSpacingRule = Spacing.m_oLineRule->GetValue();

//Line
	if(false != Spacing.m_oLine.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Spacing_LineTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(Spacing.m_oLine->ToTwips());
//Write LineRule
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Spacing_LineRule);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		switch(eLineSpacingRule)
		{
		case SimpleTypes::linespacingruleAtLeast: m_oBcw.m_oStream.WriteBYTE(linerule_AtLeast);break;
		case SimpleTypes::linespacingruleExact: m_oBcw.m_oStream.WriteBYTE(linerule_Exact);break;
		default:m_oBcw.m_oStream.WriteBYTE(linerule_Auto);break;
		}
	}
//Before
	if(Spacing.m_oBeforeAutospacing.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Spacing_BeforeAuto);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(Spacing.m_oBeforeAutospacing->ToBool());
	}
	if(false != Spacing.m_oBefore.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Spacing_BeforeTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(Spacing.m_oBefore->ToTwips());
	}
//After
	if(Spacing.m_oAfterAutospacing.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Spacing_AfterAuto);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(Spacing.m_oAfterAutospacing->ToBool());
	}
	if(false != Spacing.m_oAfter.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Spacing_AfterTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(Spacing.m_oAfter->ToTwips());
	}
}
void Binary_pPrWriter::WriteTabs(const OOX::Logic::CTabs& Tab, const nullable<ComplexTypes::Word::CInd>& oInd)
{
	int nCurPos = 0;
//Len
	for(size_t i = 0, length = Tab.m_arrTabs.size(); i < length; ++i)
	{
		const ComplexTypes::Word::CTabStop& tabItem = *Tab.m_arrTabs[i];
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Tab_Item);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteTabItem(tabItem, oInd);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_pPrWriter::WriteTabItem(const ComplexTypes::Word::CTabStop& TabItem, const nullable<ComplexTypes::Word::CInd>& oInd)
{
//type
	bool bRight = false;
	if(false != TabItem.m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Tab_Item_Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(TabItem.m_oVal->GetValue());
	}
//pos
	if(false != TabItem.m_oPos.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Tab_Item_PosTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(TabItem.m_oPos->ToTwips());
	}
	if(false != TabItem.m_oLeader.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::Tab_Item_Leader);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)TabItem.m_oLeader->GetValue());
	}
}
void Binary_pPrWriter::WriteNumPr(const OOX::Logic::CNumPr& numPr, const OOX::Logic::CParagraphProperty& pPr)
{
	int nCurPos = 0, listNum = numPr.m_oNumID.IsInit() ? numPr.m_oNumID->m_oVal.get_value_or(0) : -1;
	
	if (m_oParamsWriter.m_pEmbeddedNumbering && listNum >= 0)
	{
		std::map<int, int>::iterator pFind = m_oParamsWriter.m_pNumbering->m_mapEmbeddedNames.back().find(listNum);

		if (pFind == m_oParamsWriter.m_pNumbering->m_mapEmbeddedNames.back().end())
		{
			if ((m_oParamsWriter.m_pEmbeddedNumbering->m_arrNum[listNum - 1]) && (m_oParamsWriter.m_pEmbeddedNumbering->m_arrNum[listNum - 1]->m_oNumId.IsInit()))
			{
				int abstractNumId = 0;
				if ((m_oParamsWriter.m_pEmbeddedNumbering->m_arrNum[listNum - 1]) && (m_oParamsWriter.m_pEmbeddedNumbering->m_arrNum[listNum - 1]->m_oAbstractNumId.IsInit()) &&
					(m_oParamsWriter.m_pEmbeddedNumbering->m_arrNum[listNum - 1]->m_oAbstractNumId->m_oVal.IsInit()))
				{
					abstractNumId = *m_oParamsWriter.m_pEmbeddedNumbering->m_arrNum[listNum - 1]->m_oAbstractNumId->m_oVal;
				}
				OOX::Numbering::CAbstractNum *abstractNum = NULL;
				
				if (abstractNumId < m_oParamsWriter.m_pEmbeddedNumbering->m_arrAbstractNum.size())
				{
					abstractNum = m_oParamsWriter.m_pEmbeddedNumbering->m_arrAbstractNum[abstractNumId];
				}
				if (abstractNum)
				{
					abstractNumId = m_oParamsWriter.m_pNumbering->m_arrAbstractNum.size();
					m_oParamsWriter.m_pNumbering->m_arrAbstractNum.push_back(new OOX::Numbering::CAbstractNum());

					abstractNum->copy(m_oParamsWriter.m_pNumbering->m_arrAbstractNum.back());
					m_oParamsWriter.m_pNumbering->m_arrAbstractNum.back()->m_oAbstractNumId = abstractNumId;

					int listNumNew = m_oParamsWriter.m_pNumbering->m_arrNum.size() + 1;
					m_oParamsWriter.m_pNumbering->m_mapEmbeddedNames.back().insert(std::make_pair(listNum, listNumNew));

					m_oParamsWriter.m_pNumbering->m_arrNum.push_back(new OOX::Numbering::CNum());
					
					m_oParamsWriter.m_pNumbering->m_arrNum.back()->m_oNumId = listNumNew;
					
					m_oParamsWriter.m_pNumbering->m_arrNum.back()->m_oAbstractNumId.Init();
					m_oParamsWriter.m_pNumbering->m_arrNum.back()->m_oAbstractNumId->m_oVal = abstractNumId;
					
					listNum = listNumNew;
				}
			}		
		}
		else
		{
			listNum = pFind->second;
		}
	}
	
	if (listNum >= 0)
	{
	//pos	
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::numPr_id);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(listNum);
	}
	if (false != numPr.m_oIlvl.IsInit())
	{
		const ComplexTypes::Word::CDecimalNumber& oCurLvl = numPr.m_oIlvl.get();
		if (oCurLvl.m_oVal.IsInit())
		{
	//type
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::numPr_lvl);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(*oCurLvl.m_oVal);
		}
	}
	if (numPr.m_oIns.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_pPrType::numPr_Ins);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(numPr.m_oIns.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_pPrWriter::WriteFramePr(const ComplexTypes::Word::CFramePr& oFramePr)
{
	if(oFramePr.m_oDropCap.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::DropCap);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFramePr.m_oDropCap->GetValue());
	}
	if(oFramePr.m_oH.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::H);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFramePr.m_oH->ToTwips());
	}
	if(oFramePr.m_oHAnchor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::HAnchor);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFramePr.m_oHAnchor->GetValue());
	}
	if(oFramePr.m_oHRule.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::HRule);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFramePr.m_oHRule->GetValue());
	}
	if(oFramePr.m_oHSpace.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::HSpace);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFramePr.m_oHSpace->ToTwips());
	}
	if(oFramePr.m_oLines.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::Lines);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFramePr.m_oLines->GetValue());
	}
	if(oFramePr.m_oVAnchor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::VAnchor);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFramePr.m_oVAnchor->GetValue());
	}
	if(oFramePr.m_oVSpace.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::VSpace);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFramePr.m_oVSpace->ToTwips());
	}
	if(oFramePr.m_oW.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::W);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFramePr.m_oW->ToTwips());
	}
	if(oFramePr.m_oWrap.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::Wrap);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFramePr.m_oWrap->GetValue());
	}
	if(oFramePr.m_oX.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::X);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFramePr.m_oX->ToTwips());
	}
	if(oFramePr.m_oXAlign.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::XAlign);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFramePr.m_oXAlign->GetValue());
	}
	if(oFramePr.m_oY.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::Y);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFramePr.m_oY->ToTwips());
	}
	if(oFramePr.m_oYAlign.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_FramePrType::YAlign);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFramePr.m_oYAlign->GetValue());
	}
}
void Binary_pPrWriter::WriteSectPr (OOX::Logic::CSectionProperty* pSectPr)
{
	if (pSectPr == NULL) return;

	int nCurPos = 0;
//pgSz
	nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::pgSz);
		WritePageSize(pSectPr);
	m_oBcw.WriteItemEnd(nCurPos);
//pgMar
	nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::pgMar);
		WritePageMargin(pSectPr);
	m_oBcw.WriteItemEnd(nCurPos);
//titlePg
	nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::setting);
		WritePageSettings(pSectPr);
	m_oBcw.WriteItemEnd(nCurPos);
//Header
	if(false == pSectPr->m_arrHeaderReference.empty())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::headers);
			WriteHeaderFooter(pSectPr, pSectPr->m_arrHeaderReference, true);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Footer
	if(false == pSectPr->m_arrFooterReference.empty())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::footers);
			WriteHeaderFooter(pSectPr, pSectPr->m_arrFooterReference, false);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oPgNumType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::pageNumType);
			WritePageNumType(pSectPr->m_oPgNumType.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oLnNumType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::lnNumType);
			WriteLineNumType(pSectPr->m_oLnNumType.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oSectPrChange.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::sectPrChange);
			WriteSectPrChange(pSectPr->m_oSectPrChange.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oCols.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::cols);
			WriteColumns(pSectPr->m_oCols.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oPgBorders.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::pgBorders);
			WritePageBorders(pSectPr->m_oPgBorders.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oFootnotePr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::footnotePr);
			WriteNotePr(pSectPr->m_oFootnotePr->m_oNumFmt, pSectPr->m_oFootnotePr->m_oNumRestart, pSectPr->m_oFootnotePr->m_oNumStart,
					&pSectPr->m_oFootnotePr->m_oPos, NULL, NULL);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oEndnotePr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::endnotePr);
			WriteNotePr(pSectPr->m_oEndnotePr->m_oNumFmt, pSectPr->m_oEndnotePr->m_oNumRestart, pSectPr->m_oEndnotePr->m_oNumStart,
					NULL, &pSectPr->m_oEndnotePr->m_oPos, NULL);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pSectPr->m_oRtlGutter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::rtlGutter);
			m_oBcw.m_oStream.WriteBOOL(pSectPr->m_oRtlGutter->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void Binary_pPrWriter::WritePageSettings(OOX::Logic::CSectionProperty* pSectPr)
{
	bool titlePg = false;
	bool EvenAndOddHeaders = false;
	if (NULL != pSectPr && pSectPr->m_oTitlePg.IsInit() && SimpleTypes::onoffTrue == pSectPr->m_oTitlePg->m_oVal.GetValue())
		titlePg = true;
	
	if (m_oParamsWriter.m_pSettings && m_oParamsWriter.m_pSettings->m_oEvenAndOddHeaders.IsInit() && SimpleTypes::onoffTrue == m_oParamsWriter.m_pSettings->m_oEvenAndOddHeaders.get().m_oVal.GetValue())
		EvenAndOddHeaders = true;
//titlePg
	m_oBcw.m_oStream.WriteBYTE(c_oSerProp_secPrSettingsType::titlePg);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
	m_oBcw.m_oStream.WriteBOOL(titlePg);
//EvenAndOddHeaders
	m_oBcw.m_oStream.WriteBYTE(c_oSerProp_secPrSettingsType::EvenAndOddHeaders);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
	m_oBcw.m_oStream.WriteBOOL(EvenAndOddHeaders);

	if(pSectPr->m_oType.IsInit() && pSectPr->m_oType->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_secPrSettingsType::SectionType);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pSectPr->m_oType->m_oVal->GetValue());
	}
}
void Binary_pPrWriter::WritePageSize(OOX::Logic::CSectionProperty* pSectPr)
{
	if(NULL != pSectPr && pSectPr->m_oPgSz.IsInit())
	{
		const ComplexTypes::Word::CPageSz& pSz = pSectPr->m_oPgSz.get();
		if(pSz.m_oW.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgSzType::WTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pSz.m_oW->ToTwips());
		}
		if(pSz.m_oH.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgSzType::HTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pSz.m_oH->ToTwips());
		}
		if(pSz.m_oOrient.IsInit())
		{
			BYTE Orientation = orientation_Portrait;
			switch(pSz.m_oOrient.get().GetValue())
			{
			case SimpleTypes::pageorientPortrait: Orientation = orientation_Portrait;break;
			case SimpleTypes::pageorientLandscape: Orientation = orientation_Landscape;break;
			default: break;
			}
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgSzType::Orientation);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(Orientation);
		}
	}
}
void Binary_pPrWriter::WritePageMargin(OOX::Logic::CSectionProperty* pSectPr)
{
	if(NULL != pSectPr && pSectPr->m_oPgSz.IsInit() && pSectPr->m_oPgMar.IsInit())
	{
		const ComplexTypes::Word::CPageMar& pMar = pSectPr->m_oPgMar.get();
		if(pMar.m_oLeft.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgMarType::LeftTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMar.m_oLeft->ToTwips());
		}
		if(pMar.m_oTop.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgMarType::TopTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMar.m_oTop->ToTwips());
		}
		if(pMar.m_oRight.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgMarType::RightTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMar.m_oRight->ToTwips());
		}
		if(pMar.m_oBottom.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgMarType::BottomTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMar.m_oBottom->ToTwips());
		}
		if(pMar.m_oHeader.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgMarType::HeaderTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMar.m_oHeader->ToTwips());
		}
		if(pMar.m_oFooter.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgMarType::FooterTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMar.m_oFooter->ToTwips());
		}
		if(pMar.m_oGutter.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_pgMarType::GutterTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMar.m_oGutter->ToTwips());
		}
	}				
}
void Binary_pPrWriter::WriteHeaderFooter(OOX::Logic::CSectionProperty* pSectPr, std::vector<ComplexTypes::Word::CHdrFtrRef*>& aRefs, bool bHdr)
{
	int nCurPos = 0;
	for (size_t i = 0, length = aRefs.size(); i < length; ++i)
	{
		const ComplexTypes::Word::CHdrFtrRef& oRef = *aRefs[i];
		if( oRef.m_oType.IsInit() && oRef.m_oId.IsInit())
		{
			int nIndex = 0;
			OOX::CHdrFtr* pHdrFtr = NULL;
			
			smart_ptr<OOX::File> oFile = m_oBinaryHeaderFooterTableWriter->m_oDocumentRelsWriter->Find(oRef.m_oId->GetValue());
			if (oFile.IsInit() && (OOX::FileTypes::Header == oFile->type() || OOX::FileTypes::Footer == oFile->type()))
			{
				pHdrFtr = (OOX::CHdrFtr*)oFile.GetPointer();
			}

			if (pHdrFtr)
			{
				if(bHdr)
				{
					nIndex = (int)m_oBinaryHeaderFooterTableWriter->m_aHeaders.size();
					m_oBinaryHeaderFooterTableWriter->m_aHeaders.push_back(pHdrFtr);
					m_oBinaryHeaderFooterTableWriter->m_aHeaderTypes.push_back(oRef.m_oType->GetValue());
					m_oBinaryHeaderFooterTableWriter->m_aHeaderSectPrs.push_back(pSectPr);
				}
				else
				{
					nIndex = (int)m_oBinaryHeaderFooterTableWriter->m_aFooters.size();
					m_oBinaryHeaderFooterTableWriter->m_aFooters.push_back(pHdrFtr);
					m_oBinaryHeaderFooterTableWriter->m_aFooterTypes.push_back(oRef.m_oType->GetValue());
					m_oBinaryHeaderFooterTableWriter->m_aFooterSectPrs.push_back(pSectPr);
				}
				nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrType::hdrftrelem);
				m_oBcw.m_oStream.WriteLONG(nIndex);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
}
void Binary_pPrWriter::WritePageNumType(const ComplexTypes::Word::CPageNumber& pPageNumber)
{
	int nCurPos = 0;
	if(pPageNumber.m_oStart.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrPageNumType::start);
		m_oBcw.m_oStream.WriteLONG(pPageNumber.m_oStart->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void Binary_pPrWriter::WriteLineNumType(const ComplexTypes::Word::CLineNumber& pLineNumber)
{
	int nCurPos = 0;
	if(pLineNumber.m_oCountBy.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrLineNumType::CountBy);
		m_oBcw.m_oStream.WriteLONG(pLineNumber.m_oCountBy->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pLineNumber.m_oDistance.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrLineNumType::Distance);
		m_oBcw.m_oStream.WriteLONG(pLineNumber.m_oDistance->ToTwips());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pLineNumber.m_oRestart.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrLineNumType::Restart);
		m_oBcw.m_oStream.WriteBYTE(pLineNumber.m_oRestart->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pLineNumber.m_oStart.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_secPrLineNumType::Start);
		m_oBcw.m_oStream.WriteLONG(pLineNumber.m_oStart->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void Binary_pPrWriter::WriteSectPrChange(const OOX::Logic::CSectPrChange& sectPrChange)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(sectPrChange);
	if(sectPrChange.m_pSecPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::sectPrChange);
		WriteSectPr(sectPrChange.m_pSecPr.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_pPrWriter::WriteColumns(const OOX::Logic::CColumns& columns)
{
	int nCurPos = 0;

	if(columns.m_oEqualWidth.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_Columns::EqualWidth);
		m_oBcw.m_oStream.WriteBOOL(columns.m_oEqualWidth->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(columns.m_oNum.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_Columns::Num);
		m_oBcw.m_oStream.WriteLONG(columns.m_oNum->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(columns.m_oSep.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_Columns::Sep);
		m_oBcw.m_oStream.WriteBOOL(columns.m_oSep->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(columns.m_oSpace.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_Columns::Space);
		m_oBcw.m_oStream.WriteLONG(columns.m_oSpace->ToTwips());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	for(size_t i = 0; i < columns.m_arrColumns.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_Columns::Column);
		WriteColumn(*columns.m_arrColumns[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_pPrWriter::WriteColumn(const ComplexTypes::Word::CColumn& column)
{
	int nCurPos = 0;

	if(column.m_oSpace.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_Columns::ColumnSpace);
		m_oBcw.m_oStream.WriteLONG(column.m_oSpace->ToTwips());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(column.m_oW.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_Columns::ColumnW);
		m_oBcw.m_oStream.WriteLONG(column.m_oW->ToTwips());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_pPrWriter::WritePageBorders(const OOX::Logic::CPageBorders& PageBorders)
{
	int nCurPos = 0;
	if(PageBorders.m_oDisplay.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerPageBorders::Display);
		m_oBcw.m_oStream.WriteBYTE(PageBorders.m_oDisplay->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(PageBorders.m_oOffsetFrom.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerPageBorders::OffsetFrom);
		m_oBcw.m_oStream.WriteBYTE(PageBorders.m_oOffsetFrom->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(PageBorders.m_oZOrder.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerPageBorders::ZOrder);
		m_oBcw.m_oStream.WriteBYTE(PageBorders.m_oZOrder->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(PageBorders.m_oBottom.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerPageBorders::Bottom);
		WritePageBorder(PageBorders.m_oBottom.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(PageBorders.m_oLeft.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerPageBorders::Left);
		WritePageBorder(PageBorders.m_oLeft.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(PageBorders.m_oRight.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerPageBorders::Right);
		WritePageBorder(PageBorders.m_oRight.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(PageBorders.m_oTop.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerPageBorders::Top);
		WritePageBorder(PageBorders.m_oTop.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_pPrWriter::WritePageBorder(const ComplexTypes::Word::CPageBorder& pageBorder)
{
	int nCurPos = 0;
	if(pageBorder.m_oColor.IsInit())
	{
		m_oBcw.WriteColor(c_oSerPageBorders::Color, pageBorder.m_oColor.get());
	}
	m_oBcw.WriteThemeColor(c_oSerPageBorders::ColorTheme, pageBorder.m_oColor, pageBorder.m_oThemeColor, pageBorder.m_oThemeTint, pageBorder.m_oThemeShade);
	if(pageBorder.m_oSpace.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPageBorders::Space);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pageBorder.m_oSpace->GetValue());
	}
	if(pageBorder.m_oSz.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPageBorders::Sz);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pageBorder.m_oSz->GetValue());
	}
	if(pageBorder.m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPageBorders::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pageBorder.m_oVal.get().GetValue());
	}
	if(pageBorder.m_oFrame.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPageBorders::Frame);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pageBorder.m_oFrame->ToBool());
	}
	if(pageBorder.m_oShadow.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPageBorders::Shadow);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pageBorder.m_oShadow->ToBool());
	}
	//todo id
}
void Binary_pPrWriter::WriteNotePr(const nullable<ComplexTypes::Word::CNumFmt>& numFmt, const nullable<ComplexTypes::Word::CNumRestart>& numRestart,
					 const nullable<ComplexTypes::Word::CDecimalNumber>& numStart, nullable<ComplexTypes::Word::CFtnPos>* ftnPos,
					 nullable<ComplexTypes::Word::CEdnPos>* endPos, std::vector<OOX::CFtnEdnSepRef*>* refs)
{
	int nCurPos = 0;
	if(numFmt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::PrFmt);
		WriteNumFmt(numFmt.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(numRestart.IsInit() && numRestart->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::PrRestart);
		m_oBcw.m_oStream.WriteBYTE(numRestart->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(numStart.IsInit() && numStart->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::PrStart);
		m_oBcw.m_oStream.WriteLONG(*numStart->m_oVal);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(NULL != ftnPos && ftnPos->IsInit() && (*ftnPos)->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::PrFntPos);
		m_oBcw.m_oStream.WriteBYTE((*ftnPos)->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(NULL != endPos && endPos->IsInit() && (*endPos)->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::PrEndPos);
		m_oBcw.m_oStream.WriteBYTE((*endPos)->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(NULL != refs)
	{
		for(size_t i = 0; i < refs->size(); ++i)
		{
			OOX::CFtnEdnSepRef* pRef = (*refs)[i];
			if(NULL != pRef && pRef->m_oId.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::PrRef);
				m_oBcw.m_oStream.WriteLONG(pRef->m_oId->GetValue());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
	}
}
void Binary_pPrWriter::WriteNumFmt(const ComplexTypes::Word::CNumFmt& oNumFmt)
{
	int nCurPos = 0;
	if(oNumFmt.m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::NumFmtVal);
		m_oBcw.m_oStream.WriteBYTE(oNumFmt.m_oVal->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oNumFmt.m_sFormat.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart	(c_oSerNumTypes::NumFmtFormat);
		m_oBcw.m_oStream.WriteStringW3	(*oNumFmt.m_sFormat);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}

	
Binary_tblPrWriter::Binary_tblPrWriter(ParamsWriter& oParamsWriter):m_oBcw(oParamsWriter)
{
}
void Binary_tblPrWriter::WriteTblPr(OOX::Logic::CTableProperty* p_tblPr)
{
	OOX::Logic::CTableProperty& tblPr = *p_tblPr;
	int nCurPos = 0;
	if(tblPr.m_oTblStyleRowBandSize.IsInit() && tblPr.m_oTblStyleRowBandSize->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::RowBandSize);
		m_oBcw.m_oStream.WriteLONG(*tblPr.m_oTblStyleRowBandSize->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(tblPr.m_oTblStyleColBandSize.IsInit() && tblPr.m_oTblStyleColBandSize->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::ColBandSize);
		m_oBcw.m_oStream.WriteLONG(*tblPr.m_oTblStyleColBandSize->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Jc
	if(false != tblPr.m_oJc.IsInit() && tblPr.m_oJc->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::Jc);
		switch(tblPr.m_oJc->m_oVal->GetValue())
		{
		case SimpleTypes::jctableCenter:m_oBcw.m_oStream.WriteBYTE(align_Center);break;
		case SimpleTypes::jctableEnd:
		case SimpleTypes::jctableRight:m_oBcw.m_oStream.WriteBYTE(align_Right);break;
		case SimpleTypes::jctableStart:
		case SimpleTypes::jctableLeft:m_oBcw.m_oStream.WriteBYTE(align_Left);break;
		default:m_oBcw.m_oStream.WriteBYTE(align_Left);break;
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TableInd
	if(tblPr.m_oTblInd.IsInit())
	{
		if(tblPr.m_oTblInd->m_oW.IsInit() && false == tblPr.m_oTblInd->m_oW->IsPercent() &&
			tblPr.m_oTblInd->m_oType.IsInit() && SimpleTypes::tblwidthDxa == tblPr.m_oTblInd->m_oType->GetValue())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::TableIndTwips);
			m_oBcw.m_oStream.WriteLONG(tblPr.m_oTblInd->m_oW->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	//TableW
	if(tblPr.m_oTblW.IsInit())
		WriteW(c_oSerProp_tblPrType::TableW, tblPr.m_oTblW.get());
	//TableCellMar
	if(tblPr.m_oTblCellMar.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::TableCellMar);
		WriteTblMar(tblPr.m_oTblCellMar.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TableBorders
	if(tblPr.m_oTblBorders.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::TableBorders);
		m_oBcw.WriteTblBorders(tblPr.m_oTblBorders.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Shd
	if(tblPr.m_oShade.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::Shd);
		m_oBcw.WriteShd(tblPr.m_oShade.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(tblPr.m_oTblStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_tblPrType::Style);
		m_oBcw.m_oStream.WriteStringW(tblPr.m_oTblStyle->ToString2());
	}
	//Look
	if(tblPr.m_oTblLook.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::Look);
		m_oBcw.m_oStream.WriteLONG(tblPr.m_oTblLook->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Layout
	if(tblPr.m_oTblLayout.IsInit() && tblPr.m_oTblLayout->m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::Layout);
		m_oBcw.m_oStream.WriteBYTE((BYTE)tblPr.m_oTblLayout->m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//tblpPr
	if(tblPr.m_oTblpPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::tblpPr2);
		Write_tblpPr(tblPr.m_oTblpPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(tblPr.m_oTblPrChange.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::tblPrChange);
		WriteTblPrChange(tblPr.m_oTblPrChange.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(tblPr.m_oTblCellSpacing.IsInit())
	{
		const ComplexTypes::Word::CTblWidth& cs = tblPr.m_oTblCellSpacing.get();
		if(cs.m_oW.IsInit() && false == cs.m_oW->IsPercent() &&
			cs.m_oType.IsInit() && SimpleTypes::tblwidthDxa == cs.m_oType->GetValue())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::TableCellSpacingTwips);
			m_oBcw.m_oStream.WriteLONG(cs.m_oW->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	if(tblPr.m_oTblCaption.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_tblPrType::tblCaption);
		m_oBcw.m_oStream.WriteStringW(tblPr.m_oTblCaption->ToString2());
	}
	if(tblPr.m_oTblDescription.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_tblPrType::tblDescription);
		m_oBcw.m_oStream.WriteStringW(tblPr.m_oTblDescription->ToString2());
	}
	if (tblPr.m_oTblOverlap.IsInit() && tblPr.m_oTblOverlap->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblPrType::tblOverlap);
		m_oBcw.m_oStream.WriteBYTE((BYTE)tblPr.m_oTblOverlap->m_oVal->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteTblMar(const OOX::Logic::CTblCellMar& cellMar)
{
	int nCurPos = 0;
	//Left
	if(cellMar.m_oStart.IsInit())
		WriteW(c_oSerMarginsType::left, cellMar.m_oStart.get());
	//Top
	if(cellMar.m_oTop.IsInit())
		WriteW(c_oSerMarginsType::top, cellMar.m_oTop.get());
	//Right
	if(cellMar.m_oEnd.IsInit())
		WriteW(c_oSerMarginsType::right, cellMar.m_oEnd.get());
	//Bottom
	if(cellMar.m_oBottom.IsInit())
		WriteW(c_oSerMarginsType::bottom, cellMar.m_oBottom.get());
}
void Binary_tblPrWriter::WriteCellMar(const OOX::Logic::CTcMar& cellMar)
{
	int nCurPos = 0;
	//Left
	if(cellMar.m_oStart.IsInit())
		WriteW(c_oSerMarginsType::left, cellMar.m_oStart.get());
	//Top
	if(cellMar.m_oTop.IsInit())
		WriteW(c_oSerMarginsType::top, cellMar.m_oTop.get());
	//Right
	if(cellMar.m_oEnd.IsInit())
		WriteW(c_oSerMarginsType::right, cellMar.m_oEnd.get());
	//Bottom
	if(cellMar.m_oBottom.IsInit())
		WriteW(c_oSerMarginsType::bottom, cellMar.m_oBottom.get());
}
void Binary_tblPrWriter::Write_tblpPr(const ComplexTypes::Word::CTblPPr& pr)
{
	int nCurPos = 0;
	if(pr.m_oHorzAnchor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_tblpPrType2::HorzAnchor);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pr.m_oHorzAnchor->GetValue());
	}
	if(pr.m_oTblpX.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_tblpPrType2::TblpXTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pr.m_oTblpX->ToTwips());
	}
	if(pr.m_oTblpXSpec.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_tblpPrType2::TblpXSpec);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pr.m_oTblpXSpec->GetValue());
	}
	if(pr.m_oVertAnchor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_tblpPrType2::VertAnchor);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pr.m_oVertAnchor->GetValue());
	}
	if(pr.m_oTblpY.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_tblpPrType2::TblpYTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pr.m_oTblpY->ToTwips());
	}
	if(pr.m_oTblpYSpec.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_tblpPrType2::TblpYSpec);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pr.m_oTblpYSpec->GetValue());
	}
	if(pr.m_oLeftFromText.IsInit() || pr.m_oTopFromText.IsInit() || pr.m_oRightFromText.IsInit() || pr.m_oBottomFromText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_tblpPrType2::Paddings);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WritePaddings(pr.m_oLeftFromText, pr.m_oTopFromText, pr.m_oRightFromText, pr.m_oBottomFromText);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteRowPr(const OOX::Logic::CTableRowProperties& rowPr)
{
	int nCurPos = 0;
	//CantSplit
	if(rowPr.m_oCantSplit.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::CantSplit);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL( SimpleTypes::onoffTrue == rowPr.m_oCantSplit->m_oVal.GetValue());
	}
	//After
	if(rowPr.m_oGridAfter.IsInit() || rowPr.m_oWAfter.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::After);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteAfter(rowPr.m_oGridAfter, rowPr.m_oWAfter);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Before
	if(rowPr.m_oGridBefore.IsInit() || rowPr.m_oWBefore.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::Before);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteBefore(rowPr.m_oGridBefore, rowPr.m_oWBefore);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Jc
	if(rowPr.m_oJc.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::Jc);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		switch(rowPr.m_oJc->m_oVal->GetValue())
		{
		case SimpleTypes::jctableCenter:m_oBcw.m_oStream.WriteBYTE(align_Center);break;
		case SimpleTypes::jctableEnd:
		case SimpleTypes::jctableRight:m_oBcw.m_oStream.WriteBYTE(align_Right);break;
		case SimpleTypes::jctableStart:
		case SimpleTypes::jctableLeft:m_oBcw.m_oStream.WriteBYTE(align_Left);break;
		default:m_oBcw.m_oStream.WriteBYTE(align_Left);break;
		}
	}
	//TableCellSpacing
	if(rowPr.m_oTblCellSpacing.IsInit())
	{
		const ComplexTypes::Word::CTblWidth& cs = rowPr.m_oTblCellSpacing.get();
		if(cs.m_oW.IsInit() && false == cs.m_oW->IsPercent() &&
			cs.m_oType.IsInit() && SimpleTypes::tblwidthDxa == cs.m_oType->GetValue())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::TableCellSpacingTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(cs.m_oW->GetValue());
		}
	}
	//Height
	if(rowPr.m_oTblHeight.IsInit() && rowPr.m_oTblHeight->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::Height);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteRowHeight(rowPr.m_oTblHeight.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//TableHeader
	if(rowPr.m_oTblHeader.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::TableHeader);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rowPr.m_oTblHeader->m_oVal.ToBool());
	}
	if(rowPr.m_oDel.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::Del);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(rowPr.m_oDel.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(rowPr.m_oIns.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::Ins);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(rowPr.m_oIns.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(rowPr.m_oTrPrChange.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::trPrChange);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteTrPrChange(rowPr.m_oTrPrChange.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteAfter(const nullable<ComplexTypes::Word::CDecimalNumber>& GridAfter, const nullable<ComplexTypes::Word::CTblWidth>& WAfter)
{
	int nCurPos = 0;
	//GridAfter
	if(GridAfter.IsInit() && GridAfter->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::GridAfter);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*GridAfter->m_oVal);
	}
	//WAfter
	if(WAfter.IsInit())
	{
		WriteW(c_oSerProp_rowPrType::WAfter, WAfter.get(), true);
	}
}
void Binary_tblPrWriter::WriteBefore(const nullable<ComplexTypes::Word::CDecimalNumber>& GridBefore, const nullable<ComplexTypes::Word::CTblWidth>& WBefore)
{
	int nCurPos = 0;
	//GridBefore
	if(GridBefore.IsInit() && GridBefore->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::GridBefore);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*GridBefore->m_oVal);
	}
	//WBefore
	if(WBefore.IsInit())
	{
		WriteW(c_oSerProp_rowPrType::WBefore, WBefore.get(), true);
	}
}
void Binary_tblPrWriter::WriteRowHeight(const ComplexTypes::Word::CHeight& rowHeight)
{
	if(rowHeight.m_oVal.IsInit())
	{
		//HRule
		//хотя по спецификации отсутсвие hRule долно трактоваться как auto(17.4.81  trHeight (Table Row Height))
		//word трактует <w:trHeight w:val="542"/> как AtLeast
		SimpleTypes::EHeightRule eHRule = SimpleTypes::heightruleAtLeast;
		if(rowHeight.m_oHRule.IsInit())
			eHRule = rowHeight.m_oHRule->GetValue();

		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::Height_Rule);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(eHRule);


		//Value
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_rowPrType::Height_ValueTwips);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(rowHeight.m_oVal->ToTwips());
	}
}
void Binary_tblPrWriter::WriteW(int nType, const ComplexTypes::Word::CTblWidth& tableW, bool bWrite2)
{
	int nCurPos = 0;
	if(tableW.m_oW.IsInit() && tableW.m_oType.IsInit())
	{
		if(nType >= 0)
		{
			if(true == bWrite2)
			{
				m_oBcw.m_oStream.WriteBYTE(nType);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
			}
			else
				nCurPos = m_oBcw.WriteItemStart(nType);
		}
		else
		{
			if(true == bWrite2)
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
			nCurPos = m_oBcw.WriteItemWithLengthStart();
		}

		//Type
		if(false != tableW.m_oType.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerWidthType::Type);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			switch(tableW.m_oType->GetValue())
			{
			case SimpleTypes::tblwidthAuto:m_oBcw.m_oStream.WriteBYTE(tblwidth_Auto);break;
			case SimpleTypes::tblwidthDxa:m_oBcw.m_oStream.WriteBYTE(tblwidth_Mm);break;
			case SimpleTypes::tblwidthNil:m_oBcw.m_oStream.WriteBYTE(tblwidth_Nil);break;
			case SimpleTypes::tblwidthPct:m_oBcw.m_oStream.WriteBYTE(tblwidth_Pct);break;
			default:m_oBcw.m_oStream.WriteBYTE(tblwidth_Auto);break;
			}

		}
		//W
		if(false != tableW.m_oW.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerWidthType::WDocx);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(tableW.m_oW->GetValueIntegerPercent());
		}
		if(nType >= 0)
			m_oBcw.WriteItemEnd(nCurPos);
		else
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteCellPr(OOX::Logic::CTableCellProperties* cellPr)
{
	if (!cellPr) return;

	int nCurPos = 0;
	//TableCellW
	if (cellPr->m_oTcW.IsInit())
	{
		WriteW(c_oSerProp_cellPrType::TableCellW, cellPr->m_oTcW.get(), true);
	}
	//GridSpan
	if (cellPr->m_oGridSpan.IsInit() && cellPr->m_oGridSpan->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::GridSpan);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*cellPr->m_oGridSpan->m_oVal);
	}
	//VMerge
	if (cellPr->m_oVMerge.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::VMerge);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		if (cellPr->m_oVMerge->m_oVal.IsInit())
		{
			switch (cellPr->m_oVMerge->m_oVal->GetValue())
			{
			case SimpleTypes::mergeContinue: m_oBcw.m_oStream.WriteBYTE(vmerge_Continue); break;
			case SimpleTypes::mergeRestart: m_oBcw.m_oStream.WriteBYTE(vmerge_Restart); break;
			default: m_oBcw.m_oStream.WriteBYTE(vmerge_Continue); break;
			}
		}
		else
		{
			m_oBcw.m_oStream.WriteBYTE(vmerge_Continue);
		}
	}
	if (cellPr->m_oHMerge.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::HMerge);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		if (cellPr->m_oHMerge->m_oVal.IsInit())
		{
			switch (cellPr->m_oHMerge->m_oVal->GetValue())
			{
			case SimpleTypes::mergeContinue: m_oBcw.m_oStream.WriteBYTE(vmerge_Continue); break;
			case SimpleTypes::mergeRestart: m_oBcw.m_oStream.WriteBYTE(vmerge_Restart); break;
			default: m_oBcw.m_oStream.WriteBYTE(vmerge_Continue); break;
			}
		}
		else
		{
			m_oBcw.m_oStream.WriteBYTE(vmerge_Continue);
		}
	}
	//TableCellBorders
	if(cellPr->m_oTcBorders.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::TableCellBorders);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTcBorders(cellPr->m_oTcBorders.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Shd
	if (cellPr->m_oShd.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::Shd);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteShd(cellPr->m_oShd.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (cellPr->m_oNoWrap.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::noWrap);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(cellPr->m_oNoWrap->m_oVal.ToBool());
	}
	//CellMar
	if(cellPr->m_oTcMar.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::CellMar);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteCellMar(cellPr->m_oTcMar.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (cellPr->m_oTextDirection.IsInit() && cellPr->m_oTextDirection->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::textDirection);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(cellPr->m_oTextDirection->m_oVal->GetValue());
	}
	if (cellPr->m_oTcFitText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::tcFitText);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(cellPr->m_oTcFitText->m_oVal.ToBool());
	}	
	//VAlign
	if(cellPr->m_oVAlign.IsInit() && cellPr->m_oVAlign->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::VAlign);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		switch(cellPr->m_oVAlign->m_oVal->GetValue())
		{
		case SimpleTypes::verticaljcBottom: m_oBcw.m_oStream.WriteBYTE(vertalignjc_Bottom); break;
		case SimpleTypes::verticaljcCenter: m_oBcw.m_oStream.WriteBYTE(vertalignjc_Center); break;
		case SimpleTypes::verticaljcBoth:
		case SimpleTypes::verticaljcTop: m_oBcw.m_oStream.WriteBYTE(vertalignjc_Top); break;
		default: m_oBcw.m_oStream.WriteBYTE(vertalignjc_Top);break;
		}
	}
	if (cellPr->m_oHideMark.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::hideMark);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(cellPr->m_oHideMark->m_oVal.ToBool());
	}
	if(cellPr->m_oCellDel.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::CellDel);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(cellPr->m_oCellDel.get2());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(cellPr->m_oCellIns.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::CellIns);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteTrackRevision(cellPr->m_oCellIns.get2());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(cellPr->m_oCellMerge.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::CellMerge);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteCellMerge(cellPr->m_oCellMerge.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(cellPr->m_oTcPrChange.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProp_cellPrType::tcPrChange);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteTcPrChange(cellPr->m_oTcPrChange.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteCellMerge(const ComplexTypes::Word::CCellMergeTrackChange& cellMerge)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(cellMerge);
	if(cellMerge.m_oVMerge.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::VMerge);
		m_oBcw.m_oStream.WriteLONG(cellMerge.m_oVMerge->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(cellMerge.m_oVMergeOrig.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::VMergeOrigin);
		m_oBcw.m_oStream.WriteLONG(cellMerge.m_oVMergeOrig->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteTblPrChange(const OOX::Logic::CTblPrChange& tblPrChange)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(tblPrChange);
	if(tblPrChange.m_pTblPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::tblPrChange);
		WriteTblPr(tblPrChange.m_pTblPr.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteTcPrChange(const OOX::Logic::CTcPrChange& tcPrChange)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(tcPrChange);
	if(tcPrChange.m_pTcPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::tcPrChange);
		WriteCellPr(tcPrChange.m_pTcPr.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void Binary_tblPrWriter::WriteTrPrChange(const OOX::Logic::CTrPrChange& trPrChange)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(trPrChange);
	if(trPrChange.m_pTrPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::trPrChange);
		WriteRowPr(trPrChange.m_pTrPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}


BinaryStyleTableWriter::BinaryStyleTableWriter(ParamsWriter& oParamsWriter):
  m_oBcw(oParamsWriter),bpPrs(oParamsWriter, NULL),brPrs(oParamsWriter),btblPrs(oParamsWriter)
{
	m_nReaderGenName = 0;
}
void BinaryStyleTableWriter::Write(OOX::CStyles& styles)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteStylesContent(styles);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryStyleTableWriter::WriteStylesContent(OOX::CStyles& styles)
{
	int nCurPos;
	if(false != styles.m_oDocDefaults.IsInit())
	{
		const OOX::CDocDefaults& oDocDefaults = styles.m_oDocDefaults.get();
		if (oDocDefaults.m_oParPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_st::DefpPr);
			bpPrs.Write_pPr(oDocDefaults.m_oParPr.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oDocDefaults.m_oRunPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_st::DefrPr);
			brPrs.Write_rPr(oDocDefaults.m_oRunPr.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	//styles
	nCurPos = m_oBcw.WriteItemStart(c_oSer_st::Styles);
	WriteStyles(styles.m_arrStyle);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryStyleTableWriter::WriteStyles(std::vector<OOX::CStyle*>& styles)
{
	int nCurPos = 0;
	for(size_t i = 0, length = styles.size(); i < length; ++i)
	{
		const OOX::CStyle& style = *styles[i];
		if(false != style.m_sStyleId.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style);
			WriteStyle(style);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryStyleTableWriter::WriteStyle(const OOX::CStyle& style)
{
	int nCurPos = 0;
	//ID
	if(false != style.m_sStyleId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_sts::Style_Id);
		m_oBcw.m_oStream.WriteStringW(*style.m_sStyleId);
	}
	//Name
	std::wstring sName;
	if(false != style.m_oName.IsInit())
		sName = style.m_oName->ToString2();
	else
		sName = L"StGen" + std::to_wstring( m_nReaderGenName++ );
	m_oBcw.m_oStream.WriteBYTE(c_oSer_sts::Style_Name);
	m_oBcw.m_oStream.WriteStringW(sName);
	//Type
	if(false != style.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_Type);
		BYTE byteType = 0;
		switch(style.m_oType->GetValue())
		{
		case SimpleTypes::styletypeCharacter: byteType = styletype_Character;break;
		case SimpleTypes::styletypeNumbering: byteType = styletype_Numbering;break;
		case SimpleTypes::styletypeTable: byteType = styletype_Table;break;
		default:byteType = styletype_Paragraph;break;
		}
		m_oBcw.m_oStream.WriteBYTE(byteType);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Default
	if(false != style.m_oDefault.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_Default);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == style.m_oDefault->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//BasedOn
	if(false != style.m_oBasedOn.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_sts::Style_BasedOn);
		m_oBcw.m_oStream.WriteStringW(style.m_oBasedOn.get().ToString2());
	}
	//NextId
	if(false != style.m_oNext.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_sts::Style_Next);
		m_oBcw.m_oStream.WriteStringW(style.m_oNext.get().ToString2());
	}
	//Link
	if(false != style.m_oLink.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_sts::Style_Link);
		m_oBcw.m_oStream.WriteStringW(style.m_oLink.get().ToString2());
	}

	//qFormat
	if(false != style.m_oQFormat.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_qFormat);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == style.m_oQFormat->m_oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//uiPriority
	if(false != style.m_oUiPriority.IsInit() && style.m_oUiPriority->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_uiPriority);
		m_oBcw.m_oStream.WriteLONG(*style.m_oUiPriority->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//hidden
	if(false != style.m_oHidden.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_hidden);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == style.m_oHidden->m_oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//semiHidden
	if(false != style.m_oSemiHidden.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_semiHidden);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == style.m_oSemiHidden->m_oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//unhideWhenUsed
	if(false != style.m_oUnhideWhenUsed.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_unhideWhenUsed);
		m_oBcw.m_oStream.WriteBOOL(SimpleTypes::onoffTrue == style.m_oUnhideWhenUsed->m_oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TextPr
	if(false != style.m_oRunPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_TextPr);
		brPrs.Write_rPr(style.m_oRunPr.GetPointer());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//ParaPr
	if(false != style.m_oParPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_ParaPr);
		bpPrs.Write_pPr(style.m_oParPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TblPr
	if(false != style.m_oTblPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_TablePr);
		btblPrs.WriteTblPr(style.m_oTblPr.operator ->());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TrPr
	if(false != style.m_oTrPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_RowPr);
		btblPrs.WriteRowPr(style.m_oTrPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TcPr
	if(false != style.m_oTcPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_CellPr);
		btblPrs.WriteCellPr(style.m_oTcPr.GetPointer());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TblStylePr
	if(style.m_arrTblStylePr.size() > 0)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_TblStylePr);
		WriteTblStylePr(style.m_arrTblStylePr);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(style.m_oCustomStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_CustomStyle);
		m_oBcw.m_oStream.WriteBOOL(style.m_oCustomStyle->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(style.m_oAliases.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_Aliases);
		m_oBcw.m_oStream.WriteStringW3(style.m_oAliases->m_sVal.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(style.m_oAutoRedefine.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_AutoRedefine);
		m_oBcw.m_oStream.WriteBOOL(style.m_oAutoRedefine->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(style.m_oLocked.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_Locked);
		m_oBcw.m_oStream.WriteBOOL(style.m_oLocked->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(style.m_oPersonal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_Personal);
		m_oBcw.m_oStream.WriteBOOL(style.m_oPersonal->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(style.m_oPersonalCompose.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_PersonalCompose);
		m_oBcw.m_oStream.WriteBOOL(style.m_oPersonalCompose->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(style.m_oPersonalReply.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_sts::Style_PersonalReply);
		m_oBcw.m_oStream.WriteBOOL(style.m_oPersonalReply->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteTblStylePr(const std::vector<OOX::Logic::CTableStyleProperties*>& aProperties)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aProperties.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblStylePrType::TblStylePr);
		WriteTblStyleProperties(*aProperties[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteTblStyleProperties(const OOX::Logic::CTableStyleProperties& oProperty)
{
	int nCurPos = 0;
	//Type
	if(oProperty.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblStylePrType::Type);
		m_oBcw.m_oStream.WriteBYTE(oProperty.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);

		//TextPr
		if(false != oProperty.m_oRunPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblStylePrType::RunPr);
			brPrs.Write_rPr(oProperty.m_oRunPr.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//ParaPr
		if(false != oProperty.m_oParPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblStylePrType::ParPr);
			bpPrs.Write_pPr(oProperty.m_oParPr.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//TblPr
		if(false != oProperty.m_oTblPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblStylePrType::TblPr);
			btblPrs.WriteTblPr(oProperty.m_oTblPr.operator ->());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//TrPr
		if(false != oProperty.m_oTrPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblStylePrType::TrPr);
			btblPrs.WriteRowPr(oProperty.m_oTrPr.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//TcPr
		if(false != oProperty.m_oTcPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerProp_tblStylePrType::TcPr);
			btblPrs.WriteCellPr(oProperty.m_oTcPr.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}


BinaryNumberingTableWriter::BinaryNumberingTableWriter(ParamsWriter& oParamsWriter):
  m_oBcw(oParamsWriter),bpPrs(oParamsWriter, NULL),brPrs(oParamsWriter)
{
}
void BinaryNumberingTableWriter::Write(const OOX::CNumbering& numbering)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteNumberingContent(numbering);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryNumberingTableWriter::WriteNumberingContent(const OOX::CNumbering& numbering)
{
	int nCurPos = 0;
	//AbstractNums
	nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::AbstractNums);
	WriteAbstractNums(numbering);
	m_oBcw.WriteItemEnd(nCurPos);

	//Nums
	nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::Nums);
	WriteNums(numbering);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryNumberingTableWriter::WriteNums(const OOX::CNumbering& numbering)
{
	int nCurPos = 0;

	for(size_t i = 0, length = numbering.m_arrNum.size(); i < length; ++i)
	{
		const OOX::Numbering::CNum& num = *numbering.m_arrNum[i];
		//num
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::Num);
		WriteNum(num);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryNumberingTableWriter::WriteNum(const OOX::Numbering::CNum& num)
{
	int nCurPos = 0;
	if (num.m_oAbstractNumId.IsInit() && num.m_oAbstractNumId->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::Num_ANumId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*num.m_oAbstractNumId->m_oVal);
	}
	if (num.m_oNumId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::Num_NumId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*num.m_oNumId);
	}
	for (size_t i = 0; i < num.m_arrLvlOverride.size(); ++i)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::Num_LvlOverride);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteLvlOverride(*num.m_arrLvlOverride[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}

}
void BinaryNumberingTableWriter::WriteLvlOverride(const OOX::Numbering::CNumLvl& numLvl)
{
	int nCurPos = 0;
	if(numLvl.m_oIlvl.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::ILvl);
		m_oBcw.m_oStream.WriteLONG(*numLvl.m_oIlvl);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(numLvl.m_oStartOverride.IsInit() && numLvl.m_oStartOverride->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::StartOverride);
		m_oBcw.m_oStream.WriteLONG(*numLvl.m_oStartOverride->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(numLvl.m_oLvl.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::Lvl);
		WriteLevel(numLvl.m_oLvl.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}

}
void BinaryNumberingTableWriter::WriteAbstractNums(const OOX::CNumbering& numbering)
{
	int nCurPos = 0;
	int nRealCount = 0;

	for(size_t i = 0, length = numbering.m_arrAbstractNum.size(); i < length; ++i)
	{
		const OOX::Numbering::CAbstractNum& num = *numbering.m_arrAbstractNum[i];
		
		if (false != num.m_oAbstractNumId.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::AbstractNum);
			WriteAbstractNum(num, nRealCount, numbering.m_arrNum);
			m_oBcw.WriteItemEnd(nCurPos);
			nRealCount++;
		}
	}
}
void BinaryNumberingTableWriter::WriteAbstractNum(const OOX::Numbering::CAbstractNum& num, int nIndex, const std::vector<OOX::Numbering::CNum*>& aNums)
{
	int nCurPos = 0;
	//Id
	if(num.m_oAbstractNumId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::AbstractNum_Id);
		m_oBcw.m_oStream.WriteLONG(*num.m_oAbstractNumId);
		m_oBcw.WriteItemEnd(nCurPos);
	}

	//Type
	if(false != num.m_oMultiLevelType.IsInit())
	{
		//todo
		//nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::AbstractNum_Type);
		//m_oBcw.m_oStream.WriteBYTE(num.Type);
		//m_oBcw.WriteItemEnd(nCurPos);
	}

	//NumStyleLink
	if(false != num.m_oNumStyleLink.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::NumStyleLink);
		m_oBcw.m_oStream.WriteStringW(num.m_oNumStyleLink.get().ToString2());
	}

	//StyleLink
	if(false != num.m_oStyleLink.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::StyleLink);
		m_oBcw.m_oStream.WriteStringW(num.m_oStyleLink.get().ToString2());
	}

	//Lvl
	if (false != num.m_oAbstractNumId.IsInit() && false == num.m_arrLvl.empty())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::AbstractNum_Lvls);
		WriteLevels(num.m_arrLvl, *num.m_oAbstractNumId, aNums);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryNumberingTableWriter::WriteLevels(const std::vector<OOX::Numbering::CLvl*>& lvls, int nAId, const std::vector<OOX::Numbering::CNum*>& aNums)
{
	int nCurPos = 0;

	for(size_t i = 0, length = lvls.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::Lvl);
		WriteLevel(*lvls[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryNumberingTableWriter::WriteLevel(const OOX::Numbering::CLvl& lvl)
{
	int nCurPos = 0;
	//Format
	if(lvl.m_oNumFmt.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_NumFmt);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		bpPrs.WriteNumFmt(lvl.m_oNumFmt.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Jc
	if(lvl.m_oLvlJc.IsInit() && lvl.m_oLvlJc->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_Jc);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(lvl.m_oLvlJc->m_oVal->GetValue());
	}
	//LvlText
	if(false != lvl.m_oLvlText.IsInit())
	{
		const ComplexTypes::Word::CLevelText& oText = lvl.m_oLvlText.get();
		if(false != oText.m_sVal.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_LvlText);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
			nCurPos = m_oBcw.WriteItemWithLengthStart();
			WriteLevelText(oText.m_sVal.get());
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
	//Restart
	if(false != lvl.m_oLvlRestart.IsInit())
	{
		const ComplexTypes::Word::CDecimalNumber& oVal = lvl.m_oLvlRestart.get();
		if(oVal.m_oVal.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_Restart);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_oVal);
		}
	}
	//Start
	if(false != lvl.m_oStart.IsInit())
	{
		const ComplexTypes::Word::CDecimalNumber& oVal = lvl.m_oStart.get();
		if(oVal.m_oVal.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_Start);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_oVal);
		}
	}
	//Suff
	if(false != lvl.m_oSuffix.IsInit())
	{
		const ComplexTypes::Word::CLevelSuffix& oSuff = lvl.m_oSuffix.get();
		if(false != oSuff.m_oVal.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_Suff);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			switch(oSuff.m_oVal.get().GetValue())
			{
			case SimpleTypes::levelsuffixNothing: m_oBcw.m_oStream.WriteBYTE(numbering_suff_Nothing);break;
			case SimpleTypes::levelsuffixSpace: m_oBcw.m_oStream.WriteBYTE(numbering_suff_Space);break;
			case SimpleTypes::levelsuffixTab: m_oBcw.m_oStream.WriteBYTE(numbering_suff_Tab);break;
			default: m_oBcw.m_oStream.WriteBYTE(numbering_suff_Tab);break;
			}
		}
	}
	//PStyle
	if(false != lvl.m_oPStyle.IsInit() && lvl.m_oPStyle->m_sVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_PStyle);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*lvl.m_oPStyle->m_sVal);
	}
	//ParaPr
	if(false != lvl.m_oPPr.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_ParaPr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		bpPrs.Write_pPr(lvl.m_oPPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//TextPr
	if(false != lvl.m_oRPr.IsInit())
	{
		OOX::Logic::CRunProperty* p_rPr = NULL;
		if(false != lvl.m_oRPr.IsInit())
			p_rPr = lvl.m_oRPr.operator ->();

		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_TextPr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		if(NULL != p_rPr)
			brPrs.Write_rPr(p_rPr);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(lvl.m_oIlvl.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::ILvl);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*lvl.m_oIlvl);
	}
	if(lvl.m_oTentative.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::Tentative);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(lvl.m_oTentative->ToBool());
	}
	if(lvl.m_oTplc.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::Tplc);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG(lvl.m_oTplc->GetValue());
	}
	if(lvl.m_oIsLgl.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::IsLgl);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(lvl.m_oIsLgl->m_oVal.ToBool());
	}
	if(lvl.m_oLegacy.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::LvlLegacy);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteLvlLegacy(lvl.m_oLegacy.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryNumberingTableWriter::WriteLvlLegacy(const ComplexTypes::Word::CLvlLegacy& lvlLegacy)
{
	int nCurPos = 0;
	if (lvlLegacy.m_oLegacy.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::Legacy);
		m_oBcw.m_oStream.WriteBOOL(lvlLegacy.m_oLegacy->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (lvlLegacy.m_oLegacyIndent.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::LegacyIndent);
		m_oBcw.m_oStream.WriteLONG(lvlLegacy.m_oLegacyIndent->ToTwips());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (lvlLegacy.m_oLegacySpace.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::LegacySpace);
		m_oBcw.m_oStream.WriteULONG(lvlLegacy.m_oLegacySpace->ToUnsignedTwips());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryNumberingTableWriter::WriteLevelText(const std::wstring& text)
{
	int nCurPos = 0;
	for(size_t i = 0, length = text.length(); i < length; ++i)
	{
		if('%' == text[i] && i + 1 < length && '0' <= text[i + 1] && text[i + 1] <= '9')
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::lvl_LvlTextItem);

			m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_LvlTextItemNum);
			int nCurPos2 = m_oBcw.WriteItemWithLengthStart();
			int nNum = text[i + 1] - '0';
			if(nNum > 0)
				nNum--;
			m_oBcw.m_oStream.WriteBYTE(nNum);
			m_oBcw.WriteItemWithLengthEnd(nCurPos2);

			m_oBcw.WriteItemEnd(nCurPos);
			i++;
		}
		else
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerNumTypes::lvl_LvlTextItem);

			m_oBcw.m_oStream.WriteBYTE(c_oSerNumTypes::lvl_LvlTextItemText);
			std::wstring strChar (text.c_str() + i, 1);
			m_oBcw.m_oStream.WriteStringW(strChar);
			if(NULL != m_oBcw.m_pEmbeddedFontsManager)
				m_oBcw.m_pEmbeddedFontsManager->CheckString(strChar);

			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}


BinaryOtherTableWriter::BinaryOtherTableWriter(ParamsWriter& oParamsWriter, PPTX::Theme* pTheme) : m_oParamsWriter(oParamsWriter), m_oBcw(oParamsWriter), m_pTheme(pTheme)
{
}
void BinaryOtherTableWriter::Write()
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteOtherContent();
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryOtherTableWriter::WriteOtherContent()
{
//EmbeddedFonts
	if(NULL != m_oBcw.m_pEmbeddedFontsManager)
	{
		EmbeddedBinaryWriter oEmbeddedBinaryWriter(m_oBcw.m_oStream);
		int nStart = m_oBcw.WriteItemStart(c_oSerOtherTableTypes::EmbeddedFonts);
		m_oBcw.m_pEmbeddedFontsManager->WriteEmbeddedFonts<EmbeddedBinaryWriter>(&oEmbeddedBinaryWriter);
		m_oBcw.WriteItemEnd(nStart);
	}
	if(NULL != m_pTheme)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerOtherTableTypes::DocxTheme);

		int nCurPos = m_oBcw.WriteItemWithLengthStart();
			m_pTheme->toPPTY(&m_oBcw.m_oStream);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}

BinaryDocumentTableWriter::BinaryDocumentTableWriter(ParamsWriter& oParamsWriter, ParamsDocumentWriter& oParamsDocumentWriter, std::map<int, bool>* mapIgnoreComments, BinaryHeaderFooterTableWriter* oBinaryHeaderFooterTableWriter):
	m_oParamsWriter(oParamsWriter), m_oParamsDocumentWriter(oParamsDocumentWriter), m_oBcw(oParamsWriter), bpPrs(oParamsWriter, oBinaryHeaderFooterTableWriter), brPrs(oParamsWriter), btblPrs(oParamsWriter), m_pOfficeDrawingConverter(oParamsWriter.m_pOfficeDrawingConverter),m_mapIgnoreComments(mapIgnoreComments)
{
	pBackground		= NULL;
	pSectPr			= NULL;
	poDocument		= NULL;
	pJsaProject		= NULL;
	m_bWriteSectPr	= false;
}
void BinaryDocumentTableWriter::Write(OOX::Logic::CDocPartPr* pDocPartPr)
{
	if (!pDocPartPr) return;

	int nCurPos = 0;
	if (pDocPartPr->m_oName.IsInit() && pDocPartPr->m_oName->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::Name);
		m_oBcw.m_oStream.WriteStringW3(pDocPartPr->m_oName->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pDocPartPr->m_oStyle.IsInit() && pDocPartPr->m_oStyle->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::Style);
		m_oBcw.m_oStream.WriteStringW3(pDocPartPr->m_oStyle->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pDocPartPr->m_oGuid.IsInit() && pDocPartPr->m_oGuid->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::Guid);
		m_oBcw.m_oStream.WriteStringW3(pDocPartPr->m_oGuid->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pDocPartPr->m_oDescription.IsInit() && pDocPartPr->m_oDescription->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::Description);
		m_oBcw.m_oStream.WriteStringW3(pDocPartPr->m_oDescription->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pDocPartPr->m_oCategory.IsInit())
	{
		if (pDocPartPr->m_oCategory->m_oName.IsInit() && pDocPartPr->m_oCategory->m_oName->m_sVal.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::CategoryName);
			m_oBcw.m_oStream.WriteStringW3(pDocPartPr->m_oCategory->m_oName->m_sVal.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pDocPartPr->m_oCategory->m_oGallery.IsInit() && pDocPartPr->m_oCategory->m_oGallery->m_oVal.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::CategoryGallery);
			m_oBcw.m_oStream.WriteBYTE(pDocPartPr->m_oCategory->m_oGallery->m_oVal->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}

	Write(pDocPartPr->m_oTypes.GetPointer());
	Write(pDocPartPr->m_oBehaviors.GetPointer());
}
void BinaryDocumentTableWriter::Write(OOX::Logic::CDocPartTypes* pDocPartTypes)
{
	if (!pDocPartTypes) return;

	int nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::Types);
	for (size_t i = 0; i < pDocPartTypes->m_arrItems.size(); ++i)
	{
		if (pDocPartTypes->m_arrItems[i] && pDocPartTypes->m_arrItems[i]->m_sVal.IsInit())
		{
			int nCurPos1 = m_oBcw.WriteItemStart(c_oSerGlossary::Type);
			m_oBcw.m_oStream.WriteStringW3(pDocPartTypes->m_arrItems[i]->m_sVal.get());
			m_oBcw.WriteItemEnd(nCurPos1);
		}
	}
	m_oBcw.WriteItemEnd(nCurPos);

}
void BinaryDocumentTableWriter::Write(OOX::Logic::CDocPartBehaviors* pDocPartBehaviors)
{
	if (!pDocPartBehaviors) return;

	int nCurPos = m_oBcw.WriteItemStart(c_oSerGlossary::Behaviors);
	for (size_t i = 0; i < pDocPartBehaviors->m_arrItems.size(); ++i)
	{
		if (pDocPartBehaviors->m_arrItems[i] && pDocPartBehaviors->m_arrItems[i]->m_oVal.IsInit())
		{
			int nCurPos1 = m_oBcw.WriteItemStart(c_oSerGlossary::Behavior);
			m_oBcw.m_oStream.WriteBYTE(pDocPartBehaviors->m_arrItems[i]->m_oVal->GetValue());
			m_oBcw.WriteItemEnd(nCurPos1);
		}
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::Write(OOX::Logic::CDocParts* pDocParts)
{
	if (!pDocParts) return;

	for (size_t i = 0; i < pDocParts->m_arrItems.size(); ++i)
	{
		if (pDocParts->m_arrItems[i])
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerGlossary::DocPart);
			int nCurPos1 = m_oBcw.WriteItemWithLengthStart();
			
			if (pDocParts->m_arrItems[i]->m_oDocPartPr.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerGlossary::DocPartPr);
				int nCurPos2 = m_oBcw.WriteItemWithLengthStart();
				Write(pDocParts->m_arrItems[i]->m_oDocPartPr.GetPointer());
				m_oBcw.WriteItemWithLengthEnd(nCurPos2);
			}
			if (pDocParts->m_arrItems[i]->m_oDocPartBody.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerGlossary::DocPartBody);
				int nCurPos2 = m_oBcw.WriteItemWithLengthStart();

				WriteDocumentContent(pDocParts->m_arrItems[i]->m_oDocPartBody->m_arrItems);

				m_oBcw.WriteItemWithLengthEnd(nCurPos2);
			}
			m_oBcw.WriteItemWithLengthEnd(nCurPos1);
		}
	}
}
void BinaryDocumentTableWriter::Write(std::vector<OOX::WritingElement*> & aElems)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteDocumentContent(aElems);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryDocumentTableWriter::WriteDocumentContent(const std::vector<OOX::WritingElement*> & aElems)
{
	int nCurPos = 0;
	for ( size_t i = 0; i < aElems.size(); ++i)
	{
		OOX::WritingElement* item = aElems[i];

		switch(item->getType())
		{
			case OOX::et_w_commentRangeStart:
			{
				OOX::Logic::CCommentRangeStart* pCommentRangeStart = static_cast<OOX::Logic::CCommentRangeStart*>(item);
				WriteComment(OOX::et_w_commentRangeStart, pCommentRangeStart->m_oId);			
			}break;
			case OOX::et_w_commentRangeEnd:
			{
				OOX::Logic::CCommentRangeEnd* pCommentRangeEnd = static_cast<OOX::Logic::CCommentRangeEnd*>(item);
				WriteComment(OOX::et_w_commentRangeEnd, pCommentRangeEnd->m_oId);			
			}break;
			case OOX::et_w_altChunk:
			{
				OOX::Logic::CAltChunk* pAltChunk = static_cast<OOX::Logic::CAltChunk*>(item);
				if (pAltChunk->m_oId.IsInit())
				{
					OOX::Rels::CRelationShip* oRels = NULL;
					smart_ptr<OOX::File> pFile = m_oParamsDocumentWriter.m_pRels->Find( OOX::RId(pAltChunk->m_oId.get().GetValue()));
					if (pFile.IsInit() && OOX::FileTypes::Media == pFile->type())
					{
						OOX::Media* pAltChunkFile = static_cast<OOX::Media*>(pFile.operator ->());
						
						WriteAltChunk(*pAltChunkFile, m_oParamsWriter.m_pStyles);
					}
				}
			}break;
			case OOX::et_w_p:
			{
				OOX::Logic::CParagraph* pParagraph = static_cast<OOX::Logic::CParagraph*>(item);
				OOX::Logic::CParagraphProperty* pPr = pParagraph->m_oParagraphProperty;

				m_oBcw.m_oStream.WriteBYTE(c_oSerParType::Par);
					nCurPos = m_oBcw.WriteItemWithLengthStart();
					WriteParapraph(*pParagraph, pPr);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}break;
			case OOX::et_w_tbl:
			{
				OOX::Logic::CTbl* pTbl = static_cast<OOX::Logic::CTbl*>(item);
					m_oBcw.m_oStream.WriteBYTE(c_oSerParType::Table);
					nCurPos = m_oBcw.WriteItemWithLengthStart();
					WriteDocTable(pTbl);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}break;
			case OOX::et_w_sdt:
			{
				OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::Sdt);
				WriteSdt(pStd, 0, NULL, 0, 0, 0);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}break;
			case OOX::et_w_smartTag:
			{
				OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
				WriteDocumentContent(pSmartTag->m_arrItems);
			}break;
			case OOX::et_w_dir:
			{
				OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
				WriteDocumentContent(pDir->m_arrItems);
			}break;
			case OOX::et_w_bdo:
			{
				OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
				WriteDocumentContent(pBdo->m_arrItems);
			}break;
			case OOX::et_w_bookmarkStart:
			{
				OOX::Logic::CBookmarkStart* pBookmarkStart = static_cast<OOX::Logic::CBookmarkStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::BookmarkStart);
				WriteBookmarkStart(*pBookmarkStart);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}break;
			case OOX::et_w_bookmarkEnd:
			{
				OOX::Logic::CBookmarkEnd* pBookmarkEnd = static_cast<OOX::Logic::CBookmarkEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::BookmarkEnd);
				WriteBookmarkEnd(*pBookmarkEnd);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}break;
			case OOX::et_w_moveFromRangeStart:
			{
				OOX::Logic::CMoveFromRangeStart* pMoveFromRangeStart = static_cast<OOX::Logic::CMoveFromRangeStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveFromRangeStart);
				WriteMoveRangeStart(*pMoveFromRangeStart);
				m_oBcw.WriteItemEnd(nCurPos);
			}break;
			case OOX::et_w_moveFromRangeEnd:
			{
				OOX::Logic::CMoveFromRangeEnd* pMoveFromRangeEnd = static_cast<OOX::Logic::CMoveFromRangeEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveFromRangeEnd);
				WriteMoveRangeEnd(*pMoveFromRangeEnd);
				m_oBcw.WriteItemEnd(nCurPos);
			}break;
			case OOX::et_w_moveToRangeStart:
			{
				OOX::Logic::CMoveToRangeStart* pMoveToRangeStart = static_cast<OOX::Logic::CMoveToRangeStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveToRangeStart);
				WriteMoveRangeStart(*pMoveToRangeStart);
				m_oBcw.WriteItemEnd(nCurPos);
			}break;
			case OOX::et_w_moveToRangeEnd:
			{
				OOX::Logic::CMoveToRangeEnd* pMoveToRangeEnd = static_cast<OOX::Logic::CMoveToRangeEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveToRangeEnd);
				WriteMoveRangeEnd(*pMoveToRangeEnd);
				m_oBcw.WriteItemEnd(nCurPos);
			}break;
			case OOX::et_w_docParts:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerParType::DocParts);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				Write(static_cast<OOX::Logic::CDocParts*>(item));
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}break;
		default:
			break;
		}
	}
//SectPr  & Background
	if(true == m_bWriteSectPr && (NULL != pSectPr || NULL != pBackground))
	{
		if (NULL != pSectPr)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerParType::sectPr);
				bpPrs.WriteSectPr(pSectPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != pBackground)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerParType::Background);
				WriteBackground(pBackground);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
//Write JsaProject
	if (NULL != pJsaProject)
	{
		BYTE* pData = NULL;
		DWORD nBytesCount;
		if(NSFile::CFileBinary::ReadAllBytes(pJsaProject->filename().GetPath(), &pData, nBytesCount))
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerParType::JsaProject);
			m_oBcw.m_oStream.WriteBYTEArray(pData, nBytesCount);
			m_oBcw.WriteItemEnd(nCurPos);
			RELEASEARRAYOBJECTS(pData);
		}
	}
}
void BinaryDocumentTableWriter::WriteBackground (OOX::WritingElement* pElement)
{
	if (pElement == NULL) return;

	std::wstring sXml;
	//if (pBackground->m_oDrawing.IsInit())
	//{
	//	sXml = pBackground->m_oDrawing->m_sXml.get2();
	//}
	//else
	OOX::Logic::CBackground *pBackground = dynamic_cast<OOX::Logic::CBackground*>(pElement);
	OOX::Logic::CBgPict *pBgPict = dynamic_cast<OOX::Logic::CBgPict*>(pElement);

	if (pBackground)
	{
		if (pBackground->m_oBackground.IsInit())
		{
			sXml = pBackground->m_oBackground->toXML();
		}
		if (pBackground->m_oColor.IsInit())
		{
			m_oBcw.WriteColor(c_oSerBackgroundType::Color, pBackground->m_oColor.get());
		}	
		if (pBackground->m_oThemeColor.IsInit())
		{
			m_oBcw.WriteThemeColor(c_oSerBackgroundType::ColorTheme, pBackground->m_oColor, pBackground->m_oThemeColor, pBackground->m_oThemeTint, pBackground->m_oThemeShade);
		}
	}
	if (pBgPict)
	{
		if (pBgPict->m_oBackground.IsInit())
		{
			sXml = pBgPict->m_oBackground->toXML();
		}
		if (pBgPict->m_oColor.IsInit())
		{
			m_oBcw.WriteColor(c_oSerBackgroundType::Color, pBgPict->m_oColor.get());
		}	
	}

	if (!sXml.empty())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerBackgroundType::pptxDrawing);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();
			WriteDrawing(&sXml, NULL, NULL);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
						
void BinaryDocumentTableWriter::WriteAltChunk(OOX::Media& oAltChunkFile, OOX::CStyles* styles)
{
	if (false == oAltChunkFile.IsExist()) return;


	std::wstring file_name_inp = oAltChunkFile.filename().GetPath();
	
	std::wstring sTempDir = NSDirectory::CreateDirectoryWithUniqueName(oAltChunkFile.filename().GetDirectory()); 
	std::wstring sResultDocxDir = NSDirectory::CreateDirectoryWithUniqueName(oAltChunkFile.filename().GetDirectory()); 

	bool result = false;

	if (sResultDocxDir.empty() || sTempDir.empty()) return;

	COfficeFileFormatChecker OfficeFileFormatChecker;
	if (OfficeFileFormatChecker.isOfficeFile(file_name_inp))
	{
		switch (OfficeFileFormatChecker.nFileType)
		{
#ifndef DONT_USED_EXTRA_LIBRARY
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC_FLAT:
		{
#ifndef _IOS
			COfficeDocFile docFile;
			docFile.m_sTempFolder = sTempDir;

			bool bMacros = false;

			result = (S_OK == docFile.LoadFromFile(file_name_inp, sResultDocxDir, NULL, bMacros));
#else
			result = S_FALSE;
#endif
		}break;
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF:
		{
			RtfConvertationManager rtfConvert;
			rtfConvert.m_sTempFolder = sTempDir;

			result = (S_OK == rtfConvert.ConvertRtfToOOX(file_name_inp, sResultDocxDir));
		}break;
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML:
		{
			CHtmlFile2 htmlConvert;
			CHtmlParams	paramsHtml;

			htmlConvert.SetTmpDirectory(sTempDir);

			if (styles)
			{
				if (styles->m_oDocDefaults.IsInit())
				{
					paramsHtml.m_sdocDefaults = styles->m_oDocDefaults->toXML();
				}
				std::map<SimpleTypes::EStyleType, size_t>::iterator pFind = styles->m_mapStyleDefaults.find(SimpleTypes::styletypeParagraph);
				if (pFind != styles->m_mapStyleDefaults.end())
				{
					if (styles->m_arrStyle[pFind->second])
					{
						//change styleId

						OOX::CStyle updateStyle(*styles->m_arrStyle[pFind->second]);
						updateStyle.m_sStyleId = L"normal";
						paramsHtml.m_sNormal = updateStyle.toXML();
					}
				}
			}

			result = (S_OK == htmlConvert.OpenHtml(file_name_inp, sResultDocxDir, &paramsHtml));
		}break;
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT:
		{
			CHtmlFile2 htmlConvert;
			htmlConvert.SetTmpDirectory(sTempDir);

			result = (S_OK == htmlConvert.OpenMht(file_name_inp, sResultDocxDir));
		}break;
#endif
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
		{
			COfficeUtils oCOfficeUtils(NULL);
			result = (S_OK == oCOfficeUtils.ExtractToDirectory(file_name_inp, sResultDocxDir, NULL, 0));
		}break;
		}
	}
	NSDirectory::DeleteDirectory(sTempDir);

	if (result)
	{
		OOX::CDocx oDocx = OOX::CDocx(OOX::CPath(sResultDocxDir));

		if (oDocx.m_oMain.document)
		{
			ParamsDocumentWriter oParamsDocumentWriterEmb(oDocx.m_oMain.document);
			
			ParamsWriter oParamsWriterEmb(	m_oParamsWriter.m_pCBufferedStream, 
											m_oParamsWriter.m_pFontProcessor, 
											m_oParamsWriter.m_pOfficeDrawingConverter, 
											m_oParamsWriter.m_pEmbeddedFontsManager);
			
			if (!m_oParamsWriter.m_pStyles)
			{
				m_oParamsWriter.m_pStyles = new OOX::CStyles(NULL);
				m_oParamsWriter.m_bLocalStyles = true;
			}
			if (!m_oParamsWriter.m_pNumbering)
			{
				m_oParamsWriter.m_pNumbering = new OOX::CNumbering(NULL);
				m_oParamsWriter.m_bLocalNumbering = true;				
			}
			std::map<std::wstring, std::wstring> mapItemStyles;
			m_oParamsWriter.m_pStyles->m_mapEmbeddedStyleNames.push_back(mapItemStyles);
			
			std::map<int, int> mapItemNumbering;
			m_oParamsWriter.m_pNumbering->m_mapEmbeddedNames.push_back(mapItemNumbering);

			oParamsWriterEmb.m_pStyles = m_oParamsWriter.m_pStyles;
			oParamsWriterEmb.m_pNumbering = m_oParamsWriter.m_pNumbering;

			oParamsWriterEmb.m_pEmbeddedStyles = oDocx.m_oMain.styles;
			oParamsWriterEmb.m_pEmbeddedNumbering = oDocx.m_oMain.numbering;

			oParamsWriterEmb.m_pTheme = oDocx.m_pTheme;
			oParamsWriterEmb.m_pSettings = oDocx.m_oMain.settings;
			oParamsWriterEmb.m_pCurRels = oParamsDocumentWriterEmb.m_pRels;

			BinaryDocumentTableWriter oBinaryDocumentEmbTableWriter(oParamsWriterEmb, oParamsDocumentWriterEmb, &oParamsWriterEmb.m_mapIgnoreComments, NULL);

			oBinaryDocumentEmbTableWriter.WriteDocumentContent(oDocx.m_oMain.document->m_arrItems);
		}
		else
		{
			OOX::CDocxFlat oDocxFlat = OOX::CDocxFlat(OOX::CPath(sResultDocxDir));
			if (oDocxFlat.m_pDocument.IsInit())
			{
				ParamsDocumentWriter oParamsDocumentWriterEmb(oDocxFlat.m_pDocument.GetPointer());
				
				ParamsWriter oParamsWriterEmb(	m_oParamsWriter.m_pCBufferedStream, 
												m_oParamsWriter.m_pFontProcessor, 
												m_oParamsWriter.m_pOfficeDrawingConverter, 
												m_oParamsWriter.m_pEmbeddedFontsManager);
				
				std::map<std::wstring, std::wstring> mapItem;
				m_oParamsWriter.m_pStyles->m_mapEmbeddedStyleNames.push_back(mapItem);
				
				oParamsWriterEmb.m_pStyles = m_oParamsWriter.m_pStyles;
				oParamsWriterEmb.m_pNumbering = m_oParamsWriter.m_pNumbering;

				oParamsWriterEmb.m_pEmbeddedStyles = oDocxFlat.m_pStyles.GetPointer();
				oParamsWriterEmb.m_pEmbeddedNumbering = oDocxFlat.m_pNumbering.GetPointer();

				BinaryDocumentTableWriter oBinaryDocumentEmbTableWriter(oParamsWriterEmb, oParamsDocumentWriterEmb, &oParamsWriterEmb.m_mapIgnoreComments, NULL);
				oBinaryDocumentEmbTableWriter.WriteDocumentContent(oDocxFlat.m_pDocument->m_arrItems);
			}
		}
	}
	NSDirectory::DeleteDirectory(sResultDocxDir);
}

void BinaryDocumentTableWriter::WriteParapraph(OOX::Logic::CParagraph& par, OOX::Logic::CParagraphProperty* pPr)
{
	int nCurPos = 0;
//pPr
	if(NULL != pPr)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerParType::pPr);
			nCurPos = m_oBcw.WriteItemWithLengthStart();
			bpPrs.Write_pPr(*pPr);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Content
	m_oBcw.m_oStream.WriteBYTE(c_oSerParType::Content);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteParagraphContent(par.m_arrItems);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);

}
void BinaryDocumentTableWriter::WriteParagraphContent(const std::vector<OOX::WritingElement*> & content, bool bHyperlink)
{
	int nCurPos = 0;
	for ( size_t i = 0; i < content.size(); ++i)
	{
		OOX::WritingElement* item = content[i];

		switch (item->getType())
		{
			case OOX::et_w_permStart:
			{
				OOX::Logic::CPermStart* pPermStart = static_cast<OOX::Logic::CPermStart*>(item);
				WritePermission(pPermStart);
			}break;
			case OOX::et_w_permEnd:
			{
				OOX::Logic::CPermEnd* pPermEnd = static_cast<OOX::Logic::CPermEnd*>(item);
				WritePermission(pPermEnd);
			}break;
			case OOX::et_w_fldSimple:
			{
				OOX::Logic::CFldSimple* pFldSimple = static_cast<OOX::Logic::CFldSimple*>(item);
				WriteFldSimple(pFldSimple);

			}break;
			case OOX::et_w_hyperlink:
			{
				OOX::Logic::CHyperlink* pHyperlink = static_cast<OOX::Logic::CHyperlink*>(item);
				WriteHyperlink(pHyperlink);
			}break;
			case OOX::et_w_pPr:
				break;
			case OOX::et_w_br:
			{
				OOX::Logic::CRun oRun;
				oRun.m_arrItems.push_back(item);
				WriteRun(&oRun, false, false);
				oRun.m_arrItems.clear();
			}break;
			case OOX::et_w_r:
			{
				OOX::Logic::CRun* pRun = static_cast<OOX::Logic::CRun*>(item);
				WriteRun(pRun, bHyperlink, false);				
			}break;
			case OOX::et_w_sdt:
			{
				OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::Sdt);
				WriteSdt(pStd, 1, NULL, 0, 0, 0);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);				
			}break;
			case OOX::et_w_smartTag:
			{
				OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
				WriteParagraphContent(pSmartTag->m_arrItems);				
			}break;
			case OOX::et_w_dir:
			{
				OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
				WriteParagraphContent(pDir->m_arrItems);				
			}break;
		case OOX::et_w_bdo:
			{
				OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
				WriteParagraphContent(pBdo->m_arrItems);
				break;
			}
		case OOX::et_w_del:
			{
				OOX::Logic::CDel* pDel = static_cast<OOX::Logic::CDel*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::Del);
				WriteDel(*pDel);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_ins:
			{
				OOX::Logic::CIns* pIns = static_cast<OOX::Logic::CIns*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::Ins);
				WriteIns(*pIns);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveFrom:
			{
				OOX::Logic::CMoveFrom* pMoveFrom = static_cast<OOX::Logic::CMoveFrom*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveFrom);
				WriteMoveFrom(*pMoveFrom);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveTo:
			{
				OOX::Logic::CMoveTo* pMoveTo = static_cast<OOX::Logic::CMoveTo*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveTo);
				WriteMoveTo(*pMoveTo);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_commentRangeStart:
			{
				OOX::Logic::CCommentRangeStart* pCommentRangeStart = static_cast<OOX::Logic::CCommentRangeStart*>(item);
				WriteComment(OOX::et_w_commentRangeStart, pCommentRangeStart->m_oId);
				break;
			}
		case OOX::et_w_commentRangeEnd:
			{
				OOX::Logic::CCommentRangeEnd* pCommentRangeEnd = static_cast<OOX::Logic::CCommentRangeEnd*>(item);
				WriteComment(OOX::et_w_commentRangeEnd, pCommentRangeEnd->m_oId);
				break;
			}
		case OOX::et_w_bookmarkStart:
			{
				OOX::Logic::CBookmarkStart* pBookmarkStart = static_cast<OOX::Logic::CBookmarkStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::BookmarkStart);
				WriteBookmarkStart(*pBookmarkStart);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_bookmarkEnd:
			{
				OOX::Logic::CBookmarkEnd* pBookmarkEnd = static_cast<OOX::Logic::CBookmarkEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::BookmarkEnd);
				WriteBookmarkEnd(*pBookmarkEnd);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveFromRangeStart:
			{
				OOX::Logic::CMoveFromRangeStart* pMoveFromRangeStart = static_cast<OOX::Logic::CMoveFromRangeStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveFromRangeStart);
				WriteMoveRangeStart(*pMoveFromRangeStart);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveFromRangeEnd:
			{
				OOX::Logic::CMoveFromRangeEnd* pMoveFromRangeEnd = static_cast<OOX::Logic::CMoveFromRangeEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveFromRangeEnd);
				WriteMoveRangeEnd(*pMoveFromRangeEnd);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveToRangeStart:
			{
				OOX::Logic::CMoveToRangeStart* pMoveToRangeStart = static_cast<OOX::Logic::CMoveToRangeStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveToRangeStart);
				WriteMoveRangeStart(*pMoveToRangeStart);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveToRangeEnd:
			{
				OOX::Logic::CMoveToRangeEnd* pMoveToRangeEnd = static_cast<OOX::Logic::CMoveToRangeEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MoveToRangeEnd);
				WriteMoveRangeEnd(*pMoveToRangeEnd);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_oMathPara:
			{
				OOX::Logic::COMathPara* pOMathPara = static_cast<OOX::Logic::COMathPara*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::OMathPara);
				WriteMathOMathPara(pOMathPara->m_arrItems);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_oMath:
			{
				OOX::Logic::COMath* pOMath = static_cast<OOX::Logic::COMath*>(item);
				int nCurPos = m_oBcw.WriteItemStart(c_oSerParType::OMath);
				WriteMathArgNodes( pOMath->m_arrItems );
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_r:
			{
				OOX::Logic::CMRun* pMRun = static_cast<OOX::Logic::CMRun*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSerParType::MRun);
				WriteMathRunContent(pMRun);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		default:
			break;
		}
	}
}
void BinaryDocumentTableWriter::WriteDel(const OOX::Logic::CDel& oDel)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(oDel);

	nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::Content);
		WriteParagraphContent(oDel.m_arrItems);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteIns(const OOX::Logic::CIns& oIns)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(oIns);

	nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::Content);
		WriteParagraphContent(oIns.m_arrItems);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMoveFrom(const OOX::Logic::CMoveFrom& oMoveFrom)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(oMoveFrom);

	nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::Content);
		WriteParagraphContent(oMoveFrom.m_arrItems);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMoveTo(const OOX::Logic::CMoveTo& oMoveTo)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(oMoveTo);

	nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::Content);
		WriteParagraphContent(oMoveTo.m_arrItems);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
template<typename T> void BinaryDocumentTableWriter::WriteMoveRangeStart(const T& elem)
{
	int nCurPos = 0;
	if (elem.m_sAuthor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::Author);
		m_oBcw.m_oStream.WriteStringW3(elem.m_sAuthor.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_oColFirst.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::ColFirst);
		m_oBcw.m_oStream.WriteLONG(elem.m_oColFirst->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_oColLast.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::ColLast);
		m_oBcw.m_oStream.WriteLONG(elem.m_oColLast->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_oDate.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::Date);
		m_oBcw.m_oStream.WriteStringW3(elem.m_oDate->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_oDisplacedByCustomXml.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::DisplacedByCustomXml);
		m_oBcw.m_oStream.WriteBYTE(elem.m_oDisplacedByCustomXml->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::Id);
		m_oBcw.m_oStream.WriteLONG(elem.m_oId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_sName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::Name);
		m_oBcw.m_oStream.WriteStringW3(elem.m_sName.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_sUserId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::UserId);
		m_oBcw.m_oStream.WriteStringW3(elem.m_sUserId.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
template<typename T> void BinaryDocumentTableWriter::WriteMoveRangeEnd(const T& elem)
{
	int nCurPos = 0;
	if (elem.m_oDisplacedByCustomXml.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::DisplacedByCustomXml);
		m_oBcw.m_oStream.WriteBYTE(elem.m_oDisplacedByCustomXml->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (elem.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerMoveRange::Id);
		m_oBcw.m_oStream.WriteLONG(elem.m_oId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteComment(OOX::EElementType eType, nullable<SimpleTypes::CDecimalNumber>& oId)
{
	int nCurPos = 0;
	if(oId.IsInit() && (NULL == m_mapIgnoreComments || m_mapIgnoreComments->end() == m_mapIgnoreComments->find(oId->GetValue())))
	{
		switch(eType)
		{
		case OOX::et_w_commentRangeStart:	nCurPos = m_oBcw.WriteItemStart(c_oSerParType::CommentStart);	break;
		case OOX::et_w_commentRangeEnd:		nCurPos = m_oBcw.WriteItemStart(c_oSerParType::CommentEnd);		break;

		case OOX::et_w_commentReference:	nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::CommentReference);break;
		default:
			break;
		}

		int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_CommentsType::Id);
		m_oBcw.m_oStream.WriteLONG(oId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos2);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WritePermission(OOX::Logic::CPermStart* pPerm)
{
	if (!pPerm) return;

	int nCurPos2 = m_oBcw.WriteItemStart(c_oSerParType::PermStart);

	if (pPerm->m_sId.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::Id);
		m_oBcw.m_oStream.WriteStringW3(pPerm->m_sId.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (pPerm->m_oDisplacedByCustomXml.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::DisplacedByCustomXml);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pPerm->m_oDisplacedByCustomXml->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (pPerm->m_oColFirst.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::ColFirst);
		m_oBcw.m_oStream.WriteLONG(pPerm->m_oColFirst->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (pPerm->m_oColLast.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::ColLast);
		m_oBcw.m_oStream.WriteLONG(pPerm->m_oColLast->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (pPerm->m_sEd.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::Ed);
		m_oBcw.m_oStream.WriteStringW3(pPerm->m_sEd.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (pPerm->m_oEdGrp.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::EdGroup);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pPerm->m_oEdGrp->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	m_oBcw.WriteItemWithLengthEnd(nCurPos2);
}
void BinaryDocumentTableWriter::WritePermission(OOX::Logic::CPermEnd* pPerm)
{
	if (!pPerm) return;
	
	int nCurPos2 = m_oBcw.WriteItemStart(c_oSerParType::PermEnd);
	if (pPerm->m_sId.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::Id);
		m_oBcw.m_oStream.WriteStringW3(pPerm->m_sId.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (pPerm->m_oDisplacedByCustomXml.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerPermission::DisplacedByCustomXml);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pPerm->m_oDisplacedByCustomXml->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	m_oBcw.WriteItemWithLengthEnd(nCurPos2);
}
void BinaryDocumentTableWriter::WriteFldChar(OOX::Logic::CFldChar* pFldChar)
{
	int nCurPos = 0;
	if(pFldChar->m_oFldCharType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FldSimpleType::CharType);
		m_oBcw.m_oStream.WriteBYTE((BYTE)pFldChar->m_oFldCharType->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//FFData
	if (pFldChar->m_oFFData.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FldSimpleType::FFData);
		WriteFFData(pFldChar->m_oFFData.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteFldSimple(OOX::Logic::CFldSimple* pFldSimple)
{
	int nCurPos = 0;
	if(pFldSimple->m_sInstr.IsInit() && !pFldSimple->m_sInstr->empty())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerParType::FldSimple);
			WriteFldSimpleContent(pFldSimple);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	else
		WriteParagraphContent(pFldSimple->m_arrItems);
}
void BinaryDocumentTableWriter::WriteFldSimpleContent(OOX::Logic::CFldSimple* pFldSimple)
{
	int nCurPos = 0;
	//порядок записи важен
	//Instr
	nCurPos = m_oBcw.WriteItemStart(c_oSer_FldSimpleType::Instr);
		m_oBcw.m_oStream.WriteStringW3(*pFldSimple->m_sInstr);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
	//FFData
	if(pFldSimple->m_oFFData.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FldSimpleType::FFData);
		WriteFFData(pFldSimple->m_oFFData.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Content
	nCurPos = m_oBcw.WriteItemStart(c_oSer_FldSimpleType::Content);
		WriteParagraphContent(pFldSimple->m_arrItems);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteFFData(const OOX::Logic::CFFData& oFFData)
{
	int nCurPos = 0;
	if(oFFData.m_oCalcOnExit.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::CalcOnExit);
		m_oBcw.m_oStream.WriteBOOL(oFFData.m_oCalcOnExit->m_oVal.ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oCheckBox.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::CheckBox);
		WriteFFCheckBox(oFFData.m_oCheckBox.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oDDList.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::DDList);
		WriteDDList(oFFData.m_oDDList.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oEnabled.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::Enabled);
		m_oBcw.m_oStream.WriteBOOL(oFFData.m_oEnabled->m_oVal.ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oEntryMacro.IsInit() && oFFData.m_oEntryMacro->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::EntryMacro);
		m_oBcw.m_oStream.WriteStringW3(oFFData.m_oEntryMacro->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oExitMacro.IsInit() && oFFData.m_oExitMacro->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::ExitMacro);
		m_oBcw.m_oStream.WriteStringW3(oFFData.m_oExitMacro->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oHelpText.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::HelpText);
		WriteFFHelpText(oFFData.m_oHelpText.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oLabel.IsInit() && oFFData.m_oLabel->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::Label);
		m_oBcw.m_oStream.WriteLONG(*oFFData.m_oLabel->m_oVal);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oName.IsInit() && oFFData.m_oName->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::Name);
		m_oBcw.m_oStream.WriteStringW3(oFFData.m_oName->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oStatusText.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::StatusText);
		WriteFFStatusText(oFFData.m_oStatusText.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oTabIndex.IsInit() && oFFData.m_oTabIndex->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::TabIndex);
		m_oBcw.m_oStream.WriteLONG(oFFData.m_oTabIndex->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oFFData.m_oTextInput.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::TextInput);
		WriteTextInput(oFFData.m_oTextInput.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteFFCheckBox(const OOX::Logic::CFFCheckBox& oCheckBox)
{
	int nCurPos = 0;
	if(oCheckBox.m_oChecked.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::CBChecked);
		m_oBcw.m_oStream.WriteBOOL(oCheckBox.m_oChecked->m_oVal.ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oCheckBox.m_oDefault.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::CBDefault);
		m_oBcw.m_oStream.WriteBOOL(oCheckBox.m_oDefault->m_oVal.ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oCheckBox.m_oSize.IsInit() && oCheckBox.m_oSize->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::CBSize);
		m_oBcw.m_oStream.WriteULONG(oCheckBox.m_oSize->m_oVal->ToHps());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oCheckBox.m_oSizeAuto.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::CBSizeAuto);
		m_oBcw.m_oStream.WriteBOOL(oCheckBox.m_oSizeAuto->m_oVal.ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteDDList(const OOX::Logic::CFFDDList& oDDList)
{
	int nCurPos = 0;
	if(oDDList.m_oDefault.IsInit() && oDDList.m_oDefault->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::DLDefault);
		m_oBcw.m_oStream.WriteLONG(*oDDList.m_oDefault->m_oVal);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oDDList.m_oResult.IsInit() && oDDList.m_oResult->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::DLResult);
		m_oBcw.m_oStream.WriteLONG(*oDDList.m_oResult->m_oVal);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	for(size_t i = 0 ; i < oDDList.m_arrListEntry.size(); ++i)
	{
		ComplexTypes::Word::String* pVal = oDDList.m_arrListEntry[i];
		if(pVal->m_sVal.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::DLListEntry);
			m_oBcw.m_oStream.WriteStringW3(pVal->m_sVal.get());
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
}
void BinaryDocumentTableWriter::WriteFFHelpText(const ComplexTypes::Word::CFFHelpText& oHelpText)
{
	int nCurPos = 0;
	if(oHelpText.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::HTType);
		m_oBcw.m_oStream.WriteBYTE(oHelpText.m_oType->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oHelpText.m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::HTVal);
		m_oBcw.m_oStream.WriteStringW3(oHelpText.m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteFFStatusText(const ComplexTypes::Word::CFFStatusText& oStatusText)
{
	int nCurPos = 0;
	if(oStatusText.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::HTType);
		m_oBcw.m_oStream.WriteBYTE(oStatusText.m_oType->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oStatusText.m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::HTVal);
		m_oBcw.m_oStream.WriteStringW3(oStatusText.m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteTextInput(const OOX::Logic::CFFTextInput& oTextInput)
{
	int nCurPos = 0;
	if(oTextInput.m_oDefault.IsInit() && oTextInput.m_oDefault->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::TIDefault);
		m_oBcw.m_oStream.WriteStringW3(oTextInput.m_oDefault->m_sVal.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oTextInput.m_oFormat.IsInit() && oTextInput.m_oFormat->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::TIFormat);
		m_oBcw.m_oStream.WriteStringW3(oTextInput.m_oFormat->m_sVal.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oTextInput.m_oMaxLength.IsInit() && oTextInput.m_oMaxLength->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::TIMaxLength);
		m_oBcw.m_oStream.WriteLONG(*oTextInput.m_oMaxLength->m_oVal);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oTextInput.m_oType.IsInit() && oTextInput.m_oType->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFFData::TIType);
		m_oBcw.m_oStream.WriteBYTE(oTextInput.m_oType->m_oVal->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteBookmarkStart(const OOX::Logic::CBookmarkStart& oBookmarkStart)
{
	int nCurPos = 0;
	if (oBookmarkStart.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBookmark::Id);
		m_oBcw.m_oStream.WriteLONG(oBookmarkStart.m_oId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oBookmarkStart.m_sName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBookmark::Name);
		m_oBcw.m_oStream.WriteStringW3(oBookmarkStart.m_sName.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oBookmarkStart.m_oDisplacedByCustomXml.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBookmark::DisplacedByCustomXml);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oBookmarkStart.m_oDisplacedByCustomXml->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oBookmarkStart.m_oColFirst.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBookmark::ColFirst);
		m_oBcw.m_oStream.WriteLONG(oBookmarkStart.m_oColFirst->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oBookmarkStart.m_oColLast.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBookmark::ColLast);
		m_oBcw.m_oStream.WriteLONG(oBookmarkStart.m_oColLast->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteBookmarkEnd(const OOX::Logic::CBookmarkEnd& oBookmarkEnd)
{
	int nCurPos = 0;
	if (oBookmarkEnd.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBookmark::Id);
		m_oBcw.m_oStream.WriteLONG(oBookmarkEnd.m_oId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oBookmarkEnd.m_oDisplacedByCustomXml.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBookmark::DisplacedByCustomXml);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oBookmarkEnd.m_oDisplacedByCustomXml->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}

void BinaryDocumentTableWriter::WriteHyperlink(OOX::Logic::CHyperlink* pHyperlink)
{
	int nCurPos = 0;
	std::wstring sLink;
	if(pHyperlink->m_oId.IsInit())
	{
		OOX::Rels::CRelationShip* oRels = NULL;
		smart_ptr<OOX::File> pFile = m_oParamsDocumentWriter.m_pRels->Find( OOX::RId(pHyperlink->m_oId.get().GetValue()));
		if (pFile.IsInit() && OOX::FileTypes::HyperLink == pFile->type())
		{
			OOX::HyperLink* pHyperlinkFile = static_cast<OOX::HyperLink*>(pFile.operator ->());
			sLink = pHyperlinkFile->Uri().GetPath();
		}
	}
	else if (pHyperlink->m_sDestinition.IsInit())
	{
		sLink = *pHyperlink->m_sDestinition;
	}

	nCurPos = m_oBcw.WriteItemStart(c_oSerParType::Hyperlink);
		WriteHyperlinkContent(sLink, pHyperlink);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteHyperlinkContent(std::wstring& sLink, OOX::Logic::CHyperlink* pHyperlink)
{
	int nCurPos = 0;
//Link
	if(!sLink.empty())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HyperlinkType::Link);
			m_oBcw.m_oStream.WriteStringW3(sLink);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Anchor
	if(pHyperlink->m_sAnchor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HyperlinkType::Anchor);
			m_oBcw.m_oStream.WriteStringW3(*pHyperlink->m_sAnchor);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Tooltip
	if(pHyperlink->m_sTooltip.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HyperlinkType::Tooltip);
			m_oBcw.m_oStream.WriteStringW3(*pHyperlink->m_sTooltip);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//History
	if(pHyperlink->m_oHistory.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HyperlinkType::History);
			m_oBcw.m_oStream.WriteBOOL(pHyperlink->m_oHistory->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//DocLocation
	if(pHyperlink->m_sDocLocation.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HyperlinkType::DocLocation);
			m_oBcw.m_oStream.WriteStringW3(*pHyperlink->m_sDocLocation);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//TgtFrame
	if(pHyperlink->m_sTgtFrame.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HyperlinkType::TgtFrame);
			m_oBcw.m_oStream.WriteStringW3(*pHyperlink->m_sTgtFrame);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Content
	nCurPos = m_oBcw.WriteItemStart(c_oSer_HyperlinkType::Content);
		WriteParagraphContent(pHyperlink->m_arrItems, true);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteRun(OOX::Logic::CRun* pRun, bool bHyperlink, bool bMathRun)
{
	int nCurPos			= 0;
	bool bWasText		= false;
	int nRecordType		= 0;

	std::vector<OOX::WritingElement*>::iterator nIndexStart   = pRun->m_arrItems.begin();
	std::vector<OOX::WritingElement*>::iterator nIndexEnd     = pRun->m_arrItems.end();

	if (bMathRun)
		nRecordType = c_oSer_OMathContentType::Run;
	else
		nRecordType = c_oSerParType::Run;

//Разбиваем массив по знаку et_w_sym
	for (std::vector<OOX::WritingElement*>::iterator it = nIndexStart; it != nIndexEnd; ++it)
	{
		OOX::WritingElement* item = (*it);

		if(OOX::et_w_sym == item->getType())
		{
			if(bWasText)
			{
				bWasText = false;
				nCurPos = m_oBcw.WriteItemStart(nRecordType);
					WritePreparedRun(pRun, bHyperlink, nIndexStart, it);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			nCurPos = m_oBcw.WriteItemStart(nRecordType);
				std::vector<OOX::WritingElement*>::iterator it_next = it; it_next++;
				WritePreparedRun( pRun, bHyperlink, it, it_next);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
			nIndexStart = it; nIndexStart++;
		}
		else if(OOX::et_w_rPr != item->getType())
			bWasText = true;
	}
	if(nIndexStart != nIndexEnd)
	{
		nCurPos = m_oBcw.WriteItemStart(nRecordType);
			WritePreparedRun( pRun, bHyperlink, nIndexStart, nIndexEnd);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteMathArgNodes(const std::vector<OOX::WritingElement*>& arrItems)
{
	for (size_t i = 0; i < arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		OOX::EElementType eType = item->getType();
		int nCurPos = 0;
		switch(eType)
		{
		case OOX::et_m_acc:
			{
				OOX::Logic::CAcc* pAcc = static_cast<OOX::Logic::CAcc*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Acc);

				if ( pAcc->m_oAccPr.IsInit() )
					WriteMathAccPr(pAcc->m_oAccPr.get());
				if ( pAcc->m_oElement.IsInit() )
					WriteMathElement(pAcc->m_oElement.get());
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_argPr:
			{
				OOX::Logic::CArgPr* pArgPr = static_cast<OOX::Logic::CArgPr*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::ArgPr);

				if ( pArgPr->m_oArgSz.IsInit() )
					WriteMathArgSz(pArgPr->m_oArgSz.get());
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_bar:
			{
				OOX::Logic::CBar* pBar = static_cast<OOX::Logic::CBar*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Bar);

				if ( pBar->m_oBarPr.IsInit() )
					WriteMathBarPr(pBar->m_oBarPr.get());
				if ( pBar->m_oElement.IsInit() )
					WriteMathElement(pBar->m_oElement.get());
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_borderBox:
			{
				OOX::Logic::CBorderBox* pBorderBox = static_cast<OOX::Logic::CBorderBox*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::BorderBox);

				if ( pBorderBox->m_oBorderBoxPr.IsInit() )
					WriteMathBorderBoxPr(pBorderBox->m_oBorderBoxPr.get());
				if ( pBorderBox->m_oElement.IsInit() )
					WriteMathElement(pBorderBox->m_oElement.get());
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_box:
			{
				OOX::Logic::CBox* pBox = static_cast<OOX::Logic::CBox*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Box);

				if ( pBox->m_oBoxPr.IsInit() )
					WriteMathBoxPr(pBox->m_oBoxPr.get());
				if ( pBox->m_oElement.IsInit() )
					WriteMathElement(pBox->m_oElement.get());
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_ctrlPr:
			{
				OOX::Logic::CCtrlPr* pCtrlPr = static_cast<OOX::Logic::CCtrlPr*>(item);
				WriteMathCtrlPr(*pCtrlPr, c_oSer_OMathContentType::CtrlPr);
				break;
			}
		case OOX::et_m_d:
			{
				OOX::Logic::CDelimiter* pDelimiter = static_cast<OOX::Logic::CDelimiter*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Delimiter);

				WriteMathDelimiter(pDelimiter->m_arrItems, pDelimiter->m_lColumn);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_del:
			{
				OOX::Logic::CDel* pDel = static_cast<OOX::Logic::CDel*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Del);
				WriteDel(*pDel);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_eqArr:
			{
				OOX::Logic::CEqArr* pEqArr = static_cast<OOX::Logic::CEqArr*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::EqArr);

				WriteMathEqArr(pEqArr->m_arrItems, pEqArr->m_lRow);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_f:
			{
				OOX::Logic::CFraction* pFraction = static_cast<OOX::Logic::CFraction*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Fraction);

				if ( pFraction->m_oFPr.IsInit() )
					WriteMathFPr(pFraction->m_oFPr.get());
				if ( pFraction->m_oDen.IsInit() )
					WriteMathDen(pFraction->m_oDen.get());
				if ( pFraction->m_oNum.IsInit() )
					WriteMathNum(pFraction->m_oNum.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_func:
			{
				OOX::Logic::CFunc* pFunc = static_cast<OOX::Logic::CFunc*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Func);

				if ( pFunc->m_oFuncPr.IsInit() )
					WriteMathFuncPr(pFunc->m_oFuncPr.get());
				if ( pFunc->m_oElement.IsInit() )
					WriteMathElement(pFunc->m_oElement.get());
				if ( pFunc->m_oFName.IsInit() )
					WriteMathFName(pFunc->m_oFName.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_groupChr:
			{
				OOX::Logic::CGroupChr* pGroupChr = static_cast<OOX::Logic::CGroupChr*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::GroupChr);

				if ( pGroupChr->m_oGroupChrPr.IsInit() )
					WriteMathGroupChrPr(pGroupChr->m_oGroupChrPr.get());
				if ( pGroupChr->m_oElement.IsInit() )
					WriteMathElement(pGroupChr->m_oElement.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_ins:
			{
				OOX::Logic::CIns* pIns = static_cast<OOX::Logic::CIns*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Ins);
				WriteIns(*pIns);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_limLow:
			{
				OOX::Logic::CLimLow* pLimLow = static_cast<OOX::Logic::CLimLow*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::LimLow);

				if ( pLimLow->m_oLimLowPr.IsInit() )
					WriteMathLimLowPr(pLimLow->m_oLimLowPr.get());
				if ( pLimLow->m_oElement.IsInit() )
					WriteMathElement(pLimLow->m_oElement.get());
				if ( pLimLow->m_oLim.IsInit() )
					WriteMathLim(pLimLow->m_oLim.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_limUpp:
			{
				OOX::Logic::CLimUpp* pLimUpp = static_cast<OOX::Logic::CLimUpp*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::LimUpp);

				if ( pLimUpp->m_oLimUppPr.IsInit() )
					WriteMathLimUppPr(pLimUpp->m_oLimUppPr.get());
				if ( pLimUpp->m_oElement.IsInit() )
					WriteMathElement(pLimUpp->m_oElement.get());
				if ( pLimUpp->m_oLim.IsInit() )
					WriteMathLim(pLimUpp->m_oLim.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_m:
			{
				OOX::Logic::CMatrix* pMatrix = static_cast<OOX::Logic::CMatrix*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Matrix);

				LONG lCol = 0;
//TODO убрать, тк при отсутствии m:mcs, к-во столбцов должно разруливаться динамически в скрипте
				for (std::vector<OOX::WritingElement*>::iterator jt = pMatrix->m_arrItems.begin(); jt != pMatrix->m_arrItems.end(); jt++)
				{
					OOX::WritingElement* item = *jt;
					if (item->getType() == OOX::et_m_mr)
					{
						OOX::Logic::CMr* pMr = static_cast<OOX::Logic::CMr*>(item);
						lCol = pMr->GetCol();
						break;
					}
				}
				WriteMathMatrix(pMatrix->m_arrItems, pMatrix->m_lRow, lCol);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_nary:
			{
				OOX::Logic::CNary* pNary = static_cast<OOX::Logic::CNary*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Nary);

				if ( pNary->m_oNaryPr.IsInit() )
					WriteMathNaryPr(pNary->m_oNaryPr.get());
				if ( pNary->m_oSub.IsInit() )
					WriteMathSub(pNary->m_oSub.get());
				if ( pNary->m_oSup.IsInit() )
					WriteMathSup(pNary->m_oSup.get());
				if ( pNary->m_oElement.IsInit() )
					WriteMathElement(pNary->m_oElement.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_oMath:
			{
				OOX::Logic::COMath* pOMath = static_cast<OOX::Logic::COMath*>(item);

				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::OMath);
					WriteMathArgNodes(pOMath->m_arrItems);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_oMathPara:
			{
				OOX::Logic::COMathPara* pOMathPara = static_cast<OOX::Logic::COMathPara*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::OMathPara);

				WriteMathOMathPara(pOMathPara->m_arrItems);

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_phant:
			{
				OOX::Logic::CPhant* pPhant = static_cast<OOX::Logic::CPhant*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Phant);

				if ( pPhant->m_oPhantPr.IsInit() )
					WriteMathPhantPr(pPhant->m_oPhantPr.get());
				if ( pPhant->m_oElement.IsInit() )
					WriteMathElement(pPhant->m_oElement.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_r:
			{
				OOX::Logic::CMRun* pMRun = static_cast<OOX::Logic::CMRun*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MRun);
				WriteMathRunContent(pMRun);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_rad:
			{
				OOX::Logic::CRad* pRad = static_cast<OOX::Logic::CRad*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Rad);

				if ( pRad->m_oRadPr.IsInit() )
					WriteMathRadPr(pRad->m_oRadPr.get());
				if ( pRad->m_oDeg.IsInit() )
					WriteMathDeg(pRad->m_oDeg.get());
				if ( pRad->m_oElement.IsInit() )
					WriteMathElement(pRad->m_oElement.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_sPre:
			{
				OOX::Logic::CSPre* pSPre = static_cast<OOX::Logic::CSPre*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SPre);

				if ( pSPre->m_oSPrePr.IsInit() )
					WriteMathSPrePr(pSPre->m_oSPrePr.get());
				if ( pSPre->m_oSub.IsInit() )
					WriteMathSub(pSPre->m_oSub.get());
				if ( pSPre->m_oSup.IsInit() )
					WriteMathSup(pSPre->m_oSup.get());
				if ( pSPre->m_oElement.IsInit() )
					WriteMathElement(pSPre->m_oElement.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_sSub:
			{
				OOX::Logic::CSSub* pSSub = static_cast<OOX::Logic::CSSub*>(item);
				int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SSub);

				if ( pSSub->m_oSSubPr.IsInit() )
					WriteMathSSubPr(pSSub->m_oSSubPr.get());
				if ( pSSub->m_oElement.IsInit() )
					WriteMathElement(pSSub->m_oElement.get());
				if ( pSSub->m_oSub.IsInit() )
					WriteMathSub(pSSub->m_oSub.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_sSubSup:
			{
				OOX::Logic::CSSubSup* pSSubSup = static_cast<OOX::Logic::CSSubSup*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SSubSup);

				if ( pSSubSup->m_oSSubSupPr.IsInit() )
					WriteMathSSubSupPr(pSSubSup->m_oSSubSupPr.get());
				if ( pSSubSup->m_oElement.IsInit() )
					WriteMathElement(pSSubSup->m_oElement.get());
				if ( pSSubSup->m_oSub.IsInit() )
					WriteMathSub(pSSubSup->m_oSub.get());
				if ( pSSubSup->m_oSup.IsInit() )
					WriteMathSup(pSSubSup->m_oSup.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_sSup:
			{
				OOX::Logic::CSSup* pSSup = static_cast<OOX::Logic::CSSup*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SSup);

				if ( pSSup->m_oSSupPr.IsInit() )
					WriteMathSSupPr(pSSup->m_oSSupPr.get());
				if ( pSSup->m_oElement.IsInit() )
					WriteMathElement(pSSup->m_oElement.get());
				if ( pSSup->m_oSup.IsInit() )
					WriteMathSup(pSSup->m_oSup.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_bookmarkStart:
			{
				OOX::Logic::CBookmarkStart* pBookmarkStart = static_cast<OOX::Logic::CBookmarkStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::BookmarkStart);
				WriteBookmarkStart(*pBookmarkStart);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_bookmarkEnd:
			{
				OOX::Logic::CBookmarkEnd* pBookmarkEnd = static_cast<OOX::Logic::CBookmarkEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::BookmarkEnd);
				WriteBookmarkEnd(*pBookmarkEnd);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveFromRangeStart:
			{
				OOX::Logic::CMoveFromRangeStart* pMoveFromRangeStart = static_cast<OOX::Logic::CMoveFromRangeStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MoveFromRangeStart);
				WriteMoveRangeStart(*pMoveFromRangeStart);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveFromRangeEnd:
			{
				OOX::Logic::CMoveFromRangeEnd* pMoveFromRangeEnd = static_cast<OOX::Logic::CMoveFromRangeEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MoveFromRangeEnd);
				WriteMoveRangeEnd(*pMoveFromRangeEnd);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveToRangeStart:
			{
				OOX::Logic::CMoveToRangeStart* pMoveToRangeStart = static_cast<OOX::Logic::CMoveToRangeStart*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MoveToRangeStart);
				WriteMoveRangeStart(*pMoveToRangeStart);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_moveToRangeEnd:
			{
				OOX::Logic::CMoveToRangeEnd* pMoveToRangeEnd = static_cast<OOX::Logic::CMoveToRangeEnd*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MoveToRangeEnd);
				WriteMoveRangeEnd(*pMoveToRangeEnd);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		default:
			break;
		}
	}
}
void BinaryDocumentTableWriter::WriteMathRunContent(OOX::Logic::CMRun* pMRun)
{
	if ( pMRun->m_oMRPr.IsInit() )
		WriteMathMRPr(pMRun->m_oMRPr.get());
	if ( pMRun->m_oRPr.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_OMathContentType::RPr);
		brPrs.Write_rPr(pMRun->m_oRPr.GetPointer());
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	if ( pMRun->m_oARPr.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_OMathContentType::ARPr);
		m_oBcw.m_oStream.WriteRecord2(0, pMRun->m_oARPr);
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	if ( pMRun->m_oMText.IsInit() )
		WriteMathText(pMRun->m_oMText.get());

	if ( pMRun->m_oBr.IsInit() )
	{
		int nBreakType = -1;
		switch(pMRun->m_oBr->m_oType.GetValue())
		{
		case SimpleTypes::brtypeColumn: nBreakType = c_oSer_OMathContentType::columnbreak;break;
		case SimpleTypes::brtypePage: nBreakType = c_oSer_OMathContentType::pagebreak;break;
		case SimpleTypes::brtypeTextWrapping: nBreakType = c_oSer_OMathContentType::linebreak;break;
		}
		if(-1 != nBreakType)
		{
			m_oBcw.m_oStream.WriteBYTE(nBreakType);
			m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
		}
	}
	if ( pMRun->m_oDel.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Del);
		WriteMathDel(pMRun->m_oDel.get());
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	if ( pMRun->m_oIns.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Ins);
		WriteMathIns(pMRun->m_oIns.get());
		m_oBcw.WriteItemEnd(nCurPos2);
	}
}
void BinaryDocumentTableWriter::WriteMathAccPr(const OOX::Logic::CAccPr &pAccPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::AccPr);

	if ( pAccPr.m_oChr.IsInit() )
		WriteMathChr(pAccPr.m_oChr.get());
	if ( pAccPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pAccPr.m_oCtrlPr.get());
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathAln(const OOX::Logic::CAln &pAln)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Aln);
	if (pAln.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pAln.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathAlnScr(const OOX::Logic::CAlnScr &pAlnScr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::AlnScr);
	if (pAlnScr.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pAlnScr.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathArgPr(const OOX::Logic::CArgPr &pArgPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::ArgPr);

	if ( pArgPr.m_oArgSz.IsInit() )
		WriteMathArgSz(pArgPr.m_oArgSz.get());
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathArgSz(const OOX::Logic::CArgSz &pArgSz)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::ArgSz);
	if (pArgSz.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pArgSz.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathBarPr(const OOX::Logic::CBarPr &pBarPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::BarPr);

	if ( pBarPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pBarPr.m_oCtrlPr.get());
	if ( pBarPr.m_oPos.IsInit() )
		WriteMathPos(pBarPr.m_oPos.get());
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathBaseJc(const OOX::Logic::CBaseJc &pBaseJc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::BaseJc);
	if (pBaseJc.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pBaseJc.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathBegChr(const OOX::Logic::CBegChr &pBegChr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::BegChr);
	if (pBegChr.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(pBegChr.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathBorderBoxPr(const OOX::Logic::CBorderBoxPr &pBorderBoxPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::BorderBoxPr);

	if ( pBorderBoxPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pBorderBoxPr.m_oCtrlPr.get());

	if ( pBorderBoxPr.m_oHideBot.IsInit() )
		WriteMathHideBot(pBorderBoxPr.m_oHideBot.get());
	if ( pBorderBoxPr.m_oHideLeft.IsInit() )
		WriteMathHideLeft(pBorderBoxPr.m_oHideLeft.get());
	if ( pBorderBoxPr.m_oHideRight.IsInit() )
		WriteMathHideRight(pBorderBoxPr.m_oHideRight.get());
	if ( pBorderBoxPr.m_oHideTop.IsInit() )
		WriteMathHideTop(pBorderBoxPr.m_oHideTop.get());
	if ( pBorderBoxPr.m_oStrikeBLTR.IsInit() )
		WriteMathStrikeBLTR(pBorderBoxPr.m_oStrikeBLTR.get());
	if ( pBorderBoxPr.m_oStrikeH.IsInit() )
		WriteMathStrikeH(pBorderBoxPr.m_oStrikeH.get());
	if ( pBorderBoxPr.m_oStrikeTLBR.IsInit() )
		WriteMathStrikeTLBR(pBorderBoxPr.m_oStrikeTLBR.get());
	if ( pBorderBoxPr.m_oStrikeV.IsInit() )
		WriteMathStrikeV(pBorderBoxPr.m_oStrikeV.get());
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathBoxPr(const OOX::Logic::CBoxPr &pBoxPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::BoxPr);

	if ( pBoxPr.m_oAln.IsInit() )
		WriteMathAln(pBoxPr.m_oAln.get());
	if ( pBoxPr.m_oBrk.IsInit() )
		WriteMathBrk(pBoxPr.m_oBrk.get());
	if ( pBoxPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pBoxPr.m_oCtrlPr.get());
	if ( pBoxPr.m_oDiff.IsInit() )
		WriteMathDiff(pBoxPr.m_oDiff.get());
	if ( pBoxPr.m_oNoBreak.IsInit() )
		WriteMathNoBreak(pBoxPr.m_oNoBreak.get());
	if ( pBoxPr.m_oOpEmu.IsInit() )
		WriteMathOpEmu(pBoxPr.m_oOpEmu.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathBrk(const OOX::Logic::CBrk &pBrk)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Brk);
	if (pBrk.m_alnAt.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::AlnAt);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pBrk.m_alnAt->GetValue());
	}
	//заглушка для <m:brk>
	else
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(false);
	}
	m_oBcw.WriteItemEnd(nCurPos);

}
void BinaryDocumentTableWriter::WriteMathCGp(const OOX::Logic::CCGp &pCGp)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::CGp);
	if (pCGp.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pCGp.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathCGpRule(const OOX::Logic::CCGpRule &pCGpRule)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::CGpRule);
	if (pCGpRule.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pCGpRule.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathChr(const OOX::Logic::CChr &pChr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Chr);
	if (pChr.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		std::wstring str = pChr.m_val->GetValue();
		m_oBcw.m_oStream.WriteStringW(pChr.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathCount(const OOX::Logic::CCount &pCount)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Count);
	if (pCount.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pCount.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathCSp(const OOX::Logic::CCSp &pCSp)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::CSp);
	if (pCSp.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pCSp.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathCtrlPr(const OOX::Logic::CCtrlPr &pCtrlPr, BYTE type)
{
	int nCurPos = m_oBcw.WriteItemStart(type);

	if ( pCtrlPr.m_oRPr.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSerRunType::rPr);
		brPrs.Write_rPr(pCtrlPr.m_oRPr.GetPointer());
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	if ( pCtrlPr.m_oARPr.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSerRunType::arPr);
		m_oBcw.m_oStream.WriteRecord2(0, pCtrlPr.m_oARPr);
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	if ( pCtrlPr.m_oDel.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSerRunType::del);
		brPrs.Write_rPrChange(pCtrlPr.m_oDel.get());
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	if ( pCtrlPr.m_oIns.IsInit() )
	{
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSerRunType::ins);
		brPrs.Write_rPrChange(pCtrlPr.m_oIns.get());
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathDelimiter(const std::vector<OOX::WritingElement*> & arrItems, LONG &lColumn)
{
	for(size_t i = 0; i < arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		OOX::EElementType eType = item->getType();
		int nCurPos = 0;
		switch(eType)
		{
		case OOX::et_m_dPr:
			{
				OOX::Logic::CDelimiterPr* pDelimiterPr = static_cast<OOX::Logic::CDelimiterPr*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::DelimiterPr);

				if ( lColumn )
					WriteMathColumn (lColumn);
				if ( pDelimiterPr->m_oBegChr.IsInit() )
					WriteMathBegChr(pDelimiterPr->m_oBegChr.get());
				if ( pDelimiterPr->m_oEndChr.IsInit() )
					WriteMathEndChr(pDelimiterPr->m_oEndChr.get());
				if ( pDelimiterPr->m_oCtrlPr.IsInit() )
					WriteMathCtrlPr(pDelimiterPr->m_oCtrlPr.get());
				if ( pDelimiterPr->m_oGrow.IsInit() )
					WriteMathGrow(pDelimiterPr->m_oGrow.get());
				if ( pDelimiterPr->m_oSepChr.IsInit() )
					WriteMathSepChr(pDelimiterPr->m_oSepChr.get());
				if ( pDelimiterPr->m_oShp.IsInit() )
					WriteMathShp(pDelimiterPr->m_oShp.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_e:
			{
				OOX::Logic::CElement* pElement = static_cast<OOX::Logic::CElement*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Element);

				WriteMathArgNodes(pElement->m_arrItems);

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		default:
			break;
		}
	}
}
void BinaryDocumentTableWriter::WriteMathDeg(const OOX::Logic::CDeg &pDeg)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Deg);
	WriteMathArgNodes(pDeg.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathDegHide(const OOX::Logic::CDegHide &pDegHide)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::DegHide);
	if (pDegHide.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pDegHide.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathDen(const OOX::Logic::CDen &pDen)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Den);
	WriteMathArgNodes(pDen.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathDiff(const OOX::Logic::CDiff &pDiff)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Diff);
	if (pDiff.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pDiff.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathDelimiterPr(const OOX::Logic::CDelimiterPr &pDelimiterPr, long &lColumn)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::DelimiterPr);

	if ( lColumn )
		WriteMathColumn (lColumn);
	if ( pDelimiterPr.m_oBegChr.IsInit() )
		WriteMathBegChr(pDelimiterPr.m_oBegChr.get());
	if ( pDelimiterPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pDelimiterPr.m_oCtrlPr.get());
	if ( pDelimiterPr.m_oEndChr.IsInit() )
		WriteMathEndChr(pDelimiterPr.m_oEndChr.get());
	if ( pDelimiterPr.m_oGrow.IsInit() )
		WriteMathGrow(pDelimiterPr.m_oGrow.get());
	if ( pDelimiterPr.m_oSepChr.IsInit() )
		WriteMathSepChr(pDelimiterPr.m_oSepChr.get());
	if ( pDelimiterPr.m_oShp.IsInit() )
		WriteMathShp(pDelimiterPr.m_oShp.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathElement(const OOX::Logic::CElement &pElement)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Element);
	WriteMathArgNodes(pElement.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathEndChr(const OOX::Logic::CEndChr &pEndChr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::EndChr);
	if (pEndChr.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(pEndChr.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathEqArr(const std::vector<OOX::WritingElement*> & arrItems, LONG& lRow)
{
	for(size_t i = 0; i < arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		OOX::EElementType eType = item->getType();
		int nCurPos = 0;
		switch(eType)
		{
		case OOX::et_m_eqArrPr:
			{
				OOX::Logic::CEqArrPr* pEqArrPr = static_cast<OOX::Logic::CEqArrPr*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::EqArrPr);

				if ( lRow )
					WriteMathRow (lRow);
				if ( pEqArrPr->m_oBaseJc.IsInit() )
					WriteMathBaseJc(pEqArrPr->m_oBaseJc.get());
				if ( pEqArrPr->m_oCtrlPr.IsInit() )
					WriteMathCtrlPr(pEqArrPr->m_oCtrlPr.get());
				if ( pEqArrPr->m_oMaxDist.IsInit() )
					WriteMathMaxDist(pEqArrPr->m_oMaxDist.get());
				if ( pEqArrPr->m_oObjDist.IsInit() )
					WriteMathObjDist(pEqArrPr->m_oObjDist.get());
				if ( pEqArrPr->m_oRSp.IsInit() )
					WriteMathRSp(pEqArrPr->m_oRSp.get());
				if ( pEqArrPr->m_oRSpRule.IsInit() )
					WriteMathRSpRule(pEqArrPr->m_oRSpRule.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_e:
			{
				OOX::Logic::CElement* pElement = static_cast<OOX::Logic::CElement*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Element);

				WriteMathArgNodes(pElement->m_arrItems);

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		default:
			break;
		}
	}
}
void BinaryDocumentTableWriter::WriteMathFName(const OOX::Logic::CFName &pFName)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::FName);
	WriteMathArgNodes(pFName.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathFPr(const OOX::Logic::CFPr &pFPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::FPr);

	if ( pFPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pFPr.m_oCtrlPr.get());
	if ( pFPr.m_oType.IsInit() )
		WriteMathType(pFPr.m_oType.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathFuncPr(const OOX::Logic::CFuncPr &pFuncPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::FuncPr);

	if ( pFuncPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pFuncPr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathGroupChrPr(const OOX::Logic::CGroupChrPr &pGroupChrPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::GroupChrPr);

	if ( pGroupChrPr.m_oChr.IsInit() )
		WriteMathChr(pGroupChrPr.m_oChr.get());
	if ( pGroupChrPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pGroupChrPr.m_oCtrlPr.get());
	if ( pGroupChrPr.m_oPos.IsInit() )
		WriteMathPos(pGroupChrPr.m_oPos.get());
	if ( pGroupChrPr.m_oVertJc.IsInit() )
		WriteMathVertJc(pGroupChrPr.m_oVertJc.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathGrow(const OOX::Logic::CGrow &pGrow)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Grow);
	if (pGrow.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pGrow.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathHideBot(const OOX::Logic::CHideBot &pHideBot)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::HideBot);
	if (pHideBot.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pHideBot.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathHideLeft(const OOX::Logic::CHideLeft &pHideLeft)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::HideLeft);
	if (pHideLeft.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pHideLeft.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathHideRight(const OOX::Logic::CHideRight &pHideRight)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::HideRight);
	if (pHideRight.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pHideRight.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathHideTop(const OOX::Logic::CHideTop &pHideTop)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::HideTop);
	if (pHideTop.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pHideTop.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMJc(const OOX::Logic::CMJc &pMJc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::MJc);
	if (pMJc.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pMJc.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathLim(const OOX::Logic::CLim &pLim)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Lim);
	WriteMathArgNodes(pLim.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathLimLoc(const OOX::Logic::CLimLoc &pLimLoc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::LimLoc);
	if (pLimLoc.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pLimLoc.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathColumn(const LONG &lColumn)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Column);
	if (lColumn)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(lColumn);
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathRow(const LONG &lRow)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Row);
	if (lRow)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(lRow);
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathLimLowPr(const OOX::Logic::CLimLowPr &pLimLowPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::LimLowPr);

	if ( pLimLowPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pLimLowPr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathLimUppPr(const OOX::Logic::CLimUppPr &pLimUppPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::LimUppPr);

	if ( pLimUppPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pLimUppPr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathLit(const OOX::Logic::CLit &pLit)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Lit);
	if (pLit.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pLit.m_val->ToBool());
	}
	else
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(true);
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMatrix(const std::vector<OOX::WritingElement*> & arrItems, LONG &lRow, LONG &lCol)
{
	bool bColumn = false;
	for(size_t i = 0; i < arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		OOX::EElementType eType = item->getType();
		int nCurPos = 0;
		switch(eType)
		{
		case OOX::et_m_mPr:
			{
				OOX::Logic::CMPr* pMPr = static_cast<OOX::Logic::CMPr*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MPr);

				if (lRow)
					WriteMathRow(lRow);
				if ( pMPr->m_oBaseJc.IsInit() )
					WriteMathBaseJc(pMPr->m_oBaseJc.get());
				if ( pMPr->m_oCGp.IsInit() )
					WriteMathCGp(pMPr->m_oCGp.get());
				if ( pMPr->m_oCGpRule.IsInit() )
					WriteMathCGpRule(pMPr->m_oCGpRule.get());
				if ( pMPr->m_oCSp.IsInit() )
					WriteMathCSp(pMPr->m_oCSp.get());
				if ( pMPr->m_oCtrlPr.IsInit() )
					WriteMathCtrlPr(pMPr->m_oCtrlPr.get());
				if ( pMPr->m_oMcs.IsInit() )
					WriteMathMcs(pMPr->m_oMcs.get());
				else
					WriteMathMcs(lCol);
				if ( pMPr->m_oPlcHide.IsInit() )
					WriteMathPlcHide(pMPr->m_oPlcHide.get());
				if ( pMPr->m_oRSp.IsInit() )
					WriteMathRSp(pMPr->m_oRSp.get());
				if ( pMPr->m_oRSpRule.IsInit() )
					WriteMathRSpRule(pMPr->m_oRSpRule.get());

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_m_mr:
			{
				OOX::Logic::CMr* pMr = static_cast<OOX::Logic::CMr*>(item);
				nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Mr);

				WriteMathMr(pMr->m_arrItems);

				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		default:
			break;
		}
	}
}
void BinaryDocumentTableWriter::WriteMathMaxDist(const OOX::Logic::CMaxDist &pMaxDist)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::MaxDist);
	if (pMaxDist.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pMaxDist.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMc(const OOX::Logic::CMc &pMc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Mc);

	if ( pMc.m_oMcPr.IsInit() )
		WriteMathMcPr(pMc.m_oMcPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMcJc(const OOX::Logic::CMcJc &pMcJc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::McJc);
	if (pMcJc.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pMcJc.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMcPr(const OOX::Logic::CMcPr &pMcPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::McPr);

	if ( pMcPr.m_oMcJc.IsInit() )
		WriteMathMcJc(pMcPr.m_oMcJc.get());
	if ( pMcPr.m_oCount.IsInit() )
		WriteMathCount(pMcPr.m_oCount.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMcs(const OOX::Logic::CMcs &pMcs)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Mcs);

	for(size_t i = 0; i < pMcs.m_arrItems.size(); ++i)
	{
		OOX::WritingElement* item = pMcs.m_arrItems[i];

		OOX::EElementType eType = item->getType();
		int nCurPos1 = 0;
		if (eType == OOX::et_m_mc)
		{
			OOX::Logic::CMc* pMc = static_cast<OOX::Logic::CMc*>(item);
			nCurPos1 = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Mc);

			if ( pMc->m_oMcPr.IsInit() )
				WriteMathMcPr(pMc->m_oMcPr.get());

			m_oBcw.WriteItemEnd(nCurPos1);
		}
	}

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMcs ( LONG lColumn )
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Mcs);
	int nCurPos1 = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Mc);
	int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_OMathContentType::McPr);

	int nCurPos3 = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Count);
	m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG(lColumn);
	m_oBcw.WriteItemEnd(nCurPos3);

	m_oBcw.WriteItemEnd(nCurPos2);
	m_oBcw.WriteItemEnd(nCurPos1);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMPr(const OOX::Logic::CMPr &pMPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MPr);

	if ( pMPr.m_oBaseJc.IsInit() )
		WriteMathBaseJc(pMPr.m_oBaseJc.get());
	if ( pMPr.m_oCGp.IsInit() )
		WriteMathCGp(pMPr.m_oCGp.get());
	if ( pMPr.m_oCGpRule.IsInit() )
		WriteMathCGpRule(pMPr.m_oCGpRule.get());
	if ( pMPr.m_oCSp.IsInit() )
		WriteMathCSp(pMPr.m_oCSp.get());
	if ( pMPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pMPr.m_oCtrlPr.get());
	if ( pMPr.m_oMcs.IsInit() )
		WriteMathMcs(pMPr.m_oMcs.get());
	if ( pMPr.m_oPlcHide.IsInit() )
		WriteMathPlcHide(pMPr.m_oPlcHide.get());
	if ( pMPr.m_oRSp.IsInit() )
		WriteMathRSp(pMPr.m_oRSp.get());
	if ( pMPr.m_oRSpRule.IsInit() )
		WriteMathRSpRule(pMPr.m_oRSpRule.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMr(const std::vector<OOX::WritingElement*> & arrItems)
{
	for(size_t i = 0; i < arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		OOX::EElementType eType = item->getType();
		int nCurPos = 0;
		if (eType == OOX::et_m_e)
		{
			OOX::Logic::CElement* pElement = static_cast<OOX::Logic::CElement*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Element);

			WriteMathArgNodes(pElement->m_arrItems);

			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryDocumentTableWriter::WriteMathNaryPr(const OOX::Logic::CNaryPr &pNaryPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::NaryPr);

	if ( pNaryPr.m_oChr.IsInit() )
		WriteMathChr(pNaryPr.m_oChr.get());
	if ( pNaryPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pNaryPr.m_oCtrlPr.get());
	if ( pNaryPr.m_oGrow.IsInit() )
		WriteMathGrow(pNaryPr.m_oGrow.get());
	if ( pNaryPr.m_oLimLoc.IsInit() )
		WriteMathLimLoc(pNaryPr.m_oLimLoc.get());
	if ( pNaryPr.m_oSubHide.IsInit() )
		WriteMathSubHide(pNaryPr.m_oSubHide.get());
	if ( pNaryPr.m_oSupHide.IsInit() )
		WriteMathSupHide(pNaryPr.m_oSupHide.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathNoBreak(const OOX::Logic::CNoBreak &pNoBreak)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::NoBreak);
	if (pNoBreak.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pNoBreak.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathNor(const OOX::Logic::CNor &pNor)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Nor);
	m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
	if (pNor.m_val.IsInit())
		m_oBcw.m_oStream.WriteBOOL(pNor.m_val->ToBool());
	else
		m_oBcw.m_oStream.WriteBOOL(true);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathNum(const OOX::Logic::CNum &pNum)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Num);
	WriteMathArgNodes(pNum.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathObjDist(const OOX::Logic::CObjDist &pObjDist)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::ObjDist);
	if (pObjDist.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pObjDist.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathOMath(const OOX::Logic::COMath &pOMath)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::OMath);
	WriteMathArgNodes(pOMath.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathOMathPara(const std::vector<OOX::WritingElement*> & arrItems)
{
	for(size_t i = 0; i < arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		OOX::EElementType eType = item->getType();
		int nCurPos = 0;
		if (eType == OOX::et_m_oMath)
		{
			OOX::Logic::COMath* pOMath = static_cast<OOX::Logic::COMath*>(item);
			int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::OMath);
			WriteMathArgNodes(pOMath->m_arrItems);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if (eType == OOX::et_m_oMathParaPr)
		{
			OOX::Logic::COMathParaPr* pOMathParaPr = static_cast<OOX::Logic::COMathParaPr*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::OMathParaPr);

			if ( pOMathParaPr->m_oMJc.IsInit() )
				WriteMathMJc(pOMathParaPr->m_oMJc.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if (eType == OOX::et_w_r)
		{
			bool bHyperlink = false;
			bool bMathRun = true;
			OOX::Logic::CRun* pRun = static_cast<OOX::Logic::CRun*>(item);
			WriteRun(pRun, bHyperlink, bMathRun);
		}
	}
}
void BinaryDocumentTableWriter::WriteMathOMathParaPr(const OOX::Logic::COMathParaPr &pOMathParaPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::OMathParaPr);

	if ( pOMathParaPr.m_oMJc.IsInit() )
		WriteMathMJc(pOMathParaPr.m_oMJc.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathOpEmu(const OOX::Logic::COpEmu &pOpEmu)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::OpEmu);
	if (pOpEmu.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pOpEmu.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathPhantPr(const OOX::Logic::CPhantPr &pPhantPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::PhantPr);

	if ( pPhantPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pPhantPr.m_oCtrlPr.get());
	if ( pPhantPr.m_oShow.IsInit() )
		WriteMathShow(pPhantPr.m_oShow.get());
	if ( pPhantPr.m_oTransp.IsInit() )
		WriteMathTransp(pPhantPr.m_oTransp.get());
	if ( pPhantPr.m_oZeroAsc.IsInit() )
		WriteMathZeroAsc(pPhantPr.m_oZeroAsc.get());
	if ( pPhantPr.m_oZeroDesc.IsInit() )
		WriteMathZeroDesc(pPhantPr.m_oZeroDesc.get());
	if ( pPhantPr.m_oZeroWid.IsInit() )
		WriteMathZeroWid(pPhantPr.m_oZeroWid.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathPlcHide(const OOX::Logic::CPlcHide &pPlcHide)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::PlcHide);
	if (pPlcHide.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pPlcHide.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathPos(const OOX::Logic::CPos &pPos)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Pos);
	if (pPos.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pPos.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathRadPr(const OOX::Logic::CRadPr &pRadPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::RadPr);

	if ( pRadPr.m_oDegHide.IsInit() )
		WriteMathDegHide(pRadPr.m_oDegHide.get());
	if ( pRadPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pRadPr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathMRPr(const OOX::Logic::CMRPr &pMRPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MRPr);

	if ( pMRPr.m_oAln.IsInit())
		WriteMathAln(pMRPr.m_oAln.get());
	if ( pMRPr.m_oBrk.IsInit() )
		WriteMathBrk(pMRPr.m_oBrk.get());
	if ( pMRPr.m_oLit.IsInit() )
		WriteMathLit(pMRPr.m_oLit.get());
	if ( pMRPr.m_oNor.IsInit() )
		WriteMathNor(pMRPr.m_oNor.get());
	if ( pMRPr.m_oScr.IsInit() )
		WriteMathScr(pMRPr.m_oScr.get());
	if ( pMRPr.m_oSty.IsInit() )
		WriteMathSty(pMRPr.m_oSty.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathRSp(const OOX::Logic::CRSp &pRSp)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::RSp);
	if (pRSp.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pRSp.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathRSpRule(const OOX::Logic::CRSpRule &pRSpRule)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::RSpRule);
	if (pRSpRule.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(pRSpRule.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathScr(const OOX::Logic::CScr &pScr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Scr);
	if (pScr.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pScr.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSepChr(const OOX::Logic::CSepChr &pSepChr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::SepChr);
	if (pSepChr.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(pSepChr.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathShow(const OOX::Logic::CShow &pShow)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Show);
	if (pShow.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pShow.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathShp(const OOX::Logic::CShp &pShp)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Shp);
	if (pShp.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pShp.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSPrePr(const OOX::Logic::CSPrePr &pSPrePr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SPrePr);

	if ( pSPrePr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pSPrePr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSSubPr(const OOX::Logic::CSSubPr &pSSubPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SSubPr);

	if ( pSSubPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pSSubPr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSSubSupPr(const OOX::Logic::CSSubSupPr &pSSubSupPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SSubSupPr);

	if ( pSSubSupPr.m_oAlnScr.IsInit() )
		WriteMathAlnScr(pSSubSupPr.m_oAlnScr.get());
	if ( pSSubSupPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pSSubSupPr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSSupPr(const OOX::Logic::CSSupPr &pSSupPr)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::SSupPr);

	if ( pSSupPr.m_oCtrlPr.IsInit() )
		WriteMathCtrlPr(pSSupPr.m_oCtrlPr.get());

	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathStrikeBLTR(const OOX::Logic::CStrikeBLTR &pStrikeBLTR)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::StrikeBLTR);
	if (pStrikeBLTR.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pStrikeBLTR.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathStrikeH(const OOX::Logic::CStrikeH &pStrikeH)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::StrikeH);
	if (pStrikeH.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pStrikeH.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathStrikeTLBR(const OOX::Logic::CStrikeTLBR &pStrikeTLBR)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::StrikeTLBR);
	if (pStrikeTLBR.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pStrikeTLBR.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathStrikeV(const OOX::Logic::CStrikeV &pStrikeV)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::StrikeV);
	if (pStrikeV.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pStrikeV.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSty(const OOX::Logic::CSty &pSty)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Sty);
	if (pSty.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pSty.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSub(const OOX::Logic::CSub &pSub)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Sub);
	WriteMathArgNodes(pSub.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSubHide(const OOX::Logic::CSubHide &pSubHide)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::SubHide);
	if (pSubHide.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pSubHide.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSup(const OOX::Logic::CSup &pSup)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::Sup);
	WriteMathArgNodes(pSup.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathSupHide(const OOX::Logic::CSupHide &pSupHide)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::SupHide);
	if (pSupHide.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pSupHide.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathDel(const OOX::Logic::CMDel &pMDel)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(pMDel);
	if(pMDel.m_oRun.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::ContentRun);
		WriteMathRunContent(pMDel.m_oRun.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteMathIns(const OOX::Logic::CMIns &pMIns)
{
	int nCurPos = 0;
	m_oBcw.WriteTrackRevision(pMIns);

	if(pMIns.m_oRun.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::ContentRun);
		WriteMathRunContent(pMIns.m_oRun.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteMathText(const OOX::Logic::CMText &pMText)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathContentType::MText);
	if(!pMText.m_sText.empty())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(pMText.m_sText.c_str());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathTransp(const OOX::Logic::CTransp &pTransp)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Transp);
	if (pTransp.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pTransp.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathType(const OOX::Logic::CType &pType)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::Type);
	if (pType.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pType.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathVertJc(const OOX::Logic::CVertJc &pVertJc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::VertJc);
	if (pVertJc.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(pVertJc.m_val->GetValue());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathZeroAsc(const OOX::Logic::CZeroAsc &pZeroAsc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::ZeroAsc);
	if (pZeroAsc.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pZeroAsc.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathZeroDesc(const OOX::Logic::CZeroDesc &pZeroDesc)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::ZeroDesc);
	if (pZeroDesc.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pZeroDesc.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteMathZeroWid(const OOX::Logic::CZeroWid &pZeroWid)
{
	int nCurPos = m_oBcw.WriteItemStart(c_oSer_OMathBottomNodesType::ZeroWid);
	if (pZeroWid.m_val.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(pZeroWid.m_val->ToBool());
	}
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WritePreparedRun(OOX::Logic::CRun *pRun, bool bHyperlink, std::vector<OOX::WritingElement*>::iterator &start, std::vector<OOX::WritingElement*>::iterator &end)
{
	if (!pRun) return;

	int							nCurPos		= 0;
	OOX::Logic::CRunProperty*	oCur_rPr	= pRun->m_oRunProperty;

//Если первый элемент символ надо выставить в его настройки шрифт
	if(start != pRun->m_arrItems.end() && OOX::et_w_sym == (*start)->getType())
	{
		OOX::Logic::CSym* oSym = static_cast<OOX::Logic::CSym*>(*start);
		if(oSym->m_oFont.IsInit())
		{
			const std::wstring& sFont = oSym->m_oFont.get();
			if(NULL == oCur_rPr)
				oCur_rPr = new OOX::Logic::CRunProperty();

			oCur_rPr->m_oRFonts.Init();
			oCur_rPr->m_oRFonts->m_sAscii.Init();
			oCur_rPr->m_oRFonts->m_sAscii->append(sFont);
			oCur_rPr->m_oRFonts->m_sCs.Init();
			oCur_rPr->m_oRFonts->m_sCs->append(sFont);
			oCur_rPr->m_oRFonts->m_sEastAsia.Init();
			oCur_rPr->m_oRFonts->m_sEastAsia->append(sFont);
			oCur_rPr->m_oRFonts->m_sHAnsi.Init();
			oCur_rPr->m_oRFonts->m_sHAnsi->append(sFont);
		}
	}
//пишем rPr
	if(NULL != oCur_rPr)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::rPr);
			brPrs.Write_rPr(oCur_rPr);
		m_oBcw.WriteItemEnd(nCurPos);
	}

	//Content пишется начиная от индекса nIndexStart и заканчивая предшествующим элементом для nIndexStop
	nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::Content);
		WriteRunContent( start, end, bHyperlink);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteRunContent(std::vector<OOX::WritingElement*>::iterator &start, std::vector<OOX::WritingElement*>::iterator &end, bool bHyperlink)
{
	for ( std::vector<OOX::WritingElement*>::iterator it = start; it != end; ++it )
	{
		OOX::WritingElement* item = *it;
		switch (item->getType())
		{
		case OOX::et_w_br:
			{
				OOX::Logic::CBr* pBr = static_cast<OOX::Logic::CBr*>(item);
				int nBreakType = -1;
				switch(pBr->m_oType.GetValue())
				{
				case SimpleTypes::brtypeColumn:			nBreakType = c_oSerRunType::columnbreak;	break;
				case SimpleTypes::brtypePage:			nBreakType = c_oSerRunType::pagebreak;		break;
				case SimpleTypes::brtypeTextWrapping:
				{
					switch (pBr->m_oClear.GetValue())
					{
					case SimpleTypes::brclearAll:	nBreakType = c_oSerRunType::linebreakClearAll;		break;
					case SimpleTypes::brclearLeft:	nBreakType = c_oSerRunType::linebreakClearLeft;		break;
					case SimpleTypes::brclearRight:	nBreakType = c_oSerRunType::linebreakClearRight;	break;
					default:						nBreakType = c_oSerRunType::linebreak;				break;
					}
					
				}break;
				default:
					break;
				}
				if(-1 != nBreakType)
				{
					m_oBcw.m_oStream.WriteBYTE(nBreakType);
					m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
				}				
			}break;
		case OOX::et_w_cr:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::cr);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
				break;
			}
		case OOX::et_mc_alternateContent:
		case OOX::et_w_pict:
		case OOX::et_w_drawing:
			{
				WriteDrawingPptx(item);
				break;
			}
		case OOX::et_w_fldChar:
			{
				OOX::Logic::CFldChar* pFldChar = static_cast<OOX::Logic::CFldChar*>(item);
				int nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::fldChar);
				WriteFldChar(pFldChar);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			break;
		case OOX::et_w_delInstrText:
			{
				OOX::Logic::CDelInstrText* pInstrText = static_cast<OOX::Logic::CDelInstrText*>(item);
				WriteText(pInstrText->m_sText, c_oSerRunType::delInstrText);
			}
			break;
		case OOX::et_w_instrText:
			{
				OOX::Logic::CInstrText* pInstrText = static_cast<OOX::Logic::CInstrText*>(item);
				WriteText(pInstrText->m_sText, c_oSerRunType::instrText);
			}
			break;
		case OOX::et_w_nonBreakHyphen:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::noBreakHyphen);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
				break;
			}
			break;
		case OOX::et_w_pgNum:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::pagenum);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
			}
			break;
		case OOX::et_w_ptab:
			break;
		case OOX::et_w_rPr:
			break;
		case OOX::et_w_softHyphen:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::softHyphen);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
				break;
			}
			break;
		case OOX::et_w_sym:
			{
				OOX::Logic::CSym* oSym = static_cast<OOX::Logic::CSym*>(item);
				wchar_t ch = 0x0FFF & oSym->m_oChar->GetValue();
				std::wstring sText(&ch, 1);
				WriteText(sText, c_oSerRunType::run);
				break;
			}
		case OOX::et_w_delText:
			{
				std::wstring& sText = static_cast<OOX::Logic::CDelText*>(item)->m_sText;
				WriteText(sText, c_oSerRunType::delText);
			}
			break;
		case OOX::et_w_t:
			{
				std::wstring& sText = static_cast<OOX::Logic::CText*>(item)->m_sText;
				WriteText(sText, c_oSerRunType::run);
			}
			break;
		case OOX::et_w_tab:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::tab);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
			}
			break;
		case OOX::et_w_separator:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::separator);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
			}
			break;
		case OOX::et_w_continuationSeparator:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::continuationSeparator);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
			}
			break;
		case OOX::et_w_commentReference:
			{
				OOX::Logic::CCommentReference* pCommentReference = static_cast<OOX::Logic::CCommentReference*>(item);
				WriteComment(OOX::et_w_commentReference, pCommentReference->m_oId);
				break;
			}
		case OOX::et_w_object:
			{
				int nPosObject = m_oBcw.WriteItemStart(c_oSerRunType::object);

				OOX::Logic::CObject* pObject = static_cast<OOX::Logic::CObject*>(item);
				std::wstring* pXml = pObject ? pObject->m_sXml.GetPointer() : NULL;

				int nPosImageCache = m_oBcw.WriteItemStart(c_oSerRunType::pptxDrawing);
				WriteDrawing(pXml, NULL, NULL);
				m_oBcw.WriteItemEnd(nPosImageCache);

				m_oBcw.WriteItemEnd(nPosObject);
				break;
			}
		case OOX::et_w_footnoteRef:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::footnoteRef);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
			}
			break;
		case OOX::et_w_endnoteRef:
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRunType::endnoteRef);
				m_oBcw.m_oStream.WriteLONG(c_oSerPropLenType::Null);
			}
			break;
		case OOX::et_w_footnoteReference:
			{
				OOX::Logic::CFootnoteReference* pFootnoteReference = static_cast<OOX::Logic::CFootnoteReference*>(item);
				int nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::footnoteReference);
				WriteNoteRef(pFootnoteReference->m_oCustomMarkFollows, pFootnoteReference->m_oId);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		case OOX::et_w_endnoteReference:
			{
				OOX::Logic::CEndnoteReference* pEndnoteReference = static_cast<OOX::Logic::CEndnoteReference*>(item);
				int nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::endnoteReference);
				WriteNoteRef(pEndnoteReference->m_oCustomMarkFollows, pEndnoteReference->m_oId);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		default:
			break;
		}
	}
}
void BinaryDocumentTableWriter::WriteNoteRef(const nullable<SimpleTypes::COnOff>& oCustomMarkFollows, const nullable<SimpleTypes::CDecimalNumber>& oId)
{
	int nCurPos = 0;
	if (oCustomMarkFollows.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::RefCustomMarkFollows);
		m_oBcw.m_oStream.WriteBOOL(oCustomMarkFollows->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::RefId);
		m_oBcw.m_oStream.WriteLONG(oId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteText(const std::wstring& text, BYTE type)
{
	if(!text.empty())
	{
		m_oBcw.m_oStream.WriteBYTE(type);
		m_oBcw.m_oStream.WriteStringW(text);
		if(NULL != m_oBcw.m_pEmbeddedFontsManager)
			m_oBcw.m_pEmbeddedFontsManager->CheckString(text);
	}
}
bool BinaryDocumentTableWriter::WriteDrawingPptx(OOX::WritingElement* item)
{
	if (item == NULL) return false;

	OOX::EElementType		pElementType	= item->getType();
	std::wstring*			pXml			= NULL;

	OOX::Logic::CDrawing*		pGraphicDrawing	= NULL;
	PPTX::Logic::GraphicFrame*	pGraphic		= NULL;

	m_oBcw.m_oStream.ClearCurShapePositionAndSizes();

	bool res = true;

	if(OOX::et_mc_alternateContent == pElementType)
	{
		OOX::WritingElement* we = NULL;
		OOX::Logic::CAlternateContent* pAlternateContent = static_cast<OOX::Logic::CAlternateContent*>(item);

		if (pAlternateContent)
		{
			if (false == pAlternateContent->m_arrChoiceItems.empty())
			{
				we = pAlternateContent->m_arrChoiceItems[0];
			}
			res = WriteDrawingPptx(we);

			if (res == false || we == NULL)
			{
				if (false == pAlternateContent->m_arrFallbackItems.empty())
				{
					we = pAlternateContent->m_arrFallbackItems[0];
					res = WriteDrawingPptx(we);
				}
			}
		}

	}
	else if(OOX::et_w_drawing == pElementType)
	{
		OOX::Logic::CDrawing* pDrawing = static_cast<OOX::Logic::CDrawing*>(item);
		if(pDrawing->m_bAnchor && pDrawing->m_oAnchor.IsInit())
		{
			pGraphicDrawing = pDrawing;
			pGraphic = &pDrawing->m_oAnchor->m_oGraphic;
		}
		else if(pDrawing->m_oInline.IsInit())
		{
			pGraphicDrawing = pDrawing;
			pGraphic = &pDrawing->m_oInline->m_oGraphic;
		}
	}
	else if(OOX::et_w_pict == pElementType)
	{
		OOX::Logic::CPicture* pPicture = static_cast<OOX::Logic::CPicture*>(item);
		pXml = pPicture->m_sXml.GetPointer();
	}
	if (pXml)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::pptxDrawing);
		WriteDrawing(pXml, NULL, NULL);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	else if (pGraphicDrawing || pGraphic)
	{
		if ((pGraphic) && (pGraphic->contentPart.IsInit()))
		{
			//todooo разобрать по типам вставок
			res = false;
		}
		else
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSerRunType::pptxDrawing);
			WriteDrawing(NULL, pGraphicDrawing, pGraphic);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	else
	{
		res = false;
	}
	return res;
}
void BinaryDocumentTableWriter::WriteDrawing(std::wstring* pXml, OOX::Logic::CDrawing* pDrawing, PPTX::Logic::GraphicFrame *pGraphic)
{
	if (!pGraphic && !pXml) return;

	int nCurPos = 0;
	bool bDeleteDrawing = false;
//pptxdata
	if (pXml)
	{
		std::wstring* bstrOutputXml = NULL;
		m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::PptxData);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

		nCurPos = m_oBcw.WriteItemWithLengthStart();
			HRESULT hRes = m_pOfficeDrawingConverter->AddObject(*pXml, &bstrOutputXml);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);

		if(S_OK == hRes && NULL != bstrOutputXml)
		{
			std::wstring sBegin(_T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\"><w:drawing>"));

			std::wstring sEnd(_T("</w:drawing></root>"));
			std::wstring sDrawingXml = sBegin +  *bstrOutputXml + sEnd;

			XmlUtils::CXmlLiteReader oReader;
			if (oReader.FromString(sDrawingXml))
			{
				oReader.ReadNextNode();//root
				oReader.ReadNextNode();//drawing
				pDrawing = new OOX::Logic::CDrawing();
				pDrawing->fromXML(oReader);
			}
			bDeleteDrawing = true;
		}
		RELEASEOBJECT(bstrOutputXml);
	}
	else if (pGraphic)
	{
		if (pGraphic->chartRec.IsInit() && pGraphic->chartRec->id_data.IsInit() )
		{
			m_oBcw.m_oStream.WriteBYTE(pGraphic->chartRec->m_bChartEx ? c_oSerImageType2::ChartEx : c_oSerImageType2::Chart);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

			int nCurPos = m_oBcw.WriteItemWithLengthStart();

			pGraphic->chartRec->toPPTY(&m_oBcw.m_oStream);

			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::PptxData);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

			int nCurPos = m_oBcw.WriteItemWithLengthStart();

			m_oBcw.m_oStream.StartRecord(0);
			m_oBcw.m_oStream.StartRecord(1);

			if (pGraphic->olePic.is_init())
			{
				pGraphic->olePic->toPPTY(&m_oBcw.m_oStream);
			}
			//else if (pGraphic->smartArt.is_init())
			//{
			//	pGraphic->smartArt->LoadDrawing(&m_oBcw.m_oStream);
			//	pGraphic->smartArt->toPPTY(&m_oBcw.m_oStream);
			//}
			else if (pGraphic->element.is_init())
			{
				pGraphic->element.toPPTY(&m_oBcw.m_oStream);
			}
			else
			{
				pGraphic->toPPTY(&m_oBcw.m_oStream);
			}
			m_oBcw.m_oStream.EndRecord();
			m_oBcw.m_oStream.EndRecord();

			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
//inline or anchor props
	if(NULL != pDrawing)
	{
		OOX::Logic::CDrawing& img = *pDrawing;
		if(img.m_oInline.IsInit())
		{
			const OOX::Drawing::CInline& pInline = img.m_oInline.get();
			if(pInline.m_oExtent.IsInit())
			{
				//Type
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::Type);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBYTE(c_oAscWrapStyle::Inline);
				//Extent
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::Extent);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteExtent(pInline.m_oExtent.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
				//EffectExtent
				if(pInline.m_oEffectExtent.IsInit())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::EffectExtent);
					m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
					nCurPos = m_oBcw.WriteItemWithLengthStart();
					WriteEffectExtent(pInline.m_oEffectExtent.get());
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
				if (pInline.m_oGraphic.nvGraphicFramePr.IsInit())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::GraphicFramePr);
					m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
					nCurPos = m_oBcw.WriteItemWithLengthStart();
					WriteNvGraphicFramePr(pInline.m_oGraphic.nvGraphicFramePr.get());
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
				if(pInline.m_oDocPr.IsInit())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::DocPr);
					m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
					nCurPos = m_oBcw.WriteItemWithLengthStart();
					WriteDocPr(pInline.m_oDocPr.get());
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
			}
		}
		else if(img.m_oAnchor.IsInit() )
		{
			const OOX::Drawing::CAnchor& pAnchor = img.m_oAnchor.get();
			m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::Type);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(c_oAscWrapStyle::Flow);
			if(pAnchor.m_oAllowOverlap.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::AllowOverlap);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(pAnchor.m_oAllowOverlap->ToBool());
			}
			if(pAnchor.m_oBehindDoc.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::BehindDoc);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(pAnchor.m_oBehindDoc->ToBool());
			}
			if(pAnchor.m_oDistL.IsInit())
			{
				//document is corrupted if the value is greater UINT32
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::DistLEmu);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteULONG((_UINT32)pAnchor.m_oDistL->GetValue());
			}
			if(pAnchor.m_oDistT.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::DistTEmu);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteULONG((_UINT32)pAnchor.m_oDistT->GetValue());
			}
			if(pAnchor.m_oDistR.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::DistREmu);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteULONG((_UINT32)pAnchor.m_oDistR->GetValue());
			}
			if(pAnchor.m_oDistB.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::DistBEmu);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteULONG((_UINT32)pAnchor.m_oDistB->GetValue());
			}
			if(pAnchor.m_oHidden.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::Hidden);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(pAnchor.m_oHidden->ToBool());
			}
			if(pAnchor.m_oLayoutInCell.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::LayoutInCell);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(pAnchor.m_oLayoutInCell->ToBool());
			}
			if(pAnchor.m_oLocked.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::Locked);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(pAnchor.m_oLocked->ToBool());
			}
			if(pAnchor.m_oRelativeHeight.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::RelativeHeight);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteULONG(pAnchor.m_oRelativeHeight->GetValue());
			}
			if(pAnchor.m_bSimplePos.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::BSimplePos);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(pAnchor.m_bSimplePos->ToBool());
			}
			if(pAnchor.m_oEffectExtent.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::EffectExtent);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteEffectExtent(pAnchor.m_oEffectExtent.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oExtent.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::Extent);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteExtent(pAnchor.m_oExtent.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oPositionH.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::PositionH);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WritePositionH(pAnchor.m_oPositionH.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oPositionV.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::PositionV);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WritePositionV(pAnchor.m_oPositionV.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oSimplePos.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::SimplePos);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteSimplePos(pAnchor.m_oSimplePos.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oSizeRelH.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::SizeRelH);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteSizeRelH(pAnchor.m_oSizeRelH.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oSizeRelV.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::SizeRelV);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteSizeRelV(pAnchor.m_oSizeRelV.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oWrapNone.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::WrapNone);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Null);
			}
			if(pAnchor.m_oWrapSquare.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::WrapSquare);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteWrapSquare(pAnchor.m_oWrapSquare.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oWrapThrough.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::WrapThrough);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteWrapThrough(pAnchor.m_oWrapThrough.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oWrapTight.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::WrapTight);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteWrapTight(pAnchor.m_oWrapTight.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oWrapTopAndBottom.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::WrapTopAndBottom);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteWrapTopBottom(pAnchor.m_oWrapTopAndBottom.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if (pAnchor.m_oGraphic.nvGraphicFramePr.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::GraphicFramePr);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteNvGraphicFramePr(pAnchor.m_oGraphic.nvGraphicFramePr.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			if(pAnchor.m_oDocPr.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerImageType2::DocPr);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteDocPr(pAnchor.m_oDocPr.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
	}
	if(bDeleteDrawing)
		RELEASEOBJECT(pDrawing);
}
void BinaryDocumentTableWriter::WriteNvGraphicFramePr(const PPTX::Logic::NvGraphicFramePr& oGraphicFramePr)
{
	const PPTX::Logic::CNvGraphicFramePr& oLocks = oGraphicFramePr.cNvGraphicFramePr;
	if(oLocks.noChangeAspect.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerGraphicFramePr::NoChangeAspect);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oLocks.noChangeAspect.get());
	}
	if(oLocks.noDrilldown.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerGraphicFramePr::NoDrilldown);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oLocks.noDrilldown.get());
	}
	if(oLocks.noGrp.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerGraphicFramePr::NoGrp);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oLocks.noGrp.get());
	}
	if(oLocks.noMove.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerGraphicFramePr::NoMove);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oLocks.noMove.get());
	}
	if(oLocks.noResize.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerGraphicFramePr::NoResize);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oLocks.noResize.get());
	}
	if(oLocks.noSelect.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerGraphicFramePr::NoSelect);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oLocks.noSelect.get());
	}
}
void BinaryDocumentTableWriter::WriteDocPr(const PPTX::Logic::CNvPr& oDocPr)
{
	int nCurPos;
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocPr::Id);
		m_oBcw.m_oStream.WriteLONG(oDocPr.id);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocPr::Name);
		m_oBcw.m_oStream.WriteStringW3(oDocPr.name);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oDocPr.hidden.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocPr::Hidden);
		m_oBcw.m_oStream.WriteBOOL(oDocPr.hidden.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oDocPr.title.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocPr::Title);
		m_oBcw.m_oStream.WriteStringW3(oDocPr.title.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oDocPr.descr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocPr::Descr);
		m_oBcw.m_oStream.WriteStringW3(oDocPr.descr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oDocPr.form.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocPr::Form);
		m_oBcw.m_oStream.WriteBOOL(oDocPr.form.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteEffectExtent(const OOX::Drawing::CEffectExtent& oEffectExtent)
{
	int nCurPos = 0;
	//document is corrupted if the value is greater INT32
	m_oBcw.m_oStream.WriteBYTE(c_oSerEffectExtent::LeftEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oEffectExtent.m_oL.ToEmu());

	m_oBcw.m_oStream.WriteBYTE(c_oSerEffectExtent::TopEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oEffectExtent.m_oT.ToEmu());

	m_oBcw.m_oStream.WriteBYTE(c_oSerEffectExtent::RightEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oEffectExtent.m_oR.ToEmu());

	m_oBcw.m_oStream.WriteBYTE(c_oSerEffectExtent::BottomEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oEffectExtent.m_oB.ToEmu());
}
void BinaryDocumentTableWriter::WriteExtent(const ComplexTypes::Drawing::CPositiveSize2D& oExtent)
{
	int nCurPos = 0;
	//document is corrupted if the value is greater UINT32
	m_oBcw.m_oStream.WriteBYTE(c_oSerExtent::CxEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteULONG((_UINT32)oExtent.m_oCx.GetValue());

	m_oBcw.m_oStream.WriteBYTE(c_oSerExtent::CyEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteULONG((_UINT32)oExtent.m_oCy.GetValue());
}
void BinaryDocumentTableWriter::WritePositionH(const OOX::Drawing::CPosH& oPosH)
{
	int nCurPos = 0;
	if(oPosH.m_oRelativeFrom.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::RelativeFrom);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oPosH.m_oRelativeFrom->GetValue());
	}
	if(oPosH.m_oAlign.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::Align);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oPosH.m_oAlign->GetValue());
	}
	if(oPosH.m_oPosOffset.IsInit())
	{
		//document is corrupted if the value is greater INT32
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::PosOffsetEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG((_INT32)oPosH.m_oPosOffset->GetValue());
	}
	if(oPosH.m_oPctOffset.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::PctOffset);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDouble(oPosH.m_oPctOffset->GetValue());
	}
}
void BinaryDocumentTableWriter::WritePositionV(const OOX::Drawing::CPosV& oPosV)
{
	int nCurPos = 0;
	if(oPosV.m_oRelativeFrom.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::RelativeFrom);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oPosV.m_oRelativeFrom->GetValue());
	}
	if(oPosV.m_oAlign.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::Align);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oPosV.m_oAlign->GetValue());
	}
	if(oPosV.m_oPosOffset.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::PosOffsetEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG((_INT32)oPosV.m_oPosOffset->GetValue());
	}
	if(oPosV.m_oPctOffset.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerPosHV::PctOffset);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDouble(oPosV.m_oPctOffset->GetValue());
	}
}
void BinaryDocumentTableWriter::WriteSizeRelH(const OOX::Drawing::CSizeRelH& oSizeRelH)
{
	if(oSizeRelH.m_oRelativeFrom.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSizeRelHV::RelativeFrom);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oSizeRelH.m_oRelativeFrom->GetValue());
	}
	if(oSizeRelH.m_oPctWidth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSizeRelHV::Pct);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDouble(oSizeRelH.m_oPctWidth->GetValue());
	}
}
void BinaryDocumentTableWriter::WriteSizeRelV(const OOX::Drawing::CSizeRelV& oSizeRelV)
{
	if(oSizeRelV.m_oRelativeFrom.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSizeRelHV::RelativeFrom);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oSizeRelV.m_oRelativeFrom->GetValue());
	}
	if(oSizeRelV.m_oPctHeight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSizeRelHV::Pct);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDouble(oSizeRelV.m_oPctHeight->GetValue());
	}
}
void BinaryDocumentTableWriter::WriteSimplePos(const ComplexTypes::Drawing::CPoint2D& oSimplePos)
{
	int nCurPos = 0;
	//document is corrupted if the value is greater INT32
	m_oBcw.m_oStream.WriteBYTE(c_oSerSimplePos::XEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oSimplePos.m_oX.GetValue());

	m_oBcw.m_oStream.WriteBYTE(c_oSerSimplePos::YEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oSimplePos.m_oY.GetValue());
}
void BinaryDocumentTableWriter::WriteWrapSquare(const OOX::Drawing::CWrapSquare& oWrapSquare)
{
	int nCurPos = 0;
	if(oWrapSquare.m_oDistL.IsInit())
	{
		//document is corrupted if the value is greater UINT32
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapSquare::DistLEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapSquare.m_oDistL->GetValue());
	}
	if(oWrapSquare.m_oDistT.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapSquare::DistTEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapSquare.m_oDistT->GetValue());
	}
	if(oWrapSquare.m_oDistR.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapSquare::DistREmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapSquare.m_oDistR->GetValue());
	}
	if(oWrapSquare.m_oDistB.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapSquare::DistBEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapSquare.m_oDistB->GetValue());
	}
	if(oWrapSquare.m_oWrapText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapSquare::WrapText);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oWrapSquare.m_oWrapText->GetValue());
	}
	if(oWrapSquare.m_oEffectExtent.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapSquare::EffectExtent);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

		nCurPos = m_oBcw.WriteItemWithLengthStart();
			WriteEffectExtent(oWrapSquare.m_oEffectExtent.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteWrapThrough(const OOX::Drawing::CWrapThrough& oWrapThrough)
{
	int nCurPos = 0;
	if(oWrapThrough.m_oDistL.IsInit())
	{
		//document is corrupted if the value is greater UINT32
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::DistLEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapThrough.m_oDistL->GetValue());
	}
	if(oWrapThrough.m_oDistR.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::DistREmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapThrough.m_oDistR->GetValue());
	}
	if(oWrapThrough.m_oWrapText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::WrapText);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oWrapThrough.m_oWrapText->GetValue());
	}
	if(oWrapThrough.m_oWrapPolygon.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::WrapPolygon);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

		nCurPos = m_oBcw.WriteItemWithLengthStart();
			WriteWrapPolygon(oWrapThrough.m_oWrapPolygon.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteWrapTight(const OOX::Drawing::CWrapTight& oWrapTight)
{
	int nCurPos = 0;
	if(oWrapTight.m_oDistL.IsInit())
	{
		//document is corrupted if the value is greater UINT32
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::DistLEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapTight.m_oDistL->GetValue());
	}
	if(oWrapTight.m_oDistR.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::DistREmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapTight.m_oDistR->GetValue());
	}
	if(oWrapTight.m_oWrapText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::WrapText);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oWrapTight.m_oWrapText->GetValue());
	}
	if(oWrapTight.m_oWrapPolygon.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapThroughTight::WrapPolygon);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

		nCurPos = m_oBcw.WriteItemWithLengthStart();
			WriteWrapPolygon(oWrapTight.m_oWrapPolygon.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteWrapTopBottom(const OOX::Drawing::CWrapTopBottom& oWrapTopBottom)
{
	int nCurPos = 0;
	if(oWrapTopBottom.m_oDistT.IsInit())
	{
		//document is corrupted if the value is greater UINT32
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapTopBottom::DistTEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapTopBottom.m_oDistT->GetValue());
	}
	if(oWrapTopBottom.m_oDistB.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapTopBottom::DistBEmu);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG((_UINT32)oWrapTopBottom.m_oDistB->GetValue());
	}
	if(oWrapTopBottom.m_oEffectExtent.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapTopBottom::EffectExtent);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteEffectExtent(oWrapTopBottom.m_oEffectExtent.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteWrapPolygon(const OOX::Drawing::CWrapPath& oWrapPath)
{
	int nCurPos = 0;
	if(oWrapPath.m_oEdited.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapPolygon::Edited);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oWrapPath.m_oEdited->ToBool());
	}
	if(oWrapPath.m_oStart.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapPolygon::Start);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WritePoint2D(oWrapPath.m_oStart.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oWrapPath.m_arrLineTo.size() > 0)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapPolygon::ALineTo);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

		nCurPos = m_oBcw.WriteItemWithLengthStart();
			WriteLineTo(oWrapPath.m_arrLineTo);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteLineTo(const std::vector<ComplexTypes::Drawing::CPoint2D*>& aLineTo)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aLineTo.size(); i < length; ++i)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWrapPolygon::LineTo);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WritePoint2D(*aLineTo[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WritePoint2D(const ComplexTypes::Drawing::CPoint2D& oPoint2D)
{
	//document is corrupted if the value is greater INT32
	int nCurPos = 0;
	m_oBcw.m_oStream.WriteBYTE(c_oSerPoint2D::XEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oPoint2D.m_oX.ToEmu());

	m_oBcw.m_oStream.WriteBYTE(c_oSerPoint2D::YEmu);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
	m_oBcw.m_oStream.WriteLONG((_INT32)oPoint2D.m_oY.ToEmu());
}
void BinaryDocumentTableWriter::WriteDocTable(OOX::Logic::CTbl* tbl)
{
	if (!tbl) return;

	int nCurPos = 0;
	//ищем tblPr
	//tblPr должна идти раньше Content
	int nRows = 0;
	int nCols = 0;
	OOX::Logic::CTableProperty* pTblPr = NULL;
//считаем количество строк и столбцов
	GetTableSize(tbl->m_arrItems, nRows, nCols, &pTblPr);
	if(nRows > 0 && nCols > 0)
	{
//TblPr
		if(NULL != pTblPr)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::tblPr);
			btblPrs.WriteTblPr(pTblPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}

//tblGrid
		if(tbl->m_oTblGrid.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::tblGrid);
			WriteTblGrid(tbl->m_oTblGrid.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
//Content
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Content);
			WriteTableContent(tbl->m_arrItems, pTblPr, nRows, nCols);
		m_oBcw.WriteItemEnd(nCurPos);
	}

	RELEASEOBJECT(pTblPr);
}
bool BinaryDocumentTableWriter::ValidateRow(const std::vector<OOX::WritingElement *> & arrItems)
{
//Проверяем чтобы не все ячейки в ряду были вертикально замержены
	bool bRes = true;

	for(size_t i = 0; i < arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		if(OOX::et_w_tc == item->getType())
		{
			OOX::Logic::CTc* tc = static_cast<OOX::Logic::CTc*>(item);
			OOX::Logic::CTableCellProperties* pTcPr = tc->m_pTableCellProperties;
			bool bVMerge = false;
			if(NULL != pTcPr)
			{
				if(pTcPr->m_oVMerge.IsInit() && pTcPr->m_oVMerge->m_oVal.IsInit() && SimpleTypes::mergeContinue == pTcPr->m_oVMerge->m_oVal->GetValue())
					bVMerge = true;
			}
			if(false == bVMerge)
				return true;
		}
		else if(OOX::et_w_sdt == item->getType())
		{
			OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
			if(pStd->m_oSdtContent.IsInit())
				if(true == ValidateRow(pStd->m_oSdtContent.get().m_arrItems))
					return true;
		}
		else if(OOX::et_w_smartTag == item->getType())
		{
			OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
			if(true == ValidateRow(pSmartTag->m_arrItems))
				return true;
		}
		else if(OOX::et_w_dir == item->getType())
		{
			OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
			if(true == ValidateRow(pDir->m_arrItems))
				return true;
		}
		else if(OOX::et_w_bdo == item->getType())
		{
			OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
			if(true == ValidateRow(pBdo->m_arrItems))
				return true;
		}
	}
	return false;
}
void BinaryDocumentTableWriter::GetTableSize(std::vector<OOX::WritingElement *> & rows, int& nRows, int& nCols, OOX::Logic::CTableProperty** ppTblPr)
{
	for(size_t i = 0; i < rows.size(); ++i)
	{
		OOX::WritingElement* item = rows[i];

		if(OOX::et_w_tblPr == item->getType())
		{
			*ppTblPr = new OOX::Logic::CTableProperty();
			**ppTblPr = *static_cast<OOX::Logic::CTableProperty*>(item);
		}
		else if(OOX::et_w_tr == item->getType())
		{
			OOX::Logic::CTr* pTr = static_cast<OOX::Logic::CTr*>(item);
			if(ValidateRow(pTr->m_arrItems))
			{
				nRows++;
				if(0 == nCols)
				{
					OOX::Logic::CTr* pTr = static_cast<OOX::Logic::CTr*>(item);
					nCols = GetColsCount(pTr->m_arrItems);
				}
			}
			else
			{
				rows.erase(rows.begin() + i);
			}
		}
		else if(OOX::et_w_sdt == item->getType())
		{
			OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
			if(pStd->m_oSdtContent.IsInit())
				GetTableSize(pStd->m_oSdtContent->m_arrItems, nRows, nCols, ppTblPr);
		}
		else if(OOX::et_w_smartTag == item->getType())
		{
			OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
			GetTableSize(pSmartTag->m_arrItems, nRows, nCols, ppTblPr);
		}
		else if(OOX::et_w_dir == item->getType())
		{
			OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
			GetTableSize(pDir->m_arrItems, nRows, nCols, ppTblPr);
		}
		else if(OOX::et_w_bdo == item->getType())
		{
			OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
			GetTableSize(pBdo->m_arrItems, nRows, nCols, ppTblPr);
		}
	}
}
int BinaryDocumentTableWriter::GetColsCount(const std::vector<OOX::WritingElement *>& arrItems)
{
	int nColCount = 0;

	for(size_t i = 0; i <  arrItems.size(); ++i)
	{
		OOX::WritingElement* item = arrItems[i];

		if(OOX::et_w_tc == item->getType())
		{
			nColCount++;
			OOX::Logic::CTc* tc = static_cast<OOX::Logic::CTc*>(item);

			for(std::vector<OOX::WritingElement *>::iterator jt = tc->m_arrItems.begin(); jt != tc->m_arrItems.end(); jt++)
			{
				OOX::WritingElement* item2 = *jt;

				if(OOX::et_w_tcPr == item2->getType())
				{
					OOX::Logic::CTableCellProperties* tcPr = static_cast<OOX::Logic::CTableCellProperties*>(item2);
					if(tcPr->m_oGridSpan.IsInit() && tcPr->m_oGridSpan->m_oVal.IsInit())
					{
						int nGridSpan = *tcPr->m_oGridSpan->m_oVal;
						if(nGridSpan > 0)
							nColCount += nGridSpan;
					}
				}
			}
		}
		else if(OOX::et_w_sdt == item->getType())
		{
			OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
			if(pStd->m_oSdtContent.IsInit())
				nColCount += GetColsCount(pStd->m_oSdtContent.get().m_arrItems);
		}
		else if(OOX::et_w_smartTag == item->getType())
		{
			OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
			nColCount += GetColsCount(pSmartTag->m_arrItems);
		}
		else if(OOX::et_w_dir == item->getType())
		{
			OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
			nColCount += GetColsCount(pDir->m_arrItems);
		}
		else if(OOX::et_w_bdo == item->getType())
		{
			OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
			nColCount += GetColsCount(pBdo->m_arrItems);
		}
	}
	return nColCount;
}
void BinaryDocumentTableWriter::WriteTblGrid(const OOX::Logic::CTblGrid& grid)
{
	int nCurPos = 0;
	for(size_t i = 0, length = grid.m_arrGridCol.size(); i < length; i++)
	{
		const ComplexTypes::Word::CTblGridCol& item = *grid.m_arrGridCol[i];
		if(item.m_oW.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerDocTableType::tblGrid_ItemTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(item.m_oW->ToTwips());
		}
	}
	if(grid.m_oTblGridChange.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerDocTableType::tblGridChange);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);

		nCurPos = m_oBcw.WriteItemWithLengthStart();
			WriteTblGridChange(grid.m_oTblGridChange.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteTblGridChange(const OOX::Logic::CTblGridChange& tblGridChange)
{
	int nCurPos = 0;
	if(tblGridChange.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::Id);
			m_oBcw.m_oStream.WriteLONG(tblGridChange.m_oId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(NULL != tblGridChange.m_pTblGrid)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerProp_RevisionType::tblGridChange);
			WriteTblGrid(*tblGridChange.m_pTblGrid);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}

void BinaryDocumentTableWriter::WriteTableContent(std::vector<OOX::WritingElement *>& content, OOX::Logic::CTableProperty* pTblPr, int nRows, int nCols)
{
	int nCurPos			= 0;
	int nCurRowIndex	= 0;

	for(size_t i = 0; i < content.size(); ++i)
	{
		OOX::WritingElement* item = content[i];

		if(OOX::et_w_tr == item->getType())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Row);
			WriteRow(*static_cast<OOX::Logic::CTr*>(item), pTblPr, nCurRowIndex, nRows, nCols);
			m_oBcw.WriteItemEnd(nCurPos);
			nCurRowIndex++;
		}
		else if(OOX::et_w_sdt == item->getType())
		{
			OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Sdt);
			WriteSdt(pStd, 2, pTblPr, 0, nRows, nCols);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else if(OOX::et_w_smartTag == item->getType())
		{
			OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
			WriteTableContent(pSmartTag->m_arrItems, pTblPr, nRows, nCols);
		}
		else if(OOX::et_w_dir == item->getType())
		{
			OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
			WriteTableContent(pDir->m_arrItems, pTblPr, nRows, nCols);
		}
		else if(OOX::et_w_bdo == item->getType())
		{
			OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
			WriteTableContent(pBdo->m_arrItems, pTblPr, nRows, nCols);
		}
		else if(OOX::et_w_bookmarkStart == item->getType())
		{
			OOX::Logic::CBookmarkStart* pBookmarkStart = static_cast<OOX::Logic::CBookmarkStart*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::BookmarkStart);
			WriteBookmarkStart(*pBookmarkStart);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else if(OOX::et_w_bookmarkEnd == item->getType())
		{
			OOX::Logic::CBookmarkEnd* pBookmarkEnd = static_cast<OOX::Logic::CBookmarkEnd*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::BookmarkEnd);
			WriteBookmarkEnd(*pBookmarkEnd);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else if(OOX::et_w_moveFromRangeStart == item->getType())
		{
			OOX::Logic::CMoveFromRangeStart* pMoveFromRangeStart = static_cast<OOX::Logic::CMoveFromRangeStart*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveFromRangeStart);
			WriteMoveRangeStart(*pMoveFromRangeStart);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if(OOX::et_w_moveFromRangeEnd == item->getType())
		{
			OOX::Logic::CMoveFromRangeEnd* pMoveFromRangeEnd = static_cast<OOX::Logic::CMoveFromRangeEnd*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveFromRangeEnd);
			WriteMoveRangeEnd(*pMoveFromRangeEnd);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if(OOX::et_w_moveToRangeStart == item->getType())
		{
			OOX::Logic::CMoveToRangeStart* pMoveToRangeStart = static_cast<OOX::Logic::CMoveToRangeStart*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveToRangeStart);
			WriteMoveRangeStart(*pMoveToRangeStart);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if(OOX::et_w_moveToRangeEnd == item->getType())
		{
			OOX::Logic::CMoveToRangeEnd* pMoveToRangeEnd = static_cast<OOX::Logic::CMoveToRangeEnd*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveToRangeEnd);
			WriteMoveRangeEnd(*pMoveToRangeEnd);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryDocumentTableWriter::WriteRow(const OOX::Logic::CTr& Row, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex, int nRows, int nCols)
{
	int nCurPos = 0;

	if(NULL != Row.m_pTableRowProperties)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Row_Pr);
			btblPrs.WriteRowPr(*Row.m_pTableRowProperties);
		m_oBcw.WriteItemEnd(nCurPos);
	}

	//Content
	nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Row_Content);
	WriteRowContent(Row.m_arrItems, pTblPr, nCurRowIndex, nRows, nCols);
	m_oBcw.WriteItemEnd(nCurPos);
}

void BinaryDocumentTableWriter::WriteRowContent(const std::vector<OOX::WritingElement *> & content, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex, int nRows, int nCols)
{
	int nCurPos = 0;
	int nCurColIndex = 0;

	for(size_t i = 0; i < content.size(); ++i)
	{
		OOX::WritingElement* item = content[i];

		if(OOX::et_w_tc == item->getType())
		{
			OOX::Logic::CTc* tc = static_cast<OOX::Logic::CTc*>(item);

			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Cell);
				WriteCell(*tc, pTblPr, nCurRowIndex, nCurColIndex, nRows, nCols);
			m_oBcw.WriteItemEnd(nCurPos);

			nCurColIndex++;
		}
		else if(OOX::et_w_sdt == item->getType())
		{
			OOX::Logic::CSdt* pStd = static_cast<OOX::Logic::CSdt*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Sdt);
			WriteSdt(pStd, 3, pTblPr, nCurRowIndex, nRows, nCols);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else if(OOX::et_w_smartTag == item->getType())
		{
			OOX::Logic::CSmartTag* pSmartTag = static_cast<OOX::Logic::CSmartTag*>(item);
			WriteRowContent(pSmartTag->m_arrItems, pTblPr, nCurRowIndex, nRows, nCols);
		}
		else if(OOX::et_w_dir == item->getType())
		{
			OOX::Logic::CDir* pDir = static_cast<OOX::Logic::CDir*>(item);
			WriteRowContent(pDir->m_arrItems, pTblPr, nCurRowIndex, nRows, nCols);
		}
		else if(OOX::et_w_bdo == item->getType())
		{
			OOX::Logic::CBdo* pBdo = static_cast<OOX::Logic::CBdo*>(item);
			WriteRowContent(pBdo->m_arrItems, pTblPr, nCurRowIndex, nRows, nCols);
		}
		else if(OOX::et_w_bookmarkStart == item->getType())
		{
			OOX::Logic::CBookmarkStart* pBookmarkStart = static_cast<OOX::Logic::CBookmarkStart*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::BookmarkStart);
			WriteBookmarkStart(*pBookmarkStart);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else if(OOX::et_w_bookmarkEnd == item->getType())
		{
			OOX::Logic::CBookmarkEnd* pBookmarkEnd = static_cast<OOX::Logic::CBookmarkEnd*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::BookmarkEnd);
			WriteBookmarkEnd(*pBookmarkEnd);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else if(OOX::et_w_moveFromRangeStart == item->getType())
		{
			OOX::Logic::CMoveFromRangeStart* pMoveFromRangeStart = static_cast<OOX::Logic::CMoveFromRangeStart*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveFromRangeStart);
			WriteMoveRangeStart(*pMoveFromRangeStart);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if(OOX::et_w_moveFromRangeEnd == item->getType())
		{
			OOX::Logic::CMoveFromRangeEnd* pMoveFromRangeEnd = static_cast<OOX::Logic::CMoveFromRangeEnd*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveFromRangeEnd);
			WriteMoveRangeEnd(*pMoveFromRangeEnd);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if(OOX::et_w_moveToRangeStart == item->getType())
		{
			OOX::Logic::CMoveToRangeStart* pMoveToRangeStart = static_cast<OOX::Logic::CMoveToRangeStart*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveToRangeStart);
			WriteMoveRangeStart(*pMoveToRangeStart);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if(OOX::et_w_moveToRangeEnd == item->getType())
		{
			OOX::Logic::CMoveToRangeEnd* pMoveToRangeEnd = static_cast<OOX::Logic::CMoveToRangeEnd*>(item);
			nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::MoveToRangeEnd);
			WriteMoveRangeEnd(*pMoveToRangeEnd);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryDocumentTableWriter::WriteCell(OOX::Logic::CTc& tc, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex, int nCurColIndex, int nRows, int nCols)
{
	int nCurPos = 0;
//св-ва ячейки
	if(NULL != tc.m_pTableCellProperties)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Cell_Pr);
			btblPrs.WriteCellPr(tc.m_pTableCellProperties);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Content
	BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, m_oParamsDocumentWriter, m_mapIgnoreComments, bpPrs.m_oBinaryHeaderFooterTableWriter);

	nCurPos = m_oBcw.WriteItemStart(c_oSerDocTableType::Cell_Content);
		oBinaryDocumentTableWriter.WriteDocumentContent(tc.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryDocumentTableWriter::WriteSdt(OOX::Logic::CSdt* pStd, int type, OOX::Logic::CTableProperty* pTblPr, int nCurRowIndex, int nRows, int nCols)
{
	if (!pStd) return;

	int nCurPos = 0;
	if(pStd->m_oSdtPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Pr);
		WriteSdtPr(pStd->m_oSdtPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pStd->m_oSdtEndPr.IsInit() && pStd->m_oSdtEndPr->m_oRPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::EndPr);
		brPrs.Write_rPr(pStd->m_oSdtEndPr->m_oRPr.GetPointer());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pStd->m_oSdtContent.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Content);
		switch(type)
		{
			case 0:
			{
				BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, m_oParamsDocumentWriter, m_mapIgnoreComments, bpPrs.m_oBinaryHeaderFooterTableWriter);
				oBinaryDocumentTableWriter.WriteDocumentContent(pStd->m_oSdtContent.get().m_arrItems);
				break;
			}
			case 1:
			{
				WriteParagraphContent(pStd->m_oSdtContent.get().m_arrItems);
				break;
			}
			case 2:
			{
				WriteTableContent(pStd->m_oSdtContent->m_arrItems, pTblPr, nRows, nCols);
				break;
			}
			case 3:
			{
				WriteRowContent(pStd->m_oSdtContent.get().m_arrItems, pTblPr, nCurRowIndex, nRows, nCols);
				break;
			}
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtPr(const OOX::Logic::CSdtPr& oStdPr)
{
	int nCurPos = 0;
	nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Type);
	m_oBcw.m_oStream.WriteBYTE(oStdPr.m_eType);
	m_oBcw.WriteItemEnd(nCurPos);

	if(oStdPr.m_oAlias.IsInit() && oStdPr.m_oAlias->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Alias);
		m_oBcw.m_oStream.WriteStringW3(oStdPr.m_oAlias->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oAppearance.IsInit() && oStdPr.m_oAppearance->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Appearance);
		m_oBcw.m_oStream.WriteBYTE(oStdPr.m_oAppearance->m_oVal->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oComboBox.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::ComboBox);
		WriteSdtComboBox(oStdPr.m_oComboBox.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Color);
		if(oStdPr.m_oColor->m_oVal.IsInit())
			m_oBcw.WriteColor(c_oSerProp_rPrType::Color, oStdPr.m_oColor->m_oVal.get());

		m_oBcw.WriteThemeColor(c_oSerProp_rPrType::ColorTheme, oStdPr.m_oColor->m_oVal, oStdPr.m_oColor->m_oThemeColor, oStdPr.m_oColor->m_oThemeTint, oStdPr.m_oColor->m_oThemeShade);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oDataBinding.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DataBinding);
		WriteSdtPrDataBinding(oStdPr.m_oDataBinding.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oDate.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PrDate);
		WriteSdtPrDate(oStdPr.m_oDate.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oDocPartList.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DocPartList);
		WriteDocPartList(oStdPr.m_oDocPartList.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oDocPartObj.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DocPartObj);
		WriteDocPartList(oStdPr.m_oDocPartObj.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oDropDownList.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DropDownList);
		WriteDropDownList(oStdPr.m_oDropDownList.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oId.IsInit() && oStdPr.m_oId->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Id);
		m_oBcw.m_oStream.WriteULONG(*oStdPr.m_oId->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oLabel.IsInit() && oStdPr.m_oLabel->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Label);
		m_oBcw.m_oStream.WriteULONG(*oStdPr.m_oLabel->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oLock.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Lock);
		m_oBcw.m_oStream.WriteBYTE(oStdPr.m_oLock->m_oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oPlaceHolder.IsInit() && oStdPr.m_oPlaceHolder->m_oDocPart.IsInit() && oStdPr.m_oPlaceHolder->m_oDocPart->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PlaceHolder);
		m_oBcw.m_oStream.WriteStringW3(oStdPr.m_oPlaceHolder->m_oDocPart->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oRPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::RPr);
		brPrs.Write_rPr(oStdPr.m_oRPr.GetPointer());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oShowingPlcHdr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::ShowingPlcHdr);
		m_oBcw.m_oStream.WriteBOOL(oStdPr.m_oShowingPlcHdr->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oTabIndex.IsInit() && oStdPr.m_oTabIndex->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TabIndex);
		m_oBcw.m_oStream.WriteULONG(oStdPr.m_oTabIndex->m_oVal->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oTag.IsInit() && oStdPr.m_oTag->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Tag);
		m_oBcw.m_oStream.WriteStringW3(oStdPr.m_oTag->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oTemporary.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Temporary);
		m_oBcw.m_oStream.WriteBOOL(oStdPr.m_oTemporary->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oText.IsInit() && oStdPr.m_oText->m_oMultiLine.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::MultiLine);
		m_oBcw.m_oStream.WriteBOOL(oStdPr.m_oText->m_oMultiLine->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oCheckbox.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Checkbox);
		WriteSdtCheckBox(oStdPr.m_oCheckbox.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oFormPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FormPr);
		WriteSdtFormPr(oStdPr.m_oFormPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oStdPr.m_oTextFormPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPr);
		WriteSdtTextFormPr(oStdPr.m_oTextFormPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oStdPr.m_oPicture.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PictureFormPr);
		WriteSdtPicture(oStdPr.m_oPicture.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oStdPr.m_oComplexFormPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::ComplexFormPr);
		WriteSdtComplexFormPr(oStdPr.m_oComplexFormPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtPicture(const OOX::Logic::CSdtPicture& oSdtPicture)
{
	int nCurPos = 0;
	if (oSdtPicture.m_oScaleFlag.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PictureFormPrScaleFlag);
		m_oBcw.m_oStream.WriteLONG(*oSdtPicture.m_oScaleFlag);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSdtPicture.m_oLockProportions.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PictureFormPrLockProportions);
		m_oBcw.m_oStream.WriteBOOL(*oSdtPicture.m_oLockProportions);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSdtPicture.m_oRespectBorders.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PictureFormPrRespectBorders);
		m_oBcw.m_oStream.WriteBOOL(*oSdtPicture.m_oRespectBorders);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSdtPicture.m_oShiftX.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PictureFormPrShiftX);
		m_oBcw.m_oStream.WriteDoubleReal(*oSdtPicture.m_oShiftX);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSdtPicture.m_oShiftY.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PictureFormPrShiftY);
		m_oBcw.m_oStream.WriteDoubleReal(*oSdtPicture.m_oShiftY);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtComplexFormPr(const OOX::Logic::CComplexFormPr& oComplexFormPr)
{
	int nCurPos = 0;
	if (oComplexFormPr.m_oType.IsInit() && !oComplexFormPr.m_oType->IsDefaultValue())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::ComplexFormPrType);
		m_oBcw.m_oStream.WriteLONG(oComplexFormPr.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtCheckBox(const OOX::Logic::CSdtCheckBox& oSdtCheckBox)
{
	int nCurPos = 0;
	if(oSdtCheckBox.m_oChecked.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::CheckboxChecked);
		m_oBcw.m_oStream.WriteBOOL(oSdtCheckBox.m_oChecked->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSdtCheckBox.m_oCheckedState.IsInit())
	{
		if(oSdtCheckBox.m_oCheckedState->m_oFont.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::CheckboxCheckedFont);
			m_oBcw.m_oStream.WriteStringW3(oSdtCheckBox.m_oCheckedState->m_oFont.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(oSdtCheckBox.m_oCheckedState->m_oVal.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::CheckboxCheckedVal);
			m_oBcw.m_oStream.WriteLONG(oSdtCheckBox.m_oCheckedState->m_oVal->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	if(oSdtCheckBox.m_oUncheckedState.IsInit())
	{
		if(oSdtCheckBox.m_oUncheckedState->m_oFont.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::CheckboxUncheckedFont);
			m_oBcw.m_oStream.WriteStringW3(oSdtCheckBox.m_oUncheckedState->m_oFont.get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(oSdtCheckBox.m_oUncheckedState->m_oVal.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::CheckboxUncheckedVal);
			m_oBcw.m_oStream.WriteLONG(oSdtCheckBox.m_oUncheckedState->m_oVal->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	if(oSdtCheckBox.m_oGroupKey.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::CheckboxGroupKey);
		m_oBcw.m_oStream.WriteStringW3(oSdtCheckBox.m_oGroupKey->ToString2());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtComboBox(const OOX::Logic::CSdtComboBox& oSdtComboBox)
{
	int nCurPos = 0;
	if(oSdtComboBox.m_sLastValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::LastValue);
		m_oBcw.m_oStream.WriteStringW3(oSdtComboBox.m_sLastValue.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSdtComboBox.m_oFormat.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrFormat);
		WriteSdtTextFormPrFormat(oSdtComboBox.m_oFormat.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	for(size_t i = 0; i < oSdtComboBox.m_arrListItem.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::SdtListItem);
		WriteSdtListItem(*oSdtComboBox.m_arrListItem[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtListItem(const ComplexTypes::Word::CSdtListItem& oSdtListItem)
{
	int nCurPos = 0;
	if(oSdtListItem.m_sDisplayText.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DisplayText);
		m_oBcw.m_oStream.WriteStringW3(oSdtListItem.m_sDisplayText.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSdtListItem.m_sValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Value);
		m_oBcw.m_oStream.WriteStringW3(oSdtListItem.m_sValue.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtPrDataBinding(const ComplexTypes::Word::CDataBinding& oDataBinding)
{
	int nCurPos = 0;
	if(oDataBinding.m_sPrefixMappings.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::PrefixMappings);
		m_oBcw.m_oStream.WriteStringW3(oDataBinding.m_sPrefixMappings.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDataBinding.m_sStoreItemID.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::StoreItemID);
		m_oBcw.m_oStream.WriteStringW3(oDataBinding.m_sStoreItemID.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDataBinding.m_sXPath.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::XPath);
		m_oBcw.m_oStream.WriteStringW3(oDataBinding.m_sXPath.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtPrDate(const OOX::Logic::CDate& oDate)
{
	int nCurPos = 0;
	if(oDate.m_oFullDate.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FullDate);
		m_oBcw.m_oStream.WriteStringW3(oDate.m_oFullDate->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDate.m_oCalendar.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Calendar);
		m_oBcw.m_oStream.WriteBYTE(oDate.m_oCalendar->m_oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDate.m_oDateFormat.IsInit() && oDate.m_oDateFormat->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DateFormat);
		m_oBcw.m_oStream.WriteStringW3(oDate.m_oDateFormat->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDate.m_oLid.IsInit() && oDate.m_oLid->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::Lid);
		m_oBcw.m_oStream.WriteStringW3(*oDate.m_oLid->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDate.m_oStoreMappedDateAs.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::StoreMappedDataAs);
		m_oBcw.m_oStream.WriteBYTE(oDate.m_oStoreMappedDateAs->m_oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteDocPartList(const OOX::Logic::CSdtDocPart& oSdtDocPart)
{
	int nCurPos = 0;
	if(oSdtDocPart.m_oDocPartCategory.IsInit() && oSdtDocPart.m_oDocPartCategory->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DocPartCategory);
		m_oBcw.m_oStream.WriteStringW3(oSdtDocPart.m_oDocPartCategory->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSdtDocPart.m_oDocPartGallery.IsInit() && oSdtDocPart.m_oDocPartGallery->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DocPartGallery);
		m_oBcw.m_oStream.WriteStringW3(oSdtDocPart.m_oDocPartGallery->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSdtDocPart.m_oDocPartUnique.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::DocPartUnique);
		m_oBcw.m_oStream.WriteBOOL(oSdtDocPart.m_oDocPartUnique->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteDropDownList(const OOX::Logic::CSdtDropDownList& oDropDownList)
{
	int nCurPos = 0;
	if(oDropDownList.m_sLastValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::LastValue);
		m_oBcw.m_oStream.WriteStringW3(oDropDownList.m_sLastValue.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	for(size_t i = 0; i < oDropDownList.m_arrListItem.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::SdtListItem);
		WriteSdtListItem(*oDropDownList.m_arrListItem[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtFormPr(const OOX::Logic::CFormPr& oFormPr)
{
	int nCurPos = 0;
	if(oFormPr.m_oKey.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FormPrKey);
		m_oBcw.m_oStream.WriteStringW3(oFormPr.m_oKey.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFormPr.m_oLabel.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FormPrLabel);
		m_oBcw.m_oStream.WriteStringW3(oFormPr.m_oLabel.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFormPr.m_oHelpText.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FormPrHelpText);
		m_oBcw.m_oStream.WriteStringW3(oFormPr.m_oHelpText.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFormPr.m_oRequired.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FormPrRequired);
		m_oBcw.m_oStream.WriteBOOL(oFormPr.m_oRequired.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oFormPr.m_oBorder.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FormPrBorder);
		m_oBcw.WriteBorder(oFormPr.m_oBorder.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oFormPr.m_oShd.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::FormPrShd);
		m_oBcw.WriteShd(oFormPr.m_oShd.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oFormPr.m_oFieldRid.IsInit())
	{
		smart_ptr<OOX::File> pFile = m_oParamsDocumentWriter.m_pRels->Find(oFormPr.m_oFieldRid->GetValue());
		if (pFile.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::OformMaster);
			m_oBcw.m_oStream.WriteStringW3(pFile->m_sOutputFilename);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryDocumentTableWriter::WriteSdtTextFormPr(const OOX::Logic::CTextFormPr& oTextFormPr)
{
	int nCurPos = 0;
	if(oTextFormPr.m_oComb.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrComb);
		WriteSdtTextFormPrComb(oTextFormPr.m_oComb.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTextFormPr.m_oMaxCharacters.IsInit() && oTextFormPr.m_oMaxCharacters->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrMaxCharacters);
		m_oBcw.m_oStream.WriteLONG(oTextFormPr.m_oMaxCharacters->m_oVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTextFormPr.m_oCombBorder.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrCombBorder);
		m_oBcw.WriteBorder(oTextFormPr.m_oCombBorder.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oTextFormPr.m_oAutoFit.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrAutoFit);
		m_oBcw.m_oStream.WriteBOOL(oTextFormPr.m_oAutoFit.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oTextFormPr.m_oMultiLine.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrMultiLine);
		m_oBcw.m_oStream.WriteBOOL(oTextFormPr.m_oMultiLine.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oTextFormPr.m_oFormat.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrFormat);
		WriteSdtTextFormPrFormat(oTextFormPr.m_oFormat.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}	
}
void BinaryDocumentTableWriter::WriteSdtTextFormPrFormat(const ComplexTypes::Word::CTextFormFormat& oFormat)
{
	int nCurPos = 0;
	if (oFormat.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrFormatType);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oFormat.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oFormat.m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrFormatVal);
		m_oBcw.m_oStream.WriteStringW3(oFormat.m_oVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oFormat.m_oSymbols.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrFormatSymbols);
		m_oBcw.m_oStream.WriteStringW3(oFormat.m_oSymbols.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryDocumentTableWriter::WriteSdtTextFormPrComb(const ComplexTypes::Word::CComb& oComb)
{
	int nCurPos = 0;
	if(oComb.m_oWidth.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrCombWidth);
		m_oBcw.m_oStream.WriteLONG(oComb.m_oWidth.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oComb.m_oSym.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrCombSym);
		m_oBcw.m_oStream.WriteStringW3(oComb.m_oSym.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oComb.m_oFont.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrCombFont);
		m_oBcw.m_oStream.WriteStringW3(oComb.m_oFont.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oComb.m_oWRule.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSdt::TextFormPrCombWRule);
		m_oBcw.m_oStream.WriteBYTE(oComb.m_oWRule->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}


BinaryCommentsTableWriter::BinaryCommentsTableWriter(ParamsWriter& oParamsWriter) : 
	m_oBcw(oParamsWriter), m_oParamsWriter(oParamsWriter), m_pOfficeDrawingConverter(oParamsWriter.m_pOfficeDrawingConverter)
{
};
void BinaryCommentsTableWriter::Write(OOX::CComments& oComments, OOX::CCommentsExt* pCommentsExt, OOX::CCommentsExtensible* pCommentsExtensible, OOX::CCommentsUserData* pCommentsUserData, OOX::CPeople* pPeople, OOX::CCommentsIds* pCommentsIds, std::map<int, bool>& mapIgnoreComments)
{
	ParamsDocumentWriter oParamsDocumentWriter(&oComments);
	m_oParamsWriter.m_pCurRels = oParamsDocumentWriter.m_pRels;

	smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
	m_pOfficeDrawingConverter->SetRels(oParamsDocumentWriter.m_pRels);
	m_pOfficeDrawingConverter->ClearShapeTypes();

	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteCommentsContent(oComments, pCommentsExt, pCommentsExtensible, pCommentsUserData, pPeople, pCommentsIds, mapIgnoreComments, oParamsDocumentWriter);
	m_oBcw.WriteItemWithLengthEnd(nStart);
	
	m_pOfficeDrawingConverter->SetRels(oldRels);
}
void BinaryCommentsTableWriter::WriteCommentsContent(OOX::CComments& oComments, OOX::CCommentsExt* pCommentsExt, OOX::CCommentsExtensible* pCommentsExtensible, OOX::CCommentsUserData* pCommentsUserData, OOX::CPeople* pPeople, OOX::CCommentsIds* pCommentsIds, std::map<int, bool>& mapIgnoreComments, ParamsDocumentWriter& oParamsDocumentWriter)
{
	BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, oParamsDocumentWriter, &m_oParamsWriter.m_mapIgnoreComments, NULL);

	smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
	m_pOfficeDrawingConverter->SetRels(oParamsDocumentWriter.m_pRels);
	m_pOfficeDrawingConverter->ClearShapeTypes();

	std::map<std::wstring, OOX::CPerson*> mapAuthorToUserId;
	std::map<unsigned int, CCommentWriteTemp*> mapParaIdToComment;
	std::map<unsigned int, CCommentWriteTemp*> mapDurableIdToComment;
	std::vector<CCommentWriteTemp*> aCommentsToWrite;
	//map author -> userId
	if(NULL != pPeople)
	{
		for(size_t i = 0, length = pPeople->m_arrPeoples.size(); i < length; i++)
		{
			OOX::CPerson* pPerson = pPeople->m_arrPeoples[i];
			if(NULL != pPerson && pPerson->m_oAuthor.IsInit())
				mapAuthorToUserId[pPerson->m_oAuthor.get2()] = pPerson;
		}
	}
	//map paraId -> CCommentWriteTemp
	for(size_t i = 0, length = oComments.m_arrComments.size(); i < length; ++i)
	{
		OOX::CComment* pComment = oComments.m_arrComments[i];
		CCommentWriteTemp* pNewCommentWriteTemp = new CCommentWriteTemp();
		pNewCommentWriteTemp->pComment = pComment;
		if(pComment->m_oAuthor.IsInit())
		{
			std::map<std::wstring, OOX::CPerson*>::const_iterator pPair = mapAuthorToUserId.find(pComment->m_oAuthor.get2());
			if(mapAuthorToUserId.end() != pPair)
			{
				OOX::CPerson* pPerson = pPair->second;
				if(pPerson->m_oPresenceInfo.IsInit())
				{
					if(pPerson->m_oPresenceInfo->m_oUserId.IsInit())
					{
						pNewCommentWriteTemp->sUserId = pPerson->m_oPresenceInfo->m_oUserId.get();
					}
					if(pPerson->m_oPresenceInfo->m_oProviderId.IsInit())
					{
						pNewCommentWriteTemp->sProviderId = pPerson->m_oPresenceInfo->m_oProviderId.get();
					}
				}
			}
		}
		for(std::vector<OOX::WritingElement*>::iterator jt = pComment->m_arrItems.begin(); jt != pComment->m_arrItems.end(); jt++)
		{
			OOX::WritingElement* pWe = *jt;

			if(OOX::et_w_p == pWe->getType())
			{
				OOX::Logic::CParagraph* pParagraph = static_cast<OOX::Logic::CParagraph*>(pWe);
				if(pParagraph->m_oParaId.IsInit())
					mapParaIdToComment[pParagraph->m_oParaId->GetValue()] = pNewCommentWriteTemp;
			}
		}
		aCommentsToWrite.push_back(pNewCommentWriteTemp);
	}
	//разбираемся с reply и done
	if(NULL != pCommentsExt)
	{
		for(size_t i = 0, length = pCommentsExt->m_arrComments.size(); i < length; i++)
		{
			OOX::CCommentExt* pCommentExt = pCommentsExt->m_arrComments[i];
			if(pCommentExt->m_oParaId.IsInit())
			{
				std::map<unsigned int, CCommentWriteTemp*>::const_iterator pPair = mapParaIdToComment.find(pCommentExt->m_oParaId->GetValue());
				if(mapParaIdToComment.end() != pPair)
				{
					CCommentWriteTemp* pCommentWriteTemp = pPair->second;
					OOX::CComment* pComment = pCommentWriteTemp->pComment;
					if(pCommentExt->m_oDone.IsInit())
						pCommentWriteTemp->bDone = pCommentExt->m_oDone->ToBool();
					if(pCommentExt->m_oParaIdParent.IsInit())
					{
						unsigned int nParaIdParent = pCommentExt->m_oParaIdParent->GetValue();
						std::map<unsigned int, CCommentWriteTemp*>::const_iterator pPairParent = mapParaIdToComment.find(nParaIdParent);
						if(mapParaIdToComment.end() != pPairParent)
						{
							CCommentWriteTemp* pCommentWriteTempParent = pPairParent->second;
							pCommentWriteTempParent->aReplies.push_back(pCommentWriteTemp);
							if(NULL != pComment && pComment->m_oId.IsInit())
								mapIgnoreComments[pComment->m_oId->GetValue()] = true;
						}
					}
				}
			}
		}
	}
	if(NULL != pCommentsIds)
	{
		for(size_t i = 0, length = pCommentsIds->m_arrComments.size(); i < length; i++)
		{
			OOX::CCommentId* pCommentId = pCommentsIds->m_arrComments[i];
			if(pCommentId->m_oParaId.IsInit() && pCommentId->m_oDurableId.IsInit())
			{
				std::map<unsigned int, CCommentWriteTemp*>::const_iterator pPair = mapParaIdToComment.find(pCommentId->m_oParaId->GetValue());
				if(mapParaIdToComment.end() != pPair)
				{
					CCommentWriteTemp* pCommentWriteTemp = pPair->second;
					pCommentWriteTemp->nDurableId = pCommentId->m_oDurableId;
					mapDurableIdToComment[pCommentWriteTemp->nDurableId->GetValue()] = pCommentWriteTemp;
				}
			}
		}
	}

	if(NULL != pCommentsExtensible)
	{
		for(size_t i = 0, length = pCommentsExtensible->m_arrComments.size(); i < length; i++)
		{
			OOX::CCommentExtensible* pCommentExtensible = pCommentsExtensible->m_arrComments[i];
			if(pCommentExtensible->m_oDurableId.IsInit())
			{
				std::map<unsigned int, CCommentWriteTemp*>::const_iterator pPair = mapDurableIdToComment.find(pCommentExtensible->m_oDurableId->GetValue());
				if(mapDurableIdToComment.end() != pPair)
				{
					CCommentWriteTemp* pCommentWriteTemp = pPair->second;
					if (pCommentExtensible->m_oDateUtc.IsInit())
					{
						pCommentWriteTemp->sDateUtc = pCommentExtensible->m_oDateUtc->GetValue();
					}
				}
			}
		}
	}
	if(NULL != pCommentsUserData)
	{
		for(size_t i = 0, length = pCommentsUserData->m_arrComments.size(); i < length; i++)
		{
			OOX::CCommentExtensible* pCommentExtensible = pCommentsUserData->m_arrComments[i];
			if(pCommentExtensible->m_oDurableId.IsInit())
			{
				std::map<unsigned int, CCommentWriteTemp*>::const_iterator pPair = mapDurableIdToComment.find(pCommentExtensible->m_oDurableId->GetValue());
				if(mapDurableIdToComment.end() != pPair)
				{
					CCommentWriteTemp* pCommentWriteTemp = pPair->second;
					if(pCommentExtensible->m_oExtLst.IsInit())
					{
						for(size_t i = 0; i < pCommentExtensible->m_oExtLst->m_arrExt.size(); ++i)
						{
							OOX::Drawing::COfficeArtExtension* pExt = pCommentExtensible->m_oExtLst->m_arrExt[i];
							if ( pExt->m_oPresenceInfo.IsInit() )
							{
								pCommentWriteTemp->sUserData = pExt->m_oPresenceInfo->m_oUserId;
							}
						}
					}
				}
			}
		}
	}

	for(size_t i = 0, length = aCommentsToWrite.size(); i < length; ++i)
	{
		CCommentWriteTemp* pCommentWriteTemp = aCommentsToWrite[i];
		if(NULL != pCommentWriteTemp && NULL != pCommentWriteTemp->pComment && pCommentWriteTemp->pComment->m_oId.IsInit() && mapIgnoreComments.end() == mapIgnoreComments.find(pCommentWriteTemp->pComment->m_oId->GetValue()))
		{
			int nStart = m_oBcw.WriteItemStart(c_oSer_CommentsType::Comment);
			WriteComment(*aCommentsToWrite[i], oBinaryDocumentTableWriter);
			m_oBcw.WriteItemEnd(nStart);
		}
	}

	for(size_t i = 0, length = aCommentsToWrite.size(); i < length; ++i)
		delete aCommentsToWrite[i];
};
void BinaryCommentsTableWriter::WriteComment(CCommentWriteTemp& oComment, BinaryDocumentTableWriter & oBinaryDocumentTableWriter)
{
	int nCurPos = 0;
	OOX::CComment* pComment = oComment.pComment;
	if(NULL != pComment)
	{
		if(pComment->m_oAuthor.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::UserName);
			m_oBcw.m_oStream.WriteStringW(pComment->m_oAuthor.get2());
		}
		if(pComment->m_oDate.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::Date);
			m_oBcw.m_oStream.WriteStringW(pComment->m_oDate->ToString());
		}
		if(pComment->m_oOOData.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::OOData);
			m_oBcw.m_oStream.WriteStringW(pComment->m_oOOData.get2());
		}
		if(oComment.sDateUtc.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::DateUtc);
			m_oBcw.m_oStream.WriteStringW(oComment.sDateUtc.get2());
		}
		if(oComment.sUserData.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::UserData);
			m_oBcw.m_oStream.WriteStringW(oComment.sUserData.get2());
		}
		if(pComment->m_oId.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentsType::Id);
			m_oBcw.m_oStream.WriteLONG(pComment->m_oId->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(pComment->m_oInitials.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::Initials);
			m_oBcw.m_oStream.WriteStringW(pComment->m_oInitials.get2());
		}
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::Text);
		m_oBcw.m_oStream.WriteStringW(pComment->getText());

		if(oComment.sUserId.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::UserId);
			m_oBcw.m_oStream.WriteStringW(oComment.sUserId.get2());
		}
		if(oComment.sProviderId.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentsType::ProviderId);
			m_oBcw.m_oStream.WriteStringW(oComment.sProviderId.get2());
		}
		if(oComment.bDone.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentsType::Solved);
			m_oBcw.m_oStream.WriteBOOL(oComment.bDone.get2());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		_UINT32 nDurableId;
		if(oComment.nDurableId.IsInit())
		{
			nDurableId = oComment.nDurableId->GetValue();
		}
		else
		{
			//numbers greater than 0x7FFFFFFE cause MS Office errors(ST_LongHexNumber by spec)
			nDurableId = XmlUtils::GenerateInt() & 0x7FFFFFFF;
			nDurableId = (0x7FFFFFFF != nDurableId) ? nDurableId : nDurableId - 1;
		}
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentsType::DurableId);
		m_oBcw.m_oStream.WriteULONG(nDurableId);
		m_oBcw.WriteItemEnd(nCurPos);
//----------------------------
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentsType::CommentContent);
		oBinaryDocumentTableWriter.WriteDocumentContent(pComment->m_arrItems);
		m_oBcw.WriteItemEnd(nCurPos);
//----------------------------
		if(oComment.aReplies.size() > 0)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentsType::Replies);
			WriteReplies(oComment.aReplies, oBinaryDocumentTableWriter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}

};
void BinaryCommentsTableWriter::WriteReplies(std::vector<CCommentWriteTemp*>& aCommentWriteTemp, BinaryDocumentTableWriter & oBinaryDocumentTableWriter)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aCommentWriteTemp.size(); i < length; i++)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentsType::Comment);
		WriteComment(*aCommentWriteTemp[i], oBinaryDocumentTableWriter);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}

BinarySettingsTableWriter::BinarySettingsTableWriter(ParamsWriter& oParamsWriter):m_oBcw(oParamsWriter),bpPrs(oParamsWriter, NULL), brPrs(oParamsWriter)
{
};
void BinarySettingsTableWriter::Write(OOX::CSettings& oSettings, OOX::CSettingsCustom& oSettingsCustom)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteSettingsContent(oSettings, oSettingsCustom);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinarySettingsTableWriter::WriteSettingsContent(OOX::CSettings& oSettings, OOX::CSettingsCustom& oSettingsCustom)
{
	int nCurPos = 0;
	if(oSettings.m_oClrSchemeMapping.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::ClrSchemeMapping);
		WriteColorSchemeMapping(oSettings.m_oClrSchemeMapping.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oDefaultTabStop.IsInit() && oSettings.m_oDefaultTabStop->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::DefaultTabStopTwips);
		m_oBcw.m_oStream.WriteLONG(oSettings.m_oDefaultTabStop->m_oVal->ToTwips());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oMathPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::MathPr);
		WriteMathPr(oSettings.m_oMathPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oTrackRevisions.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::TrackRevisions);
		m_oBcw.m_oStream.WriteBOOL(oSettings.m_oTrackRevisions->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oFootnotePr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::FootnotePr);
		bpPrs.WriteNotePr(oSettings.m_oFootnotePr->m_oNumFmt, oSettings.m_oFootnotePr->m_oNumRestart, oSettings.m_oFootnotePr->m_oNumStart,
						  &oSettings.m_oFootnotePr->m_oPos, NULL, &oSettings.m_oFootnotePr->m_arrFootnote);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oEndnotePr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::EndnotePr);
		bpPrs.WriteNotePr(oSettings.m_oEndnotePr->m_oNumFmt, oSettings.m_oEndnotePr->m_oNumRestart, oSettings.m_oEndnotePr->m_oNumStart,
						  NULL, &oSettings.m_oEndnotePr->m_oPos, &oSettings.m_oEndnotePr->m_arrEndnote);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oCompat.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::Compat);
		WriteCompat(oSettings.m_oCompat.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oDecimalSymbol.IsInit() && oSettings.m_oDecimalSymbol->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::DecimalSymbol);
		m_oBcw.m_oStream.WriteStringW3(oSettings.m_oDecimalSymbol->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oListSeparator.IsInit() && oSettings.m_oListSeparator->m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::ListSeparator);
		m_oBcw.m_oStream.WriteStringW3(oSettings.m_oListSeparator->m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oGutterAtTop.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::GutterAtTop);
		m_oBcw.m_oStream.WriteBOOL(oSettings.m_oGutterAtTop->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oMirrorMargins.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::MirrorMargins);
		m_oBcw.m_oStream.WriteBOOL(oSettings.m_oMirrorMargins->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oPrintTwoOnOne.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::PrintTwoOnOne);
		m_oBcw.m_oStream.WriteBOOL(oSettings.m_oPrintTwoOnOne->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oBookFoldPrinting.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::BookFoldPrinting);
		m_oBcw.m_oStream.WriteBOOL(oSettings.m_oBookFoldPrinting->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oBookFoldPrintingSheets.IsInit() && oSettings.m_oBookFoldPrintingSheets->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::BookFoldPrintingSheets);
		m_oBcw.m_oStream.WriteLONG(*oSettings.m_oBookFoldPrintingSheets->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettings.m_oBookFoldRevPrinting.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::BookFoldRevPrinting);
		m_oBcw.m_oStream.WriteBOOL(oSettings.m_oBookFoldRevPrinting->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettingsCustom.m_oSdtGlobalColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::SdtGlobalColor);
		OOX::Logic::CRunProperty oRPr;
		oRPr.m_oColor = oSettingsCustom.m_oSdtGlobalColor;
		brPrs.Write_rPr(&oRPr);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettingsCustom.m_oSdtGlobalShowHighlight.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::SdtGlobalShowHighlight);
		m_oBcw.m_oStream.WriteBOOL(oSettingsCustom.m_oSdtGlobalShowHighlight->m_oVal.ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSettingsCustom.m_oSpecialFormsHighlight.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::SpecialFormsHighlight);
		OOX::Logic::CRunProperty oRPr;
		oRPr.m_oColor = oSettingsCustom.m_oSpecialFormsHighlight;
		brPrs.Write_rPr(&oRPr);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSettings.m_oDocumentProtection.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::DocumentProtection);
		WriteDocumentProtection(oSettings.m_oDocumentProtection.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSettings.m_oWriteProtection.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SettingsType::WriteProtection);
		WriteWriteProtection(oSettings.m_oWriteProtection.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
};
void BinarySettingsTableWriter::WriteMathPr(const OOX::Logic::CMathPr &pMathPr)
{
	if (pMathPr.m_oBrkBin.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::BrkBin);
		if (pMathPr.m_oBrkBin->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(pMathPr.m_oBrkBin->m_val->GetValue());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oBrkBinSub.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::BrkBinSub);
		if (pMathPr.m_oBrkBinSub->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(pMathPr.m_oBrkBinSub->m_val->GetValue());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oDefJc.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::DefJc);
		if (pMathPr.m_oDefJc->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(pMathPr.m_oDefJc->m_val->GetValue());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oDispDef.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::DispDef);
		if (pMathPr.m_oDispDef->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBOOL(pMathPr.m_oDispDef->m_val->ToBool());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oInterSp.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::InterSp);
		if (pMathPr.m_oInterSp->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::ValTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMathPr.m_oInterSp->m_val->ToTwips());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oIntLim.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::IntLim);
		if (pMathPr.m_oIntLim->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(pMathPr.m_oIntLim->m_val->GetValue());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oIntraSp.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::IntraSp);
		if (pMathPr.m_oIntraSp->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::ValTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMathPr.m_oIntraSp->m_val->ToTwips());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oLMargin.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::LMargin);
		if (pMathPr.m_oLMargin->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::ValTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMathPr.m_oLMargin->m_val->ToTwips());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oMathFont.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::MathFont);
		if (pMathPr.m_oMathFont->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
			m_oBcw.m_oStream.WriteStringW(*pMathPr.m_oMathFont->m_val);
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oNaryLim.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::NaryLim);
		if (pMathPr.m_oNaryLim->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBYTE(pMathPr.m_oNaryLim->m_val->GetValue());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oPostSp.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::PostSp);
		if (pMathPr.m_oPostSp->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::ValTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMathPr.m_oPostSp->m_val->ToTwips());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oPreSp.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::PreSp);
		if (pMathPr.m_oPreSp->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::ValTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMathPr.m_oPreSp->m_val->ToTwips());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oRMargin.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::RMargin);
		if (pMathPr.m_oRMargin->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::ValTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMathPr.m_oRMargin->m_val->ToTwips());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oSmallFrac.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::SmallFrac);
		if (pMathPr.m_oSmallFrac->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBOOL(pMathPr.m_oSmallFrac->m_val->ToBool());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oWrapIndent.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::WrapIndent);
		if (pMathPr.m_oWrapIndent->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::ValTwips);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
			m_oBcw.m_oStream.WriteLONG(pMathPr.m_oWrapIndent->m_val->ToTwips());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pMathPr.m_oWrapRight.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_MathPrType::WrapRight);
		if (pMathPr.m_oWrapRight->m_val.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_OMathBottomNodesValType::Val);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBOOL(pMathPr.m_oWrapRight->m_val->ToBool());
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinarySettingsTableWriter::WriteColorSchemeMapping(const PPTX::Logic::ClrMap& oColorSchemeMapping)
{
	int re_index[] = {0, 1, 2, 3, 4, 5, 10, 11, 6, 7, 9, 8, 10, 11, 10, 6, 7};

	int nCurPos = 0;

	PPTX::Limit::ColorSchemeIndex index;

	if (oColorSchemeMapping.Find(L"accent1", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Accent1);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if(oColorSchemeMapping.Find(L"accent2", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Accent2);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"accent3", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Accent3);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"accent4", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Accent4);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"accent5", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Accent5);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"accent6", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Accent6);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"bg1", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Bg1);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"bg2", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Bg2);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"folHlink", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::FollowedHyperlink);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"hlink", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::Hyperlink);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"tx1", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::T1);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
	if (oColorSchemeMapping.Find(L"tx2", index))
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ClrSchemeMappingType::T2);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(re_index[index.GetBYTECode()]);
	}
};
void BinarySettingsTableWriter::WriteCompat(const OOX::Settings::CCompat& oCompat)
{
	int nCurPos = 0;
	_UINT32 nFlags;
	nFlags = 0;
	COMPLEX_BOOL_TO_UINT(0, oCompat.m_oUseSingleBorderforContiguousCells);
	COMPLEX_BOOL_TO_UINT(1, oCompat.m_oWpJustification);
	COMPLEX_BOOL_TO_UINT(2, oCompat.m_oNoTabHangInd);
	COMPLEX_BOOL_TO_UINT(3, oCompat.m_oNoLeading);
	COMPLEX_BOOL_TO_UINT(4, oCompat.m_oSpaceForUL);
	COMPLEX_BOOL_TO_UINT(5, oCompat.m_oNoColumnBalance);
	COMPLEX_BOOL_TO_UINT(6, oCompat.m_oBalanceSingleByteDoubleByteWidth);
	COMPLEX_BOOL_TO_UINT(7, oCompat.m_oNoExtraLineSpacing);
	COMPLEX_BOOL_TO_UINT(8, oCompat.m_oDoNotLeaveBackslashAlone);
	COMPLEX_BOOL_TO_UINT(9, oCompat.m_oUlTrailSpace);
	COMPLEX_BOOL_TO_UINT(10, oCompat.m_oDoNotExpandShiftReturn);
	COMPLEX_BOOL_TO_UINT(11, oCompat.m_oSpacingInWholePoints);
	COMPLEX_BOOL_TO_UINT(12, oCompat.m_oLineWrapLikeWord6);
	COMPLEX_BOOL_TO_UINT(13, oCompat.m_oPrintBodyTextBeforeHeader);
	COMPLEX_BOOL_TO_UINT(14, oCompat.m_oPrintColBlack);
	COMPLEX_BOOL_TO_UINT(15, oCompat.m_oWpSpaceWidth);
	COMPLEX_BOOL_TO_UINT(16, oCompat.m_oShowBreaksInFrames);
	COMPLEX_BOOL_TO_UINT(17, oCompat.m_oSubFontBySize);
	COMPLEX_BOOL_TO_UINT(18, oCompat.m_oSuppressBottomSpacing);
	COMPLEX_BOOL_TO_UINT(19, oCompat.m_oSuppressTopSpacing);
	COMPLEX_BOOL_TO_UINT(20, oCompat.m_oSuppressSpacingAtTopOfPage);
	COMPLEX_BOOL_TO_UINT(21, oCompat.m_oSuppressTopSpacingWP);
	COMPLEX_BOOL_TO_UINT(22, oCompat.m_oSuppressSpBfAfterPgBrk);
	COMPLEX_BOOL_TO_UINT(23, oCompat.m_oSwapBordersFacingPages);
	COMPLEX_BOOL_TO_UINT(24, oCompat.m_oConvMailMergeEsc);
	COMPLEX_BOOL_TO_UINT(25, oCompat.m_oTruncateFontHeightsLikeWP6);
	COMPLEX_BOOL_TO_UINT(26, oCompat.m_oMwSmallCaps);
	COMPLEX_BOOL_TO_UINT(27, oCompat.m_oUsePrinterMetrics);
	COMPLEX_BOOL_TO_UINT(28, oCompat.m_oDoNotSuppressParagraphBorders);
	COMPLEX_BOOL_TO_UINT(29, oCompat.m_oWrapTrailSpaces);
	COMPLEX_BOOL_TO_UINT(30, oCompat.m_oFootnoteLayoutLikeWW8);
	COMPLEX_BOOL_TO_UINT(31, oCompat.m_oShapeLayoutLikeWW8);
	nCurPos = m_oBcw.WriteItemStart(c_oSerCompat::Flags1);
	m_oBcw.m_oStream.WriteULONG(nFlags);
	m_oBcw.WriteItemEnd(nCurPos);
	nFlags = 0;
	COMPLEX_BOOL_TO_UINT(0, oCompat.m_oAlignTablesRowByRow);
	COMPLEX_BOOL_TO_UINT(1, oCompat.m_oForgetLastTabAlignment);
	COMPLEX_BOOL_TO_UINT(2, oCompat.m_oAdjustLineHeightInTable);
	COMPLEX_BOOL_TO_UINT(3, oCompat.m_oAutoSpaceLikeWord95);
	COMPLEX_BOOL_TO_UINT(4, oCompat.m_oNoSpaceRaiseLower);
	COMPLEX_BOOL_TO_UINT(5, oCompat.m_oDoNotUseHTMLParagraphAutoSpacing);
	COMPLEX_BOOL_TO_UINT(6, oCompat.m_oLayoutRawTableWidth);
	COMPLEX_BOOL_TO_UINT(7, oCompat.m_oLayoutTableRowsApart);
	COMPLEX_BOOL_TO_UINT(8, oCompat.m_oUseWord97LineBreakRules);
	COMPLEX_BOOL_TO_UINT(9, oCompat.m_oDoNotBreakWrappedTables);
	COMPLEX_BOOL_TO_UINT(10, oCompat.m_oDoNotSnapToGridInCell);
	COMPLEX_BOOL_TO_UINT(11, oCompat.m_oSelectFldWithFirstOrLastChar);
	COMPLEX_BOOL_TO_UINT(12, oCompat.m_oApplyBreakingRules);
	COMPLEX_BOOL_TO_UINT(13, oCompat.m_oDoNotWrapTextWithPunct);
	COMPLEX_BOOL_TO_UINT(14, oCompat.m_oDoNotUseEastAsianBreakRules);
	COMPLEX_BOOL_TO_UINT(15, oCompat.m_oUseWord2002TableStyleRules);
	COMPLEX_BOOL_TO_UINT(16, oCompat.m_oGrowAutofit);
	COMPLEX_BOOL_TO_UINT(17, oCompat.m_oUseFELayout);
	COMPLEX_BOOL_TO_UINT(18, oCompat.m_oUseNormalStyleForList);
	COMPLEX_BOOL_TO_UINT(19, oCompat.m_oDoNotUseIndentAsNumberingTabStop);
	COMPLEX_BOOL_TO_UINT(20, oCompat.m_oUseAltKinsokuLineBreakRules);
	COMPLEX_BOOL_TO_UINT(21, oCompat.m_oAllowSpaceOfSameStyleInTable);
	COMPLEX_BOOL_TO_UINT(22, oCompat.m_oDoNotSuppressIndentation);
	COMPLEX_BOOL_TO_UINT(23, oCompat.m_oDoNotAutofitConstrainedTables);
	COMPLEX_BOOL_TO_UINT(24, oCompat.m_oAutofitToFirstFixedWidthCell);
	COMPLEX_BOOL_TO_UINT(25, oCompat.m_oUnderlineTabInNumList);
	COMPLEX_BOOL_TO_UINT(26, oCompat.m_oDisplayHangulFixedWidth);
	COMPLEX_BOOL_TO_UINT(27, oCompat.m_oSplitPgBreakAndParaMark);
	COMPLEX_BOOL_TO_UINT(28, oCompat.m_oDoNotVertAlignCellWithSp);
	COMPLEX_BOOL_TO_UINT(29, oCompat.m_oDoNotBreakConstrainedForcedTable);
	COMPLEX_BOOL_TO_UINT(30, oCompat.m_oDoNotVertAlignInTxbx);
	COMPLEX_BOOL_TO_UINT(31, oCompat.m_oUseAnsiKerningPairs);
	nCurPos = m_oBcw.WriteItemStart(c_oSerCompat::Flags2);
	m_oBcw.m_oStream.WriteULONG(nFlags);
	m_oBcw.WriteItemEnd(nCurPos);
	nFlags = 0;
	COMPLEX_BOOL_TO_UINT(0, oCompat.m_oCachedColBalance);
	nCurPos = m_oBcw.WriteItemStart(c_oSerCompat::Flags3);
	m_oBcw.m_oStream.WriteULONG(nFlags);
	m_oBcw.WriteItemEnd(nCurPos);

	for(size_t i = 0; i < oCompat.m_arrCompatSettings.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCompat::CompatSetting);
		WriteCompatSetting(*oCompat.m_arrCompatSettings[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinarySettingsTableWriter::WriteCompatSetting(const OOX::Settings::CCompatSetting& oCompatSetting)
{
	int nCurPos = 0;
	if(oCompatSetting.m_sName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart	(c_oSerCompat::CompatName);
		m_oBcw.m_oStream.WriteStringW3(oCompatSetting.m_sName.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oCompatSetting.m_sUri.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart	(c_oSerCompat::CompatUri);
		m_oBcw.m_oStream.WriteStringW3(oCompatSetting.m_sUri.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oCompatSetting.m_sVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart	(c_oSerCompat::CompatValue);
		m_oBcw.m_oStream.WriteStringW3(oCompatSetting.m_sVal.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinarySettingsTableWriter::WriteDocumentProtection(const OOX::Settings::CDocProtect& oDocProtect)
{
	int nCurPos = 0;

	if (oDocProtect.m_oAlgorithmName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::AlgorithmName);
		m_oBcw.m_oStream.WriteBYTE(oDocProtect.m_oAlgorithmName->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oEdit.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::Edit);
		m_oBcw.m_oStream.WriteBYTE(oDocProtect.m_oEdit->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oEnforcment.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::Enforcment);
		m_oBcw.m_oStream.WriteBOOL(oDocProtect.m_oEnforcment.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oFormatting.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::Formatting);
		m_oBcw.m_oStream.WriteBOOL(oDocProtect.m_oFormatting.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_sHashValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::HashValue);
		m_oBcw.m_oStream.WriteStringW3(oDocProtect.m_sHashValue.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_sSaltValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::SaltValue);
		m_oBcw.m_oStream.WriteStringW3(oDocProtect.m_sSaltValue.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oSpinCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::SpinCount);
		m_oBcw.m_oStream.WriteLONG(oDocProtect.m_oSpinCount.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//ext
	if (oDocProtect.m_oAlgIdExt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::AlgIdExt);
		m_oBcw.m_oStream.WriteStringW3(oDocProtect.m_oAlgIdExt.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oAlgIdExtSource.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::AlgIdExtSource);
		m_oBcw.m_oStream.WriteStringW3(oDocProtect.m_oAlgIdExtSource.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oCryptAlgorithmClass.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::CryptAlgorithmClass);
		m_oBcw.m_oStream.WriteBYTE(oDocProtect.m_oCryptAlgorithmClass->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oCryptAlgorithmSid.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::CryptAlgorithmSid);
		m_oBcw.m_oStream.WriteLONG(oDocProtect.m_oCryptAlgorithmSid.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oCryptAlgorithmType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::CryptAlgorithmType);
		m_oBcw.m_oStream.WriteBYTE(oDocProtect.m_oCryptAlgorithmType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oCryptProvider.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::CryptProvider);
		m_oBcw.m_oStream.WriteStringW3(oDocProtect.m_oCryptProvider.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oCryptProviderType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::CryptProviderType);
		m_oBcw.m_oStream.WriteBYTE(oDocProtect.m_oCryptProviderType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oCryptProviderTypeExt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::CryptProviderTypeExt);
		m_oBcw.m_oStream.WriteStringW3(oDocProtect.m_oCryptProviderTypeExt.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDocProtect.m_oCryptProviderTypeExtSource.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oDocProtect::CryptProviderTypeExtSource);
		m_oBcw.m_oStream.WriteStringW3(oDocProtect.m_oCryptProviderTypeExtSource.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinarySettingsTableWriter::WriteWriteProtection(const OOX::Settings::CWriteProtection& oWriteProtect)
{
	int nCurPos = 0;
	if (oWriteProtect.m_oAlgorithmName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::AlgorithmName);
		m_oBcw.m_oStream.WriteBYTE(oWriteProtect.m_oAlgorithmName->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oRecommended.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::Recommended);
		m_oBcw.m_oStream.WriteBOOL(oWriteProtect.m_oRecommended.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_sHashValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::HashValue);
		m_oBcw.m_oStream.WriteStringW3(oWriteProtect.m_sHashValue.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_sSaltValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::SaltValue);
		m_oBcw.m_oStream.WriteStringW3(oWriteProtect.m_sSaltValue.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oSpinCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::SpinCount);
		m_oBcw.m_oStream.WriteLONG(oWriteProtect.m_oSpinCount.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//ext
	if (oWriteProtect.m_oAlgIdExt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::AlgIdExt);
		m_oBcw.m_oStream.WriteStringW3(oWriteProtect.m_oAlgIdExt.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oAlgIdExtSource.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::AlgIdExtSource);
		m_oBcw.m_oStream.WriteStringW3(oWriteProtect.m_oAlgIdExtSource.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oCryptAlgorithmClass.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::CryptAlgorithmClass);
		m_oBcw.m_oStream.WriteBYTE(oWriteProtect.m_oCryptAlgorithmClass->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oCryptAlgorithmSid.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::CryptAlgorithmSid);
		m_oBcw.m_oStream.WriteLONG(oWriteProtect.m_oCryptAlgorithmSid.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oCryptAlgorithmType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::CryptAlgorithmType);
		m_oBcw.m_oStream.WriteBYTE(oWriteProtect.m_oCryptAlgorithmType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oCryptProvider.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::CryptProvider);
		m_oBcw.m_oStream.WriteStringW3(oWriteProtect.m_oCryptProvider.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oCryptProviderType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::CryptProviderType);
		m_oBcw.m_oStream.WriteBYTE(oWriteProtect.m_oCryptProviderType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oCryptProviderTypeExt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::CryptProviderTypeExt);
		m_oBcw.m_oStream.WriteStringW3(oWriteProtect.m_oCryptProviderTypeExt.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oWriteProtect.m_oCryptProviderTypeExtSource.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oWriteProtect::CryptProviderTypeExtSource);
		m_oBcw.m_oStream.WriteStringW3(oWriteProtect.m_oCryptProviderTypeExtSource.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}

}

BinaryNotesTableWriter::BinaryNotesTableWriter(ParamsWriter& oParamsWriter):
	m_oBcw(oParamsWriter), m_oParamsWriter(oParamsWriter), m_pOfficeDrawingConverter(oParamsWriter.m_pOfficeDrawingConverter)
{
}
void BinaryNotesTableWriter::WriteFootnotes(OOX::CFootnotes& oFootnotes)
{
	ParamsDocumentWriter oParamsDocumentWriter(&oFootnotes);
	m_oParamsWriter.m_pCurRels = oParamsDocumentWriter.m_pRels;

	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteNotes(oFootnotes.m_arrFootnote, oParamsDocumentWriter);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryNotesTableWriter::WriteEndnotes(OOX::CEndnotes& oEndnotes)
{
	ParamsDocumentWriter oParamsDocumentWriter(&oEndnotes);
	m_oParamsWriter.m_pCurRels = oParamsDocumentWriter.m_pRels;

	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteNotes(oEndnotes.m_arrEndnote, oParamsDocumentWriter);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryNotesTableWriter::WriteNotes(const std::vector<OOX::CFtnEdn*>& arrNotes, ParamsDocumentWriter& oParamsDocumentWriter)
{
	BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, oParamsDocumentWriter, &m_oParamsWriter.m_mapIgnoreComments, NULL);

	smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
	m_pOfficeDrawingConverter->SetRels(oParamsDocumentWriter.m_pRels);
	m_pOfficeDrawingConverter->ClearShapeTypes();

	int nCurPos = 0;
	for(size_t i = 0 ; i < arrNotes.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::Note);
		WriteNote(*arrNotes[i], oBinaryDocumentTableWriter);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	m_pOfficeDrawingConverter->SetRels(oldRels);
}
void BinaryNotesTableWriter::WriteNote(const OOX::CFtnEdn& oFtnEdn, BinaryDocumentTableWriter & oBinaryDocumentTableWriter)
{
	int nCurPos = 0;
	if(oFtnEdn.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::NoteType);
		m_oBcw.m_oStream.WriteBYTE(oFtnEdn.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFtnEdn.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::NoteId);
		m_oBcw.m_oStream.WriteLONG(oFtnEdn.m_oId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	nCurPos = m_oBcw.WriteItemStart(c_oSerNotes::NoteContent);
	oBinaryDocumentTableWriter.WriteDocumentContent(oFtnEdn.m_arrItems);
	m_oBcw.WriteItemEnd(nCurPos);
}
BinaryCustomsTableWriter::BinaryCustomsTableWriter(ParamsWriter& oParamsWriter) : m_oParamsWriter(oParamsWriter), m_oBcw(oParamsWriter)
{
}
void BinaryCustomsTableWriter::Write(OOX::CDocument* pDocument)
{
	if (!pDocument) return;

	int nStart = m_oBcw.WriteItemWithLengthStart();
	
	std::vector<smart_ptr<OOX::File>>& container = pDocument->GetContainer();
	for (size_t i = 0; i < container.size(); ++i)
	{
		if (OOX::FileTypes::CustomXml == container[i]->type())
		{
			OOX::CCustomXML* pCustomXml = dynamic_cast<OOX::CCustomXML*>(container[i].GetPointer());
			if (pCustomXml->m_bUsed) continue;

			int nCurPos = m_oBcw.WriteItemStart(BinDocxRW::c_oSerCustoms::Custom);
			
			std::vector<smart_ptr<OOX::File>>& containerCustom = pCustomXml->GetContainer();
			for (size_t i = 0; i < containerCustom.size(); ++i)
			{
				if (OOX::FileTypes::CustomXmlProps == containerCustom[i]->type())
				{
					OOX::CCustomXMLProps* pCustomXmlProps = dynamic_cast<OOX::CCustomXMLProps*>(containerCustom[i].GetPointer());
					
					int nCurPos1 = m_oBcw.WriteItemStart(c_oSerCustoms::ItemId);
					m_oBcw.m_oStream.WriteStringW3(pCustomXmlProps->m_oItemID.ToString());
					m_oBcw.WriteItemEnd(nCurPos1);

					if (pCustomXmlProps->m_oShemaRefs.IsInit())
					{
						for (size_t j = 0; j < pCustomXmlProps->m_oShemaRefs->m_arrItems.size(); ++j)
						{
							nCurPos1 = m_oBcw.WriteItemStart(c_oSerCustoms::Uri);
							m_oBcw.m_oStream.WriteStringW3(pCustomXmlProps->m_oShemaRefs->m_arrItems[j]->m_sUri);
							m_oBcw.WriteItemEnd(nCurPos1);
						}
					}
				}
			}
			
			int nCurPos2 = m_oBcw.WriteItemStart(c_oSerCustoms::ContentA);
			m_oBcw.m_oStream.WriteStringA(pCustomXml->m_sXmlA);
			m_oBcw.WriteItemEnd(nCurPos2);
			
			m_oBcw.WriteItemEnd(nCurPos);
			pCustomXml->m_bUsed = true;
		}
	}
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
//----------------------------------------------------------------------------------------------------------------------------
BinaryFileWriter::BinaryFileWriter(ParamsWriter& oParamsWriter) : m_oParamsWriter(oParamsWriter), m_oBcw(oParamsWriter)
{
	m_nLastFilePos = 0;
	m_nRealTableCount = 0;
}
std::wstring BinaryFileWriter::WriteFileHeader(long nDataSize, int version)
{
	std::wstring sHeader = std::wstring(g_sFormatSignature) + L";v" + std::to_wstring(version) + L";" + std::to_wstring(nDataSize) + L";";
	return sHeader;
}
void BinaryFileWriter::WriteMainTableStart(bool bSigTable)
{
	int nTableCount = 128;//Специально ставим большое число, чтобы не увеличивать его при добавлении очередной таблицы.
	m_nRealTableCount = 0;
	m_nMainTableStart = m_oBcw.m_oStream.GetPosition();
	//вычисляем с какой позиции можно писать таблицы
	int nmtItemSize = 5;//5 byte
	m_nLastFilePos = m_nMainTableStart + nTableCount * nmtItemSize;
	//Write mtLen
	m_oBcw.m_oStream.WriteBYTE(0);

	if (bSigTable)
	{
		//BinarySigTableWriter
		int nCurPos = WriteTableStart(c_oSerTableTypes::Signature);
		BinarySigTableWriter oBinarySigTableWriter(m_oParamsWriter);
		oBinarySigTableWriter.Write();
		WriteTableEnd(nCurPos);
	}
}
void BinaryFileWriter::WriteMainTableEnd()
{
	//Количество таблиц
	m_oBcw.m_oStream.SetPosition(m_nMainTableStart);
	m_oBcw.m_oStream.WriteBYTE(m_nRealTableCount);

	//Seek в конец
	m_oBcw.m_oStream.SetPosition(m_nLastFilePos);
}
int BinaryFileWriter::WriteTableStart(BYTE type, int nStartPos)
{
	if(-1 != nStartPos)
		m_oBcw.m_oStream.SetPosition(nStartPos);
	//Write mtItem
	//Write mtiType
	m_oBcw.m_oStream.WriteBYTE(type);
	//Write mtiOffBits
	m_oBcw.m_oStream.WriteLONG(m_nLastFilePos);

	//Write table
	//Запоминаем позицию в MainTable
	int nCurPos = m_oBcw.m_oStream.GetPosition();
	//Seek в свободную область
	m_oBcw.m_oStream.SetPosition(m_nLastFilePos);
	return nCurPos;
}
void BinaryFileWriter::WriteTableEnd(int nCurPos)
{
	//сдвигаем позицию куда можно следующую таблицу
	m_nLastFilePos = m_oBcw.m_oStream.GetPosition();
	m_nRealTableCount++;
	//Seek вобратно в MainTable
	m_oBcw.m_oStream.SetPosition(nCurPos);
}
void BinaryFileWriter::intoBindoc(const std::wstring& sSrcPath)
{
	NSBinPptxRW::CBinaryFileWriter& oBufferedStream = m_oBcw.m_oStream;

	OOX::CDocx		*pDocx = new OOX::CDocx(OOX::CPath(sSrcPath));
	OOX::CDocxFlat	*pDocxFlat = NULL;

	OOX::CDocument	*pDocument = NULL;
	OOX::CComments	*pComments = NULL;

	if ((pDocx) && (pDocx->m_oMain.document))
	{
		m_oParamsWriter.m_pTheme = pDocx->m_pTheme;

		*oBufferedStream.m_pTheme = smart_ptr<PPTX::Theme>(pDocx->m_pTheme);
		oBufferedStream.m_pTheme->AddRef();

		pDocument = pDocx->m_oMain.document;
		pComments = pDocx->m_oMain.comments;

		m_oParamsWriter.m_pSettings = pDocx->m_oMain.settings;
		m_oParamsWriter.m_pStyles = pDocx->m_oMain.styles;
		m_oParamsWriter.m_pNumbering = pDocx->m_oMain.numbering;
		m_oParamsWriter.m_pMain = dynamic_cast<OOX::Document*>(pDocx);
	}
	else
	{
		if (pDocx) delete pDocx; pDocx = NULL;

		pDocxFlat = new OOX::CDocxFlat(OOX::CPath(sSrcPath));
		if ((pDocxFlat) && (pDocxFlat->m_pDocument.IsInit()))
		{
			pDocument = pDocxFlat->m_pDocument.GetPointer();
			pComments = pDocxFlat->m_pComments.GetPointer();

			m_oParamsWriter.m_pSettings = pDocxFlat->m_pSettings.GetPointer();
			m_oParamsWriter.m_pStyles = pDocxFlat->m_pStyles.GetPointer();
			m_oParamsWriter.m_pNumbering = pDocxFlat->m_pNumbering.GetPointer();
			m_oParamsWriter.m_pMain = dynamic_cast<OOX::Document*>(pDocxFlat);
		}
	}

	if (pDocument == NULL)
	{
		if (pDocx)		delete pDocx;		pDocx = NULL;
		if (pDocxFlat)	delete pDocxFlat;	pDocxFlat = NULL;
		return;
	}

	m_oParamsWriter.m_pOfficeDrawingConverter->m_nDrawingMaxZIndex = pDocument->m_nDrawingMaxZIndex;

	this->WriteMainTableStart();

	OOX::CSettingsCustom oSettingsCustom;
	if ((pDocx) && (pDocx->m_oMain.settings))
	{
		std::wstring sSettings = pDocx->GetCustomSettings();
		if (false == sSettings.empty())
		{
			oSettingsCustom.fromXML(sSettings);
		}
	}
	int nCurPos = 0;

	if (m_oParamsWriter.m_pSettings)
	{
		BinDocxRW::BinarySettingsTableWriter oBinarySettingsTableWriter(m_oParamsWriter);
		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Settings);
		oBinarySettingsTableWriter.Write(*m_oParamsWriter.m_pSettings, oSettingsCustom);
		this->WriteTableEnd(nCurPos);
	}

	if (pComments)
	{
		BinDocxRW::BinaryCommentsTableWriter oBinaryCommentsTableWriter(m_oParamsWriter);
		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Comments);
		oBinaryCommentsTableWriter.Write(*pComments, (pDocx ? pDocx->m_pCommentsExt : NULL),
			(pDocx ? pDocx->m_pCommentsExtensible : NULL),
			(pDocx ? pDocx->m_pCommentsUserData : NULL),
			(pDocx ? pDocx->m_pPeople : NULL),
			(pDocx ? pDocx->m_pCommentsIds : NULL),
			m_oParamsWriter.m_mapIgnoreComments);
		this->WriteTableEnd(nCurPos);
	}
	if ((pDocx) && (pDocx->m_pDocumentComments))
	{
		BinDocxRW::BinaryCommentsTableWriter oBinaryCommentsTableWriter(m_oParamsWriter);
		int nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::DocumentComments);
		oBinaryCommentsTableWriter.Write(*pDocx->m_pDocumentComments, pDocx->m_pDocumentCommentsExt, pDocx->m_pDocumentCommentsExtensible, pDocx->m_pCommentsUserData, pDocx->m_pDocumentPeople, pDocx->m_pDocumentCommentsIds, m_oParamsWriter.m_mapIgnoreComments);
		this->WriteTableEnd(nCurPos);
	}
	{
		BinDocxRW::BinaryNotesTableWriter oBinaryNotesWriter(m_oParamsWriter);
		if ((pDocx) && (pDocx->m_oMain.footnotes))
		{
			nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Footnotes);
			oBinaryNotesWriter.WriteFootnotes(*pDocx->m_oMain.footnotes);
			this->WriteTableEnd(nCurPos);
		}
		if ((pDocx) && (pDocx->m_oMain.endnotes))
		{
			nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Endnotes);
			oBinaryNotesWriter.WriteEndnotes(*pDocx->m_oMain.endnotes);
			this->WriteTableEnd(nCurPos);
		}
	}

//Write App
	if ((pDocx) && (pDocx->m_pApp))
	{
		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::App);
		pDocx->m_pApp->ToPptxApp()->toPPTY(&oBufferedStream);
		this->WriteTableEnd(nCurPos);
	}

	if ((pDocx) && (pDocx->m_pCore))
	{
		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Core);
		pDocx->m_pCore->ToPptxCore()->toPPTY(&oBufferedStream);
		this->WriteTableEnd(nCurPos);
	}
	if (NULL != m_oParamsWriter.m_pTheme)
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::Other);
		BinaryOtherTableWriter oBinaryOtherTableWriter(m_oParamsWriter, m_oParamsWriter.m_pTheme);
		oBinaryOtherTableWriter.Write();
		WriteTableEnd(nCurPos);
	}
	if(pDocx)
	{
		smart_ptr<OOX::File> pFile = pDocx->Find(OOX::FileTypes::CustomProperties);
		PPTX::CustomProperties *pCustomProperties = dynamic_cast<PPTX::CustomProperties*>(pFile.GetPointer());
		if (pCustomProperties)
		{
			nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::CustomProperties);
			pCustomProperties->toPPTY(&m_oBcw.m_oStream);
			this->WriteTableEnd(nCurPos);
		}
	}
	{
		ParamsDocumentWriter oParamsDocumentWriter(pDocument);
		m_oParamsWriter.m_pCurRels = oParamsDocumentWriter.m_pRels;

		BinDocxRW::BinaryHeaderFooterTableWriter oBinaryHeaderFooterTableWriter(m_oParamsWriter, pDocument, &m_oParamsWriter.m_mapIgnoreComments);
		BinDocxRW::BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, oParamsDocumentWriter, &m_oParamsWriter.m_mapIgnoreComments, &oBinaryHeaderFooterTableWriter);

		m_oParamsWriter.m_pOfficeDrawingConverter->SetRels(oParamsDocumentWriter.m_pRels);
		m_oParamsWriter.m_pOfficeDrawingConverter->ClearShapeTypes();

		oBinaryDocumentTableWriter.poDocument = pDocument;
		oBinaryDocumentTableWriter.pSectPr = pDocument->m_oSectPr.GetPointer();;
		oBinaryDocumentTableWriter.pJsaProject = pDocx ? pDocx->m_pJsaProject : NULL;
		oBinaryDocumentTableWriter.m_bWriteSectPr = true;

		if (pDocx)
		{
			oBinaryDocumentTableWriter.pBackground = dynamic_cast<OOX::WritingElement*>(pDocument->m_oBackground.GetPointer());
		}
		else if (pDocxFlat)
		{
			oBinaryDocumentTableWriter.pBackground = dynamic_cast<OOX::WritingElement*>(pDocxFlat->m_pBgPict.GetPointer());
		}

		// Write content
		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Document);
		oBinaryDocumentTableWriter.Write(pDocument->m_arrItems);
		this->WriteTableEnd(nCurPos);

		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::HdrFtr);
		oBinaryHeaderFooterTableWriter.Write();
		this->WriteTableEnd(nCurPos);

		if ((pDocx) && (pDocx->m_pVbaProject))
		{
			nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::VbaProject);

			m_oBcw.m_oStream.StartRecord(0);
			pDocx->m_pVbaProject->toPPTY(&m_oBcw.m_oStream);
			m_oBcw.m_oStream.EndRecord();

			this->WriteTableEnd(nCurPos);			

			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		std::wstring pathOForm = sSrcPath + FILE_SEPARATOR_STR + L"oform";
		if (NSDirectory::Exists(pathOForm))
		{
			std::wstring pathOForm_tmp_zip = NSFile::CFileBinary::CreateTempFileWithUniqueName(sSrcPath, L"oform");
			COfficeUtils oCOfficeUtils(NULL);
			if (S_OK == oCOfficeUtils.CompressFileOrDirectory(pathOForm, pathOForm_tmp_zip))
			{
				BYTE *pDataOForm = NULL;
				DWORD nDataOForm = 0;
				if (NSFile::CFileBinary::ReadAllBytes(pathOForm_tmp_zip, &pDataOForm, nDataOForm) && (pDataOForm))
				{
					nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::OForm);

					m_oBcw.WriteBytesArray(pDataOForm, nDataOForm);
					
					this->WriteTableEnd(nCurPos);

					m_oBcw.WriteItemWithLengthEnd(nCurPos);

					delete[]pDataOForm;
				}
				NSFile::CFileBinary::Remove(pathOForm_tmp_zip);
			}
		}
	}

	if (m_oParamsWriter.m_pStyles)
	{
		BinDocxRW::BinaryStyleTableWriter oBinaryStyleTableWriter(m_oParamsWriter);

		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Style);
		oBinaryStyleTableWriter.Write(*m_oParamsWriter.m_pStyles);
		this->WriteTableEnd(nCurPos);
	}

	if (m_oParamsWriter.m_pNumbering)
	{
		BinDocxRW::BinaryNumberingTableWriter oBinaryNumberingTableWriter(m_oParamsWriter);

		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Numbering);
		oBinaryNumberingTableWriter.Write(*m_oParamsWriter.m_pNumbering);
		this->WriteTableEnd(nCurPos);
	}

	if (m_oParamsWriter.m_bLocalStyles)
		delete m_oParamsWriter.m_pStyles;

	if (m_oParamsWriter.m_bLocalNumbering)
		delete m_oParamsWriter.m_pNumbering;	
	
	{
		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Customs);
			BinDocxRW::BinaryCustomsTableWriter oBinaryCustomsTableWriter(m_oParamsWriter);
			oBinaryCustomsTableWriter.Write(pDocument);
		this->WriteTableEnd(nCurPos);
	}

	if (pDocx && pDocx->m_oGlossary.document)
	{
		m_oParamsWriter.m_pSettings = pDocx->m_oGlossary.settings;
		m_oParamsWriter.m_pStyles = pDocx->m_oGlossary.styles;
		m_oParamsWriter.m_pNumbering = pDocx->m_oGlossary.numbering;
		
		nCurPos = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Glossary);

		int nRealTableCount = m_nRealTableCount;
		int nMainTableStart = m_nMainTableStart;
		int nLastFilePos = m_nLastFilePos;
		
		this->WriteMainTableStart(false);
		{
			int nCurPos1 = 0;
			if (m_oParamsWriter.m_pSettings)
			{
				BinDocxRW::BinarySettingsTableWriter oBinarySettingsTableWriter(m_oParamsWriter);
				nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Settings);
				oBinarySettingsTableWriter.Write(*m_oParamsWriter.m_pSettings, oSettingsCustom);
				this->WriteTableEnd(nCurPos1);
			}

			if (pDocx->m_oGlossary.comments)
			{
				BinDocxRW::BinaryCommentsTableWriter oBinaryCommentsTableWriter(m_oParamsWriter);
				nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Comments);
				oBinaryCommentsTableWriter.Write(*pDocx->m_oGlossary.comments, NULL, NULL, NULL, NULL, NULL, m_oParamsWriter.m_mapIgnoreComments);
				this->WriteTableEnd(nCurPos1);
			}
			{
				ParamsDocumentWriter oParamsDocumentWriter(pDocx->m_oGlossary.document);
				m_oParamsWriter.m_pCurRels = oParamsDocumentWriter.m_pRels;

				BinDocxRW::BinaryHeaderFooterTableWriter oBinaryHeaderFooterTableWriter(m_oParamsWriter, pDocument, &m_oParamsWriter.m_mapIgnoreComments);
				BinDocxRW::BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, oParamsDocumentWriter, &m_oParamsWriter.m_mapIgnoreComments, &oBinaryHeaderFooterTableWriter);

				m_oParamsWriter.m_pOfficeDrawingConverter->SetRels(oParamsDocumentWriter.m_pRels);
				m_oParamsWriter.m_pOfficeDrawingConverter->ClearShapeTypes();

				oBinaryDocumentTableWriter.poDocument = pDocx->m_oGlossary.document;
				oBinaryDocumentTableWriter.pSectPr = pDocx->m_oGlossary.document->m_oSectPr.GetPointer();;
				oBinaryDocumentTableWriter.pBackground = dynamic_cast<OOX::WritingElement*>(pDocx->m_oGlossary.document->m_oBackground.GetPointer());
				oBinaryDocumentTableWriter.m_bWriteSectPr = true;

				nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Document);
					oBinaryDocumentTableWriter.Write(pDocx->m_oGlossary.document->m_arrItems);
				this->WriteTableEnd(nCurPos1);

				nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::HdrFtr);
				oBinaryHeaderFooterTableWriter.Write();
				this->WriteTableEnd(nCurPos1);
			}
			{
				BinDocxRW::BinaryNotesTableWriter oBinaryNotesWriter(m_oParamsWriter);
				if (pDocx->m_oGlossary.footnotes)
				{
					nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Footnotes);
					oBinaryNotesWriter.WriteFootnotes(*pDocx->m_oGlossary.footnotes);
					this->WriteTableEnd(nCurPos1);
				}
				if (pDocx->m_oGlossary.endnotes)
				{
					nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Endnotes);
					oBinaryNotesWriter.WriteEndnotes(*pDocx->m_oGlossary.endnotes);
					this->WriteTableEnd(nCurPos1);
				}
			}
			if (m_oParamsWriter.m_pStyles)
			{
				BinDocxRW::BinaryStyleTableWriter oBinaryStyleTableWriter(m_oParamsWriter);

				nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Style);
				oBinaryStyleTableWriter.Write(*m_oParamsWriter.m_pStyles);
				this->WriteTableEnd(nCurPos1);
			}
			if (m_oParamsWriter.m_pNumbering)
			{
				BinDocxRW::BinaryNumberingTableWriter oBinaryNumberingTableWriter(m_oParamsWriter);

				nCurPos1 = this->WriteTableStart(BinDocxRW::c_oSerTableTypes::Numbering);
				oBinaryNumberingTableWriter.Write(*m_oParamsWriter.m_pNumbering);
				this->WriteTableEnd(nCurPos1);
			}
		}
		this->WriteMainTableEnd();

		m_nRealTableCount = nRealTableCount;
		m_nMainTableStart = nMainTableStart;
		m_nLastFilePos =  nLastFilePos;

		this->WriteTableEnd(nCurPos);
	}
	this->WriteMainTableEnd();

	if (pDocx)		delete pDocx;		pDocx = NULL;
	if (pDocxFlat)	delete pDocxFlat;	pDocxFlat = NULL;
}
void BinaryFileWriter::ParagraphAddBreak(OOX::Logic::CParagraph* pParagraph)
{
	if(NULL != pParagraph)
	{
		OOX::Logic::CParagraphProperty* pPr = NULL;

		for(size_t i = 0; i < pParagraph->m_arrItems.size(); ++i)
		{
			OOX::WritingElement* we = pParagraph->m_arrItems[i];

			if(OOX::et_w_pPr == we->getType())
			{
				pPr = static_cast<OOX::Logic::CParagraphProperty*>(we);
				break;
			}
		}
		if(NULL == pPr)
		{
			pPr = new OOX::Logic::CParagraphProperty();
			if(pParagraph->m_arrItems.size() > 0)
				pParagraph->m_arrItems.insert(pParagraph->m_arrItems.begin(), pPr);
			else
				pParagraph->m_arrItems.push_back(pPr);
		}
		pPr->m_oPageBreakBefore.Init();
		pPr->m_oPageBreakBefore->m_oVal.FromBool(true);
	}
}

}

