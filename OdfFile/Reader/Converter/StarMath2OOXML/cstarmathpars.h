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
		Attribute,
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
		brace,
		round,
		square,
		ldbracket,
		lbrace,
		langle,
		lceil,
		lfloor,
		lline,
		ldline,
	};
	enum TypeAttribute
	{
		acute,
		breve,
		dot,
		dddot,
		vec,
		tilde,
		check,
		grave,
		circle,
		ddot,
		bar,
		harpoon,
		hat,
		widevec,
		widetilde,
		overline,
		overstrike,
		wideharpoon,
		widehat,
		underline,//top elements
		phantom,
		bold,
		ital,//properties (without a pin and headset)
		black,
		green,
		aqua,
		yellow,
		lime,
		navy,
		purple,
		teal,
		blue,
		red,
		fuchsia,
		gray,
		maroon,
		olive,
		silver,
		coral,
		midnightblue,
		crimson,
		violet,//color(without rgb and hex)
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
	class CBinaryOperator: public CElement
	{
	public:
		CBinaryOperator();
		CBinaryOperator(const TypeBinOperator& enType);
		virtual ~CBinaryOperator();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		TypeBinOperator GetTypeBin();
		void SetTypeBin(const TypeBinOperator& enType);
		void SetLeftArg(CElement* oLeftArg);
		void SetRightArg(CElement* oRightArg);
	private:
		TypeBinOperator enTypeBinOp;
		CElement* arLeftArg;
		CElement* arRightArg;
	};
	class COperator: public CElement
	{
	public:
		COperator();
		virtual ~COperator();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		TypeOperator GetTypeOp();
		void SetTypeOp(const TypeOperator& enType);
		void SetFrom(CElement* oFrom);
		void SetTo(CElement* oTo);
		void SetValueOp(CElement* oValue);
		void AvailabilityMline();
	private:
		TypeOperator enTypeOp;
		CElement* oFromValue{nullptr};
		CElement* oToValue{nullptr};
		CElement* oValueOp{nullptr};
		bool bMline{false};
	};
	class CBracket: public CElement
	{
	public:
		CBracket();
		CBracket(const std::vector<CElement*>& arValue,const TypeBracket& enType);
		virtual ~CBracket();
		std::wstring GetValue() override;
		TypeElement GetType() override;
	private:
		TypeBracket enTypeBracket;
		std::vector<CElement*> arBrecketVal;
	};
	class CAttribute: public CElement
	{
	public:
		CAttribute();
		CAttribute(CElement* oValue,const TypeAttribute& enType);
		virtual ~CAttribute();
		std::wstring GetValue() override;
		TypeElement GetType() override;
		void SetTypeAtt(const TypeAttribute& enType);
		void SetValueAtt(CElement* oValue);
	private:
		TypeAttribute enTypeAtt;
		CElement* oValueAtt;
	};
	class CStarMathPars
	{
	public:
		CStarMathPars();
		virtual ~CStarMathPars();
		void Pars(std::wstring& wsStarMathLine);
		std::wstring GetElement(std::wstring::iterator& itFirst,std::wstring::iterator& itEnd);
		CElement* ParsElement(std::wstring::iterator& itFirst, std::wstring::iterator& itEnd, std::vector<CElement*>& arParsLine);
		bool CheckDigit(const std::wstring& wsCheckToken);
		bool CheckUnarSign(std::wstring& wsCheckToken,CUnarySign& oUnarSign);
		bool CheckBinOperator(const std::wstring& wsCheckToken,TypeBinOperator& enTypeBinOperator);
		bool CheckOneElementBinOperator(const char& wsCheckToken, TypeBinOperator& enTypeBinOperator);
		bool CheckOperator(const std::wstring& wsCheckToken,TypeOperator& enTypeOperator);
		bool CheckIndex(const std::wstring& wsCheckToken);
		bool CheckBracket(const char& wsCheckToken,TypeBracket& enTypeBracket);
		bool CheckScalable_NotScalableBracket(const std::wstring& wsCheckToken, TypeBracket& enType);
		bool CheckTopAttribute(const std::wstring& wsCheckToken,TypeAttribute& enTypeAtt);
		bool CheckPropertiesAttribute(const std::wstring& wsCheckToken,TypeAttribute& enTypeAtt);
		bool CheckColorAttribute(const std::wstring& wsCheckToken, TypeAttribute& enTypeAtt);
		void PrintAr();
	private:
		std::vector<CElement*> m_arParsLine;
	};
}

#endif // CSTARMATHPARS_H
