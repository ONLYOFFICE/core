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
#ifndef _AGGPLUSTYPES_H
#define _AGGPLUSTYPES_H

#include "../common/Types.h"

namespace Aggplus
{
typedef INT PixelFormat;

typedef struct tagRECT
{
    long    left;
    long    top;
    long    right;
    long    bottom;
} RECT, *PRECT;

typedef struct tagPOINT
{
    long    x;
    long    y;

} POINT, *PPOINT;

#define REAL_MAX            FLT_MAX
#define REAL_MIN            FLT_MIN
#define REAL_TOLERANCE     (FLT_MIN * 100)
#define REAL_EPSILON        1.192092896e-07F        /* FLT_EPSILON */

//--------------------------------------------------------------------------
// Status return values from AGG+ methods
//--------------------------------------------------------------------------

enum Status
{
    Ok							= 0,
    GenericError				= 1,
    InvalidParameter			= 2,
    OutOfMemory					= 3,
    ObjectBusy					= 4,
    InsufficientBuffer			= 5,
    NotImplemented				= 6,
    Win32Error					= 7,
    WrongState					= 8,
    Aborted						= 9,
    FileNotFound				= 10,
    ValueOverflow				= 11,
    AccessDenied				= 12,
    UnknownImageFormat			= 13,
    FontFamilyNotFound			= 14,
    FontStyleNotFound			= 15,
    NotTrueTypeFont				= 16,
    UnsupportedGdiplusVersion	= 17,
    AggplusNotInitialized		= 18,
    PropertyNotFound			= 19,
    PropertyNotSupported		= 20
};

enum PenAlignment
{
    PenAlignmentCenter       = 0,
    PenAlignmentInset        = 1
};

template <typename T>
class SizeF_T
{
public:
		SizeF_T() : Width(0), Height(0) { }
		SizeF_T(const SizeF_T& size) : Width(size.Width), Height(size.Height) { }
		SizeF_T(T width, T height) : Width(width), Height(height) { }

		SizeF_T operator+(const SizeF_T& sz) const { return SizeF_T(Width+sz.Width, Height+sz.Height); }
		SizeF_T operator-(const SizeF_T& sz) const { return SizeF_T(Width-sz.Width, Height-sz.Height); }
		INT Equals(const SizeF_T& sz) const { return((Width == sz.Width) && (Height == sz.Height)); }
		INT Empty() const { return(Width == 0 && Height == 0); }

public:
		T Width, Height;
};

typedef SizeF_T<REAL> SizeF;
typedef SizeF_T<int> Size;

template <typename T>
class PointF_T
{
public:
		PointF_T() : X(0), Y(0) { }
		PointF_T(const PointF_T &point) : X(point.X), Y(point.Y)  { }
		PointF_T(const SizeF_T<T> &size) : X(size.Width), Y(size.Height)  { }
		PointF_T(T x, T y) : X(x), Y(y) { }
		//~PointF() { }
		INT Equals(const PointF_T& point) const { return(X==point.X && Y==point.Y); }
		PointF_T operator+(const PointF_T& point) const { return PointF_T(X + point.X, Y + point.Y); }
		PointF_T operator-(const PointF_T& point) const { return PointF_T(X - point.X, Y - point.Y); }
public:
		T X, Y;
};

typedef PointF_T<REAL> PointF;
typedef PointF_T<int> Point;
typedef PointF_T<double> PointD;

template <typename T>
class RectF_T
{
public:
		RectF_T() : X(0), Y(0), Width(0), Height(0) { }
		RectF_T(T x, T y, T width, T height) : X(x), Y(y), Width(width), Height(height) { }

		RectF_T* Clone() const { return new RectF_T(X, Y, Width, Height); }

		void GetLocation(PointF_T<T>* point) const { point->X = X; point->Y = Y; }
		void GetSize(SizeF_T<T>* size) const { size->Width = Width; size->Height = Height; }
		void GetBounds(RectF_T* rect) const { rect->X = X; rect->Y = Y; rect->Width = Width; rect->Height = Height; }
		inline T GetLeft() const { return X; }
		inline T GetTop() const { return Y; }
		inline T GetRight() const { return X+Width; }
		inline T GetBottom() const { return Y+Height; }
		INT IsEmptyArea() const { return (Width <= (T)REAL_EPSILON) || (Height <= (T)REAL_EPSILON); }
		INT Equals(const RectF_T & rect) const
		{
			return X == rect.X && Y == rect.Y &&
				Width == rect.Width && Height == rect.Height;
		}
		INT Contains(T x, T y) const
		{
			return x >= X && x < X+Width &&
				y >= Y && y < Y+Height;
		}
		INT Contains(const PointF_T<T>& pt) const { return Contains(pt.X, pt.Y); }
		INT Contains(const RectF_T& rect) const
		{
			return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
				(Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
		}

		void Inflate(T dx, T dy) { X -= dx; Y -= dy; Width += 2*dx; Height += 2*dy; }
		void Inflate(const PointF_T<T>& point) { Inflate(point.X, point.Y); }

		INT Intersect(const RectF_T& rect) { return Intersect(*this, *this, rect); }
		static INT Intersect(RectF_T& c, const RectF_T& a, const RectF_T& b)
		{
				T right = min(a.GetRight(), b.GetRight());
				T bottom = min(a.GetBottom(), b.GetBottom());
				T left = max(a.GetLeft(), b.GetLeft());
				T top = max(a.GetTop(), b.GetTop());

				c.X = left;
				c.Y = top;
				c.Width = right - left;
				c.Height = bottom - top;
				return !c.IsEmptyArea();
		}

		INT IntersectsWith(const RectF_T& rect) const
		{
			return (GetLeft() < rect.GetRight() &&
				GetTop() < rect.GetBottom() &&
				GetRight() > rect.GetLeft() &&
				GetBottom() > rect.GetTop());
		}

		static INT Union(RectF_T& c, const RectF_T& a, const RectF_T& b)
		{
				T right = max(a.GetRight(), b.GetRight());
				T bottom = max(a.GetBottom(), b.GetBottom());
				T left = min(a.GetLeft(), b.GetLeft());
				T top = min(a.GetTop(), b.GetTop());

				c.X = left;
				c.Y = top;
				c.Width = right - left;
				c.Height = bottom - top;
				return !c.IsEmptyArea();
		}

		void Offset(const PointF_T<T>& point) { Offset(point.X, point.Y); }
		void Offset(T dx, T dy) { X += dx; Y += dy; }

public:
		T X, Y, Width, Height;
};

typedef RectF_T<REAL> RectF;
typedef RectF_T<int> Rect;

class CDoublePoint
{
public:
	double x;
	double y;
public:
	CDoublePoint()
	{
		x = 0;
		y = 0;
	}
	CDoublePoint(double dx, double dy)
	{
		x = dx;
		y = dy;
	}
	CDoublePoint& operator=(const CDoublePoint& oSrc)
	{
		x = oSrc.x;
		y = oSrc.y;

		return *this;
	}
	CDoublePoint(const CDoublePoint& oSrc)
	{
		*this = oSrc;
	}
};

class CDoubleRect
{
public:
	double left;
	double top;
	double right;
	double bottom;

public:
	CDoubleRect(double dLeft = 0.0, double dTop = 0.0, double dRight = 0.0, double dBottom = 0.0)
	{
		left	= dLeft;
		top		= dTop;
		right	= dRight;
		bottom	= dBottom;
	}
	CDoubleRect& operator=(const CDoubleRect& oSrc)
	{
		left	= oSrc.left;
		top		= oSrc.top;
		right	= oSrc.right;
		bottom	= oSrc.bottom;

		return *this;
	}
	CDoubleRect(const CDoubleRect& oSrc)
	{
		*this = oSrc;
	}

	inline double GetWidth() const
	{
		return right - left;
	}
	inline double GetHeight() const
	{
		return bottom - top;
	}

	inline void Offset(double dX, double dY)
	{
		left	+= dX;
		top		+= dY;
		right	+= dX;
		bottom	+= dY;
	}

	inline double GetCentreX() const
	{
		return (left + right) / 2.0;
	}

	inline double GetCentreY() const
	{
		return (top + bottom) / 2.0;
	}

	inline CDoublePoint GetCenter() const
	{
		return CDoublePoint((left + right) / 2.0, (top + bottom) / 2.0);
	}

	inline INT IsPointInside(const CDoublePoint& oPoint)
	{
		return IsPointInside(oPoint.x, oPoint.y);
	}
	INT IsPointInside(const double& pointX, const double& pointY)
	{
		return ((left <= pointX) && (right >= pointX) &&
				(top <= pointY) && (bottom >= pointY));
	}
};

} //namespace Aggplus

#endif // _AGGPLUSTYPES_H
