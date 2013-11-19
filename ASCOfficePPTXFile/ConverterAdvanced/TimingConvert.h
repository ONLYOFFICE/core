#pragma once
#include "Structures.h"

#include "../PPTXFormat/Logic/Timing/Seq.h"
#include "../PPTXFormat/Logic/Timing/Par.h"
#include "../PPTXFormat/Logic/Timing/Audio.h"
#include "../PPTXFormat/Logic/Timing/Video.h"
#include "../PPTXFormat/Logic/Timing/Excl.h"
#include "../PPTXFormat/Logic/Timing/Anim.h"
#include "../PPTXFormat/Logic/Timing/AnimClr.h"
#include "../PPTXFormat/Logic/Timing/AnimEffect.h"
#include "../PPTXFormat/Logic/Timing/AnimMotion.h"
#include "../PPTXFormat/Logic/Timing/AnimRot.h"
#include "../PPTXFormat/Logic/Timing/AnimScale.h"
#include "../PPTXFormat/Logic/Timing/Cmd.h"
#include "../PPTXFormat/Logic/Timing/Set.h"

#include "../PPTXFormat/Logic/Timing/AttrName.h"
#include "../PPTXFormat/Logic/Timing/Cond.h"
#include "../PPTXFormat/Logic/Timing/Tav.h"

using namespace PPTX::Logic;

namespace PPTX2EditorAdvanced
{
	typedef NSPresentationEditor::CAnimationSimple Effect;
	typedef CAtlArray <NSPresentationEditor::CAnimationSimple> Effects;
	typedef CSimpleMap <DWORD,Effects*> EffectsMap;
	typedef CAtlArray <double> TimeArray;
	
	class CIndLoop
	{
	public:
		CIndLoop () : m_nRefID (-1), m_nCount (0)
		{

		}
		
		CIndLoop (long RefID, long Count) : m_nRefID (RefID), m_nCount (Count)
		{

		}
	
		inline long Get () const
		{
			return m_nCount;
		}

		long	m_nRefID;
		long	m_nCount;	//	-1	- end slide, 0 - default, N - count loop
	};
	class CMediaDur
	{
	public:
		CMediaDur () : m_nRefID (-1), m_dDur (0)
		{

		}
		
		CMediaDur (long RefID, long Dur) : m_nRefID (RefID), m_dDur (Dur)
		{

		}

		inline double Get () const
		{
			return m_dDur;
		}

		long	m_nRefID;
		long	m_dDur;	
	};
}

namespace PPTX2EditorAdvanced
{
	class CAnimateEffects
	{
	public:
		
		static const int ClickEffectNode			=	1;
		static const int WithPreviousNode			=	2;
		static const int AfterPreviousNode			=	3;
		static const int MainSequenceNode			=	4;
		static const int InteractiveSequenceNode	=	5; 
		static const int ClickParallelNode			=	6; 
		static const int WithGroupNode				=	7; 
		static const int AfterGroupNode				=	8; 
		static const int TimingRootNode				=	9; 

	public:
		CAnimateEffects ()
		{
			m_bHaveEffect	=	TRUE;
			
			m_dTime			=	0.0;
			m_dAfterTime	=	0.0;
			m_dClickTime	=	0.0;
		}

		~CAnimateEffects ()
		{
			Clear ();
		}

		inline BOOL Build (const PPTX::Logic::Timing& pTiming )
		{
			if(pTiming.tnLst.is_init())
			{
#ifdef _DEBUG
				ATLTRACE(_T("======================================== Slide ========================================\n"));
#endif
				m_bHaveEffect		=	TRUE;
				m_HaveAfterEffect	=	FALSE;
				
				m_dTime				=	0.0;
				m_dAfterTime		=	0.0;
				m_dClickTime		=	0.0;
	
				if (ExploreMainSeqence(pTiming.tnLst.get()))
				{
#ifdef _DEBUG
					ATLTRACE(_T("duration : %f,\n"), GetTime () );
					ATLTRACE(_T("=======================================================================================\n"));
#endif
					return (0 != m_oAnimation.GetSize());
				}
			}

			return FALSE;
		}

