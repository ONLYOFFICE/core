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
#include "Element.h"


PPT::CElement::CElement()
{
    m_bIsBackground				= false;
    m_bHaveAnchor				= true;
    m_bHidden					= false;

    m_bChildAnchorEnabled		= false;
    m_bAnchorEnabled			= false;
    m_bGroupAnchorEnabled		= false;

    m_lID						= -1;
    m_lLayoutID					= -1;

    m_lPlaceholderID			= -1;
    m_lPlaceholderType			= -1;
    m_bPlaceholderSet			= false;
    m_lPlaceholderSizePreset	= -1;
    m_lPlaceholderUserStr		= -1;
    m_nFormatDate				= 1;

    m_etType					= etShape;

    m_dStartTime				= 0.0;
    m_dEndTime					= 30.0;

    m_dRotate					= 0.0;
    m_bFlipH					= false;
    m_bFlipV					= false;
    m_bLine						= true;
    m_bIsFilled					= true;

    m_pTheme					= NULL;
    m_pLayout					= NULL;
    m_pParentElements			= NULL;
}

PPT::CElement::~CElement()
{
}

void PPT::CElement::SetProperiesToDublicate(CElementPtr pDublicate)
{
    if (!pDublicate)
        return;

    pDublicate->m_bIsBackground				= m_bIsBackground;
    pDublicate->m_bHaveAnchor				= m_bHaveAnchor;
    pDublicate->m_bHidden					= m_bHidden;

    pDublicate->m_etType					= m_etType;

    pDublicate->m_rcChildAnchor				= m_rcChildAnchor;
    pDublicate->m_rcAnchor					= m_rcAnchor;
    pDublicate->m_rcGroupAnchor				= m_rcGroupAnchor;

    pDublicate->m_bChildAnchorEnabled		= m_bChildAnchorEnabled;
    pDublicate->m_bAnchorEnabled			= m_bAnchorEnabled;
    pDublicate->m_bGroupAnchorEnabled		= m_bGroupAnchorEnabled;

    pDublicate->m_dStartTime				= m_dStartTime;
    pDublicate->m_dEndTime					= m_dEndTime;

    pDublicate->m_lID						= m_lID;
    pDublicate->m_lLayoutID					= m_lLayoutID;

    pDublicate->m_arrActions					= m_arrActions;
    pDublicate->m_oTextActions				= m_oTextActions;
    pDublicate->m_oAnimations				= m_oAnimations;
    pDublicate->m_oEffects					= m_oEffects;

    pDublicate->m_lPlaceholderID			= m_lPlaceholderID;
    pDublicate->m_lPlaceholderType			= m_lPlaceholderType;
    pDublicate->m_bPlaceholderSet			= m_bPlaceholderSet;

    pDublicate->m_lPlaceholderSizePreset	= m_lPlaceholderSizePreset;
    pDublicate->m_lPlaceholderUserStr		= m_lPlaceholderUserStr;
    pDublicate->m_nFormatDate				= m_nFormatDate;

    pDublicate->m_dRotate					= m_dRotate;
    pDublicate->m_bFlipH					= m_bFlipH;
    pDublicate->m_bFlipV					= m_bFlipV;
    pDublicate->m_bLine						= m_bLine;
    pDublicate->m_bIsFilled					= m_bIsFilled;

    pDublicate->m_pTheme					= m_pTheme;
    pDublicate->m_pLayout					= m_pLayout;

    pDublicate->m_oPen						= m_oPen;
    pDublicate->m_oBrush					= m_oBrush;
    pDublicate->m_oShadow					= m_oShadow;
}

void PPT::CElement::NormalizeCoordsByMetric()
{
    if (!m_bAnchorEnabled) return;
    double dScale = 1587.5; //master to emu

    m_rcAnchor.left		= dScale * m_rcAnchor.left;
    m_rcAnchor.right	= dScale * m_rcAnchor.right;
    m_rcAnchor.top		= dScale * m_rcAnchor.top;
    m_rcAnchor.bottom	= dScale * m_rcAnchor.bottom;
}
