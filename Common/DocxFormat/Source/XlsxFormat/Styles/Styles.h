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

#include "../Xlsx.h"
#include "../../XlsbFormat/Xlsb.h"
#include "../XlsxFlat.h"
#include "../CommonInclude.h"

#include "Borders.h"
#include "CellStyles.h"
#include "Xfs.h"
#include "Colors.h"
#include "dxf.h"
#include "Fills.h"
#include "Fonts.h"
#include "NumFmts.h"
#include "TableStyles.h"


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
		class CStyle2003 : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CStyle2003)
			CStyle2003(OOX::Document* pMain) : WritingElement(pMain)
			{
			}
			virtual ~CStyle2003()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nDocumentDepth = oReader.GetDepth();
				std::wstring sName;

				while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
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
					else if ( L"Alignment" == sName )
						m_oAligment = oReader;
					else if ( L"Font" == sName )
						m_oFont = oReader;
					else if ( L"Interior" == sName )
						m_oFill = oReader;
					else if ( L"NumberFormat" == sName )
						m_oNumFmt = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Style2003;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"ss:ID",		m_sId )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"ss:Name",	m_sName )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"ss:Parent",	m_sParentId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable_string		m_sName;
			nullable_string		m_sId;
			nullable_string		m_sParentId;

            nullable<OOX::Spreadsheet::CBorder>		m_oBorder;
			nullable<OOX::Spreadsheet::CFill>		m_oFill;
			nullable<OOX::Spreadsheet::CFont>		m_oFont;
			nullable<OOX::Spreadsheet::CNumFmt>		m_oNumFmt;
			nullable<OOX::Spreadsheet::CAligment>	m_oAligment;

			bool bStyleContinuous = false;
		};

		//необработанные child:
		//<extLst>
		class CStyles : public OOX::File, public OOX::IFileContainer, public WritingElement
		{
		public:
			CStyles(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain), WritingElement(pMain)
			{
				m_bSpreadsheets = true;
				
				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if (xlsx)
				{
					xlsx->m_pStyles = this;
				}
			}
			CStyles(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain), WritingElement(pMain)
			{
				m_bSpreadsheets = true;

				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if (xlsx)
				{
					xlsx->m_pStyles = this;
				}
				read( oRootPath, oPath );
			}
			virtual ~CStyles()
			{
				for (size_t i = 0; i < m_arrStyles2003.size(); i++)
				{
					if (m_arrStyles2003[i]) delete m_arrStyles2003[i]; m_arrStyles2003[i] = NULL;
				}
				m_arrStyles2003.clear();
			}

            void readBin(const CPath& oPath)
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

			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

                if( m_oReadPath.GetExtention() == _T(".bin"))
                {
                    readBin(m_oReadPath);
                    return;
                }

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( _T("styleSheet") == sName )
				{
					fromXML(oReader);
				}
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oNumFmts.IsInit())
					m_oNumFmts->toXML(writer);
				if(m_oFonts.IsInit())
					m_oFonts->toXML(writer);
				if(m_oFills.IsInit())
					m_oFills->toXML(writer);
				if(m_oBorders.IsInit())
					m_oBorders->toXML(writer);
				if(m_oCellStyleXfs.IsInit())
					m_oCellStyleXfs->toXML(writer);
				if(m_oCellXfs.IsInit())
					m_oCellXfs->toXML(writer);
				if(m_oCellStyles.IsInit())
					m_oCellStyles->toXML(writer);
				if(m_oColors.IsInit())
					m_oColors->toXML(writer);
				if(m_oDxfs.IsInit())
					m_oDxfs->toXML(writer);
				if(m_oTableStyles.IsInit())
					m_oTableStyles->toXML(writer);
				if(m_oExtLst.IsInit())
					writer.WriteString(m_oExtLst->toXMLWithNS(L""));

			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() ) return;

				int nStylesDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"borders" == sName )
						m_oBorders = oReader;
					else if ( _T("cellStyles") == sName )
						m_oCellStyles = oReader;
					else if ( L"cellStyleXfs" == sName )
						m_oCellStyleXfs = oReader;
					else if ( L"cellXfs" == sName )
						m_oCellXfs = oReader;
					else if ( L"colors" == sName )
						m_oColors = oReader;
					else if ( L"dxfs" == sName )
						m_oDxfs = oReader;
					//else if ( _T("extLst") == sName )
					//	pItem = new CSi( oReader );
					else if ( L"fills" == sName )
						m_oFills = oReader;
					else if ( L"fonts" == sName )
						m_oFonts = oReader;
					else if ( L"numFmts" == sName )
						m_oNumFmts = oReader;
					else if ( L"tableStyles" == sName )
						m_oTableStyles = oReader;
					else if (L"Style" == sName)
					{
						CStyle2003 *style = new CStyle2003(WritingElement::m_pMainDocument);
						style->fromXML(oReader);

						m_arrStyles2003.push_back( style);
					}
					else if (L"extLst" == sName)
						m_oExtLst = oReader;
				}
				AfterRead();
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
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

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			}
			void AfterRead()
			{
				if (m_arrStyles2003.empty()) return;

				m_oBorders.Init();
				m_oCellStyles.Init();
				m_oCellStyleXfs.Init();
				m_oCellXfs.Init();
				m_oFills.Init();
				m_oFonts.Init();
				m_oNumFmts.Init();

				m_oFills->m_arrItems.push_back(new CFill);
				m_oFills->m_arrItems.back()->m_oPatternFill.Init();
				m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType.Init();
				m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeNone);

				m_oFills->m_arrItems.push_back(new CFill);
				m_oFills->m_arrItems.back()->m_oPatternFill.Init();
				m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType.Init();
				m_oFills->m_arrItems.back()->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeGray125);

				for (size_t i = 0; i < m_arrStyles2003.size(); ++i)
				{
					if (m_arrStyles2003[i]->m_sId.IsInit() == false) continue;
					
					CXfs *pStyleXfs = new CXfs();

					if (m_arrStyles2003[i]->m_oBorder.IsInit())
					{
						int index = m_oBorders->m_arrItems.size();
						m_oBorders->m_arrItems.push_back(m_arrStyles2003[i]->m_oBorder.GetPointerEmptyNullable());
						m_oBorders->m_mapBorders.insert(std::make_pair(index, m_oBorders->m_arrItems.back()));
						
						pStyleXfs->m_oBorderId = index;
						pStyleXfs->m_oApplyBorder.Init();
						pStyleXfs->m_oApplyBorder->FromBool(true);
					}
					if ((m_arrStyles2003[i]->m_oFill.IsInit())/* &&
						(m_arrStyles2003[i]->m_oFill->m_oPatternFill.IsInit())*/)
					{
						int index = m_oFills->m_arrItems.size();
						m_oFills->m_arrItems.push_back(m_arrStyles2003[i]->m_oFill.GetPointerEmptyNullable());
						m_oFills->m_mapFills.insert(std::make_pair(index, m_oFills->m_arrItems.back()));
						
						pStyleXfs->m_oFillId = index;
						pStyleXfs->m_oApplyFill.Init();
						pStyleXfs->m_oApplyFill->FromBool(true);
					}
					if (m_arrStyles2003[i]->m_oFont.IsInit())
					{
						int index = m_oFonts->m_arrItems.size();
						m_oFonts->m_arrItems.push_back(m_arrStyles2003[i]->m_oFont.GetPointerEmptyNullable());
						m_oFonts->m_mapFonts.insert(std::make_pair(index, m_oFonts->m_arrItems.back()));
						
						pStyleXfs->m_oFontId = index;
						pStyleXfs->m_oApplyFont.Init();
						pStyleXfs->m_oApplyFont->FromBool(true);
					}
					if (m_arrStyles2003[i]->m_oNumFmt.IsInit())
					{
						int index = 0;
						if (m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode.IsInit())
						{
								if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"General"|| 
									*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"@")
								index = 0;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"General Number")
								index = 0;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"0")
								index = 0;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"0.0")
								index = 0;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"0.00")
								index = 2;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"General Date")
								index = 22;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Long Date")
								index = 15;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Medium Date")
								index = 14;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Short Date")
								index = 16;
							else if (std::wstring::npos != 
								m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode->find(L"d/m/yy"))
								index = 16;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Long Time")
								index = 21;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Medium Time")
								index = 20;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Short Time")
								index = 20;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Fixed")
								index = 2;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Standard")
								index = 0;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Percent")
								index = 10;
							else if (*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode == L"Scientific")
								index = 11;
							//Yes/No, True/False, or On/Off
							else
							{
								std::map<std::wstring, int>::iterator pFind = 
									m_oNumFmts->m_mapFormatCode.find(*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode);

								if (pFind == m_oNumFmts->m_mapFormatCode.end())
								{
									index = 168 + m_oNumFmts->m_arrItems.size();	
									
									m_arrStyles2003[i]->m_oNumFmt->m_oNumFmtId = index;

									m_oNumFmts->m_mapFormatCode.insert(std::make_pair(*m_arrStyles2003[i]->m_oNumFmt->m_oFormatCode, index));
									m_oNumFmts->m_arrItems.push_back(m_arrStyles2003[i]->m_oNumFmt.GetPointerEmptyNullable());

									m_arrStyles2003[i]->m_oNumFmt.Init(); //repair
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
					if (m_arrStyles2003[i]->m_oAligment.IsInit())
					{
						pStyleXfs->m_oAligment = m_arrStyles2003[i]->m_oAligment;
						pStyleXfs->m_oApplyAlignment.Init();
						pStyleXfs->m_oApplyAlignment->FromBool(true);
					}
					CXfs *pCellXfs = new CXfs(*pStyleXfs);
					
					std::map<std::wstring, size_t>::iterator pFind = m_mapStyles2003.end();
					if (m_arrStyles2003[i]->m_sParentId.IsInit())
					{
						pFind = m_mapStyles2003.find(*m_arrStyles2003[i]->m_sParentId);
					}
					if (pFind != m_mapStyles2003.end())
					{
						pCellXfs->m_oXfId = m_oCellXfs->m_arrItems[pFind->second]->m_oXfId->GetValue();
						
						m_oCellXfs->m_arrItems.push_back(pCellXfs);
					}
					else
					{
						int iXfs = m_oCellStyleXfs->m_arrItems.size();
						m_oCellStyleXfs->m_arrItems.push_back(pStyleXfs);

						pCellXfs->m_oXfId = iXfs;
						m_oCellXfs->m_arrItems.push_back(pCellXfs);
						
						if (m_arrStyles2003[i]->m_sName.IsInit())
						{
							CCellStyle *cell_style = new CCellStyle();
							cell_style->m_oXfId = iXfs;
							cell_style->m_oName = m_arrStyles2003[i]->m_sName;

							m_oCellStyles->m_arrItems.push_back(cell_style);
						}			
					}
					m_mapStyles2003.insert(std::make_pair(*m_arrStyles2003[i]->m_sId, m_oCellXfs->m_arrItems.size() - 1));

					if (m_arrStyles2003[i]->bStyleContinuous)
						m_mapStylesContinues2003.insert(std::make_pair(m_oCellXfs->m_arrItems.size() - 1, true));
				}
			}
			void PrepareToWrite()
			{
				//fonts
				if(false == m_oFonts.IsInit())
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
				if(false == m_oFonts->m_oCount.IsInit())
				{
					m_oFonts->m_oCount.Init();
					m_oFonts->m_oCount->SetValue((unsigned int)m_oFonts->m_arrItems.size());
				}
				//fills
				if(false == m_oFills.IsInit())
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
				if(false == m_oFills->m_oCount.IsInit())
				{
					m_oFills->m_oCount.Init();
					m_oFills->m_oCount->SetValue((unsigned int)m_oFills->m_arrItems.size());
				}
				//borders
				if(false == m_oBorders.IsInit())
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
				if(false == m_oBorders->m_oCount.IsInit())
				{
					m_oBorders->m_oCount.Init();
					m_oBorders->m_oCount->SetValue((unsigned int)m_oBorders->m_arrItems.size());
				}
			//cellXfs
				if(m_oCellXfs.IsInit())
				{
                    for ( size_t i = 0; i < m_oCellXfs->m_arrItems.size(); ++i)
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
				if(false == m_oCellStyles.IsInit())
					m_oCellStyles.Init();

				if(m_oCellStyles->m_arrItems.empty())
				{
					CCellStyle* pCellStyle = new CCellStyle();
						pCellStyle->m_oName = _T("Normal");
						pCellStyle->m_oXfId.Init();
						pCellStyle->m_oXfId->SetValue(0);
						pCellStyle->m_oBuiltinId.Init();
						pCellStyle->m_oBuiltinId->SetValue(0);
					
					m_oCellStyles->m_arrItems.push_back(pCellStyle);
				}
				if(false == m_oCellStyles->m_oCount.IsInit())
				{
					m_oCellStyles->m_oCount.Init();
					m_oCellStyles->m_oCount->SetValue((unsigned int)m_oCellStyles->m_arrItems.size());
				}
				//cellStyleXfs
				if(false == m_oCellStyleXfs.IsInit())
					m_oCellStyleXfs.Init();

				if(0 == m_oCellStyleXfs->m_arrItems.size())
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
				if(false == m_oCellStyleXfs->m_oCount.IsInit())
				{
					m_oCellStyleXfs->m_oCount.Init();
					m_oCellStyleXfs->m_oCount->SetValue((unsigned int)m_oCellStyleXfs->m_arrItems.size());
				}
				//dxfs
				if(false == m_oDxfs.IsInit())
					m_oDxfs.Init();
				if(false == m_oDxfs->m_oCount.IsInit())
				{
					m_oDxfs->m_oCount.Init();
					m_oDxfs->m_oCount->SetValue(0);
				}
				//tableStyles
				if(false == m_oTableStyles.IsInit())
				{
					m_oTableStyles.Init();
					m_oTableStyles->m_oCount.Init();
					m_oTableStyles->m_oCount->SetValue(0);
				}
				if(false == m_oTableStyles->m_oDefaultPivotStyle.IsInit())
					m_oTableStyles->m_oDefaultTableStyle = _T("TableStyleMedium2");
				if(false == m_oTableStyles->m_oDefaultPivotStyle.IsInit())
					m_oTableStyles->m_oDefaultPivotStyle = _T("PivotStyleLight16");
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Styles;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}

		public:

			CPath										m_oReadPath;

			nullable<OOX::Spreadsheet::CBorders>		m_oBorders;
			nullable<OOX::Spreadsheet::CCellStyles>		m_oCellStyles;
			nullable<OOX::Spreadsheet::CCellStyleXfs>	m_oCellStyleXfs;
			nullable<OOX::Spreadsheet::CCellXfs>		m_oCellXfs;
			nullable<OOX::Spreadsheet::CColors>			m_oColors;
			nullable<OOX::Spreadsheet::CDxfs>			m_oDxfs;
			nullable<OOX::Spreadsheet::CFills>			m_oFills;
			nullable<OOX::Spreadsheet::CFonts>			m_oFonts;
			nullable<OOX::Spreadsheet::CNumFmts>		m_oNumFmts;
			nullable<OOX::Spreadsheet::CTableStyles>	m_oTableStyles;

			nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;

			std::vector<CStyle2003*>		m_arrStyles2003;
			std::map<std::wstring, size_t>	m_mapStyles2003;
			std::map<unsigned int, bool>	m_mapStylesContinues2003;
		};
	} //Spreadsheet
} // namespace OOX

