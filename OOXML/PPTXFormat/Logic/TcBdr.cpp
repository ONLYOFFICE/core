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

#include "TcBdr.h"

namespace PPTX
{
	namespace Logic
	{		
		void TcBdr::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t count = oNodes.size();

				for (size_t i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("left") == strName)
						left = oNode;
					else if (_T("right") == strName)
						right = oNode;
					else if (_T("top") == strName)
						top = oNode;
					else if (_T("bottom") == strName)
						bottom = oNode;
					else if (_T("insideH") == strName)
						insideH = oNode;
					else if (_T("insideV") == strName)
						insideV = oNode;
					else if (_T("tl2br") == strName)
						tl2br = oNode;
					else if (_T("tr2bl") == strName)
						tr2bl = oNode;
				}
			}

			FillParentPointersForChilds();
		}
		void TcBdr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tcBdr"));
			pWriter->EndAttributes();

			pWriter->Write(left);
			pWriter->Write(right);
			pWriter->Write(top);
			pWriter->Write(bottom);
			pWriter->Write(insideH);
			pWriter->Write(insideV);
			pWriter->Write(tl2br);
			pWriter->Write(tr2bl);

			pWriter->EndNode(_T("a:tcBdr"));
		}
		void TcBdr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, left);
			pWriter->WriteRecord2(1, right);
			pWriter->WriteRecord2(2, top);
			pWriter->WriteRecord2(3, bottom);
			pWriter->WriteRecord2(4, insideH);
			pWriter->WriteRecord2(5, insideV);
			pWriter->WriteRecord2(6, tl2br);
			pWriter->WriteRecord2(7, tr2bl);
		}
		void TcBdr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						left = new LineStyle();
						left->fromPPTY(pReader);
						left->m_name = _T("a:left");
						break;
					}
					case 1:
					{
						right = new LineStyle();
						right->fromPPTY(pReader);
						right->m_name = _T("a:right");
						break;
					}
					case 2:
					{
						top = new LineStyle();
						top->fromPPTY(pReader);
						top->m_name = _T("a:top");
						break;
					}
					case 3:
					{
						bottom = new LineStyle();
						bottom->fromPPTY(pReader);
						bottom->m_name = _T("a:bottom");
						break;
					}
					case 4:
					{
						insideH = new LineStyle();
						insideH->fromPPTY(pReader);
						insideH->m_name = _T("a:insideH");
						break;
					}
					case 5:
					{
						insideV = new LineStyle();
						insideV->fromPPTY(pReader);
						insideV->m_name = _T("a:insideV");
						break;
					}
					case 6:
					{
						tl2br = new LineStyle();
						tl2br->fromPPTY(pReader);
						tl2br->m_name = _T("a:tl2br");
						break;
					}
					case 7:
					{
						tr2bl = new LineStyle();
						tr2bl->fromPPTY(pReader);
						tr2bl->m_name = _T("a:tr2bl");
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TcBdr::FillParentPointersForChilds()
		{
			if(left.IsInit())
				left->SetParentPointer(this);
			if(right.IsInit())
				right->SetParentPointer(this);
			if(top.IsInit())
				top->SetParentPointer(this);
			if(bottom.IsInit())
				bottom->SetParentPointer(this);
			if(insideH.IsInit())
				insideH->SetParentPointer(this);
			if(insideV.IsInit())
				insideV->SetParentPointer(this);
			if(tl2br.IsInit())
				tl2br->SetParentPointer(this);
			if(tr2bl.IsInit())
				tr2bl->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
