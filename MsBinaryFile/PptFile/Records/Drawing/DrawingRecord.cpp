#include "DrawingRecord.h"

using namespace PPT;

CRecordDrawingRecord::CRecordDrawingRecord()
{

}

CRecordDrawingRecord::~CRecordDrawingRecord()
{

}

void CRecordDrawingRecord::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_nCountShapes = (UINT)StreamUtils::ReadDWORD(pStream);
    m_nPidCur = (UINT)StreamUtils::ReadDWORD(pStream);
}
