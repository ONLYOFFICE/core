#include "stdafx.h"
#include "./../Converter.h"

#include "Logic/Transitions/EmptyTransition.h"
#include "Logic/Transitions/OrientationTransition.h"
#include "Logic/Transitions/EightDirectionTransition.h"
#include "Logic/Transitions/OptionalBlackTransition.h"
#include "Logic/Transitions/SideDirectionTransition.h"
#include "Logic/Transitions/CornerDirectionTransition.h"
#include "Logic/Transitions/WheelTransition.h"
#include "Logic/Transitions/SplitTransition.h"
#include "Logic/Transitions/ZoomTransition.h"

#import "..\..\..\..\Redist\AVSVideoFile3.dll" raw_interfaces_only

namespace PPTX2DrawingXML
{
	DWORD Converter::GetTransitionSpeedFromStr(const std::string& str)
	{
		if(str == "fast") return 250;//2;
		if(str == "med") return 500;//1;
		if(str == "slow") return 750;//0;
		return 250;//2;
	}

	int Converter::TransitionConvert(const DWORD SldNum, const boost::shared_ptr<PPTX::Slide> Slide, CDrawingDocument& pDocument)
	{
		ODCSlideShowSlideInfoAtom lDestTransition;

		lDestTransition.m_nEffectType = 255;

		const PPTX::Logic::Transition pTransition = Slide->transition.get();

		if(pTransition.base.is<PPTX::Logic::EmptyTransition>())
		{
			const std::string name = pTransition.base.as<PPTX::Logic::EmptyTransition>().name.get();

			if(name == "random")
				lDestTransition.m_nEffectType = 1;
			else if(name == "circle")
			{
				lDestTransition.m_nEffectType = 27;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(name == "dissolve")
			{
				lDestTransition.m_nEffectType = 5;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(name == "diamond")
			{
				lDestTransition.m_nEffectType = 17;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(name == "newsflash")
			{
				lDestTransition.m_nEffectType = 22;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(name == "plus")
			{
				lDestTransition.m_nEffectType = 18;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(name == "wedge")
			{
				lDestTransition.m_nEffectType = 19;
				lDestTransition.m_nEffectDirection = 0;
			}
		}
		else if(pTransition.base.is<PPTX::Logic::OrientationTransition>())
		{
			const std::string name = pTransition.base.as<PPTX::Logic::OrientationTransition>().name.get();
			const std::string dir = pTransition.base.as<PPTX::Logic::OrientationTransition>().dir.get_value_or("horz");

			if(name == "blinds")
				lDestTransition.m_nEffectType = 2;
			else if(name == "checker")
				lDestTransition.m_nEffectType = 3;
			else if(name == "comb")
				lDestTransition.m_nEffectType = 21;
			else if(name == "randomBar")
				lDestTransition.m_nEffectType = 8;

			if(dir == "horz")
				lDestTransition.m_nEffectDirection = 0;
			else if(dir == "vert")
				lDestTransition.m_nEffectDirection = 1;
		}
		else if(pTransition.base.is<PPTX::Logic::SideDirectionTransition>())
		{
			const std::string name = pTransition.base.as<PPTX::Logic::SideDirectionTransition>().name.get();
			const std::string dir = pTransition.base.as<PPTX::Logic::SideDirectionTransition>().dir.get_value_or("l");

			if(name == "push")
				lDestTransition.m_nEffectType = 20;
			else if(name == "wipe")
				lDestTransition.m_nEffectType = 10;

			if(dir == "l")
				lDestTransition.m_nEffectDirection = 0;
			else if(dir == "u")
				lDestTransition.m_nEffectDirection = 1;
			else if(dir == "r")
				lDestTransition.m_nEffectDirection = 2;
			else if(dir == "d")
				lDestTransition.m_nEffectDirection = 3;
		}
		else if(pTransition.base.is<PPTX::Logic::EightDirectionTransition>())
		{
			const std::string name = pTransition.base.as<PPTX::Logic::EightDirectionTransition>().name.get();
			const std::string dir = pTransition.base.as<PPTX::Logic::EightDirectionTransition>().dir.get_value_or("l");

			if(name == "cover")
				lDestTransition.m_nEffectType = 4;
			else if(name == "pull")
				lDestTransition.m_nEffectType = 7;

			if(dir == "l")
				lDestTransition.m_nEffectDirection = 0;
			else if(dir == "u")
				lDestTransition.m_nEffectDirection = 1;
			else if(dir == "r")
				lDestTransition.m_nEffectDirection = 2;
			else if(dir == "d")
				lDestTransition.m_nEffectDirection = 3;
			else if(dir == "lu")
				lDestTransition.m_nEffectDirection = 4;
			else if(dir == "ru")
				lDestTransition.m_nEffectDirection = 5;
			else if(dir == "ld")
				lDestTransition.m_nEffectDirection = 6;
			else if(dir == "rd")
				lDestTransition.m_nEffectDirection = 7;
		}
		else if(pTransition.base.is<PPTX::Logic::CornerDirectionTransition>())
		{
			const std::string name = pTransition.base.as<PPTX::Logic::CornerDirectionTransition>().name.get();
			const std::string dir = pTransition.base.as<PPTX::Logic::CornerDirectionTransition>().dir.get_value_or("lu");

			if(name == "strips")
				lDestTransition.m_nEffectType = 9;

			if(dir == "lu")
				lDestTransition.m_nEffectDirection = 4;
			else if(dir == "ru")
				lDestTransition.m_nEffectDirection = 5;
			else if(dir == "ld")
				lDestTransition.m_nEffectDirection = 6;
			else if(dir == "rd")
				lDestTransition.m_nEffectDirection = 7;
		}
		else if(pTransition.base.is<PPTX::Logic::OptionalBlackTransition>())
		{
			const std::string name = pTransition.base.as<PPTX::Logic::OptionalBlackTransition>().name.get();

			if(name == "fade")
			{
				lDestTransition.m_nEffectType = 6;
				lDestTransition.m_nEffectDirection = 0;
			}
			else if(name == "cut")
			{
				lDestTransition.m_nEffectType = 0;
				lDestTransition.m_nEffectDirection = pTransition.base.as<PPTX::Logic::OptionalBlackTransition>().thruBlk.get_value_or(false)?1:0;
			}
		}
		else if(pTransition.base.is<PPTX::Logic::WheelTransition>())
		{
			lDestTransition.m_nEffectType = 26;
			lDestTransition.m_nEffectDirection = pTransition.base.as<PPTX::Logic::WheelTransition>().spokes.get_value_or(4);
		}
		else if(pTransition.base.is<PPTX::Logic::SplitTransition>())
		{
			const std::string orient = pTransition.base.as<PPTX::Logic::SplitTransition>().orient.get_value_or("horz");
			const std::string dir = pTransition.base.as<PPTX::Logic::SplitTransition>().dir.get_value_or("out");

			lDestTransition.m_nEffectType = 13;
			if(orient == "horz")
				lDestTransition.m_nEffectDirection = 0;
			else if(orient == "vert")
				lDestTransition.m_nEffectDirection = 2;
			if(dir == "in")
				lDestTransition.m_nEffectDirection++;
		}
		else if(pTransition.base.is<PPTX::Logic::ZoomTransition>())
		{
			lDestTransition.m_nEffectType = 11;
			lDestTransition.m_nEffectDirection = pTransition.base.as<PPTX::Logic::ZoomTransition>().dir.get_value_or("out") == "in"?1:0;
		}

		//lDestTransition.m_nSpeed = GetTransitionSpeedFromStr(pTransition.spd.get_value_or("fast"));
		lDestTransition.m_nTime = GetTransitionSpeedFromStr(pTransition.spd.get_value_or("fast"));
		pDocument.m_mapTransitions.Add(SldNum, lDestTransition);

		if(pTransition.sndAc.is_init())
		{
			if(pTransition.sndAc->stSnd.is_init())
			{
				CAudioElement* lpAudioElement = new CAudioElement;
				lpAudioElement->m_strFileName = CStringW(Slide->GetLinkFromRId(pTransition.sndAc->stSnd->embed.get()).c_str());

				int lIndex = lpAudioElement->m_strFileName.Find(L"file:///");
				if (0 == lIndex)
				{
					lpAudioElement->m_strFileName = lpAudioElement->m_strFileName.Mid(8);
					//lpAudioElement->m_strFileName.Replace('/', '\\');
					//lpAudioElement->m_strFileName.Replace(L"%20", L" ");
				}
				//lpAudioElement->m_strFileName.Replace(L"&", L"&amp;");
				//lpAudioElement->m_strFileName.Replace('/', '\\');
				//lpAudioElement->m_strFileName.Replace(L"%20", L" ");
				//lpAudioElement->m_strFileName.Replace(L">", L"&gt;");
				//lpAudioElement->m_strFileName.Replace(L"<", L"&lt;");
				//lpAudioElement->m_strFileName.Replace(L"'", L"&apos;");
				//lpAudioElement->m_strFileName.Replace(L"\"", L"&quot;");

				BSTR bstrFile = lpAudioElement->m_strFileName.AllocSysString();

				AVSVideoFile3::IVideoFile3* pVideoFile = NULL;
				CoCreateInstance(__uuidof(AVSVideoFile3::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(AVSVideoFile3::IVideoFile3), (void**)(&pVideoFile));

				double dDuration = 0;
				if (NULL != pVideoFile)
				{
					pVideoFile->OpenFile(bstrFile);
					pVideoFile->get_audioDuration(&dDuration);
					pVideoFile->CloseFile();
				}
				SysFreeString(bstrFile);

				lpAudioElement->m_dStartTime = pDocument.m_arPages[SldNum]->m_dStartTime;
				lpAudioElement->m_dEndTime = lpAudioElement->m_dStartTime + dDuration;

				pDocument.m_arPages[SldNum]->m_arElements.Add(lpAudioElement);
			}
		}
		return pTransition.advTm.get_value_or(0);
	}
} // namespace PPTX2DrawingXML