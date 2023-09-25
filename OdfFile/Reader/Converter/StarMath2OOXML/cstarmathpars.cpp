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
//Test version (works correctly only with operators, does not work correctly with attributes)
	CElement* CStarMathPars::ReadWithoutBrackets(std::wstring::iterator &itFirst, std::wstring::iterator &itEnd,std::vector<CElement*>& arParsLine)
	{
		CElement* m_oOneElement = ParsElement(itFirst,itEnd,arParsLine);
		if(m_oOneElement->GetType() != Bracket)
		{
			std::vector<CElement*> arTempValue;
			arTempValue.push_back(m_oOneElement);
			while(CheckingTheNextElement(itFirst,itEnd,CheckBinOperator)|| CheckingTheNextElement(itFirst,itEnd,CheckPlusOrMinus)||CheckingTheNextElement(itFirst,itEnd,CheckBinOperatorLowPriority))
			{
				arTempValue.push_back(ParsElement(itFirst,itEnd,arTempValue));
			}
			if(arTempValue.size()== 1) return m_oOneElement = arTempValue.back();
			else
			{
				CBracket* m_oBracket = new CBracket;
				m_oBracket->SetBracketVal(arTempValue);
				m_oBracket->SetTypeBracket(L"brace");
				return m_oBracket;
			}
		}
		else return m_oOneElement;
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
		else if(CheckSpecialCharacter(wsOneElement) || CheckSpecialOperation(wsOneElement) || CheckSpecialConnection(wsOneElement)) return new CSpecialSymbol(wsOneElement);
		else if(CheckFunction(wsOneElement))
		{
			CFunction* m_oFunction = new CFunction;
			m_oFunction->SetTypeFunction(wsOneElement);
			if(L"ln" == wsOneElement) m_oFunction->SetArgument(ReadWithoutBrackets(itFirst,itEnd,arParsLine));
			else m_oFunction->SetArgument(ParsElement(itFirst,itEnd,arParsLine));
			m_oFunction->SetAttribute(m_oAttribute);
			return m_oFunction;
		}
		else if(CheckIndex(wsOneElement))
		{
			CIndex* m_oIndex = new CIndex(wsOneElement);
			m_oIndex->SetArgument(arParsLine.back());
			m_oIndex->SetIndex(ParsElement(itFirst,itEnd,arParsLine));
			return m_oIndex;
		}
		else if(CheckMatrix(wsOneElement))
		{
			CMatrix* m_oMatrix = new CMatrix(wsOneElement);	
			m_oMatrix->SetArgument(ParsElement(itFirst,itEnd,arParsLine));
			m_oMatrix->SetAttribute(m_oAttribute);
			return m_oMatrix;
		}
		else if(CheckBinOperator(wsOneElement) || CheckPlusOrMinus(wsOneElement) || CheckBinOperatorLowPriority(wsOneElement))
		{
			if(!arParsLine.empty())
			{
				CBinaryOperator* m_pBinOp = new CBinaryOperator(wsOneElement);
				CElement* m_oTempElement{ParsElement(itFirst,itEnd,arParsLine)};
				m_pBinOp->SetArgument(arParsLine.back());
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
		return(L"*" ==wsCheckToken || L"/" == wsCheckToken || wsCheckToken == L"over" || wsCheckToken == L"cdot" || wsCheckToken == L"times"  || wsCheckToken == L"div"  || wsCheckToken == L"odot" || wsCheckToken == L"otimes" || wsCheckToken == L"odivide"  || wsCheckToken == L"wideslash" || wsCheckToken == L"widebslash");
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
		return (wsCheckToken == L"_" || wsCheckToken == L"^" || L"lsup" == wsCheckToken || L"lsub" == wsCheckToken || L"csup" == wsCheckToken || L"csub" == wsCheckToken);
	}
	bool CStarMathPars::CheckIndexOp(const std::wstring &wsCheckToken)
	{
		return(L"from" == wsCheckToken || L"to" == wsCheckToken);
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
		return (L"mline" == wsCheckToken || L"#" == wsCheckToken || L"##" == wsCheckToken);
	}
	bool CStarMathPars::CheckSpecialOperation(const std::wstring &wsCheckToken)
	{
		return(L"intersection" == wsCheckToken || L"union" == wsCheckToken || L"setminus" == wsCheckToken || L"setquoyient" == wsCheckToken || L"subseteq" == wsCheckToken || L"subset" == wsCheckToken || L"supset" == wsCheckToken || L"supseteq" == wsCheckToken || L"nsubset" == wsCheckToken || L"nsupseteq" == wsCheckToken || L"nsupset" == wsCheckToken || L"nsubseteq" == wsCheckToken || L"in" == wsCheckToken || L"notin" == wsCheckToken || L"owns" == wsCheckToken);
	}
	bool CStarMathPars::CheckSpecialConnection(const std::wstring &wsCheckToken)
	{
		return(L"approx" == wsCheckToken || L"sim" == wsCheckToken || L"simeq" == wsCheckToken || L"equiv" == wsCheckToken || L"prop" == wsCheckToken || L"parallel" == wsCheckToken || L"ortho" == wsCheckToken || L"divides" == wsCheckToken || L"ndivides" == wsCheckToken || L"toward" == wsCheckToken || L"transl" == wsCheckToken || L"transr" == wsCheckToken || L"def" == wsCheckToken || L"=" == wsCheckToken || L"<>" == wsCheckToken || L"<" == wsCheckToken || L"<=" == wsCheckToken || L"leslant" == wsCheckToken || L">" == wsCheckToken || L">=" == wsCheckToken || L"geslant" == wsCheckToken || L"<<" == wsCheckToken || L">>" == wsCheckToken || L"prec" == wsCheckToken || L"succ" == wsCheckToken || L"preccurlyeq" == wsCheckToken || L"succcurlyeq" == wsCheckToken || L"precsim" == wsCheckToken || L"succsim" == wsCheckToken || L"nprec" == wsCheckToken || L"nsucc" == wsCheckToken || L"dlarrow" == wsCheckToken || L"dlrarrow" == wsCheckToken || L"drarrow" == wsCheckToken);
	}
	std::vector<CElement*> CStarMathPars::GetVector()
	{
		return m_arParsLine;
	}
	bool CStarMathPars::CheckFunction(const std::wstring &wsCheckToken)
	{
		return(L"sin" == wsCheckToken || L"cos" == wsCheckToken || L"ln" == wsCheckToken || L"tan" == wsCheckToken || L"cot" == wsCheckToken || L"sinh" == wsCheckToken || L"cosh" == wsCheckToken || L"tanh" == wsCheckToken || L"coth" == wsCheckToken || L"arcsin" == wsCheckToken || L"arccos" == wsCheckToken || L"arctan" == wsCheckToken || L"arccot" == wsCheckToken || L"arsinh" == wsCheckToken || L"arcosh" == wsCheckToken || L"artanh" == wsCheckToken || L"arcoth" == wsCheckToken || L"abs" == wsCheckToken || L"fact" == wsCheckToken || L"sqrt" == wsCheckToken || L"nroot" == wsCheckToken);
	}
	bool CStarMathPars::CheckOperation(const std::wstring &wsCheckToken)
	{
		return(L"intersection" == wsCheckToken || L"union" == wsCheckToken || L"setminus" == wsCheckToken || L"setquoytient" == wsCheckToken || L"subseteq" == wsCheckToken || L"subset" == wsCheckToken);
	}
	bool CStarMathPars::CheckMatrix(const std::wstring &wsCheckToken)
	{
		return (L"stack" == wsCheckToken || L"matrix" == wsCheckToken || L"binom" == wsCheckToken);
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
	{
		oRightArg = nullptr;
	}
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
		oRightArg = nullptr;
	}
	CBinaryOperator::~CBinaryOperator()
	{
		delete oRightArg;
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
	void CBinaryOperator::SetRightArg(CElement *oArgument)
	{
		oRightArg = oArgument;
	}
	void CBinaryOperator::SetTypeBin(const TypeBinOperator &enType)
	{
		this->enTypeBinOp = enType;
	}
	CElement* CBinaryOperator::GetRightArg()
	{
		return oRightArg;
	}
	TypeElement CBinaryOperator::GetTypeRight()
	{
		return oRightArg->GetType();
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
	CElement* COperator::GetValueOp()
	{
		return oValueOp;
	}
	CElement* COperator::GetFrom()
	{
		return oFromValue;
	}
	CElement* COperator::GetTo()
	{
		return oToValue;
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
		if (L"{" == wsCheckToken) enTypeBracket = brace;
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
	CSpecialSymbol::CSpecialSymbol(const std::wstring& wsToken)
	{
		if(L"mline" == wsToken) enTypeSpecial = mline;
		else if(L"#" == wsToken) enTypeSpecial = grid;
		else if(L"##" == wsToken) enTypeSpecial = dlgrid;
		else if(L"intersection" == wsToken) enTypeSpecial = intersection;
		else if(L"union" == wsToken) enTypeSpecial = Union;
		else if(L"setminus" == wsToken) enTypeSpecial = setminus;
		else if(L"setquoyient" == wsToken) enTypeSpecial = setquoyient;
		else if(L"subseteg" == wsToken) enTypeSpecial = subseteq;
		else if(L"subset" == wsToken) enTypeSpecial = subset;
		else if(L"supset" == wsToken) enTypeSpecial = supset;
		else if(L"supseteq" == wsToken) enTypeSpecial = supseteq;
		else if(L"nsubset" == wsToken) enTypeSpecial = nsubset;
		else if(L"nsubseteq" == wsToken) enTypeSpecial = nsubseteq;
		else if(L"nsupseteq" == wsToken) enTypeSpecial = nsupseteq;
		else if(L"nsupset" == wsToken) enTypeSpecial = nsupset;
		else if(L"in" == wsToken) enTypeSpecial = in;
		else if(L"notin" == wsToken) enTypeSpecial = notin;
		else if(L"owns" == wsToken) enTypeSpecial = owns;
		else if(L"approx" == wsToken) enTypeSpecial = approx;
		else if(L"sim" == wsToken) enTypeSpecial = sim;
		else if(L"simeq" == wsToken) enTypeSpecial = simeq;
		else if(L"equiv" == wsToken) enTypeSpecial = equiv;
		else if(L"prop" == wsToken) enTypeSpecial = prop;
		else if(L"parallel" == wsToken) enTypeSpecial = parallel;
		else if(L"ortho" == wsToken) enTypeSpecial = ortho;
		else if(L"divides" == wsToken) enTypeSpecial = divides;
		else if(L"ndivides" == wsToken) enTypeSpecial = ndivides;
		else if(L"toward" == wsToken) enTypeSpecial = toward;
		else if(L"transl" == wsToken) enTypeSpecial = transl;
		else if(L"transr" == wsToken) enTypeSpecial = transr;
		else if(L"def" == wsToken) enTypeSpecial = def;
		else if(L"=" == wsToken) enTypeSpecial = equals;
		else if(L"<>" == wsToken) enTypeSpecial = notequals;
		else if(L"<" == wsToken) enTypeSpecial = learrow;
		else if(L"<=" == wsToken) enTypeSpecial = learrowequals;
		else if(L"leslant" == wsToken) enTypeSpecial = leslant;
		else if(L">" == wsToken) enTypeSpecial = riarrow;
		else if(L">=" == wsToken) enTypeSpecial = riarrowequals;
		else if(L"geslant" == wsToken) enTypeSpecial = geslant;
		else if(L"<<" == wsToken) enTypeSpecial = dllearrow;
		else if(L">>" == wsToken) enTypeSpecial = dlriarrow;
		else if(L"prec" == wsToken) enTypeSpecial = prec;
		else if(L"succ" == wsToken) enTypeSpecial = succ;
		else if(L"preccurlyeq" == wsToken) enTypeSpecial = preccurlyeq;
		else if(L"succcurlyeq" == wsToken) enTypeSpecial = succcurlyeq;
		else if(L"precsim" == wsToken) enTypeSpecial = precsim;
		else if(L"succsim" == wsToken) enTypeSpecial = succsim;
		else if(L"nprec" == wsToken) enTypeSpecial = nprec;
		else if(L"nsucc" == wsToken) enTypeSpecial = nsucc;
		else if(L"dlarrow" == wsToken) enTypeSpecial = dlarrow;
		else if(L"dlrarrow" == wsToken) enTypeSpecial = dlrarrow;
		else if(L"drarrow" == wsToken) enTypeSpecial = drarrow;
		SetAttribute(nullptr);
	}
	CSpecialSymbol::~CSpecialSymbol()
	{}
	std::wstring CSpecialSymbol::GetValue()
	{
		return {};
	}
	TypeElement CSpecialSymbol::GetType()
	{
		return SpecialSymbol;
	}
//class methods CFunction
	CFunction::CFunction()
	{}
	CFunction::~CFunction()
	{
	}
	std::wstring CFunction::GetValue()
	{
		return L"";
	}
	TypeElement CFunction::GetType()
	{
		return Function;
	}
	TypeFunction CFunction::GetTypeFun()
	{
		return enTypeFun;
	}
	void CFunction::SetTypeFunction(const std::wstring &wsCheckToken)
	{
		if(L"sin" == wsCheckToken) enTypeFun = sin;
		else if(L"abs" == wsCheckToken) enTypeFun = abs;
		else if(L"fact" == wsCheckToken) enTypeFun = fact;
		else if(L"sqrt" == wsCheckToken) enTypeFun = sqrt;
		else if(L"cos" == wsCheckToken) enTypeFun = cos;
		else if(L"tan" == wsCheckToken) enTypeFun = tan;
		else if(L"cot" == wsCheckToken) enTypeFun = cot;
		else if(L"sinh" == wsCheckToken) enTypeFun = sinh;
		else if(L"cosh" == wsCheckToken) enTypeFun = cosh;
		else if(L"tanh" == wsCheckToken) enTypeFun = tanh;
		else if(L"coth" == wsCheckToken) enTypeFun = coth;
		else if(L"arcsin" == wsCheckToken) enTypeFun = arcsin;
		else if(L"arccos" == wsCheckToken) enTypeFun = arccos;
		else if(L"arctan" == wsCheckToken) enTypeFun = arctan;
		else if(L"arccot" == wsCheckToken) enTypeFun = arccot;
		else if(L"arsinh" == wsCheckToken) enTypeFun = arsinh;
		else if(L"arcosh" == wsCheckToken) enTypeFun = arcosh;
		else if(L"artanh" == wsCheckToken) enTypeFun = artanh;
		else if(L"arcoth" == wsCheckToken) enTypeFun = arcoth;
		else if(L"ln" == wsCheckToken) enTypeFun = ln;
	}
//class methods CArgumentContainer
	CArgumentContainer::CArgumentContainer()
	{
		oArgument = nullptr;
	}
	CArgumentContainer::~CArgumentContainer()
	{
		delete oArgument;
	}
	std::wstring CArgumentContainer::GetValue()
	{
		return L"";
	}
	TypeElement CArgumentContainer::GetType()
	{
		return TwoArgumentContainer;
	}
	void CArgumentContainer::SetArgument(CElement *oValue)
	{
		oArgument = oValue;
	}
	CElement* CArgumentContainer::GetArgument()
	{
		return oArgument;
	}
//class methods CIndex
	CIndex::CIndex(const std::wstring& wsToken)
	{
		if(L"^" == wsToken) enTypeIn = upper;
		else if(L"_" == wsToken) enTypeIn = lower;
		else if(L"lsup" == wsToken) enTypeIn = lsup;
		else if(L"lsub" == wsToken) enTypeIn = lsub;
		else if(L"csup" == wsToken) enTypeIn = csup;
		else if(L"csub" == wsToken) enTypeIn = csub;
		SetAttribute(nullptr);
	}
	CIndex::~CIndex()
	{
		delete oIndex;
	}
	std::wstring CIndex::GetValue()
	{
		return {};
	}
	TypeElement CIndex::GetType()
	{
		return Index;
	}
	void CIndex::SetIndex(CElement *oValueIndex)
	{
		oIndex = oValueIndex;
	}
//class methods CMatrix
   CMatrix::CMatrix(const std::wstring& wsToken)
   {
	   if (L"binom" == wsToken) enTypeMatrix = binom;
	   else if(L"stack" == wsToken) enTypeMatrix = stack;
	   else if(L"matrix" == wsToken) enTypeMatrix = matrix;
   }
   CMatrix::~CMatrix()
   {}
   std::wstring CMatrix::GetValue()
   {
	   return {};
   }
   TypeElement CMatrix::GetType()
   {
	   return Matrix;
   }
}



