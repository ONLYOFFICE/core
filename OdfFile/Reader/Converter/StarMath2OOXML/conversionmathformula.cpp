#include "conversionmathformula.h"
#include "cconversionsmtoooxml.h"
#include "cstarmathpars.h"
#include "TFormulaSize.h"

namespace StarMath
{

	CStarMathConverter::CStarMathConverter():m_pParser(nullptr)
	{
		m_pParser = new CParserStarMathString;
	}
	CStarMathConverter::~CStarMathConverter()
	{
		if(m_pParser != nullptr)
			delete m_pParser;
	}

	void CStarMathConverter::SetBaseFont(const std::wstring &wsNameFont)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseFont(wsNameFont);
	}

	void CStarMathConverter::SetBaseSize(const unsigned int &nSize)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseSize(nSize);
	}

	void CStarMathConverter::SetBaseAlignment(const unsigned int &nAlignment)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseAlignment(nAlignment);
	}

	void CStarMathConverter::SetBaseItalic(bool bItal)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseItalic(bItal);
	}

	void CStarMathConverter::SetBaseBold(bool bBold)
	{
		if(m_pParser != nullptr)
			m_pParser->SetBaseBold(bBold);
	}

	std::queue<TFormulaSize> CStarMathConverter::GetFormulaSize()
	{
		if(m_pParser != nullptr)
			return m_pParser->GetFormulaSize();
		return std::queue<TFormulaSize>{};
	}

	std::wstring CStarMathConverter::ConvertStarMathToOOXml(const std::wstring &wsFormula, const unsigned int &iTypeConversion)
	{
		StarMath::CConversionSMtoOOXML oConverterSM;
		oConverterSM.StartConversion(m_pParser->Parse(wsFormula,iTypeConversion),m_pParser->GetAlignment());
		return oConverterSM.GetOOXML();
	}
	std::wstring CStarMathConverter::ConvertEQNToOOXml(const std::wstring& wsFormula)
	{
		StarMath::CConversionSMtoOOXML oConverterEQN;
		oConverterEQN.StartConversion(StarMath::CParserStarMathString().ParseEQN(wsFormula));
		return oConverterEQN.GetOOXML();
	}

}
