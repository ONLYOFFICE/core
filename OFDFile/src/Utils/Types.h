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

#ifndef TYPES_H
#define TYPES_H

#include <string>

namespace OFD
{
#define OFD_EPSILON 0.0001
struct TBox
{
	double m_dX;
	double m_dY;
	double m_dWidth;
	double m_dHeight;

	TBox();

	bool Empty() const;
	bool Read(const std::string& wsValue);
};

struct TMatrix
{
	double m_dM11;
	double m_dM12;
	double m_dM21;
	double m_dM22;
	double m_dDx;
	double m_dDy;

	TMatrix();

	bool Read(const std::string& sValue);
};

struct TPos
{
	double m_dX;
	double m_dY;

	TPos();

	bool Read(const std::string& sValue);
};

class CXmlReader;

struct TDest
{
	enum class EType
	{
		XYZ,  // Go to a point with a specified zoom level (Left, Top, Zoom)
		Fit,  // Fit the whole page
		FitH, // Fit to width, scroll to top (Top)
		FitV, // Fit to height, scroll to Left (Left)
		FitR  // Fit the specified rectangle (Left, Top, Right, Bottom)
	}m_eType;

	unsigned int m_unPageID;

	double* m_pLeft;
	double* m_pTop;
	double* m_pRight;
	double* m_pBottom;
	double* m_pZoom;

	TDest();
	~TDest();

	static TDest* Read(CXmlReader& oReader);
};
}

#endif // TYPES_H
