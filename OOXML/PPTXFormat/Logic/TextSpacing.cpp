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

#include "TextSpacing.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType TextSpacing::getType () const
		{
			return OOX::et_Unknown;
		}
		void TextSpacing::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				nullable_int val;
				if (sName == L"a:spcPct")
				{
					ReadAttributes(oReader, val);
					spcPct = val;
				}
				if (sName == L"a:spcPts")
				{
					ReadAttributes(oReader, val);
					spcPts = val;
				}
			}
			Normalize();
		}
		void TextSpacing::ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable_int & val)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"val", val)
			WritingElement_ReadAttributes_End	( oReader )
		}
		void TextSpacing::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();

			XmlUtils::CXmlNode node1 = node.ReadNode(L"a:spcPct");
			XmlMacroReadAttributeBase(node1, L"val", spcPct);

			XmlUtils::CXmlNode node2 = node.ReadNode(L"a:spcPts");
			XmlMacroReadAttributeBase(node2, L"val", spcPts);

			Normalize();
		}
		void TextSpacing::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);
			pWriter->EndAttributes();

			if (spcPct.is_init())
			{
				pWriter->StartNode(_T("a:spcPct"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("val"), *spcPct);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:spcPct"));
			}
			else
			{
				pWriter->StartNode(_T("a:spcPts"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("val"), spcPts.get_value_or(0));
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:spcPts"));
			}

			pWriter->EndNode(m_name);
		}
		void TextSpacing::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, spcPct);
			pWriter->WriteInt2(1, spcPts);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void TextSpacing::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						spcPct = pReader->GetLong();
						break;
					}
					case 1:
					{
						spcPts = pReader->GetLong();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		int TextSpacing::GetVal()const
		{
			if(spcPts.is_init())
				return -spcPts.get();
			else
				return spcPct.get_value_or(100000)/1000;
		}
		void TextSpacing::FillParentPointersForChilds(){}
		void TextSpacing::Normalize()
		{
			spcPts.normalize(0, 158400);
		}
	} // namespace Logic
} // namespace PPTX
