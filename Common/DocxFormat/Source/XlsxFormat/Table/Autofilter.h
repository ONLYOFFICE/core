/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
			WritingElementSpreadsheet_AdditionConstructors(CSortCondition)
			CSortCondition()
			{
			}
			virtual ~CSortCondition()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oDescending.IsInit() || m_oRef.IsInit() || m_oSortBy.IsInit() || m_oDxfId.IsInit())
				{
					writer.WriteString(CString(_T("<sortCondition")));
					if(m_oSortBy.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" sortBy=\"%ls\""), m_oSortBy->ToString());
						writer.WriteString(sXml);
					}
					if(m_oDescending.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" descending=\"%ls\""), m_oDescending->ToString2(SimpleTypes::onofftostring1));
						writer.WriteString(sXml);
					}
					if(m_oRef.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" ref=\"%ls\""), XmlUtils::EncodeXmlString(m_oRef->GetValue()));
						writer.WriteString(sXml);
					}
					if(m_oDxfId.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" dxfId=\"%d\""), m_oDxfId->GetValue());
						writer.WriteString(sXml);
					}
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_SortCondition;
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
			WritingElementSpreadsheet_AdditionConstructors(CSortState)
			CSortState()
			{
			}
			virtual ~CSortState()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oRef.IsInit() && m_arrItems.size() > 0)
				{
					CString sXml;
					sXml.Format(_T("<sortState ref=\"%ls\""), XmlUtils::EncodeXmlString(m_oRef->GetValue()));
					if(m_oCaseSensitive.IsInit())
                        sXml.AppendFormat(_T(" caseSensitive=\"%ls\""), (const TCHAR *) m_oCaseSensitive->ToString2(SimpleTypes::onofftostring1));
					sXml.Append(_T(">"));
					writer.WriteString(sXml);

					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);

					writer.WriteString(CString(_T("</sortState>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("sortCondition") == sName )
						m_arrItems.push_back(new CSortCondition(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_SortState;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("caseSensitive"),      m_oCaseSensitive )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<SimpleTypes::COnOff<> > m_oCaseSensitive;
		};

		//Filters
		class CColorFilter : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CColorFilter)
			CColorFilter()
			{
			}
			virtual ~CColorFilter()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oDxfId.IsInit())
				{
					CString sXml;
					sXml.Format(_T("<colorFilter dxfId=\"%d\""), m_oDxfId->GetValue());
					writer.WriteString(sXml);
					if(m_oCellColor.IsInit() && false == m_oCellColor->ToBool())
						writer.WriteString(CString(_T(" cellColor=\"0\"")));
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_ColorFilter;
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
			WritingElementSpreadsheet_AdditionConstructors(CDynamicFilter)
			CDynamicFilter()
			{
			}
			virtual ~CDynamicFilter()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oType.IsInit())
				{
					CString sXml;
					sXml.Format(_T("<dynamicFilter type=\"%ls\""), m_oType->ToString());
					writer.WriteString(sXml);
					if(m_oVal.IsInit())
					{
						CString sVal;
						sVal.Format(_T(" val=\"%ls\""), OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(m_oVal->GetValue()));
						writer.WriteString(sVal);
					}
					if(m_oMaxVal.IsInit())
					{
						CString sVal;
						sVal.Format(_T(" maxVal=\"%ls\""), OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(m_oMaxVal->GetValue()));
						writer.WriteString(sVal);
					}
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_DynamicFilter;
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
			WritingElementSpreadsheet_AdditionConstructors(CCustomFilter)
			CCustomFilter()
			{
			}
			virtual ~CCustomFilter()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oOperator.IsInit() && m_oVal.IsInit())
				{
					CString sXml;
					sXml.Format(_T("<customFilter operator=\"%ls\" val=\"%ls\"/>"), m_oOperator->ToString(), m_oVal.get());
					writer.WriteString(sXml);
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_CustomFilters;
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
			nullable<CString > m_oVal;
		};
		class CCustomFilters : public WritingElementWithChilds<CCustomFilter>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCustomFilters)
			CCustomFilters()
			{
			}
			virtual ~CCustomFilters()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_arrItems.size() > 0)
				{
					writer.WriteString(CString(_T("<customFilters")));
					if(m_oAnd.IsInit() && true == m_oAnd->ToBool())
						writer.WriteString(CString(_T(" and=\"1\"")));
					writer.WriteString(CString(_T(">")));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(CString(_T("</customFilters>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("customFilter") == sName )
						m_arrItems.push_back( new CCustomFilter(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_CustomFilters;
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
			WritingElementSpreadsheet_AdditionConstructors(CFilter)
			CFilter()
			{
			}
			virtual ~CFilter()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oVal.IsInit())
				{
					CString sXml;
					sXml.Format(_T("<filter val=\"%ls\"/>"), XmlUtils::EncodeXmlString(m_oVal.get()));
					writer.WriteString(sXml);
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_Filter;
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
			nullable<CString > m_oVal;
		};
		class CDateGroupItem : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CDateGroupItem)
			CDateGroupItem()
			{
			}
			virtual ~CDateGroupItem()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oDateTimeGrouping.IsInit())
				{
					writer.WriteString(CString(_T("<dateGroupItem")));
					if(m_oYear.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" year=\"%d\""), m_oYear->GetValue());
						writer.WriteString(sXml);
					}
					if(m_oMonth.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" month=\"%d\""), m_oMonth->GetValue());
						writer.WriteString(sXml);
					}
					if(m_oDay.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" day=\"%d\""), m_oDay->GetValue());
						writer.WriteString(sXml);
					}
					if(m_oHour.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" hour=\"%d\""), m_oHour->GetValue());
						writer.WriteString(sXml);
					}
					if(m_oMinute.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" minute=\"%d\""), m_oMinute->GetValue());
						writer.WriteString(sXml);
					}
					if(m_oSecond.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" second=\"%d\""), m_oSecond->GetValue());
						writer.WriteString(sXml);
					}
					if(m_oDateTimeGrouping.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" dateTimeGrouping=\"%ls\""), m_oDateTimeGrouping->ToString());
						writer.WriteString(sXml);
					}
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_DateGroupItem;
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
			WritingElementSpreadsheet_AdditionConstructors(CFilters)
			CFilters()
			{
			}
			virtual ~CFilters()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_arrItems.size() > 0 || m_oBlank.IsInit())
				{
					writer.WriteString(_T("<filters"));
					if(m_oBlank.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" blank=\"%ls\""), m_oBlank->ToString2(SimpleTypes::onofftostring1));
						writer.WriteString(sXml);
					}
					writer.WriteString(_T(">"));

					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);

					writer.WriteString(_T("</filters>"));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("dateGroupItem") == sName )
						m_arrItems.push_back( new CDateGroupItem(oReader));
					if ( _T("filter") == sName )
						m_arrItems.push_back( new CFilter(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_Filters;
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
			WritingElementSpreadsheet_AdditionConstructors(CTop10)
			CTop10()
			{
			}
			virtual ~CTop10()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oVal.IsInit())
				{
					writer.WriteString(CString(_T("<top10")));
					if(m_oTop.IsInit() && false == m_oTop->ToBool())
						writer.WriteString(CString(_T(" top=\"0\"")));
					if(m_oPercent.IsInit() && true == m_oPercent->ToBool())
						writer.WriteString(CString(_T(" percent=\"1\"")));
					if(m_oVal.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" val=\"%ls\""), OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(m_oVal->GetValue()));
						writer.WriteString(sXml);
					}
					if(m_oFilterVal.IsInit())
					{
						CString sXml;
						sXml.Format(_T(" filterVal=\"%ls\""), OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(m_oFilterVal->GetValue()));
						writer.WriteString(sXml);
					}
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_ColorFilter;
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
			WritingElementSpreadsheet_AdditionConstructors(CFilterColumn)
			CFilterColumn()
			{
			}
			virtual ~CFilterColumn()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oColId.IsInit() && (m_oColorFilter.IsInit() || m_oDynamicFilter.IsInit() || m_oCustomFilters.IsInit() || m_oFilters.IsInit() ||
										m_oTop10.IsInit() || m_oShowButton.IsInit() || m_oHiddenButton.IsInit()))
				{
					CString sXml;
					sXml.Format(_T("<filterColumn colId=\"%d\""), m_oColId->GetValue());
					writer.WriteString(sXml);
					if(m_oShowButton.IsInit() && false == m_oShowButton->ToBool())
						writer.WriteString(CString(_T(" showButton=\"0\"")));
					if(m_oHiddenButton.IsInit())
					{
						CString sXml;sXml.Format(_T(" hiddenButton=\"%ls\""), m_oHiddenButton->ToString2(SimpleTypes::onofftostring1));
						writer.WriteString(sXml);
					}
					writer.WriteString(CString(_T(">")));
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
					writer.WriteString(CString(_T("</filterColumn>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

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
				return et_FilterColumn;
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
			WritingElementSpreadsheet_AdditionConstructors(CAutofilter)
			CAutofilter()
			{
			}
			virtual ~CAutofilter()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oRef.IsInit())
				{
					CString sXml;
					sXml.Format(_T("<autoFilter ref=\"%ls\">"), XmlUtils::EncodeXmlString(m_oRef->GetValue()));
					writer.WriteString(sXml);
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					if(m_oSortState.IsInit())
						m_oSortState->toXML(writer);
					writer.WriteString(CString(_T("</autoFilter>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("filterColumn") == sName )
						m_arrItems.push_back(new CFilterColumn(oReader));
					else if ( _T("sortState") == sName )
						m_oSortState = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_Autofilter;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;

			nullable<CSortState > m_oSortState;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_AUTOFILTER_FILE_INCLUDE_H_
