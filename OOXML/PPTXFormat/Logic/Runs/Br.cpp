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

#include "Br.h"

namespace PPTX
{
	namespace Logic
	{
		Br& Br::operator=(const Br& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			rPr = oSrc.rPr;
			return *this;
		}
		OOX::EElementType Br::getType () const
		{
			return OOX::et_a_br;
		}
		void Br::fromXML(XmlUtils::CXmlNode& node)
		{
			rPr = node.ReadNode(_T("a:rPr"));
			FillParentPointersForChilds();
		}
		void Br::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if ( _T("a:rPr") == sName )
					rPr = oReader;
			}
		}
		void Br::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:br"));
			pWriter->EndAttributes();

			pWriter->Write(rPr);

			pWriter->EndNode(_T("a:br"));
		}
		void Br::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(PARRUN_TYPE_BR);

			pWriter->WriteRecord2(0, rPr);

			pWriter->EndRecord();
		}
		std::wstring Br::GetText() const
		{
			return _T("\n");
		}
		void Br::FillParentPointersForChilds()
		{
			if(rPr.IsInit())
				rPr->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
