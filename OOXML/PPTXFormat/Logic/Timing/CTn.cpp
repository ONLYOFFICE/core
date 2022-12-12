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

#include "CTn.h"

namespace PPTX
{
	namespace Logic
	{
		void CTn::fromXML(XmlUtils::CXmlNode& node)
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

			stCondLst	= node.ReadNode(L"p:stCondLst");
			childTnLst	= node.ReadNode(L"p:childTnLst");
			endCondLst	= node.ReadNode(L"p:endCondLst");
			endSync		= node.ReadNode(L"p:endSync");
			iterate		= node.ReadNode(L"p:iterate");

			subTnLst	= node.ReadNode(L"p:subTnLst");

			Normalize();

			FillParentPointersForChilds();
		}
		std::wstring CTn::toXML() const
		{
			XmlUtils::CAttribute oAttr;

			oAttr.Write(L"id", id);
			oAttr.Write(L"presetID", presetID);
			oAttr.WriteLimitNullable(L"presetClass", presetClass);
			oAttr.Write(L"presetSubtype", presetSubtype);
			oAttr.Write(L"dur", dur);
			oAttr.Write(L"repeatCount", repeatCount);
			oAttr.Write(L"repeatDur", repeatDur);
			oAttr.Write(L"spd", spd);
			oAttr.Write(L"accel", accel);
			oAttr.Write(L"decel", decel);
			oAttr.Write(L"autoRev", autoRev);
			oAttr.WriteLimitNullable(L"restart", restart);
			oAttr.WriteLimitNullable(L"fill", fill);
			oAttr.WriteLimitNullable(L"syncBehavior", syncBehavior);
			oAttr.Write(L"tmFilter", tmFilter);
			oAttr.Write(L"evtFilter", evtFilter);
			oAttr.Write(L"display", display);
			oAttr.WriteLimitNullable(L"masterRel", masterRel);
			oAttr.Write(L"bldLvl", bldLvl);
			oAttr.Write(L"grpId", grpId);
			oAttr.Write(L"afterEffect", afterEffect);
			oAttr.WriteLimitNullable(L"nodeType", nodeType);
			oAttr.Write(L"nodePh", nodePh);

			XmlUtils::CNodeValue oValue;

			oValue.WriteNullable(stCondLst);
			oValue.WriteNullable(endCondLst);
			oValue.WriteNullable(endSync);
			oValue.WriteNullable(iterate);
			oValue.WriteNullable(childTnLst);
			oValue.WriteNullable(subTnLst);

			return XmlUtils::CreateNode(L"p:cTn", oAttr, oValue);
		}
		void CTn::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:cTn");
				pWriter->WriteAttribute(L"id", id);
				pWriter->WriteAttribute(L"presetID", presetID);
				pWriter->WriteAttribute(L"presetClass", presetClass);
				pWriter->WriteAttribute(L"presetSubtype", presetSubtype);
				pWriter->WriteAttribute(L"dur", dur);
				pWriter->WriteAttribute(L"repeatCount", repeatCount);
				pWriter->WriteAttribute(L"repeatDur", repeatDur);
				pWriter->WriteAttribute(L"spd", spd);
				pWriter->WriteAttribute(L"accel", accel);
				pWriter->WriteAttribute(L"decel", decel);
				pWriter->WriteAttribute(L"autoRev", autoRev);
				pWriter->WriteAttribute(L"restart", restart);
				pWriter->WriteAttribute(L"fill", fill);
				pWriter->WriteAttribute(L"syncBehavior", syncBehavior);
				pWriter->WriteAttribute(L"tmFilter", tmFilter);
				pWriter->WriteAttribute(L"evtFilter", evtFilter);
				pWriter->WriteAttribute(L"display", display);
				pWriter->WriteAttribute(L"masterRel", masterRel);
				pWriter->WriteAttribute(L"bldLvl", bldLvl);
				pWriter->WriteAttribute(L"grpId", grpId);
				pWriter->WriteAttribute(L"afterEffect", afterEffect);
				pWriter->WriteAttribute(L"nodeType", nodeType);
			pWriter->EndAttributes();

			if (stCondLst.IsInit())
				stCondLst->toXmlWriter(pWriter);

			if (endCondLst.IsInit())
				endCondLst->toXmlWriter(pWriter);

			if (endSync.IsInit())
				endSync->toXmlWriter(pWriter);

