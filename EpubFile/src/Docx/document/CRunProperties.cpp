#include "CRunProperties.h"

CRunProperties::CRunProperties()
    : CElement(L"rPr", L"", L"w")
{
}

void CRunProperties::SetDefoult()
{
    CElement *oI = new CElement(L"i", L"", L"w");
    AddChildren(oI);
}
