/*
  *(c) Copyright Ascensio System SIA 2010-2019
 *
  *This program is a free software product. You can redistribute it and/or
  *modify it under the terms of the GNU Affero General Public License (AGPL)
  *version 3 as published by the Free Software Foundation. In accordance with
  *Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
  *that Ascensio System SIA expressly excludes the warranty of non-infringement
  *of any third-party rights.
 *
  *This program is distributed WITHOUT ANY WARRANTY; without even the implied
  *warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
  *details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
  *You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
  *street, Riga, Latvia, EU, LV-1050.
 *
  *The  interactive user interfaces in modified source and object code versions
  *of the Program must display Appropriate Legal Notices, as required under
  *Section 5 of the GNU AGPL version 3.
 *
  *Pursuant to Section 7(b) of the License you must retain the original Product
  *logo when distributing the program. Pursuant to Section 7(e) we decline to
  *grant you any rights under trademark law for use of our trademarks.
 *
  *All the Product's GUI elements, including illustrations and icon sets, as
  *well as technical writing content are licensed under the terms of the
  *Creative Commons Attribution-ShareAlike 4.0 International. See the License
  *terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Timing.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Anim.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimEffect.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimMotion.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimRot.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimScale.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/AnimVariant.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Audio.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CBhvr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/ChildTnLst.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Cmd.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/CondLst.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Par.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Seq.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Set.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Video.h"

#include "../Records/SlideProgTagsContainer.h"
#include "../Records/SoundCollectionContainer.h"
#include "../Records/Animations/AnimationInfoContainer.h"

namespace PPT_FORMAT
{
struct SOldAnimation
{
    UINT shapeId;
    CRecordAnimationInfoContainer* anim;

    // There will be additional records for animation here;
};

// Extenstion for CRecordExtTimeNodeContainer
class Animation
{
public:
    Animation(CRecordPP10SlideBinaryTagExtension *pPPT10Ext, const std::vector<SOldAnimation> &oldAnim) :
        m_pPPT10(pPPT10Ext),
        m_arrOldAnim(oldAnim),
        m_pSoundContainer(nullptr),
        m_cTnId(1),
        m_pBldLst(nullptr),
        m_currentBldP(nullptr)
    {

    }

    // Not delete any pointers
    CRecordPP10SlideBinaryTagExtension *m_pPPT10;       // Must be it xor (maybe 'or' i dunno)
    std::vector<SOldAnimation>          m_arrOldAnim;   // this one

    CRecordSoundCollectionContainer *m_pSoundContainer; // Optional

    // Call it and only it to convert animation
    void Convert(PPTX::Logic::Timing &oTiming);

private:
    void FillTnLst(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::TnLst &oTnLst);
    void FillCTn(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::CTn &oCTn);
    void FillTnChild(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::TimeNodeBase &oChild);
    void FillAnim(
            CRecordTimeAnimateBehaviorContainer *pTimeAnimateBehavior,
            PPTX::Logic::Anim &oAnim);
    void FillAnimClr(
            CRecordTimeColorBehaviorContainer *pColor,
            CRecordTimePropertyList4TimeNodeContainer *pProp,
            PPTX::Logic::AnimClr &oAnimClr);
    void FillAnimEffect(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::AnimEffect &oAnim);
    void FillAnimMotion(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::AnimMotion &oAnim);
    void FillAnimRot(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::AnimRot &oAnim);
    void FillAnimScale(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::AnimScale &oAnim);
    void FillAudio(
            PPT_FORMAT::CRecordClientVisualElementContainer *pCVEC,
            PPTX::Logic::Audio &oAudio);
    void FillCmd(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::Cmd &oCmd);
    void FillVideo(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::Video &oVideo);
    void FillPar(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::Par &oPar);
    void FillSeq(
            PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::Seq &oSec);
    void FillCBhvr(
            CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::CBhvr &oBhvr);
    void FillCBhvr(
            CRecordTimeBehaviorContainer *pBhvr,
            PPTX::Logic::CBhvr &oBhvr);
    void FillCBhvr(
            int dur, UINT spid, std::wstring attrname, int delay,
            PPTX::Logic::CBhvr &oBhvr);
    void FillCond(
            PPT_FORMAT::CRecordTimeConditionContainer *oldCond,
            PPTX::Logic::Cond &cond);
    void FillBldLst(
            PPT_FORMAT::CRecordBuildListContainer *pBLC,
            PPTX::Logic::BldLst &oBL);
    void FillBldP(
            PPT_FORMAT::CRecordParaBuildContainer *pPBC,
            PPTX::Logic::BldP &oBP);
    void FillSet(
            PPT_FORMAT::CRecordExtTimeNodeContainer *pETNC,
            PPTX::Logic::Set& oSet);
    void FillSet(
            PPT_FORMAT::CRecordTimeSetBehaviorContainer *pTSBC,
            PPTX::Logic::Set& oSet);
    void FillSubTnLst (
            std::vector<CRecordSubEffectContainer*> &vecSEC,
            PPTX::Logic::TnLst &oSubTnLst);
    void FillCondLst(
            std::vector<CRecordTimeConditionContainer*>& oCondVec,
            PPTX::Logic::CondLst &oCondLst);
    void FillCTn(
            CRecordTimePropertyList4TimeNodeContainer *pProp,
            PPTX::Logic::CTn &oCTn);
//    void SubEffectToETNC(
//            const CRecordSubEffectContainer *pSub,
//            CRecordExtTimeNodeContainer *pETNC);

    // For old animation.
    void InitTimingTags(PPTX::Logic::Timing &oTiming); // Initialize non-exist (in 95-97 format) struct
    void FillOldAnim(SOldAnimation& oldAnim, PPTX::Logic::TimeNodeBase &oTimeNodeBase);

public:
    inline void setNextRId(int nextRId)
    {
        m_nextRID = nextRId;
    }

private:
    unsigned m_cTnId;
    PPTX::Logic::BldLst *m_pBldLst; // Do not delete
    PPTX::Logic::BldP   *m_currentBldP;

    int m_nextRID; // it needs for audio maybe video for compisation id number;
};

}
