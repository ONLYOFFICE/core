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

    std::map<std::string, std::string> CCssCalculator::GetCompiledStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure)
    {
        return  m_pInternal->GetCompiledStyle(arSelectors, unitMeasure);
    }

    std::map<std::wstring, std::wstring> CCssCalculator::GetCompiledStyleW(std::vector<std::string> arSelectors, UnitMeasure unitMeasure)
    {
        return  m_pInternal->GetCompiledStyleW(arSelectors, unitMeasure);
    }

    void CCssCalculator::AddStyle(std::vector<std::string> sSelectors, const std::string &sStyle)
    {
        m_pInternal->AddStyle(sSelectors, sStyle);
    }

    void CCssCalculator::AddStyle(const std::string &sStyle)
    {
        m_pInternal->AddStyle(sStyle);
    }

    void CCssCalculator::AddStyles(const std::wstring &sFileName)
    {
        m_pInternal->AddStyles(sFileName);
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
