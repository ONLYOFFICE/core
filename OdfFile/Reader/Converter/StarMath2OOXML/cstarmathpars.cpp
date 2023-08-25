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
		PrintAr();
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
			else if(!m_wsElement.empty() && (*itFirst == L'{' || *itFirst == L'}' || *itFirst == L'+' || *itFirst == L'-' || *itFirst == L'/' || *itFirst == L'*' || (iswalpha(*itFirst) && iswdigit(m_wsElement.back())) || (iswdigit(*itFirst) && iswalpha(m_wsElement.back()))))
			{
				return m_wsElement;
			}
			else if((*itFirst == L'{' || *itFirst == L'}' || *itFirst == L'+' || *itFirst == L'-' || *itFirst == L'/' || *itFirst == L'*' ) && m_wsElement.empty() )
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
		TypeBinOperator enTypeBinOp;
		TypeBracket enTypeBracket;
		TypeOperator enTypeOp;
		TypeAttribute enTypeAtt;
		std::wstring wsOneElement = GetElement(itFirst,itEnd);
		if(CheckDigit(wsOneElement))
		{
			return new CNumber(wsOneElement);
		}
		else if(wsOneElement.size() == 1 && CheckOneElementBinOperator(wsOneElement[0],enTypeBinOp))
		{
			return new CBinaryOperator(enTypeBinOp);
		}
		else if(CheckBinOperator(wsOneElement,enTypeBinOp))
		{
			if(!arParsLine.empty())
			{
				CBinaryOperator* m_pBinOp = new CBinaryOperator;
				m_pBinOp->SetLeftArg(arParsLine.back());
				m_pBinOp->SetRightArg(ParsElement(itFirst,itEnd,arParsLine));
				m_pBinOp->SetTypeBin(enTypeBinOp);
				arParsLine.pop_back();
				return m_pBinOp;
			}
			return NULL;
		}
		else if(CheckBracket(wsOneElement[0],enTypeBracket))
		{
			std::vector<CElement*> arValueBrecket;
			std::wstring::iterator itTempIteratorforBacket = itFirst;
			wsOneElement = GetElement(itFirst,itEnd);
			while(wsOneElement[0] != L'}')
			{
				arValueBrecket.push_back(ParsElement(itTempIteratorforBacket,itEnd,arValueBrecket));
				itFirst = itTempIteratorforBacket;
				wsOneElement = GetElement(itFirst,itEnd);
			}
			return new CBracket(arValueBrecket,enTypeBracket);
		}
		else if(CheckOperator(wsOneElement,enTypeOp))
		{
			COperator* oTempOp = new COperator;
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
			oTempOp->SetTypeOp(enTypeOp);
			return oTempOp;
		}
		else if(CheckTopAttribute(wsOneElement,enTypeAtt) || CheckPropertiesAttribute(wsOneElement,enTypeAtt)|| (wsOneElement == L"color" && CheckColorAttribute(GetElement(itFirst,itEnd),enTypeAtt)))
		{
			return new CAttribute(ParsElement(itFirst,itEnd,arParsLine),enTypeAtt);
		}
