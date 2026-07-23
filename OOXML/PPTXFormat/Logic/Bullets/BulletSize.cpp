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

#include "BulletSize.h"

namespace PPTX
{
	namespace Logic
	{		
		BulletSize& BulletSize::operator=(const BulletSize& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			m_Size			= oSrc.m_Size;

			return *this;
		}
		OOX::EElementType BulletSize::getType () const
		{
			if (m_Size.IsInit())
				return m_Size->getType();
			return OOX::et_Unknown;
		}
		void BulletSize::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring strName = oReader.GetName();

			if (strName == _T("a:buSzTx"))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzTx>(oReader));
			else if (strName == _T("a:buSzPct"))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzPct>(oReader));
			else if (strName == _T("a:buSzPts"))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzPts>(oReader));
			else
				m_Size.reset();
		}
		void BulletSize::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = node.GetName();

			if (strName == _T("a:buSzTx"))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzTx>(node));
			else if (strName == _T("a:buSzPct"))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzPct>(node));
			else if (strName == _T("a:buSzPts"))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzPts>(node));
			else
				m_Size.reset();
		}
		void BulletSize::ReadBulletSizeFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if(element.GetNode(_T("a:buSzTx"), oNode))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzTx>(oNode));
			else if(element.GetNode(_T("a:buSzPct"), oNode))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzPct>(oNode));
			else if(element.GetNode(_T("a:buSzPts"), oNode))
				m_Size.reset(CreatePtrXmlContent<Logic::BuSzPts>(oNode));
			else
				m_Size.reset();
		}
		bool BulletSize::is_init()const{return (m_Size.IsInit());};
		bool BulletSize::has_spec_size()const{return ((is_init()) && (!is<BuSzTx>()));};
		std::wstring BulletSize::toXML()const
		{
			if (m_Size.IsInit())
				return m_Size->toXML();
			return _T("");
		}
		void BulletSize::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_Size.is_init())
				m_Size->toXmlWriter(pWriter);
		}
		void BulletSize::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (m_Size.is_init())
				m_Size->toPPTY(pWriter);
		}
		void BulletSize::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			if (pReader->GetPos() == _end_rec)
				return;

			BYTE _type = pReader->GetUChar();

			if (_type == BULLET_TYPE_SIZE_TX)
			{
				m_Size.reset(new Logic::BuSzTx());
			}
			else if (_type == BULLET_TYPE_SIZE_PTS)
			{
				Logic::BuSzPts* p = new Logic::BuSzPts();
				pReader->Skip(6); // len + + startattr type(0)
				p->val = pReader->GetLong();
				m_Size.reset(p);
			}
			else
			{
				Logic::BuSzPct* p = new Logic::BuSzPct();
				pReader->Skip(6); // len + + startattr type(0)
				p->val = pReader->GetLong();
				m_Size.reset(p);
			}

			pReader->Seek(_end_rec);
		}
		void BulletSize::FillParentPointersForChilds(){}
		void BulletSize::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				m_Size->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
