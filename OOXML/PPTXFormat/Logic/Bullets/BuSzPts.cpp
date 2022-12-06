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

#include "BuSzPts.h"

namespace PPTX
{
	namespace Logic
	{
		BuSzPts& BuSzPts::operator=(const BuSzPts& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			val = oSrc.val;
			return *this;
		}
		OOX::EElementType BuSzPts::getType() const
		{
			return OOX::et_a_buSzPts;
		}
		void BuSzPts::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void BuSzPts::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, L"val", val)
			WritingElement_ReadAttributes_End( oReader )
		}
		void BuSzPts::fromXML(XmlUtils::CXmlNode& node)
		{
			val = node.ReadAttributeInt(L"val");
		}
		void BuSzPts::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:buSzPts");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"val", val);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:buSzPts");
		}
		void BuSzPts::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(BULLET_TYPE_SIZE_PTS);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt1(0, val.get_value_or(0));
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void BuSzPts::FillParentPointersForChilds(){}
		void BuSzPts::Normalize()
		{
			if (val.IsInit())
			{
				int tmp = *val;
				normalize_value(tmp, 100, 400000);
				val = tmp;
			}
		}
	} // namespace Logic
} // namespace PPTX
