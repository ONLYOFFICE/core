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

#include "BuAutoNum.h"

namespace PPTX
{
	namespace Logic
	{
		BuAutoNum& BuAutoNum::operator=(const BuAutoNum& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			type	= oSrc.type;
			startAt = oSrc.startAt;

			return *this;
		}
		void BuAutoNum::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		OOX::EElementType BuAutoNum::getType() const
		{
			return OOX::et_a_buChar;
		}
		void BuAutoNum::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"type", type);
			XmlMacroReadAttributeBase(node, L"startAt", startAt);

			Normalize();
		}
		void BuAutoNum::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if	  ( oReader, L"startAt",	startAt)
				WritingElement_ReadAttributes_Read_else_if( oReader, L"type",		type)
			WritingElement_ReadAttributes_End	( oReader )

			Normalize();
		}
		void BuAutoNum::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:buAutoNum");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"type", type.get());
			pWriter->WriteAttribute(L"startAt", startAt);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:buAutoNum");
		}
		void BuAutoNum::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(BULLET_TYPE_BULLET_AUTONUM);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit1(0, type);
			pWriter->WriteInt2(1, startAt);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void BuAutoNum::Normalize()
		{
			startAt.normalize(1, 32767);
		}
		void BuAutoNum::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
