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

#include "../DocxFormat/WritingElement.h"
#include "../Base/Nullable.h"

#include "SimpleTypes_Word.h"
#include "SimpleTypes_Shared.h"
#include "SimpleTypes_Drawing.h"

namespace ComplexTypes
{
#define ComplexTypes_AdditionConstructors(Class) \
	Class(XmlUtils::CXmlNode& oNode)\
	{\
		FromXML( oNode );\
	}\
	Class(XmlUtils::CXmlLiteReader& oReader)\
	{\
		FromXML( oReader );\
	}\
	const Class& operator =(const XmlUtils::CXmlNode &oNode)\
	{\
		FromXML( (XmlUtils::CXmlNode &)oNode );\
		return *this;\
	}\
	const Class& operator =(const XmlUtils::CXmlLiteReader& oReader)\
	{\
		FromXML( (XmlUtils::CXmlLiteReader&)oReader );\
		return *this;\
	}
#define ComplexTypes_WriteAttribute_( sStartString, oValue ) \
	if ( oValue.IsInit() )\
	{\
		sResult += sStartString + oValue->ToString() + L"\"";\
	}
#define ComplexTypes_WriteAttribute2_( sStartString, oValue ) \
	if ( oValue.IsInit() )\
	{\
		sResult += sStartString + oValue.get2() + L"\"";\
	}
//---------------------------------------------------------------------------------------------
#define ComplexTypes_WriteAttribute( sStartString, oValue ) \
	if ( oValue.IsInit() )\
	{\
		sResult += sStartString + oValue->ToString() + L"\" ";\
	}

#define ComplexTypes_WriteAttribute2( sStartString, oValue ) \
	if ( oValue.IsInit() )\
	{\
		sResult += sStartString + oValue.get2() + L"\" ";\
	}
#define ComplexTypes_WriteAttribute3( sStartString, oValue ) \
	if ( oValue.IsInit() )\
	{\
		sResult += sStartString + (*oValue) + L"\" ";\
	}
#define ComplexTypes_WriteAttribute2Encode( sStartString, oValue ) \
	if ( oValue.IsInit() )\
	{\
		sResult += sStartString;\
		sResult += XmlUtils::EncodeXmlString(oValue.get2());\
		sResult += L"\" ";\
	}

	//--------------------------------------------------------------------------------
	// ComplexType
	//--------------------------------------------------------------------------------

	class ComplexType
	{
	public:
		ComplexType();
		virtual ~ComplexType();

		virtual void FromXML(XmlUtils::CXmlNode& oNode) = 0;
        virtual std::wstring ToString() const = 0;
		
		virtual inline std::wstring ValNode(const std::wstring &node_name);
	};

	//--------------------------------------------------------------------------------
	// DecimalNumber 17.3.1.10 (Part 1)
	//--------------------------------------------------------------------------------

	class CDecimalNumber : public ComplexType
	{
	public:
		ComplexTypes_AdditionConstructors(CDecimalNumber)
		CDecimalNumber();
		virtual ~CDecimalNumber();

		virtual void FromXML(XmlUtils::CXmlNode& oNode);
		virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring ToString() const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable_int m_oVal;
	};

	namespace Word
	{
		//--------------------------------------------------------------------------------
		// Border 17.3.4 (Part 1)
		//--------------------------------------------------------------------------------

