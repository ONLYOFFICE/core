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

#include "GrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{
		GrpSpPr::GrpSpPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		GrpSpPr& GrpSpPr::operator=(const GrpSpPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			xfrm		= oSrc.xfrm;
			Fill		= oSrc.Fill;
			EffectList	= oSrc.EffectList;
			scene3d		= oSrc.scene3d;
			bwMode		= oSrc.bwMode;
			return *this;
		}
		OOX::EElementType GrpSpPr::getType () const
		{
			return OOX::et_p_groupSpPr;
		}
		void GrpSpPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"xfrm" == sName)
					xfrm = oReader;
				else if ( L"blipFill"	== sName	||
						  L"gradFill"	== sName	||
						  L"grpFill"	== sName	||
						  L"noFill"		== sName	||
						  L"pattFill"	== sName	||
						  L"solidFill"	== sName )
				{
					Fill.fromXML(oReader);
				}
				else if ( L"effectDag"	== sName	||
						  L"effectLst"	== sName)
				{
					EffectList.fromXML(oReader);
				}
				else if ( L"extLst"		== sName )
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						Ext element;
						element.fromXML(oReader);
						extLst.push_back (element);
					}
				}
			}
			FillParentPointersForChilds();
		}
		void GrpSpPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"bwMode", bwMode )
			WritingElement_ReadAttributes_End( oReader )
		}
		void GrpSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"bwMode", bwMode);

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				size_t count = oNodes.size();
				for (size_t i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());
					if (L"xfrm" == strName)
					{
						if (!xfrm.IsInit())
							xfrm = oNode;
					}
					else if (L"blipFill" == strName ||
						L"gradFill" == strName ||
						L"grpFill" == strName ||
						L"noFill" == strName ||
						L"pattFill" == strName ||
						L"solidFill" == strName)
					{
						Fill.fromXML(oNode);
					}
					else if (L"scene3d" == strName)
					{
						if (!scene3d.IsInit())
							scene3d = oNode;
					}
					else if (L"effectDag" == strName ||
							L"effectLst" == strName)
					{
						EffectList.fromXML(oNode);
					}
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring GrpSpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(L"bwMode", bwMode);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(xfrm);
			oValue.Write(Fill);
			oValue.Write(EffectList);
			oValue.WriteNullable(scene3d);

			return XmlUtils::CreateNode(m_namespace + L":grpSpPr", oAttr, oValue);
		}
		void GrpSpPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring namespace_ = m_namespace;
			if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
					 pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)	namespace_ = L"wpg";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			namespace_ = L"xdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

			pWriter->StartNode(namespace_ + L":grpSpPr");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"bwMode", bwMode);
			pWriter->EndAttributes();

			pWriter->Write(xfrm);
			Fill.toXmlWriter(pWriter);
			EffectList.toXmlWriter(pWriter);
			pWriter->Write(scene3d);

			pWriter->EndNode(namespace_ + L":grpSpPr");
		}
		void GrpSpPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, bwMode);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, xfrm);
			pWriter->WriteRecord1(1, Fill);
			pWriter->WriteRecord1(2, EffectList);
			pWriter->WriteRecord2(3, scene3d);
		}
		void GrpSpPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
				{
					bwMode = pReader->GetUChar();
				}
				else
					break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						if (xfrm.is_init() == false)
						{
							xfrm = new Xfrm();
						}
						xfrm->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						Fill.fromPPTY(pReader);
						break;
					}
					case 2:
					{
						EffectList.fromPPTY(pReader);
						break;
					}
					case 3:
					{
						scene3d = new PPTX::Logic::Scene3d();
						scene3d->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void GrpSpPr::FillParentPointersForChilds()
		{
			Fill.SetParentPointer(this);
			EffectList.SetParentPointer(this);

			if(xfrm.IsInit())		xfrm->SetParentPointer(this);
			if(scene3d.IsInit())	scene3d->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
