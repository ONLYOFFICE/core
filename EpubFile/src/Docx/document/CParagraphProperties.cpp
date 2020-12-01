#include "CParagraphProperties.h"
#include "CRunProperties.h"

CParagraphProperties::CParagraphProperties()
    : CElement(L"pPr", L"", L"w")
{
}

void CParagraphProperties::SetDefoult()
{
    CRunProperties *oRunProperties = new CRunProperties;
    AddChildren(oRunProperties);
}
