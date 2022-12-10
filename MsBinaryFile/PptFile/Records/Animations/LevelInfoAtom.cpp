#include "LevelInfoAtom.h"

using namespace PPT;

CRecordLevelInfoAtom::CRecordLevelInfoAtom()
{

}

CRecordLevelInfoAtom::~CRecordLevelInfoAtom()
{

}

void CRecordLevelInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader	=	oHeader;

    m_nLevel	=	StreamUtils::ReadDWORD ( pStream );
}
