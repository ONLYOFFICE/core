#include <iostream>
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/File.h"

int main()
{
    XmlUtils::CXmlLiteReader oLightReader;
    if (oLightReader.FromFile(NSFile::GetProcessDirectory() + L"/../../../examples/test.xml"))
    {
        while(oLightReader.ReadNextNode())
        {
            // Читает файл потоком заменяя \n и \t
            // std::wcout << oLightReader.GetOuterXml() << std::endl;

            // Читаю с помощью функций
            std::wcout << "tag: "         << oLightReader.GetName()
                       << "\tdepth: "     << oLightReader.GetDepth();
            while(oLightReader.MoveToNextAttribute())
            {
                std::wcout << "\tattribute: " << oLightReader.GetName()
                           << "\ttext: "     << oLightReader.GetText();
            }
            oLightReader.MoveToElement();
            if(oLightReader.ReadNextSiblingNode2(oLightReader.GetDepth()))
            {
                std::wstring res = oLightReader.GetText();
                std::wcout << "\ttext: " << res;
            }
            std::wcout << std::endl;
        }
    }
    else
    {
        std::cout << "File don't open" << std::endl;
    }

    return 0;
}
