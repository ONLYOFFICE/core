#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/Timing/Seq.h"
#include "Logic/Timing/Par.h"
#include "Logic/Timing/Audio.h"
#include "Logic/Timing/Video.h"
#include "Logic/Timing/Excl.h"
#include "Logic/Timing/Anim.h"
#include "Logic/Timing/AnimClr.h"
#include "Logic/Timing/AnimEffect.h"
#include "Logic/Timing/AnimMotion.h"
#include "Logic/Timing/AnimRot.h"
#include "Logic/Timing/AnimScale.h"
#include "Logic/Timing/Cmd.h"
#include "Logic/Timing/Set.h"

#include "Parse.h"
#include "Logic/Timing/AttrName.h"
#include "Logic/Timing/Cond.h"
#include "Logic/Timing/Tav.h"

namespace PPTX2DrawingXML
{
	void Converter::TimingConvert(const DWORD SldNum, const PPTX::Logic::Timing& pTiming, CDrawingDocument& pDocument)
	{
		if(pTiming.tnLst.is_init())
		{
			ODPP10SlideBinaryTagExtension Source;
			Source.m_pExtTimeNodeContainer = new Animations::ODExtTimeNodeContainer();

			//здесь заполняем Source.m_pExtTimeNodeContainer
			TimeNodeListConvert(pTiming.tnLst.get(), *Source.m_pExtTimeNodeContainer);

			Animations::ODCSlideTimeLine* TimeLine = new Animations::ODCSlideTimeLine();
			bool result = TimeLine->Process(Source.m_pExtTimeNodeContainer);
			if(result)
				pDocument.m_mapAnimations.Add(SldNum, TimeLine);
		}
	}

	void Converter::TimeNodeListConvert(const PPTX::Logic::TnLst& tnLst, Animations::ODExtTimeNodeContainer& tnContainer)
	{
		//здесь надо заполнять всякие атомы
		tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Parallel;
		tnContainer.m_bIsExistsTimeList = true;

		for(std::list<PPTX::Logic::TimeNodeBase>::const_iterator i = tnLst.list->begin(); i != tnLst.list->end(); i++)
		{
			Animations::ODExtTimeNodeContainer* container = new Animations::ODExtTimeNodeContainer();

			container->m_oTimePropertyList.m_EffectType.m_Value = tnContainer.m_oTimePropertyList.m_EffectType.m_Value;
			container->m_oTimePropertyList.m_EffectID.m_Value = tnContainer.m_oTimePropertyList.m_EffectID.m_Value;
			container->m_oTimePropertyList.m_EffectDir.m_Value = tnContainer.m_oTimePropertyList.m_EffectDir.m_Value;
			container->m_oTimePropertyList.m_EffectNodeType.m_Value = tnContainer.m_oTimePropertyList.m_EffectNodeType.m_Value;

			tnContainer.m_arrTimeNodes.Add(container);
			TimeNodeBaseConvert(*i, *tnContainer.m_arrTimeNodes.GetAt(tnContainer.m_arrTimeNodes.GetCount() - 1));
		}
	}

