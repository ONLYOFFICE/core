#ifndef _ZIPLIB_H_
#define _ZIPLIB_H_

#include <string>
#include <vector>

#include "../../Common/kernel_config.h"
#include "../../DesktopEditor/common/Types.h"

class KERNEL_DECL CZLib
{
private:
    BYTE* m_zipFile;
    DWORD m_sizeZip;

    struct CFile
    {
        std::string m_sPath;
        BYTE* m_pData;
        DWORD m_nLength;
        CFile(const std::string& sPath, BYTE* pData, DWORD nLength) : m_sPath(sPath), m_pData(pData), m_nLength(nLength) {}
    };
    std::vector<CFile> m_arrFiles;
public:
    CZLib()
    {
        create();
    }
    CZLib(BYTE* buffer, DWORD size)
    {
        open(buffer, size);
    }
    ~CZLib()
    {
        close();
    }

    void create();
    void open(BYTE* buffer, DWORD size);
    void close();

    std::vector<std::string> getPaths();
    std::pair<DWORD, BYTE*> save();
    std::pair<DWORD, BYTE*> getFile(const std::string& sPath);
    void addFile   (const std::string& sPath, BYTE* data, DWORD length);
    bool removeFile(const std::string& sPath);
};

#endif // _ZIPLIB_H_
