#include "cstarmathpars.h"
namespace StarMath
{
//Class methods CStarMathPars
	CStarMathPars::CStarMathPars()
	{
	}

	 CStarMathPars::~CStarMathPars()
	{
		for(CElement* ReleaseObject: m_arParsLine)
		{
			delete ReleaseObject;
		}
	}

	 void CStarMathPars::PrintAr()
	 {
		 for(int i = 0;i<m_arParsLine.size();i++)
		 {
			m_arParsLine[i]->GetType();
		 }
	 }

	void CStarMathPars::Pars(std::wstring& wsStarMathLine)
	{
		std::wstring::iterator itFirst = wsStarMathLine.begin(), itEnd = wsStarMathLine.end();
		while(itFirst != itEnd)
		{
		m_arParsLine.push_back(ParsElement(itFirst,itEnd,m_arParsLine));
		//std::wcout << GetElement(itFirst,itEnd)<<std::endl;
		}

		//PrintAr();
	}
	std::wstring CStarMathPars::GetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd)
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
			else if(!m_wsElement.empty() && (*itFirst == L'{' || *itFirst == L'}' || *itFirst == L'+' || *itFirst == L'-' || *itFirst == L'/' || *itFirst == L'*' || L'^' == *itFirst || L'_' == *itFirst || (iswalpha(*itFirst) && iswdigit(m_wsElement.back())) || (iswdigit(*itFirst) && iswalpha(m_wsElement.back()))))
			{
				return m_wsElement;
			}
			else if((*itFirst == L'{' || *itFirst == L'}' || *itFirst == L'+' || *itFirst == L'-' || *itFirst == L'/' || *itFirst == L'*' || L'^' == *itFirst || L'_' == *itFirst ) && m_wsElement.empty() )
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
	CElement* CStarMathPars::ParsElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd,std::vector<CElement*>& arParsLine)
	{
		CAttribute* m_oAttribute = new CAttribute;
		std::wstring wsOneElement = GetElement(itFirst,itEnd);
		while((L"color" == wsOneElement && CheckingTheNextElement(itFirst,itEnd,CheckColorAttribute))|| CheckPropertiesAttribute(wsOneElement) || CheckTopAttribute(wsOneElement))
		{
			if(wsOneElement == L"color") wsOneElement = GetElement(itFirst,itEnd);
			m_oAttribute->SetTypeAtt(wsOneElement);
			wsOneElement = GetElement(itFirst,itEnd);
		}

		if(CheckDigit(wsOneElement))
		{
			CNumber* m_oCNumber = new CNumber(wsOneElement);
			m_oCNumber->SetAttribute(m_oAttribute);
			return m_oCNumber;
		}
		if(CheckSpecialCharacter(wsOneElement)) return new CSpecialCharacters(wsOneElement);
		else if(CheckBinOperator(wsOneElement) || CheckPlusOrMinus(wsOneElement) || CheckBinOperatorLowPriority(wsOneElement))
		{
			if(!arParsLine.empty())
			{
				CBinaryOperator* m_pBinOp = new CBinaryOperator(wsOneElement);
				CElement* m_oTempElement{ParsElement(itFirst,itEnd,arParsLine)};
				m_pBinOp->SetLeftArg(arParsLine.back());
				arParsLine.pop_back();
				if(((CheckPlusOrMinus(wsOneElement) || CheckBinOperatorLowPriority(wsOneElement)) && (CheckingTheNextElement(itFirst, itEnd,CheckBinOperator)||CheckingTheNextElement(itFirst, itEnd, CheckIndex))) || (CheckBinOperator(wsOneElement) && CheckingTheNextElement(itFirst, itEnd,CheckIndex)))
				{
					arParsLine.push_back(m_oTempElement);
					m_pBinOp->SetRightArg(ParsElement(itFirst,itEnd,arParsLine));
				}
				else m_pBinOp->SetRightArg(m_oTempElement);
				m_pBinOp->SetAttribute(m_oAttribute);
				return m_pBinOp;
			}
			return NULL;
		}
		else if(CheckBracketOpen(wsOneElement) || CheckScalable_NotScalableBracketLeft(wsOneElement) || (L"left" == wsOneElement && CheckingTheNextElement(itFirst,itEnd,CheckScalable_NotScalableBracketLeft)))
		{
			CBracket* m_oBracket = new CBracket;
			std::wstring wsTypeBracket;
			std::vector<CElement*> arValueBrecket;
			if(L"left" == wsOneElement)
			{
				m_oBracket->SetScalable();
				wsOneElement = GetElement(itFirst,itEnd);
				wsTypeBracket = wsOneElement;
			}
			else wsTypeBracket = wsOneElement;
			while(!CheckingTheNextElement(itFirst,itEnd,CheckBracketClose) && !CheckingTheNextElement(itFirst,itEnd,CheckScalable_NotScalableBracketRight))
			{
				arValueBrecket.push_back(ParsElement(itFirst,itEnd,arValueBrecket));
			}
			wsOneElement = GetElement(itFirst,itEnd);
			std::wcout << wsOneElement << std::endl;
			if(L"right" == wsOneElement) wsOneElement = GetElement(itFirst,itEnd);
			m_oBracket->SetBracketVal(arValueBrecket);
			m_oBracket->SetTypeBracket(wsTypeBracket);
			m_oBracket ->SetAttribute(m_oAttribute);
			return m_oBracket;
		}
		else if(CheckOperator(wsOneElement))
		{
			COperator* oTempOp = new COperator(wsOneElement);
			std::vector<CElement*> arValueBrecket;
			std::wstring::iterator itSavePos;
			do
			{
				itSavePos = itFirst;
				wsOneElement = GetElement(itFirst,itEnd);
				if( wsOneElement == L"from")
					oTempOp->SetFrom(ParsElement(itFirst,itEnd,arValueBrecket));
				else if(wsOneElement == L"to")
					oTempOp->SetTo(ParsElement(itFirst,itEnd,arValueBrecket));
				else
					break;
			}while(true);
			oTempOp->SetValueOp(ParsElement(itSavePos,itEnd,arValueBrecket));
			oTempOp->SetAttribute(m_oAttribute);
			return oTempOp;
		}
	}

	bool CStarMathPars::CheckDigit(const std::wstring &wsCheckToken)
	{
		for(char Check: wsCheckToken)
		{
			if(!isdigit(Check)) return false;
		}
		return true;
	}
	bool CStarMathPars::CheckScalable_NotScalableBracketLeft(const std::wstring &wsCheckToken)
	{
		return(L"ldbracket" == wsCheckToken || L"lbrace" == wsCheckToken || L"langle" == wsCheckToken || L"lceil" == wsCheckToken || L"lfloor" == wsCheckToken || L"lline" == wsCheckToken || L"ldline" == wsCheckToken);
	}
	bool CStarMathPars::CheckScalable_NotScalableBracketRight(const std::wstring &wsCheckToken)
	{
		return(L"rdbracket" == wsCheckToken || L"rbrace" == wsCheckToken || L"rangle" == wsCheckToken || L"rceil" == wsCheckToken || L"rfloor" == wsCheckToken || L"rline" == wsCheckToken || L"rdline" == wsCheckToken || L"right" == wsCheckToken);
	}
