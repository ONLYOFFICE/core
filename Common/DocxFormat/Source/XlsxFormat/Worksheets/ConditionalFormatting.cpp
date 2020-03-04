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
#include "ConditionalFormatting.h"

namespace OOX
{
namespace Spreadsheet
{
void CFormulaCF::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	std::wstring node_name = bExtendedWrite ? L"xm:f" : L"formula";
	
	writer.WriteString(L"<" + node_name + L">");
	writer.WriteEncodeXmlString(m_sText);
	writer.WriteString(L"</" + node_name + L">");
}
void CFormulaCF::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	m_sNodeName = oReader.GetName();

	if (oReader.IsEmptyNode())
		return;

	m_sText = oReader.GetText3();
}
const CFormulaCF CFormulaCF::Merge(const CFormulaCF& oPrev, const CFormulaCF& oCurrent)
{
	CFormulaCF oFormula;
	
	if (oCurrent.m_sText.empty() == false)
	{
		oFormula.m_sText = oCurrent.m_sText;
		oFormula.m_sNodeName = oCurrent.m_sNodeName;
	}
	else
	{
		oFormula.m_sText = oPrev.m_sText;
		oFormula.m_sNodeName = oPrev.m_sNodeName;
	}

	return oFormula;
}
bool CFormulaCF::isExtended ()
{
	return (m_sNodeName == L"xm:f");
}

void CConditionalFormatValueObject::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	if (false == m_oType.IsInit()) return;
	
	if (bExtendedWrite == false)
	{
		if (m_oType->GetValue() == SimpleTypes::Spreadsheet::autoMin) m_oType->SetValue(SimpleTypes::Spreadsheet::Minimum);
		if (m_oType->GetValue() == SimpleTypes::Spreadsheet::autoMax) m_oType->SetValue(SimpleTypes::Spreadsheet::Maximum); 
	}

	std::wstring node_name = bExtendedWrite ? L"x14:cfvo" : L"cfvo";

	writer.WriteString(L"<" + node_name);
		WritingStringAttrString(L"type", m_oType->ToString());
		if (m_oGte.IsInit() && false == m_oGte->ToBool())
		{
			writer.WriteString(L" gte=\"0\"");
		}
		if (!bExtendedWrite)
		{
			if (m_oVal.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"val", m_oVal.get());
			}
			else if (m_oFormula.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"val", m_oFormula->m_sText);
			}

		}
	writer.WriteString(L">");

	if (bExtendedWrite)
	{
		if (true == m_oFormula.IsInit())
		{
			m_oFormula->toXML2(writer, true);
		}
		else if (m_oVal.IsInit())
		{
			CFormulaCF formla; formla.m_sText = m_oVal.get();
			formla.toXML2(writer, true);
		}
				
	}

	writer.WriteString(L"</" + node_name + L">");
}
void CConditionalFormatValueObject::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);
	
	if (oReader.IsEmptyNode())
		return;

	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		if (L"formula" == sName || L"f" == sName)
			m_oFormula = oReader;
	}
}

void CConditionalFormatIconSet::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	if (!bExtendedWrite) return;

	writer.WriteString(L"<x14:cfIcon");
		WritingStringNullableAttrString(L"iconSet", m_oIconSet, m_oIconSet->ToString())
		WritingStringNullableAttrInt(L"iconId", m_oIconId, m_oIconId->GetValue())
	writer.WriteString(L"/>");
}
void CConditionalFormatIconSet::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);

	if (oReader.IsEmptyNode())
		return;
}
void CConditionalFormatIconSet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_Start(oReader)

	WritingElement_ReadAttributes_Read_if		(oReader, L"iconSet", m_oIconSet)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"iconId"	, m_oIconId)

	WritingElement_ReadAttributes_End(oReader)
}

