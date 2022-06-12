#ifndef COLORTABLE_H
#define COLORTABLE_H

#include "../DesktopEditor/common/StringBuilder.h"
#include <algorithm>
#include <map>

#ifdef __cplusplus

class ColorTable
{
public:
    ColorTable ()
    {
        InitClrTable ();
    }

    inline std::wstring ConverColorToString(const unsigned int& sKey)
    {
        std::map<unsigned int, std::wstring>::iterator iter = m_Table.find(sKey);
        if (iter == m_Table.end())
            return L"white";
        return iter->second;
    }

private:
    std::map<unsigned int, std::wstring> m_Table;

private:
    void InitClrTable()
    {
        if (m_Table.size())
            return;

        m_Table.insert({0xFFFF00, L"yellow" });
        m_Table.insert({0x00FF00, L"green" });
        m_Table.insert({0x00FFFF, L"cyan" });
        m_Table.insert({0xFF00FF, L"magenta" });
        m_Table.insert({0x0000FF, L"blue" });
        m_Table.insert({0xFF0000, L"red" });
        m_Table.insert({0x00008B, L"darkBlue" });
        m_Table.insert({0x008B8B, L"darkCyan" });
        m_Table.insert({0x006400, L"darkGreen" });
        m_Table.insert({0x800080, L"darkMagenta" });
        m_Table.insert({0x8B0000, L"darkRed" });
        m_Table.insert({0x808000, L"darkYellow" });
        m_Table.insert({0xA9A9A9, L"darkGray" });
        m_Table.insert({0xD3D3D3, L"lightGray" });
        m_Table.insert({0x000000, L"black" });
        m_Table.insert({0xFFFFFF, L"white" });
    }
};

#endif /*__cplusplus*/

#endif // COLORTABLE_H
