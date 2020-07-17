#include "CStyle.h"

CStyle::CStyle()
    : CElement(L"style", L"", L"w")
{
}

void CStyle::SetDefoult()
{
    AddArgument(L"type", L"paragraph");
    AddArgument(L"default", L"1");
    AddArgument(L"styleId", L"a");

    CElement *oName = new CElement(L"name", L"", L"w");
    oName->AddArgument(L"val", L"Normal");
    AddChildren(oName);

    CElement *oQFormat = new CElement(L"qFormat", L"", L"w");
    AddChildren(oQFormat);
}
