﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef OOX_CONDITIONALFORMATTING_FILE_INCLUDE_H_
#define OOX_CONDITIONALFORMATTING_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../Styles/dxf.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CFormulaCF : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFormulaCF)
			CFormulaCF()
			{
				m_sNodeName = L"formula";
			}
			virtual ~CFormulaCF()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<" + m_sNodeName + L">");
				writer.WriteEncodeXmlString(m_sText);
				writer.WriteString(L"</" + m_sNodeName + L">");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_sNodeName = oReader.GetName();

				if (oReader.IsEmptyNode())
					return;

				m_sText = oReader.GetText3();
			}

			virtual EElementType getType () const
			{
				return et_x_FormulaCF;
			}

			std::wstring m_sNodeName;
			std::wstring m_sText;
		};


		class CConditionalFormatValueObject : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConditionalFormatValueObject)
			CConditionalFormatValueObject()
			{
			}
			virtual ~CConditionalFormatValueObject()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if (m_oType.IsInit())
				{
					writer.WriteString(L"<cfvo");
					WritingStringAttrString(L"type", m_oType->ToString());
					if (m_oGte.IsInit() && false == m_oGte->ToBool())
						writer.WriteString(L" gte=\"0\"");
					WritingStringNullableAttrEncodeXmlString(L"val", m_oVal, m_oVal.get());
					writer.WriteString(_T("/>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_ConditionalFormatValueObject;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("gte")		, m_oGte)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("type")	, m_oType)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("val")		, m_oVal)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::COnOff<>>						m_oGte;
			nullable<SimpleTypes::Spreadsheet::ST_CfvoType<>>	m_oType;	
			nullable<std::wstring>								m_oVal;

			nullable<CFormulaCF>								m_oFormula;
		};

		class CColorScale : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CColorScale)
			CColorScale()
			{
			}
			virtual ~CColorScale()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if (3 < m_arrItems.size()) // min 2 + 2
				{
                    std::wstring sValue;
					writer.WriteString(_T("<colorScale>"));

                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }

					writer.WriteString(_T("</colorScale>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (_T("cfvo") == sName)
						m_arrItems.push_back(new CConditionalFormatValueObject(oReader));
					else if (_T("color") == sName)
						m_arrItems.push_back(new CColor(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_ColorScale;
			}

		public:
		};

		class CDataBar : public WritingElementWithChilds<CConditionalFormatValueObject>
		{
		public:
			WritingElement_AdditionConstructors(CDataBar)
			CDataBar()
			{
			}
			virtual ~CDataBar()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if (2 == m_arrItems.size() && m_oColor.IsInit())
				{
                    std::wstring sValue;
					writer.WriteString(_T("<dataBar"));
					WritingStringNullableAttrInt(L"maxLength", m_oMaxLength, m_oMaxLength->GetValue());
					WritingStringNullableAttrInt(L"minLength", m_oMinLength, m_oMinLength->GetValue());
					if (m_oShowValue.IsInit() && false == m_oShowValue->ToBool())
					{
						writer.WriteString(_T(" showValue=\"0\""));
					}

					writer.WriteString(_T(">"));

                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }

					m_oColor->toXML2(writer, _T("color"));

					writer.WriteString(_T("</dataBar>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (_T("cfvo") == sName)
						m_arrItems.push_back(new CConditionalFormatValueObject(oReader));
					else if (_T("color") == sName)
						m_oColor = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_DataBar;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("maxLength")	, m_oMaxLength)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("minLength")	, m_oMinLength)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("showValue")	, m_oShowValue)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMaxLength;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMinLength;
			nullable<SimpleTypes::COnOff<>>					m_oShowValue;

			nullable<CColor>								m_oColor;
		};

		class CIconSet : public WritingElementWithChilds<CConditionalFormatValueObject>
		{
		public:
			WritingElement_AdditionConstructors(CIconSet)
			CIconSet()
			{
			}
			virtual ~CIconSet()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if (1 < m_arrItems.size()) // min value = 2
				{
                    std::wstring sValue;
					writer.WriteString(_T("<iconSet"));
					WritingStringNullableAttrString(L"iconSet", m_oIconSet, m_oIconSet->ToString())
					if (m_oPercent.IsInit() && false == m_oPercent->ToBool())
					{
						writer.WriteString(_T(" percent=\"0\""));
					}
					if (m_oReverse.IsInit() && true == m_oReverse->ToBool())
					{
						writer.WriteString(_T(" reverse=\"1\""));
					}
					if (m_oShowValue.IsInit() && false == m_oShowValue->ToBool())
					{
						writer.WriteString(_T(" showValue=\"0\""));
					}

					writer.WriteString(_T(">"));

                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }

					writer.WriteString(_T("</iconSet>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (_T("cfvo") == sName)
						m_arrItems.push_back(new CConditionalFormatValueObject(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_IconSet;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("iconSet")		, m_oIconSet)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("percent")		, m_oPercent)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("reverse")		, m_oReverse)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("showValue")	, m_oShowValue)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::Spreadsheet::ST_IconSetType<>>m_oIconSet;
			nullable<SimpleTypes::COnOff<>>						m_oPercent;
			nullable<SimpleTypes::COnOff<>>						m_oReverse;
			nullable<SimpleTypes::COnOff<>>						m_oShowValue;
		};

		//необработано:
		//<extLst>
		class CConditionalFormattingRule : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElement_AdditionConstructors(CConditionalFormattingRule)
			CConditionalFormattingRule()
			{
			}
			virtual ~CConditionalFormattingRule()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if (isValid())
				{
					writer.WriteString(L"<cfRule");
					WritingStringAttrString(L"type", m_oType->ToString());
					WritingStringAttrInt(L"priority", m_oPriority->GetValue());
					if (m_oAboveAverage.IsInit() && false == m_oAboveAverage->ToBool())
						writer.WriteString(_T (" aboveAverage=\"0\""));
					if (m_oBottom.IsInit() && true == m_oBottom->ToBool())
						writer.WriteString(_T (" bottom=\"1\""));
					WritingStringNullableAttrInt(L"dxfId", m_oDxfId, m_oDxfId->GetValue());
					if (m_oEqualAverage.IsInit() && true == m_oEqualAverage->ToBool())
						writer.WriteString(_T (" equalAverage=\"1\""));
					WritingStringNullableAttrString(L"operator", m_oOperator, m_oOperator->ToString());
					if (m_oPercent.IsInit() && true == m_oPercent->ToBool())
						writer.WriteString(_T (" percent=\"1\""));
					WritingStringNullableAttrInt(L"rank", m_oRank, m_oRank->GetValue());
					WritingStringNullableAttrInt(L"stdDev", m_oStdDev, m_oStdDev->GetValue());
					if (m_oStopIfTrue.IsInit() && true == m_oStopIfTrue->ToBool())
						writer.WriteString(_T (" stopIfTrue=\"1\""));
					WritingStringNullableAttrString(L"text", m_oText, m_oText.get());
					WritingStringNullableAttrString(L"timePeriod", m_oTimePeriod, m_oTimePeriod.get());

					writer.WriteString(_T(">"));

                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }

					writer.WriteString(_T("</cfRule>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (L"colorScale" == sName)
						m_arrItems.push_back(new CColorScale(oReader));
					else if (L"dataBar" == sName)
						m_arrItems.push_back(new CDataBar(oReader));
					else if (L"formula" == sName || L"f" == sName)
						m_arrItems.push_back(new CFormulaCF(oReader));
					else if (L"iconSet" == sName)
						m_arrItems.push_back(new CIconSet(oReader));
					else if (L"dxf" == sName)
						m_oDxf = oReader;

				}
			}

			virtual EElementType getType () const
			{
				return et_x_ConditionalFormattingRule;
			}

			bool isValid () const
			{
				return m_oType.IsInit() && m_oPriority.IsInit();
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("aboveAverage")	, m_oAboveAverage)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("bottom")			, m_oBottom)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("dxfId")			, m_oDxfId)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("equalAverage")	, m_oEqualAverage)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("operator")		, m_oOperator)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("percent")			, m_oPercent)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("priority")		, m_oPriority)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("rank")			, m_oRank)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("stdDev")			, m_oStdDev)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("stopIfTrue")		, m_oStopIfTrue)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("text")			, m_oText)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("timePeriod")		, m_oTimePeriod)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("type")			, m_oType)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<CDxf>										m_oDxf;
			nullable<SimpleTypes::COnOff<>>						m_oAboveAverage;
			nullable<SimpleTypes::COnOff<>>						m_oBottom;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oDxfId;
			nullable<SimpleTypes::COnOff<>>						m_oEqualAverage;
			nullable<SimpleTypes::Spreadsheet::ST_CfOperator<>>	m_oOperator;	
			nullable<SimpleTypes::COnOff<>>						m_oPercent;
			nullable<SimpleTypes::CDecimalNumber<>>				m_oPriority;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRank;
			nullable<SimpleTypes::CDecimalNumber<>>				m_oStdDev;
			nullable<SimpleTypes::COnOff<>>						m_oStopIfTrue;
			nullable<std::wstring>								m_oText;
			nullable<std::wstring>								m_oTimePeriod;	// ToDo переделать на тип ST_TimePeriod (18.18.82)
			nullable<SimpleTypes::Spreadsheet::ST_CfType<>>		m_oType;	
		};

		//необработано:
		//<extLst>
		class CConditionalFormatting  : public WritingElementWithChilds<CConditionalFormattingRule>
		{
		public:
			WritingElement_AdditionConstructors(CConditionalFormatting)
			CConditionalFormatting()
			{
			}
			virtual ~CConditionalFormatting()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				bool isValid = false;

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( (m_arrItems[i]) && (m_arrItems[i]->isValid()) )
					{
						isValid = true;
						break;
					}
				}
				if (m_oSqRef.IsInit() && isValid)
				{
					writer.WriteString(L"<conditionalFormatting");
					WritingStringAttrString(L"sqref", m_oSqRef.get());

					if (m_oPivot.IsInit() && true == m_oPivot->ToBool())
					{
						writer.WriteString(_T (" pivot=\"1\""));
					}

					writer.WriteString(_T(">"));

                    for ( size_t i = 0; i < m_arrItems.size(); ++i)
                    {
                        if (  m_arrItems[i] )
                        {
                            m_arrItems[i]->toXML(writer);
                        }
                    }

					writer.WriteString(_T("</conditionalFormatting>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (_T("cfRule") == sName)
						m_arrItems.push_back(new CConditionalFormattingRule(oReader));
					if (_T("sqref") == sName)
						m_oSqRef = oReader.GetText2();
				}
			}

			virtual EElementType getType () const
			{
				return et_x_ConditionalFormatting;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("sqref")	, m_oSqRef)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("pivot")	, m_oPivot)

				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::COnOff<>>			m_oPivot;
			nullable<std::wstring >	m_oSqRef; // ToDo переделать на тип "sqref" (18.18.76) - последовательность "ref", разделенные пробелом
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CONDITIONALFORMATTING_FILE_INCLUDE_H_