	void Converter::TimeNodeBaseConvert(const PPTX::Logic::TimeNodeBase& timeNode, Animations::ODExtTimeNodeContainer& tnContainer)
	{
		if(timeNode.is<PPTX::Logic::Par>())
		{
			tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Parallel;
			CTN2TimeNodeAtom(timeNode.as<PPTX::Logic::Par>().cTn.get(), tnContainer);

			//добавить iterate
			if(timeNode.as<PPTX::Logic::Par>().cTn->childTnLst.is_init())
			{
				tnContainer.m_bIsExistsTimeList = true;

				Animations::ODExtTimeNodeContainer* container = new Animations::ODExtTimeNodeContainer();

				container->m_oTimePropertyList.m_EffectType.m_Value = tnContainer.m_oTimePropertyList.m_EffectType.m_Value;
				container->m_oTimePropertyList.m_EffectID.m_Value = tnContainer.m_oTimePropertyList.m_EffectID.m_Value;
				container->m_oTimePropertyList.m_EffectDir.m_Value = tnContainer.m_oTimePropertyList.m_EffectDir.m_Value;
				container->m_oTimePropertyList.m_EffectNodeType.m_Value = tnContainer.m_oTimePropertyList.m_EffectNodeType.m_Value;

				tnContainer.m_arrTimeNodes.Add(container);

				TimeNodeListConvert(timeNode.as<PPTX::Logic::Par>().cTn->childTnLst.get(), *tnContainer.m_arrTimeNodes.GetAt(0));
			}
		}
		else if(timeNode.is<PPTX::Logic::Seq>())
		{
			tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Sequential;
			if(timeNode.as<PPTX::Logic::Seq>().prevCondLst.is_init())
				AddCondLst(timeNode.as<PPTX::Logic::Seq>().prevCondLst.get(), tnContainer);

			CTN2TimeNodeAtom(timeNode.as<PPTX::Logic::Seq>().cTn.get(), tnContainer);

			tnContainer.m_bIsExistsTimeSequence = true;
			tnContainer.m_oTimeSequenceDataAtom.m_bConcurrencyPropertyUsed = true;
			tnContainer.m_oTimeSequenceDataAtom.m_bNextActionPropertyUsed = true;
			tnContainer.m_oTimeSequenceDataAtom.m_bPreviousActionPropertyUsed = true;
			tnContainer.m_oTimeSequenceDataAtom.m_nConcurrency = timeNode.as<PPTX::Logic::Seq>().concurrent.get_value_or(false) ? 1 : 0;
			tnContainer.m_oTimeSequenceDataAtom.m_nNextAction = timeNode.as<PPTX::Logic::Seq>().nextAc.get_value_or("none") == "seek" ? 1 : 0;
			tnContainer.m_oTimeSequenceDataAtom.m_nPreviousAction = timeNode.as<PPTX::Logic::Seq>().prevAc.get_value_or("none") == "skipTimed" ? 1 : 0;

			if(timeNode.as<PPTX::Logic::Seq>().cTn->childTnLst.is_init())
			{
				tnContainer.m_bIsExistsTimeList = true;

				Animations::ODExtTimeNodeContainer* container = new Animations::ODExtTimeNodeContainer();

				container->m_oTimePropertyList.m_EffectType.m_Value = tnContainer.m_oTimePropertyList.m_EffectType.m_Value;
				container->m_oTimePropertyList.m_EffectID.m_Value = tnContainer.m_oTimePropertyList.m_EffectID.m_Value;
				container->m_oTimePropertyList.m_EffectDir.m_Value = tnContainer.m_oTimePropertyList.m_EffectDir.m_Value;
				container->m_oTimePropertyList.m_EffectNodeType.m_Value = tnContainer.m_oTimePropertyList.m_EffectNodeType.m_Value;

				tnContainer.m_arrTimeNodes.Add(container);

				TimeNodeListConvert(timeNode.as<PPTX::Logic::Seq>().cTn->childTnLst.get(), *tnContainer.m_arrTimeNodes.GetAt(0));
			}
		}
		//else if(timeNode.is<PPTX::Logic::Audio>())
		//{
		//	tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Media;
		//	tnContainer.m_b???
		//}
		//else if(timeNode.is<PPTX::Logic::Video>())
		//{
		//	tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Media;
		//	tnContainer.m_b???
		//}
		//else if(timeNode.is<PPTX::Logic::Excl>())
		//{
		//	//tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_???;
		//}
		else if(timeNode.is<PPTX::Logic::Anim>())
		{
			tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
			tnContainer.m_bIsExistsTimeAnimateBehaviorContainer = true;
			CTN2TimeNodeAtom(timeNode.as<PPTX::Logic::Anim>().cBhvr->cTn.get(), tnContainer);

			tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bByPropertyUsed = timeNode.as<PPTX::Logic::Anim>().by.is_init();
			if(tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bByPropertyUsed)
				tnContainer.m_oTimeAnimateBehaviorContainer.m_VarBy.m_Value = CStringW(timeNode.as<PPTX::Logic::Anim>().by.get().c_str());

			tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bToPropertyUsed = timeNode.as<PPTX::Logic::Anim>().to.is_init();
			if(tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bToPropertyUsed)
				tnContainer.m_oTimeAnimateBehaviorContainer.m_VarTo.m_Value = CStringW(timeNode.as<PPTX::Logic::Anim>().to.get().c_str());

			tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bFromPropertyUsed = timeNode.as<PPTX::Logic::Anim>().from.is_init();
			if(tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bFromPropertyUsed)
				tnContainer.m_oTimeAnimateBehaviorContainer.m_VarFrom.m_Value = CStringW(timeNode.as<PPTX::Logic::Anim>().from.get().c_str());

			tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bCalcModePropertyUsed = timeNode.as<PPTX::Logic::Anim>().calcmode.is_init();
			if(tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bCalcModePropertyUsed)
				tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_nCalcMode = GetTLCalcModeFromStr(timeNode.as<PPTX::Logic::Anim>().calcmode.get());

			tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bValueTypePropertyUsed = timeNode.as<PPTX::Logic::Anim>().valueType.is_init();
			if(tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bValueTypePropertyUsed)
				tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_ValueType =
					(Animations::TimeAnimateBehaviorValueTypeEnum)GetTLCalcModeFromStr(timeNode.as<PPTX::Logic::Anim>().calcmode.get());
			else
				tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_ValueType = (Animations::TimeAnimateBehaviorValueTypeEnum)1;

			tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bAnimationValuesPropertyUsed = timeNode.as<PPTX::Logic::Anim>().tavLst.is_init();
			if(tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateBehaviorAtom.m_bAnimationValuesPropertyUsed)
			{
				for(std::list<PPTX::Logic::Tav>::const_iterator i = timeNode.as<PPTX::Logic::Anim>().tavLst->list->begin();
					i != timeNode.as<PPTX::Logic::Anim>().tavLst->list->end(); i++)
				{
					Animations::ODTimeAnimationEntry entry;

					entry.m_VarFormula.m_Value = CStringW(i->fmla.get_value_or("").c_str());
					entry.m_VarValue.m_Value = CStringW(i->val->strVal.get_value_or("").c_str());
					entry.m_oTimeAnimationValueAtom.m_nTime = ParseTLTime(i->tm.get_value_or("indefinite"));

					tnContainer.m_oTimeAnimateBehaviorContainer.m_oAnimateValueList.m_arrEntry.Add(entry);
				}
			}

			CBhvr2TimeBehaviorContainer(timeNode.as<PPTX::Logic::Anim>().cBhvr.get(), tnContainer.m_oTimeAnimateBehaviorContainer.m_oBehavior);
		}
		//else if(timeNode.is<PPTX::Logic::AnimClr>())
		//{
		//	tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
		//	tnContainer.m_b???
		//}
		else if(timeNode.is<PPTX::Logic::AnimEffect>())
		{
			tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
			tnContainer.m_bIsExistsTimeEffectBehavior = true;
			CTN2TimeNodeAtom(timeNode.as<PPTX::Logic::AnimEffect>().cBhvr->cTn.get(), tnContainer);

			if(timeNode.as<PPTX::Logic::AnimEffect>().filter.is_init())
			{
				tnContainer.m_oTimeEffectBehavior.m_oEffectBehaviorAtom.m_bTypePropertyUsed = true;
				//tnContainer.m_oTimeEffectBehavior.m_varType.m_Type = Animations::TL_TVT_String;
				tnContainer.m_oTimeEffectBehavior.m_varType.m_Value = CStringW(timeNode.as<PPTX::Logic::AnimEffect>().filter.get().c_str());
			}
			if(timeNode.as<PPTX::Logic::AnimEffect>().transition.get_value_or("none") != "none")
			{
				tnContainer.m_oTimeEffectBehavior.m_oEffectBehaviorAtom.m_bTransitionPropertyUsed = true;
				tnContainer.m_oTimeEffectBehavior.m_oEffectBehaviorAtom.m_nEffectTransition = timeNode.as<PPTX::Logic::AnimEffect>().transition.get() == "in" ? 0 : 1;
			}

			CBhvr2TimeBehaviorContainer(timeNode.as<PPTX::Logic::AnimEffect>().cBhvr.get(), tnContainer.m_oTimeEffectBehavior.m_oBehavior);
		}
		else if(timeNode.is<PPTX::Logic::AnimMotion>())
		{
			tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
			tnContainer.m_bIsExistsTimeMotionBehavior = true;
			CTN2TimeNodeAtom(timeNode.as<PPTX::Logic::AnimMotion>().cBhvr->cTn.get(), tnContainer);

			tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bPathPropertyUsed = timeNode.as<PPTX::Logic::AnimMotion>().path.is_init();
			if(tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bPathPropertyUsed)
				tnContainer.m_oTimeMotionBehavior.m_VarPath.m_Value = CStringW(timeNode.as<PPTX::Logic::AnimMotion>().path.get().c_str());

			tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bOriginPropertyUsed = timeNode.as<PPTX::Logic::AnimMotion>().origin.is_init();
			if(tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bOriginPropertyUsed)
				tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_nBehaviorOrigin = timeNode.as<PPTX::Logic::AnimMotion>().origin.get() == "parent"? 2 : 0;

			tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bByPropertyUsed
				= timeNode.as<PPTX::Logic::AnimMotion>().byX.is_init() || timeNode.as<PPTX::Logic::AnimMotion>().byY.is_init();
			if(tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bByPropertyUsed)
			{
				tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_nXBY = (float)timeNode.as<PPTX::Logic::AnimMotion>().byX.get_value_or(0);
				tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_nYBY = (float)timeNode.as<PPTX::Logic::AnimMotion>().byY.get_value_or(0);
			}

			tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bFromPropertyUsed
				= timeNode.as<PPTX::Logic::AnimMotion>().fromX.is_init() || timeNode.as<PPTX::Logic::AnimMotion>().fromY.is_init();
			if(tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bFromPropertyUsed)
			{
				tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_nXFROM = (float)timeNode.as<PPTX::Logic::AnimMotion>().fromX.get_value_or(0);
				tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_nYFROM = (float)timeNode.as<PPTX::Logic::AnimMotion>().fromY.get_value_or(0);
			}

			tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bToPropertyUsed
				= timeNode.as<PPTX::Logic::AnimMotion>().toX.is_init() || timeNode.as<PPTX::Logic::AnimMotion>().toY.is_init();
			if(tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_bToPropertyUsed)
			{
				tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_nXTO = (float)timeNode.as<PPTX::Logic::AnimMotion>().toX.get_value_or(0);
				tnContainer.m_oTimeMotionBehavior.m_oMotionBehaviorAtom.m_nYTO = (float)timeNode.as<PPTX::Logic::AnimMotion>().toY.get_value_or(0);
			}

			CBhvr2TimeBehaviorContainer(timeNode.as<PPTX::Logic::AnimMotion>().cBhvr.get(), tnContainer.m_oTimeMotionBehavior.m_oBehavior);
		}
		//else if(timeNode.is<PPTX::Logic::AnimRot>())
		//{
		//	tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
		//	tnContainer.m_b???
		//}
		//else if(timeNode.is<PPTX::Logic::AnimScale>())
		//{
		//	tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
		//	tnContainer.m_b???
		//}
		//else if(timeNode.is<PPTX::Logic::Cmd>())
		//{
		//	tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
		//	tnContainer.m_b???
		//}
		else if(timeNode.is<PPTX::Logic::Set>())
		{
			tnContainer.m_oTimeNodeAtom.m_dwType = Animations::TL_TNT_Behavior;
			tnContainer.m_bIsExistsTimeSetBehaviorContainer = true;
			CTN2TimeNodeAtom(timeNode.as<PPTX::Logic::Set>().cBhvr->cTn.get(), tnContainer);

			tnContainer.m_oTimeSetBehaviorContainer.m_oSetBehaviorAtom.m_bToPropertyUsed =
				((timeNode.as<PPTX::Logic::Set>().to.is_init()) && (timeNode.as<PPTX::Logic::Set>().to->strVal.is_init()));
			if(tnContainer.m_oTimeSetBehaviorContainer.m_oSetBehaviorAtom.m_bToPropertyUsed)
			{
				//tnContainer.m_oTimeSetBehaviorContainer.m_VarTO.m_Type = Animations::TL_TVT_String;
				tnContainer.m_oTimeSetBehaviorContainer.m_VarTO.m_Value = CStringW(timeNode.as<PPTX::Logic::Set>().to->strVal.get().c_str());
			}
			CBhvr2TimeBehaviorContainer(timeNode.as<PPTX::Logic::Set>().cBhvr.get(), tnContainer.m_oTimeSetBehaviorContainer.m_oBehavior);
		}
	}

