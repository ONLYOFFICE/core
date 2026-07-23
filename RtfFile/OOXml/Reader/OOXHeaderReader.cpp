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
#include "OOXHeaderReader.h"
#include "OOXTextItemReader.h"
#include "OOXRelsReader.h"

OOXHeaderReader::OOXHeaderReader(ComplexTypes::Word::CHdrFtrRef* ref)
{
	m_ref = ref;
}
bool OOXHeaderReader::Parse( ReaderParameter oParam, std::wstring sRID, TextItemContainerPtr& oOutput, bool bHeader )
{
	if (m_ref == NULL) return false;
	if (m_ref->m_oId.IsInit() == false ) return false;

	OOX::CHdrFtr    *pHdrFtr = oParam.oDocx->GetHeaderOrFooter(m_ref->m_oId->GetValue());
	if (pHdrFtr == NULL) return false;	

	oParam.oReader->m_currentContainer = dynamic_cast<OOX::IFileContainer*>(pHdrFtr);
	
	OOXTextItemReader oOOXTextItemReader;
	oOutput = TextItemContainerPtr ( new TextItemContainer() );
	oOOXTextItemReader.m_oTextItems = oOutput;

    for (std::vector<OOX::WritingElement*>::iterator it = pHdrFtr->m_arrItems.begin(); it != pHdrFtr->m_arrItems.end(); ++it)
	{
		oOOXTextItemReader.Parse(*it, oParam );
	}
	
	oParam.oReader->m_currentContainer = dynamic_cast<OOX::IFileContainer*>(oParam.oDocx->m_oMain.document);
	
	return true;
}
