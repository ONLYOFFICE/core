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

#include "TintEffect.h"

namespace PPTX
{
	namespace Logic
	{
		TintEffect& TintEffect::operator=(const TintEffect& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			amt = oSrc.amt;
			hue = oSrc.hue;
			return *this;
		}
		OOX::EElementType TintEffect::getType() const
		{
			return OOX::et_a_tint;
		}
		void TintEffect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void TintEffect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("amt"), amt)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("hue"), hue)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void TintEffect::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"amt", amt);
			XmlMacroReadAttributeBase(node, L"hue", hue);

			Normalize();
		}
		std::wstring TintEffect::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("hue"), hue);
			oAttr.Write(_T("amt"), amt);

			return XmlUtils::CreateNode(_T("a:tint"), oAttr);
		}
		void TintEffect::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:tint");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"hue", hue);
			pWriter->WriteAttribute(L"amt", amt);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:tint");
		}
		void TintEffect::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_TINTEFFECT);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, amt);
			pWriter->WriteInt2(1, hue);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void TintEffect::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar();
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:	amt = pReader->GetLong(); break;
					case 1:	hue = pReader->GetLong(); break;
				}
			}
			pReader->Seek(_e);
		}
		void TintEffect::FillParentPointersForChilds(){}
		void TintEffect::Normalize()
		{
			hue.normalize(0, 21600000);
		}
	} // namespace Logic
} // namespace PPTX
