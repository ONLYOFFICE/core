#ifndef CSTARMATHPARS_H
#define CSTARMATHPARS_H
#include "typeselements.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <stack>

namespace StarMath
{
	class CAttribute
	{
	public:
		CAttribute(const TypeElement& enType);
		~CAttribute();
		static TypeElement IsAttribute(const std::wstring& wsToken);
		TypeElement GetType();
	private:
		TypeElement enTypeAttr;
	};

	class CIndex;

	class CElement
	{
	public:
		CElement();
		virtual ~CElement();
		virtual void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) = 0;
		//принимает подтип (over, frac и т.д) и создает нужный нам класс(внутри уже идет проверка по классам)
		static CElement* CreateElement(const std::wstring& wsToken);
		//static TypeElement GetTypeElement(const std::wstring& wsToken);
		void SetAttribute(const std::vector<CAttribute*> arAttr);
		void SetIndex(CIndex* pIndex);
		void SetBaseType(const TypeElement& enType);
		TypeElement GetBaseType();
	private:
		CIndex* pElementIndex;
		std::vector<CAttribute*> arElementAttributes;
		TypeElement enBaseType;
	};

	class CIndex
	{
	public:
		CIndex(const TypeElement& enType);
		~CIndex();
		void SetValueIndex(CElement* pElement);
		CElement* GetValueIndex();
		static bool IsIndex(const std::wstring& wsCheckToken);
		static CIndex* CreateIndex(const std::wstring& wsToken);
	private:
		CElement* pValueIndex;
		TypeElement enTypeIndex;
	};

	class CElementString: public CElement
	{
	public:
		CElementString(const std::wstring& wsTokenString);
		virtual ~CElementString();
		void SetString(const std::wstring& wsTokenString);
		std::wstring GetString();
		static bool IsDigit(const std::wstring& wsCheckToken);
	private:
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		std::wstring wsString;
	};

	class CElementBinOperator: public CElement
	{
	public:
		CElementBinOperator(const TypeElement& enType);
		virtual ~CElementBinOperator();
		void SetLeftArg(CElement* pElement);
		void SetRightArg(CElement* pElement);
		void SetTypeBinOP(const TypeElement& enType);
		CElement* GetRightArg();
		CElement* GetLeftArg();
		static bool IsBinOperatorHightPrior(const std::wstring& wsToken);
	private:
		bool IsBinOperatorLowPrior();
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		CElement* pLeftArgument;
		CElement* pRightArgument;
		TypeElement enTypeBinOp;
	};

	class CElementOperator: public CElement
	{
	public:
		CElementOperator(const TypeElement& enType);
		virtual ~CElementOperator();
		void SetValueOperator(CElement* pElement);
		CElement* GetValueOperator();
		void SetFromValue(CElement* pElement);
		CElement* GetFromValue();
		void SetToValue(CElement* pElement);
		CElement* GetToValue();
	private:
		void Pars(std::wstring::iterator &itStart, std::wstring::iterator &itEnd) override;
		CElement* pValueOperator;
		CElement* pValueFrom;
		CElement* pValueTo;
		TypeElement enTypeOperator;
	};

	class CElementBracket: public CElement
	{
	public:
		CElementBracket(const TypeElement& enType);
		virtual ~CElementBracket();
		void SetBracketValue(const std::vector<CElement*>& arValue);
	private:
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		static bool IsBracketClose(const std::wstring& wsToken);
		TypeElement enTypeBracket;
		std::vector<CElement*> arBrecketValue;
	};

	class CElementSetOperations: public CElement
	{
	public:
		CElementSetOperations(const TypeElement& enType);
		virtual ~CElementSetOperations();
		void SetLeftArg(CElement* pElement);
		CElement* GetLeftArg();
		void SetRightArg(CElement* pElement);
		CElement* GetRightArg();
		static bool IsSetOperation(const std::wstring& wsToken);
	private:
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		CElement* pLeftArgument;
		CElement* pRightArgument;
		TypeElement enTypeSet;
	};

	class CElementConnection: public CElement
	{
	public:
		CElementConnection(const TypeElement& enType);
		virtual ~CElementConnection();
		void SetRightArg(CElement* pElement);
		CElement* GetRightArg();
		void SetLeftArg(CElement* pElement);
		CElement* GetLeftArg();
		static bool IsConnection(const std::wstring& wsToken);
	private:
		void Pars(std::wstring::iterator& itStart, std::wstring::iterator& itEnd) override;
		CElement* pLeftArgument;
		CElement* pRightArgument;
		TypeElement enTypeCon;
	};

	class CElementFunction: public CElement
	{
	public:
		CElementFunction(const TypeElement& enType);
		virtual ~CElementFunction();
		void SetValueFunction(CElement* pElement);
		CElement* GetValueFunction();
	private:
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		CElement* pValue;
		TypeElement enTypeFunction;
	};

	class CElementSpecialSymbol: public CElement
	{
	public:
		CElementSpecialSymbol(const TypeElement& enType);
		virtual ~CElementSpecialSymbol();
	private:
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		TypeElement enTypeSpecial;
	};

	class CParseStarMathString
	{
	public:
		std::vector<CElement*> Parse(std::wstring& wsParseString);
		static CElement* ParsElement(std::wstring::iterator&  itStart, std::wstring::iterator& itEnd);
		static std::wstring GetElement(std::wstring::iterator& itStart,std::wstring::iterator& itEnd);
		static bool CheckingTheNextElement(std::wstring::iterator& itStart,std::wstring::iterator& itEnd, bool (&func)(const std::wstring&));
		static bool MoveToNextElement(std::wstring::iterator& itStart,std::wstring::iterator& itEnd);
		static void AddLeftArgument(CElement* pLeftArg,CElement* pElementWhichAdd);
		template<typename T>
		static void SetLeft(CElement* pLeftArg, CElement* pElementWhichaAdd);
	private:
		std::vector<CElement*> arEquation;
	};
}

#endif // CSTARMATHPARS_H
