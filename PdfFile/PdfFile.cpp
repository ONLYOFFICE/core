#include "PdfFile.h"
#include "../PdfWriter/PdfRenderer.h"
#include "../PdfReader/PdfReader.h"
#include "../DesktopEditor/common/File.h"

class CPdfFile_Private
{
public:
    CPdfRenderer* pWriter;
    PdfReader::CPdfReader* pReader;
    std::wstring wsSrcFile;
    std::wstring wsPassword;
};

CPdfFile::CPdfFile(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA)
{
    m_pInternal = new CPdfFile_Private();

    m_pInternal->pWriter = new CPdfRenderer (pAppFonts, isPDFA);
    m_pInternal->pReader = new PdfReader::CPdfReader(pAppFonts);
    m_pInternal->wsPassword = L"";
}

CPdfFile::~CPdfFile()
{
    RELEASEOBJECT(m_pInternal->pWriter);
    RELEASEOBJECT(m_pInternal->pReader);
}

void CPdfFile::SetTempDirectory(const std::wstring& wsPath)
{
    m_pInternal->pWriter->SetTempFolder   (wsPath);
    m_pInternal->pReader->SetTempDirectory(wsPath);
}

bool CPdfFile::LoadFromFile(const std::wstring& wsSrcFile, const std::wstring& wsPassword)
{
    m_pInternal->wsSrcFile  = wsSrcFile;
    m_pInternal->wsPassword = wsPassword;
    return m_pInternal->pReader->LoadFromFile(wsSrcFile, L"", wsPassword, wsPassword) &&
          (m_pInternal->pReader->GetError() == 0);
}

bool CPdfFile::EditPdf(const std::wstring& wsDstFile)
{
    if (!wsDstFile.empty())
        NSFile::CFileBinary::Copy(m_pInternal->wsSrcFile, wsDstFile);
    return m_pInternal->pReader->EditPdf(m_pInternal->pWriter, !wsDstFile.empty() ? wsDstFile : m_pInternal->wsSrcFile, m_pInternal->wsPassword);
}
