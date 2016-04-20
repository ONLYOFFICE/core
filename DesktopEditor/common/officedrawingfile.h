#ifndef _OFFICE_DRAWING_FILE_H
#define _OFFICE_DRAWING_FILE_H

#include <string>
class IRenderer;
class CApplicationFonts;
class CFontManager;
class IOfficeDrawingFile
{
public:
    virtual ~IOfficeDrawingFile() {}

    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
                                const std::wstring& owner_password = L"", const std::wstring& user_password = L"") = 0;

    virtual void Close() = 0;

    virtual std::wstring GetTempDirectory() = 0;
    virtual void SetTempDirectory(const std::wstring& directory) = 0;

    virtual int GetPagesCount() = 0;
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) = 0;
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak) = 0;
    virtual void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType) = 0;
};

#endif // _OFFICE_DRAWING_FILE_H
