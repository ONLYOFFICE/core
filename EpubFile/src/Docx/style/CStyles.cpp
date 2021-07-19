#include "CStyles.h"
#include "CStyle.h"

CStyles::CStyles()
    :CElement(L"styles", L"", L"w")
{

}

void CStyles::SetDefoult()
{
    CElement *oDocDefaults = new CElement(L"docDefaults", L"", L"w");
    CElement *oRPrDefault = new CElement(L"rPrDefault", L"", L"w");
    CElement *oRPr = new CElement(L"rPr", L"", L"w");

    CElement *oRFonts = new CElement(L"rFonts", L"", L"w");
    oRFonts->AddArgument(L"asciiTheme", L"minorHAnsi");
    oRFonts->AddArgument(L"eastAsiaTheme", L"minorEastAsia");
    oRFonts->AddArgument(L"hAnsiTheme", L"minorHAnsi");
    oRFonts->AddArgument(L"cstheme", L"minorBidi");
    oRPr->AddChildren(oRFonts);

    CElement *oSz = new CElement(L"sz", L"", L"w");
    oSz->AddArgument(L"val", L"22");
    oRPr->AddChildren(oSz);

    CElement *oSzCs = new CElement(L"sz", L"", L"w");
    oSzCs->AddArgument(L"val", L"22");
    oRPr->AddChildren(oSzCs);

    CElement *oLang = new CElement(L"lang", L"", L"w");
    oLang->AddArgument(L"val", L"ru-RU");
    oLang->AddArgument(L"eastAsia", L"zh-TW");
    oLang->AddArgument(L"bidi", L"ar-SA");
    oRPr->AddChildren(oLang);

    oRPrDefault->AddChildren(oRPr);
    oDocDefaults->AddChildren(oRPrDefault);

    CElement *oPPrDefault = new CElement(L"pPrDefault", L"", L"w");
    CElement *oPPr = new CElement(L"pPr", L"", L"w");
    CElement *oSpacing = new CElement(L"spacing", L"", L"w");
    oSpacing->AddArgument(L"after", L"160");
    oSpacing->AddArgument(L"line", L"259");
    oSpacing->AddArgument(L"lineRule", L"auto");

    oPPr->AddChildren(oSpacing);
    oPPrDefault->AddChildren(oPPr);
    oDocDefaults->AddChildren(oPPrDefault);

    AddChildren(oDocDefaults);

    CStyle *oPStyle = new CStyle;
    oPStyle->SetDefoult();
    AddChildren(oPStyle);
}
