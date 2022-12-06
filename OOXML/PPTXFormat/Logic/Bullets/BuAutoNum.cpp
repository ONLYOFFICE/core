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

#include "BuAutoNum.h"

namespace PPTX
{
	namespace Logic
	{
		BuAutoNum& BuAutoNum::operator=(const BuAutoNum& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			type	= oSrc.type;
			startAt = oSrc.startAt;

			return *this;
		}
		void BuAutoNum::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		OOX::EElementType BuAutoNum::getType() const
		{
			return OOX::et_a_buChar;
		}
		void BuAutoNum::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"type", type);
			XmlMacroReadAttributeBase(node, L"startAt", startAt);

			Normalize();
		}
		void BuAutoNum::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if	  ( oReader, L"startAt",	startAt)
				WritingElement_ReadAttributes_Read_else_if( oReader, L"type",		type)
			WritingElement_ReadAttributes_End	( oReader )

			Normalize();
		}
		void BuAutoNum::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:buAutoNum");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"type", type.get());
			pWriter->WriteAttribute(L"startAt", startAt);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:buAutoNum");
		}
		void BuAutoNum::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(BULLET_TYPE_BULLET_AUTONUM);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit1(0, type);
			pWriter->WriteInt2(1, startAt);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void BuAutoNum::Normalize()
		{
			startAt.normalize(1, 32767);
		}
		void BuAutoNum::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
