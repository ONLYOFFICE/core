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

#include "ComplexTypes.h"

namespace ComplexTypes
{

	ComplexType::ComplexType() {};
	ComplexType::~ComplexType() {};

	std::wstring ComplexType::ValNode(const std::wstring &node_name)
	{
		return L"<" + node_name + L" " + ToString() + L"/>";
	}

//--------------------------------------------------------------------------------
// DecimalNumber 17.3.1.10 (Part 1)
//--------------------------------------------------------------------------------

	CDecimalNumber::CDecimalNumber()
	{
	}
	CDecimalNumber::~CDecimalNumber()
	{
	}

	void CDecimalNumber::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"val", m_oVal );
	}
	void CDecimalNumber::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CDecimalNumber::ToString() const
	{
		std::wstring sResult;

		if ( m_oVal.IsInit() )
		{
			sResult += L"val=\"" + std::to_wstring(*m_oVal) + L"\"" ;
		}
		return sResult;
	}
	void CDecimalNumber::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"val", m_oVal )
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}

namespace Word
{
//--------------------------------------------------------------------------------
// Border 17.3.4 (Part 1)
//--------------------------------------------------------------------------------

	CBorder::CBorder()
	{
	}
	CBorder::~CBorder()
	{
	}

	void CBorder::FromXML(XmlUtils::CXmlNode& oNode)
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
	void CBorder::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CBorder::ToString() const
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

	void CBorder::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

//--------------------------------------------------------------------------------
// rFonts (Fonts) 17.3.2.20 (Part 1)
//--------------------------------------------------------------------------------

	CFonts::CFonts()
	{
	}
	CFonts::~CFonts()
	{
	}

	void    CFonts::FromXML(XmlUtils::CXmlNode& oNode)
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
	void    CFonts::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CFonts::ToString() const
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

	void CFonts::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:h-ansi", m_sHAnsi)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:fareast", m_sEastAsia)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// Lang (Language) 17.3.2.20 (Part 1)
//--------------------------------------------------------------------------------

	CLanguage::CLanguage()
	{
	}
	CLanguage::~CLanguage()
	{
	}

	void    CLanguage::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode,  L"w:bidi",     m_oBidi );
		XmlMacroReadAttributeBase(oNode,  L"w:eastAsia", m_oEastAsia );
		XmlMacroReadAttributeBase(oNode,  L"w:val",      m_oVal );
	}
	void    CLanguage::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CLanguage::ToString() const
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
	void CLanguage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if     ( oReader, L"w:bidi",     m_oBidi )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:eastAsia", m_oEastAsia )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",      m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// Lang (Language) 17.3.3.14 (Part 1)
//--------------------------------------------------------------------------------

	CLang::CLang()
	{
	}
	CLang::~CLang()
	{
	}

	void CLang::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void CLang::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CLang::ToString() const
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

	void CLang::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// Shading 17.3.5 (Part 1)
//--------------------------------------------------------------------------------

	CShading::CShading()
	{
	}
	CShading::~CShading()
	{
	}

	void CShading::FromXML(XmlUtils::CXmlNode& oNode)
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
	void CShading::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CShading::ToString() const
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

	void CShading::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

//--------------------------------------------------------------------------------
// TblWidth 17.4.88 (Part 1)
//--------------------------------------------------------------------------------

	CTblWidth::CTblWidth()
	{
	}
	CTblWidth::~CTblWidth()
	{
	}

	void    CTblWidth::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );
		XmlMacroReadAttributeBase( oNode, L"w:w",    m_oW );
	}
	void    CTblWidth::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CTblWidth::ToString() const
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

	void CTblWidth::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

//--------------------------------------------------------------------------------
// OnOff 17.17.4 (Part 1)
//--------------------------------------------------------------------------------

	COnOff::COnOff()
	{
	}
	COnOff::~COnOff()
	{
	}
	void COnOff::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void COnOff::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring COnOff::ToString() const
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


	void COnOff::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// COnOff2
