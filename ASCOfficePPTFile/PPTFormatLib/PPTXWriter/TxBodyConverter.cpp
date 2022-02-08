#include "TxBodyConverter.h"
#include "../../../Common/MS-LCID.h"
#include "../../../ASCOfficeXlsFile2/source/XlsXlsxConverter/ShapeType.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h"
#include "StylesWriter.h"

using namespace PPT_FORMAT;

TxBodyConverter::TxBodyConverter(CShapeElement *pShapeElement, CRelsGenerator* pRels, TxBodyConverter::eTxType txType) :
    m_pShapeElement(pShapeElement), m_pRels(pRels), m_txType(txType),
    m_bWordArt(false)
{
    if (pShapeElement && pShapeElement->m_pShape)
        pText = &pShapeElement->m_pShape->m_oText;
    else
        pText = nullptr;
}

void TxBodyConverter::FillTxBody(PPTX::Logic::TxBody &oTxBody)
{
    if (m_pShapeElement == nullptr)
    {
        FillMergedTxBody(oTxBody);
        return;
    }

    switch (m_txType)
    {
    case shape: ConvertShapeTxBody(oTxBody); break;
    case table: ConvertTableTxBody(oTxBody); break;
    }
}

void TxBodyConverter::ConvertTableTxBody(PPTX::Logic::TxBody &oTxBody)
{
    oTxBody.m_name = L"a:txBody";


    oTxBody.bodyPr = new PPTX::Logic::BodyPr;
    FillBodyPr(oTxBody.bodyPr.get2());


    size_t nCount = pText->m_arParagraphs.size();
    if (nCount == 0)
    {
        oTxBody.lstStyle = new PPTX::Logic::TextListStyle;
        PPTX::Logic::Paragraph paragraph;
        unsigned sz = m_pShapeElement->m_rcChildAnchor.GetHeight() * 10;
        paragraph.endParaRPr = getNewEndParaRPr(false, sz > 2000 ? 2000 : sz);
        oTxBody.Paragrs.push_back(paragraph);

        return;
    }


    if (pText == nullptr)
        return;


    // todo
    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;
    if (!m_bWordArt)
        FillLstStyles(oTxBody.lstStyle.get2(), pText->m_oStyles);


    for (size_t nIndexPar = 0; nIndexPar < nCount; ++nIndexPar)
    {
        PPTX::Logic::Paragraph p;
        FillParagraph(p,pText->m_arParagraphs[nIndexPar]);
        oTxBody.Paragrs.push_back(std::move(p));
    }
    //    if (m_pText)
    //    {
    //        FillLstStyles(oTxBody.lstStyle.get2(), m_pText->m_oStyles);
    //        FillParagraphs(oTxBody.Paragrs, m_pText->m_arParagraphs);
    //    }
}

void TxBodyConverter::ConvertShapeTxBody(PPTX::Logic::TxBody &oTxBody)
{

}

void TxBodyConverter::FillMergedTxBody(PPTX::Logic::TxBody &oTxBody)
{
    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;
    oTxBody.bodyPr = new PPTX::Logic::BodyPr;
    oTxBody.lstStyle = new PPTX::Logic::TextListStyle;

    PPTX::Logic::Paragraph paragraph;
    paragraph.endParaRPr = getNewEndParaRPr();
    oTxBody.Paragrs.push_back(paragraph);
}

