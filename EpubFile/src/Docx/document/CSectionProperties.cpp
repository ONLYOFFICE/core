#include "CSectionProperties.h"

CSectionProperties::CSectionProperties()
    :CElement(L"sectPr", L"", L"w")
{
}

void CSectionProperties::SetDefoult()
{
    AddArgument(L"rsidR", L"003939F5");
    AddArgument(L"rsidSect", L"00215247");

    CElement *oPgSz = new CElement(L"pgSz", L"", L"w");
    oPgSz->AddArgument(L"w", L"11910");
    oPgSz->AddArgument(L"h", L"16850");
    AddChildren(oPgSz);

    CElement *oPgMar = new CElement(L"pgMar", L"", L"w");
    oPgMar->AddArgument(L"top", L"1040");
    oPgMar->AddArgument(L"right", L"697");
    oPgMar->AddArgument(L"bottom", L"1260");
    oPgMar->AddArgument(L"left", L"1580");
    oPgMar->AddArgument(L"header", L"0");
    oPgMar->AddArgument(L"footer", L"1026");
    oPgMar->AddArgument(L"gutter", L"0");
    AddChildren(oPgMar);

    CElement *oCols = new CElement(L"cols", L"", L"w");
    oCols->AddArgument(L"space", L"708");
    AddChildren(oCols);

    CElement *oDocGrid = new CElement(L"docGrid", L"", L"w");
    oDocGrid->AddArgument(L"linePitch", L"299");
    AddChildren(oDocGrid);
}