		inline EffectsMap& GetAnimation ()
		{
			return m_oAnimation;
		}
		inline double GetTime ()
		{
#define MIN_SLIDE_TIME	5000.0

			if (m_dTime < MIN_SLIDE_TIME)
				return MIN_SLIDE_TIME;

			if (((long)m_dTime) % 1000)
				return ( ((long)((long)(m_dTime) / 1000) + 1 ) * 1000.0 );

			return m_dTime;
		}

		inline long GetLoopFor(long RefID) const
		{
			if (0 == (long)m_arLoop.GetCount())
				return 0;

			for (long i = 0; i < (long)m_arLoop.GetCount(); ++i)
			{
				if (RefID == m_arLoop[i].m_nRefID)
					return m_arLoop[i].Get();
			}

			return 0;
		}

		inline double GetMediaDur (long RefID) const	//	длительность видео/аудио для LOOP задается не описании источника
		{
			if (0 == (long)m_arMediaDur.GetCount())
				return 0.0;

			for (long i = 0; i < (long)m_arMediaDur.GetCount(); ++i)
			{
				if (RefID == m_arMediaDur[i].m_nRefID)
					return m_arMediaDur[i].Get();
			}

			return 0;
		}

	private:
		 
		inline BOOL ExploreMainSeqence (const PPTX::Logic::TnLst& oTimeList)
		{
			for (long i = 0; i < (long)oTimeList.list.GetCount(); ++i)
			{
				ExploreMedia (oTimeList.list[i]);
			}

			for (long i = 0; i < (long)oTimeList.list.GetCount(); ++i)
			{
				m_arBeginTimes.RemoveAll ();
				m_arDurations.RemoveAll ();

				ExploreTimeNode (oTimeList.list[i]);
			}

			return ((long)oTimeList.list.GetCount() > 0);
		}

		inline BOOL ExplorerTree (const PPTX::Logic::TnLst& oTimeList)
		{
			for (long i = 0; i < (long)oTimeList.list.GetCount(); ++i)
				ExploreTimeNode (oTimeList.list[i]);

			return TRUE;
		}

