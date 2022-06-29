#pragma once
#include <string>
#include <map>
#include <algorithm>

class ColorTable
{
    public:
        ColorTable ()
        {
            InitClrTable ();
        }

        inline std::wstring ConverColorToString(const unsigned int& sKey)
        {
            auto iter = m_Table.find(sKey);
            if (iter == m_Table.end())
            {
                //note если не нашли стандартный цвет, отсылаем что есть
                return L"none";
            }
            else
            {
                return iter->second;
            }
        }

        inline bool IsStandardColor(const unsigned int& sKey)
        {
            auto iter = m_Table.find(sKey);
            return iter == m_Table.end() ? false : true;
        }

    private:
        std::map<unsigned int, std::wstring> m_Table;

    private:
        void InitClrTable()
        {
            if (m_Table.size())
                return;

            //ECMA-376-1:2016 17.18.40 ST_HighlightColor (Text Highlight Colors)
            m_Table.insert({0x000000, L"black" });
            m_Table.insert({0x0000FF, L"blue" });
            m_Table.insert({0x00FFFF, L"cyan" });
            m_Table.insert({0x00008B, L"darkBlue" });
            m_Table.insert({0x008B8B, L"darkCyan" });
            m_Table.insert({0xA9A9A9, L"darkGray" });
            m_Table.insert({0x006400, L"darkGreen" });
            m_Table.insert({0x800080, L"darkMagenta" });
            m_Table.insert({0x8B0000, L"darkRed" });
            m_Table.insert({0x808000, L"darkYellow" });
            m_Table.insert({0x00FF00, L"green" });
            m_Table.insert({0xD3D3D3, L"lightGray" });
            m_Table.insert({0xFF00FF, L"magenta" });
            m_Table.insert({0xFF0000, L"red" });
            m_Table.insert({0xFFFFFF, L"white" });
            m_Table.insert({0xFFFF00, L"yellow" });

            //note Больше цветов здесь
            //core\Common\3dParty\html\css\src\ConstValues.h
            //core\DesktopEditor\agg-2.4\svg\agg_svg_color_parser.cpp
        }
};
