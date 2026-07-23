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

#include "ClrMapOvr.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType ClrMapOvr::getType() const
		{
			return OOX::et_a_clrMapOvr;
		}
		void ClrMapOvr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"overrideClrMapping" == sName)
				{
					overrideClrMapping = oReader;
					break;
				}
			}
			if (overrideClrMapping.is_init())
				overrideClrMapping->m_name = _T("a:overrideClrMapping");

			FillParentPointersForChilds();
		}
		void ClrMapOvr::fromXML(XmlUtils::CXmlNode& node)
		{
			overrideClrMapping = node.ReadNodeNoNS(_T("overrideClrMapping"));
			if (overrideClrMapping.is_init())
				overrideClrMapping->m_name = _T("a:overrideClrMapping");
			FillParentPointersForChilds();
		}
		std::wstring ClrMapOvr::toXML() const
		{
			if (overrideClrMapping.IsInit())
				return _T("<p:clrMapOvr>") + overrideClrMapping->toXML() + _T("</p:clrMapOvr>");
			return _T("<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>");
		}
		void ClrMapOvr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, overrideClrMapping);
		}
		void ClrMapOvr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (overrideClrMapping.is_init())
			{
				pWriter->WriteString(_T("<p:clrMapOvr>"));
				overrideClrMapping->m_name = _T("a:overrideClrMapping");
				overrideClrMapping->toXmlWriter(pWriter);
				pWriter->WriteString(_T("</p:clrMapOvr>"));
			}
			else
			{
				pWriter->WriteString(_T("<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
			}
		}
		void ClrMapOvr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			if (pReader->GetPos() < _e)
			{
				pReader->Skip(1); // "0"-rectype
				overrideClrMapping = new Logic::ClrMap();
				overrideClrMapping->fromPPTY(pReader);
			}

			pReader->Seek(_e);
		}
		void ClrMapOvr::FillParentPointersForChilds()
		{
			if(overrideClrMapping.IsInit())
				overrideClrMapping->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