	void Converter::CBhvr2TimeBehaviorContainer(const PPTX::Logic::CBhvr& bhvr, Animations::ODTimeBehaviorContainer& container)
	{
		//container.m_oBehaviorAtom.m_bAdditivePropertyUsed = bhvr.additive.is_init();
		//DWORD	m_nBehaviorAdditive;
		container.m_oBehaviorAtom.m_bAttributeNamesPropertyUsed = bhvr.attrNameLst.is_init();
		if(container.m_oBehaviorAtom.m_bAttributeNamesPropertyUsed)
		{
			container.m_oStringList.m_Values.RemoveAll();
			Animations::ODTimeVariantString varString;
			//varString.m_Type = Animations::TL_TVT_String;
			for(std::list<PPTX::Logic::AttrName>::const_iterator i = bhvr.attrNameLst->list->begin(); i != bhvr.attrNameLst->list->end(); i++)
			{
				varString.m_Value = CStringW(i->text.get().c_str());
				container.m_oStringList.m_Values.Add(varString);
			}
		}

		container.m_oBehaviorAtom.m_nBehaviorAccumulate = 0;//bhvr.accumulate.get_value_or("none") == "none" ? 0 : 1;
		container.m_oBehaviorAtom.m_nBehaviorTransform = 0;//bhvr.xfrm.get_value_or(???)???
/*
//		bool							m_bIsExistPropertyList;
//		ODTimePropertyList4TimeBehavior	m_oPropertyList;
*/
		container.m_oClientVisualElement.m_bVisualShapeAtom = bhvr.tgtEl->spTgt.is_init();
		if(container.m_oClientVisualElement.m_bVisualShapeAtom)
		{
			container.m_oClientVisualElement.m_oVisualShapeAtom.m_Type = Animations::TL_TVET_Shape;
			container.m_oClientVisualElement.m_oVisualShapeAtom.m_RefType = Animations::TL_ET_ShapeType;
			container.m_oClientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef = Parse<int>(bhvr.tgtEl->spTgt->spid.get());
		}

		container.m_oClientVisualElement.m_bVisualPageAtom = ((!bhvr.tgtEl->spTgt.is_init()) && (!bhvr.tgtEl->inkTgt.is_init()) && (!bhvr.tgtEl->embed.is_init()));
		if(container.m_oClientVisualElement.m_bVisualPageAtom)
			container.m_oClientVisualElement.m_oVisualPageAtom.m_Type = Animations::TL_TVET_Page;
	}

