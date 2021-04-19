#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../wasm/src/base.h"
#include "../../../OfficeUtils.h"

#include <string>
#include <vector>

unsigned int GetLength(BYTE* x)
{
    return x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24;
}

bool RFC(std::wstring& file_name, BYTE*& pData, long& nSize, void* pParam)
{
    return true;
}

int main()
{
    COfficeUtils cOU;
    cOU.CompressFileOrDirectory(NSFile::GetProcessDirectory() + L"/test", NSFile::GetProcessDirectory() + L"/test.zip");
    RequestFileCallback callback = RFC;
    bool bResult;
    cOU.CompressFilesFromMemory(NSFile::GetProcessDirectory() + L"/test.zip", callback, NULL, -1, &bResult);

    DWORD nBytesCount;
    BYTE* pData;
    NSFile::CFileBinary oFile;
    if (oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.zip", &pData, nBytesCount))
        oFile.CloseFile();

    Zlib* zlib = Zlib_Load(pData, nBytesCount);

    BYTE* sPaths = Zlib_GetPaths(zlib);
    unsigned int nLength = GetLength(sPaths);
    unsigned int i = 4;
    nLength -= 4;

    while (i < nLength)
    {
        unsigned int nPathLength = GetLength(sPaths + i);
        i += 4;
        BYTE* sFile = Zlib_GetFileByPath(zlib, std::string((char*)(sPaths + i), nPathLength).c_str());

        if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L'/' + NSFile::GetFileName(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(sPaths + i, nPathLength))))
        {
            if (sFile)
            {
                unsigned int nFileLength = GetLength(sFile);
                oFile.WriteFile(sFile + 4, nFileLength);
            }
            oFile.CloseFile();
        }
        i += nPathLength;
    }

    Zlib_Destroy(zlib);
    return 0;
}
