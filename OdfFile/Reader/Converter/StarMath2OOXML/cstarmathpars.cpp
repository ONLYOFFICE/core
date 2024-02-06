/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "cstarmathpars.h"
#include "cconversionsmtoooxml.h"
namespace StarMath
{
//class methods CParsStarMath
	std::vector<CElement*> CParserStarMathString::Parse(std::wstring& wsParseString,const TBaseAttribute* pBaseAttribute)
	{
		std::wstring::iterator itStart = wsParseString.begin(),itEnd = wsParseString.end();
		CStarMathReader* pReader = new CStarMathReader(itStart,itEnd);
		pReader->SetBaseAttribute(pBaseAttribute);
		while(pReader->CheckIteratorPosition())
		{
			CElement* pTempElement;
			if(pReader->GetLocalType() == TypeElement::newline)
			{
				m_arEquation.push_back(new CElementSpecialSymbol(pReader->GetLocalType()));
				pReader->ClearReader();
			}
			else
				pTempElement = ParseElement(pReader);
			if(nullptr == pTempElement)
				continue;
			if(!m_arEquation.empty() && CheckForLeftArgument(pTempElement->GetBaseType()) )
			{
				if(AddLeftArgument(m_arEquation.back(),pTempElement))
					m_arEquation.pop_back();
			}
			m_arEquation.push_back(pTempElement);
		}
		if(!pReader->EmptyString())
		{
			CElement* pTempElement;
			if(pReader->GetLocalType() == TypeElement::newline)
			{
				m_arEquation.push_back(new CElementSpecialSymbol(pReader->GetLocalType()));
				pReader->ClearReader();
			}
			else
				pTempElement = ParseElement(pReader);
			if(nullptr != pTempElement)
					m_arEquation.push_back(pTempElement);
		}
		return m_arEquation;
	}

	CElement* CParserStarMathString::ParseElement(CStarMathReader* pReader)
	{
		CElement* pElement;

		pReader->ReadingTheNextToken();

		if(TypeElement::newline == pReader->GetLocalType())
			return nullptr;

		pElement = CElement::CreateElement(pReader);

		if(pElement != nullptr)
		{
			if(pReader->GetAttribute() != nullptr && !CheckForLeftArgument(pReader->GetGlobalType()))
				pElement->SetBaseAttribute(pReader->GetAttribute());
			else if(pReader->GetAttribute() != nullptr && (pReader->GetLocalType() == TypeElement::plus || TypeElement::minus == pReader->GetLocalType()))
				pElement->SetBaseAttribute(pReader->GetAttribute());
			pReader->ClearReader();
			pElement->Parse(pReader);
			if(pReader->GetBaseAttribute()!=nullptr)
				pElement->SetBaseAttribute(pReader->GetBaseAttribute());
			if(pElement->GetAttribute() != nullptr && TypeElement::Function != pElement->GetBaseType())
				pElement->SetAttribute(pElement->GetAttribute());
			return pElement;
		}
		else
		{
			pReader->ClearReader();
			return pElement;
		}
	}
	template<typename T>
	bool SetLeft(CElement *pLeftArg, CElement *pElementWhichAdd)
	{
		T* pTempElement = dynamic_cast<T*>(pElementWhichAdd);
		if(pTempElement->GetLeftArg() == nullptr)
		{
			pTempElement->SetLeftArg(pLeftArg);
			pElementWhichAdd = pTempElement;
			return true;
		}
		else return false;
	}

	bool CParserStarMathString::AddLeftArgument(CElement *pLeftArg, CElement *pElementWhichAdd)
	{
		switch(pElementWhichAdd->GetBaseType())
		{
			case TypeElement::BinOperator:
			{
				return SetLeft<CElementBinOperator>(pLeftArg, pElementWhichAdd);
			}
			case TypeElement::SetOperations:
			{
				SetLeft<CElementSetOperations>(pLeftArg, pElementWhichAdd);
				break;
			}
			case TypeElement::Connection:
			{
				return SetLeft<CElementConnection>(pLeftArg,pElementWhichAdd);
			}
			case TypeElement::BracketWithIndex:
			{
				return SetLeft<CElementBracketWithIndex>(pLeftArg,pElementWhichAdd);
			}
			case TypeElement::Index:
			{
				return SetLeft<CElementIndex>(pLeftArg,pElementWhichAdd);
			}
			default:
				break;
		}
	}
	CElement* CParserStarMathString::ReadingWithoutBracket(CStarMathReader *pReader)
	{
		CElement* pFirstTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->GetToken();
		pReader->SetTypesToken();
		while(CheckForLeftArgument(pReader->GetGlobalType()) && (pReader->GetLocalType() != TypeElement::frac || pReader->GetLocalType()!=TypeElement::nroot || pReader->GetLocalType()!=TypeElement::sqrt))
		{
			CElement* pSecondTempElement = CParserStarMathString::ParseElement(pReader);
			if(pFirstTempElement != nullptr)
			{
				CParserStarMathString::AddLeftArgument(pFirstTempElement,pSecondTempElement);
			}
			pFirstTempElement = pSecondTempElement;
			if(pReader->EmptyString())
			{
				pReader->GetToken();
				pReader->SetTypesToken();
			}
		}
		return pFirstTempElement;
	}
	bool CParserStarMathString::CheckForLeftArgument(const TypeElement &enType)
	{
		switch(enType)
		{
			case TypeElement::BinOperator:
				return true;
			case TypeElement::Connection:
				return true;
			case TypeElement::SetOperations:
				return true;
			case TypeElement::BracketWithIndex:
				return true;
			case TypeElement::Index:
				return true;
			default:
				return false;
		}
	}
//class methods CAttribute
	CAttribute::CAttribute(): m_bBold(false),m_bItal(false),m_bPhantom(false),m_bStrike(false),m_iSize(0)
	{
	}
	CAttribute::~CAttribute()
	{}
	void CAttribute::SetSize(const unsigned int &iSize)
	{
		m_iSize = iSize;
	}
	void CAttribute::SetBold()
	{
		m_bBold = true;
	}
	void CAttribute::SetItal()
	{
		m_bItal = true;
	}
	void CAttribute::SetPhantom()
	{
		m_bPhantom = true;
	}
	void CAttribute::SetStrike()
	{
		m_bStrike = true;
	}
	void CAttribute::SetColor(const TypeElement &enColor)
	{
		if(enColor != TypeElement::undefine)
		{
			switch (enColor) {
			case TypeElement::black:
			m_wsColor = L"000000";
			break;
			case TypeElement::blue:
			m_wsColor = L"0400ff";
			break;
			case TypeElement::green:
			m_wsColor =L"00FF00";
			break;
			case TypeElement::red:
			m_wsColor =L"FF0000";
			break;
			case TypeElement::fuchsia:
			m_wsColor =L"ED0DD9";
			break;
			case TypeElement::aqua:
			m_wsColor =L"30D5C8";
			break;
			case TypeElement::yellow:
			m_wsColor =L"FFFF00";
			break;
			case TypeElement::gray:
			m_wsColor =L"808080";
			break;
			case TypeElement::lime:
			m_wsColor =L"00FF00";
			break;
			case TypeElement::maroon:
			m_wsColor =L"800000";
			break;
			case TypeElement::navy:
			m_wsColor =L"000080";
			break;
			case TypeElement::olive:
			m_wsColor =L"808000";
			break;
			case TypeElement::purple:
			m_wsColor =L"800080";
			break;
			case TypeElement::silver:
			m_wsColor =L"C0C0C0";
			break;
			case TypeElement::teal:
			m_wsColor =L"008080";
			break;
			case TypeElement::coral:
			m_wsColor =L"FF7F50";
			break;
			case TypeElement::midnightblue:
			m_wsColor =L"191970";
			break;
			case TypeElement::crimson:
			m_wsColor =L"DC143C";
			break;
			case TypeElement::violet:
			m_wsColor =L"EE82EE";
			break;
			case TypeElement::orange:
			m_wsColor =L"FFA500";
			break;
			case TypeElement::orangered:
			m_wsColor =L"FF4500";
			break;
			case TypeElement::seagreen:
			m_wsColor =L"2E8B57";
			break;
			case TypeElement::indigo:
			m_wsColor =L"4B0082";
			break;
			case TypeElement::hotpink:
			m_wsColor =L"FF69B4";
			break;
			case TypeElement::lavender:
			m_wsColor =L"FFF0F5";
			break;
			default:
			break;
			}
		}
	}
	void CAttribute::SetColor(const std::wstring &wsColor)
	{
		m_wsColor = wsColor;
	}
	void CAttribute::SetFont(const TypeElement &enFont)
	{
		switch (enFont) {
		case TypeElement::ital:
		SetItal();
		break;
		case TypeElement::bold:
		SetBold();
		break;
		case TypeElement::phantom:
		SetPhantom();
		break;
		case TypeElement::overstrike:
		SetStrike();
		break;
		default:
		break;
		}
	}
	void CAttribute::SetFontName(const std::wstring &wsNameFont)
	{
		m_wsNameFont = wsNameFont;
	}
	bool CAttribute::GetBold()
	{
		return m_bBold;
	}
	bool CAttribute::GetItal()
	{
		return m_bItal;
	}
	bool CAttribute::GetPhantom()
	{
		return m_bPhantom;
	}
	bool CAttribute::GetStrike()
	{
		return m_bStrike;
	}
	std::wstring CAttribute::GetColor()
	{
		return m_wsColor;
	}
	const std::wstring& CAttribute::GetFontName()
	{
		return m_wsNameFont;
	}
	unsigned int CAttribute::GetSize()
	{
		return m_iSize;
	}
	bool CAttribute::EmptyColor()
	{
		return m_wsColor.empty();
	}
	void CAttribute::ParseColorAttribute(const std::wstring &wsToken,CStarMathReader* pReader)
	{
		TypeElement enTempColor = GetTypeColorAttribute(wsToken);
		switch(enTempColor)
		{
		case TypeElement::hex:
		{
			std::wstring wsTempHex;
			do
			{
				wsTempHex += pReader->GetElement();
			}while(wsTempHex.size()< 6);
			m_wsColor = wsTempHex;
		break;
		}
		case TypeElement::rgb:
		{
		const int iTempLen = 7;
		wchar_t arTemp[iTempLen];
		unsigned int  wsRed,wsGreen,wsBlue;
		pReader->GetToken();
		wsRed = std::stoi(pReader->GetString());
		pReader->GetToken();
		wsGreen = std::stoi(pReader->GetString());
		pReader->GetToken();
		wsBlue = std::stoi(pReader->GetString());
		if(wsRed > 255 || wsGreen > 255 || wsBlue > 255)
			m_wsColor = L"000000";
		else
		{
			swprintf(arTemp,iTempLen,L"%02X%02X%02X",wsRed,wsGreen,wsBlue);
			m_wsColor = std::wstring(arTemp,6);
		}
		break;
		}
		default:
		SetColor(enTempColor);
		break;
		}
	}
	void CAttribute::ParseFontAttribute(const TypeElement& enTypeFont, CStarMathReader *pReader)
	{
		switch(enTypeFont)
		{
		case TypeElement::size:
		{
			pReader->GetToken();
			int iTemp = std::stoi(pReader->GetString());
			if (iTemp >= 0)
				m_iSize = iTemp*2;
			else
				m_iSize = 24;
			break;
		}
		case TypeElement::font:
		{
			pReader->GetToken();
			if(pReader->GetString() == L"sans")
				m_wsNameFont = L"Liberation Sans";
			else if(pReader->GetString() == L"serif")
				m_wsNameFont = L"Liberation Serif";
			else if(pReader->GetString() == L"fixed")
				m_wsNameFont = L"Liberation Mono";
			else
				m_wsNameFont = pReader->GetString();
			break;
		}
		default:
		SetFont(enTypeFont);
		break;
		}
	}

//нет phantom, rgb, 16 , гарнитуры и кегля
	TypeElement CAttribute::GetTypeColorAttribute(const std::wstring &wsToken)
	{
		if(L"color" == wsToken) return TypeElement::color;
		else if(L"hex"==wsToken) return TypeElement::hex;
		else if(L"rgb" == wsToken) return TypeElement::rgb;
		else if(L"black" == wsToken) return TypeElement::black;
		else if(L"green" == wsToken) return TypeElement::green;
		else if(L"aqua" == wsToken) return TypeElement::aqua;
		else if(L"yellow" == wsToken) return TypeElement::yellow;
		else if(L"lime" == wsToken) return TypeElement::lime;
		else if(L"navy" == wsToken) return TypeElement::navy;
		else if(L"purple" == wsToken) return TypeElement::purple;
		else if(L"teal" == wsToken) return TypeElement::teal;
		else if(L"blue" == wsToken) return TypeElement::blue;
		else if(L"red" == wsToken) return TypeElement::red;
		else if(L"fuchsia" == wsToken) return TypeElement::fuchsia;
		else if(L"gray" == wsToken) return TypeElement::gray;
		else if(L"maroon" == wsToken) return TypeElement::maroon;
		else if(L"olive" == wsToken) return TypeElement::olive;
		else if(L"silver" == wsToken) return TypeElement::silver;
		else if(L"coral" == wsToken) return TypeElement::coral;
		else if(L"midnightblue" == wsToken) return TypeElement::midnightblue;
		else if(L"crimson" == wsToken) return TypeElement::crimson;
		else if(L"violet" == wsToken) return TypeElement::violet;
		else if(L"orange" == wsToken) return TypeElement::orange;
		else if(L"seagreen" == wsToken) return TypeElement::seagreen;
		else if(L"hotpink" == wsToken) return TypeElement::hotpink;
		else if(L"orangered" == wsToken) return TypeElement::orangered;
		else if(L"indigo" == wsToken) return TypeElement::indigo;
		else if(L"lavender" == wsToken) return TypeElement::lavender;
		else return TypeElement::undefine;
	}
	TypeElement CAttribute::GetTypeFontAttribute(const std::wstring &wsToken)
	{
		if(L"bold" == wsToken) return TypeElement::bold;
		else if(L"size" == wsToken) return TypeElement::size;
		else if(L"font" == wsToken) return TypeElement::font;
		else if(L"ital" == wsToken) return TypeElement::ital;
		else if(L"phantom" == wsToken ) return TypeElement::phantom;
		else if(L"overstrike" == wsToken) return TypeElement::overstrike;
		else return TypeElement::undefine;
	}
//class methods CElement
	CElement::~CElement()
	{
		delete m_pAttribute;
	}
	CElement::CElement(): m_pAttribute(nullptr)
	{}
	CElement::CElement(const TypeElement &enTypeBase): m_pAttribute(nullptr),m_enBaseType(enTypeBase)
	{
	}
	CElement* CElement::CreateElement(CStarMathReader* pReader)
	{
		switch (pReader->GetGlobalType()) {
			case TypeElement::String:
				return new CElementString(pReader->GetString());
			case TypeElement::BinOperator:
				return new CElementBinOperator(pReader->GetLocalType());
			case TypeElement::SetOperations:
				return new CElementSetOperations(pReader->GetLocalType());
			case TypeElement::Connection:
				return new CElementConnection(pReader->GetLocalType());
			case TypeElement::Function:
				{
				if(pReader->GetLocalType() == TypeElement::func)
				{
					pReader->GetToken();
					return new CElementFunction(pReader->GetLocalType(),pReader->GetString());
				}
				else
					return new CElementFunction(pReader->GetLocalType());
				}
			case TypeElement::Bracket:
				return new CElementBracket(pReader->GetLocalType());
			case TypeElement::Operation:
				{
				if(pReader->GetLocalType() == TypeElement::oper)
				{
					pReader->GetToken();
					return new CElementOperator(pReader->GetLocalType(),pReader->GetString());
				}
				else
					return new CElementOperator(pReader->GetLocalType());
				}
			case TypeElement::BracketWithIndex:
				return new CElementBracketWithIndex(pReader->GetLocalType());
			case TypeElement::Grade:
				return new CElementGrade();
			case TypeElement::Matrix:
				return new CElementMatrix(pReader->GetLocalType());
			case TypeElement::SpecialSymbol:
				return new CElementSpecialSymbol(pReader->GetLocalType());
			case TypeElement::Index:
				return new CElementIndex(pReader->GetLocalType());
			case TypeElement::Mark:
				return new CElementDiacriticalMark(pReader->GetLocalType());
			default:
				return nullptr;
		}
	}
	const TypeElement& CElement::GetBaseType()
	{
		return m_enBaseType;
	}
	void CElement::SetBaseType(const TypeElement &enType)
	{
		m_enBaseType = enType;
	}
	void CElement::SetBaseAttribute(CAttribute *pAttribute)
	{
		if(m_pAttribute == nullptr && pAttribute != nullptr)
			m_pAttribute = pAttribute;
		else if(pAttribute != nullptr && pAttribute != m_pAttribute)
		{
			if(!m_pAttribute->GetBold() && pAttribute->GetBold())
				m_pAttribute->SetBold();
			if(!m_pAttribute->GetItal() && pAttribute->GetItal())
				m_pAttribute->SetItal();
			if(!m_pAttribute->GetPhantom() && pAttribute->GetPhantom())
				m_pAttribute->SetPhantom();
			if(!m_pAttribute->GetStrike() && pAttribute->GetStrike())
				m_pAttribute->SetStrike();
			if(m_pAttribute->EmptyColor() && !pAttribute->EmptyColor())
				m_pAttribute->SetColor(pAttribute->GetColor());
			if(m_pAttribute->GetSize() == 0 && pAttribute->GetSize() != 0)
				m_pAttribute->SetSize(pAttribute->GetSize());
		}
	}
	CAttribute* CElement::GetAttribute()
	{
		return m_pAttribute;
	}
//class methods CElementString
	CElementString::CElementString(const std::wstring& wsTokenString)
		:CElement(TypeElement::String),m_wsString(wsTokenString)
	{
	}
	CElementString::~CElementString()
	{}
	void CElementString::SetString(const std::wstring& wsTokenString)
	{
		m_wsString = wsTokenString;
	}
	void CElementString::Parse(CStarMathReader* pReader)
	{
		pReader->ClearReader();
	}
	void CElementString::ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		pXmlWrite->WriteString(m_wsString);
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
	}
	std::wstring CElementString::GetString()
	{
		return m_wsString;
	}
	TypeElement CElementString::GetDigit(const std::wstring &wsToken)
	{
		if(wsToken.empty()) return TypeElement::undefine;

		for(wchar_t cOneElement: wsToken)
		{
			if(!isdigit(cOneElement) ||  L'\0' == cOneElement) return TypeElement::undefine;
		}
		return TypeElement::String;
	}
	TypeElement CElementString::GetWord(const std::wstring &wsToken)
	{
		if(wsToken.empty()) return TypeElement::undefine;

		for(wchar_t cOneElement: wsToken)
		{
			if(!isalpha(cOneElement)) return TypeElement::undefine;
		}
		return TypeElement::String;
	}
	void CElementString::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
	}
