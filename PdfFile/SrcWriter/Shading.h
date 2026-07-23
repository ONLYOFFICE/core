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
#ifndef _PDF_WRITER_SRC_SHADING_H
#define _PDF_WRITER_SRC_STREAMS_H

#include "Objects.h"
#include "Pattern.h"

namespace PdfWriter
{
	enum EShadingType
	{
		shadingtype_Unknown = 0,
		shadingtype_Axial   = 2,
		shadingtype_Radial  = 3
	};
	//----------------------------------------------------------------------------------------
	// CShading
	//----------------------------------------------------------------------------------------
	class CShading : public CDictObject
	{
	public:
		CShading(CXref* pXref);
		virtual ~CShading();

		void SetRgbColors(unsigned char* pColors, double* dPoints, int nCount);
		void SetGrayColors(unsigned char* pColors, double* dPoints, int nCount);
		void SetExtend(bool bBeing, bool bEnd);
		bool CompareColors(unsigned char* pColors, double* pPoints, int nCount, bool bRgb);
		bool CompareExtend(bool bBeing, bool bEnd);

		virtual EShadingType GetShadingType()
		{
			return shadingtype_Unknown;
		}

	protected:

		CXref*       m_pXref;
		
	private:

		bool           m_bRgb;            // Rgb or Gray
		unsigned char* m_pColors;
		double*        m_pColorsPoints;
		int            m_nColorsCount;

		bool           m_bBeginExtend;
		bool           m_bEndExtend;
	};
	//----------------------------------------------------------------------------------------
	// CAxialShading
	//----------------------------------------------------------------------------------------
	class CAxialShading : public CShading
	{
	public:
		CAxialShading(CXref* pXref, double dX0, double dY0, double dX1, double dY1);
		bool Compare(double dX0, double dY0, double dX1, double dY1);
		EShadingType GetShadingType()
		{
			return shadingtype_Axial;
		}
	private:
		double m_dX0;
		double m_dY0;
		double m_dX1;
		double m_dY1;
	};
	//----------------------------------------------------------------------------------------
	// CRadialShading
	//----------------------------------------------------------------------------------------
	class CRadialShading : public CShading
	{
	public:
		CRadialShading(CXref* pXref, double dX0, double dY0, double dR0, double dX1, double dY1, double dR1);
		bool Compare(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1);
		EShadingType GetShadingType()
		{
			return shadingtype_Radial;
		}
	private:
		double m_dX0;
		double m_dY0;
		double m_dR0;
		double m_dX1;
		double m_dY1;
		double m_dR1;
	};
}

#endif // _PDF_WRITER_SRC_STREAMS_H