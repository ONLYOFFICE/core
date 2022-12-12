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

#include "Tab.h"

namespace PPTX
{
	namespace Logic
	{		
		void Tab::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;
		}
		OOX::EElementType Tab::getType () const
		{
			return OOX::et_a_tab;
		}
		void Tab::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if	  ( oReader, _T("pos"),		pos)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("algn"),	algn)
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Tab::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"pos", pos);
			XmlMacroReadAttributeBase(node, L"algn", algn);

			FillParentPointersForChilds();
		}
		void Tab::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, algn);
			pWriter->WriteInt2(1, pos);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Tab::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tab"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("pos"), pos);
			pWriter->WriteAttribute(_T("algn"), algn);
			pWriter->EndAttributes();

			pWriter->EndNode(_T("a:tab"));
		}
		void Tab::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
