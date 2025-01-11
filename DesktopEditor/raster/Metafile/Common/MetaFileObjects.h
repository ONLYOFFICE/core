/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

		virtual std::vector<std::pair<unsigned int, double>> GetGradientColors() const = 0;

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
