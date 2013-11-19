#pragma once
#include "../stdafx.h"

#include "ShapeConvert.h"
#include "TransitionConvert.h"
#include "TimingConvert.h"
#include "ThemeConvert.h"
#include "LayoutConvert.h"

#include "../PPTXFormat/Logic/SpTreeElem.h"
#include "../PPTXFormat/Logic/BgPr.h"
#include "../PPTXFormat/SlideLayout.h"
#include "../PPTXFormat/SlideMaster.h"

namespace PPTX2EditorAdvanced
{
	static void AddAnimation (CAnimateEffects* pSEffects, CAtlArray<CColor>& oSchemeColors, NSPresentationEditor::CSlide& oSlide, IElement* pElement)
	{
		if (pSEffects)
		{
			CAtlArray <NSPresentationEditor::CAnimationSimple>* arEffects = pSEffects->GetAnimation().Lookup ( pElement->m_lID );
			if (arEffects)
			{
				for ( long i = 0; i < (long)arEffects->GetCount(); ++i )
				{								
					CAnimationSimple oEffect	=	arEffects->GetAt(i);
					CAnimationSimple oAnimation	=	oEffect;

					if (0 == oEffect.m_nSchemeColor)		//	RGB
					{
						oAnimation.m_nColorTo	=	oEffect.m_nColorTo;
					}
					else if (2 == oEffect.m_nSchemeColor)	//	Index From Table
					{
						if ((int)oEffect.m_nColorTo >= (int)oSchemeColors.GetCount())
							continue;

						oAnimation.m_nColorTo	=	oSchemeColors[oEffect.m_nColorTo].GetLONG();
					}

					pElement->m_oAnimations.m_dSlideWidth		=	oSlide.m_lOriginalWidth;
					pElement->m_oAnimations.m_dSlideHeight		=	oSlide.m_lOriginalHeight;

					pElement->m_oAnimations.m_arAnimations.Add ( oAnimation );
				}
			}
		}
	}

