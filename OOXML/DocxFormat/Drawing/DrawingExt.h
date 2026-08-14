/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace SimpleTypes
{
	class CGuid;
}
namespace PPTX
{
	namespace Logic
	{
		class CNvPr;
	}
}
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
        class CWorkbookPivotCaches;
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
            nullable<SimpleTypes::CGuid>							m_oId;
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
            nullable<OOX::Spreadsheet::CWorkbookPivotCaches>        m_oWorkbookPivotCaches;

			std::vector<OOX::Spreadsheet::CSlicerCachePivotTable*>	m_oSlicerCachePivotTables;
            nullable<OOX::Spreadsheet::CTableSlicerCache>           m_oTableSlicerCache;
            nullable<OOX::Spreadsheet::CSlicerCacheHideNoData>      m_oSlicerCacheHideItemsWithNoData;

			std::vector<OOX::Spreadsheet::CConditionalFormatting*>	m_arrConditionalFormatting;

			nullable<OOX::Spreadsheet::CDynamicArrayProperties>	m_oDynamicArrayProperties;
			nullable<OOX::Spreadsheet::CRichValueBlock>			m_oRichValueBlock;

			nullable<PPTX::Logic::CNvPr> m_oCNvPr;

			nullable<OOX::CPresenceInfo> m_oPresenceInfo;

			nullable_string m_oFileKey;
			nullable_string m_oInstanceId;

			nullable<OOX::Spreadsheet::CT_DLbl> m_oChartDataLabel;
			nullable<OOX::Spreadsheet::CSeriesFiltering> m_oChartFiltering;
			nullable_bool m_oDataDisplayNaAsBlank;

			nullable<OOX::Spreadsheet::CUserProtectedRanges> m_oUserProtectedRanges;

			nullable_bool m_oExternalLinksAutoRefresh;

			nullable_uint m_oXfComplement;
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
			XLS::BaseObjectPtr toBinMetadata();
			virtual EElementType getType() const;

            std::vector<OOX::Drawing::COfficeArtExtension*> m_arrExt;
		};

	} // namespace Drawing
} // namespace OOX

