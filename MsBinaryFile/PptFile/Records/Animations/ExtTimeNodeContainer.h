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
#pragma once


#include "../../Reader/Records.h"
#include "../../Enums/_includer.h"


#include "TimeAnimateBehaviorContainer.h"
#include "TimeColorBehaviorContainer.h"
#include "TimeEffectBehaviorContainer.h"
#include "TimeMotionBehaviorContainer.h"
#include "TimeRotationBehaviorContainer.h"
#include "TimeScaleBehaviorContainer.h"
#include "TimeSetBehaviorContainer.h"
#include "TimeCommandBehaviorContainer.h"
#include "ClientVisualElementContainer.h"
#include "TimeIterateDataAtom.h"
#include "TimeSequenceDataAtom.h"
#include "TimeConditionContainer.h"
#include "TimeModifierAtom.h"
#include "SubEffectContainer.h"


namespace PPT
{

class CRecordExtTimeNodeContainer : public CUnknownRecord
{
public:

    /*-------------------------------------- record --------------------------------- */

    CRecordTimeNodeAtom                             m_oTimeNodeAtom;
    //
    CRecordTimePropertyList4TimeNodeContainer*		m_pTimePropertyList;			// OPTIONAL
    CRecordTimeAnimateBehaviorContainer*			m_pTimeAnimateBehavior; 		// OPTIONAL
    CRecordTimeColorBehaviorContainer*				m_pTimeColorBehavior;           // OPTIONAL
    CRecordTimeEffectBehaviorContainer*             m_pTimeEffectBehavior;			// OPTIONAL
    CRecordTimeMotionBehaviorContainer*             m_pTimeMotionBehavior;			// OPTIONAL
    CRecordTimeRotationBehaviorContainer*			m_pTimeRotationBehavior;        // OPTIONAL
    CRecordTimeScaleBehaviorContainer*				m_pTimeScaleBehavior;           // OPTIONAL
    CRecordTimeSetBehaviorContainer*				m_pTimeSetBehavior;             // OPTIONAL
    CRecordTimeCommandBehaviorContainer*            m_pTimeCommandBehavior;         // OPTIONAL
    CRecordClientVisualElementContainer*            m_pClientVisualElement;         // OPTIONAL	-	It MUST exist only if timeNodeAtom.type is TL_TNT_Media

    CRecordTimeIterateDataAtom*                     m_pTimeIterateDataAtom;         // OPTIONAL
    CRecordTimeSequenceDataAtom*					m_pTimeSequenceDataAtom;		// OPTIONAL

    std::vector<CRecordTimeConditionContainer*>     m_arrRgBeginTimeCondition;      // OPTIONAL
    std::vector<CRecordTimeConditionContainer*>     m_arrRgNextTimeCondition;      // OPTIONAL // Same as m_arrRgBeginTimeCondition in ms-ppt
    std::vector<CRecordTimeConditionContainer*>     m_arrRgEndTimeCondition;        // OPTIONAL
    CRecordTimeConditionContainer*                  m_pTimeEndSyncTimeCondition;    // OPTIONAL

    std::vector<CRecordTimeModifierAtom*>			m_arrRgTimeModifierAtom;        // OPTIONAL
    std::vector<CRecordSubEffectContainer*>     	m_arrRgSubEffect;				// OPTIONAL
    std::vector <CRecordExtTimeNodeContainer*>		m_arrRgExtTimeNodeChildren;     // OPTIONAL

    /*-------------------------------------- record --------------------------------- */

    bool									m_haveTimePropertyList;
    bool									m_haveAnimateBehavior;
    bool									m_haveColorBehavior;
    bool									m_haveEffectBehavior;
    bool									m_haveMotionBehavior;
    bool									m_haveRotationBehavior;
    bool									m_haveScaleBehavior;
    bool									m_haveSetBehavior;
    bool                                    m_haveCommandBehavior;
    bool									m_haveClientVisualElement;

    bool									m_haveIterateDataAtom;
    bool									m_haveSequenceAtom;

    bool									m_haveTimeEndSyncTime;

public:

    CRecordExtTimeNodeContainer();
    virtual ~CRecordExtTimeNodeContainer();

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream ) override;

    CRecordTimeModifierAtom* GetModifier (_UINT32 Type);
    int GetNodeType () const;
};

}
