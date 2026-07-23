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
#ifndef _PATH_H
#define _PATH_H

#include "../common/Types.h"
#include <memory>
#include <string.h>
#include <stdlib.h>
#include "ftbitmap.h"
#include "ftimage.h"

#include "../graphics/pro/Fonts.h"

//-------------------------------------------------------------------------------------------------------------------------------
// TPathPoint
//-------------------------------------------------------------------------------------------------------------------------------

struct TPathPoint 
{
	double dX;
	double dY;
};

//-------------------------------------------------------------------------------------------------------------------------------
// CPath.nFlags
//-------------------------------------------------------------------------------------------------------------------------------

// Flag for first point of any subpath
#define PathFirst         0x01

// Flag for last point of any subpath
#define PathLast          0x02

// If subpath is closed, then its last and first points must coincide, their flag in this case is
#define PathClosed        0x04

// Flag indicating that this point is a control point for Bezier curve
#define PathCurve         0x08

//-------------------------------------------------------------------------------------------------------------------------------
// TPathHint
//-------------------------------------------------------------------------------------------------------------------------------

struct TPathHint 
{
	int nFirstControl;
	int nSecondControl;
	int nFirstPoint;
	int nLastPoint;
};

//-------------------------------------------------------------------------------------------------------------------------------
// CFontPath
//-------------------------------------------------------------------------------------------------------------------------------

class CFontPath : public NSFonts::IFontPath
{
public:

	CFontPath();
	virtual ~CFontPath();

	void Append(CFontPath *pPath);

	int MoveTo(double dX, double dY);

	int LineTo(double dX, double dY);

	int CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);

	int Close();

	void Offset(double dDx, double dDy);

	void Reverse();

	void ToMM(double dHorDpi, double dVerDpi);

	int GetCount()
	{
		return m_nPointsCount;
	}
	void GetPoint(int nIndex, double *pdX, double *pdY, unsigned char *punFlag)
	{
		*pdX = m_pPoints[nIndex].dX;
		*pdY = m_pPoints[nIndex].dY;
		*punFlag = m_pFlags[nIndex];
	}

	INT GetCurPoint(double *pdX, double *pdY);

	// Add the StrokeAdjust flag.
	void AddStrokeAdjustHint(int nFirstControl, int nSecondControl, int nFirstPoint, int nLastPoint);
	
	virtual int ToInterface(NSFonts::ISimpleGraphicsPath* pPath);

private:

	CFontPath(CFontPath *pPath);
	void Resize(int nPointsCount);
	INT NoCurrentPoint()
	{
		return m_nCurSubpath == m_nPointsCount;
	}
	INT OnePointSubpath()
	{
		return m_nCurSubpath == m_nPointsCount - 1;
	}
	INT OpenSubpath()
	{
		return m_nCurSubpath < m_nPointsCount - 1;
	}

private:

	TPathPoint    *m_pPoints;      // Array of points
	unsigned char *m_pFlags;       // Array of flags indicating point value in SubPath
	int            m_nPointsCount; // Number of points
	int            m_nSize;        // Actual array size

	int            m_nCurSubpath;  // Index of first point in last SubPath

	TPathHint     *m_pHints;       //
	int            m_nHintsCount;
	int            m_nHintsSize;
};

#endif /* _PATH_H */
