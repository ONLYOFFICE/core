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


#include "SpPr.h"
#include "PrstGeom.h"
#include "CustGeom.h"

namespace PPTX
{
	namespace Logic
	{
		SpPr::SpPr(std::wstring ns)
		{
			m_namespace = ns;
			Fill.m_type = UniFill::notInit;
		}
		SpPr::~SpPr()
		{
		}
		SpPr::SpPr(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		SpPr::SpPr(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
		}
		const SpPr& SpPr::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		const SpPr& SpPr::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
			return *this;
		}
		void SpPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
				else if ( L"ln" == sName)
					ln = oReader;
				else if ( L"scene3d" == sName)
					scene3d = oReader;
				else if ( L"sp3d" == sName)
					sp3d = oReader;
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
						  L"effectLst"	== sName )
				{
					EffectList.fromXML(oReader);		
				}
				else if ( L"prstGeom"	== sName	||
						  L"custGeom"	== sName)
				{
					Geometry.fromXML(oReader);		
				}
				else if ( L"extLst"		== sName)
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
		void SpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			Geometry.GetGeometryFrom(node);
			Fill.GetFillFrom(node);
			EffectList.GetEffectListFrom(node);

            XmlMacroReadAttributeBase(node,L"bwMode", bwMode);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("xfrm") == strName)
						xfrm = oNode;
					else if (_T("ln") == strName)
						ln = oNode;
					else if (_T("scene3d") == strName)
						scene3d = oNode;
					else if (_T("sp3d") == strName)
						sp3d = oNode;
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring SpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(L"bwMode", bwMode);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(xfrm);
			oValue.Write(Geometry);
			oValue.Write(Fill);
			oValue.WriteNullable(ln);
			oValue.Write(EffectList);
			oValue.WriteNullable(scene3d);
			oValue.WriteNullable(sp3d);

			return XmlUtils::CreateNode(m_namespace + L":spPr", oAttr, oValue);
		}
		void SpPr::Merge(SpPr& spPr)const
		{
			if(xfrm.IsInit())
				xfrm->Merge(spPr.xfrm);
			if(Geometry.is_init())
				spPr.Geometry = Geometry;
			if(Fill.is_init())
				spPr.Fill = Fill;//.fromXML(Fill.toXML());
			if(ln.IsInit())
				ln->Merge(spPr.ln);
		}
		void SpPr::FillParentPointersForChilds()
		{
			if(xfrm.IsInit())
				xfrm->SetParentPointer(this);
			Geometry.SetParentPointer(this);
			Fill.SetParentPointer(this);
			if(ln.IsInit())
				ln->SetParentPointer(this);
			EffectList.SetParentPointer(this);
			if(scene3d.IsInit())
				scene3d->SetParentPointer(this);
			if(sp3d.IsInit())
				sp3d->SetParentPointer(this);
		}
		void SpPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						xfrm = new Logic::Xfrm();
						xfrm->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						Geometry.fromPPTY(pReader);
						break;
					}
					case 2:
					{
						Fill.fromPPTY(pReader);
						break;
					}
					case 3:
					{
						ln = new Logic::Ln();
						ln->fromPPTY(pReader);
						break;
					}
					case 4:
					{
						EffectList.fromPPTY(pReader);
						break;
					}
					case 5:
					{
						scene3d = new Logic::Scene3d();
						scene3d->fromPPTY(pReader);
						break;
					}
					case 6:
					{
						sp3d = new Logic::Sp3d();
						sp3d->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		OOX::EElementType SpPr::getType () const
		{
			return OOX::et_a_spPr;
		}
		void SpPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("bwMode"), bwMode )
			WritingElement_ReadAttributes_End( oReader )
		}
		void SpPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring name_ = L"a:spPr";

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				if (0 == (pWriter->m_lFlag & 0x01))								name_ = L"wps:spPr";
				else															name_ = L"pic:spPr";
			}
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			name_ = L"xdr:spPr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	name_ = L"cdr:spPr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			name_ = L"dgm:spPr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		name_ = L"dsp:spPr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART)			name_ = L"c:spPr";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		name_ = L"a:spPr";
			else
			{//theme
				if (0 != (pWriter->m_lFlag & 0x04))								name_ = L"a:spPr";
				else															name_ = L"p:spPr";
			}
			pWriter->StartNode(name_);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("bwMode"), bwMode);
			pWriter->EndAttributes();

			pWriter->Write(xfrm);
			Geometry.toXmlWriter(pWriter);

			if ((pWriter->m_lFlag & 0x02) != 0 && !Fill.is_init())
			{
				pWriter->WriteString(_T("<a:grpFill/>"));
			}
			Fill.toXmlWriter(pWriter);

			pWriter->Write(ln);
			EffectList.toXmlWriter(pWriter);
			pWriter->Write(scene3d);
			pWriter->Write(sp3d);

			pWriter->EndNode(name_);
		}
		void SpPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, bwMode);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, xfrm);
			pWriter->WriteRecord1(1, Geometry);
			pWriter->WriteRecord1(2, Fill);
			pWriter->WriteRecord2(3, ln);
			pWriter->WriteRecord1(4, EffectList);
			pWriter->WriteRecord2(5, scene3d);
			pWriter->WriteRecord2(6, sp3d);
		}
	} // namespace Logic
} // namespace PPTX