		inline BOOL ExploreTimeNode (const PPTX::Logic::TimeNodeBase& oTimeNode)
		{
			if (oTimeNode.is<PPTX::Logic::Par>())
			{
				if(oTimeNode.is_init())
				{
					CTN2TimeNodeAtom (oTimeNode.as<PPTX::Logic::Par>().cTn);

					if (oTimeNode.as<PPTX::Logic::Par>().cTn.childTnLst.is_init())
					{
						ExplorerTree (*oTimeNode.as<PPTX::Logic::Par>().cTn.childTnLst);

						if (m_oEffect.m_nEffectType)
						{
							if (m_bHaveEffect)
							{
								AddEffect (m_oEffect);	

								m_oEffect.m_dTimeAccel		=	0.0;
								m_oEffect.m_dTimeDecel		=	0.0;
								m_oEffect.m_bIgnoreShape	=	FALSE;
								m_oEffect.m_nTextSequence	=	-1;
								m_oEffect.m_nMediaCMD		=	-1;

								m_bHaveEffect				=	FALSE;
								m_HaveAfterEffect			=	FALSE;
							}

							m_bHaveEffect					=	TRUE;
						}

						return TRUE;
					}
				}
			}

			if (oTimeNode.is<PPTX::Logic::Seq>())
			{
				if(oTimeNode.is_init())
				{
					CTN2TimeNodeAtom(oTimeNode.as<PPTX::Logic::Seq>().cTn);

					if (oTimeNode.as<PPTX::Logic::Seq>().cTn.childTnLst.is_init())
					{
						ExplorerTree (*oTimeNode.as<PPTX::Logic::Seq>().cTn.childTnLst);

						return TRUE;
					}
				}
			}

			if (oTimeNode.is<PPTX::Logic::AnimEffect>())
			{
				if(oTimeNode.is_init())
				{
					CTN2TimeNodeAtom (oTimeNode.as<PPTX::Logic::AnimEffect>().cBhvr.cTn);

					ReadBehavior (oTimeNode.as<PPTX::Logic::AnimEffect>().cBhvr);
					return TRUE;
				}
			}

			if (oTimeNode.is<PPTX::Logic::AnimMotion>())	
			{
				if(oTimeNode.is_init())
				{
					ReadBehavior (oTimeNode.as<PPTX::Logic::AnimMotion>().cBhvr);

					if (oTimeNode.as<PPTX::Logic::AnimMotion>().path.is_init())
					{
						m_oEffect.m_MotionPath		=	oTimeNode.as<PPTX::Logic::AnimMotion>().path.get();

						return TRUE;
					}
				}
			}

			if (oTimeNode.is<PPTX::Logic::Set>())	
			{	
				if(oTimeNode.is_init())
				{
					ReadBehavior (oTimeNode.as<PPTX::Logic::Set>().cBhvr);

					if (9 == m_oEffect.m_nEffectID && EmphasisEffect == m_oEffect.m_nEffectType)
					{
						if (oTimeNode.as<PPTX::Logic::Set>().to.is_init())
							m_oEffect.m_dTransparency	=	_tstof (oTimeNode.as<PPTX::Logic::Set>().to.get().strVal.get());
					}

					return TRUE;
				}
			}
			
			if (oTimeNode.is<PPTX::Logic::AnimScale>())	
			{
				if(oTimeNode.is_init())
				{
					ReadBehavior (oTimeNode.as<PPTX::Logic::AnimScale>().cBhvr);

					if (oTimeNode.as<PPTX::Logic::AnimScale>().byX.is_init())
						m_oEffect.m_dSX				=	((double)oTimeNode.as<PPTX::Logic::AnimScale>().byX.get()) * 0.00001;
					if (oTimeNode.as<PPTX::Logic::AnimScale>().byY.is_init())
						m_oEffect.m_dSY				=	((double)oTimeNode.as<PPTX::Logic::AnimScale>().byY.get()) * 0.00001;

					return TRUE;
				}
			}

			if (oTimeNode.is<PPTX::Logic::AnimRot>())	
			{	
				if(oTimeNode.is_init())
				{
					ReadBehavior (oTimeNode.as<PPTX::Logic::AnimRot>().cBhvr);

					m_oEffect.m_dRotateAngle		=	(double) oTimeNode.as<PPTX::Logic::AnimRot>().by.get() / 60000.0;

					return TRUE;
				}
			}

			if (oTimeNode.is<PPTX::Logic::AnimClr>())	
			{
				if(oTimeNode.is_init())
				{
					ReadBehavior (oTimeNode.as<PPTX::Logic::AnimClr>().cBhvr);

					if (oTimeNode.as<PPTX::Logic::AnimClr>().clrSpc.is_init())
						ReadColor (oTimeNode.as<PPTX::Logic::AnimClr>().to);

					return TRUE;
				}
			}

			if (oTimeNode.is<PPTX::Logic::Anim>())	
			{	
				if(oTimeNode.is_init())
				{
					ReadBehavior (oTimeNode.as<PPTX::Logic::Anim>().cBhvr);
				}
				return TRUE;
			}

			if (oTimeNode.is<PPTX::Logic::Cmd>())
			{
				if(oTimeNode.is_init())
				{
					ReadBehavior (oTimeNode.as<PPTX::Logic::Cmd>().cBhvr);
					
					if (oTimeNode.as<PPTX::Logic::Cmd>().cmd.is_init())
					{
						CString CMD	= oTimeNode.as<PPTX::Logic::Cmd>().cmd.get_value_or(_T(""));
					
						if (CString(_T("togglePause")) == CMD )
							m_oEffect.m_nMediaCMD	=	2;
						if (CString(_T("stop")) == CMD )
							m_oEffect.m_nMediaCMD	=	0;
						if (-1 != CMD.Find(_T("playFrom")) )
							m_oEffect.m_nMediaCMD	=	1;

						ReadLoopDur (oTimeNode.as<PPTX::Logic::Cmd>().cBhvr);
					}
				}

				return TRUE;
			}

			//	if(timeNode.is<PPTX::Logic::Excl>())	{	ATLTRACE(_T("PPTX::Logic::Excl\n"));		}

			return FALSE;
		}

