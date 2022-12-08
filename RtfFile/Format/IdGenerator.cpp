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
