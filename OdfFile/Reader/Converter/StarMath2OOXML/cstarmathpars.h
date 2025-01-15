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

#ifndef CSTARMATHPARS_H
#define CSTARMATHPARS_H
#include "typeselements.h"
#include "typeConversion.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <stack>
#include <queue>
#include <ctype.h>
#include <cwctype>
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"
#include "../../../../OOXML/Base/Unit.h" 

namespace StarMath
{
	class CStarMathReader;

	struct TBaseAttribute
	{
		TBaseAttribute():base_font_size(0),base_alignment(1),base_font_bold(false),base_font_italic(false){};
		unsigned int base_font_size;
		std::wstring base_font_name;
		unsigned int base_alignment;
		bool base_font_bold;
		bool base_font_italic;
	};
	
	struct TFormulaSize
	{
		TFormulaSize():m_iHeight(0),m_iWidth(0) {};
		TFormulaSize(const unsigned int& iHeight,const unsigned int& iwidth):m_iHeight(iHeight),m_iWidth(iwidth) {};
		float m_iHeight;
		float m_iWidth;
		void Zeroing()
		{
			this->m_iHeight = 0; 
			this->m_iWidth = 0;
		}
	};
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
		unsigned int GetAlignment();
		std::wstring GetColor();
		const std::wstring& GetFontName();
		bool EmptyColor();
		bool ParseFontAttribute(const TypeElement& enTypeFont,CStarMathReader* pReader);
		bool ParseColorAttribute(const std::wstring& wsToken,CStarMathReader* pReader);
		void SetSize(const unsigned int& iSize);
		void SetAlignment(const unsigned int& iAlignment);
		void SetBold();
		void SetItal();
		void SetPhantom();
		void SetStrike();
		bool SetColor(const TypeElement& enColor);
		void SetColor(const std::wstring& wsColor);
		bool SetFont(const TypeElement& enFont);
		void SetFontName(const std::wstring& wsNameFont);
		void SetParent();
		bool GetParent();
		bool CheckAttribute();
		unsigned int GetCount();
		static void ComparingAttributes(CAttribute* pAttributeParent,CAttribute* pAttributeChild);
		//checking an element for a number from 1 to 9 or from the letter A to F
		static bool CheckHexPosition(const wchar_t& cToken);
		bool CheckingForEmptiness();
		void AddRef();
		void Release();
	private:
		void RefundOfTheAmountRGB(CStarMathReader* pReader,const int& iRed, const int& iGreen, const int& iBlue);
		std::wstring m_wsColor,m_wsNameFont;
		bool m_bBold,m_bItal,m_bPhantom,m_bStrike,m_bParent;
		unsigned int m_iSize,m_iAlignment;
		unsigned int m_unCount;
	};
	//Сlass for working with tokens (reading, defining types, passing)
	class CStarMathReader
	{
	public:
		CStarMathReader(std::wstring::iterator& itStart, std::wstring::iterator& itEnd,const TypeConversion &enTypeConversion);
		~CStarMathReader();
		bool GetToken();
		//getting a subtype and setting the global type of a token to variables m_enUnderType and m_enGlobalType
		void SetTypesToken();
		void TokenProcessing(const std::wstring& wsToken = L"");
		TypeElement GetGlobalType();
		TypeElement GetLocalType();
		std::wstring GetLowerCaseString();
		std::wstring GetOriginalString();
		//clearing a variable m_wsToken
		void ClearReader();
		bool CheckIteratorPosition();
		bool EmptyString();
		void SetAttribute(CAttribute* pAttribute);
		CAttribute* GetAttribute();
		void SetBaseAttribute(const TBaseAttribute& pAttribute);
		CAttribute* GetBaseAttribute();
		//The function returns a Token from a string (the iterator pointer m_itStart is on the next element)
		std::wstring GetElement();
		wchar_t GetOneElement();
		//taking a token for a color in hex form
		std::wstring TakingElementForHex();
		//taking a token for a color in rgb form
		int TakingElementForRGB();
		void SetString(const std::wstring& wsToken);
		void FindingTheEndOfParentheses();
		void IteratorNullification();
		void SettingTheIteratorToTheClosingBracket();
		void ReadingTheNextToken();
		void SetMarkForUnar(const bool& bMark);
		bool GetMarkForUnar();
		void SetTypeConversion(const TypeConversion &enTypeCon);
		TypeConversion GetTypeConversion();
	private:
		bool CheckTokenForGetElement(const wchar_t& cToken);
		bool CheckIsalhpaForGetElement(const wchar_t& cToken,const wchar_t& cLastToken);
		bool m_bMarkForUnar;
		std::wstring::iterator m_itStart,m_itEnd;
		TypeElement m_enGlobalType,m_enUnderType;
		std::wstring m_wsLowerCaseToken,m_wsOriginalToken;
		CAttribute* m_pAttribute;
		CAttribute* m_pBaseAttribute;
		TypeConversion m_enTypeCon;
		std::stack<std::wstring::iterator> m_stBracket,m_stCloseBracket;
	};

	class CElement
	{
	public:
		CElement();
		CElement(const TypeElement& enTypeBase, const TypeConversion& enTypeConversion);
		virtual ~CElement();
		virtual void Parse(CStarMathReader* pReader) = 0;
		//The function creates the class we need (by determining the class type by a variable m_enGlobalType from the class CStarMathReader)
		static CElement* CreateElement(CStarMathReader* pReader);
		virtual void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) = 0;
		virtual void SetAttribute(CAttribute* pAttribute) = 0;
		virtual TFormulaSize GetSize() = 0;
		void SetBaseAttribute(CAttribute* pAttribute);
		void SetBaseType(const TypeElement& enType);
		CAttribute* GetAttribute();
		const TypeElement& GetBaseType();
		const TypeConversion& GetTypeConversion();
		void DeleteAttribute();
	private:
		CAttribute* m_pAttribute;
		TypeElement m_enBaseType;
		TypeConversion m_enTypeConversion;
	};

	class CElementIndex: public CElement
	{
	public:
		CElementIndex(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementIndex();
		void SetValueIndex(CElement* pElement);
		void SetLeftArg(CElement* pElement);
		CElement* GetValueIndex();
		CElement* GetLeftArg();
		static TypeElement GetIndex(const std::wstring& wsCheckToken);
		static bool GetUpperIndex(const TypeElement& enType);
		static bool GetLowerIndex(const TypeElement& enType);
		const TypeElement& GetType();
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		void ConversionOfIndicesToValue(XmlUtils::CXmlWriter* pXmlWrite);
		void ConversionOfIndicesAfterValue(XmlUtils::CXmlWriter* pXmlWrite);
		CElement* m_pValueIndex;
		CElement* m_pUpperIndex;
		CElement* m_pLowerIndex;
		CElement* m_pLsubIndex;
		CElement* m_pLsupIndex;
		CElement* m_pCsubIndex;
		CElement* m_pCsupIndex;
		CElement* m_pLeftArg;
		TypeElement m_enTypeIndex;
	};

	class  CElementString: public CElement
	{
	public:
		CElementString(const std::wstring& wsTokenString, const TypeConversion &enTypeConversion);
		virtual ~CElementString();
		void SetString(const std::wstring& wsTokenString);
		std::wstring GetString();
		static TypeElement  GetDigit(const std::wstring& wsCheckToken);
		static TypeElement GetWord(const std::wstring& wsToken);
		void SetAttribute(CAttribute* pAttribute) override;
	private:
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		std::wstring m_wsString;
	};

	class CElementBinOperator: public CElement
	{
	public:
		CElementBinOperator(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementBinOperator();
		void SetLeftArg(CElement* pElement);
		void SetRightArg(CElement* pElement);
		void SetTypeBinOP(const TypeElement& enType);
		CElement* GetRightArg();
		CElement* GetLeftArg();
		static TypeElement GetBinOperator(const std::wstring& wsToken);
		static void UnaryCheck(CStarMathReader* pReader,CElement* pLastElement);
		const TypeElement& GetType();
		//checking for signs such as -,+,-+,+-.
		static bool MixedOperators(const TypeElement& enType);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		bool IsBinOperatorLowPrior();
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pLeftArgument;
		CElement* m_pRightArgument;
		TypeElement m_enTypeBinOp;
	};

	class CElementOperator: public CElement
	{
	public:
		CElementOperator(const TypeElement& enType, const TypeConversion &enTypeConversion ,const std::wstring& wsNameOp = L"");
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
		TFormulaSize GetSize() override;
		CElement* m_pValueOperator;
		CElement* m_pValueFrom;
		CElement* m_pValueTo;
		CElement* m_pUpperIndex;
		CElement* m_pLowerIndex;
		TypeElement m_enTypeOperator;
		std::wstring m_wsName;
	};

	class CElementGrade: public CElement
	{
	public:
		CElementGrade(const TypeConversion &enTypeConversion);
		virtual ~CElementGrade();
		void SetValueGrade(CElement* pElement);
		void SetValueFrom(CElement* pElement);
		void SetValueTo(CElement* pElement);
		static TypeElement GetGrade(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pValueGrade;
		CElement* m_pValueFrom;
		CElement* m_pValueTo;
	};

	class CElementBracket: public CElement
	{
	public:
		CElementBracket(const TypeElement& enType,const TypeConversion &enTypeConversion,const bool& bScalability = false);
		virtual ~CElementBracket();
		void SetBracketValue(const std::vector<CElement*>& arValue);
		static TypeElement GetBracketOpen(const std::wstring& wsToken);
		static TypeElement GetBracketClose(const std::wstring& wsToken);
		std::vector<CElement*> GetBracketValue();
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		bool CheckMline(CElement* pElement);
		std::wstring DefiningBracket(const TypeElement& enTypeBracket);
		TypeElement m_enTypeBracket,m_enLeftBracket,m_enRightBracket;
		std::vector<CElement*> m_arBrecketValue;
		bool m_bScalability;
	};

	class CElementBracketWithIndex: public CElement
	{
	public:
		CElementBracketWithIndex(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementBracketWithIndex();
		void SetLeftArg(CElement* pElement);
		void SetBracketValue(CElement* pElement);
		CElement* GetLeftArg();
		static TypeElement GetBracketWithIndex(const std::wstring& wsToken);
		const TypeElement& GetType();
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pLeftArg;
		CElement* m_pValue;
		TypeElement m_enTypeBracketWithIndex;
	};

	class CElementSetOperations: public CElement
	{
	public:
		CElementSetOperations(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementSetOperations();
		void SetLeftArg(CElement* pElement);
		CElement* GetLeftArg();
		void SetRightArg(CElement* pElement);
		CElement* GetRightArg();
		static TypeElement GetSetOperation(const std::wstring& wsToken);
		const TypeElement& GetType();
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pLeftArgument;
		CElement* m_pRightArgument;
		TypeElement m_enTypeSet;
	};

	class CElementConnection: public CElement
	{
	public:
		CElementConnection(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementConnection();
		void SetRightArg(CElement* pElement);
		CElement* GetRightArg();
		void SetLeftArg(CElement* pElement);
		CElement* GetLeftArg();
		static TypeElement GetConnection(const std::wstring& wsToken);
		const TypeElement& GetType();
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pLeftArgument;
		CElement* m_pRightArgument;
		TypeElement m_enTypeCon;
	};

	class CElementFunction: public CElement
	{
	public:
		CElementFunction(const TypeElement& enType, const TypeConversion &enTypeConversion,const std::wstring& wsNameFunc = L"");
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
		TFormulaSize GetSize() override;
		CElement* m_pValue;
		CElement* m_pIndex;
		std::wstring m_wsNameFunc;
		TypeElement m_enTypeFunction;
	};

	class CElementSpecialSymbol: public CElement
	{
	public:
		CElementSpecialSymbol(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementSpecialSymbol();
		static TypeElement GetSpecialSymbol(std::wstring& wsToken);
		void SetValue(CElement* pValue);
		const TypeElement GetType();
	private:
		void SetTypeSymbol();
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pValue;
		TypeElement m_enTypeSpecial;
		std::wstring m_wsType;
	};

	class CElementMatrix: public CElement
	{
	public:
		CElementMatrix(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementMatrix();
		void SetFirstArgument(CElement* pElement);
		void SetSecondArgument(CElement* pElement);
		static TypeElement GetMatrix(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader *pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pFirstArgument;
		CElement* m_pSecondArgument;
		TypeElement m_enTypeMatrix;
	};

	class CElementDiacriticalMark: public CElement
	{
	public:
		CElementDiacriticalMark(const TypeElement& enType,const TypeConversion &enTypeConversion);
		virtual ~CElementDiacriticalMark();
		void SetValueMark(CElement* pValue);
		static TypeElement GetMark(const std::wstring& wsToken);
	private:
		void SetAttribute(CAttribute* pAttribute) override;
		void Parse(CStarMathReader* pReader) override;
		void ConversionToOOXML(XmlUtils::CXmlWriter* pXmlWrite) override;
		TFormulaSize GetSize() override;
		CElement* m_pValueMark;
		TypeElement m_enTypeMark;
	};

	class CParserStarMathString
	{
	public:
		CParserStarMathString();
		~CParserStarMathString();
		std::vector<CElement*> Parse(std::wstring& wsParseString,int iTypeConversion = 0);
		static CElement* ParseElement(CStarMathReader* pReader);
		//Function for adding a left argument (receives the argument itself and the element to which it needs to be added as input. Works with classes:CElementBinOperator,CElementConnection,CElementSetOperation).
		static bool AddLeftArgument(CElement* pLeftArg,CElement* pElementWhichAdd,CStarMathReader* pReader);
		static bool CheckForLeftArgument(const TypeElement& enType, const bool& bConnection = true);
		static CElement* ReadingWithoutBracket(CStarMathReader* pReader,const bool& bConnection = true);
		//checking the element (true if it is newline)
		static bool CheckNewline(CElement* pElement);
		static bool CheckGrid(CElement* pElement);
		//adding an element to the array, checking that it is not empty and adding the left element, if there is one.
		static void AddingAnElementToAnArray(std::vector<CElement*>& arrEquation,CElement* pAddElement,CStarMathReader* pReader);
		//Receives the left element as input, reads the next one, if the next element has a higher priority and contains the left element, the element received at the input is passed to it. The entire structure is saved and returned.
		static void ReadingElementsWithPriorities(CStarMathReader* pReader,CElement*& pLeftElement);
		//method for parsing indexes with attributes. If there is an attribute present when indexes are read, then all subsequent indexes are applied to the index with the attribute.
		static void ReadingElementsWithAttributes(CStarMathReader* pReader,CElement*& pSavingElement);
		static void ParsElementAddingToArray(CStarMathReader* pReader, std::vector<CElement*>& arElements);
		void SetAlignment(const unsigned int& iAlignment);
		const unsigned int& GetAlignment();
		void SetBaseFont(const std::wstring& wsNameFont);
		void SetBaseSize(const unsigned int& iSize);
		void SetBaseAlignment(const unsigned int& iAlignment);
		void SetBaseItalic(const bool& bItal);
		void SetBaseBold(const bool& bBold);
		std::queue<TFormulaSize> GetFormulaSize();
		static void ComparisonByHeight(TFormulaSize& tLeftSize,const TFormulaSize&  tRightSize);
		static void ComparisonByWidth(TFormulaSize& tLeftSize, const TFormulaSize& tRightSize);
		static std::wstring ConvertToLowerCase(const std::wstring& wsToken);
	private:
		TBaseAttribute m_stBaseAttribute;
		std::vector<CElement*> m_arEquation;
		unsigned int m_iAlignment;
		std::queue<TFormulaSize> m_qSize;
	};
}

#endif // CSTARMATHPARS_H
