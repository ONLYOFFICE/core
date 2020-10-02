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

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/TimeNodeBase.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/WrapperWritingElement.h"

#include "Anim.h"
#include "AnimClr.h"
#include "AnimEffect.h"
#include "AnimMotion.h"
#include "AnimRot.h"
#include "AnimScale.h"
#include "Audio.h"
#include "Cmd.h"
#include "Par.h"
#include "Seq.h"
#include "Set.h"
#include "Video.h"


namespace PPT_FORMAT
{
    void ConvertTnChild(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::TimeNodeBase &oChild)
    {
        if (pETNC->m_haveSequenceAtom)
        {
            auto seq = new PPTX::Logic::Seq;
            FillSeq(pETNC, *seq);
            oChild.m_node = seq;
        }
        else if (pETNC->m_haveSetBehavior)
        {
            auto set = new PPTX::Logic::Set;
            FillSet(pETNC, *set);
            oChild.m_node = set;
        }
        else if (pETNC->m_haveAnimateBehavior)
        {
            auto anim = new PPTX::Logic::Anim;
            FillAnim(pETNC, *anim);
            oChild.m_node = anim;
        }
        else if (pETNC->m_haveColorBehavior)
        {
            // AnimClr
        }
        else if (pETNC->m_haveEffectBehavior)
        {
            auto animEffect = new PPTX::Logic::AnimEffect;
            FillAnimEffect(pETNC, *animEffect);
            oChild.m_node = animEffect;
        }
        else if (pETNC->m_haveMotionBehavior)
        {
            // AnimMotion
        }
        else if (pETNC->m_haveRotationBehavior)
        {
            // AnimRot
        }
        else if (pETNC->m_haveScaleBehavior)
        {
            // AnimScale
        }
        else if (pETNC->m_haveCommandBehavior)
        {
            // Cmd
        }
        else if (pETNC->m_oTimeNodeAtom.m_dwType == TL_TNT_Parallel)
        {
            auto par = new PPTX::Logic::Par;
            FillPar(pETNC, *par);
            oChild.m_node = par;
        }
        else
        {
            // ? Audio Video
        }
    }
}
