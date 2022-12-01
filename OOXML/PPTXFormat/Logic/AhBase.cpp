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

#include "AhBase.h"

namespace PPTX
{
	namespace Logic
	{		
		OOX::EElementType AhBase::getType() const
		{
			if (ah.IsInit())
				return ah->getType();
			return OOX::et_Unknown;
		}
		void AhBase::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("ahXY"))
				ah.reset(new Logic::AhXY(node));
			else if (name == _T("ahPolar"))
				ah.reset(new Logic::AhPolar(node));
			else ah.reset();
		}
		void AhBase::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (sName == L"ahXY")
				ah.reset(new Logic::AhXY(oReader));
			else if(sName == L"ahPolar")
				ah.reset(new Logic::AhPolar(oReader));
			else ah.reset();
		}
		void AhBase::GetAdjustHandleFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:ahXY"), oNode))
				ah.reset(new Logic::AhXY(oNode));
			else if(element.GetNode(_T("a:ahPolar"), oNode))
				ah.reset(new Logic::AhPolar(oNode));
			else ah.reset();
		}
		std::wstring AhBase::toXML() const
		{
			if (ah.is_init())
				return ah->toXML();

			return _T("");
		}
		void AhBase::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (ah.is_init())
				ah->toPPTY(pWriter);
		}
		void AhBase::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (ah.is_init())
				ah->toXmlWriter(pWriter);
		}
		bool AhBase::is_init() const
		{
			return (ah.is_init());
		}
		void AhBase::FillParentPointersForChilds()
		{
		}
		void AhBase::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				ah->SetParentPointer(pParent);
		}
		std::wstring AhBase::GetODString()const
		{
			if (!ah.IsInit())
				return _T("");
			return ah->GetODString();
		}
	} // namespace Logic
} // namespace PPTX
