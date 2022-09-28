#ifndef _PDF_FILE_H
#define _PDF_FILE_H

#ifndef PDFFILE_USE_DYNAMIC_LIBRARY
#define PDFFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define PDFFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../PdfWriter/PdfRenderer.h"
#include "../PdfReader/PdfReader.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/xmlsec/src/include/Certificate.h"

class CPdfFile_Private;
class PDFFILE_DECL_EXPORT CPdfFile
{
public:
    CPdfFile(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA = false);
    ~CPdfFile();

    PdfReader::CPdfReader* GetReader();
    CPdfRenderer*          GetWriter();

    void SetTempDirectory(const std::wstring& wsPath);
    bool LoadFromFile(const std::wstring& wsSrcFile, const std::wstring& wsPassword = L"");
    bool LoadFromMemory (BYTE* pData, DWORD nLength, const std::wstring& wsPassword = L"");

    int  GetPagesCount();
    void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
    void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
    int  GetError();
    void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType, const int nRasterW = -1, const int nRasterH = -1,
                         bool bIsFlip = false, NSFonts::IFontManager* pFonts = NULL, int nBackgroundColor = 0xFFFFFF, bool bIsDarkMode = false);

    bool EditPdf(const std::wstring& wsDstFile = L"");
    bool EditClose();
    bool EditPage  (int nPageIndex);
    bool DeletePage(int nPageIndex);
    bool AddPage   (int nPageIndex);
    void Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate);
    void PageRotate(int nRotate);

    HRESULT OnlineWordToPdf(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams = NULL, bool bBinary = true);
    void SetPassword  (const std::wstring& wsPassword);
    void SetDocumentID(const std::wstring& wsDocumentID);
    int  SaveToFile   (const std::wstring& wsPath);

    void TEST(int i);
private:
    CPdfFile_Private* m_pInternal;
};

#endif // _PDF_FILE_H
