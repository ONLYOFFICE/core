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
		CAttribute();
		CAttribute(const TypeAttributeTop& enType);
		virtual ~CAttribute();
		void SetTypeAtt(const std::wstring& wsCheckToken);
		TypeAttributeTop GetTypeAtt();
	private:
		TypeAttributeTop enTypeTop{noneTop};
		TypeAttributeColor enTypeColor{noneColor};
		bool bBold{false};
		bool bItal{false};
		bool bPhantom{false};
	};

	class CElement
	{
	public:
		virtual ~CElement();
		virtual std::wstring GetValue() = 0;
		virtual TypeElement GetType() = 0;
		void SetAttribute(CAttribute* m_oCAttribute);
		TypeAttributeTop GetTypeAttribute();
	private:
		CAttribute* oCAttribute;
	};
	class CSpecialSymbol: public CElement
	{
	public:
		CSpecialSymbol(const std::wstring& wsToken);
		virtual ~CSpecialSymbol();
		std::wstring GetValue() override;
		TypeElement GetType() override;
	private:
		TypeSymbol enTypeSpecial;
	};
	class CNumber: public CElement
	{
	public:
		CNumber();
		CNumber(const std::wstring& wsValue);
		virtual ~CNumber();
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

	class CArgumentContainer: public CElement
	{
	public:
		CArgumentContainer();
		virtual ~CArgumentContainer();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		void SetArgument(CElement* oValue);
		CElement* GetArgument();
	private:
		CElement* oArgument;
	};

	class CBinaryOperator: public CArgumentContainer
	{
	public:
		CBinaryOperator();
		CBinaryOperator(const std::wstring& wsToken);
		virtual ~CBinaryOperator();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		TypeBinOperator GetTypeBin();
		void SetRightArg(CElement* oArgument);
		void SetTypeBin(const TypeBinOperator& enType);
		CElement* GetRightArg();
		TypeElement GetTypeRight();
	private:
		CElement* oRightArg;
		TypeBinOperator enTypeBinOp;
	};
	class COperator: public CElement
	{
	public:
		COperator();
		COperator(const std::wstring& wsToken);
		virtual ~COperator();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		TypeOperator GetTypeOp();
		void SetTypeOp(const TypeOperator& enType);
		void SetFrom(CElement* oFrom);
		void SetTo(CElement* oTo);
		void SetValueOp(CElement* oValue);
		CElement* GetValueOp();
		CElement* GetFrom();
		CElement* GetTo();
	private:
		TypeOperator enTypeOp;
		CElement* oFromValue{nullptr};
		CElement* oToValue{nullptr};
		CElement* oValueOp{nullptr};
	};
	class CBracket: public CElement
	{
	public:
		CBracket();
		CBracket(const std::vector<CElement*>& arValue,const std::wstring& wsCheckToken);
		virtual ~CBracket();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		void SetScalable();
		void SetBracketVal(const std::vector<CElement*>& arBrecketValue);
		void SetTypeBracket(const std::wstring& wsCheckToken);
	private:
		bool bScalable{false};
		TypeBracket enTypeBracket;
		std::vector<CElement*> arBrecketVal;
	};
	class CFunction: public CArgumentContainer
	{
	public:
		CFunction();
		virtual ~CFunction();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		TypeFunction GetTypeFun();
		void SetTypeFunction(const std::wstring& wsCheckToken);
	private:
		TypeFunction enTypeFun;
	};
	class CIndex: public CArgumentContainer
	{
	public:
		CIndex(const std::wstring& wsToken);
		virtual ~CIndex();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		void SetIndex(CElement* oValueIndex);
		CElement* GetIndex();
	private:
		TypeIndex enTypeIn;
		CElement* oIndex;
	};
	class CMatrix: public CArgumentContainer
	{
	public:
		CMatrix(const std::wstring& wsToken);
		virtual	~CMatrix();
		std::wstring GetValue() override;
		TypeElement GetType() override;
	private:
		TypeMatrix enTypeMatrix;
	};
	class CStarMathPars
	{
	public:
		CStarMathPars();
		virtual ~CStarMathPars();
		std::vector<CElement*> GetVector();
		void Pars(std::wstring& wsStarMathLine);
		std::wstring GetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd);
		CElement* ParsElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd, std::vector<CElement*>& arParsLine);
		CElement* ReadWithoutBrackets(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd,std::vector<CElement*>& arParsLine);
		bool CheckDigit(const std::wstring& wsCheckToken);
		//bool CheckUnarSign(std::wstring& wsCheckToken,CUnarySign& oUnarSign);
		static bool CheckBinOperator(const std::wstring& wsCheckToken);
		static bool CheckBinOperatorLowPriority(const std::wstring& wsCheckToken);
		static bool CheckPlusOrMinus(const std::wstring& wsCheckToken);
		bool CheckOperator(const std::wstring& wsCheckToken);
		static bool CheckIndex(const std::wstring& wsCheckToken);
		bool CheckIndexOp(const std::wstring& wsCheckToken);
		bool CheckBracketOpen(const std::wstring& wsCheckToken);
		static bool CheckBracketClose(const std::wstring& wsCheckToken);
		static bool CheckScalable_NotScalableBracketLeft(const std::wstring& wsCheckToken);
		static bool CheckScalable_NotScalableBracketRight(const std::wstring& wsCheckToken);
		bool CheckTopAttribute(const std::wstring& wsCheckToken);
		bool CheckPropertiesAttribute(const std::wstring& wsCheckToken);
		static bool CheckColorAttribute(const std::wstring& wsCheckToken);
		bool CheckSpecialCharacter(const std::wstring& wsCheckToken);
		bool CheckSpecialOperation(const std::wstring& wsCheckToken);//doing first
		bool CheckSpecialConnection(const std::wstring& wsCheckToken);// doint first
		bool CheckingTheNextElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd,bool (&func)(const std::wstring& wsCheckToken));
		bool CheckFunction(const std::wstring& wsCheckToken);
		bool CheckOperation(const std::wstring& wsCheckToken);
		bool CheckMatrix(const std::wstring& wsCheckToken);
		void PrintAr();
	private:
		std::vector<CElement*> m_arParsLine;
	};
}

#endif // CSTARMATHPARS_H
