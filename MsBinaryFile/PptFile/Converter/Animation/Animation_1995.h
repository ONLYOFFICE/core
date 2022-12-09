#pragma once

#include "intermediate_anim.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/ChildTnLst.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Anim.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimEffect.h"


namespace PPT {
	namespace Converter {
		class Animation_1995
		{
		public:
			Animation_1995(_INT32& cTnId);
			void FillCTnAnimation(PPTX::Logic::CTn &oCTN, Intermediate::SOldAnimation *pOldAnim);

		private:
			void FillAnim(PPTX::Logic::Anim& oAnim, Intermediate::SOldAnimation* pOldAnim, int dur, std::wstring attrname,
				Intermediate::SValue val1, Intermediate::SValue val2, std::wstring fmla = L"");
			void FillAnimEffect(PPTX::Logic::AnimEffect& oAnimEffect, Intermediate::SOldAnimation* pOldAnim, std::wstring filter, std::wstring transition = L"in");

			void FillCBhvrForAnim(PPTX::Logic::Anim& oAnim, Intermediate::SOldAnimation* pOldAnim, int dur, std::wstring attrname);
			void FillCBhvr(PPTX::Logic::CBhvr &oBhvr, int dur, UINT spid, std::wstring attrname, int delay = 499);
			void FillCBhvr(PPTX::Logic::CBhvr &oCBhvr, Intermediate::SOldAnimation* pOldAnim, int delay = 499);

			// This methods fill ChildTnLst with anim nodes
			void ConvertAppear(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim);
			void ConvertFlyIn(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertBlinds(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertShape(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertCheckerboard(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim);
			void ConvertCrawlIn(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertDissolveIn(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim);
			void ConvertFade(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim);
			void ConvertFlashOnce(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertPeekIn(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertRandomBars(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertSpiralIn(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim);
			void ConvertSplit(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertStretch(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertStrips(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertBasicSwivel(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertWipe(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertBasicZoom(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim, int& presetSub);
			void ConvertRandomEffect(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation* pOldAnim);

			void PushAnim(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation *pOldAnim, int dur,
				std::wstring attrname1, Intermediate::SValue val1, Intermediate::SValue val2,
				std::wstring attrname2, Intermediate::SValue val3, Intermediate::SValue val4,
				std::wstring fmla1 = L"", std::wstring fmla2 = L"");
			void PushAnim(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation *pOldAnim,
				std::wstring attrname1, Intermediate::SValue val1, Intermediate::SValue val2, std::wstring fmla1 = L"");

			void PushAnimEffect(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation *pOldAnim, std::wstring filter, std::wstring transition = L"in");
			void PushSet(PPTX::Logic::ChildTnLst& oParent, Intermediate::SOldAnimation *pOldAnim, int dur = 0);

		private:
			_INT32 & cTnId;
		};
	}
}
