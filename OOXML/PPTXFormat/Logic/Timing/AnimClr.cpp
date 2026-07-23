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

#include "AnimClr.h"

namespace PPTX
{
	namespace Logic
	{
		void AnimClr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"clrSpc", clrSpc);
			XmlMacroReadAttributeBase(node, L"dir", dir);

			XmlUtils::CXmlNode oNodeBhvr;
			if (node.GetNode(L"p:cBhvr", oNodeBhvr))
				cBhvr.fromXML(oNodeBhvr);

			XmlUtils::CXmlNode oNodeFrom;
			if (node.GetNode(L"p:from", oNodeFrom))
				from.GetColorFrom(oNodeFrom);

			XmlUtils::CXmlNode oNodeTo;
			if (node.GetNode(L"p:to", oNodeTo))
				to.GetColorFrom(oNodeTo);

			XmlUtils::CXmlNode oNodeBy;
			if (node.GetNode(L"p:by", oNodeBy))
			{
				XmlUtils::CXmlNode oRGB;
				XmlUtils::CXmlNode oHSL;
				if (oNodeBy.GetNode(L"p:rgb", oRGB))
				{
					XmlMacroReadAttributeBase(oRGB, L"r", byR);
					XmlMacroReadAttributeBase(oRGB, L"g", byG);
					XmlMacroReadAttributeBase(oRGB, L"b", byB);
				}
				else if (oNodeBy.GetNode(L"p:hsl", oHSL))
				{
					XmlMacroReadAttributeBase(oHSL, L"h", byH);
					XmlMacroReadAttributeBase(oHSL, L"s", byS);
					XmlMacroReadAttributeBase(oHSL, L"l", byL);
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring AnimClr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(L"clrSpc", clrSpc);
			oAttr.WriteLimitNullable(L"dir", dir);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cBhvr);

			if (byR.IsInit() && byG.IsInit() && byB.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(L"r", byR);
				oAttr1.Write(L"g", byG);
				oAttr1.Write(L"b", byB);

				oValue.m_strValue += (L"<p:by>") + XmlUtils::CreateNode(L"p:rgb", oAttr1) + L"</p:by>";
			}
			else if (byH.IsInit() && byS.IsInit() && byL.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(L"h", byH);
				oAttr1.Write(L"s", byS);
				oAttr1.Write(L"l", byL);

				oValue.m_strValue += (L"<p:by>") + XmlUtils::CreateNode(L"p:hsl", oAttr1) + L"</p:by>";
			}

			if (from.is_init())
				oValue.m_strValue += XmlUtils::CreateNode(L"p:from", from.toXML());
			if (to.is_init())
				oValue.m_strValue += XmlUtils::CreateNode(L"p:to", to.toXML());

			return XmlUtils::CreateNode(L"p:animClr", oAttr, oValue);
		}
		OOX::EElementType AnimClr::getType() const
		{
			return OOX::et_p_animClr;
		}
		void AnimClr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void AnimClr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, clrSpc);
				pWriter->WriteLimit2(1, dir);
				pWriter->WriteInt2(2, byR);
				pWriter->WriteInt2(3, byG);
				pWriter->WriteInt2(4, byB);
				pWriter->WriteInt2(5, byH);
				pWriter->WriteInt2(6, byS);
				pWriter->WriteInt2(7, byL);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, cBhvr);
			pWriter->WriteRecord1(1, from);
			pWriter->WriteRecord1(2, to);
		}
		void AnimClr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	clrSpc = pReader->GetUChar();
				else if (1 == _at)	dir = pReader->GetUChar();
				else if (2 == _at)	byR = pReader->GetLong();
				else if (3 == _at)	byG = pReader->GetLong();
				else if (4 == _at)	byB = pReader->GetLong();
				else if (5 == _at)	byH = pReader->GetLong();
				else if (6 == _at)	byS = pReader->GetLong();
				else if (7 == _at)	byL = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						cBhvr.fromPPTY(pReader);
					}break;
					case 1:
					{
						from.fromPPTY(pReader);
					}break;
					case 2:
					{
						to.fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void AnimClr::FillParentPointersForChilds()
		{
			cBhvr.SetParentPointer(this);
			from.SetParentPointer(this);
			to.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
