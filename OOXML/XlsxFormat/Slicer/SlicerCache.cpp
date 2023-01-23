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
#include "SlicerCache.h"
#include "SlicerCacheExt.h"
#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/SlicerCachesStream.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHE.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHEOLAPIMPL.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHENATIVEITEMS.h"
#include "../../XlsbFormat/Biff12_unions/FRTSLICERCACHE.h"
#include "../../XlsbFormat/Biff12_records/BeginSlicerCacheDef.h"
#include "../../XlsbFormat/Biff12_records/SlicerCachePivotTables.h"
#include "../../XlsbFormat/Biff12_records/BeginSlicerCacheNative.h"
#include "../../XlsbFormat/Biff12_records/SlicerCacheNativeItem.h"
#include "../../XlsbFormat/Biff12_records/BeginSlicerCacheOlapImpl.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHESELECTIONS.h"
#include "../../XlsbFormat/Biff12_records/SlicerCacheSelection.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHELEVELSDATA.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHELEVELDATA.h"
#include "../../XlsbFormat/Biff12_records/BeginSlicerCacheLevelData.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHESIRANGES.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHESIRANGE.h"
#include "../../XlsbFormat/Biff12_records/BeginSlicerCacheSiRange.h"
#include "../../XlsbFormat/Biff12_records/SlicerCacheOlapItem.h"

