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
#include "Slicer.h"
#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/SlicersStream.h"
#include "../../XlsbFormat/Biff12_unions/SLICERS.h"
#include "../../XlsbFormat/Biff12_unions/SLICER.h"
#include "../../XlsbFormat/Biff12_records/BeginSlicer.h"

namespace OOX
{
namespace Spreadsheet
{
void CSlicers::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicers::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
void CSlicers::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICERS*>(obj.get());
    if(ptr != nullptr)
    {
        for(auto &slicer : ptr->m_arSLICER)
            m_oSlicer.push_back(CSlicer(slicer));
    }
}
void CSlicers::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	writer.WriteString(L" xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x xr10\" xmlns:x=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:xr10=\"http://schemas.microsoft.com/office/spreadsheetml/2016/revision10\"");
	writer.EndAttributes();
	if(m_oSlicer.size() > 0)
	{
		for(size_t i = 0; i < m_oSlicer.size(); ++i)
		{
			(&m_oSlicer[i])->toXML(writer, L"slicer");
		}
	}
	writer.EndNode(sName);
}
void CSlicers::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

	pWriter->WriteRecordArray(0, 0, m_oSlicer);
}
void CSlicers::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			pReader->Skip(4);
			ULONG _c = pReader->GetULong();
			for (ULONG i = 0; i < _c; ++i)
			{
				pReader->Skip(1); // type
				m_oSlicer.emplace_back();
				m_oSlicer.back().fromPPTY(pReader);
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
void CSlicer::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar( oReader, "name", m_oName)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "uid", m_oUid)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "cache", m_oCache)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "caption", m_oCaption)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "startItem", m_oStartItem)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "columnCount", m_oColumnCount)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "showCaption", m_oShowCaption)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "level", m_oLevel)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "style", m_oStyle)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "lockedPosition", m_oLockedPosition)
			WritingElement_ReadAttributes_Read_else_ifChar( oReader, "rowHeight", m_oRowHeight)
			WritingElement_ReadAttributes_EndChar_No_NS( oReader )
}
void CSlicer::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
void CSlicer::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::SLICER*>(obj.get());
    if(ptr != nullptr)
    {
        ReadAttributes(ptr->m_BrtBeginSlicer);
    }
}
void CSlicer::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginSlicer*>(obj.get());
    if(ptr != nullptr)
    {
        if(!ptr->stName.value().empty())
            m_oName         = ptr->stName.value();

        if(!ptr->stName.value().empty())
            m_oUid          = ptr->stName.value();

        if(!ptr->stSlicerCacheName.value().empty())
            m_oCache        = ptr->stSlicerCacheName.value();

        if(!ptr->stCaption.value().empty())
            m_oCaption      = ptr->stCaption.value();

        if(!ptr->stStyle.value().empty())
            m_oStyle        = ptr->stStyle.value();

        m_oStartItem        = ptr->dwStartSlicerItem;
        m_oColumnCount      = ptr->dwColumnCount;
        m_oShowCaption      = ptr->fCaptionVisible;
        m_oLevel            = ptr->dwLevel;
        m_oLockedPosition   = ptr->fLockedPosition;
        m_oRowHeight        = ptr->dxRowHeight;
    }
}

void CSlicer::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
	writer.StartNode(sName);
	writer.StartAttributes();
	WritingNullable(m_oName, writer.WriteAttributeEncodeXml(L"name", *m_oName););
	WritingNullable(m_oUid, writer.WriteAttributeEncodeXml(L"xr10:uid", *m_oUid););
	WritingNullable(m_oCache, writer.WriteAttributeEncodeXml(L"cache", *m_oCache););
	WritingNullable(m_oCaption, writer.WriteAttributeEncodeXml(L"caption", *m_oCaption););
	WritingNullable(m_oStartItem, writer.WriteAttribute(L"startItem", *m_oStartItem););
	WritingNullable(m_oColumnCount, writer.WriteAttribute(L"columnCount", *m_oColumnCount););
	WritingNullable(m_oShowCaption, writer.WriteAttribute(L"showCaption", *m_oShowCaption););
	WritingNullable(m_oLevel, writer.WriteAttribute(L"level", *m_oLevel););
	WritingNullable(m_oStyle, writer.WriteAttributeEncodeXml(L"style", *m_oStyle););
	WritingNullable(m_oLockedPosition, writer.WriteAttribute(L"lockedPosition", *m_oLockedPosition););
	WritingNullable(m_oRowHeight, writer.WriteAttribute(L"rowHeight", *m_oRowHeight););
	writer.EndAttributes();
	WritingNullable(m_oExtLst, writer.WriteString(m_oExtLst->toXMLWithNS(L"")););
	writer.EndNode(sName);
}
void CSlicer::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
{
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
	pWriter->WriteString2(0, m_oName);
	pWriter->WriteString2(1, m_oUid);
	pWriter->WriteString2(2, m_oCache);
	pWriter->WriteString2(3, m_oCaption);
	pWriter->WriteUInt2(4, m_oStartItem);
	pWriter->WriteUInt2(5, m_oColumnCount);
	pWriter->WriteBool2(6, m_oShowCaption);
	pWriter->WriteUInt2(7, m_oLevel);
	pWriter->WriteString2(8, m_oStyle);
	pWriter->WriteBool2(9, m_oLockedPosition);
	pWriter->WriteUInt2(10, m_oRowHeight);
	pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

//	pWriter->WriteRecord2(0, m_oExtLst);
}
void CSlicer::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			m_oCache = pReader->GetString2();
			break;
		}
		case 3:
		{
			m_oCaption = pReader->GetString2();
			break;
		}
		case 4:
		{
			m_oStartItem = pReader->GetULong();
			break;
		}
		case 5:
		{
			m_oColumnCount = pReader->GetULong();
			break;
		}
		case 6:
		{
			m_oShowCaption = pReader->GetBool();
			break;
		}
		case 7:
		{
			m_oLevel = pReader->GetULong();
			break;
		}
		case 8:
		{
			m_oStyle = pReader->GetString2();
			break;
		}
		case 9:
		{
			m_oLockedPosition = pReader->GetBool();
			break;
		}
		case 10:
		{
			m_oRowHeight = pReader->GetULong();
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

void CSlicerFile::readBin(const CPath& oPath)
{
    CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
    if (xlsb)
    {
        XLSB::SlicersStreamPtr slicersStream(new XLSB::SlicersStream);

        xlsb->ReadBin(oPath, slicersStream.get());

        if (slicersStream != nullptr)
        {
            if (slicersStream->m_SLICERS != nullptr)
                m_oSlicers = slicersStream->m_SLICERS;
        }

        //slicersStream.reset();

    }
}

void CSlicerFile::read(const CPath& oRootPath, const CPath& oPath)
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

	m_oSlicers = oReader;
}
void CSlicerFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
{
	if(!m_oSlicers.IsInit())
		return;

	NSStringUtils::CStringBuilder sXml;

	sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
	m_oSlicers->toXML(sXml, L"slicers");

	std::wstring sPath = oPath.GetPath();
	NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

	oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
	IFileContainer::Write( oPath, oDirectory, oContent );
}

} //Spreadsheet
} // namespace OOX