void TxBodyConverter::FillBodyPr(PPTX::Logic::BodyPr &oBodyPr)
{
    CDoubleRect oTextRect;
    m_pShapeElement->m_pShape->GetTextRect(oTextRect);

    oBodyPr.lIns = oTextRect.left;
    oBodyPr.rIns = oTextRect.right;
    oBodyPr.tIns = oTextRect.top;
    oBodyPr.bIns = oTextRect.bottom;


    auto pAnchor = new PPTX::Limit::TextAnchor;
    switch (pText->m_oAttributes.m_nTextAlignVertical)
    {
    case 0: pAnchor->set(L"t"); break;
    case 2: pAnchor->set(L"b"); break;
    default:
        pAnchor->set(L"ctr");
        oBodyPr.anchorCtr = false;
    }
    oBodyPr.anchor = pAnchor;


    if (m_pShapeElement->m_pShape->m_oText.m_bAutoFit)
        oBodyPr.Fit.type = PPTX::Logic::TextFit::FitSpAuto;


    if (pText->m_oAttributes.m_dTextRotate > 0)
        oBodyPr.rot = round(pText->m_oAttributes.m_dTextRotate * 60000);


    if (pText->m_nTextFlow >= 0)
    {
        auto *pVert = new PPTX::Limit::TextVerticalType;
        switch(pText->m_nTextFlow)
        {
        case 1:
        case 3: pVert->set(L"vert"); break;
        case 2: pVert->set(L"vert270"); break;
        case 5: pVert->set(L"wordArtVert");	break;
        }
        oBodyPr.vert = pVert;
    }
    else if (pText->m_bVertical)
    {
        auto *pVert = new PPTX::Limit::TextVerticalType;
        pVert->set(L"eaVert");
        oBodyPr.vert = pVert;
    }


    std::wstring prstGeom	= oox::Spt2ShapeType_mini((oox::MSOSPT)m_pShapeElement->m_lShapeType);
    std::wstring prstTxWarp = oox::Spt2WordArtShapeType((oox::MSOSPT)m_pShapeElement->m_lShapeType);
    if (prstGeom.size() && prstTxWarp.size())
    {
        m_bWordArt = true;


        auto pPrstTxWarp = new PPTX::Logic::PrstTxWarp;
        pPrstTxWarp->prst.set(prstTxWarp);


        CPPTShape *pPPTShape = dynamic_cast<CPPTShape *>(m_pShapeElement->m_pShape->getBaseShape().get());
        std::wstring strVal;

        for (size_t i = 0 ; (pPPTShape) && (i < pPPTShape->m_arAdjustments.size()); i++)
        {
            PPTX::Logic::Gd gd;
            switch(m_pShapeElement->m_lShapeType)
            {
            case oox::msosptTextFadeUp:
            {
                double kf = 4.63; //"волшебный"
                std::wstring strVal = std::to_wstring(round(kf * pPPTShape->m_arAdjustments[i]));

                gd.fmla = L"val";
                gd.name = L"adj";
                pPrstTxWarp->avLst.push_back(std::move(gd));
            }break;
            }

        }

        oBodyPr.prstTxWarp = pPrstTxWarp;
    }
}

void TxBodyConverter::FillLstStyles(PPTX::Logic::TextListStyle &oTLS, CTextStyles &oStyles)
{

}

void TxBodyConverter::FillParagraph(PPTX::Logic::Paragraph &p, CParagraph &paragraph)
{
    p.pPr = new PPTX::Logic::TextParagraphPr;
    FillPPr(p.pPr.get2(), paragraph, m_pRels);

    size_t nCountSpans = paragraph.m_arSpans.size();
    for (size_t nSpan = 0; nSpan < nCountSpans; ++nSpan)
    {
        if ((nSpan == (nCountSpans - 1)) && (_T("\n") == paragraph.m_arSpans[nSpan].m_strText || paragraph.m_arSpans[nSpan].m_strText.empty()) )
        {
            PPT_FORMAT::CTextCFRun* pCF = &paragraph.m_arSpans[nSpan].m_oRun;
            if ((pCF->Size.is_init()) && (pCF->Size.get() > 0) && (pCF->Size.get() < 4001))
                p.endParaRPr = getNewEndParaRPr(-1, (100 * pCF->Size.get()), L"en-US");
            else
                p.endParaRPr = getNewEndParaRPr(-1, -1, L"en-US");

            continue;
        }


        int span_sz = paragraph.m_arSpans[nSpan].m_strText.length() ;
        if	((span_sz==1 && ( paragraph.m_arSpans[nSpan].m_strText[0] == (wchar_t)13 )) ||
             ((span_sz==2 && ( paragraph.m_arSpans[nSpan].m_strText[0] == (wchar_t)13 ) &&
               ( paragraph.m_arSpans[nSpan].m_strText[1] == (wchar_t)13 ))))
            continue;


        PPTX::Logic::RunElem runElem;
        if (paragraph.m_arSpans[nSpan].m_bBreak)
        {
            auto pBr = new PPTX::Logic::Br;
            pBr->rPr = new PPTX::Logic::RunProperties;
            FillRPr(pBr->rPr.get2(), paragraph.m_arSpans[nSpan].m_oRun);
            runElem.InitRun(pBr);
        } else
        {
            auto pRun = new PPTX::Logic::Run();
            FillRun(*pRun, paragraph.m_arSpans[nSpan]);
            runElem.InitRun(pRun);

            auto pFld = new PPTX::Logic::Fld;
            if (m_pShapeElement->m_lPlaceholderType == PT_MasterSlideNumber/* && paragraph.m_arSpans[nSpan].m_bField*/)
            {
                pFld->id = L"{D038279B-FC19-497E-A7D1-5ADD9CAF016F}";
                pFld->type = L"slidenum";
            } else if (m_pShapeElement->m_lPlaceholderType == PT_MasterDate && m_pShapeElement->m_nFormatDate == 1/* && pParagraph->m_arSpans[nSpan].m_bField*/)
            {
                pFld->id = L"{D7E01130-044F-4930-9A27-C729C70D8524}";
                pFld->type = L"datetime1";
            } else {
                delete pFld;
                pFld = nullptr;
            }
            if (pFld)
            {
                PPTX::Logic::RunElem runElemFld;
                runElemFld.InitRun(pFld);
                p.RunElems.push_back(std::move(runElemFld));
            }
        }
        p.RunElems.push_back(std::move(runElem));
    }
}

