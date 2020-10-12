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
#include "../Reader/Records.h"
#include "Animations/_includer.h"
#include "HeadersFootersAtom.h"
#include "Animations/ExtTimeNodeContainer.h"
#include "Animations/SubEffectContainer.h"

#define MIN_SLIDE_TIME	5000.0


namespace PPT_FORMAT
{


class CSlideTimeLine
{
public:

    CSlideTimeLine ()
    {
        m_dEffectDuration		=	0.0;

        m_nMediaID				=	-1;
        m_nMediaShapeID			=	-1;
        m_nMediaPush			=	0;
    }

    ~CSlideTimeLine()
    {
        Clear ();
    }

    inline bool				Build (CRecordExtTimeNodeContainer* pContainer)
    {
        if (pContainer)
        {
            Clear ();

            m_bSaveEffect		=	false;
            m_HaveAfterEffect	=	false;

            m_oParagraphBuilds.Create(pContainer);

            ExploreTree ( pContainer );

            UpdateParagraph ();

            return (0 != m_oAnimation.size());
        }

        return false;
    }

    inline EffectsMap& GetAnimation ()
    {
        return m_oAnimation;
    }

    inline double GetTime ()
    {
        double dTime	=	m_oNodeTiming.GetTimeLine ();
        if (dTime < MIN_SLIDE_TIME)
            return MIN_SLIDE_TIME;

        if (((long)dTime) % 1000)
            return ( ((long)((long)(dTime) / 1000) + 1 ) * 1000.0 );

        return dTime;
    }

private:
    inline void	ExploreTree (CRecordExtTimeNodeContainer* pContainer)
    {
        unsigned long nNodeType =	pContainer->GetEffectNodeType ();
        if (nNodeType)
        {
            if (CNodeTiming::MainSequenceNode != nNodeType && CNodeTiming::TimingRootNode != nNodeType)
            {
                m_oTopEffect =	CreateEffectFromNode ( pContainer->timePropertyList );

                m_ComposeEffectMothionPath	=	_T("");

                FindEffectMothionPath ( pContainer );

                // время старта анимации
                if ( pContainer->timeCondition.size() )
                {
                    double dTime	=	static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay;
                    m_oNodeTiming.SetTimeDelay (dTime);

                    //m_oTopEffect.m_nBeginTime	=	static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay;

                    m_bSaveEffect	=	false;
                }

                // setup accelerate - decelerate

                if (pContainer->rgTimeModifierAtom.size())
                {
                    if (pContainer->GetModifier(3))
                        m_oTopEffect.m_dTimeAccel			=	(std::max)((std::min)(pContainer->GetModifier(3)->value,1.0f),0.0f);

                    if (pContainer->GetModifier(4))
                        m_oTopEffect.m_dTimeDecel			=	(std::max)((std::min)(pContainer->GetModifier(4)->value,1.0f),0.0f);
                }

                if (pContainer->haveSlaveContainer)
                {
                    CRecordSubContainer* pSlave	= pContainer->rgSlave;
                    if(pSlave->haveSetBehavior)				// после анимации к объекту может быть применена дополнительная анимация
                    {
                        m_HaveAfterEffect	 = true;

                        // эффект исчезновения элемента (Appear)
                        m_oAfterEffect.m_nDuration			=	1.0;
                        m_oAfterEffect.m_nEffectID			=	1;
                        m_oAfterEffect.m_nEffectNodeType	=	1;
                        m_oAfterEffect.m_nEffectType		=	2;

                        if (pSlave->timeSetBehavior)
                        {
                            if (pSlave->timeSetBehavior->behavior.clientVisualElement.m_bVisualShapeAtom)
                                m_oAfterEffect.m_nRefID		=	pSlave->timeSetBehavior->behavior.GetObjectID ();
                        }

                        if (pSlave->extTimeContainer)
                        {
                            ExploreTree (static_cast<ExtTimeNodeContainer*>(pSlave->extTimeContainer));
                        }
                    }
                }

                ProcessMediaCall (pContainer);
            }
        }

        int nID =	GetShapeID ( pContainer );
        if ( -1 != nID )
        {
            //	TODO :
            //	нужно из всех объектов (*Behavior) для конкретного эффекта,
            //	который заранее прочитан в m_oTopEffect дополнять параметры из потомков

            if ( false == m_bSaveEffect )
            {
                AddAnimation ( pContainer, nID );

                m_bSaveEffect	=	true;
            }
        }

        for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
        {
            if ( (WORD)RT_TimeExtTimeNodeContainer	==	pContainer->rgExtTimeNodeChildren [i]->m_oHeader.RecType )
            {
                Animations::ExtTimeNodeContainer* pChild = static_cast<Animations::ExtTimeNodeContainer*> ( pContainer->rgExtTimeNodeChildren [i] );
                if ( pChild )
                    ExploreTree ( pChild );
            }
        }
    }

