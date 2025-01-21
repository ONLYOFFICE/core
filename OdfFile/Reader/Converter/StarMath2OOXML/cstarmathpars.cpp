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
	CParserStarMathString::CParserStarMathString():m_iAlignment(1){}
	CParserStarMathString::~CParserStarMathString()
	{
		for(CElement* pElement:m_arEquation)
			delete pElement;
	}
	std::vector<CElement*> CParserStarMathString::Parse(std::wstring& wsParseString, int iTypeConversion)
	{
		TypeConversion enTypeConvers = (TypeConversion)iTypeConversion;
		std::wstring::iterator itStart = wsParseString.begin(),itEnd = wsParseString.end();
		CStarMathReader* pReader = new CStarMathReader(itStart,itEnd,enTypeConvers);
		pReader->SetBaseAttribute(m_stBaseAttribute);
		while(pReader->CheckIteratorPosition())
		{
			ParsElementAddingToArray(pReader,m_arEquation);
		}
		if(!pReader->EmptyString())
		{
			if(pReader->GetLocalType() == TypeElement::newline)
			{																																																																																																																																																																																																																																																																																																																																																																											m_arEquation.push_back(new CElementSpecialSymbol(pReader->GetLocalType(),pReader->GetTypeConversion()));
				pReader->ClearReader();
			}
			CElement* pTempElement = ParseElement(pReader);
			if(nullptr != pTempElement)
					m_arEquation.push_back(pTempElement);
		}
		TFormulaSize tSize;
		for(CElement* pElement:m_arEquation)
		{
			if(pElement->GetBaseType() == TypeElement::SpecialSymbol)
			{
				CElementSpecialSymbol* pSpecial = dynamic_cast<CElementSpecialSymbol*>(pElement);
				if(pSpecial->GetType() == TypeElement::newline)
				{
					m_qSize.push(tSize);
					tSize.Zeroing();
				}
				else
					ComparisonByHeight(tSize,pElement->GetSize());			
			}
			else
				ComparisonByHeight(tSize,pElement->GetSize());
		}
		if(tSize.m_iHeight != 0 && tSize.m_iWidth != 0)
			m_qSize.push(tSize);
		return m_arEquation;
	}

	CElement* CParserStarMathString::ParseElement(CStarMathReader* pReader)
	{
		CElement* pElement;
		pReader->ReadingTheNextToken();
		pElement = CElement::CreateElement(pReader);
		if(pElement != nullptr)
		{

			if(pReader->GetAttribute() != nullptr && !CheckForLeftArgument(pReader->GetGlobalType()))
				pElement->SetBaseAttribute(pReader->GetAttribute());
			else if(pReader->GetAttribute() != nullptr && (pReader->GetLocalType() == TypeElement::plus || TypeElement::minus == pReader->GetLocalType() || TypeElement::frac == pReader->GetLocalType() || TypeElement::neg == pReader->GetLocalType()))
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
			if(CParserStarMathString::CheckNewline(pLeftArg) || CParserStarMathString::CheckGrid(pLeftArg))
				return false;
			pTempElement->SetLeftArg(pLeftArg);
			pElementWhichAdd = pTempElement;
			if(pElementWhichAdd->GetBaseType() == TypeElement::Index)
				pElementWhichAdd->SetAttribute(nullptr);
			return true;
		}
		else return false;
	}

	bool CParserStarMathString::AddLeftArgument(CElement *pLeftArg, CElement *pElementWhichAdd, CStarMathReader *pReader)
	{
		if(pElementWhichAdd!=nullptr)
		{
			switch(pElementWhichAdd->GetBaseType())
			{
				case TypeElement::BinOperator:
				{
					CElementBinOperator* pBinOp = dynamic_cast<CElementBinOperator*>(pElementWhichAdd);
					if(pBinOp->GetType() == TypeElement::neg || (pBinOp->GetAttribute() != nullptr && pBinOp->GetAttribute() != pReader->GetBaseAttribute() && pBinOp->MixedOperators(pBinOp->GetType())))
						return false;
					else
						return SetLeft<CElementBinOperator>(pLeftArg, pElementWhichAdd);
				}
				case TypeElement::SetOperations:
				{
					return SetLeft<CElementSetOperations>(pLeftArg, pElementWhichAdd);
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
					CElementIndex * pIndex = dynamic_cast<CElementIndex*>(pElementWhichAdd);
					if(pIndex->GetType() != TypeElement::sqrt && pIndex->GetType() != TypeElement::nroot)
						return SetLeft<CElementIndex>(pLeftArg,pElementWhichAdd);
					else return false;
				}
				default:
					break;
			}
		}
		else
			return false;
	}
	CElement* CParserStarMathString::ReadingWithoutBracket(CStarMathReader *pReader, const bool& bConnection)
	{
		CElement* pFirstTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		while(CheckForLeftArgument(pReader->GetGlobalType(),bConnection) && (pReader->GetLocalType() != TypeElement::neg && pReader->GetLocalType() != TypeElement::frac && pReader->GetLocalType()!=TypeElement::nroot && pReader->GetLocalType()!=TypeElement::sqrt))
		{
			CParserStarMathString::ReadingElementsWithPriorities(pReader,pFirstTempElement);
		}
		return pFirstTempElement;
	}
	bool CParserStarMathString::CheckForLeftArgument(const TypeElement &enType,const bool& bConnection)
	{
		switch(enType)
		{
			case TypeElement::BinOperator:
			case TypeElement::SetOperations:
			case TypeElement::BracketWithIndex:
			case TypeElement::Index:
				return true;
			case TypeElement::Connection:
				return bConnection;
			default:
				return false;
		}
	}
	bool CParserStarMathString::CheckNewline(CElement* pElement)
	{
		if(pElement == nullptr)
			return false;
		if(pElement->GetBaseType() == TypeElement::SpecialSymbol)
		{
			CElementSpecialSymbol* pSpecial = dynamic_cast<CElementSpecialSymbol*>(pElement);
			return (pSpecial->GetType() == TypeElement::newline);
		}
		else
			return false;
	}
	bool CParserStarMathString::CheckGrid(CElement *pElement)
	{
		if(pElement == nullptr)
			return false;
		if(pElement->GetBaseType() == TypeElement::SpecialSymbol)
		{
			CElementSpecialSymbol* pSpecial = dynamic_cast<CElementSpecialSymbol*>(pElement);
			if(pSpecial->GetType() == TypeElement::grid || pSpecial->GetType() == TypeElement::transition)
				return true;
			else return false;
		}
		else return false;
	}
	void CParserStarMathString::AddingAnElementToAnArray(std::vector<CElement *> &arrEquation, CElement *pAddElement, CStarMathReader *pReader)
	{
		if(pAddElement !=nullptr)
		{
			if(!arrEquation.empty() && CheckForLeftArgument(pAddElement->GetBaseType()))
			{
				if(AddLeftArgument(arrEquation.back(),pAddElement,pReader))
					arrEquation.pop_back();
			}
			arrEquation.push_back(pAddElement);
		}
	}
	void CParserStarMathString::ReadingElementsWithPriorities(CStarMathReader *pReader, CElement *&pLeftElement)
	{
		CElement* pNextElement = ParseElement(pReader);
		if(pLeftElement != nullptr)
			AddLeftArgument(pLeftElement,pNextElement,pReader);
		pLeftElement = pNextElement;
		pReader->ReadingTheNextToken();
	}
	void CParserStarMathString::ReadingElementsWithAttributes(CStarMathReader *pReader, CElement*& pSavingElement)
	{
		CElement* pElement = CParserStarMathString::ParseElement(pReader);
		if(pElement->GetAttribute() != pReader->GetBaseAttribute())
		{
			pReader->ReadingTheNextToken();
			if(CElementIndex::GetLowerIndex(pReader->GetLocalType()) || CElementIndex::GetUpperIndex(pReader->GetLocalType()))
			{
				CElement* pIndex = new CElementIndex(pReader->GetLocalType(),pReader->GetTypeConversion());
				pReader->ClearReader();
				pIndex->Parse(pReader);
				AddLeftArgument(pElement,pIndex,pReader);
				pSavingElement = pIndex;
			}
			else
				pSavingElement = pElement;
		}
		else
			pSavingElement = pElement;
	}
	void CParserStarMathString::SetAlignment(const unsigned int &iAlignment)
	{
		m_iAlignment = iAlignment;
	}
	const unsigned int& CParserStarMathString::GetAlignment()
	{
		return m_stBaseAttribute.base_alignment;
	}
	void CParserStarMathString::SetBaseFont(const std::wstring &wsNameFont)
	{
		m_stBaseAttribute.base_font_name = wsNameFont;
	}
	void CParserStarMathString::SetBaseSize(const unsigned int &iSize)
	{
		if(iSize < 130)
			m_stBaseAttribute.base_font_size = iSize*2;
	}
	void CParserStarMathString::SetBaseAlignment(const unsigned int &iAlignment)
	{
		if(iAlignment >= 0 && iAlignment < 3)
			m_stBaseAttribute.base_alignment = iAlignment;
	}
	void CParserStarMathString::SetBaseBold(const bool &bBold)
	{
		m_stBaseAttribute.base_font_bold = bBold;
	}
	void CParserStarMathString::SetBaseItalic(const bool &bItal)
	{
		m_stBaseAttribute.base_font_italic = bItal;
	}
	std::wstring CParserStarMathString::ConvertToLowerCase(const std::wstring& wsToken)
	{
		if(!wsToken.empty() &&  wsToken[0] == L'%')
			return wsToken;
		std::wstring wsLowerCase;
		for(wchar_t cOneElement:wsToken)
			wsLowerCase+= std::tolower(cOneElement);
		if(wsLowerCase.empty())
			return wsToken;
		else
			return wsLowerCase;
	}
	void CParserStarMathString::ParsElementAddingToArray(CStarMathReader *pReader, std::vector<CElement *> &arElements)
	{
		if(!arElements.empty())
			CElementBinOperator::UnaryCheck(pReader,arElements.back());
		CElement* pTempElement = ParseElement(pReader);
		AddingAnElementToAnArray(arElements,pTempElement,pReader);
	}
	std::queue<TFormulaSize> CParserStarMathString::GetFormulaSize()
	{
		return m_qSize;
	}
	void CParserStarMathString::ComparisonByHeight(TFormulaSize &tLeftSize, const TFormulaSize &tRightSize)
	{
		if(tLeftSize.m_iHeight < tRightSize.m_iHeight)
			tLeftSize.m_iHeight = tRightSize.m_iHeight;
		if(tRightSize.m_iWidth != 0)
			tLeftSize.m_iWidth += tRightSize.m_iWidth;
	}
	void CParserStarMathString::ComparisonByWidth(TFormulaSize &tLeftSize, const TFormulaSize &tRightSize)
	{
		if(tRightSize.m_iHeight != 0)
			tLeftSize.m_iHeight += tRightSize.m_iHeight;
		if(tLeftSize.m_iWidth < tRightSize.m_iWidth)
			tLeftSize.m_iWidth = tRightSize.m_iWidth;
	}
