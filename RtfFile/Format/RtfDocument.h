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
#pragma once

#include "Basic.h"

#include "RtfGlobalTables.h"
#include "RtfSection.h"
#include "RtfMath.h"
#include "RtfShape.h"

#include "../../Common/MS-LCID.h"

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

//to convert old List format to Numbering
	std::vector<RtfOldListPtr> m_aOldLists;
	
	NSFonts::IApplicationFonts* m_pAppFonts;
	int							m_nUserLCID;
	bool						m_bStartRead;

private: 
	std::vector<int>	m_aShapeId;
	int					m_nZIndexLast;

public: 
	MS_LCID_converter m_lcidConverter;

	int GetZIndex(bool bReverse = false);
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
