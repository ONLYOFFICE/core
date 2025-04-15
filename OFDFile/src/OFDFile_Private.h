#ifndef OFDFILE_PRIVATE_H
#define OFDFILE_PRIVATE_H

#include "../../OfficeUtils/src/ZipFolder.h"

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"

#include "Base.h"

class COFDFile_Private
{
	NSFonts::IApplicationFonts* m_pAppFonts;
	NSFonts::IFontManager*      m_pFontManager;
	IFolder*                    m_pTempFolder;

	OFD::CBase m_oBase;

	bool Read(IFolder* pFolder);
public:
	COFDFile_Private(NSFonts::IApplicationFonts* pFonts);
	~COFDFile_Private();

	void Close();

	void SetTempDir(const std::wstring& wsPath);
	std::wstring GetTempDir() const;

	bool LoadFromFile(const std::wstring& wsFilePath);
	bool LoadFromMemory(BYTE* pData, DWORD ulLength);

	unsigned int GetPageCount() const;
	void GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const;

	void DrawPage(IRenderer* pRenderer, int nPageIndex);
	void DrawPage(IRenderer* pRenderer, int nPageIndex, const double& dX, const double& dY, const double& dWidth, const double& dHeight);

	NSFonts::IApplicationFonts* GetFonts();
};

#endif // OFDFILE_PRIVATE_H
