#ifndef _HTMLFILE_HTMLFILE_H_
#define _HTMLFILE_HTMLFILE_H_

#include <string>
#include <vector>

#ifndef HTMLFILE_USE_DYNAMIC_LIBRARY
#define HTMLFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define HTMLFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

class HTMLFILE_DECL_EXPORT CHtmlFile
{
private:

public:

    CHtmlFile();
    ~CHtmlFile();

    /// \param sPathInternal - path (subprocesspath = path + HtmlFileInternal.exe) ("" -> GetProcessDirectory()/HtmlFileInternal/HtmlFileInternal.exe)
    /// \return 1 error, 0 - success

    int Convert(const std::vector<std::wstring>& arFiles, const std::wstring& sDstfolder, const std::wstring& sPathInternal = L"");
    int ConvertEpub(const std::wstring& sFolder, std::wstring& sMetaInfo, const std::wstring& sDstfolder, const std::wstring& sPathInternal = L"");
};

#endif // _HTMLFILE_HTMLFILE_H_
