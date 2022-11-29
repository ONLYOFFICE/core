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

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		class CTextField : public WritingElement
		{
		public:
			CTextField();
			WritingElement_AdditionConstructors(CTextField)
			WritingElement_XlsbConstructors(CTextField)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CTextFields)
			WritingElement_XlsbConstructors(CTextFields)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_int m_oCount;
		};

		class CParameter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CParameter)

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
			WritingElement_AdditionConstructors(CParameters)

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
			WritingElement_AdditionConstructors(CRangePr)
			WritingElement_XlsbConstructors(CRangePr)

			CRangePr();
			virtual ~CRangePr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string		m_oSourceName;
		};

		class CDbPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDbPr)
			WritingElement_XlsbConstructors(CDbPr)

			CDbPr();
			virtual ~CDbPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

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
			WritingElement_AdditionConstructors(COlapPr)
			WritingElement_XlsbConstructors(COlapPr)

			COlapPr();
			virtual ~COlapPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CWebPr)
			WritingElement_XlsbConstructors(CWebPr)

			CWebPr();
			virtual ~CWebPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

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
			WritingElement_AdditionConstructors(CTextPr)
			WritingElement_XlsbConstructors(CTextPr)

			CTextPr();
			virtual ~CTextPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

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
			WritingElement_AdditionConstructors(CConnection)
			WritingElement_XlsbConstructors(CConnection)

			CConnection();
			virtual ~CConnection();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_int		m_oType;	//EDatabaseSourceType
			nullable_string		m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oId;
			nullable_string		m_oIdExt;
			nullable_string		m_oUId;

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
			WritingElement_AdditionConstructors(CConnections)
			WritingElement_XlsbConstructors(CConnections)

			CConnections();
			virtual ~CConnections();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

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
