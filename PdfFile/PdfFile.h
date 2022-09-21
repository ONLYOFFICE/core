#ifndef _PDF_FILE_H
#define _PDF_FILE_H

#ifndef PDFFILE_USE_DYNAMIC_LIBRARY
#define PDFFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define PDFFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/graphics/pro/Fonts.h"

class CPdfFile_Private;
class PDFFILE_DECL_EXPORT CPdfFile
{
public:
    CPdfFile(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA = false);
    ~CPdfFile();

    void SetTempDirectory(const std::wstring& wsPath);
    bool LoadFromFile(const std::wstring& wsSrcFile, const std::wstring& wsPassword = L"");
    bool EditPdf(const std::wstring& wsDstFile = L"");
private:
    CPdfFile_Private* m_pInternal;
};

#endif // _PDF_FILE_H
