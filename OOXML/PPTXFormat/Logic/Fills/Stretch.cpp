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

#include "Stretch.h"

namespace PPTX
{
	namespace Logic
	{
		Stretch& Stretch::operator=(const Stretch& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			fillRect = oSrc.fillRect;
			return *this;
		}
		OOX::EElementType Stretch::getType() const
		{
			return OOX::et_a_stretch;
		}
		void Stretch::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				if (_T("fillRect") == XmlUtils::GetNameNoNS(oReader.GetName()))
					fillRect = oReader;
			}
		}
		void Stretch::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t count = oNodes.size();
				for (size_t i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					if (_T("fillRect") == XmlUtils::GetNameNoNS(oNode.GetName()))
						fillRect = oNode;
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring Stretch::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(fillRect);

			return XmlUtils::CreateNode(_T("a:stretch"), oValue);
		}
		void Stretch::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring strName;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				strName = L"w14:stretch";
			else
				strName = L"a:stretch";

			pWriter->StartNode(strName);
			pWriter->EndAttributes();

			pWriter->Write(fillRect);

			pWriter->EndNode(strName);
		}
		void Stretch::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE rec = pReader->GetUChar();

				switch (rec)
				{
					case 0:
					{
						fillRect = new PPTX::Logic::Rect();
						fillRect->m_name = L"a:fillRect";
						fillRect->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}
				}
			}
			pReader->Seek(_end_rec);
		}
		void Stretch::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, fillRect);
		}
		void Stretch::FillParentPointersForChilds()
		{
			if(fillRect.IsInit())
				fillRect->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
