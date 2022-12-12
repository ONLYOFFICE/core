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
				m_Typeface.reset(new Logic::BuFontTx(oReader));
			else if (strName == _T("a:buFont"))
				m_Typeface.reset(new Logic::TextFont(oReader));
			else
				m_Typeface.reset();
		}
		void BulletTypeface::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = node.GetName();

			if (strName == _T("a:buFontTx"))
				m_Typeface.reset(new Logic::BuFontTx(node));
			else if (strName == _T("a:buFont"))
				m_Typeface.reset(new Logic::TextFont(node));
			else
				m_Typeface.reset();
		}
		void BulletTypeface::ReadBulletTypefaceFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:buFontTx"), oNode))
				m_Typeface.reset(new Logic::BuFontTx(oNode));
			else if(element.GetNode(_T("a:buFont"), oNode))
				m_Typeface.reset(new Logic::TextFont(oNode));
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
