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

#include "NvGraphicFramePr.h"

namespace PPTX
{
	namespace Logic
	{
		NvGraphicFramePr::NvGraphicFramePr(std::wstring ns) : cNvPr(ns), cNvGraphicFramePr(ns), nvPr(ns)
		{
			m_namespace = ns;
		}
		NvGraphicFramePr& NvGraphicFramePr::operator=(const NvGraphicFramePr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			cNvPr				= oSrc.cNvPr;
			cNvGraphicFramePr	= oSrc.cNvGraphicFramePr;
			nvPr				= oSrc.nvPr;

			m_namespace		= oSrc.m_namespace;

			return *this;
		}
		void NvGraphicFramePr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace			= XmlUtils::GetNamespace(node.GetName());

			cNvPr				= node.ReadNodeNoNS(_T("cNvPr"));
			cNvGraphicFramePr	= node.ReadNodeNoNS(_T("cNvGraphicFramePr"));
			nvPr				= node.ReadNodeNoNS(_T("nvPr"));

			FillParentPointersForChilds();
		}
		void NvGraphicFramePr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (sName == L"cNvPr")
					cNvPr.fromXML( oReader);
				else if (sName == L"cNvGraphicFramePr")
					cNvGraphicFramePr.fromXML( oReader);
				else if (sName == L"nvPr")
					nvPr.fromXML( oReader);
			}
			FillParentPointersForChilds();
		}
		std::wstring NvGraphicFramePr::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(cNvPr);
			oValue.Write(cNvGraphicFramePr);
			oValue.Write(nvPr);

			return XmlUtils::CreateNode(m_namespace + L":nvGraphicFramePr", oValue);
		}
		void NvGraphicFramePr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if ((pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				 pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY) && pWriter->m_lGroupIndex >= 0)
			{
				cNvPr.toXmlWriter2(_T("wpg"), pWriter);
				pWriter->WriteString(_T("<wpg:cNvFrPr/>"));
				return;
			}

			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX && pWriter->m_lGroupIndex >= 0)
			{
				pWriter->StartNode(_T("xdr:nvGraphicFramePr"));
				pWriter->EndAttributes();

				cNvPr.toXmlWriter(pWriter);
				cNvGraphicFramePr.toXmlWriter(pWriter);

				pWriter->EndNode(_T("xdr:nvGraphicFramePr"));
				return;
			}
			std::wstring namespace_ = m_namespace;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)				namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":nvGraphicFramePr");
			pWriter->EndAttributes();

			cNvPr.toXmlWriter(pWriter);
			cNvGraphicFramePr.toXmlWriter(pWriter);
			nvPr.toXmlWriter(pWriter);

			pWriter->EndNode(namespace_ + L":nvGraphicFramePr");
		}
		void NvGraphicFramePr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, cNvPr);
			pWriter->WriteRecord1(1, cNvGraphicFramePr);
			pWriter->WriteRecord1(2, nvPr);
		}
		void NvGraphicFramePr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						cNvGraphicFramePr.fromPPTY(pReader);
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
		void NvGraphicFramePr::FillParentPointersForChilds()
		{
			cNvPr.SetParentPointer(this);
			cNvGraphicFramePr.SetParentPointer(this);
			nvPr.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
