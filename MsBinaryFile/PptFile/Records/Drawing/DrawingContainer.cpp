#include "DrawingContainer.h"


CRecordDrawingContainer::CRecordDrawingContainer()
{

}

CRecordDrawingContainer::~CRecordDrawingContainer()
{

}

void CRecordDrawingContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
