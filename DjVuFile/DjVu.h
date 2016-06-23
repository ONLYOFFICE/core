#pragma once

#ifndef DJVU_USE_DYNAMIC_LIBRARY
#define DJVU_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define DJVU_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/common/officedrawingfile.h"

// работаем через класс CDjVuFileImplementation, чтобы когда цеплялся данный h-файл, ничего лишнего не инклюдилось
class CDjVuFileImplementation;

class DJVU_DECL_EXPORT CDjVuFile : public IOfficeDrawingFile
{
private:
	CDjVuFileImplementation* m_pImplementation;

public:

    CDjVuFile(CApplicationFonts* fonts);
    virtual ~CDjVuFile();

    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
                                    const std::wstring& owner_password = L"", const std::wstring& user_password = L"");

    virtual void Close();

    virtual std::wstring GetTempDirectory();
    virtual void SetTempDirectory(const std::wstring& directory);

    virtual int GetPagesCount();
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
    virtual void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType);

    void ConvertToPdf(const std::wstring& path);
};
