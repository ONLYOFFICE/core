#include "BuildListSubContainer.h"

using namespace PPT;

CRecordBuildListSubContainer::CRecordBuildListSubContainer()
{

}

CRecordBuildListSubContainer::~CRecordBuildListSubContainer()
{

}

void CRecordBuildListSubContainer::ReadFromStream(SRecordHeader &header, POLE::Stream *pStream)
{
    m_oHeader = header;

    SRecordHeader buildAtomHeader;
    if (buildAtomHeader.ReadFromStream(pStream))
        buildAtom.ReadFromStream ( buildAtomHeader, pStream );
}
