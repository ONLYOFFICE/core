#include "cstarmathpars.h"
#include "cconversionsmtoooxml.h"
namespace StarMath
{
//class methods CParsStarMath
	std::vector<CElement*> CParserStarMathString::Parse(std::wstring& wsParseString)
	{
		std::wstring::iterator itStart = wsParseString.begin(),itEnd = wsParseString.end();
		CStarMathReader* pReader = new CStarMathReader(itStart,itEnd);
		while(!pReader->CheckIteratorPosition())
		{
			CElement* pTempElement = ParseElement(pReader);
			if(nullptr == pTempElement)
				break;
			if(!m_arEquation.empty() && CheckForLeftArgument(pTempElement->GetBaseType()) )
			{
				AddLeftArgument(m_arEquation.back(),pTempElement);
				m_arEquation.pop_back();
			}
			m_arEquation.push_back(pTempElement);
		}
		return m_arEquation;
	}

	CElement* CParserStarMathString::ParseElement(CStarMathReader* pReader)
	{
		CElement* pElement;
		std::vector<CAttribute*> arAttributes;

		if(pReader->EmptyString())
		{
			pReader->GetToken();
			pReader->SetTypesToken();
		}

		while(pReader->GetGlobalType() == TypeElement::Attribute)
		{
			if(pReader->GetLocalType() != TypeElement::color) arAttributes.push_back(new CAttribute(pReader->GetLocalType()));
			pReader->GetToken();
			pReader->SetTypesToken();
		}

		pElement = CElement::CreateElement(pReader);

		if(pElement != nullptr)
		{
			pElement->SetAttribute(arAttributes);
			pReader->ClearReader();
			pElement->Parse(pReader);
//			if(CheckingTheNextElement(itStart,itEnd,CIndex::IsIndex))
//			{
//				CIndex* pIndex = CIndex::CreateIndex(GetElement(itStart,itEnd));
//				pIndex->SetValueIndex(ParseElement(itStart,itEnd));
//				pElement->SetIndex(pIndex);
//			}
			return pElement;
		}
		else  return pElement;
	}
	template<typename T>
	void SetLeft(CElement *pLeftArg, CElement *pElementWhichAdd)
	{
		T* pTempElement = dynamic_cast<T*>(pElementWhichAdd);
		pTempElement->SetLeftArg(pLeftArg);
		pElementWhichAdd = pTempElement;
	}

