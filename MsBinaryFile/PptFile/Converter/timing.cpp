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
