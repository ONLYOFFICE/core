#ifndef CSTARMATHPARS_H
#define CSTARMATHPARS_H
#include <iostream>
#include <vector>
#include <iterator>
#include <stack>

namespace StarMath
{
	enum TypeElement{
		Number,
		BinOperator,
		Operator,
		Bracket,
		UnarSign,
		Box,
		SM_Index_from,
		SM_Index_to,
	};
	enum TypeBinOperator
	{
		cdot,
		times,
		over,
		plus,
		minus,
		frac,
		div,
		multipl,
		division,
		oplus,
		ominus,
		odot,
		otimes,
		odivide,
		circ,
		wideslash,
		widebslash,
	};
	enum TypeOperator
	{
		lim,
		sum,
	};
	enum TypeBracket
	{
		brace_opening,
		brace_closing,
		round_opening,
		round_closing,
		square_opening,
		square_closing,
	};
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
		CNumber(const std::wstring& wsValue);
		~CNumber();
		std::wstring GetValue() override;
		TypeElement GetType() override ;
	private:
		std::wstring m_wsValueNumber;
	};

	class CUnarySign: public CElement
	{
	public:
		CUnarySign();
		~CUnarySign();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		CUnarySign* GetUnarSign(const std::wstring& wsUnarToken);
	private:
		std::wstring m_wsUnar;
		TypeElement m_enUnarType = UnarSign;
	};
	class CBinaryOperator: public CElement
	{
	public:
		CBinaryOperator();
		virtual ~CBinaryOperator();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		TypeBinOperator GetTypeBin();
		void SetTypeBin(const TypeBinOperator& enType);
		void SetLeftArg(CElement* oLeftArg);
		void SetRightArg(CElement* oRightArg);
	private:
		TypeBinOperator enTypeBinOp;
		std::vector<CElement*> arLeftArg;
		std::vector<CElement*> arRightArg;
	};
	class COperator: public CElement
	{
	public:
		COperator();
		~COperator();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		COperator* GetOperator(const TypeElement& enTypeOper);
	private:
		TypeElement enType;
	};
	class CIndex: public CElement
	{
	public:
		CIndex();
		~CIndex();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		CIndex* GetIndex(const TypeElement& enTypeIndex);
	private:
		TypeElement enType;
	};
	class CBracket: public CElement
	{
	public:
		CBracket();
		~CBracket();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		CBracket* GetBracket(const TypeBracket& enTypeBracket);
	private:
		TypeBracket enTypeBracket;
		TypeElement enTypeEl;
	};
	class CStarMathPars
	{
	public:
		CStarMathPars();
		virtual ~CStarMathPars();
		void Pars(std::wstring& wsStarMathLine);
		std::wstring GetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd);
		CElement* ParsElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd);
		bool CheckDigit(const std::wstring& wsCheckToken);
		bool CheckUnarSign(std::wstring& wsCheckToken,CUnarySign& oUnarSign);
		bool CheckBinOperator(const std::wstring& wsCheckToken,TypeBinOperator& enTypeBinOperator);
		bool CheckOneElementBinOperator(const char& wsCheckToken, TypeBinOperator& enTypeBinOperator);
		bool CheckOperator(const std::wstring& wsCheckToken,TypeOperator& enTypeOperator);
		bool CheckIndex(const std::wstring& wsCheckToken, TypeElement& TypeIndex);
		bool CheckBracket(const char& wsCheckToken,TypeBracket& TypeBracket);
		bool CheckScalable_NotScalableBracket(const std::wstring& wsCheckToken, TypeElement& TypeBracket);
		void PrintAr();
	private:
		std::vector<CElement*> m_arParsLine;
	};
}

#endif // CSTARMATHPARS_H
