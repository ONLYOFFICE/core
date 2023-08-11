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

	void CStarMathPars::Pars(std::wstring& sStarMathLine)
	{
		std::wstring m_wsTempValueElement;
		std::wstring::iterator itFirst = sStarMathLine.begin(), itEnd = sStarMathLine.end();
		while(itFirst != itEnd)
		{
			m_wsTempValueElement = sGetElement(itFirst,itEnd);
			ParsElement(m_wsTempValueElement);
		}
	}
	std::wstring CStarMathPars::sGetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd)
	{
		std::wstring m_wsElement;
		for(itFirst; itFirst != itEnd;itFirst++)
		{
			if(*itFirst != ' ') m_wsElement.push_back(*itFirst);
			else
			{
				itFirst++;
				break;
			}
		}
		return m_wsElement;
	}
	void CStarMathPars::ParsElement(std::wstring &sOneElement)
	{
		CNumber m_oDigit;
		CUnarySign m_oUnarSign;
		bool itDigit = CheckDigit(sOneElement);
		if(itDigit) m_arParsLine.push_back(m_oDigit.GetNumber(sOneElement));
		else if(sOneElement.size() > 3 && CheckUnarSign(sOneElement,m_oUnarSign)) ParsElement(sOneElement);
	}

	bool CStarMathPars::CheckDigit(std::wstring &wsCheckToken)
	{
		for(char Check: wsCheckToken)
		{
			if(!isdigit(Check)) return false;
		}
		return true;
	}

	bool CStarMathPars::CheckUnarSign(std::wstring &wsCheckToken,CUnarySign& m_oUnarSign)
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
	}
//Class methods CNumber
	CNumber::CNumber()
	{
	}

	CNumber::~CNumber()
	{
	}

	std::wstring CNumber::GetValue()
	{
		return m_sValueNumber;
	}
	TypeElement CNumber::GetType()
	{
		return Number;
	}
	CNumber* CNumber::GetNumber(std::wstring& sNumberElement)
	{

		CNumber* oValue = new CNumber;
		oValue->m_sValueNumber = sNumberElement;
		return oValue;
//Class methods CUnarySign
	}
	CUnarySign* CUnarySign::GetUnarSign(std::wstring& sUnarToken)
	{
		CUnarySign* oSign = new CUnarySign;
		oSign->m_sUnar = sUnarToken;
		return oSign;
	}

	std::wstring CUnarySign::GetValue()
	{
		return m_sUnar;
	}
	TypeElement CUnarySign::GetType()
	{
		return m_UnarType;
	}
	CUnarySign::CUnarySign()
	{}
	CUnarySign::~CUnarySign()
	{}
//Class methods CBinaryOperator
	CBinaryOperator::CBinaryOperator()
	{}
	CBinaryOperator::~CBinaryOperator()
	{}
	TypeElement CBinaryOperator::GetType()
	{
		return m_TypeSign;
	}
	std::wstring CBinaryOperator::GetValue()
	{
		std::cout << m_TypeSign;
		return L"Good";
	}
}

