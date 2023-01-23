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

#include "AlphaBiLevel.h"

namespace PPTX
{
	namespace Logic
	{
		AlphaBiLevel& AlphaBiLevel::operator=(const AlphaBiLevel& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			thresh = oSrc.thresh;
			return *this;
		}
		OOX::EElementType AlphaBiLevel::getType() const
		{
			return OOX::et_a_alphaBiLevel;
		}
		void AlphaBiLevel::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void AlphaBiLevel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle     ( oReader, _T("thresh"), thresh)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			Normalize();
		}
		void AlphaBiLevel::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"thresh", thresh);
			Normalize();
		}
		std::wstring AlphaBiLevel::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("thresh"), thresh);

			return XmlUtils::CreateNode(_T("a:alphaBiLevel"), oAttr);
		}
		void AlphaBiLevel::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:alphaBiLevel");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"thresh", thresh);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:alphaBiLevel");
		}
		void AlphaBiLevel::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_ALPHABILEVEL);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, thresh);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void AlphaBiLevel::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					thresh = pReader->GetLong();
				else break;
			}
			pReader->Seek(_e);
		}
		void AlphaBiLevel::FillParentPointersForChilds(){}
		void AlphaBiLevel::Normalize()
		{
			thresh.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
