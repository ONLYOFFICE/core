/*
 * (c) Copyright Ascensio System SIA 2010-2023
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

#include "WrapperWritingElement.h"

namespace PPTX
{
	WrapperWritingElement::WrapperWritingElement(OOX::Document *pMain) :  OOX::WritingElement(pMain), parentElement(NULL), parentFile(NULL)
	{
	}
	WrapperWritingElement::~WrapperWritingElement()
	{
	}
	void WrapperWritingElement::FillParentPointersForChilds()
	{
	}
	void WrapperWritingElement::SetParentPointer(const WrapperWritingElement* pParent)
	{
		parentElement	= pParent;
		parentFile		= parentElement->parentFile;

		FillParentPointersForChilds();
	}
	void WrapperWritingElement::SetParentFilePointer(const WrapperFile* pFile)
	{
		parentFile = pFile;
		FillParentPointersForChilds();
	}
	WrapperWritingElement const* const	WrapperWritingElement::GetParentPointer() const
	{
		return parentElement;
	}
	WrapperFile const* const WrapperWritingElement::GetParentFilePointer() const
	{
		return parentFile;
	}
	void WrapperWritingElement::fromXMLString(std::wstring strXml)
	{
		XmlUtils::CXmlNode oNode;
		oNode.FromXmlString(strXml);
		fromXML(oNode);
	}
	OOX::EElementType WrapperWritingElement::getType() const
	{
		return OOX::et_Unknown;
	}

	void WrapperWritingElement::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
	}
	std::wstring WrapperWritingElement::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
} // namespace PPTX