    //
    inline void	AddAnimation (ExtTimeNodeContainer* pTimeNode, int nID)
    {
        CalculateTimeEffect (pTimeNode);

        Effect oEffect						=	m_oTopEffect;

        oEffect.m_nRefID					=	nID;
        oEffect.m_nBeginTime				=	m_oNodeTiming.GetTop ().dTime;
        oEffect.m_nDuration					=	m_oNodeTiming.GetTop ().dDuration;

        if (pTimeNode->timeMotionBehavior)
        {
            oEffect.m_MotionPath			=	pTimeNode->timeMotionBehavior->m_VarPath.stringValue;
        }

        if (pTimeNode->haveScaleBehavior)		//	GrowAndShrinkEffect	=	6	//	простое увеличение
        {
            oEffect.m_dSX					=	pTimeNode->timeScaleBehavior->scaleBehaviorAtom.fXBy * 0.01;
            oEffect.m_dSY					=	pTimeNode->timeScaleBehavior->scaleBehaviorAtom.fYBy * 0.01;
        }

        if (pTimeNode->haveRotationBehavior)	//	SpinEffect			=	8	//	вращение
        {
            oEffect.m_dRotateAngle			=	pTimeNode->timeRotationBehavior->rotationBehaviorAtom.fBy;
            //oEffect.m_nRotateDirection	=	pTimeNode->timeRotationBehavior->rotationBehaviorAtom.rotationDirection;
        }

        if (pTimeNode->haveSetBehavior)
        {
            if (9 == oEffect.m_nEffectID)		//	TransparencyEffect	=	9,	// временная прозрачность
                oEffect.m_dTransparency		=	_tstof (pTimeNode->timeSetBehavior->varTo.stringValue.c_str());
        }

        if (m_ComposeEffectMothionPath.length())
        {
            oEffect.m_MotionPath			=	m_ComposeEffectMothionPath;		//	составной эффект может иметь траекторию для движения
        }

        if(pTimeNode->haveColorBehavior)
        {
            oEffect.m_nSchemeColor			=	pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.model;

            if (0 == pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.model)
            {
                oEffect.m_nColorTo			=	pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.FRGB();
            }

            // TODO : HSL
            // TODO : ColorDirection

            if (2 == pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.model)
            {
                oEffect.m_nColorTo			=	pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.red;	//	index from table
            }
        }

        if (MediaCallEffect == oEffect.m_nEffectType)
        {
            if (1 == oEffect.m_nEffectID)	//	PLAY
                oEffect.m_nMediaCMD				=	1;

            if (2 == oEffect.m_nEffectID)	//	PAUSE
                oEffect.m_nMediaCMD				=	2;

            if (3 == oEffect.m_nEffectID)	//	STOP
                oEffect.m_nMediaCMD				=	0;
        }

        // oEffect.m_nTextSequence			=	m_oParagraphBuilds.GetBuild(nID);

        AddEffectTopMap (oEffect, nID);
        AddAfterEffect	(nID);

        m_oTopEffect.m_dTimeAccel		=	0.0;
        m_oTopEffect.m_dTimeDecel		=	0.0;
        m_oTopEffect.m_bIgnoreShape		=	false;
    }

