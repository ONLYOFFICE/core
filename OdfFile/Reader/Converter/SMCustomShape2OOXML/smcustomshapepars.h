#ifndef SMCUSTOMSHAPEPARS_H
#define SMCUSTOMSHAPEPARS_H
#include<iostream>
#include<iterator>
#include<cctype>
#include<string>
#include<vector>
#include<stack>
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"

namespace StarMathCustomShape
{
	class CElement;
	enum class TypeElement
	{
		//name class
		NumberOrName,
		ArithmeticOperation,
		Function,
		Bracket,
		//arithmetic sign
		plus,
		minus,
		division,
		multiplication,
		//function
		sqrt,
		abs,
		sin,
		cos,
		tan,
		min,
		max,
		If,
		atan,
		atan2,
		empty,
		comma,
	};
	class CSMReader
	{
	public:
		CSMReader(std::wstring& wsStarMath);
		~CSMReader();
		std::wstring GetElement(std::wstring::iterator& itStart,std::wstring::iterator& itEnd);
		CElement* ReadingElement();
		bool ReadingNextElement();
		bool CheckIteratorPosition();
		std::wstring GetElement();
		void ClearElement();
		void FindingTheEndOfTheBrackets();
		void RemovingTheParenthesisIterator();
		void SetDoubleSign(const bool& bDoubleSign);
		bool GetDoubleSign();
	private:
		std::wstring::iterator m_itStart;
		std::wstring::iterator m_itEnd,m_itEndForBrecket;
		std::stack<std::wstring::iterator> m_stEndBrecket;
		std::wstring m_wsElement;
		CElement* m_pElement;
		bool m_bDoubleSign;
	};
	class SMCustomShapePars
	{
	public:
		SMCustomShapePars();
		~SMCustomShapePars();
		void StartParsSMCustomShape(std::wstring& wsStarMath);
		static CElement* ParseElement(CSMReader* pReader);
		static void ParsString(CSMReader* pReader, std::vector<CElement*>& arVec);
		std::vector<CElement*>& GetVector();
	private:
		std::vector<CElement*> m_arVecElements;
	};
	class SMCustomShapeConversion
	{
	public:
		SMCustomShapeConversion();
		~SMCustomShapeConversion();
		void StartConversion(std::vector<CElement*>& arElements, const std::wstring& wsFormulaName = L"");
		std::wstring GetStringXml();
		static void WritingFormulaXml(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsNameFormula,const std::wstring& wsFormula);
		static std::wstring ParsFormulaName(const std::wstring& wsFormulaName, std::wstring& wsName);
	private:
		XmlUtils::CXmlWriter* m_pXmlWriter;
	};
	class CElement
	{
	public:
		CElement();
		virtual ~CElement();
		virtual void Parse(CSMReader* pReader) = 0;
		virtual void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsName = L"") = 0;
		static CElement* CreateElement(const std::wstring& wsElement);
		void SetBaseType(const TypeElement& enType);
		TypeElement GetBaseType();
		void SetNameFormula(const std::wstring& wsName);
		std::wstring GetNameFormula();
	private:
		TypeElement m_enBaseType;
		std::wstring m_wsNameFormula;
	};
	class CElementNumber:public CElement
	{
	public:
		CElementNumber();
		CElementNumber(const std::wstring& wsName);
		virtual ~CElementNumber();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsName = L"") override;
		static bool CheckNumber(const std::wstring& wsNumber);
		std::wstring GetString();
	private:
		std::wstring m_wsNumber;
	};
	class CElementArithmeticOperations:public CElement
	{
	public:
		CElementArithmeticOperations();
		CElementArithmeticOperations(const std::wstring& wsSign);
		virtual ~CElementArithmeticOperations();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring& wsName = L"") override;
		static bool CheckArithmeticOperators(const std::wstring& wsElement);
		bool ComparisonSign(const std::wstring& wsSign);
		bool ComparingPriorities(const std::wstring& wsSign);
		void SetFirstValue(CElement* pElement);
		static TypeElement SetTypeSign(const std::wstring& wsSign);
		TypeElement GetTypeSign();
		void SignRecording(XmlUtils::CXmlWriter* pXmlWriter, const TypeElement &enTypeSign);
		CElement* GetSecondValue();
		std::wstring ConversionValueSign(XmlUtils::CXmlWriter* pXmlWriter, CElement* pElement);
		void RecordingTheValuesSign(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsNameFirst, const std::wstring& wsNameSecond);
	private:
		TypeElement m_enTypeSign;
		CElement* m_pSecondSign;
		CElement* m_pFirstValue;
		CElement* m_pSecondValue;
		unsigned int m_uiNumberFormula;
	};
	class CElementBracket: public CElement
	{
	public:
		CElementBracket();
		virtual ~CElementBracket();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsName = L"") override;
		std::vector<CElement*> GetVector();
	private:
		std::vector<CElement*> m_arElements;
	};
	class CElementFunction: public CElement
	{
	public:
		CElementFunction();
		CElementFunction(const TypeElement& enType);
		virtual ~CElementFunction();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring& wsName = L"") override;
		void ConversionElement(XmlUtils::CXmlWriter* pXmlWriter,CElement* pElement, std::wstring& wsFormula);
		static TypeElement TypeCheckingByFunction(const std::wstring& wsFunction);
		void ConvertBracketsForTrigonometry(XmlUtils::CXmlWriter *pXmlWriter, std::wstring &wsFormula);
	private:
		TypeElement m_enTypeFunction;
		CElement* m_pValue;
		unsigned int m_uiNumberFormula;
	};
	class CElementComma: public CElement
	{
	public:
		CElementComma();
		virtual ~CElementComma();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring& wsName = L"") override;
	};
}
#endif // SMCUSTOMSHAPEPARS_H
