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
#include "NamedSheetViews.h"
#include "../Styles/dxf.h"
#include "../../Binary/Sheets/Reader/BinaryWriter.h"
#include "../../Binary/Sheets/Writer/BinaryReader.h"

namespace OOX
{
namespace Spreadsheet
{
	void CSortRule::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar( oReader, "colId", m_oColId)
				WritingElement_ReadAttributes_Read_else_ifChar( oReader, "id", m_oId)
				WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}
	void CSortRule::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("dxf", sName) == 0)
				m_oDxf = oReader;
//			else if (strcmp("richSortCondition", sName) == 0)
//				m_oRichSortCondition = oReader;
			else if (strcmp("sortCondition", sName) == 0)
				m_oSortCondition = oReader;
		}
	}
	void CSortRule::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		writer.StartNode(sName);
		writer.StartAttributes();
		WritingNullable(m_oColId, writer.WriteAttribute(L"colId", *m_oColId););
		WritingNullable(m_oId, writer.WriteAttributeEncodeXml(L"id", *m_oId););
		writer.EndAttributes();
		WritingNullable(m_oDxf, m_oDxf->toXMLWithNS(writer, L"", L"dxf", L"x:"););
//		WritingNullable(m_oRichSortCondition, m_oRichSortCondition->toXML(writer, L"richSortCondition"););
		WritingNullable(m_oSortCondition, m_oSortCondition->toXMLWithNS(writer, L"", L"sortCondition", L"x:"););
		writer.EndNode(sName);
	}
	void CSortRule::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, m_oColId);
		pWriter->WriteString2(1, m_oId);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		if (m_oDxf.is_init())
		{
			pWriter->StartRecord(0);
			BinXlsxRW::BinaryStyleTableWriter oBinaryStyleTableWriter(*pWriter, &pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts);
			DocWrapper::FontProcessor oFontProcessor;
			OOX::Spreadsheet::CIndexedColors* pIndexedColors = NULL;
			CXlsx* xlsx = dynamic_cast<CXlsx*>(pWriter->m_pMainDocument);
			if (xlsx && xlsx->m_pStyles && xlsx->m_pStyles->m_oColors.IsInit() && xlsx->m_pStyles->m_oColors->m_oIndexedColors.IsInit())
			{
				pIndexedColors = xlsx->m_pStyles->m_oColors->m_oIndexedColors.GetPointer();
			}
			oBinaryStyleTableWriter.WriteDxf(m_oDxf.get(), pIndexedColors, pWriter->m_pTheme->GetPointer(), oFontProcessor);
			pWriter->EndRecord();
		}
//		pWriter->WriteRecord2(1, m_oRichSortCondition);
		if (m_oSortCondition.IsInit())
		{
			pWriter->StartRecord(2);
			BinXlsxRW::BinaryTableWriter oBinaryTableWriter(*pWriter);
			oBinaryTableWriter.WriteSortCondition(*m_oSortCondition);
			pWriter->EndRecord();
		}
	}
	void CSortRule::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oColId = pReader->GetULong();
				break;
			}
			case 1:
			{
				m_oId = pReader->GetString2();
				break;
			}
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _at = pReader->GetUChar();
			switch (_at)
			{
			case 0:
			{
				OOX::Spreadsheet::CStyles oStyles(NULL);
				BinXlsxRW::BinaryStyleTableReader style_reader(*pReader, oStyles);
				m_oDxf.Init();
				style_reader.ReadDxfExternal(m_oDxf.GetPointer());
				break;
			}
//			case 1:
//			{
//				m_oRichSortCondition.Init();
//				m_oRichSortCondition->fromPPTY(pReader);
//				break;
//			}
			case 2:
			{
				BinXlsxRW::BinaryTableReader table_reader(*pReader, NULL);
				m_oSortCondition.Init();
				table_reader.ReadSortConditionExternal(m_oSortCondition.GetPointer());
				break;
			}
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}
		pReader->Seek(_end_rec);
	}
	void CSortRules::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar( oReader, "sortMethod", m_oSortMethod)
				WritingElement_ReadAttributes_Read_else_ifChar( oReader, "caseSensitive", m_oCaseSensitive)
				WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}
	void CSortRules::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("sortRule", sName) == 0)
			{
				m_arrItems.push_back(new CSortRule(oReader));
			}
			else if (strcmp("extLst", sName) == 0)
				m_oExtLst = oReader;
		}
	}
	void CSortRules::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		writer.StartNode(sName);
		writer.StartAttributes();
		WritingNullable(m_oSortMethod, writer.WriteAttribute(L"sortMethod", m_oSortMethod->ToString()););
		WritingNullable(m_oCaseSensitive, writer.WriteAttribute(L"caseSensitive", *m_oCaseSensitive););
		writer.EndAttributes();
		for(size_t i = 0; i < m_arrItems.size(); ++i)
		{
			m_arrItems[i]->toXML(writer, L"sortRule");
		}
		WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
		writer.EndNode(sName);
	}
	void CSortRules::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		if(m_oSortMethod.IsInit())
		{
			pWriter->WriteByte1(0, m_oSortMethod->GetValue());
		}
		pWriter->WriteBool2(1, m_oCaseSensitive);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecordArrayOfPointers(0, 0, m_arrItems);
