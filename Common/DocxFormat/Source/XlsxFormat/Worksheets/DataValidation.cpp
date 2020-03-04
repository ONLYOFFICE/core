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
#include "DataValidation.h"

namespace OOX
{
namespace Spreadsheet
{
void CDataValidationFormula::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	std::wstring node_name = bExtendedWrite ? L"xm:f" : L"formula";
	
	writer.WriteString(L"<" + node_name + L">");
	writer.WriteEncodeXmlString(m_sText);
	writer.WriteString(L"</" + node_name + L">");
}
void CDataValidationFormula::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	m_sNodeName = oReader.GetName();

	m_sText = oReader.GetText3();
}

void CDataValidation::toXML(NSStringUtils::CStringBuilder& writer) const
{
	toXML2(writer, false);
}
void CDataValidation::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	std::wstring node_name = bExtendedWrite ? L"x14:dataValidation" : L"dataValidation";

	writer.WriteString(L"<" + node_name);
		if (bExtendedWrite)
		{
			if (false == m_oUuid.IsInit())
			{
				m_oUuid = L"{" + XmlUtils::GenerateGuid() + L"}";
			}
			WritingStringNullableAttrString	(L"xr:uid",	m_oUuid, m_oUuid.get());
		}
		else
		{
			WritingStringNullableAttrString(L"sqref", m_oSqRef, m_oSqRef.get());
		}
		WritingStringNullableAttrString				(L"type",			m_oType,			m_oType->ToString());
		WritingStringNullableAttrInt				(L"allowBlank",		m_oAllowBlank,		m_oAllowBlank->GetValue());
		WritingStringNullableAttrEncodeXmlStringHHHH(L"error",			m_oError,			m_oError.get());
		WritingStringNullableAttrString				(L"errorStyle",		m_oErrorStyle,		m_oErrorStyle->ToString());
		WritingStringNullableAttrEncodeXmlStringHHHH(L"errorTitle",		m_oErrorTitle,		m_oErrorTitle.get());
		WritingStringNullableAttrString				(L"imeMode",		m_oImeMode,			m_oImeMode->ToString());
		WritingStringNullableAttrString				(L"operator",		m_oOperator,		m_oOperator->ToString());
		WritingStringNullableAttrEncodeXmlStringHHHH(L"prompt",			m_oPrompt,			m_oPrompt.get());
		WritingStringNullableAttrEncodeXmlStringHHHH(L"promptTitle",	m_oPromptTitle,		m_oPromptTitle.get());
		WritingStringNullableAttrInt				(L"showDropDown",	m_oShowDropDown,	m_oShowDropDown->GetValue());
		WritingStringNullableAttrInt				(L"showErrorMessage",m_oShowErrorMessage,m_oShowErrorMessage->GetValue());
		WritingStringNullableAttrInt				(L"showInputMessage",m_oShowInputMessage,m_oShowInputMessage->GetValue());
	writer.WriteString(L">");

	if (bExtendedWrite)
	{
		if (m_oFormula1.IsInit())
		{
			writer.WriteString(L"<x14:formula1>");
				m_oFormula1->toXML2(writer, true);
			writer.WriteString(L"</x14:formula1>");
		}
		if (m_oFormula2.IsInit())
		{
			writer.WriteString(L"<x14:formula2>");
				m_oFormula2->toXML2(writer, true);
			writer.WriteString(L"</x14:formula2>");
		}
		if (m_oSqRef.IsInit())
		{
			writer.WriteString(L"<xm:sqref>" + m_oSqRef.get() + L"</xm:sqref>");
		}
	}
	else
	{
		if (m_oFormula1.IsInit())
		{
			writer.WriteString(L"<formula1>");
				writer.WriteString(m_oFormula1->m_sText);
			writer.WriteString(L"</formula1>");
		}
		if (m_oFormula2.IsInit())
		{
			writer.WriteString(L"<formula2>");
				writer.WriteString(m_oFormula2->m_sText);
			writer.WriteString(L"</formula2>");
		}
	}
	writer.WriteString(L"</" + node_name + L">");
}

