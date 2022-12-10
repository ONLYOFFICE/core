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
#include "Animation_1995.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Anim.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimClr.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimEffect.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimMotion.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimRot.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimScale.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimVariant.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Audio.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/CBhvr.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/ChildTnLst.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Cmd.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/CondLst.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Set.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Video.h"
#include "../../Records/Animations/AnimationInfoContainer.h"

using namespace PPT::Converter;

Animation_1995::Animation_1995(_INT32 &cTnId) :
    cTnId(cTnId)
{}

void Animation_1995::FillCTnAnimation(PPTX::Logic::CTn &oCTN, PPT::Intermediate::SOldAnimation *pOldAnim)
{
    oCTN.presetClass = L"entr";
    oCTN.grpId = 0;
    oCTN.childTnLst = new PPTX::Logic::ChildTnLst;
    oCTN.presetSubtype = 0;
    int presetSub = -1;

    const UINT effect = pOldAnim->anim->m_AnimationAtom.m_AnimEffect;
    const UINT direct = pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection;

    // Todo 4, 7. 0x11 - 0x1B
    switch (effect)
    {
    case 0x00:
    {
        if (direct != 1)
        {
            oCTN.presetID = 1;
            ConvertAppear(oCTN.childTnLst.get2(), pOldAnim);
        }
        else
        {
            oCTN.presetID = 10;
            ConvertFade(oCTN.childTnLst.get2(), pOldAnim);
        }
        break;
    }
    case 0x01:
    {
        oCTN.presetID = 24;
        ConvertRandomEffect(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x02:
    {
        oCTN.presetID = 3;
        ConvertBlinds(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x03:
    {
        oCTN.presetID = 5;
        ConvertCheckerboard(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x05:
    {
        oCTN.presetID = 9;
        ConvertDissolveIn(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x06:
    {
        oCTN.presetID = 10;
        ConvertFade(oCTN.childTnLst.get2(), pOldAnim);
        break;
    }
    case 0x08:
    {
        oCTN.presetID = 14;
        ConvertRandomBars(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x09:
    {
        oCTN.presetID = 18;
        ConvertStrips(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0A:
    {
        oCTN.presetID = 22;
        ConvertWipe(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0B:
    {
        oCTN.presetID = 4;
        ConvertShape(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0C:
    {
        if (direct >= 0 && direct <= 7)
        {
            oCTN.presetID = 2;
            ConvertFlyIn(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 8 && direct <= 11)
        {
            oCTN.presetID = 12;
            ConvertPeekIn(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 12 && direct <= 15)
        {
            oCTN.presetID = 7;
            ConvertCrawlIn(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 16 && direct <= 21)
        {
            oCTN.presetID = 23;
            ConvertBasicZoom(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct >= 22 && direct <= 26)
        {
            oCTN.presetID = 17;
            ConvertStretch(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct == 27)
        {
            oCTN.presetID = 19;
            ConvertBasicSwivel(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        } else if (direct == 28)
        {
            oCTN.presetID = 15;
            ConvertSpiralIn(oCTN.childTnLst.get2(), pOldAnim);
        }
        break;
    }
    case 0x0D:
    {
        oCTN.presetID = 16;
        ConvertSplit(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    case 0x0E:
    {
        oCTN.presetID = 11;
        ConvertFlashOnce(oCTN.childTnLst.get2(), pOldAnim, presetSub);
        break;
    }
    default:
        oCTN.presetID = 1;
        ConvertAppear(oCTN.childTnLst.get2(), pOldAnim);
        std::wcout << "Error: Unknown old animation id: " << std::to_wstring(effect) << L"\n";

    }

    if (presetSub != -1)
    {
        oCTN.presetSubtype = presetSub;
    }
}



void Animation_1995::FillCBhvrForAnim (PPTX::Logic::Anim& oAnim, PPT::Intermediate::SOldAnimation* pOldAnim, int dur, std::wstring attrname)
{
    FillCBhvr(oAnim.cBhvr, dur, pOldAnim->shapeId, attrname, -1);

    oAnim.cBhvr.additive = new PPTX::Limit::TLAdditive;
    oAnim.cBhvr.additive->set(L"base");

    oAnim.calcmode       = new PPTX::Limit::TLCalcMode;
    oAnim.calcmode->set(L"lin");
    oAnim.valueType      = new PPTX::Limit::TLValueType;
    oAnim.valueType->set(L"num");
}

void Animation_1995::FillCBhvr(PPTX::Logic::CBhvr &oBhvr, int dur, UINT spid, std::wstring attrname, int delay)
{
    oBhvr.cTn.id = cTnId++;
    oBhvr.cTn.fill = L"hold";
    oBhvr.cTn.dur = std::to_wstring(dur);
    if (delay > -1)
    {
        oBhvr.cTn.stCondLst = new PPTX::Logic::CondLst;
        oBhvr.cTn.stCondLst->node_name = L"stCondLst";
        PPTX::Logic::Cond cond;
        cond.delay = std::to_wstring(delay);
        oBhvr.cTn.stCondLst->list.push_back(cond);
    }

    oBhvr.tgtEl.spTgt = new PPTX::Logic::SpTgt;
    oBhvr.tgtEl.spTgt->spid = std::to_wstring(spid);

    if (!attrname.empty())
    {
        oBhvr.attrNameLst = new PPTX::Logic::AttrNameLst;
        PPTX::Logic::AttrName attrName;
        attrName.text = attrname;
        oBhvr.attrNameLst->list.push_back(attrName);
    }
}

void Animation_1995::FillCBhvr(PPTX::Logic::CBhvr &oCBhvr, PPT::Intermediate::SOldAnimation *pOldAnim, int delay)
{
    FillCBhvr(oCBhvr, 1, pOldAnim->shapeId, L"style.visibility", delay);
}

void Animation_1995::FillAnim (PPTX::Logic::Anim& oAnim, PPT::Intermediate::SOldAnimation* pOldAnim, int dur, std::wstring attrname,
	PPT::Intermediate::SValue val1, PPT::Intermediate::SValue val2, std::wstring fmla)
{
    FillCBhvrForAnim(oAnim, pOldAnim, dur, attrname);

    oAnim.tavLst = new PPTX::Logic::TavLst;
    PPTX::Logic::Tav tav;

    tav.tm = L"0";
    if (fmla.size())
    {
        tav.fmla = fmla;
    }
    auto val = new PPTX::Logic::AnimVariant;
    val->node_name = L"val";
    if (val1.type == Intermediate::SValue::str)
        val->strVal = val1.strVal;
    else if (val1.type == Intermediate::SValue::dbl)
        val->fltVal = val1.dblVal;

    tav.val = val;
    oAnim.tavLst->list.push_back(tav);
    tav.fmla.reset();

    tav.tm = L"100000";
    val = new PPTX::Logic::AnimVariant;
    val->node_name = L"val";
    if (val2.type == Intermediate::SValue::str)
        val->strVal = val2.strVal;
    else if (val2.type == Intermediate::SValue::dbl)
        val->fltVal = val2.dblVal;

    tav.val = val;
    oAnim.tavLst->list.push_back(tav);

}

void Animation_1995::FillAnimEffect (PPTX::Logic::AnimEffect& oAnimEffect, PPT::Intermediate::SOldAnimation* pOldAnim, std::wstring filter, std::wstring transition)
{
    oAnimEffect.transition = new PPTX::Limit::TLTransition();
    oAnimEffect.transition->set(transition);
    oAnimEffect.filter = filter;

    oAnimEffect.cBhvr.cTn.id = cTnId++;
    oAnimEffect.cBhvr.cTn.dur = std::to_wstring(pOldAnim->getAnimDur());
    oAnimEffect.cBhvr.tgtEl.spTgt = new PPTX::Logic::SpTgt;
    oAnimEffect.cBhvr.tgtEl.spTgt->spid = std::to_wstring(pOldAnim->shapeId);
}

// This methods fill ChildTnLst with anim nodes
void Animation_1995::ConvertAppear(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim)
{
    PushSet(oParent, pOldAnim, 0);
}

void Animation_1995::ConvertFlyIn(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 8;
        break;
    }
    case 1:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 1;
        break;
    }
    case 2:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 2;
        break;
    }
    case 3:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 4;
        break;
    }
    case 4:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 9;
        break;
    }
    case 5:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 3;
        break;
    }
    case 6:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 12;
        break;
    }
    case 7:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 6;
        break;
    }
    }
}

void Animation_1995::ConvertBlinds(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int &presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"blinds(vertical)", L"in");

        presetSub = 5;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"blinds(horizontal)", L"in");

        presetSub = 10;
        break;
    }
    }
}

void Animation_1995::ConvertShape(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"box(out)", L"in");

        presetSub = 32;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"box(in)", L"in");

        presetSub = 16;
        break;
    }
    }
}

void Animation_1995::ConvertCheckerboard(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim)
{
    PushSet(oParent, pOldAnim);
    PushAnimEffect(oParent, pOldAnim, L"checkerboard(across)", L"in");
}

void Animation_1995::ConvertCrawlIn(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 15:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"1+#ppt_h/2", L"#ppt_y");

        presetSub = 4;
        break;
    }
    case 12:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"0-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 8;
        break;
    }
    case 14:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"1+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");

        presetSub = 2;
        break;
    }
    case 13:
    {
        PushAnim(oParent, pOldAnim, 5000,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"0-#ppt_h/2", L"#ppt_y");

        presetSub = 1;
        break;
    }
    }
}

void Animation_1995::ConvertDissolveIn(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim)
{
    PushSet(oParent, pOldAnim);
    PushAnimEffect(oParent, pOldAnim, L"dissolve", L"in");
}

void Animation_1995::ConvertFade(PPTX::Logic::ChildTnLst &oParent, PPT::Intermediate::SOldAnimation *pOldAnim)
{
    PushSet(oParent, pOldAnim);
    PushAnimEffect(oParent, pOldAnim, L"fade", L"in");
}

void Animation_1995::ConvertFlashOnce(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);
    presetSub = 0;

    // correct cTn params
    auto set = dynamic_cast<PPTX::Logic::Set*>(oParent.list.front().m_node.GetPointer());
    if (!set) return;
    set->cBhvr.cTn.dur = std::to_wstring(pOldAnim->getAnimDur());
    set->cBhvr.cTn.fill.reset();
}