	void CParserStarMathString::AddLeftArgument(CElement *pLeftArg, CElement *pElementWhichAdd)
	{
		switch(pElementWhichAdd->GetBaseType())
		{
			case TypeElement::BinOperator:
			{
				SetLeft<CElementBinOperator>(pLeftArg, pElementWhichAdd);
				break;
			}
			case TypeElement::SetOperations:
			{
				SetLeft<CElementSetOperations>(pLeftArg, pElementWhichAdd);
				break;
			}
			case TypeElement::Connection:
			{
				SetLeft<CElementConnection>(pLeftArg,pElementWhichAdd);
				break;
			}
			case TypeElement::BracketWithIndex:
			{
				SetLeft<CElementBracketWithIndex>(pLeftArg,pElementWhichAdd);
				break;
			}
			default:
				break;
		}
	}
	CElement* CParserStarMathString::ReadingWithoutBracket(CStarMathReader *pReader)
	{
		CElement* pFirstTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->GetToken();
		pReader->SetTypesToken();
		while(CheckForLeftArgument(pReader->GetGlobalType()))
		{
			CElement* pSecondTempElement = CParserStarMathString::ParseElement(pReader);
			if(pFirstTempElement != nullptr)
			{
				CParserStarMathString::AddLeftArgument(pFirstTempElement,pSecondTempElement);
			}
			pFirstTempElement = pSecondTempElement;
			if(pReader->GetGlobalType() == TypeElement::Empty)
			{
				pReader->GetToken();
				pReader->SetTypesToken();
			}
		}
		return pFirstTempElement;
	}
	bool CParserStarMathString::CheckForLeftArgument(const TypeElement &enType)
	{
		switch(enType)
		{
			case TypeElement::BinOperator:
				return true;
			case TypeElement::Connection:
				return true;
			case TypeElement::SetOperations:
				return true;
			case TypeElement::BracketWithIndex:
				return true;
			default:
				return false;
		}
	}
//class methods CAttribute
	CAttribute::~CAttribute()
	{}
	CAttribute::CAttribute(const TypeElement &enType)
	{
		m_enTypeAttr = enType;
	}
	TypeElement CAttribute::GetType()
	{
		return m_enTypeAttr;
	}
//нет phantom, rgb, 16 , гарнитуры и кегля
	TypeElement CAttribute::GetTypeAttribute(const std::wstring &wsToken)
	{
		if(L"acute" == wsToken) return TypeElement::acute;
		else if(L"color" == wsToken) return TypeElement::color;
		else if(L"breve" == wsToken) return TypeElement::breve;
		else if(L"dot" == wsToken) return TypeElement::dot;
		else if(L"dddot" == wsToken) return TypeElement::dddot;
		else if(L"vec" == wsToken) return TypeElement::vec;
		else if(L"tilde" == wsToken) return TypeElement::tilde;
		else if(L"check" == wsToken) return TypeElement::check;
		else if(L"grave" == wsToken) return TypeElement::grave;
		else if(L"circle" == wsToken) return TypeElement::circle;
		else if(L"ddot" == wsToken) return TypeElement::ddot;
		else if(L"bar" == wsToken) return TypeElement::bar;
		else if(L"harpoon" == wsToken) return TypeElement::harpoon;
		else if(L"hat" == wsToken) return TypeElement::hat;
		else if(L"widevec" == wsToken) return TypeElement::widevec;
		else if(L"widetilde" == wsToken) return TypeElement::widetilde;
		else if(L"overline" == wsToken) return TypeElement::overline;
		else if(L"overstrike" == wsToken) return TypeElement::overstrike;
		else if(L"wideharpoon" == wsToken) return TypeElement::wideharpoon;
		else if(L"widehat" == wsToken) return TypeElement::widehat;
		else if(L"underline" == wsToken) return TypeElement::underline;
		else if(L"ital" == wsToken) return TypeElement::ital;
		else if(L"bold" == wsToken) return TypeElement::bold;
		else if(L"black" == wsToken) return TypeElement::black;
		else if(L"green" == wsToken) return TypeElement::green;
		else if(L"aqua" == wsToken) return TypeElement::aqua;
		else if(L"yellow" == wsToken) return TypeElement::yellow;
		else if(L"lime" == wsToken) return TypeElement::lime;
		else if(L"navy" == wsToken) return TypeElement::navy;
		else if(L"purple" == wsToken) return TypeElement::purple;
		else if(L"teal" == wsToken) return TypeElement::teal;
		else if(L"blue" == wsToken) return TypeElement::blue;
		else if(L"red" == wsToken) return TypeElement::red;
		else if(L"fuchsia" == wsToken) return TypeElement::fuchsia;
		else if(L"gray" == wsToken) return TypeElement::gray;
		else if(L"maroon" == wsToken) return TypeElement::maroon;
		else if(L"olive" == wsToken) return TypeElement::olive;
		else if(L"silver" == wsToken) return TypeElement::silver;
		else if(L"coral" == wsToken) return TypeElement::coral;
		else if(L"midnightblue" == wsToken) return TypeElement::midnightblue;
		else if(L"crimson" == wsToken) return TypeElement::crimson;
		else if(L"violet" == wsToken) return TypeElement::violet;
		else if(L"orange" == wsToken) return TypeElement::orange;
		else if(L"seagreen" == wsToken) return TypeElement::seagreen;
		else if(L"hotpink" == wsToken) return TypeElement::hotpink;
		else if(L"orangered" == wsToken) return TypeElement::orangered;
		else if(L"indigo" == wsToken) return TypeElement::indigo;
		else if(L"lavender" == wsToken) return TypeElement::lavender;
		else return TypeElement::undefine;
	}
//class methods CElement
	CElement::~CElement()
	{
		for(CAttribute* pTemp:m_arElementAttributes)
		{
			delete pTemp;
		}
		delete m_pElementIndex;
	}
	CElement::CElement(): m_pElementIndex(nullptr)
	{}
//	TypeElement CElement::GetTypeElement(const std::wstring& wsToken)
//	{
//			if (wsToken == L"+")
//			{
//				return  TypeElement::plus;
//			}
//			else if (wsToken == L"-")
//			{
//				return  TypeElement::minus;
//			}
//			else if (wsToken == L"*")
//			{
//				return  TypeElement::multipl;
//			}
//			else if (wsToken == L"/")
//			{
//				return  TypeElement::division;
//			}
//			else if (wsToken == L"over")
//			{
//				return  TypeElement::over;
//			}
//			else if (wsToken == L"cdot")
//			{
//				return  TypeElement::cdot;
//			}
//			else if (wsToken == L"times")
//			{
//				return  TypeElement::times;
//			}
//			else if (wsToken == L"frac")
//			{
//				return  TypeElement::frac;
//			}
//			else if (wsToken == L"div")
//			{
//				return  TypeElement::div;
//			}
//			else if (wsToken == L"oplus")
//			{
//				return  TypeElement::oplus;
//			}
//			else if (wsToken == L"ominus")
//			{
//				return  TypeElement::ominus;
//			}
//			else if (wsToken == L"odot")
//			{
//				return  TypeElement::odot;
//			}
//			else if (wsToken == L"otimes")
//			{
//				return  TypeElement::otimes;
//			}
//			else if (wsToken == L"odivide")
//			{
//				return  TypeElement::odivide;
//			}
//			else if (wsToken == L"circ")
//			{
//				return  TypeElement::circ;
//			}
//			else if (wsToken == L"wideslash")
//			{
//				return  TypeElement::wideslash;
//			}
//			else if (wsToken == L"widebslash")
//			{
//				return  TypeElement::widebslash;
//			}
//			else if(L"{" == wsToken) return TypeElement::brace;
//			else if(L"(" == wsToken) return TypeElement::round;
//			else if(L"[" == wsToken) return TypeElement::square;
//			else if(L"ldbracket" == wsToken) return TypeElement::ldbracket;
//			else if(L"lbrace" == wsToken) return TypeElement::lbrace;
//			else if(L"langle" == wsToken) return TypeElement::langle;
//			else if(L"lceil" == wsToken) return TypeElement::lceil;
//			else if(L"lfloor" == wsToken) return TypeElement::lfloor;
//			else if(L"lline" == wsToken) return TypeElement::lline;
//			else if(L"ldline" == wsToken) return TypeElement::ldline;
//			else return TypeElement::undefine;
//	}
	CElement* CElement::CreateElement(CStarMathReader* pReader)
	{
		switch (pReader->GetGlobalType()) {
			case TypeElement::String:
				return new CElementString(pReader->GetString());
			case TypeElement::BinOperator:
				return new CElementBinOperator(pReader->GetLocalType());
			case TypeElement::SetOperations:
				return new CElementSetOperations(pReader->GetLocalType());
			case TypeElement::Connection:
				return new CElementConnection(pReader->GetLocalType());
			case TypeElement::Function:
				return new CElementFunction(pReader->GetLocalType());
			case TypeElement::Bracket:
				return new CElementBracket(pReader->GetLocalType());
			case TypeElement::Operation:
				return new CElementOperator(pReader->GetLocalType());
			case TypeElement::BracketWithIndex:
				return new CElementBracketWithIndex(pReader->GetLocalType());
			case TypeElement::Grade:
				return new CElementGrade();
			case TypeElement::Matrix:
				return new CElementMatrix(pReader->GetLocalType());
			case TypeElement::SpecialSymbol:
				return new CElementSpecialSymbol(pReader->GetLocalType());
			default:
				return nullptr;
		}
	}
	void CElement::SetAttribute(const std::vector<CAttribute *> arAttr)
	{
		m_arElementAttributes = arAttr;
	}
	void CElement::SetIndex(CIndex *pIndex)
	{
		m_pElementIndex = pIndex;
	}
	TypeElement CElement::GetBaseType()
	{
		return m_enBaseType;
	}
	void CElement::SetBaseType(const TypeElement &enType)
	{
		m_enBaseType = enType;
	}
//class methods CElementString
	CElementString::CElementString(const std::wstring& wsTokenString)
	{
		m_wsString = wsTokenString;
		SetBaseType(TypeElement::String);
	}
	CElementString::~CElementString()
	{}
	void CElementString::SetString(const std::wstring& wsTokenString)
	{
		m_wsString = wsTokenString;
	}
	void CElementString::Parse(CStarMathReader* pReader)
	{
		pReader->ClearReader();
	}
	void CElementString::ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite)
	{
		oXmlWrite->WriteNodeBegin(L"m:r",false);
		CConversionSMtoOOXML::StandartProperties(oXmlWrite);
		oXmlWrite->WriteNodeBegin(L"m:t",false);
		oXmlWrite->WriteString(m_wsString);
		oXmlWrite->WriteNodeEnd(L"m:t",false,false);
		oXmlWrite->WriteNodeEnd(L"m:r",false,false);
	}
	std::wstring CElementString::GetString()
	{
		return m_wsString;
	}
	TypeElement CElementString::GetDigit(const std::wstring &wsToken)
	{
		for(wchar_t cOneElement: wsToken)
		{
			if(!isdigit(cOneElement) ||  L'\0' == cOneElement) return TypeElement::undefine;
		}
		return TypeElement::String;
	}
//class methods CElementBinOperator
	CElementBinOperator::CElementBinOperator(const TypeElement& enType): m_pLeftArgument(nullptr) , m_pRightArgument(nullptr)
	{
		m_enTypeBinOp = enType;
		SetBaseType(TypeElement::BinOperator);
	}
	CElementBinOperator::~CElementBinOperator()
	{
		delete m_pLeftArgument;
		delete m_pRightArgument;
	}
	void CElementBinOperator::SetLeftArg(CElement *pElement)
	{
		m_pLeftArgument = pElement;
	}
	void CElementBinOperator::SetRightArg(CElement *pElement)
	{
		m_pRightArgument = pElement;
	}
	void CElementBinOperator::Parse(CStarMathReader* pReader)
	{
		if(m_enTypeBinOp == TypeElement::frac)
		{
			SetLeftArg(CParserStarMathString::ReadingWithoutBracket(pReader));
			SetRightArg(CParserStarMathString::ReadingWithoutBracket(pReader));
		}
		else
		{
			CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
			pReader->GetToken();
			pReader->SetTypesToken();
			if(IsBinOperatorLowPrior() && pReader->GetGlobalType() == TypeElement::BinOperator || pReader->GetGlobalType() == TypeElement::BracketWithIndex)
			{
				CElement* pBinOp = CParserStarMathString::ParseElement(pReader);
				CParserStarMathString::AddLeftArgument(pTempElement,pBinOp);
				SetRightArg(pBinOp);
			}
			else
				SetRightArg(pTempElement);
		}
	}
	void CElementBinOperator::ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite)
	{
		if(m_enTypeBinOp == TypeElement::over || m_enTypeBinOp ==TypeElement::division)
		{
			oXmlWrite->WriteNodeBegin(L"m:f",false);
			if(m_enTypeBinOp == TypeElement::division) CConversionSMtoOOXML::PropertiesMFPR(true,L"lin",oXmlWrite);
			else CConversionSMtoOOXML::PropertiesMFPR(false,L"",oXmlWrite);
			CConversionSMtoOOXML::BlockRecording(L"m:num",m_pLeftArgument,oXmlWrite);
			CConversionSMtoOOXML::BlockRecording(L"m:den",m_pRightArgument,oXmlWrite);
			oXmlWrite->WriteNodeEnd(L"m:f",false,false);
		}
		else if(m_enTypeBinOp == TypeElement::plus ||m_enTypeBinOp == TypeElement::minus || m_enTypeBinOp == TypeElement::multipl || m_enTypeBinOp == TypeElement::cdot || m_enTypeBinOp == TypeElement::times || m_enTypeBinOp == TypeElement::div || m_enTypeBinOp == TypeElement::odivide || m_enTypeBinOp == TypeElement::oplus || m_enTypeBinOp == TypeElement::ominus || m_enTypeBinOp == TypeElement::odot || m_enTypeBinOp == TypeElement::otimes)
		{
			m_pLeftArgument->ConversionToOOXML(oXmlWrite);
			oXmlWrite->WriteNodeBegin(L"m:r",false);
			CConversionSMtoOOXML::StandartProperties(oXmlWrite);
			oXmlWrite->WriteNodeBegin(L"m:t",false);
			switch (m_enTypeBinOp)
			{
			case TypeElement::plus:
				oXmlWrite->WriteString(L"+");
				break;
			case TypeElement::minus:
				oXmlWrite->WriteString(L"-");
				break;
			case TypeElement::multipl:
				oXmlWrite->WriteString(L"*");
				break;
			case TypeElement::cdot:
				oXmlWrite->WriteString(L"\u00B7");
				break;
			case TypeElement::times:
				oXmlWrite->WriteString(L"\u00D7");
				break;
			case TypeElement::div:
				oXmlWrite->WriteString(L"\u00F7");
				break;
			case TypeElement::odivide:
				oXmlWrite->WriteString(L"\u2298");
				break;
			case TypeElement::oplus:
				oXmlWrite->WriteString(L"\u2295");
				break;
			case TypeElement::ominus:
				oXmlWrite->WriteString(L"\u2296");
				break;
			case TypeElement::odot:
				oXmlWrite->WriteString(L"\u2299");
				break;
			case TypeElement::otimes:
				oXmlWrite->WriteString(L"\u2297");
				break;
			default:
				break;
			}
			if(m_pRightArgument->GetBaseType() == TypeElement::String)
			{
				CElementString* oNumber = dynamic_cast<CElementString*> (m_pRightArgument);
				oXmlWrite->WriteString(oNumber->GetString());
				oXmlWrite->WriteNodeEnd(L"m:t",false,false);
				oXmlWrite->WriteNodeEnd(L"m:r",false,false);
			}
			else
			{
				oXmlWrite->WriteNodeEnd(L"m:t",false,false);
				oXmlWrite->WriteNodeEnd(L"m:r",false,false);
				m_pRightArgument->ConversionToOOXML(oXmlWrite);
			}

		}
	}
	void CElementBinOperator::SetTypeBinOP(const TypeElement &enType)
	{
		m_enTypeBinOp = enType;
	}
	TypeElement CElementBinOperator::GetBinOperator(const std::wstring& wsToken)
	{
		if(L"cdot" == wsToken) return TypeElement::cdot;
		else if(L"+" == wsToken) return TypeElement::plus;
		else if(L"-" == wsToken) return TypeElement::minus;
		else if(L"oplus" == wsToken) return TypeElement::oplus;
		else if(L"ominus" == wsToken) return TypeElement::ominus;
		else if(L"circ" == wsToken) return TypeElement::circ;
		else if(L"times" == wsToken) return TypeElement::times;
		else if(L"over" == wsToken) return TypeElement::over;
		else if(L"frac" == wsToken) return TypeElement::frac;
		else if(L"div" == wsToken) return TypeElement::div;
		else if(L"*" == wsToken) return TypeElement::multipl;
		else if(L"/" == wsToken) return TypeElement::division;
		else if(L"odot" == wsToken) return TypeElement::odot;
		else if(L"otimes" == wsToken) return TypeElement::otimes;
		else if(L"odivide" == wsToken) return TypeElement::odivide;
		else if(L"wideslash" == wsToken) return TypeElement::wideslash;
		else if(L"widebslash" == wsToken) return TypeElement::widebslash;
		else return TypeElement::undefine;
	}
	bool CElementBinOperator::IsBinOperatorLowPrior()
	{
		switch (m_enTypeBinOp) {
		case TypeElement::plus:
		return true;
		case TypeElement::minus:
		return true;
		case TypeElement::oplus:
		return true;
		case TypeElement::ominus:
		return true;
		case TypeElement::circ:
		return true;
		default:
		return false;
		}
	}
