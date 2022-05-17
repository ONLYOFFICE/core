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
#ifndef OOX_LOGIC_RUN_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_RUN_PROPERTY_INCLUDE_H_

#include "../WritingElement.h"

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/ComplexTypes.h"

#include "../../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Ln.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// EastAsianLayout 17.3.2.10 (Part 1)
		//--------------------------------------------------------------------------------
		class CEastAsianLayout : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CEastAsianLayout)
			CEastAsianLayout()
			{
			}
			virtual ~CEastAsianLayout()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:combine",         m_oCombine );
				XmlMacroReadAttributeBase( oNode, L"w:combineBrackets", m_oCombineBrackets );
				XmlMacroReadAttributeBase( oNode, L"w:id",              m_oID );
				XmlMacroReadAttributeBase( oNode, L"w:vert",            m_oVert );
				XmlMacroReadAttributeBase( oNode, L"w:vertCompress",    m_oVertCompress );
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

				if ( m_oCombine.IsInit() )
				{
					sResult += L"w:combine=\"";
					sResult += m_oCombine->ToString();
					sResult += L"\" ";
				}

				if ( m_oCombineBrackets.IsInit() )
				{
					sResult += L"w:combineBrackets=\"";
					sResult += m_oCombineBrackets->ToString();
					sResult += L"\" ";
				}

				if ( m_oID.IsInit() )
				{
					sResult += L"w:themeTint=\"";
					sResult += m_oID->ToString();
					sResult += L"\" ";
				}

				if ( m_oVert.IsInit() )
				{
					sResult += L"w:vert=\"";
					sResult += m_oVert->ToString();
					sResult += L"\" ";
				}

				if ( m_oVertCompress.IsInit() )
				{
					sResult += L"w:vertCompress=\"";
					sResult += m_oVert->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:combine",         m_oCombine )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:combineBrackets", m_oCombineBrackets )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",              m_oID )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:vert",            m_oVert )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:vertCompress",    m_oVertCompress )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::COnOff<>           > m_oCombine;
			nullable<SimpleTypes::CCombineBrackets<> > m_oCombineBrackets;
			nullable<SimpleTypes::CDecimalNumber<>   > m_oID;
			nullable<SimpleTypes::COnOff<>           > m_oVert;
			nullable<SimpleTypes::COnOff<>           > m_oVertCompress;
		};

		//--------------------------------------------------------------------------------
		// Effect 17.3.2.11 (Part 1)
		//--------------------------------------------------------------------------------
		class CTextEffect : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextEffect)
			CTextEffect()
			{
			}
			virtual ~CTextEffect()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CTextEffect<>> m_oVal;
		};


		//--------------------------------------------------------------------------------
		// Em 17.3.2.12 (Part 1)
		//--------------------------------------------------------------------------------
		class CEm : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CEm)
			CEm()
			{
			}
			virtual ~CEm()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CEm<>> m_oVal;
		};


		//--------------------------------------------------------------------------------
		// FitText 17.3.2.14 (Part 1)
		//--------------------------------------------------------------------------------
		class CFitText : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFitText)
			CFitText()
			{
			}
			virtual ~CFitText()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:id",  m_oID );
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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

				if ( m_oID.IsInit() )
				{
					sResult += L"w:id=\"";
					sResult += m_oID->ToString();
					sResult += L"\" ";
				}

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
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:id",  m_oID  )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CDecimalNumber<>> m_oID;
			nullable<SimpleTypes::CTwipsMeasure   > m_oVal;
		};


		//--------------------------------------------------------------------------------
		// Highlight 17.3.2.14 (Part 1)
		//--------------------------------------------------------------------------------
		class CHighlight : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHighlight)
			CHighlight()
			{
			}
			virtual ~CHighlight()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CHighlightColor<>> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// u (Underline) 17.3.2.40 (Part 1)
		//--------------------------------------------------------------------------------
		class CUnderline : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CUnderline)
			CUnderline()
			{
			}
			virtual ~CUnderline()
			{
			}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:color",      m_oColor );
				XmlMacroReadAttributeBase( oNode, L"w:themeColor", m_oThemeColor );
				XmlMacroReadAttributeBase( oNode, L"w:themeShade", m_oThemeShade );
				XmlMacroReadAttributeBase( oNode, L"w:themeTint",  m_oThemeTint );
				XmlMacroReadAttributeBase( oNode, L"w:val",        m_oVal );
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
					sResult += L"w:color=\"";
					sResult += m_oColor->ToString();
					sResult += L"\" ";
				}

				if ( m_oThemeColor.IsInit() )
				{
					sResult += L"w:themeColor=\"";
					sResult += m_oThemeColor->ToString();
					sResult += L"\" ";
				}

				if ( m_oThemeShade.IsInit() )
				{
					sResult += L"w:themeShade=\"";
					sResult += m_oThemeShade->ToString();
					sResult += L"\" ";
				}

				if ( m_oThemeTint.IsInit() )
				{
					sResult += L"w:themeTint=\"";
					sResult += m_oThemeTint->ToString();
					sResult += L"\" ";
				}

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
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:color",      m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeColor", m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeShade", m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeTint",  m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",        m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CHexColor<>       > m_oColor;
			nullable<SimpleTypes::CThemeColor<>     > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber<> > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<> > m_oThemeTint;
			nullable<SimpleTypes::CUnderline<>      > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// vertAlign (VerticalAlignRun) 17.3.2.40 (Part 1)
		//--------------------------------------------------------------------------------
		class CVerticalAlignRun : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CVerticalAlignRun)
			CVerticalAlignRun()
			{
			}
			virtual ~CVerticalAlignRun()
			{
			}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val",        m_oVal );
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
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CVerticalAlignRun<>> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// w (TextScale) 17.3.2.40 (Part 1)
		//--------------------------------------------------------------------------------
		class CTextScale : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextScale)
			CTextScale()
			{
			}
			virtual ~CTextScale()
			{
			}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CTextScale<> > m_oVal;
		};


	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		class CRun;
		class CRunProperty;

		//--------------------------------------------------------------------------------
		// RPrChange 17.3.2.31 (Part 1)
		//--------------------------------------------------------------------------------	

		class CRPrChange : public WritingElement
		{
		public:
			CRPrChange();
			CRPrChange(XmlUtils::CXmlNode& oNode);
			CRPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CRPrChange();
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:

            nullable<std::wstring                   > m_sAuthor;
            nullable<SimpleTypes::CDateTime         > m_oDate;
            nullable<SimpleTypes::CDecimalNumber<>  > m_oId;
            nullable<std::wstring                   > m_sUserId;

            nullable<CRunProperty>                  m_pRunPr;
		};


		class CRunProperty : public WritingElement
		{
		public:
			CRunProperty()
			{
				m_pText = NULL;
				m_bRPRChange = false;
			}
			virtual ~CRunProperty()
			{
				RELEASEOBJECT(m_pText);
			}
			CRunProperty(const XmlUtils::CXmlNode &oNode)
			{
				m_pText = NULL;
				m_bRPRChange = false;
				fromXML( (XmlUtils::CXmlNode &)oNode );
			}
			CRunProperty(const XmlUtils::CXmlLiteReader& oReader)
			{
				m_pText = NULL;
				m_bRPRChange = false;
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			}
			const CRunProperty& operator=(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode &)oNode );
				return *this;
			}
			const CRunProperty& operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXML(XmlUtils::CXmlLiteReader& oReader, CRun* pRun);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_w_rPr;
			}
			static const CRunProperty Merge(const CRunProperty& oPrev, const CRunProperty& oCurrent)
			{
				CRunProperty oProperties;
				oProperties.m_oBold            = Merge( oPrev.m_oBold,            oCurrent.m_oBold );
				oProperties.m_oBoldCs          = Merge( oPrev.m_oBoldCs,          oCurrent.m_oBoldCs );
				oProperties.m_oBdr             = Merge( oPrev.m_oBdr,             oCurrent.m_oBdr );
				oProperties.m_oCaps            = Merge( oPrev.m_oCaps,            oCurrent.m_oCaps );
				oProperties.m_oColor           = Merge( oPrev.m_oColor,           oCurrent.m_oColor );
				oProperties.m_oCs              = Merge( oPrev.m_oCs,              oCurrent.m_oCs );
				oProperties.m_oDel             = Merge( oPrev.m_oDel,             oCurrent.m_oDel );
				oProperties.m_oDStrike         = Merge( oPrev.m_oDStrike,         oCurrent.m_oDStrike );
				oProperties.m_oEastAsianLayout = Merge( oPrev.m_oEastAsianLayout, oCurrent.m_oEastAsianLayout );
				oProperties.m_oEffect          = Merge( oPrev.m_oEffect,          oCurrent.m_oEffect );
				oProperties.m_oEm              = Merge( oPrev.m_oEm,              oCurrent.m_oEm );
				oProperties.m_oEmboss          = Merge( oPrev.m_oEmboss,          oCurrent.m_oEmboss );
				oProperties.m_oFitText         = Merge( oPrev.m_oFitText,         oCurrent.m_oFitText );
				oProperties.m_oHighlight       = Merge( oPrev.m_oHighlight,       oCurrent.m_oHighlight );
				oProperties.m_oIns             = Merge( oPrev.m_oIns,             oCurrent.m_oIns );
				oProperties.m_oItalic          = Merge( oPrev.m_oItalic,          oCurrent.m_oItalic );
				oProperties.m_oItalicCs        = Merge( oPrev.m_oItalicCs,        oCurrent.m_oItalicCs );
				oProperties.m_oImprint         = Merge( oPrev.m_oImprint,         oCurrent.m_oImprint );
				oProperties.m_oKern            = Merge( oPrev.m_oKern,            oCurrent.m_oKern );
				oProperties.m_oLang            = Merge( oPrev.m_oLang,            oCurrent.m_oLang );
				oProperties.m_oNoProof         = Merge( oPrev.m_oNoProof,         oCurrent.m_oNoProof );
				oProperties.m_oMath            = Merge( oPrev.m_oMath,            oCurrent.m_oMath );
				oProperties.m_oMoveFrom        = Merge( oPrev.m_oMoveFrom,        oCurrent.m_oMoveFrom );
				oProperties.m_oMoveTo          = Merge( oPrev.m_oMoveTo,          oCurrent.m_oMoveTo );
				oProperties.m_oOutline         = Merge( oPrev.m_oOutline,         oCurrent.m_oOutline );
				oProperties.m_oPosition        = Merge( oPrev.m_oPosition,        oCurrent.m_oPosition );
				oProperties.m_oRFonts          = Merge( oPrev.m_oRFonts,          oCurrent.m_oRFonts );
				oProperties.m_oRStyle          = Merge( oPrev.m_oRStyle,          oCurrent.m_oRStyle );
				oProperties.m_oRtL             = Merge( oPrev.m_oRtL,             oCurrent.m_oRtL );
				oProperties.m_oShadow          = Merge( oPrev.m_oShadow,          oCurrent.m_oShadow );
				oProperties.m_oShd             = Merge( oPrev.m_oShd,             oCurrent.m_oShd );
				oProperties.m_oSmallCaps       = Merge( oPrev.m_oSmallCaps,       oCurrent.m_oSmallCaps );
				oProperties.m_oSnapToGrid      = Merge( oPrev.m_oSnapToGrid,      oCurrent.m_oSnapToGrid );
				oProperties.m_oSpacing         = Merge( oPrev.m_oSpacing,         oCurrent.m_oSpacing );
				oProperties.m_oSpecVanish      = Merge( oPrev.m_oSpecVanish,      oCurrent.m_oSpecVanish );
				oProperties.m_oStrike          = Merge( oPrev.m_oStrike,          oCurrent.m_oStrike );
				oProperties.m_oSz              = Merge( oPrev.m_oSz,              oCurrent.m_oSz );
				oProperties.m_oSzCs            = Merge( oPrev.m_oSzCs,            oCurrent.m_oSzCs );
				oProperties.m_oU               = Merge( oPrev.m_oU,               oCurrent.m_oU );
				oProperties.m_oVanish          = Merge( oPrev.m_oVanish,          oCurrent.m_oVanish );
				oProperties.m_oVertAlign       = Merge( oPrev.m_oVertAlign,       oCurrent.m_oVertAlign );
				oProperties.m_oW               = Merge( oPrev.m_oW,               oCurrent.m_oW );
				oProperties.m_oWebHidden       = Merge( oPrev.m_oWebHidden,       oCurrent.m_oWebHidden );

				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}

			const bool IsSimple() const
			{
				if ( m_oBold.IsInit() )
					return false;

				if ( m_oBoldCs.IsInit() )
					return false;

				if ( m_oBdr.IsInit() )
					return false;

				if ( m_oCaps.IsInit() )
					return false;

				if ( m_oColor.IsInit() )
					return false;

				if ( m_oCs.IsInit() )
					return false;

				if ( m_oDel.IsInit() )
					return false;

				if ( m_oDStrike.IsInit() )
					return false;

				if ( m_oEastAsianLayout.IsInit() )
					return false;

				if ( m_oEffect.IsInit() )
					return false;

				if ( m_oEm.IsInit() )
					return false;

				if ( m_oEmboss.IsInit() )
					return false;

				if ( m_oFitText.IsInit() )
					return false;

				if ( m_oHighlight.IsInit() )
					return false;

				if ( m_oIns.IsInit() )
					return false;

				if ( m_oItalic.IsInit() )
					return false;

				if ( m_oItalicCs.IsInit() )
					return false;

				if ( m_oImprint.IsInit() )
					return false;

				if ( m_oKern.IsInit() )
					return false;

				if ( m_oLang.IsInit() )
					return false;

				if ( m_oNoProof.IsInit() )
					return false;

				if ( m_oMath.IsInit() )
					return false;

				if ( m_oMoveFrom.IsInit() )
					return false;

				if ( m_oMoveTo.IsInit() )
					return false;

				if ( m_oOutline.IsInit() )
					return false;

				if ( m_oPosition.IsInit() )
					return false;

				if ( m_oRFonts.IsInit() )
					return false;

				if ( m_oRStyle.IsInit() )
					return false;

				if ( m_oRtL.IsInit() )
					return false;

				if ( m_oShadow.IsInit() )
					return false;

				if ( m_oShd.IsInit() )
					return false;

				if ( m_oSmallCaps.IsInit() )
					return false;

				if ( m_oSnapToGrid.IsInit() )
					return false;

				if ( m_oSpacing.IsInit() )
					return false;

				if ( m_oSpecVanish.IsInit() )
					return false;

				if ( m_oStrike.IsInit() )
					return false;

				if ( m_oSz.IsInit() )
					return false;

				if ( m_oSzCs.IsInit() )
					return false;

				if ( m_oU.IsInit() )
					return false;

				if ( m_oVanish.IsInit() )
					return false;

				if ( m_oVertAlign.IsInit() )
					return false;

				if ( m_oW.IsInit() )
					return false;

				if ( m_oWebHidden.IsInit() )
					return false;

				return true;
			}

			bool m_bRPRChange; // Является ли данный w:rPr дочерним по отношению к w:rPrChange

			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oBold;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oBoldCs;
			nullable<ComplexTypes::Word::CBorder                         > m_oBdr;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oCaps;
			nullable<ComplexTypes::Word::CColor                          > m_oColor;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oCs;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oDel;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oDStrike;
			nullable<ComplexTypes::Word::CEastAsianLayout                > m_oEastAsianLayout;
			nullable<ComplexTypes::Word::CTextEffect                     > m_oEffect;
			nullable<ComplexTypes::Word::CEm                             > m_oEm;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oEmboss;
			nullable<ComplexTypes::Word::CFitText                        > m_oFitText;
			nullable<ComplexTypes::Word::CHighlight                      > m_oHighlight;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oIns;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oItalic;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oItalicCs;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oImprint;
			nullable<ComplexTypes::Word::CHpsMeasure                     > m_oKern;
			nullable<ComplexTypes::Word::CLanguage                       > m_oLang;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oNoProof;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oMath;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oMoveFrom;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oMoveTo;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oOutline;
			nullable<ComplexTypes::Word::CSignedHpsMeasure               > m_oPosition;
			nullable<ComplexTypes::Word::CFonts                          > m_oRFonts;
			nullable<OOX::Logic::CRPrChange                              > m_oRPrChange;
            nullable<ComplexTypes::Word::String                          > m_oRStyle;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oRtL;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oShadow;
			nullable<ComplexTypes::Word::CShading                        > m_oShd;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> >	m_oSmallCaps;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> >	m_oSnapToGrid;
			nullable<ComplexTypes::Word::CSignedTwipsMeasure             >	m_oSpacing;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> >	m_oSpecVanish;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> >	m_oStrike;
			nullable<ComplexTypes::Word::CHpsMeasure                     >	m_oSz;
			nullable<ComplexTypes::Word::CHpsMeasure                     >	m_oSzCs;
			nullable<ComplexTypes::Word::CUnderline                      >	m_oU;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> >	m_oVanish;
			nullable<ComplexTypes::Word::CVerticalAlignRun               >	m_oVertAlign;
			nullable<ComplexTypes::Word::CTextScale                      >	m_oW;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> >	m_oWebHidden;
			PPTX::Logic::UniFill											m_oTextFill;			
            nullable<PPTX::Logic::Ln>										m_oTextOutline;

			OOX::WritingElement*											m_pText; //temp ... for Run object -> XpertdocOnlineDemoEn.docx
		};

	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RUN_PROPERTY_INCLUDE_H_
