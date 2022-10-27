#ifndef _PDF_FILE_H
#define _PDF_FILE_H

#ifndef PDFFILE_USE_DYNAMIC_LIBRARY
#define PDFFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define PDFFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/graphics/pro/officedrawingfile.h"

#include "../PdfWriter/PdfRenderer.h"
#include "../PdfReader/PdfReader.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/xmlsec/src/include/Certificate.h"

class CPdfFile_Private;
/*
    int  GetError();
    void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType, const int nRasterW = -1, const int nRasterH = -1,
                         bool bIsFlip = false, NSFonts::IFontManager* pFonts = NULL, int nBackgroundColor = 0xFFFFFF, bool bIsDarkMode = false);
    int  SavePdfToFile(const std::wstring& wsPath);

    void SetPassword  (const std::wstring& wsPassword);
    void SetDocumentID(const std::wstring& wsDocumentID);
    HRESULT DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH);
*/

class PDFFILE_DECL_EXPORT CPdfFile : public IOfficeDrawingFile, public IRenderer
{
public:
    CPdfFile(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA = false);
    virtual ~CPdfFile();

    void SetTemp(const std::wstring& wsPath);
    NSFonts::IFontManager* GetFontManager();

    bool EditPdf(const std::wstring& wsDstFile = L"");
    bool EditClose();
    bool EditPage  (int nPageIndex);
    bool DeletePage(int nPageIndex);
    bool AddPage   (int nPageIndex);
    void Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate);
    void PageRotate(int nRotate);

    // --- READER ---

    virtual bool LoadFromFile  (const std::wstring& file, const std::wstring& options = L"", const std::wstring& owner_password = L"", const std::wstring& user_password = L"");
    virtual bool LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options = L"", const std::wstring& owner_password = L"", const std::wstring& user_password = L"");

    virtual void Close();

    virtual NSFonts::IApplicationFonts* GetFonts();

    virtual OfficeDrawingFileType GetType();

    virtual std::wstring GetTempDirectory();
    virtual void SetTempDirectory(const std::wstring& directory);

    virtual int GetPagesCount();
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
    virtual std::wstring GetInfo();

#ifdef BUILDING_WASM_MODULE
    virtual BYTE* GetStructure();
    virtual BYTE* GetLinks(int nPageIndex);
