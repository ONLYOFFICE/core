﻿/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "./../../WrapperWritingElement.h"
#include "CondLst.h"
#include "Cond.h"
#include "TnLst.h"
#include "Iterate.h"
#include "ChildTnLst.h"
#include "./../../Limit/TLRestart.h"
#include "./../../Limit/TLNodeType.h"
#include "./../../Limit/TLNodeFillType.h"
#include "./../../Limit/TLPresetClass.h"
#include "./../../Limit/TLMasterRelation.h"
#include "./../../Limit/TLSyncBehavior.h"

namespace PPTX
{
	namespace Logic
	{
		class CTn : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CTn)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable<CondLst>			stCondLst;
			nullable<CondLst>			endCondLst;
			nullable<Cond>				endSync;
			nullable<Iterate>			iterate;
            nullable<ChildTnLst>        childTnLst;
			nullable<TnLst>				subTnLst;

			nullable_int							accel;
			nullable_bool							afterEffect;
			nullable_bool							autoRev;
			nullable_int							bldLvl;
			nullable_int							decel;
			nullable_bool							display;
			nullable_string							dur;	//unsigned int/"indefinite"
			nullable_string							evtFilter;
			nullable_limit<Limit::TLNodeFillType>	fill;
			nullable_int							grpId;
			nullable_int							id;
			nullable_limit<Limit::TLMasterRelation> masterRel;
			nullable_bool							nodePh;
			nullable_limit<Limit::TLNodeType>		nodeType;
			nullable_limit<Limit::TLPresetClass>	presetClass;
			nullable_int							presetID;
			nullable_int							presetSubtype;
			nullable_string							repeatCount;
			nullable_string							repeatDur;
			nullable_limit<Limit::TLRestart>		restart;
			nullable_int							spd;
			nullable_limit<Limit::TLSyncBehavior>	syncBehavior;
			nullable_string							tmFilter;

		protected:
			virtual void FillParentPointersForChilds();
			void Normalize();
		};
	} // namespace Logic
} // namespace PPTX
