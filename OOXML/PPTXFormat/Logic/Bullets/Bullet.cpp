/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "Bullet.h"

namespace PPTX
{
	namespace Logic
	{
		Bullet& Bullet::operator=(const Bullet& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			m_Bullet		= oSrc.m_Bullet;

			return *this;
		}
		OOX::EElementType Bullet::getType () const
		{
			if (m_Bullet.IsInit())
				return m_Bullet->getType();
			return OOX::et_Unknown;
		}
		void Bullet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (strName == _T("buNone"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuNone>(oReader));
			else if (strName == _T("buChar"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuChar>(oReader));
			else if (strName == _T("buAutoNum"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuAutoNum>(oReader));
			else if (strName == _T("buBlip"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuBlip>(oReader));
			else
				m_Bullet.reset();
		}
		void Bullet::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring strName = XmlUtils::GetNameNoNS(node.GetName());

			if (strName == _T("buNone"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuNone>(node));
			else if (strName == _T("buChar"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuChar>(node));
			else if (strName == _T("buAutoNum"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuAutoNum>(node));
			else if (strName == _T("buBlip"))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuBlip>(node));
			else
				m_Bullet.reset();
		}
		void Bullet::ReadBulletFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:buNone"), oNode))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuNone>(oNode));
			else if (element.GetNode(_T("a:buChar"), oNode))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuChar>(oNode));
			else if (element.GetNode(_T("a:buAutoNum"), oNode))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuAutoNum>(oNode));
			else if (element.GetNode(_T("a:buBlip"), oNode))
				m_Bullet.reset(CreatePtrXmlContent<Logic::BuBlip>(oNode));
			else
				m_Bullet.reset();
		}
		bool Bullet::is_init()const{return (m_Bullet.IsInit());};
		bool Bullet::has_bullet()const{return ((is_init()) && (!is<BuNone>()));};
		std::wstring Bullet::toXML()const
		{
			if (m_Bullet.IsInit())
				return m_Bullet->toXML();
			return _T("");
		}
		void Bullet::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_Bullet.is_init())
				m_Bullet->toXmlWriter(pWriter);
		}
		void Bullet::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (m_Bullet.is_init())
				m_Bullet->toPPTY(pWriter);
		}
		void Bullet::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			if (pReader->GetPos() == _end_rec)
				return;

			BYTE _type = pReader->GetUChar();

			switch (_type)
			{
			case BULLET_TYPE_BULLET_CHAR:
				{
					pReader->Skip(6);
					Logic::BuChar* p1 = new Logic::BuChar();
					p1->Char = pReader->GetString2();
					pReader->Skip(1);

					m_Bullet.reset(p1);
					break;
				}
			case BULLET_TYPE_BULLET_AUTONUM:
				{
					Logic::BuAutoNum* p2 = new Logic::BuAutoNum();
					pReader->Skip(5); // len + type + start attr

					while (true)
					{
						BYTE _at = pReader->GetUChar_TypeNode();
						if (_at == NSBinPptxRW::g_nodeAttributeEnd)
							break;

						switch (_at)
						{
							case 0:
							{
								p2->type.SetBYTECode(pReader->GetUChar());
								break;
							}
							case 1:
							{
								p2->startAt = pReader->GetLong();
								break;
							}
							default:
								break;
						}
					}

					m_Bullet.reset(p2);
					break;
				}
			case BULLET_TYPE_BULLET_BLIP:
			{
				pReader->Skip(5); // len + type + start attr

				Logic::BuBlip *pBuBlip = new Logic::BuBlip();
				pBuBlip->blip.fromPPTY(pReader);

				if (pBuBlip->blip.embed.IsInit())
				{
					m_Bullet.reset(pBuBlip);
				}
				else
				{//??? сбой ???
					delete pBuBlip;

					Logic::BuChar *pBuChar = new Logic::BuChar();
					pBuChar->Char = wchar_t(L'\x2022');
					m_Bullet.reset(pBuChar);
				}
			}break;
			default:
				m_Bullet.reset(new Logic::BuNone());
				break;
			}

			pReader->Seek(_end_rec);
		}
		void Bullet::FillParentPointersForChilds(){}
		void Bullet::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				m_Bullet->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
