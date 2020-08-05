#include "CSdtPr.h"

CSdtPr::CSdtPr()
    : CElement(L"stdPr", L"", L"w")
{
}

void CSdtPr::SetDefoult()
{
    CElement *oId = new CElement(L"id", L"", L"w");
    oId->AddArgument(L"val", L"1");
    AddChildren(oId);

    CElement *oDocPartObj = new CElement(L"docPartObj", L"", L"w");
        CElement *oDocPartGallery = new CElement(L"docPartGallery", L"", L"w");
        oDocPartGallery->AddArgument(L"val", L"Table of Contents");
        oDocPartObj->AddChildren(oDocPartGallery);

        CElement *oDocPartUnique = new CElement(L"docPartUnique", L"", L"w");
        oDocPartObj->AddChildren(oDocPartUnique);

    AddChildren(oDocPartObj);
}
