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

enum class eSimpleLineType
{
    sltUnknown,
    sltDot,
    sltDash,
    sltLongDash,
    sltWave
};

enum class eLineType
{
    ltUnknown,
    ltSingle,
    ltDouble,
    ltThick,
    ltDotted,
    ltDottedHeavy,
    ltDash,
    ltDashedHeavy,
    ltDashLong,
    ltDashLongHeavy,
    ltDotDash,
    ltDashDotHeavy,
    ltDotDotDash,
    ltDashDotDotHeavy,
    ltWave,
    ltWavyHeavy,
    ltWavyDouble,
    ltWords,
    ltNone
};

class LinesTable
{
    public:
        LinesTable ()
        {
            InitLinesTable ();
        }

        inline std::wstring ConverLineToString(const eLineType& sKey)
        {
            auto iter = m_Table.find(sKey);
            return iter == m_Table.end() ? L"\"none\"" : iter->second;
        }

    private:
        std::map<eLineType, std::wstring> m_Table;

    private:
        void InitLinesTable()
        {
            if (m_Table.size())
                return;

            //ECMA-376 Part 1 17.18.99 ST_Underline (Underline Patterns)
            m_Table.insert({eLineType::ltSingle,          L"\"single\""});
            m_Table.insert({eLineType::ltDouble,          L"\"double\"" });
            m_Table.insert({eLineType::ltThick,           L"\"thick\"" });
            m_Table.insert({eLineType::ltDotted,          L"\"dotted\"" });
            m_Table.insert({eLineType::ltDottedHeavy,     L"\"dottedHeavy\"" });
            m_Table.insert({eLineType::ltDash,            L"\"dash\"" });
            m_Table.insert({eLineType::ltDashedHeavy,     L"\"dashedHeavy\"" });
            m_Table.insert({eLineType::ltDashLong,        L"\"dashLong\"" });
            m_Table.insert({eLineType::ltDashLongHeavy,   L"\"dashLongHeavy\"" });
            m_Table.insert({eLineType::ltDotDash,         L"\"dotDash\"" });
            m_Table.insert({eLineType::ltDashDotHeavy,    L"\"dashDotHeavy\"" });
            m_Table.insert({eLineType::ltDotDotDash,      L"\"dotDotDash\"" });
            m_Table.insert({eLineType::ltDashDotDotHeavy, L"\"dashDotDotHeavy\"" });
            m_Table.insert({eLineType::ltWave,            L"\"wave\"" });
            m_Table.insert({eLineType::ltWavyHeavy,       L"\"wavyHeavy\"" });
            m_Table.insert({eLineType::ltWavyDouble,      L"\"wavyDouble\"" });
            m_Table.insert({eLineType::ltWords,           L"\"words\"" });
            m_Table.insert({eLineType::ltNone,            L"\"none\"" });
        }
};

