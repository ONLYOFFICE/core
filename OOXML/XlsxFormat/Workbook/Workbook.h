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

#include "../Xlsx.h"
#include "../XlsxFlat.h"
#include "../../XlsbFormat/Xlsb.h"

#include "../WritingElement.h"

#include "BookViews.h"
#include "CalcPr.h"
#include "DefinedNames.h"
#include "Sheets.h"
#include "WorkbookPr.h"
#include "ExternalReferences.h"

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
		class CPersonList;

	//not implemented children:
		//<customWorkbookViews>
		//<extLst>
		//<fileRecoveryPr>
		//<fileVersion>
		//<functionGroups>
		//<smartTagPr>
		//<smartTagTypes>
		//<webPublishing>
		//<webPublishObjects>

		class CWorkbookPivotCache : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CWorkbookPivotCache)
			WritingElement_XlsbConstructors(CWorkbookPivotCache)

			CWorkbookPivotCache();
			virtual ~CWorkbookPivotCache();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			XLS::BaseObjectPtr toBin();
            XLS::BaseObjectPtr toBin14();
			XLS::BaseObjectPtr toXLS();
			void fromBin(XLS::BaseObjectPtr& obj);
            void fromBin14(XLS::BaseObjectPtr& obj);
			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	//----------
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCacheId;
			nullable<SimpleTypes::CRelationshipId>			m_oRid;
		};

		class CWorkbookPivotCaches : public WritingElementWithChilds<CWorkbookPivotCache>
		{
		public:
			WritingElement_AdditionMethods(CWorkbookPivotCaches)
            WritingElement_XlsbConstructors(CWorkbookPivotCaches)
			CWorkbookPivotCaches(OOX::Document *pMain = NULL);
			virtual ~CWorkbookPivotCaches();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			XLS::BaseObjectPtr toBin();
            XLS::BaseObjectPtr toBin14();
            void fromBin14(XLS::BaseObjectPtr& obj);
			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType() const;

            bool pivotCaches14 = false;
		};

//-----------------------------------------------------------------------------------------------------------

		class CWorkbook : public OOX::File, public OOX::IFileContainer, public WritingElement
		{
		public:
			CWorkbook(OOX::Document* pMain);
			CWorkbook(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CWorkbook();

			void readBin(const CPath& oPath);
			XLS::BaseObjectPtr WriteBin() const;
			XLS::BaseObjectPtr toXLS() const;
			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual EElementType getType () const;
			const CPath& GetReadPath();

			void PrepareToWrite();
			LONG GetActiveSheetIndex();
			XLS::BaseObjectPtr WriteXtiRefs() const;
			XLS::BaseObjectPtr WriteXtiRefsXLS() const;

			CPath											m_oReadPath;

			nullable<OOX::Spreadsheet::CWorkbookProtection>	m_oWorkbookProtection;
			nullable<OOX::Spreadsheet::CBookViews>			m_oBookViews;
            nullable<OOX::Spreadsheet::CDefinedNames>		m_oDefinedNames;
			nullable<OOX::Spreadsheet::CSheets>				m_oSheets;
			nullable<OOX::Spreadsheet::CWorkbookPr>			m_oWorkbookPr;
			nullable<OOX::Spreadsheet::CExternalReferences>	m_oExternalReferences;
            nullable<OOX::Spreadsheet::CCalcPr>				m_oCalcPr;
			nullable_string									m_oAppName;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable_string									m_oOleSize;
			nullable<OOX::Spreadsheet::CWorkbookPivotCaches>m_oPivotCaches;
			nullable<std::wstring>							m_oPivotCachesXml;
			nullable<OOX::Spreadsheet::CFileSharing>		m_oFileSharing;

			CPersonList*									m_pPersonList;
			bool											m_bMacroEnabled;
		};

	} //Spreadsheet
} // namespace OOX