	void Converter::CTN2TimeNodeAtom(const PPTX::Logic::CTn& cTn, Animations::ODExtTimeNodeContainer& container)
	{
		if(cTn.presetClass.is_init())
			container.m_oTimePropertyList.m_EffectType.m_Value = GetTLPresetClassFromStr(cTn.presetClass.get());
		if(cTn.presetID.is_init())
			container.m_oTimePropertyList.m_EffectID.m_Value = cTn.presetID.get_value_or(0);
		if(cTn.presetSubtype.is_init())
			container.m_oTimePropertyList.m_EffectDir.m_Value = cTn.presetSubtype.get_value_or(0);
		if(cTn.nodeType.is_init())
			container.m_oTimePropertyList.m_EffectNodeType.m_Value = GetTLNodeTypeFromStr(cTn.nodeType.get());

		container.m_oTimeNodeAtom.m_bRestartProperty = cTn.restart.is_init();
		if(container.m_oTimeNodeAtom.m_bRestartProperty)
			container.m_oTimeNodeAtom.m_dwRestart = GetTLRestartFromStr(cTn.restart.get());
//		bool				m_bFillProperty;
//		DWORD				m_dwFill;
		container.m_oTimeNodeAtom.m_bDurationProperty = true;//(ParseTLTime(cTn.dur.get_value_or("indefinite")) >= 0);
//		if(container.m_oTimeNodeAtom.m_bDurationProperty)
			container.m_oTimeNodeAtom.m_dwDuration = ParseTLTime(cTn.dur.get_value_or("indefinite"));

		if(cTn.stCondLst.is_init())
			AddCondLst(cTn.stCondLst.get(), container);
	}

