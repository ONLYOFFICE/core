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
#ifndef OOX_AUTOFILTER_FILE_INCLUDE_H_
#define OOX_AUTOFILTER_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//Sort
		class CSortCondition : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSortCondition)
			CSortCondition()
			{
			}
			virtual ~CSortCondition()
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
				if(m_oDescending.IsInit() || m_oRef.IsInit() || m_oSortBy.IsInit() || m_oDxfId.IsInit())
				{
					writer.WriteString(L"<sortCondition");
					WritingStringNullableAttrString(L"sortBy", m_oSortBy, m_oSortBy->ToString());
					WritingStringNullableAttrBool(L"descending", m_oDescending);
					WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef->ToString());
					WritingStringNullableAttrInt(L"dxfId", m_oDxfId, m_oDxfId->GetValue());
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_SortCondition;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("descending"),      m_oDescending )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sortBy"),      m_oSortBy )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dxfId"),      m_oDxfId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<> > m_oDescending;
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<SimpleTypes::Spreadsheet::CSortBy<> > m_oSortBy;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oDxfId;
		};
		class CSortState : public WritingElementWithChilds<CSortCondition>
		{
		public:
			WritingElement_AdditionConstructors(CSortState)
			CSortState()
			{
			}
			virtual ~CSortState()
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
				if(m_oRef.IsInit() && !m_arrItems.empty())
				{
					writer.WriteString(L"<sortState");
					WritingStringAttrEncodeXmlString(L"ref", m_oRef->ToString());
					WritingStringNullableAttrBool(L"caseSensitive", m_oCaseSensitive);
					WritingStringNullableAttrBool(L"columnSort", m_oColumnSort);
					WritingStringNullableAttrString(L"sortMethod", m_oSortMethod, m_oSortMethod->ToString());
					writer.WriteString(L">");

                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }

					writer.WriteString(L"</sortState>");
				}
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

					if ( _T("sortCondition") == sName )
						m_arrItems.push_back(new CSortCondition(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_SortState;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("caseSensitive"),	m_oCaseSensitive )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("columnSort"),		m_oColumnSort )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sortMethod"),		m_oSortMethod )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<SimpleTypes::COnOff<> > m_oCaseSensitive;
			nullable<SimpleTypes::COnOff<> > m_oColumnSort;
			nullable<SimpleTypes::Spreadsheet::CSortMethod<> > m_oSortMethod;
		};

		//Filters
		class CColorFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorFilter)
			CColorFilter()
			{
			}
			virtual ~CColorFilter()
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
				if(m_oDxfId.IsInit())
				{
					writer.WriteString(L"<colorFilter");
					WritingStringAttrInt(L"dxfId", m_oDxfId->GetValue());
					if(m_oCellColor.IsInit() && false == m_oCellColor->ToBool())
						writer.WriteString(L" cellColor=\"0\"");
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_ColorFilter;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cellColor"),      m_oCellColor )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dxfId"),      m_oDxfId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<> > m_oCellColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oDxfId;
		};
		class CDynamicFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDynamicFilter)
			CDynamicFilter()
			{
			}
			virtual ~CDynamicFilter()
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
				if(m_oType.IsInit())
				{
					writer.WriteString(L"<dynamicFilter");
					WritingStringAttrString(L"type", m_oType->ToString());
					WritingStringNullableAttrDouble(L"val", m_oVal, m_oVal->GetValue());
					WritingStringNullableAttrDouble(L"maxVal", m_oMaxVal, m_oMaxVal->GetValue());
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_DynamicFilter;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("maxVal"),      m_oMaxVal )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CDynamicFilterType<> > m_oType;
			nullable<SimpleTypes::CDouble > m_oVal;
			nullable<SimpleTypes::CDouble > m_oMaxVal;
		};
		class CCustomFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomFilter)
			CCustomFilter()
			{
			}
			virtual ~CCustomFilter()
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
				if(m_oOperator.IsInit() && m_oVal.IsInit())
				{
					writer.WriteString(L"<customFilter operator=\"");
					writer.WriteString(m_oOperator->ToString());
					writer.WriteString(L"\" val=\"");
					writer.WriteString(m_oVal.get());
					writer.WriteString(L"\"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_CustomFilters;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("operator"),      m_oOperator )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CCustomFilter<> > m_oOperator;
			nullable<std::wstring > m_oVal;
		};
		class CCustomFilters : public WritingElementWithChilds<CCustomFilter>
		{
		public:
			WritingElement_AdditionConstructors(CCustomFilters)
			CCustomFilters()
			{
			}
			virtual ~CCustomFilters()
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
				if(m_arrItems.empty()) return;
				
				writer.WriteString(L"<customFilters");
				if (m_oAnd.IsInit() && true == m_oAnd->ToBool())
					writer.WriteString(L" and=\"1\"");
				writer.WriteString(L">");
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString(L"</customFilters>");
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

					if ( _T("customFilter") == sName )
						m_arrItems.push_back( new CCustomFilter(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_CustomFilters;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("and"),      m_oAnd )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<> > m_oAnd;
		};
		class CFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFilter)
			CFilter()
			{
			}
			virtual ~CFilter()
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
				if(m_oVal.IsInit())
				{
					WritingStringValAttrEncodeXmlString(L"filter", m_oVal.get());
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Filter;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring > m_oVal;
		};
		class CDateGroupItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDateGroupItem)
			CDateGroupItem()
			{
			}
			virtual ~CDateGroupItem()
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
				if(m_oDateTimeGrouping.IsInit())
				{
					writer.WriteString(L"<dateGroupItem");
					WritingStringNullableAttrInt(L"year", m_oYear, m_oYear->GetValue());
					WritingStringNullableAttrInt(L"month", m_oMonth, m_oMonth->GetValue());
					WritingStringNullableAttrInt(L"day", m_oDay, m_oDay->GetValue());
					WritingStringNullableAttrInt(L"hour", m_oHour, m_oHour->GetValue());
					WritingStringNullableAttrInt(L"minute", m_oMinute, m_oMinute->GetValue());
					WritingStringNullableAttrInt(L"second", m_oSecond, m_oSecond->GetValue());
					WritingStringNullableAttrString(L"dateTimeGrouping", m_oDateTimeGrouping, m_oDateTimeGrouping->ToString());
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_DateGroupItem;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dateTimeGrouping"),      m_oDateTimeGrouping )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("day"),      m_oDay )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hour"),      m_oHour )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("minute"),      m_oMinute )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("month"),      m_oMonth )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("second"),      m_oSecond )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("year"),      m_oYear )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CDateTimeGroup<> > m_oDateTimeGrouping;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oDay;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oHour;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oMinute;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oMonth;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oSecond;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oYear;
		};
		class CFilters : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CFilters)
			CFilters()
			{
			}
			virtual ~CFilters()
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
				if(!m_arrItems.empty() || m_oBlank.IsInit())
				{
					writer.WriteString(_T("<filters"));
					WritingStringNullableAttrBool(L"blank", m_oBlank);
					writer.WriteString(_T(">"));

                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }

					writer.WriteString(_T("</filters>"));
				}
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

					if ( _T("dateGroupItem") == sName )
						m_arrItems.push_back( new CDateGroupItem(oReader));
					if ( _T("filter") == sName )
						m_arrItems.push_back( new CFilter(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Filters;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("blank"),      m_oBlank )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<> > m_oBlank;
		};
		class CTop10 : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTop10)
			CTop10()
			{
			}
			virtual ~CTop10()
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
				if(m_oVal.IsInit())
				{
					writer.WriteString(L"<top10");
					if(m_oTop.IsInit() && false == m_oTop->ToBool())
						writer.WriteString(L" top=\"0\"");
					if(m_oPercent.IsInit() && true == m_oPercent->ToBool())
						writer.WriteString(L" percent=\"1\"");
					WritingStringNullableAttrDouble(L"val", m_oVal, m_oVal->GetValue());
					WritingStringNullableAttrDouble(L"filterVal", m_oFilterVal, m_oFilterVal->GetValue());
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_ColorFilter;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("filterVal"),      m_oFilterVal )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("percent"),      m_oPercent )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("top"),      m_oTop )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CDouble > m_oFilterVal;
			nullable<SimpleTypes::COnOff<> > m_oPercent;
			nullable<SimpleTypes::COnOff<> > m_oTop;
			nullable<SimpleTypes::CDouble > m_oVal;
		};
		class CFilterColumn : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFilterColumn)
			CFilterColumn()
			{
			}
			virtual ~CFilterColumn()
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
				if(m_oColId.IsInit() && (m_oColorFilter.IsInit() || m_oDynamicFilter.IsInit() || m_oCustomFilters.IsInit() || m_oFilters.IsInit() ||
										m_oTop10.IsInit() || m_oShowButton.IsInit() || m_oHiddenButton.IsInit()))
				{
					writer.WriteString(L"<filterColumn");
					WritingStringNullableAttrInt(L"colId", m_oColId, m_oColId->GetValue());
					if(m_oShowButton.IsInit() && false == m_oShowButton->ToBool())
						writer.WriteString(L" showButton=\"0\"");
					WritingStringNullableAttrBool(L"hiddenButton", m_oHiddenButton);
					writer.WriteString(L">");
					if(m_oColorFilter.IsInit())
						m_oColorFilter->toXML(writer);
					if(m_oDynamicFilter.IsInit())
						m_oDynamicFilter->toXML(writer);
					if(m_oCustomFilters.IsInit())
						m_oCustomFilters->toXML(writer);
					if(m_oFilters.IsInit())
						m_oFilters->toXML(writer);
					if(m_oTop10.IsInit())
						m_oTop10->toXML(writer);
					writer.WriteString(L"</filterColumn>");
				}
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

					if ( _T("colorFilter") == sName )
						m_oColorFilter = oReader;
					else if ( _T("dynamicFilter") == sName )
						m_oDynamicFilter = oReader;
					else if ( _T("customFilters") == sName )
						m_oCustomFilters = oReader;
					else if ( _T("filters") == sName )
						m_oFilters = oReader;
					else if ( _T("top10") == sName )
						m_oTop10 = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_FilterColumn;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("colId"),      m_oColId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hiddenButton"),      m_oHiddenButton )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showButton"),      m_oShowButton )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oColId;
			nullable<SimpleTypes::COnOff<> > m_oHiddenButton;
			nullable<SimpleTypes::COnOff<> > m_oShowButton;

			nullable<CColorFilter > m_oColorFilter;
			nullable<CDynamicFilter > m_oDynamicFilter;
			nullable<CCustomFilters > m_oCustomFilters;
			nullable<CFilters > m_oFilters;
			nullable<CTop10 > m_oTop10;
		};
		class CAutofilter : public WritingElementWithChilds<CFilterColumn>
		{
		public:
			WritingElement_AdditionConstructors(CAutofilter)
			CAutofilter()
			{
			}
			virtual ~CAutofilter()
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
				if(m_oRef.IsInit())
				{
					writer.WriteString(L"<autoFilter");
					WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef->ToString());
					writer.WriteString(L">");
					
                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }
					
					if(m_oSortState.IsInit())
						m_oSortState->toXML(writer);
					writer.WriteString(L"</autoFilter>");
				}
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

					if ( _T("filterColumn") == sName )
						m_arrItems.push_back(new CFilterColumn(oReader));
					else if ( _T("sortState") == sName )
						m_oSortState = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Autofilter;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("ref"),      m_oRef )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<CSortState >					m_oSortState;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_AUTOFILTER_FILE_INCLUDE_H_