////////////////Requires improvement
	/*bool CStarMathPars::CheckUnarSign(std::wstring &wsCheckToken,CUnarySign& m_oUnarSign)
	{
		std::wstring wsTempValueUnarSign;
		bool UnarSign = false;
		for(int i = 0;i<2;i++)
		{
			if(wsCheckToken[i] == '+' || wsCheckToken[i] == '-')
			{
				wsTempValueUnarSign.push_back(wsCheckToken[i]);
				UnarSign = true;
			}
		}
		if(!wsTempValueUnarSign.empty() && UnarSign)
		{
			m_arParsLine.push_back(m_oUnarSign.GetUnarSign(wsTempValueUnarSign));
			wsCheckToken =  wsCheckToken.substr(wsTempValueUnarSign.size());
		}
		return UnarSign;
	}*/
	bool CStarMathPars::CheckingTheNextElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd, bool (&func)(const std::wstring&))
	{
		std::wstring::iterator itTempVal = itFirst;
		bool bResult = func(GetElement(itFirst,itEnd));
		itFirst = itTempVal;
		return bResult;
	}
	bool CStarMathPars::CheckBinOperator(const std::wstring &wsCheckToken)
	{
		return(L"*" ==wsCheckToken || L"/" == wsCheckToken || wsCheckToken == L"over" || wsCheckToken == L"cdot" || wsCheckToken == L"times" || wsCheckToken == L"frac" || wsCheckToken == L"div"  || wsCheckToken == L"odot" || wsCheckToken == L"otimes" || wsCheckToken == L"odivide"  || wsCheckToken == L"wideslash" || wsCheckToken == L"widebslash");
	}
	bool CStarMathPars::CheckBinOperatorLowPriority(const std::wstring &wsCheckToken)
	{
		return (wsCheckToken == L"oplus" || wsCheckToken == L"ominus"|| wsCheckToken == L"circ");
	}
	bool CStarMathPars::CheckPlusOrMinus(const std::wstring &wsCheckToken)
	{
		switch (wsCheckToken[0])
		{
		case L'+':
			return true;
		case L'-':
			return true;
		default:
			return false;
		}
	}
	bool CStarMathPars::CheckOperator(const std::wstring &wsCheckToken)
	{
		return(wsCheckToken == L"lim" || wsCheckToken == L"sum");
	}
	bool CStarMathPars::CheckIndex(const std::wstring &wsCheckToken)
	{
		return (wsCheckToken == L"from" || wsCheckToken == L"to" || wsCheckToken[0] == L'_' || wsCheckToken[0] == L'^' );
	}
	bool CStarMathPars::CheckBracketOpen(const std::wstring& wsCheckToken)
	{
		switch (wsCheckToken[0]) {
		case '{':
			return true;
		case '(':
			return true;
		case '[':
			return true;
		case '}':
			return true;
		case ')':
			return true;
		case ']':
			return true;
		default:
			return false;
		}
	}
	bool CStarMathPars::CheckBracketClose(const std::wstring& wsCheckToken)
	{
		switch (wsCheckToken[0]) {;
		case '}':
			return true;
		case ')':
			return true;
		case ']':
			return true;
		default:
			return false;
		}
	}
	bool CStarMathPars::CheckTopAttribute(const std::wstring& wsCheckToken)
	{
		return(L"acute" == wsCheckToken || L"breve" == wsCheckToken || L"dot" == wsCheckToken || L"dddot" == wsCheckToken || L"vec" == wsCheckToken || L"tilde" == wsCheckToken || L"check" == wsCheckToken || L"grave" == wsCheckToken || L"circle" == wsCheckToken || L"ddot" == wsCheckToken || L"bar" == wsCheckToken || L"harpoon" == wsCheckToken || L"hat" == wsCheckToken || L"widevec" == wsCheckToken || L"widetilde" == wsCheckToken || L"overline" == wsCheckToken || L"overstrike" == wsCheckToken || L"wideharpoon" == wsCheckToken || L"widehat" == wsCheckToken || L"underline" == wsCheckToken);
	}
	bool CStarMathPars::CheckPropertiesAttribute(const std::wstring& wsCheckToken)
	{
		return(L"ital" == wsCheckToken || L"bold" == wsCheckToken || L"phantom" == wsCheckToken);
	}
	bool CStarMathPars::CheckColorAttribute(const std::wstring &wsCheckToken)
	{
		return (L"violet" == wsCheckToken || L"black" == wsCheckToken || L"green" == wsCheckToken || L"aqua" == wsCheckToken || L"yellow" == wsCheckToken || L"lime" == wsCheckToken || L"navy" == wsCheckToken || L"purple" == wsCheckToken || L"teal" == wsCheckToken || L"blue" == wsCheckToken || L"red" == wsCheckToken || L"fuchsia" == wsCheckToken || L"gray" == wsCheckToken || L"maroon" == wsCheckToken || L"olive" == wsCheckToken || L"silver" == wsCheckToken || L"coral" == wsCheckToken || L"midnightblue" == wsCheckToken || L"crimson" == wsCheckToken || L"violet" == wsCheckToken);
	}
	bool CStarMathPars::CheckSpecialCharacter(const std::wstring &wsCheckToken)
	{
		return (L"mline" == wsCheckToken || L"grid" == wsCheckToken);
	}
	std::vector<CElement*> CStarMathPars::GetVector()
	{
		return m_arParsLine;
	}
