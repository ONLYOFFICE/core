#include "smcustomshapepars.h"

namespace StarMathCustomShape
{
	SMCustomShapePars::SMCustomShapePars()
	{}
	SMCustomShapePars::~SMCustomShapePars()
	{
		for(CElement* pElement:m_arVecElements)
			delete pElement;
	}
	void SMCustomShapePars::StartParsSMCustomShape(std::wstring &wsStarMath)
	{
		CSMReader* pReader = new CSMReader(wsStarMath);
		SMCustomShapePars::ParsString(pReader,m_arVecElements);
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
		else if(CElementArithmeticOperations::CheckArithmeticOperators(wsElement))
			return new CElementArithmeticOperations(wsElement);
		else if(wsElement == L"(")
			return new CElementBracket();
		else if(wsElement == L",")
			return new CElementComma();
		TypeElement enType = CElementFunction::TypeCheckingByFunction(wsElement);
		if(enType != TypeElement::empty)
			return new CElementFunction(enType);
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
	CElementArithmeticOperations::CElementArithmeticOperations():m_enTypeSign(TypeElement::empty),m_pSecondSign(nullptr),m_pFirstValue(nullptr),m_pSecondValue(nullptr),m_uiNumberFormula(1),m_wsNameFormula(L""),m_wsNumberFormula(L"")
	{}
	CElementArithmeticOperations::CElementArithmeticOperations(const std::wstring& wsSign):m_enTypeSign(TypeElement::empty),m_pSecondSign(nullptr),m_pFirstValue(nullptr),m_pSecondValue(nullptr),m_uiNumberFormula(1),m_wsNameFormula(L""),m_wsNumberFormula(L"")
	{
		m_enTypeSign = CElementArithmeticOperations::SetTypeSign(wsSign);
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
		if(pElement->GetBaseType() != TypeElement::ArithmeticOperation && pElement->GetBaseType() != TypeElement::comma && !ComparingPriorities(pReader->GetElement()))//утечка данных, если тип ариф знаки
			m_pSecondValue = pElement;
		else if(ComparingPriorities(pReader->GetElement()))
		{
			CElement* pTempElement = SMCustomShapePars::ParseElement(pReader);
			if(pTempElement->GetBaseType() == TypeElement::ArithmeticOperation)
			{
				CElementArithmeticOperations* pTempArithOp = dynamic_cast<CElementArithmeticOperations*>(pTempElement);
				pTempArithOp->SetFirstValue(pElement);
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
	}
	void CElementArithmeticOperations::ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring &wsName)
	{
		if(wsName != L"")
		{
			SetNameFormula(wsName);
			m_wsNumberFormula = SMCustomShapeConversion::ParsFormulaName(wsName,m_wsNameFormula);
		}
		if(m_pSecondSign != nullptr)
		{
			CElementArithmeticOperations* pTemp = dynamic_cast<CElementArithmeticOperations*>(m_pSecondSign);
			std::wstring wsNameValueSecondSign(L""),wsNameFirstValue(L""),wsNameSecondValue(L"");
			if(pTemp->GetSecondValue() != nullptr)
				wsNameValueSecondSign = ConversionValueSign(pXmlWriter,pTemp->GetSecondValue());
			if(m_pFirstValue != nullptr)
				wsNameFirstValue = ConversionValueSign(pXmlWriter,m_pFirstValue);
			if(m_pSecondValue != nullptr)
				wsNameSecondValue = ConversionValueSign(pXmlWriter,m_pFirstValue);
			pXmlWriter->WriteNodeBegin(L"a:gd",true);
			if(!GetNameFormula().empty())
				pXmlWriter->WriteAttribute(L"name",GetNameFormula());
			else
				pXmlWriter->WriteAttribute(L"name",L"Unknown");
			pXmlWriter->WriteString(L" fmla=\"");
			SignRecording(pXmlWriter,m_enTypeSign);
			SignRecording(pXmlWriter,pTemp->GetTypeSign());
			pXmlWriter->WriteString(L" ");
			if(!wsNameFirstValue.empty())
				pXmlWriter->WriteString(wsNameFirstValue + L" ");
			else if(m_pFirstValue != nullptr)
				m_pFirstValue->ConversionOOXml(pXmlWriter);
			if(!wsNameSecondValue.empty())
				pXmlWriter->WriteString(wsNameSecondValue + L" ");
			else if(m_pSecondValue != nullptr)
				m_pSecondValue->ConversionOOXml(pXmlWriter);
			if(!wsNameValueSecondSign.empty())
				pXmlWriter->WriteString(wsNameValueSecondSign + L" ");
			else if(pTemp->GetSecondValue() != nullptr)
				pTemp->GetSecondValue()->ConversionOOXml(pXmlWriter);
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
				if(!wsNameFirstValue.empty())
					pXmlWriter->WriteString(wsNameFirstValue + L" ");
				else if(m_pFirstValue != nullptr)
					m_pFirstValue->ConversionOOXml(pXmlWriter);
				if(!wsNameSecondValue.empty())
					pXmlWriter->WriteString(wsNameSecondValue + L" ");
				else if(m_pSecondValue != nullptr)
					m_pSecondValue->ConversionOOXml(pXmlWriter);
				pXmlWriter->WriteString(L"1 ");
			}
			else if(bMinusDivision)
			{
				pXmlWriter->WriteString(L"0 ");
				if(!wsNameFirstValue.empty())
					pXmlWriter->WriteString(wsNameFirstValue + L" ");
				else if(m_pFirstValue != nullptr)
					m_pFirstValue->ConversionOOXml(pXmlWriter);
				if(!wsNameSecondValue.empty())
					pXmlWriter->WriteString(wsNameSecondValue + L" ");
				else if(m_pSecondValue != nullptr)
					m_pSecondValue->ConversionOOXml(pXmlWriter);
			}
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
		if(pElement->GetBaseType() != TypeElement::NumberOrName)
		{
			// m_uiNumberFormula++;
			std::wstring wsTempName = m_wsNameFormula + m_wsNumberFormula + L"-" + std::to_wstring(m_uiNumberFormula);
			m_uiNumberFormula++;
			pElement->ConversionOOXml(pXmlWriter,wsTempName);
			return pElement->GetNameFormula();
		}
		else
			return L"";
	}
//CSMReader
	CSMReader::CSMReader(std::wstring& wsStarMath):m_wsElement(L""),m_pElement(nullptr),m_bDoubleSign(false)
	{
		m_itStart = wsStarMath.begin();
		m_itEnd = wsStarMath.end();
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
				{
					// std::wcout << wsOneElement << std::endl;
					return wsOneElement;
				}
				else
					continue;
			}
			else if(!wsOneElement.empty() && (L'+' == *itStart || L'-' == *itStart || L'*' == *itStart || L'/' == *itStart || L',' == *itStart || L'(' == *itStart || L')' == *itStart))
			{
				// std::wcout << wsOneElement << std::endl;
				return wsOneElement;
			}
			else if(wsOneElement.empty() && (L'+' == *itStart || L'-' == *itStart || L'*' == *itStart || L'/' == *itStart || L',' == *itStart || L'(' == *itStart || L')' == *itStart))
			{
				wsOneElement.push_back(*itStart);
				itStart++;
				// std::wcout << wsOneElement << std::endl;
				return wsOneElement;
			}
			else
				wsOneElement.push_back(*itStart);
		}
		if(!wsOneElement.empty())
		{
			// std::wcout << wsOneElement << std::endl;
			return wsOneElement;
		}
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
		for(;m_itStart !=m_itEnd;m_itStart++)
		{
			if(*m_itStart == L')')
			{
				m_itEndForBrecket = m_itEnd;
				m_itEnd = m_itStart;
				m_itStart = itStartTemp;
				return;
			}
		}
	}
	void CSMReader::RemovingTheParenthesisIterator()
	{
		if(m_itEndForBrecket != m_itEnd)
			m_itEnd = m_itEndForBrecket;
	}
	void CSMReader::SetDoubleSign(const bool& bDoubleSign)
	{
		m_bDoubleSign = bDoubleSign;
	}
	bool CSMReader::GetDoubleSign()
	{
		return m_bDoubleSign;
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
		for(CElement* pElement:m_arElements)
		{
			if(pElement != nullptr)
				pElement->ConversionOOXml(pXmlWriter);
		}

	}
	std::vector<CElement*> CElementBracket::GetVector()
	{
		return m_arElements;
	}
//CElementFunction
	CElementFunction::CElementFunction():m_enTypeFunction(TypeElement::empty),m_pValue(nullptr)
	{
		SetBaseType(TypeElement::Function);
	}
	CElementFunction::CElementFunction(const TypeElement& enType):m_pValue(nullptr)
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
		// else if(wsFunction == L"atan2")
		// 	return TypeElement::atan2;
		// else if(wsFunction == L"atan")
		// 	return TypeElement::atan;
		else
			return TypeElement::empty;
	}
	void CElementFunction::Parse(CSMReader* pReader)
	{
		m_pValue = SMCustomShapePars::ParseElement(pReader);
		pReader->ReadingNextElement();
		if(CElementArithmeticOperations::CheckArithmeticOperators(pReader->GetElement()))
		{
			CElement* pSign = SMCustomShapePars::ParseElement(pReader);
			CElementArithmeticOperations* pTempSign = dynamic_cast<CElementArithmeticOperations*>(pSign);
			pTempSign->SetFirstValue(m_pValue);
			m_pValue = pTempSign;
		}
	}
	void CElementFunction::ConversionOOXml(XmlUtils::CXmlWriter* pXmlWriter, const std::wstring &wsName)
	{
		if(m_pValue == nullptr)
			return;
		switch (m_enTypeFunction) {
		case TypeElement::If:
		{
			SetNameFormula(L"FunctionIf");
			std::wstring wsFormula = L"?: ";
			if(m_pValue->GetBaseType() == TypeElement::Bracket)
			{
				CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
				std::vector<CElement*> arVector = pBracket->GetVector();
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
			}
			SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,GetNameFormula(),wsFormula);
			break;
		}
		case TypeElement::abs:
		case TypeElement::sqrt:
		{
			std::wstring wsFormula;
			std::wstring wsNameFormula;
			if(m_enTypeFunction == TypeElement::abs)
			{
				wsFormula = L"abs ";
				wsNameFormula = L"FunctionAbs";
			}
			else
			{
				wsFormula = L"sqrt ";
				wsNameFormula = L"FunctionSqrt";
			}
			SetNameFormula(wsNameFormula);
			if(m_pValue->GetBaseType() == TypeElement::Bracket)
			{
				CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
				std::vector<CElement*> arValues = pBracket->GetVector();
				ConversionElement(pXmlWriter,arValues[0],wsFormula);
			}
			else
				ConversionElement(pXmlWriter,m_pValue,wsFormula);
			SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,wsNameFormula,wsFormula);
			break;
		}
		case TypeElement::min:
		case TypeElement::max:
		{
			std::wstring wsNameFormula;
			std::wstring wsFormula;
			if(m_enTypeFunction == TypeElement::max)
			{
				wsNameFormula = L"FunctionMax";
				wsFormula = L"max ";
			}
			else
			{
				wsNameFormula = L"FunctionMin";
				wsFormula = L"min ";
			}
			if(m_pValue->GetBaseType() == TypeElement::Bracket)
			{
				CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
				std::vector<CElement*> pElements = pBracket->GetVector();
				for(unsigned int i = 0; i < pElements.size();i++)
				{
					if(pElements[i]->GetBaseType() == TypeElement::comma)
					{
						if(i - 1 >= 0)
							ConversionElement(pXmlWriter,pElements[i-1],wsFormula);
						if(i + 1 <= pElements.size())
							ConversionElement(pXmlWriter,pElements[i+1],wsFormula);
						i = pElements.size();
					}
				}
				SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,wsNameFormula,wsFormula);
			}
			break;
		}
		case TypeElement::sin:
		case TypeElement::cos:
		{
			std::wstring wsNameFormula;
			std::wstring wsFormula;
			if(m_enTypeFunction == TypeElement::sin)
			{
				wsNameFormula = L"FunctionSin";
				wsFormula = L"sin 1 ";
			}
			else
			{
				wsNameFormula = L"FunctionCos";
				wsFormula = L"cos 1 ";
			}
			if(m_pValue->GetBaseType() == TypeElement::Bracket)
			{
				CElementBracket* pBracket = dynamic_cast<CElementBracket*>(m_pValue);
				std::vector<CElement*> arVec = pBracket->GetVector();
				if(arVec[0] != nullptr)
					ConversionElement(pXmlWriter,arVec[0],wsFormula);
			}
			else
				ConversionElement(pXmlWriter,m_pValue,wsFormula);
			SMCustomShapeConversion::WritingFormulaXml(pXmlWriter,wsNameFormula,wsFormula);
		}
		default:
			break;
		}
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
			pElement->ConversionOOXml(pXmlWriter);
			wsFormula += pElement->GetNameFormula() + L" ";
		}
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
	{};
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
			m_pXmlWriter->WriteAttribute(L"name",L"Text");
			m_pXmlWriter->WriteString(L" fmla=\"val ");
			arElements[0]->ConversionOOXml(m_pXmlWriter,wsFormulaName);
			m_pXmlWriter->WriteString(L"\"");
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
			if(iswdigit(wcElement) && L'-' == wcElement)
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