namespace OOX
{
namespace Spreadsheet
{
void COlapSlicerCacheItem::fromBin(XLS::BaseObjectPtr& obj)
{
    ReadAttributes(obj);
    auto ptr = static_cast<XLSB::SlicerCacheOlapItem*>(obj.get());
    if(ptr != nullptr)
    {
        COlapSlicerCacheItemParent oCOlapSlicerCacheItemParent;
        for(auto &item : ptr->parents)
        {
            oCOlapSlicerCacheItemParent.m_oN = item.value();
            m_oP.push_back(oCOlapSlicerCacheItemParent);
        }
    }
}
void COlapSlicerCacheItem::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SlicerCacheOlapItem*>(obj.get());
    if(ptr != nullptr)
    {
        m_oNd    = ptr->fNoData;

        if(!ptr->stName.value().empty())
            m_oN = ptr->stName.value();

        if(!ptr->stTitle.value().empty())
            m_oC = ptr->stTitle.value();
    }
}
void COlapSlicerCacheItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
		WritingElement_ReadAttributes_Read_ifChar( oReader, "n", m_oN)
		WritingElement_ReadAttributes_Read_else_ifChar( oReader, "c", m_oC)
		WritingElement_ReadAttributes_Read_else_ifChar( oReader, "nd", m_oNd)
	WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCacheItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("p", sName) == 0)
		{
			m_oP.emplace_back();
			m_oP.back() = oReader;
		}
	}
}
void COlapSlicerCacheItem::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oN, writer.WriteAttributeEncodeXml(L"n", *m_oN););
	WritingNullable(m_oC, writer.WriteAttributeEncodeXml(L"c", *m_oC););
	WritingNullable(m_oNd, writer.WriteAttribute(L"nd", *m_oNd););
	writer.EndAttributes();
	if(m_oP.size() > 0)
	{
		for(size_t i = 0; i < m_oP.size(); ++i)
		{
			(&m_oP[i])->toXML(writer, L"p");
		}
	}
	writer.EndNode(sName);
}
void COlapSlicerCacheItem::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oN);
	pWriter->WriteString2(1, m_oC);
	pWriter->WriteBool2(2, m_oNd);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oP);
}
void COlapSlicerCacheItem::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oN = pReader->GetString2();
			break;
		}
		case 1:
		{
			m_oC = pReader->GetString2();
			break;
		}
		case 2:
		{
			m_oNd = pReader->GetBool();
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
				m_oP.emplace_back();
				m_oP.back().fromPPTY(pReader);
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
void COlapSlicerCacheItemParent::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "n", m_oN)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCacheItemParent::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	oReader.ReadTillEnd();
}
void COlapSlicerCacheItemParent::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oN, writer.WriteAttributeEncodeXml(L"n", *m_oN););
	writer.EndAttributes();
	writer.EndNode(sName);
}
void COlapSlicerCacheItemParent::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oN);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

}
void COlapSlicerCacheItemParent::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oN = pReader->GetString2();
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
void COlapSlicerCacheRange::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERCACHESIRANGE*>(obj.get());
    if(ptr != nullptr)
    {
        ReadAttributes(ptr->m_BrtBeginSlicerCacheSiRange);

        COlapSlicerCacheItem oCOlapSlicerCacheItem;
        for(auto &item : ptr->m_arBrtSlicerCacheOlapItem)
        {
            oCOlapSlicerCacheItem.fromBin(item);
            m_oI.push_back(oCOlapSlicerCacheItem);
        }
    }
}
void COlapSlicerCacheRange::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginSlicerCacheSiRange*>(obj.get());
    if(ptr != nullptr)
    {
        m_oStartItem = ptr->iitemstart;
    }
}
void COlapSlicerCacheRange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "startItem", m_oStartItem)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCacheRange::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("i", sName) == 0)
		{
			m_oI.emplace_back();
			m_oI.back() = oReader;
		}
	}
}
void COlapSlicerCacheRange::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oStartItem, writer.WriteAttribute(L"startItem", *m_oStartItem););
	writer.EndAttributes();
	if(m_oI.size() > 0)
	{
		for(size_t i = 0; i < m_oI.size(); ++i)
		{
			(&m_oI[i])->toXML(writer, L"i");
		}
	}
	writer.EndNode(sName);
}
void COlapSlicerCacheRange::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oStartItem);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oI);
}
void COlapSlicerCacheRange::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oStartItem = pReader->GetULong();
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
				m_oI.emplace_back();
				m_oI.back().fromPPTY(pReader);
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
void CTabularSlicerCacheItem::fromBin(XLS::BiffStructure& obj)
{
    ReadAttributes(obj);
}
void CTabularSlicerCacheItem::ReadAttributes(XLS::BiffStructure& obj)
{
    auto ptr = static_cast<XLSB::SlicerCacheNativeItemStruct*>(&obj);
    if(ptr != nullptr)
    {
        m_oX    = ptr->iCache;
        m_oS    = ptr->fSelected;
        m_oNd   = ptr->fNoData;
    }
}
void CTabularSlicerCacheItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "x", m_oX)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "s", m_oS)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "nd", m_oNd)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CTabularSlicerCacheItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	oReader.ReadTillEnd();
}
void CTabularSlicerCacheItem::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oX, writer.WriteAttribute(L"x", *m_oX););
	WritingNullable(m_oS, writer.WriteAttribute(L"s", *m_oS););
	WritingNullable(m_oNd, writer.WriteAttribute(L"nd", *m_oNd););
	writer.EndAttributes();
	writer.EndNode(sName);
}
void CTabularSlicerCacheItem::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oX);
	pWriter->WriteBool2(1, m_oS);
	pWriter->WriteBool2(2, m_oNd);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

}
void CTabularSlicerCacheItem::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oX = pReader->GetULong();
			break;
		}
		case 1:
		{
			m_oS = pReader->GetBool();
			break;
		}
		case 2:
		{
			m_oNd = pReader->GetBool();
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
void COlapSlicerCacheSelection::fromBin(XLS::BaseObjectPtr& obj)
{
    ReadAttributes(obj);

    auto ptr = static_cast<XLSB::SlicerCacheSelection*>(obj.get());
    if(ptr != nullptr)
    {
        COlapSlicerCacheItemParent oCOlapSlicerCacheItemParent;
        for(auto &item : ptr->parents)
        {
            oCOlapSlicerCacheItemParent.m_oN = item.value();
            m_oP.push_back(oCOlapSlicerCacheItemParent);
        }
    }
}
void COlapSlicerCacheSelection::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SlicerCacheSelection*>(obj.get());
    if(ptr != nullptr)
    {
        if(!ptr->stUniqueName.value().empty())
            m_oN = ptr->stUniqueName.value();
    }
}
void COlapSlicerCacheSelection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "n", m_oN)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCacheSelection::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("p", sName) == 0)
		{
			m_oP.emplace_back();
			m_oP.back() = oReader;
		}
	}
}
void COlapSlicerCacheSelection::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oN, writer.WriteAttributeEncodeXml(L"n", *m_oN););
	writer.EndAttributes();
	if(m_oP.size() > 0)
	{
		for(size_t i = 0; i < m_oP.size(); ++i)
		{
			(&m_oP[i])->toXML(writer, L"p");
		}
	}
	writer.EndNode(sName);
}
void COlapSlicerCacheSelection::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oN);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oP);
}
void COlapSlicerCacheSelection::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oN = pReader->GetString2();
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
				m_oP.emplace_back();
				m_oP.back().fromPPTY(pReader);
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
void COlapSlicerCacheLevelData::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERCACHELEVELDATA*>(obj.get());
    if(ptr != nullptr)
    {
        ReadAttributes(ptr->m_BrtBeginSlicerCacheLevelData);

        auto ptr1 = static_cast<XLSB::SLICERCACHESIRANGES*>(ptr->m_SLICERCACHESIRANGES.get());
        if(ptr1 != nullptr)
        {
            COlapSlicerCacheRange oCOlapSlicerCacheRange;
            for(auto &item : ptr1->m_arSLICERCACHESIRANGE)
            {
                oCOlapSlicerCacheRange.fromBin(item);
                m_oRanges.push_back(oCOlapSlicerCacheRange);
            }
        }
    }
}
void COlapSlicerCacheLevelData::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginSlicerCacheLevelData*>(obj.get());
    if(ptr != nullptr)
    {
        m_oCount             = ptr->citem;
        m_oSortOrder         = (SimpleTypes::Spreadsheet::EOlapSlicerCacheSortOrder)ptr->fSortOrder;

        if(!ptr->stUniqueName.value().empty())
            m_oUniqueName    = ptr->stUniqueName.value();

        if(!ptr->stSourceCaption.value().empty())
            m_oSourceCaption = ptr->stSourceCaption.value();

        m_oCrossFilter      = (SimpleTypes::Spreadsheet::ESlicerCacheCrossFilter)ptr->fCrossFilter;
    }
}
void COlapSlicerCacheLevelData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "uniqueName", m_oUniqueName)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "sourceCaption", m_oSourceCaption)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "count", m_oCount)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "sortOrder", m_oSortOrder)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "crossFilter", m_oCrossFilter)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCacheLevelData::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("ranges", sName) == 0)
		{
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
				if (strcmp("range", sName) == 0)
				{
					m_oRanges.emplace_back();
					m_oRanges.back() = oReader;
				}
			}
		}
	}
}
void COlapSlicerCacheLevelData::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oUniqueName, writer.WriteAttributeEncodeXml(L"uniqueName", *m_oUniqueName););
	WritingNullable(m_oSourceCaption, writer.WriteAttributeEncodeXml(L"sourceCaption", *m_oSourceCaption););
	WritingNullable(m_oCount, writer.WriteAttribute(L"count", *m_oCount););
	WritingNullable(m_oSortOrder, writer.WriteAttribute(L"sortOrder", m_oSortOrder->ToString()););
	WritingNullable(m_oCrossFilter, writer.WriteAttribute(L"crossFilter", m_oCrossFilter->ToString()););
	writer.EndAttributes();
	if(m_oRanges.size() > 0)
	{
		writer.StartNode(L"ranges");
		writer.StartAttributes();
		writer.EndAttributes();
		for(size_t i = 0; i < m_oRanges.size(); ++i)
		{
			(&m_oRanges[i])->toXML(writer, L"range");
		}
		writer.EndNode(L"ranges");
	}
	writer.EndNode(sName);
}
void COlapSlicerCacheLevelData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oUniqueName);
	pWriter->WriteString2(1, m_oSourceCaption);
	pWriter->WriteUInt2(2, m_oCount);
	if(m_oSortOrder.IsInit())
	{
		pWriter->WriteByte1(3, m_oSortOrder->GetValue());
	}
	if(m_oCrossFilter.IsInit())
	{
		pWriter->WriteByte1(4, m_oCrossFilter->GetValue());
	}
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oRanges);
}
void COlapSlicerCacheLevelData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oSourceCaption = pReader->GetString2();
			break;
		}
		case 2:
		{
			m_oCount = pReader->GetULong();
			break;
		}
		case 3:
		{
			m_oSortOrder.Init();
			m_oSortOrder->SetValue((SimpleTypes::Spreadsheet::EOlapSlicerCacheSortOrder)pReader->GetUChar());
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
		case 0:
		{
			pReader->Skip(4);
			ULONG _c = pReader->GetULong();
			for (ULONG i = 0; i < _c; ++i)
			{
				pReader->Skip(1); // type
				m_oRanges.emplace_back();
				m_oRanges.back().fromPPTY(pReader);
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

void CTabularSlicerCacheItems::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SlicerCacheNativeItem*>(obj.get());
    if(ptr != nullptr)
    {
        m_oCount = ptr->cItems;
        CTabularSlicerCacheItem oCTabularSlicerCacheItem;
        for(auto &item : ptr->rgItems)
        {
            oCTabularSlicerCacheItem.fromBin(item);
            m_oI.push_back(oCTabularSlicerCacheItem);
        }
    }
}

void CTabularSlicerCacheItems::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "count", m_oCount)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CTabularSlicerCacheItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("i", sName) == 0)
		{
			m_oI.emplace_back();
			m_oI.back() = oReader;
		}
	}
}
void CTabularSlicerCacheItems::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oCount, writer.WriteAttribute(L"count", *m_oCount););
	writer.EndAttributes();
	if(m_oI.size() > 0)
	{
		for(size_t i = 0; i < m_oI.size(); ++i)
		{
			(&m_oI[i])->toXML(writer, L"i");
		}
	}
	writer.EndNode(sName);
}
void CTabularSlicerCacheItems::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oCount);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oI);
}
void CTabularSlicerCacheItems::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oI.emplace_back();
				m_oI.back().fromPPTY(pReader);
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

