#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../DesktopEditor/common/Path.h"
#include "../../wasm/src/base.h"
#include "../../wasm/src/engine.h"
#include "../../../OfficeUtils.h"

#include <string>
#include <vector>

static unsigned int GetLength(BYTE* x)
{
    return x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24;
}

int main()
{
    CData oTree;
    oTree.ClearNoAttack();
    oTree.SkipLen();
    std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(NSFile::GetProcessDirectory() + L"/test", true);
    for (std::wstring& path : arrFiles)
    {
        BYTE* arrPath = NULL;
        LONG pathLen;
        std::wstring sPath = NSFile::GetFileName(path);
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sPath.c_str(), sPath.length(), arrPath, pathLen);
        oTree.WriteString(arrPath, pathLen);

        DWORD nBytesCount;
        BYTE* pData;
        NSFile::CFileBinary oFile;
        if (oFile.ReadAllBytes(path, &pData, nBytesCount))
            oFile.CloseFile();
        oTree.WriteString(pData, nBytesCount);
    }
    oTree.WriteLen();

    Zlib* zlib1 = Zlib_Create();
    BYTE* oZip = Zlib_CompressFiles(zlib1, oTree.GetBuffer());
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

    Zlib* zlib = Zlib_Load(pData, nBytesCount);

    BYTE* sPaths = Zlib_GetPathsInArchive(zlib);
    unsigned int nLength = GetLength(sPaths);
    unsigned int i = 4;
    nLength -= 4;

    while (i < nLength)
    {
        unsigned int nPathLength = GetLength(sPaths + i);
        i += 4;
        BYTE* sFile = Zlib_GetFileFromArchive(zlib, std::string((char*)(sPaths + i), nPathLength).c_str());

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
