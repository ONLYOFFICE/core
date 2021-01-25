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

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Timing/Anim.h"
#include "../../Records/Animations/ExtTimeNodeContainer.h"
#include "CBhvr.h"
#include "../Enums/_includer.h"


namespace PPT_FORMAT
{
    void FillAnim(CRecordExtTimeNodeContainer* pETNC, PPTX::Logic::Anim& oAnim)
    {
        auto& oldAnim = pETNC->m_pTimeAnimateBehavior;

        if (oldAnim->m_oAnimateBehaviorAtom.m_bCalcModePropertyUsed)
        {
            oAnim.calcmode = oldAnim->m_oAnimateBehaviorAtom.m_nCalcMode;
        }

        if (oldAnim->m_oAnimateBehaviorAtom.m_bValueTypePropertyUsed)
        {
            std::wstring valueType;
            switch (oldAnim->m_oAnimateBehaviorAtom.m_ValueType) {
            case TL_TABVT_Color: valueType = L"Color"; break;
            case TL_TABVT_Number: valueType = L"Number"; break;
            case TL_TABVT_String: valueType = L"String"; break;
            }
            if (!valueType.empty())
            {
                oAnim.valueType = new PPTX::Limit::TLValueType;
                oAnim.valueType = valueType;
            }
        }

        // By
        if (oldAnim->m_oAnimateBehaviorAtom.m_bByPropertyUsed)
        {
            oAnim.by = oldAnim->m_oVarBy.m_Value;
        }
        // To
        if (oldAnim->m_oAnimateBehaviorAtom.m_bToPropertyUsed)
        {
            oAnim.to = oldAnim->m_oVarTo.m_Value;
        }
        //From
        if (oldAnim->m_oAnimateBehaviorAtom.m_bFromPropertyUsed)
        {
            oAnim.from = oldAnim->m_oVarFrom.m_Value;
        }

        //// Writing childs

        if (!oldAnim->m_oAnimateValueList.m_arrEntry.empty())
            oAnim.tavLst = new PPTX::Logic::TavLst;
        for (auto& animValue : oldAnim->m_oAnimateValueList.m_arrEntry)
        {
            PPTX::Logic::Tav tav;
            tav.val = new PPTX::Logic::AnimVariant;

            tav.val->node_name = L"val";

            if (animValue->m_pVarValue.is_init())
            switch (animValue->m_pVarValue->m_Type) {
            case TL_TVT_String:
            {
                tav.val->strVal = dynamic_cast<const CRecordTimeVariantString&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            case TL_TVT_Bool:
            {
                tav.val->boolVal = dynamic_cast<const CRecordTimeVariantBool&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            case TL_TVT_Int:
            {
                tav.val->intVal = dynamic_cast<const CRecordTimeVariantInt&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            case TL_TVT_Float:
            {
                tav.val->fltVal = dynamic_cast<const CRecordTimeVariantFloat&>
                        (animValue->m_pVarValue.get()).m_Value;
                break;
            }
            }

            tav.tm = std::to_wstring(
                        animValue->m_oTimeAnimationValueAtom.m_nTime * 100);

            if (!animValue->m_VarFormula.m_Value.empty())
            {
                tav.fmla = animValue->m_VarFormula.m_Value;
            }

            oAnim.tavLst->list.push_back(tav);
    }
        FillCBhvr(pETNC, oAnim.cBhvr);
    }
}
