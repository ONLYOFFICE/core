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

		//////////////////////////////////////////////////////////////////
		TransitionSerialize::TransitionSerialize(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const TransitionSerialize& TransitionSerialize::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
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
	} // namespace Logic
} // namespace PPTX
