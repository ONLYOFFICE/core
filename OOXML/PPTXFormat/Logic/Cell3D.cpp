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

#include "Cell3D.h"

namespace PPTX
{
	namespace Logic
	{
		Cell3D::Cell3D()
		{
		}
		void Cell3D::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("bevel") == strName)
					bevel = oReader;
				else if (_T("lightRig") == strName)
					lightRig = oReader;
			}
			FillParentPointersForChilds();
		}
		void Cell3D::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("prstMaterial"), prstMaterial )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Cell3D::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"prstMaterial", prstMaterial);

			bevel		= node.ReadNodeNoNS(_T("bevel"));
			lightRig	= node.ReadNodeNoNS(_T("lightRig"));

			FillParentPointersForChilds();
		}
		std::wstring Cell3D::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("prstMaterial"), prstMaterial);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(bevel);
			oValue.WriteNullable(lightRig);

			return XmlUtils::CreateNode(_T("a:cell3D"), oAttr, oValue);
		}
		void Cell3D::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			// TODO:
		}
		void Cell3D::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			// TODO:
		}
		void Cell3D::FillParentPointersForChilds()
		{
			if(bevel.IsInit())
				bevel->SetParentPointer(this);
			if(lightRig.IsInit())
				lightRig->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX
