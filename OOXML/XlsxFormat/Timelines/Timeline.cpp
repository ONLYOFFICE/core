/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "Timeline.h"

#include "../FileTypes_Spreadsheet.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../DocxFormat/Drawing/DrawingExt.h"
#include "../../../DesktopEditor/common/File.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CTimelineCachePivotTable::CTimelineCachePivotTable() {}
		CTimelineCachePivotTable::~CTimelineCachePivotTable() {}
		void CTimelineCachePivotTable::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineCachePivotTable::toXML() const
		{
			return L"";
		}
		EElementType CTimelineCachePivotTable::getType() const
		{
			return et_x_TimelineCachePivotTable;
		}
		void CTimelineCachePivotTable::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<pivotTable");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrInt2(L"tabId", m_oTabId);
			writer.WriteString(L"/>");
		}
		void CTimelineCachePivotTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CTimelineCachePivotTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"tabId", m_oTabId)
			WritingElement_ReadAttributes_End(oReader)
		}
//-------------------------------------------------------------------------------------
		CTimelineRange::CTimelineRange() {}
		CTimelineRange::~CTimelineRange() {}
		void CTimelineRange::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineRange::toXML() const
		{
			return L"";
		}
		EElementType CTimelineRange::getType() const
		{
			return et_x_TimelineRange;
		}
		void CTimelineRange::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<" + m_name);
			WritingStringNullableAttrEncodeXmlString2(L"startDate", m_oStartDate);
			WritingStringNullableAttrEncodeXmlString2(L"endDate", m_oEndDate);
			writer.WriteString(L"/>");
		}
		void CTimelineRange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();

			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CTimelineRange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"startDate", m_oStartDate)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"endDate", m_oEndDate)
			WritingElement_ReadAttributes_End(oReader)
		}
//-------------------------------------------------------------------------------------
		CTimelineCachePivotTables::CTimelineCachePivotTables() {}
		CTimelineCachePivotTables::~CTimelineCachePivotTables() {}
		void CTimelineCachePivotTables::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineCachePivotTables::toXML() const
		{
			return L"";
		}
		EElementType CTimelineCachePivotTables::getType() const
		{
			return et_x_TimelineCachePivotTables;
		}
		void CTimelineCachePivotTables::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"pivotTable" == sName)
				{
					CTimelineCachePivotTable* pPivotTable = new CTimelineCachePivotTable();
					*pPivotTable = oReader;
					m_arrItems.push_back(pPivotTable);
				}
			}
		}
		void CTimelineCachePivotTables::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<pivotTables>");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</pivotTables>");
		}
//--------------------------------------------------------------------------------------------------------------------------
		CTimeline::CTimeline() {}
		CTimeline::~CTimeline() {}
		void CTimeline::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimeline::toXML() const
		{
			return L"";
		}
		EElementType CTimeline::getType () const
		{
			return et_x_Timeline;
		}
		void CTimeline::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<timeline");
				WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
				WritingStringNullableAttrEncodeXmlString2(L"caption", m_oCaption);
				WritingStringNullableAttrEncodeXmlString2(L"xr10:uid", m_oUid);
				WritingStringNullableAttrEncodeXmlString2(L"cache", m_oCache);
				WritingStringNullableAttrInt2(L"level", m_oLevel);
				WritingStringNullableAttrInt2(L"selectionLevel", m_oSelectionLevel);
				WritingStringNullableAttrEncodeXmlString2(L"scrollPosition", m_oScrollPosition);
				WritingStringNullableAttrBool2(L"showHeader", m_oShowHeader);
				WritingStringNullableAttrBool2(L"showTimeLevel", m_oShowTimeLevel);
				WritingStringNullableAttrBool2(L"showSelectionLabel", m_oShowSelectionLabel);
				WritingStringNullableAttrBool2(L"showHorizontalScrollbar", m_oShowHorizontalScrollbar);
				WritingStringNullableAttrString(L"style", m_oStyle, m_oStyle->ToString());
			writer.WriteString(L"/>");
		}
		void CTimeline::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"extLst" == sName)
					m_oExtLst = oReader;
			}
		}
		void CTimeline::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"caption", m_oCaption)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"xr10:uid", m_oUid)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"cache", m_oCache)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"showHeader", m_oShowHeader)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"showTimeLevel", m_oShowTimeLevel)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"showSelectionLabel", m_oShowSelectionLabel)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"showHorizontalScrollbar", m_oShowHorizontalScrollbar)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"level", m_oLevel)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"selectionLevel", m_oSelectionLevel)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"scrollPosition", m_oScrollPosition)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"style", m_oStyle)				
			WritingElement_ReadAttributes_End( oReader )
		}