			if (iterate.IsInit())
				iterate->toXmlWriter(pWriter);

			if (childTnLst.IsInit())
				childTnLst->toXmlWriter(pWriter);

			if (subTnLst.IsInit())
				subTnLst->toXmlWriter(pWriter);

			pWriter->EndNode(L"p:cTn");
		}
		void CTn::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at) accel = pReader->GetLong();
				else if (1 == _at)	afterEffect = pReader->GetBool();
				else if (2 == _at)	autoRev = pReader->GetBool();
				else if (3 == _at)	fill = pReader->GetUChar();
				else if (4 == _at)	masterRel = pReader->GetUChar();
				else if (5 == _at)	nodeType = pReader->GetUChar();
				else if (6 == _at)	presetClass = pReader->GetUChar();
				else if (7 == _at)	restart = pReader->GetUChar();
				else if (8 == _at)	syncBehavior = pReader->GetUChar();
				else if (9 == _at)	display = pReader->GetBool();
				else if (10 == _at)	nodePh = pReader->GetBool();
				else if (11 == _at)	bldLvl = pReader->GetLong();
				else if (12 == _at)	decel = pReader->GetLong();
				else if (13 == _at)	bldLvl = pReader->GetLong();
				else if (14 == _at)	grpId = pReader->GetLong();
				else if (15 == _at)	id = pReader->GetLong();
				else if (16 == _at)	presetID = pReader->GetLong();
				else if (17 == _at)	presetSubtype = pReader->GetLong();
				else if (18 == _at)	spd = pReader->GetLong();
				else if (19 == _at)	dur = pReader->GetString2();
				else if (20 == _at)	evtFilter = pReader->GetString2();
				else if (21 == _at)	repeatCount = pReader->GetString2();
				else if (22 == _at)	repeatDur = pReader->GetString2();
				else if (23 == _at)	tmFilter = pReader->GetString2();

			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						stCondLst.Init(); stCondLst->node_name = L"stCondLst";
						stCondLst->fromPPTY(pReader);
					}break;
					case 1:
					{
						endCondLst.Init(); endCondLst->node_name = L"endCondLst";
						endCondLst->fromPPTY(pReader);
					}break;
					case 2:
					{
						endSync.Init(); endSync->node_name = L"endSync";
						endSync->fromPPTY(pReader);
					}break;
					case 3:
					{
						iterate.Init();
						iterate->fromPPTY(pReader);
					}break;
					case 4:
					{
						childTnLst.Init();
						childTnLst->fromPPTY(pReader);
					}break;
					case 5:
					{
						subTnLst.Init();
						subTnLst->node_name = L"subTnLst";
						subTnLst->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();

					}break;
				}
			}
			pReader->Seek(end);
		}
		void CTn::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, accel);
				pWriter->WriteBool2(1, afterEffect);
				pWriter->WriteBool2(2, autoRev);
				pWriter->WriteLimit2(3, fill);
				pWriter->WriteLimit2(4, masterRel);
				pWriter->WriteLimit2(5, nodeType);
				pWriter->WriteLimit2(6, presetClass);
				pWriter->WriteLimit2(7, restart);
				pWriter->WriteLimit2(8, syncBehavior);
				pWriter->WriteBool2(9, display);
				pWriter->WriteBool2(10, nodePh);
				pWriter->WriteInt2(11, bldLvl);
				pWriter->WriteInt2(12, decel);
				pWriter->WriteInt2(13, bldLvl);
				pWriter->WriteInt2(14, grpId);
				pWriter->WriteInt2(15, id);
				pWriter->WriteInt2(16, presetID);
				pWriter->WriteInt2(17, presetSubtype);
				pWriter->WriteInt2(18, spd);
				pWriter->WriteString2(19, dur);
				pWriter->WriteString2(20, evtFilter);
				pWriter->WriteString2(21, repeatCount);
				pWriter->WriteString2(22, repeatDur);
				pWriter->WriteString2(23, tmFilter);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, stCondLst);
			pWriter->WriteRecord2(1, endCondLst);
			pWriter->WriteRecord2(2, endSync);
			pWriter->WriteRecord2(3, iterate);
			pWriter->WriteRecord2(4, childTnLst);
			pWriter->WriteRecord2(5, subTnLst);
		}
		void CTn::FillParentPointersForChilds()
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
		void CTn::Normalize()
		{
			grpId.normalize_positive();
			id.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
