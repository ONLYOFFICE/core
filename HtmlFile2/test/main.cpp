#include <iostream>
#include <vector>
#include <chrono>
#include "../htmlfile2.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

void getDirectories(const std::wstring& sDirectory, std::vector<std::wstring>& arrDirectory)
{
    arrDirectory.push_back(sDirectory);
    for(const std::wstring& sT : NSDirectory::GetDirectories(sDirectory))
        getDirectories(sT, arrDirectory);
}

int main()
{
    bool bBatchMode = false;
    bool bMhtMode = false;
    if(bBatchMode)
    {
        // Директория файлов
        std::wstring sDirectory = NSFile::GetProcessDirectory() + L"/../../../examples/html";
        // Вложенные директории
        std::vector<std::wstring> arrDirectory;
        getDirectories(sDirectory, arrDirectory);
        COfficeUtils oZip;
        // Выставляем временную директорию
        std::wstring sTmp = NSFile::GetProcessDirectory() + L"/tmp";
        NSDirectory::DeleteDirectory(sTmp);
        NSDirectory::CreateDirectory(sTmp);

        for(std::wstring sD : arrDirectory)
        {
            std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(sD);
            // Директория, где будем создавать docx
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
        // Директория, где будем создавать docx
        std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
        NSDirectory::DeleteDirectory(sOutputDirectory);
        NSDirectory::CreateDirectory(sOutputDirectory);

        CHtmlParams oParams;
        oParams.SetAuthors(L"last first middle;last2 first2 middle2");
        oParams.SetGenres(L"fantazy, drama");
        oParams.SetTitle(L"The Last Wish");
        oParams.SetDate(L"2010-06-03T04:00:00+00:00");
        oParams.SetDescription(L"Description");

        // Файл, который открываем
        std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test.html";
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
