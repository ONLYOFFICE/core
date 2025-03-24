#ifndef OFDFILE_H
#define OFDFILE_H

#ifndef OFD_USE_DYNAMIC_LIBRARY
#define OFD_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define OFD_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"

class COFDFile_Private;
class OFD_DECL_EXPORT COFDFile : public IOfficeDrawingFile
{
	COFDFile_Private* m_pInternal;
public:
	COFDFile(NSFonts::IApplicationFonts* pFonts);
	virtual ~COFDFile();

	// Open
	virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
	                          const std::wstring& owner_password = L"", const std::wstring& user_password = L"") override;
	virtual bool LoadFromMemory(unsigned char* data, unsigned long length, const std::wstring& options = L"",
	                            const std::wstring& owner_password = L"", const std::wstring& user_password = L"") override;

	// Close
	virtual void Close() override;

	// Get IApplicationFonts for wrappers
	virtual NSFonts::IApplicationFonts* GetFonts() override;

	// Type
	virtual OfficeDrawingFileType GetType() override;

	// Temp directory
	virtual std::wstring GetTempDirectory() override;
	virtual void SetTempDirectory(const std::wstring& directory) override;

	// Pages info/draw
	virtual int GetPagesCount() override;
	virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) override;
	virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams = NULL) override;

	#ifndef DISABLE_PDF_CONVERTATION
	void ConvertToPdf(const std::wstring& wsDstPath);
	#endif

	// Common methods for viewer
	virtual std::wstring GetInfo() override;
	virtual unsigned char* GetStructure() override;
	virtual unsigned char* GetLinks(int nPageIndex) override;
};

#endif // OFDFILE_H