void Animation_1995::ConvertPeekIn(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 8:
    {
        PushAnim(oParent, pOldAnim, L"ppt_x", L"#ppt_x-#ppt_w*1.125000", L"#ppt_x");
        PushAnimEffect(oParent, pOldAnim, L"wipe(right)", L"in");

        presetSub = 8;
        break;
    }
    case 9:
    {
        PushAnim(oParent, pOldAnim, L"ppt_y", L"#ppt_y+#ppt_h*1.125000", L"#ppt_y");
        PushAnimEffect(oParent, pOldAnim, L"wipe(up)", L"in");

        presetSub = 4;
        break;
    }
    case 10:
    {
        PushAnim(oParent, pOldAnim, L"ppt_x", L"#ppt_x+#ppt_w*1.125000", L"#ppt_x");
        PushAnimEffect(oParent, pOldAnim, L"wipe(left)", L"in");

        presetSub = 2;
        break;
    }
    case 11:
    {
        PushAnim(oParent, pOldAnim, L"ppt_y", L"#ppt_y-#ppt_h*1.125000", L"#ppt_y");
        PushAnimEffect(oParent, pOldAnim, L"wipe(down)", L"in");

        presetSub = 1;
        break;
    }
    }
}

void Animation_1995::ConvertRandomBars(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"randombar(horizontal)", L"in");

        presetSub = 10;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"randombar(vertical)", L"in");

        presetSub = 5;
        break;
    }
    }
}

