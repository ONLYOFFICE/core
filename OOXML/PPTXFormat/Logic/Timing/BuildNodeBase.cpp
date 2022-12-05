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

#include "BuildNodeBase.h"
#include "BldP.h"
#include "BldDgm.h"
#include "BldGraphic.h"
#include "BldOleChart.h"

namespace PPTX
{
	namespace Logic
	{
		BuildNodeBase::BuildNodeBase()
		{
		}
		BuildNodeBase::~BuildNodeBase()
		{
		}
		BuildNodeBase::BuildNodeBase(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const BuildNodeBase& BuildNodeBase::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		void BuildNodeBase::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("bldP"))
				m_node.reset(new Logic::BldP(node));
			else if (name == _T("bldDgm"))
				m_node.reset(new Logic::BldDgm(node));
			else if (name == _T("bldGraphic"))
				m_node.reset(new Logic::BldGraphic(node));
			else if (name == _T("bldOleChart"))
				m_node.reset(new Logic::BldOleChart(node));
			else m_node.reset();
		}
		void BuildNodeBase::GetBuildNodeFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			
			if (element.GetNode(_T("p:bldP"), oNode))
				m_node.reset(new Logic::BldP(oNode));
			else if (element.GetNode(_T("p:bldDgm"), oNode))
				m_node.reset(new Logic::BldDgm(oNode));
			else if (element.GetNode(_T("p:bldGraphic"), oNode))
				m_node.reset(new Logic::BldGraphic(oNode));
			else if (element.GetNode(_T("p:bldOleChart"), oNode))
				m_node.reset(new Logic::BldOleChart(oNode));
			else m_node.reset();
		}
		std::wstring BuildNodeBase::toXML() const
		{
			if (m_node.IsInit())
				return m_node->toXML();
			return _T("");
		}
		void BuildNodeBase::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_node.IsInit())
				m_node->toXmlWriter(pWriter);
		}
		void BuildNodeBase::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			BYTE _type = pReader->GetUChar();

			switch (_type)
			{
				case 1:	m_node = new Logic::BldDgm(); break;
				case 2:	m_node = new Logic::BldOleChart(); break;
				case 3:	m_node = new Logic::BldGraphic(); break;
				case 4:	m_node = new Logic::BldP(); break;
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
		void BuildNodeBase::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			switch (m_node->getType())
			{
				case OOX::et_p_bldDgm:		pWriter->StartRecord(1); break;
				case OOX::et_p_bldOleChart:	pWriter->StartRecord(2); break;
				case OOX::et_p_bldGraphic:	pWriter->StartRecord(3); break;
				case OOX::et_p_bldP:		pWriter->StartRecord(4); break;
				default:					pWriter->StartRecord(0); break;
			}
			if (m_node.IsInit())
				m_node->toPPTY(pWriter);

			pWriter->EndRecord();
		}
		void BuildNodeBase::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				m_node->SetParentPointer(pParent);
		}
		void BuildNodeBase::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
