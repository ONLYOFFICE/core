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

#include "BulletTypeface.h"

namespace PPTX
{
	namespace Logic
	{
		BulletTypeface& BulletTypeface::operator=(const BulletTypeface& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			m_Typeface		= oSrc.m_Typeface;

			return *this;
		}
		OOX::EElementType BulletTypeface::getType () const
		{
			if (m_Typeface.IsInit())
				return m_Typeface->getType();
			return OOX::et_Unknown;
		}
		void BulletTypeface::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring strName = oReader.GetName();
			if (strName == _T("a:buFontTx"))
				m_Typeface.reset(CreatePtrXmlContent<Logic::BuFontTx>(oReader));
			else if (strName == _T("a:buFont"))
				m_Typeface.reset(CreatePtrXmlContent<Logic::TextFont>(oReader));
			else
				m_Typeface.reset();
		}
		void BulletTypeface::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = node.GetName();

			if (strName == _T("a:buFontTx"))
				m_Typeface.reset(CreatePtrXmlContent<Logic::BuFontTx>(node));
			else if (strName == _T("a:buFont"))
				m_Typeface.reset(CreatePtrXmlContent<Logic::TextFont>(node));
			else
				m_Typeface.reset();
		}
		void BulletTypeface::ReadBulletTypefaceFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:buFontTx"), oNode))
				m_Typeface.reset(CreatePtrXmlContent<Logic::BuFontTx>(oNode));
			else if(element.GetNode(_T("a:buFont"), oNode))
				m_Typeface.reset(CreatePtrXmlContent<Logic::TextFont>(oNode));
			else m_Typeface.reset();
		}
		bool BulletTypeface::is_init()const{return (m_Typeface.IsInit());};
		bool BulletTypeface::has_spec_typeface()const{return is<TextFont>();};
		std::wstring BulletTypeface::toXML()const
		{
			if (m_Typeface.IsInit())
				return m_Typeface->toXML();
			return _T("");
		}
		void BulletTypeface::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_Typeface.is_init())
				m_Typeface->toXmlWriter(pWriter);
		}
		void BulletTypeface::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (m_Typeface.is_init())
			{
				if (m_Typeface.is<Logic::TextFont>())
				{
					pWriter->StartRecord(BULLET_TYPE_TYPEFACE_BUFONT);
					m_Typeface->toPPTY(pWriter);
					pWriter->EndRecord();
				}
				else
				{
					m_Typeface->toPPTY(pWriter);
				}
			}
		}
		void BulletTypeface::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			if (pReader->GetPos() == _end_rec)
				return;

			BYTE _type = pReader->GetUChar();

			if (_type == BULLET_TYPE_TYPEFACE_BUFONT)
			{
				Logic::TextFont* p = new Logic::TextFont();
				p->m_name = _T("a:buFont");
				p->fromPPTY(pReader);
				m_Typeface.reset(p);
			}
			else
			{
				m_Typeface.reset(new Logic::BuFontTx());
			}

			pReader->Seek(_end_rec);
		}
		void BulletTypeface::FillParentPointersForChilds(){}
		void BulletTypeface::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				m_Typeface->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