//Class methods CNumber
	CNumber::CNumber()
	{
	}
	CNumber::CNumber(const std::wstring &wsValue)
	{
		this->m_wsValueNumber = wsValue;
	}
	CNumber::~CNumber()
	{
	}

	std::wstring CNumber::GetValue()
	{
		return m_wsValueNumber;
	}
	TypeElement CNumber::GetType()
	{
		return Number;
	}
//Class methods CUnarySign
	CUnarySign* CUnarySign::GetUnarSign(const std::wstring& wsUnarToken)
	{
		CUnarySign* oSign = new CUnarySign;
		oSign->m_wsUnar = wsUnarToken;
		return oSign;
	}

	std::wstring CUnarySign::GetValue()
	{
		return m_wsUnar;
	}
	TypeElement CUnarySign::GetType()
	{
		return m_enUnarType;
	}
	CUnarySign::CUnarySign()
	{}
	CUnarySign::~CUnarySign()
	{}
//Class methods CBinaryOperator
	CBinaryOperator::CBinaryOperator()
	{}
	CBinaryOperator::CBinaryOperator(const std::wstring& wsToken)
	{
		if(wsToken == L"+")
		{
			enTypeBinOp = plus;
		}
		else if(wsToken == L"-")
		{
			enTypeBinOp = minus;
		}
		else if(wsToken == L"*")
		{
			enTypeBinOp = multipl;
		}
		else if(wsToken == L"/")
		{
			enTypeBinOp = division;
		}
		else if(wsToken == L"over")
		{
			enTypeBinOp = over;
		}
		else if(wsToken == L"cdot")
		{
			enTypeBinOp = cdot;
		}
		else if(wsToken == L"times")
		{
			enTypeBinOp = times;
		}
		else if(wsToken == L"frac")
		{
			enTypeBinOp = frac;
		}
		else if(wsToken == L"div")
		{
			enTypeBinOp = div;
		}
		else if(wsToken == L"oplus")
		{
			enTypeBinOp = oplus;
		}
		else if(wsToken == L"ominus")
		{
			enTypeBinOp = ominus;
		}
		else if(wsToken == L"odot")
		{
			enTypeBinOp = odot;
		}
		else if(wsToken == L"otimes")
		{
			enTypeBinOp = otimes;
		}
		else if(wsToken == L"odivide")
		{
			enTypeBinOp = odivide;
		}
		else if(wsToken == L"circ")
		{
			enTypeBinOp = circ;
		}
		else if(wsToken == L"wideslash")
		{
			enTypeBinOp = wideslash;
		}
		else if(wsToken == L"widebslash")
		{
			enTypeBinOp = widebslash;
		}
		arLeftArg = nullptr;
		arRightArg = nullptr;
	}
	CBinaryOperator::~CBinaryOperator()
	{
		delete arLeftArg;
		delete arRightArg;
	}
	TypeElement CBinaryOperator::GetType()
	{
		return BinOperator;
	}
	std::wstring CBinaryOperator::GetValue()
	{
		return {};
	}
	TypeBinOperator CBinaryOperator::GetTypeBin()
	{
		return enTypeBinOp;
	}
	void CBinaryOperator::SetLeftArg( CElement *oLeftArg)
	{
		this->arLeftArg = oLeftArg;
	}
	void CBinaryOperator::SetRightArg(CElement *oRightArg)
	{
		this->arRightArg  = oRightArg;
	}
	void CBinaryOperator::SetTypeBin(const TypeBinOperator &enType)
	{
		this->enTypeBinOp = enType;
	}
	CElement* CBinaryOperator::GetLeftArg()
	{
		return arLeftArg;
	}
	CElement* CBinaryOperator::GetRightArg()
	{
		return arRightArg;
	}
	TypeElement CBinaryOperator::GetTypeRight()
	{
		return arRightArg->GetType();
	}
