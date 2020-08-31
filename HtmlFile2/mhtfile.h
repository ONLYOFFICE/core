#ifndef _MHTFILE_MHTFILE_H
#define _MHTFILE_MHTFILE_H

#include <string>
#include <vector>
#include "../DesktopEditor/common/Types.h"

#ifndef MHTFILE_USE_DYNAMIC_LIBRARY
#define MHTFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define MHTFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

struct CHtmlParams;
class CHtmlFile2_Private;
class MHTFILE_DECL_EXPORT CMhtFile
{
private:
    CHtmlFile2_Private* m_internal;
public:
    CMhtFile();
    ~CMhtFile();

    bool IsMhtFile(const std::wstring& sFile);
    void SetTmpDirectory(const std::wstring& sFolder);
    HRESULT Open(const std::wstring& sPath, const std::wstring& sDirectory, CHtmlParams* oParams = NULL);
};

#endif // _MHTFILE_MHTFILE_H
