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
