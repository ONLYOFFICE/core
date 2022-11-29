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

#include "ChartStyle.h"

namespace OOX
{
	namespace Spreadsheet
	{
		namespace ChartEx
		{
			CStyleEntry::CStyleEntry() {}
			void CStyleEntry::fromXML(XmlUtils::CXmlNode& node) {}
			void CStyleEntry::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name = XmlUtils::GetNameNoNS(oReader.GetName());
				if (oReader.IsEmptyNode())
					return;

				int nParentDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nParentDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (L"extLst" == sName)
					{
						m_extLst = oReader;
					}
					else if(L"bodyPr" == sName)
					{
						m_bodyPr = oReader;
					}
					else if (L"spPr" == sName)
					{
						m_spPr = oReader;
					}
					else if (L"defRPr" == sName)
					{
						m_defRPr = oReader;
					}
					else if (L"lineWidthScale" == sName)
					{
						m_lineWidthScale = oReader.GetText();
					}
					else if (L"lnRef" == sName)
					{
						m_lnRef.fromXML(oReader);
					}
					else if (L"fillRef" == sName)
					{
						m_fillRef.fromXML(oReader);
					}
					else if (L"effectRef" == sName)
					{
						m_effectRef.fromXML(oReader);
					}
					else if (L"fontRef" == sName)
					{
						m_fontRef.fromXML(oReader);
					}
				}
			}
			void CStyleEntry::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<cs:" + m_name + L">");

				writer.WriteString(m_lnRef.toXML());
				writer.WriteString(m_fillRef.toXML());
				writer.WriteString(m_effectRef.toXML());
				writer.WriteString(m_fontRef.toXML());

