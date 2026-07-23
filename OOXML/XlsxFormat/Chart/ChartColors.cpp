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

#include "ChartColors.h"
#include "../../PPTXFormat/Logic/UniColor.h"
#include "../../DocxFormat/Drawing/DrawingExt.h"

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