		class CBorder : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CBorder)
			CBorder();
			virtual ~CBorder();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHexColor>			m_oColor;
			nullable<SimpleTypes::COnOff>				m_oFrame;
			nullable<SimpleTypes::COnOff>				m_oShadow;
			nullable<SimpleTypes::CPointMeasure>		m_oSpace;
			nullable<SimpleTypes::CEighthPointMeasure>	m_oSz;
			nullable<SimpleTypes::CThemeColor>			m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber>		m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber>		m_oThemeTint;
			nullable<SimpleTypes::CBorder>				m_oVal;
		};

		//--------------------------------------------------------------------------------
		// rFonts (Fonts) 17.3.2.20 (Part 1)
		//--------------------------------------------------------------------------------

		class CFonts : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFonts)
			CFonts();
			virtual ~CFonts();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring>			m_sAscii;
			nullable<SimpleTypes::CTheme>	m_oAsciiTheme;
			nullable<std::wstring>			m_sCs;
			nullable<SimpleTypes::CTheme>	m_oCsTheme;
			nullable<std::wstring>			m_sEastAsia;
			nullable<SimpleTypes::CTheme>	m_oEastAsiaTheme;
			nullable<std::wstring>			m_sHAnsi;
			nullable<SimpleTypes::CTheme>	m_oHAnsiTheme;
			nullable<SimpleTypes::CHint>	m_oHint;
		};

		//--------------------------------------------------------------------------------
		// Lang (Language) 17.3.2.20 (Part 1)
		//--------------------------------------------------------------------------------

		class CLanguage : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLanguage)
			CLanguage();
			virtual ~CLanguage();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// TO DO: сделать парсер языка по спецификации RFC 4646/BCP 47 - CLang
			nullable_string m_oBidi;
			nullable_string m_oEastAsia;
			nullable_string m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Lang (Language) 17.3.3.14 (Part 1)
		//--------------------------------------------------------------------------------

		class CLang : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLang)
			CLang();
			virtual ~CLang();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Shading 17.3.5 (Part 1)
		//--------------------------------------------------------------------------------

		class CShading : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CShading)
			CShading();
			virtual ~CShading();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHexColor             > m_oColor;
			nullable<SimpleTypes::CHexColor             > m_oFill;
			nullable<SimpleTypes::CThemeColor           > m_oThemeColor;
			nullable<SimpleTypes::CThemeColor           > m_oThemeFill;
			nullable<SimpleTypes::CUcharHexNumber       > m_oThemeFillShade;
			nullable<SimpleTypes::CUcharHexNumber       > m_oThemeFillTint;
			nullable<SimpleTypes::CUcharHexNumber       > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber       > m_oThemeTint;
			nullable<SimpleTypes::CShd                  > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TblWidth 17.4.88 (Part 1)
		//--------------------------------------------------------------------------------

		class CTblWidth : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblWidth)
			CTblWidth();
			virtual ~CTblWidth();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTblWidth>				m_oType;
			nullable<SimpleTypes::CDecimalNumberOrPercent>	m_oW;
		};

		//--------------------------------------------------------------------------------
		// OnOff 17.17.4 (Part 1)
		//--------------------------------------------------------------------------------

		class COnOff : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(COnOff)

			COnOff();
			virtual ~COnOff();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// OnOff2
		//--------------------------------------------------------------------------------

		class COnOff2 : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(COnOff2)
			COnOff2();
			virtual ~COnOff2();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
            virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual inline std::wstring ValNode(const std::wstring &node_name);

		public:
            SimpleTypes::COnOff m_oVal = SimpleTypes::onoffTrue;
		};

		//--------------------------------------------------------------------------------
		// String 17.3.1.27  (Part 1)
		//--------------------------------------------------------------------------------

        class String : public ComplexType
		{
		public:
            ComplexTypes_AdditionConstructors(String)
			String();
			virtual ~String();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;
			virtual std::wstring ToStringWithNS(const std::wstring& ns) const;
			std::wstring ToString2() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable_string m_sVal;
		};

		//--------------------------------------------------------------------------------
		// Spacing (SignedTwipsMeasure) 17.3.2.19 (Part 1)
		//--------------------------------------------------------------------------------

		class CSignedTwipsMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSignedTwipsMeasure)
			CSignedTwipsMeasure();
			virtual ~CSignedTwipsMeasure();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CSignedTwipsMeasure> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Kern (HpsMeasure) 17.3.2.19 (Part 1)
		//--------------------------------------------------------------------------------

		class CHpsMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHpsMeasure)
			CHpsMeasure();
			virtual ~CHpsMeasure();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHpsMeasure> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Border 17.3.2.6 (Part 1)
		//--------------------------------------------------------------------------------

		class CColor : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CColor)
			CColor();
			virtual ~CColor();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CThemeColor           > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber       > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber       > m_oThemeTint;
			nullable<SimpleTypes::CHexColor             > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Position (HpsMeasure) 17.3.2.19 (Part 1)
		//--------------------------------------------------------------------------------

		class CSignedHpsMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSignedHpsMeasure)
			CSignedHpsMeasure();
			virtual ~CSignedHpsMeasure();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::CSignedHpsMeasure> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// NumFmt 17.11.17 (Part 1)
		//--------------------------------------------------------------------------------

		class CNumFmt : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CNumFmt)
			CNumFmt();
			virtual ~CNumFmt();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<std::wstring> m_sFormat;
			nullable<SimpleTypes::CNumberFormat> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// NumFmt 17.11.19 (Part 1)
		//--------------------------------------------------------------------------------

		class CNumRestart : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CNumRestart)
			CNumRestart();
			virtual ~CNumRestart();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CRestartNumber> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// DecimalNumber 17.3.1.10 (Part 1)
		//--------------------------------------------------------------------------------

		class CDecimalNumber : public ComplexTypes::CDecimalNumber
		{
		public:
			ComplexTypes_AdditionConstructors(CDecimalNumber)
			CDecimalNumber();
			virtual ~CDecimalNumber();

			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual std::wstring ToString() const;
		};

		//--------------------------------------------------------------------------------
		// DecimalNumberOrPrecent 17.3.1.10 (Part 1)
		//--------------------------------------------------------------------------------

		class CDecimalNumberOrPrecent : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDecimalNumberOrPrecent)
			CDecimalNumberOrPrecent();
			virtual ~CDecimalNumberOrPrecent();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumberOrPercent> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Rel 17.6.14 (Part 1)
		//--------------------------------------------------------------------------------

		class CRel : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CRel)
			CRel();
			virtual ~CRel();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};

		//--------------------------------------------------------------------------------
		// TextDirection 17.6.14 (Part 1)
		//--------------------------------------------------------------------------------

		class CTextDirection : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextDirection)
			CTextDirection();
			virtual ~CTextDirection();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTextDirection> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// VerticalJc 17.6.23 (Part 1)
		//--------------------------------------------------------------------------------

		class CVerticalJc : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CVerticalJc)
			CVerticalJc();
			virtual ~CVerticalJc();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CVerticalJc> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// cnfStyle (Cnf) 17.3.1.8 (Part 1) + 9.2.1.1 (Part 4)
		//--------------------------------------------------------------------------------

		class CCnf : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CCnf)
			CCnf();
			virtual ~CCnf();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:			
			nullable<SimpleTypes::COnOff> m_oEvenHBand;
			nullable<SimpleTypes::COnOff> m_oEvenVBand;
			nullable<SimpleTypes::COnOff> m_oFirstColumn;
			nullable<SimpleTypes::COnOff> m_oFirstRow;
			nullable<SimpleTypes::COnOff> m_oFirstRowFirstColumn;
			nullable<SimpleTypes::COnOff> m_oFirstRowLastColumn;
			nullable<SimpleTypes::COnOff> m_oLastColumn;
			nullable<SimpleTypes::COnOff> m_oLastRow;
			nullable<SimpleTypes::COnOff> m_oLastRowFirstColumn;
			nullable<SimpleTypes::COnOff> m_oLastRowLastColumn;
			nullable<SimpleTypes::COnOff> m_oOddHBand;
			nullable<SimpleTypes::COnOff> m_oOddVBand;
			nullable<SimpleTypes::CCnf  > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// jc (Jc) 17.3.1.13 (Part 1)
		//--------------------------------------------------------------------------------

		class CJc : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CJc)
			CJc();
			virtual ~CJc();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CJc> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TrackChange 17.13.5.19 (Part 1)
		//--------------------------------------------------------------------------------

		class CTrackChange : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTrackChange)
			CTrackChange();
			virtual ~CTrackChange();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<std::wstring> m_sAuthor;
			nullable<SimpleTypes::CDateTime> m_oDate;
			nullable<SimpleTypes::CDecimalNumber> m_oId;
            nullable<std::wstring> m_sUserId;
		};

		//--------------------------------------------------------------------------------
		// LongHexNumber 17.9.30 (Part 1)
		//--------------------------------------------------------------------------------

		class CLongHexNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLongHexNumber)
			CLongHexNumber();
			virtual ~CLongHexNumber();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CLongHexNumber> m_oVal;

		};

		//--------------------------------------------------------------------------------
		// UnsignedDecimalNumber 17.16.32 (Part 1)
		//--------------------------------------------------------------------------------

		class CUnsignedDecimalNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CUnsignedDecimalNumber)
			CUnsignedDecimalNumber();
			virtual ~CUnsignedDecimalNumber();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TwipsMeasure 17.15.1.25 (Part 1)
		//--------------------------------------------------------------------------------

		class CTwipsMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTwipsMeasure)
			CTwipsMeasure();
			virtual ~CTwipsMeasure();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTwipsMeasure> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Empty 17.15.1.48 (Part 1)
		//--------------------------------------------------------------------------------

		class CEmpty : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CEmpty)
			CEmpty();
			virtual ~CEmpty();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;
		};

		//--------------------------------------------------------------------------------
		// MatchSrc 17.17.2.3 (Part 1)
		//--------------------------------------------------------------------------------

		class CMatchSrc : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CMatchSrc)
			CMatchSrc();
			virtual ~CMatchSrc();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// EdnPos 17.11.22 (Part 1)
		//--------------------------------------------------------------------------------

		class CEdnPos : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CEdnPos)
			CEdnPos();
			virtual ~CEdnPos();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CEdnPos> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FtnPos 17.11.21 (Part 1)
		//--------------------------------------------------------------------------------

		class CFtnPos : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFtnPos)
			CFtnPos();
			virtual ~CFtnPos();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CFtnPos> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// DocPartGallery
		//--------------------------------------------------------------------------------

		class CDocPartGallery : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDocPartGallery)
			CDocPartGallery();
			virtual ~CDocPartGallery();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDocPartGallery> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// DocPartBehavior
		//--------------------------------------------------------------------------------

		class CDocPartBehavior : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDocPartBehavior)
			
			CDocPartBehavior();
			virtual ~CDocPartBehavior();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDocPartBehavior> m_oVal;
		};	

		//--------------------------------------------------------------------------------
		// EastAsianLayout 17.3.2.10 (Part 1)
		//--------------------------------------------------------------------------------

		class CEastAsianLayout : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CEastAsianLayout)
			CEastAsianLayout();
			virtual ~CEastAsianLayout();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff             > m_oCombine;
			nullable<SimpleTypes::CCombineBrackets   > m_oCombineBrackets;
			nullable<SimpleTypes::CDecimalNumber	 > m_oID;
			nullable<SimpleTypes::COnOff             > m_oVert;
			nullable<SimpleTypes::COnOff             > m_oVertCompress;
		};

		//--------------------------------------------------------------------------------
		// Effect 17.3.2.11 (Part 1)
		//--------------------------------------------------------------------------------

		class CTextEffect : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextEffect)
			CTextEffect();
			virtual ~CTextEffect();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTextEffect> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Em 17.3.2.12 (Part 1)
		//--------------------------------------------------------------------------------

		class CEm : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CEm)
			CEm();
			virtual ~CEm();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CEm> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FitText 17.3.2.14 (Part 1)
		//--------------------------------------------------------------------------------

		class CFitText : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFitText)
			CFitText();
			virtual ~CFitText();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber  > m_oID;
			nullable<SimpleTypes::CTwipsMeasure   > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Highlight 17.3.2.14 (Part 1)
		//--------------------------------------------------------------------------------

		class CHighlight : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHighlight)
			CHighlight();
			virtual ~CHighlight();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHighlightColor> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// u (Underline) 17.3.2.40 (Part 1)
		//--------------------------------------------------------------------------------

		class CUnderline : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CUnderline)
			CUnderline();
			virtual ~CUnderline();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHexColor         > m_oColor;
			nullable<SimpleTypes::CThemeColor       > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber   > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber   > m_oThemeTint;
			nullable<SimpleTypes::CUnderline        > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// vertAlign (VerticalAlignRun) 17.3.2.40 (Part 1)
		//--------------------------------------------------------------------------------

		class CVerticalAlignRun : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CVerticalAlignRun)
			CVerticalAlignRun();
			virtual ~CVerticalAlignRun();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CVerticalAlignRun> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// w (TextScale) 17.3.2.40 (Part 1)
		//--------------------------------------------------------------------------------

		class CTextScale : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextScale)
			CTextScale();
			virtual ~CTextScale();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTextScale> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// framePr (FramePr) 17.3.1.11 (Part 1)
		//--------------------------------------------------------------------------------

		class CFramePr : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFramePr)
			CFramePr();
			virtual ~CFramePr();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff              > m_oAnchorLock;
			nullable<SimpleTypes::CDropCap            > m_oDropCap;
			nullable<SimpleTypes::CTwipsMeasure       > m_oH;
			nullable<SimpleTypes::CHAnchor            > m_oHAnchor;
			nullable<SimpleTypes::CHeightRule         > m_oHRule;
			nullable<SimpleTypes::CTwipsMeasure       > m_oHSpace;
			nullable<SimpleTypes::CDecimalNumber      > m_oLines;
			nullable<SimpleTypes::CVAnchor            > m_oVAnchor;
			nullable<SimpleTypes::CTwipsMeasure       > m_oVSpace;
			nullable<SimpleTypes::CTwipsMeasure       > m_oW;
			nullable<SimpleTypes::CWrap               > m_oWrap;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oX;
			nullable<SimpleTypes::CXAlign             > m_oXAlign;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oY;
			nullable<SimpleTypes::CYAlign             > m_oYAlign;
		};

		//--------------------------------------------------------------------------------
		// ind (Ind) 17.3.1.12 (Part 1) + 9.2.1.2 (Part 4)
		//--------------------------------------------------------------------------------

		class CInd : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CInd)
			CInd();
			virtual ~CInd();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

			static const CInd Merge(const CInd& oPrev, const CInd& oCurrent)
			{
				CInd oProperties;
				oProperties.m_oEnd = Merge(oPrev.m_oEnd, oCurrent.m_oEnd);
				oProperties.m_oEndChars = Merge(oPrev.m_oEndChars, oCurrent.m_oEndChars);
				oProperties.m_oFirstLine = Merge(oPrev.m_oFirstLine, oCurrent.m_oFirstLine);
				oProperties.m_oFirstLineChars = Merge(oPrev.m_oFirstLineChars, oCurrent.m_oFirstLineChars);
				oProperties.m_oHanging = Merge(oPrev.m_oHanging, oCurrent.m_oHanging);
				oProperties.m_oHangingChars = Merge(oPrev.m_oHangingChars, oCurrent.m_oHangingChars);
				oProperties.m_oStart = Merge(oPrev.m_oStart, oCurrent.m_oStart);
				oProperties.m_oStartChars = Merge(oPrev.m_oStartChars, oCurrent.m_oStartChars);
				return oProperties;
			}

			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if (oCurrent.IsInit())
					oResult = oCurrent;
				else if (oPrev.IsInit())
					oResult = oPrev;

				return oResult;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oEnd;
			nullable<SimpleTypes::CDecimalNumber      > m_oEndChars;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oFirstLine;
			nullable<SimpleTypes::CDecimalNumber      > m_oFirstLineChars;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oHanging;
			nullable<SimpleTypes::CDecimalNumber      > m_oHangingChars;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oStart;
			nullable<SimpleTypes::CDecimalNumber      > m_oStartChars;
		};

		//--------------------------------------------------------------------------------
		// Spacing 17.3.1.33 (Part 1)
		//--------------------------------------------------------------------------------

		class CSpacing : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSpacing)
			CSpacing();
			virtual ~CSpacing();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

			static const CSpacing Merge(const CSpacing& oPrev, const CSpacing& oCurrent);

			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if (oCurrent.IsInit())
					oResult = oCurrent;
				else if (oPrev.IsInit())
					oResult = oPrev;

				return oResult;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTwipsMeasure       > m_oAfter;
			nullable<SimpleTypes::COnOff              > m_oAfterAutospacing;
			nullable<SimpleTypes::CDecimalNumber      > m_oAfterLines;
			nullable<SimpleTypes::CTwipsMeasure       > m_oBefore;
			nullable<SimpleTypes::COnOff              > m_oBeforeAutospacing;
			nullable<SimpleTypes::CDecimalNumber      > m_oBeforeLines;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oLine;
			nullable<SimpleTypes::CLineSpacingRule    > m_oLineRule;
		};

		//--------------------------------------------------------------------------------
		// TabStop 17.3.1.37 (Part 1)
		//--------------------------------------------------------------------------------

		class CTabStop : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTabStop)
			CTabStop();
			virtual ~CTabStop();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTabTlc             > m_oLeader;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oPos;
			nullable<SimpleTypes::CTabJc              > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TextAlignment 17.3.1.39 (Part 1)
		//--------------------------------------------------------------------------------

		class CTextAlignment : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextAlignment)
			CTextAlignment();
			virtual ~CTextAlignment();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTextAlignment> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TextboxTightWrap 17.3.1.40 (Part 1)
		//--------------------------------------------------------------------------------

		class CTextboxTightWrap : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextboxTightWrap)
			CTextboxTightWrap();
			virtual ~CTextboxTightWrap();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTextboxTightWrap> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Ligatures
		//--------------------------------------------------------------------------------

		class CLigatures : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLigatures)
			
			CLigatures();
			virtual ~CLigatures();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CLigatures> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// NumSpacing
		//--------------------------------------------------------------------------------

		class CNumSpacing : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CNumSpacing)

			CNumSpacing();
			virtual ~CNumSpacing();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CNumSpacing> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// NumForm
		//--------------------------------------------------------------------------------

		class CNumForm : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CNumForm)

			CNumForm();
			virtual ~CNumForm();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CNumForm> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// StylisticSet
		//--------------------------------------------------------------------------------

		class CStylisticSet : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CStylisticSet)

			CStylisticSet();
			virtual ~CStylisticSet();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oId;
			nullable<SimpleTypes::COnOff>					m_oVal;
		};

	} // Word
//-------------------------------------------------------------------------------------------------------------------------

	namespace Drawing
    {
        // ---------------------------------------------------------------------------
        // Offset (Point2D) 20.1.7.4
        // ---------------------------------------------------------------------------

        class CPoint2D : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPoint2D)
			CPoint2D();
			virtual ~CPoint2D();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CCoordinate m_oX;
			SimpleTypes::CCoordinate m_oY;
		};

        // -----------------------------------------------------------------------
        // Extent (PositiveSize2D) 20.4.2.7
        // -----------------------------------------------------------------------

        class CPositiveSize2D : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPositiveSize2D)
			CPositiveSize2D();
			virtual ~CPositiveSize2D();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:			
			SimpleTypes::CPositiveCoordinate m_oCx;
			SimpleTypes::CPositiveCoordinate m_oCy;
		};

    } // Drawing
} // ComplexTypes