//class methods CElementBinOperator
	CElementBinOperator::CElementBinOperator(const TypeElement& enType)
		:CElement(TypeElement::BinOperator),m_pLeftArgument(nullptr) , m_pRightArgument(nullptr),m_enTypeBinOp(enType)
	{
	}
	CElementBinOperator::~CElementBinOperator()
	{
		delete m_pLeftArgument;
		delete m_pRightArgument;
	}
	void CElementBinOperator::SetLeftArg(CElement *pElement)
	{
		m_pLeftArgument = pElement;
	}
	void CElementBinOperator::SetRightArg(CElement *pElement)
	{
		m_pRightArgument = pElement;
	}
	void CElementBinOperator::Parse(CStarMathReader* pReader)
	{
		if(m_enTypeBinOp == TypeElement::frac)
		{
			SetLeftArg(CParserStarMathString::ReadingWithoutBracket(pReader));
			SetRightArg(CParserStarMathString::ReadingWithoutBracket(pReader));
		}
		else if(m_enTypeBinOp == TypeElement::And || m_enTypeBinOp == TypeElement::neg)
		{
			SetRightArg(CParserStarMathString::ParseElement(pReader));
		}
		else
		{
			CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
			if((IsBinOperatorLowPrior() && pReader->GetGlobalType() == TypeElement::BinOperator) || pReader->GetGlobalType() == TypeElement::BracketWithIndex || (pReader->GetGlobalType() == TypeElement::Index && (pReader->GetLocalType() != TypeElement::nroot || pReader->GetLocalType() != TypeElement::sqrt)))
			{
				CElement* pBinOp = CParserStarMathString::ParseElement(pReader);
				CParserStarMathString::AddLeftArgument(pTempElement,pBinOp);
				SetRightArg(pBinOp);
			}
			else
				SetRightArg(pTempElement);
		}
	}
	void CElementBinOperator::ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite)
	{
		if(m_enTypeBinOp == TypeElement::over || m_enTypeBinOp ==TypeElement::division || TypeElement::frac == m_enTypeBinOp)
		{
			pXmlWrite->WriteNodeBegin(L"m:f",false);
			if(m_enTypeBinOp == TypeElement::division)
				CConversionSMtoOOXML::PropertiesMFPR(true,pXmlWrite,GetAttribute());
			else
				CConversionSMtoOOXML::PropertiesMFPR(false,pXmlWrite,GetAttribute());
			CConversionSMtoOOXML::WriteNodeConversion(L"m:num",m_pLeftArgument,pXmlWrite);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:den",m_pRightArgument,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:f",false,false);
		}
		else
		{
			CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArgument);
			pXmlWrite->WriteNodeBegin(L"m:r",false);
			CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeBegin(L"m:t",false);
			switch (m_enTypeBinOp)
			{
			case TypeElement::plus:
				pXmlWrite->WriteString(L"+");
				break;
			case TypeElement::minus:
				pXmlWrite->WriteString(L"-");
				break;
			case TypeElement::multipl:
				pXmlWrite->WriteString(L"*");
				break;
			case TypeElement::cdot:
				pXmlWrite->WriteString(L"\u00B7");
				break;
			case TypeElement::times:
				pXmlWrite->WriteString(L"\u00D7");
				break;
			case TypeElement::div:
				pXmlWrite->WriteString(L"\u00F7");
				break;
			case TypeElement::odivide:
				pXmlWrite->WriteString(L"\u2298");
				break;
			case TypeElement::oplus:
				pXmlWrite->WriteString(L"\u2295");
				break;
			case TypeElement::ominus:
				pXmlWrite->WriteString(L"\u2296");
				break;
			case TypeElement::odot:
				pXmlWrite->WriteString(L"\u2299");
				break;
			case TypeElement::otimes:
				pXmlWrite->WriteString(L"\u2297");
				break;
			case TypeElement::plus_minus:
				pXmlWrite->WriteString(L"\u00B1");
				break;
			case TypeElement::minus_plus:
				pXmlWrite->WriteString(L"\u2213");
				break;
			case TypeElement::Or:
				pXmlWrite->WriteString(L"\u2228");
				break;
			case TypeElement::And:
				pXmlWrite->WriteString(L"\u2227");
				break;
			default:
				break;
			}
			if(m_pRightArgument!=nullptr && m_pRightArgument->GetBaseType() == TypeElement::String && GetAttribute() == m_pRightArgument->GetAttribute())
			{
				CElementString* oNumber = dynamic_cast<CElementString*> (m_pRightArgument);
				pXmlWrite->WriteString(oNumber->GetString());
				pXmlWrite->WriteNodeEnd(L"m:t",false,false);
				pXmlWrite->WriteNodeEnd(L"m:r",false,false);
			}
			else
			{
				pXmlWrite->WriteNodeEnd(L"m:t",false,false);
				pXmlWrite->WriteNodeEnd(L"m:r",false,false);
				CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pRightArgument);
			}

		}
	}
	void CElementBinOperator::SetTypeBinOP(const TypeElement &enType)
	{
		m_enTypeBinOp = enType;
	}
	TypeElement CElementBinOperator::GetBinOperator(const std::wstring& wsToken)
	{
		if(L"cdot" == wsToken) return TypeElement::cdot;
		else if(L"+" == wsToken) return TypeElement::plus;
		else if(L"-" == wsToken) return TypeElement::minus;
		else if(L"oplus" == wsToken) return TypeElement::oplus;
		else if(L"ominus" == wsToken) return TypeElement::ominus;
		else if(L"circ" == wsToken) return TypeElement::circ;
		else if(L"times" == wsToken) return TypeElement::times;
		else if(L"over" == wsToken) return TypeElement::over;
		else if(L"frac" == wsToken) return TypeElement::frac;
		else if(L"div" == wsToken) return TypeElement::div;
		else if(L"*" == wsToken) return TypeElement::multipl;
		else if(L"/" == wsToken) return TypeElement::division;
		else if(L"odot" == wsToken) return TypeElement::odot;
		else if(L"otimes" == wsToken) return TypeElement::otimes;
		else if(L"odivide" == wsToken) return TypeElement::odivide;
		else if(L"wideslash" == wsToken) return TypeElement::wideslash;
		else if(L"widebslash" == wsToken) return TypeElement::widebslash;
		else if(L"+-" == wsToken) return TypeElement::plus_minus;
		else if(L"-+" == wsToken) return TypeElement::minus_plus;
		else if(L"neg" == wsToken) return TypeElement::neg;
		else if(L"or" == wsToken) return TypeElement::Or;
		else if(L"and" == wsToken) return TypeElement::And;
		else return TypeElement::undefine;
	}
	bool CElementBinOperator::IsBinOperatorLowPrior()
	{
		switch (m_enTypeBinOp) {
		case TypeElement::plus:
		return true;
		case TypeElement::minus:
		return true;
		case TypeElement::oplus:
		return true;
		case TypeElement::ominus:
		return true;
		case TypeElement::circ:
		return true;
		case TypeElement::Or:
		return true;
		default:
		return false;
		}
	}
	void CElementBinOperator::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pLeftArgument !=nullptr)
			m_pLeftArgument->SetAttribute(pAttribute);
		if(m_pRightArgument!=nullptr)
			m_pRightArgument->SetAttribute(pAttribute);
	}
	CElement* CElementBinOperator::GetLeftArg()
	{
		return m_pLeftArgument;
	}
	CElement* CElementBinOperator::GetRightArg()
	{
		return m_pRightArgument;
	}
