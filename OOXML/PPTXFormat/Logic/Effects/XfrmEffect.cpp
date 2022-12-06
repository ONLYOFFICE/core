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

#include "XfrmEffect.h"

namespace PPTX
{
	namespace Logic
	{
		XfrmEffect& XfrmEffect::operator=(const XfrmEffect& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			kx = oSrc.kx;
			ky = oSrc.ky;
			sx = oSrc.sx;
			sy = oSrc.sy;
			tx = oSrc.tx;
			ty = oSrc.ty;

			return *this;
		}
		OOX::EElementType XfrmEffect::getType() const
		{
			return OOX::et_a_xfrm;
		}
		void XfrmEffect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void XfrmEffect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("kx"), kx)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("ky"), ky)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("sx"), sx)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("sy"), sy)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("tx"), tx)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("ty"), ty)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void XfrmEffect::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"kx", kx);
			XmlMacroReadAttributeBase(node, L"ky", ky);
			XmlMacroReadAttributeBase(node, L"sx", sx);
			XmlMacroReadAttributeBase(node, L"sy", sy);
			XmlMacroReadAttributeBase(node, L"tx", tx);
			XmlMacroReadAttributeBase(node, L"ty", ty);

			Normalize();
		}
		std::wstring XfrmEffect::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("sx"), sx);
			oAttr.Write(_T("sy"), sy);
			oAttr.Write(_T("kx"), kx);
			oAttr.Write(_T("ky"), ky);
			oAttr.Write(_T("tx"), tx);
			oAttr.Write(_T("ty"), ty);

			return XmlUtils::CreateNode(_T("a:xfrm"), oAttr);
		}
		void XfrmEffect::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:xfrm");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"kx", kx);
			pWriter->WriteAttribute(L"ky", ky);
			pWriter->WriteAttribute(L"sx", sx);
			pWriter->WriteAttribute(L"sy", sy);
			pWriter->WriteAttribute(L"tx", tx);
			pWriter->WriteAttribute(L"ty", ty);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:xfrm");
		}
		void XfrmEffect::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_XFRM);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, kx);
			pWriter->WriteInt2(1, ky);
			pWriter->WriteInt2(2, sx);
			pWriter->WriteInt2(3, sy);
			pWriter->WriteSize_t2(4, tx);
			pWriter->WriteSize_t2(5, tx);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void XfrmEffect::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					case 0:	kx	= pReader->GetLong(); break;
					case 1: ky	= pReader->GetLong(); break;
					case 2: sx	= pReader->GetLong(); break;
					case 3: sy	= pReader->GetLong(); break;
					case 4: tx	= (size_t)pReader->GetULong(); break;
					case 5: ty	= (size_t)pReader->GetULong(); break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void XfrmEffect::FillParentPointersForChilds(){}
		void XfrmEffect::Normalize()
		{
			kx.normalize(-5400000, 5400000);
			ky.normalize(-5400000, 5400000);
		}
	} // namespace Logic
} // namespace PPTX
