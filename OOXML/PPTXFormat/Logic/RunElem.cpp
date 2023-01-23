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

#include "RunElem.h"

namespace PPTX
{
	namespace Logic
	{
		RunElem::RunElem() {}
		OOX::EElementType RunElem::getType () const
		{
			if (Elem.IsInit())
				return Elem->getType();
			return OOX::et_Unknown;
		}
		void RunElem::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());

			if(name == _T("r"))
				Elem.reset(new Logic::Run(oReader));
			else if(name == _T("fld"))
				Elem.reset(new Logic::Fld(oReader));
			else if(name == _T("br"))
				Elem.reset(new Logic::Br(oReader));
			else if(name == _T("m"))
				Elem.reset(new Logic::MathParaWrapper(oReader));
			else
				Elem.reset();
		}
		void RunElem::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if(name == _T("r"))
				Elem.reset(new Logic::Run(node));
			else if(name == _T("fld"))
				Elem.reset(new Logic::Fld(node));
			else if(name == _T("br"))
				Elem.reset(new Logic::Br(node));
			else if(name == _T("m"))
				Elem.reset(new Logic::MathParaWrapper(node));
			else
				Elem.reset();
		}
		std::wstring RunElem::toXML() const
		{
			if (is_init())
				return Elem->toXML();
			return _T("");
		}
		void RunElem::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (is_init())
				Elem->toXmlWriter(pWriter);
		}
		void RunElem::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (Elem.is_init())
				Elem->toPPTY(pWriter);
		}
		void RunElem::InitRun(RunBase* pRun)
		{
			Elem.reset(pRun);
		}
		bool RunElem::is_init() const
		{
			return (Elem.IsInit());
		}
		std::wstring RunElem::GetText() const
		{
			return Elem->GetText();
		}
		smart_ptr<RunBase> RunElem::GetElem()
		{
			return Elem;
		}
		void RunElem::FillParentPointersForChilds()
		{
		}
		void RunElem::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				Elem->SetParentPointer(pParent);
		}
	} // namespace Logic
} // namespace PPTX
