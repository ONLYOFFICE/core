#include "CWebSettings.h"

CWebSettings::CWebSettings()
    : CElement(L"webSettings", L"", L"w", false)
{
}

void CWebSettings::SetDefoult()
{
    AddArgument(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
    AddArgument(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
    AddArgument(L"xmlns:w", L"http://schemas.openxmlformats.org/wordprocessingml/2006/main");
    AddArgument(L"xmlns:w14", L"http://schemas.microsoft.com/office/word/2010/wordml");
    AddArgument(L"xmlns:w15", L"http://schemas.microsoft.com/office/word/2012/wordml");
    AddArgument(L"xmlns:w16se", L"http://schemas.microsoft.com/office/word/2015/wordml/symex");
    AddArgument(L"mc:Ignorable", L"w14 w15 w16se");

    CElement *oOptimizeForBrowser = new CElement(L"optimizeForBrowser", L"", L"w");
    AddChildren(oOptimizeForBrowser);

    CElement *oAllowPNG = new CElement(L"allowPNG", L"", L"w");
    AddChildren(oAllowPNG);
}