//class methods CElementBracket
	CElementBracket::CElementBracket(const TypeElement& enType)
	{
		m_enTypeBracket = enType;
		SetBaseType(TypeElement::Bracket);
	}
	CElementBracket::~CElementBracket()
	{
		for(CElement* pTemp:m_arBrecketValue) delete pTemp;
	}
	void CElementBracket::SetBracketValue(const std::vector<CElement*> &arValue)
	{
		m_arBrecketValue = arValue;
	}
	TypeElement CElementBracket::GetBracketOpen(const std::wstring &wsToken)
	{
		if(L"{" == wsToken) return TypeElement::brace;
		else if(L"(" == wsToken) return TypeElement::round;
		else if(L"[" == wsToken) return TypeElement::square;
		else if(L"ldbracket" == wsToken) return TypeElement::ldbracket;
		else if(L"lbrace" == wsToken) return TypeElement::lbrace;
		else if(L"langle" == wsToken) return TypeElement::langle;
		else if(L"lceil" == wsToken) return TypeElement::lceil;
		else if(L"lfloor" == wsToken) return TypeElement::lfloor;
		else if(L"lline" == wsToken) return TypeElement::lline;
		else if(L"ldline" == wsToken) return TypeElement::ldline;
		else return TypeElement::undefine;
	}
	TypeElement CElementBracket::GetBracketClose(const std::wstring &wsToken)
	{
		if(L"}" == wsToken) return TypeElement::rwbrace;
		else if(L")" == wsToken) return TypeElement::rround;
		else if(L"]" == wsToken) return TypeElement::rsquare;
		else if(L"rdbracket" == wsToken) return TypeElement::rdbracket;
		else if(L"rbrace" == wsToken) return TypeElement::rbrace;
		else if(L"rangle" == wsToken) return TypeElement::rangle;
		else if(L"rceil" == wsToken) return TypeElement::rceil;
		else if(L"rfloor" == wsToken) return TypeElement::rfloor;
		else if(L"rline" == wsToken) return TypeElement::rline;
		else if(L"rdline" == wsToken) return TypeElement::rdline;
		else if(L"right" == wsToken) return TypeElement::right;
		else return TypeElement::undefine;
	}
	void CElementBracket::Parse(CStarMathReader* pReader)
	{
		pReader->GetToken();
		TypeElement enBracketClose = GetBracketClose(pReader->GetString());
		if(enBracketClose == TypeElement::undefine)
		{
			pReader->SetTypesToken();
		}
		while(enBracketClose == TypeElement::undefine)
		{
			CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
			if(!m_arBrecketValue.empty() && CParserStarMathString::CheckForLeftArgument(pTempElement->GetBaseType()))
			{
				CParserStarMathString::AddLeftArgument(m_arBrecketValue.back(),pTempElement);
				m_arBrecketValue.pop_back();
			}
			m_arBrecketValue.push_back(pTempElement);
			if(pReader->EmptyString())
			{
				pReader->GetToken();
				enBracketClose = GetBracketClose(pReader->GetString());
				if(enBracketClose  == TypeElement::undefine)
				{
					pReader->SetTypesToken();
				}
			}
			else enBracketClose = GetBracketClose(pReader->GetString());
		}
		//доработать()
		pReader->ClearReader();
	}
	void CElementBracket::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{

	}
//class methods CElementSpecialSymbol
	CElementSpecialSymbol::CElementSpecialSymbol(const TypeElement &enType)
	{
		m_enTypeSpecial = enType;
		SetBaseType(TypeElement::SpecialSymbol);
	}
	CElementSpecialSymbol::~CElementSpecialSymbol()
	{}
	void CElementSpecialSymbol::Parse(CStarMathReader* pReader)
	{
	}
	TypeElement CElementSpecialSymbol::GetSpecialSymbol(const std::wstring &wsToken)
	{
		if(L"emptyset" == wsToken) return TypeElement::emptyset;
		else if(L"aleph" == wsToken) return TypeElement::aleph;
		else if(L"setN" == wsToken) return TypeElement::setN;
		else if(L"setZ" == wsToken) return TypeElement::setZ;
		else if(L"setQ" == wsToken) return TypeElement::setQ;
		else if(L"setR" == wsToken) return TypeElement::setR;
		else if(L"setc" == wsToken) return TypeElement::setC;
		else return TypeElement::undefine;
	}
	void CElementSpecialSymbol::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{}
