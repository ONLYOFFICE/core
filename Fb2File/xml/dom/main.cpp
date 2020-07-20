#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/File.h"

int main()
{
    XmlUtils::CXmlNode oNode;
    oNode.FromXmlFile(NSFile::GetProcessDirectory() + L"/../../../examples/test.xml");

    return 0;
}
