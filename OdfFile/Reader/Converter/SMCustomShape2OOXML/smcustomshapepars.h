#ifndef SMCUSTOMSHAPEPARS_H
#define SMCUSTOMSHAPEPARS_H
#include<iostream>
#include<iterator>
#include<cctype>
#include<vector>
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
		void StartConversion(std::vector<CElement*>& arElements);
		std::wstring GetStringXml();
	private:
		XmlUtils::CXmlWriter* m_pXmlWriter;
	};
	class CElement
	{
	public:
		CElement();
		virtual ~CElement();
		virtual void Parse(CSMReader* pReader) = 0;
		virtual void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter) = 0;
		static CElement* CreateElement(const std::wstring& wsElement);
		void SetBaseType(const TypeElement& enType);
		TypeElement GetBaseType();
	private:
		TypeElement m_enBaseType;
	};
	class CElementNumber:public CElement
	{
	public:
		CElementNumber();
		CElementNumber(const std::wstring& wsName);
		virtual ~CElementNumber();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter) override;
		static bool CheckNumber(const std::wstring& wsNumber);
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
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter) override;
		static bool CheckArithmeticOperators(const std::wstring& wsElement);
		bool ComparisonSign(const std::wstring& wsSign);
		bool ComparingPriorities(const std::wstring& wsSign);
		void SetFirstValue(CElement* pElement);
		static TypeElement SetTypeSign(const std::wstring& wsSign);
		TypeElement GetTypeSign();
		void SignRecording(XmlUtils::CXmlWriter* pXmlWriter, const TypeElement &enTypeSign);
		CElement* GetSecondValue();
		std::wstring ConversionValueSign(XmlUtils::CXmlWriter* pXmlWriter, CElement* pElement);
	private:
		TypeElement m_enTypeSign;
		CElement* m_pSecondSign;
		CElement* m_pFirstValue;
		CElement* m_pSecondValue;
	};
	class CElementBracket: public CElement
	{
	public:
		CElementBracket();
		virtual ~CElementBracket();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter) override;
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
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter) override;
		static TypeElement TypeCheckingByFunction(const std::wstring& wsFunction);
	private:
		TypeElement m_enTypeFunction;
		CElement* m_pValue;
	};
	class CElementComma: public CElement
	{
	public:
		CElementComma();
		virtual ~CElementComma();
		void Parse(CSMReader* pReader) override;
		void ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter) override;
	};
}
#endif // SMCUSTOMSHAPEPARS_H
