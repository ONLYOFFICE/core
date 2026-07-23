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

#include "AhBase.h"

namespace PPTX
{
	namespace Logic
	{		
		OOX::EElementType AhBase::getType() const
		{
			if (ah.IsInit())
				return ah->getType();
			return OOX::et_Unknown;
		}
		void AhBase::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("ahXY"))
				ah.reset(CreatePtrXmlContent<Logic::AhXY>(node));
			else if (name == _T("ahPolar"))
				ah.reset(CreatePtrXmlContent<Logic::AhPolar>(node));
			else ah.reset();
		}
		void AhBase::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (sName == L"ahXY")
				ah.reset(CreatePtrXmlContent<Logic::AhXY>(oReader));
			else if(sName == L"ahPolar")
				ah.reset(CreatePtrXmlContent<Logic::AhPolar>(oReader));
			else ah.reset();
		}
		void AhBase::GetAdjustHandleFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:ahXY"), oNode))
				ah.reset(CreatePtrXmlContent<Logic::AhXY>(oNode));
			else if(element.GetNode(_T("a:ahPolar"), oNode))
				ah.reset(CreatePtrXmlContent<Logic::AhPolar>(oNode));
			else ah.reset();
		}
		std::wstring AhBase::toXML() const
		{
			if (ah.is_init())
				return ah->toXML();

			return _T("");
		}
		void AhBase::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (ah.is_init())
				ah->toPPTY(pWriter);
		}
		void AhBase::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (ah.is_init())
				ah->toXmlWriter(pWriter);
		}
		bool AhBase::is_init() const
		{
			return (ah.is_init());
		}
		void AhBase::FillParentPointersForChilds()
		{
		}
		void AhBase::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				ah->SetParentPointer(pParent);
		}
		std::wstring AhBase::GetODString()const
		{
			if (!ah.IsInit())
				return _T("");
			return ah->GetODString();
		}
	} // namespace Logic
} // namespace PPTX
