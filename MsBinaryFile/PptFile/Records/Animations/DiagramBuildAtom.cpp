#include "DiagramBuildAtom.h"


CRecordDiagramBuildAtom::CRecordDiagramBuildAtom()
{

}

CRecordDiagramBuildAtom::~CRecordDiagramBuildAtom()
{

}

void CRecordDiagramBuildAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_oDiagramBuild        = (DiagramBuildEnum)StreamUtils::ReadDWORD(pStream);
}