//--------------------------------------------------------------------------------

    COnOff2::COnOff2()
	{
	}

    COnOff2::~COnOff2()
	{
	}

    void COnOff2::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}

    void COnOff2::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}

    std::wstring COnOff2::ToString() const
	{
		std::wstring sResult;

		sResult += L"w:val=\"" + m_oVal.ToString() + L"\" ";

		return sResult;
	}

    void COnOff2::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"val", m_oVal )
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}

    std::wstring COnOff2::ValNode(const std::wstring &node_name)
	{
		if (m_oVal.ToBool())
			return L"<" + node_name + L"/>";
		else
			return L"<" + node_name + L" w:val=\"0\"/>";
		return L"";
	}

//--------------------------------------------------------------------------------
// String 17.3.1.27  (Part 1)
//--------------------------------------------------------------------------------

	String::String()
	{
	}
	String::~String()
	{
	}

	void String::FromXML(XmlUtils::CXmlNode& oNode)
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
	void String::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring String::ToString() const
	{
		return ToStringWithNS(L"w:");
	}
	std::wstring String::ToStringWithNS(const std::wstring& ns) const
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
	std::wstring String::ToString2() const
	{
		std::wstring sResult;

		if ( m_sVal.IsInit() )
			sResult += *m_sVal;

		return sResult;
	}

	void String::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"val", m_sVal )
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}

//--------------------------------------------------------------------------------
// Spacing (SignedTwipsMeasure) 17.3.2.19 (Part 1)
//--------------------------------------------------------------------------------

	CSignedTwipsMeasure::CSignedTwipsMeasure()
	{
	}
	CSignedTwipsMeasure::~CSignedTwipsMeasure()
	{
	}

	void CSignedTwipsMeasure::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void CSignedTwipsMeasure::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CSignedTwipsMeasure::ToString() const
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

	void CSignedTwipsMeasure::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// Kern (HpsMeasure) 17.3.2.19 (Part 1)
//--------------------------------------------------------------------------------

	CHpsMeasure::CHpsMeasure()
	{
	}
	CHpsMeasure::~CHpsMeasure()
	{
	}

	void CHpsMeasure::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void CHpsMeasure::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CHpsMeasure::ToString() const
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

	void CHpsMeasure::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// Border 17.3.2.6 (Part 1)
//--------------------------------------------------------------------------------

	CColor::CColor()
	{
	}
	CColor::~CColor()
	{
	}
	void CColor::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:themeColor", m_oThemeColor );
		XmlMacroReadAttributeBase( oNode, L"w:themeShade", m_oThemeShade );
		XmlMacroReadAttributeBase( oNode, L"w:themeTint",  m_oThemeTint );
		XmlMacroReadAttributeBase( oNode, L"w:val",        m_oVal );
	}
	void CColor::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CColor::ToString() const
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

	void CColor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if     ( oReader, L"w:themeColor", m_oThemeColor )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeShade", m_oThemeShade )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:themeTint",  m_oThemeTint )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",        m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// Position (HpsMeasure) 17.3.2.19 (Part 1)
//--------------------------------------------------------------------------------

	CSignedHpsMeasure::CSignedHpsMeasure()
	{
	}
	CSignedHpsMeasure::~CSignedHpsMeasure()
	{
	}

	void CSignedHpsMeasure::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void CSignedHpsMeasure::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CSignedHpsMeasure::ToString() const
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

	void CSignedHpsMeasure::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// NumFmt 17.11.17 (Part 1)
//--------------------------------------------------------------------------------

	CNumFmt::CNumFmt()
	{
	}
	CNumFmt::~CNumFmt()
	{
	}

	void CNumFmt::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:format", m_sFormat );
		XmlMacroReadAttributeBase(oNode, L"w:val",    m_oVal );
	}
	void CNumFmt::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CNumFmt::ToString() const
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

	void CNumFmt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if     ( oReader, L"w:format", m_sFormat )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",    m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// NumFmt 17.11.19 (Part 1)
//--------------------------------------------------------------------------------

	CNumRestart::CNumRestart()
	{
	}
	CNumRestart::~CNumRestart()
	{
	}
	void CNumRestart::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode,  L"w:val", m_oVal );
	}
	void CNumRestart::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CNumRestart::ToString() const
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

	void CNumRestart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// DecimalNumber 17.3.1.10 (Part 1)
