#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../DesktopEditor/common/Path.h"
#include "../../wasm/src/base.h"
#include "../../../OfficeUtils.h"

#include <string>
#include <vector>

static unsigned int GetLength(BYTE* x)
{
    return x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24;
}

int main()
{
    CZipBuffer* zlib1 = Zlib_Create();
    std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(NSFile::GetProcessDirectory() + L"/mytest", true);
    for (std::wstring& path : arrFiles)
    {
        DWORD nBytesCount;
        BYTE* pData;
        NSFile::CFileBinary oFile;
        if (oFile.ReadAllBytes(path, &pData, nBytesCount))
            oFile.CloseFile();
        zlib1->addFile(U_TO_UTF8(NSFile::GetFileName(path)), pData, nBytesCount);
    }

    BYTE* oZip = Zlib_Save(zlib1);
    NSFile::CFileBinary oFile1;
    if (oFile1.CreateFileW(NSFile::GetProcessDirectory() + L"/test.zip"))
    {
        oFile1.WriteFile(oZip + 4, GetLength(oZip));
        oFile1.CloseFile();
    }

    DWORD nBytesCount;
    BYTE* pData;
    NSFile::CFileBinary oFile;
    if (oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.zip", &pData, nBytesCount))
        oFile.CloseFile();

    CZipBuffer* zlib = Zlib_Open(pData, nBytesCount);

    BYTE* sPaths = Zlib_GetPaths(zlib);
    unsigned int nLength = GetLength(sPaths);
    unsigned int i = 4;
    nLength -= 4;

    while (i < nLength)
    {
        unsigned int nPathLength = GetLength(sPaths + i);
        i += 4;
        BYTE* sFile = Zlib_GetFile(zlib, std::string((char*)(sPaths + i), nPathLength).c_str());

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

    Zlib_Close(zlib);
    return 0;
}
