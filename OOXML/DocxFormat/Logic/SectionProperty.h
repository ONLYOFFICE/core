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

#include "../../Base/Nullable.h"

#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/ComplexTypes.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// Column 17.6.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CColumn : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CColumn)
			CColumn()
			{
			}
			virtual ~CColumn()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:space", m_oSpace );
				XmlMacroReadAttributeBase( oNode, L"w:w",     m_oW );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:space"), m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:w"),     m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CTwipsMeasure > m_oSpace;
			nullable<SimpleTypes::CTwipsMeasure > m_oW;
		};

		//--------------------------------------------------------------------------------
		// DocGrid 17.6.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CDocGrid : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDocGrid)
			CDocGrid()
			{
			}
			virtual ~CDocGrid()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:charSpace"), m_oCharSpace );
				XmlMacroReadAttributeBase( oNode, (L"w:linePitch"), m_oLinePitch );
				XmlMacroReadAttributeBase( oNode, (L"w:type"),      m_oType );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:charSpace"), m_oCharSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:linePitch"), m_oLinePitch )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:type"),      m_oType )
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"w:line-pitch"), m_oLinePitch)	//2003 xml
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CDecimalNumber>	m_oCharSpace;
			nullable<SimpleTypes::CDecimalNumber>	m_oLinePitch;
			nullable<SimpleTypes::CDocGrid>			m_oType;
		};

		//--------------------------------------------------------------------------------
		// HdrFtrRef 17.10.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CHdrFtrRef : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHdrFtrRef)
			CHdrFtrRef()
			{
			}
			virtual ~CHdrFtrRef()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{//todooo переделать на перебор всех и без неймспейсов
				XmlMacroReadAttributeBase( oNode, L"r:id",   m_oId );
				XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );
				
				if (false == m_oId.IsInit())
				{
					XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
				}
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, (L"id"),	m_oId )
					WritingElement_ReadAttributes_Read_else_if( oReader, (L"type"),	m_oType )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}

		public:

			nullable<SimpleTypes::CRelationshipId>		m_oId;
			nullable<SimpleTypes::CHdrFtr>				m_oType;
		};

		//--------------------------------------------------------------------------------
		// LineNumber 17.6.8 (Part 1)
		//--------------------------------------------------------------------------------
		class CLineNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLineNumber)
			CLineNumber()
			{
			}
			virtual ~CLineNumber()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:countBy"),  m_oCountBy );
				XmlMacroReadAttributeBase( oNode, (L"w:distance"), m_oDistance );
				XmlMacroReadAttributeBase( oNode, (L"w:restart"),  m_oRestart );
				XmlMacroReadAttributeBase( oNode, (L"w:start"),    m_oStart );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:countBy"),  m_oCountBy )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:distance"), m_oDistance )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:restart"),  m_oRestart )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:start"),    m_oStart )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CDecimalNumber      > m_oCountBy;
			nullable<SimpleTypes::CTwipsMeasure       > m_oDistance;
			nullable<SimpleTypes::CLineNumberRestart  > m_oRestart;
			nullable<SimpleTypes::CDecimalNumber      > m_oStart;
		};

		//--------------------------------------------------------------------------------
		// PaperSource 17.6.9 (Part 1)
		//--------------------------------------------------------------------------------
		class CPaperSource : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPaperSource)
			CPaperSource()
			{
			}
			virtual ~CPaperSource()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:first"), m_oFirst );
				XmlMacroReadAttributeBase( oNode, (L"w:other"), m_oOther );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:first"), m_oFirst )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:other"), m_oOther )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CDecimalNumber> m_oFirst;
			nullable<SimpleTypes::CDecimalNumber> m_oOther;
		};

		//--------------------------------------------------------------------------------
		// PageBorder 17.6.7 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageBorder : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageBorder)
			CPageBorder()
			{
			}
			virtual ~CPageBorder()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
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
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			nullable<SimpleTypes::CHexColor                       > m_oColor;
			nullable<SimpleTypes::COnOff                          > m_oFrame;
			nullable<SimpleTypes::CRelationshipId                 > m_oId;
			nullable<SimpleTypes::COnOff                          > m_oShadow;
			nullable<SimpleTypes::CPointMeasure                   > m_oSpace;
			nullable<SimpleTypes::CEighthPointMeasure             > m_oSz;
			nullable<SimpleTypes::CThemeColor                     > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber                 > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber                 > m_oThemeTint;
			nullable<SimpleTypes::CBorder                         > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// BottomPageBorder 17.6.2 (Part 1)
		//--------------------------------------------------------------------------------
		class CBottomPageBorder : public CPageBorder
		{
		public:
			ComplexTypes_AdditionConstructors(CBottomPageBorder)
			CBottomPageBorder()
			{
			}
			virtual ~CBottomPageBorder()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
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
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			nullable<SimpleTypes::CRelationshipId                 > m_oBottomLeft;
			nullable<SimpleTypes::CRelationshipId                 > m_oBottomRight;
		};

		//--------------------------------------------------------------------------------
		// TopPageBorder 17.6.7 (Part 1)
		//--------------------------------------------------------------------------------
		class CTopPageBorder : public CPageBorder
		{
		public:
			ComplexTypes_AdditionConstructors(CTopPageBorder)
			CTopPageBorder()
			{
			}
			virtual ~CTopPageBorder()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
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
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			nullable<SimpleTypes::CRelationshipId                 > m_oTopLeft;
			nullable<SimpleTypes::CRelationshipId                 > m_oTopRight;
		};

		//--------------------------------------------------------------------------------
		// PageMar 17.6.11 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageMar : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageMar)
			CPageMar()
			{
			}
			virtual ~CPageMar()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:bottom"),  m_oBottom );
				XmlMacroReadAttributeBase( oNode, (L"w:footer"),  m_oFooter );
				XmlMacroReadAttributeBase( oNode, (L"w:gutter"),  m_oGutter );
				XmlMacroReadAttributeBase( oNode, (L"w:header"),  m_oHeader );
				XmlMacroReadAttributeBase( oNode, (L"w:left"),    m_oLeft   );
				XmlMacroReadAttributeBase( oNode, (L"w:right"),   m_oRight  );
				XmlMacroReadAttributeBase( oNode, (L"w:top"),     m_oTop    );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
		public:
			nullable<SimpleTypes::CSignedTwipsMeasure> m_oBottom;
			nullable<SimpleTypes::CTwipsMeasure> m_oFooter;
			nullable<SimpleTypes::CTwipsMeasure> m_oGutter;
			nullable<SimpleTypes::CTwipsMeasure> m_oHeader;
			nullable<SimpleTypes::CTwipsMeasure> m_oLeft;
			nullable<SimpleTypes::CTwipsMeasure> m_oRight;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// PageNumber 17.6.12 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageNumber)
			CPageNumber()
			{
			}
			virtual ~CPageNumber()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:chapSep"),   m_oChapSep );
				XmlMacroReadAttributeBase( oNode, (L"w:chapStyle"), m_oChapStyle );
				XmlMacroReadAttributeBase( oNode, (L"w:fmt"),       m_oFmt );
				XmlMacroReadAttributeBase( oNode, (L"w:start"),     m_oStart );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:chapSep"),   m_oChapSep )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:chapStyle"), m_oChapStyle )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:fmt"),       m_oFmt )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:start"),     m_oStart )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CChapterSep      > m_oChapSep;
			nullable<SimpleTypes::CDecimalNumber   > m_oChapStyle;
			nullable<SimpleTypes::CNumberFormat    > m_oFmt;
			nullable<SimpleTypes::CDecimalNumber   > m_oStart;
		};

		//--------------------------------------------------------------------------------
		// PageSz 17.6.13 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageSz : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageSz)
			CPageSz()
			{
			}
			virtual ~CPageSz()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:code"),   m_oCode );
				XmlMacroReadAttributeBase( oNode, (L"w:h"),      m_oH );
				XmlMacroReadAttributeBase( oNode, (L"w:orient"), m_oOrient );
				XmlMacroReadAttributeBase( oNode, (L"w:w"),      m_oW );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:code"),   m_oCode )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:h"),      m_oH )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:orient"), m_oOrient )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:w"),      m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CDecimalNumber     > m_oCode;
			nullable<SimpleTypes::CTwipsMeasure      > m_oH;
			nullable<SimpleTypes::CPageOrientation   > m_oOrient;
			nullable<SimpleTypes::CTwipsMeasure      > m_oW;
		};

		//--------------------------------------------------------------------------------
		// SectType 17.6.22 (Part 1)
		//--------------------------------------------------------------------------------
		class CSectType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSectType)
			CSectType()
			{
			}
			virtual ~CSectType()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
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
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, (L"w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CSectionMark> m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Columns 17.6.4 (Part 1)
		//--------------------------------------------------------------------------------
		class CColumns : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColumns)
			CColumns();
			virtual ~CColumns();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff             > m_oEqualWidth;
			nullable<SimpleTypes::CDecimalNumber     > m_oNum;
			nullable<SimpleTypes::COnOff             > m_oSep;
			nullable<SimpleTypes::CTwipsMeasure      > m_oSpace;

			std::vector<ComplexTypes::Word::CColumn *> m_arrColumns;
		};

		//--------------------------------------------------------------------------------
		// EdnProps 17.11.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CEdnProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEdnProps)
			CEdnProps();
			virtual ~CEdnProps();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::CNumFmt        > m_oNumFmt;
			nullable<ComplexTypes::Word::CNumRestart    > m_oNumRestart;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumStart;
			nullable<ComplexTypes::Word::CEdnPos        > m_oPos;
		};

		//--------------------------------------------------------------------------------
		// FtnProps 17.11.11 (Part 1)
		//--------------------------------------------------------------------------------
		class CFtnProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFtnProps)
			CFtnProps();
			virtual ~CFtnProps();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::CNumFmt        > m_oNumFmt;
			nullable<ComplexTypes::Word::CNumRestart    > m_oNumRestart;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumStart;
			nullable<ComplexTypes::Word::CFtnPos        > m_oPos;
		};

		//--------------------------------------------------------------------------------
		// PageBorders 17.6.10 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageBorders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageBorders)
			CPageBorders();
			virtual ~CPageBorders();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CPageBorderDisplay > m_oDisplay;
			nullable<SimpleTypes::CPageBorderOffset  > m_oOffsetFrom;
			nullable<SimpleTypes::CPageBorderZOrder  > m_oZOrder;

			// Childs
			nullable<ComplexTypes::Word::CBottomPageBorder > m_oBottom;
			nullable<ComplexTypes::Word::CPageBorder       > m_oLeft;
			nullable<ComplexTypes::Word::CPageBorder       > m_oRight;
			nullable<ComplexTypes::Word::CTopPageBorder    > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// SectPrChange 17.13.5.32 (Part 1)
		//--------------------------------------------------------------------------------
		class CSectionProperty;

		class CSectPrChange : public WritingElement
		{
		public:
			CSectPrChange();
			CSectPrChange(XmlUtils::CXmlNode &oNode);
			CSectPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CSectPrChange();
			virtual void    fromXML(XmlUtils::CXmlNode& oNode);
			virtual void    fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<std::wstring                  > m_sAuthor;
			nullable<SimpleTypes::CDateTime        > m_oDate;
			nullable<SimpleTypes::CDecimalNumber   > m_oId;
			nullable<std::wstring                  > m_sUserId;

			// Childs
			nullable<CSectionProperty>               m_pSecPr;
		};

		//--------------------------------------------------------------------------------
		// SectionProperty
		//--------------------------------------------------------------------------------
		class CSectionProperty : public WritingElement
		{
		public:
			CSectionProperty(OOX::Document *pMain = NULL);
			CSectionProperty(XmlUtils::CXmlNode &oNode);
			CSectionProperty(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CSectionProperty();

			virtual void ClearItems();
			const CSectionProperty& operator =(const XmlUtils::CXmlNode &oNode);
			const CSectionProperty& operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode &oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			bool m_bSectPrChange;

			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidRPr;
			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidSect;

			nullable<ComplexTypes::Word::COnOff2>		m_oBidi;
			nullable<OOX::Logic::CColumns>										m_oCols;
			nullable<ComplexTypes::Word::CDocGrid>								m_oDocGrid;
			nullable<OOX::Logic::CEdnProps>										m_oEndnotePr;
			std::vector<ComplexTypes::Word::CHdrFtrRef*>						m_arrFooterReference;
			nullable<OOX::Logic::CFtnProps>										m_oFootnotePr;
			nullable<ComplexTypes::Word::COnOff2>		m_oFormProt;
			std::vector<ComplexTypes::Word::CHdrFtrRef*>						m_arrHeaderReference;
			nullable<ComplexTypes::Word::CLineNumber>							m_oLnNumType;
			nullable<ComplexTypes::Word::COnOff2>		m_oNoEndnote;
			nullable<ComplexTypes::Word::CPaperSource>							m_oPaperSrc;
			nullable<OOX::Logic::CPageBorders>									m_oPgBorders;
			nullable<ComplexTypes::Word::CPageMar>								m_oPgMar;
			nullable<ComplexTypes::Word::CPageNumber>							m_oPgNumType;
			nullable<ComplexTypes::Word::CPageSz>								m_oPgSz;
			nullable<ComplexTypes::Word::CRel>									m_oPrinterSettings;
			nullable<ComplexTypes::Word::COnOff2>		m_oRtlGutter;
			nullable<OOX::Logic::CSectPrChange>									m_oSectPrChange;
			nullable<ComplexTypes::Word::CTextDirection>						m_oTextDirection;
			nullable<ComplexTypes::Word::COnOff2 >		m_oTitlePg;
			nullable<ComplexTypes::Word::CSectType>								m_oType;
			nullable<ComplexTypes::Word::CVerticalJc>							m_oVAlign;
		};

	} // namespace Logic
}// namespace OOX

