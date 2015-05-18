#pragma once

#include <string>
#include "../DesktopEditor/graphics/IRenderer.h"


// –аботаем через класс CDjVuFileImplementation, чтобы когда цепл€лс€ данный h-файл, ничего лишнего не инклюдилось
class CDjVuFileImplementation;
class CApplicationFonts;

class CDjVuFile
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
};