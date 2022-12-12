/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "BulletsConverter.h"
#include "StylesWriter.h"

using namespace PPT;

BulletsConverter::BulletsConverter(CRelsGenerator* pRels) : m_pRels(pRels) {}

void BulletsConverter::FillPPr(PPTX::Logic::TextParagraphPr &oPPr, CParagraph &paragraph)
{
    oPPr.lvl = paragraph.m_lTextLevel;

    auto* pPF = &(paragraph.m_oPFRun);
    if (pPF)
        ConvertPFRun(oPPr, pPF);
}

void BulletsConverter::ConvertPFRun(PPTX::Logic::TextParagraphPr &oPPr, CTextPFRun *pPF)
{
    const int oneTabSize = 457200;

    const int tabLvl = oPPr.lvl.get_value_or(0);
    int leftMargin = 0;
    if (pPF->leftMargin.is_init())
    {
        leftMargin = pPF->leftMargin.get();
        oPPr.marL = leftMargin;
    }
    if (pPF->indent.is_init())
    {
        if (pPF->hasBullet.get_value_or(false) && tabLvl == -1)
        {
            oPPr.indent = pPF->indent.get() - leftMargin;
        }
        else
        {
            oPPr.indent = pPF->indent.get();
        }
    } else if (pPF->hasBullet.get_value_or(false))
    {
        if (tabLvl != -1)
            oPPr.indent = oneTabSize*tabLvl - leftMargin;
        else
            oPPr.indent = -leftMargin;
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
    if (pPF->wrapFlags.is_init() && pPF->wrapFlags.get() & 0b111)
    {
        const auto& flags = pPF->wrapFlags.get();
        if (flags & 0b100)
            oPPr.hangingPunct = true;

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
        if (val > 0)                        // Spacing Points
            pSpcAft->spcPts = round(12.5 * pPF->spaceAfter.get());
        else if (val < 0 && val > -13200)   // Spacing Percent
            pSpcAft->spcPct = val * -1000;

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

    ConvertAllBullets(oPPr, pPF);
}

void BulletsConverter::ConvertTabStops(std::vector<PPTX::Logic::Tab> &arrTabs, std::vector<std::pair<int, int> > &arrTabStops)
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

void BulletsConverter::FillBuChar(PPTX::Logic::Bullet &oBullet, WCHAR symbol, CTextPFRun *pPF)
{
    auto pBuChar = new PPTX::Logic::BuChar;
    pBuChar->Char.clear();
    if (pPF != nullptr && pPF->bulletFontProperties.IsInit())
    {
        const auto& fontProp = *(pPF->bulletFontProperties);
        if (fontProp.Charset == 10)
        {
            symbol &= 0x00ff;
            symbol |= 0xf000;
        }
    }
    pBuChar->Char.push_back(symbol);
    oBullet.m_Bullet.reset(pBuChar);
}

void BulletsConverter::ConvertAllBullets(PPTX::Logic::TextParagraphPr &oPPr, CTextPFRun *pPF)
{
    if (pPF->hasBullet.is_init() == false)
        return;

    if (pPF->hasBullet.get() == false)
    {
        oPPr.buTypeface.m_Typeface.reset(new PPTX::Logic::BuNone);
        return;
    }
    if (pPF->bulletColor.is_init())
    {
        FillBuClr(oPPr.buColor, pPF->bulletColor.get());
    }
    if (pPF->bulletSize.is_init())
    {
        PPTX::WrapperWritingElement* pBuSize(nullptr);
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
        if (pBuSize != nullptr)
            oPPr.buSize.m_Size = pBuSize;
    }
    if (pPF->bulletFontProperties.is_init())
    {
        auto pBuFont = new PPTX::Logic::TextFont;
        pBuFont->m_name = L"a:buFont";
        pBuFont->typeface = pPF->bulletFontProperties->Name;

        if ( CFontProperty::IsValidPitchFamily(pPF->bulletFontProperties->PitchFamily))
            pBuFont->pitchFamily = std::to_wstring(pPF->bulletFontProperties->PitchFamily);
        if ( CFontProperty::IsValidCharset(pPF->bulletFontProperties->Charset))
            pBuFont->charset = std::to_wstring(pPF->bulletFontProperties->Charset);

        oPPr.buTypeface.m_Typeface.reset(pBuFont);
    }

    // Bullets (numbering, else picture, else char, else default)
    if (pPF->bulletBlip.is_init() && pPF->bulletBlip->tmpImagePath.size() && m_pRels != nullptr)
    {
        auto strRID = m_pRels->WriteImage(pPF->bulletBlip->tmpImagePath);
        if (strRID.empty())
            FillBuChar(oPPr.ParagraphBullet, L'\x2022');    // error rId
        else
        {
            auto pBuBlip = new PPTX::Logic::BuBlip;
            pBuBlip->blip.embed = new OOX::RId(strRID);
            oPPr.ParagraphBullet.m_Bullet.reset(pBuBlip);
        }
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
    else if (pPF->bulletChar.is_init())
    {
        FillBuChar(oPPr.ParagraphBullet, pPF->bulletChar.get(), pPF);
    }
    else
    {
        FillBuChar(oPPr.ParagraphBullet, L'\x2022');
    }
}


void BulletsConverter::FillBuClr(PPTX::Logic::BulletColor &oBuClr, ODRAW::CColor &oColor)
{
    auto pBuClr = new PPTX::Logic::BuClr;
    pBuClr->Color.SetRGBColor(oColor.GetR(), oColor.GetG(), oColor.GetB());
    oBuClr.m_Color.reset(pBuClr);
}
