#include "TxBodyConverter.h"
#include "../../../Common/MS-LCID.h"

using namespace PPT_FORMAT;

TxBodyConverter::TxBodyConverter(CTextAttributesEx *pText, TxBodyConverter::eTxType txType) :
    m_pText(pText), m_txType(txType)
{

}

void TxBodyConverter::FillTxBody(PPTX::Logic::TxBody &oTxBody)
{
    if (m_pText == nullptr)
        return;
    switch (m_txType)
    {
    case shape: ConvertShapeTxBody(oTxBody); break;
    case table: ConvertTableTxBody(oTxBody); break;
    }

}

void TxBodyConverter::ConvertTableTxBody(PPTX::Logic::TxBody &oTxBody)
{
    oTxBody.bodyPr = new PPTX::Logic::BodyPr;
    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;
    oTxBody.m_name = L"a:txBody";

    FillParagraphs(oTxBody.Paragrs, m_pText->m_arParagraphs);

}

void TxBodyConverter::ConvertShapeTxBody(PPTX::Logic::TxBody &oTxBody)
{

}

void TxBodyConverter::FillParagraphs(std::vector<PPTX::Logic::Paragraph> &arrP, std::vector<CParagraph> &arrParagraphs)
{
    if (m_pText == nullptr)
    {
        PPTX::Logic::Paragraph p;
        p.endParaRPr = new PPTX::Logic::RunProperties;
        p.endParaRPr->m_name = L"a:endParaRPr";
        p.endParaRPr->lang = L"en-US";

        arrP.push_back(p);
        return;
    }

    for (auto& paragraph : arrParagraphs)
    {
        PPTX::Logic::Paragraph p;
        FillParagraph(p, paragraph);
        arrP.push_back(p);
    }
}

void TxBodyConverter::FillParagraph(PPTX::Logic::Paragraph &p, CParagraph &paragraph)
{
    for (auto& span : paragraph.m_arSpans)
    {
        PPTX::Logic::RunElem runElem;
        auto pRun = new PPTX::Logic::Run();
        FillRun(*pRun, span);
        runElem.InitRun(pRun);

        p.RunElems.push_back(runElem);
    }

    p.pPr = new PPTX::Logic::TextParagraphPr;
    FillPPr(p.pPr.get2(), paragraph.m_oPFRun);

    p.endParaRPr = new PPTX::Logic::RunProperties;
    FillEndParaRPr(p.endParaRPr.get2(), paragraph.m_oPFRun);
}

void TxBodyConverter::FillRun(PPTX::Logic::Run &oRun, CSpan& oSpan)
{
    oRun.SetText(oSpan.m_strText);

    oRun.rPr = new PPTX::Logic::RunProperties;
    FillRPr(oRun.rPr.get2(), oSpan.m_oRun);
}

void TxBodyConverter::FillEndParaRPr(PPTX::Logic::RunProperties &oEndPr, CTextPFRun &oPFRun)
{
    oEndPr.m_name = L"a:endParaRPr";
    oEndPr.lang = L"en-US";
    oEndPr.dirty = true;

//    if ((oPFRun.Size.is_init()) && (oPFRun.Size.get() > 0) && (oCFRun.Size.get() < 4001))
//    {
//        oEndPr.sz = int(100 * oPFRun.Size.get());
//    } else
//    {
//        oEndPr.sz = 18;
//    }
//    if (oCFRun.BaseLineOffset.is_init())
//    {
//        oEndPr.baseline = int(1000 * oCFRun.BaseLineOffset.get());
//    }
//    if (oCFRun.FontBold.is_init())
//    {
//        oEndPr.b = oCFRun.FontBold.get();
//    }
//    if (oCFRun.FontItalic.is_init())
//    {
//        oEndPr.i = oCFRun.FontItalic.get();
//    }
//    if (oCFRun.FontUnderline.is_init())
//    {
//        oEndPr.u = new PPTX::Limit::TextUnderline;
//        oEndPr.u->set(oCFRun.FontUnderline.get() ? L"sng" : L"none");
//    }
//    if (oCFRun.Language.is_init())
//    {
//        std::wstring str_lang = msLCID2wstring(oCFRun.Language.get());

//        if (str_lang.length() > 0)
//            oEndPr.lang = msLCID2wstring(oCFRun.Language.get());
    //    }
}

void TxBodyConverter::FillPPr(PPTX::Logic::TextParagraphPr &oPPr, CTextPFRun &oPFRun)
{
    int leftMargin = 0;
    if (oPFRun.leftMargin.is_init())
    {
        leftMargin = oPFRun.leftMargin.get();
        oPPr.marL = leftMargin;
        oPPr.indent = -leftMargin;
    }

    if (oPFRun.indent.is_init())
        oPPr.indent = oPFRun.indent.get() - leftMargin;

    if (oPFRun.bulletFontProperties.is_init())
    {
        auto pBuFont = new PPTX::Logic::TextFont;
        pBuFont->typeface = oPFRun.bulletFontProperties->Name;

        if ( oPFRun.bulletFontProperties->PitchFamily > 0)
        {
            pBuFont->pitchFamily = std::to_wstring(oPFRun.bulletFontProperties->PitchFamily);
        }
        if ( oPFRun.bulletFontProperties->Charset > 0)
        {
            pBuFont->charset = std::to_wstring((char)oPFRun.bulletFontProperties->Charset);
        }
        oPPr.buTypeface.SetParentPointer(pBuFont);
    }


    if (oPFRun.bulletAutoNum.is_init())
    {
        auto pBuAutoNum = new PPTX::Logic::BuAutoNum;
        oPPr.ParagraphBullet.m_Bullet.reset(pBuAutoNum);
        if (oPFRun.bulletAutoNum->startAt.is_init() && oPFRun.bulletAutoNum->startAt.get() != 1)
            pBuAutoNum->startAt = oPFRun.bulletAutoNum->startAt.get();
        if (oPFRun.bulletAutoNum->type.is_init())
            pBuAutoNum->type = oPFRun.bulletAutoNum->type.get();
    }
}

void TxBodyConverter::FillRPr(PPTX::Logic::RunProperties &oRPr, CTextCFRun &oCFRun)
{
    if ((oCFRun.Size.is_init()) && (oCFRun.Size.get() > 0) && (oCFRun.Size.get() < 4001))
    {
        oRPr.sz = int(100 * oCFRun.Size.get());
    } else
    {
        oRPr.sz = 1800;
    }
    if (oCFRun.BaseLineOffset.is_init())
    {
        oRPr.baseline = int(1000 * oCFRun.BaseLineOffset.get());
    }
    if (oCFRun.FontBold.is_init())
    {
        oRPr.b = oCFRun.FontBold.get();
    }
    else
    {
        oRPr.b = false;
    }
    if (oCFRun.FontItalic.is_init())
    {
        oRPr.i = oCFRun.FontItalic.get();
    }
    if (oCFRun.FontUnderline.is_init())
    {
        oRPr.u = new PPTX::Limit::TextUnderline;
        oRPr.u->set(oCFRun.FontUnderline.get() ? L"sng" : L"none");
    }
    if (oCFRun.Language.is_init())
    {
        std::wstring str_lang = msLCID2wstring(oCFRun.Language.get());

        if (str_lang.length() > 0)
            oRPr.lang = msLCID2wstring(oCFRun.Language.get());
    }
}
