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
		void TableCell::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tc"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("rowSpan"), RowSpan);
			pWriter->WriteAttribute(_T("gridSpan"), GridSpan);
			pWriter->WriteAttribute(_T("hMerge"), HMerge);
			pWriter->WriteAttribute(_T("vMerge"), VMerge);
			pWriter->WriteAttribute(_T("id"), Id);

			pWriter->EndAttributes();

			pWriter->Write(txBody);
			pWriter->Write(CellProperties);

			pWriter->EndNode(_T("a:tc"));
		}
		void TableCell::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, Id);
			pWriter->WriteInt2(1, RowSpan);
			pWriter->WriteInt2(2, GridSpan);
			pWriter->WriteBool2(3, HMerge);
			pWriter->WriteBool2(4, VMerge);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, CellProperties);
			pWriter->WriteRecord2(1, txBody);
		}
		void TableCell::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						Id = pReader->GetString2();
						break;
					}
					case 1:
					{
						RowSpan = pReader->GetLong();
						break;
					}
					case 2:
					{
						GridSpan = pReader->GetLong();
						break;
					}
					case 3:
					{
						HMerge = pReader->GetBool();
						break;
					}
					case 4:
					{
						VMerge = pReader->GetBool();
						break;
					}
					default:
						break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						CellProperties = new TableCellProperties();
						CellProperties->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						txBody = new Logic::TxBody();
						txBody->fromPPTY(pReader);
						txBody->m_name = _T("a:txBody");
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
	} // namespace Logic
} // namespace PPTX
