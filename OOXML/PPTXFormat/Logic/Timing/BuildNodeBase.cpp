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

#include "BuildNodeBase.h"
#include "BldP.h"
#include "BldDgm.h"
#include "BldGraphic.h"
#include "BldOleChart.h"

namespace PPTX
{
	namespace Logic
	{
		BuildNodeBase::BuildNodeBase()
		{
		}
		BuildNodeBase::~BuildNodeBase()
		{
		}
		BuildNodeBase::BuildNodeBase(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const BuildNodeBase& BuildNodeBase::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		void BuildNodeBase::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("bldP"))
				m_node.reset(CreatePtrXmlContent<Logic::BldP>(node));
			else if (name == _T("bldDgm"))
				m_node.reset(CreatePtrXmlContent<Logic::BldDgm>(node));
			else if (name == _T("bldGraphic"))
				m_node.reset(CreatePtrXmlContent<Logic::BldGraphic>(node));
			else if (name == _T("bldOleChart"))
				m_node.reset(CreatePtrXmlContent<Logic::BldOleChart>(node));
			else m_node.reset();
		}
		void BuildNodeBase::GetBuildNodeFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			
			if (element.GetNode(_T("p:bldP"), oNode))
				m_node.reset(CreatePtrXmlContent<Logic::BldP>(oNode));
			else if (element.GetNode(_T("p:bldDgm"), oNode))
				m_node.reset(CreatePtrXmlContent<Logic::BldDgm>(oNode));
			else if (element.GetNode(_T("p:bldGraphic"), oNode))
				m_node.reset(CreatePtrXmlContent<Logic::BldGraphic>(oNode));
			else if (element.GetNode(_T("p:bldOleChart"), oNode))
				m_node.reset(CreatePtrXmlContent<Logic::BldOleChart>(oNode));
			else m_node.reset();
		}
		std::wstring BuildNodeBase::toXML() const
		{
			if (m_node.IsInit())
				return m_node->toXML();
			return _T("");
		}
		void BuildNodeBase::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_node.IsInit())
				m_node->toXmlWriter(pWriter);
		}
		void BuildNodeBase::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			BYTE _type = pReader->GetUChar();

			switch (_type)
			{
				case 1:	m_node = new Logic::BldDgm(); break;
				case 2:	m_node = new Logic::BldOleChart(); break;
				case 3:	m_node = new Logic::BldGraphic(); break;
				case 4:	m_node = new Logic::BldP(); break;
				default:break;
			}
			if (m_node.IsInit())
			{
				m_node->fromPPTY(pReader);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		void BuildNodeBase::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			switch (m_node->getType())
			{
				case OOX::et_p_bldDgm:		pWriter->StartRecord(1); break;
				case OOX::et_p_bldOleChart:	pWriter->StartRecord(2); break;
				case OOX::et_p_bldGraphic:	pWriter->StartRecord(3); break;
				case OOX::et_p_bldP:		pWriter->StartRecord(4); break;
				default:					pWriter->StartRecord(0); break;
			}
			if (m_node.IsInit())
				m_node->toPPTY(pWriter);

			pWriter->EndRecord();
		}
		void BuildNodeBase::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				m_node->SetParentPointer(pParent);
		}
		void BuildNodeBase::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
