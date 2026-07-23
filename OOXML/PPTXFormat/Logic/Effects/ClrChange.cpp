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

#include "ClrChange.h"

namespace PPTX
{
	namespace Logic
	{
		ClrChange& ClrChange::operator=(const ClrChange& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			ClrFrom	= oSrc.ClrFrom;
			ClrTo	= oSrc.ClrTo;
			useA	= oSrc.useA;
			return *this;
		}
		OOX::EElementType ClrChange::getType() const
		{
			return OOX::et_a_clrChange;
		}
		void ClrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:clrTo")
					ClrTo.fromXMLParent(oReader);
				else if (strName == L"a:clrFrom")
					ClrFrom.fromXMLParent(oReader);
			}
			FillParentPointersForChilds();
		}
		void ClrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("useA"), useA)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void ClrChange::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode node1 = node.ReadNode(_T("a:clrFrom"));
			ClrFrom.GetColorFrom(node1);

			node1 = node.ReadNode(_T("a:clrTo"));
			ClrTo.GetColorFrom(node1);

			XmlMacroReadAttributeBase(node, L"useA", useA);
			FillParentPointersForChilds();
		}
		std::wstring ClrChange::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("useA"), useA);

			XmlUtils::CNodeValue oValue;
			oValue.Write(_T("a:clrFrom"), ClrFrom);
			oValue.Write(_T("a:clrTo"), ClrTo);

			return XmlUtils::CreateNode(_T("a:clrChange"), oAttr, oValue);
		}
		void ClrChange::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:clrChange");
			pWriter->EndAttributes();
				pWriter->StartNode(L"a:clrFrom");
					pWriter->EndAttributes();
					ClrFrom.toXmlWriter(pWriter);
				pWriter->EndNode(L"a:clrFrom");
				pWriter->StartNode(L"a:clrTo");
					pWriter->EndAttributes();
					ClrTo.toXmlWriter(pWriter);
				pWriter->EndNode(L"a:clrTo");
			pWriter->EndNode(L"a:clrChange");
		}
		void ClrChange::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_CLRCHANGE);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, useA);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, ClrFrom);
			pWriter->WriteRecord1(1, ClrTo);

			pWriter->EndRecord();
		}
		void ClrChange::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar();
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
						useA = pReader->GetBool(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						ClrFrom.fromPPTY(pReader);
					}break;
					case 1:
					{
						ClrTo.fromPPTY(pReader);
					}break;
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void ClrChange::FillParentPointersForChilds()
		{
			ClrFrom.SetParentPointer(this);
			ClrTo.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