void COlapSlicerCacheSelections::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERCACHESELECTIONS*>(obj.get());
    if(ptr != nullptr)
    {
        m_oCount = ptr->m_arBrtSlicerCacheSelection.size();

        COlapSlicerCacheSelection oCOlapSlicerCacheSelection;
        for(auto &item : ptr->m_arBrtSlicerCacheSelection)
        {
            oCOlapSlicerCacheSelection.fromBin(item);
            m_oSelection.push_back(oCOlapSlicerCacheSelection);
        }

    }
}
void COlapSlicerCacheSelections::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "count", m_oCount)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCacheSelections::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("selection", sName) == 0)
		{
			m_oSelection.emplace_back();
			m_oSelection.back() = oReader;
		}
	}
}
void COlapSlicerCacheSelections::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oCount, writer.WriteAttribute(L"count", *m_oCount););
	writer.EndAttributes();
	if(m_oSelection.size() > 0)
	{
		for(size_t i = 0; i < m_oSelection.size(); ++i)
		{
			(&m_oSelection[i])->toXML(writer, L"selection");
		}
	}
	writer.EndNode(sName);
}
void COlapSlicerCacheSelections::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oCount);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oSelection);
}
void COlapSlicerCacheSelections::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oSelection.emplace_back();
				m_oSelection.back().fromPPTY(pReader);
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
void COlapSlicerCacheLevelsData::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERCACHELEVELSDATA*>(obj.get());
    if(ptr != nullptr)
    {
        m_oCount = ptr->m_arSLICERCACHELEVELDATA.size();

        COlapSlicerCacheLevelData oCOlapSlicerCacheLevelData;
        for(auto &item : ptr->m_arSLICERCACHELEVELDATA)
        {
            oCOlapSlicerCacheLevelData.fromBin(item);
            m_oLevel.push_back(oCOlapSlicerCacheLevelData);
        }

    }
}
void COlapSlicerCacheLevelsData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "count", m_oCount)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCacheLevelsData::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("level", sName) == 0)
		{
			m_oLevel.emplace_back();
			m_oLevel.back() = oReader;
		}
	}
}
void COlapSlicerCacheLevelsData::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oCount, writer.WriteAttribute(L"count", *m_oCount););
	writer.EndAttributes();
	if(m_oLevel.size() > 0)
	{
		for(size_t i = 0; i < m_oLevel.size(); ++i)
		{
			(&m_oLevel[i])->toXML(writer, L"level");
		}
	}
	writer.EndNode(sName);
}
void COlapSlicerCacheLevelsData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oCount);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oLevel);
}
void COlapSlicerCacheLevelsData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				m_oLevel.emplace_back();
				m_oLevel.back().fromPPTY(pReader);
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
void CTabularSlicerCache::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERCACHENATIVEITEMS*>(obj.get());
    if(ptr != nullptr)
    {
        ReadAttributes(ptr->m_BrtBeginSlicerCacheNative);

        if(ptr->m_BrtSlicerCacheNativeItem != nullptr)
            m_oItems = ptr->m_BrtSlicerCacheNativeItem;
    }
}
void CTabularSlicerCache::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginSlicerCacheNative*>(obj.get());
    if(ptr != nullptr)
    {
        m_oPivotCacheId     = ptr->dwcacheId;
        m_oSortOrder        = (SimpleTypes::Spreadsheet::ETabularSlicerCacheSortOrder)(ptr->fSortOrder - 1);
        m_oCustomListSort   = ptr->fSortUsingCustomLists;
        m_oShowMissing      = ptr->fShowAllItems;

        m_oCrossFilter      = (SimpleTypes::Spreadsheet::ESlicerCacheCrossFilter)ptr->fCrossFilter;
    }
}
void CTabularSlicerCache::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "pivotCacheId", m_oPivotCacheId)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "sortOrder", m_oSortOrder)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "customListSort", m_oCustomListSort)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "showMissing", m_oShowMissing)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "crossFilter", m_oCrossFilter)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CTabularSlicerCache::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("items", sName) == 0)
			m_oItems = oReader;
		else if (strcmp("extLst", sName) == 0)
			m_oExtLst = oReader;
	}
}
void CTabularSlicerCache::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oPivotCacheId, writer.WriteAttribute(L"pivotCacheId", *m_oPivotCacheId););
	WritingNullable(m_oSortOrder, writer.WriteAttribute(L"sortOrder", m_oSortOrder->ToString()););
	WritingNullable(m_oCustomListSort, writer.WriteAttribute(L"customListSort", *m_oCustomListSort););
	WritingNullable(m_oShowMissing, writer.WriteAttribute(L"showMissing", *m_oShowMissing););
	WritingNullable(m_oCrossFilter, writer.WriteAttribute(L"crossFilter", m_oCrossFilter->ToString()););
	writer.EndAttributes();
	WritingNullable(m_oItems, m_oItems->toXML(writer, L"items"););
	WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
	writer.EndNode(sName);
}
void CTabularSlicerCache::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oPivotCacheId);
	if(m_oSortOrder.IsInit())
	{
		pWriter->WriteByte1(1, m_oSortOrder->GetValue());
	}
	pWriter->WriteBool2(2, m_oCustomListSort);
	pWriter->WriteBool2(3, m_oShowMissing);
	if(m_oCrossFilter.IsInit())
	{
		pWriter->WriteByte1(4, m_oCrossFilter->GetValue());
	}
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecord2(0, m_oItems);
//	pWriter->WriteRecord2(1, m_oExtLst);
}
void CTabularSlicerCache::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oPivotCacheId = pReader->GetULong();
			break;
		}
		case 1:
		{
			m_oSortOrder.Init();
			m_oSortOrder->SetValue((SimpleTypes::Spreadsheet::ETabularSlicerCacheSortOrder)pReader->GetUChar());
			break;
		}
		case 2:
		{
			m_oCustomListSort = pReader->GetBool();
			break;
		}
		case 3:
		{
			m_oShowMissing = pReader->GetBool();
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
		case 0:
		{
			m_oItems.Init();
			m_oItems->fromPPTY(pReader);
			break;
		}
//		case 1:
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

void COlapSlicerCache::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERCACHEOLAPIMPL*>(obj.get());
    if(ptr != nullptr)
    {
        ReadAttributes(ptr->m_BrtBeginSlicerCacheOlapImpl);

        if(ptr->m_SLICERCACHELEVELSDATA != nullptr)
            m_oLevels = ptr->m_SLICERCACHELEVELSDATA;

        if(ptr->m_SLICERCACHESELECTIONS != nullptr)
            m_oSelections = ptr->m_SLICERCACHESELECTIONS;
    }
}
void COlapSlicerCache::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginSlicerCacheOlapImpl*>(obj.get());
    if(ptr != nullptr)
    {
        m_oPivotCacheId = ptr->ipivotcacheid;
    }
}

