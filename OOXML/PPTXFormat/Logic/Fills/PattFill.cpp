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

#include "PattFill.h"

namespace PPTX
{
	namespace Logic
	{
		PattFill::PattFill(std::wstring ns)
		{
			m_namespace = ns;
		}
		PattFill& PattFill::operator=(const PattFill& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			prst = oSrc.prst;

			fgClr = oSrc.fgClr;
			bgClr = oSrc.bgClr;

			return *this;
		}
		void PattFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if (sName == L"a:fgClr")
					fgClr.fromXMLParent(oReader);
				if (sName == L"a:bgClr")
					bgClr.fromXMLParent(oReader);
			}
			FillParentPointersForChilds();
		}
		OOX::EElementType PattFill::getType () const
		{
			return OOX::et_a_pattFill;
		}
		void PattFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("prst"), prst )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void PattFill::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"prst", prst);

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:fgClr"), oNode))
				fgClr.GetColorFrom(oNode);
			if (node.GetNode(_T("a:bgClr"), oNode))
				bgClr.GetColorFrom(oNode);

			FillParentPointersForChilds();
		}
		std::wstring PattFill::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("prst"), prst);

			XmlUtils::CNodeValue oValue;
			if (fgClr.is_init())
			{
				oValue.Write(_T("a:fgClr"), fgClr);
			}
			if (bgClr.is_init())
			{
				oValue.Write(_T("a:bgClr"), bgClr);
			}

			std::wstring strName = (_T("") == m_namespace) ? _T("pattFill") : (m_namespace + _T(":pattFill"));
			return XmlUtils::CreateNode(strName, oAttr, oValue);
		}
		void PattFill::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring strName;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				strName = L"w14:pattFill";
			else
				strName = m_namespace.empty() ? L"pattFill" : (m_namespace + L":pattFill");

			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("prst"), prst);
			pWriter->EndAttributes();

			if (fgClr.is_init())
			{
				pWriter->StartNode(_T("a:fgClr"));
				pWriter->EndAttributes();
				fgClr.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:fgClr"));
			}

			if (bgClr.is_init())
			{
				pWriter->StartNode(_T("a:bgClr"));
				pWriter->EndAttributes();
				bgClr.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:bgClr"));
			}

			pWriter->EndNode(strName);
		}
		void PattFill::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(FILL_TYPE_PATT);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, prst);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, fgClr);
			pWriter->WriteRecord1(1, bgClr);

			pWriter->EndRecord();
		}
		void PattFill::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar(); // FILL_TYPE_PATT
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
				case 0:
					prst = pReader->GetUChar();
					break;
				default:
					break;
				}
			}

			while (pReader->GetPos() < _e)
			{
				BYTE rec = pReader->GetUChar();

				switch (rec)
				{
					case 0:
					{
						fgClr.fromPPTY(pReader);					
					}break;
					case 1:
					{
						bgClr.fromPPTY(pReader);					
					}break;
					default:
					{
						pReader->SkipRecord();
					}
				}
			}
			pReader->Seek(_e);
		}
		void PattFill::FillParentPointersForChilds()
		{
			fgClr.SetParentPointer(this);
			bgClr.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
