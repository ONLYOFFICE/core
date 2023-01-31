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

#include "InnerShdw.h"

namespace PPTX
{
	namespace Logic
	{
		InnerShdw& InnerShdw::operator=(const InnerShdw& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Color	= oSrc.Color;
			blurRad	= oSrc.blurRad;
			dir		= oSrc.dir;
			dist	= oSrc.dist;
			return *this;
		}
		OOX::EElementType InnerShdw::getType() const
		{
			return OOX::et_a_innerShdw;
		}
		void InnerShdw::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();
				Color.fromXML(oReader);
			}

			FillParentPointersForChilds();
		}
		void InnerShdw::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("blurRad"), blurRad)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dir"), dir)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dist"), dist)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void InnerShdw::fromXML(XmlUtils::CXmlNode& node)
		{
			Color.GetColorFrom(node);
			XmlMacroReadAttributeBase(node, L"blurRad", blurRad);
			XmlMacroReadAttributeBase(node, L"dir", dir);
			XmlMacroReadAttributeBase(node, L"dist", dist);

			Normalize();
			FillParentPointersForChilds();
		}
		std::wstring InnerShdw::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("blurRad"), blurRad);
			oAttr.Write(_T("dist"), dist);
			oAttr.Write(_T("dir"), dir);

			return XmlUtils::CreateNode(_T("a:innerShdw"), oAttr, Color.toXML());
		}
		void InnerShdw::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:innerShdw");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"blurRad", blurRad);
			pWriter->WriteAttribute(L"dist", dist);
			pWriter->WriteAttribute(L"dir", dir);
			pWriter->EndAttributes();

			Color.toXmlWriter(pWriter);

			pWriter->EndNode(L"a:innerShdw");
		}
		void InnerShdw::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_INNERSHDW);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, dir);
			pWriter->WriteInt2(1, dist);
			pWriter->WriteInt2(2, blurRad);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, Color);

			pWriter->EndRecord();
		}
		void InnerShdw::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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

				switch (_at)
				{
					case 0:	dir = pReader->GetLong(); break;
					case 1:	dist = pReader->GetLong(); break;
					case 2:	blurRad = pReader->GetLong(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						Color.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void InnerShdw::FillParentPointersForChilds()
		{
			Color.SetParentPointer(this);
		}
		void InnerShdw::Normalize()
		{
			blurRad.normalize_positive();
			dist.normalize_positive();

			dir.normalize(0, 21600000);
		}
	} // namespace Logic
} // namespace PPTX

