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
