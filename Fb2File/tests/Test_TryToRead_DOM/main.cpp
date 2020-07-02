#include <iostream>
#include "../../../DesktopEditor/xml/include/xmlutils.h"

int main()
{
    XmlUtils::CXmlNode oNode;
    oNode.FromXmlFile(L"../../../../test.xml");

    return 0;
}
