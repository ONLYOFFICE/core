#ifndef CELEMENT_H
#define CELEMENT_H

#include <map>
#include <vector>

namespace NSCSS
{
    class CElement
    {
        std::map<std::wstring, std::wstring> m_mStyle;
        std::wstring sSelector;

        std::vector<CElement*> *m_oClass;
        std::vector<CElement*> *m_oId;
        std::vector<CElement*> m_oNextElement;
    public:
        CElement();
        ~CElement();
        void AddPropertie(const std::wstring& sName, const std::wstring sValue);
        void AddProperties(std::map<std::wstring, std::wstring> mProperties);
    };
}


#endif // CELEMENT_H
