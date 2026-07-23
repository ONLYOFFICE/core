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

#include "FontCollection.h"

namespace PPTX
{
	namespace Logic
	{
		FontCollection& FontCollection::operator=(const FontCollection& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			latin	= oSrc.latin;
			ea		= oSrc.ea;
			cs		= oSrc.cs;

			for (size_t i=0; i < oSrc.Fonts.size(); i++)
				Fonts.push_back(oSrc.Fonts[i]);

			m_name	= oSrc.m_name;

			return *this;
		}
		void FontCollection::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("latin") == strName)
						latin = oNode;
					else if (_T("ea") == strName)
						ea = oNode;
					else if (_T("cs") == strName)
						cs = oNode;
					else if (_T("font") == strName)
					{
						SupplementalFont elem;
						elem = oNode;
						Fonts.push_back(elem);
					}
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring FontCollection::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(latin);
			oValue.Write(ea);
			oValue.Write(cs);
			oValue.WriteArray(Fonts);

			return XmlUtils::CreateNode(m_name, oValue);
		}
		void FontCollection::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);
			pWriter->EndAttributes();

			latin.toXmlWriter(pWriter);
			ea.toXmlWriter(pWriter);
			cs.toXmlWriter(pWriter);

			size_t nCount = Fonts.size();
			for (size_t i = 0; i < nCount; ++i)
				Fonts[i].toXmlWriter(pWriter);

			pWriter->EndNode(m_name);
		}
		void FontCollection::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, latin);
			pWriter->WriteRecord1(1, ea);
			pWriter->WriteRecord1(2, cs);
			pWriter->WriteRecordArray(3, 0, Fonts);
		}
		void FontCollection::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						latin.fromPPTY(pReader);
						latin.m_name = _T("a:latin");
						break;
					}
					case 1:
					{
						ea.fromPPTY(pReader);
						ea.m_name = _T("a:ea");
						break;
					}
					case 2:
					{
						cs.fromPPTY(pReader);
						cs.m_name = _T("a:cs");
						break;
					}
					case 3:
					{
						pReader->Skip(4);
						ULONG _c = pReader->GetULong();
						for (ULONG i = 0; i < _c; ++i)
						{
							pReader->Skip(1); // type

							SupplementalFont elm;
							Fonts.push_back(elm);

							Fonts[i].m_name = _T("a:font");
							Fonts[i].fromPPTY(pReader);
						}
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void FontCollection::FillParentPointersForChilds()
		{
			latin.SetParentPointer(this);
			ea.SetParentPointer(this);
			cs.SetParentPointer(this);

			size_t count = Fonts.size();
			for (size_t i = 0; i < count; ++i)
				Fonts[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
