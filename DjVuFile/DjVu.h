#pragma once

#include <string>
#include "../DesktopEditor/graphics/IRenderer.h"

#ifndef DJVU_USE_DYNAMIC_LIBRARY
#define DJVU_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define DJVU_DECL_EXPORT Q_DECL_EXPORT
#endif

// –аботаем через класс CDjVuFileImplementation, чтобы когда цепл¤лс¤ данный h-файл, ничего лишнего не инклюдилось
class CDjVuFileImplementation;
class CApplicationFonts;

class DJVU_DECL_EXPORT CDjVuFile
{
private:

	CDjVuFileImplementation* m_pImplementation;

public:

	CDjVuFile();
	~CDjVuFile();

	bool         LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions = L"");
	void         Close();
	std::wstring GetTempDirectory() const;
	void         SetTempDirectory(const std::wstring& wsDirectory);
	int          GetPagesCount() const;
	void         GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) const;
	void         DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
	void         ConvertToRaster(CApplicationFonts* pAppFonts, int nPageIndex, const std::wstring& wsDstPath, int nImageType);
	void         ConvertToPdf(CApplicationFonts* pAppFonts, const std::wstring& wsDstPath);
};
