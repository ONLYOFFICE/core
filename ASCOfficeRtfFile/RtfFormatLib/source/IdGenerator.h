/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#ifdef _WIN32
	#include <atlbase.h>
	#include <atlstr.h>
#else
    #include "../../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

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
	IdGenerator()
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

	CString Generate_rId()
	{
		CString sResult;
		sResult.AppendFormat( _T("rId%d"), m_nrIdCount++ );
		return sResult;
	}

	CString Generate_FitTextId()
	{
		CString sResult;
		sResult.AppendFormat( _T("%d"), m_nFitTextIdCount++ );
		return sResult;
	}

	int Generate_ImageIndex()
	{
		return m_nImageIndex++;
	}
	int Generate_OleIndex()
	{
		return m_nOleIndex++;
	}
	int Generate_HeaderNumber()
	{
		return m_nHeaderNumber++;
	}
	int Generate_FooterNumber()
	{
		return m_nFooterNumber++;
	}

	int Generate_BookmarkNumber()
	{
		return m_nBookmarkNumber++;
	}
	int Generate_FootnoteNumber()
	{
		return m_nFootnoteNumber++;
	}

	int Generate_EndnoteNumber()
	{
		return m_nEndnoteNumber++;
	}

	int Generate_ImagePropId()
	{
		return m_nImageProp++;
	}

	int Generate_ShapeId()
	{
		return m_nShapeId++;
	}
	
	CString Generate_OleId()
	{
		CString sId;
		sId.AppendFormat( _T("%d"), m_nOleId++ );
		CString sResult = _T("_1330071130");
		sResult = sResult.Left( sResult.GetLength() - sId.GetLength() );
		sResult.Append( sId );
		return sResult;
	}

	int Generate_PnId()
	{
		return m_nPnListId++;
	}	
};

class OOXIdGenerator
{
private: 

	std::map<CString, long> m_mapId;
	int m_nCounter;

public: 

	OOXIdGenerator()
	{
		m_nCounter = 1;
	}
	int GetId( CString sKey )
	{
		std::map<CString, long>::iterator pPair = m_mapId.find( sKey );
		
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
};
