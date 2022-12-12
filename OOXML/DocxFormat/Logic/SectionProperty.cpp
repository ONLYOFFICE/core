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
#include "../DocxFlat.h"
#include "../HeaderFooter.h"

#include "SectionProperty.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// Column 17.6.3 (Part 1)
		//--------------------------------------------------------------------------------

		CColumn::CColumn()
		{
		}
		CColumn::~CColumn()
		{
		}
		void CColumn::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:space", m_oSpace );
			XmlMacroReadAttributeBase( oNode, L"w:w",     m_oW );
		}
		void CColumn::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CColumn::ToString() const
		{
			std::wstring sResult;

			if ( m_oW.IsInit() )
			{
				sResult += L"w:w=\"" + std::to_wstring(m_oW->ToTwips()) + L"\" ";
			}

			if ( m_oSpace.IsInit() )
			{
				sResult += L"w:space=\"" + std::to_wstring(m_oSpace->ToTwips()) + L"\" ";
			}

			return sResult;
		}
		void CColumn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:space"), m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:w"),     m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// DocGrid 17.6.5 (Part 1)
		//--------------------------------------------------------------------------------

		CDocGrid::CDocGrid()
		{
		}
		CDocGrid::~CDocGrid()
		{
		}
		void CDocGrid::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:charSpace"), m_oCharSpace );
			XmlMacroReadAttributeBase( oNode, (L"w:linePitch"), m_oLinePitch );
			XmlMacroReadAttributeBase( oNode, (L"w:type"),      m_oType );
		}
		void CDocGrid::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CDocGrid::ToString() const
		{
			std::wstring sResult;

			if (m_oType.IsInit())
			{
				sResult += L" w:type=\"" + m_oType->ToString() + L"\"";
			}
			if ( m_oLinePitch.IsInit() )
			{
				sResult += L" w:linePitch=\"" + m_oLinePitch->ToString() + L"\"";
			}
			if (m_oCharSpace.IsInit())
			{
				sResult += L" w:charSpace=\"" + m_oCharSpace->ToString() + L"\"";
			}
			return sResult;
		}
		void CDocGrid::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:charSpace"), m_oCharSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:linePitch"), m_oLinePitch )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:type"),      m_oType )
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"w:line-pitch"), m_oLinePitch)	//2003 xml
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// HdrFtrRef 17.10.5 (Part 1)
		//--------------------------------------------------------------------------------

		CHdrFtrRef::CHdrFtrRef()
		{
		}
		CHdrFtrRef::~CHdrFtrRef()
		{
		}
		void CHdrFtrRef::FromXML(XmlUtils::CXmlNode& oNode)
		{//todooo переделать на перебор всех и без неймспейсов
			XmlMacroReadAttributeBase( oNode, L"r:id",   m_oId );
			XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );

			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
			}
		}
		void CHdrFtrRef::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CHdrFtrRef::ToString() const
		{
			std::wstring sResult;

			if ( m_oType.IsInit() )
			{
				sResult += L" w:type=\"" + m_oType->ToString() + L"\"";
			}
			if ( m_oId.IsInit() )
			{
				sResult += L" r:id=\"" + m_oId->ToString() + L"\"";
			}
			return sResult;
		}
		void CHdrFtrRef::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, (L"id"),	m_oId )
					WritingElement_ReadAttributes_Read_else_if( oReader, (L"type"),	m_oType )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}

		//--------------------------------------------------------------------------------
		// LineNumber 17.6.8 (Part 1)
		//--------------------------------------------------------------------------------

		CLineNumber::CLineNumber()
		{
		}
		CLineNumber::~CLineNumber()
		{
		}
		void CLineNumber::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:countBy"),  m_oCountBy );
			XmlMacroReadAttributeBase( oNode, (L"w:distance"), m_oDistance );
			XmlMacroReadAttributeBase( oNode, (L"w:restart"),  m_oRestart );
			XmlMacroReadAttributeBase( oNode, (L"w:start"),    m_oStart );
		}
		void CLineNumber::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CLineNumber::ToString() const
		{
			std::wstring sResult;

			if ( m_oCountBy.IsInit() )
			{
				sResult += (L"w:countBy=\"");
				sResult += m_oCountBy->ToString();
				sResult += (L"\" ");
			}

			if ( m_oDistance.IsInit() )
			{
				sResult += (L"w:distance=\"");
				sResult += std::to_wstring(m_oDistance->ToTwips());
				sResult += (L"\" ");
			}

			if ( m_oRestart.IsInit() )
			{
				sResult += (L"w:restart=\"");
				sResult += m_oRestart->ToString();
				sResult += (L"\" ");
			}

			if ( m_oStart.IsInit() )
			{
				sResult += (L"w:start=\"");
				sResult += m_oStart->ToString();
				sResult += (L"\" ");
			}

			return sResult;
		}
		void CLineNumber::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:countBy"),  m_oCountBy )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:distance"), m_oDistance )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:restart"),  m_oRestart )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:start"),    m_oStart )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// PaperSource 17.6.9 (Part 1)
		//--------------------------------------------------------------------------------

		CPaperSource::CPaperSource()
		{
		}
		CPaperSource::~CPaperSource()
		{
		}
		void CPaperSource::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:first"), m_oFirst );
			XmlMacroReadAttributeBase( oNode, (L"w:other"), m_oOther );
		}
		void CPaperSource::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPaperSource::ToString() const
		{
			std::wstring sResult;

			if ( m_oFirst.IsInit() )
			{
				sResult += (L"w:first=\"");
				sResult += m_oFirst->ToString();
				sResult += (L"\" ");
			}

			if ( m_oOther.IsInit() )
			{
				sResult += (L"w:other=\"");
				sResult += m_oOther->ToString();
				sResult += (L"\" ");
			}

			return sResult;
		}
		void CPaperSource::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:first"), m_oFirst )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:other"), m_oOther )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// PageBorder 17.6.7 (Part 1)
		//--------------------------------------------------------------------------------

		CPageBorder::CPageBorder()
		{
		}
		CPageBorder::~CPageBorder()
		{
		}
		void CPageBorder::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:color"),       m_oColor );
			XmlMacroReadAttributeBase( oNode, (L"w:frame"),       m_oFrame );
			XmlMacroReadAttributeBase( oNode, (L"r:id"),          m_oId );
			XmlMacroReadAttributeBase( oNode, (L"w:shadow"),      m_oShadow );
			XmlMacroReadAttributeBase( oNode, (L"w:space"),       m_oSpace );
			XmlMacroReadAttributeBase( oNode, (L"w:sz"),          m_oSz );
			XmlMacroReadAttributeBase( oNode, (L"w:themeColor"),  m_oThemeColor );
			XmlMacroReadAttributeBase( oNode, (L"w:themeShade"),  m_oThemeShade );
			XmlMacroReadAttributeBase( oNode, (L"w:themeTint"),   m_oThemeTint );
			XmlMacroReadAttributeBase( oNode, (L"w:val"),         m_oVal );

			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
			}
		}
		void CPageBorder::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPageBorder::ToString() const
		{
			std::wstring sResult;

			if ( m_oColor.IsInit() )
			{
				sResult += (L"w:color=\"");
				sResult += m_oColor->ToStringNoAlpha();
				sResult += (L"\" ");
			}

			if ( m_oFrame.IsInit() )
			{
				sResult += (L"w:frame=\"");
				sResult += m_oFrame->ToString2(SimpleTypes::onofftostring1);
				sResult += (L"\" ");
			}
			if ( m_oId.IsInit() )
			{
				sResult += (L"r:id=\"");
				sResult += m_oId->ToString();
				sResult += (L"\" ");
			}

			if ( m_oShadow.IsInit() )
			{
				sResult += (L"w:shadow=\"");
				sResult += m_oShadow->ToString2(SimpleTypes::onofftostring1);
				sResult += (L"\" ");
			}

			if ( m_oSpace.IsInit() )
			{
				sResult += (L"w:space=\"");
				sResult += m_oSpace->ToString();
				sResult += (L"\" ");
			}

			if ( m_oSz.IsInit() )
			{
				sResult += (L"w:sz=\"");
				sResult += m_oSz->ToString();
				sResult += (L"\" ");
			}
			if ( m_oThemeColor.IsInit() )
			{
				sResult += (L"w:themeColor=\"");
				sResult += m_oThemeColor->ToString();
				sResult += (L"\" ");
			}

			if ( m_oThemeShade.IsInit() )
			{
				sResult += (L"w:themeShade=\"");
				sResult += m_oThemeShade->ToString();
				sResult += (L"\" ");
			}

			if ( m_oThemeTint.IsInit() )
			{
				sResult += (L"w:themeTint=\"");
				sResult += m_oThemeTint->ToString();
				sResult += (L"\" ");
			}

			if ( m_oVal.IsInit() )
			{
				sResult += (L"w:val=\"");
				sResult += m_oVal->ToString();
				sResult += (L"\" ");
			}

			return sResult;
		}
		void CPageBorder::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:color"),			m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:frame"),			m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:id"),				m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"relationships:id"),	m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:shadow"),			m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:space"),			m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:sz"),				m_oSz )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeColor"),		m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeShade"),		m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeTint"),		m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:val"),			m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// BottomPageBorder 17.6.2 (Part 1)
		//--------------------------------------------------------------------------------

		CBottomPageBorder::CBottomPageBorder()
		{
		}
		CBottomPageBorder::~CBottomPageBorder()
		{
		}
		void CBottomPageBorder::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"r:bottomLeft",  m_oBottomLeft );
			XmlMacroReadAttributeBase( oNode, L"r:bottomRight", m_oBottomRight );
			XmlMacroReadAttributeBase( oNode, L"w:color",       m_oColor );
			XmlMacroReadAttributeBase( oNode, L"w:frame",       m_oFrame );
			XmlMacroReadAttributeBase( oNode, L"r:id",          m_oId );
			XmlMacroReadAttributeBase( oNode, L"w:shadow",      m_oShadow );
			XmlMacroReadAttributeBase( oNode, L"w:space",       m_oSpace );
			XmlMacroReadAttributeBase( oNode, L"w:sz",          m_oSz );
			XmlMacroReadAttributeBase( oNode, L"w:themeColor",  m_oThemeColor );
			XmlMacroReadAttributeBase( oNode, L"w:themeShade",  m_oThemeShade );
			XmlMacroReadAttributeBase( oNode, L"w:themeTint",   m_oThemeTint );
			XmlMacroReadAttributeBase( oNode, L"w:val",         m_oVal );

			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
			}
		}
		void CBottomPageBorder::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CBottomPageBorder::ToString() const
		{
			std::wstring sResult;

			if ( m_oBottomLeft.IsInit() )
			{
				sResult += (L"r:bottomLeft=\"");
				sResult += m_oBottomLeft->ToString();
				sResult += (L"\" ");
			}

			if ( m_oBottomRight.IsInit() )
			{
				sResult += (L"r:bottomRight=\"");
				sResult += m_oBottomRight->ToString();
				sResult += (L"\" ");
			}

			if ( m_oColor.IsInit() )
			{
				sResult += (L"w:color=\"");
				sResult += m_oColor->ToStringNoAlpha();
				sResult += (L"\" ");
			}

			if ( m_oFrame.IsInit() )
			{
				sResult += (L"w:frame=\"");
				sResult += m_oFrame->ToString2(SimpleTypes::onofftostring1);
				sResult += (L"\" ");
			}
			if ( m_oId.IsInit() )
			{
				sResult += (L"r:id=\"");
				sResult += m_oId->ToString();
				sResult += (L"\" ");
			}

			if ( m_oShadow.IsInit() )
			{
				sResult += (L"w:shadow=\"");
				sResult += m_oShadow->ToString2(SimpleTypes::onofftostring1);
				sResult += (L"\" ");
			}

			if ( m_oSpace.IsInit() )
			{
				sResult += (L"w:space=\"");
				sResult += m_oSpace->ToString();
				sResult += (L"\" ");
			}

			if ( m_oSz.IsInit() )
			{
				sResult += (L"w:sz=\"");
				sResult += m_oSz->ToString();
				sResult += (L"\" ");
			}
			if ( m_oThemeColor.IsInit() )
			{
				sResult += (L"w:themeColor=\"");
				sResult += m_oThemeColor->ToString();
				sResult += (L"\" ");
			}

			if ( m_oThemeShade.IsInit() )
			{
				sResult += (L"w:themeShade=\"");
				sResult += m_oThemeShade->ToString();
				sResult += (L"\" ");
			}

			if ( m_oThemeTint.IsInit() )
			{
				sResult += (L"w:themeTint=\"");
				sResult += m_oThemeTint->ToString();
				sResult += (L"\" ");
			}

			if ( m_oVal.IsInit() )
			{
				sResult += (L"w:val=\"");
				sResult += m_oVal->ToString();
				sResult += (L"\" ");
			}

			return sResult;
		}
		void CBottomPageBorder::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"r:bottomLeft"), m_oBottomLeft )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:bottomRight"), m_oBottomRight )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:id"),          m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"relationships:id"), m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:shadow"),      m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:space"),       m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:sz"),          m_oSz )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeColor"),  m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeShade"),  m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeTint"),   m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:val"),         m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// TopPageBorder 17.6.7 (Part 1)
		//--------------------------------------------------------------------------------

		CTopPageBorder::CTopPageBorder()
		{
		}
		CTopPageBorder::~CTopPageBorder()
		{
		}
		void CTopPageBorder::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:color"),       m_oColor );
			XmlMacroReadAttributeBase( oNode, (L"w:frame"),       m_oFrame );
			XmlMacroReadAttributeBase( oNode, (L"r:id"),          m_oId );
			XmlMacroReadAttributeBase( oNode, (L"w:shadow"),      m_oShadow );
			XmlMacroReadAttributeBase( oNode, (L"w:space"),       m_oSpace );
			XmlMacroReadAttributeBase( oNode, (L"w:sz"),          m_oSz );
			XmlMacroReadAttributeBase( oNode, (L"w:themeColor"),  m_oThemeColor );
			XmlMacroReadAttributeBase( oNode, (L"w:themeShade"),  m_oThemeShade );
			XmlMacroReadAttributeBase( oNode, (L"w:themeTint"),   m_oThemeTint );
			XmlMacroReadAttributeBase( oNode, (L"r:topLeft"),     m_oTopLeft );
			XmlMacroReadAttributeBase( oNode, (L"r:topRight"),    m_oTopRight );
			XmlMacroReadAttributeBase( oNode, (L"w:val"),         m_oVal );

			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
			}
		}
		void CTopPageBorder::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CTopPageBorder::ToString() const
		{
			std::wstring sResult;

			if ( m_oColor.IsInit() )
			{
				sResult += (L"w:color=\"");
				sResult += m_oColor->ToStringNoAlpha();
				sResult += (L"\" ");
			}

			if ( m_oFrame.IsInit() )
			{
				sResult += (L"w:frame=\"");
				sResult += m_oFrame->ToString2(SimpleTypes::onofftostring1);
				sResult += (L"\" ");
			}
			if ( m_oId.IsInit() )
			{
				sResult += (L"r:id=\"");
				sResult += m_oId->ToString();
				sResult += (L"\" ");
			}

			if ( m_oShadow.IsInit() )
			{
				sResult += (L"w:shadow=\"");
				sResult += m_oShadow->ToString2(SimpleTypes::onofftostring1);
				sResult += (L"\" ");
			}

			if ( m_oSpace.IsInit() )
			{
				sResult += (L"w:space=\"");
				sResult += m_oSpace->ToString();
				sResult += (L"\" ");
			}

			if ( m_oSz.IsInit() )
			{
				sResult += (L"w:sz=\"");
				sResult += m_oSz->ToString();
				sResult += (L"\" ");
			}
			if ( m_oThemeColor.IsInit() )
			{
				sResult += (L"w:themeColor=\"");
				sResult += m_oThemeColor->ToString();
				sResult += (L"\" ");
			}

			if ( m_oThemeShade.IsInit() )
			{
				sResult += (L"w:themeShade=\"");
				sResult += m_oThemeShade->ToString();
				sResult += (L"\" ");
			}

			if ( m_oThemeTint.IsInit() )
			{
				sResult += (L"w:themeTint=\"");
				sResult += m_oThemeTint->ToString();
				sResult += (L"\" ");
			}

			if ( m_oTopLeft.IsInit() )
			{
				sResult += (L"r:topLeft=\"");
				sResult += m_oTopLeft->ToString();
				sResult += (L"\" ");
			}

			if ( m_oTopRight.IsInit() )
			{
				sResult += (L"r:topRight=\"");
				sResult += m_oTopRight->ToString();
				sResult += (L"\" ");
			}

			if ( m_oVal.IsInit() )
			{
				sResult += (L"w:val=\"");
				sResult += m_oVal->ToString();
				sResult += (L"\" ");
			}

			return sResult;
		}
		void CTopPageBorder::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:id"),          m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"relationships:id"), m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:shadow"),      m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:space"),       m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:sz"),          m_oSz )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeColor"),  m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeShade"),  m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:themeTint"),   m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:topLeft"),     m_oTopLeft )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:topRight"),    m_oTopRight )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:val"),         m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// PageMar 17.6.11 (Part 1)
		//--------------------------------------------------------------------------------

		CPageMar::CPageMar()
		{
		}
		CPageMar::~CPageMar()
		{
		}
		void CPageMar::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:bottom"),  m_oBottom );
			XmlMacroReadAttributeBase( oNode, (L"w:footer"),  m_oFooter );
			XmlMacroReadAttributeBase( oNode, (L"w:gutter"),  m_oGutter );
			XmlMacroReadAttributeBase( oNode, (L"w:header"),  m_oHeader );
			XmlMacroReadAttributeBase( oNode, (L"w:left"),    m_oLeft   );
			XmlMacroReadAttributeBase( oNode, (L"w:right"),   m_oRight  );
			XmlMacroReadAttributeBase( oNode, (L"w:top"),     m_oTop    );
		}
		void CPageMar::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPageMar::ToString() const
		{
			std::wstring sResult;

			if (m_oTop.IsInit())
			{
				sResult += L" w:top=\"" + m_oTop->ToString() + L"\"";
			}
			if (m_oRight.IsInit())
			{
				sResult += L" w:right=\"" + m_oRight->ToString() + L"\"";
			}
			if ( m_oBottom.IsInit() )
			{
				sResult += L" w:bottom=\"" + m_oBottom->ToString() + L"\"";
			}
			if (m_oLeft.IsInit())
			{
				sResult += L" w:left=\"" + m_oLeft->ToString() + L"\"";
			}
			if (m_oHeader.IsInit())
			{
				sResult += L" w:header=\"" + m_oHeader->ToString() + L"\"";
			}
			if ( m_oFooter.IsInit() )
			{
				sResult += L" w:footer=\"" + m_oFooter->ToString() + L"\"";
			}
			if ( m_oGutter.IsInit() )
			{
				sResult += L" w:gutter=\"" + m_oGutter->ToString() + L"\"";
			}
			return sResult;
		}
		void CPageMar::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:bottom"), m_oBottom )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:footer"), m_oFooter )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:gutter"), m_oGutter )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:header"), m_oHeader )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:left"),   m_oLeft )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:right"),  m_oRight )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:top"),    m_oTop )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// PageNumber 17.6.12 (Part 1)
		//--------------------------------------------------------------------------------

		CPageNumber::CPageNumber()
		{
		}
		CPageNumber::~CPageNumber()
		{
		}
		void CPageNumber::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:chapSep"),   m_oChapSep );
			XmlMacroReadAttributeBase( oNode, (L"w:chapStyle"), m_oChapStyle );
			XmlMacroReadAttributeBase( oNode, (L"w:fmt"),       m_oFmt );
			XmlMacroReadAttributeBase( oNode, (L"w:start"),     m_oStart );
		}
		void CPageNumber::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPageNumber::ToString() const
		{
			std::wstring sResult;

			if ( m_oChapSep.IsInit() )
			{
				sResult += (L"w:chapSep=\"");
				sResult += m_oChapSep->ToString();
				sResult += (L"\" ");
			}

			if ( m_oChapStyle.IsInit() )
			{
				sResult += (L"w:chapStyle=\"");
				sResult += m_oChapStyle->ToString();
				sResult += (L"\" ");
			}
			if ( m_oFmt.IsInit() )
			{
				sResult += (L"w:fmt=\"");
				sResult += m_oFmt->ToString();
				sResult += (L"\" ");
			}
			if ( m_oStart.IsInit() )
			{
				sResult += (L"w:start=\"");
				sResult += m_oStart->ToString();
				sResult += (L"\" ");
			}

			return sResult;
		}
		void CPageNumber::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:chapSep"),   m_oChapSep )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:chapStyle"), m_oChapStyle )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:fmt"),       m_oFmt )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:start"),     m_oStart )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// PageSz 17.6.13 (Part 1)
		//--------------------------------------------------------------------------------

		CPageSz::CPageSz()
		{
		}
		CPageSz::~CPageSz()
		{
		}
		void CPageSz::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:code"),   m_oCode );
			XmlMacroReadAttributeBase( oNode, (L"w:h"),      m_oH );
			XmlMacroReadAttributeBase( oNode, (L"w:orient"), m_oOrient );
			XmlMacroReadAttributeBase( oNode, (L"w:w"),      m_oW );
		}
		void CPageSz::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CPageSz::ToString() const
		{
			std::wstring sResult;

			if ( m_oCode.IsInit() )
			{
				sResult += (L"w:code=\"");
				sResult += m_oCode->ToString();
				sResult += (L"\" ");
			}

			if ( m_oH.IsInit() )
			{
				sResult += (L"w:h=\"");
				sResult += m_oH->ToString();
				sResult += (L"\" ");
			}
			if ( m_oOrient.IsInit() )
			{
				sResult += (L"w:orient=\"");
				sResult += m_oOrient->ToString();
				sResult += (L"\" ");
			}
			if ( m_oW.IsInit() )
			{
				sResult += (L"w:w=\"");
				sResult += m_oW->ToString();
				sResult += (L"\" ");
			}

			return sResult;
		}
		void CPageSz::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:code"),   m_oCode )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:h"),      m_oH )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:orient"), m_oOrient )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:w"),      m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// SectType 17.6.22 (Part 1)
		//--------------------------------------------------------------------------------

		CSectType::CSectType()
		{
		}
		CSectType::~CSectType()
		{
		}
		void CSectType::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:val"), m_oVal );
		}
		void CSectType::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSectType::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += L"w:val=\"";
				sResult += m_oVal->ToString();
				sResult += L"\" ";
			}

			return sResult;
		}
		void CSectType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, (L"w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

	} // Word
} // ComplexTypes


namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Columns 17.6.4 (Part 1)
		//--------------------------------------------------------------------------------

		CColumns::CColumns()
		{
		}
		CColumns::~CColumns()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrColumns.size(); nIndex++ )
			{
				if ( m_arrColumns[nIndex] )	delete m_arrColumns[nIndex];
				m_arrColumns[nIndex] = NULL;
			}
			m_arrColumns.clear();
		}
		void CColumns::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:equalWidth"), m_oEqualWidth );
			XmlMacroReadAttributeBase( oNode, (L"w:num"),        m_oNum );
			XmlMacroReadAttributeBase( oNode, (L"w:sep"),        m_oSep );
			XmlMacroReadAttributeBase( oNode, (L"w:space"),      m_oSpace );

			XmlUtils::CXmlNodes oCols;

			if ( oNode.GetNodes( (L"w:col"), oCols ) )
			{
				for ( int nIndex = 0; nIndex < oCols.GetCount(); nIndex++ )
				{
					XmlUtils::CXmlNode oCol;
					if ( oCols.GetAt( nIndex, oCol ) )
					{
						ComplexTypes::Word::CColumn *oColumn = new ComplexTypes::Word::CColumn(oCol);
						if (oColumn) m_arrColumns.push_back( oColumn );
					}
				}
			}
		}
		void CColumns::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:col" == sName )
				{
					ComplexTypes::Word::CColumn *oColumn = new ComplexTypes::Word::CColumn(oReader);
					if (oColumn) m_arrColumns.push_back( oColumn );
				}
			}
		}
		std::wstring CColumns::toXML() const
		{
			std::wstring sResult = L"<w:cols ";

			if ( m_oNum.IsInit() )
			{
				sResult += L"w:num=\"";
				sResult += m_oNum->ToString();
				sResult += L"\" ";
			}

			if ( m_oSep.IsInit() )
			{
				sResult += L"w:sep=\"";
				sResult += m_oSep->ToString2(SimpleTypes::onofftostring1);
				sResult += L"\" ";
			}

			if ( m_oSpace.IsInit() )
			{
				sResult +=L"w:space=\"" + std::to_wstring(m_oSpace->ToTwips()) + L"\" ";
			}

			if ( m_oEqualWidth.IsInit() )
			{
				sResult += L"w:equalWidth=\"";
				sResult += m_oEqualWidth->ToString2(SimpleTypes::onofftostring1);
				sResult += L"\" ";
			}

			sResult += L">";

			for ( unsigned int nIndex = 0; nIndex < m_arrColumns.size(); nIndex++ )
			{
				sResult += L"<w:col ";
				if (m_arrColumns[nIndex])
					sResult += m_arrColumns[nIndex]->ToString();
				sResult += L"/>";
			}

			sResult += L"</w:cols>";

			return sResult;
		}
		EElementType CColumns::getType () const
		{
			return et_w_cols;
		}
		void CColumns::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:equalWidth"), m_oEqualWidth )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:num"),        m_oNum )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:sep"),        m_oSep )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:space"),      m_oSpace )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// EdnProps 17.11.5 (Part 1)
		//--------------------------------------------------------------------------------

		CEdnProps::CEdnProps()
		{
		}
		CEdnProps::~CEdnProps()
		{
		}
		void CEdnProps::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( (L"w:numFmt"), oChild ) )
				m_oNumFmt = oChild;

			if ( oNode.GetNode( (L"w:numRestart"), oChild ) )
				m_oNumRestart = oChild;

			if ( oNode.GetNode( (L"w:numStart"), oChild ) )
				m_oNumStart = oChild;

			if ( oNode.GetNode( (L"w:pos"), oChild ) )
				m_oPos = oChild;
		}
		void CEdnProps::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( (L"w:numFmt") == sName )
					m_oNumFmt = oReader;
				else if ( (L"w:numRestart") == sName )
					m_oNumRestart = oReader;
				else if ( (L"w:numStart") == sName )
					m_oNumStart = oReader;
				else if ( (L"w:pos") == sName )
					m_oPos = oReader;
			}
		}
		std::wstring CEdnProps::toXML() const
		{
			std::wstring sResult = (L"<w:endnotePr>");

			if ( m_oNumFmt.IsInit() )
			{
				sResult += (L"<w:numFmt ");
				sResult += m_oNumFmt->ToString();
				sResult += (L"/>");
			}

			if ( m_oNumRestart.IsInit() )
			{
				sResult += (L"<w:numRestart ");
				sResult += m_oNumRestart->ToString();
				sResult += (L"/>");
			}

			if ( m_oNumStart.IsInit() )
			{
				sResult += (L"<w:numStart ");
				sResult += m_oNumStart->ToString();
				sResult += (L"/>");
			}

			if ( m_oPos.IsInit() )
			{
				sResult += (L"<w:pos ");
				sResult += m_oPos->ToString();
				sResult += (L"/>");
			}

			sResult += (L"</w:endnotePr>");

			return sResult;
		}
		EElementType CEdnProps::getType() const
		{
			return et_w_endnotePr;
		}

		//--------------------------------------------------------------------------------
		// FtnProps 17.11.11 (Part 1)
		//--------------------------------------------------------------------------------

		CFtnProps::CFtnProps()
		{
		}
		CFtnProps::~CFtnProps()
		{
		}
		void CFtnProps::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( (L"w:numFmt"), oChild ) )
				m_oNumFmt = oChild;

			if ( oNode.GetNode( (L"w:numRestart"), oChild ) )
				m_oNumRestart = oChild;

			if ( oNode.GetNode( (L"w:numStart"), oChild ) )
				m_oNumStart = oChild;

			if ( oNode.GetNode( (L"w:pos"), oChild ) )
				m_oPos = oChild;
		}
		void CFtnProps::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( (L"w:numFmt") == sName )
					m_oNumFmt = oReader;
				else if ( (L"w:numRestart") == sName )
					m_oNumRestart = oReader;
				else if ( (L"w:numStart") == sName )
					m_oNumStart = oReader;
				else if ( (L"w:pos") == sName )
					m_oPos = oReader;
			}
		}
		std::wstring CFtnProps::toXML() const
		{
			std::wstring sResult = (L"<w:footnotePr>");

			if ( m_oNumFmt.IsInit() )
			{
				sResult += (L"<w:numFmt ");
				sResult += m_oNumFmt->ToString();
				sResult += (L"/>");
			}

			if ( m_oNumRestart.IsInit() )
			{
				sResult += (L"<w:numRestart ");
				sResult += m_oNumRestart->ToString();
				sResult += (L"/>");
			}

			if ( m_oNumStart.IsInit() )
			{
				sResult += (L"<w:numStart ");
				sResult += m_oNumStart->ToString();
				sResult += (L"/>");
			}

			if ( m_oPos.IsInit() )
			{
				sResult += (L"<w:pos ");
				sResult += m_oPos->ToString();
				sResult += (L"/>");
			}

			sResult += (L"</w:footnotePr>");

			return sResult;
		}
		EElementType CFtnProps::getType() const
			{
				return et_w_footnotePr;
			}

		//--------------------------------------------------------------------------------
		// PageBorders 17.6.10 (Part 1)
		//--------------------------------------------------------------------------------

		CPageBorders::CPageBorders()
		{
		}
		CPageBorders::~CPageBorders()
		{
		}
		void CPageBorders::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, (L"w:display"),    m_oDisplay );
			XmlMacroReadAttributeBase( oNode, (L"w:offsetFrom"), m_oOffsetFrom );
			XmlMacroReadAttributeBase( oNode, (L"w:zOrder"),     m_oZOrder );

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( (L"w:bottom"), oChild ) )
				m_oBottom = oChild;

			if ( oNode.GetNode( (L"w:left"), oChild ) )
				m_oLeft = oChild;

			if ( oNode.GetNode( (L"w:right"), oChild ) )
				m_oRight = oChild;

			if ( oNode.GetNode( (L"w:top"), oChild ) )
				m_oTop = oChild;
		}
		void CPageBorders::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( (L"w:bottom") == sName )
					m_oBottom = oReader;
				else if ( (L"w:left") == sName )
					m_oLeft = oReader;
				else if ( (L"w:right") == sName )
					m_oRight = oReader;
				else if ( (L"w:top") == sName )
					m_oTop = oReader;
			}
		}
		std::wstring CPageBorders::toXML() const
		{
			std::wstring sResult = (L"<w:pgBorders ");

			if ( m_oDisplay.IsInit() )
			{
				sResult += (L"w:display=\"");
				sResult += m_oDisplay->ToString();
				sResult += (L"\" ");
			}

			if ( m_oOffsetFrom.IsInit() )
			{
				sResult += (L"w:offsetFrom=\"");
				sResult += m_oOffsetFrom->ToString();
				sResult += (L"\" ");
			}

			if ( m_oZOrder.IsInit() )
			{
				sResult += (L"w:zOrder=\"");
				sResult += m_oZOrder->ToString();
				sResult += (L"\" ");
			}

			sResult += (L">");

			if ( m_oBottom.IsInit() )
			{
				sResult += (L"<w:bottom ");
				sResult += m_oBottom->ToString();
				sResult += (L"/>");
			}

			if ( m_oLeft.IsInit() )
			{
				sResult += (L"<w:left ");
				sResult += m_oLeft->ToString();
				sResult += (L"/>");
			}

			if ( m_oRight.IsInit() )
			{
				sResult += (L"<w:right ");
				sResult += m_oRight->ToString();
				sResult += (L"/>");
			}

			if ( m_oTop.IsInit() )
			{
				sResult += (L"<w:top ");
				sResult += m_oTop->ToString();
				sResult += (L"/>");
			}

			sResult += (L"</w:pgBorders>");

			return sResult;
		}
		EElementType CPageBorders::getType() const
		{
			return et_w_pgBorders;
		}
		void CPageBorders::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:display"),    m_oDisplay )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:offsetFrom"), m_oOffsetFrom )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:zOrder"),     m_oZOrder )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// SectPrChange 17.13.5.32 (Part 1)
		//--------------------------------------------------------------------------------

		CSectPrChange::CSectPrChange()
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;
		}
		CSectPrChange::CSectPrChange(XmlUtils::CXmlNode& oNode)
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;

			fromXML( oNode );
		}
		CSectPrChange::CSectPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;

			fromXML( oReader );
		}
		CSectPrChange::~CSectPrChange()
		{
		}
		void CSectPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
            XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate );
            XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
            XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_sectPr;

			if ( m_pSecPr.IsInit() && oNode.GetNode( _T("w:sectPr"), oNode_sectPr ) )
				m_pSecPr->fromXML( oNode_sectPr );

		}
		void CSectPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("w:sectPr") == sName )
					m_pSecPr->fromXML( oReader );
			}
		}
		std::wstring CSectPrChange::toXML() const
		{			
			std::wstring sResult = _T("<w:sectPrChange ");

			if ( m_sAuthor.IsInit() )
			{
				sResult += _T("w:author=\"");
                sResult += m_sAuthor.get2();
				sResult += _T("\" ");
			}

			if ( m_oDate.IsInit() )
			{
				sResult += _T("w:date=\"");
				sResult += m_oDate->ToString();
				sResult += _T("\" ");
			}

			if ( m_oId.IsInit() )
			{
				sResult += _T("w:id=\"");
				sResult += m_oId->ToString();
				sResult += _T("\" ");
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += _T("oouserid=\"");
                sResult += m_sUserId.get2();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if ( m_pSecPr.IsInit() )
				sResult += m_pSecPr->toXML();

			sResult += _T("</w:sectPrChange>");

			return sResult;
		}
		EElementType CSectPrChange::getType() const
		{
			return et_w_sectPrChange;
		}
		void CSectPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// SectionProperty
		//--------------------------------------------------------------------------------

		CSectionProperty::CSectionProperty(OOX::Document *pMain) : WritingElement(pMain)
		{
			m_bSectPrChange = false;
		}
		CSectionProperty::CSectionProperty(XmlUtils::CXmlNode &oNode)
		{
			m_bSectPrChange = false;
			fromXML( oNode );
		}
		CSectionProperty::CSectionProperty(XmlUtils::CXmlLiteReader& oReader)
		{
			m_bSectPrChange = false;
			fromXML( oReader );
		}
		CSectionProperty::~CSectionProperty()
		{
			ClearItems();
		}
		void CSectionProperty::ClearItems()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrFooterReference.size(); nIndex++ )
			{
				if ( m_arrFooterReference[nIndex] ) delete m_arrFooterReference[nIndex];
				m_arrFooterReference[nIndex] = NULL;
			}
			m_arrFooterReference.clear();

			for ( unsigned int nIndex = 0; nIndex < m_arrHeaderReference.size(); nIndex++ )
			{
				if ( m_arrHeaderReference[nIndex] ) delete m_arrHeaderReference[nIndex];
				m_arrHeaderReference[nIndex] = NULL;
			}
			m_arrHeaderReference.clear();
		}
		const CSectionProperty& CSectionProperty::operator =(const XmlUtils::CXmlNode &oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CSectionProperty& CSectionProperty::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode &)oReader );
			return *this;
		}
		void CSectionProperty::fromXML(XmlUtils::CXmlNode &oNode)
		{
			if ( (L"w:sectPr") != oNode.GetName() )
				return;

			XmlMacroReadAttributeBase( oNode, (L"w:rsidDel"),  m_oRsidDel );
			XmlMacroReadAttributeBase( oNode, (L"w:rsidR"),    m_oRsidR );
			XmlMacroReadAttributeBase( oNode, (L"w:rsidRPr"),  m_oRsidRPr );
			XmlMacroReadAttributeBase( oNode, (L"w:rsidSect"), m_oRsidSect );

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( (L"w:bidi"), oChild ) )
				m_oBidi = oChild;

			if ( oNode.GetNode( (L"w:cols"), oChild ) )
				m_oCols = oChild;

			if ( oNode.GetNode( (L"w:docGrid"), oChild ) )
				m_oDocGrid = oChild;

			if ( oNode.GetNode( (L"w:endnotePr"), oChild ) )
				m_oEndnotePr = oChild;

			if ( !m_bSectPrChange )
			{
				XmlUtils::CXmlNodes oNodes;
				if ( oNode.GetNodes( (L"w:footerReference"), oNodes ) )
				{
					XmlUtils::CXmlNode oFooterNode;
					for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
					{
						if ( oNodes.GetAt( nIndex, oFooterNode ) )
						{
							ComplexTypes::Word::CHdrFtrRef *oFooter = new ComplexTypes::Word::CHdrFtrRef(oFooterNode);
							if (oFooter) m_arrFooterReference.push_back( oFooter );
						}
					}
				}
			}

			if ( oNode.GetNode( (L"w:footnotePr"), oChild ) )
				m_oFootnotePr = oChild;

			if ( oNode.GetNode( (L"w:formProt"), oChild ) )
				m_oFormProt = oChild;

			if ( !m_bSectPrChange )
			{
				XmlUtils::CXmlNodes oNodes;
				if ( oNode.GetNodes( L"w:headerReference", oNodes ) )
				{
					XmlUtils::CXmlNode oHeaderNode;
					for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
					{
						if ( oNodes.GetAt( nIndex, oHeaderNode ) )
						{
							ComplexTypes::Word::CHdrFtrRef *oHeader = new ComplexTypes::Word::CHdrFtrRef(oHeaderNode);
							if (oHeader) m_arrHeaderReference.push_back( oHeader );
						}
					}
				}
			}

			if ( oNode.GetNode( (L"w:lnNumType"), oChild ) )
				m_oLnNumType = oChild;

			if ( oNode.GetNode( (L"w:noEndnote"), oChild ) )
				m_oNoEndnote = oChild;

			if ( oNode.GetNode( (L"w:paperSrc"), oChild ) )
				m_oPaperSrc = oChild;

			if ( oNode.GetNode( (L"w:pgBorders"), oChild ) )
				m_oPgBorders = oChild;

			if ( oNode.GetNode( (L"w:pgMar"), oChild ) )
				m_oPgMar = oChild;

			if ( oNode.GetNode( (L"w:pgNumType"), oChild ) )
				m_oPgNumType = oChild;

			if ( oNode.GetNode( (L"w:pgSz"), oChild ) )
				m_oPgSz = oChild;

			if ( oNode.GetNode( (L"w:printerSettings"), oChild ) )
				m_oPrinterSettings = oChild;

			if ( oNode.GetNode( (L"w:rtlGutter"), oChild ) )
				m_oRtlGutter = oChild;

			if ( !m_bSectPrChange && oNode.GetNode( (L"w:sectPrChange"), oChild ) )
				m_oSectPrChange = oChild;

			if ( oNode.GetNode( (L"w:textDirection"), oChild ) )
				m_oTextDirection = oChild;

			if ( oNode.GetNode( (L"w:titlePg"), oChild ) )
				m_oTitlePg = oChild;

			if ( oNode.GetNode( (L"w:type"), oChild ) )
				m_oType = oChild;

			if ( oNode.GetNode( (L"w:vAlign"), oChild ) )
				m_oVAlign = oChild;
		}
		void CSectionProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;
			
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:bidi" == sName )
					m_oBidi = oReader;
				else if ( L"w:cols" == sName )
					m_oCols = oReader;
				else if ( L"w:docGrid" == sName )
					m_oDocGrid = oReader;
				else if ( L"w:endnotePr" == sName )
					m_oEndnotePr = oReader;
				else if ( !m_bSectPrChange && L"w:footerReference" == sName )
				{
					ComplexTypes::Word::CHdrFtrRef *oFooter = new ComplexTypes::Word::CHdrFtrRef(oReader);
					if (oFooter) m_arrFooterReference.push_back( oFooter );
				}
				else if ( L"w:footnotePr" == sName )
					m_oFootnotePr = oReader;
				else if ( L"w:formProt" == sName )
					m_oFormProt = oReader;
				else if ( !m_bSectPrChange && L"w:headerReference" == sName )
				{
					ComplexTypes::Word::CHdrFtrRef *oHeader = new ComplexTypes::Word::CHdrFtrRef( oReader);
					if (oHeader) m_arrHeaderReference.push_back( oHeader );
				}
				else if ( L"w:lnNumType" == sName )
					m_oLnNumType = oReader;
				else if ( L"w:noEndnote" == sName )
					m_oNoEndnote = oReader;
				else if ( (L"w:paperSrc") == sName )
					m_oPaperSrc = oReader;
				else if ( L"w:pgBorders" == sName )
					m_oPgBorders = oReader;
				else if ( L"w:pgMar" == sName )
					m_oPgMar = oReader;
				else if ( L"w:pgNumType" == sName )
					m_oPgNumType = oReader;
				else if ( L"w:pgSz" == sName )
					m_oPgSz = oReader;
				else if ( L"w:printerSettings" == sName )
					m_oPrinterSettings = oReader;
				else if ( L"w:rtlGutter" == sName )
					m_oRtlGutter = oReader;
				else if ( !m_bSectPrChange && L"w:sectPrChange" == sName )
					m_oSectPrChange = oReader;
				else if ( L"w:titlePg" == sName )
					m_oTitlePg = oReader;
				else if ( L"w:type" == sName )
					m_oType = oReader;
				else if ( L"w:vAlign" == sName )
					m_oVAlign = oReader;
				else if (L"w:textDirection" == sName || L"w:textFlow" == sName)
					m_oTextDirection = oReader;
				else if ( L"w:hdr" == sName )
				{
					CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(document);
					if (docx_flat)
					{
						ComplexTypes::Word::CHdrFtrRef *pHeaderRef = new ComplexTypes::Word::CHdrFtrRef();
						NSCommon::smart_ptr<OOX::CHdrFtr> pHeader = new OOX::CHdrFtr(document);

						if (pHeaderRef && pHeader.IsInit())
						{
							OOX::IFileContainer* oldContainer = docx_flat->m_currentContainer;
							docx_flat->m_currentContainer = dynamic_cast<OOX::IFileContainer*>(pHeader.GetPointer());
								pHeader->fromXML(oReader);
							docx_flat->m_currentContainer = oldContainer;
							
							NSCommon::smart_ptr<OOX::File> file = pHeader.smart_dynamic_cast<OOX::File>();
							OOX::RId rId = docx_flat->m_currentContainer->Add(file);

							pHeaderRef->m_oId = rId.get();
							pHeaderRef->m_oType = pHeader->m_oType;
							
							m_arrHeaderReference.push_back(pHeaderRef);
						}
					}
				}
				else if ( L"w:ftr" == sName )
				{
					CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(document);
					if (docx_flat)
					{
						ComplexTypes::Word::CHdrFtrRef *pFooterRef = new ComplexTypes::Word::CHdrFtrRef();
						NSCommon::smart_ptr<OOX::CHdrFtr> pFooter = new OOX::CHdrFtr(document);

						if (pFooter.IsInit() && pFooterRef)
						{
							OOX::IFileContainer* oldContainer = docx_flat->m_currentContainer;
							docx_flat->m_currentContainer = dynamic_cast<OOX::IFileContainer*>(pFooter.GetPointer());
								pFooter->fromXML(oReader);
							docx_flat->m_currentContainer = oldContainer;

							NSCommon::smart_ptr<OOX::File> file = pFooter.smart_dynamic_cast<OOX::File>();
							OOX::RId rId = docx_flat->m_currentContainer->Add(file);

							pFooterRef->m_oId = rId.get();
							pFooterRef->m_oType = pFooter->m_oType;
							
							m_arrFooterReference.push_back(pFooterRef);
						}
					}
				}
			}
		}
		std::wstring CSectionProperty::toXML() const
		{
			std::wstring sResult = (L"<w:sectPr");
				if (m_oRsidDel.IsInit())
				{
					sResult += L" w:rsidDel=\"" + m_oRsidDel->ToString() + L"\"";
				}
				if (m_oRsidR.IsInit())
				{
					sResult += L" w:rsidR=\"" + m_oRsidR->ToString() + L"\"";
				}
				if (m_oRsidRPr.IsInit())
				{
					sResult += L" w:rsidRPr=\"" + m_oRsidRPr->ToString() + L"\"";
				}
				if (m_oRsidSect.IsInit())
				{
					sResult += L" w:rsidSect=\"" + m_oRsidSect->ToString() + L"\"";
				}
			sResult += L">";

			if (!m_bSectPrChange)
			{
				for (size_t nIndex = 0; nIndex < m_arrHeaderReference.size(); nIndex++)
				{
					sResult += (L"<w:headerReference");
					if (m_arrHeaderReference[nIndex])
						sResult += m_arrHeaderReference[nIndex]->ToString();
					sResult += (L"/>");
				}
				for (size_t nIndex = 0; nIndex < m_arrFooterReference.size(); nIndex++)
				{
					sResult += (L"<w:footerReference");
					if (m_arrFooterReference[nIndex])
						sResult += m_arrFooterReference[nIndex]->ToString();
					sResult += (L"/>");
				}
			}
			if (!m_bSectPrChange && m_oSectPrChange.IsInit())
			{
				sResult += m_oSectPrChange->toXML();
			}
			if (m_oFootnotePr.IsInit())
			{
				sResult += m_oFootnotePr->toXML();
			}			
			if (m_oEndnotePr.IsInit())
			{
				sResult += m_oEndnotePr->toXML();
			}
			if (m_oType.IsInit())
			{
				sResult += L"<w:type " + m_oType->ToString() + L"/>";
			}
			if (m_oPgSz.IsInit())
			{
				sResult += L"<w:pgSz " + m_oPgSz->ToString() + L"/>";
			}
			if (m_oPgMar.IsInit())
			{
				sResult += L"<w:pgMar" + m_oPgMar->ToString() + L"/>";
			}
			if (m_oPaperSrc.IsInit())
			{
				sResult += L"<w:paperSrc " + m_oPaperSrc->ToString() + L"/>";
			}
			if (m_oPgBorders.IsInit())
			{
				sResult += m_oPgBorders->toXML();
			}
			if (m_oLnNumType.IsInit())
			{
				sResult += L"<w:lnNumType " + m_oLnNumType->ToString() + L"/>";
			}
			if (m_oPgNumType.IsInit())
			{
				sResult += L"<w:pgNumType " + m_oPgNumType->ToString() + L"/>";
			}
			if (m_oCols.IsInit())
			{
				sResult += m_oCols->toXML();
			}
			if (m_oFormProt.IsInit())
			{
				sResult += L"<w:formProt " + m_oFormProt->ToString() + L"/>";
			}
			if (m_oVAlign.IsInit())
			{
				sResult += L"<w:vAlign " + m_oVAlign->ToString() + L"/>";
			}
			if (m_oNoEndnote.IsInit())
			{
				sResult += L"<w:noEndnote " + m_oNoEndnote->ToString() + L"/>";
			}
			if (m_oTitlePg.IsInit() && m_oTitlePg->m_oVal.ToBool())
			{
				sResult += L"<w:titlePg/>";
			}
			if (m_oTextDirection.IsInit())
			{
				sResult += L"<w:textDirection " + m_oTextDirection->ToString() + L"/>";
			}
			if ( m_oBidi.IsInit() )
			{
				sResult += L"<w:bidi " + m_oBidi->ToString() + L"/>";
			}
			if (m_oRtlGutter.IsInit())
			{
				sResult += m_oRtlGutter->m_oVal.ToBool() ? L"<w:rtlGutter/>" : L"<w:rtlGutter w:val=\"0\"/>";
			}
			if ( m_oDocGrid.IsInit() )
			{
				sResult += L"<w:docGrid" + m_oDocGrid->ToString() + L"/>";
			}
			if ( m_oPrinterSettings.IsInit() )
			{
				sResult += L"<w:printerSettings " + m_oPrinterSettings->ToString() + L"/>";
			}
			sResult += L"</w:sectPr>";
			return sResult;
		}
		EElementType CSectionProperty::getType() const
		{
			return et_w_sectPr;
		}
		void CSectionProperty::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:rsidDel"),  m_oRsidDel )
			WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:rsidR"),    m_oRsidR )
			WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:rsidRPr"),  m_oRsidRPr )
			WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:rsidSect"), m_oRsidSect )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // Logic
} 
