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


#include "CxnSp.h"
#include "SpTree.h"
#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"

namespace PPTX
{
	namespace Logic
	{
		CxnSp::CxnSp(std::wstring ns)
		{
			m_namespace = ns;
		}
		CxnSp::~CxnSp()
		{
		}
		CxnSp::CxnSp(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const CxnSp& CxnSp::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		CxnSp::CxnSp(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
		}
		const CxnSp& CxnSp::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
			return *this;
		}
		void CxnSp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;
					
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("nvCxnSpPr") == strName)
					nvCxnSpPr = oReader;
				else if (_T("spPr") == strName)
					spPr = oReader;
				else if (_T("style") == strName)
					style = oReader;
			}

			FillParentPointersForChilds();
		}

		void CxnSp::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"macro", macro);

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("nvCxnSpPr") == strName)
						nvCxnSpPr = oNode;
					else if (_T("spPr") == strName)
						spPr = oNode;
					else if (_T("style") == strName)
						style = oNode;
				}
			}

			FillParentPointersForChilds();
		}

		std::wstring CxnSp::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write2(L"macro", macro);

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvCxnSpPr);
			oValue.Write(spPr);
			oValue.WriteNullable(style);

			return XmlUtils::CreateNode(m_namespace + L":cxnSp", oAttr, oValue);
		}

		void CxnSp::FillParentPointersForChilds()
		{
			nvCxnSpPr.SetParentPointer(this);
			spPr.SetParentPointer(this);
			if(style.is_init())
				style->SetParentPointer(this);
		}
		
		void CxnSp::GetRect(Aggplus::RECT& pRect)const
		{
			pRect.bottom = 0;
			pRect.left = 0;
			pRect.right = 0;
			pRect.top = 0;

			if(spPr.xfrm.IsInit())
			{
				pRect.left		= spPr.xfrm->offX.get();
				pRect.top		= spPr.xfrm->offY.get();
				pRect.right		= pRect.left + spPr.xfrm->extX.get_value_or(0);
				pRect.bottom	= pRect.top + spPr.xfrm->extY.get_value_or(0);
			}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		DWORD CxnSp::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			line.SetParentFilePointer(parentFile);

			if(style.is_init())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);

				BGRA = style->lnRef.Color.GetBGRA();
			}

			if(spPr.ln.is_init())
				spPr.ln->Merge(line);
			return BGRA;
		}

		DWORD CxnSp::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			fill.SetParentFilePointer(parentFile);

			if (style.IsInit())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);

				BGRA = style->fillRef.Color.GetBGRA();
			}

			if(spPr.Fill.is_init())
				spPr.Fill.Merge(fill);
			return BGRA;
		}
		
		void CxnSp::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(SPTREE_TYPE_CXNSP);

			pWriter->WriteRecord1(0, nvCxnSpPr);
			pWriter->WriteRecord1(1, spPr);
			pWriter->WriteRecord2(2, style);

			if (macro.IsInit())
			{
				pWriter->StartRecord(SPTREE_TYPE_MACRO);
				pWriter->WriteString1(0, *macro);
				pWriter->EndRecord();
			}
			pWriter->EndRecord();
		}
		void CxnSp::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					nvCxnSpPr.fromPPTY(pReader);
				}break;
				case 1:
				{
					spPr.fromPPTY(pReader);
				}break;
				case 2:
				{
					style = new ShapeStyle(L"p");
					style->fromPPTY(pReader);
				}break;
				case SPTREE_TYPE_MACRO:
				{
					pReader->Skip(5); // type + size
					macro = pReader->GetString2();
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void CxnSp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, _T("macro"), macro)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CxnSp::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	namespace_ = L"wps";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":cxnSp");
			pWriter->WriteAttribute2(L"macro", macro);
			pWriter->EndAttributes();

			nvCxnSpPr.toXmlWriter(pWriter);
			spPr.toXmlWriter(pWriter);

			if (style.is_init())
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
					pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	style->m_namespace = L"wps";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			style->m_namespace = L"xdr";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		style->m_namespace = L"a";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	style->m_namespace = L"cdr";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			style->m_namespace = L"dgm";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		style->m_namespace = L"dsp";

				pWriter->Write(style);
			}
			pWriter->EndNode(namespace_ + L":cxnSp");
		}
	} // namespace Logic
} // namespace PPTX