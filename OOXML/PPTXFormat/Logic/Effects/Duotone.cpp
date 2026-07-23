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

#include "Duotone.h"

namespace PPTX
{
	namespace Logic
	{
		Duotone& Duotone::operator=(const Duotone& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Colors = oSrc.Colors;
			return *this;
		}
		OOX::EElementType Duotone::getType() const
		{
			return OOX::et_a_duotone;
		}
		void Duotone::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				UniColor col;
				Colors.push_back(col);
				Colors.back().fromXML(oReader);
			}
			FillParentPointersForChilds();
		}
		void Duotone::fromXML(XmlUtils::CXmlNode& node)
		{
			Colors.clear();
			XmlMacroLoadArray(node, L"*", Colors, UniColor);
			FillParentPointersForChilds();
		}
		std::wstring Duotone::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Colors);

			return XmlUtils::CreateNode(L"a:duotone", oValue);
		}
		void Duotone::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:duotone");
			pWriter->EndAttributes();

			pWriter->WriteArray2(Colors);

			pWriter->EndNode(L"a:duotone");
		}
		void Duotone::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar();
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			ULONG count = pReader->GetULong();
			for (ULONG i = 0; i < count; ++i)
			{
				pReader->Skip(1); // type

				Colors.push_back(UniColor());
				Colors.back().fromPPTY(pReader);

				if (false == Colors.back().is_init())
				{
					Colors.pop_back();
				}
			}
			pReader->Seek(_e);
		}
		void Duotone::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_DUOTONE);

			ULONG len = (ULONG)Colors.size();
			pWriter->WriteULONG(len);

			for (ULONG i = 0; i < len; ++i)
			{
				pWriter->WriteRecord1(0, Colors[i]);
			}

			pWriter->EndRecord();
		}
		void Duotone::FillParentPointersForChilds()
		{
			size_t count = Colors.size();
			for (size_t i = 0; i < count; ++i)
				Colors[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
