#include "cstarmathpars.h"
namespace StarMath
{
//class methods CParsStarMath
	std::vector<CElement*> CParseStarMathString::Parse(std::wstring& wsParseString)
	{
		std::wstring::iterator itStart = wsParseString.begin(),itEnd = wsParseString.end();

		while(itStart != itEnd)
		{
			CElement* pTempElement = ParsElement(itStart,itEnd);
			if(nullptr == pTempElement)
				break;
			if(!arEquation.empty() && (pTempElement->GetBaseType() == TypeElement::BinOperator || pTempElement->GetBaseType() == TypeElement::SetOperations || pTempElement->GetBaseType() == TypeElement::Connection) )
			{
				AddLeftArgument(arEquation.back(),pTempElement);
				arEquation.pop_back();
			}
			arEquation.push_back(pTempElement);
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
			if(CheckingTheNextElement(itStart,itEnd,CIndex::IsIndex))
			{
				CIndex* pIndex = CIndex::CreateIndex(GetElement(itStart,itEnd));
				pIndex->SetValueIndex(ParsElement(itStart,itEnd));
				pElement->SetIndex(pIndex);
			}
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
			else if(!m_wsElement.empty() && (*itFirst == L'(' || L')' == *itFirst || *itFirst == L'{' || *itFirst == L'}' || *itFirst == L'+' || *itFirst == L'-' || *itFirst == L'/' || *itFirst == L'*' || L'^' == *itFirst || L'_' == *itFirst  || (iswdigit(*itFirst) && !iswdigit(m_wsElement.back())) || (iswalpha(*itFirst) && !iswalpha(m_wsElement.back())) || ((m_wsElement.back() != L'<' && m_wsElement.back() != L'>') && (L'<' == *itFirst || L'>' == *itFirst || L'=' == *itFirst))))
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
		std::wstring wsToken = GetElement(itFirst,itEnd);
		TypeElement enTypeAttr = CAttribute::IsAttribute(wsToken);
		while(enTypeAttr != TypeElement::undefine && (itFirst != itEnd))
		{
			wsToken = GetElement(itFirst,itEnd);
			enTypeAttr = CAttribute::IsAttribute(wsToken);
		}
		itFirst = itTempVal;
		if(func(wsToken)) return true;
		else return false;
	}
	bool CParseStarMathString::MoveToNextElement(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		if(itStart !=itEnd)
		{
			std::wstring wsNextElement = CParseStarMathString::GetElement(itStart,itEnd);
			if(L"right" == wsNextElement && (itStart!=itEnd)) wsNextElement = CParseStarMathString::GetElement(itStart,itEnd);
			return true;
		}
		return false;
	}
	template<typename T>
	void CParseStarMathString::SetLeft(CElement *pLeftArg, CElement *pElementWhichAdd)
	{
		T* pBinOpElement = dynamic_cast<T*>(pElementWhichAdd);
		pBinOpElement->SetLeftArg(pLeftArg);
		pElementWhichAdd = pBinOpElement;
	}

	void CParseStarMathString::AddLeftArgument(CElement *pLeftArg, CElement *pElementWhichAdd)
	{
		switch(pElementWhichAdd->GetBaseType())
		{
			case TypeElement::BinOperator:
			{
				SetLeft<CElementBinOperator>(pLeftArg, pElementWhichAdd);
				break;
			}
			case TypeElement::SetOperations:
			{
				SetLeft<CElementSetOperations>(pLeftArg, pElementWhichAdd);
				break;
			}
			case TypeElement::Connection:
			{
				SetLeft<CElementConnection>(pLeftArg,pElementWhichAdd);
				break;
			}
			default:
				break;
		}
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
	CElement::CElement(): pElementIndex(nullptr)
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
		else if(CElementString::IsDigit(wsToken)) return new CElementString(wsToken);
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
		else if(L"(" == wsToken) return new CElementBracket(TypeElement::round);
		else if(L"[" == wsToken) return new CElementBracket(TypeElement::square);
		else if(L"ldbracket" == wsToken) return new CElementBracket(TypeElement::ldbracket);
		else if(L"lbrace" == wsToken) return new CElementBracket(TypeElement::lbrace);
		else if(L"langle" == wsToken) return new CElementBracket(TypeElement::langle);
		else if(L"lceil" == wsToken) return new CElementBracket(TypeElement::lceil);
		else if(L"lfloor" == wsToken) return new CElementBracket(TypeElement::lfloor);
		else if(L"lline" == wsToken) return new CElementBracket(TypeElement::lline);
		else if(L"ldline" == wsToken) return new CElementBracket(TypeElement::ldline);
		else if(L"intersection" == wsToken) return new CElementSetOperations(TypeElement::intersection);
		else if(L"union" == wsToken) return new CElementSetOperations(TypeElement::Union);
		else if(L"setminus" == wsToken) return new CElementSetOperations(TypeElement::setminus);
		else if(L"setquoyient" == wsToken) return new CElementSetOperations(TypeElement::setquoyient);
		else if(L"subseteq" == wsToken) return new CElementSetOperations(TypeElement::subseteq);
		else if(L"subset" == wsToken) return new CElementSetOperations(TypeElement::subset);
		else if(L"supset" == wsToken) return new CElementSetOperations(TypeElement::supset);
		else if(L"supseteq" == wsToken) return new CElementSetOperations(TypeElement::supseteq);
		else if(L"nsubset" == wsToken) return new CElementSetOperations(TypeElement::nsubset);
		else if(L"nsubseteq" == wsToken) return new CElementSetOperations(TypeElement::nsubseteq);
		else if(L"nsupset" == wsToken) return new CElementSetOperations(TypeElement::nsupset);
		else if(L"nsubseteq" == wsToken) return new CElementSetOperations(TypeElement::nsubseteq);
		else if(L"in" == wsToken) return new CElementSetOperations(TypeElement::in);
		else if(L"notin" == wsToken) return new CElementSetOperations(TypeElement::notin);
		else if(L"owns" == wsToken) return new CElementSetOperations(TypeElement::owns);
		else if(L"approx" == wsToken) return new CElementConnection(TypeElement::approx);
		else if(L"sim" == wsToken) return new CElementConnection(TypeElement::sim);
		else if(L"simeq" == wsToken) return new CElementConnection(TypeElement::simeq);
		else if(L"equiv" == wsToken) return new CElementConnection(TypeElement::equiv);
		else if(L"prop" == wsToken) return new CElementConnection(TypeElement::prop);
		else if(L"parallel" == wsToken) return new CElementConnection(TypeElement::parallel);\
		else if(L"ortho" == wsToken) return new CElementConnection(TypeElement::ortho);
		else if(L"divides" == wsToken) return new CElementConnection(TypeElement::divides);
		else if(L"ndivides" == wsToken) return new CElementConnection(TypeElement::ndivides);
		else if(L"toward" == wsToken) return new CElementConnection(TypeElement::toward);
		else if(L"transl" == wsToken) return new CElementConnection(TypeElement::transl);
		else if(L"transr" == wsToken) return new CElementConnection(TypeElement::transr);
		else if(L"def" == wsToken) return new CElementConnection(TypeElement::def);
		else if(L"=" == wsToken) return new CElementConnection(TypeElement::equals);
		else if(L"<>" == wsToken) return new CElementConnection(TypeElement::notequals);
		else if(L"<" == wsToken) return new CElementConnection(TypeElement::learrow);
		else if(L"<=" == wsToken) return new CElementConnection(TypeElement::learrowequals);
		else if(L"leslant" == wsToken) return new CElementConnection(TypeElement::leslant);
		else if(L">" == wsToken) return new CElementConnection(TypeElement::riarrow);
		else if(L">=" == wsToken) return new CElementConnection(TypeElement::riarrowequals);
		else if(L"geslant" == wsToken) return new CElementConnection(TypeElement::geslant);
		else if(L"<<" == wsToken) return new CElementConnection(TypeElement::dllearrow);
		else if(L">>" == wsToken) return new CElementConnection(TypeElement::dlriarrow);
		else if(L"prec" == wsToken) return new CElementConnection(TypeElement::prec);
		else if(L"succ" == wsToken) return new CElementConnection(TypeElement::succ);
		else if(L"preccurlyeq" == wsToken) return new CElementConnection(TypeElement::preccurlyeq);
		else if(L"succcurlyeq" == wsToken) return new CElementConnection(TypeElement::succcurlyeq);
		else if(L"precsim" == wsToken) return new CElementConnection(TypeElement::precsim);
		else if(L"succsim" == wsToken) return new CElementConnection(TypeElement::succsim);
		else if(L"nprec" == wsToken) return new CElementConnection(TypeElement::nprec);
		else if(L"nsucc" == wsToken) return new CElementConnection(TypeElement::nsucc);
		else if(L"dlarrow" == wsToken) return new CElementConnection(TypeElement::dlarrow);
		else if(L"dlrarrow" == wsToken) return new CElementConnection(TypeElement::dlrarrow);
		else if(L"drarrow" == wsToken) return new CElementConnection(TypeElement::drarrow);
		else if(L"abs" == wsToken) return new CElementFunction(TypeElement::abs);
		else if(L"fact" == wsToken) return new CElementFunction(TypeElement::fact);
		else if(L"sqrt" == wsToken) return new CElementFunction(TypeElement::sqrt);
		else if(L"sin" == wsToken) return new CElementFunction(TypeElement::sin);
		else if(L"cos" == wsToken) return new CElementFunction(TypeElement::cos);
		else if(L"tan" == wsToken) return new CElementFunction(TypeElement::tan);
		else if(L"cot" == wsToken) return new CElementFunction(TypeElement::cot);
		else if(L"sinh" == wsToken) return new CElementFunction(TypeElement::sinh);
		else if(L"cosh" == wsToken) return new CElementFunction(TypeElement::cosh);
		else if(L"tanh" == wsToken) return new CElementFunction(TypeElement::tanh);
		else if(L"coth" == wsToken) return new CElementFunction(TypeElement::coth);
		else if(L"arcsin" == wsToken) return new CElementFunction(TypeElement::arcsin);
		else if(L"arccos" == wsToken) return new CElementFunction(TypeElement::arccos);
		else if(L"arctan" == wsToken) return new CElementFunction(TypeElement::arctan);
		else if(L"arccot" == wsToken) return new CElementFunction(TypeElement::arccot);
		else if(L"arsinh" == wsToken) return new CElementFunction(TypeElement::arsinh);
		else if(L"arcosh" == wsToken) return new CElementFunction(TypeElement::arcosh);
		else if(L"artanh" == wsToken) return new CElementFunction(TypeElement::artanh);
		else if(L"arcoth" == wsToken) return new CElementFunction(TypeElement::arcoth);
		else if(L"ln" == wsToken) return new CElementFunction(TypeElement::ln);
		else if(L"exp" == wsToken) return new CElementFunction(TypeElement::exp);
		else if(L"log" == wsToken) return new CElementFunction(TypeElement::log);
		else return nullptr;
	}
	void CElement::SetAttribute(const std::vector<CAttribute *> arAttr)
	{
		arElementAttributes = arAttr;
	}
	void CElement::SetIndex(CIndex *pIndex)
	{
		pElementIndex = pIndex;
	}
	TypeElement CElement::GetBaseType()
	{
		return enBaseType;
	}
	void CElement::SetBaseType(const TypeElement &enType)
	{
		enBaseType = enType;
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
		SetBaseType(TypeElement::BinOperator);
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
//		нужно сделать функцию для чтения без скобок
//		if(enTypeBinOp == TypeElement::frac)
//		{

//		}
		CElement* pTempElement = CParseStarMathString::ParsElement(itStart,itEnd);
		if(IsBinOperatorLowPrior() && CParseStarMathString::CheckingTheNextElement(itStart,itEnd,IsBinOperatorHightPrior))
		{
			CElement* pBinOp = CParseStarMathString::ParsElement(itStart,itEnd);
			CParseStarMathString::AddLeftArgument(pTempElement,pBinOp);
			SetRightArg(pBinOp);
		}
		else
			SetRightArg(pTempElement);
	}
	void CElementBinOperator::SetTypeBinOP(const TypeElement &enType)
	{
		enTypeBinOp = enType;
	}
	bool CElementBinOperator::IsBinOperatorHightPrior(const std::wstring& wsToken)
	{
		if(L"cdot" == wsToken) return true;
		else if(L"times" == wsToken) return true;
		else if(L"over" == wsToken) return true;
//		else if(L"frac" == wsToken) return true;
		else if(L"div" == wsToken) return true;
		else if(L"multipl" == wsToken) return true;
		else if(L"division" == wsToken) return true;
		else if(L"odot" == wsToken) return true;
		else if(L"otimes" == wsToken) return true;
		else if(L"odivide" == wsToken) return true;
		else if(L"circ" == wsToken) return true;
		else if(L"wideslash" == wsToken) return true;
		else if(L"widebslash" == wsToken) return true;
	}
	bool CElementBinOperator::IsBinOperatorLowPrior()
	{
		switch (enTypeBinOp) {
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
		else if(L"rdbracket" == wsToken) return true;
		else if(L"rbrace" == wsToken) return true;
		else if(L"rangle" == wsToken) return true;
		else if(L"rceil" == wsToken) return true;
		else if(L"rfloor" == wsToken) return true;
		else if(L"rline" == wsToken) return true;
		else if(L"rdline" == wsToken) return true;
		else if(L"right" == wsToken) return true;
		else return false;
	}
	// правка if
	void CElementBracket::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		while(!CParseStarMathString::CheckingTheNextElement(itStart,itEnd,IsBracketClose))
		{
			CElement* pTempElement = CParseStarMathString::ParsElement(itStart,itEnd);
			if(!arBrecketValue.empty() && (pTempElement->GetBaseType() == TypeElement::BinOperator || pTempElement->GetBaseType() == TypeElement::SetOperations) )
			{
				CParseStarMathString::AddLeftArgument(arBrecketValue.back(),pTempElement);
				arBrecketValue.pop_back();
			}
			arBrecketValue.push_back(pTempElement);
		}
		//доработать()
		if(!CParseStarMathString::MoveToNextElement(itStart,itEnd));
	}
//class methods CElementSpecialSymbol
	CElementSpecialSymbol::CElementSpecialSymbol(const TypeElement &enType)
	{
		enTypeSpecial = enType;
	}
	CElementSpecialSymbol::~CElementSpecialSymbol()
	{}
	void CElementSpecialSymbol::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
	}
//class methods CElementSetOperations
	CElementSetOperations::CElementSetOperations(const TypeElement &enType)
	{
		enTypeSet = enType;
		SetBaseType(TypeElement::SetOperations);
	}
	CElementSetOperations::~CElementSetOperations()
	{
		delete pLeftArgument;
		delete pRightArgument;
	}
	void CElementSetOperations::SetLeftArg(CElement *pElement)
	{
		pLeftArgument = pElement;
	}
	CElement* CElementSetOperations::GetLeftArg()
	{
		return pLeftArgument;
	}
	void CElementSetOperations::SetRightArg(CElement *pElement)
	{
		pRightArgument = pElement;
	}
	CElement* CElementSetOperations::GetRightArg()
	{
		return pRightArgument;
	}
	void CElementSetOperations::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		CElement* pTempElement = CParseStarMathString::ParsElement(itStart,itEnd);
		if(CParseStarMathString::CheckingTheNextElement(itStart,itEnd,CElementBinOperator::IsBinOperatorHightPrior))
		{
			CElement* pBinOpElement = CParseStarMathString::ParsElement(itStart,itEnd);
			CParseStarMathString::AddLeftArgument(pTempElement,pBinOpElement);
			SetRightArg(pBinOpElement);
		}
		else SetRightArg(pTempElement);
	}
	bool CElementSetOperations::IsSetOperation(const std::wstring &wsToken)
	{
		if(L"union" == wsToken) return true;
		else return false;
	}
