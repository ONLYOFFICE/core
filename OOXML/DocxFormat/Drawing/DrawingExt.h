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

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace OOX
{
	class RId;
	class CPresenceInfo;

	namespace Spreadsheet
	{
		class CSparklineGroups;
		class CAltTextTable;
		class CConditionalFormatting;
		class CDataValidations;
		class CSlicerRefs;
		class CSlicerCaches;
		class CDxfs;
		class CSlicerStyles;
		class CSlicerCachePivotTable;
		class CTableSlicerCache;
		class CSlicerCacheHideNoData;
		class CConnection;
        class CPivotCacheDefinitionExt;
		class CT_DLbl;
		class CSeriesFiltering;
		class CUserProtectedRanges;
		class CTimelineRefs;
		class CTimelineCacheRefs;
		class CTimelineStyles;
		class CDynamicArrayProperties;
		class CRichValueBlock;
	}

	namespace Drawing
	{
		class CCompatExt : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CCompatExt)

			CCompatExt();
			virtual ~CCompatExt();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable_string m_sSpId;
		};

		class CDataModelExt : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDataModelExt)

			CDataModelExt();
			virtual ~CDataModelExt();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<OOX::RId> m_oRelId;
		};

		//--------------------------------------------------------------------------------
		// COfficeArtExtension 20.1.2.2.14 (Part 1)
		//--------------------------------------------------------------------------------

		class COfficeArtExtension : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(COfficeArtExtension)

			COfficeArtExtension();
			~COfficeArtExtension();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
            std::wstring toXMLWithNS(const std::wstring& sNamespace) const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable_string											m_sUri;
            std::wstring                                            m_sAdditionalNamespace;

            nullable<CCompatExt>                                    m_oCompatExt;
            nullable<OOX::Spreadsheet::CSparklineGroups>            m_oSparklineGroups;
            nullable<CDataModelExt>                                 m_oDataModelExt;
            nullable<OOX::Spreadsheet::CAltTextTable>               m_oAltTextTable;
            nullable_string											m_oId;
            nullable<OOX::Spreadsheet::CDataValidations>            m_oDataValidations;

            nullable<OOX::Spreadsheet::CConnection>                 m_oConnection;
            nullable<OOX::Spreadsheet::CDxfs>                       m_oDxfs;

            nullable<OOX::Spreadsheet::CSlicerRefs>                 m_oSlicerList;
            nullable<OOX::Spreadsheet::CSlicerRefs>                 m_oSlicerListExt;
            nullable<OOX::Spreadsheet::CSlicerCaches>               m_oSlicerCaches;
            nullable<OOX::Spreadsheet::CSlicerCaches>               m_oSlicerCachesExt;
            nullable<OOX::Spreadsheet::CSlicerStyles>               m_oSlicerStyles;

			nullable<OOX::Spreadsheet::CTimelineRefs>				m_oTimelineRefs;
			nullable<OOX::Spreadsheet::CTimelineCacheRefs>			m_oTimelineCacheRefs;
			nullable<OOX::Spreadsheet::CTimelineStyles>				m_oTimelineStyles;

            nullable<OOX::Spreadsheet::CPivotCacheDefinitionExt>    m_oPivotCacheDefinitionExt;

			std::vector<OOX::Spreadsheet::CSlicerCachePivotTable*>	m_oSlicerCachePivotTables;
            nullable<OOX::Spreadsheet::CTableSlicerCache>           m_oTableSlicerCache;
            nullable<OOX::Spreadsheet::CSlicerCacheHideNoData>      m_oSlicerCacheHideItemsWithNoData;

			std::vector<OOX::Spreadsheet::CConditionalFormatting*>	m_arrConditionalFormatting;

			nullable < OOX::Spreadsheet::CDynamicArrayProperties>	m_oDynamicArrayProperties;
			nullable < OOX::Spreadsheet::CRichValueBlock>			m_oRichValueBlock;

			nullable<OOX::CPresenceInfo> m_oPresenceInfo;

			nullable_string m_oFileKey;
			nullable_string m_oInstanceId;

			nullable<OOX::Spreadsheet::CT_DLbl> m_oChartDataLabel;
			nullable<OOX::Spreadsheet::CSeriesFiltering> m_oChartFiltering;
			nullable_bool m_oDataDisplayNaAsBlank;

			nullable<OOX::Spreadsheet::CUserProtectedRanges> m_oUserProtectedRanges;

			nullable_bool m_oExternalLinksAutoRefresh;
		};

		//--------------------------------------------------------------------------------
		// COfficeArtExtensionList 20.1.2.2.15 (Part 1)
		//--------------------------------------------------------------------------------

		class COfficeArtExtensionList : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(COfficeArtExtensionList)
			WritingElement_XlsbConstructors(COfficeArtExtensionList)

			COfficeArtExtensionList();
			virtual ~COfficeArtExtensionList();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual std::wstring toXML() const;
			std::wstring toXMLWithNS(const std::wstring& sNamespace) const;
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBinWorksheet();
			XLS::BaseObjectPtr toBinWorkBook();
			XLS::BaseObjectPtr toBinStyles();
			XLS::BaseObjectPtr toBinTable();
			XLS::BaseObjectPtr toBinSlicerCache();
			XLS::BaseObjectPtr toBinPivotCache();
			XLS::BaseObjectPtr toBinConnections();
			virtual EElementType getType() const;

            std::vector<OOX::Drawing::COfficeArtExtension*> m_arrExt;
		};

	} // namespace Drawing
} // namespace OOX

