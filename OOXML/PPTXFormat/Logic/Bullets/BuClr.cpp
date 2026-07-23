/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "BuClr.h"

namespace PPTX
{
	namespace Logic
	{
		void BuClr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				Color.fromXML(oReader);
			}
		}
		OOX::EElementType BuClr::getType () const
		{
			return OOX::et_a_buClr;
		}
		void BuClr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
			WritingElement_ReadAttributes_End	( oReader )
		}
		BuClr& BuClr::operator=(const BuClr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Color = oSrc.Color;
			return *this;
		}
		DWORD BuClr::GetRGBA()const{return Color.GetRGBA();}
		DWORD BuClr::GetARGB()const{return Color.GetARGB();}
		DWORD BuClr::GetBGRA()const{return Color.GetBGRA();}
		DWORD BuClr::GetABGR()const{return Color.GetABGR();}
		void BuClr::fromXML(XmlUtils::CXmlNode& node)
		{
			Color.GetColorFrom(node);
		}
		void BuClr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:buClr");
			pWriter->EndAttributes();
			Color.toXmlWriter(pWriter);
			pWriter->EndNode(L"a:buClr");
		}
		void BuClr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(BULLET_TYPE_COLOR_CLR);
			pWriter->WriteRecord1(0, Color);
			pWriter->EndRecord();
		}
		void BuClr::FillParentPointersForChilds()
		{
			Color.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
