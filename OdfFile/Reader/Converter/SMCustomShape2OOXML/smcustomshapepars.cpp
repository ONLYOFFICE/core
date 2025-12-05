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
#include "smcustomshapepars.h"

namespace OdfCustomShape
{
	SMCustomShapePars::SMCustomShapePars()
	{}
	SMCustomShapePars::~SMCustomShapePars()
	{
		for(CElement* pElement:m_arVecElements)
			delete pElement;
	}
	void SMCustomShapePars::StartParsSMCustomShape(const std::wstring & wsFormula, const std::wstring& wsLeft,const std::wstring& wsTop,const std::wstring& wsRight,const std::wstring& wsBottom)
	{
		CSMReader* pReader = new CSMReader(wsFormula,wsLeft,wsTop,wsRight,wsBottom);
		SMCustomShapePars::ParsString(pReader, m_arVecElements);
		return;
	}
	CElement* SMCustomShapePars::ParseElement(CSMReader* pReader)
	{
		CElement* pElement(nullptr);
		if(!pReader->GetElement().empty())
			pElement = CElement::CreateElement(pReader->GetElement());
		else
			pElement = pReader->ReadingElement();
		if(pElement != nullptr)
			pElement->Parse(pReader);
		return pElement;
	}
	void SMCustomShapePars::ParsString(CSMReader* pReader, std::vector<CElement*>& arVec)
	{
		while(!pReader->CheckIteratorPosition())
		{
			CElement* pElement = SMCustomShapePars::ParseElement(pReader);
			if(pElement != nullptr)
			{
				if(pElement->GetBaseType() == TypeElement::ArithmeticOperation && !arVec.empty() && arVec.back()->GetBaseType() != TypeElement::comma)
				{
					CElementArithmeticOperations* pSign = static_cast<CElementArithmeticOperations*>(pElement);
					pSign->SetFirstValue(arVec.back());
					arVec.pop_back();
				}
				arVec.push_back(pElement);
			}
		}
		if(!pReader->GetElement().empty())
		{
			CElement* pElement = SMCustomShapePars::ParseElement(pReader);
			if(pElement != nullptr)
			{
				if(pElement->GetBaseType() == TypeElement::ArithmeticOperation && !arVec.empty())
				{
					CElementArithmeticOperations* pSign = static_cast<CElementArithmeticOperations*>(pElement);
					pSign->SetFirstValue(arVec.back());
					arVec.pop_back();
				}
				arVec.push_back(pElement);
			}
		}
	}
	std::vector<CElement*>& SMCustomShapePars::GetVector()
	{
		return m_arVecElements;
	}
//class CElemnt
	CElement::CElement():m_wsNameFormula(L"")
	{}
	CElement::~CElement()
	{}
	CElement *CElement::CreateElement(const std::wstring& wsElement)
	{
		if(CElementNumber::CheckNumber(wsElement))
			return new CElementNumber(wsElement);
		else if(CElementArithmeticOperations::CheckArithmeticOperators(wsElement) || wsElement == L"--")
			return new CElementArithmeticOperations(wsElement);
		else if(wsElement == L"(")
			return new CElementBracket();
		else if(wsElement == L",")
			return new CElementComma();
		TypeElement enType = CElementFunction::TypeCheckingByFunction(wsElement);
		if(enType != TypeElement::empty)
			return new CElementFunction(enType);
		if(!wsElement.empty())
			return new CElementNumber(wsElement);
		return nullptr;
	}
	void CElement::SetBaseType(const TypeElement& enType)
	{
		m_enBaseType = enType;
	}
	TypeElement CElement::GetBaseType()
	{
		return m_enBaseType;
	}
	void CElement::SetNameFormula(const std::wstring& wsNameFormula)
	{
		m_wsNameFormula = wsNameFormula;
	}
	std::wstring CElement::GetNameFormula()
	{
		return m_wsNameFormula;
	}
//class CElementNumber
	CElementNumber::CElementNumber():m_wsNumber(L"")
	{}
	CElementNumber::CElementNumber(const std::wstring& wsName):m_wsNumber(wsName)
	{
		SetBaseType(TypeElement::NumberOrName);
	}
	CElementNumber::~CElementNumber()
	{}
	void CElementNumber::Parse(CSMReader *pReader)
	{
		if(!pReader->GetElement().empty())
			pReader->ClearElement();
		if(m_wsNumber == L"left")
			m_wsNumber = pReader->GetLeft() != L"" ? pReader->GetLeft():L"l";
		else if(m_wsNumber == L"top")
			m_wsNumber = pReader->GetTop() != L"" ? pReader->GetTop():L"t";
		else if(m_wsNumber == L"right")
			m_wsNumber = pReader->GetRight() != L"" ? pReader->GetRight():L"r";
		else if(m_wsNumber == L"bottom")
			m_wsNumber = pReader->GetBottom() != L"" ? pReader->GetBottom():L"b";
		/*The height and width may be different*/
		else if(m_wsNumber == L"height")
			m_wsNumber = L"h";
		else if(m_wsNumber == L"width")
			m_wsNumber = L"w";
		else if(m_wsNumber[0] == L'\u0024' && m_wsNumber.size() >1)
			m_wsNumber = m_wsNumber.substr(1,m_wsNumber.size());
		else if(m_wsNumber == L"L\u0024")
			m_wsNumber = L"0";

	}
	bool CElementNumber::CheckNumber(const std::wstring& wsNumber)
	{
		if(wsNumber.empty())
			return false;
		if(wsNumber.size() > 2 && wsNumber[0] == L'?' && wsNumber[1] == L'f')
			return true;
		for(unsigned int i = 0;i < wsNumber.size();i++)
		{
			if(!iswdigit(wsNumber[i]))
				return false;
		}
		return true;
	}
	void CElementNumber::ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring &wsName)
	{
		if(m_wsNumber[0] == L'?' && m_wsNumber[1] == L'f')
			m_wsNumber.replace(0,2,L"gd");
		pXmlWriter->WriteString(m_wsNumber + L" ");
	}
	std::wstring CElementNumber::GetString()
	{
		if(m_wsNumber[0] == L'?' && m_wsNumber[1] == L'f')
			m_wsNumber.replace(0,2,L"gd");
		return m_wsNumber;
	}
//CElementArithmeticOperations
	CElementArithmeticOperations::CElementArithmeticOperations():m_enTypeSign(TypeElement::empty),m_pSecondSign(nullptr),m_pFirstValue(nullptr),m_pSecondValue(nullptr),m_uiNumberFormula(1)
	{}
	CElementArithmeticOperations::CElementArithmeticOperations(const std::wstring& wsSign):m_enTypeSign(TypeElement::empty),m_pSecondSign(nullptr),m_pFirstValue(nullptr),m_pSecondValue(nullptr),m_uiNumberFormula(1)
	{
		m_enTypeSign = wsSign ==L"--" ? TypeElement::plus:CElementArithmeticOperations::SetTypeSign(wsSign);
		SetBaseType(TypeElement::ArithmeticOperation);
	}
	CElementArithmeticOperations::~CElementArithmeticOperations()
	{
		delete m_pFirstValue;
		delete m_pSecondValue;
		delete m_pSecondSign;
	}
	void CElementArithmeticOperations::Parse(CSMReader *pReader)
	{
		if(!pReader->GetElement().empty())
			pReader->ClearElement();
		CElement* pElement = SMCustomShapePars::ParseElement(pReader);
		if(pElement == nullptr)
			return;
		pReader->ReadingNextElement();
		if(pElement->GetBaseType() != TypeElement::ArithmeticOperation && pElement->GetBaseType() != TypeElement::comma && !ComparingPriorities(pReader->GetElement()))
		{
			m_pSecondValue = pElement;
			pElement = nullptr;
		}
		else if(pElement->GetBaseType() == TypeElement::ArithmeticOperation && ComparisonSign(pElement))
		{
			m_pSecondSign = pElement;
			pElement = nullptr;
		}
		else if(ComparingPriorities(pReader->GetElement()))
		{
			CElement* pTempElement = SMCustomShapePars::ParseElement(pReader);
			if(pTempElement->GetBaseType() == TypeElement::ArithmeticOperation)
			{
				CElementArithmeticOperations* pTempArithOp = dynamic_cast<CElementArithmeticOperations*>(pTempElement);
				pTempArithOp->SetFirstValue(pElement);
				pElement = nullptr;
				m_pSecondValue = pTempArithOp;
			}
			else
				m_pSecondValue = pTempElement;
		}
		if(!pReader->GetDoubleSign()&& !pReader->GetElement().empty() && ComparisonSign(pReader->GetElement()))
		{
			pReader->SetDoubleSign(true);
			CElement* pSecondSign = SMCustomShapePars::ParseElement(pReader);
			if(pSecondSign != nullptr)
				m_pSecondSign = pSecondSign;
			pReader->SetDoubleSign(false);
		}
		if(pElement != nullptr)
		{
			if(m_pFirstValue == nullptr)
				m_pFirstValue  = pElement;
			else if(m_pSecondValue == nullptr)
				m_pSecondValue = pElement;
		}
	}
	void CElementArithmeticOperations::ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring &wsName)
	{
		if(wsName != L"")
			SetNameFormula(wsName);
		if(m_pSecondSign != nullptr)
		{
			CElementArithmeticOperations* pTemp = dynamic_cast<CElementArithmeticOperations*>(m_pSecondSign);
			std::wstring wsNameValueSecondSign(L""),wsNameFirstValue(L""),wsNameSecondValue(L"");
			if(pTemp->GetSecondValue() != nullptr)
				wsNameValueSecondSign = ConversionValueSign(pXmlWriter,pTemp->GetSecondValue());
			if(m_pFirstValue != nullptr)
				wsNameFirstValue = ConversionValueSign(pXmlWriter,m_pFirstValue);
			if(m_pSecondValue != nullptr)
				wsNameSecondValue = ConversionValueSign(pXmlWriter,m_pSecondValue);
			pXmlWriter->WriteNodeBegin(L"a:gd",true);
			if(!GetNameFormula().empty())
				pXmlWriter->WriteAttribute(L"name",GetNameFormula());
			else
				pXmlWriter->WriteAttribute(L"name",L"Unknown");
			pXmlWriter->WriteString(L" fmla=\"");
			SignRecording(pXmlWriter,m_enTypeSign);
			SignRecording(pXmlWriter,pTemp->GetTypeSign());
			pXmlWriter->WriteString(L" ");
			RecordingTheValuesSign(pXmlWriter,wsNameFirstValue,wsNameSecondValue);
			if(!wsNameValueSecondSign.empty())
				pXmlWriter->WriteString(wsNameValueSecondSign + L" ");
			else if(pTemp->GetSecondValue() != nullptr)
				pTemp->GetSecondValue()->ConversionOOXml(pXmlWriter);
			else if(pTemp->GetTypeSign() == TypeElement::division)
				pXmlWriter->WriteString(L"1 ");
			else
				pXmlWriter->WriteString(L"0 ");
			pXmlWriter->WriteString(L"\"");
			pXmlWriter->WriteNodeEnd(L"",true,true);
		}
		else
		{
			bool bPlusMultiplication(false),bMinusDivision(false);
			std::wstring wsNameFirstValue(L""), wsNameSecondValue(L"");
			if(m_pFirstValue != nullptr)
				wsNameFirstValue = ConversionValueSign(pXmlWriter,m_pFirstValue);
			if(m_pSecondValue != nullptr)
				wsNameSecondValue = ConversionValueSign(pXmlWriter,m_pSecondValue);
			pXmlWriter->WriteNodeBegin(L"a:gd",true);
			if(!GetNameFormula().empty())
				pXmlWriter->WriteAttribute(L"name",GetNameFormula());
			else
				pXmlWriter->WriteAttribute(L"name",L"Unknown");
			pXmlWriter->WriteString(L" fmla=\"");
			switch(m_enTypeSign) {
			case TypeElement::multiplication:
			case TypeElement::plus:
			{
				SignRecording(pXmlWriter,m_enTypeSign);
				pXmlWriter->WriteString(L"/");
				bPlusMultiplication = true;
				break;
			}
			case TypeElement::division:
			case TypeElement::minus:
			{
				pXmlWriter->WriteString(L"+");
				SignRecording(pXmlWriter,m_enTypeSign);
				bMinusDivision = true;
				break;
			}
			default:
				break;
			}
			pXmlWriter->WriteString(L" ");
			if(bPlusMultiplication)
			{
				RecordingTheValuesSign(pXmlWriter,wsNameFirstValue,wsNameSecondValue);
				pXmlWriter->WriteString(L"1 ");
			}
			else if(bMinusDivision)
			{
				pXmlWriter->WriteString(L"0 ");
				RecordingTheValuesSign(pXmlWriter,wsNameFirstValue,wsNameSecondValue);
			}
			pXmlWriter->WriteString(L"\"");
			pXmlWriter->WriteNodeEnd(L"",true,true);
		}
	}
	bool CElementArithmeticOperations::CheckArithmeticOperators(const std::wstring& wsElement)
	{
		if(wsElement.empty())
			return false;
		if(wsElement.size() == 1)
		{
			switch (wsElement[0])
			{
			case L'+':
			case L'-':
			case L'*':
			case L'/':
				return true;
			default:
				return false;
			}
		}
		else
			return false;
	}
	//Нет сравнения + и /
	bool CElementArithmeticOperations::ComparisonSign(const std::wstring& wsSign)
	{
		return((m_enTypeSign == TypeElement::multiplication && wsSign == L"/") || (m_enTypeSign == TypeElement::plus && wsSign == L"-"));
	}
	bool CElementArithmeticOperations::ComparisonSign(CElement *pElement)
	{
		if(pElement == nullptr)
			return false;
		if(pElement->GetBaseType() == TypeElement::ArithmeticOperation)
		{
			CElementArithmeticOperations* pSign = dynamic_cast<CElementArithmeticOperations*>(pElement);
			return((m_enTypeSign == TypeElement::plus && pSign->GetTypeSign() == TypeElement::minus) || (m_enTypeSign == TypeElement::multiplication && pSign->GetTypeSign() == TypeElement::division));
		}
		else
			return false;
	}
	bool CElementArithmeticOperations::ComparingPriorities(const std::wstring& wsSign)
	{
		return ((m_enTypeSign == TypeElement::plus || m_enTypeSign == TypeElement::minus) && (wsSign == L"/" || wsSign == L"*"));
	}
	void CElementArithmeticOperations::SetFirstValue(CElement* pElement)
	{
		m_pFirstValue = pElement;
	}
	TypeElement CElementArithmeticOperations::SetTypeSign(const std::wstring& wsSign)
	{
		if(wsSign.size() > 1)
			return TypeElement::empty;
		switch (wsSign[0]) {
		case '+':
			return TypeElement::plus;
		case '-':
			return TypeElement::minus;
		case '/':
			return TypeElement::division;
		case '*':
			return TypeElement::multiplication;
		default:
			return TypeElement::empty;
		}
	}
	TypeElement CElementArithmeticOperations::GetTypeSign()
	{
		return m_enTypeSign;
	}
	void CElementArithmeticOperations::SignRecording(XmlUtils::CXmlWriter* pXmlWriter, const TypeElement& enTypeSign)
	{
		switch (enTypeSign) {
		case TypeElement::multiplication:
			pXmlWriter->WriteString(L"*");
			break;
		case TypeElement::plus:
			pXmlWriter->WriteString(L"+");
			break;
		case TypeElement::minus:
			pXmlWriter->WriteString(L"-");
			break;
		case TypeElement::division:
			pXmlWriter->WriteString(L"/");
			break;
		default:
			break;
		}
	}
	CElement* CElementArithmeticOperations::GetSecondValue()
	{
		return m_pSecondValue;
	}
	std::wstring CElementArithmeticOperations::ConversionValueSign(XmlUtils::CXmlWriter *pXmlWriter, CElement* pElement)
	{
		if(pElement->GetBaseType() == TypeElement::NumberOrName)
			return L"";
		else if(pElement->GetBaseType()== TypeElement::Bracket)
		{
			CElementBracket* pTempBracket = dynamic_cast<CElementBracket*>(pElement);
			std::vector<CElement*> arTemp = pTempBracket->GetVector();
			if(!arTemp.empty() && arTemp.size() == 1 && arTemp[0]->GetBaseType() == TypeElement::NumberOrName )
				return L"";
			else
			{
				std::wstring wsTempName,wsOldName = GetNameFormula();
				CElementFunction::CalculatingNumberName(wsTempName,wsOldName,m_uiNumberFormula);
				pElement->ConversionOOXml(pXmlWriter,wsTempName);
				return pElement->GetNameFormula();
			}
		}
		else
		{
			std::wstring wsTempName,wsOldName = GetNameFormula();
			CElementFunction::CalculatingNumberName(wsTempName,wsOldName,m_uiNumberFormula);
			pElement->ConversionOOXml(pXmlWriter,wsTempName);
			return pElement->GetNameFormula();
		}
		return L"";
	}
	void CElementArithmeticOperations::RecordingTheValuesSign(XmlUtils::CXmlWriter *pXmlWriter, const std::wstring &wsNameFirst, const std::wstring &wsNameSecond)
	{
		if(!wsNameFirst.empty())
			pXmlWriter->WriteString(wsNameFirst + L" ");
		else if(m_pFirstValue != nullptr)
			m_pFirstValue->ConversionOOXml(pXmlWriter);
		else 
			pXmlWriter->WriteString(L"0 ");
		if(!wsNameSecond.empty())
			pXmlWriter->WriteString(wsNameSecond + L" ");
		else if(m_pSecondValue != nullptr)
			m_pSecondValue->ConversionOOXml(pXmlWriter);
		else if(m_enTypeSign == TypeElement::minus || m_pSecondSign != nullptr)
			pXmlWriter->WriteString(L"0 ");
		else
			pXmlWriter->WriteString(L"1 ");
	}
//CSMReader
	CSMReader::CSMReader(const std::wstring& wsFormula, const std::wstring &wsLeft, const std::wstring &wsTop, const std::wstring &wsRight, const std::wstring &wsBottom) : m_Formula (wsFormula), m_pElement(nullptr), m_bDoubleSign(false),m_wsLeft(wsLeft),m_wsTop(wsTop),m_wsRight(wsRight),m_wsBottom(wsBottom)
	{
		m_itStart = m_Formula.begin();
		m_itEnd = m_Formula.end();
	}
	CSMReader::~CSMReader()
	{
		delete m_pElement;
	}
	std::wstring CSMReader::GetElement(std::wstring::iterator& itStart,std::wstring::iterator& itEnd)
	{
		std::wstring wsOneElement{L""};
		for(;itStart != itEnd;itStart++)
		{
			if(iswspace(*itStart))
			{
				if(!wsOneElement.empty())
					return wsOneElement;
				else
					continue;
			}
			else if(!wsOneElement.empty() && wsOneElement.back() == L'-')
			{
				if(L'-' == *itStart)
				{
					wsOneElement.push_back(*itStart);
					itStart++;
					return wsOneElement;
				}
				else
					return wsOneElement;
			}
			else if(!wsOneElement.empty() && (L'+' == *itStart || L'*' == *itStart || L'/' == *itStart || L',' == *itStart || L'(' == *itStart || L')' == *itStart || L'-' == *itStart))
				return wsOneElement;
			else if(wsOneElement.empty() && (L'+' == *itStart || L'*' == *itStart || L'/' == *itStart || L',' == *itStart || L'(' == *itStart || L')' == *itStart))
			{
				wsOneElement.push_back(*itStart);
				itStart++;
				return wsOneElement;
			}
			else
				wsOneElement.push_back(*itStart);
		}
		if(!wsOneElement.empty())
			return wsOneElement;
		else
			return L"";
	}
	CElement *CSMReader::ReadingElement()
	{
		std::wstring wsElement = GetElement(m_itStart,m_itEnd);
		return CElement::CreateElement(wsElement);
	}
	bool CSMReader::ReadingNextElement()
	{
		if(!m_wsElement.empty())
			return true;
		m_wsElement = GetElement(m_itStart,m_itEnd);
		if(!m_wsElement.empty())
			return true;
		else
			return false;
	}
	bool CSMReader::CheckIteratorPosition()
	{
		if(m_itStart == m_itEnd)
			return true;
		else
			return false;
	}
	std::wstring CSMReader::GetElement()
	{
		return m_wsElement;
	}
	void CSMReader::ClearElement()
	{
		m_wsElement.clear();
	}
	void CSMReader::FindingTheEndOfTheBrackets()
	{
		std::wstring::iterator itStartTemp = m_itStart;
		unsigned int uiOpenBracket{0};
		for(;m_itStart !=m_itEnd;m_itStart++)
		{
			if(*m_itStart == L')' && uiOpenBracket == 0)
			{
				// m_itEndForBrecket = m_itEnd;
				m_stEndBrecket.push(m_itEnd);
				m_itEnd = m_itStart;
				m_itStart = itStartTemp;
				if(m_wsElement == L"(")
					m_wsElement.clear();
				return;
			}
			else if(*m_itStart == L')' && uiOpenBracket != 0)
			{
				// m_stEndBrecket.push(m_itStart);
				uiOpenBracket--;
			}
			else if(*m_itStart == L'(')
				uiOpenBracket++;
		}
	}
	void CSMReader::RemovingTheParenthesisIterator()
	{
		if(!m_stEndBrecket.empty())
		{
			m_itEnd = m_stEndBrecket.top();
			m_stEndBrecket.pop();
		}
		m_wsElement = GetElement(m_itStart,m_itEnd);
		m_wsElement.clear();
	}
	void CSMReader::SetDoubleSign(const bool& bDoubleSign)
	{
		m_bDoubleSign = bDoubleSign;
	}
	bool CSMReader::GetDoubleSign()
	{
		return m_bDoubleSign;
	}
	std::wstring CSMReader::GetLeft()
	{
		return m_wsLeft;
	}
	std::wstring CSMReader::GetTop()
	{
		return m_wsTop;
	}
	std::wstring CSMReader::GetRight()
	{
		return m_wsRight;
	}
	std::wstring CSMReader::GetBottom()
	{
		return m_wsBottom;
	}
