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
