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
#include "TimingUtils.h"
#include "../../Reader/ExtXmlUtils.h"
#include "../../Enums/enums.h"


PPT::Intermediate::ShapeAnim PPT::Intermediate::ParseExisting5Level_CTn(const PPTX::Logic::CTn &oCTn)
{
    ShapeAnim spAn;
    spAn.presetID = oCTn.presetID.get_value_or(-1);
    std::wstring xml = oCTn.toXML();
    auto arrSpids = XmlUtils::FindAttrValuesInt(xml, L"spid");
    if (arrSpids.size())
        spAn.spid = arrSpids[0];

    return spAn;
}

PPT::Intermediate::ShapeAnim PPT::Intermediate::ParseExisting5Level_ETNC(const CRecordExtTimeNodeContainer *pETNC)
{
    if (pETNC == nullptr)
        return {};

    ShapeAnim spAn;
    for (const auto& child : pETNC->m_arrRgExtTimeNodeChildren)
    {
        auto foundSpid = GetSpID(child);
        if (foundSpid != -1)
        {
            spAn.spid = foundSpid;
            break;
        }
    }

    if (pETNC->m_haveTimePropertyList && pETNC->m_pTimePropertyList != nullptr)
    {
        for (const auto prop : pETNC->m_pTimePropertyList->m_arrElements)
        {
            if (prop->m_oHeader.RecInstance == TL_TPID_EffectID)
            {
                spAn.presetID = dynamic_cast<CRecordTimeEffectID*>(prop)->m_Value;
                break;
            }
        }
    }
    return spAn;
}

bool PPT::Intermediate::operator==(const ShapeAnim &s1, const ShapeAnim &s2)
{
    if (s1.IsValid() == false)
        return false;

    return s1.spid == s2.spid && (s1.presetID == s2.presetID || s1.presetID == 1 || s2.presetID == 1);  /// 1 - Appear
}

PPT::CRecordTimeBehaviorContainer* PPT::Intermediate::GetTimeBehaviorContainer(CRecordExtTimeNodeContainer *pETNC)
{
    CRecordTimeBehaviorContainer *pBhvr = nullptr;
    if      (pETNC->m_haveSetBehavior)
        pBhvr = &(pETNC->m_pTimeSetBehavior->m_oBehavior);
    else if (pETNC->m_haveEffectBehavior)
        pBhvr = &(pETNC->m_pTimeEffectBehavior->m_oBehavior);
    else if (pETNC->m_haveAnimateBehavior)
        pBhvr = &(pETNC->m_pTimeAnimateBehavior->m_oBehavior);
    else if (pETNC->m_haveColorBehavior)
        pBhvr = &(pETNC->m_pTimeColorBehavior->m_oBehavior);
    else if (pETNC->m_haveMotionBehavior)
        pBhvr = &(pETNC->m_pTimeMotionBehavior->m_oTimeBehavior);
    else if (pETNC->m_haveRotationBehavior)
        pBhvr = &(pETNC->m_pTimeRotationBehavior->m_oBehavior);
    else if (pETNC->m_haveScaleBehavior)
        pBhvr = &(pETNC->m_pTimeScaleBehavior->m_oBehavior);
    else
        pBhvr = &(pETNC->m_pTimeCommandBehavior->m_oBevavior);

    return pBhvr;
}

_INT32 PPT::Intermediate::GetSpID(CRecordExtTimeNodeContainer *pETNC)
{
    auto pBhvr = GetTimeBehaviorContainer(pETNC);
    if (pBhvr == nullptr)
        return -1;

    if (pBhvr->m_oClientVisualElement.m_bVisualShapeAtom &&
            pBhvr->m_oClientVisualElement.m_oVisualShapeAtom.m_RefType == TL_ET_ShapeType)
        return pBhvr->m_oClientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;

    return -1;
}
