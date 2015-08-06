#include "../HtmlFile.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
#if 0

#ifdef WIN32
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/windows/Release/";

    std::wstring sXml = L"\
<html>\
<sdk>file:///D:/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Word/sdk-all.js</sdk>\
<file>file:///C:/Users/oleg.korshul/Desktop/original_message%20(5).html</file>\
<destination>D:/test/Document/</destination>\
</html>\
";
#else
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";

    std::wstring sXml = L"\
<html>\
<sdk>file:///home/oleg/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Word/sdk-all.js</sdk>\
<file>file:///home/oleg/activex/test.html</file>\
<destination>/home/oleg/activex/1/</destination>\
</html>\
";
#endif

    CHtmlFile oFile;
    int nResult = oFile.Convert(sXml, sPath);
    nResult;

#else

#ifdef WIN32
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/windows/Release/";

    std::wstring sXml = L"\
<sdk>file:///D:/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Word/sdk-all.js</sdk>\
<destination>D:/test/Document/</destination>\
";
#else
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";

    std::wstring sXml = L"\
<html>\
<sdk>file:///home/oleg/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Word/sdk-all.js</sdk>\
<file>file:///home/oleg/activex/test.html</file>\
<destination>/home/oleg/activex/1/</destination>\
</html>\
";
#endif

    CHtmlFile oFile;
    std::wstring sMetaInfo;
    int nResult = oFile.ConvertEpub(L"D:\\37898EB", sMetaInfo, sXml, sPath);
    nResult;

#endif

    return 0;
}