void TxBodyConverter::FillPPr(PPTX::Logic::TextParagraphPr &oPPr, CParagraph &paragraph, CRelsGenerator *pRels)
{
    oPPr.lvl = paragraph.m_lTextLevel;


    auto* pPF = &(paragraph.m_oPFRun);
    if (pPF)
        ConvertPFRun(oPPr, pPF, pRels);

}

void TxBodyConverter::ConvertPFRun(PPTX::Logic::TextParagraphPr &oPPr, CTextPFRun *pPF, CRelsGenerator* pRels)
{
    int leftMargin = 0;
    if (pPF->leftMargin.is_init())
    {
        leftMargin = pPF->leftMargin.get();
        oPPr.marL = leftMargin;
    }
    if (pPF->indent.is_init())
    {
        if (pPF->hasBullet.get_value_or(false))
            oPPr.indent = pPF->indent.get();
        else
            oPPr.indent = pPF->indent.get() - leftMargin;
    }
    if (pPF->textAlignment.is_init())
    {
        oPPr.algn = new PPTX::Limit::TextAlign;
        oPPr.algn->set(CStylesWriter::GetTextAlign(pPF->textAlignment.get()));
    }
    if (pPF->defaultTabSize.is_init())
    {
        oPPr.defTabSz = pPF->defaultTabSize.get();
    }
    if (pPF->textDirection.is_init())
    {
        if (pPF->textDirection.get() == 1)	oPPr.rtl = true;
        else								oPPr.rtl = false;
    }
    if (pPF->fontAlign.is_init())
    {
        oPPr.fontAlgn = new PPTX::Limit::FontAlign;
        oPPr.fontAlgn->set(CStylesWriter::GetFontAlign(pPF->fontAlign.get()));
    }


    ConvertTabStops(oPPr.tabLst, pPF->tabStops);


    if (pPF->lineSpacing.is_init())
    {
        LONG val = pPF->lineSpacing.get();
        auto pLnSpc = new PPTX::Logic::TextSpacing;
        pLnSpc->m_name = L"a:lnSpc";

        if (val > 0)
            pLnSpc->spcPct = val * 12.5;
        else if (val < 0 && val > -13200)
            pLnSpc->spcPct = val * -1000;

        oPPr.lnSpc = pLnSpc;
    }

    if (pPF->spaceAfter.is_init())
    {
        LONG val = pPF->spaceAfter.get();
        auto pSpcAft = new PPTX::Logic::TextSpacing;
        pSpcAft->m_name = L"a:spcAft";
        if (val > 0)
            pSpcAft->spcPts = round(12.5 * pPF->spaceAfter.get());
        else if (val < 0 && val > -13200)
            pSpcAft->spcPts = val * -1000;

        oPPr.spcAft = pSpcAft;
    }
    if (pPF->spaceBefore.is_init())
    {
        LONG val = pPF->spaceBefore.get();
        auto pSpcBef = new PPTX::Logic::TextSpacing;
        pSpcBef->m_name = L"a:spcBef";
        if (val > 0)
            pSpcBef->spcPts = round(12.5 * pPF->spaceBefore.get());
        else if (val < 0 && val > -13200)
            pSpcBef->spcPct = val * -1000;

        oPPr.spcBef = pSpcBef;
    }

    ConvertAllBullets(oPPr, pPF, pRels);
}

