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

#include "Blur.h"

namespace PPTX
{
	namespace Logic
	{
		Blur& Blur::operator=(const Blur& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			rad  = oSrc.rad;
			grow = oSrc.grow;
			return *this;
		}
		OOX::EElementType Blur::getType() const
		{
			return OOX::et_a_blur;
		}
		void Blur::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void Blur::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("rad"), rad)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("grow"), grow)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Blur::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"rad", rad);
			XmlMacroReadAttributeBase(node, L"grow", grow);
		}
		std::wstring Blur::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("rad"), rad);
			oAttr.Write(_T("grow"), grow);

			return XmlUtils::CreateNode(_T("a:blur"), oAttr);
		}
		void Blur::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:blur");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"rad", rad);
			pWriter->WriteAttribute(L"grow", grow);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:blur");
		}
		void Blur::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_BLUR);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteSize_t2(0, rad);
			pWriter->WriteBool2(1, grow);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void Blur::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					case 0:	rad = (size_t)pReader->GetULong(); break;
					case 1:	grow = pReader->GetBool(); break;
				}
			}
			pReader->Seek(_e);
		}
		void Blur::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
