/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#include<iostream>
#include<iterator>
#include<cctype>
#include<string>
#include<vector>
#include<stack>
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"

namespace OdfCustomShape
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
		logwidth,
		logheight,
		empty,
		comma,
	};
	class CSMReader
	{
	public:
		CSMReader(const std::wstring& wsStarMath);
		~CSMReader();
		std::wstring GetElement(std::wstring::iterator& itStart, std::wstring::iterator& itEnd);
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
		std::wstring m_Formula;
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
		void StartParsSMCustomShape(const std::wstring& wsFormula);
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
		bool ComparisonSign(CElement* pElement);
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
		void CalculatingTheLogicalHeightOrWidth(XmlUtils::CXmlWriter* pXmlWriter, std::wstring& wsFormula);
		void CalculatingNumberName(std::wstring& wsFormula,std::wstring& wsNewName);
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
