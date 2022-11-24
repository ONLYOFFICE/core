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

#include "ChartColors.h"

namespace OOX
{
	namespace Spreadsheet
	{
		namespace ChartEx
		{
			CVariation::CVariation() {}
			void CVariation::fromXML(XmlUtils::CXmlNode& node) {}
			void CVariation::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsEmptyNode())
					return;

				int nParentDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nParentDepth))
				{
					m_arrItems.push_back(new PPTX::Logic::ColorModifier());
					m_arrItems.back()->fromXML(oReader);
				}
			}
			void CVariation::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<cs:variation>");

				for (size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (m_arrItems[i])
						writer.WriteString(m_arrItems[i]->toXML());
				}
				writer.WriteString(L"</cs:variation>");
			}
			std::wstring CVariation::toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData();
			}
			EElementType CVariation::getType()
				{
					return et_cs_Variation;
				}

			//------------------------------------------------------------------------------

			CColorStyle::CColorStyle() {}
			void CColorStyle::fromXML(XmlUtils::CXmlNode& node) {}
			void CColorStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
					else if (L"variation" == sName)
					{
						m_arrItems.push_back(new CVariation());
						m_arrItems.back()->fromXML(oReader);
					}
					else
					{
						PPTX::Logic::UniColor *pColor = new PPTX::Logic::UniColor();
						pColor->fromXML(oReader);

						m_arrItems.push_back(dynamic_cast<OOX::WritingElement*>(pColor));
					}
				}
			}
			void CColorStyle::toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<cs:colorStyle");
					WritingStringAttrString(L"xmlns:cs", L"http://schemas.microsoft.com/office/drawing/2012/chartStyle");
					WritingStringAttrString(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
					WritingStringNullableAttrString(L"meth", m_meth, *m_meth)
					WritingStringNullableAttrInt(L"id", m_id, *m_id)
				writer.WriteString(L">");

				for (size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (m_arrItems[i])
						writer.WriteString(m_arrItems[i]->toXML());
				}
				writer.WriteString(L"</cs:colorStyle>");
			}
			std::wstring CColorStyle::toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData();
			}
			EElementType CColorStyle::getType() const
			{
				return et_cs_ColorStyle;
			}
			void CColorStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
				{
					WritingElement_ReadAttributes_Start_No_NS(oReader)
						WritingElement_ReadAttributes_Read_if(oReader, L"id", m_id)
						WritingElement_ReadAttributes_Read_else_if(oReader, L"meth", m_meth)
					WritingElement_ReadAttributes_End_No_NS(oReader)
				}

		}
	}
}

