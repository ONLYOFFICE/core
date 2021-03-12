#include "xmemfile.h"

CxMemFile::CxMemFile(uint8_t* pBuffer, uint32_t size)
{
    m_pBuffer = pBuffer;
    m_Position = 0;
    m_Size = m_Edge = size;
    m_bFreeOnClose = (bool)(pBuffer==0);
    m_bEOF = false;
}
CxMemFile::~CxMemFile()
{
    Close();
}
bool CxMemFile::Close()
{
    if ( (m_pBuffer) && (m_bFreeOnClose) ){
        free(m_pBuffer);
        m_pBuffer = NULL;
        m_Size = 0;
    }
    return true;
}
bool CxMemFile::Seek(int32_t offset, int32_t origin)
{
    m_bEOF = false;
    if (m_pBuffer==NULL) return false;
    int32_t lNewPos = m_Position;

    if (origin == SEEK_SET)		 lNewPos = offset;
    else if (origin == SEEK_CUR) lNewPos += offset;
    else if (origin == SEEK_END) lNewPos = m_Size + offset;
    else return false;

    if (lNewPos < 0) lNewPos = 0;

    m_Position = lNewPos;
    return true;
}
int32_t CxMemFile::Tell()
{
    if (m_pBuffer==NULL) return -1;
    return m_Position;
}
