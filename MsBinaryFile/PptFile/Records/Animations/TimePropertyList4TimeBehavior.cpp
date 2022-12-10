#include "TimePropertyList4TimeBehavior.h"

using namespace PPT;

CRecordTimePropertyList4TimeBehavior::~CRecordTimePropertyList4TimeBehavior()
{
    CRecordsContainer::Clear();
}


void CRecordTimePropertyList4TimeBehavior::ReadFromStream(SRecordHeader &thisHeader, POLE::Stream *pStream)
{
    m_oHeader = thisHeader;

    UINT lCurLen = 0;
    SRecordHeader oHeader;

    while (lCurLen < m_oHeader.RecLen)
    {
        if (oHeader.ReadFromStream(pStream)) {
            lCurLen += 8 + oHeader.RecLen;
            IRecord* pRecord = FactoryTimeVariant4Behavior::createByInstanse(oHeader.RecInstance);
            if (pRecord)
            {
                pRecord->ReadFromStream(oHeader, pStream);
                m_arRecords.push_back(pRecord);
            }
            else
                break;
        }
    }
}
