#include "BlipEntityAtom.h"
#include "../../../DesktopEditor/common/Directory.h"

CRecordBlipEntityAtom::CRecordBlipEntityAtom()
{

}

CRecordBlipEntityAtom::~CRecordBlipEntityAtom()
{
    NSDirectory::DeleteDirectory(m_tmpFolder);
}

void CRecordBlipEntityAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nWinBlipType = StreamUtils::ReadBYTE(pStream);
    StreamUtils::StreamSkip(1, pStream);
    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    m_tmpFolder = getTempFolder();
    m_oBlip.m_strTmpDirectory = m_tmpFolder;
    m_oBlip.ReadFromStream(ReadHeader, pStream);
}


std::wstring CRecordBlipEntityAtom::getTempFolder() const
{
    std::wstring tempRootPath = NSDirectory::GetTempPath();
    if (false == NSDirectory::Exists(tempRootPath))
        return L"";
    std::wstring tempFolder = NSDirectory::CreateDirectoryWithUniqueName(tempRootPath) + FILE_SEPARATOR_STR;
    if (false == NSDirectory::Exists(tempFolder))
        return L"";


    return tempFolder;
}

const std::wstring CRecordBlipEntityAtom::getTmpImgPath() const
{
    return m_tmpFolder + m_oBlip.m_sFileName;
}

const std::wstring CRecordBlipEntityAtom::getImgExtention() const
{
    switch (m_nWinBlipType)
    {
    case 2: return L".emf";
    case 3: return L".wmf";
    case 5: return L".jpeg";
    case 6: return L".png";
    default: return L"";
    }
}
