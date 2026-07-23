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

#include "Run.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CRun::CRun(OOX::Document *pMain) : WritingElementWithChilds<CText>(pMain) {}

		CRun::~CRun()
		{
		}
		void CRun::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRun::toXML() const
		{
			return _T("");
		}
		void CRun::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<r>"));
			if(m_oRPr.IsInit())
				m_oRPr->toXML(writer);

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(_T("</r>"));
		}
		void CRun::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("rPr") == sName )
					m_oRPr = oReader;
				else if ( _T("t") == sName )
				{
					CText* pText = new CText();
					*pText = oReader;

					m_arrItems.push_back(pText);
				}
			}
		}
		void CRun::fromBin(std::wstring& str, unsigned short fontindex)
		{
			auto ptr = new CText();
			ptr->fromBin(str);
			m_arrItems.push_back(ptr);

			m_oRPr.Init();
			m_oRPr->m_nFontIndex.Init();
			m_oRPr->m_nFontIndex = fontindex;
		}
		std::wstring CRun::toBin(unsigned short &fontindex)
		{
			if(m_oRPr.IsInit())
			{
				if(m_oRPr->m_nFontIndex.IsInit())
					fontindex = m_oRPr->m_nFontIndex->GetValue();
			}
			if(!m_arrItems.empty())
			{
			auto textPtr = m_arrItems.back();
				return textPtr->ToString();
			}
			return L"";
		}
		EElementType CRun::getType () const
		{
			return et_x_r;
		}
		void CRun::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
