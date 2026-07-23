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
#include "intermediate_anim.h"


_UINT32 PPT::Intermediate::SOldAnimation::getAnimDur() const
{
    _UINT32 dur = 500;
    if (!anim)
        return 0;

    const BYTE effect = anim->m_AnimationAtom.m_AnimEffect;
    const BYTE direct = anim->m_AnimationAtom.m_AnimEffectDirection;

    if (effect == 12 && ((direct >= 12 && direct <= 15) || direct == 27))   // Crawl In, Basic Swivel
        dur = 5000;
    else if (effect == 12 && direct == 28)
        dur = 1000;
    else if (effect == 14)
    {
        switch (direct)
        {
        case 0: dur = 75; break;
        case 1: dur = 500; break;
        case 2: dur = 1000; break;
        }
    } else if (effect == 19 || effect == 26)
        dur = 2000;

    return dur;
}