void CDataValidation::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes( oReader );

	if ( oReader.IsEmptyNode() )
		return;

	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		if (L"formula1" == sName)
		{
			m_oFormula1 = oReader;
		}
		else if (L"formula2" == sName)
		{
			m_oFormula2 = oReader;
		}
		else if (L"sqref" == sName)
		{
			m_oSqRef = oReader.GetText2();
		}
	}
}
bool CDataValidation::IsExtended()
{
	bool result = true;
	if (m_oFormula1.IsInit())
	{
		if (m_oFormula1->m_sText.find(L"!") != std::wstring::npos && m_oFormula1->m_sText != L"#REF!")
			result = false;
	}
	if (m_oFormula2.IsInit())
	{
		if (m_oFormula2->m_sText.find(L"!") != std::wstring::npos && m_oFormula2->m_sText != L"#REF!")
			result = false;
	}
	return result;
}
void CDataValidation::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{

	WritingElement_ReadAttributes_StartChar( oReader )
		WritingElement_ReadAttributes_Read_ifChar		( oReader, "allowBlank",	m_oAllowBlank)
		else if ( strcmp("error", wsName) == 0 )
		{
			m_oError = oReader.GetAttributeTextWithHHHH();
		}
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "errorStyle",	m_oErrorStyle)
		else if ( strcmp("errorTitle", wsName) == 0 )
		{
			m_oErrorTitle = oReader.GetAttributeTextWithHHHH();
		}
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "imeMode",		m_oImeMode)
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "operator",		m_oOperator)
		else if ( strcmp("prompt", wsName) == 0 )
		{
			m_oPrompt = oReader.GetAttributeTextWithHHHH();
		}
		else if ( strcmp("promptTitle", wsName) == 0 )
		{
			m_oPromptTitle = oReader.GetAttributeTextWithHHHH();
		}
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "showDropDown",	m_oShowDropDown)
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "showErrorMessage",m_oShowErrorMessage)
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "showInputMessage",m_oShowInputMessage)
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "sqref",			m_oSqRef)
		WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "type",			m_oType)
	WritingElement_ReadAttributes_EndChar( oReader )
}
void CDataValidations::toXML(NSStringUtils::CStringBuilder& writer) const
{
	if (m_arrItems.empty()) return;

	toXML2(writer, false);
}
void CDataValidations::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	if (m_arrItems.empty()) return;
	
	if (false == m_oCount.IsInit())
	{
		m_oCount = (int)m_arrItems.size();
	}
	std::wstring node_name = bExtendedWrite ? L"x14:dataValidations" : L"dataValidations";

	writer.WriteString(L"<" + node_name);
		if (bExtendedWrite)
		{
			WritingStringAttrString(L"xmlns:xm", L"http://schemas.microsoft.com/office/excel/2006/main");
		}
		WritingStringNullableAttrInt(L"count",			m_oCount,			*m_oCount);
		WritingStringNullableAttrInt(L"disablePrompts", m_oDisablePrompts,	m_oDisablePrompts->GetValue());
		WritingStringNullableAttrInt(L"xWindow",		m_oXWindow,			m_oXWindow->GetValue());
		WritingStringNullableAttrInt(L"yWindow",		m_oYWindow,			m_oYWindow->GetValue());
	writer.WriteString(L">");
	for ( size_t i = 0; i < m_arrItems.size(); ++i)
	{
		if ( m_arrItems[i] )
		{
			m_arrItems[i]->toXML2(writer, bExtendedWrite);
		}
	}
	writer.WriteString(L"</" + node_name + L">");
}
void CDataValidations::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes( oReader );

	if ( oReader.IsEmptyNode() )
		return;

	int nCurDepth = oReader.GetDepth();
	while( oReader.ReadNextSiblingNode( nCurDepth ) )
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

		if ( L"dataValidation" == sName )
		{
			m_arrItems.push_back( new CDataValidation( oReader ));
		}
	}
}

void CDataValidations::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if		( oReader, L"count",	m_oCount)
		WritingElement_ReadAttributes_Read_else_if	( oReader, L"disablePrompts",m_oDisablePrompts)
		WritingElement_ReadAttributes_Read_else_if	( oReader, L"xWindow",	m_oXWindow)
		WritingElement_ReadAttributes_Read_else_if	( oReader, L"yWindow",	m_oYWindow)
	WritingElement_ReadAttributes_End( oReader )
}

} //Spreadsheet
} // namespace OOX
