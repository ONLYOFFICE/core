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

#include "Cxn.h"

namespace PPTX
{
	namespace Logic
	{
		Cxn& Cxn::operator=(const Cxn& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			x	= oSrc.x;
			y	= oSrc.y;
			ang	= oSrc.ang;
			return *this;
		}
		OOX::EElementType Cxn::getType() const
		{
			return OOX::et_a_cxn;
		}
		void Cxn::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if (sName == L"a:pos")
				{
					ReadAttributes2(oReader);
				}
			}
		}
		void Cxn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("ang"), ang )
			WritingElement_ReadAttributes_End( oReader )
		}
		void Cxn::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("x"), x )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("y"), y )
			WritingElement_ReadAttributes_End( oReader )
		}
		void Cxn::fromXML(XmlUtils::CXmlNode& node)
		{
			ang = node.GetAttribute(_T("ang"));

			XmlUtils::CXmlNode oPos;
			if (node.GetNode(_T("a:pos"), oPos))
			{
				x = oPos.GetAttribute(_T("x"));
				y = oPos.GetAttribute(_T("y"));
			}
		}
		std::wstring Cxn::toXML() const
		{
			return _T("<a:cxn ang=\"") + ang + _T("\"><a:pos x=\"") + x + _T("\" y=\"") + y + _T("\" /></a:cxn>");
		}
		void Cxn::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:cxn"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("ang"), ang);
			pWriter->EndAttributes();

			pWriter->StartNode(_T("a:pos"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("x"), x);
			pWriter->WriteAttribute(_T("y"), y);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:pos"));

			pWriter->EndNode(_T("a:cxn"));
		}
		void Cxn::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, x);
			pWriter->WriteString1(1, y);
			pWriter->WriteString1(2, ang);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Cxn::FillParentPointersForChilds()
		{
		}
		std::wstring Cxn::GetODString()const
		{
			return _T("<cxn ang=\"") + ang + _T("\"><pos x=\"") + x + _T("\" y=\"") + y + _T("\"/></cxn>");
		}
	} // namespace Logic
} // namespace PPTX
