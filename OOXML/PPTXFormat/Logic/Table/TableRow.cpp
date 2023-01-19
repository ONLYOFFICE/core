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

#include "TableRow.h"

namespace PPTX
{
	namespace Logic
	{
		TableRow::TableRow()
		{
		}
		TableRow& TableRow::operator=(const TableRow& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Height = oSrc.Height;
			Cells = oSrc.Cells;

			return *this;
		}
		void TableRow::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"tc")
				{
					TableCell c;
					Cells.push_back(c);
					Cells.back().fromXML(oReader);
				}
			}
		}
		void TableRow::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("h"),	Height)
			WritingElement_ReadAttributes_End( oReader )
		}
		void TableRow::fromXML(XmlUtils::CXmlNode& node)
		{
			Height = node.ReadAttributeInt(L"h");
			XmlMacroLoadArray(node, _T("a:tc"), Cells, TableCell);
		}
		void TableRow::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tr"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"h", Height);
			pWriter->EndAttributes();

			size_t len = Cells.size();
			for (size_t i = 0; i < len; ++i)
				Cells[i].toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:tr"));
		}
		void TableRow::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, Height);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecordArray(0, 1, Cells);
		}
		void TableRow::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						Height = pReader->GetLong();
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
						pReader->Skip(4);
						LONG len = pReader->GetLong();
						for (LONG i = 0; i < len; ++i)
						{
							pReader->Skip(1);
							Cells.push_back(TableCell());
							Cells[i].fromPPTY(pReader);
						}
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TableRow::FillParentPointersForChilds()
		{
			size_t count = Cells.size();
			for (size_t i = 0; i < count; ++i)
				Cells[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
