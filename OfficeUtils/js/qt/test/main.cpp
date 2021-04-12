#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../wasm/src/base.h"

#include <string>
#include <vector>

int main()
{
    DWORD nBytesCount;
    BYTE* pData;
    NSFile::CFileBinary oFile;
    if (oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.zip", &pData, nBytesCount))
        oFile.CloseFile();

    Zlib* zlib = Zlib_Load(pData, nBytesCount);

    int nPaths = Zlib_GetNumberPaths(zlib);
    std::wstring* arrPaths = Zlib_GetPaths(zlib);
    for (int i = 0; i < nPaths; i++)
    {
        int nSizeFile = Zlib_GetSizeFileByPath(zlib, arrPaths[i].c_str());
        unsigned char* res = Zlib_GetLastFileByPath(zlib);
        if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L'/' + NSFile::GetFileName(arrPaths[i])))
        {
            if (res)
                oFile.WriteFile(res, nSizeFile);
            oFile.CloseFile();
        }
    }

    Zlib_Destroy(zlib);
    delete[] pData;
    return 0;
}
