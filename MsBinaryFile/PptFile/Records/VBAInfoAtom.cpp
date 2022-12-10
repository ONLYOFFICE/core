#include "VBAInfoAtom.h"

using namespace PPT;

CRecordVBAInfoAtom::CRecordVBAInfoAtom()
{
}

CRecordVBAInfoAtom::~CRecordVBAInfoAtom()
{
}

void CRecordVBAInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nObjStgDataRef	= StreamUtils::ReadDWORD(pStream);
    m_nHasMacros		= StreamUtils::ReadDWORD(pStream);
    m_nVersion			= StreamUtils::ReadDWORD(pStream);
}

CRecordVBAInfoContainer::CRecordVBAInfoContainer()
{}

CRecordVBAInfoContainer::~CRecordVBAInfoContainer()
{}

void CRecordVBAInfoContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}

CRecordVbaProjectStg::CRecordVbaProjectStg(std::wstring strTemp) : m_strTmpDirectory(strTemp)
{
}

CRecordVbaProjectStg::~CRecordVbaProjectStg()
{
}

void CRecordVbaProjectStg::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    ULONG decompressedSize = m_oHeader.RecLen, compressedSize = m_oHeader.RecLen;

    BYTE* pData = new BYTE[compressedSize];
    if (!pData) return;

    if (m_oHeader.RecInstance == 0x01)
    {
        decompressedSize = StreamUtils::ReadDWORD(pStream) + 64;
        compressedSize -= 4;
    }
    pStream->read(pData, compressedSize);

    //if (pDecryptor)
    //{
    //	pDecryptor->Decrypt((char*)pData, compressedSize, 0);
    //}

    if (m_oHeader.RecInstance == 0x01)
    {
        BYTE* pDataUncompress = new BYTE[decompressedSize];
        NSZip::Decompress(pData, compressedSize, pDataUncompress, decompressedSize);

        delete []pData;
        pData = pDataUncompress;
    }

    m_sFileName = m_strTmpDirectory + FILE_SEPARATOR_STR +  L"vbaProject.bin";

    NSFile::CFileBinary file;
    if (file.CreateFileW(m_sFileName))
    {
        file.WriteFile(pData, decompressedSize);
        file.CloseFile();
    }
    delete[] pData;
    pData = NULL;
}
