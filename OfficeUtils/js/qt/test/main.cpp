/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
