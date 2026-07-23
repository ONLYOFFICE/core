/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "BookViews.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/GlobalsSubstream.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Window1.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

namespace OOX
{
	namespace Spreadsheet
	{
			CWorkbookView::CWorkbookView()
			{
			}
			CWorkbookView::~CWorkbookView()
			{
			}
			void CWorkbookView::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CWorkbookView::toXML() const
			{
				return _T("");
			}
			void CWorkbookView::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<workbookView"));
				WritingStringNullableAttrInt(L"xWindow", m_oXWindow, m_oXWindow->GetValue());
				WritingStringNullableAttrInt(L"yWindow", m_oYWindow, m_oYWindow->GetValue());
				WritingStringNullableAttrInt(L"windowWidth", m_oWindowWidth, m_oWindowWidth->GetValue());
				WritingStringNullableAttrInt(L"windowHeight", m_oWindowHeight, m_oWindowHeight->GetValue());
				WritingStringNullableAttrInt(L"activeTab", m_oActiveTab, m_oActiveTab->GetValue());
				WritingStringNullableAttrInt(L"firstSheet", m_oFirstSheet, m_oFirstSheet->GetValue());
				WritingStringNullableAttrInt(L"tabRatio", m_oTabRatio, m_oTabRatio->GetValue());
				WritingStringNullableAttrBool(L"autoFilterDateGrouping", m_oAutoFilterDateGrouping);
				WritingStringNullableAttrBool(L"showSheetTabs", m_oShowSheetTabs);
				WritingStringNullableAttrBool(L"minimized", m_oMinimized);
				WritingStringNullableAttrBool(L"showHorizontalScroll", m_oShowHorizontalScroll);
				WritingStringNullableAttrBool(L"showVerticalScroll", m_oShowVerticalScroll);
				WritingStringNullableAttrString(L"visibility", m_oVisibility, m_oVisibility->ToString());
				writer.WriteString(_T("/>"));
			}
			void CWorkbookView::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CWorkbookView::fromBin(XLS::BaseObjectPtr& obj)
			{
				ReadAttributes(obj);
			}
			XLS::BaseObjectPtr CWorkbookView::toBin()
			{
                auto ptr(new XLSB::BookView);
				XLS::BaseObjectPtr objectPtr(ptr);

                if (m_oActiveTab.IsInit())
                {
                    ptr->itabCur = m_oActiveTab->GetValue();
                }
                else
                {
                    ptr->itabCur = 0;
                }

                if (m_oAutoFilterDateGrouping.IsInit())
                    ptr->fNoAFDateGroup = m_oAutoFilterDateGrouping->GetValue();
				else
					ptr->fNoAFDateGroup = false;
                if (m_oFirstSheet.IsInit())
                    ptr->itabFirst = m_oFirstSheet->GetValue();
                else
                    ptr->itabFirst = 0;
                if (m_oMinimized.IsInit())
                    ptr->fIconic = m_oMinimized->GetValue();
                else
                    ptr->fIconic = false;
                if (m_oShowHorizontalScroll.IsInit())
                    ptr->fDspHScroll = m_oShowHorizontalScroll->GetValue();
                if (m_oShowSheetTabs.IsInit())
                    ptr->fBotAdornment = m_oShowSheetTabs->GetValue();
                if (m_oShowVerticalScroll.IsInit())
                    ptr->fDspVScroll = m_oShowVerticalScroll->GetValue();
                if (m_oTabRatio.IsInit())
                    ptr->wTabRatio = m_oTabRatio->GetValue();
                else
                    ptr->wTabRatio = 600;
                if (m_oWindowHeight.IsInit())
                    ptr->dyWn = m_oWindowHeight->GetValue();
                else
                    ptr->dyWn = 12750;
                if (m_oWindowWidth.IsInit())
                    ptr->dxWn = m_oWindowWidth->GetValue();
                else
                    ptr->dxWn = 21240;
                if (m_oXWindow.IsInit())
                    ptr->xWn = m_oXWindow->GetValue() * 6;
                else
                    ptr->xWn = 2280;
                if (m_oYWindow.IsInit())
                    ptr->yWn = m_oYWindow->GetValue() * 110;
                else
                    ptr->yWn = 1650;

                if (m_oVisibility == SimpleTypes::Spreadsheet::EVisibleType::visibleHidden)
                {
                    ptr->fHidden = true;
                    ptr->fVeryHidden = false;
                }
                else if (m_oVisibility == SimpleTypes::Spreadsheet::EVisibleType::visibleVeryHidden)
                {
                    ptr->fHidden = false;
                    ptr->fVeryHidden = true;
                }
                else
                {
                    ptr->fHidden = false;
                    ptr->fVeryHidden = false;
                }

				return objectPtr;
			}
			XLS::BaseObjectPtr CWorkbookView::toXLS()
			{
				auto ptr = new XLS::Window1;
				if (m_oActiveTab.IsInit())
				{
					ptr->itabCur = m_oActiveTab->GetValue();
				}
				else
				{
					ptr->itabCur = 0;
				}

				if (m_oAutoFilterDateGrouping.IsInit())
					ptr->fNoAFDateGroup = m_oAutoFilterDateGrouping->GetValue();
				else
					ptr->fNoAFDateGroup = false;
				if (m_oFirstSheet.IsInit())
					ptr->itabFirst = m_oFirstSheet->GetValue();
				else
					ptr->itabFirst = 0;
				if (m_oMinimized.IsInit())
					ptr->fIconic = m_oMinimized->GetValue();
				else
					ptr->fIconic = false;
				if (m_oShowHorizontalScroll.IsInit())
					ptr->fDspHScroll = m_oShowHorizontalScroll->GetValue();
				if (m_oShowSheetTabs.IsInit())
					ptr->fBotAdornment = m_oShowSheetTabs->GetValue();
				if (m_oShowVerticalScroll.IsInit())
					ptr->fDspVScroll = m_oShowVerticalScroll->GetValue();
				if (m_oTabRatio.IsInit())
					ptr->wTabRatio = m_oTabRatio->GetValue();
				else
					ptr->wTabRatio = 600;
				if (m_oWindowHeight.IsInit())
					ptr->dyWn = m_oWindowHeight->GetValue();
				else
					ptr->dyWn = 12750;
				if (m_oWindowWidth.IsInit())
					ptr->dxWn = m_oWindowWidth->GetValue();
				else
					ptr->dxWn = 21240;
				if (m_oXWindow.IsInit())
					ptr->xWn = m_oXWindow->GetValue() * 6;
				else
					ptr->xWn = 2280;
				if (m_oYWindow.IsInit())
					ptr->yWn = m_oYWindow->GetValue() * 110;
				else
					ptr->yWn = 1650;

				if (m_oVisibility == SimpleTypes::Spreadsheet::EVisibleType::visibleHidden)
				{
					ptr->fHidden = true;
					ptr->fVeryHidden = false;
				}
				else if (m_oVisibility == SimpleTypes::Spreadsheet::EVisibleType::visibleVeryHidden)
				{
					ptr->fHidden = false;
					ptr->fVeryHidden = true;
				}
				else
				{
					ptr->fHidden = false;
					ptr->fVeryHidden = false;
				}
				return XLS::BaseObjectPtr(ptr);
			}
			EElementType CWorkbookView::getType () const
			{
				return et_x_WorkbookView;
			}
			void CWorkbookView::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("activeTab"), m_oActiveTab )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("autoFilterDateGrouping"), m_oAutoFilterDateGrouping )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("firstSheet"), m_oFirstSheet )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("minimized"), m_oMinimized )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showHorizontalScroll"), m_oShowHorizontalScroll )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showSheetTabs"), m_oShowSheetTabs )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showVerticalScroll"), m_oShowVerticalScroll )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("tabRatio"), m_oTabRatio )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("visibility"), m_oVisibility )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("windowHeight"), m_oWindowHeight )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("windowWidth"), m_oWindowWidth )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("xWindow"), m_oXWindow )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("yWindow"), m_oYWindow )
				WritingElement_ReadAttributes_End( oReader )
			}
			void CWorkbookView::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::BookView*>(obj.get());
				m_oActiveTab                = ptr->itabCur;
				m_oAutoFilterDateGrouping   = ptr->fNoAFDateGroup;
				m_oFirstSheet               = ptr->itabFirst;
				m_oMinimized                = ptr->fIconic;
				m_oShowHorizontalScroll     = ptr->fDspHScroll;
				m_oShowSheetTabs            = ptr->fBotAdornment;
				m_oShowVerticalScroll       = ptr->fDspVScroll;
				m_oTabRatio                 = ptr->wTabRatio;
				m_oWindowHeight             = ptr->dyWn;
				m_oWindowWidth              = ptr->dxWn;
				m_oXWindow                  = (int)ptr->xWn;
				m_oYWindow                  = (int)ptr->yWn;

				if(ptr->fHidden)
					m_oVisibility = SimpleTypes::Spreadsheet::EVisibleType::visibleHidden;
				else if(ptr->fVeryHidden)
					m_oVisibility = SimpleTypes::Spreadsheet::EVisibleType::visibleVeryHidden;
				else if(!ptr->fHidden && !ptr->fVeryHidden)
					m_oVisibility = SimpleTypes::Spreadsheet::EVisibleType::visibleVisible;

			}

			CBookViews::CBookViews()
			{
			}
			CBookViews::~CBookViews()
			{
			}
			void CBookViews::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CBookViews::toXML() const
			{
				return _T("");
			}
			void CBookViews::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<bookViews>"));

				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (  m_arrItems[i] )
					{
						m_arrItems[i]->toXML(writer);
					}
				}

				writer.WriteString(_T("</bookViews>"));
			}
			void CBookViews::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("workbookView") == sName )
					{
						CWorkbookView* pWorkbookView = new CWorkbookView();
						*pWorkbookView = oReader;
						m_arrItems.push_back(pWorkbookView);
					}
				}
			}
			void CBookViews::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
			{
				//ReadAttributes(obj);

				if (obj.empty())
					return;

				for(auto &workbookView : obj)
				{
					m_arrItems.push_back(new CWorkbookView(workbookView));
				}
			}
			std::vector<XLS::BaseObjectPtr> CBookViews::toBin()
			{
				std::vector<XLS::BaseObjectPtr> ptrVector{};
				for(auto i:m_arrItems)
					ptrVector.push_back(i->toBin());

				return ptrVector;
			}
			void CBookViews::toXLS(XLS::BaseObjectPtr substreamPtr)
			{
				auto Substream = static_cast<XLS::GlobalsSubstream*>(substreamPtr.get());
				for(auto i:m_arrItems)
					Substream->m_arWindow1.push_back(i->toXLS());
			}
			EElementType CBookViews::getType () const
			{
				return et_x_BookViews;
			}
			void CBookViews::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
