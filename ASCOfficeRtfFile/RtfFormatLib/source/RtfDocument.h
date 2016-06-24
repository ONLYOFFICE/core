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
#include "Basic.h"

#include "RtfGlobalTables.h"
#include "RtfSection.h"
#include "RtfMath.h"

class RtfDocument :public ItemContainer<RtfSectionPtr>
{
public: 
	RtfDocumentProperty		m_oProperty;
	RtfFontTable			m_oFontTable;
	RtfColorTable			m_oColorTable;
	RtfStyleTable			m_oStyleTable;
	RtfInformation			m_oInformation;

	RtfListTable			m_oListTabel;
	RtfListOverrideTable	m_oListOverrideTabel;

	RtfParagraphProperty	m_oDefaultParagraphProp;
	RtfCharProperty			m_oDefaultCharProp;
	RtfMath					m_oMathProp;

	TextItemContainerPtr	m_oFootnoteSep;
	TextItemContainerPtr	m_oFootnoteCon;
	TextItemContainerPtr	m_oEndnoteSep;
	TextItemContainerPtr	m_oEndnoteCon;

//для того чтобы конвертировать старый формат List в Numbering
	std::vector<RtfOldListPtr> m_aOldLists;

	struct _status_section
	{
		bool	start_new;
		int		number;
	}m_oStatusSection;

private: 
	std::vector<int> m_aShapeId;
public: 
	IdGenerator m_oIdGenerator;
	void SetShapeId( int nShapeId )
	{
		for( int i = 0; i < (int)m_aShapeId.size(); i++ )
			if( nShapeId == m_aShapeId[i] )
				return;
		m_aShapeId.push_back( nShapeId );
	}
	int GetShapeId(  int& nShapeId  )
	{
		if( PROP_DEF != nShapeId )
			return nShapeId;
		int nNewShapeId;
		while( true )
		{
			bool bUnique = true;
			nNewShapeId = m_oIdGenerator.Generate_ShapeId();
			for( int i = 0; i < (int)m_aShapeId.size(); i++ )
				if( nNewShapeId == m_aShapeId[i] )
				{
					bUnique = false;
					break;
				}
			if( true == bUnique )
			{
				nShapeId = nNewShapeId;
				m_aShapeId.push_back( nNewShapeId );
				break;
			}
		}
		return nShapeId;
	}
	RtfDocument();
	int GetType();

	CString RenderToRtf(RenderParameter oRenderParameter);
	CString RenderToOOX(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfDocument> RtfDocumentPtr;