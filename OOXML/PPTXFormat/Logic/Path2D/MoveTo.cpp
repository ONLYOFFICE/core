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

#include "MoveTo.h"

namespace PPTX
{
	namespace Logic
	{
		MoveTo& MoveTo::operator=(const MoveTo& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			x = oSrc.x;
			y = oSrc.y;
			return *this;
		}
		OOX::EElementType MoveTo::getType() const
		{
			return OOX::et_a_moveTo;
		}
		void MoveTo::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if (sName == L"a:pt")
				{
					ReadAttributes2(oReader);
				}
			}
		}
		void MoveTo::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("x"), x )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("y"), y )
			WritingElement_ReadAttributes_End( oReader )
		}
		void MoveTo::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:pt"), oNode))
			{
				x = oNode.GetAttribute(_T("x"));
				y = oNode.GetAttribute(_T("y"));
			}
		}
		void MoveTo::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:moveTo"));
			pWriter->EndAttributes();

			pWriter->StartNode(_T("a:pt"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("x"), x);
			pWriter->WriteAttribute(_T("y"), y);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:pt"));

			pWriter->EndNode(_T("a:moveTo"));
		}
		void MoveTo::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(GEOMETRY_TYPE_PATH_MOVETO);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, x);
			pWriter->WriteString1(1, y);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void MoveTo::FillParentPointersForChilds(){}
		std::wstring MoveTo::GetODString()const
		{
			std::wstring str = L"<pt x=\"" + x + L"\" y=\"" + y + L"\" />";
			return _T("<moveTo>") + str + _T("</moveTo>");
		}
	} // namespace Logic
} // namespace PPTX