//		else if(wsOneElement == L"left" && CheckScalable_NotScalableBracket(GetElement(itFirst,itEnd),enTypeBracket))
//		{
//			std::vector<CElement*> arValueBrecket;
//			std::wstring::iterator itTempIteratorforBacket = itFirst;
//			wsOneElement = GetElement(itFirst,itEnd);
//			while(wsOneElement != L"right")
//			{
//				arValueBrecket.push_back(ParsElement(itTempIteratorforBacket,itEnd,arValueBrecket));
//				itFirst = itTempIteratorforBacket;
//				wsOneElement = GetElement(itFirst,itEnd);
//			}
//			wsOneElement = GetElement(itFirst,itEnd);
//			return new CBracket(arValueBrecket,enTypeBracket);
//		}
	}

	bool CStarMathPars::CheckDigit(const std::wstring &wsCheckToken)
	{
		for(char Check: wsCheckToken)
		{
			if(!isdigit(Check)) return false;
		}
		return true;
	}
	bool CStarMathPars::CheckScalable_NotScalableBracket(const std::wstring &wsCheckToken, TypeBracket &enType)
	{
		if(wsCheckToken == L"ldbracket")
		{
			enType = ldbracket;
			return true;
		}
		else if(wsCheckToken == L"lbrace")
		{
			enType = lbrace;
			return true;
		}
		else if(wsCheckToken == L"langle")
		{
			enType = langle;
			return true;
		}
		else if(wsCheckToken == L"lceil")
		{
			enType = lceil;
			return true;
		}
		else if(wsCheckToken == L"lfloor")
		{
			enType = lfloor;
			return true;
		}
		else if(wsCheckToken == L"lline")
		{
			enType = lline;
			return true;
		}
		else if(wsCheckToken == L"ldline")
		{
			enType = ldline;
			return true;
		}
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

	bool CStarMathPars::CheckBinOperator(const std::wstring &wsCheckToken, TypeBinOperator& enTypeBinOperator)
	{
		if(wsCheckToken == L"over")
		{
			enTypeBinOperator = over;
			return true;
		}
		else if(wsCheckToken == L"cdot")
		{
			enTypeBinOperator = cdot;
			return true;
		}
		else if(wsCheckToken == L"times")
		{
			enTypeBinOperator = times;
			return true;
		}
		else if(wsCheckToken == L"frac")
		{
			enTypeBinOperator = frac;
			return true;
		}
		else if(wsCheckToken == L"div")
		{
			enTypeBinOperator = div;
			return true;
		}
		else if(wsCheckToken == L"oplus")
		{
			enTypeBinOperator = oplus;
			return true;
		}
		else if(wsCheckToken == L"ominus")
		{
			enTypeBinOperator = ominus;
			return true;
		}
		else if(wsCheckToken == L"odot")
		{
			enTypeBinOperator = odot;
			return true;
		}
		else if(wsCheckToken == L"otimes")
		{
			enTypeBinOperator = otimes;
			return true;
		}
		else if(wsCheckToken == L"odivide")
		{
			enTypeBinOperator = odivide;
			return true;
		}
		else if(wsCheckToken == L"circ")
		{
			enTypeBinOperator = circ;
			return true;
		}
		else if(wsCheckToken == L"wideslash")
		{
			enTypeBinOperator = wideslash;
			return true;
		}
		else if(wsCheckToken == L"widebslash")
		{
			enTypeBinOperator = widebslash;
			return true;
		}
		else return false;
	}

	bool CStarMathPars::CheckOneElementBinOperator(const char &wsCheckToken, TypeBinOperator &enTypeBinOperator)
	{
		switch (wsCheckToken)
		{
		case '+':
			enTypeBinOperator = plus;
			return true;
		case '-':
			enTypeBinOperator = minus;
			return true;

		case '*':
			enTypeBinOperator = multipl;
			return true;
		case '/':
			enTypeBinOperator = division;
			return true;
		default:
		return false;
		}
	}
	bool CStarMathPars::CheckOperator(const std::wstring &wsCheckToken, TypeOperator & enTypeOperator)
	{
		if(wsCheckToken == L"lim")
		{
			 enTypeOperator = lim;
			return true;
		}
		else if(wsCheckToken == L"sum")
		{
			 enTypeOperator = sum;
			return true;
		}
		else return false;
	}
	bool CStarMathPars::CheckIndex(const std::wstring &wsCheckToken)
	{
		if(wsCheckToken == L"from")
		{
			return true;
		}
		else if(wsCheckToken ==L"to")
		{
			return true;
		}
		else return false;
	}
	bool CStarMathPars::CheckBracket(const char &wsCheckToken, TypeBracket & enTypeBracket)
	{
		switch (wsCheckToken) {
		case '{':
			enTypeBracket = brace;
			return true;
		case '(':
			enTypeBracket = round;
			return true;
		case '[':
			enTypeBracket = square;
			return true;
		default:
			return false;
		}
	}
	bool CStarMathPars::CheckTopAttribute(const std::wstring& wsCheckToken,TypeAttribute& enTypeAtt)
	{
		if(wsCheckToken == L"acute")
		{
			enTypeAtt = acute;
			return true;
		}
		else if(wsCheckToken == L"breve")
		{
			enTypeAtt = breve;
			return true;
		}
		else if(wsCheckToken == L"dot")
		{
			enTypeAtt = dot;
			return true;
		}
		else if(wsCheckToken == L"dddot")
		{
			enTypeAtt = dddot;
			return true;
		}
		else if(wsCheckToken == L"vec")
		{
			enTypeAtt = vec;
			return true;
		}
		else if(wsCheckToken == L"tilde")
		{
			enTypeAtt = tilde;
			return true;
		}
		else if(wsCheckToken == L"check")
		{
			enTypeAtt = check;
			return true;
		}
		else if(wsCheckToken == L"grave")
		{
			enTypeAtt = grave;
			return true;
		}
		else if(wsCheckToken == L"circle")
		{
			enTypeAtt = circle;
			return true;
		}
		else if(wsCheckToken == L"ddot")
		{
			enTypeAtt = ddot;
			return true;
		}
		else if(wsCheckToken == L"bar")
		{
			enTypeAtt = bar;
			return true;
		}
		else if(wsCheckToken == L"harpoon")
		{
			enTypeAtt = harpoon;
			return true;
		}
		else if(wsCheckToken == L"hat")
		{
			enTypeAtt = hat;
			return true;
		}
		else if(wsCheckToken == L"widevec")
		{
			enTypeAtt = widevec;
			return true;
		}
		else if(wsCheckToken == L"widetilde")
		{
			enTypeAtt = widetilde;
			return true;
		}
		else if(wsCheckToken == L"overline")
		{
			enTypeAtt = overline;
			return true;
		}
		else if(wsCheckToken == L"overstrike")
		{
			enTypeAtt = overstrike;
			return true;
		}
		else if(wsCheckToken == L"wideharpoon")
		{
			enTypeAtt = wideharpoon;
			return true;
		}
		else if(wsCheckToken == L"widehat")
		{
			enTypeAtt = widehat;
			return true;
		}
		else if(wsCheckToken == L"underline")
		{
			enTypeAtt = underline;
			return true;
		}
		else return false;
	}
	bool CStarMathPars::CheckPropertiesAttribute(const std::wstring& wsCheckToken,TypeAttribute& enTypeAtt)
	{
		if(wsCheckToken == L"phantom")
		{
			enTypeAtt = phantom;
			return true;
		}
		else if(wsCheckToken == L"bold")
		{
			enTypeAtt = bold;
			return true;
		}
		else if(wsCheckToken == L"ital")
		{
			enTypeAtt = ital;
			return true;
		}
		else return false;
	}
	bool CStarMathPars::CheckColorAttribute(const std::wstring &wsCheckToken, TypeAttribute &enTypeAtt)
	{
		if(wsCheckToken == L"black")
		{
			enTypeAtt = black;
			return true;
		}
		else if(wsCheckToken == L"green")
		{
			enTypeAtt = green;
			return true;
		}
		else if(wsCheckToken == L"aqua")
		{
			enTypeAtt = aqua;
			return true;
		}
		else if(wsCheckToken == L"yellow")
		{
			enTypeAtt = yellow;
			return true;
		}
		else if(wsCheckToken == L"lime")
		{
			enTypeAtt = lime;
			return true;
		}
		else if(wsCheckToken == L"navy")
		{
			enTypeAtt = navy;
			return true;
		}
		else if(wsCheckToken == L"purple")
		{
			enTypeAtt = purple;
			return true;
		}
		else if(wsCheckToken == L"teal")
		{
			enTypeAtt = teal;
			return true;
		}
		else if(wsCheckToken == L"blue")
		{
			enTypeAtt = blue;
			return true;
		}
		else if(wsCheckToken == L"red")
		{
			enTypeAtt = red;
			return true;
		}
		else if(wsCheckToken == L"fuchsia")
		{
			enTypeAtt = fuchsia;
			return true;
		}
		else if(wsCheckToken == L"gray")
		{
			enTypeAtt = gray;
			return true;
		}
		else if(wsCheckToken == L"maroon")
		{
			enTypeAtt = maroon;
			return true;
		}
		else if(wsCheckToken == L"olive")
		{
			enTypeAtt = olive;
			return true;
		}
		else if(wsCheckToken == L"silver")
		{
			enTypeAtt = silver;
			return true;
		}
		else if(wsCheckToken == L"coral")
		{
			enTypeAtt = coral;
			return true;
		}
		else if(wsCheckToken == L"midnightblue")
		{
			enTypeAtt = midnightblue;
			return true;
		}
		else if(wsCheckToken == L"crimson")
		{
			enTypeAtt = crimson;
			return true;
		}
		else if(wsCheckToken == L"violet")
		{
			enTypeAtt = violet;
			return true;
		}
		else return false;
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
	CBinaryOperator::CBinaryOperator(const TypeBinOperator &enType)
	{
		enTypeBinOp = enType;
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
//Class methods COperator
	COperator::COperator(): oFromValue(NULL),oToValue(NULL),oValueOp(NULL)
	{}
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
	void COperator::AvailabilityMline()
	{
		bMline = true;
	}
//Class methods CBracket
	CBracket::CBracket()
	{}
	CBracket::CBracket(const std::vector<CElement*>& arValue,const TypeBracket& enType)
	{
		arBrecketVal = arValue;
		enTypeBracket = enType;
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
//Class methods CAttribute
	CAttribute::CAttribute()
	{}
	CAttribute::CAttribute(CElement *oValue, const TypeAttribute &enType)
	{
		enTypeAtt = enType;
		oValueAtt = oValue;
	}
	CAttribute::~CAttribute()
	{
		delete oValueAtt;
	}
	std::wstring CAttribute::GetValue()
	{
		return {};
	}
	TypeElement CAttribute::GetType()
	{
		return Attribute;
	}
	void CAttribute::SetTypeAtt(const TypeAttribute &enType)
	{
		enTypeAtt = enType;
	}
	void CAttribute::SetValueAtt(CElement *oValue)
	{
		oValueAtt = oValue;
	}
}

