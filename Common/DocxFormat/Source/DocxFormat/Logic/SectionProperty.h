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

#ifndef OOX_LOGIC_SECTOR_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_SECTOR_PROPERTY_INCLUDE_H_

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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"w:charSpace"), m_oCharSpace );
				XmlMacroReadAttributeBase( oNode, (L"w:linePitch"), m_oLinePitch );
				XmlMacroReadAttributeBase( oNode, (L"w:type"),      m_oType );
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

				if ( m_oCharSpace.IsInit() )
				{
					sResult += (L"w:charSpace=\"");
					sResult += m_oCharSpace->ToString();
					sResult += (L"\" ");
				}

				if ( m_oLinePitch.IsInit() )
				{
					sResult += (L"w:linePitch=\"");
					sResult += m_oLinePitch->ToString();
					sResult += (L"\" ");
				}

				if ( m_oType.IsInit() )
				{
					sResult += (L"w:type=\"");
					sResult += m_oType->ToString();
					sResult += (L"\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:charSpace"), m_oCharSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:linePitch"), m_oLinePitch )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:type"),      m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CDecimalNumber<> > m_oCharSpace;
			nullable<SimpleTypes::CDecimalNumber<> > m_oLinePitch;
			nullable<SimpleTypes::CDocGrid<>       > m_oType;
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, (L"r:id"),   m_oId );
				XmlMacroReadAttributeBase( oNode, (L"w:type"), m_oType );
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

				if ( m_oId.IsInit() )
				{
					sResult += (L"r:id=\"");
					sResult += m_oId->ToString();
					sResult += (L"\" ");
				}

				if ( m_oType.IsInit() )
				{
					sResult += (L"w:type=\"");
					sResult += m_oType->ToString();
					sResult += (L"\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, (L"r:id"),   m_oId )
					WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:type"), m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CRelationshipId > m_oId;
			nullable<SimpleTypes::CHdrFtr<>       > m_oType;
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
					sResult += m_oDistance->ToString();
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

			nullable<SimpleTypes::CDecimalNumber<>    > m_oCountBy;
			nullable<SimpleTypes::CTwipsMeasure       > m_oDistance;
			nullable<SimpleTypes::CLineNumberRestart<>> m_oRestart;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oStart;
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

			nullable<SimpleTypes::CDecimalNumber<> > m_oFirst;
			nullable<SimpleTypes::CDecimalNumber<> > m_oOther;
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
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:id"),          m_oId )
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

			nullable<SimpleTypes::CHexColor<>                     > m_oColor;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse> > m_oFrame;
			nullable<SimpleTypes::CRelationshipId                 > m_oId;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse> > m_oShadow;
			nullable<SimpleTypes::CPointMeasure<>                 > m_oSpace;
			nullable<SimpleTypes::CEighthPointMeasure<>           > m_oSz;
			nullable<SimpleTypes::CThemeColor<>                   > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber<>               > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<>               > m_oThemeTint;
			nullable<SimpleTypes::CBorder<>                       > m_oVal;
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
				XmlMacroReadAttributeBase( oNode, (L"r:bottomLeft"),  m_oBottomLeft );
				XmlMacroReadAttributeBase( oNode, (L"r:bottomRight"), m_oBottomRight );
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
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"r:bottomLeft"), m_oBottomLeft )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:bottomRight"), m_oBottomRight )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:id"),          m_oId )
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
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:id"),          m_oId )
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

				if ( m_oBottom.IsInit() )
				{
					sResult += (L"w:bottom=\"");
					sResult += m_oBottom->ToString();
					sResult += (L"\" ");
				}

				if ( m_oFooter.IsInit() )
				{
					sResult += (L"w:footer=\"");
					sResult += m_oFooter->ToString();
					sResult += (L"\" ");
				}
				if ( m_oGutter.IsInit() )
				{
					sResult += (L"w:gutter=\"");
					sResult += m_oGutter->ToString();
					sResult += (L"\" ");
				}

				if ( m_oHeader.IsInit() )
				{
					sResult += (L"w:header=\"");
					sResult += m_oHeader->ToString();
					sResult += (L"\" ");
				}

				if ( m_oLeft.IsInit() )
				{
					sResult += (L"w:left=\"");
					sResult += m_oLeft->ToString();
					sResult += (L"\" ");
				}

				if ( m_oRight.IsInit() )
				{
					sResult += (L"w:right=\"");
					sResult += m_oRight->ToString();
					sResult += (L"\" ");
				}
				if ( m_oTop.IsInit() )
				{
					sResult += (L"w:top=\"");
					sResult += m_oTop->ToString();
					sResult += (L"\" ");
				}

				return sResult;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
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

			nullable<SimpleTypes::CSignedTwipsMeasure  > m_oBottom;
			nullable<SimpleTypes::CTwipsMeasure        > m_oFooter;
			nullable<SimpleTypes::CTwipsMeasure        > m_oGutter;
			nullable<SimpleTypes::CTwipsMeasure        > m_oHeader;
			nullable<SimpleTypes::CTwipsMeasure        > m_oLeft;
			nullable<SimpleTypes::CTwipsMeasure        > m_oRight;
			nullable<SimpleTypes::CSignedTwipsMeasure  > m_oTop;
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

			nullable<SimpleTypes::CChapterSep<>    > m_oChapSep;
			nullable<SimpleTypes::CDecimalNumber<> > m_oChapStyle;
			nullable<SimpleTypes::CNumberFormat<>  > m_oFmt;
			nullable<SimpleTypes::CDecimalNumber<> > m_oStart;
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

			nullable<SimpleTypes::CDecimalNumber<>   > m_oCode;
			nullable<SimpleTypes::CTwipsMeasure      > m_oH;
			nullable<SimpleTypes::CPageOrientation<> > m_oOrient;
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

			nullable<SimpleTypes::CSectionMark<> > m_oVal;
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
			CColumns()
			{
			}
			virtual ~CColumns()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrColumns.size(); nIndex++ )
				{
					if ( m_arrColumns[nIndex] )	delete m_arrColumns[nIndex];
					m_arrColumns[nIndex] = NULL;
				}
				m_arrColumns.clear();
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
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
			virtual void  fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring      toXML() const
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

			virtual EElementType getType () const
			{
				return et_w_cols;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:equalWidth"), m_oEqualWidth )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:num"),        m_oNum )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:sep"),        m_oSep )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:space"),      m_oSpace )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::COnOff<>           > m_oEqualWidth;
			nullable<SimpleTypes::CDecimalNumber<>   > m_oNum;
			nullable<SimpleTypes::COnOff<>           > m_oSep;
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
			CEdnProps()
			{
			}
			virtual ~CEdnProps()
			{
			}
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
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

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring toXML() const
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

			virtual EElementType getType() const
			{
				return et_w_endnotePr;
			}
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
			CFtnProps()
			{
			}
			virtual ~CFtnProps()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
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

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring      toXML() const
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

			virtual EElementType getType() const
			{
				return et_w_footnotePr;
			}
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
			CPageBorders()
			{
			}
			virtual ~CPageBorders()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
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

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring      toXML() const
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

			virtual EElementType getType() const
			{
				return et_w_pgBorders;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:display"),    m_oDisplay )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:offsetFrom"), m_oOffsetFrom )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:zOrder"),     m_oZOrder )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CPageBorderDisplay<> > m_oDisplay;
			nullable<SimpleTypes::CPageBorderOffset<>  > m_oOffsetFrom;
			nullable<SimpleTypes::CPageBorderZOrder<>  > m_oZOrder;

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
            nullable<std::wstring                       > m_sAuthor;
			nullable<SimpleTypes::CDateTime        > m_oDate;
			nullable<SimpleTypes::CDecimalNumber<> > m_oId;
            nullable<std::wstring                       > m_sUserId;

			// Childs
			nullable<CSectionProperty>               m_pSecPr;
		};

		//--------------------------------------------------------------------------------
		// SectionProperty
		//--------------------------------------------------------------------------------
		class CSectionProperty : public WritingElement
		{
		public:
			CSectionProperty() 
			{
				m_bSectPrChange = false;
			}
			CSectionProperty(XmlUtils::CXmlNode &oNode)
			{
				m_bSectPrChange = false;
				fromXML( oNode );
			}
			CSectionProperty(XmlUtils::CXmlLiteReader& oReader)
			{
				m_bSectPrChange = false;
				fromXML( oReader );
			}
			virtual ~CSectionProperty() 
			{
				ClearItems();
			}
			virtual void ClearItems()
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
		public:
			const CSectionProperty& operator =(const XmlUtils::CXmlNode &oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode &)oNode );
				return *this;
			}

			const CSectionProperty& operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode &)oReader );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode &oNode)
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
					if ( oNode.GetNodes( (L"w:headerReference"), oNodes ) )
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
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( (L"w:bidi") == sName )
						m_oBidi = oReader;
					else if ( (L"w:cols") == sName )
						m_oCols = oReader;
					else if ( (L"w:docGrid") == sName )
						m_oDocGrid = oReader;
					else if ( (L"w:endnotePr") == sName )
						m_oEndnotePr = oReader;
					else if ( !m_bSectPrChange && (L"w:footerReference") == sName )
					{
						ComplexTypes::Word::CHdrFtrRef *oFooter = new ComplexTypes::Word::CHdrFtrRef(oReader);
						if (oFooter) m_arrFooterReference.push_back( oFooter );
					}
					else if ( (L"w:footnotePr") == sName )
						m_oFootnotePr = oReader;
					else if ( (L"w:formProt") == sName )
						m_oFormProt = oReader;
					else if ( !m_bSectPrChange && (L"w:headerReference") == sName )
					{
						ComplexTypes::Word::CHdrFtrRef *oHeader = new ComplexTypes::Word::CHdrFtrRef( oReader);
						if (oHeader) m_arrHeaderReference.push_back( oHeader );
					}
					else if ( (L"w:lnNumType") == sName )
						m_oLnNumType = oReader;
					else if ( (L"w:noEndnote") == sName )
						m_oNoEndnote = oReader;
					else if ( (L"w:paperSrc") == sName )
						m_oPaperSrc = oReader;
					else if ( (L"w:pgBorders") == sName )
						m_oPgBorders = oReader;
					else if ( (L"w:pgMar") == sName )
						m_oPgMar = oReader;
					else if ( (L"w:pgNumType") == sName )
						m_oPgNumType = oReader;
					else if ( (L"w:pgSz") == sName )
						m_oPgSz = oReader;
					else if ( (L"w:printerSettings") == sName )
						m_oPrinterSettings = oReader;
					else if ( (L"w:rtlGutter") == sName )
						m_oRtlGutter = oReader;
					else if ( !m_bSectPrChange && (L"w:sectPrChange") == sName )
						m_oSectPrChange = oReader;
					else if ( (L"w:textDirection") == sName )
						m_oTextDirection = oReader;
					else if ( (L"w:titlePg") == sName )
						m_oTitlePg = oReader;
					else if ( (L"w:type") == sName )
						m_oType = oReader;
					else if ( (L"w:vAlign") == sName )
						m_oVAlign = oReader;
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = (L"<w:sectPr ");

				if ( m_oRsidDel.IsInit() )
				{
					sResult += (L"w:rsidDel=\"");
					sResult += m_oRsidDel->ToString();
					sResult += (L"\" ");						
				}

				if ( m_oRsidR.IsInit() )
				{
					sResult += (L"w:rsidR=\"");
					sResult += m_oRsidR->ToString();
					sResult += (L"\" ");						
				}

				if ( m_oRsidRPr.IsInit() )
				{
					sResult += (L"w:rsidRPr=\"");
					sResult += m_oRsidRPr->ToString();
					sResult += (L"\" ");						
				}

				if ( m_oRsidSect.IsInit() )
				{
					sResult += (L"w:rsidSect=\"");
					sResult += m_oRsidSect->ToString();
					sResult += (L"\" ");						
				}

				sResult += (L">");

				if ( m_oBidi.IsInit() )
				{
					sResult += (L"<w:bidi ");
					sResult += m_oBidi->ToString();
					sResult += (L"/>");
				}

				if ( m_oCols.IsInit() )
					sResult += m_oCols->toXML();

				if ( m_oDocGrid.IsInit() )
				{
					sResult += (L"<w:docGrid ");
					sResult += m_oDocGrid->ToString();
					sResult += (L"/>");
				}

				if ( m_oEndnotePr.IsInit() )
					sResult += m_oEndnotePr->toXML();

				if ( !m_bSectPrChange )
				{
					for (unsigned int nIndex = 0; nIndex < m_arrFooterReference.size(); nIndex++ )
					{
						sResult += (L"<w:footerReference ");
						if (m_arrFooterReference[nIndex])
							sResult += m_arrFooterReference[nIndex]->ToString();
						sResult += (L"/>");
					}
				}

				if ( m_oFootnotePr.IsInit() )
					sResult += m_oFootnotePr->toXML();

				if ( m_oFormProt.IsInit() )
				{
					sResult += (L"<w:formProt ");
					sResult += m_oFormProt->ToString();
					sResult += (L"/>");
				}

				if ( !m_bSectPrChange )
				{
					for (unsigned int nIndex = 0; nIndex < m_arrHeaderReference.size(); nIndex++ )
					{
						sResult += (L"<w:headerReference ");
						if (m_arrHeaderReference[nIndex])
							sResult += m_arrHeaderReference[nIndex]->ToString();
						sResult += (L"/>");
					}
				}

				if ( m_oLnNumType.IsInit() )
				{
					sResult += (L"<w:lnNumType ");
					sResult += m_oLnNumType->ToString();
					sResult += (L"/>");
				}

				if ( m_oNoEndnote.IsInit() )
				{
					sResult += (L"<w:noEndnote ");
					sResult += m_oNoEndnote->ToString();
					sResult += (L"/>");
				}

				if ( m_oPaperSrc.IsInit() )
				{
					sResult += (L"<w:paperSrc ");
					sResult += m_oPaperSrc->ToString();
					sResult += (L"/>");
				}

				if ( m_oPgBorders.IsInit() )
					sResult += m_oPgBorders->toXML();

				if ( m_oPgMar.IsInit() )
				{
					sResult += (L"<w:pgMar ");
					sResult += m_oPgMar->ToString();
					sResult += (L"/>");
				}

				if ( m_oPgNumType.IsInit() )
				{
					sResult += (L"<w:pgNumType ");
					sResult += m_oPgNumType->ToString();
					sResult += (L"/>");
				}

				if ( m_oPgSz.IsInit() )
				{
					sResult += (L"<w:pgSz ");
					sResult += m_oPgSz->ToString();
					sResult += (L"/>");
				}

				if ( m_oPrinterSettings.IsInit() )
				{
					sResult += (L"<w:printerSettings ");
					sResult += m_oPrinterSettings->ToString();
					sResult += (L"/>");
				}

				if ( m_oRtlGutter.IsInit() )
				{
					sResult += (L"<w:rtlGutter ");
					sResult += m_oRtlGutter->ToString();
					sResult += (L"/>");
				}

				if ( !m_bSectPrChange && m_oSectPrChange.IsInit() )
					sResult += m_oSectPrChange->toXML();

				if ( m_oTextDirection.IsInit() )
				{
					sResult += (L"<w:textDirection ");
					sResult += m_oTextDirection->ToString();
					sResult += (L"/>");
				}

				if ( m_oTitlePg.IsInit() )
				{
					sResult += (L"<w:titlePg ");
					sResult += m_oTitlePg->ToString();
					sResult += (L"/>");
				}

				if ( m_oType.IsInit() )
				{
					sResult += (L"<w:type ");
					sResult += m_oType->ToString();
					sResult += (L"/>");
				}

				if ( m_oVAlign.IsInit() )
				{
					sResult += (L"<w:vAlign ");
					sResult += m_oVAlign->ToString();
					sResult += (L"/>");
				}

				sResult += (L"</w:sectPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_sectPr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"w:rsidDel"),  m_oRsidDel )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:rsidR"),    m_oRsidR )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:rsidRPr"),  m_oRsidRPr )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"w:rsidSect"), m_oRsidSect )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			bool                                                           m_bSectPrChange;

			// Attributes
			nullable<SimpleTypes::CLongHexNumber<>                       > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber<>                       > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber<>                       > m_oRsidRPr;
			nullable<SimpleTypes::CLongHexNumber<>                       > m_oRsidSect;

			// Child Elements
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oBidi;
			nullable<OOX::Logic::CColumns                                > m_oCols;
			nullable<ComplexTypes::Word::CDocGrid                        > m_oDocGrid;
			nullable<OOX::Logic::CEdnProps                               > m_oEndnotePr;
			std::vector<ComplexTypes::Word::CHdrFtrRef*                  > m_arrFooterReference;
			nullable<OOX::Logic::CFtnProps                               > m_oFootnotePr;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oFormProt;
			std::vector<ComplexTypes::Word::CHdrFtrRef*                  > m_arrHeaderReference;
			nullable<ComplexTypes::Word::CLineNumber                     > m_oLnNumType;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oNoEndnote;
			nullable<ComplexTypes::Word::CPaperSource                    > m_oPaperSrc;
			nullable<OOX::Logic::CPageBorders                            > m_oPgBorders;
			nullable<ComplexTypes::Word::CPageMar                        > m_oPgMar;
			nullable<ComplexTypes::Word::CPageNumber                     > m_oPgNumType;
			nullable<ComplexTypes::Word::CPageSz                         > m_oPgSz;
			nullable<ComplexTypes::Word::CRel                            > m_oPrinterSettings;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oRtlGutter;
			nullable<OOX::Logic::CSectPrChange                           > m_oSectPrChange;
			nullable<ComplexTypes::Word::CTextDirection                  > m_oTextDirection;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oTitlePg;
			nullable<ComplexTypes::Word::CSectType                       > m_oType;
			nullable<ComplexTypes::Word::CVerticalJc                     > m_oVAlign;
		};

	} // namespace Logic
}// namespace OOX

#endif // OOX_LOGIC_SECTOR_PROPERTY_INCLUDE_H_
