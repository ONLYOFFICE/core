#include "MetafileBlob.h"

using namespace PPT;

CRecordMetafileBlob::CRecordMetafileBlob() :
    m_nMM(-1), m_nExtX(-1), m_nExtY(-1), m_pData(nullptr)
{

}
CRecordMetafileBlob::~CRecordMetafileBlob()
{
    RELEASEOBJECT (m_pData)
}

void CRecordMetafileBlob::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nMM = StreamUtils::ReadSHORT(pStream);
    m_nExtX = StreamUtils::ReadSHORT(pStream);
    m_nExtY = StreamUtils::ReadSHORT(pStream);

    const int dataLen = m_oHeader.RecLen - 6;
    if (dataLen > 6)
    {
        m_pData = new BYTE[dataLen];
        pStream->read(m_pData, dataLen);
    }
}
