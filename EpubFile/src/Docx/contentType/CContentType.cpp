#include "CContentType.h"

CContentType::CContentType()
    : CElement(L"Types")
{
}

void CContentType::SetDefoult()
{
    AddArgument(L"xmlns", L"http://schemas.openxmlformats.org/package/2006/content-types");

    CElement *oDefault1 = new CElement(L"Default");
    oDefault1->AddArgument(L"Extension", L"rels");
    oDefault1->AddArgument(L"ContentType", L"application/vnd.openxmlformats-package.relationships+xml");
    AddChildren(oDefault1);

    CElement *oDefault2 = new CElement(L"Default");
    oDefault2->AddArgument(L"Extension", L"xml");
    oDefault2->AddArgument(L"ContentType", L"application/xml");
    AddChildren(oDefault2);

    CElement *oOverride1 = new CElement(L"Override");
    oOverride1->AddArgument(L"PartName", L"/word/document.xml");
    oOverride1->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
    AddChildren(oOverride1);

    CElement *oOverride2 = new CElement(L"Override");
    oOverride2->AddArgument(L"PartName", L"/word/styles.xml");
    oOverride2->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml");
    AddChildren(oOverride2);

    CElement *oOverride3 = new CElement(L"Override");
    oOverride3->AddArgument(L"PartName", L"/word/settings.xml");
    oOverride3->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml");
    AddChildren(oOverride3);

    CElement *oOverride4 = new CElement(L"Override");
    oOverride4->AddArgument(L"PartName", L"/word/webSettings.xml");
    oOverride4->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml");
    AddChildren(oOverride4);

    CElement *oOverride5 = new CElement(L"Override");
    oOverride5->AddArgument(L"PartName", L"/word/fontTable.xml");
    oOverride5->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml");
    AddChildren(oOverride5);

    CElement *oOverride6 = new CElement(L"Override");
    oOverride6->AddArgument(L"PartName", L"/word/theme/theme1.xml");
    oOverride6->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.theme+xml");
    AddChildren(oOverride6);

    CElement *oOverride7 = new CElement(L"Override");
    oOverride7->AddArgument(L"PartName", L"/docProps/core.xml");
    oOverride7->AddArgument(L"ContentType", L"application/vnd.openxmlformats-package.core-properties+xml");
    AddChildren(oOverride7);

    CElement *oOverride8 = new CElement(L"Override");
    oOverride8->AddArgument(L"PartName", L"/docProps/app.xml");
    oOverride8->AddArgument(L"ContentType", L"application/vnd.openxmlformats-officedocument.extended-properties+xml");
    AddChildren(oOverride8);
}
