#ifndef _FB2FILE_FB2FILE_H
#define _FB2FILE_FB2FILE_H

#include <string>

#ifndef FB2FILE_USE_DYNAMIC_LIBRARY
#define FB2FILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define FB2FILE_DECL_EXPORT Q_DECL_EXPORT
#endif

class CFb2File_Private;
class FB2FILE_DECL_EXPORT CFb2File
{
private:
    CFb2File_Private* m_internal;
public:
    CFb2File();
    ~CFb2File();

    bool IsFb2File(const std::wstring& sFile);
    void SetTmpDirectory(const std::wstring& sFolder);
    int Convert(const std::wstring& sPath, const std::wstring& sDirectory);
    std::wstring GetText();
};

#endif // _FB2FILE_FB2FILE_H
