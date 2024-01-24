#ifndef CSTARMATHPARS_H
#define CSTARMATHPARS_H
#include "typeselements.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <stack>
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"

namespace StarMath
{
	class CStarMathReader;

	class CAttribute
	{
	public:
		CAttribute();
		~CAttribute();
		static TypeElement GetTypeColorAttribute(const std::wstring& wsToken);
		static TypeElement GetTypeFontAttribute(const std::wstring& wsToken);
		bool GetBold();
		bool GetItal();
		bool GetPhantom();
		bool GetStrike();
		unsigned int GetSize();
		std::wstring GetColor();
		bool EmptyColor();
		void ParseFontAttribute(const TypeElement& enTypeFont,CStarMathReader* pReader);
		void ParseColorAttribute(const std::wstring& wsToken,CStarMathReader* pReader);
		void SetSize(const unsigned int& iSize);
		void SetBold();
		void SetItal();
		void SetPhantom();
		void SetStrike();
		void SetColor(const TypeElement& enColor);
		void SetColor(const std::wstring& wsColor);
		void SetFont(const TypeElement& enFont);
	private:
		std::wstring m_wsColor;
		bool m_bBold;
		bool m_bItal;
		bool m_bPhantom;
		bool m_bStrike;
		unsigned int m_iSize;
		std::wstring m_wsNameFont;
	};
	//Сlass for working with tokens (reading, defining types, passing)
	class CStarMathReader
	{
	public:
		CStarMathReader(std::wstring::iterator& itStart, std::wstring::iterator& itEnd);
		~CStarMathReader();
		void GetToken();
		//getting a subtype and setting the global type of a token to variables m_enUnderType and m_enGlobalType
		void SetTypesToken();
		//void SkipNextElement();
		TypeElement GetGlobalType();
		TypeElement GetLocalType();
		std::wstring GetString();
		//clearing a variable m_wsToken
		void ClearReader();
		bool CheckIteratorPosition();
		bool EmptyString();
		void SetAttribute(CAttribute* pAttribute);
		CAttribute* GetAttribute();
		//The function returns a Token from a string (the iterator pointer m_itStart is on the next element)
		std::wstring GetElement();
		void FindingTheEndOfParentheses();
	private:
		bool CheckTokenForGetElement(const wchar_t& cToken);
		bool CheckIsalhpaForGetElement(const wchar_t& cToken,const wchar_t& cLastToken);
		std::wstring::iterator m_itStart,m_itEnd,m_itEndBracket;
		TypeElement m_enGlobalType;
		TypeElement m_enUnderType;
		std::wstring m_wsToken;
		CAttribute* m_pAttribute;
	};

