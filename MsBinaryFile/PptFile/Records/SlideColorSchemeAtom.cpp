#include "SlideColorSchemeAtom.h"

using namespace PPT;

CRecordSplitMenuColorRecord::CRecordSplitMenuColorRecord()
{
}

CRecordSplitMenuColorRecord::~CRecordSplitMenuColorRecord()
{
}

void CRecordSplitMenuColorRecord::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    NSStreamReader::Read(pStream, m_oFillColor);
    NSStreamReader::Read(pStream, m_oLineColor);
    NSStreamReader::Read(pStream, m_oShadowColor);
    NSStreamReader::Read(pStream, m_o3DColor);
}

CRecordMostRecentlyUserColors::CRecordMostRecentlyUserColors()
{
}

CRecordMostRecentlyUserColors::~CRecordMostRecentlyUserColors()
{
}

void CRecordMostRecentlyUserColors::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    for (unsigned int i = 0; i < m_oHeader.RecLen / 4; i++)
    {
        ODRAW::SColorAtom color;
        NSStreamReader::Read(pStream, color);

        m_oColors.push_back(color);
    }
}
