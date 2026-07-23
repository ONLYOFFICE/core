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

#include "PrstShdw.h"

namespace PPTX
{
	namespace Logic
	{
		PrstShdw& PrstShdw::operator=(const PrstShdw& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Color	= oSrc.Color;

			prst	= oSrc.prst;
			dir		= oSrc.dir;
			dist	= oSrc.dist;

			return *this;
		}
		OOX::EElementType PrstShdw::getType() const
		{
			return OOX::et_a_prstShdw;
		}
		void PrstShdw::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();
				Color.fromXML(oReader);
			}

			Normalize();
			FillParentPointersForChilds();
		}
		void PrstShdw::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("prst"), prst)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dir"),	 dir)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dist"), dist)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void PrstShdw::fromXML(XmlUtils::CXmlNode& node)
		{
			Color.GetColorFrom(node);
			prst	= node.GetAttribute(L"prst");
			XmlMacroReadAttributeBase(node, L"dir", dir);
			XmlMacroReadAttributeBase(node, L"dist", dist);

			FillParentPointersForChilds();
		}
		std::wstring PrstShdw::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("prst"), prst.get());
			oAttr.Write(_T("dist"), dist);
			oAttr.Write(_T("dir"), dir);

			XmlUtils::CNodeValue oValue;
			oValue.Write(Color);

			return XmlUtils::CreateNode(_T("a:prstShdw"), oAttr, oValue);
		}
		void PrstShdw::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:prstShdw");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"dist", dist);
			pWriter->WriteAttribute(L"dir", dir);
			pWriter->WriteAttribute(L"prst", prst.get());
			pWriter->EndAttributes();

			if (Color.is_init())
			{
				Color.toXmlWriter(pWriter);
			}
			else
			{
				pWriter->StartNode(L"a:scrgbClr");
				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"r", 0);
				pWriter->WriteAttribute(L"g", 0);
				pWriter->WriteAttribute(L"b", 0);
				pWriter->EndAttributes();
				pWriter->EndNode(L"a:scrgbClr");
			}

			pWriter->EndNode(L"a:prstShdw");
		}
		void PrstShdw::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_PRSTSHDW);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, dir);
			pWriter->WriteInt2(1, dist);
			pWriter->WriteLimit1(2, prst);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, Color);

			pWriter->EndRecord();
		}
		void PrstShdw::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					case 0:	dir		= pReader->GetLong(); break;
					case 1:	dist	= pReader->GetLong(); break;
					case 2:	prst.SetBYTECode( pReader->GetUChar()); break;
				}

			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						Color.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void PrstShdw::FillParentPointersForChilds()
		{
			Color.SetParentPointer(this);
		}
		void PrstShdw::Normalize()
		{
			dir.normalize(0, 21600000);
			dist.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
