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

    CRecordExtTimeNodeContainer() :
        m_pTimePropertyList(nullptr),
        m_pTimeAnimateBehavior(nullptr),
        m_pTimeColorBehavior(nullptr),
        m_pTimeEffectBehavior(nullptr),
        m_pTimeMotionBehavior(nullptr),
        m_pTimeRotationBehavior(nullptr),
        m_pTimeScaleBehavior(nullptr),
        m_pTimeSetBehavior(nullptr),
        m_pTimeCommandBehavior(nullptr),
        m_pClientVisualElement(nullptr),

        m_pTimeIterateDataAtom(nullptr),
        m_pTimeSequenceDataAtom(nullptr),

        m_pTimeEndSyncTimeCondition(nullptr),


        m_haveTimePropertyList(false),
        m_haveAnimateBehavior(false),
        m_haveColorBehavior(false),
        m_haveEffectBehavior(false),
        m_haveMotionBehavior(false),
        m_haveRotationBehavior(false),
        m_haveScaleBehavior(false),
        m_haveSetBehavior(false),
        m_haveCommandBehavior(false),
        m_haveClientVisualElement(false),

        m_haveIterateDataAtom(false),
        m_haveSequenceAtom(false),

        m_haveTimeEndSyncTime(false)
    {
    }

    virtual ~CRecordExtTimeNodeContainer()
    {
        RELEASEOBJECT(m_pTimePropertyList);
        RELEASEOBJECT(m_pTimeAnimateBehavior);
        RELEASEOBJECT(m_pTimeColorBehavior);
        RELEASEOBJECT(m_pTimeEffectBehavior);
        RELEASEOBJECT(m_pTimeMotionBehavior);
        RELEASEOBJECT(m_pTimeRotationBehavior);
        RELEASEOBJECT(m_pTimeScaleBehavior);
        RELEASEOBJECT(m_pTimeSetBehavior);
        RELEASEOBJECT(m_pTimeCommandBehavior);
        RELEASEOBJECT(m_pClientVisualElement);

        RELEASEOBJECT(m_pTimeIterateDataAtom);
        RELEASEOBJECT(m_pTimeSequenceDataAtom);

        RELEASEOBJECT(m_pTimeEndSyncTimeCondition);

        for ( size_t i = 0; i < m_arrRgBeginTimeCondition.size(); ++i )
            RELEASEOBJECT ( m_arrRgBeginTimeCondition[i] );

        for ( size_t i = 0; i < m_arrRgNextTimeCondition.size(); ++i )
            RELEASEOBJECT ( m_arrRgNextTimeCondition[i] );

        for ( size_t i = 0; i < m_arrRgEndTimeCondition.size(); ++i )
            RELEASEOBJECT ( m_arrRgEndTimeCondition[i] );

        for ( size_t i = 0; i < m_arrRgTimeModifierAtom.size(); ++i )
            RELEASEOBJECT ( m_arrRgTimeModifierAtom[i] );

        for ( size_t i = 0; i < m_arrRgSubEffect.size(); ++i )
            RELEASEOBJECT ( m_arrRgSubEffect[i] );

        for ( size_t i = 0; i < m_arrRgExtTimeNodeChildren.size(); ++i )
            RELEASEOBJECT ( m_arrRgExtTimeNodeChildren[i] );
    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader = oHeader;

        LONG lPos = 0;
        StreamUtils::StreamPosition ( lPos, pStream );

        SRecordHeader	rHeader;

        if ( rHeader.ReadFromStream(pStream) )
            m_oTimeNodeAtom.ReadFromStream ( rHeader, pStream );

        UINT lCurLen	=	m_oTimeNodeAtom.m_oHeader.RecLen + 8;

        SRecordHeader ReadHeader;

        while ( lCurLen < m_oHeader.RecLen )
        {
            LONG lPosExpected = 0;
            StreamUtils::StreamPosition ( lPosExpected, pStream );

            if ( ReadHeader.ReadFromStream(pStream) == false)
                break;

            lCurLen +=	8 + ReadHeader.RecLen;
            lPosExpected +=	8 + ReadHeader.RecLen;

            switch (ReadHeader.RecType)
            {
            case RT_TimePropertyList:
            {

                m_pTimePropertyList = new CRecordTimePropertyList4TimeNodeContainer;
                m_pTimePropertyList->ReadFromStream(ReadHeader, pStream);
                m_haveTimePropertyList = true;

                break;
            }

            case RT_TimeAnimateBehaviorContainer:
            {

                m_pTimeAnimateBehavior = new CRecordTimeAnimateBehaviorContainer;
                m_pTimeAnimateBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveAnimateBehavior = true;

                break;
            }

            case RT_TimeColorBehaviorContainer:
            {

                m_pTimeColorBehavior = new CRecordTimeColorBehaviorContainer;
                m_pTimeColorBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveColorBehavior = true;

                break;
            }

            case RT_TimeEffectBehaviorContainer:
            {

                m_pTimeEffectBehavior = new CRecordTimeEffectBehaviorContainer();
                m_pTimeEffectBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveEffectBehavior = true;

                break;
            }

            case RT_TimeMotionBehaviorContainer:
            {

                m_pTimeMotionBehavior = new CRecordTimeMotionBehaviorContainer();
                m_pTimeMotionBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveMotionBehavior = true;

                break;
            }

            case RT_TimeRotationBehaviorContainer:
            {

                m_pTimeRotationBehavior = new CRecordTimeRotationBehaviorContainer();
                m_pTimeRotationBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveRotationBehavior = true;

                break;
            }

            case RT_TimeScaleBehaviorContainer:
             {

                m_pTimeScaleBehavior = new CRecordTimeScaleBehaviorContainer();
                m_pTimeScaleBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveScaleBehavior = true;

                break;
             }

            case RT_TimeSetBehaviorContainer:
            {

                m_pTimeSetBehavior = new CRecordTimeSetBehaviorContainer();
                m_pTimeSetBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveSetBehavior = true;

                break;
            }

            case RT_TimeCommandBehaviorContainer:
            {

                m_pTimeCommandBehavior = new CRecordTimeCommandBehaviorContainer();
                m_pTimeCommandBehavior->ReadFromStream(ReadHeader, pStream);
                m_haveCommandBehavior = true;

                break;
            }

            case RT_TimeClientVisualElement:
            {

                m_pClientVisualElement = new CRecordClientVisualElementContainer();
                m_pClientVisualElement->ReadFromStream(ReadHeader, pStream);
                m_haveClientVisualElement = true;

                break;
            }

            case RT_TimeIterateData:
            {

                m_pTimeIterateDataAtom = new CRecordTimeIterateDataAtom();
                m_pTimeIterateDataAtom->ReadFromStream(ReadHeader, pStream);
                m_haveIterateDataAtom = true;

                break;
            }

            case RT_TimeSequenceData:
            {

                m_pTimeSequenceDataAtom = new CRecordTimeSequenceDataAtom();
                m_pTimeSequenceDataAtom->ReadFromStream(ReadHeader, pStream);
                m_haveSequenceAtom = true;

                break;
            }

            case RT_TimeConditionContainer:
            {

                CRecordTimeConditionContainer* pTimeCondition =
                        new CRecordTimeConditionContainer();
                pTimeCondition->ReadFromStream(ReadHeader, pStream);
                unsigned short recInst = ReadHeader.RecInstance;

                if (recInst == TL_CT_Begin)
                {
                    m_arrRgBeginTimeCondition.push_back(pTimeCondition);
                }
                else if (recInst == TL_CT_Next)
                {
                    m_arrRgNextTimeCondition.push_back(pTimeCondition);
                }

                else if (recInst == TL_CT_End || recInst == TL_CT_Previous)
                {
                    m_arrRgEndTimeCondition.push_back(pTimeCondition);
                }
                else
                {
                    m_haveTimeEndSyncTime = true;
                    m_pTimeEndSyncTimeCondition = pTimeCondition;
                }


                break; // A lot of records. Look at instance
            }


            case RT_TimeModifier:
            {

                CRecordTimeModifierAtom* pModAtom = new CRecordTimeModifierAtom();
                pModAtom->ReadFromStream(ReadHeader, pStream);
                m_arrRgTimeModifierAtom.push_back(pModAtom);

                break;
            }

            case RT_TimeSubEffectContainer:
            {

                CRecordSubEffectContainer* pSub = new CRecordSubEffectContainer();
                pSub->ReadFromStream(ReadHeader, pStream);
                m_arrRgSubEffect.push_back(pSub);

                break;
            }

            case RT_TimeExtTimeNodeContainer:
            {

                CRecordExtTimeNodeContainer* pExt = new CRecordExtTimeNodeContainer();
                pExt->ReadFromStream(ReadHeader, pStream);
                m_arrRgExtTimeNodeChildren.push_back(pExt);

                break;
            }


            default:
                break;
//                throw ;
            }
            LONG lPosCurrent = 0;
            StreamUtils::StreamPosition ( lPosCurrent, pStream );
            if (lPosExpected != lPosCurrent)
            {
                StreamUtils::StreamSeek ( lPosExpected, pStream );
            }

        }
        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }


    //
    inline CRecordTimeModifierAtom* GetModifier (_UINT32 Type)
    {
        for ( size_t i = 0; i < m_arrRgTimeModifierAtom.size(); ++i )
            if (m_arrRgTimeModifierAtom[i]->m_nType == Type)
                return m_arrRgTimeModifierAtom[i];

        return nullptr;
    }

    inline int GetNodeType () const
    {
        return m_oTimeNodeAtom.m_dwType;
    }


};

}