		inline BOOL ExploreMedia (const PPTX::Logic::TimeNodeBase& oTimeNode)
		{
			if(oTimeNode.is_init())
			{
				if (oTimeNode.is<PPTX::Logic::Par>())
				{
					if (oTimeNode.as<PPTX::Logic::Par>().cTn.childTnLst.is_init())
					{
						ExplorerMediaTree (*oTimeNode.as<PPTX::Logic::Par>().cTn.childTnLst);
						return TRUE;
					}
				}

				if (oTimeNode.is<Audio>())
				{
					const Audio& oAudio = oTimeNode.as<Audio>();
					ReadMediaNode (oAudio.cMediaNode);

					return TRUE;
				}

				if (oTimeNode.is<Video>())
				{
					const Video& oVideo = oTimeNode.as<Video>();
					ReadMediaNode (oVideo.cMediaNode);

					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL ExplorerMediaTree (const PPTX::Logic::TnLst& oTimeList)
		{
			for (long i = 0; i < (long)oTimeList.list.GetCount(); ++i)
				ExploreMedia (oTimeList.list[i]);

			return TRUE;
		}

		//
		inline void CTN2TimeNodeAtom(const PPTX::Logic::CTn& oTimeList)
		{
			Effect oEffect;

			if (oTimeList.presetClass.is_init())
				oEffect.m_nEffectType		=	GetTLPresetClassFromStr(oTimeList.presetClass.get_value_or(0));
			if (oTimeList.presetID.is_init())
				oEffect.m_nEffectID			=	oTimeList.presetID.get_value_or(-1);
			if (oTimeList.presetSubtype.is_init())
				oEffect.m_nEffectDir		=	oTimeList.presetSubtype.get_value_or(-1);
			if (oTimeList.nodeType.is_init())
				oEffect.m_nEffectNodeType	=	GetTLNodeTypeFromStr(oTimeList.nodeType.get_value_or(0));
			
			if (oEffect.m_nEffectType)
			{
				m_oEffect					=	oEffect;
			}
			
			if (oTimeList.accel.is_init())		//	ускорение
			{
				m_oEffect.m_dTimeAccel		=	oTimeList.accel.get_value_or(0);
			}
			
			if (oTimeList.decel.is_init())		//	замедление
			{
				m_oEffect.m_dTimeDecel		=	oTimeList.decel.get_value_or(0);
			}

			if (oTimeList.stCondLst.is_init())
			{
				for (long i = 0; i < (long)oTimeList.stCondLst.get().list.GetCount(); ++i)
				{
					double dTime			=	0.0;

					if (oTimeList.stCondLst.get().list[i].delay.get() != _T("indefinite") )
					{
						dTime				=	ParseTLTime(oTimeList.stCondLst.get().list[i].delay.get());
					}

					m_arBeginTimes.Add (dTime);
				}
			}

			if (oTimeList.subTnLst.is_init())
			{
				for (long i = 0; i < (long)oTimeList.subTnLst->list.GetCount(); ++i)
				{
					const PPTX::Logic::TimeNodeBase& oTimeNode = oTimeList.subTnLst->list.GetAt(i);
					if (oTimeNode.is_init())
					{
						if (oTimeNode.is<PPTX::Logic::Set>())	
						{	
							if (oTimeNode.as<PPTX::Logic::Set>().to.is_init())
							{
								if ( HaveAttribute (oTimeNode.as<PPTX::Logic::Set>().cBhvr, CString(_T("style.visibility")) ) )
								{
									if (CString(_T("hidden")) == oTimeNode.as<PPTX::Logic::Set>().to->strVal.get())
									{
										m_HaveAfterEffect					=	TRUE;

										// эффект исчезновения элемента (Appear) 
										m_oAfterEffect.m_nDuration			=	1.0;
										m_oAfterEffect.m_nEffectID			=	1;									
										m_oAfterEffect.m_nEffectNodeType	=	1;
										m_oAfterEffect.m_nEffectType		=	2;
									}
								}
							}
						}
					}
				}
			}
		}

		inline BOOL ReadBehavior (const PPTX::Logic::CBhvr& oBehavior)
		{
			if (oBehavior.cTn.dur.is_init())
			{
				m_arDurations.Add ( ParseTLTime(oBehavior.cTn.dur.get_value_or( _T("indefinite") )) );
			}

			if (oBehavior.tgtEl.spTgt.is_init() )
			{
				m_oEffect.m_nRefID	=	_tstol ( oBehavior.tgtEl.spTgt->spid );

				if (oBehavior.tgtEl.spTgt->txEl.is_init())	//	for paragraph
				{
					if (oBehavior.tgtEl.spTgt->txEl->st.is_init())
					{
						m_oEffect.m_nTextSequence	=	(int)oBehavior.tgtEl.spTgt->txEl->st.get_value_or(-1);
					}

					m_oEffect.m_bIgnoreShape		=	TRUE;
				}
			}

			return TRUE;
		}

		inline void ReadColor (const PPTX::Logic::UniColor& oColor)
		{
			m_oEffect.m_nSchemeColor				=	0;
			m_oEffect.m_nColorTo					=	oColor.GetABGR();

			if(oColor.is<PPTX::Logic::SchemeClr>())
			{
				m_oEffect.m_nSchemeColor			=	2;
				m_oEffect.m_nColorTo				=	GetColorIndexFromStr(oColor.as<PPTX::Logic::SchemeClr>().val.get());
			}
		}

		inline void ReadMediaNode (const CMediaNode& oMedia)
		{
			if (oMedia.cTn.repeatCount.is_init())
			{
				CIndLoop oLoop;

				CString str = oMedia.cTn.repeatCount.get();
				if (str == _T("indefinite"))
					oLoop.m_nCount	=	-1;
				else
					oLoop.m_nCount	=	ParseTLTime (str);

				if (oMedia.tgtEl.spTgt.is_init())
				{
					oLoop.m_nRefID	=	XmlUtils::GetInteger(oMedia.tgtEl.spTgt->spid);
					if (-1 != oLoop.m_nRefID)
					{
						for (long i = 0; i < (long)m_arLoop.GetCount(); ++i)
						{
							if ((oLoop.m_nRefID == m_arLoop[i].m_nRefID))
								return;
						}

						m_arLoop.Add (oLoop);
					}
				}					
			}
		}

		inline void ReadLoopDur (const PPTX::Logic::CBhvr& oBehavior)
		{
			if (oBehavior.cTn.dur.is_init())
			{
				CMediaDur oLoop;

				CString str = oBehavior.cTn.dur.get();
				if (str == _T("indefinite"))
					oLoop.m_dDur	=	0;
				else
					oLoop.m_dDur	=	ParseTLTime (str);

				if (oBehavior.tgtEl.spTgt.is_init())
				{
					oLoop.m_nRefID	=	XmlUtils::GetInteger(oBehavior.tgtEl.spTgt->spid);
					if (-1 != oLoop.m_nRefID)
					{
						for (long i = 0; i < (long)m_arMediaDur.GetCount(); ++i)
						{
							if ((oLoop.m_nRefID == m_arMediaDur[i].m_nRefID))
								return;
						}

						m_arMediaDur.Add (oLoop);
					}
				}					
			}
		}

		//
		inline BOOL AddEffect (const Effect& oSrcEffect)
		{
			Effect oEffect	=	oSrcEffect;

			if (0 == (long)m_arDurations.GetCount())
				return FALSE;

			oEffect.m_nDuration			=	GetDuration (oSrcEffect);
			oEffect.m_nBeginTime		=	GetBeginTime (oSrcEffect);

			if (0.0 == oEffect.m_nDuration)
			{
				ATLTRACE (_T("NOT CORRECT EFFECT"));
				return FALSE;
			}

			if (oEffect.m_nDuration > 0.0)
				oEffect.m_dTimeAccel	=	(oEffect.m_dTimeAccel / 100000.0) * oEffect.m_nDuration;
			
			if (oEffect.m_nDuration > 0.0)
				oEffect.m_dTimeDecel	=	(oEffect.m_dTimeDecel / 100000.0) * oEffect.m_nDuration;

			m_dTime						=	max (m_dTime, oEffect.m_nBeginTime + oEffect.m_nDuration);

			if (ClickEffectNode == oEffect.m_nEffectNodeType)
			{
				RefreshMediaMap (oEffect.m_nBeginTime);
			}

#ifdef _DEBUG	
			//ATLTRACE (_T("accel : %f, decel : %f\n"),oEffect.m_dTimeAccel, oEffect.m_dTimeDecel); 
			//TRACE_ARR (_T("timing : "), m_oTimeDelay.m_arrEffects);
			//TRACE_ARR (_T("delay : "), m_arBeginTimes);
			//TRACE_ARR (_T("duration : "), m_arDurations);
#endif
			m_arBeginTimes.RemoveAll ();
			m_arDurations.RemoveAll ();

			EffectToMap (oEffect);
			AddAfterEffect (oEffect);

#ifdef _DEBUG
			//ATLTRACE ( _T("REF : %d, EffectType : %d, EffectID : %d, EffectDir : %d, Group : %d, Begin : %f, Dur : %f, ShapeIgnore : %d, TextBlock : %d\n"), 
			//	oEffect.m_nRefID, oEffect.m_nEffectType, oEffect.m_nEffectID, oEffect.m_nEffectDir, oEffect.m_nEffectNodeType, oEffect.m_nBeginTime, oEffect.m_nDuration, oEffect.m_bIgnoreShape, oEffect.m_nTextSequence);
			ATLTRACE (_T("REF : %d, [ %f, %f ]\n"), oEffect.m_nRefID, oEffect.m_nBeginTime, oEffect.m_nDuration);
#endif
			return TRUE;
		}

		inline void	AddAfterEffect (const Effect& oEffect)
		{
			if (m_HaveAfterEffect)
			{
				m_HaveAfterEffect			=	FALSE;

				m_oAfterEffect.m_nRefID		=	oEffect.m_nRefID;
				m_oAfterEffect.m_nBeginTime	=	oEffect.m_nBeginTime + oEffect.m_nDuration;
				m_oAfterEffect.m_nDuration	=	1.0;

				EffectToMap (m_oAfterEffect);
			}
		}
		inline void EffectToMap (const Effect& oEffect, BOOL bSaveLoopElements = TRUE)
		{
			if (bSaveLoopElements)
			{
				if (0 != GetLoopFor (oEffect.m_nRefID))	
				{
					if (6 == oEffect.m_nEffectType )
					{
						Effects* pEff	=	m_oMediaMap.Lookup (oEffect.m_nRefID);
						if (NULL == pEff)
						{
							pEff		=	new Effects ();
							m_oMediaMap.Add ( oEffect.m_nRefID, pEff );	
						}

						if (pEff)
							pEff->Add (oEffect);

						return;
					}
				}
			}

			Effects* pEffects	=	m_oAnimation.Lookup (oEffect.m_nRefID);
			if (NULL == pEffects)
			{
				pEffects		=	new Effects ();
				m_oAnimation.Add ( oEffect.m_nRefID, pEffects );	
			}
			
			if (pEffects)
				pEffects->Add (oEffect);

		}

		inline void RefreshMediaMap (double dMaxTime)
		{
			for (int i = 0; i < m_oMediaMap.GetSize(); ++i)
			{
				long nRefID			=	m_oMediaMap.GetKeyAt(i);
				if (nRefID >= 0)
				{
					Effects* pEff	=	m_oMediaMap.Lookup (nRefID);
					if (pEff)
					{
						if ((long)pEff->GetCount())
						{
							long nInd	=	0;
							for (long j = 1; j < (long)pEff->GetCount(); ++j)
							{
								if (pEff->GetAt(j).m_nBeginTime >= pEff->GetAt(nInd).m_nBeginTime)
									nInd	=	j;
							}

							if (1 == pEff->GetAt(nInd).m_nMediaCMD)														// PLAY
								pEff->GetAt(nInd).m_nDuration	=	dMaxTime - pEff->GetAt(nInd).m_nBeginTime;

							for (long j = 0; j < (long)pEff->GetCount(); ++j)
								EffectToMap (pEff->GetAt(j), FALSE);
						}
					}	
				}
			}

			for ( long i = 0; i < (long)m_oMediaMap.GetSize (); ++i )
				RELEASEOBJECT (m_oMediaMap.GetValueAt(i));

			m_oMediaMap.RemoveAll ();
		}

		//
		inline double GetDuration (const Effect& oEffect)
		{			
			if (0 == (long)m_arDurations.GetCount())
				return 0.0;

			double nDur		=	m_arDurations[0];

			if ((long)m_arDurations.GetCount() >= 2)
			{
				nDur		=	m_arDurations[1];
			
				if (FadeExitType == oEffect.m_nEffectType)
				{
					nDur	=	m_arDurations[0];
				}
			}

			if (43 == oEffect.m_nEffectID)			//	CenterRevolveEffect
			{
#ifdef _DEBUG	
				TRACE_ARR (_T("duration : "), m_arDurations);
#endif
				if ((long)m_arDurations.GetCount () >= 4)
				{
					if (FadeInType == m_oEffect.m_nEffectType)
						nDur	=	m_arDurations[2] + m_arDurations[4];
					
					if (FadeExitType == m_oEffect.m_nEffectType)
						nDur	=	m_arDurations[0] + m_arDurations[1];
				}
			}

			return nDur;
		}

		inline double GetBeginTime (const Effect& oEffect)
		{			
			if (0 == (long)m_arBeginTimes.GetCount())
				return 0.0;
			
			if (0 == (long)m_oAnimation.GetSize())
			{
				if ((long)m_arBeginTimes.GetCount() >= 4)
					return m_arBeginTimes[3];
			}

			double dBeginTime	=	m_arBeginTimes[0];

			if (AfterPreviousNode == oEffect.m_nEffectNodeType)		
			{
				m_dAfterTime	=	m_dTime;

				if ((long)m_arBeginTimes.GetCount() >= 2)
					return m_arBeginTimes[0] + m_arBeginTimes[1];
			}

			if (WithPreviousNode == oEffect.m_nEffectNodeType)
			{
				if ((long)m_arBeginTimes.GetCount() >= 3)
					dBeginTime	+=	m_arBeginTimes[2];

				return dBeginTime + m_dAfterTime;
			}

			if (ClickEffectNode == oEffect.m_nEffectNodeType)
			{
				m_dClickTime	=	m_dTime;
				m_dAfterTime	=	m_dTime;

				if ((long)m_arBeginTimes.GetCount() >= 3)
					dBeginTime	+=	m_arBeginTimes[2];
			}

			return dBeginTime + m_dClickTime;
		}

		
		inline void Clear ()
		{
			for ( long i = 0; i < (long)m_oAnimation.GetSize (); ++i )
				RELEASEOBJECT(m_oAnimation.GetValueAt(i));

			m_oAnimation.RemoveAll ();
		
			for ( long i = 0; i < (long)m_oMediaMap.GetSize (); ++i )
				RELEASEOBJECT (m_oMediaMap.GetValueAt(i));

			m_oMediaMap.RemoveAll ();

			m_arLoop.RemoveAll ();
			m_arMediaDur.RemoveAll ();
		}

		//
		inline static BOOL HaveAttribute (const PPTX::Logic::CBhvr& oBehavior, const CString& oFind)
		{
			if (oBehavior.attrNameLst.is_init())
			{
				for ( long i = 0; i < (long)oBehavior.attrNameLst->list.GetCount(); ++i )
				{
					if (oFind == oBehavior.attrNameLst->list[i].text)
						return TRUE;
				}
			}

			return FALSE;
		}
		static inline int ParseTLTime (const CString& str)
		{
			if (str == _T("indefinite")) 
				return 0;//-1;
			return XmlUtils::GetInteger(str);
		}

#ifdef _DEBUG	
		static inline void TRACE_ARR (const CString& sName, CAtlArray<double>& oArr)
		{
			ATLTRACE (sName);

			for ( int i = 0; i < (int)oArr.GetCount(); ++i )
				ATLTRACE ( _T( "%f, "), oArr [i] );

			ATLTRACE ( _T("\n") );
		}
#endif
	private:

		BOOL		m_bHaveEffect;

		double		m_dTime;
		double		m_dAfterTime;
		double		m_dClickTime;
	
		Effect		m_oEffect;
		EffectsMap	m_oAnimation;
		
		TimeArray	m_arDurations;
		TimeArray	m_arBeginTimes;

		BOOL		m_HaveAfterEffect;
		Effect		m_oAfterEffect;

		CAtlArray <CIndLoop>	m_arLoop;
		CAtlArray <CMediaDur>	m_arMediaDur;
		EffectsMap				m_oMediaMap;		//	

	};
}