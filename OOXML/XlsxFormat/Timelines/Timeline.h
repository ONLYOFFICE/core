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
#pragma once

#include "../Table/Autofilter.h"
#include "../../DocxFormat/IFileContainer.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineCachePivotTable : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelineCachePivotTable)

			CTimelineCachePivotTable();
			virtual ~CTimelineCachePivotTable();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oName;
			nullable_uint m_oTabId;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineRange : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelineRange)

			CTimelineRange();
			virtual ~CTimelineRange();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oStartDate;
			nullable_string m_oEndDate;

			std::wstring m_name;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineCachePivotTables : public WritingElementWithChilds<CTimelineCachePivotTable>
		{
		public:
			WritingElement_AdditionMethods(CTimelineCachePivotTables)
			CTimelineCachePivotTables();
			virtual ~CTimelineCachePivotTables();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};
//--------------------------------------------------------------------------------------------------------------
		class CTimelineState : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelineState)
			CTimelineState();
			virtual ~CTimelineState();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_oName;
			nullable_bool m_oSingleRangeFilterState;
			nullable_uint m_oPivotCacheId;
			nullable_uint m_oMinimalRefreshVersion;
			nullable_uint m_oLastRefreshVersion;
			nullable_string m_oFilterType;

			nullable<CTimelineRange> m_oSelection;
			nullable<CTimelineRange> m_oBounds;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
//--------------------------------------------------------------------------------------------------------------
		class CTimelinePivotFilter : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelinePivotFilter)
			CTimelinePivotFilter();
			virtual ~CTimelinePivotFilter();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_oName;
			nullable_string m_oDescription;
			nullable_bool m_oUseWholeDay;
			nullable_uint m_oId;
			nullable_uint m_oFld;

			nullable<CAutofilter> m_oAutoFilter;
		};
//--------------------------------------------------------------------------------------------------------------
		class CTimelineCacheDefinition : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelineCacheDefinition)
			CTimelineCacheDefinition();
			virtual ~CTimelineCacheDefinition();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_oName;
			nullable_string m_oSourceName;
			nullable_string m_oUid;

			nullable<CTimelineCachePivotTables> m_oPivotTables;
			nullable<CTimelineState> m_oState;
			nullable<CTimelinePivotFilter> m_oPivotFilter;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimeline : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimeline)
			
			CTimeline();
			virtual ~CTimeline();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;   
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oName;
			nullable_string m_oCaption;
			nullable_string m_oUid;
			nullable_string m_oScrollPosition;
			nullable_string m_oCache;
			nullable_uint m_oSelectionLevel;
			nullable_uint m_oLevel;			
			nullable_bool m_oShowHeader;
			nullable_bool m_oShowSelectionLabel;
			nullable_bool m_oShowTimeLevel;
			nullable_bool m_oShowHorizontalScrollbar;
			nullable<SimpleTypes::Spreadsheet::CTimelineStyle> m_oStyle;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelines : public WritingElementWithChilds<CTimeline>
		{
		public:
			WritingElement_AdditionMethods(CTimelines)
			CTimelines();
			virtual ~CTimelines();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			
			virtual EElementType getType () const;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CTimelineFile(OOX::Document* pMain);
			CTimelineFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CTimelineFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CTimelines> m_oTimelines;
		private:
			CPath m_oReadPath;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineCacheFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CTimelineCacheFile(OOX::Document* pMain);
			CTimelineCacheFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CTimelineCacheFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CTimelineCacheDefinition> m_oTimelineCacheDefinition;
		private:
			CPath m_oReadPath;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineRef : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelineRef)

			CTimelineRef();
			virtual ~CTimelineRef();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::CRelationshipId> m_oRId;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineRefs : public WritingElementWithChilds<CTimelineRef>
		{
		public:
			WritingElement_AdditionMethods(CTimelineRefs)
			CTimelineRefs();
			virtual ~CTimelineRefs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};	
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineCacheRef : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelineCacheRef)

			CTimelineCacheRef();
			virtual ~CTimelineCacheRef();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::CRelationshipId> m_oRId;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CTimelineCacheRefs : public WritingElementWithChilds<CTimelineCacheRef>
		{
		public:
			WritingElement_AdditionMethods(CTimelineCacheRefs)
			CTimelineCacheRefs();
			virtual ~CTimelineCacheRefs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};
//---------------------------------------------------------------------------------------------------------------------------		
		class CDrawingTimeslicer : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDrawingTimeslicer)
			CDrawingTimeslicer();
			virtual ~CDrawingTimeslicer();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oName;
		};
//----------------------------------------------------------------------------------------------------------------------------		
		class CTimelineStyleElement : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTimelineStyleElement)
			CTimelineStyleElement();
			virtual ~CTimelineStyleElement();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::Spreadsheet::CTimelineStyleType> m_oType;
			nullable_uint m_oDxfId;
		};
		class CTimelineStyle : public WritingElementWithChilds<CTimelineStyleElement>
		{
		public:
			WritingElement_AdditionMethods(CTimelineStyle)
			
			CTimelineStyle();
			virtual ~CTimelineStyle();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
			
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oName;
		};
		class CTimelineStyles : public WritingElementWithChilds<CTimelineStyle>
		{
		public:
			WritingElement_AdditionMethods(CTimelineStyles)
			CTimelineStyles();
			virtual ~CTimelineStyles();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
			
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oDefaultTimelineStyle;
		};
	} //Spreadsheet
} // namespace OOX
