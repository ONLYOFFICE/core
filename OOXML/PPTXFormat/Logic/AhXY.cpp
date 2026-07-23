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

#include "AhXY.h"

namespace PPTX
{
	namespace Logic
	{
		void AhXY::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oPos = node.ReadNode(_T("a:pos"));

			x	= oPos.ReadAttributeBase(L"x");
			y	= oPos.ReadAttributeBase(L"y");

			XmlMacroReadAttributeBase(node, L"gdRefX", gdRefX);
			XmlMacroReadAttributeBase(node, L"gdRefY", gdRefY);
			XmlMacroReadAttributeBase(node, L"maxX", maxX);
			XmlMacroReadAttributeBase(node, L"maxY", maxY);
			XmlMacroReadAttributeBase(node, L"minX", minX);
			XmlMacroReadAttributeBase(node, L"minY", minY);
		}
		OOX::EElementType AhXY::getType() const
		{
			return OOX::et_a_ahXY;
		}
		void AhXY::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void AhXY::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("gdRefX"), gdRefX )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("minX"), minX )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("maxX"), maxX )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("gdRefY"), gdRefY )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("minY"), minY )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("maxY"), maxY )
			WritingElement_ReadAttributes_End( oReader )
		}
		void AhXY::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("x"), x )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("y"), y )
			WritingElement_ReadAttributes_End( oReader )
		}
		std::wstring AhXY::toXML() const
		{
			XmlUtils::CAttribute oAttr1;
			oAttr1.Write(_T("gdRefX"), gdRefX);
			oAttr1.Write(_T("minX"), minX);
			oAttr1.Write(_T("maxX"), maxX);
			oAttr1.Write(_T("gdRefY"), gdRefY);
			oAttr1.Write(_T("minY"), minY);
			oAttr1.Write(_T("maxY"), maxY);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(_T("x"), x);
			oAttr2.Write(_T("y"), y);

			return XmlUtils::CreateNode(_T("a:ahXY"), oAttr1, XmlUtils::CreateNode(_T("a:pos"), oAttr2));
		}
		void AhXY::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:ahXY"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("gdRefX"), gdRefX);
			pWriter->WriteAttribute(_T("minX"), minX);
			pWriter->WriteAttribute(_T("maxX"), maxX);
			pWriter->WriteAttribute(_T("gdRefY"), gdRefY);
			pWriter->WriteAttribute(_T("minY"), minY);
			pWriter->WriteAttribute(_T("maxY"), maxY);
			pWriter->EndAttributes();

			pWriter->StartNode(_T("a:pos"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("x"), x);
			pWriter->WriteAttribute(_T("y"), y);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:pos"));

			pWriter->EndNode(_T("a:ahXY"));
		}
		void AhXY::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(GEOMETRY_TYPE_AH_XY);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, x);
			pWriter->WriteString1(1, y);

			pWriter->WriteString2(2, gdRefX);
			pWriter->WriteString2(3, gdRefY);
			pWriter->WriteString2(4, maxX);
			pWriter->WriteString2(5, maxY);
			pWriter->WriteString2(6, minX);
			pWriter->WriteString2(7, minY);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void AhXY::FillParentPointersForChilds()
		{
		}
		std::wstring AhXY::GetODString()const
		{
			XmlUtils::CAttribute oAttr1;
			oAttr1.Write(_T("gdRefX"), gdRefX);
			oAttr1.Write(_T("minX"), minX);
			oAttr1.Write(_T("maxX"), maxX);
			oAttr1.Write(_T("gdRefY"), gdRefY);
			oAttr1.Write(_T("minY"), minY);
			oAttr1.Write(_T("maxY"), maxY);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(_T("x"), x);
			oAttr2.Write(_T("y"), y);

			return XmlUtils::CreateNode(_T("ahXY"), oAttr1, XmlUtils::CreateNode(_T("pos"), oAttr2));
		}
	} // namespace Logic
} // namespace PPTX
