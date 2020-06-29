#include "../../DesktopEditor/xml/include/xmlutils.h"

int main(int argc, char *argv[])
{
    // dom - model test ---------------------------------------
    XmlUtils::CXmlNode oNode;
    oNode.FromXmlFile(L"pathToXmlFile");

    // просто посмотри, как читается хмл. повызывай методы. прочитай вложенную ноду, ее атрибуты

    // примеры методов
    oNode.GetNode(L"rootNodeName");
    oNode.GetAttribute(L"attributeName");

    // sax reader ---------------------------------------------
    XmlUtils::CXmlLiteReader oLightReader;

    // просто посмотри, как читается хмл. повызывай методы. прочитай вложенную ноду, ее атрибуты
    // примеры методов
    oLightReader.ReadNextNode();


    return 0;
}