//-------------------------------------------------------------------------------------
		CTimelines::CTimelines() {}
		CTimelines::~CTimelines() {}
		void CTimelines::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelines::toXML() const
		{
			return L"";
		}
		EElementType CTimelines::getType() const
		{
			return et_x_Timelines;
		}
		void CTimelines::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"timeline" == sName)
				{
					CTimeline* pTimeline = new CTimeline();
					*pTimeline = oReader;
					m_arrItems.push_back(pTimeline);
				}
			}
		}
		void CTimelines::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<timelines xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"x xr10\" \
xmlns:x=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:xr10=\"http://schemas.microsoft.com/office/spreadsheetml/2016/revision10\">");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</timelines>");
		}
//-------------------------------------------------------------------------------------------------------
		CTimelineState::CTimelineState()
		{
		}
		CTimelineState::~CTimelineState()
		{
		}
		void CTimelineState::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineState::toXML() const
		{
			return L"";
		}
		EElementType CTimelineState::getType() const
		{
			return et_x_TimelineState;
		}
		void CTimelineState::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<state");

			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrEncodeXmlString2(L"filterType", m_oFilterType);
			WritingStringNullableAttrBool2(L"singleRangeFilterState", m_oSingleRangeFilterState);
			WritingStringNullableAttrInt2(L"pivotCacheId", m_oPivotCacheId);
			WritingStringNullableAttrInt2(L"minimalRefreshVersion", m_oMinimalRefreshVersion);
			WritingStringNullableAttrInt2(L"lastRefreshVersion", m_oLastRefreshVersion);
			writer.WriteString(L">");

			if (m_oSelection.IsInit())
			{
				m_oSelection->m_name = L"selection";
				m_oSelection->toXML(writer);
			}
			if (m_oBounds.IsInit())
			{
				m_oBounds->m_name = L"bounds";
				m_oBounds->toXML(writer);
			}
			writer.WriteString(L"</state>");
		}
		void CTimelineState::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"selection" == sName)
					m_oSelection = oReader;
				else if (L"bounds" == sName)
					m_oBounds = oReader;
				else if (L"extLst" == sName)
					m_oExtLst = oReader;
			}
		}
		void CTimelineState::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"singleRangeFilterState", m_oSingleRangeFilterState)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pivotCacheId", m_oPivotCacheId)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"minimalRefreshVersion", m_oMinimalRefreshVersion)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"lastRefreshVersion", m_oLastRefreshVersion)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"filterType", m_oFilterType)
			WritingElement_ReadAttributes_End(oReader)
		}
//-------------------------------------------------------------------------------------------------------
		CTimelinePivotFilter::CTimelinePivotFilter()
		{
		}
		CTimelinePivotFilter::~CTimelinePivotFilter()
		{
		}
		void CTimelinePivotFilter::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelinePivotFilter::toXML() const
		{
			return L"";
		}
		EElementType CTimelinePivotFilter::getType() const
		{
			return et_x_TimelinePivotFilter;
		}
		void CTimelinePivotFilter::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<timelinePivotFilter");

			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrEncodeXmlString2(L"description", m_oDescription);
			WritingStringNullableAttrBool2(L"useWholeDay", m_oUseWholeDay);
			WritingStringNullableAttrInt2(L"id", m_oId);
			WritingStringNullableAttrInt2(L"fld", m_oFld);
			writer.WriteString(L">");

			if (m_oAutoFilter.IsInit())
			{
				m_oAutoFilter->toXML(writer);
			}
			writer.WriteString(L"</timelinePivotFilter>");
		}
		void CTimelinePivotFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"autoFilter" == sName)
					m_oAutoFilter = oReader;
			}
		}
		void CTimelinePivotFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"description", m_oDescription)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"useWholeDay", m_oUseWholeDay)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"id", m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"fld", m_oFld)
			WritingElement_ReadAttributes_End(oReader)
		}
//----------------------------------------------------------------------------------------------------------------------
		CTimelineCacheDefinition::CTimelineCacheDefinition()
		{
		}
		CTimelineCacheDefinition::~CTimelineCacheDefinition()
		{
		}
		void CTimelineCacheDefinition::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineCacheDefinition::toXML() const
		{
			return L"";
		}
		EElementType CTimelineCacheDefinition::getType () const
		{
			return et_x_TimelineCacheDefinition;
		}
		void CTimelineCacheDefinition::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<timelineCacheDefinition \
xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\" \
xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr10\" \
xmlns:xr10=\"http://schemas.microsoft.com/office/spreadsheetml/2016/revision10\"");

			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrEncodeXmlString2(L"sourceName", m_oSourceName);
			WritingStringNullableAttrEncodeXmlString2(L"xr10:uid", m_oUid);

			writer.WriteString(L">");

			if(m_oPivotTables.IsInit())
			{
				m_oPivotTables->toXML(writer);
			}
			if(m_oState.IsInit())
			{
				m_oState->toXML(writer);
			}
			writer.WriteString(L"</timelineCacheDefinition>");
		}
		void CTimelineCacheDefinition::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"pivotTables" == sName )
					m_oPivotTables = oReader;
				else if ( L"state" == sName )
					m_oState = oReader;
				else if (L"pivotFilter" == sName)
					m_oPivotFilter = oReader;
				else if (L"pivotFilter" == sName)
					m_oPivotFilter = oReader;
				else if (L"extLst" == sName)
					m_oExtLst = oReader;
			}
		}
		void CTimelineCacheDefinition::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"sourceName", m_oSourceName)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"xr10:uid", m_oUid)
			WritingElement_ReadAttributes_End( oReader )
		}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
		CTimelineFile::CTimelineFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
		}
		CTimelineFile::CTimelineFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
			read( oRootPath, oPath );
		}
		CTimelineFile::~CTimelineFile()
		{
		}
		void CTimelineFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		const OOX::FileType CTimelineFile::type() const
		{
			return OOX::Spreadsheet::FileTypes::Timeline;
		}
		const CPath CTimelineFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CTimelineFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CTimelineFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CTimelineFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			m_oTimelines = oReader;
		}
		void CTimelineFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			if(false == m_oTimelines.IsInit()) return;

			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oTimelines->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
		CTimelineCacheFile::CTimelineCacheFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
		}
		CTimelineCacheFile::CTimelineCacheFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
			read(oRootPath, oPath);
		}
		CTimelineCacheFile::~CTimelineCacheFile()
		{
		}
		void CTimelineCacheFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		const OOX::FileType CTimelineCacheFile::type() const
		{
			return OOX::Spreadsheet::FileTypes::TimelineCache;
		}
		const CPath CTimelineCacheFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CTimelineCacheFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CTimelineCacheFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CTimelineCacheFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read(oRootPath, oPath);

			XmlUtils::CXmlLiteReader oReader;

			if (!oReader.FromFile(oPath.GetPath()))
				return;

			if (!oReader.ReadNextNode())
				return;

			m_oTimelineCacheDefinition = oReader;
		}
		void CTimelineCacheFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			if (false == m_oTimelineCacheDefinition.IsInit()) return;

			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oTimelineCacheDefinition->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
			IFileContainer::Write(oPath, oDirectory, oContent);
		}
//-------------------------------------------------------------------------------------
		CTimelineRefs::CTimelineRefs() {}
		CTimelineRefs::~CTimelineRefs() {}
		void CTimelineRefs::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineRefs::toXML() const
		{
			return L"";
		}
		EElementType CTimelineRefs::getType() const
		{
			return et_x_TimelineRefs;
		}
		void CTimelineRefs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"timelineRef" == sName)
				{
					CTimelineRef* pTimelineRef = new CTimelineRef();
					*pTimelineRef = oReader;
					m_arrItems.push_back(pTimelineRef);
				}
			}
		}
		void CTimelineRefs::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<x15:timelineRefs>");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</x15:timelineRefs>");
		}
//--------------------------------------------------------------------------------------------------------------------------
		CTimelineRef::CTimelineRef() {}
		CTimelineRef::~CTimelineRef() {}
		void CTimelineRef::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineRef::toXML() const
		{
			return L"";
		}
		EElementType CTimelineRef::getType() const
		{
			return et_x_TimelineRef;
		}
		void CTimelineRef::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<x15:timelineRef");
			WritingStringNullableAttrString(L"r:id", m_oRId, m_oRId->ToString());
			writer.WriteString(L"/>");
		}
		void CTimelineRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
		}
		void CTimelineRef::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"r:id", m_oRId)
			WritingElement_ReadAttributes_End(oReader)
		}
//-------------------------------------------------------------------------------------
		CTimelineCacheRefs::CTimelineCacheRefs() {}
		CTimelineCacheRefs::~CTimelineCacheRefs() {}
		void CTimelineCacheRefs::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineCacheRefs::toXML() const
		{
			return L"";
		}
		EElementType CTimelineCacheRefs::getType() const
		{
			return et_x_TimelineCacheRefs;
		}
		void CTimelineCacheRefs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"timelineCacheRef" == sName)
				{
					CTimelineCacheRef* pTimelineCacheRef = new CTimelineCacheRef();
					*pTimelineCacheRef = oReader;
					m_arrItems.push_back(pTimelineCacheRef);
				}
			}
		}
		void CTimelineCacheRefs::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<x15:timelineCacheRefs>");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</x15:timelineCacheRefs>");
		}
//--------------------------------------------------------------------------------------------------------------------------
		CTimelineCacheRef::CTimelineCacheRef() {}
		CTimelineCacheRef::~CTimelineCacheRef() {}
		void CTimelineCacheRef::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTimelineCacheRef::toXML() const
		{
			return L"";
		}
		EElementType CTimelineCacheRef::getType() const
		{
			return et_x_TimelineCacheRef;
		}
		void CTimelineCacheRef::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<x15:timelineCacheRef");
			WritingStringNullableAttrString(L"r:id", m_oRId, m_oRId->ToString());
			writer.WriteString(L"/>");
		}
		void CTimelineCacheRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
		}
		void CTimelineCacheRef::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"r:id", m_oRId)
			WritingElement_ReadAttributes_End(oReader)
		}
	} //Spreadsheet
} // namespace OOX

