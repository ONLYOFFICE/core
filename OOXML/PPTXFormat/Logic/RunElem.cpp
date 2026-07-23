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

#include "RunElem.h"

namespace PPTX
{
	namespace Logic
	{
		RunElem::RunElem() {}
		OOX::EElementType RunElem::getType () const
		{
			if (Elem.IsInit())
				return Elem->getType();
			return OOX::et_Unknown;
		}
		void RunElem::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());

			if(name == _T("r"))
				Elem.reset(CreatePtrXmlContent<Logic::Run>(oReader));
			else if(name == _T("fld"))
				Elem.reset(CreatePtrXmlContent<Logic::Fld>(oReader));
			else if(name == _T("br"))
				Elem.reset(CreatePtrXmlContent<Logic::Br>(oReader));
			else if(name == _T("m"))
				Elem.reset(CreatePtrXmlContent<Logic::MathParaWrapper>(oReader));
			else
				Elem.reset();
		}
		void RunElem::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if(name == _T("r"))
				Elem.reset(CreatePtrXmlContent<Logic::Run>(node));
			else if(name == _T("fld"))
				Elem.reset(CreatePtrXmlContent<Logic::Fld>(node));
			else if(name == _T("br"))
				Elem.reset(CreatePtrXmlContent<Logic::Br>(node));
			else if(name == _T("m"))
				Elem.reset(CreatePtrXmlContent<Logic::MathParaWrapper>(node));
			else
				Elem.reset();
		}
		std::wstring RunElem::toXML() const
		{
			if (is_init())
				return Elem->toXML();
			return _T("");
		}
		void RunElem::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (is_init())
				Elem->toXmlWriter(pWriter);
		}
		void RunElem::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (Elem.is_init())
				Elem->toPPTY(pWriter);
		}
		void RunElem::InitRun(RunBase* pRun)
		{
			Elem.reset(pRun);
		}
		bool RunElem::is_init() const
		{
			return (Elem.IsInit());
		}
		std::wstring RunElem::GetText() const
		{
			return Elem->GetText();
		}
		smart_ptr<RunBase> RunElem::GetElem()
		{
			return Elem;
		}
		void RunElem::FillParentPointersForChilds()
		{
		}
		void RunElem::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				Elem->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
