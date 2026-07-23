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


#include "Bg.h"
#include "../Slide.h"
#include "../SlideMaster.h"
#include "../SlideLayout.h"

namespace PPTX
{
	namespace Logic
	{
		Bg::Bg()
		{
		}
		Bg::~Bg()
		{
		}
		Bg::Bg(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const Bg& Bg::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		void Bg::fromXML(XmlUtils::CXmlNode& node)
		{
            XmlMacroReadAttributeBase(node, L"bwMode", bwMode);
			bgPr	= node.ReadNodeNoNS(_T("bgPr"));
			bgRef	= node.ReadNodeNoNS(_T("bgRef"));

			FillParentPointersForChilds();
		}
		std::wstring Bg::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("bwMode"), bwMode);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(bgPr);
			oValue.WriteNullable(bgRef);

			return XmlUtils::CreateNode(_T("p:bg"), oAttr, oValue);
		}
		void Bg::FillParentPointersForChilds()
		{
			//if(background != 0)
			//	background->SetParentPointer(this);
			if(bgPr.IsInit())
				bgPr->SetParentPointer(this);
			if(bgRef.IsInit())
				bgRef->SetParentPointer(this);
		}
		void Bg::GetBackground(Logic::BgPr& bg, DWORD& ARGB)const
		{
			if(bgPr.IsInit())
				bg = bgPr.get();
			else// if(cSld->bg->bgRef.is_init())
			{
				ARGB = bgRef->Color.GetARGB();
				if(parentFileIs<Slide>())
					parentFileAs<Slide>().theme->themeElements.fmtScheme.GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
				else if(parentFileIs<SlideLayout>())
					parentFileAs<SlideLayout>().theme->themeElements.fmtScheme.GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
				else if(parentFileIs<SlideMaster>())
					parentFileAs<SlideMaster>().theme->themeElements.fmtScheme.GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
			}
		}
		void Bg::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:bg"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("bwMode"), bwMode);
			pWriter->EndAttributes();

			pWriter->Write(bgPr);
			pWriter->Write(bgRef);

			pWriter->EndNode(_T("p:bg"));
		}
		void Bg::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, bwMode);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, bgPr);
			pWriter->WriteRecord2(1, bgRef);
		}
		void Bg::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
				{
					bwMode = pReader->GetUChar();
				}
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
						bgPr = new BgPr();
						bgPr->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						bgRef = new StyleRef();
						bgRef->m_name = _T("p:bgRef");
						bgRef->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
	} // namespace Logic
} // namespace PPTX