    inline void	AddAfterEffect (long nID)
    {
        if (m_HaveAfterEffect)
        {
            if (-1 == m_oAfterEffect.m_nRefID)
            {
                m_oAfterEffect.m_nRefID	=	nID;
            }

            m_oAfterEffect.m_nDuration	=	1.0;
            m_oAfterEffect.m_nBeginTime	=	m_oNodeTiming.GetTop ().dTime + m_oNodeTiming.GetTop ().dDuration;

            AddEffectTopMap (m_oAfterEffect,nID);

            m_oAfterEffect.m_nRefID		=	-1;
            m_HaveAfterEffect			=	false;
        }
    }
    inline void	AddEffectTopMap (Effect& oEffect, long nID)
    {
        oEffect.m_bRemoveEmptyBlocks	=	true;	//	ALWAYS

        EffectToMap (oEffect);
    }
    inline void EffectToMap (const Effect& oEffect)
    {
        EffectsMap::const_iterator mSearch	=	m_oAnimation.find(oEffect.m_nRefID);
        Effects* pEffects = NULL;
        if (mSearch == m_oAnimation.end())
        {
            pEffects = new Effects ();
            m_oAnimation.insert(std::pair<_UINT32,Effects*>( oEffect.m_nRefID, pEffects));
        }else
            pEffects = mSearch->second;

        if (pEffects)
            pEffects->push_back ( oEffect );
    }

    //
    inline Effect CreateEffectFromNode (TimePropertyList4TimeNodeContainer* pContainer)
    {
        Effect oEffect;

        if (pContainer)
        {
            oEffect.m_nEffectID			=	pContainer->m_EffectID.m_Value;
            oEffect.m_nEffectType		=	pContainer->m_EffectType.m_Value;
            oEffect.m_nEffectDir		=	pContainer->m_EffectDir.m_Value;
            oEffect.m_nEffectNodeType	=	pContainer->m_EffectNodeType.m_Value;
        }

        return oEffect;
    }

    inline int GetShapeID (CRecordExtTimeNodeContainer* pContainer)
    {
        if (TL_TNT_Behavior == pContainer->timeNodeAtom.m_dwType)
        {
            if (pContainer->timeMotionBehavior)
            {
                if (pContainer->timeMotionBehavior->m_oBehavior.clientVisualElement.m_bVisualShapeAtom)
                {
                    ReadPropertyIgnoreShape (pContainer->timeMotionBehavior->m_oBehavior);
                    return pContainer->timeMotionBehavior->m_oBehavior.GetObjectID ();
                }
            }

            if (pContainer->timeEffectBehavior)
            {
                if (pContainer->timeEffectBehavior->m_oBehavior.clientVisualElement.m_bVisualShapeAtom)
                {
                    ReadPropertyIgnoreShape (pContainer->timeEffectBehavior->m_oBehavior);
                    return pContainer->timeEffectBehavior->m_oBehavior.GetObjectID ();
                }
            }

            if (pContainer->timeAnimateBehavior)
            {
                if (pContainer->timeAnimateBehavior->m_oBehavior.clientVisualElement.m_bVisualShapeAtom)
                {
                    ReadPropertyIgnoreShape (pContainer->timeAnimateBehavior->m_oBehavior);
                    return pContainer->timeAnimateBehavior->m_oBehavior.GetObjectID ();
                }
            }

            if (pContainer->timeSetBehavior)
            {
                if (pContainer->timeSetBehavior->behavior.clientVisualElement.m_bVisualShapeAtom)
                {
                    ReadPropertyIgnoreShape (pContainer->timeSetBehavior->behavior);
                    return pContainer->timeSetBehavior->behavior.GetObjectID ();
                }
            }

            if (pContainer->timeScaleBehavior)
            {
                if ( pContainer->timeScaleBehavior->behavior.clientVisualElement.m_bVisualShapeAtom )
                {
                    ReadPropertyIgnoreShape (pContainer->timeScaleBehavior->behavior);
                    return pContainer->timeScaleBehavior->behavior.GetObjectID ();
                }
            }

            if (pContainer->timeRotationBehavior)
            {
                if ( pContainer->timeRotationBehavior->behavior.clientVisualElement.m_bVisualShapeAtom )
                {
                    ReadPropertyIgnoreShape (pContainer->timeRotationBehavior->behavior);
                    return pContainer->timeRotationBehavior->behavior.GetObjectID ();
                }
            }

            if (pContainer->timeColorBehavior)
            {
                if ( pContainer->timeColorBehavior->behavior.clientVisualElement.m_bVisualShapeAtom )
                {
                    ReadPropertyIgnoreShape (pContainer->timeColorBehavior->behavior);
                    return pContainer->timeColorBehavior->behavior.GetObjectID ();
                }
            }
        }

        return -1;
    }


