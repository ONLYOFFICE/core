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

#include "Bevel.h"

namespace PPTX
{
	namespace Logic
	{
		Bevel::Bevel(const std::wstring name)
		{
			m_name = name;
		}
		Bevel::~Bevel() {}
		Bevel::Bevel(const Bevel& oSrc) { *this = oSrc; }
		OOX::EElementType Bevel::getType() const
		{
			return OOX::et_a_bevel;
		}
		void Bevel::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name	= XmlUtils::GetNameNoNS(oReader.GetName());

			ReadAttributes( oReader );
		}
		void Bevel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("w"), w)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("h"), h)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("prst"), prst)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Bevel::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name	= XmlUtils::GetNameNoNS(node.GetName());

			XmlMacroReadAttributeBase(node, L"w", w);
			XmlMacroReadAttributeBase(node, L"h", h);
			XmlMacroReadAttributeBase(node, L"prst", prst);
		}
		std::wstring Bevel::toXML() const
		{
			std::wstring namespace_ = XmlUtils::GetNamespace(m_name);
			if (namespace_ == L"w14")
			{
				namespace_ + L"w14:";
			}
			else namespace_.clear();

			XmlUtils::CAttribute oAttr;
			oAttr.Write(namespace_ + L"w", w);
			oAttr.Write(namespace_ + L"h", h);
			oAttr.WriteLimitNullable(namespace_ + L"prst", prst);

			return XmlUtils::CreateNode(m_name, oAttr);
		}
		void Bevel::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = _T("w14:");
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = _T("a:");


			pWriter->StartNode(sNodeNamespace + m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"w", w);
			pWriter->WriteAttribute(sAttrNamespace + L"h", h);
			if (prst.IsInit())
			{
				pWriter->WriteAttribute(sAttrNamespace + L"prst", prst->get());
			}
			pWriter->EndAttributes();

			pWriter->EndNode(sNodeNamespace + m_name);
		}
		void Bevel::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, w);
			pWriter->WriteInt2(1, h);
			pWriter->WriteLimit2(2, prst);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Bevel::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		w = pReader->GetLong();
				else if (1 == _at)	h = pReader->GetLong();
				else if (2 == _at)	prst = pReader->GetUChar();
				else
					break;
			}

			pReader->Seek(_end_rec);
		}
		void Bevel::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
