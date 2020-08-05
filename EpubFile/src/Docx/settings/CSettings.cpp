#include "CSettings.h"

CSettings::CSettings():
    CElement(L"settings", L"", L"w", false)
{

}

void CSettings::SetDefoult()
{

    AddArgument(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
    AddArgument(L"xmlns:o", L"urn:schemas-microsoft-com:office:office");
    AddArgument(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
    AddArgument(L"xmlns:m", L"http://schemas.openxmlformats.org/officeDocument/2006/math");
    AddArgument(L"xmlns:v", L"urn:schemas-microsoft-com:vml");
    AddArgument(L"xmlns:w10", L"urn:schemas-microsoft-com:office:word");
    AddArgument(L"xmlns:w", L"http://schemas.openxmlformats.org/wordprocessingml/2006/main");
    AddArgument(L"xmlns:w14", L"http://schemas.microsoft.com/office/word/2010/wordml");
    AddArgument(L"xmlns:w15", L"http://schemas.microsoft.com/office/word/2012/wordml");
    AddArgument(L"xmlns:w16se", L"http://schemas.microsoft.com/office/word/2015/wordml/symex");
    AddArgument(L"xmlns:sl", L"http://schemas.openxmlformats.org/schemaLibrary/2006/main");
    AddArgument(L"mc:Ignorable", L"w14 w15 w16se");

    CElement *oZoom = new CElement(L"zoom", L"", L"w");
    oZoom->AddArgument(L"percent", L"100");
    AddChildren(oZoom);

    CElement *oProofState = new CElement(L"proofState", L"", L"w");
    oProofState->AddArgument(L"grammar", L"clean");
    AddChildren(oProofState);

    CElement *oDefaultTabStop = new CElement(L"defaultTabStop", L"", L"w");
    oDefaultTabStop->AddArgument(L"val", L"708");
    AddChildren(oDefaultTabStop);

    CElement *oDrawingGridHorizontalSpacing = new CElement(L"drawingGridHorizontalSpacing", L"", L"w");
    oDrawingGridHorizontalSpacing->AddArgument(L"val", L"110");
    AddChildren(oDrawingGridHorizontalSpacing);

    CElement *oDrawingGridVerticalSpacing = new CElement(L"drawingGridVerticalSpacing", L"", L"w");
    oDrawingGridVerticalSpacing->AddArgument(L"val", L"299");
    AddChildren(oDrawingGridVerticalSpacing);

    CElement *oDisplayHorizontalDrawingGridEvery = new CElement(L"displayHorizontalDrawingGridEvery", L"", L"w");
    oDisplayHorizontalDrawingGridEvery->AddArgument(L"val", L"2");
    AddChildren(oDisplayHorizontalDrawingGridEvery);

    CElement *oCharacterSpacingControl = new CElement(L"characterSpacingControl", L"", L"w");
    oCharacterSpacingControl->AddArgument(L"val", L"doNotCompress");
    AddChildren(oCharacterSpacingControl);

    CElement *oCompat = new CElement(L"compat", L"", L"w");

        CElement *oUseFELayout = new CElement(L"useFELayout", L"", L"w");
        oCompat->AddChildren(oUseFELayout);

        CElement *oCompatSetting1 = new CElement(L"compatSetting", L"", L"w");
        oCompatSetting1->AddArgument(L"name", L"compatibilityMode");
        oCompatSetting1->AddArgument(L"uri", L"http://schemas.microsoft.com/office/word");
        oCompatSetting1->AddArgument(L"val", L"15");
        oCompat->AddChildren(oCompatSetting1);

        CElement *oCompatSetting2 = new CElement(L"compatSetting", L"", L"w");
        oCompatSetting2->AddArgument(L"name", L"overrideTableStyleFontSizeAndJustification");
        oCompatSetting2->AddArgument(L"uri", L"http://schemas.microsoft.com/office/word");
        oCompatSetting2->AddArgument(L"val", L"1");
        oCompat->AddChildren(oCompatSetting2);

        CElement *oCompatSetting3 = new CElement(L"compatSetting", L"", L"w");
        oCompatSetting3->AddArgument(L"name", L"enableOpenTypeFeatures");
        oCompatSetting3->AddArgument(L"uri", L"http://schemas.microsoft.com/office/word");
        oCompatSetting3->AddArgument(L"val", L"1");
        oCompat->AddChildren(oCompatSetting3);

        CElement *oCompatSetting4 = new CElement(L"compatSetting", L"", L"w");
        oCompatSetting4->AddArgument(L"name", L"doNotFlipMirrorIndents");
        oCompatSetting4->AddArgument(L"uri", L"http://schemas.microsoft.com/office/word");
        oCompatSetting4->AddArgument(L"val", L"1");
        oCompat->AddChildren(oCompatSetting4);

        CElement *oCompatSetting5 = new CElement(L"compatSetting", L"", L"w");
        oCompatSetting5->AddArgument(L"name", L"differentiateMultirowTableHeaders");
        oCompatSetting5->AddArgument(L"uri", L"http://schemas.microsoft.com/office/word");
        oCompatSetting5->AddArgument(L"val", L"1");
        oCompat->AddChildren(oCompatSetting5);

    AddChildren(oCompat);

    CElement *oRsids = new CElement(L"rsids", L"", L"w");
        CElement *oRsidRoot = new CElement(L"rsidRoot", L"", L"w");
        oRsidRoot->AddArgument(L"val", L"006D416B");
        oRsids->AddChildren(oRsidRoot);

        CElement *oRsid = new CElement(L"rsid", L"", L"w");
        oRsid->AddArgument(L"val", L"0010077D");
        oRsids->AddChildren(oRsid);

        CElement *oRsid2 = new CElement(L"rsid", L"", L"w");
        oRsid2->AddArgument(L"val", L"00215247");
        oRsids->AddChildren(oRsid2);

        CElement *oRsid3 = new CElement(L"rsid", L"", L"w");
        oRsid3->AddArgument(L"val", L"00270044");
        oRsids->AddChildren(oRsid3);

        CElement *oRsid4 = new CElement(L"rsid", L"", L"w");
        oRsid4->AddArgument(L"val", L"002968DE");
        oRsids->AddChildren(oRsid4);

        CElement *oRsid5 = new CElement(L"rsid", L"", L"w");
        oRsid5->AddArgument(L"val", L"006D416B");
        oRsids->AddChildren(oRsid5);

        CElement *oRsid6 = new CElement(L"rsid", L"", L"w");
        oRsid6->AddArgument(L"val", L"00F308A4");
        oRsids->AddChildren(oRsid6);

        CElement *oRsid7 = new CElement(L"rsid", L"", L"w");
        oRsid7->AddArgument(L"val", L"00FA63D2");
        oRsids->AddChildren(oRsid7);

    AddChildren(oRsids);

    CElement *oMathPr = new CElement(L"mathPr", L"", L"m");
        CElement *oMathFont = new CElement(L"mathFont", L"", L"m");
        oMathFont->AddArgument(L"val", L"Cambria Math");
        oMathPr->AddChildren(oMathFont);

        CElement *oBrkBin = new CElement(L"brkBin", L"", L"m");
        oBrkBin->AddArgument(L"val", L"before");
        oMathPr->AddChildren(oBrkBin);

        CElement *oBrkBinSub = new CElement(L"brkBinSub", L"", L"m");
        oBrkBinSub->AddArgument(L"val", L"--");
        oMathPr->AddChildren(oBrkBinSub);

        CElement *oSmallFrac = new CElement(L"smallFrac", L"", L"m");
        oSmallFrac->AddArgument(L"val", L"0");
        oMathPr->AddChildren(oSmallFrac);

        CElement *oDispDef = new CElement(L"dispDef", L"", L"m");
        oMathPr->AddChildren(oDispDef);

        CElement *oLMargin = new CElement(L"lMargin", L"", L"m");
        oLMargin->AddArgument(L"val", L"0");
        oMathPr->AddChildren(oLMargin);

        CElement *oRMargin = new CElement(L"rMargin", L"", L"m");
        oRMargin->AddArgument(L"val", L"0");
        oMathPr->AddChildren(oRMargin);

        CElement *oDefJc = new CElement(L"defJc", L"", L"m");
        oDefJc->AddArgument(L"val", L"centerGroup");
        oMathPr->AddChildren(oDefJc);

        CElement *oWrapIndent = new CElement(L"wrapIndent", L"", L"m");
        oWrapIndent->AddArgument(L"val", L"1440");
        oMathPr->AddChildren(oWrapIndent);

        CElement *oIntLim = new CElement(L"intLim", L"", L"m");
        oIntLim->AddArgument(L"val", L"subSup");
        oMathPr->AddChildren(oIntLim);

        CElement *oNaryLim = new CElement(L"naryLim", L"", L"m");
        oNaryLim->AddArgument(L"val", L"undOvr");
        oMathPr->AddChildren(oNaryLim);

    AddChildren(oMathPr);

    CElement *oThemeFontLang = new CElement(L"themeFontLang", L"", L"w");
    oThemeFontLang->AddArgument(L"val", L"ru-RU");
    oThemeFontLang->AddArgument(L"eastAsia", L"zh-TW");
    AddChildren(oThemeFontLang);

    CElement *oClrSchemeMapping = new CElement(L"clrSchemeMapping", L"", L"w");
    oClrSchemeMapping->AddArgument(L"bg1", L"light1");
    oClrSchemeMapping->AddArgument(L"t1", L"dark1");
    oClrSchemeMapping->AddArgument(L"bg2", L"light2");
    oClrSchemeMapping->AddArgument(L"t2", L"dark2");
    oClrSchemeMapping->AddArgument(L"accent1", L"accent1");
    oClrSchemeMapping->AddArgument(L"accent2", L"accent2");
    oClrSchemeMapping->AddArgument(L"accent3", L"accent3");
    oClrSchemeMapping->AddArgument(L"accent4", L"accent4");
    oClrSchemeMapping->AddArgument(L"accent5", L"accent5");
    oClrSchemeMapping->AddArgument(L"accent6", L"accent6");
    oClrSchemeMapping->AddArgument(L"hyperlink", L"hyperlink");
    oClrSchemeMapping->AddArgument(L"followedHyperlink", L"followedHyperlink");
    AddChildren(oClrSchemeMapping);

    CElement *oShapeDefaults = new CElement(L"shapeDefaults", L"", L"w");
        CElement *oShapedefaults = new CElement(L"shapedefaults", L"", L"o", false);
            oShapedefaults->AddArgument(L"v:ext", L"edit");
            oShapedefaults->AddArgument(L"spidmax", L"1026");
            oShapeDefaults->AddChildren(oShapedefaults);
        CElement *oShapelayout = new CElement(L"shapelayout", L"", L"o", false);
            oShapelayout->AddArgument(L"v:ext", L"edit");
            CElement *oIdmap = new CElement(L"idmap", L"", L"o", false);
            oIdmap->AddArgument(L"v:ext", L"edit");
            oIdmap->AddArgument(L"data", L"1");
        oShapelayout->AddChildren(oIdmap);
    oShapeDefaults->AddChildren(oShapelayout);
    AddChildren(oShapeDefaults);

    CElement *oDecimalSymbol = new CElement(L"decimalSymbol", L"", L"w");
    oDecimalSymbol->AddArgument(L"val", L",");
    AddChildren(oDecimalSymbol);

    CElement *oListSeparator = new CElement(L"listSeparator", L"", L"w");
    oListSeparator->AddArgument(L"val", L";");
    AddChildren(oListSeparator);

    CElement *oDocIdw14 = new CElement(L"docId", L"", L"w14");
    oDocIdw14->AddArgument(L"val", L"589EF73F");
    AddChildren(oDocIdw14);

    CElement *oChartTrackingRefBased = new CElement(L"chartTrackingRefBased", L"", L"w15");
    AddChildren(oChartTrackingRefBased);

    CElement *oDocId = new CElement(L"docId", L"", L"w15");
    oDocId->AddArgument(L"val", L"{B9F106EB-4BF7-4706-BF25-D03C3B53E500}");
    AddChildren(oDocId);
}