void Animation_1995::ConvertSpiralIn(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim)
{
    PushSet(oParent, pOldAnim);
    //     TODO
    PushAnim(oParent, pOldAnim, 1000,
             L"ppt_w", double(0), L"#ppt_w",
             L"ppt_h", double(0), L"#ppt_h");
    PushAnim(oParent, pOldAnim, 1000,
             L"ppt_x", double(0), double(1),
             L"ppt_y", double(0), double(1),
             L"#ppt_x+(cos(-2*pi*(1-$))*-#ppt_x-sin(-2*pi*(1-$))*(1-#ppt_y))*(1-$)",
             L"#ppt_y+(sin(-2*pi*(1-$))*-#ppt_x+cos(-2*pi*(1-$))*(1-#ppt_y))*(1-$)");

    // remove additive from <p:anim>
    for (auto& child : oParent.list)
    {
        auto anim = dynamic_cast<PPTX::Logic::Anim*>(child.m_node.GetPointer());
        if (!anim) continue;
        anim->cBhvr.additive.reset();
    }

}

void Animation_1995::ConvertSplit(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(outHorizontal)", L"in");

        presetSub = 42;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(inHorizontal)", L"in");

        presetSub = 26;
        break;
    }
    case 2:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(outVertical)", L"in");

        presetSub = 37;
        break;
    }
    case 3:
    {
        PushAnimEffect(oParent, pOldAnim, L"barn(inVertical)", L"in");

        presetSub = 21;
        break;
    }
    }
}

