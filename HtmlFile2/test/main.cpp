#include <iostream>
#include <vector>
#include "../htmlfile2.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

int main()
{
    bool bBatchMode = false;
    CHtmlFile2 oFile;

    // Директория, где будем создавать docx
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
    NSDirectory::DeleteDirectory(sOutputDirectory);
    NSDirectory::CreateDirectory(sOutputDirectory);

    oFile.SetTmpDirectory(sOutputDirectory);
    HRESULT nResConvert = S_FALSE;

    CHtmlParams oParams;
    oParams.SetAuthors(L"last first middle;last2 first2 middle2");
    oParams.SetGenres(L"fantazy, drama");
    oParams.SetTitle(L"The Last Wish");
    oParams.SetDate(L"2010-06-03T04:00:00+00:00");
    oParams.SetDescription(L"Description");

    if(bBatchMode)
    {
        std::vector<std::wstring> arrFiles{NSFile::GetProcessDirectory() + L"/../../../examples/test1.html",
                                           NSFile::GetProcessDirectory() + L"/../../../examples/test2.xhtml",
                                           NSFile::GetProcessDirectory() + L"/../../../examples/test3.xhtml",
                                           NSFile::GetProcessDirectory() + L"/../../../examples/test4.xhtml"};
        nResConvert = oFile.OpenBatch(arrFiles, sOutputDirectory, &oParams);
    }
    else
    {
        // Файл, который открываем
        std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test4.xhtml";
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

    std::cout << "THE END" << std::endl;
    return 0;
}
