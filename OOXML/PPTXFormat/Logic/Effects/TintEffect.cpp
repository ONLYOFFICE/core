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

#include "TintEffect.h"

namespace PPTX
{
	namespace Logic
	{
		TintEffect& TintEffect::operator=(const TintEffect& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			amt = oSrc.amt;
			hue = oSrc.hue;
			return *this;
		}
		OOX::EElementType TintEffect::getType() const
		{
			return OOX::et_a_tint;
		}
		void TintEffect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void TintEffect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("amt"), amt)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("hue"), hue)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void TintEffect::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"amt", amt);
			XmlMacroReadAttributeBase(node, L"hue", hue);

			Normalize();
		}
		std::wstring TintEffect::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("hue"), hue);
			oAttr.Write(_T("amt"), amt);

			return XmlUtils::CreateNode(_T("a:tint"), oAttr);
		}
		void TintEffect::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:tint");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"hue", hue);
			pWriter->WriteAttribute(L"amt", amt);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:tint");
		}
		void TintEffect::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_TINTEFFECT);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, amt);
			pWriter->WriteInt2(1, hue);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void TintEffect::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar();
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:	amt = pReader->GetLong(); break;
					case 1:	hue = pReader->GetLong(); break;
				}
			}
			pReader->Seek(_e);
		}
		void TintEffect::FillParentPointersForChilds(){}
		void TintEffect::Normalize()
		{
			hue.normalize(0, 21600000);
		}
	} // namespace Logic
} // namespace PPTX
