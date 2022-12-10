#include "AnimationInfoContainer.h"

using namespace PPT;

CRecordAnimationInfoContainer::CRecordAnimationInfoContainer()
{

}

CRecordAnimationInfoContainer::~CRecordAnimationInfoContainer()
{

}

void CRecordAnimationInfoContainer::ReadFromStream(SRecordHeader &thisHeader, POLE::Stream *pStream)
{
    m_oHeader = thisHeader;

    SRecordHeader oHeader;

    if (oHeader.ReadFromStream(pStream))
        m_AnimationAtom.ReadFromStream ( oHeader, pStream );

    if (m_oHeader.RecLen != 36 && oHeader.ReadFromStream(pStream))
        m_AnimationSound.ReadFromStream ( oHeader, pStream );
}
