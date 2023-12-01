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
	private:
		//The function returns a Token from a string (the iterator pointer m_itStart is on the next element)
		std::wstring GetElement();
		std::wstring::iterator m_itStart,m_itEnd;
		TypeElement m_enGlobalType;
		TypeElement m_enUnderType;
		std::wstring m_wsToken;
	};

	class CAttribute
	{
	public:
		CAttribute(const TypeElement& enType);
		~CAttribute();
		static TypeElement GetTypeAttribute(const std::wstring& wsToken);
		TypeElement GetType();
	private:
		TypeElement m_enTypeAttr;
	};

	class CElement
	{
	public:
		CElement();
		virtual ~CElement();
		virtual void Parse(CStarMathReader* pReader) = 0;
		//The function creates the class we need (by determining the class type by a variable m_enGlobalType from the class CStarMathReader)
		static CElement* CreateElement(CStarMathReader* pReader);
		virtual void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) = 0;
		void SetAttribute(const std::vector<CAttribute*> arAttr);
		void SetBaseType(const TypeElement& enType);
		const TypeElement& GetBaseType();
	private:
		std::vector<CAttribute*> m_arElementAttributes;
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
		static TypeElement GetIndex(const std::wstring& wsCheckToken);
	private:
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pValueIndex;
		CElement* m_pValue;
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
	private:
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite) override;
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
		CElementOperator(const TypeElement& enType);
		virtual ~CElementOperator();
		void SetValueOperator(CElement* pElement);
		CElement* GetValueOperator();
		void SetFromValue(CElement* pElement);
		CElement* GetFromValue();
		void SetToValue(CElement* pElement);
		CElement* GetToValue();
		static TypeElement GetOperator(const std::wstring& wsToken);
		static TypeElement GetFromOrTo(const std::wstring& wsToken);
	private:
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite) override;
		CElement* m_pValueOperator;
		CElement* m_pValueFrom;
		CElement* m_pValueTo;
		TypeElement m_enTypeOperator;
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
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;//
		TypeElement GetBracketClose(const std::wstring& wsToken);
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
		static TypeElement GetBracketWithIndex(const std::wstring& wsToken);
	private:
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
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pLeftArgument;
		CElement* m_pRightArgument;
		TypeElement m_enTypeCon;
	};

	class CElementFunction: public CElement
	{
	public:
		CElementFunction(const TypeElement& enType);
		virtual ~CElementFunction();
		void SetValueFunction(CElement* pElement);
		CElement* GetValueFunction();
		static TypeElement GetFunction(const std::wstring& wsToken);
	private:
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pValue;
		TypeElement m_enTypeFunction;
	};

	class CElementSpecialSymbol: public CElement
	{
	public:
		CElementSpecialSymbol(const TypeElement& enType);
		virtual ~CElementSpecialSymbol();
		static TypeElement GetSpecialSymbol(const std::wstring& wsToken);
	private:
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TypeElement m_enTypeSpecial;
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
		void Parse(CStarMathReader *pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		CElement* m_pFirstArgument;
		CElement* m_pSecondArgument;
		TypeElement m_enTypeMatrix;
	};

	class CParserStarMathString
	{
	public:
		std::vector<CElement*> Parse(std::wstring& wsParseString);
		static CElement* ParseElement(CStarMathReader* pReader);
		//Function for adding a left argument (receives the argument itself and the element to which it needs to be added as input. Works with classes:CElementBinOperator,CElementConnection,CElementSetOperation).
		static void AddLeftArgument(CElement* pLeftArg,CElement* pElementWhichAdd);
		static bool CheckForLeftArgument(const TypeElement& enType);
		static CElement* ReadingWithoutBracket(CStarMathReader* pReader);
	private:
		std::vector<CElement*> m_arEquation;
	};
}

#endif // CSTARMATHPARS_H
