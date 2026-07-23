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
#ifndef _METAFILE_COMMON_METAFILEOBJECTS_H
#define _METAFILE_COMMON_METAFILEOBJECTS_H

#include <string>
#include <vector>

namespace MetaFile
{
	class IFont
	{
	public:
		IFont(){}
		virtual ~IFont(){}

		virtual double       GetHeight()      const = 0;
		virtual std::wstring GetFaceName()    const = 0;
		virtual int          GetWeight()      const = 0;
		virtual bool         IsItalic()       const = 0;
		virtual bool         IsStrikeOut()    const = 0;
		virtual bool         IsUnderline()    const = 0;
		virtual int          GetEscapement()  const = 0;
		virtual int          GetCharSet()     const = 0;
		virtual int          GetOrientation() const = 0;
	};

	class IBrush
	{
	public:
		IBrush(){}
		virtual ~IBrush(){}

		virtual int          GetColor()         const = 0;
		virtual int          GetColor2()        const = 0;
		virtual unsigned int GetStyle()         const = 0;
		virtual unsigned int GetStyleEx()       const = 0;
		virtual unsigned int GetHatch()         const = 0;
		virtual unsigned int GetAlpha()         const = 0;
		virtual unsigned int GetAlpha2()        const = 0;
		virtual std::wstring GetDibPatterPath() const = 0;
		virtual void         GetBounds(double& left, double& top, double& width, double& height) const = 0;
		virtual void         GetCenterPoint(double& dX, double& dY) const = 0;

		virtual void GetGradientColors(std::vector<long>& arColors, std::vector<double>& arPositions) const = 0;

		virtual void         GetDibPattern(unsigned char** pBuffer, unsigned int &unWidth, unsigned int &unHeight) const = 0;
	};

	class ILineCap
	{
	public:
		ILineCap(){}
		virtual ~ILineCap(){}
	};

	class IPen
	{
	public:
		IPen(){}
		virtual ~IPen(){}

		virtual int             GetColor()        const = 0;
		virtual unsigned int    GetStyle()        const = 0;
		virtual double          GetWidth()        const = 0;
		virtual unsigned int    GetAlpha()        const = 0;
		virtual double          GetMiterLimit()   const = 0;
		virtual double          GetDashOffset()   const = 0;
		virtual void            GetDashData(double*&, unsigned int&) const = 0;
		virtual const ILineCap* GetStartLineCap() const = 0;
		virtual const ILineCap* GetEndLineCap()   const = 0;
	};

	class IRegion
	{
	public:
		IRegion(){}
		virtual ~IRegion(){}
	};
}

#endif //_METAFILE_COMMON_METAFILEOBJECTS_H
