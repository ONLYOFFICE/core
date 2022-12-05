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

#include "TableProperties.h"

namespace PPTX
{
	namespace Logic
	{
		TableProperties::TableProperties()
		{
		}
		TableProperties& TableProperties::operator=(const TableProperties& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Fill		= oSrc.Fill;
			Effects		= oSrc.Effects;
			TableStyleId = oSrc.TableStyleId;

			Rtl			= oSrc.Rtl;
			FirstRow	= oSrc.FirstRow;
			FirstCol	= oSrc.FirstCol;
			LastRow		= oSrc.LastRow;
			LastCol		= oSrc.LastCol;
			BandRow		= oSrc.BandRow;
			BandCol		= oSrc.BandCol;

			return *this;
		}
		void TableProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:tableStyleId")
				{
					TableStyleId = oReader.GetText2();
				}
				else if ( L"a:blipFill"	== strName	||
						  L"a:gradFill"	== strName	||
						  L"a:grpFill"	== strName	||
						  L"a:noFill"	== strName	||
						  L"a:pattFill"	== strName	||
						  L"a:solidFill"== strName )
				{
					Fill.fromXML(oReader);
				}
				else if ( L"a:effectDag"	== strName	||
						  L"a:effectLst"	== strName)
				{
					Effects.fromXML(oReader);
				}
				else if ( L"a:extLst" == strName )
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						Ext element;
						element.fromXML(oReader);
						extLst.push_back (element);
					}
				}
			}
			FillParentPointersForChilds();
		}
		void TableProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("rtl"),	Rtl)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("firstRow"),	FirstRow)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("firstCol"),	FirstCol)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lastRow"),	LastRow)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lastCol"),	LastCol)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("bandRow"),	BandRow)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("bandCol"),	BandCol)
			WritingElement_ReadAttributes_End( oReader )
		}
		void TableProperties::fromXML(XmlUtils::CXmlNode& node)
		{
			Fill.GetFillFrom(node);
			Effects.GetEffectListFrom(node);

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:tableStyleId"), oNode))
				TableStyleId = oNode.GetTextExt();

			XmlMacroReadAttributeBase(node, L"rtl", Rtl);
			XmlMacroReadAttributeBase(node, L"firstRow", FirstRow);
			XmlMacroReadAttributeBase(node, L"firstCol", FirstCol);
			XmlMacroReadAttributeBase(node, L"lastRow", LastRow);
			XmlMacroReadAttributeBase(node, L"lastCol", LastCol);
			XmlMacroReadAttributeBase(node, L"bandRow", BandRow);
			XmlMacroReadAttributeBase(node, L"bandCol", BandCol);

			FillParentPointersForChilds();
		}
		void TableProperties::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tblPr"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("rtl"), Rtl);
			pWriter->WriteAttribute(_T("firstRow"), FirstRow);
			pWriter->WriteAttribute(_T("firstCol"), FirstCol);
			pWriter->WriteAttribute(_T("lastRow"), LastRow);
			pWriter->WriteAttribute(_T("lastCol"), LastCol);
			pWriter->WriteAttribute(_T("bandRow"), BandRow);
			pWriter->WriteAttribute(_T("bandCol"), BandCol);

			pWriter->EndAttributes();

			if (TableStyleId.is_init())
				pWriter->WriteString(_T("<a:tableStyleId>") + *TableStyleId + _T("</a:tableStyleId>"));

			Fill.toXmlWriter(pWriter);
			Effects.toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:tblPr"));
		}
		void TableProperties::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, TableStyleId);

			pWriter->WriteBool2(1, Rtl);
			pWriter->WriteBool2(2, FirstRow);
			pWriter->WriteBool2(3, FirstCol);
			pWriter->WriteBool2(4, LastRow);
			pWriter->WriteBool2(5, LastCol);
			pWriter->WriteBool2(6, BandRow);
			pWriter->WriteBool2(7, BandCol);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, Fill);
			pWriter->WriteRecord1(1, Effects);
		}
		void TableProperties::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						TableStyleId = pReader->GetString2();
						break;
					}
					case 1:
					{
						Rtl = pReader->GetBool();
						break;
					}
					case 2:
					{
						FirstRow = pReader->GetBool();
						break;
					}
					case 3:
					{
						FirstCol = pReader->GetBool();
						break;
					}
					case 4:
					{
						LastRow = pReader->GetBool();
						break;
					}
					case 5:
					{
						LastCol = pReader->GetBool();
						break;
					}
					case 6:
					{
						BandRow = pReader->GetBool();
						break;
					}
					case 7:
					{
						BandCol = pReader->GetBool();
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
						Fill.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						Effects.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TableProperties::FillParentPointersForChilds()
		{
			if(Fill.is_init())
				Fill.SetParentPointer(this);
			if(Effects.is_init())
				Effects.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
