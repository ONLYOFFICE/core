/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "NvPicPr.h"

namespace PPTX
{
	namespace Logic
	{
		NvPicPr::NvPicPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		NvPicPr& NvPicPr::operator=(const NvPicPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			cNvPr		= oSrc.cNvPr;
			cNvPicPr	= oSrc.cNvPicPr;
			nvPr		= oSrc.nvPr;

			m_namespace	= oSrc.m_namespace;

			return *this;
		}
		void NvPicPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("cNvPr") == strName)
					cNvPr.fromXML( oReader);
				else if (_T("cNvPicPr") == strName)
					cNvPicPr.fromXML( oReader);
				else if (_T("nvPr") == strName)
					nvPr.fromXML( oReader);
			}
		}
		void NvPicPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("cNvPr") == strName)
						cNvPr = oNode;
					else if (_T("cNvPicPr") == strName)
						cNvPicPr = oNode;
					else if (_T("nvPr") == strName)
						nvPr = oNode;
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring NvPicPr::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(cNvPr);
			oValue.Write(cNvPicPr);
			oValue.Write(nvPr);

			return XmlUtils::CreateNode(m_namespace + L":nvPicPr", oValue);
		}
		void NvPicPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)		namespace_ = L"pic";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":nvPicPr");

			pWriter->EndAttributes();

			cNvPr.toXmlWriter(pWriter);
			cNvPicPr.toXmlWriter(pWriter);

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_PPTX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
					nvPr.toXmlWriter(pWriter);

			pWriter->EndNode(namespace_ + L":nvPicPr");
		}
		void NvPicPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, cNvPr);
			pWriter->WriteRecord1(1, cNvPicPr);
			pWriter->WriteRecord1(2, nvPr);
		}
		void NvPicPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						cNvPicPr.fromPPTY(pReader);
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
		void NvPicPr::FillParentPointersForChilds()
		{
			cNvPr.SetParentPointer(this);
			cNvPicPr.SetParentPointer(this);
			nvPr.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
