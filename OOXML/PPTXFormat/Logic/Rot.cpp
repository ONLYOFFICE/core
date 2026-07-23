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

#include "Rot.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType Rot::getType() const
		{
			return OOX::et_a_rot;
		}
		void Rot::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void Rot::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"lat", lat)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"lon", lon)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"rev", rev)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Rot::fromXML(XmlUtils::CXmlNode& node)
		{
			lat = node.GetAttribute(L"lat");
			lon = node.GetAttribute(L"lon");
			rev = node.GetAttribute(L"rev");
		}
		std::wstring Rot::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"lat", lat);
			oAttr.Write(L"lon", lon);
			oAttr.Write(L"rev", rev);

			return XmlUtils::CreateNode(L"a:rot", oAttr);
		}
		void Rot::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = L"w14:";
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = L"a:";

			pWriter->StartNode(sNodeNamespace + L"rot");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"lat", lat);
			pWriter->WriteAttribute(sAttrNamespace + L"lon", lon);
			pWriter->WriteAttribute(sAttrNamespace + L"rev", rev);
			pWriter->EndAttributes();

			pWriter->EndNode(sNodeNamespace + L"rot");
		}
		void Rot::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, lat);
			pWriter->WriteInt2(1, lon);
			pWriter->WriteInt2(2, rev);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Rot::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		lat	= pReader->GetLong();
				else if (1 == _at)	lon	= pReader->GetLong();
				else if (2 == _at)	rev = pReader->GetLong();
				else
					break;
			}
			pReader->Seek(_end_rec);
		}
		void Rot::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
