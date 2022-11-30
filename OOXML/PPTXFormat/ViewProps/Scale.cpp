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

#include "Scale.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void Scale::fromXML(XmlUtils::CXmlNode& node)
		{
			sx = node.ReadNodeNoNS(L"sx");
			sy = node.ReadNodeNoNS(L"sy");

			FillParentPointersForChilds();
		}
		std::wstring Scale::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(sx);
			oValue.Write(sy);

			return XmlUtils::CreateNode(L"p:scale", oValue);
		}
		void Scale::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, sx);
			pWriter->WriteRecord1(1, sy);
		}
		void Scale::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						sx.name = L"sx";
						sx.fromPPTY(pReader);
					}break;
					case 1:
					{
						sy.name = L"sy";
						sy.fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Scale::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:scale");
			pWriter->EndAttributes();

			pWriter->StartNode(L"a:sx");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"n", sx.n);
			pWriter->WriteAttribute(L"d", sx.d);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:sx");

			pWriter->StartNode(L"a:sy");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"n", sy.n);
			pWriter->WriteAttribute(L"d", sy.d);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:sy");

			pWriter->EndNode(L"p:scale");
		}
		void Scale::FillParentPointersForChilds()
		{
			sx.SetParentPointer(this);
			sy.SetParentPointer(this);
		}
	} // namespace nsViewProps
} // namespace PPTX