//class methods CElementBracket
	CElementBracket::CElementBracket(const TypeElement& enType)
		:CElement(TypeElement::Bracket),m_enTypeBracket(enType)
	{
	}
	CElementBracket::~CElementBracket()
	{
		for(CElement* pTemp:m_arBrecketValue) delete pTemp;
	}
	void CElementBracket::SetBracketValue(const std::vector<CElement*> &arValue)
	{
		m_arBrecketValue = arValue;
	}
	TypeElement CElementBracket::GetBracketOpen(const std::wstring &wsToken)
	{
		if(L"{" == wsToken) return TypeElement::brace;
		else if(L"(" == wsToken) return TypeElement::round;
		else if(L"[" == wsToken) return TypeElement::square;
		else if(L"ldbracket" == wsToken) return TypeElement::ldbracket;
		else if(L"lbrace" == wsToken) return TypeElement::lbrace;
		else if(L"langle" == wsToken) return TypeElement::langle;
		else if(L"lceil" == wsToken) return TypeElement::lceil;
		else if(L"lfloor" == wsToken) return TypeElement::lfloor;
		else if(L"lline" == wsToken) return TypeElement::lline;
		else if(L"ldline" == wsToken) return TypeElement::ldline;
		else return TypeElement::undefine;
	}
	TypeElement CElementBracket::GetBracketClose(const std::wstring &wsToken)
	{
		if(L"}" == wsToken) return TypeElement::rwbrace;
		else if(L")" == wsToken) return TypeElement::rround;
		else if(L"]" == wsToken) return TypeElement::rsquare;
		else if(L"rdbracket" == wsToken) return TypeElement::rdbracket;
		else if(L"rbrace" == wsToken) return TypeElement::rbrace;
		else if(L"rangle" == wsToken) return TypeElement::rangle;
		else if(L"rceil" == wsToken) return TypeElement::rceil;
		else if(L"rfloor" == wsToken) return TypeElement::rfloor;
		else if(L"rline" == wsToken) return TypeElement::rline;
		else if(L"rdline" == wsToken) return TypeElement::rdline;
		else if(L"right" == wsToken) return TypeElement::right;
		else return TypeElement::undefine;
	}
	std::vector<CElement*> CElementBracket::GetBracketValue()
	{
		return m_arBrecketValue;
	}
	void CElementBracket::Parse(CStarMathReader* pReader)
	{
		pReader->FindingTheEndOfParentheses();
		while(pReader->CheckIteratorPosition())
		{
			CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
			if(!m_arBrecketValue.empty() && CParserStarMathString::CheckForLeftArgument(pTempElement->GetBaseType()))
			{
				if(CParserStarMathString::AddLeftArgument(m_arBrecketValue.back(),pTempElement))
					m_arBrecketValue.pop_back();
			}
			m_arBrecketValue.push_back(pTempElement);
		}
		//доработать()
		pReader->IteratorNullification();
	}
	void CElementBracket::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(m_enTypeBracket != TypeElement::brace)
		{
			pXmlWrite->WriteNodeBegin(L"m:d",false);
			CConversionSMtoOOXML::PropertiesDPr(pXmlWrite,m_enTypeBracket,GetAttribute());
			pXmlWrite->WriteNodeBegin(L"m:e",false);
			for(CElement* pTemp:m_arBrecketValue)
			{
				CConversionSMtoOOXML::ElementConversion(pXmlWrite,pTemp);
			}
			pXmlWrite->WriteNodeEnd(L"m:e",false,false);
			pXmlWrite->WriteNodeEnd(L"m:d",false,false);
		}
		else
		{
			for(CElement* pTemp:m_arBrecketValue)
			{
				CConversionSMtoOOXML::ElementConversion(pXmlWrite,pTemp);
			}
		}

	}
	void CElementBracket::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		for(CElement* pTempElement:m_arBrecketValue)
		{
			if(pTempElement!=nullptr)
				pTempElement->SetAttribute(pAttribute);
		}
	}
