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
#ifndef OOX_BOOKVIEWS_FILE_INCLUDE_H_
#define OOX_BOOKVIEWS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CWorkbookView : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWorkbookView)
            WritingElement_XlsbConstructors(CWorkbookView)
			CWorkbookView()
			{
            }
			virtual ~CWorkbookView()
			{
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
				writer.WriteString(_T("<workbookView"));
				WritingStringNullableAttrInt(L"xWindow", m_oXWindow, m_oXWindow->GetValue());
				WritingStringNullableAttrInt(L"yWindow", m_oYWindow, m_oYWindow->GetValue());
				WritingStringNullableAttrInt(L"windowWidth", m_oWindowWidth, m_oWindowWidth->GetValue());
				WritingStringNullableAttrInt(L"windowHeight", m_oWindowHeight, m_oWindowHeight->GetValue());
				WritingStringNullableAttrInt(L"activeTab", m_oActiveTab, m_oActiveTab->GetValue());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::BookView*>(obj.get());
				if (ptr != nullptr)
				{
					if (m_oActiveTab.IsInit())
						ptr->itabCur = m_oActiveTab->GetValue();
					else
						ptr->itabCur = 0;

					if (m_oAutoFilterDateGrouping.IsInit())
						ptr->fNoAFDateGroup = m_oAutoFilterDateGrouping->GetValue();
					else
						ptr->fNoAFDateGroup = true;

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
					else
						ptr->fDspHScroll = true;

					if (m_oShowVerticalScroll.IsInit())
						ptr->fDspVScroll = m_oShowVerticalScroll->GetValue();
					else
						ptr->fDspVScroll = true;

					if (m_oShowSheetTabs.IsInit())
						ptr->fBotAdornment = m_oShowSheetTabs->GetValue();
					else
						ptr->fBotAdornment = true;

					if (m_oTabRatio.IsInit())
						ptr->wTabRatio = m_oTabRatio->GetValue();
					else
						ptr->wTabRatio = 600;

					if (m_oWindowHeight.IsInit())
						ptr->dyWn = m_oWindowHeight->GetValue();
					else
						ptr->dyWn = 0;

					if (m_oWindowWidth.IsInit())
						ptr->dxWn = m_oWindowWidth->GetValue();
					else
						ptr->dxWn = 0;

					if (m_oXWindow.IsInit())
						ptr->xWn = m_oXWindow->GetValue();
					else
						ptr->xWn = 0;

					if (m_oYWindow.IsInit())
						ptr->yWn = m_oYWindow->GetValue();
					else
						ptr->yWn = 0;

					if (m_oVisibility.IsInit())
					{
						switch (m_oVisibility->GetValue())
						{
							case SimpleTypes::Spreadsheet::EVisibleType::visibleHidden:
							{
								ptr->fHidden = true;
								ptr->fVeryHidden = false;
							}
							break;
							case SimpleTypes::Spreadsheet::EVisibleType::visibleVeryHidden:
							{
								ptr->fHidden = true;
								ptr->fVeryHidden = true;
							}
							break;
							case SimpleTypes::Spreadsheet::EVisibleType::visibleVisible:
							{
								ptr->fHidden = false;
								ptr->fVeryHidden = false;
							}
							break;
						}
					}
					else
					{
						ptr->fHidden = false;
						ptr->fVeryHidden = false;
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_WorkbookView;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("activeTab"),      m_oActiveTab )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("autoFilterDateGrouping"),      m_oAutoFilterDateGrouping )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("firstSheet"),      m_oFirstSheet )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("minimized"),      m_oMinimized )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showHorizontalScroll"),      m_oShowHorizontalScroll )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showSheetTabs"),      m_oShowSheetTabs )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showVerticalScroll"),      m_oShowVerticalScroll )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("tabRatio"),      m_oTabRatio )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("visibility"),      m_oVisibility )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("windowHeight"),      m_oWindowHeight )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("windowWidth"),      m_oWindowWidth )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("xWindow"),      m_oXWindow )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("yWindow"),      m_oYWindow )

					WritingElement_ReadAttributes_End( oReader )
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BookView*>(obj.get());
				if (ptr != nullptr)
				{
					if (ptr->itabCur != 0)
						m_oActiveTab = ptr->itabCur;

					if (ptr->fNoAFDateGroup != true)
						m_oAutoFilterDateGrouping = ptr->fNoAFDateGroup;

					if (ptr->itabFirst != 0)
						m_oFirstSheet = ptr->itabFirst;
					
					if (ptr->fIconic != false)
						m_oMinimized = ptr->fIconic;
						
					if (ptr->fDspHScroll != true)
						m_oShowHorizontalScroll = ptr->fDspHScroll;

					if (ptr->fDspVScroll != true)
						m_oShowVerticalScroll = ptr->fDspVScroll;

					if (ptr->fBotAdornment != true)
						m_oShowSheetTabs = ptr->fBotAdornment;

					if (ptr->wTabRatio != 600)
						m_oTabRatio = ptr->wTabRatio;

					m_oWindowHeight = ptr->dyWn;
					m_oWindowWidth = ptr->dxWn;
					m_oXWindow = (int)ptr->xWn;
					m_oYWindow = (int)ptr->yWn;

					if (ptr->fHidden)
						m_oVisibility = SimpleTypes::Spreadsheet::EVisibleType::visibleHidden;
					else if (ptr->fVeryHidden)
						m_oVisibility = SimpleTypes::Spreadsheet::EVisibleType::visibleVeryHidden;
					else if (!ptr->fHidden && !ptr->fVeryHidden)
						m_oVisibility = SimpleTypes::Spreadsheet::EVisibleType::visibleVisible;
				}

            }


		public:
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oActiveTab;
				nullable<SimpleTypes::COnOff<>>						m_oAutoFilterDateGrouping;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oFirstSheet;
				nullable<SimpleTypes::COnOff<>>						m_oMinimized;
				nullable<SimpleTypes::COnOff<>>						m_oShowHorizontalScroll;
				nullable<SimpleTypes::COnOff<>>						m_oShowSheetTabs;
				nullable<SimpleTypes::COnOff<>>						m_oShowVerticalScroll;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTabRatio;
				nullable<SimpleTypes::Spreadsheet::CVisibleType<>>	m_oVisibility;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWindowHeight;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWindowWidth;
				nullable<SimpleTypes::CDecimalNumber<>>				m_oXWindow;
				nullable<SimpleTypes::CDecimalNumber<>>				m_oYWindow;
		};

		class CBookViews : public WritingElementWithChilds<CWorkbookView>
		{
		public:
			WritingElement_AdditionConstructors(CBookViews)
            WritingElement_XlsbVectorConstructors(CBookViews)
			CBookViews()
			{
			}            
			virtual ~CBookViews()
			{
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("workbookView") == sName )
						m_arrItems.push_back( new CWorkbookView( oReader ));
				}
			}

            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                //ReadAttributes(obj);

                if (obj.empty())
                    return;

                for(auto &workbookView : obj)
                {
                    m_arrItems.push_back(new CWorkbookView(workbookView));
                }
            }

			void toBin(std::vector<XLS::BaseObjectPtr>& obj)
			{
				obj.reserve(m_arrItems.size());
				for (size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (m_arrItems[i])
					{
						XLS::BaseObjectPtr item(new XLSB::BookView());
						m_arrItems[i]->toBin(item);
						obj.push_back(item);
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_BookViews;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_BOOKVIEWS_FILE_INCLUDE_H_
