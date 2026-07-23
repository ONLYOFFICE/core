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

#include "../../Format/RtfDocument.h"

#include "OOXCoreReader.h"

OOXCoreReader::OOXCoreReader(OOX::CCore * ooxCore)
{
	m_ooxCore = ooxCore;
}
bool OOXCoreReader::Parse( ReaderParameter oParam )
{
	if (m_ooxCore == NULL) return false;

	if (m_ooxCore->m_sTitle.IsInit())
	{
		oParam.oRtf->m_oInformation.m_sTitle = m_ooxCore->m_sTitle.get();
	}
	if (m_ooxCore->m_sSubject.IsInit())
	{
		oParam.oRtf->m_oInformation.m_sSubject = m_ooxCore->m_sSubject.get();
	}
	if (m_ooxCore->m_sCreator.IsInit())
	{
		oParam.oRtf->m_oInformation.m_sAuthor = m_ooxCore->m_sCreator.get();
	}
	if (m_ooxCore->m_sDescription.IsInit())
	{
		oParam.oRtf->m_oInformation.m_sComment = m_ooxCore->m_sDescription.get();
	}
	if (m_ooxCore->m_sKeywords.IsInit())
	{
		oParam.oRtf->m_oInformation.m_sKeywords = m_ooxCore->m_sKeywords.get();
	}
	return true;
}
