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

#include "UniPath2D.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType UniPath2D::getType() const
		{
			if (Path2D.IsInit())
				return Path2D->getType();
			return OOX::et_Unknown;
		}
		void UniPath2D::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());

			if (name == _T("moveTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::MoveTo>(oReader));
			else if (name == _T("lnTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::LineTo>(oReader));
			else if (name == _T("cubicBezTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::CubicBezTo>(oReader));
			else if (name == _T("close"))
				Path2D.reset(CreatePtrXmlContent<Logic::Close>(oReader));
			else if (name == _T("arcTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::ArcTo>(oReader));
			else if (name == _T("quadBezTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::QuadBezTo>(oReader));
			else Path2D.reset();
		}
		void UniPath2D::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("moveTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::MoveTo>(node));
			else if (name == _T("lnTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::LineTo>(node));
			else if (name == _T("cubicBezTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::CubicBezTo>(node));
			else if (name == _T("close"))
				Path2D.reset(CreatePtrXmlContent<Logic::Close>(node));
			else if (name == _T("arcTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::ArcTo>(node));
			else if (name == _T("quadBezTo"))
				Path2D.reset(CreatePtrXmlContent<Logic::QuadBezTo>(node));
			else Path2D.reset();
		}
		void UniPath2D::GetPath2DFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;

			if(element.GetNode(_T("a:moveTo"), oNode))
				Path2D.reset(CreatePtrXmlContent<Logic::MoveTo>(oNode));
			else if(element.GetNode(_T("a:lnTo"), oNode))
				Path2D.reset(CreatePtrXmlContent<Logic::LineTo>(oNode));
			else if(element.GetNode(_T("a:cubicBezTo"), oNode))
				Path2D.reset(CreatePtrXmlContent<Logic::CubicBezTo>(oNode));
			else if(element.GetNode(_T("a:close"), oNode))
				Path2D.reset(CreatePtrXmlContent<Logic::Close>(oNode));
			else if(element.GetNode(_T("a:arcTo"), oNode))
				Path2D.reset(CreatePtrXmlContent<Logic::ArcTo>(oNode));
			else if(element.GetNode(_T("a:quadBezTo"), oNode))
				Path2D.reset(CreatePtrXmlContent<Logic::QuadBezTo>(oNode));
			else Path2D.reset();
		}
		std::wstring UniPath2D::toXML() const
		{
			if (Path2D.IsInit())
				return Path2D->toXML();
			return _T("");
		}
		void UniPath2D::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (Path2D.is_init())
				Path2D->toPPTY(pWriter);
		}
		void UniPath2D::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (Path2D.is_init())
				Path2D->toXmlWriter(pWriter);
		}
		bool UniPath2D::is_init()const{return (Path2D.IsInit());};
		void UniPath2D::FillParentPointersForChilds(){}
		void UniPath2D::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				Path2D->SetParentPointer(pParent);
		}
		std::wstring UniPath2D::GetODString() const
		{
			return Path2D->GetODString();
		}
	} // namespace Logic
} // namespace PPTX