void CColorScale::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	if (1 < m_arrValues.size() && 1 < m_arrColors.size()) // min 2 + 2
	{
        std::wstring sValue;
		writer.WriteString(L"<colorScale>");

        for ( size_t i = 0; i < m_arrValues.size(); ++i)//todooo - проверить можно ли не чередовать,а как есть записать
        {
			if ( m_arrValues[i].IsInit() )
            {
                m_arrValues[i]->toXML2(writer, bExtendedWrite);
            }
			//if ( (i < m_arrColors.size()) && (m_arrColors[i].IsInit()) )
			//{
			//	m_arrColors[i]->toXML(writer);
			//}
		}
        for ( size_t i = 0/*m_arrValues.size()*/; i < m_arrColors.size(); ++i)
        {
			if ( m_arrColors[i].IsInit() )
            {
                m_arrColors[i]->toXML(writer);
            }
		}
		writer.WriteString(L"</colorScale>");
	}
}
void CColorScale::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	if (oReader.IsEmptyNode())
		return;

	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		if (L"cfvo" == sName)
		{
			nullable<CConditionalFormatValueObject> item(oReader);
			m_arrValues.push_back(item);
		}
		else if (L"color" == sName)
		{
			nullable<CColor> item(oReader);
			m_arrColors.push_back(item);
		}
	}
}
template<typename Type>
nullable<Type> CColorScale::Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
{
	nullable<Type> oResult;

	if ( oCurrent.IsInit() )	oResult = oCurrent;
	else if ( oPrev.IsInit() )	oResult = oPrev;

	return oResult;
}
const CColorScale CColorScale::Merge(const CColorScale& oPrev, const CColorScale& oCurrent)
{
	CColorScale oColorScale;
	
	for (size_t i = 0; i < oPrev.m_arrValues.size(); i++)
	{
		if (i < oCurrent.m_arrValues.size())
			oColorScale.m_arrValues.push_back(Merge( oPrev.m_arrValues[i],	oCurrent.m_arrValues[i] ));
		else
			oColorScale.m_arrValues.push_back(oPrev.m_arrValues[i]);
	}
	for (size_t i = oPrev.m_arrValues.size(); i < oCurrent.m_arrValues.size(); i++)
	{
		oColorScale.m_arrValues.push_back(oCurrent.m_arrValues[i]);
	}
	for (size_t i = 0; i < oPrev.m_arrColors.size(); i++)
	{
		if (i < oCurrent.m_arrColors.size())
			oColorScale.m_arrColors.push_back(Merge( oPrev.m_arrColors[i],	oCurrent.m_arrColors[i] ));
		else
			oColorScale.m_arrColors.push_back(oPrev.m_arrColors[i]);
	}
	for (size_t i = oPrev.m_arrColors.size(); i < oCurrent.m_arrColors.size(); i++)
	{
		oColorScale.m_arrColors.push_back(oCurrent.m_arrColors[i]);
	}
	return oColorScale;
}
bool CDataBar::isExtended ()
{
	if (m_oAxisColor.IsInit() || m_oAxisPosition.IsInit() || m_oDirection.IsInit() || m_oBorderColor.IsInit()
			|| m_oNegativeFillColor.IsInit() || m_oNegativeBorderColor.IsInit() 
			|| m_oNegativeBarColorSameAsPositive.IsInit() || m_oNegativeBarBorderColorSameAsPositive.IsInit())
	{
		return true;
	}
	return false;
}

