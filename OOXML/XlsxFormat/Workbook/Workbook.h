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

#include "../Xlsx.h"
#include "../XlsxFlat.h"
#include "../../XlsbFormat/Xlsb.h"

#include "../CommonInclude.h"

#include "BookViews.h"
#include "CalcPr.h"
#include "DefinedNames.h"
#include "Sheets.h"
#include "WorkbookPr.h"
#include "ExternalReferences.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPersonList;

	//необработанные child:
		//<customWorkbookViews>
		//<extLst>
		//<fileRecoveryPr>
		//<fileSharing>
		//<fileVersion>
		//<functionGroups>
		//<smartTagPr>
		//<smartTagTypes>
		//<webPublishing>
		//<webPublishObjects>

		class CWorkbookPivotCache : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWorkbookPivotCache)
			WritingElement_XlsbConstructors(CWorkbookPivotCache)
			CWorkbookPivotCache();
			virtual ~CWorkbookPivotCache();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CWorkbookPivotCaches)
            WritingElement_XlsbConstructors(CWorkbookPivotCaches)
			CWorkbookPivotCaches(OOX::Document *pMain = NULL);
			virtual ~CWorkbookPivotCaches();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType() const;
		};

//-----------------------------------------------------------------------------------------------------------

		class CWorkbook : public OOX::File, public OOX::IFileContainer, public WritingElement
		{
		public:
			CWorkbook(OOX::Document* pMain);
			CWorkbook(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CWorkbook();

			void readBin(const CPath& oPath);
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
			
			CPersonList*									m_pPersonList;
			bool											m_bMacroEnabled;
		};

	} //Spreadsheet
} // namespace OOX

