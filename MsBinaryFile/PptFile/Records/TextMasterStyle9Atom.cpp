#include "TextMasterStyle9Atom.h"

using namespace PPT;

void CRecordTextMasterStyle9Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    LONG lPos; StreamUtils::StreamPosition(lPos, pStream);


    m_cLevels = StreamUtils::ReadWORD(pStream);
    if (m_cLevels > 0)
    {
        m_lstLvl1 = new STextMasterStyle9Level;
        m_lstLvl1->ReadFromStream(pStream);
        if (m_cLevels > 1)
        {
            m_lstLvl2 = new STextMasterStyle9Level;
            m_lstLvl2->ReadFromStream(pStream);
            if (m_cLevels > 2)
            {
                m_lstLvl3 = new STextMasterStyle9Level;
                m_lstLvl3->ReadFromStream(pStream);
                if (m_cLevels > 3)
                {
                    m_lstLvl4 = new STextMasterStyle9Level;
                    m_lstLvl4->ReadFromStream(pStream);
                    if (m_cLevels > 4)
                    {
                        m_lstLvl5 = new STextMasterStyle9Level;
                        m_lstLvl5->ReadFromStream(pStream);
                    }
                }
            }
        }
    }

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