void CDataBar::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	if (2 != m_arrValues.size() || false == m_oColor.IsInit()) return;

	std::wstring node_name = bExtendedWrite ? L"x14:dataBar" : L"dataBar";

	writer.WriteString(L"<" + node_name);
		WritingStringNullableAttrInt(L"maxLength", m_oMaxLength, m_oMaxLength->GetValue());
		WritingStringNullableAttrInt(L"minLength", m_oMinLength, m_oMinLength->GetValue());
		if (m_oShowValue.IsInit() && false == m_oShowValue->ToBool())
		{
			writer.WriteString(L" showValue=\"0\"");
		}
		if (bExtendedWrite)
		{
			if (m_oBorderColor.IsInit()) writer.WriteString(L" border=\"1\"");
			WritingStringNullableAttrString(L"axisPosition", m_oAxisPosition, m_oAxisPosition->ToString())
			WritingStringNullableAttrString(L"direction", m_oDirection, m_oDirection->ToString())

			if (m_oGradient.IsInit() && false == m_oGradient->ToBool())
			{
				writer.WriteString(L" gradient=\"0\"");
			}
			if (m_oNegativeBarColorSameAsPositive.IsInit() && true == m_oNegativeBarColorSameAsPositive->ToBool())
			{
				writer.WriteString(L" negativeBarColorSameAsPositive=\"1\"");
			}
			if (m_oNegativeBarBorderColorSameAsPositive.IsInit() && false == m_oNegativeBarBorderColorSameAsPositive->ToBool())
			{
				writer.WriteString(L" negativeBarBorderColorSameAsPositive=\"0\"");
			}
		}
	writer.WriteString(L">");

    for ( size_t i = 0; i < m_arrValues.size(); ++i)
    {
        if (  m_arrValues[i].IsInit() )
        {
            m_arrValues[i]->toXML2(writer, bExtendedWrite);
        }
    }

	m_oColor->toXML2(writer, bExtendedWrite ? L"x14:fillColor" : L"color");

	if (bExtendedWrite)
	{
		if (m_oBorderColor.IsInit())		m_oBorderColor->toXML2(writer, L"x14:borderColor");
		if (m_oNegativeFillColor.IsInit())	m_oNegativeFillColor->toXML2(writer, L"x14:negativeFillColor");
		if (m_oNegativeBorderColor.IsInit())m_oNegativeBorderColor->toXML2(writer, L"x14:negativeBorderColor");
		if (m_oAxisColor.IsInit())			m_oAxisColor->toXML2(writer, L"x14:axisColor");
	}

	writer.WriteString(L"</" + node_name + L">");
}
void CDataBar::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);

	if (oReader.IsEmptyNode())
		return;

	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		if (L"cfvo" == sName)
		{
			nullable<CConditionalFormatValueObject> item = oReader;
			m_arrValues.push_back(item);
		}
		else if ( L"color" == sName || L"fillColor" == sName )
			m_oColor = oReader;
		else if ( L"axisColor" == sName)
			m_oAxisColor = oReader;
		else if ( L"borderColor" == sName)
			m_oBorderColor = oReader;
		else if ( L"negativeFillColor" == sName )
			m_oNegativeFillColor = oReader;
		else if ( L"negativeBorderColor" == sName )
			m_oNegativeBorderColor = oReader;
	}
}
template<typename Type>
nullable<Type> CDataBar::Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
{
	nullable<Type> oResult;

	if ( oCurrent.IsInit() )	oResult = oCurrent;
	else if ( oPrev.IsInit() )	oResult = oPrev;

	return oResult;
}
const CDataBar CDataBar::Merge(const CDataBar& oPrev, const CDataBar& oCurrent)
{
	CDataBar oDataBar;
	oDataBar.m_oMaxLength		= Merge( oPrev.m_oMaxLength,            oCurrent.m_oMaxLength );
	oDataBar.m_oMinLength		= Merge( oPrev.m_oMinLength,            oCurrent.m_oMinLength );
	oDataBar.m_oShowValue		= Merge( oPrev.m_oShowValue,            oCurrent.m_oShowValue );
	oDataBar.m_oAxisPosition	= Merge( oPrev.m_oAxisPosition,			oCurrent.m_oAxisPosition );
	oDataBar.m_oBorder			= Merge( oPrev.m_oBorder,				oCurrent.m_oBorder );
	oDataBar.m_oGradient		= Merge( oPrev.m_oGradient,				oCurrent.m_oGradient );
	oDataBar.m_oDirection		= Merge( oPrev.m_oDirection,			oCurrent.m_oDirection );
	oDataBar.m_oNegativeBarColorSameAsPositive = Merge( oPrev.m_oNegativeBarColorSameAsPositive, oCurrent.m_oNegativeBarColorSameAsPositive );
	oDataBar.m_oNegativeBarBorderColorSameAsPositive = Merge( oPrev.m_oNegativeBarBorderColorSameAsPositive, oCurrent.m_oNegativeBarBorderColorSameAsPositive );
	
	oDataBar.m_oColor				= Merge( oPrev.m_oColor,				oCurrent.m_oColor );
	oDataBar.m_oAxisColor			= Merge( oPrev.m_oAxisColor,			oCurrent.m_oAxisColor );
	oDataBar.m_oBorderColor			= Merge( oPrev.m_oBorderColor,			oCurrent.m_oBorderColor );
	oDataBar.m_oNegativeFillColor	= Merge( oPrev.m_oNegativeFillColor,	oCurrent.m_oNegativeFillColor );
	oDataBar.m_oNegativeBorderColor	= Merge( oPrev.m_oNegativeBorderColor,	oCurrent.m_oNegativeBorderColor );
	
	for (size_t i = 0; i < oPrev.m_arrValues.size(); i++)
	{
		if (i < oCurrent.m_arrValues.size())
			oDataBar.m_arrValues.push_back(Merge( oPrev.m_arrValues[i],	oCurrent.m_arrValues[i] ));
		else
			oDataBar.m_arrValues.push_back(oPrev.m_arrValues[i]);
	}
	for (size_t i = oPrev.m_arrValues.size(); i < oCurrent.m_arrValues.size(); i++)
	{
		oDataBar.m_arrValues.push_back(oCurrent.m_arrValues[i]);
	}
	return oDataBar;
}
void CDataBar::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_Read_if		(oReader, L"maxLength"	, m_oMaxLength)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"minLength"	, m_oMinLength)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"showValue"	, m_oShowValue)

		WritingElement_ReadAttributes_Read_else_if	(oReader, L"axisPosition", m_oAxisPosition)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"border"		, m_oBorder)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"gradient"	, m_oGradient)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"direction"	, m_oDirection)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"negativeBarColorSameAsPositive", m_oNegativeBarColorSameAsPositive)
		WritingElement_ReadAttributes_Read_else_if	(oReader, L"negativeBarBorderColorSameAsPositive", m_oNegativeBarBorderColorSameAsPositive)
	WritingElement_ReadAttributes_End(oReader)
}

