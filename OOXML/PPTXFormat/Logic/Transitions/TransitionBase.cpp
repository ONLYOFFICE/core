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


#include "TransitionBase.h"
#include "EmptyTransition.h"
#include "OrientationTransition.h"
#include "EightDirectionTransition.h"
#include "OptionalBlackTransition.h"
#include "SideDirectionTransition.h"
#include "CornerDirectionTransition.h"
#include "WheelTransition.h"
#include "SplitTransition.h"
#include "ZoomTransition.h"

namespace PPTX
{
	namespace Logic
	{
		TransitionBase::TransitionBase()
		{
		}
		TransitionBase::~TransitionBase()
		{
		}
		TransitionBase::TransitionBase(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const TransitionBase& TransitionBase::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		void TransitionBase::fromXML(XmlUtils::CXmlNode& oNode)
		{
			std::wstring strName = oNode.GetName();
	
				if (	strName == L"p:blinds"	|| 
						strName == L"p:checker" || 
						strName == L"p:comb"	|| 
						strName == L"p:randomBar")
			{
				base.reset(new Logic::OrientationTransition(oNode));
			}
			else if (	strName == L"p:cover"	|| 
						strName == L"p:pull")
			{
				base.reset(new Logic::EightDirectionTransition(oNode));
			}
			else if (	strName == L"p:cut"	|| 
						strName == L"p:fade")
			{
				base.reset(new Logic::OptionalBlackTransition(oNode));
			}
			else if (	strName == L"p:push"	|| 
						strName == L"p:wipe")
			{
				base.reset(new Logic::SideDirectionTransition(oNode));
			}
			else if (	strName == L"p:strips")
			{
				base.reset(new Logic::CornerDirectionTransition(oNode));
			}
			else if (	strName == L"p:wheel")
			{
				base.reset(new Logic::WheelTransition(oNode));
			}
			else if (	strName == L"p:split")
			{
				base.reset(new Logic::SplitTransition(oNode));
			}
			else if (	strName == L"p:zoom")
			{
				base.reset(new Logic::ZoomTransition(oNode));
			}
			else
			{
				base.reset(new Logic::EmptyTransition(oNode));
			}
		}
		std::wstring TransitionBase::toXML() const
		{
			if (base.IsInit())
				return base->toXML();
			
			return _T("");
		}
		void TransitionBase::FillParentPointersForChilds(){}
		void TransitionBase::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				base->SetParentPointer(pParent);
		}

		//////////////////////////////////////////////////////////////////
		TransitionSerialize::TransitionSerialize()
		{
		}
		TransitionSerialize::~TransitionSerialize()
		{
		}
		TransitionSerialize::TransitionSerialize(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const TransitionSerialize& TransitionSerialize::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		TransitionSerialize& TransitionSerialize::operator =(const TransitionSerialize& src)
		{
			//проверка на самоприсваивание
			if (this == &src) {
				return *this;
			}
			m_strNodeName = src.m_strNodeName;
			m_strAttributesNames = src.m_strAttributesNames;
			m_strAttributesValues = src.m_strAttributesValues;
			return *this;
		}
		void TransitionSerialize::fromXML(XmlUtils::CXmlNode& node)
		{
			m_strNodeName = node.GetName();
            node.GetAllAttributes(m_strAttributesNames, m_strAttributesValues);
		}
		std::wstring TransitionSerialize::toXML() const
		{	
			return _T("");
		}
		void TransitionSerialize::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						m_strNodeName = pReader->GetString2();
						break;
					}
					case 1:
					{
						m_strAttributesNames.push_back(pReader->GetString2());
						break;
					}
					case 2:
					{
						m_strAttributesValues.push_back(pReader->GetString2());
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TransitionSerialize::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			pWriter->WriteString1(0, m_strNodeName);


			std::list<std::wstring>::const_iterator pos1 = m_strAttributesNames.begin();
			while (pos1 != m_strAttributesNames.end())
			{
				const std::wstring& s = *pos1;
				pWriter->WriteString1(1, s);
				pos1++;
			}

			std::list<std::wstring>::const_iterator pos2 = m_strAttributesValues.begin();
			while (pos2 != m_strAttributesValues.end())
			{
				const std::wstring& s = *pos2;;
				pWriter->WriteString1(2, s);
				pos2++;
			}

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void TransitionSerialize::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_strNodeName);

			pWriter->StartAttributes();

			//и нафига тогда листы?? - примем аксиомно что размеры одинаковые
			//size_t c1 = m_strAttributesNames.GetCount();
			//size_t c2 = m_strAttributesValues.GetCount();

			//if (c1 == c2)
			{
				std::list<std::wstring>::const_iterator pos1 = m_strAttributesNames.begin();
				std::list<std::wstring>::const_iterator pos2 = m_strAttributesValues.begin();

				while (pos1 != m_strAttributesNames.end() && pos2 != m_strAttributesValues.end())
				{
					const std::wstring& s1 = *pos1;
					const std::wstring& s2 = *pos2;

					pWriter->WriteAttribute(s1, s2);

					pos1++;
					pos2++;
				}
			}

			pWriter->EndAttributes();

			pWriter->EndNode(m_strNodeName);
		}
		void TransitionSerialize::toXmlWriterOld(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring name = m_strNodeName;
			if ((name != _T("p:random")) &&
				(name != _T("p:circle")) &&
				(name != _T("p:dissolve")) &&
				(name != _T("p:diamond")) &&
				(name != _T("p:newsflash")) &&
				(name != _T("p:plus")) &&
				(name != _T("p:wedge")) &&
				(name != _T("p:blinds")) &&
				(name != _T("p:checker")) &&
				(name != _T("p:comb")) &&
				(name != _T("p:randomBar")) &&
				(name != _T("p:cover")) &&
				(name != _T("p:pull")) &&
				(name != _T("p:cut")) &&
				(name != _T("p:fade")) &&
				(name != _T("p:push")) &&
				(name != _T("p:wipe")) &&
				(name != _T("p:strips")) &&
				(name != _T("p:wheel")) &&
				(name != _T("p:split")) &&
				(name != _T("p:zoom")))
			{
				pWriter->WriteString(_T("<p:fade/>"));
				return;
			}

			pWriter->StartNode(name);

			pWriter->StartAttributes();

			//size_t c1 = m_strAttributesNames.GetCount();
			//size_t c2 = m_strAttributesValues.GetCount();

			//if (c1 == c2)
			{
				std::list<std::wstring>::const_iterator pos1 = m_strAttributesNames.begin();
				std::list<std::wstring>::const_iterator pos2 = m_strAttributesValues.begin();

				while (pos1 != m_strAttributesNames.end() && pos2 != m_strAttributesValues.end())
				{
					const std::wstring& s1 = *pos1;
					const std::wstring& s2 = *pos2;

					pWriter->WriteAttribute(s1, s2);

					pos1++;
					pos2++;
				}
			}

			pWriter->EndAttributes();

			pWriter->EndNode(name);
		}
		void TransitionSerialize::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