	void Converter::AddCondLst(const PPTX::Logic::CondLst& condLst, Animations::ODExtTimeNodeContainer& container)
	{
		for(std::list<PPTX::Logic::Cond>::const_iterator i = condLst.list->begin(); i != condLst.list->end(); i++)
		{
			Animations::ODTimeConditionContainer* odtcc = new Animations::ODTimeConditionContainer();

			if(i->tgtEl.is_init())
				odtcc->m_oTimeConditionAtom.m_TriggerObject = Animations::TL_TOT_VisualElement;
			else if(i->tn.is_init())
				odtcc->m_oTimeConditionAtom.m_TriggerObject = Animations::TL_TOT_TimeNode;
			else if(i->rtn.is_init())
				odtcc->m_oTimeConditionAtom.m_TriggerObject = Animations::TL_TOT_RuntimeNodeRef;
			else
				odtcc->m_oTimeConditionAtom.m_TriggerObject = Animations::TL_TOT_None;
//			if(i->delay.get_value_or("indefinite") != "indefinite")
				odtcc->m_oTimeConditionAtom.m_nTimeDelay = ParseTLTime(i->delay.get_value_or("indefinite"));
//			else
//				odtcc->m_oTimeConditionAtom.m_nTimeDelay = 0;
			container.m_arrTimeConditionRF.Add(odtcc);
		}
	}

	int Converter::ParseTLTime(const std::string& str)
	{
		if(str == "indefinite") return 0;//-1;
		return Parse<int>(str);
	}
} // namespace PPTX2DrawingXML