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
#include "timing.h"

#include "Animation/intermediate_anim.h"
#include "Animation/Timing_1995.h"
#include "Animation/Timing_2010.h"
#include "Animation/TimingExeption.h"
#include "Animation/hashcode10.h"   // not work correct


using namespace PPT::Converter;

Timing::Timing(const PPT::Intermediate::SlideAnimation& slideAnim) :
    slideAnim(slideAnim)
{}

PPTX::Logic::Timing Timing::Convert(PPT::CExMedia *pExMedia, CRelsGenerator *pRels)
{
    this->pExMedia = pExMedia;
    this->pRels = pRels;

    if (TryToConvertTiming2010() == false)
        if (TryToConvertTiming1995() == false)
            return {};

    if (timing.bldLst.IsInit() && timing.bldLst->list.empty())    // You can't leave an empty tag <p:bldLst/>
        timing.bldLst.reset();

    return std::move(timing);
}

bool Timing::HasAnimation() const
{
    if (slideAnim.arrAnim_1995.empty() && (slideAnim.pAnim_2010 == nullptr || slideAnim.pAnim_2010->m_haveExtTime == false))
        return false;
    else
        return true;
}

bool Timing::TryToConvertTiming2010()
{
    timing = PPTX::Logic::Timing();
    try {
        Timing_2010(slideAnim).
                Convert(timing, pExMedia, pRels);
    } catch (const TimingExeption &ex) {
        return false;
    } catch (...) {
        return false;
    }
    return true;
}

bool Timing::TryToConvertTiming1995()
{
    timing = PPTX::Logic::Timing();
    try {
        Timing_1995(slideAnim.arrAnim_1995).
                Convert(timing, pExMedia, pRels);
    } catch (const TimingExeption &ex) {
        return false;
    } catch (...) {
        return false;
    }
    return true;
}
