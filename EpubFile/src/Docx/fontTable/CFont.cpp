#include "CFont.h"

CFont::CFont()
    : CElement(L"font", L"", L"w")
{

}

void CFont::SetDefoult()
{
    AddArgument(L"name", L"Calibri");

    CElement *oPanose1 = new CElement(L"panose1", L"", L"w");
    oPanose1->AddArgument(L"val", L"020F0502020204030204");
    AddChildren(oPanose1);

    CElement *oCharset = new CElement(L"charset", L"", L"w");
    oCharset->AddArgument(L"val", L"CC");
    AddChildren(oCharset);

    CElement *oFamily = new CElement(L"family", L"", L"w");
    oFamily->AddArgument(L"val", L"swiss");
    AddChildren(oFamily);

    CElement *oPitch = new CElement(L"pitch", L"", L"w");
    oPitch->AddArgument(L"val", L"variable");
    AddChildren(oPitch);

    CElement *oSig = new CElement(L"sig", L"", L"w");
    oSig->AddArgument(L"usb0", L"E4002EFF");
    oSig->AddArgument(L"usb1", L"C000247B");
    oSig->AddArgument(L"usb2", L"00000009");
    oSig->AddArgument(L"usb3", L"00000000");
    oSig->AddArgument(L"csb0", L"000001FF");
    oSig->AddArgument(L"csb1", L"00000000");
    AddChildren(oSig);

}
