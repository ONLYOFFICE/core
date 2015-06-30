#include "DjVu.h"
#include "DjVuFileImplementation.h"

class CApplicationFonts;

CDjVuFile::CDjVuFile()
{
	m_pImplementation = new CDjVuFileImplementation();
}
CDjVuFile::~CDjVuFile()
{
	if (m_pImplementation)
		delete m_pImplementation;
}
bool               CDjVuFile::LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXMLOptions)
{
	if (m_pImplementation)
		return m_pImplementation->LoadFromFile(wsSrcFileName, wsXMLOptions);

	return false;
}
void               CDjVuFile::Close()
{
	if (m_pImplementation)
		m_pImplementation->Close();
}
std::wstring       CDjVuFile::GetTempDirectory() const
{
	if (m_pImplementation)
		return m_pImplementation->GetTempDirectory();

	return L"";
}
void               CDjVuFile::SetTempDirectory(const std::wstring& wsDirectory)
{
	if (m_pImplementation)
		m_pImplementation->SetTempDirectory(wsDirectory);
}
int                CDjVuFile::GetPagesCount() const
{
	if (m_pImplementation)
		return m_pImplementation->GetPagesCount();
	return 0;
}
void               CDjVuFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) const
{
	if (m_pImplementation)
		m_pImplementation->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
}
void               CDjVuFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
	if (m_pImplementation)
		m_pImplementation->DrawPageOnRenderer(pRenderer, nPageIndex, pBreak);
}
void               CDjVuFile::ConvertToRaster(CApplicationFonts* pAppFonts, int nPageIndex, const std::wstring& wsDstPath, int nImageType)
{
	if (m_pImplementation)
		m_pImplementation->ConvertToRaster(pAppFonts, nPageIndex, wsDstPath, nImageType);
}
void               CDjVuFile::ConvertToPdf(CApplicationFonts* pAppFonts, const std::wstring& wsDstPath)
{
	if (m_pImplementation)
		m_pImplementation->ConvertToPdf(pAppFonts, wsDstPath);
}