void COlapSlicerCache::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "pivotCacheId", m_oPivotCacheId)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void COlapSlicerCache::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("levels", sName) == 0)
			m_oLevels = oReader;
		else if (strcmp("selections", sName) == 0)
			m_oSelections = oReader;
//		else if (strcmp("extLst", sName) == 0)
//			m_oExtLst = oReader;
	}
}
void COlapSlicerCache::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oPivotCacheId, writer.WriteAttribute(L"pivotCacheId", *m_oPivotCacheId););
	writer.EndAttributes();
	WritingNullable(m_oLevels, m_oLevels->toXML(writer, L"levels"););
	WritingNullable(m_oSelections, m_oSelections->toXML(writer, L"selections"););
	WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
	writer.EndNode(sName);
}
void COlapSlicerCache::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oPivotCacheId);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecord2(0, m_oLevels);
	pWriter->WriteRecord2(1, m_oSelections);
//	pWriter->WriteRecord2(2, m_oExtLst);
}
void COlapSlicerCache::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oPivotCacheId = pReader->GetULong();
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
			m_oLevels.Init();
			m_oLevels->fromPPTY(pReader);
			break;
		}
		case 1:
		{
			m_oSelections.Init();
			m_oSelections->fromPPTY(pReader);
			break;
		}
