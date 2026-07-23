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

#include "Run.h"

namespace PPTX
{
	namespace Logic
	{
		Run& Run::operator=(const Run& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			rPr = oSrc.rPr;
			text = oSrc.text;

			return *this;
		}
		OOX::EElementType Run::getType () const
		{
			return OOX::et_a_r;
		}
		void Run::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"a:rPr" == sName )
				{
					rPr =  oReader ;
				}
				else if ( L"a:t" == sName )
				{
					if (!text.IsInit())
						text = oReader.GetText2();
				}

			}
		}
		void Run::fromXML(XmlUtils::CXmlNode& node)
		{
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
						rPr = oNode;
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
		void Run::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:r"));
			pWriter->EndAttributes();

			if (rPr.is_init())
				rPr->m_name = _T("a:rPr");
			pWriter->Write(rPr);

			if (text.is_init())
			{
				pWriter->WriteString(_T("<a:t>"));
				pWriter->WriteStringXML(*text);
				pWriter->WriteString(_T("</a:t>"));
			}

			pWriter->EndNode(_T("a:r"));
		}
		void Run::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(PARRUN_TYPE_RUN);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, text);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, rPr);

			pWriter->EndRecord();


			if (pWriter->m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
				pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(text);
		}
		std::wstring Run::GetText() const
		{
			return text.get_value_or(_T(""));
		}
		bool Run::HasText() const
		{
			return text.IsInit();
		}
		void Run::SetText(const std::wstring& srcText)
		{
			text = srcText;
		}
		void Run::FillParentPointersForChilds()
		{
			if(rPr.IsInit())
				rPr->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
