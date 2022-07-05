#pragma once
#include <string>
#include <map>
#include <algorithm>

enum eSimpleLineType
{
    sltUnknown,
    sltDot,
    sltDash,
    sltWave
};

enum eLineType
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

        inline std::wstring ConverLineToString(const unsigned int& sKey)
        {
            auto iter = m_Table.find(sKey);
            if (iter == m_Table.end())
            {
                return L"\"none\"";
            }
            else
            {
                return iter->second;
            }
        }

    private:
        std::map<unsigned int, std::wstring> m_Table;

    private:
        void InitLinesTable()
        {
            if (m_Table.size())
                return;

            //ECMA-376 Part 1 17.18.99 ST_Underline (Underline Patterns)
            m_Table.insert({ltSingle,          L"\"single\""});
            m_Table.insert({ltDouble,          L"\"double\"" });
            m_Table.insert({ltThick,           L"\"thick\"" });
            m_Table.insert({ltDotted,          L"\"dotted\"" });
            m_Table.insert({ltDottedHeavy,     L"\"dottedHeavy\"" });
            m_Table.insert({ltDash,            L"\"dash\"" });
            m_Table.insert({ltDashedHeavy,     L"\"dashedHeavy\"" });
            m_Table.insert({ltDashLong,        L"\"dashLong\"" });
            m_Table.insert({ltDashLongHeavy,   L"\"dashLongHeavy\"" });
            m_Table.insert({ltDotDash,         L"\"dotDash\"" });
            m_Table.insert({ltDashDotHeavy,    L"\"dashDotHeavy\"" });
            m_Table.insert({ltDotDotDash,      L"\"dotDotDash\"" });
            m_Table.insert({ltDashDotDotHeavy, L"\"dashDotDotHeavy\"" });
            m_Table.insert({ltWave,            L"\"wave\"" });
            m_Table.insert({ltWavyHeavy,       L"\"wavyHeavy\"" });
            m_Table.insert({ltWavyDouble,      L"\"wavyDouble\"" });
            m_Table.insert({ltWords,           L"\"words\"" });
            m_Table.insert({ltNone,            L"\"none\"" });
        }
};