void Animation_1995::ConvertStretch(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 22:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 10;
        break;
    }
    case 26:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x", L"#ppt_x",
                 L"ppt_y", L"#ppt_y+#ppt_h/2", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"#ppt_w", L"#ppt_w",
                 L"ppt_h", double(0), L"#ppt_h");

        presetSub = 4;
        break;
    }
    case 23:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"##ppt_x-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 8;
        break;
    }
    case 25:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x-#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 2;
        break;
    }
    case 24:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", L"#ppt_x+#ppt_w/2", L"#ppt_x",
                 L"ppt_y", L"#ppt_y", L"#ppt_y");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", L"#ppt_h", L"#ppt_h");

        presetSub = 1;
        break;
    }
    }
}

void Animation_1995::ConvertStrips(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 6:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(downLeft)", L"in");

        presetSub = 12;
        break;
    }
    case 4:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(upLeft)", L"in");

        presetSub = 9;
        break;
    }
    case 7:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(downRight)", L"in");

        presetSub = 6;
        break;
    }
    case 5:
    {
        PushAnimEffect(oParent, pOldAnim, L"strips(upRight)", L"in");

        presetSub = 3;
        break;
    }
    }
}

void Animation_1995::ConvertBasicSwivel(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);
    presetSub = 10;

    PushAnim(oParent, pOldAnim, 5000,
             L"ppt_w", double(0), double(1),
             L"ppt_h", L"#ppt_h", L"#ppt_h",
             L"#ppt_w*sin(2.5*pi*$)");
}

void Animation_1995::ConvertWipe(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 3:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(up)", L"in");

        presetSub = 1;
        break;
    }
    case 0:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(left)", L"in");

        presetSub = 2;
        break;
    }
    case 2:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(right)", L"in");

        presetSub = 8;
        break;
    }
    case 1:
    {
        PushAnimEffect(oParent, pOldAnim, L"wipe(down)", L"in");

        presetSub = 4;
        break;
    }
    }
}

