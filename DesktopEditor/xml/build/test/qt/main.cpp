#include <QCoreApplication>

#include "../../../include/xmlutils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    XmlUtils::CXmlNode oNode;
    oNode.FromXmlFile(L"D:\\document.xml");

    return a.exec();
}