//--------------------------------------------------------------------------------

	CDecimalNumber::CDecimalNumber()
	{
	}
	CDecimalNumber::~CDecimalNumber()
	{
	}
	void CDecimalNumber::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ComplexTypes::CDecimalNumber::FromXML(oReader);
	}
	void CDecimalNumber::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	std::wstring CDecimalNumber::ToString() const
	{
		std::wstring sResult;

		if ( m_oVal.IsInit() )
		{
			sResult += L"w:val=\"" + std::to_wstring(*m_oVal) + L"\" ";
		}

		return sResult;
	}

//--------------------------------------------------------------------------------
// DecimalNumberOrPrecent 17.3.1.10 (Part 1)
//--------------------------------------------------------------------------------

	CDecimalNumberOrPrecent::CDecimalNumberOrPrecent()
	{
	}
	CDecimalNumberOrPrecent::~CDecimalNumberOrPrecent()
	{
	}

	void    CDecimalNumberOrPrecent::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CDecimalNumberOrPrecent::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CDecimalNumberOrPrecent::ToString() const
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

	void CDecimalNumberOrPrecent::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// Rel 17.6.14 (Part 1)
//--------------------------------------------------------------------------------

	CRel::CRel()
	{
	}
	CRel::~CRel()
	{
	}

	void CRel::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"r:id", m_oId );
		if (false == m_oId.IsInit())
		{
			XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
		}
	}
	void CRel::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CRel::ToString() const
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

	void CRel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"id", m_oId ) //r:id  or relationships:id
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}

//--------------------------------------------------------------------------------
// TextDirection 17.6.14 (Part 1)
//--------------------------------------------------------------------------------

	CTextDirection::CTextDirection()
	{
	}
	CTextDirection::~CTextDirection()
	{
	}

	void    CTextDirection::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CTextDirection::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CTextDirection::ToString() const
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

	void CTextDirection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// VerticalJc 17.6.23 (Part 1)
//--------------------------------------------------------------------------------

	CVerticalJc::CVerticalJc()
	{
	}
	CVerticalJc::~CVerticalJc()
	{
	}

	void    CVerticalJc::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CVerticalJc::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CVerticalJc::ToString() const
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

	void CVerticalJc::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// cnfStyle (Cnf) 17.3.1.8 (Part 1) + 9.2.1.1 (Part 4)
//--------------------------------------------------------------------------------

	CCnf::CCnf()
	{
	}
	CCnf::~CCnf()
	{
	}

	void CCnf::FromXML(XmlUtils::CXmlNode& oNode)
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
	void CCnf::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CCnf::ToString() const
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

	void CCnf::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

//--------------------------------------------------------------------------------
// jc (Jc) 17.3.1.13 (Part 1)
//--------------------------------------------------------------------------------

	CJc::CJc()
	{
	}
	CJc::~CJc()
	{
	}
	void CJc::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void CJc::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CJc::ToString() const
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

	void CJc::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// TrackChange 17.13.5.19 (Part 1)
//--------------------------------------------------------------------------------

	CTrackChange::CTrackChange()
	{
	}
	CTrackChange::~CTrackChange()
	{
	}

	void CTrackChange::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
		XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
		XmlMacroReadAttributeBase( oNode, L"w:id",     m_oId );
		XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );
	}
	void CTrackChange::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CTrackChange::ToString() const
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

	void CTrackChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author", m_sAuthor )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",   m_oDate  )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",     m_oId )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// LongHexNumber 17.9.30 (Part 1)
//--------------------------------------------------------------------------------

	CLongHexNumber::CLongHexNumber()
	{
	}
	CLongHexNumber::~CLongHexNumber()
	{
	}

	void CLongHexNumber::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CLongHexNumber::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CLongHexNumber::ToString() const
	{
		std::wstring sResult;

		ComplexTypes_WriteAttribute( L"w:val=\"", m_oVal );

		return sResult;
	}

	void CLongHexNumber::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// UnsignedDecimalNumber 17.16.32 (Part 1)
//--------------------------------------------------------------------------------

	CUnsignedDecimalNumber::CUnsignedDecimalNumber()
	{
	}
	CUnsignedDecimalNumber::~CUnsignedDecimalNumber()
	{
	}

	void    CUnsignedDecimalNumber::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CUnsignedDecimalNumber::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CUnsignedDecimalNumber::ToString() const
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

	void CUnsignedDecimalNumber::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// TwipsMeasure 17.15.1.25 (Part 1)