void CIconSet::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	if (m_arrValues.size() < 2) return;	// min value = 2

	std::wstring node_name = bExtendedWrite ? L"x14:iconSet" : L"iconSet";

	std::wstring sValue;
	writer.WriteString(L"<" + node_name);
	WritingStringNullableAttrString(L"iconSet", m_oIconSet, m_oIconSet->ToString())
	if (m_oPercent.IsInit() && false == m_oPercent->ToBool())
	{
		writer.WriteString(L" percent=\"0\"");
	}
	if (m_oReverse.IsInit() && true == m_oReverse->ToBool())
	{
		writer.WriteString(L" reverse=\"1\"");
	}
	if (m_oShowValue.IsInit() && false == m_oShowValue->ToBool())
	{
		writer.WriteString(L" showValue=\"0\"");
	}
	if (bExtendedWrite && false == m_arrIconSets.empty())
	{
		writer.WriteString(L" custom=\"1\"");
	}
	writer.WriteString(L">");

    for ( size_t i = 0; i < m_arrValues.size(); ++i)
    {
		if ( m_arrValues[i].IsInit() )
        {
            m_arrValues[i]->toXML2(writer, bExtendedWrite);
        }
	}
    for ( size_t i = 0; bExtendedWrite && i < m_arrIconSets.size(); ++i)
    {
		if ( m_arrIconSets[i].IsInit() )
        {
            m_arrIconSets[i]->toXML2(writer, bExtendedWrite);
        }
	}
	writer.WriteString(L"</" + node_name + L">");
}
void CIconSet::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);

	if (oReader.IsEmptyNode())
		return;

	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		if (L"cfvo" == sName)
		{
			nullable<CConditionalFormatValueObject> item(oReader);
			m_arrValues.push_back(item);
		}
		else if (L"cfIcon" == sName)
		{
			nullable<CConditionalFormatIconSet> item(oReader);
			m_arrIconSets.push_back(item);
		}
	}
}
template<typename Type>
nullable<Type> CIconSet::Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
{
	nullable<Type> oResult;

	if ( oCurrent.IsInit() )	oResult = oCurrent;
	else if ( oPrev.IsInit() )	oResult = oPrev;

	return oResult;
}
const CIconSet CIconSet::Merge(const CIconSet& oPrev, const CIconSet& oCurrent)
{
	CIconSet oIconSet;
	oIconSet.m_oIconSet		= Merge( oPrev.m_oIconSet,		oCurrent.m_oIconSet );
	oIconSet.m_oPercent		= Merge( oPrev.m_oPercent,		oCurrent.m_oPercent );
	oIconSet.m_oReverse		= Merge( oPrev.m_oReverse,		oCurrent.m_oReverse );
	oIconSet.m_oShowValue	= Merge( oPrev.m_oShowValue,	oCurrent.m_oShowValue );
	oIconSet.m_oCustom		= Merge( oPrev.m_oCustom,		oCurrent.m_oCustom );
	
	for (size_t i = 0; i < oPrev.m_arrValues.size(); i++)
	{
		if (i < oCurrent.m_arrValues.size())
			oIconSet.m_arrValues.push_back(Merge( oPrev.m_arrValues[i],	oCurrent.m_arrValues[i] ));
		else
			oIconSet.m_arrValues.push_back(oPrev.m_arrValues[i]);
	}
	for (size_t i = oPrev.m_arrValues.size(); i < oCurrent.m_arrValues.size(); i++)
	{
		oIconSet.m_arrValues.push_back(oCurrent.m_arrValues[i]);
	}
	for (size_t i = 0; i < oPrev.m_arrIconSets.size(); i++)
	{
		if (i < oCurrent.m_arrIconSets.size())
			oIconSet.m_arrIconSets.push_back(Merge( oPrev.m_arrIconSets[i],	oCurrent.m_arrIconSets[i] ));
		else
			oIconSet.m_arrIconSets.push_back(oPrev.m_arrIconSets[i]);
	}
	for (size_t i = oPrev.m_arrIconSets.size(); i < oCurrent.m_arrIconSets.size(); i++)
	{
		oIconSet.m_arrIconSets.push_back(oCurrent.m_arrIconSets[i]);
	}
	return oIconSet;
}
bool CIconSet::isExtended ()
{
	return (false == m_arrIconSets.empty() || (m_oIconSet.IsInit() && m_oIconSet->GetValue() > 15));
}
void CIconSet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_Start(oReader)

	WritingElement_ReadAttributes_Read_if		(oReader, L"iconSet"	, m_oIconSet)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"percent"	, m_oPercent)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"reverse"	, m_oReverse)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"showValue"	, m_oShowValue)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"custom"		, m_oCustom)

	WritingElement_ReadAttributes_End(oReader)
}

