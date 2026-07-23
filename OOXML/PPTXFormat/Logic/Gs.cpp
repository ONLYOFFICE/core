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

#include "Gs.h"

namespace PPTX
{
	namespace Logic
	{
		Gs& Gs::operator=(const Gs& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			color	= oSrc.color;
			pos		= oSrc.pos;

			return *this;
		}
		OOX::EElementType Gs::getType () const
		{
			return OOX::et_a_gs;
		}
		void Gs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				color.fromXML(oReader);
				break;
			}
			FillParentPointersForChilds();
		}
		void Gs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			nullable_int tmp;
			WritingElement_ReadAttributes_Start_No_NS	( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("pos"), tmp)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			pos = tmp.get_value_or(0);
		}
		void Gs::fromXML(XmlUtils::CXmlNode& node)
		{
			pos		= node.ReadAttributeInt(L"pos");
			color.GetColorFrom(node);

			FillParentPointersForChilds();
		}
		std::wstring Gs::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("pos"), pos);

			return XmlUtils::CreateNode(_T("a:gs"), oAttr, color.toXML());
		}
		void Gs::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = _T("w14:");
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = _T("a:");
			pWriter->StartNode(sNodeNamespace + _T("gs"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + _T("pos"), pos);
			pWriter->EndAttributes();

			color.toXmlWriter(pWriter);

			pWriter->EndNode(sNodeNamespace + _T("gs"));
		}
		void Gs::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt1(0, pos);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, color);
		}
		void Gs::FillParentPointersForChilds()
		{
			color.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
