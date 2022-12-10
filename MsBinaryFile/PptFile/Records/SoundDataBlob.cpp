#include "SoundDataBlob.h"

using namespace PPT;

CRecordSoundDataBlob::CRecordSoundDataBlob()
{
    m_pData = NULL;
    m_lSize = 0;
}

CRecordSoundDataBlob::~CRecordSoundDataBlob()
{
    ReleaseData();
}

void CRecordSoundDataBlob::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_lSize = m_oHeader.RecLen;

    if (0 < m_lSize)
    {
        m_pData = new BYTE[m_lSize];
        pStream->read(m_pData, m_lSize);
    }
}

void CRecordSoundDataBlob::ReleaseData()
{
    RELEASEARRAYOBJECTS(m_pData);
}

void CRecordSoundDataBlob::SaveToFile(std::wstring strFile)
{
    if ((NULL == m_pData) || (0 >= m_lSize))
        return;

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(strFile);

    oFile.WriteFile((BYTE*)m_pData, (DWORD)m_lSize);
    oFile.CloseFile();

    ReleaseData();
}
