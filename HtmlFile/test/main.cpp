#include "../HtmlFile.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
#if 0

    std::vector<std::wstring> arFiles;
#ifdef WIN32
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/windows/Release/";

    arFiles.push_back(L"file:///C:/Users/oleg.korshul/Desktop/original_message%20(5).html");
    std::wstring sDstFolder = L"D:/test/Document";
#else
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";

    arFiles.push_back(L"/home/oleg/activex/test.html");
    std::wstring sDstFolder = L"/home/oleg/activex/1/";
#endif

    CHtmlFile oFile;
    int nResult = oFile.Convert(arFiles, sDstFolder, sPath);
    nResult;

#else

#ifdef WIN32
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/windows/Release/";

    std::wstring sSrc = L"D:\\37898EB";
    std::wstring sDstFolder = L"D:/test/Document";
#else
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";

    std::wstring sSrc = L"/home/oleg/activex/37898EB";
    std::wstring sDstFolder = L"/home/oleg/activex/1/";
#endif

    CHtmlFile oFile;
    std::wstring sMetaInfo;
    int nResult = oFile.ConvertEpub(sSrc, sMetaInfo, sDstFolder, sPath);
    nResult;

#endif

    return 0;
}
