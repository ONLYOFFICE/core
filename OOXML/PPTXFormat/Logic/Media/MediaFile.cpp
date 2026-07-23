/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "MediaFile.h"

namespace PPTX
{
	namespace Logic
	{
		MediaFile::MediaFile() {}
		MediaFile::MediaFile(std::wstring name_)
		{
			name = name_;
		}
		MediaFile& MediaFile::operator=(const MediaFile& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			name = oSrc.name;
			link = oSrc.link;
			contentType = oSrc.contentType;

			return *this;
		}
		void MediaFile::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			name = XmlUtils::GetNameNoNS(oReader.GetName());

			ReadAttributes(oReader);
		}
		void MediaFile::fromXML(XmlUtils::CXmlNode& node)
		{
			name		= XmlUtils::GetNameNoNS(node.GetName());
			link		= node.GetAttribute(_T("r:link"));
			XmlMacroReadAttributeBase(node, L"contentType", contentType);
		}
		void MediaFile::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"r:link",	link)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"contentType",	contentType)
			WritingElement_ReadAttributes_End( oReader )
		}
		std::wstring MediaFile::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"r:link", link.ToString());
			oAttr.Write(L"contentType", contentType);

			return XmlUtils::CreateNode(L"a:" + name, oAttr);
		}
		void MediaFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:" + name);
				pWriter->StartAttributes();
					pWriter->WriteAttribute (L"r:link", link.ToString());
					pWriter->WriteAttribute (L"contentType", contentType);
				pWriter->EndAttributes();
			pWriter->EndNode(L"a:" + name);
		}
		void MediaFile::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