	class CElement
	{
	public:
		CElement();
		CElement(const TypeElement& enTypeBase);
		virtual ~CElement();
		virtual void Parse(CStarMathReader* pReader) = 0;
		//The function creates the class we need (by determining the class type by a variable m_enGlobalType from the class CStarMathReader)
		static CElement* CreateElement(CStarMathReader* pReader);
		virtual void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) = 0;
		virtual void SetAttribute(CAttribute* pAttribute) = 0;
		void SetBaseAttribute(CAttribute* pAttribute);
		void SetBaseType(const TypeElement& enType);
		CAttribute* GetAttribute();
		const TypeElement& GetBaseType();
	private:
		CAttribute* m_pAttribute;
		TypeElement m_enBaseType;
	};

	class CElementIndex: public CElement
	{
	public:
		CElementIndex(const TypeElement& enType);
		virtual ~CElementIndex();
		void SetValueIndex(CElement* pElement);
		void SetLeftArg(CElement* pElement);
		CElement* GetValueIndex();
		CElement* GetLeftArg();
		static TypeElement GetIndex(const std::wstring& wsCheckToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pValueIndex;
		CElement* m_pLeftArg;
		TypeElement m_enTypeIndex;
	};

	class CElementString: public CElement
	{
	public:
		CElementString(const std::wstring& wsTokenString);
		virtual ~CElementString();
		void SetString(const std::wstring& wsTokenString);
		std::wstring GetString();
		static TypeElement GetDigit(const std::wstring& wsCheckToken);
		static TypeElement GetWord(const std::wstring& wsToken);
		void SetAttribute(CAttribute* pAttribute) override;
	private:
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		std::wstring m_wsString;
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
		static TypeElement GetBinOperator(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		bool IsBinOperatorLowPrior();
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite) override;
		CElement* m_pLeftArgument;
		CElement* m_pRightArgument;
		TypeElement m_enTypeBinOp;
	};

	class CElementOperator: public CElement
	{
	public:
		CElementOperator(const TypeElement& enType,const std::wstring& wsNameOp = L"");
		virtual ~CElementOperator();
		void SetValueOperator(CElement* pElement);
		CElement* GetValueOperator();
		void SetFromValue(CElement* pElement);
		CElement* GetFromValue();
		void SetToValue(CElement* pElement);
		CElement* GetToValue();
		void SetName(const std::wstring& wsNameOp);
		std::wstring GetName();
		static TypeElement GetOperator(const std::wstring& wsToken);
		static TypeElement GetFromOrTo(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute);
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite) override;
		CElement* m_pValueOperator;
		CElement* m_pValueFrom;
		CElement* m_pValueTo;
		TypeElement m_enTypeOperator;
		std::wstring m_wsName;
	};

	class CElementGrade: public CElement
	{
	public:
		CElementGrade();
		virtual ~CElementGrade();
		void SetValueGrade(CElement* pElement);
		void SetValueFrom(CElement* pElement);
		void SetValueTo(CElement* pElement);
		static TypeElement GetGrade(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pValueGrade;
		CElement* m_pValueFrom;
		CElement* m_pValueTo;
	};

	class CElementBracket: public CElement
	{
	public:
		CElementBracket(const TypeElement& enType);
		virtual ~CElementBracket();
		void SetBracketValue(const std::vector<CElement*>& arValue);
		static TypeElement GetBracketOpen(const std::wstring& wsToken);
		std::vector<CElement*> GetBracketValue();
	private:
		TypeElement GetBracketClose(const std::wstring& wsToken);
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;//
		TypeElement m_enTypeBracket;
		std::vector<CElement*> m_arBrecketValue;
	};

	class CElementBracketWithIndex: public CElement
	{
	public:
		CElementBracketWithIndex(const TypeElement& enType);
		virtual ~CElementBracketWithIndex();
		void SetLeftArg(CElement* pElement);
		void SetBracketValue(CElement* pElement);
		CElement* GetLeftArg();
		static TypeElement GetBracketWithIndex(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pLeftArg;
		CElement* m_pValue;
		TypeElement m_enTypeBracketWithIndex;
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
		static TypeElement GetSetOperation(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pLeftArgument;
		CElement* m_pRightArgument;
		TypeElement m_enTypeSet;
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
		static TypeElement GetConnection(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pLeftArgument;
		CElement* m_pRightArgument;
		TypeElement m_enTypeCon;
	};

	class CElementFunction: public CElement
	{
	public:
		CElementFunction(const TypeElement& enType, const std::wstring& wsNameFunc = L"");
		virtual ~CElementFunction();
		void SetValueFunction(CElement* pElement);
		CElement* GetValueFunction();
		void SetNameFunc(const std::wstring& wsNameFunc);
		std::wstring GetNameFuncInString();
		static TypeElement GetFunction(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pValue;
		std::wstring m_wsNameFunc;
		TypeElement m_enTypeFunction;
	};

	class CElementSpecialSymbol: public CElement
	{
	public:
		CElementSpecialSymbol(const TypeElement& enType);
		virtual ~CElementSpecialSymbol();
		static TypeElement GetSpecialSymbol(std::wstring& wsToken);
		void SetValue(CElement* pValue);
		const TypeElement GetType();
	private:
		void SetTypeSymbol();
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pValue;
		TypeElement m_enTypeSpecial;
		std::wstring m_wsType;
	};

	class CElementMatrix: public CElement
	{
	public:
		CElementMatrix(const TypeElement& enType);
		virtual ~CElementMatrix();
		void SetFirstArgument(CElement* pElement);
		void SetSecondArgument(CElement* pElement);
		static TypeElement GetMatrix(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader *pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pFirstArgument;
		CElement* m_pSecondArgument;
		TypeElement m_enTypeMatrix;
	};

	class CElementDiacriticalMark: public CElement
	{
	public:
		CElementDiacriticalMark(const TypeElement& enType);
		virtual ~CElementDiacriticalMark();
		void SetValueMark(CElement* pValue);
		static TypeElement GetMark(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pValueMark;
		TypeElement m_enTypeMark;
	};

	class CParserStarMathString
	{
	public:
		std::vector<CElement*> Parse(std::wstring& wsParseString);
		static CElement* ParseElement(CStarMathReader* pReader);
		//Function for adding a left argument (receives the argument itself and the element to which it needs to be added as input. Works with classes:CElementBinOperator,CElementConnection,CElementSetOperation).
		static bool AddLeftArgument(CElement* pLeftArg,CElement* pElementWhichAdd);
		static bool CheckForLeftArgument(const TypeElement& enType);
		static CElement* ReadingWithoutBracket(CStarMathReader* pReader);
	private:
		std::vector<CElement*> m_arEquation;
	};
}

#endif // CSTARMATHPARS_H
