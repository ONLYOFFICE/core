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
#pragma once

#include <algorithm>

namespace Gdiplus
{

typedef float REAL;

#ifndef REAL_EPSILON
    #define REAL_EPSILON 1.192092896e-07F
#endif

enum DashStyle
{
    DashStyleSolid,          // 0
    DashStyleDash,           // 1
    DashStyleDot,            // 2
    DashStyleDashDot,        // 3
    DashStyleDashDotDot,     // 4
    DashStyleCustom          // 5
};

enum PenAlignment
{
    PenAlignmentCenter       = 0,
    PenAlignmentInset        = 1
};
class RectF
{
public:

    RectF()
    {
        X = Y = Width = Height = 0.0f;
    }

    RectF(REAL x,
          REAL y,
          REAL width,
          REAL height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    /*RectF(const PointF& location,
          const SizeF& size)
    {
        X = location.X;
        Y = location.Y;
        Width = size.Width;
        Height = size.Height;
    }*/

    RectF* Clone() const
    {
        return new RectF(X, Y, Width, Height);
    }

    /*void GetLocation(PointF* point) const
    {
        point->X = X;
        point->Y = Y;
    }*/

    /*void GetSize(SizeF* size) const
    {
        size->Width = Width;
        size->Height = Height;
    }*/

    void GetBounds(RectF* rect) const
    {
        rect->X = X;
        rect->Y = Y;
        rect->Width = Width;
        rect->Height = Height;
    }

    REAL GetLeft() const
    {
        return X;
    }

    REAL GetTop() const
    {
        return Y;
    }

    REAL GetRight() const
    {
        return X+Width;
    }

    REAL GetBottom() const
    {
        return Y+Height;
    }

    bool IsEmptyArea() const
    {
        return (Width <= REAL_EPSILON) || (Height <= REAL_EPSILON);
    }

    bool Equals(const RectF & rect) const
    {
        return X == rect.X &&
               Y == rect.Y &&
               Width == rect.Width &&
               Height == rect.Height;
    }

    bool Contains(REAL x,
                  REAL y) const
    {
        return x >= X && x < X+Width &&
               y >= Y && y < Y+Height;
    }

   /* bool Contains(const PointF& pt) const
    {
        return Contains(pt.X, pt.Y);
    }*/

    bool Contains(const RectF& rect) const
    {
        return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
               (Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
    }

    void Inflate(REAL dx, REAL dy)
    {
        X -= dx;
        Y -= dy;
        Width += 2*dx;
        Height += 2*dy;
    }

    /*void Inflate(const PointF& point)
    {
        Inflate(point.X, point.Y);
    }
*/
    bool Intersect(const RectF& rect)
    {
        return Intersect(*this, *this, rect);
    }

    static bool Intersect(RectF& c,                          const RectF& a,                          const RectF& b)
    {
        REAL right = (std::min)(a.GetRight(), b.GetRight());
        REAL bottom = (std::min)(a.GetBottom(), b.GetBottom());
        REAL left = (std::max)(a.GetLeft(), b.GetLeft());
        REAL top = (std::max)(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    bool IntersectsWith(const RectF& rect) const
    {
        return (GetLeft() < rect.GetRight() &&
                GetTop() < rect.GetBottom() &&
                GetRight() > rect.GetLeft() &&
                GetBottom() > rect.GetTop());
    }

    static bool Union(RectF& c,                      const RectF& a,                      const RectF& b)
    {
        REAL right = (std::max)(a.GetRight(), b.GetRight());
        REAL bottom = (std::max)(a.GetBottom(), b.GetBottom());
        REAL left = (std::min)(a.GetLeft(), b.GetLeft());
        REAL top = (std::min)(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

   /* void Offset(const PointF& point)
    {
        Offset(point.X, point.Y);
    }
*/
    void Offset(REAL dx,                REAL dy)
    {
        X += dx;
        Y += dy;
    }

public:

    REAL X;
    REAL Y;
    REAL Width;
    REAL Height;
};


}