//class methods CElementConnection
	CElementConnection::CElementConnection(const TypeElement& enType)
	{
		enTypeCon = enType;
		SetBaseType(TypeElement::Connection);
	}
	CElementConnection::~CElementConnection()
	{
		delete pLeftArgument;
		delete pRightArgument;
	}
	void CElementConnection::SetRightArg(CElement *pElement)
	{
		pRightArgument = pElement;
	}
	CElement* CElementConnection::GetRightArg()
	{
		return pRightArgument;
	}
	void CElementConnection::SetLeftArg(CElement *pElement)
	{
		pLeftArgument = pElement;
	}
	CElement* CElementConnection::GetLeftArg()
	{
		return pLeftArgument;
	}
	void CElementConnection::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		CElement* pTempElement = CParseStarMathString::ParsElement(itStart,itEnd);
		if(CParseStarMathString::CheckingTheNextElement(itStart,itEnd,CElementBinOperator::IsBinOperatorHightPrior))
		{
			CElement* pBinOp = CParseStarMathString::ParsElement(itStart,itEnd);
			CParseStarMathString::AddLeftArgument(pTempElement,pBinOp);
			SetRightArg(pBinOp);
		}
		else SetRightArg(pTempElement);
	}
	bool CElementConnection::IsConnection(const std::wstring& wsToken)
	{
		if(L"def" == wsToken) return true;
		else return false;
	}
