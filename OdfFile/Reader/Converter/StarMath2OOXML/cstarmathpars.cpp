#include "cstarmathpars.h"
namespace StarMath
{
//class methods CParsStarMath
	std::vector<CElement*> CParseStarMathString::Parse(std::wstring& wsParseString)
	{
		std::wstring::iterator itStart = wsParseString.begin(),itEnd = wsParseString.end();
		while(itStart != itEnd)
		{
			arEquation.push_back(ParsElement(itStart,itEnd));
		}
		return arEquation;
	}

	CElement* CParseStarMathString::ParsElement(std::wstring::iterator& itStart, std::wstring::iterator& itEnd)
	{
		CElement* pElement;
		std::wstring wsToken = GetElement(itStart,itEnd);
		std::vector<CAttribute*> arAttributes;
		TypeElement enTypeToken = CAttribute::IsAttribute(wsToken);
		while(enTypeToken != TypeElement::undefine && itStart != itEnd)
		{
			if(enTypeToken != TypeElement::color) arAttributes.push_back(new CAttribute(enTypeToken));
			wsToken = GetElement(itStart,itEnd);
			enTypeToken = CAttribute::IsAttribute(wsToken);
		}

		if(L"left" == wsToken) pElement = CElement::CreateElement(GetElement(itStart,itEnd));
		else pElement = CElement::CreateElement(wsToken);

		if(pElement != nullptr)
		{
			pElement->SetAttribute(arAttributes);
			pElement->Pars(itStart,itEnd);
			return pElement;
		}
		else  return pElement;
	}
	std::wstring CParseStarMathString::GetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd)
	{
		std::wstring m_wsElement{};
		for(; itFirst != itEnd;itFirst++)
		{
			if(iswspace(*itFirst) && m_wsElement.empty()) continue;
			else if(iswspace(*itFirst) && !m_wsElement.empty())
			{
				itFirst++;
				break;
			}
			else if(!m_wsElement.empty() && (*itFirst == L'{' || *itFirst == L'}' || *itFirst == L'+' || *itFirst == L'-' || *itFirst == L'/' || *itFirst == L'*' || L'^' == *itFirst || L'_' == *itFirst  || (iswdigit(*itFirst) && !iswdigit(m_wsElement.back())) || (iswalpha(*itFirst) && !iswalpha(m_wsElement.back())) || ((m_wsElement.back() != L'<' && m_wsElement.back() != L'>') && (L'<' == *itFirst || L'>' == *itFirst || L'=' == *itFirst))))
			{
				return m_wsElement;
			}
			else if(((*itFirst == L'{' || *itFirst == L'}' || *itFirst == L'+' || *itFirst == L'-' || *itFirst == L'/' || *itFirst == L'*' || L'^' == *itFirst || L'_' == *itFirst || L'=' == *itFirst ) && m_wsElement.empty()) || (!m_wsElement.empty() && ((m_wsElement.back() == L'<' && (L'=' == *itFirst || L'<' == *itFirst || L'>' == *itFirst)) || (m_wsElement.back() == L'>' && (L'>' == *itFirst || L'=' == *itFirst)) ) ) )
			{
				m_wsElement.push_back(*itFirst);
				itFirst++;
				return m_wsElement;
			}
			else
			{
				m_wsElement.push_back(*itFirst);
			}
		}
		if(!m_wsElement.empty()) return m_wsElement;
		else return {};
	}
	bool CParseStarMathString::CheckingTheNextElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd, bool (&func)(const std::wstring&))
	{
		std::wstring::iterator itTempVal = itFirst;
		std::wstring wsAttributeToken = GetElement(itFirst,itEnd);
//		while(true)
//		{
//			if(wsAttributeToken == L"color") wsAttributeToken = GetElement(itFirst,itEnd);
//			wsAttributeToken = GetElement(itFirst,itEnd);
//		}
		itFirst = itTempVal;
		if(func(wsAttributeToken)) return true;
		else return false;
	}
//class methods CAttribute
	CAttribute::~CAttribute()
	{}
	CAttribute::CAttribute(const TypeElement &enType)
	{
		enTypeAttr = enType;
	}
	TypeElement CAttribute::GetType()
	{
		return enTypeAttr;
	}
