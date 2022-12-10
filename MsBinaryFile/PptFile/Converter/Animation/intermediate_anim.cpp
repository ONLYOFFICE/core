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