void Animation_1995::ConvertBasicZoom(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim, int& presetSub)
{
    PushSet(oParent, pOldAnim);

    switch (pOldAnim->anim->m_AnimationAtom.m_AnimEffectDirection)
    {
    case 16:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", double(0), L"#ppt_h");

        presetSub = 16;
        break;
    }
    case 20:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", double(0), L"#ppt_w",
                 L"ppt_h", double(0), L"#ppt_h");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", double(0.5), L"#ppt_x",
                 L"ppt_y", double(0.5), L"#ppt_y");

        presetSub = 528;
        break;
    }
    case 17:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"2/3*#ppt_w", L"#ppt_w",
                 L"ppt_h", L"2/3*#ppt_h", L"#ppt_h");

        presetSub = 272;
        break;
    }
    case 18:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"#ppt_w", L"#ppt_w",
                 L"ppt_h", L"4*#ppt_h", L"#ppt_h");

        presetSub = 32;
        break;
    }
    case 21:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"(6*min(max(#ppt_w*#ppt_h,.3),1)-7.4)/-.7*#ppt_w", L"#ppt_w",
                 L"ppt_h", L"(6*min(max(#ppt_w*#ppt_h,.3),1)-7.4)/-.7*#ppt_h", L"#ppt_h");
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_x", double(0.5), L"#ppt_x",
                 L"ppt_y", L"1+(6*min(max(#ppt_w*#ppt_h,.3),1)-7.4)/-.7*#ppt_h/2", L"#ppt_y");

        presetSub = 36;
        break;
    }
    case 19:
    {
        PushAnim(oParent, pOldAnim, 500,
                 L"ppt_w", L"4/3*#ppt_w", L"#ppt_w",
                 L"ppt_h", L"4/3*#ppt_h", L"#ppt_h");

        presetSub = 288;
        break;
    }
    }
}

void Animation_1995::ConvertRandomEffect(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation* pOldAnim)
{
    PushSet(oParent, pOldAnim, 0);

    PPTX::Logic::TimeNodeBase childTimeNode;

    auto anim = new PPTX::Logic::Anim;
    anim->to = L"";
    anim->calcmode = new PPTX::Limit::TLCalcMode;
    anim->calcmode->set(L"lin");
    anim->valueType = new PPTX::Limit::TLValueType;
    anim->valueType->set(L"num");

    FillCBhvr(anim->cBhvr, 1, pOldAnim->shapeId, L"", -1);
    childTimeNode.m_node = anim;
    oParent.list.push_back(childTimeNode);
}

void Animation_1995::PushAnim(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int dur,
                              std::wstring attrname1, PPT::Intermediate::SValue val1, PPT::Intermediate::SValue val2,
                              std::wstring attrname2, PPT::Intermediate::SValue val3, PPT::Intermediate::SValue val4, std::wstring fmla1, std::wstring fmla2)
{
    PushAnim(oParent, pOldAnim, attrname1, val1, val2, fmla1);
    PushAnim(oParent, pOldAnim, attrname2, val3, val4, fmla2);
}

void Animation_1995::PushAnim(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim,
                              std::wstring attrname1, PPT::Intermediate::SValue val1, PPT::Intermediate::SValue val2,std::wstring fmla1)
{
    PPTX::Logic::TimeNodeBase childTimeNode;

    auto anim = new PPTX::Logic::Anim;
    FillAnim(*anim, pOldAnim, pOldAnim->getAnimDur(), attrname1, val1, val2, fmla1);
    childTimeNode.m_node = anim;
    oParent.list.push_back(childTimeNode);
}

void Animation_1995::PushAnimEffect(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, std::wstring filter, std::wstring transition)
{
    PPTX::Logic::TimeNodeBase childTimeNode;

    auto animEffect = new PPTX::Logic::AnimEffect;
    FillAnimEffect(*animEffect, pOldAnim, filter, transition);
    childTimeNode.m_node = animEffect;
    oParent.list.push_back(childTimeNode);
}

void Animation_1995::PushSet(PPTX::Logic::ChildTnLst& oParent, PPT::Intermediate::SOldAnimation *pOldAnim, int dur)
{
    PPTX::Logic::TimeNodeBase childTimeNode;
    auto set = new PPTX::Logic::Set;

    FillCBhvr(set->cBhvr, pOldAnim, dur);
    set->to = new PPTX::Logic::AnimVariant;
    set->to->node_name = L"to";
    set->to->strVal = L"visible";

    childTimeNode.m_node = set;
    oParent.list.push_back(childTimeNode);
}
