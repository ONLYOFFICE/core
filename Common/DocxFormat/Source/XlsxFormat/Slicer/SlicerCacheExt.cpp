﻿/*
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
#include "SlicerCacheExt.h"

namespace OOX
{
namespace Spreadsheet
{
void CSlicerCacheOlapLevelName::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "uniqueName", m_oUniqueName)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "count", m_oCount)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerCacheOlapLevelName::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	oReader.ReadTillEnd();
}
void CSlicerCacheOlapLevelName::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oUniqueName, writer.WriteAttributeEncodeXml(L"uniqueName", *m_oUniqueName););
	WritingNullable(m_oCount, writer.WriteAttribute(L"count", *m_oCount););
	writer.EndAttributes();
	writer.EndNode(sName);
}
void CSlicerCacheOlapLevelName::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oUniqueName);
	pWriter->WriteUInt2(1, m_oCount);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

}
void CSlicerCacheOlapLevelName::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oUniqueName = pReader->GetString2();
			break;
		}
		case 1:
		{
			m_oCount = pReader->GetULong();
			break;
		}
		}
	}
	while (pReader->GetPos() < _end_rec)
	{
		BYTE _at = pReader->GetUChar();
		switch (_at)
		{
		default:
		{
			pReader->SkipRecord();
			break;
		}
		}
	}
	pReader->Seek(_end_rec);
}
void CSlicerCacheHideNoData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "count", m_oCount)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerCacheHideNoData::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("slicerCacheOlapLevelName", sName) == 0)
		{
			m_oSlicerCacheOlapLevelName.emplace_back();
			m_oSlicerCacheOlapLevelName.back() = oReader;
		}
	}
}
void CSlicerCacheHideNoData::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oCount, writer.WriteAttribute(L"count", *m_oCount););
	writer.EndAttributes();
	if(m_oSlicerCacheOlapLevelName.size() > 0)
	{
		for(size_t i = 0; i < m_oSlicerCacheOlapLevelName.size(); ++i)
		{
			(&m_oSlicerCacheOlapLevelName[i])->toXML(writer, L"x15:slicerCacheOlapLevelName");
		}
	}
	writer.EndNode(sName);
}
void CSlicerCacheHideNoData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oCount);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oSlicerCacheOlapLevelName);
}
void CSlicerCacheHideNoData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oCount = pReader->GetULong();
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
				m_oSlicerCacheOlapLevelName.emplace_back();
				m_oSlicerCacheOlapLevelName.back().fromPPTY(pReader);
			}
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
void CTableSlicerCache::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "tableId", m_oTableId)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "column", m_oColumn)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "sortOrder", m_oSortOrder)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "customListSort", m_oCustomListSort)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "crossFilter", m_oCrossFilter)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CTableSlicerCache::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("extLst", sName) == 0)
			m_oExtLst = oReader;
	}
}
void CTableSlicerCache::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oTableId, writer.WriteAttribute(L"tableId", *m_oTableId););
	WritingNullable(m_oColumn, writer.WriteAttribute(L"column", *m_oColumn););
	WritingNullable(m_oSortOrder, writer.WriteAttribute(L"sortOrder", m_oSortOrder->ToString()););
	WritingNullable(m_oCustomListSort, writer.WriteAttribute(L"customListSort", *m_oCustomListSort););
	WritingNullable(m_oCrossFilter, writer.WriteAttribute(L"crossFilter", m_oCrossFilter->ToString()););
	writer.EndAttributes();
	WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
	writer.EndNode(sName);
}
void CTableSlicerCache::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oTableId);
	pWriter->WriteUInt2(1, m_oColumn);
	if(m_oSortOrder.IsInit())
	{
		pWriter->WriteByte1(2, m_oSortOrder->GetValue());
	}
	pWriter->WriteBool2(3, m_oCustomListSort);
	if(m_oCrossFilter.IsInit())
	{
		pWriter->WriteByte1(4, m_oCrossFilter->GetValue());
	}
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

//	pWriter->WriteRecord2(0, m_oExtLst);
}
void CTableSlicerCache::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oTableId = pReader->GetULong();
			break;
		}
		case 1:
		{
			m_oColumn = pReader->GetULong();
			break;
		}
		case 2:
		{
			m_oSortOrder.Init();
			m_oSortOrder->SetValue((SimpleTypes::Spreadsheet::ETabularSlicerCacheSortOrder)pReader->GetUChar());
			break;
		}
		case 3:
		{
			m_oCustomListSort = pReader->GetBool();
			break;
		}
		case 4:
		{
			m_oCrossFilter.Init();
			m_oCrossFilter->SetValue((SimpleTypes::Spreadsheet::ESlicerCacheCrossFilter)pReader->GetUChar());
			break;
		}
		}
	}
	while (pReader->GetPos() < _end_rec)
	{
		BYTE _at = pReader->GetUChar();
		switch (_at)
		{
//		case 0:
//		{
//			m_oExtLst.Init();
//			m_oExtLst->fromPPTY(pReader);
//			break;
//		}
		default:
		{
			pReader->SkipRecord();
			break;
		}
		}
	}
	pReader->Seek(_end_rec);
}
void CSlicerStyleElement::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "type", m_oType)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "dxfId", m_oDxfId)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerStyleElement::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	oReader.ReadTillEnd();
}
void CSlicerStyleElement::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oType, writer.WriteAttribute(L"type", m_oType->ToString()););
	WritingNullable(m_oDxfId, writer.WriteAttribute(L"dxfId", *m_oDxfId););
	writer.EndAttributes();
	writer.EndNode(sName);
}
void CSlicerStyleElement::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	if(m_oType.IsInit())
	{
		pWriter->WriteByte1(0, m_oType->GetValue());
	}
	pWriter->WriteUInt2(1, m_oDxfId);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

}
void CSlicerStyleElement::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oType.Init();
			m_oType->SetValue((SimpleTypes::Spreadsheet::ESlicerStyleType)pReader->GetUChar());
			break;
		}
		case 1:
		{
			m_oDxfId = pReader->GetULong();
			break;
		}
		}
	}
	while (pReader->GetPos() < _end_rec)
	{
		BYTE _at = pReader->GetUChar();
		switch (_at)
		{
		default:
		{
			pReader->SkipRecord();
			break;
		}
		}
	}
	pReader->Seek(_end_rec);
}
void CSlicerCache::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "id", m_oRId)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerCache::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	oReader.ReadTillEnd();
}
void CSlicerCache::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oRId, writer.WriteAttribute(L"r:id", m_oRId->ToString()););
	writer.EndAttributes();
	writer.EndNode(sName);
}
void CSlicerRef::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "id", m_oRId)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	oReader.ReadTillEnd();
}
void CSlicerRef::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oRId, writer.WriteAttributeEncodeXml(L"r:id", m_oRId->ToString()););
	writer.EndAttributes();
	writer.EndNode(sName);
}
void CSlicerStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "name", m_oName)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("slicerStyleElements", sName) == 0)
		{
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
				if (strcmp("slicerStyleElement", sName) == 0)
				{
					m_oSlicerStyleElements.emplace_back();
					m_oSlicerStyleElements.back() = oReader;
				}
			}
		}
	}
}
void CSlicerStyle::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oName, writer.WriteAttributeEncodeXml(L"name", *m_oName););
	writer.EndAttributes();
	if(m_oSlicerStyleElements.size() > 0)
	{
		writer.StartNode(L"x14:slicerStyleElements");
		writer.StartAttributes();
		writer.EndAttributes();
		for(size_t i = 0; i < m_oSlicerStyleElements.size(); ++i)
		{
			(&m_oSlicerStyleElements[i])->toXML(writer, L"x14:slicerStyleElement");
		}
		writer.EndNode(L"x14:slicerStyleElements");
	}
	writer.EndNode(sName);
}
void CSlicerStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oName);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oSlicerStyleElements);
}
void CSlicerStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oSlicerStyleElements.emplace_back();
				m_oSlicerStyleElements.back().fromPPTY(pReader);
			}
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
void CSlicerCaches::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerCaches::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("slicerCache", sName) == 0)
		{
			m_oSlicerCache.emplace_back();
			m_oSlicerCache.back() = oReader;
		}
	}
}
void CSlicerCaches::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName, const std::wstring& sPrefix) const
{
	writer.StartNode(sPrefix + sName);
	writer.StartAttributes();
	std::wstring sChildPrefix;
	if(sPrefix.length() > 0 && L"x14:" != sPrefix)
	{
		sChildPrefix = L"x14:";
		writer.WriteAttribute(L"xmlns:x14", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");
	}
	writer.EndAttributes();
	if(m_oSlicerCache.size() > 0)
	{
		for(size_t i = 0; i < m_oSlicerCache.size(); ++i)
		{
			(&m_oSlicerCache[i])->toXML(writer, L"x14:slicerCache");
		}
	}
	writer.EndNode(sPrefix + sName);
}
void CSlicerRefs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerRefs::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("slicer", sName) == 0)
		{
			m_oSlicer.emplace_back();
			m_oSlicer.back() = oReader;
		}
	}
}
void CSlicerRefs::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	writer.WriteAttribute(L"xmlns:x14", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");
	writer.EndAttributes();
	if(m_oSlicer.size() > 0)
	{
		for(size_t i = 0; i < m_oSlicer.size(); ++i)
		{
			(&m_oSlicer[i])->toXML(writer, L"x14:slicer");
		}
	}
	writer.EndNode(sName);
}
void CSlicerStyles::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "defaultSlicerStyle", m_oDefaultSlicerStyle)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerStyles::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("slicerStyle", sName) == 0)
		{
			m_oSlicerStyle.emplace_back();
			m_oSlicerStyle.back() = oReader;
		}
	}
}
void CSlicerStyles::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oDefaultSlicerStyle, writer.WriteAttributeEncodeXml(L"defaultSlicerStyle", *m_oDefaultSlicerStyle););
	writer.EndAttributes();
	if(m_oSlicerStyle.size() > 0)
	{
		for(size_t i = 0; i < m_oSlicerStyle.size(); ++i)
		{
			(&m_oSlicerStyle[i])->toXML(writer, L"x14:slicerStyle");
		}
	}
	writer.EndNode(sName);
}
void CSlicerStyles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oDefaultSlicerStyle);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oSlicerStyle);
}
void CSlicerStyles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oDefaultSlicerStyle = pReader->GetString2();
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
				m_oSlicerStyle.emplace_back();
				m_oSlicerStyle.back().fromPPTY(pReader);
			}
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
void CDrawingSlicer::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "name", m_oName)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CDrawingSlicer::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("extLst", sName) == 0)
			m_oExtLst = oReader;
	}
}
std::wstring CDrawingSlicer::toXML() const
{
	NSBinPptxRW::CXmlWriter writer;
	toXML(writer, L"sle:slicer");
	return writer.GetXmlString();
}
void CDrawingSlicer::toXML(NSBinPptxRW::CXmlWriter& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	writer.WriteString(L" xmlns:sle=\"http://schemas.microsoft.com/office/drawing/2010/slicer\"");
	WritingNullable(m_oName, writer.WriteAttribute2(L"name", *m_oName););
	writer.EndAttributes();
	//WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
	writer.EndNode(sName);
}
void CDrawingSlicer::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oName);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

//	pWriter->WriteRecord2(0, m_oExtLst);
}
void CDrawingSlicer::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
		}
	}
	while (pReader->GetPos() < _end_rec)
	{
		BYTE _at = pReader->GetUChar();
		switch (_at)
		{
//		case 0:
//		{
//			m_oExtLst.Init();
//			m_oExtLst->fromPPTY(pReader);
//			break;
//		}
		default:
		{
			pReader->SkipRecord();
			break;
		}
		}
	}
	pReader->Seek(_end_rec);
}

} //Spreadsheet
} // namespace OOX