//--------------------------------------------------------------------------------

	CTwipsMeasure::CTwipsMeasure()
	{
	}
	CTwipsMeasure::~CTwipsMeasure()
	{
	}

	void    CTwipsMeasure::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CTwipsMeasure::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CTwipsMeasure::ToString() const
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

	void CTwipsMeasure::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}


//--------------------------------------------------------------------------------
// Empty 17.15.1.48 (Part 1)
//--------------------------------------------------------------------------------

	CEmpty::CEmpty()
	{
	}
	CEmpty::~CEmpty()
	{
	}

	void    CEmpty::FromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	void    CEmpty::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
	}
	std::wstring CEmpty::ToString() const
	{
		return L"";
	}

//--------------------------------------------------------------------------------
// MatchSrc 17.17.2.3 (Part 1)
//--------------------------------------------------------------------------------

	CMatchSrc::CMatchSrc()
	{
	}
	CMatchSrc::~CMatchSrc()
	{
	}

	void CMatchSrc::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void CMatchSrc::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CMatchSrc::ToString() const
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

	void CMatchSrc::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// EdnPos 17.11.22 (Part 1)
//--------------------------------------------------------------------------------

	CEdnPos::CEdnPos()
	{
	}
	CEdnPos::~CEdnPos()
	{
	}

	void    CEdnPos::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CEdnPos::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CEdnPos::ToString() const
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

	void CEdnPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
// FtnPos 17.11.21 (Part 1)
//--------------------------------------------------------------------------------

	CFtnPos::CFtnPos()
	{
	}
	CFtnPos::~CFtnPos()
	{
	}

	void    CFtnPos::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
	}
	void    CFtnPos::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CFtnPos::ToString() const
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

	void CFtnPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
		WritingElement_ReadAttributes_End( oReader )
	}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------

	CDocPartGallery::CDocPartGallery()
	{
	}
	CDocPartGallery::~CDocPartGallery()
	{
	}

	void CDocPartGallery::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CDocPartGallery::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CDocPartGallery::ToString() const
	{
		std::wstring sResult;

		if (m_oVal.IsInit())
		{
			sResult += L"w:val=\"" + m_oVal->ToString() + L"\" ";
		}

		return sResult;
	}

	void CDocPartGallery::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------

	CDocPartBehavior::CDocPartBehavior() {}
	CDocPartBehavior::~CDocPartBehavior() {}

	void CDocPartBehavior::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CDocPartBehavior::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CDocPartBehavior::ToString() const
	{
		std::wstring sResult;

		if (m_oVal.IsInit())
		{
			sResult += L"w:val=\"" + m_oVal->ToString() + L"\" ";
		}
		return sResult;
	}

	void CDocPartBehavior::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// EastAsianLayout 17.3.2.10 (Part 1)
//--------------------------------------------------------------------------------

	CEastAsianLayout::CEastAsianLayout()
	{
	}
	CEastAsianLayout::~CEastAsianLayout()
	{
	}

	void    CEastAsianLayout::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:combine", m_oCombine);
		XmlMacroReadAttributeBase(oNode, L"w:combineBrackets", m_oCombineBrackets);
		XmlMacroReadAttributeBase(oNode, L"w:id", m_oID);
		XmlMacroReadAttributeBase(oNode, L"w:vert", m_oVert);
		XmlMacroReadAttributeBase(oNode, L"w:vertCompress", m_oVertCompress);
	}
	void    CEastAsianLayout::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CEastAsianLayout::ToString() const
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

	void CEastAsianLayout::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_Read_if(oReader, L"w:combine", m_oCombine)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:combineBrackets", m_oCombineBrackets)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:id", m_oID)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:vert", m_oVert)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:vertCompress", m_oVertCompress)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// Effect 17.3.2.11 (Part 1)
//--------------------------------------------------------------------------------

	CTextEffect::CTextEffect()
	{
	}
	CTextEffect::~CTextEffect()
	{
	}

	void    CTextEffect::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CTextEffect::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CTextEffect::ToString() const
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

	void CTextEffect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// Em 17.3.2.12 (Part 1)
//--------------------------------------------------------------------------------

	CEm::CEm()
	{
	}
	CEm::~CEm()
	{
	}

	void    CEm::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CEm::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CEm::ToString() const
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

	void CEm::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// FitText 17.3.2.14 (Part 1)
