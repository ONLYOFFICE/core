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

	class ComplexType
	{
	public:
		ComplexType(){};
		virtual ~ComplexType() {};

		virtual void FromXML(XmlUtils::CXmlNode& oNode) = 0;
        virtual std::wstring ToString() const = 0;
		
		virtual inline std::wstring ValNode(const std::wstring &node_name)
		{
			return L"<" + node_name + L" " + ToString() + L"/>";
		}
	};
	//--------------------------------------------------------------------------------
	// DecimalNumber 17.3.1.10 (Part 1)
	//--------------------------------------------------------------------------------
	class CDecimalNumber : public ComplexType
	{
	public:
		ComplexTypes_AdditionConstructors(CDecimalNumber)
		CDecimalNumber()
		{
		}
		virtual ~CDecimalNumber()
		{
		}

		virtual void FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"val", m_oVal );
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

			if ( m_oVal.IsInit() )
			{
				sResult += L"val=\"" + std::to_wstring(*m_oVal) + L"\"" ;
			}
			return sResult;
		}
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"val", m_oVal )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
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
			CBorder()
			{
			}
			virtual ~CBorder()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode,  L"w:color",      m_oColor );
				XmlMacroReadAttributeBase(oNode,  L"w:frame",      m_oFrame );
				XmlMacroReadAttributeBase(oNode,  L"w:shadow",     m_oShadow );
				XmlMacroReadAttributeBase(oNode,  L"w:space",      m_oSpace );
				XmlMacroReadAttributeBase(oNode,  L"w:sz",         m_oSz );
				XmlMacroReadAttributeBase(oNode,  L"w:themeColor", m_oThemeColor );
				XmlMacroReadAttributeBase(oNode,  L"w:themeShade", m_oThemeShade );
				XmlMacroReadAttributeBase(oNode,  L"w:themeTint",  m_oThemeTint );
				XmlMacroReadAttributeBase(oNode,  L"w:val",        m_oVal );
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
					sResult += L"w:color=\"";
					sResult += m_oColor->ToStringNoAlpha();
					sResult += L"\" ";
				}
				if ( m_oFrame.IsInit() )
				{
					sResult += L"w:frame=\"";
					sResult += m_oFrame->ToString();
					sResult += L"\" ";
				}
				if ( m_oShadow.IsInit() )
				{
					sResult += L"w:shadow=\"";
					sResult += m_oShadow->ToString();
					sResult += L"\" ";
				}
				if ( m_oSpace.IsInit() )
				{
					sResult += L"w:space=\"";
					sResult += m_oSpace->ToString();
					sResult += L"\" ";
				}
				if ( m_oSz.IsInit() )
				{
					sResult += L"w:sz=\"";
					sResult += m_oSz->ToString();
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
				nullable<SimpleTypes::CSignedTwipsMeasure> m_oWidth;
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"w:color",      m_oColor )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:frame",      m_oFrame )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:shadow",     m_oShadow )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:space",      m_oSpace )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:sz",         m_oSz )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeColor", m_oThemeColor )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeShade", m_oThemeShade )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeTint",  m_oThemeTint )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",        m_oVal )
					WritingElement_ReadAttributes_Read_else_if(oReader,  L"wx:bdrwidth",  m_oWidth)
				WritingElement_ReadAttributes_End( oReader )

				if (m_oWidth.IsInit())
				{
					m_oSz.Init();  m_oSz->SetValue((int)(m_oWidth->GetValue() * 8));
				}
			}
		public:
			nullable<SimpleTypes::CHexColor<>>			m_oColor;
			nullable<SimpleTypes::COnOff<>>				m_oFrame;
			nullable<SimpleTypes::COnOff<>>				m_oShadow;
			nullable<SimpleTypes::CPointMeasure<>>		m_oSpace;
			nullable<SimpleTypes::CEighthPointMeasure<>> m_oSz;
			nullable<SimpleTypes::CThemeColor<>>		m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber<>>	m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<>>	m_oThemeTint;
			nullable<SimpleTypes::CBorder<>>			m_oVal;
		};

		//--------------------------------------------------------------------------------
		// rFonts (Fonts) 17.3.2.20 (Part 1)
		//--------------------------------------------------------------------------------
		class CFonts : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFonts)
				CFonts()
			{
			}
			virtual ~CFonts()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:ascii", m_sAscii);
				XmlMacroReadAttributeBase(oNode, L"w:asciiTheme", m_oAsciiTheme);
				XmlMacroReadAttributeBase(oNode, L"w:cs", m_sCs);
				XmlMacroReadAttributeBase(oNode, L"w:cstheme", m_oCsTheme);
				XmlMacroReadAttributeBase(oNode, L"w:eastAsia", m_sEastAsia);
				XmlMacroReadAttributeBase(oNode, L"w:eastAsiaTheme", m_oEastAsiaTheme);
				XmlMacroReadAttributeBase(oNode, L"w:hAnsi", m_sHAnsi);
				XmlMacroReadAttributeBase(oNode, L"w:hAnsiTheme", m_oHAnsiTheme);
				XmlMacroReadAttributeBase(oNode, L"w:hint", m_oHint);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oHint.IsInit())
				{
					sResult += L"w:hint=\"";
					sResult += m_oHint->ToString();
					sResult += L"\" ";
				}
				if (m_sAscii.IsInit())
				{
					sResult += L"w:ascii=\"";
					sResult += m_sAscii.get2();
					sResult += L"\" ";
				}
				if (m_sHAnsi.IsInit())
				{
					sResult += L"w:hAnsi=\"";
					sResult += m_sHAnsi.get2();
					sResult += L"\" ";
				}
				if (m_sEastAsia.IsInit())
				{
					sResult += L"w:eastAsia=\"";
					sResult += m_sEastAsia.get2();
					sResult += L"\" ";
				}
				if (m_sCs.IsInit())
				{
					sResult += L"w:cs=\"";
					sResult += m_sCs.get2();
					sResult += L"\" ";
				}
				if (m_oAsciiTheme.IsInit())
				{
					sResult += L"w:asciiTheme=\"";
					sResult += m_oAsciiTheme->ToString();
					sResult += L"\" ";
				}
				if (m_oHAnsiTheme.IsInit())
				{
					sResult += L"w:hAnsiTheme=\"";
					sResult += m_oHAnsiTheme->ToString();
					sResult += L"\" ";
				}
				if (m_oEastAsiaTheme.IsInit())
				{
					sResult += L"w:eastAsiaTheme=\"";
					sResult += m_oEastAsiaTheme->ToString();
					sResult += L"\" ";
				}
				if (m_oCsTheme.IsInit())
				{
					sResult += L"w:cstheme=\"";
					sResult += m_oCsTheme->ToString();
					sResult += L"\" ";
				}
				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:ascii", m_sAscii)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:asciiTheme", m_oAsciiTheme)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:cs", m_sCs)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:cstheme", m_oCsTheme)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:eastAsia", m_sEastAsia)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:eastAsiaTheme", m_oEastAsiaTheme)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hAnsi", m_sHAnsi)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hAnsiTheme", m_oHAnsiTheme)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hint", m_oHint)
					//2003				
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:h-ansi", m_oHint)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:fareast", m_sEastAsia)
					WritingElement_ReadAttributes_End(oReader)
			}

		public:

			nullable<std::wstring>			m_sAscii;
			nullable<SimpleTypes::CTheme<>> m_oAsciiTheme;
			nullable<std::wstring>			m_sCs;
			nullable<SimpleTypes::CTheme<>> m_oCsTheme;
			nullable<std::wstring>			m_sEastAsia;
			nullable<SimpleTypes::CTheme<>> m_oEastAsiaTheme;
			nullable<std::wstring>			m_sHAnsi;
			nullable<SimpleTypes::CTheme<>> m_oHAnsiTheme;
			nullable<SimpleTypes::CHint<> > m_oHint;
		};

		//--------------------------------------------------------------------------------
		// Lang (Language) 17.3.2.20 (Part 1)
		//--------------------------------------------------------------------------------
		class CLanguage : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLanguage)
			CLanguage()
			{
			}
			virtual ~CLanguage()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode,  L"w:bidi",     m_oBidi );
				XmlMacroReadAttributeBase(oNode,  L"w:eastAsia", m_oEastAsia );
				XmlMacroReadAttributeBase(oNode,  L"w:val",      m_oVal );
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

				if (m_oVal.IsInit())
				{
					sResult += L"w:val=\"";
					sResult += *m_oVal;
					sResult += L"\" ";
				}
				if (m_oEastAsia.IsInit())
				{
					sResult += L"w:eastAsia=\"";
					sResult += *m_oEastAsia;
					sResult += L"\" ";
				}
				if ( m_oBidi.IsInit() )
				{
					sResult += L"w:bidi=\"";
					sResult += *m_oBidi;
					sResult += L"\" ";
				}

				return sResult;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:bidi",     m_oBidi )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:eastAsia", m_oEastAsia )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",      m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
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
			CLang()
			{
			}
			virtual ~CLang()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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

				if ( m_oVal.IsInit() )
				{
					sResult += L"w:val=\"";
					sResult += *m_oVal;
					sResult += L"\" ";
				}
				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
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
			CShading()
			{
			}
			virtual ~CShading()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode,  L"w:color",          m_oColor );
				XmlMacroReadAttributeBase(oNode,  L"w:fill",           m_oFill );
				XmlMacroReadAttributeBase(oNode,  L"w:themeColor",     m_oThemeColor );
				XmlMacroReadAttributeBase(oNode,  L"w:themeFill",      m_oThemeFill );
				XmlMacroReadAttributeBase(oNode,  L"w:themeFillShade", m_oThemeFillShade );
				XmlMacroReadAttributeBase(oNode,  L"w:themeFillTint",  m_oThemeFillTint );
				XmlMacroReadAttributeBase(oNode,  L"w:themeShade",     m_oThemeShade );
				XmlMacroReadAttributeBase(oNode,  L"w:themeTint",      m_oThemeTint );
				XmlMacroReadAttributeBase(oNode,  L"w:val",            m_oVal );
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

				if (m_oVal.IsInit())
				{
					sResult += L"w:val=\"";
					sResult += m_oVal->ToString();
					sResult += L"\" ";
				}
				if ( m_oColor.IsInit() )
				{
					sResult += L"w:color=\"";
					sResult += m_oColor->ToStringNoAlpha();
					sResult += L"\" ";
				}
				if ( m_oThemeColor.IsInit() )
				{
					sResult += L"w:themeColor=\"";
					sResult += m_oThemeColor->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeTint.IsInit())
				{
					sResult += L"w:themeTint=\"";
					sResult += m_oThemeTint->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeShade.IsInit())
				{
					sResult += L"w:themeShade=\"";
					sResult += m_oThemeShade->ToString();
					sResult += L"\" ";
				}
				if (m_oFill.IsInit())
				{
					sResult += L"w:fill=\"";
					sResult += m_oFill->ToStringNoAlpha();
					sResult += L"\" ";
				}
				if (m_oThemeFill.IsInit())
				{
					sResult += L"w:themeFill=\"";
					sResult += m_oThemeFill->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeFillTint.IsInit())
				{
					sResult += L"w:themeFillTint=\"";
					sResult += m_oThemeFillTint->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeFillShade.IsInit())
				{
					sResult += L"w:themeFillShade=\"";
					sResult += m_oThemeFillShade->ToString();
					sResult += L"\" ";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:color",          m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:fill",           m_oFill )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeColor",     m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeFill",      m_oThemeFill )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeFillShade", m_oThemeFillShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeFillTint",  m_oThemeFillTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeShade",     m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeTint",      m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",            m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CHexColor<>           > m_oColor;
			nullable<SimpleTypes::CHexColor<>           > m_oFill;
			nullable<SimpleTypes::CThemeColor<>         > m_oThemeColor;
			nullable<SimpleTypes::CThemeColor<>         > m_oThemeFill;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeFillShade;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeFillTint;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeTint;
			nullable<SimpleTypes::CShd<>                > m_oVal;
		};


		//--------------------------------------------------------------------------------
		// TblWidth 17.4.88 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblWidth : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblWidth)
			CTblWidth()
			{
			}
			virtual ~CTblWidth()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );
				XmlMacroReadAttributeBase( oNode, L"w:w",    m_oW );
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

				if ( m_oType.IsInit() )
				{
					sResult += L"w:type=\"";
					sResult += m_oType->ToString();
					sResult += L"\" ";
				}

				if ( m_oW.IsInit() )
				{
					sResult += L"w:w=\"";
					sResult += m_oW->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:type", m_oType )
				else if ( L"w:w" == wsName )
				{
					//bug 42023 w:w="460.60pt" (EP_C2crapportd_aptitudeprofessionnelle.docx)
					SimpleTypes::CUniversalMeasureOrPercent oUniversalMeasure;
                    std::wstring sValue = oReader.GetText();
					oUniversalMeasure.FromString(sValue);
					m_oW.Init();
					if(oUniversalMeasure.IsUnits())
					{
						m_oType.Init();
						m_oType->SetValue(SimpleTypes::tblwidthDxa);
						m_oW->SetValue(oUniversalMeasure.ToTwips());
						//ignore type attribute
						break;
					}
					else if(oUniversalMeasure.IsPercent())
					{
						m_oType.Init();
						m_oType->SetValue(SimpleTypes::tblwidthPct);
						m_oW->SetPercent(true);
						m_oW->SetValue(oUniversalMeasure.GetValue());
						//ignore type attribute
						break;
					}
					else
					{
						m_oW->SetValue(oUniversalMeasure.GetValue());
					}
				}
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CTblWidth<>> m_oType;
			nullable<SimpleTypes::CDecimalNumberOrPercent> m_oW;
		};


		//--------------------------------------------------------------------------------
		// OnOff 17.17.4 (Part 1)
		//--------------------------------------------------------------------------------
		class COnOff : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(COnOff)
			COnOff()
			{
			}
			virtual ~COnOff()
			{
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::COnOff<>> m_oVal;
		};

		template<SimpleTypes::EOnOff eDefValue = SimpleTypes::onoffTrue>
		class COnOff2 : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(COnOff2)
			COnOff2()
			{
			}
			virtual ~COnOff2()
			{
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
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

				sResult += L"w:val=\"" + m_oVal.ToString() + L"\" ";

				return sResult;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"val", m_oVal )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
			virtual inline std::wstring ValNode(const std::wstring &node_name)
			{
				if (m_oVal.ToBool())
					return L"<" + node_name + L"/>";
				else 
					return L"<" + node_name + L" w:val=\"0\"/>";
				return L"";
			}
		public:
			SimpleTypes::COnOff<eDefValue> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// String 17.3.1.27  (Part 1)
		//--------------------------------------------------------------------------------
        class String : public ComplexType
		{
		public:
            ComplexTypes_AdditionConstructors(String)
            String()
			{
			}
            virtual ~String()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				std::vector<std::wstring > attNames, attValues;
				oNode.GetAllAttributes(attNames,attValues);
				for(size_t i = 0; i < attNames.size(); ++i)
				{
					if(XmlUtils::GetNameNoNS(attNames[i]) == L"val")
					{
						m_sVal = attValues[i];
					}
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
				return ToStringWithNS(L"w:");
			}
			virtual std::wstring ToStringWithNS(const std::wstring& ns) const
			{
				std::wstring sResult;

				if ( m_sVal.IsInit() )
				{
					sResult += ns;
					sResult += L"val=\"";
					sResult += XmlUtils::EncodeXmlString(*m_sVal, false);
					sResult += L"\" ";
				}
				return sResult;
			}
            std::wstring ToString2() const
			{
                std::wstring sResult;

				if ( m_sVal.IsInit() )
					sResult += *m_sVal;

				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"val", m_sVal )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
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
			CSignedTwipsMeasure()
			{
			}
			virtual ~CSignedTwipsMeasure()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
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
			CHpsMeasure()
			{
			}
			virtual ~CHpsMeasure()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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

				if ( m_oVal.IsInit() )
				{
					sResult += L"w:val=\"";
					sResult += std::to_wstring(m_oVal->ToHps());
					sResult += L"\" ";
				}

				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
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
			CColor()
			{
			}
			virtual ~CColor()
			{
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:themeColor", m_oThemeColor );
				XmlMacroReadAttributeBase( oNode, L"w:themeShade", m_oThemeShade );
				XmlMacroReadAttributeBase( oNode, L"w:themeTint",  m_oThemeTint );
				XmlMacroReadAttributeBase( oNode, L"w:val",        m_oVal );
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

				if (m_oVal.IsInit())
				{
					sResult += L"w:val=\"";
					sResult += m_oVal->ToStringNoAlpha();
					sResult += L"\" ";
				}
				if ( m_oThemeColor.IsInit() )
				{
					sResult += L"w:themeColor=\"";
					sResult += m_oThemeColor->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeTint.IsInit())
				{
					sResult += L"w:themeTint=\"";
					sResult += m_oThemeTint->ToString();
					sResult += L"\" ";
				}
				if ( m_oThemeShade.IsInit() )
				{
					sResult += L"w:themeShade=\"";
					sResult += m_oThemeShade->ToString();
					sResult += L"\" ";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:themeColor", m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeShade", m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeTint",  m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",        m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CThemeColor<>         > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeTint;
			nullable<SimpleTypes::CHexColor<>           > m_oVal;
		};
		//--------------------------------------------------------------------------------
		// Position (HpsMeasure) 17.3.2.19 (Part 1)
		//--------------------------------------------------------------------------------
		class CSignedHpsMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSignedHpsMeasure)
			CSignedHpsMeasure()
			{
			}
			virtual ~CSignedHpsMeasure()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

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
			CNumFmt()
			{
			}
			virtual ~CNumFmt()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:format", m_sFormat );
				XmlMacroReadAttributeBase(oNode, L"w:val",    m_oVal );
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

				if ( m_sFormat.IsInit() )
				{
					sResult += L"w:format=\"";
                    sResult += m_sFormat.get2();
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:format", m_sFormat )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",    m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
            nullable<std::wstring> m_sFormat;
			nullable<SimpleTypes::CNumberFormat<>> m_oVal;
		};


		//--------------------------------------------------------------------------------
		// NumFmt 17.11.19 (Part 1)
		//--------------------------------------------------------------------------------
		class CNumRestart : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CNumRestart)
			CNumRestart()
			{
			}
			virtual ~CNumRestart()
			{
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode,  L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRestartNumber<> > m_oVal;
		};


		//--------------------------------------------------------------------------------
		// DecimalNumber 17.3.1.10 (Part 1)
		//--------------------------------------------------------------------------------
		class CDecimalNumber : public ComplexTypes::CDecimalNumber
		{
		public:
			ComplexTypes_AdditionConstructors(CDecimalNumber)
			CDecimalNumber()
			{
			}
			virtual ~CDecimalNumber()
			{
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ComplexTypes::CDecimalNumber::FromXML(oReader);
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
            virtual std::wstring ToString() const
			{
                std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += L"w:val=\"" + std::to_wstring(*m_oVal) + L"\" ";
				}

				return sResult;
			}
		};
		//--------------------------------------------------------------------------------
		// DecimalNumberOrPrecent 17.3.1.10 (Part 1)
		//--------------------------------------------------------------------------------
		class CDecimalNumberOrPrecent : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDecimalNumberOrPrecent)
			CDecimalNumberOrPrecent()
			{
			}
			virtual ~CDecimalNumberOrPrecent()
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
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
			CRel()
			{
			}
			virtual ~CRel()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"r:id", m_oId );
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

				if ( m_oId.IsInit() )
				{
					sResult += L"r:id=\"";
					sResult += m_oId->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"id", m_oId ) //r:id  or relationships:id
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
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
			CTextDirection()
			{
			}
			virtual ~CTextDirection()
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CTextDirection<> > m_oVal;
		};


		//--------------------------------------------------------------------------------
		// VerticalJc 17.6.23 (Part 1)
		//--------------------------------------------------------------------------------
		class CVerticalJc : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CVerticalJc)
			CVerticalJc()
			{
			}
			virtual ~CVerticalJc()
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CVerticalJc<> > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// cnfStyle (Cnf) 17.3.1.8 (Part 1) + 9.2.1.1 (Part 4)
		//--------------------------------------------------------------------------------
		class CCnf : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CCnf)
			CCnf()
			{
			}
			virtual ~CCnf()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:evenHBand",           m_oEvenHBand );
				XmlMacroReadAttributeBase( oNode, L"w:evenVBand",           m_oEvenVBand );
				XmlMacroReadAttributeBase( oNode, L"w:firstColumn",         m_oFirstColumn );
				XmlMacroReadAttributeBase( oNode, L"w:firstRow",            m_oFirstRow );
				XmlMacroReadAttributeBase( oNode, L"w:firstRowFirstColumn", m_oFirstRowFirstColumn );
				XmlMacroReadAttributeBase( oNode, L"w:firstRowLastColumn",  m_oFirstRowLastColumn );
				XmlMacroReadAttributeBase( oNode, L"w:lastColumn",          m_oLastColumn );
				XmlMacroReadAttributeBase( oNode, L"w:lastRow",             m_oLastRow );
				XmlMacroReadAttributeBase( oNode, L"w:lastRowFirstColumn",  m_oLastRowFirstColumn );
				XmlMacroReadAttributeBase( oNode, L"w:lastRowLastColumn",   m_oLastRowLastColumn );
				XmlMacroReadAttributeBase( oNode, L"w:oddHBand",            m_oOddHBand );
				XmlMacroReadAttributeBase( oNode, L"w:oddVBand",            m_oOddVBand );
				XmlMacroReadAttributeBase( oNode, L"w:val",                 m_oVal );
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

				if ( m_oEvenHBand.IsInit() )
				{
					sResult += L"w:evenHBand=\"";
					sResult += m_oEvenHBand->ToString();
					sResult += L"\" ";
				}
				if ( m_oEvenVBand.IsInit() )
				{
					sResult += L"w:evenVBand=\"";
					sResult += m_oEvenVBand->ToString();
					sResult += L"\" ";
				}
				if ( m_oFirstColumn.IsInit() )
				{
					sResult += L"w:firstColumn=\"";
					sResult += m_oFirstColumn->ToString();
					sResult += L"\" ";
				}
				if ( m_oFirstRow.IsInit() )
				{
					sResult += L"w:firstRow=\"";
					sResult += m_oFirstRow->ToString();
					sResult += L"\" ";
				}
				if ( m_oFirstRowFirstColumn.IsInit() )
				{
					sResult += L"w:firstRowFirstColumn=\"";
					sResult += m_oFirstRowFirstColumn->ToString();
					sResult += L"\" ";
				}
				if ( m_oFirstRowLastColumn.IsInit() )
				{
					sResult += L"w:firstRowLastColumn=\"";
					sResult += m_oFirstRowLastColumn->ToString();
					sResult += L"\" ";
				}
				if ( m_oLastColumn.IsInit() )
				{
					sResult += L"w:lastColumn=\"";
					sResult += m_oLastColumn->ToString();
					sResult += L"\" ";
				}
				if ( m_oLastRow.IsInit() )
				{
					sResult += L"w:lastRow=\"";
					sResult += m_oLastRow->ToString();
					sResult += L"\" ";
				}
				if ( m_oLastRowFirstColumn.IsInit() )
				{
					sResult += L"w:lastRowFirstColumn=\"";
					sResult += m_oLastRowFirstColumn->ToString();
					sResult += L"\" ";
				}
				if ( m_oLastRowLastColumn.IsInit() )
				{
					sResult += L"w:lastRowLastColumn=\"";
					sResult += m_oLastRowLastColumn->ToString();
					sResult += L"\" ";
				}
				if ( m_oOddHBand.IsInit() )
				{
					sResult += L"w:oddHBand=\"";
					sResult += m_oOddHBand->ToString();
					sResult += L"\" ";
				}
				if ( m_oOddVBand.IsInit() )
				{
					sResult += L"w:oddVBand=\"";
					sResult += m_oOddVBand->ToString();
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:evenHBand",           m_oEvenHBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:evenVBand",           m_oEvenVBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:firstColumn",         m_oFirstColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:firstRow",            m_oFirstRow )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:firstRowFirstColumn", m_oFirstRowFirstColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:firstRowLastColumn",  m_oFirstRowLastColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:lastColumn",          m_oLastColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:lastRow",             m_oLastRow )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:lastRowFirstColumn",  m_oLastRowFirstColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:lastRowLastColumn",   m_oLastRowLastColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:oddHBand",            m_oOddHBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:oddVBand",            m_oOddVBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",                 m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:			
			nullable<SimpleTypes::COnOff<> > m_oEvenHBand;
			nullable<SimpleTypes::COnOff<> > m_oEvenVBand;
			nullable<SimpleTypes::COnOff<> > m_oFirstColumn;
			nullable<SimpleTypes::COnOff<> > m_oFirstRow;
			nullable<SimpleTypes::COnOff<> > m_oFirstRowFirstColumn;
			nullable<SimpleTypes::COnOff<> > m_oFirstRowLastColumn;
			nullable<SimpleTypes::COnOff<> > m_oLastColumn;
			nullable<SimpleTypes::COnOff<> > m_oLastRow;
			nullable<SimpleTypes::COnOff<> > m_oLastRowFirstColumn;
			nullable<SimpleTypes::COnOff<> > m_oLastRowLastColumn;
			nullable<SimpleTypes::COnOff<> > m_oOddHBand;
			nullable<SimpleTypes::COnOff<> > m_oOddVBand;
			nullable<SimpleTypes::CCnf<>   > m_oVal;
		};
		//--------------------------------------------------------------------------------
		// jc (Jc) 17.3.1.13 (Part 1)
		//--------------------------------------------------------------------------------
		class CJc : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CJc)
			CJc()
			{
			}
			virtual ~CJc()
			{
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CJc<> > m_oVal;
		};
		//--------------------------------------------------------------------------------
		// TrackChange 17.13.5.19 (Part 1)
		//--------------------------------------------------------------------------------
		class CTrackChange : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTrackChange)
			CTrackChange()
			{
			}
			virtual ~CTrackChange()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
				XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
				XmlMacroReadAttributeBase( oNode, L"w:id",     m_oId );
				XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );
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

				if ( m_oId.IsInit() )
				{
					sResult += L"w:id=\"";
					sResult += m_oId->ToString();
					sResult += L"\" ";
				}	
				if ( m_sAuthor.IsInit() )
				{
					sResult += L"w:author=\"";
                    sResult += m_sAuthor.get2();
					sResult += L"\" ";
				}
				if ( m_oDate.IsInit() )
				{
					sResult += L"w:date=\"";
					sResult += m_oDate->ToString();
					sResult += L"\" ";
				}
				if ( m_sUserId.IsInit() )
				{
					sResult += L"oouserid=\"";
                    sResult += m_sUserId.get2();
					sResult += L"\" ";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author", m_sAuthor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",   m_oDate  )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",     m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
            nullable<std::wstring> m_sAuthor;
			nullable<SimpleTypes::CDateTime> m_oDate;
			nullable<SimpleTypes::CDecimalNumber<>> m_oId;
            nullable<std::wstring> m_sUserId;
		};
		//--------------------------------------------------------------------------------
		// LongHexNumber 17.9.30 (Part 1)
		//--------------------------------------------------------------------------------
		class CLongHexNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLongHexNumber)
			CLongHexNumber()
			{
			}
			virtual ~CLongHexNumber()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
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

				ComplexTypes_WriteAttribute( L"w:val=\"", m_oVal );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CLongHexNumber<>> m_oVal;

		};
		//--------------------------------------------------------------------------------
		// UnsignedDecimalNumber 17.16.32 (Part 1)
		//--------------------------------------------------------------------------------
		class CUnsignedDecimalNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CUnsignedDecimalNumber)
			CUnsignedDecimalNumber()
			{
			}
			virtual ~CUnsignedDecimalNumber()
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TwipsMeasure 17.15.1.25 (Part 1)
		//--------------------------------------------------------------------------------
		class CTwipsMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTwipsMeasure)
			CTwipsMeasure()
			{
			}
			virtual ~CTwipsMeasure()
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

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
			CEmpty()
			{
			}
			virtual ~CEmpty()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
			}
            virtual std::wstring ToString() const
			{
				return L"";
			}
		};
		//--------------------------------------------------------------------------------
		// MatchSrc 17.17.2.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CMatchSrc : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CMatchSrc)
			CMatchSrc()
			{
			}
			virtual ~CMatchSrc()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::COnOff<>> m_oVal;
		};
		//--------------------------------------------------------------------------------
		// EdnPos 17.11.22 (Part 1)
		//--------------------------------------------------------------------------------
		class CEdnPos : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CEdnPos)
			CEdnPos()
			{
			}
			virtual ~CEdnPos()
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CEdnPos<> > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FtnPos 17.11.21 (Part 1)
		//--------------------------------------------------------------------------------
		class CFtnPos : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFtnPos)
			CFtnPos()
			{
			}
			virtual ~CFtnPos()
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CFtnPos<> > m_oVal;
		};
		class CDocPartGallery : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDocPartGallery)
			CDocPartGallery()
			{
			}
			virtual ~CDocPartGallery()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
				{
					sResult += L"w:val=\"" + m_oVal->ToString() + L"\" ";
				}

				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CDocPartGallery<>> m_oVal;
		};
		class CDocPartBehavior : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDocPartBehavior)
			
			CDocPartBehavior() {}
			virtual ~CDocPartBehavior() {}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
				{
					sResult += L"w:val=\"" + m_oVal->ToString() + L"\" ";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CDocPartBehavior<>> m_oVal;
		};	
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
				XmlMacroReadAttributeBase(oNode, L"w:combine", m_oCombine);
				XmlMacroReadAttributeBase(oNode, L"w:combineBrackets", m_oCombineBrackets);
				XmlMacroReadAttributeBase(oNode, L"w:id", m_oID);
				XmlMacroReadAttributeBase(oNode, L"w:vert", m_oVert);
				XmlMacroReadAttributeBase(oNode, L"w:vertCompress", m_oVertCompress);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oCombine.IsInit())
				{
					sResult += L"w:combine=\"";
					sResult += m_oCombine->ToString();
					sResult += L"\" ";
				}
				if (m_oCombineBrackets.IsInit())
				{
					sResult += L"w:combineBrackets=\"";
					sResult += m_oCombineBrackets->ToString();
					sResult += L"\" ";
				}
				if (m_oID.IsInit())
				{
					sResult += L"w:themeTint=\"";
					sResult += m_oID->ToString();
					sResult += L"\" ";
				}
				if (m_oVert.IsInit())
				{
					sResult += L"w:vert=\"";
					sResult += m_oVert->ToString();
					sResult += L"\" ";
				}
				if (m_oVertCompress.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:combine", m_oCombine)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:combineBrackets", m_oCombineBrackets)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:id", m_oID)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:vert", m_oVert)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:vertCompress", m_oVertCompress)
				WritingElement_ReadAttributes_End(oReader)
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
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
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
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
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
				XmlMacroReadAttributeBase(oNode, L"w:id", m_oID);
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oID.IsInit())
				{
					sResult += L"w:id=\"";
					sResult += m_oID->ToString();
					sResult += L"\" ";
				}
				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:id", m_oID)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
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
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
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
				XmlMacroReadAttributeBase(oNode, L"w:color", m_oColor);
				XmlMacroReadAttributeBase(oNode, L"w:themeColor", m_oThemeColor);
				XmlMacroReadAttributeBase(oNode, L"w:themeShade", m_oThemeShade);
				XmlMacroReadAttributeBase(oNode, L"w:themeTint", m_oThemeTint);
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oColor.IsInit())
				{
					sResult += L"w:color=\"";
					sResult += m_oColor->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeColor.IsInit())
				{
					sResult += L"w:themeColor=\"";
					sResult += m_oThemeColor->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeShade.IsInit())
				{
					sResult += L"w:themeShade=\"";
					sResult += m_oThemeShade->ToString();
					sResult += L"\" ";
				}
				if (m_oThemeTint.IsInit())
				{
					sResult += L"w:themeTint=\"";
					sResult += m_oThemeTint->ToString();
					sResult += L"\" ";
				}
				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:color", m_oColor)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeColor", m_oThemeColor)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeShade", m_oThemeShade)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeTint", m_oThemeTint)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
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
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
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
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CTextScale<> > m_oVal;
		};
		//--------------------------------------------------------------------------------
		// framePr (FramePr) 17.3.1.11 (Part 1)
		//--------------------------------------------------------------------------------
		class CFramePr : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFramePr)
			CFramePr()
			{
			}
			virtual ~CFramePr()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:anchorLock", m_oAnchorLock);
				XmlMacroReadAttributeBase(oNode, L"w:dropCap", m_oDropCap);
				XmlMacroReadAttributeBase(oNode, L"w:h", m_oH);
				XmlMacroReadAttributeBase(oNode, L"w:hAnchor", m_oHAnchor);
				XmlMacroReadAttributeBase(oNode, L"w:hRule", m_oHRule);
				XmlMacroReadAttributeBase(oNode, L"w:hSpace", m_oHSpace);
				XmlMacroReadAttributeBase(oNode, L"w:lines", m_oLines);
				XmlMacroReadAttributeBase(oNode, L"w:vAnchor", m_oVAnchor);
				XmlMacroReadAttributeBase(oNode, L"w:vSpace", m_oVSpace);
				XmlMacroReadAttributeBase(oNode, L"w:w", m_oW);
				XmlMacroReadAttributeBase(oNode, L"w:wrap", m_oWrap);
				XmlMacroReadAttributeBase(oNode, L"w:x", m_oX);
				XmlMacroReadAttributeBase(oNode, L"w:xAlign", m_oXAlign);
				XmlMacroReadAttributeBase(oNode, L"w:y", m_oY);
				XmlMacroReadAttributeBase(oNode, L"w:yAlign", m_oYAlign);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oAnchorLock.IsInit())
				{
					sResult += L"w:anchorLock=\"";
					sResult += m_oAnchorLock->ToString();
					sResult += L"\" ";
				}
				if (m_oDropCap.IsInit())
				{
					sResult += L"w:dropCap=\"";
					sResult += m_oDropCap->ToString();
					sResult += L"\" ";
				}
				if (m_oH.IsInit())
				{
					sResult += L"w:h=\"";
					sResult += m_oH->ToString();
					sResult += L"\" ";
				}
				if (m_oHAnchor.IsInit())
				{
					sResult += L"w:hAnchor=\"";
					sResult += m_oHAnchor->ToString();
					sResult += L"\" ";
				}
				if (m_oHRule.IsInit())
				{
					sResult += L"w:hRule=\"";
					sResult += m_oHRule->ToString();
					sResult += L"\" ";
				}
				if (m_oHSpace.IsInit())
				{
					sResult += L"w:hSpace=\"";
					sResult += m_oHSpace->ToString();
					sResult += L"\" ";
				}
				if (m_oLines.IsInit())
				{
					sResult += L"w:lines=\"";
					sResult += m_oLines->ToString();
					sResult += L"\" ";
				}
				if (m_oVAnchor.IsInit())
				{
					sResult += L"w:vAnchor=\"";
					sResult += m_oVAnchor->ToString();
					sResult += L"\" ";
				}
				if (m_oVSpace.IsInit())
				{
					sResult += L"w:vSpace=\"";
					sResult += m_oVSpace->ToString();
					sResult += L"\" ";
				}
				if (m_oW.IsInit())
				{
					sResult += L"w:w=\"";
					sResult += m_oW->ToString();
					sResult += L"\" ";
				}
				if (m_oWrap.IsInit())
				{
					sResult += L"w:wrap=\"";
					sResult += m_oWrap->ToString();
					sResult += L"\" ";
				}
				if (m_oX.IsInit())
				{
					sResult += L"w:x=\"";
					sResult += m_oX->ToString();
					sResult += L"\" ";
				}
				if (m_oXAlign.IsInit())
				{
					sResult += L"w:xAlign=\"";
					sResult += m_oXAlign->ToString();
					sResult += L"\" ";
				}
				if (m_oY.IsInit())
				{
					sResult += L"w:y=\"";
					sResult += m_oY->ToString();
					sResult += L"\" ";
				}
				if (m_oYAlign.IsInit())
				{
					sResult += L"w:yAlign=\"";
					sResult += m_oYAlign->ToString();
					sResult += L"\" ";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:anchorLock", m_oAnchorLock)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:dropCap", m_oDropCap)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:h", m_oH)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hAnchor", m_oHAnchor)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hRule", m_oHRule)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hSpace", m_oHSpace)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:lines", m_oLines)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:vAnchor", m_oVAnchor)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:vSpace", m_oVSpace)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:w", m_oW)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:wrap", m_oWrap)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:x", m_oX)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:xAlign", m_oXAlign)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:y", m_oY)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:yAlign", m_oYAlign)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::COnOff<>            > m_oAnchorLock;
			nullable<SimpleTypes::CDropCap<>          > m_oDropCap;
			nullable<SimpleTypes::CTwipsMeasure       > m_oH;
			nullable<SimpleTypes::CHAnchor<>          > m_oHAnchor;
			nullable<SimpleTypes::CHeightRule<>       > m_oHRule;
			nullable<SimpleTypes::CTwipsMeasure       > m_oHSpace;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oLines;
			nullable<SimpleTypes::CVAnchor<>          > m_oVAnchor;
			nullable<SimpleTypes::CTwipsMeasure       > m_oVSpace;
			nullable<SimpleTypes::CTwipsMeasure       > m_oW;
			nullable<SimpleTypes::CWrap<>             > m_oWrap;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oX;
			nullable<SimpleTypes::CXAlign<>           > m_oXAlign;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oY;
			nullable<SimpleTypes::CYAlign<>           > m_oYAlign;
		};
		//--------------------------------------------------------------------------------
		// ind (Ind) 17.3.1.12 (Part 1) + 9.2.1.2 (Part 4)
		//--------------------------------------------------------------------------------
		class CInd : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CInd)
				CInd()
			{
			}
			virtual ~CInd()
			{
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:end", m_oEnd);
				XmlMacroReadAttributeBase(oNode, L"w:endChars", m_oEndChars);
				XmlMacroReadAttributeBase(oNode, L"w:firstLine", m_oFirstLine);
				XmlMacroReadAttributeBase(oNode, L"w:firstLineChars", m_oFirstLineChars);
				XmlMacroReadAttributeBase(oNode, L"w:hanging", m_oHanging);
				XmlMacroReadAttributeBase(oNode, L"w:hangingChars", m_oHangingChars);
				XmlMacroReadAttributeBase(oNode, L"w:start", m_oStart);
				XmlMacroReadAttributeBase(oNode, L"w:startChars", m_oStartChars);

				// См. 9.2.1.2 Part4
				if (!m_oStart.IsInit())
					XmlMacroReadAttributeBase(oNode, L"w:left", m_oStart);
				if (!m_oStartChars.IsInit())
					XmlMacroReadAttributeBase(oNode, L"w:leftChars", m_oStartChars);
				if (!m_oEnd.IsInit())
					XmlMacroReadAttributeBase(oNode, L"w:right", m_oEnd);
				if (!m_oEndChars.IsInit())
					XmlMacroReadAttributeBase(oNode, L"w:rightChars", m_oEndChars);
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oEnd.IsInit())
				{
					sResult += L"w:end=\"";
					sResult += m_oEnd->ToString();
					sResult += L"\" ";
				}

				if (m_oEndChars.IsInit())
				{
					sResult += L"w:endChars=\"";
					sResult += m_oEndChars->ToString();
					sResult += L"\" ";
				}

				if (m_oFirstLine.IsInit())
				{
					sResult += L"w:firstLine=\"";
					sResult += m_oFirstLine->ToString();
					sResult += L"\" ";
				}

				if (m_oFirstLineChars.IsInit())
				{
					sResult += L"w:firstLineChars=\"";
					sResult += m_oFirstLineChars->ToString();
					sResult += L"\" ";
				}

				if (m_oHanging.IsInit())
				{
					sResult += L"w:hanging=\"";
					sResult += m_oHanging->ToString();
					sResult += L"\" ";
				}

				if (m_oHangingChars.IsInit())
				{
					sResult += L"w:hangingChars=\"";
					sResult += m_oHangingChars->ToString();
					sResult += L"\" ";
				}

				if (m_oStart.IsInit())
				{
					sResult += L"w:start=\"";
					sResult += m_oStart->ToString();
					sResult += L"\" ";
				}

				if (m_oStartChars.IsInit())
				{
					sResult += L"w:startChars=\"";
					sResult += m_oStartChars->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}

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
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:end", m_oEnd)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:endChars", m_oEndChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:firstLine", m_oFirstLine)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:firstLineChars", m_oFirstLineChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hanging", m_oHanging)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hangingChars", m_oHangingChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:start", m_oStart)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:startChars", m_oStartChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:left", m_oStart)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:leftChars", m_oStartChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:right", m_oEnd)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:rightChars", m_oEndChars)
					//2003
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:first-line", m_oFirstLine)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:first-line-chars", m_oFirstLineChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:hanging-chars", m_oHangingChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:left-chars", m_oStartChars)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:right-charss", m_oEndChars)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:

			nullable<SimpleTypes::CSignedTwipsMeasure > m_oEnd;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oEndChars;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oFirstLine;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oFirstLineChars;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oHanging;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oHangingChars;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oStart;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oStartChars;
		};

		//--------------------------------------------------------------------------------
		// Spacing 17.3.1.33 (Part 1)
		//--------------------------------------------------------------------------------
		class CSpacing : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSpacing)
			CSpacing()
			{
			}
			virtual ~CSpacing()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:after", m_oAfter);
				XmlMacroReadAttributeBase(oNode, L"w:afterAutospacing", m_oAfterAutospacing);
				XmlMacroReadAttributeBase(oNode, L"w:afterLines", m_oAfterLines);
				XmlMacroReadAttributeBase(oNode, L"w:before", m_oBefore);
				XmlMacroReadAttributeBase(oNode, L"w:beforeAutospacing", m_oBeforeAutospacing);
				XmlMacroReadAttributeBase(oNode, L"w:beforeLines", m_oBeforeLines);
				XmlMacroReadAttributeBase(oNode, L"w:line", m_oLine);
				XmlMacroReadAttributeBase(oNode, L"w:lineRule", m_oLineRule);
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oAfter.IsInit())
				{
					sResult += L"w:after=\"";
					sResult += m_oAfter->ToString();
					sResult += L"\" ";
				}

				if (m_oAfterAutospacing.IsInit())
				{
					sResult += L"w:afterAutospacing=\"" + std::wstring(m_oAfterAutospacing->ToBool() ? L"1" : L"0") + L"\" ";
				}

				if (m_oAfterLines.IsInit())
				{
					sResult += L"w:afterLines=\"";
					sResult += m_oAfterLines->ToString();
					sResult += L"\" ";
				}

				if (m_oBefore.IsInit())
				{
					sResult += L"w:before=\"";
					sResult += m_oBefore->ToString();
					sResult += L"\" ";
				}

				if (m_oBeforeAutospacing.IsInit())
				{
					sResult += L"w:beforeAutospacing=\"" + std::wstring(m_oBeforeAutospacing->ToBool() ? L"1" : L"0") + L"\" ";
				}

				if (m_oBeforeLines.IsInit())
				{
					sResult += L"w:beforeLines=\"";
					sResult += m_oBeforeLines->ToString();
					sResult += L"\" ";
				}

				if (m_oLine.IsInit())
				{
					sResult += L"w:line=\"";
					sResult += m_oLine->ToString();
					sResult += L"\" ";
				}

				if (m_oLineRule.IsInit())
				{
					sResult += L"w:lineRule=\"";
					sResult += m_oLineRule->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}

			static const CSpacing Merge(const CSpacing& oPrev, const CSpacing& oCurrent)
			{
				CSpacing oProperties;
				oProperties.m_oAfter = Merge(oPrev.m_oAfter, oCurrent.m_oAfter);
				oProperties.m_oAfterAutospacing = Merge(oPrev.m_oAfterAutospacing, oCurrent.m_oAfterAutospacing);
				oProperties.m_oAfterLines = Merge(oPrev.m_oAfterLines, oCurrent.m_oAfterLines);
				oProperties.m_oBefore = Merge(oPrev.m_oBefore, oCurrent.m_oBefore);
				oProperties.m_oBeforeAutospacing = Merge(oPrev.m_oBeforeAutospacing, oCurrent.m_oBeforeAutospacing);
				oProperties.m_oBeforeLines = Merge(oPrev.m_oBeforeLines, oCurrent.m_oBeforeLines);
				oProperties.m_oLine = Merge(oPrev.m_oLine, oCurrent.m_oLine);
				oProperties.m_oLineRule = Merge(oPrev.m_oLineRule, oCurrent.m_oLineRule);
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
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:after", m_oAfter)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:afterAutospacing", m_oAfterAutospacing)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:afterLines", m_oAfterLines)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:before", m_oBefore)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:beforeAutospacing", m_oBeforeAutospacing)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:beforeLines", m_oBeforeLines)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:line", m_oLine)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:lineRule", m_oLineRule)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:before-autospacing", m_oBeforeAutospacing)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:after-autospacing", m_oAfterAutospacing)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CTwipsMeasure       > m_oAfter;
			nullable<SimpleTypes::COnOff<>            > m_oAfterAutospacing;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oAfterLines;
			nullable<SimpleTypes::CTwipsMeasure       > m_oBefore;
			nullable<SimpleTypes::COnOff<>            > m_oBeforeAutospacing;
			nullable<SimpleTypes::CDecimalNumber<>    > m_oBeforeLines;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oLine;
			nullable<SimpleTypes::CLineSpacingRule<>  > m_oLineRule;
		};

		//--------------------------------------------------------------------------------
		// TabStop 17.3.1.37 (Part 1)
		//--------------------------------------------------------------------------------
		class CTabStop : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTabStop)
			CTabStop()
			{
			}
			virtual ~CTabStop()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:leader", m_oLeader);
				XmlMacroReadAttributeBase(oNode, L"w:pos", m_oPos);
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oLeader.IsInit())
				{
					sResult += L"w:leader=\"";
					sResult += m_oLeader->ToString();
					sResult += L"\" ";
				}
				if (m_oPos.IsInit())
				{
					sResult += L"w:pos=\"";
					sResult += m_oPos->ToString();
					sResult += L"\" ";
				}
				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w:leader", m_oLeader)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:pos", m_oPos)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CTabTlc<>           > m_oLeader;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oPos;
			nullable<SimpleTypes::CTabJc<>            > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TextAlignment 17.3.1.39 (Part 1)
		//--------------------------------------------------------------------------------
		class CTextAlignment : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextAlignment)
			CTextAlignment()
			{
			}
			virtual ~CTextAlignment()
			{
			}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
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
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CTextAlignment<>> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TextboxTightWrap 17.3.1.40 (Part 1)
		//--------------------------------------------------------------------------------
		class CTextboxTightWrap : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextboxTightWrap)
			CTextboxTightWrap()	{}
			virtual ~CTextboxTightWrap() {}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
				{
					sResult += L"w:val=\"";
					sResult += m_oVal->ToString();
					sResult += L"\"";
				}

				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CTextboxTightWrap<>> m_oVal;
		};

		class CLigatures : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLigatures)
			
			CLigatures() {}
			virtual ~CLigatures() {}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
				{
					sResult += L"w14:val=\"" + m_oVal->ToString() + L"\"";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w14:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CLigatures<>> m_oVal;
		};
		class CNumSpacing : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CNumSpacing)

			CNumSpacing() {}
			virtual ~CNumSpacing() {}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
				{
					sResult += L"w14:val=\"" + m_oVal->ToString() + L"\"";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w14:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CNumSpacing<>> m_oVal;
		};
		class CNumForm : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CNumForm)

			CNumForm() {}
			virtual ~CNumForm() {}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oVal.IsInit())
				{
					sResult += L"w14:val=\"" + m_oVal->ToString() + L"\"";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w14:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CNumForm<>> m_oVal;
		};
		class CStylisticSet : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CStylisticSet)
			CStylisticSet()	{}
			virtual ~CStylisticSet(){}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"w14:id", m_oId);
				XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if (m_oId.IsInit())
				{
					sResult += L"w14:id=\"";
					sResult += m_oId->ToString();
					sResult += L"\" ";
				}
				if (m_oVal.IsInit())
				{
					sResult += L"w14:val=\"";
					sResult += m_oVal->ToString();
					sResult += L"\" ";
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"w14:id", m_oId)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w14:val", m_oVal)
				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oId;
			nullable<SimpleTypes::COnOff<>>					m_oVal;
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
			CPoint2D()
			{
			}
			virtual ~CPoint2D()
			{
			}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"x", m_oX );
				XmlMacroReadAttributeBase( oNode, L"y", m_oY );
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

				sResult += L"x=\"" + m_oX.ToString() + L"\" ";
				sResult += L"y=\"" + m_oY.ToString() + L"\" ";

				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"x", m_oX )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"y", m_oY )
				WritingElement_ReadAttributes_End( oReader )
			}
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
			CPositiveSize2D()
			{
			}
			virtual ~CPositiveSize2D()
			{
			}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"cx", m_oCx );
				XmlMacroReadAttributeBase( oNode, L"cy", m_oCy );
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

				sResult += L"cx=\"" + m_oCx.ToString() + L"\" ";
				sResult += L"cy=\"" + m_oCy.ToString() + L"\" ";

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"cx", m_oCx )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"cy", m_oCy )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:			
			SimpleTypes::CPositiveCoordinate<> m_oCx;
			SimpleTypes::CPositiveCoordinate<> m_oCy;
		};
    } // Drawing
} // ComplexTypes
