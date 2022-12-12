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

#include "Video.h"

namespace PPTX
{
	namespace Logic
	{
		void Video::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"fullScrn", fullScrn);
			cMediaNode	= node.ReadNode(_T("p:cMediaNode"));

			FillParentPointersForChilds();
		}
		std::wstring Video::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("fullScrn"), fullScrn);

			return XmlUtils::CreateNode(_T("p:video"), oAttr, cMediaNode.toXML());
		}
		OOX::EElementType Video::getType() const
		{
			return OOX::et_p_video;
		}
		void Video::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void Video::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, fullScrn);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, cMediaNode);
		}
		void Video::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)	fullScrn = pReader->GetBool();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						cMediaNode.fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void Video::FillParentPointersForChilds()
		{
			cMediaNode.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