//class methods CAttribute
	CAttribute::CAttribute(): m_bBold(false),m_bItal(false),m_bPhantom(false),m_bStrike(false),m_bParent(false),m_iSize(0),m_iAlignment(0),m_unCount(0)
	{
	}
	CAttribute::~CAttribute()
	{}
	void CAttribute::SetSize(const unsigned int &iSize)
	{
		m_iSize = iSize;
	}
	void CAttribute::SetAlignment(const unsigned int &iAlignment)
	{
		m_iAlignment = iAlignment;
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
	bool CAttribute::SetColor(const TypeElement &enColor)
	{
		if(enColor != TypeElement::undefine)
		{
			switch (enColor) {
			case TypeElement::black:
			m_wsColor = L"000000";
			return true;
			case TypeElement::blue:
			m_wsColor = L"0000ff";
			return true;
			case TypeElement::green:
			m_wsColor =L"00FF00";
			return true;
			case TypeElement::red:
			m_wsColor =L"FF0000";
			return true;
			case TypeElement::fuchsia:
			m_wsColor =L"ED0DD9";
			return true;
			case TypeElement::aqua:
			m_wsColor =L"30D5C8";
			return true;
			case TypeElement::yellow:
			m_wsColor =L"FFFF00";
			return true;
			case TypeElement::gray:
			m_wsColor =L"808080";
			return true;
			case TypeElement::lime:
			m_wsColor =L"00FF00";
			return true;
			case TypeElement::maroon:
			m_wsColor =L"800000";
			return true;
			case TypeElement::navy:
			m_wsColor =L"000080";
			return true;
			case TypeElement::olive:
			m_wsColor =L"808000";
			return true;
			case TypeElement::purple:
			m_wsColor =L"800080";
			return true;
			case TypeElement::silver:
			m_wsColor =L"C0C0C0";
			return true;
			case TypeElement::teal:
			m_wsColor =L"008080";
			return true;
			case TypeElement::coral:
			m_wsColor =L"FF7F50";
			return true;
			case TypeElement::midnightblue:
			m_wsColor =L"191970";
			return true;
			case TypeElement::crimson:
			m_wsColor =L"DC143C";
			return true;
			case TypeElement::violet:
			m_wsColor =L"EE82EE";
			return true;
			case TypeElement::orange:
			m_wsColor =L"FFA500";
			return true;
			case TypeElement::orangered:
			m_wsColor =L"FF4500";
			return true;
			case TypeElement::seagreen:
			m_wsColor =L"2E8B57";
			return true;
			case TypeElement::indigo:
			m_wsColor =L"4B0082";
			return true;
			case TypeElement::hotpink:
			m_wsColor =L"FF69B4";
			return true;
			case TypeElement::lavender:
			m_wsColor =L"FFF0F5";
			return true;
			default:
			return false;
			}
		}
		else return false;
	}
	void CAttribute::SetColor(const std::wstring &wsColor)
	{
		m_wsColor = wsColor;
	}
	bool CAttribute::SetFont(const TypeElement &enFont)
	{
		switch (enFont) {
		case TypeElement::ital:
		m_bItal = true;
		return true;
		case TypeElement::bold:
		m_bBold = true;
		return true;
		case TypeElement::phantom:
		m_bPhantom = true;
		return true;
		case TypeElement::overstrike:
		m_bStrike = true;
		return true;
		default:
		return false;
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
	unsigned int CAttribute::GetAlignment()
	{
		return m_iAlignment;
	}
	bool CAttribute::EmptyColor()
	{
		return m_wsColor.empty();
	}
	void CAttribute::SetParent()
	{
		m_bParent = true;
	}
	bool CAttribute::GetParent()
	{
		return m_bParent;
	}
	//hex current
	bool CAttribute::ParseColorAttribute(const std::wstring &wsToken,CStarMathReader* pReader)
	{
		TypeElement enTempColor = GetTypeColorAttribute(wsToken);
		switch(enTempColor)
		{
		case TypeElement::hex:
		{
			pReader->SetString(L"");
			std::wstring wsTempToken;
			wsTempToken = pReader->TakingElementForHex();
			if(wsTempToken.empty())
				return false;
			if(!m_wsColor.empty())
				m_wsColor.clear();
			if(wsTempToken.size()< 6)
			{
				int iZero = 6 - wsTempToken.size();
				for(int i = 0;i<iZero;i++)
					m_wsColor.push_back(L'0');
				m_wsColor += wsTempToken;
				return true;
			}
			else if(wsTempToken.size() == 6)
			{
				m_wsColor = wsTempToken;
				return true;
			}
			else if(wsTempToken.size() > 6)
			{
				m_wsColor += wsTempToken.substr(wsTempToken.size()-6);
				return true;
			}
		}
		case TypeElement::rgb:
		{
		pReader->SetString(L"");
		const int iTempLen = 7;
		wchar_t arTemp[iTempLen];
		int  iRed(-1),iGreen(-1),iBlue(-1);
		iRed = pReader->TakingElementForRGB();
		if(iRed == -1)
			return false;
		iGreen = pReader->TakingElementForRGB();
		if(iGreen == -1)
		{
			RefundOfTheAmountRGB(pReader,iRed,iGreen,iBlue);
			return false;
		}
		iBlue = pReader->TakingElementForRGB();
		if(iBlue == -1)
		{
			RefundOfTheAmountRGB(pReader,iRed,iGreen,iBlue);
			return false;
		}
		if(iRed > 255 || iGreen > 255 || iBlue > 255)
			m_wsColor = L"000000";
		else
		{
			swprintf(arTemp,iTempLen,L"%02X%02X%02X",iRed,iGreen,iBlue);
			m_wsColor = std::wstring(arTemp,6);
		}
		return true;
		}
		default:
		return(SetColor(enTempColor));
		}
	}
	bool CAttribute::ParseFontAttribute(const TypeElement& enTypeFont, CStarMathReader *pReader)
	{
		switch(enTypeFont)
		{
		case TypeElement::size:
		{
			std::wstring wsSize = pReader->GetElement();
			pReader->TokenProcessing(wsSize);
			int iTemp;
			if(CElementString::GetDigit(wsSize) != TypeElement::undefine)
				iTemp = std::stoi(wsSize);
			else
			{
				return false;
			}
			if (iTemp > 0 && iTemp < 127)
				m_iSize = iTemp*2;
			else
				m_iSize = 24;
			return true;
		}
		case TypeElement::font:
		{
			pReader->TokenProcessing();
			if(pReader->GetLowerCaseString() == L"sans")
				m_wsNameFont = L"Liberation Sans";
			else if(pReader->GetLowerCaseString() == L"serif")
				m_wsNameFont = L"Liberation Serif";
			else if(pReader->GetLowerCaseString() == L"fixed")
				m_wsNameFont = L"Liberation Mono";
			else
				return false;
			return true;
		}
		case TypeElement::alignl:
		case TypeElement::alignr:
		case TypeElement::alignc:
		return true;
		default:
		return SetFont(enTypeFont);
		}
	}

//нет phantom, rgb, 16 , гарнитуры и кегля
	TypeElement CAttribute::GetTypeColorAttribute(const std::wstring &wsToken)
	{
		if(L"hex"==wsToken) return TypeElement::hex;
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
		else if(L"alignl" == wsToken) return TypeElement::alignl;
		else if(L"alignr" == wsToken) return TypeElement::alignr;
		else if(L"alignc" == wsToken) return TypeElement::alignc;
		else return TypeElement::undefine;
	}
	bool CAttribute::CheckHexPosition(const wchar_t &cToken)
	{
		if(isdigit(cToken))
			return true;
		if( L'A' == cToken) return true;
		else if(L'B' == cToken) return true;
		else if(L'C' == cToken) return true;
		else if(L'D' == cToken) return true;
		else if(L'E' == cToken) return true;
		else if(L'F' == cToken) return true;
		else
			return false;
	}
	bool CAttribute::CheckAttribute()
	{
		if(m_bBold == true || m_bItal == true || m_bStrike == true || m_bPhantom == true)
			return true;
		else if(!m_wsColor.empty() || !m_wsNameFont.empty())
			return true;
		else if(m_iSize != 0 || (m_iAlignment == 1 || m_iAlignment == 2))
			return true;
		return false;
	}
	void CAttribute::RefundOfTheAmountRGB(CStarMathReader *pReader, const int &iRed, const int &iGreen, const int &iBlue)
	{
		std::wstring wsSum;
		if(iRed != -1)
			wsSum += std::to_wstring(iRed);
		if(iGreen != -1)
			wsSum += std::to_wstring(iGreen);
		if(iBlue != -1)
			wsSum += std::to_wstring(iBlue);
		pReader->TokenProcessing(wsSum);
	}
	bool CAttribute::CheckingForEmptiness()
	{
		if(!m_wsNameFont.empty())
			return true;
		if(!m_wsColor.empty())
			return true;
		if(m_bBold)
			return true;
		if(m_bItal)
			return true;
		if(m_iAlignment > 0 && m_iAlignment<=2)
			return true;
		if(m_iSize > 0)
			return true;
		return false;
	}
	void CAttribute::AddRef()
	{
		m_unCount++;
	}
	void CAttribute::Release()
	{
		m_unCount--;
		if(m_unCount == 0)
			delete this;
	}
	unsigned int CAttribute::GetCount()
	{
		return m_unCount;
	}
	void CAttribute::ComparingAttributes(CAttribute *pAttributeParent, CAttribute *pAttributeChild)
	{
		if(!pAttributeChild->GetBold() && pAttributeParent->GetBold())
			pAttributeChild->SetBold();
		if(!pAttributeChild->GetItal() && pAttributeParent->GetItal())
			pAttributeChild->SetItal();
		if(!pAttributeChild->GetPhantom() && pAttributeParent->GetPhantom())
			pAttributeChild->SetPhantom();
		if(!pAttributeChild->GetStrike() && pAttributeParent->GetStrike())
			pAttributeChild->SetStrike();
		if(pAttributeChild->EmptyColor() && !pAttributeParent->EmptyColor())
			pAttributeChild->SetColor(pAttributeParent->GetColor());
		if(pAttributeChild->GetSize() == 0 && pAttributeParent->GetSize() != 0)
			pAttributeChild->SetSize(pAttributeParent->GetSize());
	}
//class methods CElement
	CElement::~CElement()
	{
		if(m_pAttribute != nullptr)
			m_pAttribute->Release();
	}
	CElement::CElement(): m_pAttribute(nullptr)
	{}
	CElement::CElement(const TypeElement &enTypeBase,const TypeConversion &enTypeConversion): m_pAttribute(nullptr),m_enBaseType(enTypeBase),m_enTypeConversion(enTypeConversion)
	{
	}
	CElement* CElement::CreateElement(CStarMathReader* pReader)
	{
		switch (pReader->GetGlobalType()) {
			case TypeElement::String:
				return new CElementString(pReader->GetOriginalString(),pReader->GetTypeConversion());
			case TypeElement::BinOperator:
				return new CElementBinOperator(pReader->GetLocalType(),pReader->GetTypeConversion());
			case TypeElement::SetOperations:
				return new CElementSetOperations(pReader->GetLocalType(),pReader->GetTypeConversion());
			case TypeElement::Connection:
				return new CElementConnection(pReader->GetLocalType(),pReader->GetTypeConversion());
			case TypeElement::Function:
				{
				if(pReader->GetLocalType() == TypeElement::func)
				{
					if (pReader->GetToken())
						return new CElementFunction(pReader->GetLocalType(),pReader->GetTypeConversion(),pReader->GetLowerCaseString());
					else
						return nullptr;
				}
				else
					return new CElementFunction(pReader->GetLocalType(),pReader->GetTypeConversion());
				}
			case TypeElement::Bracket:
				{
				if(pReader->GetLocalType() == TypeElement::left)
					return new CElementBracket(pReader->GetLocalType(),pReader->GetTypeConversion(),true);
				else
					return new CElementBracket(pReader->GetLocalType(),pReader->GetTypeConversion());
				}
			case TypeElement::Operation:
				{
				if(pReader->GetLocalType() == TypeElement::oper)
				{
					if (pReader->GetToken())
					{
						return new CElementOperator(pReader->GetLocalType(),pReader->GetTypeConversion(),pReader->GetLowerCaseString());
					}
					else
						return nullptr;
				}
				else
					return new CElementOperator(pReader->GetLocalType(),pReader->GetTypeConversion());
				}
			case TypeElement::BracketWithIndex:
				return new CElementBracketWithIndex(pReader->GetLocalType(),pReader->GetTypeConversion());
			case TypeElement::Grade:
				return new CElementGrade(pReader->GetTypeConversion());
			case TypeElement::Matrix:
				return new CElementMatrix(pReader->GetLocalType(),pReader->GetTypeConversion());
			case TypeElement::SpecialSymbol:
				return new CElementSpecialSymbol(pReader->GetLocalType(),pReader->GetTypeConversion());
			case TypeElement::Index:
				return new CElementIndex(pReader->GetLocalType(),pReader->GetTypeConversion());
			case TypeElement::Mark:
				return new CElementDiacriticalMark(pReader->GetLocalType(),pReader->GetTypeConversion());
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
		{
			m_pAttribute = pAttribute;
			m_pAttribute->AddRef();
		}
		else if(pAttribute != nullptr && m_pAttribute!=nullptr && pAttribute != m_pAttribute)
		{
			if(m_pAttribute->GetCount() <= 1 && !m_pAttribute->GetParent())
				CAttribute::ComparingAttributes(pAttribute,m_pAttribute);
			else if(m_pAttribute->GetCount() > 1 || m_pAttribute->GetParent())
			{
				CAttribute* pTempAttribute = m_pAttribute;
				m_pAttribute = new CAttribute;
				m_pAttribute->AddRef();
				CAttribute::ComparingAttributes(pTempAttribute,m_pAttribute);
				CAttribute::ComparingAttributes(pAttribute,m_pAttribute);
			}
		}
	}
	CAttribute* CElement::GetAttribute()
	{
		return m_pAttribute;
	}
	const TypeConversion& CElement::GetTypeConversion()
	{
		return m_enTypeConversion;
	}
	void CElement::DeleteAttribute()
	{
		m_pAttribute->Release();
		m_pAttribute = nullptr;
	}
//class methods CElementString
	CElementString::CElementString(const std::wstring& wsTokenString,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::String,enTypeConversion),m_wsString(wsTokenString)
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
		if(m_wsString == L"\"")
		{
			m_wsString.clear();
			wchar_t wsOneToken = pReader->GetOneElement();
			while(wsOneToken !=L'"')
			{
				m_wsString+=wsOneToken;
				if(pReader->CheckIteratorPosition())
					wsOneToken = pReader->GetOneElement();
				else 
					break;
			}
		}
		pReader->ClearReader();
	}
	void CElementString::ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite)
	{
		if(m_wsString !=L"" && m_wsString!=L" ")
		{
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
		pXmlWrite->WriteNodeBegin(L"m:t",false);
        // if(m_wsString == L"&")
        //     pXmlWrite->WriteString(L"&amp;");
        // else
        //     pXmlWrite->WriteString(m_wsString);
        pXmlWrite->WriteString(XmlUtils::EncodeXmlString(m_wsString));
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
		}
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
			if(!iswdigit(cOneElement) ||  L'\0' == cOneElement) return TypeElement::undefine;
		}
		return TypeElement::String;
	}
	TypeElement CElementString::GetWord(const std::wstring &wsToken)
	{
		if(wsToken.empty()) return TypeElement::undefine;

		for(wchar_t cOneElement: wsToken)
		{
			if(!iswalpha(cOneElement)) return TypeElement::undefine;
		}
		return TypeElement::String;
	}
	void CElementString::SetAttribute(CAttribute *pAttribute)
	{
		SetBaseAttribute(pAttribute);
	}
	TFormulaSize CElementString::GetSize()
	{
		TFormulaSize tSize;
		if(!m_wsString.empty())
		{
			tSize.m_iHeight = 1;
			tSize.m_iWidth = m_wsString.size();
		}
		return tSize;
	}
//class methods CElementBinOperator
	CElementBinOperator::CElementBinOperator(const TypeElement& enType,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::BinOperator,enTypeConversion),m_pLeftArgument(nullptr) , m_pRightArgument(nullptr),m_enTypeBinOp(enType)
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
			m_pLeftArgument = CParserStarMathString::ReadingWithoutBracket(pReader,false);
			m_pRightArgument = CParserStarMathString::ReadingWithoutBracket(pReader,false);
			if(GetAttribute() != nullptr)
				SetAttribute(GetAttribute());
		}
		else if(m_enTypeBinOp == TypeElement::neg)
		{
			m_pRightArgument = CParserStarMathString::ParseElement(pReader);
			if(GetAttribute()!= nullptr)
				SetAttribute(GetAttribute());
		}
		else if((pReader->GetMarkForUnar() || GetAttribute()!=nullptr)&& MixedOperators(m_enTypeBinOp))
		{
			m_pRightArgument = CParserStarMathString::ParseElement(pReader);
		}
		else
		{
			pReader->SetMarkForUnar(true);
			CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
			while((IsBinOperatorLowPrior() && (pReader->GetGlobalType() == TypeElement::BinOperator || pReader->GetLocalType() == TypeElement::intersection || pReader->GetLocalType() == TypeElement::setminus || pReader->GetLocalType() == TypeElement::setquotient)) || pReader->GetGlobalType() == TypeElement::BracketWithIndex || (pReader->GetGlobalType() == TypeElement::Index && (pReader->GetLocalType() != TypeElement::nroot  && pReader->GetLocalType() != TypeElement::sqrt)))
			{
				CParserStarMathString::ReadingElementsWithPriorities(pReader,pTempElement);
			}
			m_pRightArgument = pTempElement;
		}
	}
	void CElementBinOperator::ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite)
	{
		if(m_enTypeBinOp == TypeElement::over || m_enTypeBinOp ==TypeElement::division || TypeElement::frac == m_enTypeBinOp || TypeElement::wideslash == m_enTypeBinOp)
		{
			pXmlWrite->WriteNodeBegin(L"m:f",false);
			if(m_enTypeBinOp == TypeElement::division)
				CConversionSMtoOOXML::PropertiesMFPR(L"lin",pXmlWrite,GetAttribute(),GetTypeConversion());
			else if(m_enTypeBinOp == TypeElement::wideslash)
				CConversionSMtoOOXML::PropertiesMFPR(L"skw",pXmlWrite,GetAttribute(),GetTypeConversion());
			else
				CConversionSMtoOOXML::PropertiesMFPR(L"",pXmlWrite,GetAttribute(),GetTypeConversion());
			CConversionSMtoOOXML::WriteNodeConversion(L"m:num",m_pLeftArgument,pXmlWrite);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:den",m_pRightArgument,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:f",false,false);
		}
		else
		{
			CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArgument);
			pXmlWrite->WriteNodeBegin(L"m:r",false);
			CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
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
			case TypeElement::neg:
				pXmlWrite->WriteString(L"\u00AC");
				break;
			case TypeElement::circ:
				pXmlWrite->WriteString(L"\u2218");
				break;
			case TypeElement::widebslash:
				pXmlWrite->WriteString(L"\u2216");
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
		else if(L"&" == wsToken) return TypeElement::And;
		else return TypeElement::undefine;
	}
	 bool CElementBinOperator::MixedOperators(const TypeElement &enType)
	{
		switch (enType)
		{
		case TypeElement::plus:
		return true;
		case TypeElement::plus_minus:
		return true;
		case TypeElement::minus_plus:
		return true;
		case TypeElement::minus:
		return true;
		default:
		return false;
		}
	}
	void CElementBinOperator::UnaryCheck(CStarMathReader *pReader, CElement *pLastElement)
	{
		pReader->ReadingTheNextToken();
		if(!CParserStarMathString::CheckNewline(pLastElement) && MixedOperators(pReader->GetLocalType()))
			if(pReader->GetAttribute() != nullptr)
				pReader->SetMarkForUnar(true);
			else pReader->SetMarkForUnar(false);
		else pReader->SetMarkForUnar(true);
	}
	bool CElementBinOperator::IsBinOperatorLowPrior()
	{
		switch (m_enTypeBinOp) {
		case TypeElement::oplus:
		return true;
		case TypeElement::ominus:
		return true;
		case TypeElement::circ:
		return true;
		case TypeElement::Or:
		return true;
		default:
		return MixedOperators(m_enTypeBinOp);
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
	const TypeElement& CElementBinOperator::GetType()
	{
		return m_enTypeBinOp;
	}
	TFormulaSize CElementBinOperator::GetSize()
	{
		TFormulaSize tSize;
		if(m_pLeftArgument != nullptr)
			tSize = m_pLeftArgument->GetSize();
		switch(m_enTypeBinOp)
		{
		case TypeElement::over:
		case TypeElement::wideslash:
		case TypeElement::frac:
		{
			if(m_pRightArgument!= nullptr)
				CParserStarMathString::ComparisonByWidth(tSize,m_pRightArgument->GetSize());
			tSize.m_iHeight += 1;
			return tSize;
		}
		default:
			if(m_pRightArgument != nullptr)
				CParserStarMathString::ComparisonByHeight(tSize,m_pRightArgument->GetSize());
			tSize.m_iWidth+= 1;
			return tSize;
		}
	}
//class methods CElementBracket
	CElementBracket::CElementBracket(const TypeElement& enType,const TypeConversion &enTypeConversion,const bool& bScalability)
		:CElement(TypeElement::Bracket,enTypeConversion),m_enTypeBracket(enType),m_bScalability(bScalability)
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
		else if(L"left" == wsToken) return TypeElement::left;
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
		else if(L"none" == wsToken) return TypeElement::none;
		else return TypeElement::undefine;
	}
	std::vector<CElement*> CElementBracket::GetBracketValue()
	{
		return m_arBrecketValue;
	}
	void CElementBracket::Parse(CStarMathReader* pReader)
	{
		TypeElement enOpen,enClose;
		if(m_enTypeBracket == TypeElement::left)
		{	
			pReader->ReadingTheNextToken();
			enOpen = GetBracketOpen(pReader->GetLowerCaseString());
            if(pReader->GetLowerCaseString() == L"none")
                enOpen = TypeElement::none;
            else
                enClose = GetBracketClose(pReader->GetLowerCaseString());
			if(enOpen != TypeElement::undefine)
			{
				m_enLeftBracket = enOpen;
				pReader->ClearReader();
			}
			else if(enClose != TypeElement::undefine)
			{
				m_enLeftBracket = enClose;
				pReader->ClearReader();
			}
		}
		pReader->FindingTheEndOfParentheses();
		while(pReader->CheckIteratorPosition())
		{
			pReader->ReadingTheNextToken();
			if(pReader->GetLowerCaseString() == L"right")
			{
				pReader->ClearReader();
				continue;
			}
			CParserStarMathString::ParsElementAddingToArray(pReader,m_arBrecketValue);
		}
		if(!pReader->EmptyString() && pReader->GetLowerCaseString() != L"right")
		{
			if(pReader->GetLocalType() == TypeElement::newline)
			{
				m_arBrecketValue.push_back(new CElementSpecialSymbol(pReader->GetLocalType(),pReader->GetTypeConversion()));
				pReader->ClearReader();
			}
			else
			{
				CElement* pTempElement =CParserStarMathString::ParseElement(pReader);
				if(nullptr != pTempElement)
						m_arBrecketValue.push_back(pTempElement);
			}
		}
		pReader->SettingTheIteratorToTheClosingBracket();
		pReader->ClearReader();
		pReader->ReadingTheNextToken();
		enOpen = GetBracketOpen(pReader->GetLowerCaseString());
		enClose = GetBracketClose(pReader->GetLowerCaseString());
		if(enOpen != TypeElement::undefine)
		{
			m_enRightBracket = enOpen;
			pReader->ClearReader();
		}
		else if(enClose != TypeElement::undefine)
		{
			m_enRightBracket = enClose;
			pReader->ClearReader();
		}
		pReader->IteratorNullification();
	}
	void CElementBracket::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		std::wstring wsOpenBracket,wsCloseBracket;
		if(m_bScalability)
		{
			wsOpenBracket = DefiningBracket(m_enLeftBracket);
			wsCloseBracket = DefiningBracket(m_enRightBracket);
		}
		else
		{
			wsOpenBracket = DefiningBracket(m_enTypeBracket);
			wsCloseBracket = DefiningBracket(m_enRightBracket);
		}
		if(m_enTypeBracket != TypeElement::brace)
		{
			pXmlWrite->WriteNodeBegin(L"m:d",false);
			CConversionSMtoOOXML::PropertiesDPr(pXmlWrite,wsOpenBracket,wsCloseBracket,GetAttribute(),GetTypeConversion(),m_enTypeBracket);
			pXmlWrite->WriteNodeBegin(L"m:e",false);
			for(CElement* pTemp:m_arBrecketValue)
			{
				if(CParserStarMathString::CheckNewline(pTemp))
					continue;
				if(CheckMline(pTemp))
				{
					pXmlWrite->WriteNodeEnd(L"m:e",false,false);
					pXmlWrite->WriteNodeBegin(L"m:e",false);
				}
				CConversionSMtoOOXML::ElementConversion(pXmlWrite,pTemp);
			}
			pXmlWrite->WriteNodeEnd(L"m:e",false,false);
			pXmlWrite->WriteNodeEnd(L"m:d",false,false);
		}
		else
		{
			for(CElement* pTemp:m_arBrecketValue)
			{
				if(CParserStarMathString::CheckNewline(pTemp))
					continue;
				CConversionSMtoOOXML::ElementConversion(pXmlWrite,pTemp);
			}
		}

	}
	bool CElementBracket::CheckMline(CElement *pElement)
	{
		if(pElement == nullptr)
			return false;
		if(pElement->GetBaseType() == TypeElement::SpecialSymbol)
		{
			CElementSpecialSymbol* pSpecial = dynamic_cast<CElementSpecialSymbol*>(pElement);
			return(pSpecial->GetType() == TypeElement::mline);
		}
		else
			return false;
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
	std::wstring CElementBracket::DefiningBracket(const TypeElement &enTypeBracket)
	{
		switch(enTypeBracket)
		{
			case TypeElement::langle:
				return L"\u27E8";
			case TypeElement::rangle:
				return L"\u27E9";
			case TypeElement::square:
				return L"\u005B";
			case TypeElement::rsquare:
				return L"\u005D";
			case TypeElement::ldbracket:
				return L"\u27E6";
			case TypeElement::rdbracket:
				return L"\u27E7";
			case TypeElement::lbrace:
				return L"\u007B";
			case TypeElement::rbrace:
				return L"\u007D";
			case TypeElement::lceil:
				return L"\u2308";
			case TypeElement::rceil:
				return L"\u2309";
			case TypeElement::lfloor:
				return L"\u230A";
			case TypeElement::rfloor:
				return L"\u230B";
			case TypeElement::lline:
			case TypeElement::rline:
				return L"\u007C";
			case TypeElement::ldline:
			case TypeElement::rdline:
				return L"\u2016";
			case TypeElement::none:
				return L"none";
			default:
				return L"";
		}
	}
	TFormulaSize CElementBracket::GetSize()
	{
		TFormulaSize tSize;
		std::vector<TFormulaSize> arSizeLine;
		std::vector<TFormulaSize> arSizeColumn;
		for(CElement* pElement:m_arBrecketValue)
		{
			if(pElement != nullptr)
			{
				if(pElement->GetBaseType() == TypeElement::SpecialSymbol)
				{
					CElementSpecialSymbol* pSpecial = dynamic_cast<CElementSpecialSymbol*>(pElement);
					if(pSpecial->GetType() == TypeElement::grid)
					{
						arSizeLine.push_back(tSize);
						tSize.Zeroing();
					}
					else if(pSpecial->GetType() == TypeElement::transition)
					{
						arSizeLine.push_back(tSize);
						tSize.Zeroing();
						for(TFormulaSize tTempSize:arSizeLine)
							CParserStarMathString::ComparisonByHeight(tSize,tTempSize);
						tSize.m_iWidth += 0.5;
						arSizeColumn.push_back(tSize);
						arSizeLine.clear();
						tSize.Zeroing();
					}
					else
						CParserStarMathString::ComparisonByHeight(tSize,pElement->GetSize());
				}
				else
					CParserStarMathString::ComparisonByHeight(tSize,pElement->GetSize());
			}
		}
		if(tSize.m_iHeight != 0 && tSize.m_iWidth != 0 )
		{
			if(!arSizeLine.empty())
			{
				arSizeLine.push_back(tSize);
				tSize.Zeroing();
				if(!arSizeColumn.empty())
				{
					for(int i = 0 ; i < arSizeLine.size();i++)
					{
						CParserStarMathString::ComparisonByHeight(tSize,arSizeLine[i]);
						if(i != 0)
							tSize.m_iWidth += 1;
					}
					arSizeLine.clear();
					arSizeColumn.push_back(tSize);
					tSize.Zeroing();
					for(int i = 0;i < arSizeColumn.size();i++)
					{
						CParserStarMathString::ComparisonByWidth(tSize,arSizeColumn[i]);
						if(i != 0)
							tSize.m_iHeight += 0.5;
					}
					arSizeColumn.clear();
				}
				else
				{
					for(int i = 0;i < arSizeLine.size();i++)
					{
						CParserStarMathString::ComparisonByWidth(tSize,arSizeLine[i]);
						if(i != 0)
							tSize.m_iHeight += 0.5;
					}
					arSizeLine.clear();
				}
			}
			else if(!arSizeColumn.empty())
			{
				arSizeColumn.push_back(tSize);
				tSize.Zeroing();
				for(int i = 0; i < arSizeColumn.size();i++)
				{
					CParserStarMathString::ComparisonByWidth(tSize,arSizeColumn[i]);
					if(i != 0)
						tSize.m_iHeight += 0.5;
				}
				arSizeColumn.clear();
			}
		}
		if(m_enTypeBracket != TypeElement::brace)
			tSize.m_iWidth += 1;
		return tSize;
	}
//class methods CElementSpecialSymbol
	CElementSpecialSymbol::CElementSpecialSymbol(const TypeElement &enType,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::SpecialSymbol,enTypeConversion),m_pValue(nullptr),m_enTypeSpecial(enType),m_wsType(L"")
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
		case TypeElement::abs:
		{
			CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
			while(pReader->GetGlobalType() == TypeElement::Index && (pReader->GetLocalType()!=TypeElement::nroot && pReader->GetLocalType()!=TypeElement::sqrt))
			{
				CParserStarMathString::ReadingElementsWithPriorities(pReader,pTempElement);
			}
			m_pValue = pTempElement;
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
			else if(L"<?>" == wsToken) return TypeElement::emptySquare;
			else if(L"<?" == wsToken) return TypeElement::emptySquare;
			else if(L"?>" == wsToken) return TypeElement::emptySquare;
			else if(L"mline" == wsToken) return TypeElement::mline;
			else if(L"##" == wsToken) return TypeElement::transition;
			else if(L"emptyset" == wsToken) return TypeElement::emptyset;
			else if(L"aleph" == wsToken) return TypeElement::aleph;
			else if(L"setn" == wsToken) return TypeElement::setN;
			else if(L"setz" == wsToken) return TypeElement::setZ;
			else if(L"setq" == wsToken) return TypeElement::setQ;
			else if(L"setr" == wsToken) return TypeElement::setR;
			else if(L"setc" == wsToken) return TypeElement::setC;
			else if(L"infinity" == wsToken) return TypeElement::infinity;
			else if(L"infty" == wsToken) return TypeElement::infinity;
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
			else if(L"re" == wsToken) return TypeElement::Re;
			else if(L"im" == wsToken) return TypeElement::Im;
			else if(L"wp" == wsToken) return TypeElement::wp;
			else if(L"laplace" == wsToken) return TypeElement::laplace;
			else if(L"fourier" == wsToken) return TypeElement::fourier;
			else if(L"backepsilon" == wsToken) return TypeElement::backepsilon;
			else if(L"leftarrow" == wsToken || L"<-" == wsToken) return TypeElement::leftarrow;
			else if(L"rightarrow" == wsToken || L"->" == wsToken) return TypeElement::rightarrow;
			else if(L"uparrow" == wsToken) return TypeElement::uparrow;
			else if(L"downarrow" == wsToken) return TypeElement::downarrow;
			else if(L"dotslow" == wsToken) return TypeElement::dotslow;
			else if(L"dotsaxis" == wsToken) return TypeElement::dotsaxis;
			else if(L"dotsvert" == wsToken) return TypeElement::dotsvert;
			else if(L"dotsup" == wsToken) return TypeElement::dotsup;
			else if(L"dotsdown" == wsToken) return TypeElement::dotsdown;
			else if(L"newline" == wsToken) return TypeElement::newline;
			else if(L"\\" == wsToken) return TypeElement::slash; 
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
			CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
			pXmlWrite->WriteNodeBegin(L"m:t",false);
			pXmlWrite->WriteString(L"\u0021");
			pXmlWrite->WriteNodeEnd(L"m:t",false,false);
			pXmlWrite->WriteNodeEnd(L"m:r",false,false);
			break;
		}
		case TypeElement::interval:
		{
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute(),GetTypeConversion());
			break;
		}
		case TypeElement::emptiness:
		{
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute(),GetTypeConversion());
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute(),GetTypeConversion());
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute(),GetTypeConversion());
			CConversionSMtoOOXML::WritePreserveBlock(pXmlWrite,GetAttribute(),GetTypeConversion());
			break;
		}
		case TypeElement::newline:
		case TypeElement::slash:
		{
			break;
		}
		case TypeElement::abs:
		{
			pXmlWrite->WriteNodeBegin(L"m:d",false);
			CConversionSMtoOOXML::PropertiesDPr(pXmlWrite,L"\u007C",L"\u007C",GetAttribute(),GetTypeConversion(),TypeElement::abs);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pValue,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:d",false,false);
			break;
		}
		default:
		{
			if(!m_wsType.empty())
			{
				pXmlWrite->WriteNodeBegin(L"m:r",false);
				CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
				pXmlWrite->WriteNodeBegin(L"m:t",false);
                pXmlWrite->WriteString(XmlUtils::EncodeXmlString(m_wsType));
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
		m_wsType = L"\u0127";
		break;
		case TypeElement::lambdabar:
		m_wsType = L"\u019B";
		break;
		case TypeElement::Re:
		m_wsType = L"\u211C";
		break;
		case TypeElement::Im:
		m_wsType = L"\u2111";
		break;
		case TypeElement::wp:
		m_wsType = L"\u2118";
		break;
		case TypeElement::laplace:
		m_wsType = L"\u2112";
		break;
		case TypeElement::fourier:
		m_wsType = L"\u2131";
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
		m_wsType = L"\u226B";
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
		m_wsType = L"\u2794";
		break;
		case TypeElement::leftarrow:
		m_wsType = L"\u2190";
		break;
		case TypeElement::uparrow:
		m_wsType = L"\u2191";
		break;
		case TypeElement::downarrow:
		m_wsType = L"\u2193";
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
		case TypeElement::emptyset:
		m_wsType = L"\u2205";
		break;
		case TypeElement::aleph:
		m_wsType = L"\u2135";
		break;
		case TypeElement::setN:
		m_wsType = L"\u2115";
		break;
		case TypeElement::setZ:
		m_wsType = L"\u2124";
		break;
		case TypeElement::setQ:
		m_wsType = L"\u211A";
		break;
		case TypeElement::setR:
		m_wsType = L"\u211D";
		break;
		case TypeElement::setC:
		m_wsType = L"\u2102";
		break;
		case TypeElement::emptySquare:
		m_wsType = L"\u2751";
		break;
		default:
		break;
		}
	}
	TFormulaSize CElementSpecialSymbol::GetSize()
	{
		TFormulaSize tSize;
		if(TypeElement::fact == m_enTypeSpecial || TypeElement::abs == m_enTypeSpecial)
		{
			if(m_pValue!= nullptr)
			{
				tSize = m_pValue->GetSize();
				tSize.m_iWidth += 1;
			}
			else
			{
				tSize.m_iHeight = 1;
				tSize.m_iWidth = 1;
			}
		}
		else if(TypeElement::emptiness == m_enTypeSpecial)
		{
			tSize.m_iHeight = 1;
			tSize.m_iWidth = 4;
		}
		else if(TypeElement::newline != m_enTypeSpecial && TypeElement::grid != m_enTypeSpecial && TypeElement::mline != m_enTypeSpecial)
		{
			tSize.m_iHeight = 1;
			tSize.m_iWidth = 1;
		}
		return tSize;
	}