bool CConditionalFormattingRule::isExtended()
{
	if (m_oDxf.IsInit())		return true;

	if (m_oDataBar.IsInit())	return m_oDataBar->isExtended();
	if (m_oIconSet.IsInit())	return m_oIconSet->isExtended();
	if (m_oColorScale.IsInit())	return m_oColorScale->isExtended();
	
	for (size_t i = 0; i < m_arrFormula.size(); i++)
	{
		if ((m_arrFormula[i].IsInit()) && m_arrFormula[i]->isExtended())
			return true;
	}
	return false;
}
void CConditionalFormattingRule::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{
	if (false == isValid()) return;

	std::wstring node_name = bExtendedWrite ? L"x14:cfRule" : L"cfRule";
	
	writer.WriteString(L"<" + node_name);
		WritingStringAttrString(L"type", m_oType->ToString());
		WritingStringAttrInt(L"priority", m_oPriority->GetValue());
		if (m_oAboveAverage.IsInit() && false == m_oAboveAverage->ToBool())
			writer.WriteString(L" aboveAverage=\"0\"");
		if (m_oBottom.IsInit() && true == m_oBottom->ToBool())
			writer.WriteString(L" bottom=\"1\"");
		WritingStringNullableAttrInt(L"dxfId", m_oDxfId, m_oDxfId->GetValue());
		if (m_oEqualAverage.IsInit() && true == m_oEqualAverage->ToBool())
			writer.WriteString(L" equalAverage=\"1\"");
		WritingStringNullableAttrString(L"operator", m_oOperator, m_oOperator->ToString());
		if (m_oPercent.IsInit() && true == m_oPercent->ToBool())
			writer.WriteString(L" percent=\"1\"");
		WritingStringNullableAttrInt(L"rank", m_oRank, m_oRank->GetValue());
		WritingStringNullableAttrInt(L"stdDev", m_oStdDev, m_oStdDev->GetValue());
		if (m_oStopIfTrue.IsInit() && true == m_oStopIfTrue->ToBool())
			writer.WriteString(L" stopIfTrue=\"1\"");
		WritingStringNullableAttrEncodeXmlString(L"text", m_oText, m_oText.get());
		WritingStringNullableAttrString(L"timePeriod", m_oTimePeriod, m_oTimePeriod.get());

		if (bExtendedWrite)
		{
			if (false == m_oId.IsInit())
			{
				WritingStringAttrString(L"id", L"{" + XmlUtils::GenerateGuid() + L"}");
			}
			else
				WritingStringNullableAttrString(L"id", m_oId, m_oId.get());
		}
	writer.WriteString(L">");

	if (m_oIconSet.IsInit())
		m_oIconSet->toXML2(writer, bExtendedWrite);
	if (m_oColorScale.IsInit())
		m_oColorScale->toXML2(writer, bExtendedWrite);
	if (m_oDataBar.IsInit())
		m_oDataBar->toXML2(writer, bExtendedWrite);
	
	for (size_t i = 0; i < m_arrFormula.size(); i++)
	{
		if (m_arrFormula[i].IsInit())
			m_arrFormula[i]->toXML2(writer, bExtendedWrite);
	}

	if (m_oDxf.IsInit() && bExtendedWrite)
	{
		m_oDxf->toXML2(writer, L"x14:dxf");
	}

	writer.WriteString(L"</" + node_name + L">");
}
void CConditionalFormattingRule::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	bUsage = false;
	ReadAttributes(oReader);

	if (oReader.IsEmptyNode())
		return;

	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		
		if (L"colorScale" == sName )
			m_oColorScale = oReader;
		else if ( L"dataBar" == sName)
			m_oDataBar = oReader;
		else if ( L"formula" == sName || L"f" == sName)
		{
			nullable<CFormulaCF> formula(oReader);
			m_arrFormula.push_back(formula);
		}
		else if ( L"iconSet" == sName)
			m_oIconSet = oReader;
		else if ( L"dxf" == sName)
			m_oDxf = oReader;
		else if ( L"extLst" == sName )
		{
			m_oExtLst = oReader;
			if ( m_oExtLst.IsInit() )
			{
				for (size_t i = 0; i < m_oExtLst->m_arrExt.size(); ++i)
				{
					if (!m_oExtLst->m_arrExt[i]) continue;

					if (m_oExtLst->m_arrExt[i]->m_oId.IsInit())
					{
						m_oExtId = m_oExtLst->m_arrExt[i]->m_oId;
						break;
					}
				}
			}
		}

	}
}
template<typename Type>
nullable<Type> CConditionalFormattingRule::Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
{
	nullable<Type> oResult;

	if ( oCurrent.IsInit() )	oResult = oCurrent;
	else if ( oPrev.IsInit() )	oResult = oPrev;

	return oResult;
}
const CConditionalFormattingRule CConditionalFormattingRule::Merge(const CConditionalFormattingRule& oPrev, const CConditionalFormattingRule& oCurrent)
{
	CConditionalFormattingRule oRule;
	
	oRule.m_oAboveAverage	= Merge( oPrev.m_oAboveAverage,	oCurrent.m_oAboveAverage );
	oRule.m_oBottom			= Merge( oPrev.m_oBottom,		oCurrent.m_oBottom );
	oRule.m_oDxfId			= Merge( oPrev.m_oDxfId,		oCurrent.m_oDxfId );
	oRule.m_oEqualAverage	= Merge( oPrev.m_oEqualAverage,	oCurrent.m_oEqualAverage );
	oRule.m_oOperator		= Merge( oPrev.m_oOperator,		oCurrent.m_oOperator );
	oRule.m_oPercent		= Merge( oPrev.m_oPercent,		oCurrent.m_oPercent );
	oRule.m_oPriority		= Merge( oPrev.m_oPriority,		oCurrent.m_oPriority );
	oRule.m_oRank			= Merge( oPrev.m_oRank,			oCurrent.m_oRank );
	oRule.m_oStdDev			= Merge( oPrev.m_oStdDev,		oCurrent.m_oStdDev );
	oRule.m_oStopIfTrue		= Merge( oPrev.m_oStopIfTrue,	oCurrent.m_oStopIfTrue );
	oRule.m_oText			= Merge( oPrev.m_oText,			oCurrent.m_oText );
	oRule.m_oTimePeriod		= Merge( oPrev.m_oTimePeriod,	oCurrent.m_oTimePeriod );
	oRule.m_oType			= Merge( oPrev.m_oType,			oCurrent.m_oType );
	oRule.m_oDxf			= Merge( oPrev.m_oDxf,			oCurrent.m_oDxf );

	if (oRule.m_oDxf.IsInit() && oRule.m_oDxfId.IsInit())
	{
		oRule.m_oDxfId.reset();
	}

	if (oPrev.m_oIconSet.IsInit() && oCurrent.m_oIconSet.IsInit())
		oRule.m_oIconSet	= CIconSet::Merge	( oPrev.m_oIconSet.get(),		oCurrent.m_oIconSet.get());
	else
		oRule.m_oIconSet	= Merge( oPrev.m_oIconSet,	oCurrent.m_oIconSet );

	if (oPrev.m_oColorScale.IsInit() && oCurrent.m_oColorScale.IsInit())
		oRule.m_oColorScale = CColorScale::Merge( oPrev.m_oColorScale.get(),	oCurrent.m_oColorScale.get());
	else
		oRule.m_oColorScale	= Merge( oPrev.m_oColorScale,	oCurrent.m_oColorScale );

	if (oPrev.m_oDataBar.IsInit() && oCurrent.m_oDataBar.IsInit())
		oRule.m_oDataBar	= CDataBar::Merge	( oPrev.m_oDataBar.get(),		oCurrent.m_oDataBar.get());		
	else
		oRule.m_oDataBar	= Merge( oPrev.m_oDataBar,	oCurrent.m_oDataBar );

	for (size_t i = 0; i < oCurrent.m_arrFormula.size(); i++)
	{
		oRule.m_arrFormula.push_back(oCurrent.m_arrFormula[i]);
	}
	for (size_t i = oCurrent.m_arrFormula.size(); i < oPrev.m_arrFormula.size(); i++)
	{
		oRule.m_arrFormula.push_back(oPrev.m_arrFormula[i]);
	}

	return oRule;
}
void CConditionalFormattingRule::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_Start(oReader)

	WritingElement_ReadAttributes_Read_if		(oReader, L"aboveAverage"	, m_oAboveAverage)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"bottom"			, m_oBottom)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"dxfId"			, m_oDxfId)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"equalAverage"	, m_oEqualAverage)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"operator"		, m_oOperator)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"percent"		, m_oPercent)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"priority"		, m_oPriority)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"rank"			, m_oRank)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"stdDev"			, m_oStdDev)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"stopIfTrue"		, m_oStopIfTrue)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"text"			, m_oText)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"timePeriod"		, m_oTimePeriod)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"type"			, m_oType)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"id"				, m_oId)

	WritingElement_ReadAttributes_End(oReader)
}

