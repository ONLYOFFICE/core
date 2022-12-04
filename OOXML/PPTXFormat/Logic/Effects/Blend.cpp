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

#include "Blend.h"

namespace PPTX
{
	namespace Logic
	{
		Blend& Blend::operator=(const Blend& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			cont  = oSrc.cont;
			blend = oSrc.blend;
			return *this;
		}
		OOX::EElementType Blend::getType() const
		{
			return OOX::et_a_blend;
		}
		void Blend::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:cont")
					cont = oReader;
			}
			FillParentPointersForChilds();
		}
		void Blend::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("blend"), blend)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Blend::fromXML(XmlUtils::CXmlNode& node)
		{
			cont	= node.ReadNode(_T("a:cont"));
			blend	= node.GetAttribute(_T("blend"));
			FillParentPointersForChilds();
		}
		std::wstring Blend::toXML() const
		{
			return _T("<a:blend blend=\"") + blend.get() + _T("\">") + cont.toXML() + _T("</a:blend>");
		}
		void Blend::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:blend");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"blend", blend.get());
			pWriter->EndAttributes();

			cont.toXmlWriter(pWriter);

			pWriter->EndNode(L"a:blend");
		}
		void Blend::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_BLEND);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit1(0, blend);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, cont);

			pWriter->EndRecord();
		}
		void Blend::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar();
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (_at == 0)
					blend.SetBYTECode(pReader->GetUChar());
				else break;
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						cont.m_name = L"a:cont";
						cont.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void Blend::FillParentPointersForChilds()
		{
			cont.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
