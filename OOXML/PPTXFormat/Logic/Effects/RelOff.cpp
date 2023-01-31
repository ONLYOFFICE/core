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

#include "RelOff.h"

namespace PPTX
{
	namespace Logic
	{
		RelOff& RelOff::operator=(const RelOff& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			tx = oSrc.tx;
			ty = oSrc.ty;
			return *this;
		}
		OOX::EElementType RelOff::getType() const
		{
			return OOX::et_a_relOff;
		}
		void RelOff::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void RelOff::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("tx"), tx)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("ty"), ty)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void RelOff::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"tx", tx);
			XmlMacroReadAttributeBase(node, L"ty", ty);
		}
		std::wstring RelOff::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("tx"), tx);
			oAttr.Write(_T("ty"), ty);

			return XmlUtils::CreateNode(_T("a:relOff"), oAttr);
		}
		void RelOff::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:relOff");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"tx", tx);
			pWriter->WriteAttribute(L"ty", ty);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:relOff");
		}
		void RelOff::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_RELOFF);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, tx);
			pWriter->WriteInt2(1, ty);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void RelOff::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					case 0:	tx	= pReader->GetLong(); break;
					case 1:	ty	= pReader->GetLong(); break;
				}

			}
			pReader->Seek(_end_rec);
		}
		void RelOff::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
