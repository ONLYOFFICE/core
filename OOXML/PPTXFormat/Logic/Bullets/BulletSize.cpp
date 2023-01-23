/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
				m_Size.reset(new Logic::BuSzTx(oReader));
			else if (strName == _T("a:buSzPct"))
				m_Size.reset(new Logic::BuSzPct(oReader));
			else if (strName == _T("a:buSzPts"))
				m_Size.reset(new Logic::BuSzPts(oReader));
			else
				m_Size.reset();
		}
		void BulletSize::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = node.GetName();

			if (strName == _T("a:buSzTx"))
				m_Size.reset(new Logic::BuSzTx(node));
			else if (strName == _T("a:buSzPct"))
				m_Size.reset(new Logic::BuSzPct(node));
			else if (strName == _T("a:buSzPts"))
				m_Size.reset(new Logic::BuSzPts(node));
			else
				m_Size.reset();
		}
		void BulletSize::ReadBulletSizeFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if(element.GetNode(_T("a:buSzTx"), oNode))
				m_Size.reset(new Logic::BuSzTx(oNode));
			else if(element.GetNode(_T("a:buSzPct"), oNode))
				m_Size.reset(new Logic::BuSzPct(oNode));
			else if(element.GetNode(_T("a:buSzPts"), oNode))
				m_Size.reset(new Logic::BuSzPts(oNode));
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