//		pWriter->WriteRecord2(1, m_oExtLst);
	}
	void CSortRules::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oSortMethod.Init();
				m_oSortMethod->SetValue((SimpleTypes::Spreadsheet::ESortMethod)pReader->GetUChar());
				break;
			}
			case 1:
			{
				m_oCaseSensitive = pReader->GetBool();
				break;
			}
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
				ULONG _c = pReader->GetULong();
				for (ULONG i = 0; i < _c; ++i)
				{
					pReader->Skip(1); // type
					m_arrItems.push_back(new CSortRule());
					m_arrItems.back()->fromPPTY(pReader);
				}
				break;
			}
//			case 1:
//			{
//				m_oExtLst.Init();
//				m_oExtLst->fromPPTY(pReader);
//				break;
//			}
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}
		pReader->Seek(_end_rec);
	}
	void CColumnFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar( oReader, "colId", m_oColId)
				WritingElement_ReadAttributes_Read_else_ifChar( oReader, "id", m_oId)
				WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}
	void CColumnFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("dxf", sName) == 0)
				m_oDxf = oReader;
			else if (strcmp("filter", sName) == 0)
			{
				m_arrItems.push_back(new CFilterColumn(oReader));
			}
			else if (strcmp("extLst", sName) == 0)
				m_oExtLst = oReader;
		}
	}
	void CColumnFilter::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		writer.StartNode(sName);
		writer.StartAttributes();
		WritingNullable(m_oColId, writer.WriteAttribute(L"colId", *m_oColId););
		WritingNullable(m_oId, writer.WriteAttributeEncodeXml(L"id", *m_oId););
		writer.EndAttributes();
		WritingNullable(m_oDxf, m_oDxf->toXMLWithNS(writer, L"", L"dxf", L"x:"););
		for(size_t i = 0; i < m_arrItems.size(); ++i)
		{
			m_arrItems[i]->toXMLWithNS(writer, L"", L"filter", L"x:");
		}
		WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
		writer.EndNode(sName);
	}
	void CColumnFilter::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, m_oColId);
		pWriter->WriteString2(1, m_oId);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		if (m_oDxf.is_init())
		{
			pWriter->StartRecord(0);
			BinXlsxRW::BinaryStyleTableWriter oBinaryStyleTableWriter(*pWriter, &pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts);
			DocWrapper::FontProcessor oFontProcessor;
			OOX::Spreadsheet::CIndexedColors* pIndexedColors = NULL;
			CXlsx* xlsx = dynamic_cast<CXlsx*>(pWriter->m_pMainDocument);
			if (xlsx && xlsx->m_pStyles && xlsx->m_pStyles->m_oColors.IsInit() && xlsx->m_pStyles->m_oColors->m_oIndexedColors.IsInit())
			{
				pIndexedColors = xlsx->m_pStyles->m_oColors->m_oIndexedColors.GetPointer();
			}
			oBinaryStyleTableWriter.WriteDxf(m_oDxf.get(), pIndexedColors, pWriter->m_pTheme->GetPointer(), oFontProcessor);
			pWriter->EndRecord();
		}
		{
			BinXlsxRW::BinaryTableWriter oBinaryTableWriter(*pWriter);
			pWriter->StartRecord(1);
			pWriter->WriteULONG(m_arrItems.size());
			for (_UINT32 i = 0; i < m_arrItems.size(); ++i)
			{
				pWriter->StartRecord(0);
				oBinaryTableWriter.WriteFilterColumn(*m_arrItems[i]);
				pWriter->EndRecord();
			}
			pWriter->EndRecord();
		}
