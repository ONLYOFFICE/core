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

#include "UniPath2D.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType UniPath2D::getType() const
		{
			if (Path2D.IsInit())
				return Path2D->getType();
			return OOX::et_Unknown;
		}
		void UniPath2D::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());

			if (name == _T("moveTo"))
				Path2D.reset(new Logic::MoveTo(oReader));
			else if (name == _T("lnTo"))
				Path2D.reset(new Logic::LineTo(oReader));
			else if (name == _T("cubicBezTo"))
				Path2D.reset(new Logic::CubicBezTo(oReader));
			else if (name == _T("close"))
				Path2D.reset(new Logic::Close(oReader));
			else if (name == _T("arcTo"))
				Path2D.reset(new Logic::ArcTo(oReader));
			else if (name == _T("quadBezTo"))
				Path2D.reset(new Logic::QuadBezTo(oReader));
			else Path2D.reset();
		}
		void UniPath2D::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("moveTo"))
				Path2D.reset(new Logic::MoveTo(node));
			else if (name == _T("lnTo"))
				Path2D.reset(new Logic::LineTo(node));
			else if (name == _T("cubicBezTo"))
				Path2D.reset(new Logic::CubicBezTo(node));
			else if (name == _T("close"))
				Path2D.reset(new Logic::Close(node));
			else if (name == _T("arcTo"))
				Path2D.reset(new Logic::ArcTo(node));
			else if (name == _T("quadBezTo"))
				Path2D.reset(new Logic::QuadBezTo(node));
			else Path2D.reset();
		}
		void UniPath2D::GetPath2DFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;

			if(element.GetNode(_T("a:moveTo"), oNode))
				Path2D.reset(new Logic::MoveTo(oNode));
			else if(element.GetNode(_T("a:lnTo"), oNode))
				Path2D.reset(new Logic::LineTo(oNode));
			else if(element.GetNode(_T("a:cubicBezTo"), oNode))
				Path2D.reset(new Logic::CubicBezTo(oNode));
			else if(element.GetNode(_T("a:close"), oNode))
				Path2D.reset(new Logic::Close(oNode));
			else if(element.GetNode(_T("a:arcTo"), oNode))
				Path2D.reset(new Logic::ArcTo(oNode));
			else if(element.GetNode(_T("a:quadBezTo"), oNode))
				Path2D.reset(new Logic::QuadBezTo(oNode));
			else Path2D.reset();
		}
		std::wstring UniPath2D::toXML() const
		{
			if (Path2D.IsInit())
				return Path2D->toXML();
			return _T("");
		}
		void UniPath2D::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (Path2D.is_init())
				Path2D->toPPTY(pWriter);
		}
		void UniPath2D::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (Path2D.is_init())
				Path2D->toXmlWriter(pWriter);
		}
		bool UniPath2D::is_init()const{return (Path2D.IsInit());};
		void UniPath2D::FillParentPointersForChilds(){}
		void UniPath2D::SetParentPointer(const WrapperWritingElement* pParent)
		{
			if(is_init())
				Path2D->SetParentPointer(pParent);
		}
		std::wstring UniPath2D::GetODString() const
		{
			return Path2D->GetODString();
		}
	} // namespace Logic
} // namespace PPTX
