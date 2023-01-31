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

#include "OuterShdw.h"

namespace PPTX
{
	namespace Logic
	{
		OuterShdw& OuterShdw::operator=(const OuterShdw& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Color			= oSrc.Color;

			algn			= oSrc.algn;
			blurRad			= oSrc.blurRad;
			dir				= oSrc.dir;
			dist			= oSrc.dist;
			kx				= oSrc.kx;
			ky				= oSrc.ky;
			rotWithShape	= oSrc.rotWithShape;
			sx				= oSrc.sx;
			sy				= oSrc.sy;

			return *this;
		}
		OOX::EElementType OuterShdw::getType() const
		{
			return OOX::et_a_innerShdw;
		}
		void OuterShdw::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = XmlUtils::GetNameNoNS(oReader.GetName());

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
		void OuterShdw::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("blurRad"), blurRad)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dir"), dir)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dist"), dist)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("algn"), algn)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("kx"), kx)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("ky"), ky)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rotWithShape"), rotWithShape)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("sx"), sx)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("sy"), sy)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void OuterShdw::fromXML(XmlUtils::CXmlNode& node)
		{
			Color.GetColorFrom(node);
			XmlMacroReadAttributeBase(node, L"algn", algn);
			XmlMacroReadAttributeBase(node, L"blurRad", blurRad);
			XmlMacroReadAttributeBase(node, L"dir", dir);
			XmlMacroReadAttributeBase(node, L"dist", dist);
			XmlMacroReadAttributeBase(node, L"kx", kx);
			XmlMacroReadAttributeBase(node, L"ky", ky);
			XmlMacroReadAttributeBase(node, L"rotWithShape", rotWithShape);
			XmlMacroReadAttributeBase(node, L"sx", sx);
			XmlMacroReadAttributeBase(node, L"sy", sy);

			Normalize();

			FillParentPointersForChilds();
		}
		std::wstring OuterShdw::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("blurRad"), blurRad);
			oAttr.Write(_T("dist"), dist);
			oAttr.Write(_T("dir"), dir);
			oAttr.Write(_T("sx"), sx);
			oAttr.Write(_T("sy"), sy);
			oAttr.Write(_T("kx"), kx);
			oAttr.Write(_T("ky"), ky);
			oAttr.WriteLimitNullable(_T("algn"), algn);
			oAttr.Write(_T("rotWithShape"), rotWithShape);

			XmlUtils::CNodeValue oValue;
			oValue.Write(Color);

			return XmlUtils::CreateNode(_T("a:outerShdw"), oAttr, oValue);
		}
		void OuterShdw::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = L"w14:";
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = L"a:";

			pWriter->StartNode(sNodeNamespace + m_name);
			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"blurRad", blurRad);
			pWriter->WriteAttribute(sAttrNamespace + L"dist", dist);
			pWriter->WriteAttribute(sAttrNamespace + L"dir", dir);
			pWriter->WriteAttribute(sAttrNamespace + L"sx", sx);
			pWriter->WriteAttribute(sAttrNamespace + L"sy", sy);
			pWriter->WriteAttribute(sAttrNamespace + L"kx", kx);
			pWriter->WriteAttribute(sAttrNamespace + L"ky", ky);
			pWriter->WriteAttribute(sAttrNamespace + L"algn", algn);
			pWriter->WriteAttribute(sAttrNamespace + L"rotWithShape", rotWithShape);
			pWriter->EndAttributes();

			Color.toXmlWriter(pWriter);

			pWriter->EndNode(sNodeNamespace + m_name);
		}
		void OuterShdw::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_OUTERSHDW);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, algn);
			pWriter->WriteInt2(1, blurRad);
			pWriter->WriteInt2(2, dir);
			pWriter->WriteInt2(3, dist);
			pWriter->WriteInt2(4, kx);
			pWriter->WriteInt2(5, ky);
			pWriter->WriteInt2(6, sx);
			pWriter->WriteInt2(7, sy);
			pWriter->WriteBool2(8, rotWithShape);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, Color);

			pWriter->EndRecord();
		}
		void OuterShdw::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					case 0:	algn	= pReader->GetUChar(); break;
					case 1:	blurRad = pReader->GetLong(); break;
					case 2:	dir		= pReader->GetLong(); break;
					case 3:	dist	= pReader->GetLong(); break;
					case 4:	kx		= pReader->GetLong(); break;
					case 5:	ky		= pReader->GetLong(); break;
					case 6:	sx		= pReader->GetLong(); break;
					case 7:	sy		= pReader->GetLong(); break;
					case 8:	rotWithShape = pReader->GetBool(); break;
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
		void OuterShdw::FillParentPointersForChilds()
		{
			Color.SetParentPointer(this);
		}
		void OuterShdw::Normalize()
		{
			blurRad.normalize_positive();
			dist.normalize_positive();

			dir.normalize(0, 21600000);
			kx.normalize(-5400000, 5400000);
			ky.normalize(-5400000, 5400000);
		}
	} // namespace Logic
} // namespace PPTX
