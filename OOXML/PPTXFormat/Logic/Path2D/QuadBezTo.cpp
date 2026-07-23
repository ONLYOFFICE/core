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

#include "QuadBezTo.h"

namespace PPTX
{
	namespace Logic
	{
		QuadBezTo& QuadBezTo::operator=(const QuadBezTo& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			x[0] = oSrc.x[0];
			y[0] = oSrc.y[0];
			x[1] = oSrc.x[1];
			y[1] = oSrc.y[1];
			return (*this);
		}
		OOX::EElementType QuadBezTo::getType() const
		{
			return OOX::et_a_quadBezTo;
		}
		void QuadBezTo::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int count = 0;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if (sName == L"a:pt")
				{
					ReadAttributes2(oReader, x[count], y[count]);
					count++;
				}
			}

		}
		void QuadBezTo::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader, std::wstring & x, std::wstring & y)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("x"), x )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("y"), y )
			WritingElement_ReadAttributes_End( oReader )
		}
		void QuadBezTo::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;

			if (node.GetNodes(_T("a:pt"), oNodes))
			{
				size_t count = oNodes.size();
				if(count >= 2)
				{
					XmlUtils::CXmlNode oNode;

					oNode = oNodes[0];
					x[0] = oNode.GetAttribute(_T("x"));
					y[0] = oNode.GetAttribute(_T("y"));
					
					oNode = oNodes[1];
					x[1] = oNode.GetAttribute(_T("x"));
					y[1] = oNode.GetAttribute(_T("y"));
				}
			}
		}
		void QuadBezTo::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:quadBezTo"));
			pWriter->EndAttributes();

			pWriter->StartNode(_T("a:pt"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("x"), x[0]);
			pWriter->WriteAttribute(_T("y"), y[0]);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:pt"));

			pWriter->StartNode(_T("a:pt"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("x"), x[1]);
			pWriter->WriteAttribute(_T("y"), y[1]);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:pt"));

			pWriter->EndNode(_T("a:quadBezTo"));
		}
		void QuadBezTo::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(GEOMETRY_TYPE_PATH_QUADBEZTO);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, x[0]);
			pWriter->WriteString1(1, y[0]);
			pWriter->WriteString1(2, x[1]);
			pWriter->WriteString1(3, y[1]);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void QuadBezTo::FillParentPointersForChilds(){}
		std::wstring QuadBezTo::GetODString()const
		{
			std::wstring str1 = L"<pt x=\"" + x[0] + L"\" y=\"" + y[0] + L"\" />";
			std::wstring str2 = L"<pt x=\"" + x[1] + L"\" y=\"" + y[1] + L"\" />";

			return _T("<quadBezTo>") + str1 + str2 + _T("</quadBezTo>");
		}
	} // namespace Logic
} // namespace PPTX
