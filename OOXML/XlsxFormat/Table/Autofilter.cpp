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

#include "Autofilter.h"

#include "../../XlsbFormat/Biff12_unions/SORTSTATE.h"
#include "../../XlsbFormat/Biff12_unions/SORTCONDS.h"
#include "../../XlsbFormat/Biff12_unions/SORTCOND.h"
#include "../../XlsbFormat/Biff12_records/BeginSortCond.h"
#include "../../XlsbFormat/Biff12_unions/ACSORTCONDS.h"
#include "../../XlsbFormat/Biff12_unions/SORTCOND14.h"
#include "../../XlsbFormat/Biff12_records/BeginSortCond14.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"

#include "../../XlsbFormat/Biff12_unions/AUTOFILTER.h"
#include "../../XlsbFormat/Biff12_records/BeginAFilter.h"
#include "../../XlsbFormat/Biff12_unions/FILTERCOLUMN.h"
#include "../../XlsbFormat/Biff12_records/BeginFilterColumn.h"
#include "../../XlsbFormat/Biff12_records/ColorFilter.h"
#include "../../XlsbFormat/Biff12_records/DynamicFilter.h"
#include "../../XlsbFormat/Biff12_unions/CUSTOMFILTERS.h"
#include "../../XlsbFormat/Biff12_records/BeginCustomFilters.h"
#include "../../XlsbFormat/Biff12_records/CustomFilter.h"
#include "../../XlsbFormat/Biff12_unions/FILTERS.h"
#include "../../XlsbFormat/Biff12_records/BeginFilters.h"
#include "../../XlsbFormat/Biff12_records/Filter.h"
#include "../../XlsbFormat/Biff12_records/AFilterDateGroupItem.h"
#include "../../XlsbFormat/Biff12_records/Top10Filter.h"
namespace OOX
{
	namespace Spreadsheet
	{
			CSortCondition::CSortCondition()
			{
			}
			CSortCondition::~CSortCondition()
			{
			}
			void CSortCondition::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CSortCondition::toXML() const
			{
				return _T("");
			}
			void CSortCondition::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"sortCondition", L"");
			}
			void CSortCondition::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oDescending.IsInit() || m_oRef.IsInit() || m_oSortBy.IsInit() || m_oDxfId.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringNullableAttrString(L"sortBy", m_oSortBy, m_oSortBy->ToString());
					WritingStringNullableAttrBool(L"descending", m_oDescending);
					WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef->ToString());
					WritingStringNullableAttrInt(L"dxfId", m_oDxfId, m_oDxfId->GetValue());
					writer.EndAttributesAndNode();
				}
			}
			void CSortCondition::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CSortCondition::fromBin(XLS::BaseObjectPtr& obj)
			{
				ReadAttributes(obj);
			}
			EElementType CSortCondition::getType () const
			{
				return et_x_SortCondition;
			}
			void CSortCondition::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("descending"),      m_oDescending )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sortBy"),      m_oSortBy )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dxfId"),      m_oDxfId )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CSortCondition::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptrRecord = static_cast<XLS::BiffRecord*>(obj.get());

							if(ptrRecord->getTypeId() == XLSB::rt_BeginSortCond)
							{
								auto ptr = static_cast<XLSB::BeginSortCond*>(obj.get());
								if(ptr != nullptr)
								{
									m_oDescending           = ptr->fSortDes;
									m_oRef                  = ptr->rfx.toString();
									switch (ptr->sortOn)
									{
										case 0:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyValue;
											break;
										case 1:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyCellColor;
											m_oDxfId  = ptr->condDataValue.condDataValue;
											break;
										case 2:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyFontColor;
											m_oDxfId  = ptr->condDataValue.condDataValue;
											break;
										case 3:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyIcon;
											//m_oDxfId  = ptr->cfflag;
											break;
									}

								}
							}
							else if(ptrRecord->getTypeId() == XLSB::rt_BeginSortCond14)
							{
								auto ptr = static_cast<XLSB::BeginSortCond14*>(obj.get());
								if(ptr != nullptr)
								{
									m_oDescending           = ptr->fSortDes;
									m_oRef                  = ptr->rfx.toString();
									switch (ptr->sortOn)
									{
										case 0:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyValue;
											break;
										case 1:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyCellColor;
											m_oDxfId  = ptr->condDataValue.condDataValue;
											break;
										case 2:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyFontColor;
											m_oDxfId  = ptr->condDataValue.condDataValue;
											break;
										case 3:
											m_oSortBy = SimpleTypes::Spreadsheet::ESortBy::sortbyIcon;
											//m_oDxfId  = ptr->cfflag;
											break;
									}
								}
							}

						}

			CSortState::CSortState()
			{
			}
			CSortState::~CSortState()
			{
			}
			void CSortState::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CSortState::toXML() const
			{
				return _T("");
			}
			void CSortState::toXML(NSStringUtils::CStringBuilder& writer) const
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
			void CSortState::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			void CSortState::fromBin(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::SORTSTATE*>(obj.get());

							ReadAttributes(ptr->m_BrtBeginSortState);

							auto ptrSORTCONDS = static_cast<XLSB::SORTCONDS*>(ptr->m_source.get());
							auto ptrACSORTCONDS = static_cast<XLSB::ACSORTCONDS*>(ptr->m_source.get());

							if(ptrSORTCONDS != nullptr || ptrACSORTCONDS != nullptr)
							{
								if(ptrSORTCONDS != nullptr)
									for(auto &pSORTCOND : ptrSORTCONDS->m_arSORTCOND)
										m_arrItems.push_back(new CSortCondition(static_cast<XLSB::SORTCOND*>(pSORTCOND.get())->m_BrtBeginSortCond));
								else if(ptrACSORTCONDS != nullptr)
									for(auto &pSORTCOND14 : ptrACSORTCONDS->m_arSORTCOND14)
										m_arrItems.push_back(new CSortCondition(static_cast<XLSB::SORTCOND14*>(pSORTCOND14.get())->m_BrtBeginSortCond14));
							}


						}
			EElementType CSortState::getType () const
			{
				return et_x_SortState;
			}
			void CSortState::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("caseSensitive"),	m_oCaseSensitive )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("columnSort"),		m_oColumnSort )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sortMethod"),		m_oSortMethod )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CSortState::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::BeginSortState*>(obj.get());
							if(ptr != nullptr)
							{
								m_oRef           = ptr->rfx;
								m_oCaseSensitive = ptr->fCaseSensitive;
								m_oColumnSort    = ptr->fCol;
								if(ptr->fAltMethod)
									m_oSortMethod = SimpleTypes::Spreadsheet::ESortMethod::sortmethodStroke;
								else
									m_oSortMethod = SimpleTypes::Spreadsheet::ESortMethod::sortmethodNone;
							}
						}

			CColorFilter::CColorFilter()
			{
			}
			CColorFilter::~CColorFilter()
			{
			}
			void CColorFilter::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CColorFilter::toXML() const
			{
				return _T("");
			}
			void CColorFilter::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"colorFilter", L"");
			}
			void CColorFilter::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oDxfId.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringAttrInt(L"dxfId", m_oDxfId->GetValue());
					if(m_oCellColor.IsInit() && false == m_oCellColor->ToBool())
						writer.WriteString(L" cellColor=\"0\"");
					writer.EndAttributesAndNode();
				}
			}
			void CColorFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CColorFilter::fromBin(XLS::BaseObjectPtr& obj)
						{
							ReadAttributes(obj);
						}
			EElementType CColorFilter::getType () const
			{
				return et_x_ColorFilter;
			}
			void CColorFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
							// Читаем атрибуты
							WritingElement_ReadAttributes_Start( oReader )

							WritingElement_ReadAttributes_Read_if     ( oReader, _T("cellColor"),      m_oCellColor )
							WritingElement_ReadAttributes_Read_if     ( oReader, _T("dxfId"),      m_oDxfId )

							WritingElement_ReadAttributes_End( oReader )
			}
			void CColorFilter::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::ColorFilter*>(obj.get());
							if(ptr != nullptr)
							{
							   m_oCellColor         = (bool)ptr->fCellColor;
							   m_oDxfId             = ptr->dxfid;
							}
						}

			CDynamicFilter::CDynamicFilter()
			{
			}
			CDynamicFilter::~CDynamicFilter()
			{
			}
			void CDynamicFilter::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CDynamicFilter::toXML() const
			{
				return _T("");
			}
			void CDynamicFilter::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"dynamicFilter", L"");
			}
			void CDynamicFilter::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oType.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringAttrString(L"type", m_oType->ToString());
					WritingStringNullableAttrDouble(L"val", m_oVal, m_oVal->GetValue());
					WritingStringNullableAttrDouble(L"maxVal", m_oMaxVal, m_oMaxVal->GetValue());
					writer.EndAttributesAndNode();
				}
			}
			void CDynamicFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CDynamicFilter::fromBin(XLS::BaseObjectPtr& obj)
						{
							ReadAttributes(obj);
						}
			EElementType CDynamicFilter::getType () const
			{
				return et_x_DynamicFilter;
			}
			void CDynamicFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("maxVal"),      m_oMaxVal )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CDynamicFilter::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::DynamicFilter*>(obj.get());
							if(ptr != nullptr)
							{
							   switch(ptr->cft)
							   {
								   case 0x00000000:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeNull; break;
								   case 0x00000001:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeAboveAverage; break;
								   case 0x00000002:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeBelowAverage; break;
								   case 0x00000008:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeTomorrow; break;
								   case 0x00000009:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeToday; break;
								   case 0x0000000A:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeYesterday; break;
								   case 0x0000000B:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeNextWeek; break;
								   case 0x0000000C:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeThisWeek; break;
								   case 0x0000000D:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeLastWeek; break;
								   case 0x0000000E:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeNextMonth; break;
								   case 0x0000000F:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeThisMonth; break;
								   case 0x00000010:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeLastMonth; break;
								   case 0x00000011:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeNextQuarter; break;
								   case 0x00000012:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeThisQuarter; break;
								   case 0x00000013:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeLastQuarter; break;
								   case 0x00000014:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeNextYear; break;
								   case 0x00000015:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeThisYear; break;
								   case 0x00000016:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeLastYear; break;
								   case 0x00000017:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeYearToDate; break;
								   case 0x00000018:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeQ1; break;
								   case 0x00000019:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeQ2; break;
								   case 0x0000001A:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeQ3; break;
								   case 0x0000001B:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeQ4; break;
								   case 0x0000001C:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM1; break;
								   case 0x0000001D:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM2; break;
								   case 0x0000001E:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM3; break;
								   case 0x0000001F:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM4; break;
								   case 0x00000020:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM5; break;
								   case 0x00000021:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM6; break;
								   case 0x00000022:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM7; break;
								   case 0x00000023:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM8; break;
								   case 0x00000024:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM9; break;
								   case 0x00000025:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM10; break;
								   case 0x00000026:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM11; break;
								   case 0x00000027:
									   m_oType = SimpleTypes::Spreadsheet::EDynamicFilterType::dynamicfiltertypeM12; break;
							   }

							   m_oVal    = ptr->xNumValue.data.value;
							   m_oMaxVal = ptr->xNumValueMax.data.value;
							}

						}

			CCustomFilter::CCustomFilter()
			{
			}
			CCustomFilter::~CCustomFilter()
			{
			}
			void CCustomFilter::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CCustomFilter::toXML() const
			{
				return _T("");
			}
			void CCustomFilter::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"customFilter", L"");
			}
			void CCustomFilter::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oOperator.IsInit() && m_oVal.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringAttrString(L"operator", m_oOperator->ToString());
					WritingStringAttrEncodeXmlString(L"val", m_oVal.get());
					writer.EndAttributesAndNode();
				}
			}
			void CCustomFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CCustomFilter::fromBin(XLS::BaseObjectPtr& obj)
						{
							ReadAttributes(obj);
						}
			EElementType CCustomFilter::getType () const
			{
				return et_x_CustomFilters;
			}
			void CCustomFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("operator"),      m_oOperator )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CCustomFilter::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::CustomFilter*>(obj.get());
							if(ptr != nullptr)
							{
								switch (ptr->grbitSgn)
								{
									case 0x01:
										m_oOperator = SimpleTypes::Spreadsheet::ECustomFilter::customfilterLessThan; break;
									case 0x02:
										m_oOperator = SimpleTypes::Spreadsheet::ECustomFilter::customfilterEqual; break;
									case 0x03:
										m_oOperator = SimpleTypes::Spreadsheet::ECustomFilter::customfilterLessThanOrEqual; break;
									case 0x04:
										m_oOperator = SimpleTypes::Spreadsheet::ECustomFilter::customfilterGreaterThan; break;
									case 0x05:
										m_oOperator = SimpleTypes::Spreadsheet::ECustomFilter::customfilterNotEqual; break;
									case 0x06:
										m_oOperator = SimpleTypes::Spreadsheet::ECustomFilter::customfilterGreaterThanOrEqual; break;
								}

								if(ptr->vts == 0x00000004)
									m_oVal = std::to_wstring(ptr->xdata.data.value);
								else if(ptr->vts == 0x00000008)
									m_oVal = ptr->bdata.toString();
								else if(ptr->vts == 0x00000006)
									m_oVal = ptr->vtsStringXls.value();

							}

						}

			CCustomFilters::CCustomFilters()
			{
			}
			CCustomFilters::~CCustomFilters()
			{
			}
			void CCustomFilters::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CCustomFilters::toXML() const
			{
				return _T("");
			}
			void CCustomFilters::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"customFilters", L"");
			}
			void CCustomFilters::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_arrItems.empty()) return;

				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				if (m_oAnd.IsInit() && true == m_oAnd->ToBool())
					writer.WriteString(L" and=\"1\"");
				writer.EndAttributes();

				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (  m_arrItems[i] )
					{
						m_arrItems[i]->toXMLWithNS(writer, child_ns, L"customFilter", child_ns);
					}
				}

				writer.EndNodeWithNS(node_ns, node_name);
			}
			void CCustomFilters::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			void CCustomFilters::fromBin(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::CUSTOMFILTERS*>(obj.get());
							if(ptr != nullptr)
							{
								ReadAttributes(ptr->m_BrtBeginCustomFilters);

								for(auto &customFilter : ptr->m_arBrtCustomFilter)
								{
									m_arrItems.push_back( new CCustomFilter(customFilter));
								}
							}
						}
			EElementType CCustomFilters::getType () const
			{
				return et_x_CustomFilters;
			}
			void CCustomFilters::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("and"),      m_oAnd )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CCustomFilters::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::BeginCustomFilters*>(obj.get());
							if(ptr != nullptr)
							{
							   m_oAnd = (bool)ptr->fAnd;
							}
						}

			CFilter::CFilter()
			{
			}
			CFilter::~CFilter()
			{
			}
			void CFilter::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CFilter::toXML() const
			{
				return _T("");
			}
			void CFilter::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"filter", L"");
			}
			void CFilter::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oVal.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringAttrEncodeXmlString(L"val", *m_oVal);
					writer.EndAttributesAndNode();
				}
			}
			void CFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CFilter::fromBin(XLS::BaseObjectPtr& obj)
						{
							ReadAttributes(obj);
						}
			EElementType CFilter::getType () const
			{
				return et_x_Filter;
			}
			void CFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CFilter::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::Filter*>(obj.get());
							if(ptr != nullptr)
							{
								m_oVal = ptr->rgch.value();
							}
						}

			CDateGroupItem::CDateGroupItem()
			{
			}
			CDateGroupItem::~CDateGroupItem()
			{
			}
			void CDateGroupItem::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CDateGroupItem::toXML() const
			{
				return _T("");
			}
			void CDateGroupItem::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"dateGroupItem", L"");
			}
			void CDateGroupItem::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oDateTimeGrouping.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringNullableAttrInt(L"year", m_oYear, m_oYear->GetValue());
					WritingStringNullableAttrInt(L"month", m_oMonth, m_oMonth->GetValue());
					WritingStringNullableAttrInt(L"day", m_oDay, m_oDay->GetValue());
					WritingStringNullableAttrInt(L"hour", m_oHour, m_oHour->GetValue());
					WritingStringNullableAttrInt(L"minute", m_oMinute, m_oMinute->GetValue());
					WritingStringNullableAttrInt(L"second", m_oSecond, m_oSecond->GetValue());
					WritingStringNullableAttrString(L"dateTimeGrouping", m_oDateTimeGrouping, m_oDateTimeGrouping->ToString());
					writer.EndAttributesAndNode();
				}
			}
			void CDateGroupItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CDateGroupItem::fromBin(XLS::BaseObjectPtr& obj)
						{
							ReadAttributes(obj);
						}
			EElementType CDateGroupItem::getType () const
			{
				return et_x_DateGroupItem;
			}
			void CDateGroupItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
			void CDateGroupItem::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::AFilterDateGroupItem*>(obj.get());
							if(ptr != nullptr)
							{
								switch (ptr->dntChecked)
								{
									case 0x00000000:
										m_oDateTimeGrouping = SimpleTypes::Spreadsheet::EDateTimeGroup::datetimegroupYear; break;
									case 0x00000001:
										m_oDateTimeGrouping = SimpleTypes::Spreadsheet::EDateTimeGroup::datetimegroupMonth; break;
									case 0x00000002:
										m_oDateTimeGrouping = SimpleTypes::Spreadsheet::EDateTimeGroup::datetimegroupDay; break;
									case 0x00000003:
										m_oDateTimeGrouping = SimpleTypes::Spreadsheet::EDateTimeGroup::datetimegroupHour; break;
									case 0x00000004:
										m_oDateTimeGrouping = SimpleTypes::Spreadsheet::EDateTimeGroup::datetimegroupMinute; break;
									case 0x00000005:
										m_oDateTimeGrouping = SimpleTypes::Spreadsheet::EDateTimeGroup::datetimegroupSecond; break;
								}
								m_oDay      = ptr->dom;
								m_oHour     = ptr->hour;
								m_oMinute   = ptr->min;
								m_oMonth    = ptr->mon;
								m_oSecond   = ptr->sec;
								m_oYear     = ptr->yr;
							}
						}

			CFilters::CFilters()
			{
			}
			CFilters::~CFilters()
			{
			}
			void CFilters::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CFilters::toXML() const
			{
				return _T("");
			}
			void CFilters::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"filters", L"");
			}
			void CFilters::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(!m_arrItems.empty() || m_oBlank.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringNullableAttrBool(L"blank", m_oBlank);
					writer.EndAttributes();

					for ( size_t i = 0; i < m_arrItems.size(); ++i)
					{
						if (  m_arrItems[i] )
						{
							CFilter* pFilter = dynamic_cast<CFilter*>(m_arrItems[i]);
							CDateGroupItem* pDateGroupItem = dynamic_cast<CDateGroupItem*>(m_arrItems[i]);
							if(pFilter)
							{
								pFilter->toXMLWithNS(writer, child_ns, L"filter", child_ns);
							}
							else if(pDateGroupItem)
							{
								pDateGroupItem->toXMLWithNS(writer, child_ns, L"dateGroupItem", child_ns);
							}
						}
					}

					writer.EndNodeWithNS(node_ns, node_name);
				}
			}
			void CFilters::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			void CFilters::fromBin(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::FILTERS*>(obj.get());
							if(ptr != nullptr)
							{
								ReadAttributes(ptr->m_BrtBeginFilters);

								for(auto &filter : ptr->m_arBrtFilter)
								{
									m_arrItems.push_back( new CFilter(filter));
								}

								for(auto &aFilterDateGroupItem : ptr->m_arBrtAFilterDateGroupItem)
								{
									m_arrItems.push_back( new CDateGroupItem(aFilterDateGroupItem));
								}
							}
						}
			EElementType CFilters::getType () const
			{
				return et_x_Filters;
			}
			void CFilters::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("blank"),      m_oBlank )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CFilters::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::BeginFilters*>(obj.get());
							if(ptr != nullptr)
							{
							   m_oBlank = (bool)ptr->fBlank;
							}
						}

			CTop10::CTop10()
			{
			}
			CTop10::~CTop10()
			{
			}
			void CTop10::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CTop10::toXML() const
			{
				return _T("");
			}
			void CTop10::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"top10", L"");
			}
			void CTop10::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oVal.IsInit())
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					if(m_oTop.IsInit() && false == m_oTop->ToBool())
						writer.WriteString(L" top=\"0\"");
					if(m_oPercent.IsInit() && true == m_oPercent->ToBool())
						writer.WriteString(L" percent=\"1\"");
					WritingStringNullableAttrDouble(L"val", m_oVal, m_oVal->GetValue());
					WritingStringNullableAttrDouble(L"filterVal", m_oFilterVal, m_oFilterVal->GetValue());
					writer.EndAttributesAndNode();
				}
			}
			void CTop10::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			void CTop10::fromBin(XLS::BaseObjectPtr& obj)
						{
							ReadAttributes(obj);
						}
			EElementType CTop10::getType () const
			{
				return et_x_ColorFilter;
			}
			void CTop10::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("filterVal"),      m_oFilterVal )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("percent"),      m_oPercent )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("top"),      m_oTop )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CTop10::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::Top10Filter*>(obj.get());
							if(ptr != nullptr)
							{
								m_oFilterVal   = ptr->xNumValue.data.value;
								m_oPercent     = ptr->fPercent;
								m_oTop         = ptr->fTop;
								m_oVal         = ptr->xNumFilter.data.value;
							}
						}

			CFilterColumn::CFilterColumn()
			{
			}
			CFilterColumn::~CFilterColumn()
			{
			}
			void CFilterColumn::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CFilterColumn::toXML() const
			{
				return _T("");
			}
			void CFilterColumn::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"filterColumn", L"");
			}
			void CFilterColumn::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				if(m_oColId.IsInit() && (m_oColorFilter.IsInit() || m_oDynamicFilter.IsInit() || m_oCustomFilters.IsInit() || m_oFilters.IsInit() ||
										m_oTop10.IsInit() || m_oShowButton.IsInit() || m_oHiddenButton.IsInit()))
				{
					writer.StartNodeWithNS(node_ns, node_name);
					writer.StartAttributes();
					WritingStringNullableAttrInt(L"colId", m_oColId, m_oColId->GetValue());
					if(m_oShowButton.IsInit() && false == m_oShowButton->ToBool())
						writer.WriteString(L" showButton=\"0\"");
					WritingStringNullableAttrBool(L"hiddenButton", m_oHiddenButton);
					writer.EndAttributes();
					if(m_oColorFilter.IsInit())
						m_oColorFilter->toXMLWithNS(writer, child_ns, L"colorFilter", child_ns);
					if(m_oDynamicFilter.IsInit())
						m_oDynamicFilter->toXMLWithNS(writer, child_ns, L"dynamicFilter", child_ns);
					if(m_oCustomFilters.IsInit())
						m_oCustomFilters->toXMLWithNS(writer, child_ns, L"customFilters", child_ns);
					if(m_oFilters.IsInit())
						m_oFilters->toXMLWithNS(writer, child_ns, L"filters", child_ns);
					if(m_oTop10.IsInit())
						m_oTop10->toXMLWithNS(writer, child_ns, L"top10", child_ns);
					writer.EndNodeWithNS(node_ns, node_name);
				}
			}
			void CFilterColumn::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			void CFilterColumn::fromBin(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::FILTERCOLUMN*>(obj.get());
							if(ptr != nullptr)
							{
								ReadAttributes(ptr->m_BrtBeginFilterColumn);

								if(ptr->m_source != nullptr)
								{
									if(ptr->m_source->get_type() == XLS::typeColorFilter)
										m_oColorFilter = ptr->m_source;
									else if(ptr->m_source->get_type() == XLS::typeDynamicFilter)
										m_oDynamicFilter = ptr->m_source;
									else if(ptr->m_source->get_type() == XLS::typeCUSTOMFILTERS)
										m_oCustomFilters = ptr->m_source;
									else if(ptr->m_source->get_type() == XLS::typeFILTERS)
										m_oFilters = ptr->m_source;
									else if(ptr->m_source->get_type() == XLS::typeTop10Filter)
										m_oTop10 = ptr->m_source;
								}
							}
						}
			EElementType CFilterColumn::getType () const
			{
				return et_x_FilterColumn;
			}
			void CFilterColumn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("colId"),      m_oColId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hiddenButton"),      m_oHiddenButton )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showButton"),      m_oShowButton )

					WritingElement_ReadAttributes_End( oReader )
			}
			void CFilterColumn::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::BeginFilterColumn*>(obj.get());
							if(ptr != nullptr)
							{
							   m_oColId         = ptr->dwCol;
							   m_oHiddenButton  = ptr->fHideArrow;
							   m_oShowButton    = ptr->fNoBtn;
							}
						}

			CAutofilter::CAutofilter()
			{
			}
			CAutofilter::~CAutofilter()
			{
			}
			void CAutofilter::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CAutofilter::toXML() const
			{
				return _T("");
			}
			void CAutofilter::toXML(NSStringUtils::CStringBuilder& writer) const
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
			void CAutofilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			void CAutofilter::fromBin(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::AUTOFILTER*>(obj.get());
							if(ptr != nullptr)
							{
								ReadAttributes(ptr->m_BrtBeginAFilter);

								if(ptr->m_SORTSTATE != nullptr)
									m_oSortState = ptr->m_SORTSTATE;

								for(auto &filterColumn : ptr->m_arFILTERCOLUMN)
								{
									m_arrItems.push_back(new CFilterColumn(filterColumn));
								}
							}
						}
			EElementType CAutofilter::getType () const
			{
				return et_x_Autofilter;
			}
			void CAutofilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("ref"),      m_oRef )
				WritingElement_ReadAttributes_End( oReader )
			}
			void CAutofilter::ReadAttributes(XLS::BaseObjectPtr& obj)
						{
							auto ptr = static_cast<XLSB::BeginAFilter*>(obj.get());
							if(ptr != nullptr)
							{
							   m_oRef = ptr->rfx.toString();
							}
						}

	} //Spreadsheet
} // namespace OOX