//нет phantom, rgb, 16 , гарнитуры и кегля
	TypeElement CAttribute::IsAttribute(const std::wstring &wsToken)
	{
		if(L"acute" == wsToken) return TypeElement::acute;
		else if(L"color" == wsToken) return TypeElement::color;
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
		else if(L"ital" == wsToken) return TypeElement::ital;
		else if(L"bold" == wsToken) return TypeElement::bold;
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
//class methods CElement
	CElement::~CElement()
	{}
	CElement::CElement()
	{}
//	TypeElement CElement::GetTypeElement(const std::wstring& wsToken)
//	{
//			if (wsToken == L"+")
//			{
//				return  TypeElement::plus;
//			}
//			else if (wsToken == L"-")
//			{
//				return  TypeElement::minus;
//			}
//			else if (wsToken == L"*")
//			{
//				return  TypeElement::multipl;
//			}
//			else if (wsToken == L"/")
//			{
//				return  TypeElement::division;
//			}
//			else if (wsToken == L"over")
//			{
//				return  TypeElement::over;
//			}
//			else if (wsToken == L"cdot")
//			{
//				return  TypeElement::cdot;
//			}
//			else if (wsToken == L"times")
//			{
//				return  TypeElement::times;
//			}
//			else if (wsToken == L"frac")
//			{
//				return  TypeElement::frac;
//			}
//			else if (wsToken == L"div")
//			{
//				return  TypeElement::div;
//			}
//			else if (wsToken == L"oplus")
//			{
//				return  TypeElement::oplus;
//			}
//			else if (wsToken == L"ominus")
//			{
//				return  TypeElement::ominus;
//			}
//			else if (wsToken == L"odot")
//			{
//				return  TypeElement::odot;
//			}
//			else if (wsToken == L"otimes")
//			{
//				return  TypeElement::otimes;
//			}
//			else if (wsToken == L"odivide")
//			{
//				return  TypeElement::odivide;
//			}
//			else if (wsToken == L"circ")
//			{
//				return  TypeElement::circ;
//			}
//			else if (wsToken == L"wideslash")
//			{
//				return  TypeElement::wideslash;
//			}
//			else if (wsToken == L"widebslash")
//			{
//				return  TypeElement::widebslash;
//			}
//			else if(L"{" == wsToken) return TypeElement::brace;
//			else if(L"(" == wsToken) return TypeElement::round;
//			else if(L"[" == wsToken) return TypeElement::square;
//			else if(L"ldbracket" == wsToken) return TypeElement::ldbracket;
//			else if(L"lbrace" == wsToken) return TypeElement::lbrace;
//			else if(L"langle" == wsToken) return TypeElement::langle;
//			else if(L"lceil" == wsToken) return TypeElement::lceil;
//			else if(L"lfloor" == wsToken) return TypeElement::lfloor;
//			else if(L"lline" == wsToken) return TypeElement::lline;
//			else if(L"ldline" == wsToken) return TypeElement::ldline;
//			else return TypeElement::undefine;
//	}
	CElement* CElement::CreateElement(const std::wstring& wsToken)
	{
		//binop
		if (wsToken == L"+")
		{
			return  new CElementBinOperator(TypeElement::plus);
		}
		else if (wsToken == L"-")
		{
			return new CElementBinOperator(TypeElement::minus);
		}
		else if (wsToken == L"*")
		{
			return  new CElementBinOperator(TypeElement::multipl);
		}
		else if (wsToken == L"/")
		{
			return new CElementBinOperator(TypeElement::division);
		}
		else if (wsToken == L"over")
		{
			return new CElementBinOperator(TypeElement::over);
		}
		else if (wsToken == L"cdot")
		{
			return new CElementBinOperator(TypeElement::cdot);
		}
		else if (wsToken == L"times")
		{
			return new CElementBinOperator(TypeElement::times);
		}
		else if (wsToken == L"frac")
		{
			return new CElementBinOperator(TypeElement::frac);
		}
		else if (wsToken == L"div")
		{
			return new CElementBinOperator(TypeElement::div);
		}
		else if (wsToken == L"oplus")
		{
			return new CElementBinOperator(TypeElement::oplus);
		}
		else if (wsToken == L"ominus")
		{
			return new CElementBinOperator(TypeElement::ominus);
		}
		else if (wsToken == L"odot")
		{
			return new CElementBinOperator(TypeElement::odot);
		}
		else if (wsToken == L"otimes")
		{
			return new CElementBinOperator(TypeElement::otimes);
		}
		else if (wsToken == L"odivide")
		{
			return new CElementBinOperator(TypeElement::odivide);
		}
		else if (wsToken == L"circ")
		{
			return new CElementBinOperator(TypeElement::circ);
		}
		else if (wsToken == L"wideslash")
		{
			return new CElementBinOperator(TypeElement::wideslash);
		}
		else if (wsToken == L"widebslash")
		{
			return new CElementBinOperator(TypeElement::widebslash);
		}
		//brace
		else if(L"{" == wsToken) return new CElementBracket(TypeElement::brace);
//		else if(L"(" == wsToken) return TypeElement::round;
//		else if(L"[" == wsToken) return TypeElement::square;
//		else if(L"ldbracket" == wsToken) return TypeElement::ldbracket;
//		else if(L"lbrace" == wsToken) return TypeElement::lbrace;
//		else if(L"langle" == wsToken) return TypeElement::langle;
//		else if(L"lceil" == wsToken) return TypeElement::lceil;
//		else if(L"lfloor" == wsToken) return TypeElement::lfloor;
//		else if(L"lline" == wsToken) return TypeElement::lline;
//		else if(L"ldline" == wsToken) return TypeElement::ldline;
		else if(CElementString::IsDigit(wsToken)) return new CElementString(wsToken);
		else return nullptr;
	}
	void CElement::SetAttribute(const std::vector<CAttribute *> arAttr)
	{
		arElementAttributes = arAttr;
	}
//class methods CElementString
	CElementString::CElementString(const std::wstring& wsTokenString)
	{
		wsString = wsTokenString;
	}
	CElementString::~CElementString()
	{}
	void CElementString::SetString(const std::wstring& wsTokenString)
	{
		wsString = wsTokenString;
	}
	void CElementString::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
	}
	std::wstring CElementString::GetString()
	{
		return wsString;
	}
	bool CElementString::IsDigit(const std::wstring &wsToken)
	{
		for(char cOneElement: wsToken)
		{
			if(!isdigit(cOneElement)) return false;
		}
		return true;
	}
