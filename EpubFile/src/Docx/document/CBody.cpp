#include "CBody.h"
#include "CParagraph.h"
#include "CSectionProperties.h"

CBody::CBody() :
    CElement(L"body", L"", L"w")
{
}

void CBody::SetDefoult()
{
    CParagraph *oParagraph = new CParagraph;
    oParagraph->SetDefoult();
    AddChildren(oParagraph);

    CSectionProperties *oSectionProperties = new CSectionProperties;
    oSectionProperties->SetDefoult();
    AddChildren(oSectionProperties);
}
