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

#include "Fld.h"

namespace PPTX
{
	namespace Logic
	{
		Fld& Fld::operator=(const Fld& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			id		= oSrc.id;
			type	= oSrc.type;
			rPr		= oSrc.rPr;
			pPr		= oSrc.pPr;
			text	= oSrc.text;

			return *this;
		}
		OOX::EElementType Fld::getType () const
		{
			return OOX::et_a_fld;
		}
		void Fld::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (_T("rPr") == strName)
				{
					if (!rPr.IsInit())
						rPr = oReader;
				}
				else if (_T("pPr") == strName)
				{
					if (!pPr.IsInit())
						pPr = oReader;
				}
				else if (_T("t") == strName)
				{
					if (!text.IsInit())
						text = oReader.GetText2();
				}
			}
			FillParentPointersForChilds();
		}
		void Fld::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("id"), id)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("type"), type )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Fld::fromXML(XmlUtils::CXmlNode& node)
		{
			id = node.GetAttribute(_T("id"));
			XmlMacroReadAttributeBase(node, L"type", type);

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t count = oNodes.size();
				for (size_t i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("rPr") == strName)
					{
						if (!rPr.IsInit())
							rPr = oNode;
					}
					else if (_T("pPr") == strName)
					{
						if (!pPr.IsInit())
							pPr = oNode;
					}
					else if (_T("t") == strName)
					{
						if (!text.IsInit())
							text = oNode.GetTextExt();
					}
				}
			}
			FillParentPointersForChilds();
		}
		void Fld::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:fld"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("id"), id);
			pWriter->WriteAttribute(_T("type"), type);
			pWriter->EndAttributes();

			pWriter->Write(rPr);
			pWriter->Write(pPr);

			if (text.IsInit())
			{
				pWriter->WriteString(_T("<a:t>"));
					pWriter->WriteStringXML(*text);
				pWriter->WriteString(_T("</a:t>"));
			}

			pWriter->EndNode(_T("a:fld"));
		}
		void Fld::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(PARRUN_TYPE_FLD);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, id);
			pWriter->WriteString2(1, type);
			pWriter->WriteString2(2, text);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, rPr);
			pWriter->WriteRecord2(1, pPr);

			pWriter->EndRecord();


			if (pWriter->m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
				pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(text);

		}
		void Fld::SetText(const std::wstring& src)
		{
			text = src;
		}
		std::wstring Fld::GetText() const
		{
			return text.get_value_or(_T(""));
		}
		void Fld::FillParentPointersForChilds()
		{
			if(rPr.IsInit())
				rPr->SetParentPointer(this);
			if(pPr.IsInit())
				pPr->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