//class methods CIndex
	CIndex::CIndex(const TypeElement& enType)
	{
		enTypeIndex = enType;
	}
	CIndex::~CIndex()
	{
		delete pValueIndex;
	}
	void CIndex::SetValueIndex(CElement *pElement)
	{
		pValueIndex = pElement;
	}
	CElement* CIndex::GetValueIndex()
	{
		return pValueIndex;
	}
	bool CIndex::IsIndex(const std::wstring &wsCheckToken)
	{
		if(L"^" == wsCheckToken) return true;
		else if(L"_" == wsCheckToken) return true;
		else if(L"lsup" == wsCheckToken) return true;
		else if(L"lsub" == wsCheckToken) return true;
		else if(L"csup" == wsCheckToken) return true;
		else if(L"csub" == wsCheckToken) return true;
		else return false;
	}
	CIndex* CIndex::CreateIndex(const std::wstring &wsToken)
	{
		if(L"^" == wsToken) return new CIndex(TypeElement::upper);
		else if(L"_" == wsToken) return new CIndex(TypeElement::lower);
		else if(L"lsup" == wsToken) return new CIndex(TypeElement::lsup);
		else if(L"lsub" == wsToken) return new CIndex(TypeElement::lsub);
		else if(L"csup" == wsToken) return new CIndex(TypeElement::csup);
		else if(L"csub" == wsToken) return new CIndex(TypeElement::csub);
		else return nullptr;
	}
