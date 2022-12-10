#include "TimeAnimationValueListEntry.h"

using namespace PPT;

CRecordTimeAnimationValueListEntry::CRecordTimeAnimationValueListEntry()
{

}

CRecordTimeAnimationValueListEntry::~CRecordTimeAnimationValueListEntry()
{

}

void CRecordTimeAnimationValueListEntry::ReadFromStream(UINT &CurLen, SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oTimeAnimationValueAtom.ReadFromStream ( oHeader, pStream );
    CurLen += 4;

//        UINT res = 0;
    SRecordHeader ReadHeader;

    ReadHeader.ReadFromStream(pStream);
    if ( ReadHeader.RecInstance == 0 &&
         ReadHeader.RecType == RT_TimeVariant )
    {
        m_pVarValue = TimeVariantFactoryMethod( ReadHeader, pStream );
        CurLen += 8 + ReadHeader.RecLen;
    }else
    {
        StreamUtils::StreamSkipBack(8, pStream);
    }

    ReadHeader.ReadFromStream(pStream);
    if ( ReadHeader.RecInstance == 1 &&
         ReadHeader.RecType == RT_TimeVariant)
    {
        m_VarFormula.ReadFromStream ( ReadHeader, pStream );
        CurLen += 8 + ReadHeader.RecLen;
    } else
    {
        StreamUtils::StreamSkipBack(8, pStream);
    }

}
