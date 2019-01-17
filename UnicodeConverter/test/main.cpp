/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include "../UnicodeConverter.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
    std::wstring sPathTests = NSFile::GetProcessDirectory() + L"/../testfiles/";

#if 0
    std::wstring sOriginal = sPathTests + L"0.txt";
    for (int i = 1; i < 52; i++)
    {
        std::wstring s1 = sPathTests + std::to_wstring(i) + L".txt";
        std::wstring s2 = sPathTests + std::to_wstring(i) + L"_utf8.txt";
        NSFile::CFileBinary::Copy(sOriginal, s1);
        NSFile::CFileBinary::Copy(sOriginal, s2);
    }

    return 0;
#endif

    NSUnicodeConverter::CUnicodeConverter oConverter;

    int nCountTests = 46;

    for (int i = 0; i < nCountTests; ++i)
    {
        BYTE* pDataSrc = NULL;
        DWORD nLenSrc = 0;

        if (!NSFile::CFileBinary::ReadAllBytes(sPathTests + std::to_wstring(i) + L".txt", &pDataSrc, nLenSrc))
            continue;

        std::wstring sResult = oConverter.toUnicode((const char*)pDataSrc, (unsigned int)nLenSrc, NSUnicodeConverter::Encodings[i].Name);

        std::wstring sResultCache = L"";
        NSFile::CFileBinary::ReadAllTextUtf8(sPathTests + std::to_wstring(i) + L"_utf8.txt", sResultCache);

        bool bIsEqual = (sResult == sResultCache);

        RELEASEARRAYOBJECTS(pDataSrc);

        NSFile::CFileBinary::SaveToFile(sPathTests + std::to_wstring(i) + L"_test_utf8.txt", sResult, true);
    }

#if 0
    std::string str1 = oConverter.fromUnicode(L"рус", "windows-1251");

    std::wstring str2 = oConverter.toUnicode(str1, "windows-1251");
    std::string str3 = oConverter.fromUnicode(str2, "windows-1251");
#endif

    return 0;
}
