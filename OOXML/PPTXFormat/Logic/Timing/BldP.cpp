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

#include "BldP.h"

namespace PPTX
{
	namespace Logic
	{		
		void BldP::fromXML(XmlUtils::CXmlNode& node)
		{
			tmplLst				= node.ReadNode(_T("p:tmplLst"));

			spid				= node.GetAttribute(_T("spid"));
			grpId				= node.ReadAttributeInt(L"grpId");
			XmlMacroReadAttributeBase(node, L"uiExpand", uiExpand);
			XmlMacroReadAttributeBase(node, L"build", build);
			XmlMacroReadAttributeBase(node, L"bldLvl", bldLvl);
			XmlMacroReadAttributeBase(node, L"animBg", animBg);
			XmlMacroReadAttributeBase(node, L"autoUpdateAnimBg", autoUpdateAnimBg);
			XmlMacroReadAttributeBase(node, L"rev", rev);
			XmlMacroReadAttributeBase(node, L"advAuto", advAuto);

			Normalize();
			FillParentPointersForChilds();
		}
		OOX::EElementType BldP::getType() const
		{
			return OOX::et_p_bldP;
		}
		std::wstring BldP::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("spid"), spid);
			oAttr.Write(_T("grpId"), grpId);
			oAttr.Write(_T("uiExpand"), uiExpand);
			oAttr.WriteLimitNullable(_T("build"), build);
			oAttr.Write(_T("bldLvl"), bldLvl);
			oAttr.Write(_T("animBg"), animBg);
			oAttr.Write(_T("autoUpdateAnimBg"), autoUpdateAnimBg);
			oAttr.Write(_T("rev"), rev);
			oAttr.Write(_T("advAuto"), advAuto);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(tmplLst);

			return XmlUtils::CreateNode(_T("p:bldP"), oAttr, oValue);
		}
		void BldP::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void BldP::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, build);
				pWriter->WriteBool2(1, uiExpand);
				pWriter->WriteString1(2, spid);
				pWriter->WriteInt1(3, grpId);
				pWriter->WriteInt2(4, bldLvl);
				pWriter->WriteBool2(5, animBg);
				pWriter->WriteBool2(6, autoUpdateAnimBg);
				pWriter->WriteBool2(7, rev);
				pWriter->WriteString2(8, advAuto);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, tmplLst);
		}
		void BldP::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	build = pReader->GetUChar();
				else if (1 == _at)	uiExpand = pReader->GetBool();
				else if (2 == _at)	spid = pReader->GetString2();
				else if (3 == _at)	grpId = pReader->GetLong();
				else if (4 == _at)	bldLvl = pReader->GetLong();
				else if (5 == _at)	animBg = pReader->GetBool();
				else if (6 == _at)	autoUpdateAnimBg = pReader->GetBool();
				else if (7 == _at)	rev = pReader->GetBool();
				else if (8 == _at)	advAuto = pReader->GetString2();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						tmplLst = new Logic::TmplLst();
						tmplLst->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void BldP::FillParentPointersForChilds()
		{
			if(tmplLst.IsInit())
				tmplLst->SetParentPointer(this);
		}
		void BldP::Normalize()
		{
			if (grpId < 0)
				grpId = 0;

			if (bldLvl.IsInit())
				if (*bldLvl < 0)
					*bldLvl = 0;
		}
	} // namespace Logic
} // namespace PPTX