//class methods CElementFunction
	CElementFunction::CElementFunction(const TypeElement &enType)
	{
		enTypeFunction = enType;
		SetBaseType(TypeElement::Function);
	}
	CElementFunction::~CElementFunction()
	{
		delete pValue;
	}
	void CElementFunction::SetValueFunction(CElement *pElement)
	{
		pValue = pElement;
	}
	CElement* CElementFunction::GetValueFunction()
	{
		return pValue;
	}

	void CElementFunction::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		SetValueFunction(CParseStarMathString::ParsElement(itStart,itEnd));
	}
//class methods CElementOperation
	CElementOperator::CElementOperator(const TypeElement &enType)
	{
		enTypeOperator = enType;
		SetBaseType(TypeElement::Operation);
	}
	CElementOperator::~CElementOperator()
	{
		delete pValueOperator;
		delete pValueFrom;
		delete pValueTo;
	}
	void CElementOperator::SetValueOperator(CElement *pElement)
	{
		pValueOperator = pElement;
	}
	CElement* CElementOperator::GetValueOperator()
	{
		return pValueOperator;
	}
	void CElementOperator::SetFromValue(CElement *pElement)
	{
		pValueFrom = pElement;
	}
	CElement* CElementOperator::GetFromValue()
	{
		return pValueFrom;
	}
	void CElementOperator::SetToValue(CElement *pElement)
	{
		pValueTo = pElement;
	}
	CElement* CElementOperator::GetToValue()
	{
		return pValueTo;
	}
	void CElementOperator::Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		do
		{
			CElement* pTempElement = CParseStarMathString::ParsElement(itStart,itEnd);

		}while(CParseStarMathString::CheckingTheNextElement(itStart,itEnd,CElementBinOperator::IsBinOperatorHightPrior) || CParseStarMathString::CheckingTheNextElement(itStart,itEnd,CElementSetOperations::IsSetOperation) || CParseStarMathString::CheckingTheNextElement(itStart,itEnd,CElementConnection::IsConnection));
	}
}


