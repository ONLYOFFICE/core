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
#pragma once

#include "../../Base/Nullable.h"
#include "../WritingElement.h"
#include "../RId.h"

namespace OOX
{
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
	}

	namespace Drawing
	{
		class CCompatExt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCompatExt)

			CCompatExt();
			virtual ~CCompatExt();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<std::wstring> m_sSpId;
		};

		class CDataModelExt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataModelExt)

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
			WritingElement_AdditionConstructors(COfficeArtExtension)

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
            nullable<std::wstring>                                  m_sUri;
            std::wstring                                            m_sAdditionalNamespace;

            nullable<CCompatExt>                                    m_oCompatExt;
            nullable<OOX::Spreadsheet::CSparklineGroups>            m_oSparklineGroups;
            nullable<CDataModelExt>                                 m_oDataModelExt;
            nullable<OOX::Spreadsheet::CAltTextTable>               m_oAltTextTable;
            nullable<std::wstring>                                  m_oId;
            nullable<OOX::Spreadsheet::CDataValidations>            m_oDataValidations;

            nullable<OOX::Spreadsheet::CConnection>                 m_oConnection;
            nullable<OOX::Spreadsheet::CDxfs>                       m_oDxfs;

            nullable<OOX::Spreadsheet::CSlicerRefs>                 m_oSlicerList;
            nullable<OOX::Spreadsheet::CSlicerRefs>                 m_oSlicerListExt;
            nullable<OOX::Spreadsheet::CSlicerCaches>               m_oSlicerCaches;
            nullable<OOX::Spreadsheet::CSlicerCaches>               m_oSlicerCachesExt;
            nullable<OOX::Spreadsheet::CSlicerStyles>               m_oSlicerStyles;

            nullable<OOX::Spreadsheet::CPivotCacheDefinitionExt>    m_oPivotCacheDefinitionExt;

			std::vector<OOX::Spreadsheet::CSlicerCachePivotTable*>	m_oSlicerCachePivotTables;
            nullable<OOX::Spreadsheet::CTableSlicerCache>           m_oTableSlicerCache;
            nullable<OOX::Spreadsheet::CSlicerCacheHideNoData>      m_oSlicerCacheHideItemsWithNoData;
			
			std::vector<OOX::Spreadsheet::CConditionalFormatting*>	m_arrConditionalFormatting;

			nullable<OOX::CPresenceInfo> m_oPresenceInfo;

			nullable_string m_oFileKey;
			nullable_string m_oInstanceId;
		};

		//--------------------------------------------------------------------------------
		// COfficeArtExtensionList 20.1.2.2.15 (Part 1)
		//--------------------------------------------------------------------------------	

		class COfficeArtExtensionList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COfficeArtExtensionList)
			WritingElement_XlsbConstructors(COfficeArtExtensionList)

			COfficeArtExtensionList();
			virtual ~COfficeArtExtensionList();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual std::wstring toXML() const;
			std::wstring toXMLWithNS(const std::wstring& sNamespace) const;
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType() const;

            std::vector<OOX::Drawing::COfficeArtExtension*> m_arrExt;
		};

	} // namespace Drawing
} // namespace OOX

