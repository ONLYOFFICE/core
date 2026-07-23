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

#include "BuSzPts.h"

namespace PPTX
{
	namespace Logic
	{
		BuSzPts& BuSzPts::operator=(const BuSzPts& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			val = oSrc.val;
			return *this;
		}
		OOX::EElementType BuSzPts::getType() const
		{
			return OOX::et_a_buSzPts;
		}
		void BuSzPts::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void BuSzPts::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"val", val)
			WritingElement_ReadAttributes_End( oReader )
		}
		void BuSzPts::fromXML(XmlUtils::CXmlNode& node)
		{
			val = node.ReadAttributeInt(L"val");
		}
		void BuSzPts::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:buSzPts");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"val", val);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:buSzPts");
		}
		void BuSzPts::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(BULLET_TYPE_SIZE_PTS);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt1(0, val.get_value_or(0));
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void BuSzPts::FillParentPointersForChilds(){}
		void BuSzPts::Normalize()
		{
			if (val.IsInit())
			{
				int tmp = *val;
				normalize_value(tmp, 100, 400000);
				val = tmp;
			}
		}
	} // namespace Logic
} // namespace PPTX
