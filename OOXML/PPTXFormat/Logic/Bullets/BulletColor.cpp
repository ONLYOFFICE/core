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

#include "BulletColor.h"

namespace PPTX
{
	namespace Logic
	{
		BulletColor& BulletColor::operator=(const BulletColor& oColor)
		{
			parentFile		= oColor.parentFile;
			parentElement	= oColor.parentElement;

			m_Color			= oColor.m_Color;

			return *this;
		}
		OOX::EElementType BulletColor::getType() const
		{
			if (m_Color.IsInit())
				return m_Color->getType();
			return OOX::et_Unknown;
		}
		void BulletColor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring strName = oReader.GetName();

			if (strName == _T("a:buClrTx"))
				m_Color.reset(CreatePtrXmlContent<Logic::BuClrTx>(oReader));
			else if (strName == _T("a:buClr"))
				m_Color.reset(CreatePtrXmlContent<Logic::BuClr>(oReader));
			else
				m_Color.reset();
		}
		void BulletColor::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = node.GetName();

			if (strName == _T("a:buClrTx"))
				m_Color.reset(CreatePtrXmlContent<Logic::BuClrTx>(node));
			else if (strName == _T("a:buClr"))
				m_Color.reset(CreatePtrXmlContent<Logic::BuClr>(node));
			else
				m_Color.reset();
		}
		void BulletColor::ReadBulletColorFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:buClrTx"), oNode))
				m_Color.reset(CreatePtrXmlContent<Logic::BuClrTx>(oNode));
			else if (element.GetNode(_T("a:buClr"), oNode))
				m_Color.reset(CreatePtrXmlContent<Logic::BuClr>(oNode));
			else
				m_Color.reset();
		}
		void BulletColor::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (m_Color.is_init())
				m_Color->toPPTY(pWriter);
		}
		void BulletColor::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			if (pReader->GetPos() == _end_rec)
				return;

			BYTE _type = pReader->GetUChar();

			if (_type == BULLET_TYPE_COLOR_CLRTX)
			{
				m_Color.reset(new Logic::BuClrTx());
			}
			else
			{
				LONG len = pReader->GetLong();
				if(len > 0)
				{
					Logic::BuClr* pClr = new Logic::BuClr();
					pReader->Skip(1); // type(0)
					pClr->Color.fromPPTY(pReader);
					m_Color.reset(pClr);
				}
			}

			pReader->Seek(_end_rec);
		}
		void BulletColor::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_Color.is_init())
				m_Color->toXmlWriter(pWriter);
		}
		bool BulletColor::is_init()const{return (m_Color.IsInit());};
		bool BulletColor::has_spec_color()const{return is<BuClr>();};
		DWORD BulletColor::GetRGBA()const
		{
			if(has_spec_color())
				return as<BuClr>().GetRGBA();
			return 0;
		}
		DWORD BulletColor::GetARGB()const
		{
			if(has_spec_color())
				return as<BuClr>().GetARGB();
			return 0;
		}
		DWORD BulletColor::GetBGRA()const
		{
			if(has_spec_color())
				return as<BuClr>().GetBGRA();
			return 0;
		}
		DWORD BulletColor::GetABGR()const
		{
			if(has_spec_color())
				return as<BuClr>().GetABGR();
			return 0;
		}
		std::wstring BulletColor::toXML() const
		{
			if (m_Color.IsInit())
				return m_Color->toXML();
			return _T("");
		}
		void BulletColor::FillParentPointersForChilds(){}
		void BulletColor::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				m_Color->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