#endif

    // --- WRITER ---

    int SaveToFile(const std::wstring& wsPath);
    HRESULT OnlineWordToPdf          (const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams = NULL);
    HRESULT OnlineWordToPdfFromBinary(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams = NULL);

    //----------------------------------------------------------------------------------------
    // Тип рендерера
    //----------------------------------------------------------------------------------------
    virtual HRESULT get_Type(LONG* lType);
    //----------------------------------------------------------------------------------------
    // Функции для работы со страницей
    //----------------------------------------------------------------------------------------
    virtual HRESULT NewPage();
    virtual HRESULT get_Height(double* dHeight);
    virtual HRESULT put_Height(const double& dHeight);
    virtual HRESULT get_Width(double* dWidth);
    virtual HRESULT put_Width(const double& dWidth);
    virtual HRESULT get_DpiX(double* dDpiX);
    virtual HRESULT get_DpiY(double* dDpiY);
    //----------------------------------------------------------------------------------------
    // Функции для работы с Pen
    //----------------------------------------------------------------------------------------
    virtual HRESULT get_PenColor(LONG* lColor);
    virtual HRESULT put_PenColor(const LONG& lColor);
    virtual HRESULT get_PenAlpha(LONG* lAlpha);
    virtual HRESULT put_PenAlpha(const LONG& lAlpha);
    virtual HRESULT get_PenSize(double* dSize);
    virtual HRESULT put_PenSize(const double& dSize);
    virtual HRESULT get_PenDashStyle(BYTE* nDashStyle);
    virtual HRESULT put_PenDashStyle(const BYTE& nDashStyle);
    virtual HRESULT get_PenLineStartCap(BYTE* nCapStyle);
    virtual HRESULT put_PenLineStartCap(const BYTE& nCapStyle);
    virtual HRESULT get_PenLineEndCap(BYTE* nCapStyle);
    virtual HRESULT put_PenLineEndCap(const BYTE& nCapStyle);
    virtual HRESULT get_PenLineJoin(BYTE* nJoinStyle);
    virtual HRESULT put_PenLineJoin(const BYTE& nJoinStyle);
    virtual HRESULT get_PenDashOffset(double* dOffset);
    virtual HRESULT put_PenDashOffset(const double& dOffset);
    virtual HRESULT get_PenAlign(LONG* lAlign);
    virtual HRESULT put_PenAlign(const LONG& lAlign);
    virtual HRESULT get_PenMiterLimit(double* dMiter);
    virtual HRESULT put_PenMiterLimit(const double& dMiter);
    virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);
    //----------------------------------------------------------------------------------------
    // Функции для работы с Brush
    //----------------------------------------------------------------------------------------
    virtual HRESULT get_BrushType(LONG* lType);
    virtual HRESULT put_BrushType(const LONG& lType);
    virtual HRESULT get_BrushColor1(LONG* lColor);
    virtual HRESULT put_BrushColor1(const LONG& lColor);
    virtual HRESULT get_BrushAlpha1(LONG* lAlpha);
    virtual HRESULT put_BrushAlpha1(const LONG& lAlpha);
    virtual HRESULT get_BrushColor2(LONG* lColor);
    virtual HRESULT put_BrushColor2(const LONG& lColor);
    virtual HRESULT get_BrushAlpha2(LONG* lAlpha);
    virtual HRESULT put_BrushAlpha2(const LONG& lAlpha);
    virtual HRESULT get_BrushTexturePath(std::wstring* wsPath);
    virtual HRESULT put_BrushTexturePath(const std::wstring& wsPath);
    virtual HRESULT get_BrushTextureMode(LONG* lMode);
    virtual HRESULT put_BrushTextureMode(const LONG& lMode);
    virtual HRESULT get_BrushTextureAlpha(LONG* lAlpha);
    virtual HRESULT put_BrushTextureAlpha(const LONG& lAlpha);
    virtual HRESULT get_BrushLinearAngle(double* dAngle);
    virtual HRESULT put_BrushLinearAngle(const double& dAngle);
    virtual HRESULT BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);
    virtual HRESULT BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);
    virtual HRESULT put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount);
    //----------------------------------------------------------------------------------------
    // Функции для работы со шрифтами
    //----------------------------------------------------------------------------------------
    virtual HRESULT get_FontName(std::wstring* wsName);
    virtual HRESULT put_FontName(const std::wstring& wsName);
    virtual HRESULT get_FontPath(std::wstring* wsPath);
    virtual HRESULT put_FontPath(const std::wstring& wsPath);
    virtual HRESULT get_FontSize(double* dSize);
    virtual HRESULT put_FontSize(const double& dSize);
    virtual HRESULT get_FontStyle(LONG* lStyle);
    virtual HRESULT put_FontStyle(const LONG& lStyle);
    virtual HRESULT get_FontStringGID(INT* bGid);
    virtual HRESULT put_FontStringGID(const INT& bGid);
    virtual HRESULT get_FontCharSpace(double* dSpace);
    virtual HRESULT put_FontCharSpace(const double& dSpace);
    virtual HRESULT get_FontFaceIndex(int* lFaceIndex);
    virtual HRESULT put_FontFaceIndex(const int& lFaceIndex);
    //----------------------------------------------------------------------------------------
    // Функции для вывода текста
    //----------------------------------------------------------------------------------------
    virtual HRESULT CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT CommandDrawTextCHAR2 (unsigned int* unUnicode, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH);
    //----------------------------------------------------------------------------------------
    // Маркеры команд
    //----------------------------------------------------------------------------------------
    virtual HRESULT BeginCommand(const DWORD& lType);
    virtual HRESULT EndCommand(const DWORD& lType);
    //----------------------------------------------------------------------------------------
    // Функции для работы с патом
    //----------------------------------------------------------------------------------------
    virtual HRESULT PathCommandMoveTo(const double& dX, const double& dY);
    virtual HRESULT PathCommandLineTo(const double& dX, const double& dY);
    virtual HRESULT PathCommandLinesTo(double* pPoints, const int& nCount);
    virtual HRESULT PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe);
    virtual HRESULT PathCommandCurvesTo(double* pPoints, const int& nCount);
    virtual HRESULT PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle);
    virtual HRESULT PathCommandClose();
    virtual HRESULT PathCommandEnd();
    virtual HRESULT DrawPath(const LONG& lType);
    virtual HRESULT PathCommandStart();
    virtual HRESULT PathCommandGetCurrentPoint(double* dX, double* dY);
    virtual HRESULT PathCommandTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT PathCommandText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
    //----------------------------------------------------------------------------------------
    // Функции для вывода изображений
    //----------------------------------------------------------------------------------------
    virtual HRESULT DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH);
    virtual HRESULT DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255);
    //----------------------------------------------------------------------------------------
    // Функции для выставления преобразования
    //----------------------------------------------------------------------------------------
    virtual HRESULT SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY);
    virtual HRESULT GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY);
    virtual HRESULT ResetTransform();
    //----------------------------------------------------------------------------------------
    // Тип клипа
    //----------------------------------------------------------------------------------------
    virtual HRESULT get_ClipMode(LONG* lMode);
    virtual HRESULT put_ClipMode(const LONG& lMode);
    //----------------------------------------------------------------------------------------
    // Дополнительные функции
    //----------------------------------------------------------------------------------------
    virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
    virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
    virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);
    virtual HRESULT AddHyperlink(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsUrl, const std::wstring& wsTooltip);
    virtual HRESULT AddLink(const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const int& nPage);
    virtual HRESULT AddFormField(const CFormFieldInfo& oInfo);

private:
    CPdfFile_Private* m_pInternal;
};

#endif // _PDF_FILE_H
