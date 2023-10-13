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
	private:
		std::vector<CAttribute*> arElementAttributes;
	};

	class CElementString: public CElement
	{
	public:
		CElementString(const std::wstring& wsTokenString);
		virtual ~CElementString();
		void SetString(const std::wstring& wsTokenString);
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		std::wstring GetString();
		static bool IsDigit(const std::wstring& wsCheckToken);
	private:
		std::wstring wsString;
	};

	class CElementBinOperator: public CElement
	{
	public:
		CElementBinOperator(const TypeElement& enType);
		virtual ~CElementBinOperator();
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
		void SetLeftArg(CElement* pElement);
		void SetRightArg(CElement* pElement);
		void SetTypeBinOP(const TypeElement& enType);
		CElement* GetRightArg();
		CElement* GetLeftArg();
		static bool IsBinOperator(const TypeElement& enCheckType);
	private:
		bool IsLowPriorityBinOp(const TypeElement& enType);
		bool IsHighPriorityBinOp(const TypeElement& enType);
		CElement* pLeftArgument;
		CElement* pRightArgument;
		TypeElement enTypeBinOp;
	};

	class CElementBracket: public CElement
	{
	public:
		CElementBracket(const TypeElement& enType);
		virtual ~CElementBracket();
		void SetBracketValue(const std::vector<CElement*>& arValue);
		void Pars(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) override;
	private:
		static bool IsBracketClose(const std::wstring& wsToken);
		TypeElement enTypeBracket;
		std::vector<CElement*> arBrecketValue;
	};

	class CParseStarMathString
	{
	public:
		std::vector<CElement*> Parse(std::wstring& wsParseString);
		static CElement* ParsElement(std::wstring::iterator&  itStart, std::wstring::iterator& itEnd);
		static std::wstring GetElement(std::wstring::iterator& itStart,std::wstring::iterator& itEnd);
		static bool CheckingTheNextElement(std::wstring::iterator& itStart,std::wstring::iterator& itEnd, bool (&func)(const std::wstring&));
	private:
		std::vector<CElement*> arEquation;
	};
}

#endif // CSTARMATHPARS_H
