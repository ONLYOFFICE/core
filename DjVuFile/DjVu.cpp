#include "DjVu.h"
#include "DjVuFileImplementation.h"
#include "../DesktopEditor/fontengine/ApplicationFonts.h"

CDjVuFile::CDjVuFile(CApplicationFonts* pFonts)
{
    m_pImplementation = new CDjVuFileImplementation(pFonts);
}
CDjVuFile::~CDjVuFile()
{
	if (m_pImplementation)
		delete m_pImplementation;
}
bool CDjVuFile::LoadFromFile(const std::wstring& file, const std::wstring& options,
                                const std::wstring& owner_password, const std::wstring& user_password)
{
    if (m_pImplementation)
        return m_pImplementation->LoadFromFile(file, options);

    return false;
}

void CDjVuFile::Close()
{
	if (m_pImplementation)
		m_pImplementation->Close();
}

std::wstring CDjVuFile::GetTempDirectory()
{
    return m_pImplementation ? m_pImplementation->GetTempDirectory() : L"";
}
void CDjVuFile::SetTempDirectory(const std::wstring& wsDirectory)
{
	if (m_pImplementation)
		m_pImplementation->SetTempDirectory(wsDirectory);
}

int CDjVuFile::GetPagesCount()
{
	if (m_pImplementation)
		return m_pImplementation->GetPagesCount();
	return 0;
}
void CDjVuFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	if (m_pImplementation)
		m_pImplementation->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
}
void CDjVuFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
	if (m_pImplementation)
		m_pImplementation->DrawPageOnRenderer(pRenderer, nPageIndex, pBreak);
}
void CDjVuFile::ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType)
{
	if (m_pImplementation)
        m_pImplementation->ConvertToRaster(nPageIndex, wsDstPath, nImageType);
}
void CDjVuFile::ConvertToPdf(const std::wstring& wsDstPath)
{
	if (m_pImplementation)
        m_pImplementation->ConvertToPdf(wsDstPath);
}
