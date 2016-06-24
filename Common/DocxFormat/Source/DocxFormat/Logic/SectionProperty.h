/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
				oNode.ReadAttributeBase( _T("w:space"), m_oSpace );
				oNode.ReadAttributeBase( _T("w:w"),     m_oW );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oW.IsInit() )
				{
					sResult += _T("w:w=\"");
					sResult.AppendFormat(_T("%d"), m_oW->ToTwips());
					sResult += _T("\" ");
				}

				if ( m_oSpace.IsInit() )
				{
					sResult += _T("w:space=\"");
					sResult.AppendFormat(_T("%d"), m_oSpace->ToTwips());
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:space"), m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:w"),     m_oW )
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
				oNode.ReadAttributeBase( _T("w:charSpace"), m_oCharSpace );
				oNode.ReadAttributeBase( _T("w:linePitch"), m_oLinePitch );
				oNode.ReadAttributeBase( _T("w:type"),      m_oType );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oCharSpace.IsInit() )
				{
					sResult += _T("w:charSpace=\"");
					sResult += m_oCharSpace->ToString();
					sResult += _T("\" ");
				}

				if ( m_oLinePitch.IsInit() )
				{
					sResult += _T("w:linePitch=\"");
					sResult += m_oLinePitch->ToString();
					sResult += _T("\" ");
				}

				if ( m_oType.IsInit() )
				{
					sResult += _T("w:type=\"");
					sResult += m_oType->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:charSpace"), m_oCharSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:linePitch"), m_oLinePitch )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:type"),      m_oType )
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
				oNode.ReadAttributeBase( _T("r:id"),   m_oId );
				oNode.ReadAttributeBase( _T("w:type"), m_oType );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oId.IsInit() )
				{
					sResult += _T("r:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				if ( m_oType.IsInit() )
				{
					sResult += _T("w:type=\"");
					sResult += m_oType->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),   m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:type"), m_oType )
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
				oNode.ReadAttributeBase( _T("w:countBy"),  m_oCountBy );
				oNode.ReadAttributeBase( _T("w:distance"), m_oDistance );
				oNode.ReadAttributeBase( _T("w:restart"),  m_oRestart );
				oNode.ReadAttributeBase( _T("w:start"),    m_oStart );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oCountBy.IsInit() )
				{
					sResult += _T("w:countBy=\"");
					sResult += m_oCountBy->ToString();
					sResult += _T("\" ");
				}

				if ( m_oDistance.IsInit() )
				{
					sResult += _T("w:distance=\"");
					sResult += m_oDistance->ToString();
					sResult += _T("\" ");
				}

				if ( m_oRestart.IsInit() )
				{
					sResult += _T("w:restart=\"");
					sResult += m_oRestart->ToString();
					sResult += _T("\" ");
				}

				if ( m_oStart.IsInit() )
				{
					sResult += _T("w:start=\"");
					sResult += m_oStart->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:countBy"),  m_oCountBy )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:distance"), m_oDistance )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:restart"),  m_oRestart )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:start"),    m_oStart )
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
				oNode.ReadAttributeBase( _T("w:first"), m_oFirst );
				oNode.ReadAttributeBase( _T("w:other"), m_oOther );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oFirst.IsInit() )
				{
					sResult += _T("w:first=\"");
					sResult += m_oFirst->ToString();
					sResult += _T("\" ");
				}

				if ( m_oOther.IsInit() )
				{
					sResult += _T("w:other=\"");
					sResult += m_oOther->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:first"), m_oFirst )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:other"), m_oOther )
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
				oNode.ReadAttributeBase( _T("w:color"),       m_oColor );
				oNode.ReadAttributeBase( _T("w:frame"),       m_oFrame );
				oNode.ReadAttributeBase( _T("r:id"),          m_oId );
				oNode.ReadAttributeBase( _T("w:shadow"),      m_oShadow );
				oNode.ReadAttributeBase( _T("w:space"),       m_oSpace );
				oNode.ReadAttributeBase( _T("w:sz"),          m_oSz );
				oNode.ReadAttributeBase( _T("w:themeColor"),  m_oThemeColor );
				oNode.ReadAttributeBase( _T("w:themeShade"),  m_oThemeShade );
				oNode.ReadAttributeBase( _T("w:themeTint"),   m_oThemeTint );
				oNode.ReadAttributeBase( _T("w:val"),         m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oColor.IsInit() )
				{
					sResult += _T("w:color=\"");
					sResult += m_oColor->ToStringNoAlpha();
					sResult += _T("\" ");
				}

				if ( m_oFrame.IsInit() )
				{
					sResult += _T("w:frame=\"");
					sResult += m_oFrame->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}
				if ( m_oId.IsInit() )
				{
					sResult += _T("r:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				if ( m_oShadow.IsInit() )
				{
					sResult += _T("w:shadow=\"");
					sResult += m_oShadow->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}

				if ( m_oSpace.IsInit() )
				{
					sResult += _T("w:space=\"");
					sResult += m_oSpace->ToString();
					sResult += _T("\" ");
				}

				if ( m_oSz.IsInit() )
				{
					sResult += _T("w:sz=\"");
					sResult += m_oSz->ToString();
					sResult += _T("\" ");
				}
				if ( m_oThemeColor.IsInit() )
				{
					sResult += _T("w:themeColor=\"");
					sResult += m_oThemeColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeShade.IsInit() )
				{
					sResult += _T("w:themeShade=\"");
					sResult += m_oThemeShade->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeTint.IsInit() )
				{
					sResult += _T("w:themeTint=\"");
					sResult += m_oThemeTint->ToString();
					sResult += _T("\" ");
				}

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:id"),          m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:shadow"),      m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:space"),       m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:sz"),          m_oSz )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeColor"),  m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeShade"),  m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeTint"),   m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),         m_oVal )
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
				oNode.ReadAttributeBase( _T("r:bottomLeft"),  m_oBottomLeft );
				oNode.ReadAttributeBase( _T("r:bottomRight"), m_oBottomRight );
				oNode.ReadAttributeBase( _T("w:color"),       m_oColor );
				oNode.ReadAttributeBase( _T("w:frame"),       m_oFrame );
				oNode.ReadAttributeBase( _T("r:id"),          m_oId );
				oNode.ReadAttributeBase( _T("w:shadow"),      m_oShadow );
				oNode.ReadAttributeBase( _T("w:space"),       m_oSpace );
				oNode.ReadAttributeBase( _T("w:sz"),          m_oSz );
				oNode.ReadAttributeBase( _T("w:themeColor"),  m_oThemeColor );
				oNode.ReadAttributeBase( _T("w:themeShade"),  m_oThemeShade );
				oNode.ReadAttributeBase( _T("w:themeTint"),   m_oThemeTint );
				oNode.ReadAttributeBase( _T("w:val"),         m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oBottomLeft.IsInit() )
				{
					sResult += _T("r:bottomLeft=\"");
					sResult += m_oBottomLeft->ToString();
					sResult += _T("\" ");
				}

				if ( m_oBottomRight.IsInit() )
				{
					sResult += _T("r:bottomRight=\"");
					sResult += m_oBottomRight->ToString();
					sResult += _T("\" ");
				}

				if ( m_oColor.IsInit() )
				{
					sResult += _T("w:color=\"");
					sResult += m_oColor->ToStringNoAlpha();
					sResult += _T("\" ");
				}

				if ( m_oFrame.IsInit() )
				{
					sResult += _T("w:frame=\"");
					sResult += m_oFrame->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}
				if ( m_oId.IsInit() )
				{
					sResult += _T("r:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				if ( m_oShadow.IsInit() )
				{
					sResult += _T("w:shadow=\"");
					sResult += m_oShadow->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}

				if ( m_oSpace.IsInit() )
				{
					sResult += _T("w:space=\"");
					sResult += m_oSpace->ToString();
					sResult += _T("\" ");
				}

				if ( m_oSz.IsInit() )
				{
					sResult += _T("w:sz=\"");
					sResult += m_oSz->ToString();
					sResult += _T("\" ");
				}
				if ( m_oThemeColor.IsInit() )
				{
					sResult += _T("w:themeColor=\"");
					sResult += m_oThemeColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeShade.IsInit() )
				{
					sResult += _T("w:themeShade=\"");
					sResult += m_oThemeShade->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeTint.IsInit() )
				{
					sResult += _T("w:themeTint=\"");
					sResult += m_oThemeTint->ToString();
					sResult += _T("\" ");
				}

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:bottomLeft"), m_oBottomLeft )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:bottomRight"), m_oBottomRight )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:id"),          m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:shadow"),      m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:space"),       m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:sz"),          m_oSz )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeColor"),  m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeShade"),  m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeTint"),   m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),         m_oVal )
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
				oNode.ReadAttributeBase( _T("w:color"),       m_oColor );
				oNode.ReadAttributeBase( _T("w:frame"),       m_oFrame );
				oNode.ReadAttributeBase( _T("r:id"),          m_oId );
				oNode.ReadAttributeBase( _T("w:shadow"),      m_oShadow );
				oNode.ReadAttributeBase( _T("w:space"),       m_oSpace );
				oNode.ReadAttributeBase( _T("w:sz"),          m_oSz );
				oNode.ReadAttributeBase( _T("w:themeColor"),  m_oThemeColor );
				oNode.ReadAttributeBase( _T("w:themeShade"),  m_oThemeShade );
				oNode.ReadAttributeBase( _T("w:themeTint"),   m_oThemeTint );
				oNode.ReadAttributeBase( _T("r:topLeft"),     m_oTopLeft );
				oNode.ReadAttributeBase( _T("r:topRight"),    m_oTopRight );
				oNode.ReadAttributeBase( _T("w:val"),         m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oColor.IsInit() )
				{
					sResult += _T("w:color=\"");
					sResult += m_oColor->ToStringNoAlpha();
					sResult += _T("\" ");
				}

				if ( m_oFrame.IsInit() )
				{
					sResult += _T("w:frame=\"");
					sResult += m_oFrame->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}
				if ( m_oId.IsInit() )
				{
					sResult += _T("r:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				if ( m_oShadow.IsInit() )
				{
					sResult += _T("w:shadow=\"");
					sResult += m_oShadow->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}

				if ( m_oSpace.IsInit() )
				{
					sResult += _T("w:space=\"");
					sResult += m_oSpace->ToString();
					sResult += _T("\" ");
				}

				if ( m_oSz.IsInit() )
				{
					sResult += _T("w:sz=\"");
					sResult += m_oSz->ToString();
					sResult += _T("\" ");
				}
				if ( m_oThemeColor.IsInit() )
				{
					sResult += _T("w:themeColor=\"");
					sResult += m_oThemeColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeShade.IsInit() )
				{
					sResult += _T("w:themeShade=\"");
					sResult += m_oThemeShade->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeTint.IsInit() )
				{
					sResult += _T("w:themeTint=\"");
					sResult += m_oThemeTint->ToString();
					sResult += _T("\" ");
				}

				if ( m_oTopLeft.IsInit() )
				{
					sResult += _T("r:topLeft=\"");
					sResult += m_oTopLeft->ToString();
					sResult += _T("\" ");
				}

				if ( m_oTopRight.IsInit() )
				{
					sResult += _T("r:topRight=\"");
					sResult += m_oTopRight->ToString();
					sResult += _T("\" ");
				}

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:color"),       m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:frame"),       m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:id"),          m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:shadow"),      m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:space"),       m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:sz"),          m_oSz )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeColor"),  m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeShade"),  m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeTint"),   m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:topLeft"),     m_oTopLeft )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:topRight"),    m_oTopRight )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),         m_oVal )
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
				oNode.ReadAttributeBase( _T("w:bottom"),  m_oBottom );
				oNode.ReadAttributeBase( _T("w:footer"),  m_oFooter );
				oNode.ReadAttributeBase( _T("w:gutter"),  m_oGutter );
				oNode.ReadAttributeBase( _T("w:header"),  m_oHeader );
				oNode.ReadAttributeBase( _T("w:left"),    m_oLeft   );
				oNode.ReadAttributeBase( _T("w:right"),   m_oRight  );
				oNode.ReadAttributeBase( _T("w:top"),     m_oTop    );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oBottom.IsInit() )
				{
					sResult += _T("w:bottom=\"");
					sResult += m_oBottom->ToString();
					sResult += _T("\" ");
				}

				if ( m_oFooter.IsInit() )
				{
					sResult += _T("w:footer=\"");
					sResult += m_oFooter->ToString();
					sResult += _T("\" ");
				}
				if ( m_oGutter.IsInit() )
				{
					sResult += _T("w:gutter=\"");
					sResult += m_oGutter->ToString();
					sResult += _T("\" ");
				}

				if ( m_oHeader.IsInit() )
				{
					sResult += _T("w:header=\"");
					sResult += m_oHeader->ToString();
					sResult += _T("\" ");
				}

				if ( m_oLeft.IsInit() )
				{
					sResult += _T("w:left=\"");
					sResult += m_oLeft->ToString();
					sResult += _T("\" ");
				}

				if ( m_oRight.IsInit() )
				{
					sResult += _T("w:right=\"");
					sResult += m_oRight->ToString();
					sResult += _T("\" ");
				}
				if ( m_oTop.IsInit() )
				{
					sResult += _T("w:top=\"");
					sResult += m_oTop->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:bottom"), m_oBottom )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:footer"), m_oFooter )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:gutter"), m_oGutter )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:header"), m_oHeader )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:left"),   m_oLeft )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:right"),  m_oRight )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:top"),    m_oTop )
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
				oNode.ReadAttributeBase( _T("w:chapSep"),   m_oChapSep );
				oNode.ReadAttributeBase( _T("w:chapStyle"), m_oChapStyle );
				oNode.ReadAttributeBase( _T("w:fmt"),       m_oFmt );
				oNode.ReadAttributeBase( _T("w:start"),     m_oStart );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oChapSep.IsInit() )
				{
					sResult += _T("w:chapSep=\"");
					sResult += m_oChapSep->ToString();
					sResult += _T("\" ");
				}

				if ( m_oChapStyle.IsInit() )
				{
					sResult += _T("w:chapStyle=\"");
					sResult += m_oChapStyle->ToString();
					sResult += _T("\" ");
				}
				if ( m_oFmt.IsInit() )
				{
					sResult += _T("w:fmt=\"");
					sResult += m_oFmt->ToString();
					sResult += _T("\" ");
				}
				if ( m_oStart.IsInit() )
				{
					sResult += _T("w:start=\"");
					sResult += m_oStart->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:chapSep"),   m_oChapSep )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:chapStyle"), m_oChapStyle )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:fmt"),       m_oFmt )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:start"),     m_oStart )
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
				oNode.ReadAttributeBase( _T("w:code"),   m_oCode );
				oNode.ReadAttributeBase( _T("w:h"),      m_oH );
				oNode.ReadAttributeBase( _T("w:orient"), m_oOrient );
				oNode.ReadAttributeBase( _T("w:w"),      m_oW );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oCode.IsInit() )
				{
					sResult += _T("w:code=\"");
					sResult += m_oCode->ToString();
					sResult += _T("\" ");
				}

				if ( m_oH.IsInit() )
				{
					sResult += _T("w:h=\"");
					sResult += m_oH->ToString();
					sResult += _T("\" ");
				}
				if ( m_oOrient.IsInit() )
				{
					sResult += _T("w:orient=\"");
					sResult += m_oOrient->ToString();
					sResult += _T("\" ");
				}
				if ( m_oW.IsInit() )
				{
					sResult += _T("w:w=\"");
					sResult += m_oW->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:code"),   m_oCode )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:h"),      m_oH )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:orient"), m_oOrient )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:w"),      m_oW )
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
				oNode.ReadAttributeBase( _T("w:val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
				oNode.ReadAttributeBase( _T("w:equalWidth"), m_oEqualWidth );
				oNode.ReadAttributeBase( _T("w:num"),        m_oNum );
				oNode.ReadAttributeBase( _T("w:sep"),        m_oSep );
				oNode.ReadAttributeBase( _T("w:space"),      m_oSpace );

				XmlUtils::CXmlNodes oCols;

				if ( oNode.GetNodes( _T("w:col"), oCols ) )
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
					CWCharWrapper sName = oReader.GetName();
					if ( _T("w:col") == sName )
					{
						ComplexTypes::Word::CColumn *oColumn = new ComplexTypes::Word::CColumn(oReader);
						if (oColumn) m_arrColumns.push_back( oColumn );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:cols ");

				if ( m_oNum.IsInit() )
				{
					sResult += _T("w:num=\"");
					sResult += m_oNum->ToString();
					sResult += _T("\" ");
				}

				if ( m_oSep.IsInit() )
				{
					sResult += _T("w:sep=\"");
					sResult += m_oSep->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}

				if ( m_oSpace.IsInit() )
				{
					sResult += _T("w:space=\"");
					sResult.AppendFormat(_T("%d"), m_oSpace->ToTwips());
					sResult += _T("\" ");
				}

				if ( m_oEqualWidth.IsInit() )
				{
					sResult += _T("w:equalWidth=\"");
					sResult += m_oEqualWidth->ToString2(SimpleTypes::onofftostring1);
					sResult += _T("\" ");
				}

				sResult += _T(">");

				for ( unsigned int nIndex = 0; nIndex < m_arrColumns.size(); nIndex++ )
				{
					sResult += _T("<w:col ");
					if (m_arrColumns[nIndex])
						sResult += m_arrColumns[nIndex]->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:cols>");

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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:equalWidth"), m_oEqualWidth )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:num"),        m_oNum )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:sep"),        m_oSep )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:space"),      m_oSpace )
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

				if ( oNode.GetNode( _T("w:numFmt"), oChild ) )
					m_oNumFmt = oChild;

				if ( oNode.GetNode( _T("w:numRestart"), oChild ) )
					m_oNumRestart = oChild;

				if ( oNode.GetNode( _T("w:numStart"), oChild ) )
					m_oNumStart = oChild;

				if ( oNode.GetNode( _T("w:pos"), oChild ) )
					m_oPos = oChild;
			}

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("w:numFmt") == sName )
						m_oNumFmt = oReader;
					else if ( _T("w:numRestart") == sName )
						m_oNumRestart = oReader;
					else if ( _T("w:numStart") == sName )
						m_oNumStart = oReader;
					else if ( _T("w:pos") == sName )
						m_oPos = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:endnotePr>");

				if ( m_oNumFmt.IsInit() )
				{
					sResult += _T("<w:numFmt ");
					sResult += m_oNumFmt->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumRestart.IsInit() )
				{
					sResult += _T("<w:numRestart ");
					sResult += m_oNumRestart->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumStart.IsInit() )
				{
					sResult += _T("<w:numStart ");
					sResult += m_oNumStart->ToString();
					sResult += _T("/>");
				}

				if ( m_oPos.IsInit() )
				{
					sResult += _T("<w:pos ");
					sResult += m_oPos->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:endnotePr>");

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

				if ( oNode.GetNode( _T("w:numFmt"), oChild ) )
					m_oNumFmt = oChild;

				if ( oNode.GetNode( _T("w:numRestart"), oChild ) )
					m_oNumRestart = oChild;

				if ( oNode.GetNode( _T("w:numStart"), oChild ) )
					m_oNumStart = oChild;

				if ( oNode.GetNode( _T("w:pos"), oChild ) )
					m_oPos = oChild;
			}

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("w:numFmt") == sName )
						m_oNumFmt = oReader;
					else if ( _T("w:numRestart") == sName )
						m_oNumRestart = oReader;
					else if ( _T("w:numStart") == sName )
						m_oNumStart = oReader;
					else if ( _T("w:pos") == sName )
						m_oPos = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:footnotePr>");

				if ( m_oNumFmt.IsInit() )
				{
					sResult += _T("<w:numFmt ");
					sResult += m_oNumFmt->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumRestart.IsInit() )
				{
					sResult += _T("<w:numRestart ");
					sResult += m_oNumRestart->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumStart.IsInit() )
				{
					sResult += _T("<w:numStart ");
					sResult += m_oNumStart->ToString();
					sResult += _T("/>");
				}

				if ( m_oPos.IsInit() )
				{
					sResult += _T("<w:pos ");
					sResult += m_oPos->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:footnotePr>");

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
				oNode.ReadAttributeBase( _T("w:display"),    m_oDisplay );
				oNode.ReadAttributeBase( _T("w:offsetFrom"), m_oOffsetFrom );
				oNode.ReadAttributeBase( _T("w:zOrder"),     m_oZOrder );

				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( _T("w:bottom"), oChild ) )
					m_oBottom = oChild;

				if ( oNode.GetNode( _T("w:left"), oChild ) )
					m_oLeft = oChild;

				if ( oNode.GetNode( _T("w:right"), oChild ) )
					m_oRight = oChild;

				if ( oNode.GetNode( _T("w:top"), oChild ) )
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
					CWCharWrapper sName = oReader.GetName();
					if ( _T("w:bottom") == sName )
						m_oBottom = oReader;
					else if ( _T("w:left") == sName )
						m_oLeft = oReader;
					else if ( _T("w:right") == sName )
						m_oRight = oReader;
					else if ( _T("w:top") == sName )
						m_oTop = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:pgBorders ");

				if ( m_oDisplay.IsInit() )
				{
					sResult += _T("w:display=\"");
					sResult += m_oDisplay->ToString();
					sResult += _T("\" ");
				}

				if ( m_oOffsetFrom.IsInit() )
				{
					sResult += _T("w:offsetFrom=\"");
					sResult += m_oOffsetFrom->ToString();
					sResult += _T("\" ");
				}

				if ( m_oZOrder.IsInit() )
				{
					sResult += _T("w:zOrder=\"");
					sResult += m_oZOrder->ToString();
					sResult += _T("\" ");
				}

				sResult += _T(">");

				if ( m_oBottom.IsInit() )
				{
					sResult += _T("<w:bottom ");
					sResult += m_oBottom->ToString();
					sResult += _T("/>");
				}

				if ( m_oLeft.IsInit() )
				{
					sResult += _T("<w:left ");
					sResult += m_oLeft->ToString();
					sResult += _T("/>");
				}

				if ( m_oRight.IsInit() )
				{
					sResult += _T("<w:right ");
					sResult += m_oRight->ToString();
					sResult += _T("/>");
				}

				if ( m_oTop.IsInit() )
				{
					sResult += _T("<w:top ");
					sResult += m_oTop->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:pgBorders>");

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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:display"),    m_oDisplay )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:offsetFrom"), m_oOffsetFrom )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:zOrder"),     m_oZOrder )
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
			virtual CString toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<CString                       > m_sAuthor;
			nullable<SimpleTypes::CDateTime        > m_oDate;
			nullable<SimpleTypes::CDecimalNumber<> > m_oId;
			nullable<CString                       > m_sUserId;

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
			void ClearItems()
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
				if ( _T("w:sectPr") != oNode.GetName() )
					return;

				oNode.ReadAttributeBase( _T("w:rsidDel"),  m_oRsidDel );
				oNode.ReadAttributeBase( _T("w:rsidR"),    m_oRsidR );
				oNode.ReadAttributeBase( _T("w:rsidRPr"),  m_oRsidRPr );
				oNode.ReadAttributeBase( _T("w:rsidSect"), m_oRsidSect );

				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( _T("w:bidi"), oChild ) )
					m_oBidi = oChild;

				if ( oNode.GetNode( _T("w:cols"), oChild ) )
					m_oCols = oChild;

				if ( oNode.GetNode( _T("w:docGrid"), oChild ) )
					m_oDocGrid = oChild;

				if ( oNode.GetNode( _T("w:endnotePr"), oChild ) )
					m_oEndnotePr = oChild;

				if ( !m_bSectPrChange )
				{
					XmlUtils::CXmlNodes oNodes;
					if ( oNode.GetNodes( _T("w:footerReference"), oNodes ) )
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

				if ( oNode.GetNode( _T("w:footnotePr"), oChild ) )
					m_oFootnotePr = oChild;

				if ( oNode.GetNode( _T("w:formProt"), oChild ) )
					m_oFormProt = oChild;

				if ( !m_bSectPrChange )
				{
					XmlUtils::CXmlNodes oNodes;
					if ( oNode.GetNodes( _T("w:headerReference"), oNodes ) )
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

				if ( oNode.GetNode( _T("w:lnNumType"), oChild ) )
					m_oLnNumType = oChild;

				if ( oNode.GetNode( _T("w:noEndnote"), oChild ) )
					m_oNoEndnote = oChild;

				if ( oNode.GetNode( _T("w:paperSrc"), oChild ) )
					m_oPaperSrc = oChild;

				if ( oNode.GetNode( _T("w:pgBorders"), oChild ) )
					m_oPgBorders = oChild;

				if ( oNode.GetNode( _T("w:pgMar"), oChild ) )
					m_oPgMar = oChild;

				if ( oNode.GetNode( _T("w:pgNumType"), oChild ) )
					m_oPgNumType = oChild;

				if ( oNode.GetNode( _T("w:pgSz"), oChild ) )
					m_oPgSz = oChild;

				if ( oNode.GetNode( _T("w:printerSettings"), oChild ) )
					m_oPrinterSettings = oChild;

				if ( oNode.GetNode( _T("w:rtlGutter"), oChild ) )
					m_oRtlGutter = oChild;

				if ( !m_bSectPrChange && oNode.GetNode( _T("w:sectPrChange"), oChild ) )
					m_oSectPrChange = oChild;

				if ( oNode.GetNode( _T("w:textDirection"), oChild ) )
					m_oTextDirection = oChild;

				if ( oNode.GetNode( _T("w:titlePg"), oChild ) )
					m_oTitlePg = oChild;

				if ( oNode.GetNode( _T("w:type"), oChild ) )
					m_oType = oChild;

				if ( oNode.GetNode( _T("w:vAlign"), oChild ) )
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
					CWCharWrapper sName = oReader.GetName();
					if ( _T("w:bidi") == sName )
						m_oBidi = oReader;
					else if ( _T("w:cols") == sName )
						m_oCols = oReader;
					else if ( _T("w:docGrid") == sName )
						m_oDocGrid = oReader;
					else if ( _T("w:endnotePr") == sName )
						m_oEndnotePr = oReader;
					else if ( !m_bSectPrChange && _T("w:footerReference") == sName )
					{
						ComplexTypes::Word::CHdrFtrRef *oFooter = new ComplexTypes::Word::CHdrFtrRef(oReader);
						if (oFooter) m_arrFooterReference.push_back( oFooter );
					}
					else if ( _T("w:footnotePr") == sName )
						m_oFootnotePr = oReader;
					else if ( _T("w:formProt") == sName )
						m_oFormProt = oReader;
					else if ( !m_bSectPrChange && _T("w:headerReference") == sName )
					{
						ComplexTypes::Word::CHdrFtrRef *oHeader = new ComplexTypes::Word::CHdrFtrRef( oReader);
						if (oHeader) m_arrHeaderReference.push_back( oHeader );
					}
					else if ( _T("w:lnNumType") == sName )
						m_oLnNumType = oReader;
					else if ( _T("w:noEndnote") == sName )
						m_oNoEndnote = oReader;
					else if ( _T("w:paperSrc") == sName )
						m_oPaperSrc = oReader;
					else if ( _T("w:pgBorders") == sName )
						m_oPgBorders = oReader;
					else if ( _T("w:pgMar") == sName )
						m_oPgMar = oReader;
					else if ( _T("w:pgNumType") == sName )
						m_oPgNumType = oReader;
					else if ( _T("w:pgSz") == sName )
						m_oPgSz = oReader;
					else if ( _T("w:printerSettings") == sName )
						m_oPrinterSettings = oReader;
					else if ( _T("w:rtlGutter") == sName )
						m_oRtlGutter = oReader;
					else if ( !m_bSectPrChange && _T("w:sectPrChange") == sName )
						m_oSectPrChange = oReader;
					else if ( _T("w:textDirection") == sName )
						m_oTextDirection = oReader;
					else if ( _T("w:titlePg") == sName )
						m_oTitlePg = oReader;
					else if ( _T("w:type") == sName )
						m_oType = oReader;
					else if ( _T("w:vAlign") == sName )
						m_oVAlign = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:sectPr ");

				if ( m_oRsidDel.IsInit() )
				{
					sResult += _T("w:rsidDel=\"");
					sResult += m_oRsidDel->ToString();
					sResult += _T("\" ");						
				}

				if ( m_oRsidR.IsInit() )
				{
					sResult += _T("w:rsidR=\"");
					sResult += m_oRsidR->ToString();
					sResult += _T("\" ");						
				}

				if ( m_oRsidRPr.IsInit() )
				{
					sResult += _T("w:rsidRPr=\"");
					sResult += m_oRsidRPr->ToString();
					sResult += _T("\" ");						
				}

				if ( m_oRsidSect.IsInit() )
				{
					sResult += _T("w:rsidSect=\"");
					sResult += m_oRsidSect->ToString();
					sResult += _T("\" ");						
				}

				sResult += _T(">");

				if ( m_oBidi.IsInit() )
				{
					sResult += _T("<w:bidi ");
					sResult += m_oBidi->ToString();
					sResult += _T("/>");
				}

				if ( m_oCols.IsInit() )
					sResult += m_oCols->toXML();

				if ( m_oDocGrid.IsInit() )
				{
					sResult += _T("<w:docGrid ");
					sResult += m_oDocGrid->ToString();
					sResult += _T("/>");
				}

				if ( m_oEndnotePr.IsInit() )
					sResult += m_oEndnotePr->toXML();

				if ( !m_bSectPrChange )
				{
					for (unsigned int nIndex = 0; nIndex < m_arrFooterReference.size(); nIndex++ )
					{
						sResult += _T("<w:footerReference ");
						if (m_arrFooterReference[nIndex])
							sResult += m_arrFooterReference[nIndex]->ToString();
						sResult += _T("/>");
					}
				}

				if ( m_oFootnotePr.IsInit() )
					sResult += m_oFootnotePr->toXML();

				if ( m_oFormProt.IsInit() )
				{
					sResult += _T("<w:formProt ");
					sResult += m_oFormProt->ToString();
					sResult += _T("/>");
				}

				if ( !m_bSectPrChange )
				{
					for (unsigned int nIndex = 0; nIndex < m_arrHeaderReference.size(); nIndex++ )
					{
						sResult += _T("<w:headerReference ");
						if (m_arrHeaderReference[nIndex])
							sResult += m_arrHeaderReference[nIndex]->ToString();
						sResult += _T("/>");
					}
				}

				if ( m_oLnNumType.IsInit() )
				{
					sResult += _T("<w:lnNumType ");
					sResult += m_oLnNumType->ToString();
					sResult += _T("/>");
				}

				if ( m_oNoEndnote.IsInit() )
				{
					sResult += _T("<w:noEndnote ");
					sResult += m_oNoEndnote->ToString();
					sResult += _T("/>");
				}

				if ( m_oPaperSrc.IsInit() )
				{
					sResult += _T("<w:paperSrc ");
					sResult += m_oPaperSrc->ToString();
					sResult += _T("/>");
				}

				if ( m_oPgBorders.IsInit() )
					sResult += m_oPgBorders->toXML();

				if ( m_oPgMar.IsInit() )
				{
					sResult += _T("<w:pgMar ");
					sResult += m_oPgMar->ToString();
					sResult += _T("/>");
				}

				if ( m_oPgNumType.IsInit() )
				{
					sResult += _T("<w:pgNumType ");
					sResult += m_oPgNumType->ToString();
					sResult += _T("/>");
				}

				if ( m_oPgSz.IsInit() )
				{
					sResult += _T("<w:pgSz ");
					sResult += m_oPgSz->ToString();
					sResult += _T("/>");
				}

				if ( m_oPrinterSettings.IsInit() )
				{
					sResult += _T("<w:printerSettings ");
					sResult += m_oPrinterSettings->ToString();
					sResult += _T("/>");
				}

				if ( m_oRtlGutter.IsInit() )
				{
					sResult += _T("<w:rtlGutter ");
					sResult += m_oRtlGutter->ToString();
					sResult += _T("/>");
				}

				if ( !m_bSectPrChange && m_oSectPrChange.IsInit() )
					sResult += m_oSectPrChange->toXML();

				if ( m_oTextDirection.IsInit() )
				{
					sResult += _T("<w:textDirection ");
					sResult += m_oTextDirection->ToString();
					sResult += _T("/>");
				}

				if ( m_oTitlePg.IsInit() )
				{
					sResult += _T("<w:titlePg ");
					sResult += m_oTitlePg->ToString();
					sResult += _T("/>");
				}

				if ( m_oType.IsInit() )
				{
					sResult += _T("<w:type ");
					sResult += m_oType->ToString();
					sResult += _T("/>");
				}

				if ( m_oVAlign.IsInit() )
				{
					sResult += _T("<w:vAlign ");
					sResult += m_oVAlign->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:sectPr>");

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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:rsidDel"),  m_oRsidDel )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:rsidR"),    m_oRsidR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:rsidRPr"),  m_oRsidRPr )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:rsidSect"), m_oRsidSect )
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
