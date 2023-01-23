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
#pragma once

#include "Styles.h"

#include "../../XlsbFormat/Xlsb.h"
#include "../XlsxFlat.h"

#include "../../XlsbFormat/StylesStream.h"
#include "../../XlsbFormat/Biff12_unions/FMTS.h"
#include "../../XlsbFormat/Biff12_unions/FONTS.h"
#include "../../XlsbFormat/Biff12_unions/FILLS.h"
#include "../../XlsbFormat/Biff12_unions/BORDERS.h"
#include "../../XlsbFormat/Biff12_unions/CELLSTYLEXFS.h"
#include "../../XlsbFormat/Biff12_unions/CELLXFS.h"
#include "../../XlsbFormat/Biff12_unions/STYLES.h"
#include "../../XlsbFormat/Biff12_unions/DXFS.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CStyle2003::CStyle2003(OOX::Document* pMain) : WritingElement(pMain)
		{
		}
		CStyle2003::~CStyle2003()
		{
		}
		void CStyle2003::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CStyle2003::toXML() const
		{
			return L"";
		}
		void CStyle2003::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		EElementType CStyle2003::getType () const
		{
			return et_x_Style2003;
		}
		void CStyle2003::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nDocumentDepth = oReader.GetDepth();
			std::wstring sName;

			while (oReader.ReadNextSiblingNode(nDocumentDepth))
			{
				sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"Borders" == sName)
				{
					m_oBorder = oReader;
					if (m_oBorder.IsInit())
					{
						bStyleContinuous = m_oBorder->bBorderContinuous; // todooo - one border exclusive
					}
				}
				else if (L"Alignment" == sName)
					m_oAligment = oReader;
				else if (L"Font" == sName)
					m_oFont = oReader;
				else if (L"Interior" == sName)
					m_oFill = oReader;
				else if (L"NumberFormat" == sName)
					m_oNumFmt = oReader;
			}
		}
		void CStyle2003::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"ss:ID", m_sId)
				WritingElement_ReadAttributes_Read_if(oReader, L"ss:Name", m_sName)
				WritingElement_ReadAttributes_Read_if(oReader, L"ss:Parent", m_sParentId)
				WritingElement_ReadAttributes_End(oReader)
		}

