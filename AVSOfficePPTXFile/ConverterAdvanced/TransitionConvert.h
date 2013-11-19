#pragma once
#include "Structures.h"

#include "../PPTXFormat/Logic/Transitions/EmptyTransition.h"
#include "../PPTXFormat/Logic/Transitions/OrientationTransition.h"
#include "../PPTXFormat/Logic/Transitions/EightDirectionTransition.h"
#include "../PPTXFormat/Logic/Transitions/OptionalBlackTransition.h"
#include "../PPTXFormat/Logic/Transitions/SideDirectionTransition.h"
#include "../PPTXFormat/Logic/Transitions/CornerDirectionTransition.h"
#include "../PPTXFormat/Logic/Transitions/WheelTransition.h"
#include "../PPTXFormat/Logic/Transitions/SplitTransition.h"
#include "../PPTXFormat/Logic/Transitions/ZoomTransition.h"

namespace PPTX2EditorAdvanced
{
	AVSINLINE DWORD GetTransitionSpeedFromStr(const CString& str)
	{
		if (str == _T("fast"))	return 250;//2;
		if (str == _T("med"))	return 500;//1;
		if (str == _T("slow"))	return 750;//0;
		return 250;//2;
	}

	inline int SetupTransition (const DWORD SldNum, const smart_ptr<PPTX::Slide> oSlide, NSPresentationEditor::CDocument& pDocument, double& ChangeTime)
	{
		NSPresentationEditor::CTransition oTransition;

		oTransition.m_nEffectType = 255;

		const PPTX::Logic::Transition pTransition = oSlide->transition.get();

		if(pTransition.base.is<PPTX::Logic::EmptyTransition>())
		{
			const CString name = pTransition.base.as<PPTX::Logic::EmptyTransition>().name;

			if(name == _T("random"))
				oTransition.m_nEffectType		= 1;
			else if(name == _T("circle"))
			{
				oTransition.m_nEffectType		= 27;
				oTransition.m_nEffectDirection	= 0;
			}
			else if(name == _T("dissolve"))
			{
				oTransition.m_nEffectType		= 5;
				oTransition.m_nEffectDirection	= 0;
			}
			else if(name == _T("diamond"))
			{
				oTransition.m_nEffectType		= 17;
				oTransition.m_nEffectDirection	= 0;
			}
			else if(name == _T("newsflash"))
			{
				oTransition.m_nEffectType		= 22;
				oTransition.m_nEffectDirection	= 0;
			}
			else if(name == _T("plus"))
			{
				oTransition.m_nEffectType		= 18;
				oTransition.m_nEffectDirection	= 0;
			}
			else if(name == _T("wedge"))
			{
				oTransition.m_nEffectType		= 19;
				oTransition.m_nEffectDirection	= 0;
			}
		}
		else if(pTransition.base.is<PPTX::Logic::OrientationTransition>())
		{
			PPTX::Logic::OrientationTransition trans = pTransition.base.as<PPTX::Logic::OrientationTransition>();
			
			const CString name	= trans.name;
			const CString dir	= trans.dir.get_value_or(_T("horz"));

			if (name == _T("blinds"))
				oTransition.m_nEffectType	= 2;
			else if (name == _T("checker"))
				oTransition.m_nEffectType	= 3;
			else if (name == _T("comb"))
				oTransition.m_nEffectType	= 21;
			else if (name == _T("randomBar"))
				oTransition.m_nEffectType	= 8;

			if (dir == _T("horz"))
				oTransition.m_nEffectDirection = 0;
			else if (dir == _T("vert"))
				oTransition.m_nEffectDirection = 1;
		}
		else if(pTransition.base.is<PPTX::Logic::SideDirectionTransition>())
		{
			PPTX::Logic::SideDirectionTransition trans = pTransition.base.as<PPTX::Logic::SideDirectionTransition>();
			const CString name	= trans.name;
			const CString dir	= trans.dir.get_value_or(_T("l"));

			if (name == _T("push"))
				oTransition.m_nEffectType	= 20;
			else if (name == _T("wipe"))
				oTransition.m_nEffectType	= 10;

			if (dir == _T("l"))
				oTransition.m_nEffectDirection = 0;
			else if (dir == _T("u"))
				oTransition.m_nEffectDirection = 1;
			else if (dir == _T("r"))
				oTransition.m_nEffectDirection = 2;
			else if (dir == _T("d"))
				oTransition.m_nEffectDirection = 3;
		}
		else if(pTransition.base.is<PPTX::Logic::EightDirectionTransition>())
		{
			PPTX::Logic::EightDirectionTransition trans = pTransition.base.as<PPTX::Logic::EightDirectionTransition>();
			const CString name	= trans.name;
			const CString dir	= trans.dir.get_value_or(_T("l"));

			if (name == _T("cover"))
				oTransition.m_nEffectType = 4;
			else if (name == _T("pull"))
				oTransition.m_nEffectType = 7;

			if (dir == _T("l"))
				oTransition.m_nEffectDirection = 0;
			else if (dir == _T("u"))
				oTransition.m_nEffectDirection = 1;
			else if (dir == _T("r"))
				oTransition.m_nEffectDirection = 2;
			else if (dir == _T("d"))
				oTransition.m_nEffectDirection = 3;
			else if (dir == _T("lu"))
				oTransition.m_nEffectDirection = 4;
			else if (dir == _T("ru"))
				oTransition.m_nEffectDirection = 5;
			else if (dir == _T("ld"))
				oTransition.m_nEffectDirection = 6;
			else if (dir == _T("rd"))
				oTransition.m_nEffectDirection = 7;
		}
		else if(pTransition.base.is<PPTX::Logic::CornerDirectionTransition>())
		{
			PPTX::Logic::CornerDirectionTransition trans = pTransition.base.as<PPTX::Logic::CornerDirectionTransition>();

			const CString name	= trans.name;
			const CString dir	= trans.dir.get_value_or(_T("lu"));

			if (name == _T("strips"))
				oTransition.m_nEffectType = 9;

			if (dir == _T("lu"))
				oTransition.m_nEffectDirection = 4;
			else if (dir == _T("ru"))
				oTransition.m_nEffectDirection = 5;
			else if (dir == _T("ld"))
				oTransition.m_nEffectDirection = 6;
			else if (dir == _T("rd"))
				oTransition.m_nEffectDirection = 7;
		}
		else if(pTransition.base.is<PPTX::Logic::OptionalBlackTransition>())
		{
			const CString name = pTransition.base.as<PPTX::Logic::OptionalBlackTransition>().name;

			if (name == _T("fade"))
			{
				oTransition.m_nEffectType = 6;
				oTransition.m_nEffectDirection = 0;
			}
			else if (name == _T("cut"))
			{
				oTransition.m_nEffectType = 0;
				oTransition.m_nEffectDirection = pTransition.base.as<PPTX::Logic::OptionalBlackTransition>().thruBlk.get_value_or(false)?1:0;
			}
		}
		else if(pTransition.base.is<PPTX::Logic::WheelTransition>())
		{
			oTransition.m_nEffectType = 26;
			oTransition.m_nEffectDirection = pTransition.base.as<PPTX::Logic::WheelTransition>().spokes.get_value_or(4);
		}
		else if(pTransition.base.is<PPTX::Logic::SplitTransition>())
		{
			PPTX::Logic::SplitTransition trans = pTransition.base.as<PPTX::Logic::SplitTransition>();

			const CString orient	= trans.orient.get_value_or(_T("horz"));
			const CString dir		= trans.dir.get_value_or(_T("out"));

			oTransition.m_nEffectType = 13;
			if (orient == _T("horz"))
				oTransition.m_nEffectDirection = 0;
			else if (orient == _T("vert"))
				oTransition.m_nEffectDirection = 2;
			if (dir == _T("in"))
				oTransition.m_nEffectDirection++;
		}
		else if(pTransition.base.is<PPTX::Logic::ZoomTransition>())
		{
			oTransition.m_nEffectType		= 11;
			oTransition.m_nEffectDirection	= (pTransition.base.as<PPTX::Logic::ZoomTransition>().dir.get_value_or(_T("out")) == _T("in"))?1:0;
		}

		oTransition.m_dSpeed = GetTransitionSpeedFromStr(pTransition.spd.get_value_or(_T("fast")));

		if (pTransition.dur.is_init())	// 
			oTransition.m_dSpeed	=	pTransition.dur.get();

		if (pTransition.advTm.is_init())
			ChangeTime				=	(double)pTransition.advTm.get();

#ifdef _DEBUG
		ATLTRACE (_T("transition after : %f, dur : %f\n"), ChangeTime, oTransition.m_dSpeed);
#endif
		pDocument.m_arSlides[SldNum].m_oSlideShow.m_oTransition = oTransition;
		
		if (pTransition.sndAc.is_init())
		{
			if (pTransition.sndAc->stSnd.is_init())
			{
				CAudioElement* pAudio		=	new CAudioElement ();
				if (pAudio)
				{
					pAudio->m_strFileName	=	PathHelpers::ResourceFileSystemPathXml (oSlide->GetLinkFromRId(pTransition.sndAc->stSnd->embed.get()));

					CString fileName		=	PathHelpers::ResourceFileSystemPath (oSlide->GetLinkFromRId(pTransition.sndAc->stSnd->embed.get()));
					BSTR bstrFile			=	fileName.AllocSysString();

					VideoFile::IVideoFile3* pVideoFile = NULL;
					if (SUCCEEDED(CoCreateInstance(__uuidof(VideoFile::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(VideoFile::IVideoFile3), (void**)(&pVideoFile))))
					{
						double dDuration	=	0;
						if (pVideoFile)
						{
							if (FAILED(pVideoFile->OpenFile(bstrFile)))
							{
								RELEASEINTERFACE (pVideoFile); 
								RELEASEOBJECT (pAudio);

								return pTransition.advTm.get_value_or(0);
							}

							pVideoFile->get_audioDuration (&dDuration);
							pVideoFile->CloseFile();							

							SysFreeString(bstrFile);
							RELEASEINTERFACE(pVideoFile);

							pAudio->m_dStartTime		=	0;
							pAudio->m_dEndTime			=	dDuration;
							pAudio->m_dClipStartTime	=	0.0;
							pAudio->m_dClipEndTime		=	dDuration;
							
							if (pTransition.sndAc->stSnd->loop.is_init())
							{
								if (pTransition.sndAc->stSnd->loop.get_value_or(false))
									pAudio->m_bLoop		=	TRUE;
							}

							CAnimationSimple oPlayAudio;

							oPlayAudio.m_nEffectType	=	6;
							oPlayAudio.m_nMediaCMD		=	1;
							oPlayAudio.m_nBeginTime		=	-0.0001;				//	
							oPlayAudio.m_nBeginTime		=	-oTransition.m_dSpeed;
							oPlayAudio.m_nDuration		=	dDuration;

							pAudio->m_oAnimations.m_arAnimations.Add (oPlayAudio);

							pDocument.m_arSlides[SldNum].m_arElements.Add(pAudio);
						}
					}
				}
			}
		}
		
		return pTransition.advTm.get_value_or(0);
	}
}