//class methods CElementSetOperations
	CElementSetOperations::CElementSetOperations(const TypeElement &enType): m_pRightArgument(nullptr), m_pLeftArgument(nullptr)
	{
		m_enTypeSet = enType;
		SetBaseType(TypeElement::SetOperations);
	}
	CElementSetOperations::~CElementSetOperations()
	{
		delete m_pLeftArgument;
		delete m_pRightArgument;
	}
	void CElementSetOperations::SetLeftArg(CElement *pElement)
	{
		m_pLeftArgument = pElement;
	}
	CElement* CElementSetOperations::GetLeftArg()
	{
		return m_pLeftArgument;
	}
	void CElementSetOperations::SetRightArg(CElement *pElement)
	{
		m_pRightArgument = pElement;
	}
	CElement* CElementSetOperations::GetRightArg()
	{
		return m_pRightArgument;
	}
	void CElementSetOperations::Parse(CStarMathReader* pReader)
	{
		CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->GetToken();
		pReader->SetTypesToken();
		if(pReader->GetGlobalType() == TypeElement::BinOperator || pReader->GetGlobalType() == TypeElement::BracketWithIndex)
		{
			CElement* pBinOpElement = CParserStarMathString::ParseElement(pReader);
			CParserStarMathString::AddLeftArgument(pTempElement,pBinOpElement);
			SetRightArg(pBinOpElement);
		}
		else SetRightArg(pTempElement);
	}
	void CElementSetOperations::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		m_pLeftArgument->ConversionToOOXML(pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:r", false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch(m_enTypeSet)
		{
			case TypeElement::Union:
				pXmlWrite->WriteString(L"\u22C3");
				break;
			case TypeElement::intersection:
				pXmlWrite->WriteString(L"\u22C2");
				break;
			case TypeElement::setminus:
				pXmlWrite->WriteString(L"\u2216");
				break;
			case TypeElement::setquotient:
				pXmlWrite->WriteString(L"\u2215");
				break;
			case TypeElement::subset:
				pXmlWrite->WriteString(L"\u2282");
				break;
			case TypeElement::subseteq:
				pXmlWrite->WriteString(L"\u2286");
				break;
			case TypeElement::supset:
				pXmlWrite->WriteString(L"\u2283");
				break;
			case TypeElement::supseteq:
				pXmlWrite->WriteString(L"\u2287");
				break;
			case TypeElement::nsubset:
				pXmlWrite->WriteString(L"\u2284");
				break;
			case TypeElement::nsubseteq:
				pXmlWrite->WriteString(L"\u2288");
				break;
			case TypeElement::nsupset:
				pXmlWrite->WriteString(L"\u2285");
				break;
			case TypeElement::nsupseteq:
				pXmlWrite->WriteString(L"\u2289");
				break;
			case TypeElement::in:
				pXmlWrite->WriteString(L"\u2208");
				break;
			case TypeElement::owns:
				pXmlWrite->WriteString(L"\u220B");
				break;
			case TypeElement::notin:
				pXmlWrite->WriteString(L"\u2209");
				break;
		default:
			break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
		m_pRightArgument->ConversionToOOXML(pXmlWrite);
	}
	TypeElement CElementSetOperations::GetSetOperation(const std::wstring &wsToken)
	{
		if(L"intersection" == wsToken) return TypeElement::intersection;
		else if(L"union" == wsToken) return TypeElement::Union;
		else if(L"setminus" == wsToken) return TypeElement::setminus;
		else if(L"setquotient" == wsToken) return TypeElement::setquotient;
		else if(L"subseteq" == wsToken) return TypeElement::subseteq;
		else if(L"subset" == wsToken) return TypeElement::subset;
		else if(L"supset" == wsToken) return TypeElement::supset;
		else if(L"supseteq" == wsToken) return TypeElement::supseteq;
		else if(L"nsubset" == wsToken) return TypeElement::nsubset;
		else if(L"nsubseteq" == wsToken) return TypeElement::nsubseteq;
		else if(L"nsupset" == wsToken) return TypeElement::nsupset;
		else if(L"nsupseteq" == wsToken) return TypeElement::nsupseteq;
		else if(L"in" == wsToken) return TypeElement::in;
		else if(L"notin" == wsToken) return TypeElement::notin;
		else if(L"owns" == wsToken) return TypeElement::owns;
		else return TypeElement::undefine;
	}
//class methods CElementConnection
	CElementConnection::CElementConnection(const TypeElement& enType): m_pLeftArgument(nullptr), m_pRightArgument(nullptr)
	{
		m_enTypeCon = enType;
		SetBaseType(TypeElement::Connection);
	}
	CElementConnection::~CElementConnection()
	{
		delete m_pLeftArgument;
		delete m_pRightArgument;
	}
	void CElementConnection::SetRightArg(CElement *pElement)
	{
		m_pRightArgument = pElement;
	}
	CElement* CElementConnection::GetRightArg()
	{
		return m_pRightArgument;
	}
	void CElementConnection::SetLeftArg(CElement *pElement)
	{
		m_pLeftArgument = pElement;
	}
	CElement* CElementConnection::GetLeftArg()
	{
		return m_pLeftArgument;
	}
	void CElementConnection::Parse(CStarMathReader *pReader)
	{
		CElement* pTempElement = CParserStarMathString::ParseElement(pReader);
		pReader->GetToken();
		pReader->SetTypesToken();
		if(pReader->GetGlobalType() == TypeElement::BinOperator || pReader->GetGlobalType() == TypeElement::BracketWithIndex)
		{
			CElement* pBinOp = CParserStarMathString::ParseElement(pReader);
			CParserStarMathString::AddLeftArgument(pTempElement,pBinOp);
			SetRightArg(pBinOp);
		}
		else SetRightArg(pTempElement);
	}
	//absent leslant,geslant and toward
	void CElementConnection::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		m_pLeftArgument->ConversionToOOXML(pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch(m_enTypeCon)
		{
			case TypeElement::equals:
				pXmlWrite->WriteString(L"=");
				break;
			case TypeElement::notequals:
				pXmlWrite->WriteString(L"\u2260");
				break;
			case TypeElement::learrow:
				pXmlWrite->WriteString(L"<");
				break;
			case TypeElement::learrowequals:
				pXmlWrite->WriteString(L"\u2264");
				break;
			case TypeElement::riarrow:
				pXmlWrite->WriteString(L">");
				break;
			case TypeElement::riarrowequals:
				pXmlWrite->WriteString(L"\u2265");
				break;
			case TypeElement::dllearrow:
				pXmlWrite->WriteString(L"\u226A");
				break;
			case TypeElement::dlriarrow:
				pXmlWrite->WriteString(L"\u226B");
				break;
			case TypeElement::dlrarrow:
				pXmlWrite->WriteString(L"\u21D4");
				break;
			case TypeElement::drarrow:
				pXmlWrite->WriteString(L"\u21D2");
				break;
			case TypeElement::dlarrow:
				pXmlWrite->WriteString(L"\u21D0");
				break;
			case TypeElement::prec:
				pXmlWrite->WriteString(L"\u227A");
				break;
			case TypeElement::succ:
				pXmlWrite->WriteString(L"\u227B");
				break;
			case TypeElement::preccurlyeq:
				pXmlWrite->WriteString(L"\u227C");
				break;
			case TypeElement::succcurlyeq:
				pXmlWrite->WriteString(L"\u227D");
				break;
			case TypeElement::precsim:
				pXmlWrite->WriteString(L"\u227E");
				break;
			case TypeElement::succsim:
				pXmlWrite->WriteString(L"\u227F");
				break;
			case TypeElement::nprec:
				pXmlWrite->WriteString(L"\u2280");
				break;
			case TypeElement::nsucc:
				pXmlWrite->WriteString(L"\u2281");
				break;
			case TypeElement::approx:
				pXmlWrite->WriteString(L"\u2248");
				break;
			case TypeElement::sim:
				pXmlWrite->WriteString(L"\u223C");
				break;
			case TypeElement::simeq:
				pXmlWrite->WriteString(L"\u2243");
				break;
			case TypeElement::equiv:
				pXmlWrite->WriteString(L"\u2261");
				break;
			case TypeElement::prop:
				pXmlWrite->WriteString(L"\u221D");
				break;
			case TypeElement::parallel:
				pXmlWrite->WriteString(L"\u2225");
				break;
			case TypeElement::ortho:
				pXmlWrite->WriteString(L"\u22A5");
				break;
			case TypeElement::divides:
				pXmlWrite->WriteString(L"\u2223");
				break;
			case TypeElement::ndivides:
				pXmlWrite->WriteString(L"\u2224");
				break;
			case TypeElement::toward:
				pXmlWrite->WriteString(L"\u2192");
				break;
			case TypeElement::transl:
				pXmlWrite->WriteString(L"\u22B7");
				break;
			case TypeElement::transr:
				pXmlWrite->WriteString(L"\u22B6");
				break;
			case TypeElement::def:
				pXmlWrite->WriteString(L"\u225D");
				break;
			default:
				break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t", false, false);
		pXmlWrite->WriteNodeEnd(L"m:r",false ,false);
		m_pRightArgument->ConversionToOOXML(pXmlWrite);
	}
	TypeElement CElementConnection::GetConnection(const std::wstring& wsToken)
	{
		if(L"approx" == wsToken) return TypeElement::approx;
		else if(L"sim" == wsToken) return TypeElement::sim;
		else if(L"simeq" == wsToken) return TypeElement::simeq;
		else if(L"equiv" == wsToken) return TypeElement::equiv;
		else if(L"prop" == wsToken) return TypeElement::prop;
		else if(L"parallel" == wsToken) return TypeElement::parallel;
		else if(L"ortho" == wsToken) return TypeElement::ortho;
		else if(L"divides" == wsToken) return TypeElement::divides;
		else if(L"ndivides" == wsToken) return TypeElement::ndivides;
		else if(L"toward" == wsToken) return TypeElement::toward;
		else if(L"transl" == wsToken) return TypeElement::transl;
		else if(L"transr" == wsToken) return TypeElement::transr;
		else if(L"def" == wsToken) return TypeElement::def;
		else if(L"=" == wsToken) return TypeElement::equals;
		else if(L"<>" == wsToken) return TypeElement::notequals;
		else if(L"<" == wsToken) return TypeElement::learrow;
		else if(L"<=" == wsToken) return TypeElement::learrowequals;
		else if(L"leslant" == wsToken) return TypeElement::leslant;
		else if(L">" == wsToken) return TypeElement::riarrow;
		else if(L">=" == wsToken) return TypeElement::riarrowequals;
		else if(L"geslant" == wsToken) return TypeElement::geslant;
		else if(L"<<" == wsToken) return TypeElement::dllearrow;
		else if(L">>" == wsToken) return TypeElement::dlriarrow;
		else if(L"prec" == wsToken) return TypeElement::prec;
		else if(L"succ" == wsToken) return TypeElement::succ;
		else if(L"preccurlyeq" == wsToken) return TypeElement::preccurlyeq;
		else if(L"succcurlyeq" == wsToken) return TypeElement::succcurlyeq;
		else if(L"precsim" == wsToken) return TypeElement::precsim;
		else if(L"succsim" == wsToken) return TypeElement::succsim;
		else if(L"nprec" == wsToken) return TypeElement::nprec;
		else if(L"nsucc" == wsToken) return TypeElement::nsucc;
		else if(L"dlarrow" == wsToken) return TypeElement::dlarrow;
		else if(L"dlrarrow" == wsToken) return TypeElement::dlrarrow;
		else if(L"drarrow" == wsToken) return TypeElement::drarrow;
		else return TypeElement::undefine;
}
//class methods CIndex
	CIndex::CIndex(const TypeElement& enType): m_pValueIndex(nullptr)
	{
		m_enTypeIndex = enType;
	}
	CIndex::~CIndex()
	{
		delete m_pValueIndex;
	}
	void CIndex::SetValueIndex(CElement *pElement)
	{
		m_pValueIndex = pElement;
	}
	CElement* CIndex::GetValueIndex()
	{
		return m_pValueIndex;
	}
	bool CIndex::IsIndex(const std::wstring &wsCheckToken)
	{
		if(L"^" == wsCheckToken) return true;
		else if(L"_" == wsCheckToken) return true;
		else if(L"lsup" == wsCheckToken) return true;
		else if(L"lsub" == wsCheckToken) return true;
		else if(L"csup" == wsCheckToken) return true;
		else if(L"csub" == wsCheckToken) return true;
		else return false;
	}
	CIndex* CIndex::CreateIndex(const std::wstring &wsToken)
	{
		if(L"^" == wsToken) return new CIndex(TypeElement::upper);
		else if(L"_" == wsToken) return new CIndex(TypeElement::lower);
		else if(L"lsup" == wsToken) return new CIndex(TypeElement::lsup);
		else if(L"lsub" == wsToken) return new CIndex(TypeElement::lsub);
		else if(L"csup" == wsToken) return new CIndex(TypeElement::csup);
		else if(L"csub" == wsToken) return new CIndex(TypeElement::csub);
		else return nullptr;
	}
//class methods CElementFunction
	CElementFunction::CElementFunction(const TypeElement &enType): m_pValue(nullptr)
	{
		m_enTypeFunction = enType;
		SetBaseType(TypeElement::Function);
	}
	CElementFunction::~CElementFunction()
	{
		delete m_pValue;
	}
	void CElementFunction::SetValueFunction(CElement *pElement)
	{
		m_pValue = pElement;
	}
	CElement* CElementFunction::GetValueFunction()
	{
		return m_pValue;
	}

	void CElementFunction::Parse(CStarMathReader* pReader)
	{
		SetValueFunction(CParserStarMathString::ParseElement(pReader));
		pReader->ClearReader();
	}
	void CElementFunction::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:func",false);
		CConversionSMtoOOXML::PropertiesFuncPr(pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:fName",false);
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		pXmlWrite->WriteNodeBegin(L"m:rPr",false);
		pXmlWrite->WriteNodeBegin(L"m:sty",true);
		pXmlWrite->WriteAttribute(L"m:val",L"p");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"m:rPr",false,false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch (m_enTypeFunction) {
		case TypeElement::cos:
			pXmlWrite->WriteString(L"cos");
			break;
		case TypeElement::sin:
			pXmlWrite->WriteString(L"sin");
			break;
		case TypeElement::tan:
			pXmlWrite->WriteString(L"tan");
			break;
		case TypeElement::cot:
			pXmlWrite->WriteString(L"cot");
			break;
		case TypeElement::sinh:
			pXmlWrite->WriteString(L"sinh");
			break;
		case TypeElement::cosh:
			pXmlWrite->WriteString(L"cosh");
			break;
		case TypeElement::tanh:
			pXmlWrite->WriteString(L"tanh");
			break;
		case TypeElement::coth:
			pXmlWrite->WriteString(L"coth");
			break;
		case TypeElement::arcsin:
			pXmlWrite->WriteString(L"arcsin");
			break;
		case TypeElement::arccos:
			pXmlWrite->WriteString(L"arccos");
			break;
		case TypeElement::arctan:
			pXmlWrite->WriteString(L"arctan");
			break;
		case TypeElement::arccot:
			pXmlWrite->WriteString(L"arccot");
			break;
		case TypeElement::arsinh:
			pXmlWrite->WriteString(L"arsinh");
			break;
		case TypeElement::arcosh:
			pXmlWrite->WriteString(L"arcosh");
			break;
		case TypeElement::artanh:
			pXmlWrite->WriteString(L"artanh");
			break;
		case TypeElement::arcoth:
			pXmlWrite->WriteString(L"arcoth");
			break;
		default:
			break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
		pXmlWrite->WriteNodeEnd(L"m:fName",false,false);
		pXmlWrite->WriteNodeBegin(L"m:e",false);
		m_pValue->ConversionToOOXML(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		pXmlWrite->WriteNodeEnd(L"m:func",false,false);
	}
	TypeElement CElementFunction::GetFunction(const std::wstring &wsToken)
	{
		if(L"abs" == wsToken) return TypeElement::abs;
		else if(L"fact" == wsToken) return TypeElement::fact;
		else if(L"sqrt" == wsToken) return TypeElement::sqrt;
		else if(L"sin" == wsToken) return TypeElement::sin;
		else if(L"cos" == wsToken) return TypeElement::cos;
		else if(L"tan" == wsToken) return TypeElement::tan;
		else if(L"cot" == wsToken) return TypeElement::cot;
		else if(L"sinh" == wsToken) return TypeElement::sinh;
		else if(L"cosh" == wsToken) return TypeElement::cosh;
		else if(L"tanh" == wsToken) return TypeElement::tanh;
		else if(L"coth" == wsToken) return TypeElement::coth;
		else if(L"arcsin" == wsToken) return TypeElement::arcsin;
		else if(L"arccos" == wsToken) return TypeElement::arccos;
		else if(L"arctan" == wsToken) return TypeElement::arctan;
		else if(L"arccot" == wsToken) return TypeElement::arccot;
		else if(L"arsinh" == wsToken) return TypeElement::arsinh;
		else if(L"arcosh" == wsToken) return TypeElement::arcosh;
		else if(L"artanh" == wsToken) return TypeElement::artanh;
		else if(L"arcoth" == wsToken) return TypeElement::arcoth;
		else if(L"ln" == wsToken) return TypeElement::ln;
		else if(L"exp" == wsToken) return TypeElement::exp;
		else if(L"log" == wsToken) return TypeElement::log;
		else return TypeElement::undefine;
	}
//class methods CElementOperation
	CElementOperator::CElementOperator(const TypeElement &enType): m_pValueFrom(nullptr), m_pValueTo(nullptr), m_pValueOperator(nullptr)
	{
		m_enTypeOperator = enType;
		SetBaseType(TypeElement::Operation);
	}
	CElementOperator::~CElementOperator()
	{
		delete m_pValueOperator;
		delete m_pValueFrom;
		delete m_pValueTo;
	}
	void CElementOperator::SetValueOperator(CElement *pElement)
	{
		m_pValueOperator = pElement;
	}
	CElement* CElementOperator::GetValueOperator()
	{
		return m_pValueOperator;
	}
	void CElementOperator::SetFromValue(CElement *pElement)
	{
		m_pValueFrom = pElement;
	}
	CElement* CElementOperator::GetFromValue()
	{
		return m_pValueFrom;
	}
	void CElementOperator::SetToValue(CElement *pElement)
	{
		m_pValueTo = pElement;
	}
	CElement* CElementOperator::GetToValue()
	{
		return m_pValueTo;
	}
	TypeElement CElementOperator::GetFromOrTo(const std::wstring &wsToken)
	{
		if(L"from" == wsToken) return TypeElement::from;
		else if(L"to" == wsToken) return TypeElement::to;
		else return TypeElement::undefine;
	}
	TypeElement CElementOperator::GetOperator(const std::wstring &wsToken)
	{
		if(L"lim" == wsToken) return TypeElement::lim;
		if(L"sum" == wsToken) return TypeElement::sum;
		if(L"liminf" == wsToken) return TypeElement::liminf;
		if(L"limsup" == wsToken) return TypeElement::limsup;
		if(L"prod" == wsToken) return TypeElement::prod;
		if(L"coprod" == wsToken) return TypeElement::coprod;
		if(L"Int" == wsToken) return TypeElement::Int;
		if(L"iint" == wsToken) return TypeElement::iint;
		if(L"iiint" == wsToken) return TypeElement::iiint;
		if(L"lint" == wsToken) return TypeElement::lint;
		if(L"llint" == wsToken) return TypeElement::llint;
		if(L"lllint" == wsToken) return TypeElement::lllint;
		else return TypeElement::undefine;
	}
	void CElementOperator::Parse(CStarMathReader* pReader)
	{
		pReader->GetToken();
		pReader->SetTypesToken();
		if(pReader->GetLocalType() == TypeElement::from)
		{
			pReader->ClearReader();
			SetFromValue(CParserStarMathString::ReadingWithoutBracket(pReader));
		}
		if(pReader->GetLocalType() == TypeElement::to)
		{
			pReader->ClearReader();
			SetToValue(CParserStarMathString::ReadingWithoutBracket(pReader));
		}

		SetValueOperator(CParserStarMathString::ReadingWithoutBracket(pReader));
	}
	void CElementOperator::ConversionToOOXML(XmlUtils::CXmlWriter* oXmlWrite)
	{
		if(m_enTypeOperator == TypeElement::sum)
		{
			oXmlWrite->WriteNodeBegin(L"m:nary",false);
			CConversionSMtoOOXML::PropertiesNaryPr(L"\u2211",nullptr == m_pValueFrom,nullptr == m_pValueTo,oXmlWrite);
			if(m_pValueFrom == nullptr) oXmlWrite->WriteNode(L"m:sub",L"");
			else
			{
				CConversionSMtoOOXML::BlockRecording(L"m:sub",m_pValueFrom,oXmlWrite);
			}
			if(m_pValueTo == nullptr) oXmlWrite->WriteNode(L"m:sup",L"");
			else
			{
				CConversionSMtoOOXML::BlockRecording(L"m:sup",m_pValueTo,oXmlWrite);
			}
			CConversionSMtoOOXML::BlockRecording(L"m:e",m_pValueOperator,oXmlWrite);
			oXmlWrite->WriteNodeEnd(L"m:nary",false,false);
		}
	}
// class methods CStarMathReader
	CStarMathReader::CStarMathReader(std::wstring::iterator& itStart, std::wstring::iterator& itEnd): m_enGlobalType(TypeElement::Empty)
	{
		m_itStart = itStart;
		m_itEnd = itEnd;
	}
	CStarMathReader::~CStarMathReader()
	{}
	void CStarMathReader::GetToken()
	{
		if(m_itStart != m_itEnd)
		{
			m_wsToken = GetElement();
			if(m_wsToken == L"left") m_wsToken = GetElement();
			else if(L"right" == m_wsToken ) m_wsToken = GetElement();
		}
	}
	void CStarMathReader::SetTypesToken()
	{
		m_enUnderType = CAttribute::GetTypeAttribute(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Attribute;
			return;
		}
		m_enUnderType = CElementOperator::GetFromOrTo(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Operation;
			return;
		}
		m_enUnderType = CElementBracketWithIndex::GetBracketWithIndex(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::BracketWithIndex;
			return;
		}
		m_enUnderType = CElementGrade::GetGrade(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Grade;
			return;
		}
		m_enUnderType=CElementMatrix::GetMatrix(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Matrix;
			return;
		}
		m_enUnderType = CElementBracket::GetBracketOpen(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Bracket;
			return;
		}
		m_enUnderType = CElementString::GetDigit(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::String;
			return;
		}
		m_enUnderType = CElementSpecialSymbol::GetSpecialSymbol(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::SpecialSymbol;
			return;
		}
		m_enUnderType = CElementBinOperator::GetBinOperator(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::BinOperator;
			return;
		}
		m_enUnderType = CElementSetOperations::GetSetOperation(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::SetOperations;
			return;
		}
		m_enUnderType = CElementConnection::GetConnection(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Connection;
			return;
		}
		m_enUnderType = CElementFunction::GetFunction(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Function;
			return;
		}
		m_enUnderType = CElementOperator::GetOperator(m_wsToken);
		if(m_enUnderType != TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Operation;
			return;
		}
		if(m_enUnderType == TypeElement::undefine)
		{
			m_enGlobalType = TypeElement::Empty;
			return;
		}
	}
	TypeElement CStarMathReader::GetLocalType()
	{
		return m_enUnderType;
	}
	TypeElement CStarMathReader::GetGlobalType()
	{
		return m_enGlobalType;
	}
	std::wstring CStarMathReader::GetString()
	{
		return m_wsToken;
	}
//	void CStarMathReader::SkipNextElement()
//	{
//		if(m_itStart !=m_itEnd)
//		{
//			std::wstring wsNextElement = CParserStarMathString::GetElement(m_itStart,m_itEnd);
//		}
//	}
	void CStarMathReader::ClearReader()
	{
		m_wsToken.clear();
		m_enGlobalType = TypeElement::Empty;
		m_enUnderType = TypeElement::Empty;
	}
	bool CStarMathReader::EmptyString()
	{
		return m_wsToken.empty();
	}
	bool CStarMathReader::CheckIteratorPosition()
	{
		if(m_itStart !=m_itEnd) return false;
		else return true;
	}
	std::wstring CStarMathReader::GetElement()
	{
		std::wstring m_wsElement{};
		for(; m_itStart != m_itEnd;m_itStart++)
		{
			if(iswspace(*m_itStart) && m_wsElement.empty()) continue;
			else if(iswspace(*m_itStart) && !m_wsElement.empty())
			{
				m_itStart++;
				break;
			}
			else if(!m_wsElement.empty() && (*m_itStart == L'(' || L')' == *m_itStart || *m_itStart == L'{' || *m_itStart == L'}' || *m_itStart == L'+' || *m_itStart == L'-' || *m_itStart == L'/' || *m_itStart == L'*' || L'^' == *m_itStart || L'_' == *m_itStart  || (iswdigit(*m_itStart) && !iswdigit(m_wsElement.back())) || (iswalpha(*m_itStart) && !iswalpha(m_wsElement.back())) || ((m_wsElement.back() != L'<' && m_wsElement.back() != L'>') && (L'<' == *m_itStart || L'>' == *m_itStart || L'=' == *m_itStart))))
			{
				return m_wsElement;
			}
			else if(((*m_itStart == L'{' || *m_itStart == L'}' || *m_itStart == L'+' || *m_itStart == L'-' || *m_itStart == L'/' || *m_itStart == L'*' || L'^' == *m_itStart || L'_' == *m_itStart || L'=' == *m_itStart ) && m_wsElement.empty()) || (!m_wsElement.empty() && ((m_wsElement.back() == L'<' && (L'=' == *m_itStart || L'<' == *m_itStart || L'>' == *m_itStart)) || (m_wsElement.back() == L'>' && (L'>' == *m_itStart || L'=' == *m_itStart)) ) ) )
			{
				m_wsElement.push_back(*m_itStart);
				m_itStart++;
				return m_wsElement;
			}
			else
			{
				m_wsElement.push_back(*m_itStart);
			}
		}
		if(!m_wsElement.empty()) return m_wsElement;
		else return {};
	}
//class methods CElementBracketWithIndex
	CElementBracketWithIndex::CElementBracketWithIndex(const TypeElement &enType): m_pLeftArg(nullptr), m_pValue(nullptr)
	{
		m_enTypeBracketWithIndex = enType;
		SetBaseType(TypeElement::BracketWithIndex);
	}
	CElementBracketWithIndex::~CElementBracketWithIndex()
	{
		delete m_pLeftArg;
		delete m_pValue;
	}
	void CElementBracketWithIndex::SetLeftArg(CElement *pElement)
	{
		m_pLeftArg = pElement;
	}
	void CElementBracketWithIndex::SetBracketValue(CElement *pElement)
	{
		m_pValue = pElement;
	}
	void CElementBracketWithIndex::Parse(CStarMathReader *pReader)
	{
		SetBracketValue(CParserStarMathString::ParseElement(pReader));
	}
	void CElementBracketWithIndex::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{

	}
	TypeElement CElementBracketWithIndex::GetBracketWithIndex(const std::wstring &wsToken)
	{
		if(L"overbrace" == wsToken) return TypeElement::overbrace;
		else if(L"underbrace" == wsToken) return TypeElement::underbrace;
		else return TypeElement::undefine;
	}
//class methods CElementGrade
	CElementGrade::CElementGrade(): m_pValueFrom(nullptr), m_pValueGrade(nullptr), m_pValueTo(nullptr)
	{
		SetBaseType(TypeElement::Grade);
	}
	CElementGrade::~CElementGrade()
	{
		delete m_pValueFrom;
		delete m_pValueTo;
		delete m_pValueGrade;
	}
	void CElementGrade::SetValueFrom(CElement* pElement)
	{
		m_pValueFrom = pElement;
	}
	void CElementGrade::SetValueTo(CElement *pElement)
	{
		m_pValueTo = pElement;
	}
	void CElementGrade::SetValueGrade(CElement *pElement)
	{
		m_pValueGrade = pElement;
	}
	void CElementGrade::Parse(CStarMathReader *pReader)
	{
		SetValueGrade(CParserStarMathString::ParseElement(pReader));
		pReader->GetToken();
		pReader->SetTypesToken();
		if(pReader->GetLocalType() == TypeElement::from)
		{
			pReader->ClearReader();
			SetValueFrom(CParserStarMathString::ParseElement(pReader));
			pReader->GetToken();
			pReader->SetTypesToken();
		}
		if(pReader->GetLocalType() == TypeElement::to)
		{
			pReader->ClearReader();
			SetValueTo(CParserStarMathString::ParseElement(pReader));
		}
	}
	void CElementGrade::ConversionToOOXML(XmlUtils::CXmlWriter *oXmlWrite)
	{

	}
	TypeElement CElementGrade::GetGrade(const std::wstring &wsToken)
	{
		if(L"evaluate" == wsToken) return TypeElement::evaluate;
		else return TypeElement::undefine;
	}
//class methods CElementMatrix
	CElementMatrix::CElementMatrix(const TypeElement &enType): m_pFirstArgument(nullptr), m_pSecondArgument(nullptr)
	{
		m_enTypeMatrix = enType;
		SetBaseType(TypeElement::Matrix);
	}
	CElementMatrix::~CElementMatrix()
	{
		delete m_pFirstArgument;
		delete m_pSecondArgument;
	}
	void CElementMatrix::SetFirstArgument(CElement *pElement)
	{
		m_pFirstArgument = pElement;
	}
	void CElementMatrix::SetSecondArgument(CElement *pElement)
	{
		m_pSecondArgument = pElement;
	}
	TypeElement CElementMatrix::GetMatrix(const std::wstring &wsToken)
	{
		if(L"binom" == wsToken) return TypeElement::binom;
		else if(L"stack" == wsToken) return TypeElement::stack;
		else if(L"matrix" == wsToken) return TypeElement::matrix;
		else return TypeElement::undefine;
	}
	void CElementMatrix::Parse(CStarMathReader *pReader)
	{
		if(m_enTypeMatrix == TypeElement::binom)
		{
			SetFirstArgument(CParserStarMathString::ReadingWithoutBracket(pReader));
			SetSecondArgument(CParserStarMathString::ReadingWithoutBracket(pReader));
		}
		else
		{
			SetFirstArgument(CParserStarMathString::ParseElement(pReader));
		}
	}
	void CElementMatrix::ConversionToOOXML(XmlUtils::CXmlWriter *pXmlWrite)
	{

	}
}


