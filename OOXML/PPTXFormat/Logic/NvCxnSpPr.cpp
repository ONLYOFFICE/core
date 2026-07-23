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

#include "NvCxnSpPr.h"

namespace PPTX
{
	namespace Logic
	{		
		NvCxnSpPr::NvCxnSpPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		NvCxnSpPr& NvCxnSpPr::operator=(const NvCxnSpPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			cNvPr		= oSrc.cNvPr;
			cNvCxnSpPr	= oSrc.cNvCxnSpPr;
			nvPr		= oSrc.nvPr;

			return *this;
		}
		void NvCxnSpPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"cNvPr")
					cNvPr = oReader;
				else if(strName == L"cNvCxnSpPr")
					cNvCxnSpPr = oReader;
				else if(strName == L"nvPr")
					nvPr = oReader;
			}
		}
		void NvCxnSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			cNvPr		= node.ReadNodeNoNS(_T("cNvPr"));
			cNvCxnSpPr	= node.ReadNodeNoNS(_T("cNvCxnSpPr"));
			nvPr		= node.ReadNodeNoNS(_T("nvPr"));

			FillParentPointersForChilds();
		}
		std::wstring NvCxnSpPr::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(cNvPr);
			oValue.Write(cNvCxnSpPr);

			if (m_namespace != L"xdr" && m_namespace != L"cdr")
			{
				oValue.Write(nvPr);
			}

			return XmlUtils::CreateNode(m_namespace + L":nvCxnSpPr", oValue);
		}
		void NvCxnSpPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
					 pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	namespace_ = L"wps";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";

			pWriter->StartNode(namespace_ + L":nvCxnSpPr");

			pWriter->EndAttributes();

			cNvPr.toXmlWriter2(namespace_ , pWriter);
			cNvCxnSpPr.toXmlWriter(pWriter);

			if (pWriter->m_lDocType != XMLWRITER_DOC_TYPE_GRAPHICS &&
                pWriter->m_lDocType != XMLWRITER_DOC_TYPE_CHART_DRAWING && pWriter->m_lDocType != XMLWRITER_DOC_TYPE_XLSX)
			{
				nvPr.toXmlWriter2(namespace_, pWriter);
			}

			pWriter->EndNode(namespace_ + L":nvCxnSpPr");
		}
		void NvCxnSpPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, cNvPr);
			pWriter->WriteRecord1(1, cNvCxnSpPr);
			pWriter->WriteRecord1(2, nvPr);
		}
		void NvCxnSpPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						cNvPr.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						cNvCxnSpPr.fromPPTY(pReader);
						break;
					}
					case 2:
					{
						nvPr.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void NvCxnSpPr::FillParentPointersForChilds()
		{
			cNvPr.SetParentPointer(this);
			cNvCxnSpPr.SetParentPointer(this);
			nvPr.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
