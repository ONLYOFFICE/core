#include "CSdt.h"

#include "../hyperlink/CHyperlink.h"
#include "../../../../DesktopEditor/common/File.h"

CSdt::CSdt()
    : CElement(L"sdt", L"", L"w")
{
}

void CSdt::SetDefoult()
{
    m_oSdtPr = new CSdtPr();
    m_oSdtPr->SetDefoult();
    AddChildren(m_oSdtPr);

    m_oSdtEndPr = new CSdtEndPr();
    m_oSdtEndPr->SetDefoult();
    AddChildren(m_oSdtEndPr);

    m_oSdtContent = new CSdtContent();
    m_oSdtContent->SetDefoult();
    AddChildren(m_oSdtContent);
}

void CSdt::AddHeader(std::pair<std::wstring, std::wstring> pTextAndRef, CDocumentXml *oWhereToAdd, CDocRelationshipsXml *oDocRel)
{
    nCountHyperlinks++;

    CElement *oParagraph = new CElement(L"p", L"", L"w");
    oParagraph->AddArgument(L"rsidR", L"005D3816");
    oParagraph->AddArgument(L"rsidRDefault", L"005D3816");
    oParagraph->AddArgument(L"rsidP", L"005D3816");

        CElement *oPPr = new CElement(L"pPr", L"", L"w");
            CElement *oPStyle = new CElement(L"pStyle", L"", L"w");
            oPStyle->AddArgument(L"val", L"21");
        oPPr->AddChildren(oPStyle);

            CElement *oTabs = new CElement(L"tabs", L"", L"w");
                CElement *oTab = new CElement(L"tab", L"", L"w");
                oTab->AddArgument(L"val", L"right");
                oTab->AddArgument(L"leader", L"dot");
                oTab->AddArgument(L"pos", L"9623");
            oTabs->AddChildren(oTab);
        oPPr->AddChildren(oTabs);

            CElement *oInd = new CElement(L"ind", L"", L"w");
            oInd->AddArgument(L"left", L"0");
        oPPr->AddChildren(oInd);

            CElement *oRPr = new CElement(L"rPr", L"", L"w");
                CElement *oNoProof = new CElement(L"noProof", L"", L"w");
            oRPr->AddChildren(oNoProof);
        oPPr->AddChildren(oRPr);
    oParagraph->AddChildren(oPPr);

        CElement *oRun1 = new CElement(L"r", L"", L"w");
            CElement *oFldChar = new CElement(L"fldChar", L"", L"w");
            oFldChar->AddArgument(L"fldCharType", L"begin");
        oRun1->AddChildren(oFldChar);
    oParagraph->AddChildren(oRun1);

        CElement *oRun2 = new CElement(L"r", L"", L"w");
            CElement *oInstrText = new CElement(L"instrText", L" TOC \\o \"1-3\" \\h \\z \\u ", L"w", false);
            oInstrText->AddArgument(L"xml:space", L"preserve");
        oRun2->AddChildren(oInstrText);
    oParagraph->AddChildren(oRun2);

        CElement *oRun3 = new CElement(L"r", L"", L"w");
            CElement *oFldChar2 = new CElement(L"fldChar", L"", L"w");
            oFldChar2->AddArgument(L"fldCharType", L"separate");
        oRun3->AddChildren(oFldChar2);
    oParagraph->AddChildren(oRun3);

        CHyperlink *oHyperlink = new CHyperlink;
        oHyperlink->CreateTocHyperlink(pTextAndRef.first);
    oParagraph->AddChildren(oHyperlink);

    m_oSdtContent->AddChildren(oParagraph);

    CParagraph *oTextParagraph = new CParagraph(pTextAndRef.first, true);
    oTextParagraph->AddHyperlinkToc(oHyperlink->GetId());
    oTextParagraph->SetInCenter();
    oWhereToAdd->AddParagraph(oTextParagraph);

//    CParagraph *oFileLink = new CParagraph();
//    oFileLink->AddLinkToFile(oDocRel, std::wstring(NSFile::GetProcessDirectory()+ L"\\tmp\\" + pTextAndRef.second));
//    oWhereToAdd->AddParagraph(oFileLink);
}
