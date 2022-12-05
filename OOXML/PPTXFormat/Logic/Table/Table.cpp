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
