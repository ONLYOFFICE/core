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
#ifndef _PDF_WRITER_SRC_GSTATE_H
#define _PDF_WRITER_SRC_GSTATE_H

#include "Types.h"
#include "Objects.h"

// Default values for Graphic State
#define DEF_FONT               "Helvetica"
#define DEF_WORDSPACE          0
#define DEF_CHARSPACE          0
#define DEF_FONTSIZE           10
#define DEF_HSCALING           100
#define DEF_LEADING            0
#define DEF_RENDERING_MODE     PdfWriter::textrenderingmode_Fill
#define DEF_RISE               0
#define DEF_LINEWIDTH          1
#define DEF_LINECAP            PdfWriter::linecap_Butt
#define DEF_LINEJOIN           PdfWriter::linejoin_Miter
#define DEF_MITERLIMIT         10
#define DEF_FLATNESS           1
#define DEF_PAGE_NUM           1

namespace PdfWriter
{
	class CFontDict;
	class CDictObject;
	//----------------------------------------------------------------------------------------
	// CGrState
	//----------------------------------------------------------------------------------------
	class CGrState
	{
	public:

		CGrState(CGrState* pCurrent);
		~CGrState();
		CGrState*    GetPrev()
		{
			return m_pPrev;
		}
		unsigned int GetDepth()
		{
			return m_unDepth;
		}

	private:

		CMatrix            m_oMatrix;
		double             m_dLineWidth;
		ELineCapStyle      m_eLineCap;
		ELineJoinStyle     m_eLineJoin;
		double             m_dMiterLimit;
		CDashMode          m_oDashMode;
		double             m_dFlatness;

		double             m_dCharSpace;
		double             m_dWordSpace;
		double             m_dHScalling;
		double             m_dTextLeading;
		ETextRenderingMode m_eRenderingMode;
		double             m_dTextRise;

		TRgb               m_oFillColor;
		TRgb               m_oStrokeColor;

		CFontDict*         m_pFont;
		double             m_dFontSize;

		CGrState*          m_pPrev;
		unsigned int       m_unDepth;

		friend class CPage;
	};	
	//----------------------------------------------------------------------------------------
	// CExtGrState
	//----------------------------------------------------------------------------------------
	class CExtGrState : public CDictObject
	{
	public:
		CExtGrState(CXref* pXref);
		void       SetAlphaStroke     (double dValue);
		void       SetAlphaFill       (double dValue);
		void       SetBlendMode       (EBlendMode eBlendMode);
		void       SetStrokeAdjustment(bool bValue);
		double     GetAlphaStroke();
		double     GetAlphaFill();
		EBlendMode GetBlendMode();
		bool       GetStrokeAdjustment();
		EDictType  GetDictType() const
		{
			return dict_type_EXT_GSTATE;
		}
	private:
		double     m_dAlphaStroke;
		double     m_dAlphaFill;
		EBlendMode m_eBlendMode;
		bool       m_bStrokeAdj;
	};
}

#endif // _PDF_WRITER_SRC_GSTATE_H

