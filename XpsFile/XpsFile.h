#ifndef _XPS_FILE_H
#define _XPS_FILE_H

#ifndef XPS_USE_DYNAMIC_LIBRARY
#define XPS_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define XPS_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/common/officedrawingfile.h"

class CXpsFile_Private;
class XPS_DECL_EXPORT CXpsFile : public IOfficeDrawingFile
{
public:
    CXpsFile(CApplicationFonts* fonts);
    virtual ~CXpsFile();

    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
                                    const std::wstring& owner_password = L"", const std::wstring& user_password = L"");

    virtual void Close();

    virtual std::wstring GetTempDirectory();
    virtual void SetTempDirectory(const std::wstring& directory);

    virtual int GetPagesCount();
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
    virtual void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType);

	void         ConvertToPdf(const std::wstring& wsDstPath);

private:
    CXpsFile_Private* m_pInternal;
};

#endif // _XPS_FILE_H