//class methods CElementBinOperator
	CElementBinOperator::CElementBinOperator(const TypeElement& enType)
	{
		enTypeBinOp = enType;
	}
	CElementBinOperator::~CElementBinOperator()
	{
		delete pLeftArgument;
		delete pRightArgument;
	}
	void CElementBinOperator::SetLeftArg(CElement *pElement)
	{
		pLeftArgument = pElement;
	}
	void CElementBinOperator::SetRightArg(CElement *pElement)
	{
		pRightArgument = pElement;
	}
	void CElementBinOperator::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		SetRightArg(CParseStarMathString::ParsElement(itStart,itEnd));
	}
	void CElementBinOperator::SetTypeBinOP(const TypeElement &enType)
	{
		enTypeBinOp = enType;
	}
	bool CElementBinOperator::IsBinOperator(const TypeElement& enCheckType)
	{
		switch (enCheckType)
		{
		case TypeElement::plus:
		return true;
		case TypeElement::minus:
		return true;
		case TypeElement::over:
		return true;
		case TypeElement::multipl:
		return true;
		case TypeElement::division:
		return true;
		case TypeElement::cdot:
		return true;
		case TypeElement::times:
		return true;
		case TypeElement::frac:
		return true;
		case TypeElement::div:
		return true;
		case TypeElement::oplus:
		return true;
		case TypeElement::ominus:
		return true;
		case TypeElement::odot:
		return true;
		case TypeElement::otimes:
		return true;
		case TypeElement::odivide:
		return true;
		case TypeElement::circ:
		return true;
		case TypeElement::wideslash:
		return true;
		case TypeElement::widebslash:
		return true;
		default:
		return false;
		}
	}
	bool CElementBinOperator::IsLowPriorityBinOp(const TypeElement &enType)
	{
		switch (enType)
		{
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
			default:
				 return false;
		}
	}
	bool CElementBinOperator::IsHighPriorityBinOp(const TypeElement &enType)
	{
		switch(enType)
		{
			case TypeElement::over:
				return true;
			default:
				return false;
		}
	}
//class methods CElementBracket
	CElementBracket::CElementBracket(const TypeElement& enType)
	{
		enTypeBracket = enType;
	}
	CElementBracket::~CElementBracket()
	{
		for(CElement* pTemp:arBrecketValue) delete pTemp;
	}
	void CElementBracket::SetBracketValue(const std::vector<CElement*> &arValue)
	{
		arBrecketValue = arValue;
	}
	bool CElementBracket::IsBracketClose(const std::wstring &wsToken)
	{
		if(L"}" == wsToken) return true;
		else if(L")" == wsToken) return true;
		else if(L"]" == wsToken) return true;
		else return false;
	}
	//нужно поправить GetElement(беру его тут + в методе ParsElement)
	void CElementBracket::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		std::wstring wsToken = CParseStarMathString::GetElement(itStart,itEnd) ;
		while(L"right" != wsToken && !IsBracketClose(wsToken))
		{
			arBrecketValue.push_back(CParseStarMathString::ParsElement(itStart,itEnd));
			wsToken = CParseStarMathString::GetElement(itStart,itEnd);
		}
	}
}


