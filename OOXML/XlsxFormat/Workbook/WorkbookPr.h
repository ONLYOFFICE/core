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

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace SimpleTypes
{
	class CGuid;
	class COnOff;
	class CCryptAlgoritmName;
	class CUnsignedDecimalNumber;

	namespace Spreadsheet
	{
		class CUpdateLinksType;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CWorkbookPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CWorkbookPr)
            WritingElement_XlsbConstructors(CWorkbookPr)

			CWorkbookPr();
			virtual ~CWorkbookPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>						m_oAllowRefreshQuery;
			nullable<SimpleTypes::COnOff>						m_oAutoCompressPictures;
			nullable<SimpleTypes::COnOff>						m_oBackupFile;
			nullable<SimpleTypes::COnOff>						m_oCheckCompatibility;
			nullable<SimpleTypes::COnOff>						m_oDate1904;
			nullable<SimpleTypes::COnOff>						m_oDateCompatibility;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oDefaultThemeVersion;
			nullable<SimpleTypes::COnOff>						m_oFilterPrivacy;
			nullable<SimpleTypes::COnOff>						m_oHidePivotFieldList;
			nullable<SimpleTypes::COnOff>						m_oPromptedSolutions;
			nullable<SimpleTypes::COnOff>						m_oPublishItems;
			nullable<SimpleTypes::COnOff>						m_oRefreshAllConnections;
			nullable<SimpleTypes::COnOff>						m_oShowBorderUnselectedTables;
			nullable<SimpleTypes::COnOff>						m_oShowInkAnnotation;
			nullable<SimpleTypes::COnOff>						m_oShowObjects;
			nullable<SimpleTypes::COnOff>						m_oShowPivotChartFilter;
			nullable<SimpleTypes::Spreadsheet::CUpdateLinksType> m_oUpdateLinks;
			nullable_string										m_oCodeName;
		};
		class CWorkbookProtection : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CWorkbookProtection)
			WritingElement_XlsbConstructors(CWorkbookProtection)

			CWorkbookProtection();
			virtual ~CWorkbookProtection();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toXLS();
			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable<SimpleTypes::COnOff>		m_oLockRevision;
			nullable<SimpleTypes::COnOff>		m_oLockStructure;
			nullable<SimpleTypes::COnOff>		m_oLockWindows;

			nullable<SimpleTypes::CCryptAlgoritmName>		m_oWorkbookAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oWorkbookSpinCount;
			nullable_string									m_oWorkbookHashValue;
			nullable_string									m_oWorkbookSaltValue;

			nullable_string									m_oPassword; //for old wrike protection

			nullable<SimpleTypes::CCryptAlgoritmName>		m_oRevisionsAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oRevisionsSpinCount;
			nullable_string									m_oRevisionsHashValue;
			nullable_string									m_oRevisionsSaltValue;
		};
		class CFileSharing : public WritingElement
		{
		public:
            WritingElement_AdditionMethods(CFileSharing)
            WritingElement_XlsbConstructors(CFileSharing)

			CFileSharing();
			virtual ~CFileSharing();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
            std::vector<XLS::BaseObjectPtr> toBin();
			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable_bool									m_oReadOnlyRecommended;
			nullable_string									m_oUserName;

			nullable<SimpleTypes::CCryptAlgoritmName>		m_oAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oSpinCount;
			nullable_string									m_oHashValue;
			nullable_string									m_oSaltValue;

			nullable_string									m_oPassword; //for old wrike protection
		};
	} //Spreadsheet
} // namespace OOX
