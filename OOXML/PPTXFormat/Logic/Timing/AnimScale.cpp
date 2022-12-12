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
