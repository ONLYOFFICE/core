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

#include "PhoneticPr.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../XlsbFormat/Biff12_structures/PhRun.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BiffStructure.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CPhonetic::CPhonetic(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CPhonetic::~CPhonetic()
		{
		}
		void CPhonetic::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPhonetic::toXML() const
		{
			return _T("");
		}
		void CPhonetic::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CPhonetic::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CPhonetic::fromBin(XLS::BiffStructure& obj)
		{
			ReadAttributes(obj);
		}
		void CPhonetic::toBin(XLS::BiffStructure* obj)
		{
			auto ptr = static_cast<XLSB::PhRun*>(obj);
			if(m_oAlignment.IsInit())
			{
				if(m_oAlignment == SimpleTypes::Spreadsheet::phoneticalignmentNoControl)
				{
					ptr->alcH = 0;
				}
				else if(m_oAlignment == SimpleTypes::Spreadsheet::phoneticalignmentLeft)
				{
					ptr->alcH = 1;
				}
				else if(m_oAlignment == SimpleTypes::Spreadsheet::phoneticalignmentCenter)
				{
					ptr->alcH = 2;
				}
				else if(m_oAlignment == SimpleTypes::Spreadsheet::phoneticalignmentDistributed)
				{
					ptr->alcH = 3;
				}
			}
			if(m_oType.IsInit())
			{
				if(m_oType == SimpleTypes::Spreadsheet::phonetictypeHalfwidthKatakana)
				{
					ptr->phType = 0;
				}
				else if(m_oType == SimpleTypes::Spreadsheet::phonetictypeFullwidthKatakana)
				{
					ptr->phType = 1;
				}
				else if(m_oType == SimpleTypes::Spreadsheet::phonetictypeHiragana)
				{
					ptr->phType = 2;
				}
				else if(m_oType == SimpleTypes::Spreadsheet::phonetictypeNoConversion)
				{
					ptr->phType = 3;
				}
			}

			if(m_oFontId.IsInit())
			{
				ptr->ifnt = m_oFontId->GetValue();
			}
		}
		EElementType CPhonetic::getType () const
		{
			return et_x_PhoneticPr;
		}
		void CPhonetic::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			
			WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("alignment"),      m_oAlignment )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("fontId"),      m_oFontId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )

				WritingElement_ReadAttributes_End( oReader )
		}
		void CPhonetic::ReadAttributes(XLS::BiffStructure& obj)
		{
			auto ptr = static_cast<XLSB::PhRun*>(&obj);

			if(ptr != nullptr)
			{
				switch(ptr->alcH)
				{
					case 0:
						m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentNoControl;
						break;
					case 1:
						m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentLeft;
						break;
					case 2:
						m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentCenter;
						break;
					case 3:
						m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentDistributed;
						break;
				}

				switch(ptr->phType)
				{
					case 0:
						m_oType = SimpleTypes::Spreadsheet::phonetictypeHalfwidthKatakana;
						break;
					case 1:
						m_oType = SimpleTypes::Spreadsheet::phonetictypeFullwidthKatakana;
						break;
					case 2:
						m_oType = SimpleTypes::Spreadsheet::phonetictypeHiragana;
						break;
					case 3:
						m_oType = SimpleTypes::Spreadsheet::phonetictypeNoConversion;
						break;
				}

				m_oFontId = ptr->ifnt;

			}
		}

		CRPh::CRPh(OOX::Document *pMain) : WritingElementWithChilds<CText>(pMain) {}

		CRPh::~CRPh()
		{
		}
		void CRPh::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRPh::toXML() const
		{
			return _T("");
		}
		void CRPh::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CRPh::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("t") == sName )
				{
					CText* pText = new CText();
					*pText = oReader;
					m_arrItems.push_back(pText);
				}
			}
		}
		void CRPh::fromBin(XLS::BiffStructure& obj, std::wstring& str)
		{
			auto ptr = new CText();
			ptr->fromBin(str);
			m_arrItems.push_back(ptr);
			ReadAttributes(obj);
		}
		std::wstring CRPh::toBin(XLS::BiffStructure* obj)
		{
			auto ptr = static_cast<XLSB::PhRun*>(obj);
			std::wstring result;
			if(!m_arrItems.empty())
			{
				result = m_arrItems.back()->ToString();
			}
			if(m_oEb.IsInit())
				ptr->ichMom = m_oEb->GetValue();
			if(m_oSb.IsInit())
				ptr->ichFirst = m_oSb->GetValue();
			return result;
		}
		EElementType CRPh::getType () const
		{
			return et_x_rPh;
		}
		void CRPh::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			
			WritingElement_ReadAttributes_Start( oReader )

			WritingElement_ReadAttributes_Read_if     ( oReader, _T("eb"),      m_oEb )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("sb"),      m_oSb )

			WritingElement_ReadAttributes_End( oReader )
		}
		void CRPh::ReadAttributes(XLS::BiffStructure& obj)
			{
				auto ptr = static_cast<XLSB::PhRun*>(&obj);

				if(ptr != nullptr)
				{
					m_oEb = ptr->ichMom;
					m_oSb = ptr->ichFirst;
				}
			}

	} //Spreadsheet
} // namespace OOX