//class methods CElementSpecialSymbol
	CElementSpecialSymbol::CElementSpecialSymbol(const TypeElement &enType)
		:CElement(TypeElement::SpecialSymbol),m_pValue(nullptr),m_enTypeSpecial(enType),m_wsType(L"")
	{
	}
	CElementSpecialSymbol::~CElementSpecialSymbol()
	{
		delete m_pValue;
	}
	void CElementSpecialSymbol::Parse(CStarMathReader* pReader)
	{
		switch(m_enTypeSpecial)
		{
		case TypeElement::fact:
		{
			SetValue(CParserStarMathString::ParseElement(pReader));
			break;
		}
		default:
		break;
		}
	}
	void CElementSpecialSymbol::SetValue(CElement* pValue)
	{
		m_pValue = pValue;
	}
	const TypeElement CElementSpecialSymbol::GetType()
	{
		return m_enTypeSpecial;
	}
	TypeElement CElementSpecialSymbol::GetSpecialSymbol(std::wstring &wsToken)
	{
		if(wsToken[0] != L'%')
		{
			if(L"#" == wsToken) return TypeElement::grid;
			else if(L"##" == wsToken) return TypeElement::transition;
			else if(L"emptyset" == wsToken) return TypeElement::emptyset;
			else if(L"aleph" == wsToken) return TypeElement::aleph;
			else if(L"setN" == wsToken) return TypeElement::setN;
			else if(L"setZ" == wsToken) return TypeElement::setZ;
			else if(L"setQ" == wsToken) return TypeElement::setQ;
			else if(L"setR" == wsToken) return TypeElement::setR;
			else if(L"setc" == wsToken) return TypeElement::setC;
			else if(L"infinity" == wsToken) return TypeElement::infinity;
			else if(L"fact" == wsToken) return TypeElement::fact;
			else if(L"abs" == wsToken) return TypeElement::abs;
			else if(L"`" == wsToken) return TypeElement::interval;
			else if(L"~" == wsToken) return TypeElement::emptiness;
			else if(L"partial" == wsToken) return TypeElement::partial;
			else if(L"nabla" == wsToken) return TypeElement::nabla;
			else if(L"exists" == wsToken) return TypeElement::exists;
			else if(L"notexists" == wsToken) return TypeElement::notexists;
			else if(L"forall" == wsToken) return TypeElement::forall;
			else if(L"hbar" == wsToken) return TypeElement::hbar;
			else if(L"lambdabar" == wsToken) return TypeElement::lambdabar;
			else if(L"Re" == wsToken) return TypeElement::Re;
			else if(L"Im" == wsToken) return TypeElement::Im;
			else if(L"wp" == wsToken) return TypeElement::wp;
			else if(L"laplace" == wsToken) return TypeElement::laplace;
			else if(L"fourier" == wsToken) return TypeElement::fourier;
			else if(L"backepsilon" == wsToken) return TypeElement::backepsilon;
			else if(L"leftarrow" == wsToken) return TypeElement::leftarrow;
			else if(L"rightarrow" == wsToken) return TypeElement::rightarrow;
			else if(L"uparrow" == wsToken) return TypeElement::uparrow;
			else if(L"downarrow" == wsToken) return TypeElement::downarrow;
			else if(L"dotslow" == wsToken) return TypeElement::dotslow;
			else if(L"dotsaxis" == wsToken) return TypeElement::dotsaxis;
			else if(L"dotsvert" == wsToken) return TypeElement::dotsvert;
			else if(L"dotsup" == wsToken) return TypeElement::dotsup;
			else if(L"dotsdown" == wsToken) return TypeElement::dotsdown;
			else if(L"newline" == wsToken) return TypeElement::newline;
		}
		else if(wsToken[0] == L'%')
		{
			wsToken = wsToken.substr(1,wsToken.size()-1);
			if(L"ALPHA" == wsToken) return TypeElement::alpha;
			else if(L"ZETA" == wsToken) return TypeElement::zeta;
			else if(L"LAMBDA" == wsToken) return TypeElement::lambda;
			else if(L"PI" == wsToken) return TypeElement::pi;
			else if(L"PHI" == wsToken) return TypeElement::phi;
			else if(L"alpha" == wsToken) return TypeElement::alpha_small;
			else if(L"varepsilon" == wsToken) return TypeElement::varepsilon;
			else if(L"iota" == wsToken) return TypeElement::iota_small;
			else if(L"xi" == wsToken) return TypeElement::xi_small;
			else if(L"varrho" == wsToken) return TypeElement::varrho;
			else if(L"phi" == wsToken) return TypeElement::phi_small;
			else if(L"BETA" == wsToken) return TypeElement::beta;
			else if(L"ETA" == wsToken) return TypeElement::eta;
			else if(L"MU" == wsToken) return TypeElement::mu;
			else if(L"RHO" == wsToken) return TypeElement::rho;
			else if(L"CHI" == wsToken) return TypeElement::chi;
			else if(L"beta" == wsToken) return TypeElement::beta_small;
			else if(L"zeta" == wsToken) return TypeElement::zeta_small;
			else if(L"kappa" == wsToken) return TypeElement::kappa_small;
			else if(L"omicron" == wsToken) return TypeElement::omicron_small;
			else if(L"sigma" == wsToken) return TypeElement::sigma_small;
			else if(L"varphi" == wsToken) return TypeElement::varphi;
			else if(L"GAMMA" == wsToken) return TypeElement::gamma;
			else if(L"THETA" == wsToken) return TypeElement::theta;
			else if(L"NU" == wsToken) return TypeElement::nu;
			else if(L"SIGMA" == wsToken) return TypeElement::sigma;
			else if(L"PSI" == wsToken) return TypeElement::psi;
			else if(L"gamma" == wsToken) return TypeElement::gamma_small;
			else if(L"eta" == wsToken) return TypeElement::eta_small;
			else if(L"lambda" == wsToken) return TypeElement::lambda_small;
			else if(L"pi" == wsToken) return TypeElement::pi_small;
			else if(L"varsigma" == wsToken) return TypeElement::varsigma;
			else if(L"chi" == wsToken) return TypeElement::chi_small;
			else if(L"DELTA" == wsToken) return TypeElement::delta;
			else if(L"IOTA" == wsToken) return TypeElement::iota;
			else if(L"XI" == wsToken) return TypeElement::xi;
			else if(L"TAU" == wsToken) return TypeElement::tau;
			else if(L"OMEGA" == wsToken) return TypeElement::omega;
			else if(L"delta" == wsToken) return TypeElement::delta_small;
			else if(L"theta" == wsToken) return TypeElement::theta_small;
			else if(L"mu" == wsToken) return TypeElement::mu_small;
			else if(L"varpi" == wsToken) return TypeElement::varpi;
			else if(L"tau" == wsToken) return TypeElement::tau_small;
			else if(L"psi" == wsToken) return TypeElement::psi_small;
			else if(L"EPSILON" == wsToken) return TypeElement::epsilon;
			else if(L"KAPPA" == wsToken) return TypeElement::kappa;
			else if(L"OMICRON" == wsToken) return TypeElement::omicron;
			else if(L"UPSILON" == wsToken) return TypeElement::upsilon;
			else if(L"epsilon" == wsToken) return TypeElement::epsilon_small;
			else if(L"vartheta" == wsToken) return TypeElement::vartheta;
			else if(L"nu" == wsToken) return TypeElement::nu_small;
			else if(L"rho" == wsToken) return TypeElement::rho_small;
			else if(L"upsilon" == wsToken) return TypeElement::upsilon_small;
			else if(L"omega" == wsToken) return TypeElement::omega_small;
			else if(L"and" == wsToken) return TypeElement::And;
			else if(L"infinite" == wsToken) return TypeElement::infinity;
			else if(L"perthousand" == wsToken) return TypeElement::perthousand;
			else if(L"angle" == wsToken) return TypeElement::notin;
			else if(L"strictlygreaterthan" == wsToken) return TypeElement::dlriarrow;
			else if(L"element" == wsToken) return TypeElement::in;
			else if(L"notequal" == wsToken) return TypeElement::notequals;
			else if(L"strictlylessthan" == wsToken) return TypeElement::dllearrow;
			else if(L"identical" == wsToken) return TypeElement::equiv;
			else if(L"or" == wsToken) return TypeElement::Or;
			else if(L"tendto" == wsToken) return TypeElement::rightarrow;
		}
		return TypeElement::undefine;
	}
	void CElementSpecialSymbol::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		SetTypeSymbol();
		switch(m_enTypeSpecial)
		{
		case TypeElement::fact:
		{
			CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pValue);
			pXmlWrite->WriteNodeBegin(L"m:r",false);
			CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeBegin(L"m:t",false);
			pXmlWrite->WriteString(L"\u0021");
			pXmlWrite->WriteNodeEnd(L"m:t",false,false);
			pXmlWrite->WriteNodeEnd(L"m:r",false,false);
			break;
		}
		case TypeElement::interval:
		{
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute());
			break;
		}
		case TypeElement::emptiness:
		{
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute());
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute());
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute());
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute());
			break;
		}
		case TypeElement::newline:
		{
			pXmlWrite->WriteNodeBegin(L"m:r",false);
			pXmlWrite->WriteNodeBegin(L"w:br",true);
			pXmlWrite->WriteNodeEnd(L"",true,true);
			pXmlWrite->WriteNodeEnd(L"m:r",false,false);
			pXmlWrite->WriteNodeEnd(L"m:oMath",false,false);
			pXmlWrite->WriteNodeBegin(L"m:oMath",false);
			break;
		}
		default:
		{
			if(!m_wsType.empty())
			{
				pXmlWrite->WriteNodeBegin(L"m:r",false);
				CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
				pXmlWrite->WriteNodeBegin(L"m:t",false);
				pXmlWrite->WriteString(m_wsType);
				pXmlWrite->WriteNodeEnd(L"m:t",false,false);
				pXmlWrite->WriteNodeEnd(L"m:r",false,false);
			}
		break;
		}
		}
	}
	void CElementSpecialSymbol::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
	}
	void CElementSpecialSymbol::SetTypeSymbol()
	{
		switch(m_enTypeSpecial)
		{
		case TypeElement::infinity:
		m_wsType = L"\u221E";
		break;
		case TypeElement::partial:
		m_wsType = L"\u2202";
		break;
		case TypeElement::nabla:
		m_wsType = L"\u2207";
		break;
		case TypeElement::exists:
		m_wsType = L"\u2203";
		break;
		case TypeElement::notexists:
		m_wsType = L"\u2204";
		break;
		case TypeElement::forall:
		m_wsType = L"\u2200";
		break;
		case TypeElement::hbar:
		m_wsType = L"\u";
		break;
		case TypeElement::lambdabar:
		m_wsType = L"\u";
		break;
		case TypeElement::Re:
		m_wsType = L"\u211C";
		break;
		case TypeElement::Im:
		m_wsType = L"\u2111";
		break;
		case TypeElement::wp:
		m_wsType = L"\u";
		break;
		case TypeElement::laplace:
		m_wsType = L"\u";
		break;
		case TypeElement::fourier:
		m_wsType = L"\u0192";
		break;
		case TypeElement::backepsilon:
		m_wsType = L"\u03F6";
		break;
		case TypeElement::alpha:
		m_wsType = L"\u0391";
		break;
		case TypeElement::alpha_small:
		m_wsType = L"\u03B1";
		break;
		case TypeElement::zeta:
		m_wsType = L"\u0396";
		break;
		case TypeElement::zeta_small:
		m_wsType = L"\u03B6";
		break;
		case TypeElement::lambda:
		m_wsType = L"\u039B";
		break;
		case TypeElement::lambda_small:
		m_wsType = L"\u03BB";
		break;
		case TypeElement::pi:
		m_wsType = L"\u03A0";
		break;
		case TypeElement::pi_small:
		m_wsType = L"\u03C0";
		break;
		case TypeElement::phi:
		m_wsType = L"\u03A6";
		break;
		case TypeElement::phi_small:
		m_wsType = L"\u03C6";
		break;
		case TypeElement::varepsilon:
		m_wsType = L"\u03B5";
		break;
		case TypeElement::iota_small:
		m_wsType = L"\u03B9";
		break;
		case TypeElement::iota:
		m_wsType = L"\u0399";
		break;
		case TypeElement::xi_small:
		m_wsType = L"\u03BE";
		break;
		case TypeElement::xi:
		m_wsType = L"\u039E";
		break;
		case TypeElement::varrho:
		m_wsType = L"\u03F1";
		break;
		case TypeElement::beta:
		m_wsType = L"\u0392";
		break;
		case TypeElement::beta_small:
		m_wsType = L"\u03B2";
		break;
		case TypeElement::eta:
		m_wsType = L"\u0397";
		break;
		case TypeElement::eta_small:
		m_wsType = L"\u03B7";
		break;
		case TypeElement::mu:
		m_wsType = L"\u039C";
		break;
		case TypeElement::mu_small:
		m_wsType = L"\u03BC";
		break;
		case TypeElement::rho:
		m_wsType = L"\u03A1";
		break;
		case TypeElement::rho_small:
		m_wsType = L"\u03C1";
		break;
		case TypeElement::chi:
		m_wsType = L"\u03A7";
		break;
		case TypeElement::chi_small:
		m_wsType = L"\u03C7";
		break;
		case TypeElement::kappa_small:
		m_wsType = L"\u03BA";
		break;
		case TypeElement::kappa:
		m_wsType = L"\u039A";
		break;
		case TypeElement::omicron:
		m_wsType = L"\u039F";
		break;
		case TypeElement::omicron_small:
		m_wsType = L"\u03BF";
		break;
		case TypeElement::sigma:
		m_wsType = L"\u03A3";
		break;
		case TypeElement::sigma_small:
		m_wsType = L"\u03C3";
		break;
		case TypeElement::varphi:
		m_wsType = L"\u03C6";
		break;
		case TypeElement::gamma:
		m_wsType = L"\u0393";
		break;
		case TypeElement::gamma_small:
		m_wsType = L"\u03B3";
		break;
		case TypeElement::theta:
		m_wsType = L"\u0398";
		break;
		case TypeElement::theta_small:
		m_wsType = L"\u03B8";
		break;
		case TypeElement::nu:
		m_wsType = L"\u039D";
		break;
		case TypeElement::nu_small:
		m_wsType = L"\u03BD";
		break;
		case TypeElement::psi:
		m_wsType = L"\u03A8";
		break;
		case TypeElement::psi_small:
		m_wsType = L"\u03C8";
		break;
		case TypeElement::varsigma:
		m_wsType = L"\u03DB";
		break;
		case TypeElement::delta:
		m_wsType = L"\u0394";
		break;
		case TypeElement::delta_small:
		m_wsType = L"\u03B4";
		break;
		case TypeElement::tau:
		m_wsType = L"\u03A4";
		break;
		case TypeElement::tau_small:
		m_wsType = L"\u03C4";
		break;
		case TypeElement::omega:
		m_wsType = L"\u03A9";
		break;
		case TypeElement::omega_small:
		m_wsType = L"\u03C9";
		break;
		case TypeElement::varpi:
		m_wsType = L"\u03D6";
		break;
		case TypeElement::epsilon:
		m_wsType = L"\u0395";
		break;
		case TypeElement::epsilon_small:
		m_wsType = L"\u03B5";
		break;
		case TypeElement::upsilon:
		m_wsType = L"\u03A5";
		break;
		case TypeElement::upsilon_small:
		m_wsType = L"\u03C5";
		break;
		case TypeElement::vartheta:
		m_wsType = L"\u03D1";
		break;
		case TypeElement::And:
		m_wsType = L"\u2227";
		break;
		case TypeElement::Or:
		m_wsType = L"\u2228";
		break;
		case TypeElement::perthousand:
		m_wsType = L"\u2030";
		break;
		case TypeElement::angle:
		m_wsType = L"\u2222";
		break;
		case TypeElement::notin:
		m_wsType = L"\u2209";
		break;
		case TypeElement::dlriarrow:
		m_wsType = L"\u\u226B";
		break;
		case TypeElement::dllearrow:
		m_wsType = L"\u226A";
		break;
		case TypeElement::in:
		m_wsType = L"\u2208";
		break;
		case TypeElement::notequals:
		m_wsType = L"\u2260";
		break;
		case TypeElement::equiv:
		m_wsType = L"\u2261";
		break;
		case TypeElement::rightarrow:
		m_wsType = L"\u1F872";
		break;
		case TypeElement::leftarrow:
		m_wsType = L"\u1F870";
		break;
		case TypeElement::uparrow:
		m_wsType = L"\u1F871";
		break;
		case TypeElement::downarrow:
		m_wsType = L"\u1F873";
		break;
		case TypeElement::dotsaxis:
		m_wsType = L"\u22EF";
		break;
		case TypeElement::dotsup:
		m_wsType = L"\u22F0";
		break;
		case TypeElement::dotsdown:
		m_wsType = L"\u22F1";
		break;
		case TypeElement::dotsvert:
		m_wsType = L"\u22EE";
		break;
		case TypeElement::dotslow:
		m_wsType = L"\u2026";
		break;
		default:
		break;
		}
	}
//class methods CElementSetOperations
	CElementSetOperations::CElementSetOperations(const TypeElement &enType)
		:CElement(TypeElement::SetOperations),m_pLeftArgument(nullptr), m_pRightArgument(nullptr),m_enTypeSet(enType)
	{
	}
	CElementSetOperations::~CElementSetOperations()
	{
		delete m_pLeftArgument;
		delete m_pRightArgument;
	}
	void CElementSetOperations::SetLeftArg(CElement *pElement)
	{
		m_pLeftArgument = pElement;
	}
	CElement* CElementSetOperations::GetLeftArg()
	{
		return m_pLeftArgument;
	}
	void CElementSetOperations::SetRightArg(CElement *pElement)
	{
		m_pRightArgument = pElement;
	}
	CElement* CElementSetOperations::GetRightArg()
	{
		return m_pRightArgument;
	}
	void CElementSetOperations::Parse(CStarMathReader* pReader)
	{
		CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		if((pReader->GetGlobalType() == TypeElement::BinOperator && pReader->GetLocalType()!=TypeElement::frac) || pReader->GetGlobalType() == TypeElement::BracketWithIndex || (pReader->GetGlobalType() == TypeElement::Index && (pReader->GetLocalType()!=TypeElement::nroot || pReader->GetLocalType() != TypeElement::sqrt)))
		{
			CElement* pElement = CParserStarMathString::ParseElement(pReader);
			CParserStarMathString::AddLeftArgument(pTempElement,pElement);
			SetRightArg(pElement);
		}
		else SetRightArg(pTempElement);
	}
	void CElementSetOperations::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArgument);
		pXmlWrite->WriteNodeBegin(L"m:r", false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch(m_enTypeSet)
		{
			case TypeElement::Union:
				pXmlWrite->WriteString(L"\u22C3");
				break;
			case TypeElement::intersection:
				pXmlWrite->WriteString(L"\u22C2");
				break;
			case TypeElement::setminus:
				pXmlWrite->WriteString(L"\u2216");
				break;
			case TypeElement::setquotient:
				pXmlWrite->WriteString(L"\u2215");
				break;
			case TypeElement::subset:
				pXmlWrite->WriteString(L"\u2282");
				break;
			case TypeElement::subseteq:
				pXmlWrite->WriteString(L"\u2286");
				break;
			case TypeElement::supset:
				pXmlWrite->WriteString(L"\u2283");
				break;
			case TypeElement::supseteq:
				pXmlWrite->WriteString(L"\u2287");
				break;
			case TypeElement::nsubset:
				pXmlWrite->WriteString(L"\u2284");
				break;
			case TypeElement::nsubseteq:
				pXmlWrite->WriteString(L"\u2288");
				break;
			case TypeElement::nsupset:
				pXmlWrite->WriteString(L"\u2285");
				break;
			case TypeElement::nsupseteq:
				pXmlWrite->WriteString(L"\u2289");
				break;
			case TypeElement::in:
				pXmlWrite->WriteString(L"\u2208");
				break;
			case TypeElement::owns:
				pXmlWrite->WriteString(L"\u220B");
				break;
			case TypeElement::notin:
				pXmlWrite->WriteString(L"\u2209");
				break;
		default:
			break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
		CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pRightArgument);
	}
	TypeElement CElementSetOperations::GetSetOperation(const std::wstring &wsToken)
	{
		if(L"intersection" == wsToken) return TypeElement::intersection;
		else if(L"union" == wsToken) return TypeElement::Union;
		else if(L"setminus" == wsToken) return TypeElement::setminus;
		else if(L"setquotient" == wsToken) return TypeElement::setquotient;
		else if(L"subseteq" == wsToken) return TypeElement::subseteq;
		else if(L"subset" == wsToken) return TypeElement::subset;
		else if(L"supset" == wsToken) return TypeElement::supset;
		else if(L"supseteq" == wsToken) return TypeElement::supseteq;
		else if(L"nsubset" == wsToken) return TypeElement::nsubset;
		else if(L"nsubseteq" == wsToken) return TypeElement::nsubseteq;
		else if(L"nsupset" == wsToken) return TypeElement::nsupset;
		else if(L"nsupseteq" == wsToken) return TypeElement::nsupseteq;
		else if(L"in" == wsToken) return TypeElement::in;
		else if(L"notin" == wsToken) return TypeElement::notin;
		else if(L"owns" == wsToken) return TypeElement::owns;
		else return TypeElement::undefine;
	}
	void CElementSetOperations::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pLeftArgument!= nullptr)
			m_pLeftArgument->SetAttribute(pAttribute);
		if(m_pRightArgument != nullptr)
			m_pRightArgument->SetAttribute(pAttribute);
	}