//		pWriter->WriteRecord2(2, m_oExtLst);
	}
	void CColumnFilter::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oColId = pReader->GetULong();
				break;
			}
			case 1:
			{
				m_oId = pReader->GetString2();
				break;
			}
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _at = pReader->GetUChar();
			switch (_at)
			{
			case 0:
			{
				OOX::Spreadsheet::CStyles oStyles(NULL);
				BinXlsxRW::BinaryStyleTableReader style_reader(*pReader, oStyles);
				m_oDxf.Init();
				style_reader.ReadDxfExternal(m_oDxf.GetPointer());
				break;
			}
			case 1:
			{
				pReader->Skip(4);
				BinXlsxRW::BinaryTableReader table_reader(*pReader, NULL);
				ULONG _c = pReader->GetULong();
				for (ULONG i = 0; i < _c; ++i)
				{
					pReader->Skip(1); // type
					CFilterColumn* pFilterColumn = new CFilterColumn();
					table_reader.ReadFilterColumnExternal(pFilterColumn);
					m_arrItems.push_back(pFilterColumn);
				}
				break;
			}
//			case 2:
//			{
//				m_oExtLst.Init();
//				m_oExtLst->fromPPTY(pReader);
//				break;
//			}
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}
		pReader->Seek(_end_rec);
	}
	void CNsvFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar( oReader, "filterId", m_oFilterId)
				WritingElement_ReadAttributes_Read_else_ifChar( oReader, "ref", m_oRef)
				WritingElement_ReadAttributes_Read_else_ifChar( oReader, "tableId", m_oTableId)
				WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}
	void CNsvFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("columnFilter", sName) == 0)
			{
				m_arrItems.push_back(new CColumnFilter(oReader));
			}
			else if (strcmp("sortRules", sName) == 0)
				m_oSortRules = oReader;
			else if (strcmp("extLst", sName) == 0)
				m_oExtLst = oReader;
		}
	}
	void CNsvFilter::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		writer.StartNode(sName);
		writer.StartAttributes();
		WritingNullable(m_oFilterId, writer.WriteAttributeEncodeXml(L"filterId", *m_oFilterId););
		WritingNullable(m_oRef, writer.WriteAttributeEncodeXml(L"ref", *m_oRef););
		WritingNullable(m_oTableId, writer.WriteAttribute(L"tableId", *m_oTableId););
		writer.EndAttributes();
		for(size_t i = 0; i < m_arrItems.size(); ++i)
		{
			m_arrItems[i]->toXML(writer, L"columnFilter");
		}
		WritingNullable(m_oSortRules, m_oSortRules->toXML(writer, L"sortRules"););
		WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
		writer.EndNode(sName);
	}
	void CNsvFilter::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_oFilterId);
		pWriter->WriteString2(1, m_oRef);
		pWriter->WriteUInt2(2, m_oTableId);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecordArrayOfPointers(0, 0, m_arrItems);
		pWriter->WriteRecord2(1, m_oSortRules);
//		pWriter->WriteRecord2(2, m_oExtLst);
	}
	void CNsvFilter::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oFilterId = pReader->GetString2();
				break;
			}
			case 1:
			{
				m_oRef = pReader->GetString2();
				break;
			}
			case 2:
			{
				m_oTableId = pReader->GetULong();
				break;
			}
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
				ULONG _c = pReader->GetULong();
				for (ULONG i = 0; i < _c; ++i)
				{
					pReader->Skip(1); // type
					m_arrItems.push_back(new CColumnFilter());
					m_arrItems.back()->fromPPTY(pReader);
				}
				break;
			}
			case 1:
			{
				m_oSortRules.Init();
				m_oSortRules->fromPPTY(pReader);
				break;
			}