    inline void CalculateTimeEffect (ExtTimeNodeContainer* pContainer)
    {
        m_dEffectDuration			=	pContainer->timeNodeAtom.m_nDuration;

        MediumEffectDuration ( pContainer );		//	получаем для все временные интервалы данного эффекта
        MediumEffectTimeDelay ( pContainer );		//	получаем все внутренние метки для данного эффткта

        if (MediaCallEffect == m_oTopEffect.m_nEffectType)
        {
            if (!pContainer->rgExtTimeNodeChildren.empty())
            {
                ExtTimeNodeContainer* pChild = pContainer->rgExtTimeNodeChildren[0];
                if (pChild)
                {
                    if (pChild->timeNodeAtom.m_bDurationProperty)
                    {
                        m_dEffectDuration =	pChild->timeNodeAtom.m_nDuration;
                    }
                }
            }
        }

        if ((int)m_arDurations.size() > 1)
        {
            m_dEffectDuration		=	m_arDurations[1];

            if (FadeExitType == m_oTopEffect.m_nEffectType)
                m_dEffectDuration	=	m_arDurations[0];
        }

        // TODO : для составных эффектов посчитаем время с помощью заглушек

        if (/*	BlindsEffect		*/( m_oTopEffect.m_nEffectID ==	3	||
            /*	BoxEffect			*/	m_oTopEffect.m_nEffectID ==	4	||
            /*	CheckBoardEffect	*/	m_oTopEffect.m_nEffectID ==	5	||
            /*	CircleEffect		*/	m_oTopEffect.m_nEffectID ==	6	||
            /*	DiamondEffect		*/	m_oTopEffect.m_nEffectID ==	8	||
            /*	DissolveEffect		*/	m_oTopEffect.m_nEffectID ==	9	||
            /*	PeekEffect			*/	m_oTopEffect.m_nEffectID ==	12	||
            /*	PlusEffect			*/	m_oTopEffect.m_nEffectID ==	13	||
            /*	RandomBarsEffect	*/	m_oTopEffect.m_nEffectID ==	14	||
            /*	SplitEffect			*/	m_oTopEffect.m_nEffectID ==	16	||
            /*	StripsEffect		*/	m_oTopEffect.m_nEffectID ==	18	||
            /*	WedgeEffect			*/	m_oTopEffect.m_nEffectID ==	20	||
            /*	WheelEffect			*/	m_oTopEffect.m_nEffectID ==	21	||
            /*	WipeEffect			*/	m_oTopEffect.m_nEffectID ==	22 ) && 2 == m_oTopEffect.m_nEffectType )
        {
            m_dEffectDuration	=	 m_arDurations[0];
        }

        if (43 == m_oTopEffect.m_nEffectID)			//	CenterRevolveEffect
        {
            if (5 == m_arDurations.size())
                m_dEffectDuration	=	m_arDurations[0]	+ m_arDurations[1] + m_arDurations[3];
        }

        if (45 == m_oTopEffect.m_nEffectID)			//	FadedSwivel
        {
        }

        if (55 == m_oTopEffect.m_nEffectID)			//	Expand
        {
        }

        if (52 == m_oTopEffect.m_nEffectID)			//	RiseUp
        {
        }

        if (30 == m_oTopEffect.m_nEffectID)			//	Float
        {
            if ((int)m_arDurations.size() > 5)
                m_dEffectDuration	=	 m_arDurations[1]	+ m_arDurations[5];
        }

        if (26 == m_oTopEffect.m_nEffectID)
        {
            if (!m_arDurations.empty() && !m_arBeginTimes.empty() )
                m_dEffectDuration	=	m_arDurations.at(m_arDurations.size()-1) + m_arBeginTimes.at(m_arBeginTimes.size()-1);
        }

        if ( 32 == m_oTopEffect.m_nEffectID && 3 == m_oTopEffect.m_nEffectType )		//	Teeter Effect
        {
            if ((int)m_arDurations.size())
                m_dEffectDuration	=	 m_arDurations[0] * 10.0;

        }

        if ( 26 == m_oTopEffect.m_nEffectID && 3 == m_oTopEffect.m_nEffectType )		//	FlashBulb Effect
        {
            if ((int)m_arDurations.size() >= 2)
                m_dEffectDuration	=	 m_arDurations[0];
        }

        //debug::tracearrf (m_arDurations);
        //debug::tracearrf (m_arBeginTimes);

        m_arDurations.clear();
        m_arBeginTimes.clear();

        m_oNodeTiming.Push ( m_dEffectDuration, m_oTopEffect.m_nEffectNodeType );
    }