//class methods CElementConnection
	CElementConnection::CElementConnection(const TypeElement& enType)
		:CElement(TypeElement::Connection),m_pLeftArgument(nullptr), m_pRightArgument(nullptr),m_enTypeCon(enType)
	{
	}
	CElementConnection::~CElementConnection()
	{
		delete m_pLeftArgument;
		delete m_pRightArgument;
	}
	void CElementConnection::SetRightArg(CElement *pElement)
	{
		m_pRightArgument = pElement;
	}
	CElement* CElementConnection::GetRightArg()
	{
		return m_pRightArgument;
	}
	void CElementConnection::SetLeftArg(CElement *pElement)
	{
		m_pLeftArgument = pElement;
	}
	CElement* CElementConnection::GetLeftArg()
	{
		return m_pLeftArgument;
	}
	void CElementConnection::Parse(CStarMathReader *pReader)
	{
//		pReader->SetAttribute(GetAttribute());
		CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		if((pReader->GetGlobalType() == TypeElement::BinOperator && pReader->GetLocalType()!=TypeElement::frac) || pReader->GetGlobalType() == TypeElement::BracketWithIndex || (pReader->GetGlobalType() == TypeElement::Index && (pReader->GetLocalType()!=TypeElement::nroot || pReader->GetLocalType()!=TypeElement::sqrt)))
		{
//			pReader->SetAttribute(GetAttribute());
			CElement* pBinOp = CParserStarMathString::ParseElement(pReader);
			CParserStarMathString::AddLeftArgument(pTempElement,pBinOp);
			SetRightArg(pBinOp);
		}
		else SetRightArg(pTempElement);
	}
	void CElementConnection::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArgument);
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch(m_enTypeCon)
		{
			case TypeElement::equals:
				pXmlWrite->WriteString(L"=");
				break;
			case TypeElement::notequals:
				pXmlWrite->WriteString(L"\u2260");
				break;
			case TypeElement::learrow:
				pXmlWrite->WriteString(L"<");
				break;
			case TypeElement::learrowequals:
				pXmlWrite->WriteString(L"\u2264");
				break;
			case TypeElement::riarrow:
				pXmlWrite->WriteString(L">");
				break;
			case TypeElement::riarrowequals:
				pXmlWrite->WriteString(L"\u2265");
				break;
			case TypeElement::dllearrow:
				pXmlWrite->WriteString(L"\u226A");
				break;
			case TypeElement::dlriarrow:
				pXmlWrite->WriteString(L"\u226B");
				break;
			case TypeElement::dlrarrow:
				pXmlWrite->WriteString(L"\u21D4");
				break;
			case TypeElement::drarrow:
				pXmlWrite->WriteString(L"\u21D2");
				break;
			case TypeElement::dlarrow:
				pXmlWrite->WriteString(L"\u21D0");
				break;
			case TypeElement::prec:
				pXmlWrite->WriteString(L"\u227A");
				break;
			case TypeElement::succ:
				pXmlWrite->WriteString(L"\u227B");
				break;
			case TypeElement::preccurlyeq:
				pXmlWrite->WriteString(L"\u227C");
				break;
			case TypeElement::succcurlyeq:
				pXmlWrite->WriteString(L"\u227D");
				break;
			case TypeElement::precsim:
				pXmlWrite->WriteString(L"\u227E");
				break;
			case TypeElement::succsim:
				pXmlWrite->WriteString(L"\u227F");
				break;
			case TypeElement::nprec:
				pXmlWrite->WriteString(L"\u2280");
				break;
			case TypeElement::nsucc:
				pXmlWrite->WriteString(L"\u2281");
				break;
			case TypeElement::approx:
				pXmlWrite->WriteString(L"\u2248");
				break;
			case TypeElement::sim:
				pXmlWrite->WriteString(L"\u223C");
				break;
			case TypeElement::simeq:
				pXmlWrite->WriteString(L"\u2243");
				break;
			case TypeElement::equiv:
				pXmlWrite->WriteString(L"\u2261");
				break;
			case TypeElement::prop:
				pXmlWrite->WriteString(L"\u221D");
				break;
			case TypeElement::parallel:
				pXmlWrite->WriteString(L"\u2225");
				break;
			case TypeElement::ortho:
				pXmlWrite->WriteString(L"\u22A5");
				break;
			case TypeElement::divides:
				pXmlWrite->WriteString(L"\u2223");
				break;
			case TypeElement::ndivides:
				pXmlWrite->WriteString(L"\u2224");
				break;
			case TypeElement::toward:
				pXmlWrite->WriteString(L"\u2192");
				break;
			case TypeElement::transl:
				pXmlWrite->WriteString(L"\u22B7");
				break;
			case TypeElement::transr:
				pXmlWrite->WriteString(L"\u22B6");
				break;
			case TypeElement::def:
				pXmlWrite->WriteString(L"\u225D");
				break;
			default:
				break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t", false, false);
		pXmlWrite->WriteNodeEnd(L"m:r",false ,false);
		CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pRightArgument);
	}
	TypeElement CElementConnection::GetConnection(const std::wstring& wsToken)
	{
		if(L"approx" == wsToken) return TypeElement::approx;
		else if(L"sim" == wsToken) return TypeElement::sim;
		else if(L"simeq" == wsToken) return TypeElement::simeq;
		else if(L"equiv" == wsToken) return TypeElement::equiv;
		else if(L"prop" == wsToken) return TypeElement::prop;
		else if(L"parallel" == wsToken) return TypeElement::parallel;
		else if(L"ortho" == wsToken) return TypeElement::ortho;
		else if(L"divides" == wsToken) return TypeElement::divides;
		else if(L"ndivides" == wsToken) return TypeElement::ndivides;
		else if(L"toward" == wsToken) return TypeElement::toward;
		else if(L"transl" == wsToken) return TypeElement::transl;
		else if(L"transr" == wsToken) return TypeElement::transr;
		else if(L"def" == wsToken) return TypeElement::def;
		else if(L"=" == wsToken) return TypeElement::equals;
		else if(L"<>" == wsToken) return TypeElement::notequals;
		else if(L"<" == wsToken) return TypeElement::learrow;
		else if(L"<=" == wsToken) return TypeElement::learrowequals;
		else if(L"leslant" == wsToken) return TypeElement::leslant;
		else if(L">" == wsToken) return TypeElement::riarrow;
		else if(L">=" == wsToken) return TypeElement::riarrowequals;
		else if(L"geslant" == wsToken) return TypeElement::geslant;
		else if(L"<<" == wsToken) return TypeElement::dllearrow;
		else if(L">>" == wsToken) return TypeElement::dlriarrow;
		else if(L"prec" == wsToken) return TypeElement::prec;
		else if(L"succ" == wsToken) return TypeElement::succ;
		else if(L"preccurlyeq" == wsToken) return TypeElement::preccurlyeq;
		else if(L"succcurlyeq" == wsToken) return TypeElement::succcurlyeq;
		else if(L"precsim" == wsToken) return TypeElement::precsim;
		else if(L"succsim" == wsToken) return TypeElement::succsim;
		else if(L"nprec" == wsToken) return TypeElement::nprec;
		else if(L"nsucc" == wsToken) return TypeElement::nsucc;
		else if(L"dlarrow" == wsToken) return TypeElement::dlarrow;
		else if(L"dlrarrow" == wsToken) return TypeElement::dlrarrow;
		else if(L"drarrow" == wsToken) return TypeElement::drarrow;
		else return TypeElement::undefine;
}
	void CElementConnection::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pLeftArgument!= nullptr)
			m_pLeftArgument->SetAttribute(pAttribute);
		if(m_pRightArgument != nullptr)
			m_pRightArgument->SetAttribute(pAttribute);
	}
//class methods CIndex
	CElementIndex::CElementIndex(const TypeElement& enType)
		: CElement(TypeElement::Index),m_pValueIndex(nullptr),m_pLeftArg(nullptr),m_enTypeIndex(enType)
	{
	}
	CElementIndex::~CElementIndex()
	{
		delete m_pValueIndex;
		delete m_pLeftArg;
	}
	void CElementIndex::SetValueIndex(CElement *pElement)
	{
		m_pValueIndex = pElement;
	}
	void CElementIndex::SetLeftArg(CElement *pElement)
	{
		if(m_pLeftArg == nullptr)
			m_pLeftArg = pElement;
	}
	CElement* CElementIndex::GetValueIndex()
	{
		return m_pValueIndex;
	}
	CElement* CElementIndex::GetLeftArg()
	{
		return m_pLeftArg;
	}
	TypeElement CElementIndex::GetIndex(const std::wstring &wsCheckToken)
	{
		if(L"^" == wsCheckToken) return TypeElement::upper;
		else if(L"_" == wsCheckToken) return TypeElement::lower;
		else if(L"lsup" == wsCheckToken) return TypeElement::lsup;
		else if(L"lsub" == wsCheckToken) return TypeElement::lsub;
		else if(L"csup" == wsCheckToken) return TypeElement::csup;
		else if(L"csub" == wsCheckToken) return TypeElement::csub;
		else if(L"nroot" == wsCheckToken) return TypeElement::nroot;
		else if(L"sqrt" == wsCheckToken) return TypeElement::sqrt;
		else return TypeElement::undefine;
	}
	void CElementIndex::Parse(CStarMathReader *pReader)
	{
		if(m_enTypeIndex == TypeElement::nroot)
		{
			SetLeftArg(CParserStarMathString::ParseElement(pReader));
			SetValueIndex(CParserStarMathString::ParseElement(pReader));
		}
		else
			SetValueIndex(CParserStarMathString::ParseElement(pReader));
		if(m_enTypeIndex == TypeElement::sqrt)
			SetLeftArg(GetValueIndex());
	}
	void CElementIndex::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		switch(m_enTypeIndex)
		{
		case TypeElement::upper:
		case TypeElement::lower:
		{
			std::wstring wsNameNodeIndex;
			switch(m_enTypeIndex)
			{
				case TypeElement::upper:
					wsNameNodeIndex = L"m:sSup";
					break;
				case TypeElement::lower:
					wsNameNodeIndex = L"m:sSub";
					break;
			}
			pXmlWrite->WriteNodeBegin(wsNameNodeIndex,false);
			pXmlWrite->WriteNodeBegin(wsNameNodeIndex+L"Pr",false);
			CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeEnd(wsNameNodeIndex+L"Pr",false,false);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pLeftArg,pXmlWrite);
			switch(m_enTypeIndex)
			{
				case TypeElement::upper:
					CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pValueIndex,pXmlWrite);
					break;
				case TypeElement::lower:
					CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pValueIndex,pXmlWrite);
					break;
			}
			pXmlWrite->WriteNodeEnd(wsNameNodeIndex,false,false);
			break;
		}
		case TypeElement::lsub:
		case TypeElement::lsup:
		{
			pXmlWrite->WriteNodeBegin(L"m:sPre",false);
			pXmlWrite->WriteNodeBegin(L"m:sPrePr",false);
			CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeEnd(L"m:sPrePr",false,false);
			switch(m_enTypeIndex)
			{
			case TypeElement::lsup:
			CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pValueIndex,pXmlWrite);
			break;
			case TypeElement::lsub:
			CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pValueIndex,pXmlWrite);
			break;
			}
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pLeftArg,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:sPre",false,false);
			break;
		}
		case TypeElement::nroot:
		case TypeElement::sqrt:
		{
			pXmlWrite->WriteNodeBegin(L"m:rad",false);
			pXmlWrite->WriteNodeBegin(L"m:radPr",false);
			if(TypeElement::sqrt == m_enTypeIndex)
			{
				pXmlWrite->WriteNodeBegin(L"m:degHide",true);
				pXmlWrite->WriteAttribute(L"m:val",1);
				pXmlWrite->WriteNodeEnd(L"",true,true);
			}
			CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeEnd(L"m:radPr",false,false);
			if(m_pLeftArg != nullptr && m_enTypeIndex == TypeElement::nroot)
			{
			pXmlWrite->WriteNodeBegin(L"m:deg",false);
			m_pLeftArg->ConversionToOOXML(pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:deg",false,false);
			}
			else
			{
				pXmlWrite->WriteNodeBegin(L"m:deg",true);
				pXmlWrite->WriteNodeEnd(L"w",true,true);
			}
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pValueIndex,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:rad",false,false);
			break;
		}
		case TypeElement::csub:
		case TypeElement::csup:
		{
			std::wstring wsNameLim;
			switch(m_enTypeIndex)
			{
			case TypeElement::csub:
			wsNameLim = L"m:limLow";
			break;
			case TypeElement::csup:
			wsNameLim = L"m:limUpp";
			break;
			}
			pXmlWrite->WriteNodeBegin(wsNameLim,false);
			pXmlWrite->WriteNodeBegin(wsNameLim+L"Pr",false);
			CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeEnd(wsNameLim+L"Pr",false,false);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pLeftArg,pXmlWrite);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:lim",m_pValueIndex,pXmlWrite);
			pXmlWrite->WriteNodeEnd(wsNameLim,false,false);
			break;
		}
		}
	}
	void CElementIndex::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pLeftArg != nullptr)
			m_pLeftArg->SetAttribute(pAttribute);
		if(m_pValueIndex != nullptr)
			m_pValueIndex->SetAttribute(pAttribute);
	}