//--------------------------------------------------------------------------------

	CFitText::CFitText()
	{
	}
	CFitText::~CFitText()
	{
	}

	void    CFitText::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:id", m_oID);
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CFitText::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CFitText::ToString() const
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

	void CFitText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_Read_if(oReader, L"w:id", m_oID)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// Highlight 17.3.2.14 (Part 1)
//--------------------------------------------------------------------------------

	CHighlight::CHighlight()
	{
	}
	CHighlight::~CHighlight()
	{
	}

	void    CHighlight::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CHighlight::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CHighlight::ToString() const
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

	void CHighlight::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// u (Underline) 17.3.2.40 (Part 1)
//--------------------------------------------------------------------------------

	CUnderline::CUnderline()
	{
	}
	CUnderline::~CUnderline()
	{
	}
	void    CUnderline::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:color", m_oColor);
		XmlMacroReadAttributeBase(oNode, L"w:themeColor", m_oThemeColor);
		XmlMacroReadAttributeBase(oNode, L"w:themeShade", m_oThemeShade);
		XmlMacroReadAttributeBase(oNode, L"w:themeTint", m_oThemeTint);
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CUnderline::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CUnderline::ToString() const
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

	void CUnderline::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_Read_if(oReader, L"w:color", m_oColor)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeColor", m_oThemeColor)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeShade", m_oThemeShade)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeTint", m_oThemeTint)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// vertAlign (VerticalAlignRun) 17.3.2.40 (Part 1)
//--------------------------------------------------------------------------------

	CVerticalAlignRun::CVerticalAlignRun()
	{
	}
	CVerticalAlignRun::~CVerticalAlignRun()
	{
	}
	void    CVerticalAlignRun::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CVerticalAlignRun::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CVerticalAlignRun::ToString() const
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

	void CVerticalAlignRun::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// w (TextScale) 17.3.2.40 (Part 1)
//--------------------------------------------------------------------------------

	CTextScale::CTextScale()
	{
	}
	CTextScale::~CTextScale()
	{
	}
	void    CTextScale::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CTextScale::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CTextScale::ToString() const
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

	void CTextScale::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// framePr (FramePr) 17.3.1.11 (Part 1)
//--------------------------------------------------------------------------------

	CFramePr::CFramePr()
	{
	}
	CFramePr::~CFramePr()
	{
	}

	void CFramePr::FromXML(XmlUtils::CXmlNode& oNode)
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
	void    CFramePr::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CFramePr::ToString() const
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

	void CFramePr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

//--------------------------------------------------------------------------------
// ind (Ind) 17.3.1.12 (Part 1) + 9.2.1.2 (Part 4)
//--------------------------------------------------------------------------------

	CInd::CInd()
	{
	}
	CInd::~CInd()
	{
	}
	void CInd::FromXML(XmlUtils::CXmlNode& oNode)
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
	void CInd::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CInd::ToString() const
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

	void CInd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

//--------------------------------------------------------------------------------
// Spacing 17.3.1.33 (Part 1)
//--------------------------------------------------------------------------------

	CSpacing::CSpacing()
	{
	}
	CSpacing::~CSpacing()
	{
	}

	void CSpacing::FromXML(XmlUtils::CXmlNode& oNode)
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
	void CSpacing::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CSpacing::ToString() const
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

	const CSpacing CSpacing::Merge(const CSpacing& oPrev, const CSpacing& oCurrent)
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

	void CSpacing::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

//--------------------------------------------------------------------------------
// TabStop 17.3.1.37 (Part 1)
//--------------------------------------------------------------------------------

	CTabStop::CTabStop()
	{
	}
	CTabStop::~CTabStop()
	{
	}

	void    CTabStop::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:leader", m_oLeader);
		XmlMacroReadAttributeBase(oNode, L"w:pos", m_oPos);
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CTabStop::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CTabStop::ToString() const
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

	void CTabStop::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_Read_if(oReader, L"w:leader", m_oLeader)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:pos", m_oPos)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// TextAlignment 17.3.1.39 (Part 1)
//--------------------------------------------------------------------------------

	CTextAlignment::CTextAlignment()
	{
	}
	CTextAlignment::~CTextAlignment()
	{
	}
	void    CTextAlignment::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CTextAlignment::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CTextAlignment::ToString() const
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

	void CTextAlignment::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
