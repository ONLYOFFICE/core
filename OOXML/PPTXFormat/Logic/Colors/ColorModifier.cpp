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

#include "ColorModifier.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType ColorModifier::getType() const
		{
			return OOX::et_a_prstClr;
		}
		void ColorModifier::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			name = oReader.GetName();

			if (XmlUtils::GetNameNoNS(name) == _T("alpha"))
			{
				ReadAttributes2( oReader );
			}
			else
			{
				ReadAttributes( oReader );
			}
		}
		void ColorModifier::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
		{
			nullable_string sTmp;
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("val"), sTmp)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			if (sTmp.is_init())
			{
				val = sTmp.get();
				if (val.is_init() && std::wstring::npos != sTmp->find(L"%"))
				{
					*val = (*val) * 1000;
				}
			}
		}
		void ColorModifier::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("val"), val)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void ColorModifier::fromXML(XmlUtils::CXmlNode& node)
		{
			name = node.GetName();
			XmlMacroReadAttributeBase(node, L"val", val);

			if (XmlUtils::GetNameNoNS(name) == _T("alpha"))
			{
				nullable_string sTmp;
				XmlMacroReadAttributeBase(node, L"val", sTmp);

				if (val.is_init() && sTmp.is_init() && std::wstring::npos != sTmp->find(L"%"))
					*val = (*val) * 1000;
			}
		}
		std::wstring ColorModifier::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("val"), val);
			return XmlUtils::CreateNode(name, oAttr);
		}
		ColorModifier& ColorModifier::operator=(const ColorModifier& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			name		= oSrc.name;
			val			= oSrc.val;
			return *this;
		}
		void ColorModifier::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, name);
			pWriter->WriteInt2(1, val);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void ColorModifier::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					name = pReader->GetString2();
				else if (1 == _at)
					val = pReader->GetLong();
				else
					break;
			}
			pReader->Seek(_end_rec);
		}
		void ColorModifier::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring _name;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				_name = L"w14:" + XmlUtils::GetNameNoNS(name);
				sAttrNamespace = L"w14:";
			}
			else
				_name = name;
			pWriter->StartNode(_name);
			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"val", val);
			pWriter->EndAttributes();
			pWriter->EndNode(_name);
		}
		void ColorModifier::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
