#include "htmlfile2.h"
#include "htmlfile_private.h"

CHtmlFile2::CHtmlFile2()
{
    m_internal = new CHtmlFile2_Private();
}

CHtmlFile2::~CHtmlFile2()
{
    RELEASEOBJECT(m_internal);
}

bool CHtmlFile2::IsHtmlFile(const std::wstring& sFile)
{
    // Конвертируем в xhtml
    if(!m_internal->htmlXhtml(sFile))
        return false;
    // Открывает файл на проверку
    if (!m_internal->m_oLightReader.FromFile(m_internal->m_sTmp + L"/res.xhtml"))
        return false;
    // Читаем html
    if(!m_internal->isHtml())
        return false;
    return true;
}

void CHtmlFile2::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sTmp = sFolder;
}

HRESULT CHtmlFile2::Open(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    if(!m_internal->m_oLightReader.IsValid())
        if(!IsHtmlFile(sSrc))
            return S_FALSE;

    m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);
    m_internal->readStyle();

    // Переходим в начало
    if(!m_internal->m_oLightReader.MoveToStart())
        return S_FALSE;

    if(!m_internal->readSrc(NSFile::GetFileName(sSrc)))
        return S_FALSE;
    m_internal->write();
    NSFile::CFileBinary::Remove(m_internal->m_sTmp + L"/res.xhtml");
    return S_OK;
}

HRESULT CHtmlFile2::OpenBatch(const std::vector<std::wstring>& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);

    for(const std::wstring& sS : sSrc)
        m_internal->m_sSrcs.insert(std::make_pair(NSFile::GetFileName(sS), std::vector<std::wstring>()));

    for(const std::wstring& sS : sSrc)
    {
        #ifdef _DEBUG
        std::wcout << NSFile::GetFileName(sS) << std::endl;
        #endif

        m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sS);
        if(!IsHtmlFile(sS))
            return S_FALSE;
        m_internal->readStyle();

        // Переходим в начало
        if(!m_internal->m_oLightReader.MoveToStart())
            return S_FALSE;

        if(!m_internal->readSrc(NSFile::GetFileName(sS)))
            return S_FALSE;

        NSFile::CFileBinary::Remove(m_internal->m_sTmp + L"/res.xhtml");
        m_internal->m_oLightReader.Clear();
        m_internal->m_oStylesCalculator.Clear();
        m_internal->m_sBase = L"";
    }

    m_internal->write();
    return S_OK;
}
