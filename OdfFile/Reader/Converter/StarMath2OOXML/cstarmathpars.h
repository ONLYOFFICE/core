#ifndef CSTARMATHPARS_H
#define CSTARMATHPARS_H
#include <iostream>
#include <vector>
#include <iterator>

namespace StarMath
{
	enum TypeElement{Number,UnarSign,SM_BinOper_cdot,SM_BinOper_times,SM_BinOper_over,SM_BinOper_plus,SM_BinOper_minus,SM_BinOper_frac,SM_BinOper_div};
	class CElement
	{
	public:
		virtual std::wstring GetValue() = 0;
		virtual TypeElement GetType() = 0;
	};

	class CNumber: public CElement
	{
	public:
		CNumber();
		~CNumber();
		CNumber* GetNumber(std::wstring& sNumberElement);
		std::wstring GetValue() override;
		TypeElement GetType() override ;
	private:
		std::wstring m_sValueNumber;
		TypeElement m_Number = Number;
	};

	class CUnarySign: public CElement
	{
	public:
		CUnarySign();
		~CUnarySign();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		CUnarySign* GetUnarSign(std::wstring& sUnarToken);
	private:
		std::wstring m_sUnar;
		TypeElement m_UnarType = UnarSign;
	};
	class CBinaryOperator: public CElement
	{
	public:
		CBinaryOperator();
		~CBinaryOperator();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		CBinaryOperator* GetSign(std::wstring& wsSignToken);
	private:
		TypeElement m_TypeSign;
	};

	class CBoxValue: public CElement
	{
	public:
		CBoxValue();
		~CBoxValue();
		CBoxValue* ReadBox(std::string::iterator& itStartBox,std::string::iterator& itEndBox);
		std::wstring GetValue() override;
	private:
		std::vector<CElement*> arValueBox;
	};

	class CStarMathPars
	{
	public:
		CStarMathPars();
		~CStarMathPars();
		void Pars(std::wstring& sStarMathLine);
		std::wstring sGetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd);
		void ParsElement(std::wstring& sOneElement);
		bool CheckDigit(std::wstring& wsCheckToken);
		bool CheckUnarSign(std::wstring& wsCheckToken,CUnarySign& m_oUnarSign);
	private:
		std::vector<CElement*> m_arParsLine;
	};
}

#endif // CSTARMATHPARS_H
