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

#include "CellStyles.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../XlsbFormat/Biff12_records/Style.h"
#include "../../XlsbFormat/Biff12_records/BeginStyles.h"

#include "../../XlsbFormat/Biff12_unions/STYLES.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/STYLES.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Style.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CCellStyle::CCellStyle()
		{
		}
		CCellStyle::~CCellStyle()
		{
		}
		void CCellStyle::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellStyle::toXML() const
		{
			return _T("");
		}
		void CCellStyle::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<cellStyle"));
			WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
			WritingStringNullableAttrInt(L"xfId", m_oXfId, m_oXfId->GetValue());
			WritingStringNullableAttrInt(L"builtinId", m_oBuiltinId, m_oBuiltinId->GetValue());
			writer.WriteString(_T("/>"));
		}
		void CCellStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CCellStyle::fromBin(XLS::BaseObjectPtr& obj)
		 {
			 ReadAttributes(obj);
		 }
		XLS::BaseObjectPtr CCellStyle::toBin()
		{
			auto ptr(new XLSB::Style);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oBuiltinId.IsInit())
            {
                ptr->fBuiltIn = true;
                ptr->iStyBuiltIn = m_oBuiltinId->GetValue();
            }
            else
                ptr->fBuiltIn = false;
			if (m_oCustomBuiltin.IsInit())
				ptr->fCustom = m_oCustomBuiltin->GetValue();
            else
                ptr->fCustom = false;
			if (m_oHidden.IsInit())
				ptr->fHidden = m_oHidden->GetValue();
            else
                ptr->fHidden = false;
            if (m_oILevel.IsInit() && m_oILevel->GetValue() >= 0 && m_oILevel->GetValue() <= 6)
				ptr->iLevel = m_oILevel->GetValue();
            else
                ptr->iLevel = 0;
			if (m_oName.IsInit())
				ptr->stName = m_oName.get();
			else
				ptr->stName = L"";
			if (m_oXfId.IsInit())
				ptr->ixf = m_oXfId->GetValue();
			else
				ptr->ixf = 0;
			return objectPtr;
		}
		XLS::BaseObjectPtr CCellStyle::toXLS()
		{
			auto ptr = new XLS::Style;
			if (m_oXfId.IsInit())
				ptr->ixfe = m_oXfId->GetValue() + 16;

			if (m_oName.IsInit())
			{
				ptr->fBuiltIn = false;
				ptr->user = m_oName.get();
			}
			else if(m_oBuiltinId.IsInit())
			{
				ptr->fBuiltIn = true;
				ptr->builtInData.istyBuiltIn = m_oBuiltinId->GetValue();
				if(m_oILevel.IsInit())
					ptr->builtInData.iLevel = m_oILevel->GetValue();
			}
			return XLS::BaseObjectPtr(ptr);
		}
		EElementType CCellStyle::getType () const
		{
			return et_x_CellStyle;
		}
		void CCellStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("builtinId"),      m_oBuiltinId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("customBuiltin"),      m_oCustomBuiltin )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),      m_oHidden )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("iLevel"),      m_oILevel )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("xfId"),      m_oXfId )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CCellStyle::ReadAttributes(XLS::BaseObjectPtr& obj)
		 {
			 auto ptr = static_cast<XLSB::Style*>(obj.get());
			 if(ptr != nullptr)
			 {
				 m_oBuiltinId     = ptr->fBuiltIn;
				 m_oCustomBuiltin = ptr->fCustom;
				 m_oHidden        = ptr->fHidden;
				 m_oILevel        = ptr->iLevel;
				 m_oName          = ptr->stName;
				 m_oXfId          = ptr->ixf;
			 }
		 }

		CCellStyles::CCellStyles()
		{
		}
		CCellStyles::~CCellStyles()
		{
		}
		void CCellStyles::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellStyles::toXML() const
		{
			return _T("");
		}
		void CCellStyles::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<cellStyles"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));

			 for ( size_t i = 0; i < m_arrItems.size(); ++i)
			 {
				 if ( m_arrItems[i] )
				 {
					 m_arrItems[i]->toXML(writer);
				 }
			 }

			writer.WriteString(_T("</cellStyles>"));
		}
		void CCellStyles::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("cellStyle") == sName )
				{
					CCellStyle* pCellStyle = new CCellStyle();
					*pCellStyle = oReader;
					m_arrItems.push_back(pCellStyle);
				}
			}
		}
		void CCellStyles::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		 {
			 ReadAttributes(obj);

			 for(auto &style : obj)
			 {
				 CCellStyle *pXfs = new CCellStyle(style);
				 m_arrItems.push_back(pXfs);
			 }
		 }
		XLS::BaseObjectPtr CCellStyles::toBin()
		{
			auto ptr(new XLSB::STYLES);
			auto ptr1(new XLSB::BeginStyles);
			ptr->m_BrtBeginStyles = XLS::BaseObjectPtr{ptr1};
			XLS::BaseObjectPtr objectPtr(ptr);

			for(auto i:m_arrItems)
				ptr->m_arBrtStyle.push_back(i->toBin());
			ptr1->cstyles = ptr->m_arBrtStyle.size();
			return objectPtr;
		}
		XLS::BaseObjectPtr CCellStyles::toXLS()
		{
			auto ptr = new XLS::STYLES;
			for(auto i:m_arrItems)
			{
				ptr->m_arStyles.push_back(std::make_pair(i->toXLS(), nullptr));
			}
			return XLS::BaseObjectPtr(ptr);
		}
		EElementType CCellStyles::getType () const
		{
			return et_x_CellStyles;
		}
		void CCellStyles::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("count"),      m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CCellStyles::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_UINT32)obj.size();
			}

	} //Spreadsheet
} // namespace OOX