//class methods CElementSetOperations
	CElementSetOperations::CElementSetOperations(const TypeElement &enType,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::SetOperations,enTypeConversion),m_pLeftArgument(nullptr), m_pRightArgument(nullptr),m_enTypeSet(enType)
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
		while(TypeElement::intersection != m_enTypeSet && TypeElement::setminus != m_enTypeSet && TypeElement::setquotient != m_enTypeSet && ((pReader->GetGlobalType() == TypeElement::BinOperator && pReader->GetLocalType()!=TypeElement::frac && pReader->GetLocalType()!= TypeElement::neg) || pReader->GetGlobalType() == TypeElement::BracketWithIndex || (pReader->GetGlobalType() == TypeElement::Index && pReader->GetLocalType()!=TypeElement::nroot && pReader->GetLocalType() != TypeElement::sqrt)))
		{
			CParserStarMathString::ReadingElementsWithPriorities(pReader,pTempElement);
		}
		SetRightArg(pTempElement);
	}
	void CElementSetOperations::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArgument);
		pXmlWrite->WriteNodeBegin(L"m:r", false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
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
	const TypeElement& CElementSetOperations::GetType()
	{
		return m_enTypeSet;
	}
	TFormulaSize CElementSetOperations::GetSize()
	{
		TFormulaSize tSize;
		if(m_pLeftArgument != nullptr)
			tSize = m_pLeftArgument->GetSize();
		if(m_pRightArgument != nullptr)
			CParserStarMathString::ComparisonByHeight(tSize,m_pRightArgument->GetSize());
		tSize.m_iWidth += 1;
		return tSize;
	}
//class methods CElementConnection
	CElementConnection::CElementConnection(const TypeElement& enType,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::Connection,enTypeConversion),m_pLeftArgument(nullptr), m_pRightArgument(nullptr),m_enTypeCon(enType)
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
		CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		while((pReader->GetGlobalType() == TypeElement::BinOperator && pReader->GetLocalType()!=TypeElement::frac) || pReader->GetGlobalType() == TypeElement::BracketWithIndex || (pReader->GetGlobalType() == TypeElement::Index && (pReader->GetLocalType()!=TypeElement::nroot && pReader->GetLocalType()!=TypeElement::sqrt)))
		{

			CParserStarMathString::ReadingElementsWithPriorities(pReader,pTempElement);
		}
		SetRightArg(pTempElement);
	}
	void CElementConnection::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		CConversionSMtoOOXML::ElementConversion(pXmlWrite,m_pLeftArgument);
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch(m_enTypeCon)
		{
			case TypeElement::equals:
				pXmlWrite->WriteString(L"\u003D");
				break;
			case TypeElement::notequals:
				pXmlWrite->WriteString(L"\u2260");
				break;
			case TypeElement::learrow:
				pXmlWrite->WriteString(L"\u0026lt;");
				break;
			case TypeElement::learrowequals:
				pXmlWrite->WriteString(L"\u2264");
				break;
			case TypeElement::leslant:
				pXmlWrite->WriteString(L"\u2A7D");
				break;
			case TypeElement::riarrow:
				pXmlWrite->WriteString(L"\u0026gt;");
				break;
			case TypeElement::riarrowequals:
				pXmlWrite->WriteString(L"\u2265");
				break;
			case TypeElement::geslant:
				pXmlWrite->WriteString(L"\u2A7E");
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
	const TypeElement& CElementConnection::GetType()
	{
		return m_enTypeCon;
	}
	TFormulaSize CElementConnection::GetSize()
	{
		TFormulaSize tSize;
		if(m_pLeftArgument != nullptr)
			tSize = m_pLeftArgument->GetSize();
		if(m_pRightArgument != nullptr)
			CParserStarMathString::ComparisonByHeight(tSize,m_pRightArgument->GetSize());
		tSize.m_iWidth += 1;
		return tSize;
	}
//class methods CIndex
	CElementIndex::CElementIndex(const TypeElement& enType,const TypeConversion &enTypeConversion)
		: CElement(TypeElement::Index,enTypeConversion),m_pValueIndex(nullptr),m_pUpperIndex(nullptr),m_pLowerIndex(nullptr),m_pLsubIndex(nullptr),m_pLsupIndex(nullptr),m_pCsubIndex(nullptr),m_pCsupIndex(nullptr),m_pLeftArg(nullptr),m_enTypeIndex(enType)
	{
	}
	CElementIndex::~CElementIndex()
	{
		delete m_pValueIndex;
		delete m_pLeftArg;
		delete m_pUpperIndex;
		delete m_pLowerIndex;
		delete m_pLsubIndex;
		delete m_pLsupIndex;
		delete m_pCsubIndex;
		delete m_pCsupIndex;
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
		else if(L"rsup" == wsCheckToken) return TypeElement::upper;
		else if(L"rsub" == wsCheckToken) return TypeElement::lower;
		else if(L"_" == wsCheckToken) return TypeElement::lower;
		else if(L"lsup" == wsCheckToken) return TypeElement::lsup;
		else if(L"lsub" == wsCheckToken) return TypeElement::lsub;
		else if(L"csup" == wsCheckToken) return TypeElement::csup;
		else if(L"csub" == wsCheckToken) return TypeElement::csub;
		else if(L"nroot" == wsCheckToken) return TypeElement::nroot;
		else if(L"sqrt" == wsCheckToken) return TypeElement::sqrt;
		else return TypeElement::undefine;
	}
	bool CElementIndex::GetUpperIndex(const TypeElement &enType)
	{
		switch(enType)
		{
		case TypeElement::upper:
		case TypeElement::csup:
		case TypeElement::lsup:
		return true;
		default:
		return false;
		}
	}
	bool CElementIndex::GetLowerIndex(const TypeElement &enType)
	{
		switch(enType)
		{
		case TypeElement::lower:
		case TypeElement::lsub:
		case TypeElement::csub:
		return true;
		default:
		return false;
		}
	}
	void CElementIndex::Parse(CStarMathReader *pReader)
	{
		if(m_enTypeIndex == TypeElement::nroot)
		{
			m_pLeftArg = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
			if(CElementIndex::GetLowerIndex(pReader->GetLocalType()) || CElementIndex::GetUpperIndex(pReader->GetLocalType()))
			{
				CElement* pElement = CParserStarMathString::ParseElement(pReader);
				CParserStarMathString::AddLeftArgument(m_pLeftArg,pElement,pReader);
				m_pLeftArg = pElement;
			}
			m_pValueIndex = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
			if(CElementIndex::GetLowerIndex(pReader->GetLocalType()) || CElementIndex::GetUpperIndex(pReader->GetLocalType()))
			{
				CElement* pElement = CParserStarMathString::ParseElement(pReader);
				CParserStarMathString::AddLeftArgument(m_pValueIndex,pElement,pReader);
				m_pValueIndex = pElement;
			}
		}
		else if(m_enTypeIndex == TypeElement::sqrt)
		{
			m_pValueIndex = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
			if(CElementIndex::GetLowerIndex(pReader->GetLocalType()) || CElementIndex::GetUpperIndex(pReader->GetLocalType()))
			{
				CElement* pElement = CParserStarMathString::ParseElement(pReader);
				CParserStarMathString::AddLeftArgument(m_pValueIndex,pElement,pReader);
				m_pValueIndex = pElement;
			}
		}
		else
		{
			do
			{
				pReader->ClearReader();
				switch(m_enTypeIndex)
				{
				case TypeElement::upper:
				CParserStarMathString::ReadingElementsWithAttributes(pReader,m_pUpperIndex);
				break;
				case TypeElement::lower:
				CParserStarMathString::ReadingElementsWithAttributes(pReader,m_pLowerIndex);
				break;
				case TypeElement::lsub:
				CParserStarMathString::ReadingElementsWithAttributes(pReader,m_pLsubIndex);
				break;
				case TypeElement::lsup:
				CParserStarMathString::ReadingElementsWithAttributes(pReader,m_pLsupIndex);
				break;
				case TypeElement::csub:
				CParserStarMathString::ReadingElementsWithAttributes(pReader,m_pCsubIndex);
				break;
				case TypeElement::csup:
				CParserStarMathString::ReadingElementsWithAttributes(pReader,m_pCsupIndex);
				break;
				}
				pReader->ReadingTheNextToken();
				m_enTypeIndex = pReader->GetLocalType();
			}while(GetUpperIndex(pReader->GetLocalType()) || GetLowerIndex(pReader->GetLocalType()));
		}
	}
	void CElementIndex::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(TypeElement::sqrt == m_enTypeIndex || TypeElement::nroot == m_enTypeIndex)
		{
			pXmlWrite->WriteNodeBegin(L"m:rad",false);
			pXmlWrite->WriteNodeBegin(L"m:radPr",false);
			if(TypeElement::sqrt == m_enTypeIndex)
			{
				pXmlWrite->WriteNodeBegin(L"m:degHide",true);
				pXmlWrite->WriteAttribute(L"m:val",1);
				pXmlWrite->WriteNodeEnd(L"",true,true);
			}
			CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute(),GetTypeConversion());
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
		}
		else if(m_pCsubIndex != nullptr || m_pCsupIndex != nullptr)
		{
			if(m_pCsubIndex != nullptr && m_pCsupIndex != nullptr)
			{
				pXmlWrite->WriteNodeBegin(L"m:limLow",false);
				pXmlWrite->WriteNodeBegin(L"m:limLowPr",false);
				CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,m_pCsubIndex->GetAttribute(),GetTypeConversion());
				pXmlWrite->WriteNodeEnd(L"m:limLowPr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:e",false);
				pXmlWrite->WriteNodeBegin(L"m:limUpp",false);
				pXmlWrite->WriteNodeBegin(L"m:limUppPr",false);
				CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,m_pCsupIndex->GetAttribute(),GetTypeConversion());
				pXmlWrite->WriteNodeEnd(L"m:limUppPr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:e",false);
				ConversionOfIndicesToValue(pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:e",false,false);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:lim",m_pCsupIndex,pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:limUpp",false,false);
				pXmlWrite->WriteNodeEnd(L"m:e",false,false);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:lim",m_pCsubIndex,pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:limLow",false,false);
			}
			else
			{
				std::wstring wsNameLim;
				if(m_pCsubIndex!= nullptr)
					wsNameLim = L"m:limLow";
				else
					wsNameLim = L"m:limUpp";
				pXmlWrite->WriteNodeBegin(wsNameLim,false);
				pXmlWrite->WriteNodeBegin(wsNameLim+L"Pr",false);
				CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute(),GetTypeConversion());
				pXmlWrite->WriteNodeEnd(wsNameLim+L"Pr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:e",false);
				ConversionOfIndicesToValue(pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:e",false,false);
				if(m_pCsubIndex != nullptr)
					CConversionSMtoOOXML::WriteNodeConversion(L"m:lim",m_pCsubIndex,pXmlWrite);
				else
					CConversionSMtoOOXML::WriteNodeConversion(L"m:lim",m_pCsupIndex,pXmlWrite);
				pXmlWrite->WriteNodeEnd(wsNameLim,false,false);
			}
		}
		else
			ConversionOfIndicesToValue(pXmlWrite);
	}
	void CElementIndex::SetAttribute(CAttribute *pAttribute)
	{
		if(m_pLeftArg != nullptr && m_pLeftArg->GetAttribute() == nullptr && pAttribute != nullptr)
			m_pLeftArg->SetAttribute(pAttribute);
		if(m_pLeftArg != nullptr && m_pLeftArg->GetAttribute() != nullptr)
		{
			m_pLeftArg->SetBaseAttribute(pAttribute);
			this->SetBaseAttribute(m_pLeftArg->GetAttribute());
		}
		if(m_pValueIndex != nullptr)
			m_pValueIndex->SetAttribute(pAttribute);
		if(m_pLeftArg != nullptr)
		{
			if(m_pLowerIndex != nullptr)
				m_pLowerIndex->SetAttribute(m_pLeftArg->GetAttribute());
			if(m_pUpperIndex != nullptr)
				m_pUpperIndex->SetAttribute(m_pLeftArg->GetAttribute());
			if(m_pLsubIndex != nullptr)
				m_pLsubIndex->SetAttribute(m_pLeftArg->GetAttribute());
			if(m_pLsupIndex != nullptr)
				m_pLsupIndex->SetAttribute(m_pLeftArg->GetAttribute());
			if(m_pCsubIndex != nullptr)
				m_pCsubIndex->SetAttribute(m_pLeftArg->GetAttribute());
			if(m_pCsupIndex != nullptr)
				m_pCsupIndex->SetAttribute(m_pLeftArg->GetAttribute());
		}
	}
	void CElementIndex::ConversionOfIndicesToValue(XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(m_pLsubIndex != nullptr || m_pLsupIndex != nullptr)
		{
		pXmlWrite->WriteNodeBegin(L"m:sPre",false);
		pXmlWrite->WriteNodeBegin(L"m:sPrePr",false);
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute(),GetTypeConversion());
		pXmlWrite->WriteNodeEnd(L"m:sPrePr",false,false);
		CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pLsubIndex,pXmlWrite);
		CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pLsupIndex,pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:e",false);
		ConversionOfIndicesAfterValue(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		pXmlWrite->WriteNodeEnd(L"m:sPre",false,false);
		}
		else
			ConversionOfIndicesAfterValue(pXmlWrite);
	}
	void CElementIndex::ConversionOfIndicesAfterValue(XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(m_pUpperIndex !=nullptr || m_pLowerIndex != nullptr)
		{
			std::wstring wsNameNodeIndex;
			if(m_pUpperIndex != nullptr && m_pLowerIndex != nullptr)
				wsNameNodeIndex = L"m:sSubSup";
			else if(m_pLowerIndex != nullptr && m_pUpperIndex == nullptr)
				wsNameNodeIndex = L"m:sSub";
			else if(m_pLowerIndex == nullptr && m_pUpperIndex != nullptr)
				wsNameNodeIndex = L"m:sSup";
			pXmlWrite->WriteNodeBegin(wsNameNodeIndex,false);
			pXmlWrite->WriteNodeBegin(wsNameNodeIndex+L"Pr",false);
			CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute(),GetTypeConversion());
			pXmlWrite->WriteNodeEnd(wsNameNodeIndex+L"Pr",false,false);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pLeftArg,pXmlWrite);
			if(m_pUpperIndex!=nullptr && m_pLowerIndex != nullptr)
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pLowerIndex,pXmlWrite);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pUpperIndex,pXmlWrite);
			}
			else if(m_pLowerIndex!= nullptr)
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pLowerIndex,pXmlWrite);
			else if(m_pUpperIndex != nullptr)
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pUpperIndex,pXmlWrite);
			pXmlWrite->WriteNodeEnd(wsNameNodeIndex,false,false);
		}
		else
			m_pLeftArg->ConversionToOOXML(pXmlWrite);
	}
	const TypeElement& CElementIndex::GetType()
	{
		return m_enTypeIndex;
	}
	TFormulaSize CElementIndex::GetSize()
	{
		TFormulaSize tSizeIndex,tLeftArgSize;
		if(m_pLeftArg!= nullptr)
			tLeftArgSize = m_pLeftArg->GetSize();
		if(m_pCsubIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeIndex,m_pCsubIndex->GetSize());
		else if(m_pCsupIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeIndex,m_pCsupIndex->GetSize());
		else if(m_pLowerIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeIndex,m_pLowerIndex->GetSize());
		else if(m_pUpperIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeIndex,m_pUpperIndex->GetSize());
		else if(m_pLsubIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeIndex,m_pLsubIndex->GetSize());
		else if(m_pLsupIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeIndex,m_pLsupIndex->GetSize());
		else if(m_pValueIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeIndex,m_pValueIndex->GetSize());
		tSizeIndex.m_iHeight += tLeftArgSize.m_iHeight;
		tSizeIndex.m_iWidth += tLeftArgSize.m_iWidth;
		return tSizeIndex;
	}
