#include "CSdtEndPr.h"

CSdtEndPr::CSdtEndPr()
    : CElement(L"sdtEndPr", L"", L"w")
{
}

void CSdtEndPr::SetDefoult()
{
    CElement *oRPr = new CElement(L"rPr", L"", L"w");
        CElement *oRFonts = new CElement(L"rFonts", L"", L"w");
        oRFonts->AddArgument(L"asciiTheme", L"minorHAnsi");
        oRFonts->AddArgument(L"eastAsiaTheme", L"minorEastAsia");
        oRFonts->AddArgument(L"hAnsiTheme", L"minorHAnsi");
        oRFonts->AddArgument(L"cstheme", L"minorBidi");
        oRPr->AddChildren(oRFonts);

        CElement *oB = new CElement(L"b", L"", L"w");
        oRPr->AddChildren(oB);

        CElement *oBCs = new CElement(L"bCs", L"", L"w");
        oRPr->AddChildren(oBCs);

        CElement *oColor = new CElement(L"color", L"", L"w");
        oColor->AddArgument(L"val", L"auto");
        oRPr->AddChildren(oColor);

        CElement *oSz = new CElement(L"sz", L"", L"w");
        oSz->AddArgument(L"val", L"22");
        oRPr->AddChildren(oSz);

        CElement *oSzCs = new CElement(L"szCs", L"", L"w");
        oSzCs->AddArgument(L"val", L"22");
        oRPr->AddChildren(oSzCs);

    AddChildren(oRPr);
}
