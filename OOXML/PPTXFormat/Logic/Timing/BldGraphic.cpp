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

#include "BldGraphic.h"

namespace PPTX
{
	namespace Logic
	{
		void BldGraphic::fromXML(XmlUtils::CXmlNode& node)
		{
			bldSub		= node.ReadNode(_T("p:bldSub"));
			XmlMacroReadAttributeBase(node, L"spid", spid);
			grpId = node.ReadAttributeInt(L"grpId");
			XmlMacroReadAttributeBase(node, L"uiExpand", uiExpand);

			Normalize();

			FillParentPointersForChilds();
		}
		std::wstring BldGraphic::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("spid"), spid);
			oAttr.Write(_T("grpId"), grpId);
			oAttr.Write(_T("uiExpand"), uiExpand);

			if (!bldSub.IsInit())
				return XmlUtils::CreateNode(_T("p:bldGraphic"), oAttr, _T("<p:bldAsOne/>"));

			return XmlUtils::CreateNode(_T("p:bldGraphic"), oAttr, bldSub->toXML());
		}
		void BldGraphic::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		OOX::EElementType BldGraphic::getType() const
		{
			return OOX::et_p_bldGraphic;
		}
		void BldGraphic::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, uiExpand);
				pWriter->WriteString1(1, spid);
				pWriter->WriteInt1(2, grpId);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, bldSub);
		}
		void BldGraphic::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	uiExpand = pReader->GetBool();
				else if (1 == _at)	spid = pReader->GetString2();
				else if (2 == _at)	grpId = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						bldSub = new Logic::BldSub();
						bldSub->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void BldGraphic::FillParentPointersForChilds()
		{
			if(bldSub.IsInit())
				bldSub->SetParentPointer(this);
		}
		void BldGraphic::Normalize()
		{
			if (grpId < 0)
				grpId = 0;
		}
	} // namespace Logic
} // namespace PPTX
