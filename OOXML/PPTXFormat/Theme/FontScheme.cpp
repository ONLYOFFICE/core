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

#include "FontScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		void FontScheme::fromXML(XmlUtils::CXmlNode& node)
		{
			name = node.GetAttribute(_T("name"));

			majorFont = node.ReadNode(_T("a:majorFont"));
			minorFont = node.ReadNode(_T("a:minorFont"));

			FillWithDefaults();
			FillParentPointersForChilds();
		}
		std::wstring FontScheme::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("name"), name);

			XmlUtils::CNodeValue oValue;
			oValue.Write(majorFont);
			oValue.Write(minorFont);

			return XmlUtils::CreateNode(_T("a:fontScheme"), oAttr, oValue);
		}
		void FontScheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, name);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, majorFont);
			pWriter->WriteRecord1(1, minorFont);
		}
		void FontScheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:fontScheme"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(_T("name"), name);
			pWriter->EndAttributes();

			majorFont.toXmlWriter(pWriter);
			minorFont.toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:fontScheme"));
		}
		void FontScheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();

				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					name = pReader->GetString2();
				else
					break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						majorFont.m_name = _T("a:majorFont");
						majorFont.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						minorFont.m_name = _T("a:minorFont");
						minorFont.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void FontScheme::FillWithDefaults()
		{
			if (OOX::et_Unknown == majorFont.latin.m_eType)
			{
				majorFont.latin.m_name = _T("a:latin");
				majorFont.latin.typeface = L"Arial";
			}
			if (OOX::et_Unknown == minorFont.latin.m_eType)
			{
				minorFont.latin.m_name = _T("a:latin");
				minorFont.latin.typeface = L"Arial";
			}
			FillParentPointersForChilds();
		}
		void FontScheme::FillParentPointersForChilds()
		{
			majorFont.SetParentPointer(this);
			minorFont.SetParentPointer(this);
		}
	} // namespace nsTheme
} // namespace PPTX
