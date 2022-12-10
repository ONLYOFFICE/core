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
