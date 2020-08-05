#include "CCssCalculator.h"
#include "CCssCalculator_Private.h"


namespace NSCSS
{
    CCssCalculator::CCssCalculator()
    {
        m_pInternal = new CCssCalculator_Private();
    }

    CCssCalculator::~CCssCalculator()
    {
        delete m_pInternal;
    }

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> CCssCalculator::GetDeclarations(const std::wstring& sSelector)
    {
        return m_pInternal->GetDeclarations(sSelector);
    }


    CCompiledStyle CCssCalculator::GetCompiledStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure)
    {
        return  m_pInternal->GetCompiledStyle(arSelectors, unitMeasure);
    }

    void CCssCalculator::AddStyle(std::vector<std::string> sSelectors, const std::string &sStyle)
    {
        m_pInternal->AddStyle(sSelectors, sStyle);
    }

    void CCssCalculator::AddStyles(const std::string &sStyle)
    {
        m_pInternal->AddStyles(sStyle);
    }

    void CCssCalculator::AddStylesFromFile(const std::wstring &sFileName)
    {
        m_pInternal->AddStylesFromFile(sFileName);
    }

    void CCssCalculator::SetUnitMeasure(const UnitMeasure nType)
    {
        m_pInternal->SetUnitMeasure(nType);
    }

    void CCssCalculator::SetDpi(const int nValue)
    {
        m_pInternal->SetDpi(nValue);
    }

    UnitMeasure CCssCalculator::GetUnitMeasure()
    {
        return m_pInternal->GetUnitMeasure();
    }

    std::wstring CCssCalculator::GetEncoding()
    {
        return m_pInternal->GetEncoding();
    }

    int CCssCalculator::GetDpi()
    {
        return m_pInternal->GetDpi();
    }

    void CCssCalculator::Print()
    {
        m_pInternal->Print();
    }
}
