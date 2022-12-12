/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "CubicBezTo.h"

namespace PPTX
{
	namespace Logic
	{
		CubicBezTo& CubicBezTo::operator=(const CubicBezTo& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			x[0] = oSrc.x[0];
			y[0] = oSrc.y[0];
			x[1] = oSrc.x[1];
			y[1] = oSrc.y[1];
			x[2] = oSrc.x[2];
			y[2] = oSrc.y[2];

			return *this;
		}
		void CubicBezTo::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;

			if (node.GetNodes(_T("a:pt"), oNodes))
			{
				int count = oNodes.GetCount();
				if(count >= 3)
				{
					XmlUtils::CXmlNode oNode;

					oNodes.GetAt(0, oNode);
					x[0] = oNode.GetAttribute(_T("x"));
					y[0] = oNode.GetAttribute(_T("y"));
					oNodes.GetAt(1, oNode);
					x[1] = oNode.GetAttribute(_T("x"));
					y[1] = oNode.GetAttribute(_T("y"));
					oNodes.GetAt(2, oNode);
					x[2] = oNode.GetAttribute(_T("x"));
					y[2] = oNode.GetAttribute(_T("y"));
				}
			}
		}
		OOX::EElementType CubicBezTo::getType() const
		{
			return OOX::et_a_cubicBezTo;
		}
		void CubicBezTo::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void CubicBezTo::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader, std::wstring & x, std::wstring & y)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("x"), x )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("y"), y )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CubicBezTo::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:cubicBezTo"));
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

			pWriter->StartNode(_T("a:pt"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("x"), x[2]);
			pWriter->WriteAttribute(_T("y"), y[2]);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:pt"));

			pWriter->EndNode(_T("a:cubicBezTo"));
		}
		void CubicBezTo::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(GEOMETRY_TYPE_PATH_CUBICBEZTO);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, x[0]);
			pWriter->WriteString1(1, y[0]);
			pWriter->WriteString1(2, x[1]);
			pWriter->WriteString1(3, y[1]);
			pWriter->WriteString1(4, x[2]);
			pWriter->WriteString1(5, y[2]);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void CubicBezTo::FillParentPointersForChilds(){}
		std::wstring CubicBezTo::GetODString()const
		{
			std::wstring str1 = L"<pt x=\"" + x[0] + L"\" y=\"" + y[0] + L"\"/>";
			std::wstring str2 = L"<pt x=\"" + x[1] + L"\" y=\"" + y[1] + L"\"/>";
			std::wstring str3 = L"<pt x=\"" + x[2] + L"\" y=\"" + y[2] + L"\"/>";

			return _T("<cubicBezTo>") + str1 + str2 + str3 + _T("</cubicBezTo>");
		}
	} // namespace Logic
} // namespace PPTX
