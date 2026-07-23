/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../../PPTXWriter/ImageManager.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Timing.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Par.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Seq.h"
#include "intermediate_anim.h"


namespace PPT {
namespace Converter {
class Animation_1995;

using LstAnim = std::list<Intermediate::SOldAnimation*>;
using LstLstAnim = std::list<LstAnim>;

class Timing_1995
{
public:
    Timing_1995(const std::vector<Intermediate::SOldAnimation>& vecAIC);
    void Convert(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels);
    ~Timing_1995();

private:
    bool InitAndCheckVars(PPTX::Logic::Timing& timimg, CExMedia* pExMedia, CRelsGenerator* pRels);
    void ConvertBldLst();
    void ConvertTnLst();
    PPTX::Logic::Seq *ConvertMainSeqAnimation(LstLstAnim& splitedAnim);
    LstLstAnim SplitRawAnim();
    void InsertMainSeqToTnLst(PPTX::Logic::Seq *mainSeq);
    PPTX::Logic::Par *InitRootNode();
    PPTX::Logic::Seq *InitMainSeq();
    void SortAnim();

    void FillClickGroup(LstAnim &clickPar, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    static LstLstAnim SplitClickGroupByParrallelShow(LstAnim &clickGroup);
    void FillFirstAutomaticGroup(PPTX::Logic::CTn &cTn, const CRecordAnimationInfoAtom& animAtom) const;
    void FillCTnParams(PPTX::Logic::CTn &cTn, std::wstring nodeType, std::wstring condDelay=L"", std::wstring fill=L"hold", Intermediate::SOldAnimation* pOldAnim = nullptr);
    void ConvertParallelGroupAnimation(LstAnim& parGroup, PPTX::Logic::TimeNodeBase &oTimeNodeBase, _UINT32& groupDelay);
    static _INT32 GetParallelGroupDuration(LstAnim &parGroup);
    void ConvertHeadEfectInParallelShow(const LstAnim& group, PPTX::Logic::CTn &cTn);
    void ConvertWithEfectsInParallelShow(const LstAnim& group, PPTX::Logic::CTn &cTn);
    void FillClickEffect(Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    void FillAfterEffect(Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    void FillWithEffect (Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase);
    void FillEffectType (Intermediate::SOldAnimation* pOldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase, std::wstring nodeType);

private:
    std::vector<Intermediate::SOldAnimation> arrOldAnim;
    Animation_1995* animConverter;

    PPTX::Logic::Timing* pTiming = nullptr;
    CExMedia* pExMedia;
    CRelsGenerator* pRels;
    _INT32 cTnId = 3;
};
}
}