//class methods CElementFunction
	CElementFunction::CElementFunction(const TypeElement &enType, const std::wstring &wsNameFunc)
		:CElement(TypeElement::Function), m_pValue(nullptr),m_wsNameFunc(wsNameFunc),m_enTypeFunction(enType)
	{
	}
	CElementFunction::~CElementFunction()
	{
		delete m_pValue;
	}
	void CElementFunction::SetValueFunction(CElement *pElement)
	{
		m_pValue = pElement;
	}
	CElement* CElementFunction::GetValueFunction()
	{
		return m_pValue;
	}
	void CElementFunction::SetNameFunc(const std::wstring &wsNameFunc)
	{
		m_wsNameFunc = wsNameFunc;
	}
	std::wstring CElementFunction::GetNameFuncInString()
	{
		return m_wsNameFunc;
	}
	void CElementFunction::Parse(CStarMathReader* pReader)
	{
		if(pReader->CheckIteratorPosition())
		{
			CElement* pTemp = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
			if(CParserStarMathString::CheckForLeftArgument(pReader->GetGlobalType()))
			{
				 CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
				 CParserStarMathString::AddLeftArgument(pTemp,pTempElement);
				 pTemp = pTempElement;
			}
			SetValueFunction(pTemp);
		}
		else
			SetValueFunction(nullptr);
		//pReader->ClearReader();
	}
	void CElementFunction::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:func",false);
		CConversionSMtoOOXML::PropertiesFuncPr(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeBegin(L"m:fName",false);
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		pXmlWrite->WriteNodeBegin(L"m:rPr",false);
		pXmlWrite->WriteNodeBegin(L"m:sty",true);
		pXmlWrite->WriteAttribute(L"m:val",L"p");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"m:rPr",false,false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch (m_enTypeFunction) {
		case TypeElement::cos:
			pXmlWrite->WriteString(L"cos");
			break;
		case TypeElement::sin:
			pXmlWrite->WriteString(L"sin");
			break;
		case TypeElement::tan:
			pXmlWrite->WriteString(L"tan");
			break;
		case TypeElement::cot:
			pXmlWrite->WriteString(L"cot");
			break;
		case TypeElement::sinh:
			pXmlWrite->WriteString(L"sinh");
			break;
		case TypeElement::cosh:
			pXmlWrite->WriteString(L"cosh");
			break;
		case TypeElement::tanh:
			pXmlWrite->WriteString(L"tanh");
			break;
		case TypeElement::coth:
			pXmlWrite->WriteString(L"coth");
			break;
		case TypeElement::arcsin:
			pXmlWrite->WriteString(L"arcsin");
			break;
		case TypeElement::arccos:
			pXmlWrite->WriteString(L"arccos");
			break;
		case TypeElement::arctan:
			pXmlWrite->WriteString(L"arctan");
			break;
		case TypeElement::arccot:
			pXmlWrite->WriteString(L"arccot");
			break;
		case TypeElement::arsinh:
			pXmlWrite->WriteString(L"arsinh");
			break;
		case TypeElement::arcosh:
			pXmlWrite->WriteString(L"arcosh");
			break;
		case TypeElement::artanh:
			pXmlWrite->WriteString(L"artanh");
			break;
		case TypeElement::arcoth:
			pXmlWrite->WriteString(L"arcoth");
			break;
		case TypeElement::log:
			pXmlWrite->WriteString(L"log");
			break;
		case TypeElement::ln:
			pXmlWrite->WriteString(L"ln");
			break;
		case TypeElement::exp:
			pXmlWrite->WriteString(L"exp");
			break;
		case TypeElement::func:
		{
			if(!m_wsNameFunc.empty())
				pXmlWrite->WriteString(GetNameFuncInString());
			break;
		}
		default:
			break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
		pXmlWrite->WriteNodeEnd(L"m:fName",false,false);
		if(m_pValue!=nullptr)
		{
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pValue,pXmlWrite);
		}
		else
		{
			pXmlWrite->WriteNodeBegin(L"m:e",true);
			pXmlWrite->WriteNodeEnd(L"",true,true);
		}
		pXmlWrite->WriteNodeEnd(L"m:func",false,false);
	}
	TypeElement CElementFunction::GetFunction(const std::wstring &wsToken)
	{
		if(L"sin" == wsToken) return TypeElement::sin;
		else if(L"cos" == wsToken) return TypeElement::cos;
		else if(L"tan" == wsToken) return TypeElement::tan;
		else if(L"cot" == wsToken) return TypeElement::cot;
		else if(L"sinh" == wsToken) return TypeElement::sinh;
		else if(L"cosh" == wsToken) return TypeElement::cosh;
		else if(L"tanh" == wsToken) return TypeElement::tanh;
		else if(L"coth" == wsToken) return TypeElement::coth;
		else if(L"arcsin" == wsToken) return TypeElement::arcsin;
		else if(L"arccos" == wsToken) return TypeElement::arccos;
		else if(L"arctan" == wsToken) return TypeElement::arctan;
		else if(L"arccot" == wsToken) return TypeElement::arccot;
		else if(L"arsinh" == wsToken) return TypeElement::arsinh;
		else if(L"arcosh" == wsToken) return TypeElement::arcosh;
		else if(L"artanh" == wsToken) return TypeElement::artanh;
		else if(L"arcoth" == wsToken) return TypeElement::arcoth;
		else if(L"ln" == wsToken) return TypeElement::ln;
		else if(L"exp" == wsToken) return TypeElement::exp;
		else if(L"log" == wsToken) return TypeElement::log;
		else if(L"func" == wsToken) return TypeElement::func;
		else return TypeElement::undefine;
	}
	void CElementFunction::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pValue != nullptr)
			m_pValue->SetAttribute(pAttribute);
	}
//class methods CElementOperation
	CElementOperator::CElementOperator(const TypeElement &enType, const std::wstring& wsNameOp)
		:CElement(TypeElement::Operator), m_pValueFrom(nullptr), m_pValueTo(nullptr), m_pValueOperator(nullptr),m_enTypeOperator(enType),m_wsName(wsNameOp)
	{
	}
	CElementOperator::~CElementOperator()
	{
		delete m_pValueOperator;
		delete m_pValueFrom;
		delete m_pValueTo;
	}
	void CElementOperator::SetValueOperator(CElement *pElement)
	{
		m_pValueOperator = pElement;
	}
	CElement* CElementOperator::GetValueOperator()
	{
		return m_pValueOperator;
	}
	void CElementOperator::SetFromValue(CElement *pElement)
	{
		m_pValueFrom = pElement;
	}
	CElement* CElementOperator::GetFromValue()
	{
		return m_pValueFrom;
	}
	void CElementOperator::SetToValue(CElement *pElement)
	{
		m_pValueTo = pElement;
	}
	CElement* CElementOperator::GetToValue()
	{
		return m_pValueTo;
	}
	void CElementOperator::SetName(const std::wstring &wsNameOp)
	{
		m_wsName = wsNameOp;
	}
	std::wstring CElementOperator::GetName()
	{
		return m_wsName;
	}
	TypeElement CElementOperator::GetFromOrTo(const std::wstring &wsToken)
	{
		if(L"from" == wsToken) return TypeElement::from;
		else if(L"to" == wsToken) return TypeElement::to;
		else return TypeElement::undefine;
	}
	TypeElement CElementOperator::GetOperator(const std::wstring &wsToken)
	{
		if(L"lim" == wsToken) return TypeElement::lim;
		else if(L"sum" == wsToken) return TypeElement::sum;
		else if(L"liminf" == wsToken) return TypeElement::liminf;
		else if(L"limsup" == wsToken) return TypeElement::limsup;
		else if(L"prod" == wsToken) return TypeElement::prod;
		else if(L"coprod" == wsToken) return TypeElement::coprod;
		else if(L"int" == wsToken) return TypeElement::Int;
		else if(L"iint" == wsToken) return TypeElement::iint;
		else if(L"iiint" == wsToken) return TypeElement::iiint;
		else if(L"lint" == wsToken) return TypeElement::lint;
		else if(L"llint" == wsToken) return TypeElement::llint;
		else if(L"lllint" == wsToken) return TypeElement::lllint;
		else if(L"oper" == wsToken) return TypeElement::oper;
		else return TypeElement::undefine;
	}
	void CElementOperator::Parse(CStarMathReader* pReader)
	{
		pReader->ReadingTheNextToken();
		if(pReader->GetLocalType() == TypeElement::from)
		{
			pReader->ClearReader();
			SetFromValue(CParserStarMathString::ReadingWithoutBracket(pReader));
		}
		if(pReader->GetLocalType() == TypeElement::to)
		{
			pReader->ClearReader();
			SetToValue(CParserStarMathString::ReadingWithoutBracket(pReader));
		}
		SetValueOperator(CParserStarMathString::ParseElement(pReader));
	}
	void CElementOperator::ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite)
	{
		if(m_enTypeOperator == TypeElement::lim || TypeElement::liminf == m_enTypeOperator || TypeElement::limsup == m_enTypeOperator || TypeElement::oper == m_enTypeOperator)
		{
			pXmlWrite->WriteNodeBegin(L"m:func",false);
			CConversionSMtoOOXML::PropertiesFuncPr(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeBegin(L"m:fName",false);
			if(m_pValueFrom != nullptr && m_pValueTo == nullptr)
				CConversionSMtoOOXML::WriteLimUpOrLowNode(pXmlWrite,L"m:limLow",m_pValueFrom,m_enTypeOperator,GetAttribute(),m_wsName);
			else if(m_pValueTo != nullptr && m_pValueFrom == nullptr)
				CConversionSMtoOOXML::WriteLimUpOrLowNode(pXmlWrite,L"m:limUpp",m_pValueTo,m_enTypeOperator,GetAttribute(),m_wsName);
			else if(m_pValueFrom != nullptr && m_pValueTo != nullptr)
			{
				pXmlWrite->WriteNodeBegin(L"m:limUpp",false);
				pXmlWrite->WriteNodeBegin(L"m:limUppPr",false);
				CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,nullptr);
				pXmlWrite->WriteNodeEnd(L"m:limUppPr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:e",false);
				CConversionSMtoOOXML::WriteLimUpOrLowNode(pXmlWrite,L"m:limLow",m_pValueFrom,m_enTypeOperator,GetAttribute(),m_wsName);
				pXmlWrite->WriteNodeEnd(L"m:e",false,false);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:lim",m_pValueTo,pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:limUpp",false,false);
			}
			else if(m_pValueFrom == nullptr && m_pValueTo == nullptr)
				CConversionSMtoOOXML::WriteRPrFName(m_enTypeOperator,pXmlWrite,GetAttribute(),GetName());
			pXmlWrite->WriteNodeEnd(L"m:fName",false,false);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pValueOperator,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:func",false,false);
		}
		else
		{
			pXmlWrite->WriteNodeBegin(L"m:nary",false);
			CConversionSMtoOOXML::PropertiesNaryPr(m_enTypeOperator,nullptr == m_pValueFrom,nullptr == m_pValueTo,pXmlWrite,GetAttribute());
			if(m_pValueFrom == nullptr) pXmlWrite->WriteNode(L"m:sub",L"");
			else
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pValueFrom,pXmlWrite);
			}
			if(m_pValueTo == nullptr) pXmlWrite->WriteNode(L"m:sup",L"");
			else
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pValueTo,pXmlWrite);
			}
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pValueOperator,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:nary",false,false);
		}
	}
	void CElementOperator::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pValueOperator!= nullptr)
			m_pValueOperator->SetAttribute(pAttribute);
		if(m_pValueFrom!= nullptr)
			m_pValueFrom->SetAttribute(pAttribute);
		if(m_pValueTo!=nullptr)
			m_pValueTo->SetAttribute(pAttribute);
	}
