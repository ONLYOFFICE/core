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

		bool           m_bRgb;            // Rgb или Gray
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