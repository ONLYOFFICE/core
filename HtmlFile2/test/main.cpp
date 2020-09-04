#include <iostream>
#include <vector>
#include "../htmlfile2.h"
#include "../mhtfile.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

void getDirectories(std::wstring sDirectory, std::vector<std::wstring>& arrDirectory)
{
    arrDirectory.push_back(sDirectory);
    for(std::wstring sT : NSDirectory::GetDirectories(sDirectory))
        getDirectories(sT, arrDirectory);
}

int main()
{
    bool bBatchMode = false;
    bool bMhtMode = true;
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

        int nErrorCol = 0;
        std::vector<std::wstring> arrError;

        for(std::wstring sD : arrDirectory)
        {
            std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(sD);

            // Директория, где будем создавать docx
            size_t nPos = sD.find(L"/html");
            std::wstring sOutputDirectory = sD.insert(nPos + 5, L"-res");
            NSDirectory::CreateDirectory(sOutputDirectory);

            for(std::wstring sFile : arrFiles)
            {
                std::wstring sFileName = NSFile::GetFileName(sFile);
                std::wcout << sFileName << std::endl;
                HRESULT nResConvert;
                if(bMhtMode)
                {
                    CMhtFile oFile;
                    oFile.SetTmpDirectory(sTmp);
                    if(!oFile.IsMhtFile(sFile))
                    {
                        nErrorCol++;
                        arrError.push_back(sFileName);
                        std::cout << "This isn't a mht file" << std::endl;
                        continue;
                    }
                    nResConvert = oFile.Open(sFile, sTmp);
                }
                else
                {
                    CHtmlFile2 oFile;
                    oFile.SetTmpDirectory(sTmp);
                    if(!oFile.IsHtmlFile(sFile))
                    {
                        nErrorCol++;
                        arrError.push_back(sFileName);
                        std::cout << "This isn't a html file" << std::endl;
                        continue;
                    }
                    nResConvert = oFile.Open(sFile, sTmp);
                }
                if(nResConvert == S_OK)
                {
                    std::cout << "Success" << std::endl;
                    oZip.CompressFileOrDirectory(sTmp, sOutputDirectory + L"/" + sFileName + L".docx");
                    NSDirectory::DeleteDirectory(sTmp + L"/word/media");
                }
                else
                {
                    nErrorCol++;
                    arrError.push_back(sFileName);
                    std::cout << "Failure" << std::endl;
                }
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
        std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test.mht";

        if(bMhtMode)
        {
            CMhtFile oFile;
            oFile.SetTmpDirectory(sOutputDirectory);
            nResConvert = oFile.Open(sFile, sOutputDirectory, &oParams);
        }
        else
        {
            CHtmlFile2 oFile;
            oFile.SetTmpDirectory(sOutputDirectory);
            nResConvert = oFile.Open(sFile, sOutputDirectory, &oParams);
        }

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