	static BOOL UpdateVideo (IElement* pElem, PPTX::Logic::SpTree& oGroupTree, CAnimateEffects& oEff)
	{
		if (NSPresentationEditor::etVideo == pElem->m_etType)
		{
			long nCount = (long)oGroupTree.SpTreeElems.GetCount();
			for (long j = 0; j < nCount; ++j)
			{
				const PPTX::Logic::SpTreeElem& oShape	=	oGroupTree.SpTreeElems[j];
				if(oShape.is<PPTX::Logic::Pic>())
				{
					const PPTX::Logic::Pic& obj			=	oShape.as<PPTX::Logic::Pic>();
					if (obj.GetRefId() == pElem->m_lID)
					{
						CVideoElement* pVideo			=	static_cast<CVideoElement*>(pElem);
						if (pVideo)
						{
							pVideo->m_dClipStartTime	=	obj.GetStTrim();
							pVideo->m_dClipEndTime		=	obj.GetStTrim() + oEff.GetMediaDur(pElem->m_lID);	//	obj.GetEndTrim();
						}
						
						if (-1 == oEff.GetLoopFor(pElem->m_lID))
						{
							pVideo->m_bLoop				=	TRUE;
						}

						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}

	static BOOL UpdateAudio (IElement* pElem, PPTX::Logic::SpTree& oGroupTree, const CAnimateEffects& oEff)
	{
		if (NSPresentationEditor::etAudio == pElem->m_etType)
		{
			long nCount = (long)oGroupTree.SpTreeElems.GetCount();
			for (long j = 0; j < nCount; ++j)
			{
				const PPTX::Logic::SpTreeElem& oShape	=	oGroupTree.SpTreeElems[j];
				if(oShape.is<PPTX::Logic::Pic>())
				{
					const PPTX::Logic::Pic& obj			=	oShape.as<PPTX::Logic::Pic>();
					if (obj.GetRefId() == pElem->m_lID)
					{
						CAudioElement* pAudio			=	static_cast<CAudioElement*>(pElem);
						if (pAudio)
						{
							pAudio->m_dClipStartTime	=	obj.GetStTrim();
							pAudio->m_dClipEndTime		=	obj.GetStTrim() + oEff.GetMediaDur(pElem->m_lID);	//	obj.GetEndTrim();

							if (-1 == oEff.GetLoopFor(pElem->m_lID))										// на одном слайде для всех объектов с одним источником
							{
								pAudio->m_bLoop			=	TRUE;
							}

							return TRUE;
						}
					}
				}
			}
		}

		return FALSE;
	}

	static void SlideConvert(smart_ptr<PPTX::Slide> Slide, const DWORD SldNum, double* Duration, const int cx, const int cy, NSPresentationEditor::CDocument& pDocument, CPresentationInfo& oPresentation)
	{
		pDocument.m_arSlides.Add();
		NSPresentationEditor::CSlide& oSlide = pDocument.m_arSlides[pDocument.m_arSlides.GetCount() - 1];

		oSlide.m_lOriginalWidth		= cx;
		oSlide.m_lOriginalHeight	= cy;
		oSlide.m_lWidth				= cx/36000;//pDocument->m_lWidth;
		oSlide.m_lHeight			= cy/36000;//pDocument->m_lHeight;

		// background
		if (Slide->cSld.IsInit())
		{
			if (Slide->cSld->bg.IsInit())
			{
				oSlide.m_bIsBackground = true;

				PPTX::Logic::BgPr bg;
				DWORD ARGB = 0;
				Slide->cSld->bg->GetBackground(bg, ARGB);
				DWORD BGRA = (ARGB & 0xFF) << 24;
				BGRA += (ARGB & 0xFF00) << 8;
				BGRA += (ARGB & 0xFF0000) >> 8;
				BGRA += (ARGB & 0xFF000000) >> 24;
				FillToBrush(bg.Fill, BGRA, oSlide.m_oBackground);
			}
		}

		// colorscheme
		oSlide.m_bUseLayoutColorScheme = true;
		if (Slide->clrMapOvr.is_init())
		{
			if (Slide->clrMapOvr->overrideClrMapping.is_init())
			{
				oSlide.m_bUseLayoutColorScheme = false;

				NSPresentationEditor::CColor lColor;
				lColor.m_lSchemeIndex = 0;
				oSlide.m_arColorScheme.Add(lColor);//phClr
				lColor.m_lSchemeIndex = -1;

				lColor = Slide->GetBGRAFromMap(_T("bg1"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("tx1"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("bg2"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("tx2"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("accent1"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("accent2"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("accent3"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("accent4"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("accent5"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("accent6"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("hlink"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("folHlink"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("lt1"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("dk1"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("lt2"));
				oSlide.m_arColorScheme.Add(lColor);
				lColor = Slide->GetBGRAFromMap(_T("dk2"));
				oSlide.m_arColorScheme.Add(lColor);
			}
		}


		CAtlArray<CColor> arrColors;
		{
			NSPresentationEditor::CColor lColor;
			lColor.m_lSchemeIndex = 0;
			arrColors.Add(lColor);//phClr
			lColor.m_lSchemeIndex = -1;

			lColor = Slide->GetBGRAFromMap(_T("bg1"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("tx1"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("bg2"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("tx2"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("accent1"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("accent2"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("accent3"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("accent4"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("accent5"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("accent6"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("hlink"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("folHlink"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("lt1"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("dk1"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("lt2"));
			arrColors.Add(lColor);
			lColor = Slide->GetBGRAFromMap(_T("dk2"));
			arrColors.Add(lColor);
		}

		oSlide.m_lThemeID = ThemeConvert(Slide->Layout->Master, oPresentation);

		if (-1 != oSlide.m_lThemeID)
		{
			oSlide.m_lLayoutID = LayoutConvert(Slide->Layout, oPresentation);
		}

		if (Slide->cSld.IsInit())
		{
			PPTX::Logic::SpTree& group = Slide->cSld->spTree;
			GroupConvert(group, oSlide.m_arElements, oPresentation, &oPresentation.m_arThemes[oSlide.m_lThemeID]->m_oTheme);
		}

		oSlide.m_bShowMasterShapes = (Slide->Layout->showMasterSp.get_value_or(true) && (Slide->showMasterSp.get_value_or(true)));
		
		#define MIN_SLIDE_TIME_	1000.0		//	FOR STATIC SLIDES

		double dChangeTime		=	0.0;	//	переход к следующему слайду после определенного времени (не меньше чем сумма всех анимаций на слайде)
		double dTime			=	0.0;

		oSlide.m_dStartTime	=	*Duration;
		if (Slide->transition.is_init())
			SetupTransition((DWORD)(pDocument.m_arSlides.GetCount() - 1), Slide, pDocument, dChangeTime);
		else
			oSlide.m_oSlideShow.m_oTransition.m_dSpeed	=	0.0;

		CAnimateEffects oEffects;
		if (Slide->timing.is_init() && oEffects.Build (Slide->timing.get()) )
		{
			oSlide.m_dDuration	=	max (oEffects.GetTime(), dChangeTime);
			
			for (long nShape = 0; nShape < (long)oSlide.m_arElements.GetCount(); ++nShape)
			{
				IElement* pElem	=	oSlide.m_arElements[nShape];
				if (pElem)
				{
					UpdateVideo(pElem, Slide->cSld->spTree, oEffects);
					UpdateAudio(pElem, Slide->cSld->spTree, oEffects);

					AddAnimation(&oEffects, arrColors, oSlide, pElem);
					
					pElem->m_dEndTime	+=	pElem->m_dStartTime + oSlide.m_dDuration;
				}
			}

			*Duration			+=	oSlide.m_dDuration;
		}
		else
		{
			oSlide.m_dDuration	=	max (MIN_SLIDE_TIME, dChangeTime);
			*Duration			+=	oSlide.m_dDuration;			
		}

		oSlide.m_dEndTime	=	*Duration;

		for (long i = 0; i < (long)oSlide.m_arElements.GetCount(); ++i) 
		{
			IElement* pElement = oSlide.m_arElements[i];
			if (pElement)
			{
				if (etAudio == pElement->m_etType)
				{
					CAudioElement* pAudio = static_cast<CAudioElement*>(oSlide.m_arElements[i]);
					if (pAudio && pAudio->m_bLoop)
					{
						for ( int j = 0; j < (long)pAudio->m_oAnimations.m_arAnimations.GetCount(); ++j)
						{
							CAnimationSimple& oAnimation =	pAudio->m_oAnimations.m_arAnimations.GetAt(j);
							if (0.0 > oAnimation.m_nBeginTime)
							{
								pAudio->m_oAnimations.m_arAnimations.GetAt(j).m_nDuration	=	oSlide.m_dDuration + abs (oAnimation.m_nBeginTime);
							}
						}
					}
				}
			}
		}
	}
}