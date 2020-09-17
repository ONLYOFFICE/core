#include "CRelationships.h"

CRelationships::CRelationships()
    : CElement(L"Relationships")
{
}

void CRelationships::SetDefoult()
{
    AddArgument(L"xmlns", L"http://schemas.openxmlformats.org/package/2006/relationships");

    CElement *oRelationship1 = new CElement(L"Relationship");
    oRelationship1->AddArgument(L"Id", L"rId3");
    oRelationship1->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties");
    oRelationship1->AddArgument(L"Target", L"docProps/app.xml");
    AddChildren(oRelationship1);

    CElement *oRelationship2 = new CElement(L"Relationship");
    oRelationship2->AddArgument(L"Id", L"rId2");
    oRelationship2->AddArgument(L"Type", L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties");
    oRelationship2->AddArgument(L"Target", L"docProps/core.xml");
    AddChildren(oRelationship2);

    CElement *oRelationship3 = new CElement(L"Relationship");
    oRelationship3->AddArgument(L"Id", L"rId1");
    oRelationship3->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");
    oRelationship3->AddArgument(L"Target", L"word/document.xml");
    AddChildren(oRelationship3);
}