// TextboxTightWrap 17.3.1.40 (Part 1)
//--------------------------------------------------------------------------------

	CTextboxTightWrap::CTextboxTightWrap()	{}
	CTextboxTightWrap::~CTextboxTightWrap() {}
	void    CTextboxTightWrap::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w:val", m_oVal);
	}
	void    CTextboxTightWrap::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CTextboxTightWrap::ToString() const
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

	void CTextboxTightWrap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------

	CLigatures::CLigatures() {}
	CLigatures::~CLigatures() {}

	void CLigatures::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
	}
	void CLigatures::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CLigatures::ToString() const
	{
		std::wstring sResult;

		if (m_oVal.IsInit())
		{
			sResult += L"w14:val=\"" + m_oVal->ToString() + L"\"";
		}
		return sResult;
	}

	void CLigatures::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w14:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------

	CNumSpacing::CNumSpacing() {}
	CNumSpacing::~CNumSpacing() {}

	void CNumSpacing::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
	}
	void CNumSpacing::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CNumSpacing::ToString() const
	{
		std::wstring sResult;

		if (m_oVal.IsInit())
		{
			sResult += L"w14:val=\"" + m_oVal->ToString() + L"\"";
		}
		return sResult;
	}

	void CNumSpacing::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w14:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------

	CNumForm::CNumForm() {}
	CNumForm::~CNumForm() {}

	void CNumForm::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
	}
	void CNumForm::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CNumForm::ToString() const
	{
		std::wstring sResult;

		if (m_oVal.IsInit())
		{
			sResult += L"w14:val=\"" + m_oVal->ToString() + L"\"";
		}
		return sResult;
	}

	void CNumForm::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_ReadSingle(oReader, L"w14:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------

	CStylisticSet::CStylisticSet()	{}
	CStylisticSet::~CStylisticSet(){}

	void CStylisticSet::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase(oNode, L"w14:id", m_oId);
		XmlMacroReadAttributeBase(oNode, L"w14:val", m_oVal);
	}
	void CStylisticSet::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	std::wstring CStylisticSet::ToString() const
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

	void CStylisticSet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
		WritingElement_ReadAttributes_Read_if(oReader, L"w14:id", m_oId)
		WritingElement_ReadAttributes_Read_else_if(oReader, L"w14:val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

} // Word
//-------------------------------------------------------------------------------------------------------------------------

namespace Drawing
{
// ---------------------------------------------------------------------------
// Offset (Point2D) 20.1.7.4
// ---------------------------------------------------------------------------

	CPoint2D::CPoint2D()
	{
	}
	CPoint2D::~CPoint2D()
	{
	}
	void    CPoint2D::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"x", m_oX );
		XmlMacroReadAttributeBase( oNode, L"y", m_oY );
	}
	void    CPoint2D::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CPoint2D::ToString() const
	{
		std::wstring sResult;

		sResult += L"x=\"" + m_oX.ToString() + L"\" ";
		sResult += L"y=\"" + m_oY.ToString() + L"\" ";

		return sResult;
	}

	void CPoint2D::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if     ( oReader, L"x", m_oX )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"y", m_oY )
		WritingElement_ReadAttributes_End( oReader )
	}

// -----------------------------------------------------------------------
// Extent (PositiveSize2D) 20.4.2.7
// -----------------------------------------------------------------------

	CPositiveSize2D::CPositiveSize2D()
	{
	}
	CPositiveSize2D::~CPositiveSize2D()
	{
	}
	void    CPositiveSize2D::FromXML(XmlUtils::CXmlNode& oNode)
	{
		XmlMacroReadAttributeBase( oNode, L"cx", m_oCx );
		XmlMacroReadAttributeBase( oNode, L"cy", m_oCy );
	}
	void    CPositiveSize2D::FromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}

	std::wstring CPositiveSize2D::ToString() const
	{
		std::wstring sResult;

		sResult += L"cx=\"" + m_oCx.ToString() + L"\" ";
		sResult += L"cy=\"" + m_oCy.ToString() + L"\" ";

		return sResult;
	}

	void CPositiveSize2D::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if     ( oReader, L"cx", m_oCx )
		WritingElement_ReadAttributes_Read_else_if( oReader, L"cy", m_oCy )
		WritingElement_ReadAttributes_End( oReader )
	}

} // Drawing
} // ComplexTypes
