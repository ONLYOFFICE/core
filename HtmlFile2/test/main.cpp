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

#include <iostream>
#include <vector>
#include <chrono>
#include "../htmlfile2.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include "../../Common/3dParty/md/md2html.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>

void getDirectories(const std::wstring& sDirectory, std::vector<std::wstring>& arrDirectory)
{
    arrDirectory.push_back(sDirectory);
    for(const std::wstring& sT : NSDirectory::GetDirectories(sDirectory))
        getDirectories(sT, arrDirectory);
}

int main()
{
    // if (Md::ConvertMdFileToHtml(L"C:\\ONLYOFFICE\\Files\\md\\Bugs\\74626\\file-1.md",
    //                             L"C:\\ONLYOFFICE\\Files\\md\\Bugs\\74626\\file-1.html"))
    //     std::wcout << L"Good" << std::endl;
    // else
    //     std::wcout << L"Bad" << std::endl;

    // return 0;


    bool bBatchMode = false;
    bool bMhtMode = false;
    if(bBatchMode)
    {
        // Files directory
        std::wstring sDirectory = NSFile::GetProcessDirectory() + L"/../../../examples/html";
        // Nested directories
        std::vector<std::wstring> arrDirectory;
        getDirectories(sDirectory, arrDirectory);
        COfficeUtils oZip;
        // Set temporary directory
        std::wstring sTmp = NSFile::GetProcessDirectory() + L"/tmp";
        NSDirectory::DeleteDirectory(sTmp);
        NSDirectory::CreateDirectory(sTmp);

        for(std::wstring sD : arrDirectory)
        {
            std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(sD);
            // Directory where we will create docx
            size_t nPos = sD.find(L"/html");
            std::wstring sOutputDirectory = sD.insert(nPos + 5, L"-res");
            NSDirectory::CreateDirectory(sOutputDirectory);

            for(const std::wstring& sFile : arrFiles)
            {
                std::wstring sFileName = NSFile::GetFileName(sFile);
                std::wcout << sFileName << std::endl;
                HRESULT nResConvert;
                CHtmlFile2 oFile;
                oFile.SetTmpDirectory(sTmp);
                std::chrono::system_clock::time_point begin = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point end;
                if(bMhtMode)
                {
                    if(!oFile.IsMhtFile(sFile))
                    {
                        std::cout << "This isn't a mht file" << std::endl;
                        continue;
                    }
                    end = std::chrono::system_clock::now();
                    nResConvert = oFile.OpenMht(sFile, sTmp);
                }
                else
                {
                    if(!oFile.IsHtmlFile(sFile))
                    {
                        std::cout << "This isn't a html file" << std::endl;
                        continue;
                    }
                    end = std::chrono::system_clock::now();
                    nResConvert = oFile.OpenHtml(sFile, sTmp);
                }
                std::chrono::duration<long, std::milli> dur1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
                begin = std::chrono::system_clock::now();
                std::chrono::duration<long, std::milli> dur2 = std::chrono::duration_cast<std::chrono::milliseconds>(begin - end);
                if(nResConvert == S_OK)
                {
                    std::cout << "Success " << dur1.count() << " " << dur2.count() << std::endl;
                    oZip.CompressFileOrDirectory(sTmp, sOutputDirectory + L"/" + sFileName + L".docx");
                    NSDirectory::DeleteDirectory(sTmp + L"/word/media");
                }
                else
                    std::cout << "Failure" << std::endl;
            }
        }
    }
    else
    {
        HRESULT nResConvert = S_FALSE;
        // Directory where we will create docx
        std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
        NSDirectory::DeleteDirectory(sOutputDirectory);
        NSDirectory::CreateDirectory(sOutputDirectory);

        CHtmlParams oParams;
        oParams.SetAuthors(L"last first middle;last2 first2 middle2");
        oParams.SetGenres(L"fantazy, drama");
        oParams.SetTitle(L"The Last Wish");
        oParams.SetDate(L"2010-06-03T04:00:00+00:00");
        oParams.SetDescription(L"Description");

        // File to open
        std::wstring sFile = L"C:\\ONLYOFFICE\\Files\\html\\test_tag_a_1.html";
        CHtmlFile2 oFile;
        oFile.SetTmpDirectory(sOutputDirectory);
        nResConvert = (bMhtMode ? oFile.OpenMht(sFile, sOutputDirectory, &oParams) : oFile.OpenHtml(sFile, sOutputDirectory, &oParams));

        if(nResConvert == S_OK)
        {
            std::cout << "Success" << std::endl;
            COfficeUtils oZip;
            oZip.CompressFileOrDirectory(sOutputDirectory, sOutputDirectory + L"/Aggregate.docx");
        }
        else
            std::cout << "Failure" << std::endl;
    }
    std::cout << "THE END" << std::endl;
    return 0;
}