//CElementBracket
	CElementBracket::CElementBracket()
	{
		SetBaseType(TypeElement::Bracket);
	}
	CElementBracket::~CElementBracket()
	{
		for(CElement* pElement:m_arElements)
			delete pElement;
	}
	void CElementBracket::Parse(CSMReader* pReader)
	{
		pReader->FindingTheEndOfTheBrackets();
		SMCustomShapePars::ParsString(pReader,m_arElements);
		pReader->RemovingTheParenthesisIterator();
	}
	void CElementBracket::ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring &wsName)
	{
		if(!wsName.empty())
			SetNameFormula(wsName);
		for(CElement* pElement:m_arElements)
		{
			if(pElement != nullptr)
				pElement->ConversionOOXml(pXmlWriter,wsName);
		}

	}
	std::vector<CElement*> CElementBracket::GetVector()
	{
		return m_arElements;
	}
//CElementFunction
	CElementFunction::CElementFunction():m_enTypeFunction(TypeElement::empty),m_pValue(nullptr),m_uiNumberFormula(1)
	{
		SetBaseType(TypeElement::Function);
	}
	CElementFunction::CElementFunction(const TypeElement& enType):m_pValue(nullptr),m_uiNumberFormula(1)
	{
		SetBaseType(TypeElement::Function);
		m_enTypeFunction = enType;
	}
	CElementFunction::~CElementFunction()
	{}
	TypeElement CElementFunction::TypeCheckingByFunction(const std::wstring& wsFunction)
	{
		if(wsFunction == L"sqrt")
			return TypeElement::sqrt;
		else if(wsFunction == L"sin")
			return TypeElement::sin;
		else if(wsFunction == L"cos")
			return TypeElement::cos;
		else if(wsFunction == L"abs")
			return TypeElement::abs;
		else if(wsFunction == L"if")
			return TypeElement::If;
		else if(wsFunction == L"tan")
			return TypeElement::tan;
		else if(wsFunction == L"min")
			return TypeElement::min;
		else if(wsFunction == L"max")
			return TypeElement::max;
		else if(wsFunction == L"atan")
			return TypeElement::atan;
		else if(wsFunction == L"atan2")
			return TypeElement::atan2;
		else if(wsFunction == L"logheight")
			return TypeElement::logheight;
		else if(wsFunction == L"logwidth")
			return TypeElement::logwidth;
		else if(wsFunction == L"pi")
			return TypeElement::pi;
		else
			return TypeElement::empty;
	}
	void CElementFunction::Parse(CSMReader* pReader)
	{
		pReader->ClearElement();
		if(m_enTypeFunction != TypeElement::logheight && m_enTypeFunction != TypeElement::logwidth && m_enTypeFunction != TypeElement::pi)
			m_pValue = SMCustomShapePars::ParseElement(pReader);
	}
	void CElementFunction::ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring &wsName)
	{
		if(m_pValue == nullptr && m_enTypeFunction != TypeElement::logheight && m_enTypeFunction != TypeElement::logwidth && m_enTypeFunction != TypeElement::pi)
		{
			if(!wsName.empty())
				SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,wsName,L"sqrt 0 ");
			else
				SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,L"gdTemp",L"sqrt 0 ");
			return;
		}
		if(!wsName.empty())
			SetNameFormula(wsName);
		else
			SetNameFormula(L"Function");
		switch (m_enTypeFunction) {
		case TypeElement::If:
		{
			std::wstring wsFormula = L"?: ";
			if(m_pValue->GetBaseType() == TypeElement::Bracket)
			{
				CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
				std::vector<CElement*> arVector = pBracket->GetVector();
				if(!arVector.empty())
					for(unsigned int i = 0;	i	<	arVector.size(); i++)
					{
						if(arVector[i]->GetBaseType() == TypeElement::comma)
						{
							if(i - 1 >= 0)
							{
								ConversionElement(pXmlWriter,arVector[i - 1],wsFormula);
							}
							else
								wsFormula += L"1 ";
						}
						else if(i + 1 == arVector.size() && arVector[i]->GetBaseType() != TypeElement::comma)
							ConversionElement(pXmlWriter,arVector[i],wsFormula);
					}
				else
					wsFormula += L"0 0 0 ";
			}
			SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,GetNameFormula(),wsFormula);
			break;
		}
		case TypeElement::abs:
		case TypeElement::sqrt:
		{
			std::wstring wsFormula;
			if(m_enTypeFunction == TypeElement::abs)
				wsFormula = L"abs ";
			else
				wsFormula = L"sqrt ";
			if(m_pValue->GetBaseType() == TypeElement::Bracket)
			{
				CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
				std::vector<CElement*> arValues = pBracket->GetVector();
				if(!arValues.empty())
					ConversionElement(pXmlWriter,arValues[0],wsFormula);
				else
					wsFormula += L"0 ";
			}
			else
				ConversionElement(pXmlWriter,m_pValue,wsFormula);
			SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,GetNameFormula(),wsFormula);
			break;
		}
		case TypeElement::min:
		case TypeElement::max:
		{
			std::wstring wsFormula;
			if(m_enTypeFunction == TypeElement::max)
				wsFormula = L"max ";
			else
				wsFormula = L"min ";
			if(m_pValue->GetBaseType() == TypeElement::Bracket)
			{
				CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
				std::vector<CElement*> arElements = pBracket->GetVector();
				if(!arElements.empty())	
					for(unsigned int i = 0; i < arElements.size();i++)
					{
						if(arElements[i]->GetBaseType() == TypeElement::comma)
						{
							if(i - 1 >= 0)
								ConversionElement(pXmlWriter,arElements[i-1],wsFormula);
							if(i + 1 <= arElements.size() && arElements[i+1]->GetBaseType() != TypeElement::comma)
								ConversionElement(pXmlWriter,arElements[i+1],wsFormula);
							i = arElements.size();
						}
					}
				else
					wsFormula += L"0 0 ";
				SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,GetNameFormula(),wsFormula);
			}
			break;
		}
		case TypeElement::sin:
		case TypeElement::cos:
		case TypeElement::atan:
		case TypeElement::atan2:
		case TypeElement::tan:
		{
			std::wstring wsFormula;

			if(m_enTypeFunction == TypeElement::sin)
				wsFormula = L"sin";
			else if(m_enTypeFunction == TypeElement::cos)
				wsFormula = L"cos";
			else if(m_enTypeFunction == TypeElement::tan)
				wsFormula = L"tan";
			else if(m_enTypeFunction == TypeElement::atan || m_enTypeFunction == TypeElement::atan2)
				wsFormula = L"at2 ";
			if(m_enTypeFunction != TypeElement::atan2)
				wsFormula += L" 1 ";
			if(!ConvertBracketsForTrigonometry(pXmlWriter,wsFormula,m_enTypeFunction))
			{
				if(m_enTypeFunction != TypeElement::atan2)
					wsFormula += L"0 ";
				else
					wsFormula += L" 0 0 ";
			}
			SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,GetNameFormula(),wsFormula);
			break;
		}
		case TypeElement::logheight:
		case TypeElement::logwidth:
		{
			CalculatingTheLogicalHeightOrWidth(pXmlWriter);
			break;
		}
		case TypeElement::pi:
		{
			SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,GetNameFormula(),L"+/ 0 355 113");
		}
		default:
			break;
		}
	}
	void CElementFunction::CalculatingTheLogicalHeightOrWidth(XmlUtils::CXmlWriter* pXmlWriter)
	{
		SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,GetNameFormula(),L"*/ " + std::wstring((m_enTypeFunction == TypeElement::logheight) ? L"h":L"w") + L" 25 48 ");
	}
	void CElementFunction::CalculatingNumberName(std::wstring &wsNewName, std::wstring& wsOldNameFormula, unsigned int &uiNumberFormula)
	{
		wsNewName = wsOldNameFormula + L"." + std::to_wstring(uiNumberFormula);
		uiNumberFormula++;
	}
	void CElementFunction::ConversionElement(XmlUtils::CXmlWriter* pXmlWriter, CElement *pElement, std::wstring &wsFormula)
	{
		if(pElement == nullptr)
			return;
		if(pElement->GetBaseType() == TypeElement::NumberOrName)
		{
			CElementNumber* pTempElement = dynamic_cast<CElementNumber*>(pElement);
			wsFormula += pTempElement->GetString() + L" ";
		}
		else
		{
			std::wstring wsNewName,wsOldName = GetNameFormula();
			CElementFunction::CalculatingNumberName(wsNewName,wsOldName,m_uiNumberFormula);
			wsFormula += wsNewName + L" ";
			pElement->ConversionOOXml(pXmlWriter,wsNewName);
		}
	}
	bool CElementFunction::ConvertBracketsForTrigonometry(XmlUtils::CXmlWriter* pXmlWriter, std::wstring& wsFormula, const TypeElement &enTypeFunc)
	{
		if(m_pValue->GetBaseType() == TypeElement::Bracket)
		{
			CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
			std::vector<CElement*> arVec = pBracket->GetVector();
			if(arVec.empty())
				return false;
			else
				for(unsigned int i = 0 ; i < arVec.size();i++)
				{
					if(arVec[i] != nullptr && arVec[i]->GetBaseType() != TypeElement::comma)
						ConversionElement(pXmlWriter,arVec[0],wsFormula);
					else
						wsFormula += L"0 ";
					if((enTypeFunc != TypeElement::atan2 && i == 0) || (enTypeFunc == TypeElement::atan2 && i == 1))
						return true;
				}
		}
		else if(m_pValue != nullptr && m_pValue->GetBaseType() != TypeElement::comma)
		{
			ConversionElement(pXmlWriter,m_pValue,wsFormula);
			return true;
		}
	    return false;
	}
