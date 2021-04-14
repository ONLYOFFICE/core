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

    BYTE* sPaths = Zlib_GetPaths(zlib);
    unsigned int nLength = sPaths[0] | sPaths[1] << 8 | sPaths[2] << 16 | sPaths[3] << 24;
    unsigned int i = 4;
    nLength -= 4;

    while (i < nLength)
    {
        unsigned int nPathLength = (sPaths + i)[0] | (sPaths + i)[1] << 8 | (sPaths + i)[2] << 16 | (sPaths + i)[3] << 24;
        i += 4;
        std::string sPath((const char*)(sPaths + i), nPathLength);
        i += nPathLength;
        BYTE* sFile = Zlib_GetFileByPath(zlib, sPath.c_str());

        if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L'/' + NSFile::GetFileName(UTF8_TO_U(sPath))))
        {
            if (sFile)
            {
                unsigned int nFileLength = (sFile + 4)[0] | (sFile + 4)[1] << 8 | (sFile + 4)[2] << 16 | (sFile + 4)[3] << 24;
                oFile.WriteFile(sFile + 8, nFileLength);
            }
            oFile.CloseFile();
        }
    }

    delete[] pData;
    Zlib_Destroy(zlib);
    return 0;
}
