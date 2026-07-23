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

#include "Colors.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../XlsbFormat/Biff12_unions/COLORPALETTE.h"
#include "../../XlsbFormat/Biff12_unions/INDEXEDCOLORS.h"
#include "../../XlsbFormat/Biff12_unions/MRUCOLORS.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Palette.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CColors::CColors()
		{
		}
		CColors::~CColors()
		{
		}
		void CColors::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CColors::toXML() const
		{
			return _T("");
		}
		void CColors::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CColors::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("indexedColors") == sName )
					m_oIndexedColors = oReader;
				else if ( _T("mruColors") == sName )
					m_oMruColors = oReader;
			}
		}
		void CColors::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::COLORPALETTE*>(obj.get());

			if(ptr->m_INDEXEDCOLORS != nullptr)
			{
				auto oINDEXEDCOLORS = static_cast<XLSB::INDEXEDCOLORS*>(ptr->m_INDEXEDCOLORS.get());
				if(!oINDEXEDCOLORS->m_arIndexedColor.empty())
				{
					m_oIndexedColors = oINDEXEDCOLORS->m_arIndexedColor;
				}
			}

			if(ptr->m_MRUCOLORS != nullptr)
			{
				auto oMRUCOLORS = static_cast<XLSB::MRUCOLORS*>(ptr->m_MRUCOLORS.get());
				if(!oMRUCOLORS->m_arMRUColor.empty())
				{
					m_oMruColors = oMRUCOLORS->m_arMRUColor;
				}
			}
		}
		XLS::BaseObjectPtr CColors::toBin()
		{
			auto ptr(new XLSB::COLORPALETTE);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oIndexedColors.IsInit())
			{
			auto indexColors(new XLSB::INDEXEDCOLORS);
                ptr->m_INDEXEDCOLORS = XLS::BaseObjectPtr{indexColors};
			indexColors->m_arIndexedColor =	m_oIndexedColors->toBin();
			}

			if(m_oMruColors.IsInit())
			{
				auto mruColors(new XLSB::MRUCOLORS);
				ptr->m_MRUCOLORS = XLS::BaseObjectPtr{mruColors};
                mruColors->m_arMRUColor = m_oMruColors->toBin();
			}

			return objectPtr;
		}
		XLS::BaseObjectPtr CColors::toXLS()
		{
			auto ptr = new XLS::Palette;

			if(m_oIndexedColors.IsInit())
			{
				for(auto i : m_oIndexedColors->mapIndexedColors)
					ptr->rgColor.push_back(i.second->toXLS());
			}

			return XLS::BaseObjectPtr(ptr);
		}
		EElementType CColors::getType () const
		{
			return et_x_Colors;
		}
		void CColors::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}

	} //Spreadsheet
} // namespace OOX

