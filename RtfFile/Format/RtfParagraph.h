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
#include "RtfChar.h"

class RtfParagraph;
typedef boost::shared_ptr<RtfParagraph> RtfParagraphPtr;

class RtfOldList : IRenderableProperty
{
public: 
	enum _LevelType { lt_none, lt_blt, lt_level, lt_body, lt_cont };
	enum _LevelJust { lj_none, lj_left, lj_center, lj_right };

	_LevelType		m_eLevelType;
	int				m_nLevelType;
	_LevelJust		m_eLevelJust;

	RtfParagraphPtr m_oLevelText;	//list text format

	int m_nLs;
	int m_nIlvl;

	RtfParagraphPtr m_oText;		//replacement text
	
	RtfOldList()
	{
		SetDefault();
	}
	bool CanConvertToNumbering();
	
	void SetDefault();
	
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
	
	bool operator==( const RtfOldList& oOldList );
};

typedef boost::shared_ptr<RtfOldList> RtfOldListPtr;

class RtfParagraph : public ITextItem, public ItemContainer< IDocumentElementPtr >
{
private:
	bool m_IsValid = false;
public: 
	RtfParagraphProperty	m_oProperty;
	RtfOldListPtr			m_oOldList;
	
	RtfParagraph();

	virtual bool IsValid();
	int GetType( );

	void SetValid(bool val);
	int AddItem( IDocumentElementPtr piRend );	

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
