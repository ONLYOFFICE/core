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

#include "AlphaRepl.h"

namespace PPTX
{
	namespace Logic
	{		
		AlphaRepl& AlphaRepl::operator=(const AlphaRepl& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			a = oSrc.a;
			return *this;
		}
		OOX::EElementType AlphaRepl::getType() const
		{
			return OOX::et_a_alphaRepl;
		}
		void AlphaRepl::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void AlphaRepl::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("a"), a)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void AlphaRepl::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"a", a);
			Normalize();
		}
		std::wstring AlphaRepl::toXML() const
		{
			if (!a.IsInit())
				return _T("<a:alphaRepl/>");

			std::wstring str = L"<a:alphaRepl a=\"" + std::to_wstring(*a) + L"\" />";
			return str;
		}
		void AlphaRepl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:alphaRepl");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"a", a);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:alphaRepl");
		}
		void AlphaRepl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_ALPHAREPL);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, a);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void AlphaRepl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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

				if (_at == 0)
					a = pReader->GetLong();
				else break;
			}
			pReader->Seek(_e);
		}
		void AlphaRepl::FillParentPointersForChilds(){}
		void AlphaRepl::Normalize()
		{
			a.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
