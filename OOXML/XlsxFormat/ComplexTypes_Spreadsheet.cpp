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

#include "ComplexTypes_Spreadsheet.h"

namespace ComplexTypes
{
	namespace Spreadsheet
	{
		ComplexType::ComplexType() {}
		ComplexType::~ComplexType() {}

		COnOff2::COnOff2()
		{
		}
		COnOff2::~COnOff2()
		{
		}
		void COnOff2::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void COnOff2::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}
		std::wstring COnOff2::ToString() const
		{
			std::wstring sResult;

			sResult += _T("val=\"");
			sResult += m_oVal.ToString();
			sResult += _T("\" ");

			return sResult;
		}
		void COnOff2::FromBool(bool bVal)
		{
			m_oVal.FromBool(bVal);
		}
		bool COnOff2::ToBool()
		{
			return m_oVal.ToBool();
		}

		String::String()
		{
		}
		String::~String()
		{
		}
		void String::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_sVal );
		}
		void String::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring String::ToString() const
		{
			std::wstring sResult;

			if ( m_sVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += *m_sVal;
				sResult += _T("\" ");
			}

			return sResult;
		}
		std::wstring String::ToString2() const
		{
			std::wstring sResult;

			if ( m_sVal.IsInit() )
				sResult += *m_sVal;

			return sResult;
		}
		void String::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_sVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		CDouble::CDouble()
		{
		}
		CDouble::~CDouble()
		{
		}
		void CDouble::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CDouble::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CDouble::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}

			return sResult;
		}
		void CDouble::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CPointMeasure::CPointMeasure()
		{
		}
		CPointMeasure::~CPointMeasure()
		{
		}
		void CPointMeasure::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CPointMeasure::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPointMeasure::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}

			return sResult;
		}
		void CPointMeasure::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CChartLayoutTarget::CChartLayoutTarget()
		{
		}
		CChartLayoutTarget::~CChartLayoutTarget()
		{
		}
		void CChartLayoutTarget::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CChartLayoutTarget::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CChartLayoutTarget::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}

			return sResult;
		}
		void CChartLayoutTarget::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CChartHMode::CChartHMode()
		{
		}
		CChartHMode::~CChartHMode()
		{
		}
		void CChartHMode::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CChartHMode::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CChartHMode::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}

			return sResult;
		}
		void CChartHMode::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		CChartAxPos::CChartAxPos()
		{
		}
		CChartAxPos::~CChartAxPos()
		{
		}
		void CChartAxPos::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CChartAxPos::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CChartAxPos::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}

			return sResult;
		}
		void CChartAxPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CChartBarGrouping::CChartBarGrouping()
		{
		}
		CChartBarGrouping::~CChartBarGrouping()
		{
		}
		void CChartBarGrouping::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CChartBarGrouping::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CChartBarGrouping::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}

			return sResult;
		}
		void CChartBarGrouping::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
		}

		CChartBarDerection::CChartBarDerection()
		{
		}
		CChartBarDerection::~CChartBarDerection()
		{
		}
		void CChartBarDerection::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CChartBarDerection::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CChartBarDerection::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}

			return sResult;
		}
		void CChartBarDerection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CChartSymbol::CChartSymbol(){}
		CChartSymbol::~CChartSymbol() {}
		void CChartSymbol::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CChartSymbol::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CChartSymbol::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}
			return sResult;
		}
		void CChartSymbol::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CRegionLabelLayout::CRegionLabelLayout(){}
		CRegionLabelLayout::~CRegionLabelLayout() {}
		void CRegionLabelLayout::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CRegionLabelLayout::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CRegionLabelLayout::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}
			return sResult;
		}
		void CRegionLabelLayout::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CParentLabelLayout::CParentLabelLayout() {}
		CParentLabelLayout::~CParentLabelLayout() {}
		void CParentLabelLayout::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
		}
		void CParentLabelLayout::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CParentLabelLayout::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += _T("val=\"");
				sResult += m_oVal->ToString();
				sResult += _T("\" ");
			}
			return sResult;
		}
		void CParentLabelLayout::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
	}
} // ComplexTypes
