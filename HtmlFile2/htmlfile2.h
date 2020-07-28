#ifndef _HTMLFILE2_HTMLFILE2_H
#define _HTMLFILE2_HTMLFILE2_H

#include <string>
#include "../DesktopEditor/common/Types.h"

#ifndef HTMLFILE2_USE_DYNAMIC_LIBRARY
#define HTMLFILE2_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define HTMLFILE2_DECL_EXPORT Q_DECL_EXPORT
#endif

struct CHtmlParams
{
    CHtmlParams(){}
};

class CHtmlFile2_Private;
class HTMLFILE2_DECL_EXPORT CHtmlFile2
{
private:
    CHtmlFile2_Private* m_internal;
public:
    CHtmlFile2();
    ~CHtmlFile2();

    bool IsHtmlFile(const std::wstring& sFile);
    void SetTmpDirectory(const std::wstring& sFolder);
    HRESULT Open(const std::wstring& sPath, const std::wstring& sDirectory, CHtmlParams* oParams = NULL);
};

#endif // _HTMLFILE2_HTMLFILE2_H
