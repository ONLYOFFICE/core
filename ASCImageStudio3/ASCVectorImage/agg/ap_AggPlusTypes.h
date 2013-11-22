

#ifndef _AGGPLUSTYPES_H
#define _AGGPLUSTYPES_H


namespace Aggplus
{

typedef float REAL;

#define REAL_MAX            FLT_MAX
#define REAL_MIN            FLT_MIN
#define REAL_TOLERANCE     (FLT_MIN * 100)
#define REAL_EPSILON        1.192092896e-07F        /* FLT_EPSILON */

//--------------------------------------------------------------------------
// Status return values from AGG+ methods
//--------------------------------------------------------------------------

enum Status
{
    Ok = 0,
    GenericError = 1,
    InvalidParameter = 2,
    OutOfMemory = 3,
    ObjectBusy = 4,
    InsufficientBuffer = 5,
    NotImplemented = 6,
    Win32Error = 7,
    WrongState = 8,
    Aborted = 9,
    FileNotFound = 10,
    ValueOverflow = 11,
    AccessDenied = 12,
    UnknownImageFormat = 13,
    FontFamilyNotFound = 14,
    FontStyleNotFound = 15,
    NotTrueTypeFont = 16,
    UnsupportedGdiplusVersion = 17,
    AggplusNotInitialized = 18,
    PropertyNotFound = 19,
    PropertyNotSupported = 20
};




class SizeF
{
public:
		SizeF() : Width(0.0f), Height(0.0f) { }
		SizeF(const SizeF& size) : Width(size.Width), Height(size.Height) { }
		SizeF(REAL width, REAL height) : Width(width), Height(height) { }

		SizeF operator+(const SizeF& sz) const { return SizeF(Width+sz.Width, Height+sz.Height); }
		SizeF operator-(const SizeF& sz) const { return SizeF(Width-sz.Width, Height-sz.Height); }
		BOOL Equals(const SizeF& sz) const { return((Width == sz.Width) && (Height == sz.Height)); }
		BOOL Empty() const { return(Width == 0.0f && Height == 0.0f); }

public:
		REAL Width, Height;
};


class PointF  
{
public:
		PointF() : X(0.0f), Y(0.0f) { }
		PointF(const PointF &point) : X(point.X), Y(point.Y)  { }
		PointF(const SizeF &size) : X(size.Width), Y(size.Height)  { }
			PointF(REAL x, REAL y) : X(x), Y(y) { }
			//~PointF() { }
		BOOL Equals(const PointF& point) const { return(X==point.X && Y==point.Y); }
		PointF operator+(const PointF& point) const { return PointF(X + point.X, Y + point.Y); }
		PointF operator-(const PointF& point) const { return PointF(X - point.X, Y - point.Y); }
public:
		REAL X, Y;
};

class RectF
{
public:
		RectF() : X(0.0f), Y(0.0f), Width(0.0f), Height(0.0f) { }
		RectF(REAL x, REAL y, REAL width, REAL height) : X(x), Y(y), Width(width), Height(height) { }
		RectF(RECT rct)  
		   { X = REAL(rct.left); Y = REAL(rct.top); Width = REAL(rct.right-rct.left); Height=REAL(rct.bottom-rct.top);}
		RectF(const PointF& location, const SizeF& size) : X(location.X), Y(location.Y), Width(size.Width), Height(size.Height) { }

		RectF* Clone() const { return new RectF(X, Y, Width, Height); }

		void GetLocation(PointF* point) const { point->X = X; point->Y = Y; }
		void GetSize(SizeF* size) const { size->Width = Width; size->Height = Height; }
		void GetBounds(RectF* rect) const { rect->X = X; rect->Y = Y; rect->Width = Width; rect->Height = Height; }
		REAL GetLeft() const { return X; }
		REAL GetTop() const { return Y; }
		REAL GetRight() const { return X+Width; }
		REAL GetBottom() const { return Y+Height; }
		BOOL IsEmptyArea() const { return (Width <= REAL_EPSILON) || (Height <= REAL_EPSILON); }
		BOOL Equals(const RectF & rect) const
		{
			return X == rect.X && Y == rect.Y &&
				Width == rect.Width && Height == rect.Height;
		}
		BOOL Contains(REAL x, REAL y) const
		{
			return x >= X && x < X+Width &&
				y >= Y && y < Y+Height;
		}
		BOOL Contains(const PointF& pt) const { return Contains(pt.X, pt.Y); }
		BOOL Contains(const RectF& rect) const
		{
			return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
				(Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
		}

		void Inflate(REAL dx, REAL dy) { X -= dx; Y -= dy; Width += 2*dx; Height += 2*dy; }
		void Inflate(const PointF& point) { Inflate(point.X, point.Y); }

		BOOL Intersect(const RectF& rect) { return Intersect(*this, *this, rect); }
		static BOOL Intersect(RectF& c, const RectF& a, const RectF& b)
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

		static BOOL Union(RectF& c, const RectF& a, const RectF& b)
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

		void Offset(const PointF& point) { Offset(point.X, point.Y); }
		void Offset(REAL dx, REAL dy) { X += dx; Y += dy; }

public:
		REAL X, Y, Width, Height;
};


} //namespace Aggplus

#endif // _AGGPLUSTYPES_H