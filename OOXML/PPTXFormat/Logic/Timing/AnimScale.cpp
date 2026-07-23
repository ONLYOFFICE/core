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

#include "AnimScale.h"

namespace PPTX
{
	namespace Logic
	{
		void AnimScale::fromXML(XmlUtils::CXmlNode& node)
		{
			cBhvr = node.ReadNode(_T("p:cBhvr"));

			XmlUtils::CXmlNode oNodeBy;
			if (node.GetNode(_T("p:by"), oNodeBy))
			{
				XmlMacroReadAttributeBase(oNodeBy, L"x", byX);
				XmlMacroReadAttributeBase(oNodeBy, L"y", byY);
			}
			XmlUtils::CXmlNode oNodeFrom;
			if (node.GetNode(_T("p:from"), oNodeFrom))
			{
				XmlMacroReadAttributeBase(oNodeFrom, L"x", fromX);
				XmlMacroReadAttributeBase(oNodeFrom, L"y", fromY);
			}
			XmlUtils::CXmlNode oNodeTo;
			if (node.GetNode(_T("p:to"), oNodeTo))
			{
				XmlMacroReadAttributeBase(oNodeTo, L"x", toX);
				XmlMacroReadAttributeBase(oNodeTo, L"y", toY);
			}

			XmlMacroReadAttributeBase(node, L"zoomContents", zoomContents);

			FillParentPointersForChilds();
		}
		std::wstring AnimScale::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("zoomContents"), zoomContents);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cBhvr);

			if (byX.IsInit() || byY.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("x"), byX);
				oAttr1.Write(_T("y"), byY);

				oValue.m_strValue += XmlUtils::CreateNode(_T("p:by"), oAttr1);
			}
			if (fromX.IsInit() || fromY.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("x"), fromX);
				oAttr1.Write(_T("y"), fromY);

				oValue.m_strValue += XmlUtils::CreateNode(_T("p:from"), oAttr1);
			}
			if (toX.IsInit() || toY.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("x"), toX);
				oAttr1.Write(_T("y"), toY);

				oValue.m_strValue += XmlUtils::CreateNode(_T("p:to"), oAttr1);
			}

			return XmlUtils::CreateNode(_T("p:animScale"), oAttr, oValue);
		}
		OOX::EElementType AnimScale::getType() const
		{
			return OOX::et_p_animScale;
		}
		void AnimScale::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void AnimScale::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, byX);
				pWriter->WriteInt2(1, byY);
				pWriter->WriteInt2(2, fromX);
				pWriter->WriteInt2(3, fromY);
				pWriter->WriteInt2(4, toX);
				pWriter->WriteInt2(5, toY);
				pWriter->WriteBool2(6, zoomContents);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, cBhvr);
		}
		void AnimScale::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	byX = pReader->GetLong();
				else if (1 == _at)	byY = pReader->GetLong();
				else if (2 == _at)	fromX = pReader->GetLong();
				else if (3 == _at)	fromY = pReader->GetLong();
				else if (4 == _at)	toX = pReader->GetLong();
				else if (5 == _at)	toY = pReader->GetLong();
				else if (6 == _at)	zoomContents = pReader->GetBool();
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
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(end);
		}
		void AnimScale::FillParentPointersForChilds()
		{
			cBhvr.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
