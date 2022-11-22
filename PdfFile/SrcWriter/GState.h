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
#ifndef _PDF_WRITER_SRC_GSTATE_H
#define _PDF_WRITER_SRC_GSTATE_H

#include "Types.h"
#include "Objects.h"

// Стандартные значения для Graphic State
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