    inline bool MediumEffectDuration (ExtTimeNodeContainer* pContainer)
    {
        if ( pContainer->timeNodeAtom.m_bDurationProperty )
        {
            m_arDurations.push_back ( pContainer->timeNodeAtom.m_nDuration );

            for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
            {
                if ( (WORD)RT_TimeExtTimeNodeContainer	==	pContainer->rgExtTimeNodeChildren [i]->m_oHeader.RecType )
                    MediumEffectDuration ( pContainer->rgExtTimeNodeChildren [i] );
            }
        }

        return true;
    }

    inline bool MediumEffectTimeDelay (ExtTimeNodeContainer* pContainer)
    {
        if ( pContainer->timeNodeAtom.m_bDurationProperty )
        {
            if ( pContainer->timeCondition.size() )
            {
                m_arBeginTimes.push_back( static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay );
            }

            for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
            {
                if ( (WORD)RT_TimeExtTimeNodeContainer	==	pContainer->rgExtTimeNodeChildren [i]->m_oHeader.RecType )
                    MediumEffectTimeDelay ( pContainer->rgExtTimeNodeChildren [i] );
            }
        }

        return true;
    }
    inline bool FindEffectMothionPath (ExtTimeNodeContainer* pContainer)
    {
        // сложные эффект может содержать в себе перемещение по траектории
        if (52 == m_oTopEffect.m_nEffectID)			//	ArcUp
        {
            if ( pContainer->haveMotionBehavior )
            {
                m_ComposeEffectMothionPath	=	pContainer->timeMotionBehavior->m_VarPath.stringValue;
                // debug::trace (m_ComposeEffectMothionPath);
                return true;
            }

            for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
                FindEffectMothionPath ( pContainer->rgExtTimeNodeChildren [i] );

            return true;
        }

        return false;
    }

    inline void ReadPropertyIgnoreShape (const TimeBehaviorContainer& container)
    {
        if (2 == container.clientVisualElement.m_oVisualShapeAtom.m_Type)										//	указывает на то что анимация применяется к тексту
        {
            m_oTopEffect.m_bIgnoreShape	 = true;
            m_oTopEffect.m_nTextSequence		=	container.clientVisualElement.m_oVisualShapeAtom.m_nData1;	//	номер параграфа - не нормальзован
            m_oTopEffect.m_bRemoveEmptyBlocks	=	true;

            for (size_t i = 0; i < m_arParIndexer.size(); ++i)
            {
                if (m_oTopEffect.m_nTextSequence == m_arParIndexer.at(i))
                    return;
            }

            m_arParIndexer.push_back (m_oTopEffect.m_nTextSequence);
        }
    }

    inline bool UpdateParagraph ()
    {
        if (!m_arParIndexer.empty())
        {
            //ATLTRACE ( _T("====================PARAGRAPH==========================================\n"));
            //qsort ( m_arParIndexer.GetData(), m_arParIndexer.size(), sizeof (long), (int(*)(const void*, const void*))sort );

            sort(m_arParIndexer.begin(), m_arParIndexer.end());

            for (std::map<_UINT32, Effects*>::iterator pPair = m_oAnimation.begin(); pPair != m_oAnimation.end(); ++pPair)
            {
                Effects* arEffects = pPair->second;
                if (arEffects)
                {
                    for (size_t i = 0; i < arEffects->size(); ++i)
                    {
                        Effect& oEffect = arEffects->at(i);
                        if (oEffect.m_nTextSequence >= 0)
                        {
                            for (size_t ind = 0; ind < m_arParIndexer.size(); ++ind)
                            {
                                if (oEffect.m_nTextSequence == m_arParIndexer.at(ind))
                                {
                                    oEffect.m_nTextSequence = ind;

                                    break;
                                }
                            }
                        }
                    }
                }
            }

            return true;
        }

        return false;
    }

    //
    inline void ProcessMediaCall (ExtTimeNodeContainer* pContainer)
    {
        if (MediaCallEffect == m_oTopEffect.m_nEffectType)		//	если анимация применена к VIDEO или AUDIO элементу
        {
            m_nMediaPush		=	GetAttachedShapeToVideo (pContainer);	//	если к видео добавлена картинка, надо учитывать смещение при поиск ID

            if (GetMediaID (pContainer))
            {
                AddAnimation (pContainer, m_nMediaID);
                m_bSaveEffect	=	true;
            }

            m_arrMedia.clear ();
        }
    }