void TxBodyConverter::ConvertTabStops(std::vector<PPTX::Logic::Tab> &arrTabs, std::vector<std::pair<int, int> > &arrTabStops)
{
    for (size_t t = 0 ; t < arrTabStops.size(); t++)
    {
        PPTX::Logic::Tab tab;
        tab.pos = arrTabStops[t].first;
        auto pAlgn = new PPTX::Limit::TextTabAlignType;

        switch (arrTabStops[t].second)
        {
        case 1: pAlgn->set(L"ctr"); break;
        case 2: pAlgn->set(L"r"); break;
        case 3: pAlgn->set(L"dec"); break;
        default: pAlgn->set(L"l");
        }
        tab.algn = pAlgn;

        arrTabs.push_back(tab);
    }
}

void TxBodyConverter::FillBuChar(PPTX::Logic::Bullet &oBullet, WCHAR symbol)
{
    auto pBuChar = new PPTX::Logic::BuChar;
    pBuChar->Char.clear();
    pBuChar->Char.push_back(symbol);
    oBullet.m_Bullet.reset(pBuChar);
}

void TxBodyConverter::ConvertAllBullets(PPTX::Logic::TextParagraphPr &oPPr, CTextPFRun *pPF, CRelsGenerator* pRels)
{
    if (pPF->hasBullet.is_init())
    {
        if (pPF->hasBullet.get())
        {
            if (pPF->bulletColor.is_init())
            {
                FillBuClr(oPPr.buColor, pPF->bulletColor.get());
            }
            if (pPF->bulletSize.is_init())
            {
                PPTX::WrapperWritingElement* pBuSize;
                if (pPF->bulletSize.get() > 24 && pPF->bulletSize.get() < 401)
                {
                    pBuSize = new PPTX::Logic::BuSzPct;
                    static_cast<PPTX::Logic::BuSzPct*>(pBuSize)->val = pPF->bulletSize.get() * 1000 ;
                }
                if (pPF->bulletSize.get() < 0 && pPF->bulletSize.get() > -4001)
                {
                    pBuSize = new PPTX::Logic::BuSzPts;
                    static_cast<PPTX::Logic::BuSzPts*>(pBuSize)->val = - (pPF->bulletSize.get());
                }
                oPPr.buSize.m_Size = pBuSize;
            }
            if (pPF->bulletFontProperties.is_init())
            {
                auto pBuFont = new PPTX::Logic::TextFont;
                pBuFont->m_name = L"a:buFont";
                pBuFont->typeface = pPF->bulletFontProperties->Name;

                if ( pPF->bulletFontProperties->PitchFamily > 0)
                    pBuFont->pitchFamily = std::to_wstring(pPF->bulletFontProperties->PitchFamily);
                if ( pPF->bulletFontProperties->Charset > 0)
                    pBuFont->charset = std::to_wstring(pPF->bulletFontProperties->Charset);

                oPPr.buTypeface.m_Typeface.reset(pBuFont);
            }

            // Bullets (numbering, else picture, else char, else default)
            if (pPF->bulletBlip.is_init() && pPF->bulletBlip->tmpImagePath.size() && pRels != nullptr)
            {
                auto strRID = pRels->WriteImage(pPF->bulletBlip->tmpImagePath);
                if (strRID.empty())
                    FillBuChar(oPPr.ParagraphBullet, L'\x2022');    // error rId
                else
                {
                    auto pBuBlip = new PPTX::Logic::BuBlip;
                    pBuBlip->blip.embed = new OOX::RId(strRID);
                    oPPr.ParagraphBullet.m_Bullet.reset(pBuBlip);
                }
            }
            else if (pPF->bulletChar.is_init() && (pPF->bulletAutoNum.is_init() ? pPF->bulletAutoNum->isDefault() : true))
            {
                FillBuChar(oPPr.ParagraphBullet, pPF->bulletChar.get());
            }
            else if (pPF->bulletAutoNum.is_init())
            {
                auto pBuAutoNum = new PPTX::Logic::BuAutoNum;
                oPPr.ParagraphBullet.m_Bullet.reset(pBuAutoNum);
                if (pPF->bulletAutoNum->startAt.is_init() && pPF->bulletAutoNum->startAt.get() != 1)
                    pBuAutoNum->startAt = pPF->bulletAutoNum->startAt.get();
                if (pPF->bulletAutoNum->type.is_init())
                    pBuAutoNum->type = pPF->bulletAutoNum->type.get();
            }
            else
            {
                FillBuChar(oPPr.ParagraphBullet, L'\x2022');
            }
        }
        else
        {
            oPPr.buTypeface.m_Typeface.reset(new PPTX::Logic::BuNone);
        }
    }
}

