#ifndef _XPS_FILE_H
#define _XPS_FILE_H

#include <string>

namespace XPS
{
	class Folder;
}

class IRenderer;
class CApplicationFonts;
class CFontManager;

class CXpsFile
{
public:
	CXpsFile(CApplicationFonts* pAppFonts);
	~CXpsFile();

	bool         LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions = L"");
	void         Close();
	std::wstring GetTempDirectory() const;
	void         SetTempDirectory(const std::wstring& wsPath);
	int          GetPagesCount();
	void         GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
	void         DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
	void         ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType);

private:

	CApplicationFonts* m_pAppFonts;
	CFontManager*      m_pFontManager;
	std::wstring       m_wsTempDirectory;
	XPS::Folder*       m_pFolder;
};

#endif // _XPS_FILE_H