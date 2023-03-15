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

#include "Connections.h"

#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/ConnectionsStream.h"
#include "../../XlsbFormat/Biff12_unions/EXTCONNECTIONS.h"
#include "../../XlsbFormat/Biff12_unions/EXTCONNECTION.h"
#include "../../XlsbFormat/Biff12_records/BeginExtConnection.h"
#include "../../XlsbFormat/Biff12_unions/ECDBPROPS.h"
#include "../../XlsbFormat/Biff12_records/BeginECDbProps.h"
#include "../../XlsbFormat/Biff12_unions/ECOLAPPROPS.h"
#include "../../XlsbFormat/Biff12_records/BeginECOlapProps.h"
#include "../../XlsbFormat/Biff12_unions/ECTXTWIZ.h"
#include "../../XlsbFormat/Biff12_records/BeginECTxtWiz.h"
#include "../../XlsbFormat/Biff12_unions/ECTWFLDINFOLST.h"
#include "../../XlsbFormat/Biff12_records/BeginECTwFldInfo.h"
#include "../../XlsbFormat/Biff12_unions/ECWEBPROPS.h"
#include "../../XlsbFormat/Biff12_records/BeginECWebProps.h"
#include "../../XlsbFormat/Biff12_unions/EXTCONN15.h"
#include "../../XlsbFormat/Biff12_records/BeginExtConn15.h"
#include "../../XlsbFormat/Biff12_records/RangePr15.h"
namespace OOX
{
	namespace Spreadsheet
	{
		CTextField::CTextField() {}
		void CTextField::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTextField::toXML() const
		{
			return L"";
		}
		void CTextField::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<textField");
			WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString())
				WritingStringNullableAttrInt(L"position", m_oPosition, *m_oPosition)
				writer.WriteString(L"/>");
		}
		void CTextField::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CTextField::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CTextField::getType() const
		{
			return et_x_textField;
		}
		void CTextField::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginECTwFldInfo*>(obj.get());
			if (ptr != nullptr)
			{
				m_oType = (SimpleTypes::Spreadsheet::EExternalConnectionType)ptr->data.fieldType;
				m_oPosition = ptr->data.fieldStart;
			}
		}
		void CTextField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"type", m_oType)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"position", m_oPosition)
			WritingElement_ReadAttributes_End(oReader)
		}

		CTextFields::CTextFields() {}
		void CTextFields::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTextFields::toXML() const
		{
			return L"";
		}
		void CTextFields::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<textFields ");
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
			writer.WriteString(L">");

			for (size_t i = 0; i < m_arrItems.size(); i++)
			{
				if (m_arrItems[i])
					m_arrItems[i]->toXML(writer);
			}
			writer.WriteString(L"</textFields>");
		}
		void CTextFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"textField" == sName)
				{
					CTextField *pItem = new CTextField(oReader);
					m_arrItems.push_back(pItem);
				}
			}
		}
		void CTextFields::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ECTWFLDINFOLST*>(obj.get());
			if (ptr != nullptr)
			{
				m_oCount = ptr->m_arBrtBeginECTwFldInfo.size();

				for (auto &textField : ptr->m_arBrtBeginECTwFldInfo)
					m_arrItems.push_back(new CTextField(textField));
			}
		}
		EElementType CTextFields::getType() const
		{
			return et_x_textFields;
		}
		void CTextFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"count", m_oCount)
			WritingElement_ReadAttributes_End(oReader)
		}

		void CParameter::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CParameter::toXML() const
		{
			return L"";
		}
		void CParameter::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<parameter");
			WritingStringNullableAttrString(L"name", m_oName, *m_oName)
				WritingStringNullableAttrString(L"parameterType", m_oParameterType, m_oParameterType->ToString())
				WritingStringNullableAttrInt(L"sqlType", m_oSqlType, *m_oSqlType)
				WritingStringNullableAttrString(L"string", m_oString, *m_oString)
				WritingStringNullableAttrString(L"cell", m_oCell, *m_oCell)
				WritingStringNullableAttrInt(L"integer", m_oInteger, *m_oInteger)
				WritingStringNullableAttrDouble(L"double", m_oDouble, *m_oDouble)
				WritingStringNullableAttrBool2(L"boolean", m_oBoolean)
				WritingStringNullableAttrBool2(L"refreshOnChange", m_oRefreshOnChange)
				WritingStringNullableAttrEncodeXmlString(L"prompt", m_oPrompt, *m_oPrompt)
				writer.WriteString(L"/>");
		}
		void CParameter::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		EElementType CParameter::getType() const
		{
			return et_x_parameter;
		}
		void CParameter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"prompt", m_oPrompt)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"sqlType", m_oSqlType)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"refreshOnChange", m_oRefreshOnChange)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"parameterType", m_oParameterType)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"integer", m_oInteger)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"cell", m_oCell)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"string", m_oString)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"boolean", m_oBoolean)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"double", m_oDouble)
			WritingElement_ReadAttributes_End(oReader)
		}

		void CParameters::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CParameters::toXML() const
		{
			return L"";
		}
		void CParameters::toXML(NSStringUtils::CStringBuilder& writer) const
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
		void CParameters::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"Parameter" == sName)
				{
					CParameter *pPara = new CParameter(oReader);
					m_arrItems.push_back(pPara);
				}
			}
		}
		EElementType CParameters::getType() const
		{
			return et_x_parameters;
		}
		void CParameters::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"count", m_oCount)
			WritingElement_ReadAttributes_End(oReader)
		}

		CRangePr::CRangePr() {}
		CRangePr::~CRangePr() {}
		void CRangePr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRangePr::toXML() const
		{
			return L"";
		}
		void CRangePr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<x15:rangePr");
			WritingStringNullableAttrEncodeXmlString(L"sourceName", m_oSourceName, *m_oSourceName)
				writer.WriteString(L"/>");
		}
		void CRangePr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CRangePr::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CRangePr::getType() const
		{
			return et_x_rangePr;
		}
		void CRangePr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::RangePr15*>(obj.get());
			if (ptr != nullptr)
			{
				if (!ptr->irstSourceName.value().empty())
					m_oSourceName = ptr->irstSourceName.value();
			}
		}
		void CRangePr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"sourceName", m_oSourceName)
			WritingElement_ReadAttributes_End(oReader)
		}

		CDbPr::CDbPr() {}
		CDbPr::~CDbPr() {}
		void CDbPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDbPr::toXML() const
		{
			return L"";
		}
		void CDbPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<dbPr");
			WritingStringNullableAttrEncodeXmlString(L"connection", m_oConnection, *m_oConnection)
				WritingStringNullableAttrEncodeXmlString(L"command", m_oCommand, *m_oCommand)
				WritingStringNullableAttrEncodeXmlString(L"serverCommand", m_oServerCommand, *m_oServerCommand)
				WritingStringNullableAttrInt(L"commandType", m_oCommandType, *m_oCommandType)
				writer.WriteString(L"/>");
		}
		void CDbPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CDbPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ECDBPROPS*>(obj.get());
			if (ptr != nullptr)
			{
				ReadAttributes(ptr->m_BrtBeginECDbProps);
			}
		}
		EElementType CDbPr::getType() const
		{
			return et_x_dbPr;
		}
		void CDbPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginECDbProps*>(obj.get());
			if (ptr != nullptr)
			{
				if (!ptr->stConn.value().empty())
					m_oConnection = ptr->stConn.value();

				if (!ptr->stCmd.value().empty())
					m_oCommand = ptr->stCmd.value();

				if (!ptr->stCmdSvr.value().empty())
					m_oServerCommand = ptr->stCmdSvr.value();

				m_oCommandType = ptr->icmdtype;
			}
		}
		void CDbPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"connection", m_oConnection)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"command", m_oCommand)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"serverCommand", m_oServerCommand)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"commandType", m_oCommandType)
				WritingElement_ReadAttributes_End(oReader)
		}

		COlapPr::COlapPr() {}
		COlapPr::~COlapPr() {}
		void COlapPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring COlapPr::toXML() const
		{
			return L"";
		}
		void COlapPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<olapPr");
			WritingStringNullableAttrEncodeXmlString(L"localConnection", m_oLocalConnection, *m_oLocalConnection)
				WritingStringNullableAttrInt(L"rowDrillCount", m_oRowDrillCount, *m_oRowDrillCount)
				WritingStringNullableAttrBool2(L"local", m_oLocal)
				WritingStringNullableAttrBool2(L"localRefresh", m_oLocalRefresh)
				WritingStringNullableAttrBool2(L"sendLocale", m_oSendLocale)
				WritingStringNullableAttrBool2(L"serverNumberFormat", m_oServerNumberFormat)
				WritingStringNullableAttrBool2(L"serverFont", m_oServerFont)
				WritingStringNullableAttrBool2(L"serverFontColor", m_oServerFontColor)
				writer.WriteString(L"/>");
		}
		void COlapPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void COlapPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ECOLAPPROPS*>(obj.get());
			if (ptr != nullptr)
			{
				ReadAttributes(ptr->m_BrtBeginECOlapProps);
			}
		}
		EElementType COlapPr::getType() const
		{
			return et_x_olapPr;
		}
		void COlapPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginECOlapProps*>(obj.get());
			if (ptr != nullptr)
			{
				if (!ptr->stConnLocal.value().empty())
					m_oLocalConnection = ptr->stConnLocal.value();

				m_oRowDrillCount = ptr->nDrillthroughRows;
				m_oLocal = ptr->fLocalConn;
				m_oLocalRefresh = ptr->fNoRefreshCube;
				m_oSendLocale = ptr->fUseOfficeLcid;
				m_oServerNumberFormat = ptr->fSrvFmtNum;
				m_oServerFont = ptr->fSrvFmtFlags;
				m_oServerFontColor = ptr->fSrvFmtFore;
			}
		}
		void COlapPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"localConnection", m_oLocalConnection)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"rowDrillCount", m_oRowDrillCount)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"local", m_oLocal)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"localRefresh", m_oLocalRefresh)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"sendLocale", m_oSendLocale)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"serverNumberFormat", m_oServerNumberFormat)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"serverFont", m_oServerFont)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"serverFontColor", m_oServerFontColor)
				WritingElement_ReadAttributes_End(oReader)
			}

		CWebPr::CWebPr() {}
		CWebPr::~CWebPr() {}
		void CWebPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CWebPr::toXML() const
		{
			return L"";
		}
		void CWebPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<webPr");
				WritingStringNullableAttrEncodeXmlString(L"url", m_oUrl, *m_oUrl)
				WritingStringNullableAttrEncodeXmlString(L"post", m_oPost, *m_oPost)
				WritingStringNullableAttrEncodeXmlString(L"editPage", m_oEditPage, *m_oEditPage)
				WritingStringNullableAttrString(L"htmlFormat", m_oHtmlFormat, m_oHtmlFormat->ToString())
				WritingStringNullableAttrBool2(L"sourceData", m_oSourceData)
				WritingStringNullableAttrBool2(L"consecutive", m_oConsecutive)
				WritingStringNullableAttrBool2(L"firstRow", m_oFirstRow)
				WritingStringNullableAttrBool2(L"xl97", m_oXl97)
				WritingStringNullableAttrBool2(L"textDates", m_oTextDates)
				WritingStringNullableAttrBool2(L"xl2000", m_oXl2000)
				WritingStringNullableAttrBool2(L"htmlTables", m_oHtmlTables)
				WritingStringNullableAttrBool2(L"xml", m_oXml)

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
		void CWebPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				//if (L"tables" == sName)
				//	m_oTables = oReader;
			}
		}
		void CWebPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ECWEBPROPS*>(obj.get());
			if (ptr != nullptr)
			{
				ReadAttributes(ptr->m_BrtBeginECWebProps);
			}
		}
		EElementType CWebPr::getType() const
		{
			return et_x_webPr;
		}
		void CWebPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginECWebProps*>(obj.get());
			if (ptr != nullptr)
			{
				if (!ptr->stURL.value().empty())
					m_oUrl = ptr->stURL.value();

				if (!ptr->stWebPost.value().empty())
					m_oPost = ptr->stWebPost.value();

				if (!ptr->stEditWebPage.value().empty())
					m_oEditPage = ptr->stEditWebPage.value();

				m_oXml = ptr->fSrcIsXML;
				m_oSourceData = ptr->fImportSourceData;
				m_oConsecutive = ptr->fConsecDelim;
				m_oFirstRow = ptr->fSameSettings;
				m_oXl97 = ptr->fXL97Format;
				m_oTextDates = ptr->fNoDateRecog;
				m_oXl2000 = ptr->fRefreshedInXL9;
				m_oHtmlTables = ptr->fTablesOnlyHTML;
				m_oHtmlFormat = (SimpleTypes::Spreadsheet::EHtmlFormat)ptr->wHTMLFmt;
			}
		}
		void CWebPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"url", m_oUrl)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"post", m_oPost)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"editPage", m_oEditPage)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"htmlFormat", m_oHtmlFormat)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"xml", m_oXml)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"sourceData", m_oSourceData)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"consecutive", m_oConsecutive)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"firstRow", m_oFirstRow)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"xl97", m_oXl97)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"textDates", m_oTextDates)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"xl2000", m_oXl2000)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"htmlTables", m_oHtmlTables)
			WritingElement_ReadAttributes_End(oReader)
		}

		CTextPr::CTextPr() {}
		CTextPr::~CTextPr() {}
		void CTextPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTextPr::toXML() const
		{
			return L"";
		}
		void CTextPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<textPr");
			WritingStringNullableAttrBool2(L"prompt", m_oPrompt)
				WritingStringNullableAttrInt(L"codePage", m_oCodePage, *m_oCodePage)
				WritingStringNullableAttrString(L"fileType", m_oFileType, m_oFileType->ToString())
				WritingStringNullableAttrString(L"characterSet", m_oCharacterSet, *m_oCharacterSet)
				WritingStringNullableAttrInt(L"firstRow", m_oFirstRow, *m_oFirstRow)
				WritingStringNullableAttrEncodeXmlString(L"sourceFile", m_oSourceFile, *m_oSourceFile)
				WritingStringNullableAttrBool2(L"delimited", m_oDelimited)
				WritingStringNullableAttrEncodeXmlString(L"decimal", m_oDecimal, *m_oDecimal)
				WritingStringNullableAttrEncodeXmlString(L"thousands", m_oThousands, *m_oThousands)
				WritingStringNullableAttrBool2(L"tab", m_oTab)
				WritingStringNullableAttrBool2(L"space", m_oSpace)
				WritingStringNullableAttrBool2(L"comma", m_oComma)
				WritingStringNullableAttrBool2(L"semicolon", m_oSemicolon)
				WritingStringNullableAttrBool2(L"consecutive", m_oConsecutive)
				WritingStringNullableAttrString(L"qualifier", m_oQualifier, m_oQualifier->ToString())
				WritingStringNullableAttrEncodeXmlString(L"delimiter", m_oDelimiter, *m_oDelimiter)

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
		void CTextPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"textFields" == sName)
					m_oTextFields = oReader;
			}
		}
		void CTextPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ECTXTWIZ*>(obj.get());
			if (ptr != nullptr)
			{
				ReadAttributes(ptr->m_BrtBeginECTxtWiz);

				if (ptr->m_ECTWFLDINFOLST != nullptr)
					m_oTextFields = ptr->m_ECTWFLDINFOLST;
			}
		}
		EElementType CTextPr::getType() const
		{
			return et_x_textPr;
		}
		void CTextPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginECTxtWiz*>(obj.get());
			if (ptr != nullptr)
			{
				if (!ptr->stFile.value().empty())
					m_oSourceFile = ptr->stFile.value();

				m_oFileType = (SimpleTypes::Spreadsheet::EFileType)(ptr->data.iCpid < 3 && ptr->data.iCpidNew < 3 ? ptr->data.iCpid : ptr->data.iCpidNew);
				m_oDecimal = std::to_wstring(ptr->data.chDecimal);

				if (ptr->data.fCustom)
					m_oDelimiter = std::to_wstring(ptr->data.chCustom);

				m_oThousands = std::to_wstring(ptr->data.chThousSep);
				m_oFirstRow = ptr->data.rowStartAt;
				m_oQualifier = (SimpleTypes::Spreadsheet::EQualifier)ptr->data.fTextDelim;
				m_oPrompt = ptr->data.fPromptForFile;
				m_oDelimited = ptr->data.fDelimited;
				m_oTab = ptr->data.fTab;
				m_oSpace = ptr->data.fSpace;
				m_oComma = ptr->data.fComma;
				m_oSemicolon = ptr->data.fSemiColon;
				m_oConsecutive = ptr->data.fConsecutive;
			}
		}
		void CTextPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"characterSet", m_oCharacterSet)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"decimal", m_oDecimal)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"delimiter", m_oDelimiter)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"thousands", m_oThousands)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"sourceFile", m_oSourceFile)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"firstRow", m_oFirstRow)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"qualifier", m_oQualifier)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"fileType", m_oFileType)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"prompt", m_oPrompt)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"delimited", m_oDelimited)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"tab", m_oTab)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"space", m_oSpace)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"comma", m_oComma)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"semicolon", m_oSemicolon)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"consecutive", m_oConsecutive)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"codePage", m_oCodePage)
			WritingElement_ReadAttributes_End(oReader)
		}

		CConnection::CConnection() {}
		CConnection::~CConnection()
		{
		}
		void CConnection::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CConnection::toXML() const
		{
			return L"";
		}
		void CConnection::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (false == m_oName.IsInit()) return;

			writer.WriteString(L"<connection");
			WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue())
				WritingStringNullableAttrString(L"xr16:uid", m_oUId, *m_oUId)
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
			if (m_oRangePr.IsInit())
			{
				writer.WriteString(L"<extLst>");
				writer.WriteString(L"<ext uri=\"{DE250136-89BD-433C-8126-D09CA5730AF9}\" xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\">");
				writer.WriteString(L"<x15:connection");
				WritingStringNullableAttrString(L"id", m_oIdExt, *m_oIdExt)
					writer.WriteString(L">");
				m_oRangePr->toXML(writer);
				writer.WriteString(L"</x15:connection>");
				writer.WriteString(L"</ext>");
				writer.WriteString(L"</extLst>");
			}
			writer.WriteString(L"</connection>");
		}
		void CConnection::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"dbPr" == sName)
					m_oDbPr = oReader;
				else if (L"olapPr" == sName)
					m_oOlapPr = oReader;
				else if (L"textPr" == sName)
					m_oTextPr = oReader;
				else if (L"webPr" == sName)
					m_oWebPr = oReader;
				else if (L"rangePr" == sName) //x15
					m_oRangePr = oReader;
				else if (L"extLst" == sName)
					m_oExtLst = oReader;
			}
		}
		void CConnection::fromBin(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeEXTCONNECTION)
			{
				auto ptr = static_cast<XLSB::EXTCONNECTION*>(obj.get());
				if (ptr != nullptr)
				{
					ReadAttributes(ptr->m_BrtBeginExtConnection);

					if (ptr->m_ECDBPROPS != nullptr)
						m_oDbPr = ptr->m_ECDBPROPS;

					if (ptr->m_ECOLAPPROPS != nullptr)
						m_oOlapPr = ptr->m_ECOLAPPROPS;

					if (ptr->m_ECTXTWIZ != nullptr)
						m_oTextPr = ptr->m_ECTXTWIZ;

					if (ptr->m_ECWEBPROPS != nullptr)
						m_oWebPr = ptr->m_ECWEBPROPS;

					//if(ptr->m_ECPARAMS != nullptr)
					// = ptr->m_ECPARAMS;
					if (ptr->m_FRTEXTCONNECTIONS != nullptr)
						m_oExtLst = ptr->m_FRTEXTCONNECTIONS;
				}
			}
			else if (obj->get_type() == XLS::typeEXTCONN15)
			{
				auto ptr = static_cast<XLSB::EXTCONN15*>(obj.get());
				if (ptr != nullptr)
				{
					//ReadAttributes(ptr->m_BrtBeginExtConn15);

					if (ptr->m_source != nullptr)
					{
						if (ptr->m_source->get_type() == XLS::typeRangePr15)
							m_oRangePr = ptr->m_source;
					}
				}
			}
		}
		EElementType CConnection::getType() const
		{
			return et_x_Connection;
		}
		void CConnection::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeBeginExtConnection)
			{
				auto ptr = static_cast<XLSB::BeginExtConnection*>(obj.get());
				if (ptr != nullptr)
				{
					m_oType = ptr->idbtype;

					if (!ptr->stConnName.value().empty())
						m_oName = ptr->stConnName.value();

					m_oId = ptr->dwConnID;

					m_oCredentials = (SimpleTypes::Spreadsheet::ECredMethod)ptr->iCredMethod;

					m_oBackground = ptr->fBackgroundQuery;
					m_oDeleted = ptr->fDeleted;

					if (!ptr->stConnDesc.value().empty())
						m_oDescription = ptr->stConnDesc.value();

					m_oInterval = ptr->wInterval;
					m_oKeepAlive = ptr->fMaintain;
					m_oMinRefreshableVersion = ptr->bVerRefreshableMin;
					m_oNew = ptr->fNewQuery;

					if (!ptr->stConnectionFile.value().empty())
						m_oOdcFile = ptr->stConnectionFile.value();

					m_oOnlyUseConnectionFile = ptr->fAlwaysUseConnectionFile;
					m_oReconnectionMethod = ptr->irecontype;
					m_oRefreshedVersion = ptr->bVerRefreshed;
					m_oRefreshOnLoad = ptr->fRefreshOnLoad;
					m_oSaveData = ptr->fSaveData;
					m_oSavePassword = ptr->pc == 0x01;

					if (!ptr->stSso.value().empty())
						m_oSingleSignOnId = ptr->stSso.value();

					if (!ptr->stDataFile.value().empty())
						m_oSourceFile = ptr->stDataFile.value();
				}
			}
		}
		void CConnection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"type", m_oType)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"name", m_oName)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"id", m_oIdExt)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"xr16:uid", m_oUId)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"background", m_oBackground)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"credentials", m_oCredentials)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"deleted", m_oDeleted)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"description", m_oDescription)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"interval", m_oInterval)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"keepAlive", m_oKeepAlive)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"minRefreshableVersion", m_oMinRefreshableVersion)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"new", m_oNew)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"odcFile", m_oOdcFile)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"onlyUseConnectionFile", m_oOnlyUseConnectionFile)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"reconnectionMethod", m_oReconnectionMethod)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"refreshedVersion", m_oRefreshedVersion)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"refreshOnLoad", m_oRefreshOnLoad)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"saveData", m_oSaveData)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"savePassword", m_oSavePassword)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"singleSignOnId", m_oSingleSignOnId)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"sourceFile", m_oSourceFile)
				WritingElement_ReadAttributes_End(oReader)

					if (m_oIdExt.IsInit())
					{
						m_oId = *m_oIdExt;
						if (m_oId.IsInit() && m_oId->GetValue() > 0)
						{
							m_oIdExt.reset();
						}
					}
			}

		CConnections::CConnections()
			{
			}
		CConnections::~CConnections()
			{
			}
		void CConnections::fromXML(XmlUtils::CXmlNode& node)
			{
			}
		std::wstring CConnections::toXML() const
			{
				return L"";
			}
		void CConnections::toXML(NSStringUtils::CStringBuilder& writer) const
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
		void CConnections::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (L"connection" == sName)
					{
						CConnection *pConn = new CConnection(oReader);
						m_arrItems.push_back(pConn);
					}
				}
			}
		void CConnections::fromBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTCONNECTIONS*>(obj.get());
				if (ptr != nullptr)
				{
					m_oCount = ptr->m_arEXTCONNECTION.size();

					for (auto &connection : ptr->m_arEXTCONNECTION)
						m_arrItems.push_back(new CConnection(connection));
				}
			}
		EElementType CConnections::getType() const
			{
				return et_x_Connections;
			}
		void CConnections::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"count", m_oCount)
				WritingElement_ReadAttributes_End(oReader)
			}

		CConnectionsFile::CConnectionsFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
		}
		CConnectionsFile::CConnectionsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
			read(oRootPath, oPath);
		}
		CConnectionsFile::~CConnectionsFile()
		{
		}
		void CConnectionsFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CConnectionsFile::readBin(const CPath& oPath)
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if (xlsb)
			{
				XLSB::ConnectionsStreamPtr connectionsStream(new XLSB::ConnectionsStream);

				xlsb->ReadBin(oPath, connectionsStream.get());

				if (connectionsStream != nullptr)
				{
					if (connectionsStream->m_EXTCONNECTIONS != nullptr)
						m_oConnections = connectionsStream->m_EXTCONNECTIONS;
				}

			}
		}
		void CConnectionsFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read(oRootPath, oPath);

			if (m_oReadPath.GetExtention() == _T(".bin"))
			{
				readBin(m_oReadPath);
				return;
			}

			XmlUtils::CXmlLiteReader oReader;

			if (!oReader.FromFile(oPath.GetPath()))
				return;

			if (!oReader.ReadNextNode())
				return;

			m_oConnections = oReader;
		}
		void CConnectionsFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			if (false == m_oConnections.IsInit()) return;

			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oConnections->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
			IFileContainer::Write(oPath, oDirectory, oContent);
		}
		const OOX::FileType CConnectionsFile::type() const
		{
			return OOX::Spreadsheet::FileTypes::Connections;
		}
		const CPath CConnectionsFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CConnectionsFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CConnectionsFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CConnectionsFile::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
