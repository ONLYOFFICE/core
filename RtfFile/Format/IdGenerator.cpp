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

#include "IdGenerator.h"

IdGenerator::IdGenerator()
{
	m_nrIdCount = 1;
	m_nFitTextIdCount = 1;
	m_nImageIndex = 1;
	m_nOleIndex = 1;
	m_nHeaderNumber = 1;
	m_nFooterNumber = 1;
	m_nBookmarkNumber = 1;
	m_nFootnoteNumber = 2;
	m_nEndnoteNumber = 2;
	m_nImageProp = 1;
	m_nShapeId = 1;
	m_nOleId = 1;
	m_nPnListId = 1;

}
std::wstring IdGenerator::Generate_rId()
{
	return L"rId" + std::to_wstring(m_nrIdCount++);
}
std::wstring IdGenerator::Generate_FitTextId()
{
	return std::to_wstring(m_nFitTextIdCount++);
}
int IdGenerator::Generate_ImageIndex()
{
	return m_nImageIndex++;
}
int IdGenerator::Generate_OleIndex()
{
	return m_nOleIndex++;
}
int IdGenerator::Generate_HeaderNumber()
{
	return m_nHeaderNumber++;
}
int IdGenerator::Generate_FooterNumber()
{
	return m_nFooterNumber++;
}
int IdGenerator::Generate_BookmarkNumber()
{
	return m_nBookmarkNumber++;
}
int IdGenerator::Generate_FootnoteNumber()
{
	return m_nFootnoteNumber++;
}
int IdGenerator::Generate_EndnoteNumber()
{
	return m_nEndnoteNumber++;
}
int IdGenerator::Generate_ImagePropId()
{
	return m_nImageProp++;
}
int IdGenerator::Generate_ShapeId()
{
	return m_nShapeId++;
}
std::wstring IdGenerator::Generate_OleId()
{
	std::wstring sId = std::to_wstring(m_nOleId++);
	std::wstring sResult = L"_1330071130";

	sResult = sResult.substr(0, sResult.length() - sId.length() );
	sResult += sId;
	return sResult;
}
int IdGenerator::Generate_PnId()
{
	return m_nPnListId++;
}

OOXIdGenerator::OOXIdGenerator()
{
	m_nCounter = 1;
}
int OOXIdGenerator::GetId( std::wstring sKey )
{
	std::map<std::wstring, long>::iterator pPair = m_mapId.find( sKey );

	if( m_mapId.end() == pPair )
	{
		int nResult = m_nCounter;
		m_mapId[sKey] = nResult;
		m_nCounter++;
		return nResult;
	}
	else
		return pPair->second;
}