//CElementComma
	CElementComma::CElementComma()
	{SetBaseType(TypeElement::comma);}
	CElementComma::~CElementComma()
	{}
	void CElementComma::Parse(CSMReader* pReader)
	{
		if(!pReader->GetElement().empty())
			pReader->ClearElement();
	}
	void CElementComma::ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring &wsName)
	{}
//SMCustomShapeConversion
	SMCustomShapeConversion::SMCustomShapeConversion():m_pXmlWriter(nullptr)
	{}
	SMCustomShapeConversion::~SMCustomShapeConversion()
	{
		delete m_pXmlWriter;
	}
	void SMCustomShapeConversion::StartConversion(std::vector<CElement*>& arElements, const std::wstring& wsFormulaName)
	{
		if(arElements.empty())
			return;
		m_pXmlWriter = new XmlUtils::CXmlWriter;
		if(arElements.size() == 1 && arElements[0]->GetBaseType() == TypeElement::NumberOrName)
		{
			m_pXmlWriter->WriteNodeBegin(L"a:gd",true);
			if(!wsFormulaName.empty())
				m_pXmlWriter->WriteAttribute(L"name",wsFormulaName);
			else
				m_pXmlWriter->WriteAttribute(L"name",L"Text");
			m_pXmlWriter->WriteString(L" fmla=\"val ");
			arElements[0]->ConversionOOXml(m_pXmlWriter,wsFormulaName);
			m_pXmlWriter->WriteString(L"\"");
			m_pXmlWriter->WriteNodeEnd(L"",true,true);
		}
		else
		{
			for(CElement* pElement:arElements)
			{
				if(pElement != nullptr)
					pElement->ConversionOOXml(m_pXmlWriter,wsFormulaName);
			}
		}

	}
	std::wstring SMCustomShapeConversion::GetStringXml()
	{
		return m_pXmlWriter->GetXmlString();
	}
	void SMCustomShapeConversion::WritingFormulaXml(XmlUtils::CXmlWriter* pXmlWriter,const std::wstring& wsNameFormula,const std::wstring& wsFormula)
	{
		pXmlWriter->WriteNodeBegin(L"a:gd",true);
		pXmlWriter->WriteAttribute(L"name",wsNameFormula);
		pXmlWriter->WriteAttribute(L"fmla",wsFormula);
		pXmlWriter->WriteNodeEnd(L"",true,true);
	}
	std::wstring SMCustomShapeConversion::ParsFormulaName(const std::wstring& wsFormulaName, std::wstring &wsName)
	{
		if(wsFormulaName.empty())
			return L"";
		std::wstring wsTempNumber;
		for(wchar_t wcElement: wsFormulaName)
		{
			if(iswdigit(wcElement) && (L'-' == wcElement || L'.' == wcElement))
				wsTempNumber += wcElement;
			else
				wsName += wcElement;
		}
		if(!wsTempNumber.empty())
			return wsTempNumber;
		else
			return L"";
	}
}