//Class methods COperator
	COperator::COperator(): oFromValue(NULL),oToValue(NULL),oValueOp(NULL)
	{}
	COperator::COperator(const std::wstring& wsToken): oFromValue(NULL), oToValue(NULL), oValueOp(NULL)
	{
		if(wsToken== L"lim")
		{
			 enTypeOp = lim;
		}
		else if(wsToken == L"sum")
		{
			 enTypeOp = sum;
		}
	}
	COperator::~COperator()
	{
		delete oFromValue;
		delete oToValue;
		delete oValueOp;
	}
	TypeElement COperator::GetType()
	{
		/*std::wcout << oFromValue->GetType() <<std::endl;
		std::wcout << oToValue->GetType() <<std::endl;
		std::wcout << oValueOp->GetType() <<std::endl;*/
		return Operator;
	}
	std::wstring COperator::GetValue()
	{
		return {};
	}
	TypeOperator COperator::GetTypeOp()
	{
		return enTypeOp;
	}
	void COperator::SetFrom(CElement *oFrom)
	{
		oFromValue = oFrom;
	}
	void COperator::SetTo(CElement *oTo)
	{
		oToValue = oTo;
	}
	void COperator::SetValueOp(CElement *oValue)
	{
		oValueOp = oValue;
	}
	void COperator::SetTypeOp(const TypeOperator& enType)
	{
		enTypeOp = enType;
	}
