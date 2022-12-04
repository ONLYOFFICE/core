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
