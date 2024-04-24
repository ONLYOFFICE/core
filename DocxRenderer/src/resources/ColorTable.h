/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
