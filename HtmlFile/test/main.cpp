#include "../HtmlFile.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/Debug/";

    CHtmlFile oFile;

    std::wstring sXml = L"\
<html>\
<sdk>file:///D:/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Word/sdk-all.js</sdk>\
<file>file:///C:/Users/oleg.korshul/Desktop/original_message%20(5).html</file>\
<file>file://192.168.3.208/allusers/Files/HTML/AllHTML/cars.html</file>\
<destination>D:/test/Document/</destination>\
</html>\
";

    int nResult = oFile.Convert(sXml, sPath);
    nResult;

    return 0;
}
