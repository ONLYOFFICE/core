﻿/*
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
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>						m_oAllowRefreshQuery;
			nullable<SimpleTypes::COnOff>						m_oAutoCompressPictures;
			nullable<SimpleTypes::COnOff>						m_oBackupFile;
			nullable<SimpleTypes::COnOff>						m_oCheckCompatibility;
			nullable<SimpleTypes::COnOff>	  					m_oCodeName;
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