				if (m_spPr.IsInit())
				{
					m_spPr->m_namespace = L"cs";
					writer.WriteString(m_spPr->toXML());
				}
				if (m_defRPr.IsInit())
				{
					m_defRPr->m_name = L"cs:defRPr";
					writer.WriteString(m_defRPr->toXML());
				}
				if (m_bodyPr.IsInit())
				{
					m_bodyPr->m_namespace = L"cs";
					writer.WriteString(m_bodyPr->toXML());
				}
				writer.WriteString(L"</cs:" + m_name + L">");
			}
			std::wstring CStyleEntry::toXML() const { return L""; }
			EElementType CStyleEntry::getType()
			{
				return et_cs_StyleEntry;
			}
			BYTE CStyleEntry::getTypeStyleEntry()
			{
					 if (m_name == L"axisTitle")	return 1;
				else if (m_name == L"categoryAxis") return 2;
				else if (m_name == L"chartArea")	return 3;
				else if (m_name == L"dataLabel")	return 4;
				else if (m_name == L"dataLabelCallout") return 5;
				else if (m_name == L"dataPoint")	return 6;
				else if (m_name == L"dataPoint3D")	return 7;
				else if (m_name == L"dataPointLine") return 8;
				else if (m_name == L"dataPointMarker") return 9;
				else if (m_name == L"dataPointWireframe") return 10;
				else if (m_name == L"dataTable")	return 11;
				else if (m_name == L"downBar")		return 12;
				else if (m_name == L"dropLine")		return 13;
				else if (m_name == L"errorBar")		return 14;
				else if (m_name == L"floor")		return 15;
				else if (m_name == L"gridlineMajor") return 16;
				else if (m_name == L"gridlineMinor") return 17;
				else if (m_name == L"hiLoLine")		return 18;
				else if (m_name == L"leaderLine")	return 19;
				else if (m_name == L"legend")		return 20;
				else if (m_name == L"plotArea")		return 21;
				else if (m_name == L"plotArea3D")	return 22;
				else if (m_name == L"seriesAxis")	return 23;
				else if (m_name == L"seriesLine")	return 24;
				else if (m_name == L"title")		return 25;
				else if (m_name == L"trendline")	return 26;
				else if (m_name == L"trendlineLabel") return 27;
				else if (m_name == L"upBar")		return 28;
				else if (m_name == L"valueAxis")	return 29;
				else if (m_name == L"wall")			return 30;
				else
					return 0;
			}
			void CStyleEntry::setTypeStyleEntry(BYTE type)
				{
					switch (type)
					{
					case 1: m_name = L"axisTitle"; break;
					case 2: m_name = L"categoryAxis"; break;
					case 3: m_name = L"chartArea"; break;
					case 4: m_name = L"dataLabel"; break;
					case 5: m_name = L"dataLabelCallout"; break;
					case 6: m_name = L"dataPoint"; break;
					case 7: m_name = L"dataPoint3D"; break;
					case 8: m_name = L"dataPointLine"; break;
					case 9: m_name = L"dataPointMarker"; break;
					case 10: m_name = L"dataPointWireframe"; break;
					case 11: m_name = L"dataTable"; break;
					case 12: m_name = L"downBar"; break;
					case 13: m_name = L"dropLine"; break;
					case 14: m_name = L"errorBar"; break;
					case 15: m_name = L"floor"; break;
					case 16: m_name = L"gridlineMajor"; break;
					case 17: m_name = L"gridlineMinor"; break;
					case 18: m_name = L"hiLoLine"; break;
					case 19: m_name = L"leaderLine"; break;
					case 20: m_name = L"legend"; break;
					case 21: m_name = L"plotArea"; break;
					case 22: m_name = L"plotArea3D"; break;
					case 23: m_name = L"seriesAxis"; break;
					case 24: m_name = L"seriesLine"; break;
					case 25: m_name = L"title"; break;
					case 26: m_name = L"trendline"; break;
					case 27: m_name = L"trendlineLabel"; break;
					case 28: m_name = L"upBar"; break;
					case 29: m_name = L"valueAxis"; break;
					case 30: m_name = L"wall"; break;
					default:
						break;
					}
				}

			//------------------------------------------------------------------------------

			CMarkerLayout::CMarkerLayout(){}
			void CMarkerLayout::fromXML(XmlUtils::CXmlNode& node) {}
			void CMarkerLayout::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);
			}
			void CMarkerLayout::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<cs:dataPointMarkerLayout");
					WritingStringNullableAttrString(L"symbol", m_symbol, m_symbol->ToString())
					WritingStringNullableAttrInt(L"size", m_size, *m_size)
				writer.WriteString(L"/>");
			}
			std::wstring CMarkerLayout::toXML() const { return L""; }
			EElementType CMarkerLayout::getType()
			{
				return et_cs_MarkerLayout;
			}
			void CMarkerLayout::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					WritingElement_ReadAttributes_Start_No_NS(oReader)
						WritingElement_ReadAttributes_Read_if(oReader, L"symbol", m_symbol)
						WritingElement_ReadAttributes_Read_else_if(oReader, L"size", m_size)
					WritingElement_ReadAttributes_End_No_NS(oReader)
				}

			//------------------------------------------------------------------------------

			CChartStyle::CChartStyle() {}
			CChartStyle::~CChartStyle()
			{
				for (size_t i = 0; i < m_arStyleEntries.size(); ++i)
					if (m_arStyleEntries[i]) delete m_arStyleEntries[i];

				m_arStyleEntries.clear();
			}
			void CChartStyle::fromXML(XmlUtils::CXmlNode& node) {}
			void CChartStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nParentDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nParentDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (L"extLst" == sName)
					{
						m_extLst = oReader;
					}
					else if (L"dataPointMarkerLayout" == sName)
					{
						m_dataPointMarkerLayout = oReader;
					}
					else
					{
						m_arStyleEntries.push_back(new CStyleEntry(oReader));
					}
				}
			}
			void CChartStyle::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<cs:chartStyle");
					WritingStringAttrString(L"xmlns:cs", L"http://schemas.microsoft.com/office/drawing/2012/chartStyle");
					WritingStringAttrString(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
					WritingStringNullableAttrInt(L"id", m_id, *m_id)
				writer.WriteString(L">");

				for (size_t i = 0; i < m_arStyleEntries.size(); ++i)
					m_arStyleEntries[i]->toXML(writer);

				if (m_dataPointMarkerLayout.IsInit())
				{
					m_dataPointMarkerLayout->toXML(writer);
				}
				if (m_extLst.IsInit())
				{
					writer.WriteString(m_extLst->toXML());
				}
				writer.WriteString(L"</cs:chartStyle>");
			}
			std::wstring CChartStyle::toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData();
			}
			EElementType CChartStyle::getType() const
			{
				return et_cs_ChartStyle;
			}
			void CChartStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					WritingElement_ReadAttributes_Start_No_NS(oReader)
						WritingElement_ReadAttributes_Read_if(oReader, L"id", m_id)
					WritingElement_ReadAttributes_End_No_NS(oReader)
				}

		}
	}
}

