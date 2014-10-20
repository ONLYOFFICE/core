#pragma once

namespace Gdiplus
{

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

    /*VOID GetLocation(PointF* point) const
    {
        point->X = X;
        point->Y = Y;
    }*/

    /*VOID GetSize(SizeF* size) const
    {
        size->Width = Width;
        size->Height = Height;
    }*/

    VOID GetBounds(RectF* rect) const
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

    BOOL IsEmptyArea() const
    {
        return (Width <= REAL_EPSILON) || (Height <= REAL_EPSILON);
    }

    BOOL Equals(const RectF & rect) const
    {
        return X == rect.X &&
               Y == rect.Y &&
               Width == rect.Width &&
               Height == rect.Height;
    }

    BOOL Contains(REAL x,
                  REAL y) const
    {
        return x >= X && x < X+Width &&
               y >= Y && y < Y+Height;
    }

   /* BOOL Contains(const PointF& pt) const
    {
        return Contains(pt.X, pt.Y);
    }*/

    BOOL Contains(const RectF& rect) const
    {
        return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
               (Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
    }

    VOID Inflate(REAL dx, REAL dy)
    {
        X -= dx;
        Y -= dy;
        Width += 2*dx;
        Height += 2*dy;
    }

    /*VOID Inflate(const PointF& point)
    {
        Inflate(point.X, point.Y);
    }
*/
    BOOL Intersect(const RectF& rect)
    {
        return Intersect(*this, *this, rect);
    }

    static BOOL Intersect(RectF& c,                          const RectF& a,                          const RectF& b)
    {
        REAL right = min(a.GetRight(), b.GetRight());
        REAL bottom = min(a.GetBottom(), b.GetBottom());
        REAL left = max(a.GetLeft(), b.GetLeft());
        REAL top = max(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    BOOL IntersectsWith(const RectF& rect) const
    {
        return (GetLeft() < rect.GetRight() &&
                GetTop() < rect.GetBottom() &&
                GetRight() > rect.GetLeft() &&
                GetBottom() > rect.GetTop());
    }

    static BOOL Union(RectF& c,                      const RectF& a,                      const RectF& b)
    {
        REAL right = max(a.GetRight(), b.GetRight());
        REAL bottom = max(a.GetBottom(), b.GetBottom());
        REAL left = min(a.GetLeft(), b.GetLeft());
        REAL top = min(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

   /* VOID Offset(const PointF& point)
    {
        Offset(point.X, point.Y);
    }
*/
    VOID Offset(REAL dx,                REAL dy)
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

class CPoint : public tagPOINT
{
public:
// Constructors

	// create an uninitialized point
	CPoint() throw();
	// create from two integers
	CPoint(int initX, int initY) throw();
	// create from another point
	CPoint(POINT initPt) throw();
	// create from a size
	CPoint(SIZE initSize) throw();
	// create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
	CPoint(LPARAM dwPoint) throw();


// Operations

// translate the point
	void Offset(int xOffset, int yOffset) throw();
	void Offset(POINT point) throw();
	void Offset(SIZE size) throw();
	void SetPoint(int X, int Y) throw();

	BOOL operator==(POINT point) const throw();
	BOOL operator!=(POINT point) const throw();
	void operator+=(SIZE size) throw();
	void operator-=(SIZE size) throw();
	void operator+=(POINT point) throw();
	void operator-=(POINT point) throw();

// Operators returning CPoint values
	CPoint operator+(SIZE size) const throw();
	CPoint operator-(SIZE size) const throw();
	CPoint operator-() const throw();
	CPoint operator+(POINT point) const throw();

// Operators returning CSize values
	CSize operator-(POINT point) const throw();

// Operators returning CRect values
	CRect operator+(const RECT* lpRect) const throw();
	CRect operator-(const RECT* lpRect) const throw();
};


}