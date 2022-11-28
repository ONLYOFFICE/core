#include"DiagramBuildContainer.h"


CRecordDiagramBuildContainer::CRecordDiagramBuildContainer()
{

}

CRecordDiagramBuildContainer::~CRecordDiagramBuildContainer()
{

}

void CRecordDiagramBuildContainer::ReadFromStream(SRecordHeader &header, POLE::Stream *pStream)
{
    CRecordBuildListSubContainer::ReadFromStream(header, pStream);

    SRecordHeader diagramBuildAtomHeader;

    if (diagramBuildAtomHeader.ReadFromStream(pStream))
        m_oDiagramBuildAtom.ReadFromStream (diagramBuildAtomHeader, pStream);
}
