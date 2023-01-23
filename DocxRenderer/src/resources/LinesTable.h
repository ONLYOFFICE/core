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