//class methods CElementFunction
	CElementFunction::CElementFunction(const TypeElement &enType, const TypeConversion &enTypeConversion ,const std::wstring &wsNameFunc)
		:CElement(TypeElement::Function,enTypeConversion), m_pValue(nullptr),m_pIndex(nullptr),m_enTypeFunction(enType)
	{
		switch (m_enTypeFunction) {
		case TypeElement::cos:
			m_wsNameFunc = L"cos";
			break;
		case TypeElement::sin:
			m_wsNameFunc = L"sin";
			break;
		case TypeElement::tan:
			m_wsNameFunc = L"tan";
			break;
		case TypeElement::cot:
			m_wsNameFunc = L"cot";
			break;
		case TypeElement::sinh:
			m_wsNameFunc = L"sinh";
			break;
		case TypeElement::cosh:
			m_wsNameFunc = L"cosh";
			break;
		case TypeElement::tanh:
			m_wsNameFunc = L"tanh";
			break;
		case TypeElement::coth:
			m_wsNameFunc = L"coth";
			break;
		case TypeElement::arcsin:
			m_wsNameFunc = L"arcsin";
			break;
		case TypeElement::arccos:
			m_wsNameFunc = L"arccos";
			break;
		case TypeElement::arctan:
			m_wsNameFunc = L"arctan";
			break;
		case TypeElement::arccot:
			m_wsNameFunc = L"arccot";
			break;
		case TypeElement::arsinh:
			m_wsNameFunc = L"arsinh";
			break;
		case TypeElement::arcosh:
			m_wsNameFunc = L"arcosh";
			break;
		case TypeElement::artanh:
			m_wsNameFunc = L"artanh";
			break;
		case TypeElement::arcoth:
			m_wsNameFunc = L"arcoth";
			break;
		case TypeElement::log:
			m_wsNameFunc = L"log";
			break;
		case TypeElement::ln:
			m_wsNameFunc = L"ln";
			break;
		case TypeElement::exp:
			m_wsNameFunc = L"exp";
			break;
		case TypeElement::func:
		{
			m_wsNameFunc = wsNameFunc;
			break;
		}
		default:
			break;
		}
	}
	CElementFunction::~CElementFunction()
	{
		delete m_pValue;
		delete m_pIndex;
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
		pReader->ReadingTheNextToken();
		if(CElementIndex::GetUpperIndex(pReader->GetLocalType()) || CElementIndex::GetLowerIndex(pReader->GetLocalType()))
		{
			m_pIndex = CParserStarMathString::ParseElement(pReader);
			CElementString* pString = new CElementString(m_wsNameFunc,pReader->GetTypeConversion());
			if(this->GetAttribute() != nullptr && pReader->GetBaseAttribute()!=nullptr)
			{
				pString->SetAttribute(this->GetAttribute());
				pString->SetAttribute(pReader->GetBaseAttribute());
			}
			else if(this->GetAttribute()!=nullptr)
				pString->SetAttribute(this->GetAttribute());
			else if(pReader->GetBaseAttribute()!=nullptr)
				pString->SetAttribute(pReader->GetBaseAttribute());
				
			CParserStarMathString::AddLeftArgument(pString,m_pIndex,pReader);
			return ;
		}
		CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		while(CParserStarMathString::CheckForLeftArgument(pReader->GetGlobalType()))
		{
			CParserStarMathString::ReadingElementsWithPriorities(pReader,pTempElement);
		}
		SetValueFunction(pTempElement);
	}
	void CElementFunction::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(m_pIndex !=nullptr)
			m_pIndex->ConversionToOOXML(pXmlWrite);
		else
		{
			pXmlWrite->WriteNodeBegin(L"m:func",false);
			CConversionSMtoOOXML::PropertiesFuncPr(pXmlWrite,GetAttribute(),GetTypeConversion());
			pXmlWrite->WriteNodeBegin(L"m:fName",false);
			pXmlWrite->WriteNodeBegin(L"m:r",false);
			pXmlWrite->WriteNodeBegin(L"m:rPr",false);
			pXmlWrite->WriteNodeBegin(L"m:sty",true);
			pXmlWrite->WriteAttribute(L"m:val",L"p");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			pXmlWrite->WriteNodeEnd(L"m:rPr",false,false);
			CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
			pXmlWrite->WriteNodeBegin(L"m:t",false);
			if(!m_wsNameFunc.empty())
				pXmlWrite->WriteString(m_wsNameFunc);
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
	TFormulaSize CElementFunction::GetSize()
	{
		TFormulaSize tSize;
		if(m_pValue != nullptr)
			tSize = m_pValue->GetSize();
		if(m_pIndex != nullptr)
			CParserStarMathString::ComparisonByHeight(tSize,m_pIndex->GetSize());
		if(!m_wsNameFunc.empty())
			tSize.m_iWidth += m_wsNameFunc.size();
		return tSize;
	}
//class methods CElementOperation
	CElementOperator::CElementOperator(const TypeElement &enType, const TypeConversion &enTypeConversion,const std::wstring& wsNameOp)
		:CElement(TypeElement::Operator,enTypeConversion), m_pValueFrom(nullptr), m_pValueTo(nullptr), m_pValueOperator(nullptr),m_pUpperIndex(nullptr),m_pLowerIndex(nullptr),m_enTypeOperator(enType),m_wsName(wsNameOp)
	{
	}
	CElementOperator::~CElementOperator()
	{
		delete m_pValueOperator;
		delete m_pValueFrom;
		delete m_pValueTo;
		delete m_pLowerIndex;
		delete m_pUpperIndex;
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
		while(CElementIndex::GetUpperIndex(pReader->GetLocalType()) || CElementIndex::GetLowerIndex(pReader->GetLocalType()))
		{
			if(CElementIndex::GetUpperIndex(pReader->GetLocalType()))
			{
				pReader->ClearReader();
				m_pUpperIndex = CParserStarMathString::ParseElement(pReader);
			}
			if(CElementIndex::GetLowerIndex(pReader->GetLocalType()))
			{
				pReader->ClearReader();
				m_pLowerIndex = CParserStarMathString::ParseElement(pReader);
			}
			pReader->ReadingTheNextToken();
		}
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
		m_pValueOperator = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		while(pReader->GetGlobalType() == TypeElement::Index)
		{
			CParserStarMathString::ReadingElementsWithPriorities(pReader,m_pValueOperator);
		}
	}
	void CElementOperator::ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite)
	{
		if(m_enTypeOperator == TypeElement::lim || TypeElement::liminf == m_enTypeOperator || TypeElement::limsup == m_enTypeOperator || TypeElement::oper == m_enTypeOperator)
		{
			pXmlWrite->WriteNodeBegin(L"m:func",false);
			CConversionSMtoOOXML::PropertiesFuncPr(pXmlWrite,GetAttribute(),GetTypeConversion());
			pXmlWrite->WriteNodeBegin(L"m:fName",false);
			if((m_pValueFrom != nullptr || m_pLowerIndex != nullptr) && (m_pValueTo == nullptr && m_pUpperIndex == nullptr))
				CConversionSMtoOOXML::WriteLimUpOrLowNode(pXmlWrite,L"m:limLow",m_pValueFrom,m_enTypeOperator,GetAttribute(),this->GetTypeConversion(),m_wsName,m_pLowerIndex);
			else if((m_pValueTo != nullptr || m_pUpperIndex != nullptr ) && (m_pValueFrom == nullptr && m_pLowerIndex == nullptr))
				CConversionSMtoOOXML::WriteLimUpOrLowNode(pXmlWrite,L"m:limUpp",m_pValueTo,m_enTypeOperator,GetAttribute(),this->GetTypeConversion(),m_wsName,m_pUpperIndex);
			else if ((m_pValueFrom != nullptr || m_pLowerIndex != nullptr )&& (m_pValueTo != nullptr || m_pUpperIndex != nullptr))
			{
				pXmlWrite->WriteNodeBegin(L"m:limUpp",false);
				pXmlWrite->WriteNodeBegin(L"m:limUppPr",false);
				CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,nullptr,GetTypeConversion());
				pXmlWrite->WriteNodeEnd(L"m:limUppPr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:e",false);
				CConversionSMtoOOXML::WriteLimUpOrLowNode(pXmlWrite,L"m:limLow",m_pValueFrom,m_enTypeOperator,GetAttribute(),this->GetTypeConversion(),m_wsName,m_pLowerIndex);
				pXmlWrite->WriteNodeEnd(L"m:e",false,false);
				pXmlWrite->WriteNodeBegin(L"m:lim",false);
				if(m_pUpperIndex !=nullptr)
					m_pUpperIndex->ConversionToOOXML(pXmlWrite);
				if(m_pValueTo !=nullptr)
					m_pValueTo->ConversionToOOXML(pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:lim",false,false);
				pXmlWrite->WriteNodeEnd(L"m:limUpp",false,false);
			}
			else if(m_pValueFrom == nullptr && m_pValueTo == nullptr)
				CConversionSMtoOOXML::WriteRPrFName(m_enTypeOperator,pXmlWrite,GetAttribute(),GetName(),GetTypeConversion());
			pXmlWrite->WriteNodeEnd(L"m:fName",false,false);
			CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pValueOperator,pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:func",false,false);
		}
		else
		{
			pXmlWrite->WriteNodeBegin(L"m:nary",false);
			CConversionSMtoOOXML::PropertiesNaryPr(m_enTypeOperator,(nullptr == m_pValueFrom && nullptr == m_pLowerIndex),(nullptr == m_pValueTo && nullptr == m_pUpperIndex),pXmlWrite,GetAttribute(),GetTypeConversion());
			if(m_pValueFrom != nullptr && m_pLowerIndex != nullptr)
			{
				pXmlWrite->WriteNodeBegin(L"m:sub",false);
				m_pLowerIndex->ConversionToOOXML(pXmlWrite);
				m_pValueFrom->ConversionToOOXML(pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:sub",false);
			}
			else if(m_pValueFrom == nullptr && m_pLowerIndex != nullptr)
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pLowerIndex,pXmlWrite);
			else if(m_pValueFrom != nullptr && m_pLowerIndex == nullptr)
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sub",m_pValueFrom,pXmlWrite);
			else if(m_pValueFrom == nullptr && m_pLowerIndex == nullptr)
				pXmlWrite->WriteNode(L"m:sub",L"");
			if(m_pValueTo != nullptr && m_pUpperIndex != nullptr)
			{
				pXmlWrite->WriteNodeBegin(L"m:sup",false);
				m_pUpperIndex->ConversionToOOXML(pXmlWrite);
				m_pValueTo->ConversionToOOXML(pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:sup",false);
			}
			else if(m_pValueTo == nullptr && m_pUpperIndex != nullptr)
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pUpperIndex,pXmlWrite);
			else if(m_pValueTo != nullptr && m_pUpperIndex == nullptr)
				CConversionSMtoOOXML::WriteNodeConversion(L"m:sup",m_pValueTo,pXmlWrite);
			else if(m_pValueTo == nullptr && m_pUpperIndex == nullptr)
				pXmlWrite->WriteNode(L"m:sup",L"");
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
		if(m_pLowerIndex!=nullptr)
			m_pLowerIndex->SetAttribute(pAttribute);
		if(m_pUpperIndex!=nullptr)
			m_pUpperIndex->SetAttribute(pAttribute);
	}
	TFormulaSize CElementOperator::GetSize()
	{
		TFormulaSize tSizeTo,tSizeFrom;
		if(m_pValueTo != nullptr && m_pUpperIndex != nullptr)
		{
			tSizeTo = m_pValueTo->GetSize();
			CParserStarMathString::ComparisonByHeight(tSizeTo,m_pUpperIndex->GetSize());
		}
		else if(m_pUpperIndex != nullptr)
			tSizeTo = m_pUpperIndex->GetSize();
		else if(m_pValueTo != nullptr)
			tSizeTo = m_pValueTo->GetSize();
		if(m_pValueFrom != nullptr && m_pLowerIndex != nullptr)
		{
			tSizeFrom = m_pValueFrom->GetSize();
			CParserStarMathString::ComparisonByHeight(tSizeFrom,m_pLowerIndex->GetSize());
		}
		else if(m_pLowerIndex != nullptr)
			tSizeFrom = m_pLowerIndex->GetSize();
		else if(m_pValueFrom != nullptr)
			tSizeFrom = m_pValueFrom->GetSize();
		CParserStarMathString::ComparisonByWidth(tSizeTo,tSizeFrom);
		tSizeTo.m_iHeight += 1;
		if(m_pValueOperator != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeTo,m_pValueOperator->GetSize());
		return tSizeTo;
	}
// class methods CStarMathReader
	CStarMathReader::CStarMathReader(std::wstring::iterator& itStart, std::wstring::iterator& itEnd,const TypeConversion &enTypeConversion)
		: m_enGlobalType(TypeElement::Empty),m_enUnderType(TypeElement::Empty),m_pAttribute(nullptr),m_bMarkForUnar(true),m_enTypeCon(enTypeConversion)
	{
		m_itStart = itStart;
		m_itEnd = itEnd;
	}
	CStarMathReader::~CStarMathReader()
	{
		delete m_pAttribute;
	}
	//TODO :: ParseColor and ParseFont
	bool CStarMathReader::GetToken()
	{
		if(CheckIteratorPosition())
		{
			TokenProcessing();
			TypeElement enTypeFont = CAttribute::GetTypeFontAttribute(m_wsLowerCaseToken);
			if(enTypeFont != TypeElement::undefine || L"color" == m_wsLowerCaseToken)
				m_pAttribute = new CAttribute();
			while((enTypeFont != TypeElement::undefine || L"color" == m_wsLowerCaseToken) && m_itStart != m_itEnd)
			{
				if(L"color" == m_wsLowerCaseToken)
				{
					TokenProcessing();
					if(m_pAttribute->ParseColorAttribute(m_wsLowerCaseToken,this))
						m_wsLowerCaseToken.clear();
				}
				else if(enTypeFont != TypeElement::undefine)
					if(m_pAttribute->ParseFontAttribute(enTypeFont,this))
						m_wsLowerCaseToken.clear();
					else
						enTypeFont = CAttribute::GetTypeFontAttribute(m_wsLowerCaseToken);
				if((m_itStart != m_itEnd) && m_wsLowerCaseToken.empty())
				{
					TokenProcessing();
					enTypeFont = CAttribute::GetTypeFontAttribute(m_wsLowerCaseToken);
				}
			}
			if(m_pAttribute != nullptr && !m_pAttribute->CheckAttribute())
				m_pAttribute = nullptr;
			return true;
		}
		return false;
	}
	void CStarMathReader::SetTypesToken()
	{
		m_enUnderType = CElementOperator::GetFromOrTo(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
		m_enUnderType = CElementBracketWithIndex::GetBracketWithIndex(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::BracketWithIndex;
			return;
		}
		m_enUnderType = CElementGrade::GetGrade(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Grade;
			return;
		}
		m_enUnderType = CElementIndex::GetIndex(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Index;
			return;
		}
		m_enUnderType=CElementMatrix::GetMatrix(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Matrix;
			return;
		}
		m_enUnderType = CElementDiacriticalMark::GetMark(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Mark;
			return;
		}
		m_enUnderType = CElementBracket::GetBracketOpen(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Bracket;
			return;
		}
		m_enUnderType = CElementString::GetDigit(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
		m_enUnderType = CElementSpecialSymbol::GetSpecialSymbol(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::SpecialSymbol;
			return;
		}
		m_enUnderType = CElementBinOperator::GetBinOperator(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::BinOperator;
			return;
		}
		m_enUnderType = CElementSetOperations::GetSetOperation(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::SetOperations;
			return;
		}
		m_enUnderType = CElementConnection::GetConnection(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Connection;
			return;
		}
		m_enUnderType = CElementFunction::GetFunction(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Function;
			return;
		}
		m_enUnderType = CElementOperator::GetOperator(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Operation;
			return;
		}
		m_enUnderType = CElementString::GetWord(m_wsLowerCaseToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
		if(m_enUnderType == TypeElement::undefine && !m_wsLowerCaseToken.empty())
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
	}
	void CStarMathReader::TokenProcessing(const std::wstring &wsToken)
	{
		if(wsToken.empty())
			m_wsOriginalToken = GetElement();
		else
			m_wsOriginalToken = wsToken;
		m_wsLowerCaseToken = CParserStarMathString::ConvertToLowerCase(m_wsOriginalToken);
	}
	TypeElement CStarMathReader::GetLocalType()
	{
		return m_enUnderType;
	}
	TypeElement CStarMathReader::GetGlobalType()
	{
		return m_enGlobalType;
	}
	std::wstring CStarMathReader::GetLowerCaseString()
	{
		return m_wsLowerCaseToken;
	}
	std::wstring CStarMathReader::GetOriginalString()
	{
		return m_wsOriginalToken;
	}
	void CStarMathReader::ClearReader()
	{
		m_wsLowerCaseToken.clear();
		m_wsOriginalToken.clear();
		m_enGlobalType = TypeElement::Empty;
		m_enUnderType = TypeElement::Empty;
		m_pAttribute = nullptr;
	}
	bool CStarMathReader::EmptyString()
	{
		return m_wsLowerCaseToken.empty();
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
	void CStarMathReader::SetBaseAttribute(const TBaseAttribute& pAttribute)
	{
		m_pBaseAttribute = new CAttribute;
		if(pAttribute.base_font_italic)
			m_pBaseAttribute->SetItal();
		if(pAttribute.base_font_bold)
			m_pBaseAttribute->SetBold();
		if(!pAttribute.base_font_name.empty())
			m_pBaseAttribute->SetFontName(pAttribute.base_font_name);
		if(pAttribute.base_font_size != 0)
			m_pBaseAttribute->SetSize(pAttribute.base_font_size);
		if(pAttribute.base_alignment >= 0 && pAttribute.base_alignment <=2)
			m_pBaseAttribute->SetAlignment(pAttribute.base_alignment);
		if(!m_pBaseAttribute->CheckingForEmptiness())
			m_pBaseAttribute = nullptr;
		m_pBaseAttribute->SetParent();
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
			else if(!m_wsElement.empty() && (CheckTokenForGetElement(*m_itStart)||(m_wsElement.back() == L'<' && (L'-' != *m_itStart && L'?' != *m_itStart && L'=' != *m_itStart && L'<' != *m_itStart && L'>' != *m_itStart)) ||  *m_itStart == L'(' || L')' == *m_itStart  || L'%' == *m_itStart||(L'#' == *m_itStart && L'#' != m_wsElement.back()) ||(L'-' == *m_itStart  && L'+' != m_wsElement.back() && L'<' != m_wsElement.back()) || (L'+' == *m_itStart  && L'-' != m_wsElement.back()) || (L'.' == *m_itStart && !iswdigit(m_wsElement.back())) || (iswdigit(*m_itStart) && !iswdigit(m_wsElement.back()) && L'.' != m_wsElement.back()) || (CheckIsalhpaForGetElement(*m_itStart,m_wsElement.back())) || ((m_wsElement.back() != L'<' && m_wsElement.back() != L'>') && (L'<' == *m_itStart || (L'>' == *m_itStart && L'-' !=m_wsElement.back() && L'?' != m_wsElement.back()) || L'=' == *m_itStart))))
			{
				return m_wsElement;
			}
			else if((( CheckTokenForGetElement(*m_itStart) || L'=' == *m_itStart) && m_wsElement.empty()) || (!m_wsElement.empty() && ((L'#' == m_wsElement.back() && L'#' == *m_itStart)  || (L'-' == *m_itStart  && L'+' == m_wsElement.back()) || ((L'+' == *m_itStart || L'>' == *m_itStart) && L'-' == m_wsElement.back()) || (m_wsElement.back() == L'<' && (L'=' == *m_itStart || L'<' == *m_itStart || L'>' == *m_itStart || L'-' == *m_itStart)) ||(L'?' == m_wsElement.back() && L'>' == *m_itStart) || (m_wsElement.back() == L'>' && (L'>' == *m_itStart || L'=' == *m_itStart ))  ) ) )
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
	wchar_t CStarMathReader::GetOneElement()
	{
		if(m_itStart!=m_itEnd)
		{
			wchar_t cOne = (*m_itStart);
			m_itStart++;
			return cOne;
		}
		else
			return L' ';
	}
	std::wstring CStarMathReader::TakingElementForHex()
	{
		std::wstring wsTokenHex{};
		for(;m_itStart != m_itEnd;m_itStart++)
		{
			if(iswspace(*m_itStart))
				return wsTokenHex;
			else if(CAttribute::CheckHexPosition(*m_itStart))
				wsTokenHex.push_back(*m_itStart);
			else
				return wsTokenHex;
		}
		return wsTokenHex;
	}
	int CStarMathReader::TakingElementForRGB()
	{
		int iTokenRGB{-1};
		std::wstring wsToken;
		std::wstring::iterator itTempStart = m_itStart;
		wsToken = GetElement();
		if(CElementString::GetDigit(wsToken) != TypeElement::undefine)
		{
			iTokenRGB = std::stoi(wsToken);
			if(iTokenRGB >= 0 && iTokenRGB<256)
				return iTokenRGB;
			else
			{
				m_itStart = itTempStart;
				return -1;
			}
		}
		else
		{
			m_itStart = itTempStart;
			return -1;
		}
	}
	void CStarMathReader::SetString(const std::wstring &wsToken)
	{
		m_wsLowerCaseToken = wsToken;
	}
	void CStarMathReader::FindingTheEndOfParentheses()
	{
		std::wstring::iterator itStart = m_itStart,itStartBracketClose;
		int inBracketInside = 0;
		while(CheckIteratorPosition())
		{
			itStartBracketClose = m_itStart;
			bool res = GetToken();
			if (false == res)
			{
				break;
			}
			if(CElementBracket::GetBracketOpen(m_wsLowerCaseToken) != TypeElement::undefine)
			{
				if(CElementBracket::GetBracketOpen(m_wsLowerCaseToken) == TypeElement::left)
                {
                    if(GetToken() && (m_wsLowerCaseToken == L"none" || (m_wsLowerCaseToken != L"left" && CElementBracket::GetBracketOpen(m_wsLowerCaseToken) != TypeElement::undefine)))
                    {
                        inBracketInside += 1;
                        continue;
                    }
                }
				else
					inBracketInside +=1;
			}
			else if(CElementBracket::GetBracketClose(m_wsLowerCaseToken) == TypeElement::right)
				continue;
			else if(CElementBracket::GetBracketClose(m_wsLowerCaseToken) != TypeElement::undefine && inBracketInside == 0)
			{
				m_stBracket.push(m_itEnd);
				m_stCloseBracket.push(m_itStart);
				m_itEnd = itStartBracketClose;
				break;
			}
			else if(CElementBracket::GetBracketClose(m_wsLowerCaseToken) != TypeElement::undefine && inBracketInside != 0)
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
		ClearReader();
	}
	void CStarMathReader::SettingTheIteratorToTheClosingBracket()
	{
		if(!m_stCloseBracket.empty())
		{
			m_itEnd = m_stCloseBracket.top();
			m_stCloseBracket.pop();
		}
		ClearReader();
	}
	void CStarMathReader::ReadingTheNextToken()
	{
		if(m_wsLowerCaseToken.empty())
		{
			if (GetToken())
				SetTypesToken();
			else
			{
				ClearReader();
			}
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
		case L'"':
		case L'\'':
		return true;
		default:
		return false;
		}
	}
	bool CStarMathReader::CheckIsalhpaForGetElement(const wchar_t &cToken, const wchar_t &cLastToken)
	{
		if(iswalpha(cToken))
		{
			if(L'%' == cLastToken)
				return false;
//			else if(L'\u2030' == cLastToken)
//				return false;
			if(iswalpha(cLastToken))
				return false;
		}
		else
			return false;
		return true;
	}
	void CStarMathReader::SetMarkForUnar(const bool &bMark)
	{
		m_bMarkForUnar = bMark;
	}
	bool CStarMathReader::GetMarkForUnar()
	{
		return m_bMarkForUnar;
	}
	void CStarMathReader::SetTypeConversion(const TypeConversion &enTypeCon)
	{
		m_enTypeCon = enTypeCon;
	}
	TypeConversion CStarMathReader::GetTypeConversion()
	{
		return m_enTypeCon;
	}
//class methods CElementBracketWithIndex
	CElementBracketWithIndex::CElementBracketWithIndex(const TypeElement &enType,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::BracketWithIndex,enTypeConversion),m_pLeftArg(nullptr), m_pValue(nullptr),m_enTypeBracketWithIndex(enType)
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
		m_pValue = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		while(pReader->GetGlobalType() == TypeElement::Index)
		{
			CParserStarMathString::ReadingElementsWithPriorities(pReader,m_pValue);
		}
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
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute(),GetTypeConversion());
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
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute(),GetTypeConversion());
		pXmlWrite->WriteNodeEnd(L"m:groupChrPr",false,false);
		CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pLeftArg,pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:groupChr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		CConversionSMtoOOXML::WriteNodeConversion(L"m:lim",m_pValue,pXmlWrite);
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
	const TypeElement& CElementBracketWithIndex::GetType()
	{
		return m_enTypeBracketWithIndex;
	}
	TFormulaSize CElementBracketWithIndex::GetSize()
	{
		TFormulaSize tSize,tSizeFrom;
		if(m_pLeftArg != nullptr)
			tSize = m_pLeftArg->GetSize();
		if(m_pValue != nullptr)
			tSizeFrom = m_pValue->GetSize();
		CParserStarMathString::ComparisonByWidth(tSize,tSizeFrom);
		return tSize;
	}
//class methods CElementGrade
	CElementGrade::CElementGrade(const TypeConversion &enTypeConversion)
		:CElement(TypeElement::Grade,enTypeConversion),m_pValueGrade(nullptr), m_pValueFrom(nullptr), m_pValueTo(nullptr)
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
		m_pValueGrade = CParserStarMathString::ParseElement(pReader);
		pReader->ReadingTheNextToken();
		if(pReader->GetLocalType() == TypeElement::from)
		{
			pReader->ClearReader();
			m_pValueFrom = CParserStarMathString::ParseElement(pReader);
			pReader->ReadingTheNextToken();
		}
		if(pReader->GetLocalType() == TypeElement::to)
		{
			pReader->ClearReader();
			m_pValueTo = CParserStarMathString::ParseElement(pReader);
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
			CConversionSMtoOOXML::StandartProperties(pXmlWrite,GetAttribute(),GetTypeConversion());
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
	TFormulaSize CElementGrade::GetSize()
	{
		TFormulaSize tSizeTo,tSizeFrom;
		if(m_pValueFrom != nullptr)
			tSizeFrom = m_pValueFrom->GetSize();
		if(m_pValueTo != nullptr)
			tSizeTo = m_pValueTo->GetSize();
		CParserStarMathString::ComparisonByWidth(tSizeTo,tSizeFrom);
		tSizeTo.m_iHeight += 1;
		if(m_pValueGrade != nullptr)
			CParserStarMathString::ComparisonByHeight(tSizeTo,m_pValueGrade->GetSize());
		return tSizeTo;
	}
//class methods CElementMatrix
	CElementMatrix::CElementMatrix(const TypeElement &enType,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::Matrix,enTypeConversion), m_pFirstArgument(nullptr), m_pSecondArgument(nullptr), m_enTypeMatrix(enType)
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
			SetFirstArgument(CParserStarMathString::ReadingWithoutBracket(pReader,false));
			SetSecondArgument(CParserStarMathString::ReadingWithoutBracket(pReader,false));
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
		CConversionSMtoOOXML::PropertiesMPr(pXmlWrite,m_enTypeMatrix,GetAttribute(),GetTypeConversion());
		pXmlWrite->WriteNodeBegin(L"m:mr",false);
		bool bNormal(false);
		switch(m_enTypeMatrix)
		{
			case TypeElement::matrix:
			case TypeElement::stack:
			{
			if(m_pFirstArgument != nullptr)
			{
				if(m_pFirstArgument->GetBaseType() == TypeElement::Bracket)
				{
					CElementBracket* pTempBracket = dynamic_cast<CElementBracket*>(m_pFirstArgument);
					std::vector<CElement*> pTempValue = pTempBracket->GetBracketValue();
					pXmlWrite->WriteNodeBegin(L"m:e",false);
					for(CElement* pOneElement:pTempValue)
					{
						if(pOneElement->GetBaseType() != TypeElement::undefine && pOneElement->GetBaseType() != TypeElement::SpecialSymbol)
							pOneElement->ConversionToOOXML(pXmlWrite);
//						{
//							CConversionSMtoOOXML::WriteNodeConversion(L"m:e",pOneElement,pXmlWrite);
//							pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
//							pXmlWrite->WriteNodeBegin(L"m:mr",false);
//						}
//						else if(pOneElement->GetBaseType() != TypeElement::SpecialSymbol && pOneElement->GetBaseType()!= TypeElement::undefine && m_enTypeMatrix == TypeElement::matrix)
//						{
//							CConversionSMtoOOXML::WriteNodeConversion(L"m:e",pOneElement,pXmlWrite);
//						}
						else if(pOneElement->GetBaseType()!= TypeElement::undefine && pOneElement->GetBaseType() == TypeElement::SpecialSymbol)
						{
							CElementSpecialSymbol* pTempSpecial = dynamic_cast<CElementSpecialSymbol*>(pOneElement);
							if(pTempSpecial->GetType() == TypeElement::transition && m_enTypeMatrix == TypeElement::matrix)
							{
								pXmlWrite->WriteNodeEnd(L"m:e",false,false);
								pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
								pXmlWrite->WriteNodeBegin(L"m:mr",false);
								pXmlWrite->WriteNodeBegin(L"m:e",false);
							}
							else if(pTempSpecial->GetType() == TypeElement::grid)
							{
								switch(m_enTypeMatrix)
								{
								case TypeElement::stack:
								{
									pXmlWrite->WriteNodeEnd(L"m:e",false,false);
									pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
									pXmlWrite->WriteNodeBegin(L"m:mr",false);
									pXmlWrite->WriteNodeBegin(L"m:e",false);
									break;
								}
								case TypeElement::matrix:
								{
									pXmlWrite->WriteNodeEnd(L"m:e",false,false);
									pXmlWrite->WriteNodeBegin(L"m:e",false);
									break;
								}
								default:
								break;
								}
							}
							else if(pTempSpecial->GetType() != TypeElement::grid && pTempSpecial->GetType() != TypeElement::transition)
								pOneElement->ConversionToOOXML(pXmlWrite);
						}
					}
					bNormal = true;
				}
				else if(m_enTypeMatrix == TypeElement::matrix)
				{
					CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pFirstArgument,pXmlWrite);
					CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
					pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
					pXmlWrite->WriteNodeBegin(L"m:mr",false);
					CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
					CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				}
				else if (m_enTypeMatrix == TypeElement::stack)
				{
					CConversionSMtoOOXML::WriteNodeConversion(L"m:e",m_pFirstArgument,pXmlWrite);
					pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
					pXmlWrite->WriteNodeBegin(L"m:mr",false);
					CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				}
				break;
			}
			else if(m_pFirstArgument == nullptr && m_enTypeMatrix == TypeElement::matrix)
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:mr",false);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				break;
			}
			else if(m_pFirstArgument == nullptr && m_enTypeMatrix == TypeElement::stack)
			{
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				pXmlWrite->WriteNodeEnd(L"m:mr",false,false);
				pXmlWrite->WriteNodeBegin(L"m:mr",false);
				CConversionSMtoOOXML::WriteNodeConversion(L"m:e",nullptr,pXmlWrite);
				break;
			}
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
		if(bNormal)
			pXmlWrite->WriteNodeEnd(L"m:e",false,false);
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
	TFormulaSize CElementMatrix::GetSize()
	{
		TFormulaSize tSize;
		if(m_pFirstArgument!= nullptr)
			tSize = m_pFirstArgument->GetSize();
		if(m_pSecondArgument != nullptr)
			CParserStarMathString::ComparisonByWidth(tSize,m_pSecondArgument->GetSize());
		else if(m_pFirstArgument!= nullptr && m_pFirstArgument->GetBaseType() != TypeElement::Bracket)
		{
			tSize.m_iHeight += 1.5;
			tSize.m_iWidth += 2;
		}
		return tSize;
	}
//class CElementDiacriticalMark
	CElementDiacriticalMark::CElementDiacriticalMark(const TypeElement& enType,const TypeConversion &enTypeConversion)
		:CElement(TypeElement::Mark,enTypeConversion),m_pValueMark(nullptr),m_enTypeMark(enType)
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
		else return TypeElement::undefine;
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
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,GetAttribute(),GetTypeConversion());
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
	TFormulaSize CElementDiacriticalMark::GetSize()
	{
		TFormulaSize tSize;
		tSize = m_pValueMark->GetSize();
		tSize.m_iHeight += 1;
		return tSize;
	}
}


