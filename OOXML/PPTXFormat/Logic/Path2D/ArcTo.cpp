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

#include "ArcTo.h"

namespace PPTX
{
	namespace Logic
	{
		ArcTo& ArcTo::operator=(const ArcTo& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			wR		= oSrc.wR;
			hR		= oSrc.hR;
			stAng	= oSrc.stAng;
			swAng	= oSrc.swAng;

			return *this;
		}
		OOX::EElementType ArcTo::getType() const
		{
			return OOX::et_a_arcTo;
		}
		void ArcTo::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
		}
		void ArcTo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("wR"), wR )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("hR"), hR)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("stAng"), stAng)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("swAng"), swAng)
			WritingElement_ReadAttributes_End( oReader )
		}
		void ArcTo::fromXML(XmlUtils::CXmlNode& node)
		{
			wR		= node.GetAttribute(_T("wR"));
			hR		= node.GetAttribute(_T("hR"));
			stAng	= node.GetAttribute(_T("stAng"));
			swAng	= node.GetAttribute(_T("swAng"));
		}
		void ArcTo::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:arcTo"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("wR"), wR);
			pWriter->WriteAttribute(_T("hR"), hR);
			pWriter->WriteAttribute(_T("stAng"), stAng);
			pWriter->WriteAttribute(_T("swAng"), swAng);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("a:arcTo"));
		}
		void ArcTo::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(GEOMETRY_TYPE_PATH_ARCTO);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, wR);
			pWriter->WriteString1(1, hR);
			pWriter->WriteString1(2, stAng);
			pWriter->WriteString1(3, swAng);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		void ArcTo::FillParentPointersForChilds(){}
		std::wstring ArcTo::GetODString()const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("wR"), wR);
			oAttr.Write(_T("hR"), hR);
			oAttr.Write(_T("stAng"), stAng);
			oAttr.Write(_T("swAng"), swAng);

			return XmlUtils::CreateNode(_T("arcTo"), oAttr);
		}
	} // namespace Logic
} // namespace PPTX
