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

#include "OOXAppReader.h"

OOXAppReader::OOXAppReader(OOX::CApp *ooxApp)
{
	m_ooxApp = ooxApp;
}
bool OOXAppReader::Parse( ReaderParameter oParam )
{
	if (m_ooxApp == NULL) return false;

	if(m_ooxApp->m_nTotalTime.IsInit())
	{
		oParam.oRtf->m_oInformation.m_nEndingTime = m_ooxApp->m_nTotalTime.get();
	}
	if(m_ooxApp->m_nPages.IsInit())
	{
		oParam.oRtf->m_oInformation.m_nNumberOfPages = m_ooxApp->m_nPages.get();
	}
	if(m_ooxApp->m_nWords.IsInit())
	{
		oParam.oRtf->m_oInformation.m_nNumberOfWords = m_ooxApp->m_nWords.get();
	}
	if(m_ooxApp->m_nCharacters.IsInit())
	{
		oParam.oRtf->m_oInformation.m_nNumberOfCharactersWithoutSpace = m_ooxApp->m_nCharacters.get();
	}
	if(m_ooxApp->m_nCharactersWithSpaces.IsInit())
	{
		oParam.oRtf->m_oInformation.m_nNumberOfCharactersWithSpace = m_ooxApp->m_nCharactersWithSpaces.get();
	}
	if (m_ooxApp->m_sApplication.IsInit())
	{
		oParam.oRtf->m_oInformation.m_sApplication = m_ooxApp->m_sApplication.get();
	}
	return true;
}
