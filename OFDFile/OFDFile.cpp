#include "OFDFile.h"
#include "src/OFDFile_Private.h"

#ifndef DISABLE_PDF_CONVERTATION
#include "../PdfFile/PdfFile.h"
#endif

COFDFile::COFDFile(NSFonts::IApplicationFonts* pFonts)
	: m_pInternal(new COFDFile_Private(pFonts))
{}

COFDFile::~COFDFile()
{
	Close();

	if (nullptr != m_pInternal)
		delete m_pInternal;
}

bool COFDFile::LoadFromFile(const std::wstring& file, const std::wstring& options, const std::wstring& owner_password, const std::wstring& user_password)
{
	if (nullptr == m_pInternal)
		return false;

	Close();

	return m_pInternal->LoadFromFile(file);
}

bool COFDFile::LoadFromMemory(unsigned char* data, unsigned long length, const std::wstring& options, const std::wstring& owner_password, const std::wstring& user_password)
{
	if (nullptr == m_pInternal)
		return false;

	Close();

	return m_pInternal->LoadFromMemory(data, length);
}

void COFDFile::Close()
{
	if (nullptr != m_pInternal)
		m_pInternal->Close();
}

NSFonts::IApplicationFonts* COFDFile::GetFonts()
{
	return (nullptr != m_pInternal) ? m_pInternal->GetFonts() : nullptr;
}

OfficeDrawingFileType COFDFile::GetType()
{
	return odftOFD;
}

std::wstring COFDFile::GetTempDirectory()
{
	if (nullptr != m_pInternal)
		return m_pInternal->GetTempDir();

	return std::wstring();
}

void COFDFile::SetTempDirectory(const std::wstring& directory)
{
	if (nullptr != m_pInternal)
		m_pInternal->SetTempDir(directory);
}

int COFDFile::GetPagesCount()
{
	if (nullptr != m_pInternal)
		return m_pInternal->GetPageCount();

	return 0;
}

void COFDFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	if (nullptr == m_pInternal)
		return;

	m_pInternal->GetPageSize(nPageIndex, *pdWidth, *pdHeight);

	*pdDpiX = 25.4;
	*pdDpiY = 25.4;
}

void COFDFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams)
{
	if (nullptr == pRenderer || nullptr == m_pInternal)
		return;

	m_pInternal->DrawPage(pRenderer, nPageIndex);
}

#ifndef DISABLE_PDF_CONVERTATION
void COFDFile::ConvertToPdf(const std::wstring& wsDstPath)
{
	CPdfFile oPdf(GetFonts());
	oPdf.CreatePdf();
	bool bBreak = false;

	int nPagesCount = GetPagesCount();
	for (int nPageIndex = 0; nPageIndex < nPagesCount; nPageIndex++)
	{
		oPdf.NewPage();
		oPdf.BeginCommand(c_nPageType);

		double dPageDpiX = 96., dPageDpiY = 96.;
		double dWidth = 0., dHeight = 0.;

		GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

		dWidth  *= 25.4 / dPageDpiX;
		dHeight *= 25.4 / dPageDpiY;

		oPdf.put_Width(dWidth);
		oPdf.put_Height(dHeight);

		DrawPageOnRenderer(&oPdf, nPageIndex, &bBreak);

		oPdf.EndCommand(c_nPageType);

#ifdef _DEBUG
		printf("page %d / %d\n", nPageIndex + 1, nPagesCount);
#endif
	}

	oPdf.SaveToFile(wsDstPath);
}

std::wstring COFDFile::GetInfo()
{
	return std::wstring();
}

unsigned char* COFDFile::GetStructure()
{
	return nullptr;
}

unsigned char* COFDFile::GetLinks(int nPageIndex)
{
	return nullptr;
}
#endif
