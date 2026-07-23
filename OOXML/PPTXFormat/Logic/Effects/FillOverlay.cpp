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

#include "FillOverlay.h"

namespace PPTX
{
	namespace Logic
	{
		FillOverlay& FillOverlay::operator=(const FillOverlay& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Fill = oSrc.Fill;
			blend = oSrc.blend;
			return *this;
		}
		OOX::EElementType FillOverlay::getType() const
		{
			return OOX::et_a_fillOverlay;
		}
		void FillOverlay::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				Fill.fromXML(oReader);
			}
			FillParentPointersForChilds();
		}
		void FillOverlay::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("blend"), blend)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void FillOverlay::fromXML(XmlUtils::CXmlNode& node)
		{
			Fill.GetFillFrom(node);
			blend = node.GetAttribute(_T("blend"));

			FillParentPointersForChilds();
		}
		std::wstring FillOverlay::toXML() const
		{
			std::wstring str = _T("<a:fillOverlay blend=\"") + blend.get() + _T("\">");
			str += Fill.toXML();
			str += _T("</a:fillOverlay>");

			return str;
		}
		void FillOverlay::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:fillOverlay");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"blend", blend.get());
			pWriter->EndAttributes();

			Fill.toXmlWriter(pWriter);

			pWriter->EndNode(L"a:fillOverlay");
		}
		void FillOverlay::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_FILLOVERLAY);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit1(0, blend);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, Fill);

			pWriter->EndRecord();
		}
		void FillOverlay::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar();
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (_at == 0)
					blend.SetBYTECode(pReader->GetUChar());
				else break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						Fill.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void FillOverlay::FillParentPointersForChilds()
		{
			Fill.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
