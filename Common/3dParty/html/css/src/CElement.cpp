#include "CElement.h"

namespace NSCSS
{
    CElement::CElement()
    {

    }

    void NSCSS::CElement::AddPropertie(const std::wstring &sName, const std::wstring sValue)
    {
        m_mStyle[sName] = sValue;
    }

    void CElement::AddProperties(std::map<std::wstring, std::wstring> mProperties)
    {
        for (const std::pair<std::wstring, std::wstring>& pPropertie : mProperties)
            m_mStyle[pPropertie.first] = pPropertie.second;
    }

}

