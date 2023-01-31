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
			XmlUtils::CXmlNodes oNodes;

			if (node.GetNodes(_T("a:pt"), oNodes))
			{
				int count = oNodes.GetCount();
				if(count >= 2)
				{
					XmlUtils::CXmlNode oNode;

					oNodes.GetAt(0, oNode);
					x[0] = oNode.GetAttribute(_T("x"));
					y[0] = oNode.GetAttribute(_T("y"));
					oNodes.GetAt(1, oNode);
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
