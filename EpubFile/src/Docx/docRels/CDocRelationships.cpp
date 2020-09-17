#include "CDocRelationships.h"

CDocRelationships::CDocRelationships()
    : CElement(L"Relationships", L"", L"", false)
{
}

void CDocRelationships::SetDefoult()
{
    AddArgument(L"xmlns", L"http://schemas.openxmlformats.org/package/2006/relationships");

    CElement *oRelationship1 = new CElement(L"Relationship");
    oRelationship1->AddArgument(L"Id", L"rId3");
    oRelationship1->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings");
    oRelationship1->AddArgument(L"Target", L"webSettings.xml");
    AddChildren(oRelationship1);

    CElement *oRelationship2 = new CElement(L"Relationship");
    oRelationship2->AddArgument(L"Id", L"rId2");
    oRelationship2->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings");
    oRelationship2->AddArgument(L"Target", L"settings.xml");
    AddChildren(oRelationship2);

    CElement *oRelationship3 = new CElement(L"Relationship");
    oRelationship3->AddArgument(L"Id", L"rId1");
    oRelationship3->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles");
    oRelationship3->AddArgument(L"Target", L"styles.xml");
    AddChildren(oRelationship3);

    CElement *oRelationship4 = new CElement(L"Relationship");
    oRelationship4->AddArgument(L"Id", L"rId5");
    oRelationship4->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme");
    oRelationship4->AddArgument(L"Target", L"theme/theme1.xml");
    AddChildren(oRelationship4);

    CElement *oRelationship5 = new CElement(L"Relationship");
    oRelationship5->AddArgument(L"Id", L"rId4");
    oRelationship5->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable");
    oRelationship5->AddArgument(L"Target", L"fontTable.xml");
    AddChildren(oRelationship5);
}
