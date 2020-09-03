#include "mhtfile.h"
#include "htmlfile_private.h"

CMhtFile::CMhtFile()
{
    m_internal = new CHtmlFile2_Private();
}

CMhtFile::~CMhtFile()
{
    RELEASEOBJECT(m_internal);
}

bool CMhtFile::IsMhtFile(const std::wstring& sFile)
{
    // Конвертируем в xhtml
    if(!m_internal->mhtXhtml(sFile))
        return false;
    // Открывает файл на проверку
    if (!m_internal->m_oLightReader.FromFile(m_internal->m_sTmp + L"/res.xhtml"))
        return false;
    // Читаем html
    if(!m_internal->isHtml())
        return false;
    return true;
}

void CMhtFile::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sTmp = sFolder;
}

HRESULT CMhtFile::Open(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    if(!m_internal->m_oLightReader.IsValid())
        if(!IsMhtFile(sSrc))
            return S_FALSE;

    m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);
    m_internal->readStyle();

    // Переходим в начало
    if(!m_internal->m_oLightReader.MoveToStart())
        return S_FALSE;

    m_internal->readSrc(NSFile::GetFileName(sSrc));
    m_internal->write();
    NSFile::CFileBinary::Remove(m_internal->m_sTmp + L"/res.xhtml");
    return S_OK;
}
