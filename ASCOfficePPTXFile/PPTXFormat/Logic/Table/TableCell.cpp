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


#include "TableCell.h"
#include "../../Slide.h"

namespace PPTX
{
	namespace Logic
	{
		TableCell::TableCell()
		{
		}
		TableCell::~TableCell()
		{
		}
		TableCell::TableCell(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const TableCell& TableCell::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		TableCell::TableCell(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
		}
		const TableCell& TableCell::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
			return *this;
		}
		void TableCell::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("rowSpan"),	RowSpan)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("gridSpan"),	GridSpan)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("hMerge"),	HMerge)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("vMerge"),	VMerge)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("id"),		Id)
			WritingElement_ReadAttributes_End( oReader )
		}
		void TableCell::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;
				
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"txBody")
					txBody = oReader;
				else if (strName == L"tcPr")
					CellProperties = oReader;
			}

			FillParentPointersForChilds();
		}
		void TableCell::fromXML(XmlUtils::CXmlNode& node)
		{
            txBody			= node.ReadNodeNoNS(_T("txBody"));
			CellProperties	= node.ReadNode(_T("a:tcPr"));

            XmlMacroReadAttributeBase(node, L"rowSpan", RowSpan);
            XmlMacroReadAttributeBase(node, L"gridSpan", GridSpan);
            XmlMacroReadAttributeBase(node, L"hMerge", HMerge);
            XmlMacroReadAttributeBase(node, L"vMerge", VMerge);
            XmlMacroReadAttributeBase(node, L"id", Id);

			FillParentPointersForChilds();
		}

		void TableCell::FillParentPointersForChilds()
		{
            if(txBody.IsInit())
                txBody->SetParentPointer(this);
			if(CellProperties.IsInit())
				CellProperties->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX
