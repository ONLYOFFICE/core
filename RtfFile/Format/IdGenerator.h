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
#pragma once 

#include <map>
#include <boost/lexical_cast.hpp>

class IdGenerator
{
private: 
	int m_nOleId;
	int m_nPnListId;
	int m_nShapeId;
	int m_nImageProp;
	int m_nEndnoteNumber;
	int m_nrIdCount;
	int m_nFitTextIdCount;
	int m_nImageIndex;
	int m_nOleIndex;
	int m_nHeaderNumber;
	int m_nFootnoteNumber;
	int m_nFooterNumber;
	int m_nBookmarkNumber;

public: 
	IdGenerator();

	std::wstring Generate_rId();

	std::wstring Generate_FitTextId();

	int Generate_ImageIndex();
	int Generate_OleIndex();
	int Generate_HeaderNumber();
	int Generate_FooterNumber();
	int Generate_BookmarkNumber();
	int Generate_FootnoteNumber();
	int Generate_EndnoteNumber();
	int Generate_ImagePropId();
	int Generate_ShapeId();
	std::wstring Generate_OleId();
	int Generate_PnId();
};

class OOXIdGenerator
{
private: 

    std::map<std::wstring, long> m_mapId;
	int m_nCounter;

public: 
	OOXIdGenerator();

	int GetId( std::wstring sKey );
};
