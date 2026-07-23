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
#include "GState.h"

namespace PdfWriter
{
	const static char* c_sBlendModeNames[] = 
	{
		"Normal",
		"Multiply",
		"Screen",
		"Overlay",
		"Darken",
		"Lighten",
		"ColorDodge",
		"ColorBurn",
		"HardLight",
		"SoftLight",
		"Difference",
		"Exclusion"
	};
	//----------------------------------------------------------------------------------------
	// CGrState
	//----------------------------------------------------------------------------------------
	CGrState::CGrState(CGrState* pCurrent)
	{
		if (pCurrent)
		{
			m_oMatrix        = pCurrent->m_oMatrix;
			m_dLineWidth     = pCurrent->m_dLineWidth;
			m_eLineCap       = pCurrent->m_eLineCap;
			m_eLineJoin      = pCurrent->m_eLineJoin;
			m_dMiterLimit    = pCurrent->m_dMiterLimit;
			m_oDashMode      = pCurrent->m_oDashMode;
			m_dFlatness      = pCurrent->m_dFlatness;

			m_dCharSpace     = pCurrent->m_dCharSpace;
			m_dWordSpace     = pCurrent->m_dWordSpace;
			m_dHScalling     = pCurrent->m_dHScalling;
			m_dTextLeading   = pCurrent->m_dTextLeading;
			m_eRenderingMode = pCurrent->m_eRenderingMode;
			m_dTextRise      = pCurrent->m_dTextRise;

			m_oFillColor     = pCurrent->m_oFillColor;
			m_oStrokeColor   = pCurrent->m_oStrokeColor;

			m_pFont          = pCurrent->m_pFont;
			m_dFontSize      = pCurrent->m_dFontSize;

			m_pPrev          = pCurrent;
			m_unDepth        = pCurrent->m_unDepth + 1;
		}
		else
		{
			m_dLineWidth     = DEF_LINEWIDTH;
			m_eLineCap       = DEF_LINECAP;
			m_eLineJoin      = DEF_LINEJOIN;
			m_dMiterLimit    = DEF_MITERLIMIT;
			m_dFlatness      = DEF_FLATNESS;

			m_dCharSpace     = DEF_CHARSPACE;
			m_dWordSpace     = DEF_WORDSPACE;
			m_dHScalling     = DEF_HSCALING;
			m_dTextLeading   = DEF_LEADING;
			m_eRenderingMode = DEF_RENDERING_MODE;
			m_dTextRise      = DEF_RISE;

			m_pFont          = NULL;
			m_dFontSize      = 0;

			m_pPrev          = NULL;
			m_unDepth        = 1;
		}
	}
	CGrState::~CGrState()
	{
	}
	//----------------------------------------------------------------------------------------
	// CExtGrState
	//----------------------------------------------------------------------------------------
	CExtGrState::CExtGrState(CXref* pXref)
	{
		if (pXref)
			pXref->Add(this);
		Add("Type", "ExtGState");

		m_eBlendMode   = blendmode_Unknown;
		m_dAlphaFill   = 1;
		m_dAlphaStroke = 1;
		m_bStrokeAdj   = true;
	}
	void CExtGrState::SetAlphaStroke     (double dValue)
	{
        dValue = std::min(1.0, std::max(0.0, dValue));
	    Add("CA", dValue);
		m_dAlphaStroke = dValue;
	}
	void CExtGrState::SetAlphaFill       (double dValue)
	{
        dValue = std::min(1.0, std::max(0.0, dValue));
	    Add("ca", dValue);
		m_dAlphaFill = dValue;
	}	
	void CExtGrState::SetBlendMode       (EBlendMode eBlendMode)
	{
        eBlendMode = std::min(blendmode_Max, std::max(blendmode_Min, eBlendMode));
		Add("BM", c_sBlendModeNames[(int)eBlendMode]);
		m_eBlendMode = eBlendMode;
	}	
	void CExtGrState::SetStrokeAdjustment(bool bValue)
	{
	    Add("SA", bValue);
		m_bStrokeAdj = bValue;
	}
	double     CExtGrState::GetAlphaStroke()
	{
		return m_dAlphaStroke;
	}
	double     CExtGrState::GetAlphaFill()
	{
		return m_dAlphaFill;
	}
	EBlendMode CExtGrState::GetBlendMode()
	{
		return m_eBlendMode;
	}
	bool       CExtGrState::GetStrokeAdjustment()
	{
		return m_bStrokeAdj;
	}
}
