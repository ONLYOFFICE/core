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

#include "TimeNodeBase.h"
#include "Seq.h"
#include "Par.h"
#include "Audio.h"
#include "Video.h"
#include "Excl.h"
#include "Anim.h"
#include "AnimClr.h"
#include "AnimEffect.h"
#include "AnimMotion.h"
#include "AnimRot.h"
#include "AnimScale.h"
#include "Cmd.h"
#include "Set.h"

namespace PPTX
{
	namespace Logic
	{
		TimeNodeBase::TimeNodeBase()
		{
		}
		TimeNodeBase::~TimeNodeBase()
		{
		}
		TimeNodeBase::TimeNodeBase(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const TimeNodeBase& TimeNodeBase::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		void TimeNodeBase::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if(name == L"seq")
				m_node.reset(new Logic::Seq(node));
			else if(name == L"par")
				m_node.reset(new Logic::Par(node));
			else if(name == L"audio")
				m_node.reset(new Logic::Audio(node));
			else if(name == L"video")
				m_node.reset(new Logic::Video(node));
			else if(name == L"excl")
				m_node.reset(new Logic::Excl(node));
			else if(name == L"anim")
				m_node.reset(new Logic::Anim(node));
			else if(name == L"animClr")
				m_node.reset(new Logic::AnimClr(node));
			else if(name == L"animEffect")
				m_node.reset(new Logic::AnimEffect(node));
			else if(name == L"animMotion")
				m_node.reset(new Logic::AnimMotion(node));
			else if(name == L"animRot")
				m_node.reset(new Logic::AnimRot(node));
			else if(name == L"animScale")
				m_node.reset(new Logic::AnimScale(node));
			else if(name == L"cmd")
				m_node.reset(new Logic::Cmd(node));
			else if(name == L"set")
				m_node.reset(new Logic::Set(node));
			else m_node.reset();
		}
		void TimeNodeBase::GetTimeNodeFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;			
			if(element.GetNode(L"p:seq", oNode))
				m_node.reset(new Logic::Seq(oNode));
			else if(element.GetNode(L"p:par", oNode))
				m_node.reset(new Logic::Par(oNode));
			else if(element.GetNode(L"p:audio", oNode))
				m_node.reset(new Logic::Audio(oNode));
			else if(element.GetNode(L"p:video", oNode))
				m_node.reset(new Logic::Video(oNode));
			else if(element.GetNode(L"p:excl", oNode))
				m_node.reset(new Logic::Excl(oNode));
			else if(element.GetNode(L"p:anim", oNode))
				m_node.reset(new Logic::Anim(oNode));
			else if(element.GetNode(L"p:animClr", oNode))
				m_node.reset(new Logic::AnimClr(oNode));
			else if(element.GetNode(L"p:animEffect", oNode))
				m_node.reset(new Logic::AnimEffect(oNode));
			else if(element.GetNode(L"p:animMotion", oNode))
				m_node.reset(new Logic::AnimMotion(oNode));
			else if(element.GetNode(L"p:animRot", oNode))
				m_node.reset(new Logic::AnimRot(oNode));
			else if(element.GetNode(L"p:animScale", oNode))
				m_node.reset(new Logic::AnimScale(oNode));
			else if(element.GetNode(L"p:cmd", oNode))
				m_node.reset(new Logic::Cmd(oNode));
			else if(element.GetNode(L"p:set", oNode))
				m_node.reset(new Logic::Set(oNode));
			else m_node.reset();
		}
		std::wstring TimeNodeBase::toXML() const
		{
			if (m_node.IsInit())
				return m_node->toXML();
			return L"";
		}
		void TimeNodeBase::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (false == m_node.IsInit()) return;

			m_node->toXmlWriter(pWriter);
		}
		void TimeNodeBase::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			BYTE _type = pReader->GetUChar();
			
			switch (_type)
			{
				case 1:	m_node = new Logic::Par(); break;
				case 2:	m_node = new Logic::Seq(); break;
				case 3:	m_node = new Logic::Audio(); break;
				case 4:	m_node = new Logic::Video(); break;
				case 5:	m_node = new Logic::Excl(); break;
				case 6:	m_node = new Logic::Anim(); break;
				case 7:	m_node = new Logic::AnimClr(); break;
				case 8:	m_node = new Logic::AnimEffect(); break;
				case 9:	m_node = new Logic::AnimMotion(); break;
				case 10: m_node = new Logic::AnimRot(); break;
				case 11: m_node = new Logic::AnimScale(); break;
				case 12: m_node = new Logic::Cmd(); break;
				case 13: m_node = new Logic::Set(); break;
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
		void TimeNodeBase::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			switch (m_node->getType())
			{
				case OOX::et_p_par:			pWriter->StartRecord(1); break;
				case OOX::et_p_seq:			pWriter->StartRecord(2); break;
				case OOX::et_p_audio:		pWriter->StartRecord(3); break;
				case OOX::et_p_video:		pWriter->StartRecord(4); break;
				case OOX::et_p_excl:		pWriter->StartRecord(5); break;
				case OOX::et_p_anim:		pWriter->StartRecord(6); break;
				case OOX::et_p_animClr:		pWriter->StartRecord(7); break;
				case OOX::et_p_animEffect:	pWriter->StartRecord(8); break;
				case OOX::et_p_animMotion:	pWriter->StartRecord(9); break;
				case OOX::et_p_animRot:		pWriter->StartRecord(10); break;
				case OOX::et_p_animScale:	pWriter->StartRecord(11); break;
				case OOX::et_p_cmd:			pWriter->StartRecord(12); break;
				case OOX::et_p_set:			pWriter->StartRecord(13); break;
				default:					pWriter->StartRecord(0); break;
			}
			if (m_node.IsInit())
				m_node->toPPTY(pWriter);

			pWriter->EndRecord();
		}
		void TimeNodeBase::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(m_node.IsInit())
				m_node->SetParentPointer(pParent);
		}
		void TimeNodeBase::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
