/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("bwMode"), bwMode )
			WritingElement_ReadAttributes_End( oReader )
		}
		void GrpSpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"bwMode", bwMode);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int count = oNodes.GetCount();
				for (int i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());
					if (_T("xfrm") == strName)
					{
						if (!xfrm.IsInit())
							xfrm = oNode;
					}
					else if (_T("scene3d") == strName)
					{
						if (!scene3d.IsInit())
							scene3d = oNode;
					}
				}
			}

			Fill.GetFillFrom(node);
			EffectList.GetEffectListFrom(node);

			FillParentPointersForChilds();
		}
		std::wstring GrpSpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("bwMode"), bwMode);

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
			pWriter->WriteAttribute(_T("bwMode"), bwMode);
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