// class methods CStarMathReader
	CStarMathReader::CStarMathReader(std::wstring::iterator& itStart, std::wstring::iterator& itEnd)
		: m_enGlobalType(TypeElement::Empty),m_enUnderType(TypeElement::Empty),m_pAttribute(nullptr)
	{
		m_itStart = itStart;
		m_itEnd = itEnd;
		//m_pAttribute = new CAttribute();
	}
	CStarMathReader::~CStarMathReader()
	{
		delete m_pAttribute;
	}
	//TODO :: ParseColor and ParseFont
	void CStarMathReader::GetToken()
	{
		if(CheckIteratorPosition())
		{
			m_wsToken = GetElement();
			TypeElement enTypeFont =CAttribute::GetTypeFontAttribute(m_wsToken),enTypeColor = CAttribute::GetTypeColorAttribute(m_wsToken);
			if(enTypeFont != TypeElement::undefine ||enTypeColor != TypeElement::undefine)
				m_pAttribute = new CAttribute();
			while((enTypeFont != TypeElement::undefine ||enTypeColor != TypeElement::undefine) && m_itStart != m_itEnd)
			{
				if(enTypeColor == TypeElement::color) m_pAttribute->ParseColorAttribute(GetElement(),this);
				else if(enTypeFont != TypeElement::undefine) m_pAttribute->ParseFontAttribute(enTypeFont,this);

				if(m_itStart != m_itEnd)
				{
					m_wsToken = GetElement();
					enTypeColor = CAttribute::GetTypeColorAttribute(m_wsToken);
					enTypeFont = CAttribute::GetTypeFontAttribute(m_wsToken);
				}
			}
			if(m_wsToken == L"left") m_wsToken = GetElement();
			else if(L"right" == m_wsToken ) m_wsToken = GetElement();
		}
		//std::wcout<<m_wsToken << std::endl;
	}
	void CStarMathReader::SetTypesToken()
	{
		m_enUnderType = CElementOperator::GetFromOrTo(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Operation;
			return;
		}
		m_enUnderType = CElementBracketWithIndex::GetBracketWithIndex(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::BracketWithIndex;
			return;
		}
		m_enUnderType = CElementGrade::GetGrade(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Grade;
			return;
		}
		m_enUnderType = CElementIndex::GetIndex(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Index;
			return;
		}
		m_enUnderType=CElementMatrix::GetMatrix(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Matrix;
			return;
		}
		m_enUnderType = CElementDiacriticalMark::GetMark(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Mark;
			return;
		}
		m_enUnderType = CElementBracket::GetBracketOpen(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Bracket;
			return;
		}
		m_enUnderType = CElementString::GetDigit(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
		m_enUnderType = CElementSpecialSymbol::GetSpecialSymbol(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::SpecialSymbol;
			return;
		}
		m_enUnderType = CElementBinOperator::GetBinOperator(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::BinOperator;
			return;
		}
		m_enUnderType = CElementSetOperations::GetSetOperation(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::SetOperations;
			return;
		}
		m_enUnderType = CElementConnection::GetConnection(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Connection;
			return;
		}
		m_enUnderType = CElementFunction::GetFunction(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Function;
			return;
		}
		m_enUnderType = CElementOperator::GetOperator(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Operation;
			return;
		}
		m_enUnderType = CElementString::GetWord(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
		if(m_enUnderType == TypeElement::undefine && !m_wsToken.empty())
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
	}
	TypeElement CStarMathReader::GetLocalType()
	{
		return m_enUnderType;
	}
	TypeElement CStarMathReader::GetGlobalType()
	{
		return m_enGlobalType;
	}
	std::wstring CStarMathReader::GetString()
	{
		return m_wsToken;
	}
	void CStarMathReader::ClearReader()
	{
		m_wsToken.clear();
		m_enGlobalType = TypeElement::Empty;
		m_enUnderType = TypeElement::Empty;
		m_pAttribute = nullptr;
	}
	bool CStarMathReader::EmptyString()
	{
		return m_wsToken.empty();
	}
	bool CStarMathReader::CheckIteratorPosition()
	{
		return (m_itStart != m_itEnd);
	}
	void CStarMathReader::SetAttribute(CAttribute *pAttribute)
	{
		m_pAttribute = pAttribute;
	}
	CAttribute* CStarMathReader::GetAttribute()
	{
		return m_pAttribute;
	}
	void CStarMathReader::SetBaseAttribute(const TBaseAttribute* pAttribute)
	{
		if(pAttribute!=nullptr)
		{
			m_pBaseAttribute = new CAttribute;
			if(pAttribute->base_font_italic)
				m_pBaseAttribute->SetItal();
			if(pAttribute->base_font_bold)
				m_pBaseAttribute->SetBold();
			if(!pAttribute->base_font_name.empty())
				m_pBaseAttribute->SetFontName(pAttribute->base_font_name);
			if(pAttribute->base_font_size != 0)
				m_pBaseAttribute->SetSize(pAttribute->base_font_size);
	//		if(pAttribute.base_alignment != 0)
	//			m_pBaseAttribute->
		}
		else
			m_pBaseAttribute = nullptr;
	}
	CAttribute* CStarMathReader::GetBaseAttribute()
	{
		return m_pBaseAttribute;
	}
	std::wstring CStarMathReader::GetElement()
	{
		std::wstring m_wsElement{};
		for(; m_itStart != m_itEnd;m_itStart++)
		{
			if(iswspace(*m_itStart) && m_wsElement.empty()) continue;
			else if(iswspace(*m_itStart) && !m_wsElement.empty())
			{
				m_itStart++;
				break;
			}
			else if(!m_wsElement.empty() && (CheckTokenForGetElement(*m_itStart) ||  *m_itStart == L'(' || L')' == *m_itStart  || L'%' == *m_itStart||(L'#' == *m_itStart && L'#' != m_wsElement.back()) ||(L'-' == *m_itStart  && L'+' != m_wsElement.back()) || (L'+' == *m_itStart  && L'-' != m_wsElement.back()) || (iswdigit(*m_itStart) && !iswdigit(m_wsElement.back())) || (CheckIsalhpaForGetElement(*m_itStart,m_wsElement.back())) || ((m_wsElement.back() != L'<' && m_wsElement.back() != L'>') && (L'<' == *m_itStart || L'>' == *m_itStart || L'=' == *m_itStart))))
			{
				return m_wsElement;
			}
			else if((( CheckTokenForGetElement(*m_itStart) || L'=' == *m_itStart) && m_wsElement.empty()) || (!m_wsElement.empty() && ((L'#' == m_wsElement.back() && L'#' == *m_itStart)  || (L'-' == *m_itStart  && L'+' == m_wsElement.back()) || (L'+' == *m_itStart  && L'-' == m_wsElement.back()) || (m_wsElement.back() == L'<' && (L'=' == *m_itStart || L'<' == *m_itStart || L'>' == *m_itStart)) || (m_wsElement.back() == L'>' && (L'>' == *m_itStart || L'=' == *m_itStart))  ) ) )
			{
				m_wsElement.push_back(*m_itStart);
				m_itStart++;
				return m_wsElement;
			}
			else
			{
				m_wsElement.push_back(*m_itStart);
			}
		}
		if(!m_wsElement.empty()) return m_wsElement;
		else return {};
	}
	void CStarMathReader::FindingTheEndOfParentheses()
	{
		std::wstring::iterator itStart = m_itStart,itStartBracketClose;
		int inBracketInside = 0;
		while(CheckIteratorPosition())
		{
			itStartBracketClose = m_itStart;
			GetToken();
			if(CElementBracket::GetBracketOpen(m_wsToken) != TypeElement::undefine)
			{
				inBracketInside +=1;
			}
			else if(CElementBracket::GetBracketClose(m_wsToken) != TypeElement::undefine && inBracketInside == 0)
			{
				m_stBracket.push(m_itEnd);
				m_itEnd = itStartBracketClose;
				break;
			}
			else if(CElementBracket::GetBracketClose(m_wsToken) != TypeElement::undefine && inBracketInside != 0)
			{
				inBracketInside -=1;
			}
		}
		ClearReader();
		m_itStart = itStart;
	}
	void CStarMathReader::IteratorNullification()
	{
		if(!m_stBracket.empty())
		{
			m_itEnd = m_stBracket.top();
			m_stBracket.pop();
		}
		while(TypeElement::undefine == CElementBracket::GetBracketClose(GetString()) && CheckIteratorPosition())
		{
			GetToken();
		}
		ClearReader();
	}
	void CStarMathReader::ReadingTheNextToken()
	{
		if(m_wsToken.empty())
		{
			GetToken();
			SetTypesToken();
		}
	}
	bool CStarMathReader::CheckTokenForGetElement(const wchar_t &cToken)
	{
		switch(cToken)
		{
		case L'[':
		case L']':
		case L'{':
		case L'}':
		case L'_':
		case L'^':
		case L'*':
		case L'/':
		case L'`':
		case L'~':
		return true;
		default:
		return false;
		}
	}
	bool CStarMathReader::CheckIsalhpaForGetElement(const wchar_t &cToken, const wchar_t &cLastToken)
	{
		if(isalpha(cToken))
		{
			if(L'%' == cLastToken)
				return false;
			if(isalpha(cLastToken))
				return false;
		}
		else
			return false;
		return true;
	}
//class methods CElementBracketWithIndex
	CElementBracketWithIndex::CElementBracketWithIndex(const TypeElement &enType)
		:CElement(TypeElement::BracketWithIndex),m_pLeftArg(nullptr), m_pValue(nullptr),m_enTypeBracketWithIndex(enType)
	{
	}
	CElementBracketWithIndex::~CElementBracketWithIndex()
	{
		delete m_pLeftArg;
		delete m_pValue;
	}
	void CElementBracketWithIndex::SetLeftArg(CElement *pElement)
	{
		m_pLeftArg = pElement;
	}
	void CElementBracketWithIndex::SetBracketValue(CElement *pElement)
	{
		m_pValue = pElement;
	}
	CElement* CElementBracketWithIndex::GetLeftArg()
	{
		return m_pLeftArg;
	}
	void CElementBracketWithIndex::Parse(CStarMathReader *pReader)
	{
		SetBracketValue(CParserStarMathString::ParseElement(pReader));
	}
	void CElementBracketWithIndex::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		std::wstring wsNameNode;
		switch (m_enTypeBracketWithIndex) {
		case TypeElement::overbrace:
		wsNameNode = L"m:limUpp";
		break;
		case TypeElement::underbrace:
		wsNameNode = L"m:limLow";
		break;
		default:
		break;
		}
		pXmlWrite->WriteNodeBegin(wsNameNode,false);
		pXmlWrite->WriteNodeBegin(wsNameNode+L"Pr",false);
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeEnd(wsNameNode+L"Pr",false,false);
		pXmlWrite->WriteNodeBegin(L"m:e",false);
		pXmlWrite->WriteNodeBegin(L"m:groupChr",false);
		pXmlWrite->WriteNodeBegin(L"m:groupChrPr",false);
		if(TypeElement::overbrace == m_enTypeBracketWithIndex)
		{
			pXmlWrite->WriteNodeBegin(L"m:chr",true);
			pXmlWrite->WriteAttribute(L"m:val",L"\u23DE");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			pXmlWrite->WriteNodeBegin(L"m:pos",true);
			pXmlWrite->WriteAttribute(L"m:val",L"top");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			pXmlWrite->WriteNodeBegin(L"m:vertJc",true);
			pXmlWrite->WriteAttribute(L"m:val",L"bot");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeEnd(L"m:groupChrPr",false,false);
		pXmlWrite->WriteNodeBegin(L"m:e",false);
		switch(m_enTypeBracketWithIndex)
		{
		case TypeElement::overbrace:
			CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArg);
		break;
		case TypeElement::underbrace:
			CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pValue);
		break;
		default:
		break;
		}
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		pXmlWrite->WriteNodeEnd(L"m:groupChr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		pXmlWrite->WriteNodeBegin(L"m:lim",false);
		switch(m_enTypeBracketWithIndex)
		{
		case TypeElement::overbrace:
			CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pValue);
		break;
		case TypeElement::underbrace:
			CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArg);
		break;
		default:
		break;
		}
		pXmlWrite->WriteNodeEnd(L"m:lim",false,false);
		pXmlWrite->WriteNodeEnd(wsNameNode,false,false);
	}
	TypeElement CElementBracketWithIndex::GetBracketWithIndex(const std::wstring &wsToken)
	{
		if(L"overbrace" == wsToken) return TypeElement::overbrace;
		else if(L"underbrace" == wsToken) return TypeElement::underbrace;
		else return TypeElement::undefine;
	}
	void CElementBracketWithIndex::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pValue!= nullptr)
			m_pValue->SetAttribute(pAttribute);
		if(m_pLeftArg != nullptr)
			m_pLeftArg->SetAttribute(pAttribute);
	}
