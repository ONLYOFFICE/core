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
