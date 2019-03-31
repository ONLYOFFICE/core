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
			WritingElement_AdditionConstructors(CTextField)

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<textField");
					WritingStringNullableAttrString	(L"type", m_oType, m_oType->ToString())
					WritingStringNullableAttrInt	(L"position", m_oPosition, *m_oPosition)
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
			}
			virtual EElementType getType () const
			{
				return et_x_textField;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"type",		m_oType )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"position",	m_oPosition )
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::Spreadsheet::CExternalConnectionType<>>	m_oType;
			nullable_int													m_oPosition;
		};
		class CTextFields : public WritingElementWithChilds<CTextField>
		{
		public:
			WritingElement_AdditionConstructors(CTextFields)

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<textField ");
					WritingStringAttrInt(L"count", (int)m_arrItems.size());
				writer.WriteString(L">");

				for (size_t i = 0; i < m_arrItems.size(); i++)
				{
					if (m_arrItems[i])
						m_arrItems[i]->toXML(writer);
				}
				writer.WriteString(L"</textField>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"textField" == sName )
					{
						CTextField *pPara = new CTextField(oReader);
						m_arrItems.push_back(pPara);
					}
				}
			}
			virtual EElementType getType () const
			{
				return et_x_textFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, L"count", m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}

			nullable_int m_oCount;
		};

		class CParameter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CParameter)

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<parameter");
					WritingStringNullableAttrString	(L"name",			m_oName,		*m_oName)
					WritingStringNullableAttrString	(L"parameterType",	m_oParameterType, m_oParameterType->ToString())
					WritingStringNullableAttrInt	(L"sqlType",		m_oSqlType,		*m_oSqlType)
					WritingStringNullableAttrString	(L"string",			m_oString,		*m_oString)
					WritingStringNullableAttrString	(L"cell",			m_oCell,		*m_oCell)
					WritingStringNullableAttrInt	(L"integer",		m_oInteger,		*m_oInteger)
					WritingStringNullableAttrDouble	(L"double",			m_oDouble,		*m_oDouble)
					WritingStringNullableAttrBool2	(L"boolean",		m_oBoolean)
					WritingStringNullableAttrBool2	(L"refreshOnChange",m_oRefreshOnChange)
					WritingStringNullableAttrEncodeXmlString(L"prompt",	m_oPrompt,		*m_oPrompt)
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
			}
			virtual EElementType getType () const
			{
				return et_x_parameter;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"name",		m_oName )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"prompt",	m_oPrompt )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"sqlType",	m_oSqlType )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshOnChange",	m_oRefreshOnChange )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"parameterType",	m_oParameterType )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"integer",	m_oInteger )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"cell",		m_oCell )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"string",	m_oString )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"boolean",	m_oBoolean )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"double",	m_oDouble )
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::Spreadsheet::CParameterType<>> m_oParameterType;

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

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<parameters ");
					WritingStringAttrInt(L"count", (int)m_arrItems.size());
				writer.WriteString(L">");

				for (size_t i = 0; i < m_arrItems.size(); i++)
				{
					if (m_arrItems[i])
						m_arrItems[i]->toXML(writer);
				}
				writer.WriteString(L"</parameters>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"Parameter" == sName )
					{
						CParameter *pPara = new CParameter(oReader);
						m_arrItems.push_back(pPara);
					}
				}
			}
			virtual EElementType getType () const
			{
				return et_x_parameters;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, L"count", m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}

			nullable_int m_oCount;
		};
		class CDbPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDbPr)
			CDbPr() {}
			virtual ~CDbPr() {}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<dbPr");
					WritingStringNullableAttrEncodeXmlString(L"connection",		m_oConnection,		*m_oConnection)
					WritingStringNullableAttrEncodeXmlString(L"command",		m_oCommand,			*m_oCommand)
					WritingStringNullableAttrEncodeXmlString(L"serverCommand",	m_oServerCommand,	*m_oServerCommand)
					WritingStringNullableAttrInt			(L"commandType",	m_oCommandType,		*m_oCommandType)
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
			}
			virtual EElementType getType () const
			{
				return et_x_dbPr;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"connection",		m_oConnection )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"command",			m_oCommand )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"serverCommand",	m_oServerCommand )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"commandType",		m_oCommandType )
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable_string		m_oConnection;
			nullable_string		m_oCommand;
			nullable_string		m_oServerCommand;
			nullable_int		m_oCommandType;
		};

		class COlapPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapPr)

			COlapPr() {}
			virtual ~COlapPr() {}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<olapPr");
					WritingStringNullableAttrEncodeXmlString (L"localConnection",	m_oLocalConnection,	*m_oLocalConnection)
					WritingStringNullableAttrInt	(L"rowDrillCount",		m_oRowDrillCount,	*m_oRowDrillCount)
					WritingStringNullableAttrBool2	(L"local",				m_oLocal)
					WritingStringNullableAttrBool2	(L"localRefresh",		m_oLocalRefresh)
					WritingStringNullableAttrBool2	(L"sendLocale",			m_oSendLocale)
					WritingStringNullableAttrBool2	(L"serverNumberFormat",	m_oServerNumberFormat)
					WritingStringNullableAttrBool2	(L"serverFont",			m_oServerFont)
					WritingStringNullableAttrBool2	(L"serverFontColor",	m_oServerFontColor)
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
			}
			virtual EElementType getType () const
			{
				return et_x_olapPr;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"localConnection",	m_oLocalConnection )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"rowDrillCount",	m_oRowDrillCount )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"local",			m_oLocal )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"localRefresh",		m_oLocalRefresh )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"sendLocale",		m_oSendLocale )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"serverNumberFormat",m_oServerNumberFormat )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"serverFont",		m_oServerFont )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"serverFontColor",	m_oServerFontColor )
				WritingElement_ReadAttributes_End( oReader )
			}
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

			CWebPr() {}
			virtual ~CWebPr() {}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<webPr");
					WritingStringNullableAttrString	(L"url",		m_oUrl,			*m_oUrl)
					WritingStringNullableAttrString	(L"post",		m_oPost,		*m_oPost)
					WritingStringNullableAttrString	(L"editPage",	m_oEditPage,	*m_oEditPage)
					WritingStringNullableAttrString	(L"htmlFormat",	m_oHtmlFormat,	m_oHtmlFormat->ToString())
					WritingStringNullableAttrBool2	(L"sourceData",	m_oSourceData)
					WritingStringNullableAttrBool2	(L"consecutive",m_oConsecutive)
					WritingStringNullableAttrBool2	(L"firstRow",	m_oFirstRow)
					WritingStringNullableAttrBool2	(L"xl97",		m_oXl97)
					WritingStringNullableAttrBool2	(L"textDates",	m_oTextDates)
					WritingStringNullableAttrBool2	(L"xl2000",		m_oXl2000)
					WritingStringNullableAttrBool2	(L"htmlTables",	m_oHtmlTables)
					WritingStringNullableAttrBool2	(L"xml",		m_oXml)

				//if (m_oTables.IsInit())
				//{
				//	writer.WriteString(L">");
				//		m_oTables->toXML(writer);
				//	writer.WriteString(L"</webPr>");
				//}
				//else
				//{
					writer.WriteString(L"/>");
				//}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
				
				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					//if (L"tables" == sName)
					//	m_oTables = oReader;
				}
			}
			virtual EElementType getType () const
			{
				return et_x_webPr;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"url",			m_oUrl )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"post",			m_oPost )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"editPage",		m_oEditPage )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"htmlFormat",	m_oHtmlFormat )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"xml",			m_oXml )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"sourceData",	m_oSourceData )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"consecutive",	m_oConsecutive )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"firstRow",		m_oFirstRow )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"xl97",			m_oXl97 )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"textDates",	m_oTextDates )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"xl2000",		m_oXl2000 )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"htmlTables",	m_oHtmlTables )
				WritingElement_ReadAttributes_End( oReader )
			}
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
			nullable<SimpleTypes::Spreadsheet::CHtmlFormat<>> m_oHtmlFormat;

			//nullable<CTables>					m_oTables;
		};
		class CTextPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTextPr)

			CTextPr() {}
			virtual ~CTextPr() {}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<textPr");
					WritingStringNullableAttrString	(L"characterSet",m_oCharacterSet,	*m_oCharacterSet)
					WritingStringNullableAttrEncodeXmlString	(L"sourceFile",	m_oSourceFile,		*m_oSourceFile)
					WritingStringNullableAttrEncodeXmlString	(L"decimal",	m_oDecimal,			*m_oDecimal)
					WritingStringNullableAttrEncodeXmlString	(L"delimiter",	m_oDelimiter,		*m_oDelimiter)
					WritingStringNullableAttrEncodeXmlString	(L"thousands",	m_oThousands,		*m_oThousands)
					WritingStringNullableAttrString	(L"qualifier",	m_oQualifier,		m_oQualifier->ToString())
					WritingStringNullableAttrString	(L"fileType",	m_oFileType,		m_oFileType->ToString())
					WritingStringNullableAttrInt	(L"firstRow",	m_oFirstRow,		*m_oFirstRow)
					WritingStringNullableAttrBool2	(L"prompt",		m_oPrompt)
					WritingStringNullableAttrBool2	(L"delimited",	m_oDelimited)
					WritingStringNullableAttrBool2	(L"tab",		m_oTab)
					WritingStringNullableAttrBool2	(L"space",		m_oSpace)
					WritingStringNullableAttrBool2	(L"comma",		m_oComma)
					WritingStringNullableAttrBool2	(L"semicolon",	m_oSemicolon)
					WritingStringNullableAttrBool2	(L"consecutive",m_oConsecutive)

				if (m_oTextFields.IsInit())
				{
					writer.WriteString(L">");
						m_oTextFields->toXML(writer);
					writer.WriteString(L"</textPr>");
				}
				else
				{
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
				
				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (L"textFields" == sName)
						m_oTextFields = oReader;
				}
			}
			virtual EElementType getType () const
			{
				return et_x_textPr;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"characterSet",	m_oCharacterSet )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"decimal",		m_oDecimal )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"delimiter",	m_oDelimiter )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"thousands",	m_oThousands )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"sourceFile",	m_oSourceFile )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"firstRow",		m_oFirstRow )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"qualifier",	m_oQualifier )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"fileType",		m_oFileType )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"prompt",		m_oPrompt )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"delimited",	m_oDelimited )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"tab",			m_oTab )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"space",		m_oSpace )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"comma",		m_oComma )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"semicolon",	m_oSemicolon )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"consecutive",	m_oConsecutive )
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable_string			m_oCharacterSet;
			nullable_string			m_oSourceFile;
			nullable_string			m_oDecimal;
			nullable_string			m_oDelimiter;
			nullable_string			m_oThousands;
			nullable_int			m_oFirstRow;

			nullable<SimpleTypes::Spreadsheet::CQualifier<>> m_oQualifier;
			nullable<SimpleTypes::Spreadsheet::CFileType<>>	m_oFileType;

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

			CConnection() {}
			virtual ~CConnection()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(false == m_oName.IsInit()) return;

				writer.WriteString(L"<connection");
					WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue())
					WritingStringNullableAttrEncodeXmlString(L"odcFile", m_oOdcFile, *m_oOdcFile)
					WritingStringNullableAttrBool2(L"keepAlive", m_oKeepAlive)
					WritingStringNullableAttrEncodeXmlString(L"name", m_oName, *m_oName)
					WritingStringNullableAttrEncodeXmlString(L"description", m_oDescription, *m_oDescription)
					WritingStringNullableAttrInt(L"type", m_oType, *m_oType)
					WritingStringNullableAttrInt(L"interval", m_oInterval, *m_oInterval)
					WritingStringNullableAttrBool2(L"background", m_oBackground)
					WritingStringNullableAttrBool2(L"deleted", m_oDeleted)
					WritingStringNullableAttrBool2(L"new", m_oNew)
					WritingStringNullableAttrBool2(L"refreshOnLoad", m_oRefreshOnLoad)
					WritingStringNullableAttrBool2(L"savePassword", m_oSavePassword)
					WritingStringNullableAttrInt(L"minRefreshableVersion", m_oMinRefreshableVersion, *m_oMinRefreshableVersion)
					WritingStringNullableAttrInt(L"refreshedVersion", m_oRefreshedVersion, *m_oRefreshedVersion)
					WritingStringNullableAttrInt(L"reconnectionMethod", m_oReconnectionMethod, *m_oReconnectionMethod)
					WritingStringNullableAttrBool2(L"onlyUseConnectionFile", m_oOnlyUseConnectionFile)
					WritingStringNullableAttrString(L"singleSignOnId", m_oSingleSignOnId, *m_oSingleSignOnId)
					WritingStringNullableAttrString(L"sourceFile", m_oSourceFile, *m_oSourceFile)
					WritingStringNullableAttrBool2(L"saveData", m_oSaveData)
				writer.WriteString(L">");
					if (m_oDbPr.IsInit())	m_oDbPr->toXML(writer);
					if (m_oOlapPr.IsInit()) m_oOlapPr->toXML(writer);
					if (m_oTextPr.IsInit()) m_oTextPr->toXML(writer);
					if (m_oWebPr.IsInit())	m_oWebPr->toXML(writer);
					//if (m_oExtLst.IsInit()) m_oExtLst->toXML(writer);
				writer.WriteString(L"</connection>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"dbPr" == sName )
						m_oDbPr = oReader;
					else if ( L"olapPr" == sName )
						m_oOlapPr = oReader;
					else if ( L"textPr" == sName )
						m_oTextPr = oReader;
					else if ( L"webPr" == sName )
						m_oWebPr = oReader;
					else if (L"extLst" == sName)
						m_oExtLst = oReader;
				}
			}
			virtual EElementType getType () const
			{
				return et_x_Connection;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"type",	m_oType )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"name",	m_oName )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"id",	m_oId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"background",	m_oBackground )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"credentials",	m_oCredentials )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"deleted",		m_oDeleted )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"description",	m_oDescription )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"interval",		m_oInterval )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"keepAlive",	m_oKeepAlive )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"minRefreshableVersion", m_oMinRefreshableVersion )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"new", m_oNew )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"odcFile", m_oOdcFile )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"onlyUseConnectionFile", m_oOnlyUseConnectionFile )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"reconnectionMethod", m_oReconnectionMethod )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshedVersion", m_oRefreshedVersion )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshOnLoad", m_oRefreshOnLoad )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"saveData", m_oSaveData )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"savePassword", m_oSavePassword )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"singleSignOnId", m_oSingleSignOnId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"sourceFile", m_oSourceFile )
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable_int		m_oType;	//EDatabaseSourceType
			nullable_string		m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oId;

			nullable<CDbPr>		m_oDbPr;
			nullable<COlapPr>	m_oOlapPr;
			nullable<CTextPr>	m_oTextPr;
			nullable<CWebPr>	m_oWebPr;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

			nullable<SimpleTypes::Spreadsheet::CCredMethod<>> m_oCredentials;
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
			CConnections()
			{
			}
			virtual ~CConnections()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<connections \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\">");

				//WritingStringAttrInt(L"count", (int)m_arrItems.size());
				//writer.WriteString(L">");

				for (size_t i = 0; i < m_arrItems.size(); i++)
				{
					if (m_arrItems[i])
						m_arrItems[i]->toXML(writer);
				}
				writer.WriteString(L"</connections>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"connection" == sName )
					{
						CConnection *pConn = new CConnection(oReader);
						m_arrItems.push_back(pConn);
					}
				}
			}
			virtual EElementType getType () const
			{
				return et_x_Connections;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, L"count", m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}

			nullable_int m_oCount;
		};
		class CConnectionsFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CConnectionsFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CConnectionsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CConnectionsFile()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				m_oConnections = oReader;
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				if(false == m_oConnections.IsInit()) return;

				NSStringUtils::CStringBuilder sXml;

				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				m_oConnections->toXML(sXml);

                std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write( oPath, oDirectory, oContent );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Connections;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}

			nullable<CConnections>	m_oConnections;
		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX
