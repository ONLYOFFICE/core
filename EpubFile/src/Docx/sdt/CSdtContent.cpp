#include "CSdtContent.h"

CSdtContent::CSdtContent()
    : CElement(L"sdtContent", L"", L"w")
{

}

void CSdtContent::SetDefoult()
{
    CElement *oParagraph = new CElement(L"p", L"", L"w");
    oParagraph->AddArgument(L"rsidR", L"005D3816");
    oParagraph->AddArgument(L"rsidRDefault", L"005D3816");

        CElement *oPPr = new CElement(L"pPr", L"", L"w");
            CElement *oPStyle = new CElement(L"pStyle", L"", L"w");
            oPStyle->AddArgument(L"val", L"a3");

            CElement *oJc = new CElement(L"jc", L"", L"w");
            oJc->AddArgument(L"val", L"center");

        oPPr->AddChildren(oJc);
        oPPr->AddChildren(oPStyle);

    oParagraph->AddChildren(oPPr);

        CElement *oRun = new CElement(L"r", L"", L"w");
            CElement *oText = new CElement(L"t", L"Table of content", L"w");

        oRun->AddChildren(oText);

    oParagraph->AddChildren(oRun);

    AddChildren(oParagraph);
}