bool CConditionalFormatting::IsExtended()
{
	m_bIsExtended = false;
	m_bIsValid = false;
	
	if (m_oSqRef.IsInit() == false) return m_bIsExtended;

	m_bIsValid = true;

    for ( size_t i = 0; i < m_arrItems.size(); ++i)
	{
        if ( !m_arrItems[i]) continue;
		
		if (m_arrItems[i]->isValid() == false)
			m_bIsValid = false;
		if (m_arrItems[i]->isExtended() == true)
			m_bIsExtended = true;
	}
	return m_bIsExtended;
}
void CConditionalFormatting::toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const
{			
	std::wstring node_name = bExtendedWrite ? L"x14:conditionalFormatting" : L"conditionalFormatting";

	writer.WriteString(L"<" + node_name);
		if (bExtendedWrite)
		{
			WritingStringAttrString(L"xmlns:xm", L"http://schemas.microsoft.com/office/excel/2006/main");
		}
		else
		{
			WritingStringAttrString(L"sqref", m_oSqRef.get());
		}
		if (m_oPivot.IsInit() && true == m_oPivot->ToBool())
		{
			writer.WriteString(L" pivot=\"1\"");
		}
	writer.WriteString(L">");
		
    for ( size_t i = 0; i < m_arrItems.size(); ++i)
    {
        if (  m_arrItems[i] )
        {
            m_arrItems[i]->toXML2(writer, bExtendedWrite);
        }
    }
	if (bExtendedWrite)
	{
		writer.WriteString(L"<xm:sqref>" + m_oSqRef.get() + L"</xm:sqref>");
	}
	writer.WriteString(L"</" + node_name + L">");
}

void CConditionalFormatting::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
	ReadAttributes(oReader);

	if (oReader.IsEmptyNode())
		return;

	int nCurDepth = oReader.GetDepth();
	while (oReader.ReadNextSiblingNode(nCurDepth))
	{
		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

		if (L"cfRule" == sName)
			m_arrItems.push_back(new CConditionalFormattingRule(oReader));
		if (L"sqref" == sName)
			m_oSqRef = oReader.GetText2();
	}
}
bool CConditionalFormatting::IsUsage()
{
    for ( size_t i = 0; i < m_arrItems.size(); ++i)
    {
        if ( (m_arrItems[i]) && (m_arrItems[i]->bUsage)) return true;
    }
	return false;
}
	
void CConditionalFormatting::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
	WritingElement_ReadAttributes_Start(oReader)

	WritingElement_ReadAttributes_Read_if		(oReader, L"sqref"	, m_oSqRef)
	WritingElement_ReadAttributes_Read_else_if	(oReader, L"pivot"	, m_oPivot)

	WritingElement_ReadAttributes_End(oReader)
}

} //Spreadsheet
} // namespace OOX
