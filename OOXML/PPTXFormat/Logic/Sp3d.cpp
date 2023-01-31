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

#include "Sp3d.h"

namespace PPTX
{
	namespace Logic
	{
		Sp3d::Sp3d() {}
		Sp3d& Sp3d::operator=(const Sp3d& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			contourW		= oSrc.contourW;
			extrusionH		= oSrc.extrusionH;
			prstMaterial	= oSrc.prstMaterial;
			z				= oSrc.z;

			bevelT			= oSrc.bevelT;
			bevelB			= oSrc.bevelB;
			extrusionClr	= oSrc.extrusionClr;
			contourClr		= oSrc.contourClr;

			return *this;
		}
		OOX::EElementType Sp3d::getType() const
		{
			return OOX::et_a_prstClr;
		}
		void Sp3d::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = XmlUtils::GetNameNoNS(oReader.GetName());
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("bevelT") == strName)
					bevelT = oReader;
				else if (_T("bevelB") == strName)
					bevelB = oReader;
				else if (_T("extrusionClr") == strName)
				{
					extrusionClr.GetColorFrom(oReader);
				}
				else if (_T("contourClr") == strName)
				{
					contourClr.GetColorFrom(oReader);
				}
			}
		}
		void Sp3d::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"contourW", contourW);
			XmlMacroReadAttributeBase(node, L"extrusionH", extrusionH);
			XmlMacroReadAttributeBase(node, L"prstMaterial", prstMaterial);
			XmlMacroReadAttributeBase(node, L"z", z);
			XmlMacroReadAttributeBase(node, L"macro", macro);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("bevelT") == strName)
						bevelT = oNode;
					else if (_T("bevelB") == strName)
						bevelB = oNode;
					else if (_T("extrusionClr") == strName)
					{
						extrusionClr.GetColorFrom(oNode);
					}
					else if (_T("contourClr") == strName)
					{
						contourClr.GetColorFrom(oNode);
					}
				}
			}

			FillParentPointersForChilds();
		}
		void Sp3d::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if		( oReader, _T("contourW"), contourW)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("extrusionH"), extrusionH)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("prstMaterial"), prstMaterial)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("z"), z)
				WritingElement_ReadAttributes_Read_else_if  (oReader, _T("macro"), macro)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		void Sp3d::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace, sNodeChildNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = L"w14:";
				sNodeChildNamespace = sAttrNamespace = sNodeNamespace;
			}
			else
			{
				sNodeNamespace = m_namespace + L":";
				sNodeChildNamespace = L"a:";
			}


			pWriter->StartNode(sNodeNamespace + m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"macro", macro);
			pWriter->WriteAttribute(sAttrNamespace + L"contourW", contourW);
			pWriter->WriteAttribute(sAttrNamespace + L"extrusionH", extrusionH);
			pWriter->WriteAttribute(sAttrNamespace + L"prstMaterial", prstMaterial);
			pWriter->WriteAttribute(sAttrNamespace + L"z", z);
			pWriter->EndAttributes();

			pWriter->Write(bevelT);
			pWriter->Write(bevelB);

			if (extrusionClr.is_init())
			{
				pWriter->StartNode(sNodeChildNamespace + L"extrusionClr");
				pWriter->EndAttributes();
				extrusionClr.toXmlWriter(pWriter);
				pWriter->EndNode(sNodeChildNamespace + L"extrusionClr");
			}
			if (contourClr.is_init())
			{
				pWriter->StartNode(sNodeChildNamespace + L"contourClr");
				pWriter->EndAttributes();
				contourClr.toXmlWriter(pWriter);
				pWriter->EndNode(sNodeChildNamespace + L"contourClr");
			}

			pWriter->EndNode(sNodeNamespace + m_name);
		}
		void Sp3d::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, contourW);
				pWriter->WriteInt2(1, extrusionH);
				pWriter->WriteLimit2(2, prstMaterial);
				pWriter->WriteInt2(3, z);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, bevelT);
			pWriter->WriteRecord2(1, bevelB);
			pWriter->WriteRecord1(2, extrusionClr);
			pWriter->WriteRecord1(3, contourClr);

			if (macro.IsInit())
			{
				pWriter->StartRecord(SPTREE_TYPE_MACRO);
				pWriter->WriteString1(0, *macro);
				pWriter->EndRecord();
			}
		}
		void Sp3d::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		contourW = pReader->GetLong();
				else if (1 == _at)	extrusionH = pReader->GetLong();
				else if (2 == _at)	prstMaterial = pReader->GetUChar();
				else if (3 == _at)	z = pReader->GetLong();
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
						bevelT = new Logic::Bevel(L"bevelT");
						bevelT->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						bevelB = new Logic::Bevel(L"bevelB");
						bevelB->fromPPTY(pReader);
						break;
					}
					case 2:
					{
						extrusionClr.fromPPTY(pReader);
						break;
					}
					case 3:
					{
						contourClr.fromPPTY(pReader);
						break;
					}
					case SPTREE_TYPE_MACRO:
					{
						pReader->Skip(5); // type + size
						macro = pReader->GetString2();
					}break;
					default:
						break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Sp3d::FillParentPointersForChilds()
		{
			if(bevelT.IsInit())
				bevelT->SetParentPointer(this);
			if(bevelB.IsInit())
				bevelB->SetParentPointer(this);
			extrusionClr.SetParentPointer(this);
			contourClr.SetParentPointer(this);
		}
		void Sp3d::Normalize()
		{
			contourW.normalize_positive();
			extrusionH.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX

