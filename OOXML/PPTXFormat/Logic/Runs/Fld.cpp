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

#include "Fld.h"

namespace PPTX
{
	namespace Logic
	{
		Fld& Fld::operator=(const Fld& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			id		= oSrc.id;
			type	= oSrc.type;
			rPr		= oSrc.rPr;
			pPr		= oSrc.pPr;
			text	= oSrc.text;

			return *this;
		}
		OOX::EElementType Fld::getType () const
		{
			return OOX::et_a_fld;
		}
		void Fld::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (_T("rPr") == strName)
				{
					if (!rPr.IsInit())
						rPr = oReader;
				}
				else if (_T("pPr") == strName)
				{
					if (!pPr.IsInit())
						pPr = oReader;
				}
				else if (_T("t") == strName)
				{
					if (!text.IsInit())
						text = oReader.GetText2();
				}
			}
			FillParentPointersForChilds();
		}
		void Fld::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("id"), id)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("type"), type )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Fld::fromXML(XmlUtils::CXmlNode& node)
		{
			id = node.GetAttribute(_T("id"));
			XmlMacroReadAttributeBase(node, L"type", type);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int count = oNodes.GetCount();
				for (int i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("rPr") == strName)
					{
						if (!rPr.IsInit())
							rPr = oNode;
					}
					else if (_T("pPr") == strName)
					{
						if (!pPr.IsInit())
							pPr = oNode;
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
		void Fld::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:fld"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("id"), id);
			pWriter->WriteAttribute(_T("type"), type);
			pWriter->EndAttributes();

			pWriter->Write(rPr);
			pWriter->Write(pPr);

			if (text.IsInit())
			{
				pWriter->WriteString(_T("<a:t>"));
					pWriter->WriteStringXML(*text);
				pWriter->WriteString(_T("</a:t>"));
			}

			pWriter->EndNode(_T("a:fld"));
		}
		void Fld::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(PARRUN_TYPE_FLD);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, id);
			pWriter->WriteString2(1, type);
			pWriter->WriteString2(2, text);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, rPr);
			pWriter->WriteRecord2(1, pPr);

			pWriter->EndRecord();


			if (pWriter->m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
				pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(text);

		}
		void Fld::SetText(const std::wstring& src)
		{
			text = src;
		}
		std::wstring Fld::GetText() const
		{
			return text.get_value_or(_T(""));
		}
		void Fld::FillParentPointersForChilds()
		{
			if(rPr.IsInit())
				rPr->SetParentPointer(this);
			if(pPr.IsInit())
				pPr->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
