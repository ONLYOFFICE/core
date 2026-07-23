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

#include "Table.h"

namespace PPTX
{
	namespace Logic
	{
		Table::Table()
		{
		}
		Table& Table::operator=(const Table& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			TableCols = oSrc.TableCols;
			TableRows = oSrc.TableRows;

			tableProperties = oSrc.tableProperties;
			return *this;
		}
		void Table::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:tblGrid"), oNode))
				XmlMacroLoadArray(oNode, _T("a:gridCol"), TableCols, TableCol);

			XmlMacroLoadArray(node, _T("a:tr"), TableRows, TableRow);

			tableProperties = node.ReadNode(_T("a:tblPr"));

			FillParentPointersForChilds();
		}
		void Table::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"tblGrid")
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring strName1 = XmlUtils::GetNameNoNS(oReader.GetName());
						if (strName1 == L"gridCol")
						{
							TableCol col;
							TableCols.push_back(col);
							TableCols.back().fromXML(oReader);
						}
					}
				}
				else if (strName == L"tblPr")
					tableProperties = oReader;
				else if (strName == L"tr")
				{
					TableRow tr;
					TableRows.push_back(tr);
					TableRows.back().fromXML(oReader);
				}
			}
			FillParentPointersForChilds();
		}
		void Table::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, tableProperties);
			pWriter->WriteRecordArray(1, 0, TableCols);
			pWriter->WriteRecordArray(2, 0, TableRows);
		}
		void Table::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						tableProperties = new Logic::TableProperties();
						tableProperties->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						pReader->Skip(4);
						LONG lCount = pReader->GetLong();
						for (LONG i = 0; i < lCount; ++i)
						{
							pReader->Skip(1);
							TableCols.push_back(TableCol());
							TableCols[i].fromPPTY(pReader);
						}
						break;
					}
					case 2:
					{
						pReader->Skip(4);
						LONG lCount = pReader->GetLong();
						for (LONG i = 0; i < lCount; ++i)
						{
							pReader->Skip(1);
							TableRows.push_back(TableRow());
							TableRows[i].fromPPTY(pReader);
						}
					}
					default:
					{
						break;
					}
				}
			}

			pReader->Seek(_end_rec);
		}
		void Table::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tbl"));
			pWriter->EndAttributes();

			pWriter->Write(tableProperties);

			pWriter->WriteString(_T("<a:tblGrid>"));
			size_t n1 = TableCols.size();
			for (size_t i = 0; i < n1; ++i)
				TableCols[i].toXmlWriter(pWriter);
			pWriter->WriteString(_T("</a:tblGrid>"));

			size_t n2 = TableRows.size();
			for (size_t i = 0; i < n2; ++i)
				TableRows[i].toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:tbl"));
		}
		void Table::FillParentPointersForChilds()
			{
                if (tableProperties.IsInit())
                    tableProperties->SetParentPointer(this);
				
				size_t count = TableRows.size();
				for (size_t i = 0; i < count; ++i)
					TableRows[i].SetParentPointer(this);
			}		
	} // namespace Logic
} // namespace PPTX
