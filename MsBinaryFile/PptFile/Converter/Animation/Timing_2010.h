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

#include "../../PPTXWriter/ImageManager.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Timing.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Par.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Seq.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Audio.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimClr.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimEffect.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimMotion.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimRot.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/AnimScale.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Cmd.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Video.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Anim.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/Set.h"

#include "../../../../OOXML/PPTXFormat/Logic/Timing/BldP.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/BldDgm.h"
#include "../../../../OOXML/PPTXFormat/Logic/Timing/BldOleChart.h"

#include "../../Records/Animations/ParaBuildContainer.h"
#include "../../Records/Animations/ChartBuildContainer.h"
#include "../../Records/Animations/DiagramBuildContainer.h"

#include "intermediate_anim.h" // it using


namespace PPT {
	namespace Converter {
		class Animation_2010;

		class Timing_2010
		{
		public:
            Timing_2010(const Intermediate::SlideAnimation& sldAnim);
			void Convert(PPTX::Logic::Timing& timing, CExMedia* pExMedia, CRelsGenerator* pRels);

		public:
			enum TimeNodeLevel {
				zero,
				root = 1,
				mainSeqOrTrigger = 2,
				eventOrClickGroup = 3,
				parallelShow = 4,
				oneAnim = 5,
				animEffectDescription = 6
			};


		private:
			void ConvertBldLst(PPTX::Logic::Timing& timimg, CRecordBuildListContainer *pBLC);
			void InsertBuildNode(PPTX::Logic::BldLst &bldLst, PPTX::Logic::BuildNodeBase &bnb);
			void InsertBldP(PPTX::Logic::BldLst &bldLst, PPTX::Logic::BuildNodeBase &bnb);
			void FillBuildNodeBase(CRecordBuildListSubContainer* pSub, PPTX::Logic::BuildNodeBase &oBuildNodeBase);
			void FillBldP(CRecordParaBuildContainer *pPBC, PPTX::Logic::BldP &oBP);
			void FillBldOleChart(CRecordChartBuildContainer* pCBC, PPTX::Logic::BldOleChart &oBP);
			void FillBldDgm(CRecordDiagramBuildContainer *pDBC, PPTX::Logic::BldDgm &oBP);

			void ConvertTnLst(PPTX::Logic::Timing &timing, CRecordExtTimeNodeContainer* pETNC);
			bool FillTnChild(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::TimeNodeBase &oChild);
			void FillSeq(PPT::CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::Seq& oSec);
			void FillPar(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::Par &oPar);
			void FillCBhvr(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::CBhvr &oBhvr);
			void FillCBhvr(
				CRecordTimeBehaviorContainer *pBhvr,
				PPTX::Logic::CBhvr &oBhvr);
            bool isTextShape(UINT spid) const;
            void FillTgtEl(PPTX::Logic::TgtEl& tgtEl, CRecordClientVisualElementContainer &clientVisualElement);
			void FillCBhvr(
				PPTX::Logic::CBhvr &oBhvr, int dur,
				UINT spid, std::wstring attrname, int delay);
			void FillSet(
				PPT::CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::Set& oSet);
			void FillAnim(
				CRecordTimeAnimateBehaviorContainer *pTimeAnimateBehavior,
				PPTX::Logic::Anim &oAnim);

			void FillCTnRecursive(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn);
			bool CheckAnimation5Level(const CRecordExtTimeNodeContainer *pETNC, const PPTX::Logic::CTn &oCTn);
            bool CheckSlideSpid(_INT32 spid);
            bool IsSlideSpId(_INT32 spid) const;
            bool CheckMainSeqSyncWithAnim95(_INT32 spid) const;
			bool IsCorrectAnimationSpId(_INT32 spid) const;
			void InsertAnimationSpId(_INT32 spid);
			void ConvertChildTnLst(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn);
			void ConvertCTnIterate(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn);
			void ConvertCTnEndSync(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn);
			void ConvertCTnSubTnLst(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn);
			void ConvertCTnStCondLst(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn);
			void FillCond(PPT::CRecordTimeConditionContainer *oldCond, PPTX::Logic::Cond &cond);
			void FillStCondLst(const std::vector<CRecordTimeConditionContainer*>& timeCondCont, PPTX::Logic::CondLst& stCondLst);
			void FillSubTnLst(std::vector<CRecordSubEffectContainer*> &vecSEC, PPTX::Logic::TnLst &oSubTnLst);
			void FillCondLst(std::vector<CRecordTimeConditionContainer*>& oCondVec, PPTX::Logic::CondLst &oCondLst);
			void FillEmptyTargetCond(PPTX::Logic::Cond &cond);
			void FillCTnProps(CRecordTimePropertyList4TimeNodeContainer *pProp, PPTX::Logic::CTn &oCTn);
			void FillCTnHeadArgs(CRecordExtTimeNodeContainer *pETNC, PPTX::Logic::CTn &oCTn);


			void FillAnimClr(
				CRecordTimeColorBehaviorContainer *pColor,
				CRecordTimePropertyList4TimeNodeContainer *pProp,
				PPTX::Logic::AnimClr &oAnimClr);
			void FillAnimEffect(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::AnimEffect &oAnim);
			void FillAnimMotion(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::AnimMotion &oAnim);
			void FillAnimRot(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::AnimRot &oAnim);
			void FillAnimScale(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::AnimScale &oAnim);
			void FillAudio(CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::Audio &oAudio);
			void FillAudio(CRecordClientVisualElementContainer *pCVEC,
				PPTX::Logic::Audio &oAudio);
			void FillCmd(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::Cmd &oCmd);
			void FillVideo(
				CRecordExtTimeNodeContainer *pETNC,
				PPTX::Logic::Video &oVideo);

		private:
            const Intermediate::SlideAnimation& slideAnim;
            std::unordered_set<int> correctAnimatedShapes;
			CExMedia *m_pExMedia = nullptr;
			CRelsGenerator *m_pRels = nullptr;
			bool isMainSeq = false;

			PPTX::Logic::BldLst *m_pBldLst = nullptr; // Do not delete
			PPTX::Logic::BldP   *m_currentBldP = nullptr;

			int cTnId = 1;
            int cTNLevel = TimeNodeLevel::zero;
		};

	}
}
