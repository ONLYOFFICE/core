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
		 m_arParsLine.push_back(ParsElement(itFirst,itEnd));
		}
		PrintAr();
	}
	std::wstring CStarMathPars::GetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd)
	{
		std::wstring m_wsElement;
		for(itFirst; itFirst != itEnd;itFirst++)
		{
			if(iswspace(*itFirst))
			{
				itFirst++;
				break;
			}
			else if(!m_wsElement.empty() && (*itFirst == L'{' || *itFirst == L'}' || (isalpha(*itFirst) && isdigit(m_wsElement.back())) || (isdigit(*itFirst) && isalpha(m_wsElement.back()))))
			{
				return m_wsElement;
			}
			else if((*itFirst == L'{' || *itFirst == L'}') && m_wsElement.empty() )
			{
				m_wsElement.push_back(*itFirst);
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
	CElement* CStarMathPars::ParsElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd)
	{
		TypeBinOperator enTypeBinOp;
		std::wstring wsOneElement = GetElement(itFirst,itEnd);
		if(CheckDigit(wsOneElement))
		{
			return new CNumber(wsOneElement);
		}
		else if(CheckBinOperator(wsOneElement,enTypeBinOp) || (wsOneElement.size() == 1 && CheckOneElementBinOperator(wsOneElement[0],enTypeBinOp)))
		{
			if(!m_arParsLine.empty())
			{
				CBinaryOperator* m_pBinOp = new CBinaryOperator;
				m_pBinOp->SetLeftArg(m_arParsLine.back());
				m_pBinOp->SetRightArg(ParsElement(itFirst,itEnd));
				m_pBinOp->SetTypeBin(enTypeBinOp);
				m_arParsLine.pop_back();
				return m_pBinOp;
			}
			return NULL;
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

	/*bool CStarMathPars::CheckScalable_NotScalableBracket(const std::wstring &wsCheckToken, TypeElement &TypeBracket)
	{
		std::wstring wsSubstring;
		if((wsSubstring = wsCheckToken.substr(0,4)) == L"left")
		{
			wsSubstring = wsCheckToken.substr(5);
			return CheckScalable_NotScalableBracket(wsSubstring,TypeBracket);
		}
	}*/
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
			break;
		case '-':
			enTypeBinOperator = minus;
			return true;
			break;
		case '*':
			enTypeBinOperator = multipl;
			return true;
			break;
		case '/':
			enTypeBinOperator = division;
			return true;
			break;
		default:
		return false;
		break;
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
	/*bool CStarMathPars::CheckIndex(const std::wstring &wsCheckToken, TypeBinOperator &TypeIndex)
	{
		if(wsCheckToken == L"from")
		{
			TypeIndex = SM_Index_from;
			return true;
		}
		else if(wsCheckToken ==L"to")
		{
			TypeIndex = SM_Index_to;
			return true;
		}
		else return false;
	}*/
	bool CStarMathPars::CheckBracket(const char &wsCheckToken, TypeBracket & enTypeBracket)
	{
		switch (wsCheckToken) {
		case '{':
			enTypeBracket = brace_opening;
			return true;
			break;
		case '}':
			enTypeBracket = brace_closing;
			return true;
			break;
		case '(':
			enTypeBracket = round_opening;
			return true;
			break;
		case ')':
			enTypeBracket = round_closing;
			return true;
			break;
		case '[':
			enTypeBracket = square_opening;
			return true;
			break;
		case ']':
			enTypeBracket = square_closing;
			return true;
			break;
		default:
			return false;
			break;
		}
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
	CBinaryOperator::~CBinaryOperator()
	{
		for(CElement* ReleaseObject: arLeftArg)
		{
			delete ReleaseObject;
		}
		for(CElement* ReleaseObject: arRightArg)
		{
			delete ReleaseObject;
		}
	}
	TypeElement CBinaryOperator::GetType()
	{
		for(CElement* enTemp: arLeftArg)
		{
			std::wcout<< enTemp->GetType() << std::endl;
		}
		for(CElement* enTemp: arRightArg)
		{
			std::wcout<< enTemp->GetType() << std::endl;
		}
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
		this->arLeftArg.push_back(oLeftArg);
	}
	void CBinaryOperator::SetRightArg(CElement *oRightArg)
	{
		this->arRightArg.push_back(oRightArg);
	}
	void CBinaryOperator::SetTypeBin(const TypeBinOperator &enType)
	{
		this->enTypeBinOp = enType;
	}
//Class methods COperator
	COperator::COperator()
	{}
	COperator::~COperator()
	{}
	TypeElement COperator::GetType()
	{
		return enType;
	}
	std::wstring COperator::GetValue()
	{
		return L"";
	}
	COperator* COperator::GetOperator(const TypeElement &enTypeOper)
	{
		COperator* oOper = new COperator;
		oOper->enType = enTypeOper;
		return oOper;
	}
//Class methods CIndex
	CIndex::CIndex()
	{}
	CIndex::~CIndex()
	{}
	TypeElement CIndex::GetType()
	{
		return enType;
	}
	std::wstring CIndex::GetValue()
	{
		return L"Good";
	}
	CIndex* CIndex::GetIndex(const TypeElement &enTypeIndex)
	{
		CIndex* oIndex = new CIndex;
		oIndex->enType = enTypeIndex;
		return oIndex;
	}
//Class methods CBracket
	CBracket::CBracket()
	{}
	CBracket::~CBracket()
	{}
	TypeElement CBracket::GetType()
	{
		return enTypeEl;
	}
	std::wstring CBracket::GetValue()
	{
		return L"Good";
	}
	CBracket* CBracket::GetBracket(const TypeBracket &enTypeBracket)
	{
		CBracket* oBracket = new CBracket;
		oBracket->enTypeBracket = enTypeBracket;
		return oBracket;
	}
}

