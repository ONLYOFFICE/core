#include "ExOleObjAtom.h"

using namespace PPT;

void CRecordExOleObjAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nDrawAspect	= StreamUtils::ReadDWORD(pStream);
    m_nType			= StreamUtils::ReadDWORD(pStream);
    m_nExObjID		= StreamUtils::ReadDWORD(pStream);
    m_nSubType		= StreamUtils::ReadDWORD(pStream);
    m_nPersistID	= StreamUtils::ReadDWORD(pStream);

    StreamUtils::StreamSkip ( 4, pStream ); //unused
}

CRecordExOleObjStg::CRecordExOleObjStg(std::wstring strTemp) : m_strTmpDirectory(strTemp)
{
}

CRecordExOleObjStg::~CRecordExOleObjStg()
{

}

void CRecordExOleObjStg::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    ULONG decompressedSize = m_oHeader.RecLen, compressedSize = m_oHeader.RecLen;

    if (m_oHeader.RecInstance == 0x01)
    {
        compressedSize = m_oHeader.RecLen - 4;
        decompressedSize = StreamUtils::ReadDWORD(pStream);
    }

    BYTE* pData = (compressedSize > 0 && compressedSize < 0xffffff ) ? new BYTE[compressedSize] : NULL;

    if (!pData) return;

    compressedSize = pStream->read(pData, compressedSize);

    if (m_oHeader.RecInstance == 0x01)
    {
        BYTE* pDataUncompress = (compressedSize > 0 && compressedSize < 0xffffff) ?  new BYTE[decompressedSize + 64] : NULL;
        if ((pDataUncompress) && (NSZip::Decompress(pData, compressedSize, pDataUncompress, decompressedSize)))
        {
            delete []pData;
            pData = pDataUncompress;
        }
        else
        {
            delete []pData;
            pData = NULL;
        }
    }
    //if (pDecryptor)
    //{
    //	pDecryptor->Decrypt((char*)pData, oHeader.RecLen - lOffset, 0);
    //}
    if (pData)
    {
        m_sFileName = m_strTmpDirectory + FILE_SEPARATOR_STR +  L"oleObject_xxx.bin";

        NSFile::CFileBinary file;
        if (file.CreateFileW(m_sFileName))
        {
            file.WriteFile(pData, decompressedSize);
            file.CloseFile();
        }
        delete[] pData;
        pData = NULL;
    }
}
