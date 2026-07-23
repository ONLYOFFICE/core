/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "../CEpubFile.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

#include <time.h>
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::wstring sTmp = NSFile::GetProcessDirectory() + L"/tmp";
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/OutputFiles";
    NSDirectory::CreateDirectory(sOutputDirectory);
    NSDirectory::CreateDirectory(sTmp);

    bool bFromHtml = false;
    if (bFromHtml)
    {
        std::wstring sFile = NSFile::GetProcessDirectory() + L"/test.html";
        CEpubFile oEpub;
        oEpub.SetTempDirectory(sTmp);
        oEpub.FromHtml(sFile, sOutputDirectory + L"/res.epub", L"");
        NSDirectory::DeleteDirectory(sTmp);
        return 0;
    }

    std::vector<std::wstring> arFiles = NSDirectory::GetFiles(NSFile::GetProcessDirectory() + L"/../../../Files");

    clock_t tTime1 = clock();
    for (const std::wstring& sFileName : arFiles)
    {
        clock_t tTimeBegin = clock();

        CEpubFile oEpub;

        if (oEpub.IsEpubFile(sFileName) == S_OK)
        {
            std::wstring sFile = NSFile::GetFileName(sFileName);
            sFile = sFile.substr(0, sFile.rfind(L'.'));

            std::wcout << L"|----------|" << sFile << L"|----------|" << std::endl;

            oEpub.SetTempDirectory(sTmp);
            oEpub.Convert(sFileName, sOutputDirectory + L"/" + sFile + L".docx");
            NSDirectory::DeleteDirectory(sTmp + L"/res/word/media");
//            oEpub.ShowMap();
        }
        else
            std::wcout << sFileName << L" this is not an epub format!" << std::endl;
        clock_t tTimeEnd = clock();

        std::wcout << (double)(tTimeEnd - tTimeBegin) / CLOCKS_PER_SEC << std::endl;
    }

    clock_t tTime2 = clock();

    std::wcout << (double)(tTime2 - tTime1) / CLOCKS_PER_SEC << std::endl;
    NSDirectory::DeleteDirectory(sTmp);

    return 0;
}
