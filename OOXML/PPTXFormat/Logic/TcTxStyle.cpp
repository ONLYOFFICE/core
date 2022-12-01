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

#include "TcTxStyle.h"

namespace PPTX
{
	namespace Logic
	{		
		void TcTxStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"i", i);
			XmlMacroReadAttributeBase(node, L"b", b);

			fontRef = node.ReadNodeNoNS(_T("fontRef"));
			Color.GetColorFrom(node);

			FillParentPointersForChilds();
		}
		void TcTxStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tcTxStyle"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("i"), i);
			pWriter->WriteAttribute(_T("b"), b);
			pWriter->EndAttributes();

			pWriter->Write(fontRef);
			Color.toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:tcTxStyle"));
		}
		void TcTxStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, i);
			pWriter->WriteLimit2(1, b);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, fontRef);
			pWriter->WriteRecord1(1, Color);
		}
		void TcTxStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						i = pReader->GetUChar();
						break;
					}
					case 1:
					{
						b = pReader->GetUChar();
						break;
					}
					default:
						break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						fontRef = new Logic::FontRef();
						fontRef->fromPPTY(pReader);
						fontRef->m_name = _T("a:fontRef");
						break;
					}
					case 1:
					{
						Color.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TcTxStyle::FillParentPointersForChilds()
		{
			if(fontRef.IsInit())
				fontRef->SetParentPointer(this);
			Color.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