//			case 2:
//			{
//				m_oExtLst.Init();
//				m_oExtLst->fromPPTY(pReader);
//				break;
//			}
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}
		pReader->Seek(_end_rec);
	}
	void CNamedSheetView::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar( oReader, "name", m_oName)
				WritingElement_ReadAttributes_Read_else_ifChar( oReader, "id", m_oId)
				WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}
	void CNamedSheetView::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("nsvFilter", sName) == 0)
			{
				m_arrItems.push_back(new CNsvFilter(oReader));
			}
			else if (strcmp("extLst", sName) == 0)
				m_oExtLst = oReader;
		}
	}
	void CNamedSheetView::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		writer.StartNode(sName);
		writer.StartAttributes();
		WritingNullable(m_oName, writer.WriteAttributeEncodeXml(L"name", *m_oName););
		WritingNullable(m_oId, writer.WriteAttributeEncodeXml(L"id", *m_oId););
		writer.EndAttributes();
		for(size_t i = 0; i < m_arrItems.size(); ++i)
		{
			m_arrItems[i]->toXML(writer, L"nsvFilter");
		}
		WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
		writer.EndNode(sName);
	}
	void CNamedSheetView::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_oName);
		pWriter->WriteString2(1, m_oId);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecordArrayOfPointers(0, 0, m_arrItems);
//		pWriter->WriteRecord2(1, m_oExtLst);
	}
	void CNamedSheetView::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oName = pReader->GetString2();
				break;
			}
			case 1:
			{
				m_oId = pReader->GetString2();
				break;
			}
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
				ULONG _c = pReader->GetULong();
				for (ULONG i = 0; i < _c; ++i)
				{
					pReader->Skip(1); // type
					m_arrItems.push_back(new CNsvFilter());
					m_arrItems.back()->fromPPTY(pReader);
				}
				break;
			}
//			case 1:
//			{
//				m_oExtLst.Init();
//				m_oExtLst->fromPPTY(pReader);
//				break;
//			}
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}
		pReader->Seek(_end_rec);
	}
	void CNamedSheetViews::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}
	void CNamedSheetViews::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("namedSheetView", sName) == 0)
			{
				m_arrItems.push_back(new CNamedSheetView(oReader));
			}
			else if (strcmp("extLst", sName) == 0)
				m_oExtLst = oReader;
		}
	}
	void CNamedSheetViews::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		writer.StartNode(sName);
		writer.StartAttributes();
		writer.WriteString(L" xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2019/namedsheetviews\" xmlns:x=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"");
		writer.EndAttributes();
		for(size_t i = 0; i < m_arrItems.size(); ++i)
		{
			m_arrItems[i]->toXML(writer, L"namedSheetView");
		}
		WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
		writer.EndNode(sName);
	}
	void CNamedSheetViews::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecordArrayOfPointers(0, 0, m_arrItems);
//		pWriter->WriteRecord2(1, m_oExtLst);
	}
	void CNamedSheetViews::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1); // type
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _at = pReader->GetUChar();
			switch (_at)
			{
			case 0:
			{
				pReader->Skip(4);
				ULONG _c = pReader->GetULong();
				for (ULONG i = 0; i < _c; ++i)
				{
					pReader->Skip(1); // type
					m_arrItems.push_back(new CNamedSheetView());
					m_arrItems.back()->fromPPTY(pReader);
				}
				break;
			}
//			case 1:
//			{
//				m_oExtLst.Init();
//				m_oExtLst->fromPPTY(pReader);
//				break;
//			}
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}
		pReader->Seek(_end_rec);
	}

	CNamedSheetViewFile::CNamedSheetViewFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		m_bSpreadsheets = true;
	}
	CNamedSheetViewFile::CNamedSheetViewFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		m_bSpreadsheets = true;
		read( oRootPath, oPath );
	}
	void CNamedSheetViewFile::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	const OOX::FileType CNamedSheetViewFile::type() const
	{
		return OOX::Spreadsheet::FileTypes::NamedSheetView;
	}
	const CPath CNamedSheetViewFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CNamedSheetViewFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CNamedSheetViewFile::GetReadPath()
	{
		return m_oReadPath;
	}
	void CNamedSheetViewFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		m_oNamedSheetViews = oReader;
	}
	void CNamedSheetViewFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if(!m_oNamedSheetViews.IsInit())
			return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		m_oNamedSheetViews->toXML(sXml, L"namedSheetViews");

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
	void CNamedSheetViewFile::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
	}

} //Spreadsheet
} // namespace OOX

