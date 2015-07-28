#ifndef _HTMLFILE_HTMLFILE_H_
#define _HTMLFILE_HTMLFILE_H_

#include <string>

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

    int Convert(const std::wstring& sXml, const std::wstring& sPathInternal = L"");
};

#endif // _HTMLFILE_HTMLFILE_H_
