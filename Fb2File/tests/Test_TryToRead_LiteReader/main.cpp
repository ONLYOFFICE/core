#include <iostream>
#include "../../../DesktopEditor/xml/include/xmlutils.h"

int main()
{
    XmlUtils::CXmlLiteReader oLightReader;
    if (oLightReader.FromFile(L"../../../../test.xml"))
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