void TxBodyConverter::FillBuClr(PPTX::Logic::BulletColor &oBuClr, CColor &oColor)
{
    auto pBuClr = new PPTX::Logic::BuClr;
    pBuClr->Color.SetRGBColor(oColor.GetR(), oColor.GetG(), oColor.GetB());
    oBuClr.m_Color.reset(pBuClr);
}

void TxBodyConverter::FillRun(PPTX::Logic::Run &oRun, CSpan &oSpan)
{
    oRun.SetText(oSpan.m_strText);

    oRun.rPr = new PPTX::Logic::RunProperties;
    FillRPr(oRun.rPr.get2(), oSpan.m_oRun);
}

PPTX::Logic::RunProperties *TxBodyConverter::getNewEndParaRPr(const int dirty, const int sz , const std::wstring& lang)
{
    auto pEndParaRPr = new PPTX::Logic::RunProperties;
    pEndParaRPr->m_name = L"a:endParaRPr";

    if (dirty == 0 || dirty == 1)
        pEndParaRPr->dirty = (bool)dirty;
    if (sz > 0)
        pEndParaRPr->sz = sz;
    if (lang.size())
        pEndParaRPr->lang = lang;

    return pEndParaRPr;
}

void TxBodyConverter::FillRPr(PPTX::Logic::RunProperties &oRPr, CTextCFRun &oCFRun)
{
    oRPr.normalizeH = false;
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


    if (m_bWordArt)
    {//порядок важен - линия, заливка, тень !!!
        if (m_pShapeElement->m_bLine)
        {
            oRPr.ln = new PPTX::Logic::Ln;
            ConvertLine(oRPr.ln.get2());
        }
        // I don't know todo or not
//        m_oWriter.WriteString(ConvertBrush(pShapeElement->m_oBrush));
//        m_oWriter.WriteString(ConvertShadow(pShapeElement->m_oShadow));
    }
    else
    {
        if (oCFRun.Color.is_init())
        {
            if (oCFRun.Color->m_lSchemeIndex != -1)
            {
                FillSchemeClr(oRPr.Fill, oCFRun.Color.get());
            }
            else
            {
                FillSolidFill(oRPr.Fill, oCFRun.Color.get());
            }
        }
    }

    FillEffectLst(oRPr.EffectList, oCFRun);
    if (oCFRun.Color.is_init())
        FillSolidFill(oRPr.Fill, oCFRun.Color.get());


    if (oCFRun.font.font.is_init())
    {
        auto charset = oCFRun.font.font->Charset;
        oRPr.latin = new PPTX::Logic::TextFont;
        oRPr.latin->m_name = L"a:latin";
        oRPr.latin->typeface = oCFRun.font.font->Name;
        if (charset < 128)
            oRPr.latin->charset = std::to_wstring(charset);
        oRPr.latin->pitchFamily = std::to_wstring(oCFRun.font.font->PitchFamily + 2);
    } // todo else for fontRef.is_init() // it's theme

    if (oCFRun.font.ea.is_init())
    {
        auto charset = oCFRun.font.ea->Charset;
        oRPr.ea = new PPTX::Logic::TextFont;
        oRPr.ea->m_name = L"a:ea";
        oRPr.ea->typeface = oCFRun.font.ea->Name;
        if (charset < 128)
            oRPr.ea->charset = std::to_wstring(oCFRun.font.ea->Charset);
    }
    if (oCFRun.font.sym.is_init())
    {
        oRPr.sym = new PPTX::Logic::TextFont;
        oRPr.sym->m_name = L"a:sym";
        oRPr.sym->typeface = oCFRun.font.sym->Name;
        oRPr.sym->charset = std::to_wstring(oCFRun.font.sym->Charset);
    }
}