    inline int GetMediaID (ExtTimeNodeContainer* pContainer)
    {
        m_nMediaID			=	-1;

        if (TL_TNT_Media == pContainer->GetNodeType ())
        {
            if (pContainer->clientVisualElement)
            {
                m_nMediaID	=	pContainer->clientVisualElement->m_oVisualShapeAtom.m_nObjectIdRef;

                if (0 == m_nMediaPush)
                    return true;

                --m_nMediaPush;
            }
        }

        for (size_t i = 0; i < pContainer->rgExtTimeNodeChildren.size(); ++i )
        {
            if (GetMediaID(pContainer->rgExtTimeNodeChildren [i]))
                return true;
        }

        return false;
    }

    inline bool GetAttachedShapeToVideo (ExtTimeNodeContainer* pContainer)
    {
        if (TL_TNT_Media == pContainer->GetNodeType ())																//	нод типа Media
        {
            if (pContainer->clientVisualElement)
            {
                m_arrMedia.push_back (pContainer->clientVisualElement->m_oVisualShapeAtom.m_nObjectIdRef);

                if (2 == (int)m_arrMedia.size())																//	у видео есть статическое изображение
                    return true;
            }
        }

        for (size_t i = 0; i < pContainer->rgExtTimeNodeChildren.size(); ++i )
        {
            if (CNodeTiming::AfterPreviousNode == pContainer->rgExtTimeNodeChildren [i]->GetEffectNodeType ())		//	нод с описанием эффекта
                return false;

            if (GetAttachedShapeToVideo(pContainer->rgExtTimeNodeChildren [i]))
                return true;
        }

        return false;
    }

    //
    inline void Clear ()
    {
        for (std::map<_UINT32,Effects*>::iterator pPair = m_oAnimation.begin(); pPair != m_oAnimation.end(); ++pPair)
        {
            RELEASEOBJECT(pPair->second);
        }

        m_oAnimation.clear();
    }


private:

    Effect				m_oAfterEffect;

    bool				m_bSaveEffect;
    Effect				m_oTopEffect;
    CNodeTiming			m_oNodeTiming;

    double				m_dEffectDuration;
    TimeArray			m_arDurations;
    TimeArray			m_arBeginTimes;

    std::wstring		m_ComposeEffectMothionPath;
    bool				m_HaveAfterEffect;

    EffectsMap			m_oAnimation;
    CParagraphBuilds	m_oParagraphBuilds;

    std::vector <_INT32>	m_arParIndexer;

    // media
    int					m_nMediaID;
    int					m_nMediaShapeID;
    int					m_nMediaPush;					//	глубина поиска

    std::vector <_INT32> m_arrMedia;
};



// TODO this struct
struct PP9SlideBinaryTagExtension : public CRecordsContainer
{
public:
    PP9SlideBinaryTagExtension()
    {

    }

    ~PP9SlideBinaryTagExtension()
    {

    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;

        CUnknownRecord::ReadFromStream(m_oHeader, pStream);
    }
};

struct PP10SlideBinaryTagExtension : public CUnknownRecord
{
public:

    PP10SlideBinaryTagExtension ()
    {
        extTimeNodeContainer	=	NULL;
        buildListContainer		=	NULL;
    }

    virtual ~PP10SlideBinaryTagExtension ()
    {
        RELEASEOBJECT (extTimeNodeContainer);
        RELEASEOBJECT (buildListContainer);
    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;

        LONG lPos			=	0;
        StreamUtils::StreamPosition ( lPos, pStream );

        UINT lCurLen		=	0;

        SRecordHeader ReadHeader;

        while ( lCurLen < m_oHeader.RecLen )
        {
            if ( ReadHeader.ReadFromStream(pStream) == false)
                break;

            lCurLen += 8 + ReadHeader.RecLen;

            if (RT_TimeExtTimeNodeContainer == ReadHeader.RecType)
            {
                extTimeNodeContainer = new CRecordExtTimeNodeContainer ();
                if (extTimeNodeContainer)
                {
                    extTimeNodeContainer->ReadFromStream  ( ReadHeader, pStream );
                    continue;
                }
            }

            if (RT_BuildList == ReadHeader.RecType)
            {
                buildListContainer = new CRecordBuildListContainer ();
                if (buildListContainer)
                {
                    buildListContainer->ReadFromStream  ( ReadHeader, pStream );
                    continue;
                }
            }

            StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
        }

        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }
public:

    //Animations::SlideTime10Atom			m_oSlideTime10Atom;
    //Animations::HashCode10Atom			m_oHashCode10Atom;

    CRecordExtTimeNodeContainer*	extTimeNodeContainer;	//	ROOT
    CRecordBuildListContainer*		buildListContainer;		//	OPTIONAL
};

struct PP12SlideBinaryTagExtension : public CUnknownRecord
{
public:
    PP12SlideBinaryTagExtension()
    {

    }

    ~PP12SlideBinaryTagExtension()
    {

    }

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oHeader			=	oHeader;

        m_oRoundTripHeaderFooterDefaultsAtom.ReadFromStream(m_oHeader, pStream);
    }

public:
    CRecordRoundTripHeaderFooterDefaults12Atom m_oRoundTripHeaderFooterDefaultsAtom;


};


struct SlideProgTagsContainer : public CUnknownRecord
{
public:

    SlideProgTagsContainer ()
    {

    }


    virtual void ReadFromStream (SRecordHeader & oHeader, POLE::Stream* pStream)
    {
        m_oHeader	=	oHeader;

        LONG lPos = 0;
        StreamUtils::StreamPosition ( lPos, pStream );

        m_oHeaderChild.ReadFromStream(pStream) ;

        if ( RECORD_PROG_BINARY_TAG == m_oHeaderChild.RecType )
        {
            SRecordHeader rgSubRec;

            rgSubRec.ReadFromStream(pStream) ;

            WCHAR Name[9];
            if (sizeof (wchar_t) == 4)
            {
                for (int i = 0; i < 8; i++)
                    Name[i] = (WCHAR)StreamUtils::ReadWORD(pStream);
                Name[8]	=	L'\0';
            }
            else
            {
                pStream->read ((unsigned char*) Name, 16 );
                Name[8]	=	L'\0';
            }


            tagName	=	std::wstring ( Name );

            if ( std::wstring ( L"___PPT9" ) == tagName )
            {
                SRecordHeader rhData;
                rhData.ReadFromStream(pStream) ;
                // TODO PP9SlideBinaryTagExtension
                m_oBinaryTagExtension = new PP9SlideBinaryTagExtension;

                if (rhData.RecType == 0x138B && rhData.RecVersion == 0x0 && rhData.RecInstance == 0x000)	//	RT_BinaryTagDataBlob - 0x138B
                {
                    m_oBinaryTagExtension->ReadFromStream (rhData, pStream);

                }
            } else if ( std::wstring ( L"___PPT10" ) == tagName )
            {
                SRecordHeader rhData;
                rhData.ReadFromStream(pStream) ;
                m_oBinaryTagExtension = new PP10SlideBinaryTagExtension;

                if (rhData.RecType == 0x138B && rhData.RecVersion == 0x0 && rhData.RecInstance == 0x000)	//	RT_BinaryTagDataBlob - 0x138B
                {
                    m_oBinaryTagExtension->ReadFromStream (rhData, pStream);

                }
            } else if ( std::wstring ( L"___PPT12" ) == tagName )
            {
                SRecordHeader rhData;
                rhData.ReadFromStream(pStream) ;
                m_oBinaryTagExtension = new PP12SlideBinaryTagExtension;

                if (rhData.RecType == 0x138B && rhData.RecVersion == 0x0 && rhData.RecInstance == 0x000)	//	RT_BinaryTagDataBlob - 0x138B
                {
                    m_oBinaryTagExtension->ReadFromStream (rhData, pStream);

                }
            }

        }

        StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
    }

    inline CSlideTimeLine* GetTimeLine ()
    {
        if (tagName == L"___PPT10" and
                dynamic_cast<PP10SlideBinaryTagExtension*>(m_oBinaryTagExtension)->extTimeNodeContainer)
        {
            CSlideTimeLine* pTimeLine = new CSlideTimeLine ();
            if (pTimeLine)
            {
                if (pTimeLine->Build(
                            dynamic_cast<PP10SlideBinaryTagExtension*>
                            (m_oBinaryTagExtension)->extTimeNodeContainer))

                    return pTimeLine;

                RELEASEOBJECT (pTimeLine);
            }
        }

        return NULL;
    }

public:

    std::wstring				tagName;

    IRecord*                    m_oBinaryTagExtension;

    SRecordHeader				m_oHeaderChild;
};
}