//		case 2:
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

void CSlicerCacheData::fromBin(XLS::BaseObjectPtr& obj)
{
    if(obj->get_type() == XLS::typeSLICERCACHEOLAPIMPL)
    {
        m_oOlap = obj;
    }

    if(obj->get_type() == XLS::typeSLICERCACHENATIVEITEMS)
    {
        m_oTabular = obj;
    }
}
void CSlicerCacheData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerCacheData::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("olap", sName) == 0)
			m_oOlap = oReader;
		else if (strcmp("tabular", sName) == 0)
			m_oTabular = oReader;
	}
}
void CSlicerCacheData::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	writer.EndAttributes();
	WritingNullable(m_oOlap, m_oOlap->toXML(writer, L"olap"););
	WritingNullable(m_oTabular, m_oTabular->toXML(writer, L"tabular"););
	writer.EndNode(sName);
}
void CSlicerCacheData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecord2(0, m_oOlap);
	pWriter->WriteRecord2(1, m_oTabular);
}
void CSlicerCacheData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
		}
	}
	while (pReader->GetPos() < _end_rec)
	{
		BYTE _at = pReader->GetUChar();
		switch (_at)
		{
		case 0:
		{
			m_oOlap.Init();
			m_oOlap->fromPPTY(pReader);
			break;
		}
		case 1:
		{
			m_oTabular.Init();
			m_oTabular->fromPPTY(pReader);
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

void CSlicerCachePivotTable::fromBin(XLS::BiffStructure& obj)
{
    ReadAttributes(obj);
}
void CSlicerCachePivotTable::ReadAttributes(XLS::BiffStructure& obj)
{
    auto ptr = static_cast<XLSB::SlicerCachePivotTable*>(&obj);
    if(ptr != nullptr)
    {
        m_oTabId    = ptr->iTabId;

        if(!ptr->stPivotTable.value().empty())
            m_oName = ptr->stPivotTable.value();
    }
}

void CSlicerCachePivotTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "tabId", m_oTabId)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "name", m_oName)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerCachePivotTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	oReader.ReadTillEnd();
}
void CSlicerCachePivotTable::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oTabId, writer.WriteAttribute(L"tabId", *m_oTabId););
	WritingNullable(m_oName, writer.WriteAttributeEncodeXml(L"name", *m_oName););
	writer.EndAttributes();
	writer.EndNode(sName);
}
void CSlicerCachePivotTable::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteUInt2(0, m_oTabId);
	pWriter->WriteString2(1, m_oName);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

}
void CSlicerCachePivotTable::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oTabId = pReader->GetULong();
			break;
		}
		case 1:
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
		default:
		{
			pReader->SkipRecord();
			break;
		}
		}
	}
	pReader->Seek(_end_rec);
}
void CSlicerCacheDefinition::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERCACHE*>(obj.get());
    if(ptr != nullptr)
    {
        ReadAttributes(ptr->m_BrtBeginSlicerCacheDef);

        if(ptr->m_BrtSlicerCachePivotTables != nullptr)
        {
            auto ptrSCPT = static_cast<XLSB::SlicerCachePivotTables*>(ptr->m_BrtSlicerCachePivotTables.get());
            CSlicerCachePivotTable oCSlicerCachePivotTable;
            for(auto &item : ptrSCPT->pivotTables)
            {
                oCSlicerCachePivotTable.fromBin(item);
                m_oPivotTables.push_back(oCSlicerCachePivotTable);
            }
        }

        if(ptr->m_slicerCacheData != nullptr)
            m_oData = ptr->m_slicerCacheData;

        if(ptr->m_FRTSLICERCACHE != nullptr)
            m_oExtLst = ptr->m_FRTSLICERCACHE;
    }
}
void CSlicerCacheDefinition::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginSlicerCacheDef*>(obj.get());
    if(ptr != nullptr)
    {
        if(!ptr->stName.value().empty())
            m_oName         = ptr->stName.value();

        if(!ptr->stHierarchy.value().empty())
            m_oSourceName        = ptr->stHierarchy.value();

    }
}
void CSlicerCacheDefinition::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "name", m_oName)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "uid", m_oUid)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "sourceName", m_oSourceName)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicerCacheDefinition::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	if (oReader.IsEmptyNode())
		return;
	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
		if (strcmp("pivotTables", sName) == 0)
		{
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
				if (strcmp("pivotTable", sName) == 0)
				{
					m_oPivotTables.emplace_back();
					m_oPivotTables.back() = oReader;
				}
			}
		}
		else if (strcmp("data", sName) == 0)
			m_oData = oReader;
		else if (strcmp("extLst", sName) == 0)
			m_oExtLst = oReader;
	}
}
void CSlicerCacheDefinition::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.WriteString(L" xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x xr10\" xmlns:x=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:xr10=\"http://schemas.microsoft.com/office/spreadsheetml/2016/revision10\"");
	writer.StartAttributes();
	WritingNullable(m_oName, writer.WriteAttributeEncodeXml(L"name", *m_oName););
	WritingNullable(m_oUid, writer.WriteAttribute(L"xr10:uid", m_oUid->ToString()););
	WritingNullable(m_oSourceName, writer.WriteAttributeEncodeXml(L"sourceName", *m_oSourceName););
	writer.EndAttributes();
	if(m_oPivotTables.size() > 0)
	{
		writer.StartNode(L"pivotTables");
		writer.StartAttributes();
		writer.EndAttributes();
		for(size_t i = 0; i < m_oPivotTables.size(); ++i)
		{
			(&m_oPivotTables[i])->toXML(writer, L"pivotTable");
		}
		writer.EndNode(L"pivotTables");
	}
	WritingNullable(m_oData, m_oData->toXML(writer, L"data"););
	WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
	writer.EndNode(sName);
}
void CSlicerCacheDefinition::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oName);
	if(m_oUid.IsInit())
	{
		pWriter->WriteString1(1, m_oUid->ToString());
	}
	pWriter->WriteString2(2, m_oSourceName);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oPivotTables);
	pWriter->WriteRecord2(1, m_oData);
	if (m_oExtLst.IsInit())
	{
		for(size_t i = 0; i < m_oExtLst->m_arrExt.size(); ++i)
		{
			OOX::Drawing::COfficeArtExtension* pExt = m_oExtLst->m_arrExt[i];
			if(!pExt->m_oSlicerCachePivotTables.empty())
			{
				pWriter->StartRecord(2);

				_UINT32 len = (_UINT32)pExt->m_oSlicerCachePivotTables.size();
				pWriter->WriteULONG(len);

				for (_UINT32 i = 0; i < len; ++i)
					pWriter->WriteRecord1(0, *pExt->m_oSlicerCachePivotTables[i]);

				pWriter->EndRecord();
			}
			pWriter->WriteRecord2(3, pExt->m_oTableSlicerCache);
			pWriter->WriteRecord2(4, pExt->m_oSlicerCacheHideItemsWithNoData);
		}
	}
}
void CSlicerCacheDefinition::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oUid = pReader->GetString2();
			break;
		}
		case 2:
		{
			m_oSourceName = pReader->GetString2();
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
				m_oPivotTables.emplace_back();
				m_oPivotTables.back().fromPPTY(pReader);
			}
			break;
		}
		case 1:
		{
			m_oData.Init();
			m_oData->fromPPTY(pReader);
			break;
		}
		case 2:
		{
			OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();

			pReader->Skip(4);
			ULONG _c = pReader->GetULong();
			for (ULONG i = 0; i < _c; ++i)
			{
				pReader->Skip(1); // type
				OOX::Spreadsheet::CSlicerCachePivotTable* pSlicerCachePivotTable = new OOX::Spreadsheet::CSlicerCachePivotTable();
				pSlicerCachePivotTable->fromPPTY(pReader);
				pOfficeArtExtension->m_oSlicerCachePivotTables.push_back(pSlicerCachePivotTable);
			}

			pOfficeArtExtension->m_sUri.Init();
			pOfficeArtExtension->m_sUri->append(_T("{03082B11-2C62-411c-B77F-237D8FCFBE4C}"));
			pOfficeArtExtension->m_sAdditionalNamespace = _T("xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\"");
			if(!m_oExtLst.IsInit())
			{
				m_oExtLst.Init();
			}
			m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
			break;
		}
		case 3:
		{
			OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
			pOfficeArtExtension->m_oTableSlicerCache.Init();
			pOfficeArtExtension->m_oTableSlicerCache->fromPPTY(pReader);

			pOfficeArtExtension->m_sUri.Init();
			pOfficeArtExtension->m_sUri->append(_T("{2F2917AC-EB37-4324-AD4E-5DD8C200BD13}"));
			pOfficeArtExtension->m_sAdditionalNamespace = _T("xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\"");
			if(!m_oExtLst.IsInit())
			{
				m_oExtLst.Init();
			}
			m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
			break;
		}
		case 4:
		{
			OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
			pOfficeArtExtension->m_oSlicerCacheHideItemsWithNoData.Init();
			pOfficeArtExtension->m_oSlicerCacheHideItemsWithNoData->fromPPTY(pReader);

			pOfficeArtExtension->m_sUri.Init();
			pOfficeArtExtension->m_sUri->append(_T("{470722E0-AACD-4C17-9CDC-17EF765DBC7E}"));
			pOfficeArtExtension->m_sAdditionalNamespace = _T("xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\"");
			if(!m_oExtLst.IsInit())
			{
				m_oExtLst.Init();
			}
			m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
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

void CSlicerCacheFile::readBin(const CPath& oPath)
{
    CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
    if (xlsb)
    {
        XLSB::SlicerCachesStreamPtr slicerCachesStream(new XLSB::SlicerCachesStream);

        xlsb->ReadBin(oPath, slicerCachesStream.get());

        if (slicerCachesStream != nullptr)
        {
            if (slicerCachesStream->m_SLICERCACHE != nullptr)
                m_oSlicerCacheDefinition = slicerCachesStream->m_SLICERCACHE;
        }

        //slicerCachesStream.reset();
    }
}

void CSlicerCacheFile::read(const CPath& oRootPath, const CPath& oPath)
{
	m_oReadPath = oPath;
	IFileContainer::Read( oRootPath, oPath );

    if( m_oReadPath.GetExtention() == _T(".bin"))
    {
        readBin(m_oReadPath);
        return;
    }

    XmlUtils::CXmlLiteReader oReader;

	if ( !oReader.FromFile( oPath.GetPath() ) )
		return;

	if ( !oReader.ReadNextNode() )
		return;

	m_oSlicerCacheDefinition = oReader;
}

void CSlicerCacheFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
{
	if(!m_oSlicerCacheDefinition.IsInit())
		return;

	NSStringUtils::CStringBuilder sXml;

	sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
	m_oSlicerCacheDefinition->toXML(sXml, L"slicerCacheDefinition");

	std::wstring sPath = oPath.GetPath();
	NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

	oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
	IFileContainer::Write( oPath, oDirectory, oContent );
}

} //Spreadsheet
} // namespace OOX