void TxBodyConverter::ConvertLine(PPTX::Logic::Ln &oLn)
{
    auto& oPen = m_pShapeElement->m_oPen;
    oLn.cmpd = new PPTX::Limit::CompoundLine;
    switch(oPen.LineStyle)
    {
        case 1: oLn.cmpd->set(L"dbl");		break;
        case 2: oLn.cmpd->set(L"thickThin"); break;
        case 3: oLn.cmpd->set(L"thinThick");	break;
        case 4: oLn.cmpd->set(L"tri");		break;
    }


    oLn.w = oPen.Size;
    FillSolidFill(oLn.Fill, oPen.Color);

    if (oPen.DashStyle > 0 && oPen.DashStyle < 11)
    {
        oLn.prstDash = new PPTX::Logic::PrstDash;
        oLn.prstDash.get2().val = new PPTX::Limit::PrstDashVal;
        auto& val = oLn.prstDash.get2().val;
        switch(oPen.DashStyle)
        {
        case 1:     val->set(L"sysDash");           break;
        case 2:     val->set(L"sysDot");			break;
        case 3:     val->set(L"sysDashDot");		break;
        case 4:     val->set(L"sysDashDotDot");     break;
        case 5:     val->set(L"dot");				break;
        case 6:     val->set(L"dash");				break;
        case 7:     val->set(L"lgDash");			break;
        case 8:     val->set(L"dashDot");			break;
        case 9:     val->set(L"lgDashDot");         break;
        case 10:    val->set(L"lgDashDotDot");		break;
        }
    }

    switch(oPen.LineJoin)
    {
    case 0:	oLn.Join.type = PPTX::Logic::eJoin::JoinBevel;	break;
    case 1:	oLn.Join.type = PPTX::Logic::eJoin::JoinMiter;  break;
    case 2:	oLn.Join.type = PPTX::Logic::eJoin::JoinRound;	break;
    }

    oLn.headEnd = new PPTX::Logic::LineEnd;
    oLn.tailEnd = new PPTX::Logic::LineEnd;

    ConvertLineEnd(oLn.headEnd.get2(), oPen.LineStartCap, oPen.LineStartLength, oPen.LineStartWidth, true);
    ConvertLineEnd(oLn.tailEnd.get2(), oPen.LineEndCap, oPen.LineEndLength, oPen.LineEndWidth, false);
}

void TxBodyConverter::ConvertLineEnd(PPTX::Logic::LineEnd &oLine, unsigned char cap, unsigned char length, unsigned char width, bool isHead)
{
    if (cap < 1)
        return;

    if (isHead)
        oLine.m_name = L"a:headEnd";
    else
        oLine.m_name = L"a:tailEnd";

    oLine.type = new PPTX::Limit::LineEndType;
    switch(cap)
    {
        case 1: oLine.type->set(L"triangle");	break;
        case 2: oLine.type->set(L"stealth");	break;
        case 3: oLine.type->set(L"diamond");	break;
        case 4: oLine.type->set(L"oval");		break;
        case 5: oLine.type->set(L"arrow");		break;
    }

    oLine.len = new PPTX::Limit::LineEndSize;
    switch(length)
    {
        case 0: oLine.len->set(L"sm");	break;
        case 1: oLine.len->set(L"med");	break;
        case 2: oLine.len->set(L"lg");	break;
    }

    oLine.w = new PPTX::Limit::LineEndSize;
    switch(width)
    {
        case 0: oLine.w->set(L"sm");	break;
        case 1: oLine.w->set(L"med");	break;
        case 2: oLine.w->set(L"lg");	break;
    }
}

void TxBodyConverter::ConvertBrush(PPTX::Logic::RunProperties &oRPr)
{
    CBrush& brush = m_pShapeElement->m_oBrush;
}

void TxBodyConverter::FillSolidFill(PPTX::Logic::UniFill &oUF, CColor &oColor)
{
    auto pSolidFill = new PPTX::Logic::SolidFill;
    pSolidFill->Color.SetRGBColor(oColor.GetR(), oColor.GetG(), oColor.GetB());
    oUF.Fill.reset(pSolidFill);
}

void TxBodyConverter::FillSchemeClr(PPTX::Logic::UniFill &oUF, CColor &oColor)
{
    auto pSolidFill = new PPTX::Logic::SolidFill;
    auto pScheme = new PPTX::Logic::SchemeClr;
    pScheme->val.set(CStylesWriter::GetColorInScheme(oColor.m_lSchemeIndex));
    pSolidFill->Color.Color.reset(pScheme);
    oUF.Fill.reset(pSolidFill);
}

void TxBodyConverter::FillEffectLst(PPTX::Logic::EffectProperties &oEList, CTextCFRun &oCFRun)
{
    auto pELst = new PPTX::Logic::EffectLst;
    if (oCFRun.FontShadow.is_init() && oCFRun.FontShadow.get())
    {
        pELst->outerShdw = new PPTX::Logic::OuterShdw;
        pELst->outerShdw->blurRad = 38100;
        pELst->outerShdw->dist = 38100;
        pELst->outerShdw->dir = 2700000;
        pELst->outerShdw->algn = new PPTX::Limit::RectAlign;
        pELst->outerShdw->algn->set(L"tl");
        pELst->outerShdw->Color.SetRGBColor(0, 0, 0);
    }
    oEList.List.reset(pELst);
}
