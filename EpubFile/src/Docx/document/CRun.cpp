#include "CRun.h"
#include "CRunProperties.h"

CRun::CRun()
    :CElement(L"r", L"", L"w")
{
}

void CRun::SetDefoult()
{
    CRunProperties *oRunProperties = new CRunProperties;
    oRunProperties->SetDefoult();
    AddChildren(oRunProperties);

    CElement *oText = new CElement(L"t",L"", L"w", false);
    AddChildren(oText);
}