//class methods CElementGrade
	CElementGrade::CElementGrade()
		:CElement(TypeElement::Grade),m_pValueGrade(nullptr), m_pValueFrom(nullptr), m_pValueTo(nullptr)
	{
	}
	CElementGrade::~CElementGrade()
	{
		delete m_pValueFrom;
		delete m_pValueTo;
		delete m_pValueGrade;
	}
	void CElementGrade::SetValueFrom(CElement* pElement)
	{
		m_pValueFrom = pElement;
	}
	void CElementGrade::SetValueTo(CElement *pElement)
	{
		m_pValueTo = pElement;
	}
	void CElementGrade::SetValueGrade(CElement *pElement)
	{
		m_pValueGrade = pElement;
	}
	void CElementGrade::Parse(CStarMathReader *pReader)
	{
		SetValueGrade(CParserStarMathString::ParseElement(pReader));
		pReader->ReadingTheNextToken();
		if(pReader->GetLocalType() == TypeElement::from)
		{
			pReader->ClearReader();
			SetValueFrom(CParserStarMathString::ParseElement(pReader));
			pReader->ReadingTheNextToken();
		}
		if(pReader->GetLocalType() == TypeElement::to)
		{
			pReader->ClearReader();
			SetValueTo(CParserStarMathString::ParseElement(pReader));
		}
		if(GetAttribute() != nullptr)
			SetAttribute(GetAttribute());
	}
	void CElementGrade::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(m_pValueFrom == nullptr && m_pValueTo == nullptr)
		{
			CConversionSMtoOOXML::NodeGrade(pXmlWrite,m_pValueGrade,GetAttribute());
		}
		else
		{
			std::wstring wsNodeGrade;
			if(m_pValueFrom != nullptr && m_pValueTo == nullptr)
			{
				wsNodeGrade = L"m:sSub";
			}
			else if(m_pValueFrom == nullptr && m_pValueTo != nullptr)
			{
				wsNodeGrade = L"m:sSup";
			}
			else if(m_pValueFrom != nullptr && m_pValueTo != nullptr)
			{
				wsNodeGrade  = L"m:sSubSup";
			}
			pXmlWrite->WriteNodeBegin(wsNodeGrade,false);
			pXmlWrite->WriteNodeBegin(wsNodeGrade + L"Pr",false);
			pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
			CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute());
			pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
			pXmlWrite->WriteNodeEnd(wsNodeGrade + L"Pr",false,false);
			pXmlWrite->WriteNodeBegin(L"m:e",false);
			CConversionSMtoOOXML::NodeGrade(pXmlWrite,m_pValueGrade,GetAttribute());
			pXmlWrite->WriteNodeEnd(L"m:e",false,false);
			if(m_pValueFrom != nullptr)
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pValueFrom,pXmlWrite);
			}
			if(m_pValueTo != nullptr)
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pValueTo,pXmlWrite);
			}
			pXmlWrite->WriteNodeEnd(wsNodeGrade,false,false);
		}
	}
	TypeElement CElementGrade::GetGrade(const std::wstring &wsToken)
	{
		if(L"evaluate" == wsToken) return TypeElement::evaluate;
		else return TypeElement::undefine;
	}
	void CElementGrade::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pValueGrade!=nullptr)
			m_pValueGrade->SetAttribute(pAttribute);
		if(m_pValueFrom!=nullptr)
			m_pValueFrom->SetAttribute(pAttribute);
		if(m_pValueTo!=nullptr)
			m_pValueTo->SetAttribute(pAttribute);
	}
//class methods CElementMatrix
	CElementMatrix::CElementMatrix(const TypeElement &enType)
		:CElement(TypeElement::Matrix), m_pFirstArgument(nullptr), m_pSecondArgument(nullptr), m_enTypeMatrix(enType)
	{
	}
	CElementMatrix::~CElementMatrix()
	{
		delete m_pFirstArgument;
		delete m_pSecondArgument;
	}
	void CElementMatrix::SetFirstArgument(CElement *pElement)
	{
		m_pFirstArgument = pElement;
	}
	void CElementMatrix::SetSecondArgument(CElement *pElement)
	{
		m_pSecondArgument = pElement;
	}
	TypeElement CElementMatrix::GetMatrix(const std::wstring &wsToken)
	{
		if(L"binom" == wsToken) return TypeElement::binom;
		else if(L"stack" == wsToken) return TypeElement::stack;
		else if(L"matrix" == wsToken) return TypeElement::matrix;
		else return TypeElement::undefine;
	}
	void CElementMatrix::Parse(CStarMathReader *pReader)
	{
		if(m_enTypeMatrix == TypeElement::binom)
		{
			SetFirstArgument(CParserStarMathString::ReadingWithoutBracket(pReader));
			SetSecondArgument(CParserStarMathString::ReadingWithoutBracket(pReader));
		}
		else
		{
			SetFirstArgument(CParserStarMathString::ParseElement(pReader));
		}
		if(GetAttribute() != nullptr)
			SetAttribute(GetAttribute());
	}
	void CElementMatrix::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:m",false);
		CConversionSMtoOOXML::PropertiesMPr(pXmlWrite,m_enTypeMatrix,GetAttribute());
		pXmlWrite->WriteNodeBegin(L"m:mr",false);
		switch(m_enTypeMatrix)
		{
			case TypeElement::matrix:
			{
				CElementBracket* pTempBracket = dynamic_cast<CElementBracket*>(m_pFirstArgument);
				std::vector<CElement*> pTempValue = pTempBracket->GetBracketValue();
				for(CElement* pOneElement:pTempValue)
				{
					if(pOneElement->GetBaseType() != TypeElement::SpecialSymbol && pOneElement->GetBaseType()!= TypeElement::undefine)
					{
						CConversionSMtoOOXML::WriteNodeConversion(L"m:e",pOneElement,pXmlWrite);
					}
					else if(pOneElement->GetBaseType() == TypeElement::SpecialSymbol)
					{
						CElementSpecialSymbol* pTempSpecial = dynamic_cast<CElementSpecialSymbol*>(pOneElement);
						if(pTempSpecial->GetType() == TypeElement::transition)
						{
							pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
							pXmlWrite->WriteNodeBegin(L"m:mr",false);
						}
					}
					else if(pOneElement->GetBaseType() != TypeElement::undefine)
						CConversionSMtoOOXML::ElementConversion(pXmlWrite,pOneElement);
				}
				break;
			}
			case TypeElement::stack:
			{
				CElementBracket* pTempBracket = dynamic_cast<CElementBracket*>(m_pFirstArgument);
				std::vector<CElement*> pTempValue = pTempBracket->GetBracketValue();
				for(CElement* pOneElement:pTempValue)
				{
					if(pOneElement->GetBaseType() != TypeElement::SpecialSymbol)
					{
						CConversionSMtoOOXML::WriteNodeConversion(L"m:e",pOneElement,pXmlWrite);
						pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
						pXmlWrite->WriteNodeBegin(L"m:mr",false);
					}
				}
				break;
			}
			case TypeElement::binom:
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pFirstArgument,pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:mr",false);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pSecondArgument,pXmlWrite);
				break;
			}
		}
		pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:m",false,false);
	}
	void CElementMatrix::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pFirstArgument != nullptr)
			m_pFirstArgument->SetAttribute(pAttribute);
		if(m_pSecondArgument != nullptr)
			m_pSecondArgument->SetAttribute(pAttribute);
	}
//class CElementDiacriticalMark
	CElementDiacriticalMark::CElementDiacriticalMark(const TypeElement& enType)
		:CElement(TypeElement::Mark),m_pValueMark(nullptr),m_enTypeMark(enType)
	{
	}
	CElementDiacriticalMark::~CElementDiacriticalMark()
	{
		delete m_pValueMark;
	}
	void CElementDiacriticalMark::SetValueMark(CElement *pValue)
	{
		m_pValueMark = pValue;
	}
	TypeElement CElementDiacriticalMark::GetMark(const std::wstring &wsToken)
	{
		if(L"acute" == wsToken) return TypeElement::acute;
		else if(L"breve" == wsToken) return TypeElement::breve;
		else if(L"dot" == wsToken) return TypeElement::dot;
		else if(L"dddot" == wsToken) return TypeElement::dddot;
		else if(L"vec" == wsToken) return TypeElement::vec;
		else if(L"tilde" == wsToken) return TypeElement::tilde;
		else if(L"check" == wsToken) return TypeElement::check;
		else if(L"grave" == wsToken) return TypeElement::grave;
		else if(L"circle" == wsToken) return TypeElement::circle;
		else if(L"ddot" == wsToken) return TypeElement::ddot;
		else if(L"bar" == wsToken) return TypeElement::bar;
		else if(L"harpoon" == wsToken) return TypeElement::harpoon;
		else if(L"hat" == wsToken) return TypeElement::hat;
		else if(L"widevec" == wsToken) return TypeElement::widevec;
		else if(L"widetilde" == wsToken) return TypeElement::widetilde;
		else if(L"overline" == wsToken) return TypeElement::overline;
		else if(L"overstrike" == wsToken) return TypeElement::overstrike;
		else if(L"wideharpoon" == wsToken) return TypeElement::wideharpoon;
		else if(L"widehat" == wsToken) return TypeElement::widehat;
		else if(L"underline" == wsToken) return TypeElement::underline;
	}
	void CElementDiacriticalMark::Parse(CStarMathReader *pReader)
	{
		SetValueMark(CParserStarMathString::ParseElement(pReader));
	}
	void CElementDiacriticalMark::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		std::wstring wsTypeMark;
		switch(m_enTypeMark)
		{
			case TypeElement::dot:
				wsTypeMark = L"\u0307";
				break;
			case TypeElement::overline:
				wsTypeMark = L"\u0305";
				break;
			case TypeElement::vec:
				wsTypeMark = L"\u20D7";
				break;
			case TypeElement::acute:
				wsTypeMark = L"\u0301";
				break;
			case TypeElement::grave:
				wsTypeMark = L"\u0300";
				break;
			case TypeElement::breve:
				wsTypeMark = L"\u0306";
				break;
			case TypeElement::circle:
				wsTypeMark = L"\u030A";
				break;
			case TypeElement::ddot:
				wsTypeMark = L"\u0308";
				break;
			case TypeElement::bar:
				wsTypeMark = L"\u0304";
				break;
			case TypeElement::dddot:
				wsTypeMark = L"\u20DB";
				break;
			case TypeElement::harpoon:
				wsTypeMark = L"\u20D1";
			break;
			case TypeElement::tilde:
				wsTypeMark = L"\u0342";
			break;
			case TypeElement::hat:
				wsTypeMark = L"\u0302";
			break;
			case TypeElement::check:
				wsTypeMark = L"\u030C";
			break;
			case TypeElement::widevec:
				wsTypeMark = L"\u20D7";
			break;
			case TypeElement::widetilde:
				wsTypeMark = L"\u0360";
			break;
			case TypeElement::wideharpoon:
				wsTypeMark = L"\u20D1";
			break;
			case TypeElement::underline:
				wsTypeMark = L"\u0332";
			break;
			default:
				break;
		}
		pXmlWrite->WriteNodeBegin(L"m:acc",false);
		pXmlWrite->WriteNodeBegin(L"m:accPr",false);
		switch(m_enTypeMark)
		{
			case TypeElement::widehat:
			break;
			default:
			{
				pXmlWrite->WriteNodeBegin(L"m:chr",true);
				pXmlWrite->WriteAttribute(L"m:val",wsTypeMark);
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				break;
			}
		}
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute());
		pXmlWrite->WriteNodeEnd(L"m:accPr",false,false);
		CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pValueMark,pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:acc",false,false);
	}
	void CElementDiacriticalMark::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pValueMark != nullptr)
			m_pValueMark->SetAttribute(pAttribute);
	}
}


