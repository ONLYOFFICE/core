/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
