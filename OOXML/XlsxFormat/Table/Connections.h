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
#include "../../DocxFormat/IFileContainer.h"

namespace SimpleTypes
{
	class CUnsignedDecimalNumber;
	class CGuid;

	namespace Spreadsheet
	{
		class CExternalConnectionType;
		class CParameterType;
		class CHtmlFormat;
		class CCredMethod;
		class CQualifier;
		class CFileType;
	}
}

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
		class CTextField : public WritingElement
		{
		public:
			CTextField();
			WritingElement_AdditionMethods(CTextField)
			WritingElement_XlsbConstructors(CTextField)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::Spreadsheet::CExternalConnectionType>		m_oType;
			nullable_int													m_oPosition;
		};

		class CTextFields : public WritingElementWithChilds<CTextField>
		{
		public:
			CTextFields();
			WritingElement_AdditionMethods(CTextFields)
			WritingElement_XlsbConstructors(CTextFields)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_int m_oCount;
		};

		class CParameter : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CParameter)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::Spreadsheet::CParameterType> m_oParameterType;

			nullable_string		m_oName;
			nullable_string		m_oPrompt;
			nullable_int		m_oSqlType;
			nullable_bool		m_oRefreshOnChange;

			nullable_string		m_oString;
			nullable_string		m_oCell;
			nullable_int		m_oInteger;
			nullable_double		m_oDouble;
			nullable_bool		m_oBoolean;
		};

		class CParameters : public WritingElementWithChilds<CParameter>
		{
		public:
			WritingElement_AdditionMethods(CParameters)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_int m_oCount;
		};

		class CRangePr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRangePr)
			WritingElement_XlsbConstructors(CRangePr)

			CRangePr();
			virtual ~CRangePr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string		m_oSourceName;
		};

		class CDbPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDbPr)
			WritingElement_XlsbConstructors(CDbPr)

			CDbPr();
			virtual ~CDbPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string		m_oConnection;
			nullable_string		m_oCommand;
			nullable_string		m_oServerCommand;
			nullable_int		m_oCommandType;
		};

		class COlapPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(COlapPr)
			WritingElement_XlsbConstructors(COlapPr)

			COlapPr();
			virtual ~COlapPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string	m_oLocalConnection;
			nullable_int	m_oRowDrillCount;
			nullable_bool	m_oLocal;
			nullable_bool	m_oLocalRefresh;
			nullable_bool	m_oSendLocale;
			nullable_bool	m_oServerNumberFormat;
			nullable_bool	m_oServerFont;
			nullable_bool	m_oServerFontColor;
		};

		class CWebPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CWebPr)
			WritingElement_XlsbConstructors(CWebPr)

			CWebPr();
			virtual ~CWebPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string		m_oUrl;
			nullable_string		m_oPost;
			nullable_string		m_oEditPage;
			nullable_bool		m_oXml;
			nullable_bool		m_oSourceData;
			nullable_bool		m_oConsecutive;
			nullable_bool		m_oFirstRow;
			nullable_bool		m_oXl97;
			nullable_bool		m_oTextDates;
			nullable_bool		m_oXl2000;
			nullable_bool		m_oHtmlTables;
			nullable<SimpleTypes::Spreadsheet::CHtmlFormat> m_oHtmlFormat;

			//nullable<CTables>					m_oTables;
		};

		class CTextPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTextPr)
			WritingElement_XlsbConstructors(CTextPr)

			CTextPr();
			virtual ~CTextPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string			m_oCharacterSet;
			nullable_string			m_oSourceFile;
			nullable_string			m_oDecimal;
			nullable_string			m_oDelimiter;
			nullable_string			m_oThousands;
			nullable_int			m_oFirstRow;

			nullable_int			m_oCodePage;

			nullable<SimpleTypes::Spreadsheet::CQualifier>	m_oQualifier;
			nullable<SimpleTypes::Spreadsheet::CFileType>	m_oFileType;

			nullable_bool			m_oPrompt;
			nullable_bool			m_oDelimited;
			nullable_bool			m_oTab;
			nullable_bool			m_oSpace;
			nullable_bool			m_oComma;
			nullable_bool			m_oSemicolon;
			nullable_bool			m_oConsecutive;

			nullable<CTextFields>	m_oTextFields;
		};

		class CConnection : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CConnection)
			WritingElement_XlsbConstructors(CConnection)

			CConnection();
			virtual ~CConnection();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toBin15();

			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_int		m_oType;	//EDatabaseSourceType
			nullable_string		m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oId;
			nullable_string		m_oIdExt;
			nullable<SimpleTypes::CGuid> m_oUId;

			nullable<CDbPr>		m_oDbPr;
			nullable<COlapPr>	m_oOlapPr;
			nullable<CTextPr>	m_oTextPr;
			nullable<CWebPr>	m_oWebPr;
			nullable<CRangePr>	m_oRangePr;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

			nullable<SimpleTypes::Spreadsheet::CCredMethod> m_oCredentials;
			nullable_bool		m_oBackground;
			nullable_bool		m_oDeleted;
			nullable_string		m_oDescription;
			nullable_int		m_oInterval;
			nullable_bool		m_oKeepAlive;
			nullable_int		m_oMinRefreshableVersion;
			nullable_bool		m_oNew;
			nullable_string		m_oOdcFile;
			nullable_bool		m_oOnlyUseConnectionFile;
			nullable_int		m_oReconnectionMethod;
			nullable_int		m_oRefreshedVersion;
			nullable_bool		m_oRefreshOnLoad;
			nullable_bool		m_oSaveData;
			nullable_bool		m_oSavePassword;
			nullable_string		m_oSingleSignOnId;
			nullable_string		m_oSourceFile;
		};

		class CConnections : public WritingElementWithChilds<CConnection>
		{
		public:
			WritingElement_AdditionMethods(CConnections)
			WritingElement_XlsbConstructors(CConnections)

			CConnections();
			virtual ~CConnections();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_int m_oCount;
		};

		class CConnectionsFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CConnectionsFile(OOX::Document* pMain);
			CConnectionsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CConnectionsFile();

			virtual void read(const CPath& oPath);
			void readBin(const CPath& oPath);
			XLS::BaseObjectPtr WriteBin() const;
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CConnections>	m_oConnections;

		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} //Spreadsheet
} // namespace OOX
