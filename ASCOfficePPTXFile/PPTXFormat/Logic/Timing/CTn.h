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
#ifndef PPTX_LOGIC_CTN_INCLUDE_H_
#define PPTX_LOGIC_CTN_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CondLst.h"
#include "Cond.h"
#include "TnLst.h"
#include "Iterate.h"
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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
                XmlMacroReadAttributeBase(node, L"accel", accel);
                XmlMacroReadAttributeBase(node, L"afterEffect", afterEffect);
                XmlMacroReadAttributeBase(node, L"autoRev", autoRev);
                XmlMacroReadAttributeBase(node, L"bldLvl", bldLvl);
                XmlMacroReadAttributeBase(node, L"decel", decel);
                XmlMacroReadAttributeBase(node, L"display", display);
                XmlMacroReadAttributeBase(node, L"dur", dur);
                XmlMacroReadAttributeBase(node, L"evtFilter", evtFilter);
                XmlMacroReadAttributeBase(node, L"fill", fill);
                XmlMacroReadAttributeBase(node, L"grpId", grpId);
                XmlMacroReadAttributeBase(node, L"id", id);
                XmlMacroReadAttributeBase(node, L"masterRel", masterRel);
                XmlMacroReadAttributeBase(node, L"nodePh", nodePh);
                XmlMacroReadAttributeBase(node, L"nodeType", nodeType);
                XmlMacroReadAttributeBase(node, L"presetClass", presetClass);
                XmlMacroReadAttributeBase(node, L"presetID", presetID);
                XmlMacroReadAttributeBase(node, L"presetSubtype", presetSubtype);
                XmlMacroReadAttributeBase(node, L"repeatCount", repeatCount);
                XmlMacroReadAttributeBase(node, L"repeatDur", repeatDur);
                XmlMacroReadAttributeBase(node, L"restart", restart);
                XmlMacroReadAttributeBase(node, L"spd", spd);
                XmlMacroReadAttributeBase(node, L"syncBehavior", syncBehavior);
                XmlMacroReadAttributeBase(node, L"tmFilter", tmFilter);

				stCondLst	= node.ReadNode(_T("p:stCondLst"));
				endCondLst	= node.ReadNode(_T("p:endCondLst"));
				endSync		= node.ReadNode(_T("p:endSync"));
				iterate		= node.ReadNode(_T("p:iterate"));
				childTnLst	= node.ReadNode(_T("p:childTnLst"));
				subTnLst	= node.ReadNode(_T("p:subTnLst"));

				Normalize();

				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;

				oAttr.Write(_T("id"), id);
				oAttr.Write(_T("presetID"), presetID);
				oAttr.WriteLimitNullable(_T("presetClass"), presetClass);
				oAttr.Write(_T("presetSubtype"), presetSubtype);
				oAttr.Write(_T("dur"), dur);
				oAttr.Write(_T("repeatCount"), repeatCount);
				oAttr.Write(_T("repeatDur"), repeatDur);
				oAttr.Write(_T("spd"), spd);
				oAttr.Write(_T("accel"), accel);
				oAttr.Write(_T("decel"), decel);
				oAttr.Write(_T("autoRev"), autoRev);
				oAttr.WriteLimitNullable(_T("restart"), restart);
				oAttr.WriteLimitNullable(_T("fill"), fill);
				oAttr.WriteLimitNullable(_T("syncBehavior"), syncBehavior);
				oAttr.Write(_T("tmFilter"), tmFilter);
				oAttr.Write(_T("evtFilter"), evtFilter);
				oAttr.Write(_T("display"), display);
				oAttr.WriteLimitNullable(_T("masterRel"), masterRel);
				oAttr.Write(_T("bldLvl"), bldLvl);
				oAttr.Write(_T("grpId"), grpId);
				oAttr.Write(_T("afterEffect"), afterEffect);
				oAttr.WriteLimitNullable(_T("nodeType"), nodeType);
				oAttr.Write(_T("nodePh"), nodePh);


				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(stCondLst);
				oValue.WriteNullable(endCondLst);
				oValue.WriteNullable(endSync);
				oValue.WriteNullable(iterate);
				oValue.WriteNullable(childTnLst);
				oValue.WriteNullable(subTnLst);

				return XmlUtils::CreateNode(_T("p:cTn"), oAttr, oValue);
			}

		public:
			//Childs
			nullable<CondLst>			stCondLst;
			nullable<CondLst>			endCondLst;
			nullable<Cond>				endSync;
			nullable<Iterate>			iterate;
			nullable<TnLst>				childTnLst;
			nullable<TnLst>				subTnLst;


			//Attributes
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
			virtual void FillParentPointersForChilds()
			{
				if(stCondLst.IsInit())
					stCondLst->SetParentPointer(this);
				if(endCondLst.IsInit())
					endCondLst->SetParentPointer(this);
				if(endSync.IsInit())
					endSync->SetParentPointer(this);
				if(iterate.IsInit())
					iterate->SetParentPointer(this);
				if(childTnLst.IsInit())
					childTnLst->SetParentPointer(this);
				if(subTnLst.IsInit())
					subTnLst->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				grpId.normalize_positive();
				id.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CTN_INCLUDE_H