//----------------------------------------------------------------------------------------------------------------------

		CStyles::CStyles(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain), WritingElement(pMain)
		{
			m_bSpreadsheets = true;

			CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
			if (xlsx)
			{
				xlsx->m_pStyles = this;
			}
		}
		CStyles::CStyles(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain), WritingElement(pMain)
		{
			m_bSpreadsheets = true;

			CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
			if (xlsx)
			{
				xlsx->m_pStyles = this;
			}
			read(oRootPath, oPath);
		}
		CStyles::~CStyles()
		{
			for (size_t i = 0; i < m_arrStyles2003.size(); i++)
			{
				if (m_arrStyles2003[i]) delete m_arrStyles2003[i]; m_arrStyles2003[i] = NULL;
			}
			m_arrStyles2003.clear();
			m_mapStyles2003.clear();

		}
		void CStyles::readBin(const CPath& oPath)
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if (xlsb)
			{
				XLSB::StylesStreamPtr stylesStream(new XLSB::StylesStream);

				xlsb->ReadBin(oPath, stylesStream.get());

				if (stylesStream != nullptr)
				{
					if (stylesStream->m_FMTS != nullptr)
						m_oNumFmts = static_cast<XLSB::FMTS*>(stylesStream->m_FMTS.get())->m_arFmt;

					if (stylesStream->m_FONTS != nullptr)
						m_oFonts = static_cast<XLSB::FONTS*>(stylesStream->m_FONTS.get())->m_arBrtFont;

					if (stylesStream->m_FILLS != nullptr)
						m_oFills = static_cast<XLSB::FILLS*>(stylesStream->m_FILLS.get())->m_arBrtFill;

					if (stylesStream->m_BORDERS != nullptr)
						m_oBorders = static_cast<XLSB::BORDERS*>(stylesStream->m_BORDERS.get())->m_arBrtBorder;

					if (stylesStream->m_CELLSTYLEXFS != nullptr)
						m_oCellStyleXfs = static_cast<XLSB::CELLSTYLEXFS*>(stylesStream->m_CELLSTYLEXFS.get())->m_arBrtXF;

					if (stylesStream->m_CELLXFS != nullptr)
						m_oCellXfs = static_cast<XLSB::CELLXFS*>(stylesStream->m_CELLXFS.get())->m_arBrtXF;

					if (stylesStream->m_STYLES != nullptr)
						m_oCellStyles = static_cast<XLSB::STYLES*>(stylesStream->m_STYLES.get())->m_arBrtStyle;

					if (stylesStream->m_DXFS != nullptr)
						m_oDxfs = static_cast<XLSB::DXFS*>(stylesStream->m_DXFS.get())->m_aruDXF;

					if (stylesStream->m_TABLESTYLES != nullptr)
						m_oTableStyles = stylesStream->m_TABLESTYLES;

					if (stylesStream->m_COLORPALETTE != nullptr)
						m_oColors = stylesStream->m_COLORPALETTE;

					if (stylesStream->m_FRTSTYLESHEET != nullptr)
						m_oExtLst = stylesStream->m_FRTSTYLESHEET;

					AfterRead();
				}

				//stylesStream.reset();

			}
		}
		void CStyles::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CStyles::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CStyles::toXML() const
		{
			return _T("");
		}
		void CStyles::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read(oRootPath, oPath);

			if (m_oReadPath.GetExtention() == _T(".bin"))
			{
				readBin(m_oReadPath);
				return;
			}

			XmlUtils::CXmlLiteReader oReader;

			if (!oReader.FromFile(oPath.GetPath()))
				return;

			if (!oReader.ReadNextNode())
				return;

			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if (_T("styleSheet") == sName)
			{
				fromXML(oReader);
			}
		}
		void CStyles::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_oNumFmts.IsInit())
				m_oNumFmts->toXML(writer);
			if (m_oFonts.IsInit())
				m_oFonts->toXML(writer);
			if (m_oFills.IsInit())
				m_oFills->toXML(writer);
			if (m_oBorders.IsInit())
				m_oBorders->toXML(writer);
			if (m_oCellStyleXfs.IsInit())
				m_oCellStyleXfs->toXML(writer);
			if (m_oCellXfs.IsInit())
				m_oCellXfs->toXML(writer);
			if (m_oCellStyles.IsInit())
				m_oCellStyles->toXML(writer);
			if (m_oColors.IsInit())
				m_oColors->toXML(writer);
			if (m_oDxfs.IsInit())
				m_oDxfs->toXML(writer);
			if (m_oTableStyles.IsInit())
				m_oTableStyles->toXML(writer);
			if (m_oExtLst.IsInit())
				writer.WriteString(m_oExtLst->toXMLWithNS(L""));

		}
		void CStyles::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode()) return;

			int nStylesDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nStylesDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"borders" == sName)
					m_oBorders = oReader;
				else if (_T("cellStyles") == sName)
					m_oCellStyles = oReader;
				else if (L"cellStyleXfs" == sName)
					m_oCellStyleXfs = oReader;
				else if (L"cellXfs" == sName)
					m_oCellXfs = oReader;
				else if (L"colors" == sName)
					m_oColors = oReader;
				else if (L"dxfs" == sName)
					m_oDxfs = oReader;
				//else if ( _T("extLst") == sName )
				//	pItem = new CSi( oReader );
				else if (L"fills" == sName)
					m_oFills = oReader;
				else if (L"fonts" == sName)
					m_oFonts = oReader;
				else if (L"numFmts" == sName)
					m_oNumFmts = oReader;
				else if (L"tableStyles" == sName)
					m_oTableStyles = oReader;
				else if (L"Style" == sName)
				{
					CStyle2003 *style = new CStyle2003(WritingElement::m_pMainDocument);
					style->fromXML(oReader);

					if (	((style->m_sName.IsInit())	&& (style->m_sName == L"Normal"	|| style->m_sName == L"Default"))
						||	((style->m_sId.IsInit())	&& (style->m_sId == L"Normal"	|| style->m_sId == L"Default")))
					{
						m_nStyleNormal2003 = m_arrStyles2003.size();
					}
					m_arrStyles2003.push_back(style);
				}
				else if (L"extLst" == sName)
					m_oExtLst = oReader;
			}
			AfterRead();
		}
		void CStyles::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<styleSheet \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\" \
mc:Ignorable=\"x14ac x16r2\" \
xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\" \
xmlns:x16r2=\"http://schemas.microsoft.com/office/spreadsheetml/2015/02/main\">");

			toXML(sXml);

			sXml.WriteString(L"</styleSheet>");

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath.c_str(), sXml.GetData());

			oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
		}
		void CStyles::AfterRead()
		{
			if (m_arrStyles2003.empty()) return;

			if (!m_oCellStyles.IsInit()) m_oCellStyles.Init();
			if (!m_oCellStyleXfs.IsInit()) m_oCellStyleXfs.Init();
			if (!m_oCellXfs.IsInit()) m_oCellXfs.Init();

			if (!m_oBorders.IsInit()) m_oBorders.Init();
			if (!m_oFills.IsInit()) m_oFills.Init();
			if (!m_oFonts.IsInit()) m_oFonts.Init();
			if (!m_oNumFmts.IsInit()) m_oNumFmts.Init();

			m_oFills->m_arrItems.push_back(new CFill);
			m_oFills->m_arrItems.back()->m_oPatternFill.Init();
			m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType.Init();
			m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeNone);

			m_oFills->m_arrItems.push_back(new CFill);
			m_oFills->m_arrItems.back()->m_oPatternFill.Init();
			m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType.Init();
			m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeGray125);

			if (m_nStyleNormal2003 != 0xffffffff)
			{
				ConvertStyle2003(m_arrStyles2003[m_nStyleNormal2003], true);
			}
			else
			{
				m_oBorders->m_arrItems.push_back(new CBorder);
				m_oBorders->m_arrItems.back()->m_oBottom.Init(); m_oBorders->m_arrItems.back()->m_oTop.Init();
				m_oBorders->m_arrItems.back()->m_oStart.Init(); m_oBorders->m_arrItems.back()->m_oEnd.Init();

				m_oFonts->m_arrItems.push_back(new CFont);
				m_oFonts->m_arrItems.back()->m_oSz.Init(); m_oFonts->m_arrItems.back()->m_oSz->m_oVal.Init();
				m_oFonts->m_arrItems.back()->m_oSz->m_oVal->SetValue(11);

				m_oFonts->m_arrItems.back()->m_oRFont.Init();
				m_oFonts->m_arrItems.back()->m_oRFont->m_sVal = L"Calibri";

				CXfs *pStyleXfs = new CXfs();
				int iXfs = m_oCellStyleXfs->m_arrItems.size();
				pStyleXfs->m_oNumFmtId.Init(); pStyleXfs->m_oNumFmtId->SetValue(0);
				pStyleXfs->m_oFontId.Init(); pStyleXfs->m_oFontId->SetValue(0);
				m_oCellStyleXfs->m_arrItems.push_back(pStyleXfs);

				CCellStyle *cell_style = new CCellStyle();
				cell_style->m_oXfId = iXfs;
				cell_style->m_oName = L"Normal";

				m_oCellStyles->m_arrItems.push_back(cell_style);
				
				CXfs *cell_xfs = new CXfs();
				cell_xfs->m_oXfId = iXfs;

				m_oCellXfs->m_arrItems.push_back(cell_xfs);

			}
			//--------------------------------------------------------------------------------------------
			for (size_t i = 0; i < m_arrStyles2003.size(); ++i)
			{
				if (m_arrStyles2003[i]->m_sId.IsInit() == false) continue;

				ConvertStyle2003(m_arrStyles2003[i]);
			}
		}
		void CStyles::ConvertStyle2003(CStyle2003 *style2003, bool bDefault)
		{
			if (!style2003) return;
			if (style2003->bUsed) return;

			style2003->bUsed = true;
			if (style2003->m_sParentId.IsInit())
			{
				std::map<std::wstring, size_t>::iterator pFind = m_mapStyles2003.find(*style2003->m_sParentId);
				if (pFind != m_mapStyles2003.end())
				{
					MergeStyles2003(style2003, m_arrStyles2003[pFind->second]);
				}
			}

			CXfs *pStyleXfs = new CXfs();

			if (style2003->m_oBorder.IsInit())
			{
				nullable<OOX::Spreadsheet::CBorder> borderCopy = style2003->m_oBorder;
				int index = m_oBorders->m_arrItems.size();
				m_oBorders->m_arrItems.push_back(borderCopy.GetPointerEmptyNullable());
				m_oBorders->m_mapBorders.insert(std::make_pair(index, m_oBorders->m_arrItems.back()));

				pStyleXfs->m_oBorderId = index;
				pStyleXfs->m_oApplyBorder.Init();
				pStyleXfs->m_oApplyBorder->FromBool(true);
			}
			if ((style2003->m_oFill.IsInit())/* &&
											 (style2003->m_oFill->m_oPatternFill.IsInit())*/)
			{
				nullable<OOX::Spreadsheet::CFill> fillCopy = style2003->m_oFill;
				int index = m_oFills->m_arrItems.size();
				m_oFills->m_arrItems.push_back(fillCopy.GetPointerEmptyNullable());
				m_oFills->m_mapFills.insert(std::make_pair(index, m_oFills->m_arrItems.back()));

				pStyleXfs->m_oFillId = index;
				pStyleXfs->m_oApplyFill.Init();
				pStyleXfs->m_oApplyFill->FromBool(true);
			}
			if (style2003->m_oFont.IsInit() || bDefault)
			{
				int index = m_oFonts->m_arrItems.size();

				if (style2003->m_oFont.IsInit())
				{
					nullable<OOX::Spreadsheet::CFont> fontCopy = style2003->m_oFont;
					m_oFonts->m_arrItems.push_back(fontCopy.GetPointerEmptyNullable());
					m_oFonts->m_mapFonts.insert(std::make_pair(index, m_oFonts->m_arrItems.back()));
				}
				else
				{
					m_oFonts->m_arrItems.push_back(new CFont);
				}

				if (false == m_oFonts->m_arrItems.back()->m_oSz.IsInit())
				{
					m_oFonts->m_arrItems.back()->m_oSz.Init(); m_oFonts->m_arrItems.back()->m_oSz->m_oVal.Init();
					m_oFonts->m_arrItems.back()->m_oSz->m_oVal->SetValue(10);
				}
				if (false == m_oFonts->m_arrItems.back()->m_oRFont.IsInit())
				{
					m_oFonts->m_arrItems.back()->m_oRFont.Init();
					m_oFonts->m_arrItems.back()->m_oRFont->m_sVal = L"Arial";
				}

				pStyleXfs->m_oFontId = index;
				pStyleXfs->m_oApplyFont.Init();
				pStyleXfs->m_oApplyFont->FromBool(true);
			}

			if (style2003->m_oNumFmt.IsInit())
			{
				int index = 0;
				if (style2003->m_oNumFmt->m_oFormatCode.IsInit())
				{
					if (*style2003->m_oNumFmt->m_oFormatCode == L"General" ||
						*style2003->m_oNumFmt->m_oFormatCode == L"@")
						index = 0;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"General Number")
						index = 0;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"0")
						index = 0;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"0.0")
						index = 0;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"0.00")
						index = 2;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"General Date")
						index = 22;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Long Date")
						index = 15;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Medium Date")
						index = 14;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Short Date")
						index = 16;
					else if (std::wstring::npos !=
						style2003->m_oNumFmt->m_oFormatCode->find(L"d/m/yy"))
						index = 16;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Long Time")
						index = 21;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Medium Time")
						index = 20;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Short Time")
						index = 20;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Fixed")
						index = 2;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Standard")
						index = 0;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Percent")
						index = 10;
					else if (*style2003->m_oNumFmt->m_oFormatCode == L"Scientific")
						index = 11;
					//Yes/No, True/False, or On/Off
					else
					{
						std::map<std::wstring, int>::iterator pFind =
							m_oNumFmts->m_mapFormatCode.find(*style2003->m_oNumFmt->m_oFormatCode);

						if (pFind == m_oNumFmts->m_mapFormatCode.end())
						{
							index = 168 + m_oNumFmts->m_arrItems.size();

							style2003->m_oNumFmt->m_oNumFmtId = index;

							m_oNumFmts->m_mapFormatCode.insert(std::make_pair(*style2003->m_oNumFmt->m_oFormatCode, index));

							nullable<OOX::Spreadsheet::CNumFmt> numFmtCopy = style2003->m_oNumFmt;
							m_oNumFmts->m_arrItems.push_back(numFmtCopy.GetPointerEmptyNullable());

							style2003->m_oNumFmt.Init(); //repair
						}
						else
						{
							index = pFind->second;
						}
					}
					pStyleXfs->m_oApplyNumberFormat.Init();
					pStyleXfs->m_oApplyNumberFormat->FromBool(true);
				}
				pStyleXfs->m_oNumFmtId = index;
			}
			if (style2003->m_oAligment.IsInit())
			{
				pStyleXfs->m_oAligment = style2003->m_oAligment;
				pStyleXfs->m_oApplyAlignment.Init();
				pStyleXfs->m_oApplyAlignment->FromBool(true);
			}
			CXfs *pCellXfs = new CXfs(*pStyleXfs);

			int iXfs = m_oCellStyleXfs->m_arrItems.size();
			m_oCellStyleXfs->m_arrItems.push_back(pStyleXfs);

			pCellXfs->m_oXfId = iXfs;
			m_oCellXfs->m_arrItems.push_back(pCellXfs);

			if (style2003->m_sName.IsInit())
			{
				CCellStyle *cell_style = new CCellStyle();
				cell_style->m_oXfId = iXfs;
				cell_style->m_oName = style2003->m_sName;

				m_oCellStyles->m_arrItems.push_back(cell_style);
			}
			if (style2003->m_sId.IsInit())
			{
				m_mapStyles2003.insert(std::make_pair(*style2003->m_sId, iXfs));
			}
			if (style2003->bStyleContinuous)
				m_mapStylesContinues2003.insert(std::make_pair(m_oCellXfs->m_arrItems.size() - 1, true));
		}
		void CStyles::MergeStyles2003(CStyle2003 *style2003, CStyle2003 *parent)
		{
			if (!style2003 || !parent) return;

			if (false == style2003->m_oNumFmt.IsInit())
			{
				style2003->m_oNumFmt = parent->m_oNumFmt;
			}
			if (false == style2003->m_oBorder.IsInit())
			{
				style2003->m_oBorder = parent->m_oBorder;
			}
			if (false == style2003->m_oFill.IsInit())
			{
				style2003->m_oFill = parent->m_oFill;
			}
			if (false == style2003->m_oFont.IsInit())
			{
				style2003->m_oFont = parent->m_oFont;
			}
			if (false == style2003->m_oAligment.IsInit())
			{
				style2003->m_oAligment = parent->m_oAligment;
			}
			//---------------------------------------
			if (parent->m_sParentId.IsInit())
			{
				std::map<std::wstring, size_t>::iterator pFind = m_mapStyles2003.find(*parent->m_sParentId);
				if (pFind != m_mapStyles2003.end())
				{
					MergeStyles2003(style2003, m_arrStyles2003[pFind->second]);
				}
			}
		}
		void CStyles::PrepareToWrite()
		{
			//fonts
			if (false == m_oFonts.IsInit())
			{
				m_oFonts.Init();
				OOX::Spreadsheet::CFont* pFont = new OOX::Spreadsheet::CFont();
				pFont->m_oSz.Init();
				pFont->m_oSz->m_oVal.Init();
				pFont->m_oSz->m_oVal->SetValue(11);
				pFont->m_oColor.Init();
				pFont->m_oColor->m_oThemeColor.Init();
				pFont->m_oColor->m_oThemeColor->SetValue(SimpleTypes::Spreadsheet::themecolorDark1);
				pFont->m_oRFont.Init();
				pFont->m_oRFont->m_sVal = L"Calibri";
				pFont->m_oFamily.Init();
				pFont->m_oFamily->m_oFontFamily.Init();
				pFont->m_oFamily->m_oFontFamily->SetValue(SimpleTypes::Spreadsheet::fontfamilySwiss);
				pFont->m_oScheme.Init();
				pFont->m_oScheme->m_oFontScheme.Init();
				pFont->m_oScheme->m_oFontScheme->SetValue(SimpleTypes::Spreadsheet::fontschemeMinor);
				m_oFonts->m_arrItems.push_back(pFont);
			}
			if (false == m_oFonts->m_oCount.IsInit())
			{
				m_oFonts->m_oCount.Init();
				m_oFonts->m_oCount->SetValue((unsigned int)m_oFonts->m_arrItems.size());
			}
			//fills
			if (false == m_oFills.IsInit())
			{
				m_oFills.Init();
				OOX::Spreadsheet::CFill* pFill1 = new OOX::Spreadsheet::CFill();
				pFill1->m_oPatternFill.Init();
				pFill1->m_oPatternFill->m_oPatternType.Init();
				pFill1->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeNone);
				OOX::Spreadsheet::CFill* pFill2 = new OOX::Spreadsheet::CFill();
				pFill2->m_oPatternFill.Init();
				pFill2->m_oPatternFill->m_oPatternType.Init();
				pFill2->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeGray125);
				m_oFills->m_arrItems.push_back(pFill1);
				m_oFills->m_arrItems.push_back(pFill2);
			}
			if (false == m_oFills->m_oCount.IsInit())
			{
				m_oFills->m_oCount.Init();
				m_oFills->m_oCount->SetValue((unsigned int)m_oFills->m_arrItems.size());
			}
			//borders
			if (false == m_oBorders.IsInit())
			{
				m_oBorders.Init();
				OOX::Spreadsheet::CBorder* pBorder = new OOX::Spreadsheet::CBorder();
				pBorder->m_oStart.Init();
				pBorder->m_oEnd.Init();
				pBorder->m_oTop.Init();
				pBorder->m_oBottom.Init();
				pBorder->m_oDiagonal.Init();
				m_oBorders->m_arrItems.push_back(pBorder);
			}
			if (false == m_oBorders->m_oCount.IsInit())
			{
				m_oBorders->m_oCount.Init();
				m_oBorders->m_oCount->SetValue((unsigned int)m_oBorders->m_arrItems.size());
			}
			//cellXfs
			if (m_oCellXfs.IsInit())
			{
				for (size_t i = 0; i < m_oCellXfs->m_arrItems.size(); ++i)
				{
					CXfs* xfs = m_oCellXfs->m_arrItems[i];

					if ((xfs) && (false == xfs->m_oXfId.IsInit()))
					{
						xfs->m_oXfId.Init();
						xfs->m_oXfId->SetValue(0);
					}

				}
			}
			//cellStyles
			if (false == m_oCellStyles.IsInit())
				m_oCellStyles.Init();

			if (m_oCellStyles->m_arrItems.empty())
			{
				CCellStyle* pCellStyle = new CCellStyle();
				pCellStyle->m_oName = _T("Normal");
				pCellStyle->m_oXfId.Init();
				pCellStyle->m_oXfId->SetValue(0);
				pCellStyle->m_oBuiltinId.Init();
				pCellStyle->m_oBuiltinId->SetValue(0);

				m_oCellStyles->m_arrItems.push_back(pCellStyle);
			}
			if (false == m_oCellStyles->m_oCount.IsInit())
			{
				m_oCellStyles->m_oCount.Init();
				m_oCellStyles->m_oCount->SetValue((unsigned int)m_oCellStyles->m_arrItems.size());
			}
			//cellStyleXfs
			if (false == m_oCellStyleXfs.IsInit())
				m_oCellStyleXfs.Init();

			if (0 == m_oCellStyleXfs->m_arrItems.size())
			{
				CXfs* pXfs = new CXfs();
				pXfs->m_oNumFmtId.Init();
				pXfs->m_oNumFmtId->SetValue(0);
				pXfs->m_oFontId.Init();
				pXfs->m_oFontId->SetValue(0);
				pXfs->m_oFillId.Init();
				pXfs->m_oFillId->SetValue(0);
				pXfs->m_oBorderId.Init();
				pXfs->m_oBorderId->SetValue(0);
				m_oCellStyleXfs->m_arrItems.push_back(pXfs);
			}
			if (false == m_oCellStyleXfs->m_oCount.IsInit())
			{
				m_oCellStyleXfs->m_oCount.Init();
				m_oCellStyleXfs->m_oCount->SetValue((unsigned int)m_oCellStyleXfs->m_arrItems.size());
			}
			//dxfs
			if (false == m_oDxfs.IsInit())
				m_oDxfs.Init();
			if (false == m_oDxfs->m_oCount.IsInit())
			{
				m_oDxfs->m_oCount.Init();
				m_oDxfs->m_oCount->SetValue(0);
			}
			//tableStyles
			if (false == m_oTableStyles.IsInit())
			{
				m_oTableStyles.Init();
				m_oTableStyles->m_oCount.Init();
				m_oTableStyles->m_oCount->SetValue(0);
			}
			if (false == m_oTableStyles->m_oDefaultPivotStyle.IsInit())
				m_oTableStyles->m_oDefaultTableStyle = _T("TableStyleMedium2");
			if (false == m_oTableStyles->m_oDefaultPivotStyle.IsInit())
				m_oTableStyles->m_oDefaultPivotStyle = _T("PivotStyleLight16");
		}
		const OOX::FileType CStyles::type() const
		{
			return OOX::Spreadsheet::FileTypes::Styles;
		}
		const CPath CStyles::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CStyles::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CStyles::GetReadPath()
		{
			return m_oReadPath;
		}

	} //Spreadsheet
} // namespace OOX