//Class methods CBracket
	CBracket::CBracket()
	{}
	CBracket::CBracket(const std::vector<CElement*>& arValue,const std::wstring& wsCheckToken)
	{
		arBrecketVal = arValue;
		if (L"{" == wsCheckToken) enTypeBracket =brace;
		else if (L"(" == wsCheckToken) enTypeBracket = round;
		else if (L"[" == wsCheckToken) enTypeBracket = square;
		else if(wsCheckToken == L"ldbracket")
		{
			enTypeBracket = ldbracket;
		}
		else if(wsCheckToken == L"lbrace")
		{
			enTypeBracket = lbrace;
		}
		else if(wsCheckToken == L"langle")
		{
			enTypeBracket = langle;
		}
		else if(wsCheckToken == L"lceil")
		{
			enTypeBracket = lceil;
		}
		else if(wsCheckToken == L"lfloor")
		{
			enTypeBracket = lfloor;
		}
		else if(wsCheckToken == L"lline")
		{
			enTypeBracket = lline;
		}
		else if(wsCheckToken == L"ldline")
		{
			enTypeBracket = ldline;
		}
	}
	CBracket::~CBracket()
	{
		for(CElement* ReleaseObject: arBrecketVal)
		{
			delete ReleaseObject;
		}
	}
	TypeElement CBracket::GetType()
	{
		for(CElement* enTemp: arBrecketVal)
		{
			std::wcout<< enTemp->GetType() << std::endl;
		}
		return Bracket;
	}
	std::wstring CBracket::GetValue()
	{
		return {};
	}
	void CBracket::SetScalable()
	{
		bScalable = true;
	}
	void CBracket::SetBracketVal(const std::vector<CElement *> &arBrecketValue)
	{
		arBrecketVal = arBrecketValue;
	}
	void CBracket::SetTypeBracket(const std::wstring& wsCheckToken)
	{
		if (L"{" == wsCheckToken) enTypeBracket =brace;
		else if (L"(" == wsCheckToken) enTypeBracket = round;
		else if (L"[" == wsCheckToken) enTypeBracket = square;
		else if(wsCheckToken == L"ldbracket")
		{
			enTypeBracket = ldbracket;
		}
		else if(wsCheckToken == L"lbrace")
		{
			enTypeBracket = lbrace;
		}
		else if(wsCheckToken == L"langle")
		{
			enTypeBracket = langle;
		}
		else if(wsCheckToken == L"lceil")
		{
			enTypeBracket = lceil;
		}
		else if(wsCheckToken == L"lfloor")
		{
			enTypeBracket = lfloor;
		}
		else if(wsCheckToken == L"lline")
		{
			enTypeBracket = lline;
		}
		else if(wsCheckToken == L"ldline")
		{
			enTypeBracket = ldline;
		}
	}
