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

		virtual void    FromXML(XmlUtils::CXmlNode& oNode) = 0;
        virtual std::wstring ToString() const                   = 0;
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
				sResult += L"val=\"" + m_oVal->ToString() + L"\"" ;
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

		nullable<SimpleTypes::CDecimalNumber<> > m_oVal;
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode,  _T("w:color"),      m_oColor );
				XmlMacroReadAttributeBase(oNode,  _T("w:frame"),      m_oFrame );
				XmlMacroReadAttributeBase(oNode,  _T("w:shadow"),     m_oShadow );
				XmlMacroReadAttributeBase(oNode,  _T("w:space"),      m_oSpace );
				XmlMacroReadAttributeBase(oNode,  _T("w:sz"),         m_oSz );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeColor"), m_oThemeColor );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeShade"), m_oThemeShade );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeTint"),  m_oThemeTint );
				XmlMacroReadAttributeBase(oNode,  _T("w:val"),        m_oVal );
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
					sResult += _T("w:color=\"");
					sResult += m_oColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oFrame.IsInit() )
				{
					sResult += _T("w:frame=\"");
					sResult += m_oFrame->ToString();
					sResult += _T("\" ");
				}

				if ( m_oShadow.IsInit() )
				{
					sResult += _T("w:shadow=\"");
					sResult += m_oShadow->ToString();
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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:color"),      m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:frame"),      m_oFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:shadow"),     m_oShadow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:space"),      m_oSpace )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:sz"),         m_oSz )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeColor"), m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeShade"), m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeTint"),  m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),        m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CHexColor<>           > m_oColor;
			nullable<SimpleTypes::COnOff<>              > m_oFrame;
			nullable<SimpleTypes::COnOff<>              > m_oShadow;
			nullable<SimpleTypes::CPointMeasure<>       > m_oSpace;
			nullable<SimpleTypes::CEighthPointMeasure<> > m_oSz;
			nullable<SimpleTypes::CThemeColor<>         > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeTint;
			nullable<SimpleTypes::CBorder<>             > m_oVal;

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
				XmlMacroReadAttributeBase(oNode,  _T("w:bidi"),     m_oBidi );
				XmlMacroReadAttributeBase(oNode,  _T("w:eastAsia"), m_oEastAsia );
				XmlMacroReadAttributeBase(oNode,  _T("w:val"),      m_oVal );
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

				if ( m_oBidi.IsInit() )
				{
					sResult += _T("w:bidi=\"");
					sResult += m_oBidi->ToString();
					sResult += _T("\" ");
				}

				if ( m_oEastAsia.IsInit() )
				{
					sResult += _T("w:eastAsia=\"");
					sResult += m_oEastAsia->ToString();
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

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:bidi"),     m_oBidi )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:eastAsia"), m_oEastAsia )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),      m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CLang> m_oBidi;
			nullable<SimpleTypes::CLang> m_oEastAsia;
			nullable<SimpleTypes::CLang> m_oVal;
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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

			nullable<SimpleTypes::CLang> m_oVal;
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode,  _T("w:color"),          m_oColor );
				XmlMacroReadAttributeBase(oNode,  _T("w:fill"),           m_oFill );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeColor"),     m_oThemeColor );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeFill"),      m_oThemeFill );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeFillShade"), m_oThemeFillShade );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeFillTint"),  m_oThemeFillTint );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeShade"),     m_oThemeShade );
				XmlMacroReadAttributeBase(oNode,  _T("w:themeTint"),      m_oThemeTint );
				XmlMacroReadAttributeBase(oNode,  _T("w:val"),            m_oVal );
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
					sResult += _T("w:color=\"");
					sResult += m_oColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oFill.IsInit() )
				{
					sResult += _T("w:fill=\"");
					sResult += m_oFill->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeColor.IsInit() )
				{
					sResult += _T("w:themeColor=\"");
					sResult += m_oThemeColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeFill.IsInit() )
				{
					sResult += _T("w:themeFill=\"");
					sResult += m_oThemeFill->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeFillShade.IsInit() )
				{
					sResult += _T("w:themeFillShade=\"");
					sResult += m_oThemeFillShade->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeFillTint.IsInit() )
				{
					sResult += _T("w:themeFillTint=\"");
					sResult += m_oThemeFillTint->ToString();
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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:color"),          m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:fill"),           m_oFill )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeColor"),     m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeFill"),      m_oThemeFill )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeFillShade"), m_oThemeFillShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeFillTint"),  m_oThemeFillTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeShade"),     m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeTint"),      m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),            m_oVal )
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
				XmlMacroReadAttributeBase( oNode, _T("w:type"), m_oType );
				XmlMacroReadAttributeBase( oNode, _T("w:w"),    m_oW );
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
					sResult += _T("w:type=\"");
					sResult += m_oType->ToString();
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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:type"), m_oType )
				else if ( _T("w:w") == wsName )
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

			nullable<SimpleTypes::CTblWidth<>            > m_oType;
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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

				sResult += _T("w:val=\"");
				sResult += m_oVal.ToString();
				sResult += _T("\" ");

				return sResult;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_sVal );
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

				if ( m_sVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += XmlUtils::EncodeXmlString(m_sVal.get2(), false);
					sResult += _T("\" ");
				}

				return sResult;
			}
            std::wstring ToString2() const
			{
                std::wstring sResult;

				if ( m_sVal.IsInit() )
					sResult += m_sVal.get();

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_sVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

            nullable<std::wstring> m_sVal;
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += std::to_wstring(m_oVal->ToHps());
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:themeColor"), m_oThemeColor );
				XmlMacroReadAttributeBase( oNode, _T("w:themeShade"), m_oThemeShade );
				XmlMacroReadAttributeBase( oNode, _T("w:themeTint"),  m_oThemeTint );
				XmlMacroReadAttributeBase( oNode, _T("w:val"),        m_oVal );
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
					sResult += m_oVal->ToStringNoAlpha();
					sResult += _T("\" ");
				}

				sResult;

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:themeColor"), m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeShade"), m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeTint"),  m_oThemeTint )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),        m_oVal )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, _T("w:format"), m_sFormat );
				XmlMacroReadAttributeBase(oNode, _T("w:val"),    m_oVal );
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

				if ( m_sFormat.IsInit() )
				{
					sResult += _T("w:format=\"");
                    sResult += m_sFormat.get2();
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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:format"), m_sFormat )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),    m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

            nullable<std::wstring                      > m_sFormat;
			nullable<SimpleTypes::CNumberFormat<> > m_oVal;
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode,  _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
					sResult += L"w:val=\"" + m_oVal->ToString() + L"\" ";
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("r:id"), m_oId );
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
					sResult += _T("r:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("r:id"), m_oId )
				WritingElement_ReadAttributes_End( oReader )
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:evenHBand"),           m_oEvenHBand );
				XmlMacroReadAttributeBase( oNode, _T("w:evenVBand"),           m_oEvenVBand );
				XmlMacroReadAttributeBase( oNode, _T("w:firstColumn"),         m_oFirstColumn );
				XmlMacroReadAttributeBase( oNode, _T("w:firstRow"),            m_oFirstRow );
				XmlMacroReadAttributeBase( oNode, _T("w:firstRowFirstColumn"), m_oFirstRowFirstColumn );
				XmlMacroReadAttributeBase( oNode, _T("w:firstRowLastColumn"),  m_oFirstRowLastColumn );
				XmlMacroReadAttributeBase( oNode, _T("w:lastColumn"),          m_oLastColumn );
				XmlMacroReadAttributeBase( oNode, _T("w:lastRow"),             m_oLastRow );
				XmlMacroReadAttributeBase( oNode, _T("w:lastRowFirstColumn"),  m_oLastRowFirstColumn );
				XmlMacroReadAttributeBase( oNode, _T("w:lastRowLastColumn"),   m_oLastRowLastColumn );
				XmlMacroReadAttributeBase( oNode, _T("w:oddHBand"),            m_oOddHBand );
				XmlMacroReadAttributeBase( oNode, _T("w:oddVBand"),            m_oOddVBand );
				XmlMacroReadAttributeBase( oNode, _T("w:val"),                 m_oVal );
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

				if ( m_oEvenHBand.IsInit() )
				{
					sResult += _T("w:evenHBand=\"");
					sResult += m_oEvenHBand->ToString();
					sResult += _T("\" ");
				}

				if ( m_oEvenVBand.IsInit() )
				{
					sResult += _T("w:evenVBand=\"");
					sResult += m_oEvenVBand->ToString();
					sResult += _T("\" ");
				}

				if ( m_oFirstColumn.IsInit() )
				{
					sResult += _T("w:firstColumn=\"");
					sResult += m_oFirstColumn->ToString();
					sResult += _T("\" ");
				}

				if ( m_oFirstRow.IsInit() )
				{
					sResult += _T("w:firstRow=\"");
					sResult += m_oFirstRow->ToString();
					sResult += _T("\" ");
				}

				if ( m_oFirstRowFirstColumn.IsInit() )
				{
					sResult += _T("w:firstRowFirstColumn=\"");
					sResult += m_oFirstRowFirstColumn->ToString();
					sResult += _T("\" ");
				}

				if ( m_oFirstRowLastColumn.IsInit() )
				{
					sResult += _T("w:firstRowLastColumn=\"");
					sResult += m_oFirstRowLastColumn->ToString();
					sResult += _T("\" ");
				}

				if ( m_oLastColumn.IsInit() )
				{
					sResult += _T("w:lastColumn=\"");
					sResult += m_oLastColumn->ToString();
					sResult += _T("\" ");
				}

				if ( m_oLastRow.IsInit() )
				{
					sResult += _T("w:lastRow=\"");
					sResult += m_oLastRow->ToString();
					sResult += _T("\" ");
				}

				if ( m_oLastRowFirstColumn.IsInit() )
				{
					sResult += _T("w:lastRowFirstColumn=\"");
					sResult += m_oLastRowFirstColumn->ToString();
					sResult += _T("\" ");
				}

				if ( m_oLastRowLastColumn.IsInit() )
				{
					sResult += _T("w:lastRowLastColumn=\"");
					sResult += m_oLastRowLastColumn->ToString();
					sResult += _T("\" ");
				}

				if ( m_oOddHBand.IsInit() )
				{
					sResult += _T("w:oddHBand=\"");
					sResult += m_oOddHBand->ToString();
					sResult += _T("\" ");
				}

				if ( m_oOddVBand.IsInit() )
				{
					sResult += _T("w:oddVBand=\"");
					sResult += m_oOddVBand->ToString();
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
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:evenHBand"),           m_oEvenHBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:evenVBand"),           m_oEvenVBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:firstColumn"),         m_oFirstColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:firstRow"),            m_oFirstRow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:firstRowFirstColumn"), m_oFirstRowFirstColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:firstRowLastColumn"),  m_oFirstRowLastColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:lastColumn"),          m_oLastColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:lastRow"),             m_oLastRow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:lastRowFirstColumn"),  m_oLastRowFirstColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:lastRowLastColumn"),   m_oLastRowLastColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:oddHBand"),            m_oOddHBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:oddVBand"),            m_oOddVBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),                 m_oVal )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				sResult;

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
				XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate );
				XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
				XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );
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

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate  )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
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

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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

				ComplexTypes_WriteAttribute( _T("w:val=\""), m_oVal );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
				return _T("");
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
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
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
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
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CFtnPos<> > m_oVal;
		};

	} // Word
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

		public:
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("x"), m_oX );
				XmlMacroReadAttributeBase( oNode, _T("y"), m_oY );
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

				sResult += _T("x=\"") + m_oX.ToString() + _T("\" ");
				sResult += _T("y=\"") + m_oY.ToString() + _T("\" ");

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("x"), m_oX )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("y"), m_oY )
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

		public:

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("cx"), m_oCx );
				XmlMacroReadAttributeBase( oNode, _T("cy"), m_oCy );
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

				sResult += _T("cx=\"") + m_oCx.ToString() + _T("\" ");
				sResult += _T("cy=\"") + m_oCy.ToString() + _T("\" ");

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("cx"), m_oCx )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("cy"), m_oCy )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:			
			SimpleTypes::CPositiveCoordinate<> m_oCx;
			SimpleTypes::CPositiveCoordinate<> m_oCy;
		}; // Drawing
    }
} // ComplexTypes
