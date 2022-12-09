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

#include "Basic.h"

#include "RtfGlobalTables.h"
#include "RtfSection.h"
#include "RtfMath.h"
#include "RtfShape.h"

namespace OOX
{
	class WritingElement;
}

namespace NSFonts
{
	class IApplicationFonts;
}

struct _section
{
	RtfSectionPtr props;
    size_t	start_para = 0;
	size_t	end_para = 0;
};

class RtfDocument : public ItemContainer<_section>
{
public: 
	RtfDocumentProperty		m_oProperty;
	RtfFontTable			m_oFontTable;
	RtfColorTable			m_oColorTable;
	RtfStyleTable			m_oStyleTable;
	RtfRevisionTable		m_oRevisionTable;
	RtfInformation			m_oInformation;
	RtfShapePtr				m_pBackground;

	std::wstring			m_sThemeXml;

	RtfListTable			m_oListTable;
	RtfListOverrideTable	m_oListOverrideTable;

	RtfParagraphProperty	m_oDefaultParagraphProp;
	RtfCharProperty			m_oDefaultCharProp;
	RtfMathPtr				m_pMathProp;

	TextItemContainerPtr	m_oFootnoteSep;
	TextItemContainerPtr	m_oFootnoteCon;
	TextItemContainerPtr	m_oEndnoteSep;
	TextItemContainerPtr	m_oEndnoteCon;

//для того чтобы конвертировать старый формат List в Numbering
	std::vector<RtfOldListPtr> m_aOldLists;
	
	NSFonts::IApplicationFonts* m_pAppFonts;
	int							m_nUserLCID;
	bool						m_bStartRead;

private: 
	std::vector<int>	m_aShapeId;
	int					m_nZIndexLast;

public: 
	int GetZIndex();
	void SetZIndex(int val);

	IdGenerator m_oIdGenerator;

	void SetShapeId( int nShapeId );
	int GetShapeId(  int& nShapeId  );
	
	RtfDocument();
	virtual ~RtfDocument();
	
	int GetType();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfDocument> RtfDocumentPtr;
