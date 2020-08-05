#include "CParagraph.h"
#include "CParagraphProperties.h"
#include "CRun.h"
#include "../hyperlink/CHyperlink.h"
#include <stack>
#include <string>

CParagraph::CParagraph()
    : CElement(L"p", L"", L"w")
{
}

CParagraph::CParagraph(std::wstring sText, bool bNewPage)
    : CElement(L"p", L"", L"w")
{
    SetDefoult();
    SetText(sText, bNewPage);

    if (bNewPage)
        SetTagNewPage();
}

void CParagraph::SetText(std::wstring sText, bool bNewPage)
{
    std::stack <CElement*> stElements;
    for (int i = 0; i < GetCountChildrens(); i++)
        stElements.push(GetChildren(i));

    while (!stElements.empty())
    {
        CElement* oElement = stElements.top();
        stElements.pop();
        if (oElement->GetName() == L"t")
        {
            oElement->setValue(sText);
            return;
        }
        for (int i = 0; i < oElement->GetCountChildrens(); i++)
            stElements.push(oElement->GetChildren(i));
    }
}

void CParagraph::AddText(std::wstring sText, bool bNewPage)
{
    while (true)
    {
        for (int i = 0; i < GetCountChildrens(); i++)
        {
            CElement oElement = *GetChildren(i);
            if (oElement.GetName() == L"t")
            {
                oElement.setValue(oElement.GetValue() + sText);
                return;
            }
        }
    }
}

void CParagraph::SetDefoult()
{
    AddArgument(L"rsidR", L"003939F5");
    AddArgument(L"rsidRDefault", L"00481163");

    CElement *oBookmarkStart = new CElement(L"bookmarkStart", L"", L"w");
    oBookmarkStart->AddArgument(L"id", L"0");
    oBookmarkStart->AddArgument(L"name", L"_GoBack");
    AddChildren(oBookmarkStart);

    CElement *oBookmarkEnd = new CElement(L"bookmarkEnd", L"", L"w");
    oBookmarkEnd->AddArgument(L"id", L"0");
    AddChildren(oBookmarkEnd);

    CRun *oRun = new CRun;
    oRun->SetDefoult();
    AddChildren(oRun);
}

void CParagraph::SetTagNewPage()
{
    CRun *oRun = new CRun;
        CElement *oBr = new CElement(L"br", L"", L"w");
        oBr->AddArgument(L"type", L"page");
    oRun->AddChildren(oBr);
    AddChildren(oRun, 0);

}

void CParagraph::AddHyperlinkToc(int nIdToc)
{
    for (int i = 0; i < GetCountChildrens(); i++)
    {
        CElement *oElement = GetChildren(i);
        if (oElement->GetName() == L"bookmarkStart")
        {
            oElement->DeleteArgument(L"name");
            oElement->AddArgument(L"name", L"_Toc" + std::to_wstring(nIdToc));
        }
    }
}

void CParagraph::AddLinkToFile(CDocRelationshipsXml *oDocRel, std::wstring sPathFile)
{
    CHyperlink *oHyperlink = new CHyperlink;
    oHyperlink->CreateFileLink(sPathFile);
    if (!IsEmpty())
        Clear();

    AddArgument(L"rsidR", L"00EF05E0");
    AddArgument(L"rsidRDefault", L"006D386F");
    AddArgument(L"rsidP", L"00670FDF");

    AddChildren(oHyperlink);

    oDocRel->AddLinkToFile(sPathFile, oHyperlink->GetId());
}

void CParagraph::SetInCenter()
{
    for (int i = 0; i < GetCountChildrens(); i++)
    {
        CElement *oElement = GetChildren(i);
        if (oElement->GetName() == L"pPr")
        {
            CElement *oJc = new CElement(L"jc", L"", L"w");
            oJc->AddArgument(L"val", L"center");
            oElement->AddChildren(oJc);
            return;
        }
    }
    CElement *oPPr = new CElement(L"pPr", L"", L"w");
        CElement *oJc = new CElement(L"jc", L"", L"w");
        oJc->AddArgument(L"val", L"center");
    oPPr->AddChildren(oJc, 0);
    AddChildren(oPPr, 0);
}