//Class methods CAttribute
	CAttribute::CAttribute()
	{
	}
	CAttribute::CAttribute(const TypeAttributeTop &enType)
	{
		enTypeTop = enType;
	}
	CAttribute::~CAttribute()
	{
	}
	void CAttribute::SetTypeAtt(const std::wstring& wsCheckToken)
	{
		if(wsCheckToken == L"phantom")
		{
			bPhantom = true;
		}
		else if(wsCheckToken == L"bold")
		{
			bBold = true;
		}
		else if(wsCheckToken == L"ital")
		{
			bItal = true;
		}
		else if(wsCheckToken == L"black")
		{
			enTypeColor = black;
		}
		else if(wsCheckToken == L"green")
		{
			enTypeColor = green;
		}
		else if(wsCheckToken == L"aqua")
		{
			enTypeColor = aqua;
		}
		else if(wsCheckToken == L"yellow")
		{
			enTypeColor = yellow;
		}
		else if(wsCheckToken == L"lime")
		{
			enTypeColor = lime;
		}
		else if(wsCheckToken == L"navy")
		{
			enTypeColor = navy;
		}
		else if(wsCheckToken == L"purple")
		{
			enTypeColor = purple;
		}
		else if(wsCheckToken == L"teal")
		{
			enTypeColor = teal;
		}
		else if(wsCheckToken == L"blue")
		{
			enTypeColor = blue;
		}
		else if(wsCheckToken == L"red")
		{
			enTypeColor = red;
		}
		else if(wsCheckToken == L"fuchsia")
		{
			enTypeColor = fuchsia;
		}
		else if(wsCheckToken == L"gray")
		{
			enTypeColor = gray;
		}
		else if(wsCheckToken == L"maroon")
		{
			enTypeColor = maroon;
		}
		else if(wsCheckToken == L"olive")
		{
			enTypeColor = olive;
		}
		else if(wsCheckToken == L"silver")
		{
			enTypeColor = silver;
		}
		else if(wsCheckToken == L"coral")
		{
			enTypeColor = coral;
		}
		else if(wsCheckToken == L"midnightblue")
		{
			enTypeColor = midnightblue;
		}
		else if(wsCheckToken == L"crimson")
		{
			enTypeColor = crimson;
		}
		else if(wsCheckToken == L"violet")
		{
			enTypeColor = violet;
		}
		else if(wsCheckToken == L"acute")
		{
			enTypeTop = acute;
		}
		else if(wsCheckToken == L"breve")
		{
			enTypeTop = breve;
		}
		else if(wsCheckToken == L"dot")
		{
			enTypeTop = dot;
		}
		else if(wsCheckToken == L"dddot")
		{
			enTypeTop = dddot;
		}
		else if(wsCheckToken == L"vec")
		{
			enTypeTop = vec;
		}
		else if(wsCheckToken == L"tilde")
		{
			enTypeTop = tilde;
		}
		else if(wsCheckToken == L"check")
		{
			enTypeTop = check;
		}
		else if(wsCheckToken == L"grave")
		{
			enTypeTop = grave;
		}
		else if(wsCheckToken == L"circle")
		{
			enTypeTop = circle;
		}
		else if(wsCheckToken == L"ddot")
		{
			enTypeTop = ddot;
		}
		else if(wsCheckToken == L"bar")
		{
			enTypeTop = bar;
		}
		else if(wsCheckToken == L"harpoon")
		{
			enTypeTop = harpoon;
		}
		else if(wsCheckToken == L"hat")
		{
			enTypeTop = hat;
		}
		else if(wsCheckToken == L"widevec")
		{
			enTypeTop = widevec;
		}
		else if(wsCheckToken == L"widetilde")
		{
			enTypeTop = widetilde;
		}
		else if(wsCheckToken == L"overline")
		{
			enTypeTop = overline;
		}
		else if(wsCheckToken == L"overstrike")
		{
			enTypeTop = overstrike;
		}
		else if(wsCheckToken == L"wideharpoon")
		{
			enTypeTop = wideharpoon;
		}
		else if(wsCheckToken == L"widehat")
		{
			enTypeTop = widehat;
		}
		else if(wsCheckToken == L"underline")
		{
			enTypeTop = underline;
		}
	}
	TypeAttributeTop CAttribute::GetTypeAtt()
	{
		return enTypeTop;
	}
//Class methods CElement
	CElement::~CElement()
	{
	}
	void CElement::SetAttribute(CAttribute* m_oCAttribute)
	{
		oCAttribute = m_oCAttribute;
	}
	TypeAttributeTop CElement::GetTypeAttribute()
	{
		return oCAttribute->GetTypeAtt();
	}
//Class methods CSpecial
	CSpecialCharacters::CSpecialCharacters(const std::wstring& wsToken)
	{
		if(L"mline" == wsToken) enTypeSpecial = mline;
		else if(L"grid" == wsToken) enTypeSpecial = grid;
	}
	CSpecialCharacters::~CSpecialCharacters()
	{}
	std::wstring CSpecialCharacters::GetValue()
	{
		return {};
	}
	TypeElement CSpecialCharacters::GetType()
	{
		return SpecialCharacter;
	}
}

