#ifndef _CMAP_WASM_SERIALIZE_H
#define _CMAP_WASM_SERIALIZE_H

#include "serialize.h"
#include "../../../../../common/File.h"
#include "../../../../../common/Directory.h"

#include <vector>

namespace NSWasm
{
void CMapDirToFile()
{
    std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(NSFile::GetProcessDirectory() + L"/../../../../../../../../PdfFile/Resources/CMap/CMap");

    CData oRes;
    for (const std::wstring& sFile : arrFiles)
    {
        BYTE* pData = NULL;
        DWORD nSize;
        NSFile::CFileBinary oFile;
        if (oFile.ReadAllBytes(sFile, &pData, nSize))
        {
            std::wstring sFileName = NSFile::GetFileName(sFile);
            BYTE* pStr = NULL;
            LONG nStrSize;
            NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), (LONG)sFileName.length(), pStr, nStrSize);
            oRes.WriteString(pStr, nStrSize);
            RELEASEARRAYOBJECTS(pStr);

            oRes.AddInt(nSize);
            oRes.Write(pData, nSize);

            oFile.CloseFile();
        }

        RELEASEARRAYOBJECTS(pData);
    }

    NSFile::CFileBinary oFile;
    if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/../../../../../../../../PdfFile/Resources/CMapMemory/CMapData"))
    {
        oFile.WriteFile(oRes.GetBuffer(), oRes.GetSize());
        oFile.CloseFile();
    }

    oRes.Clear();
}
}

#endif // _CMAP_WASM_SERIALIZE_H
