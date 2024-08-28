#include "BooleanOperations.h"
#include <algorithm>

namespace Aggplus {
Segment::Segment() :
	P(PointD()),
	HI(PointD()),
	HO(PointD()),
	Id(0),
	Index(0),
	Winding(0) {}

Segment::Segment(const std::vector<PointD>& points, bool isCurve,
				 int index, int id, CGraphicsPath* path) :
	IsCurve(isCurve),
	Visited(false),
	Index(index), 
	Id(id),
	Winding(0),
	Path(path)
{
	if (IsCurve)
	{
		P = points[2];
		HI = PointD(points[0].X - P.X, points[0].Y - P.Y);
		HO = PointD(points[1].X - P.X, points[1].Y - P.Y);
	}
	else
		P = points[0];
}

Segment::Segment(const PointD& p, const PointD& hi, const PointD& ho) :
	P(p),
	HI(hi),
	HO(ho),
	IsCurve(false)
{
	if (!hi.Equals(PointD()) || !ho.Equals(PointD()))
		IsCurve = true;
}

void Segment::SetHandles(const PointD &hi, const PointD &ho)
{
	IsCurve = true;
	HI = PointD(hi.X - P.X, hi.Y - P.Y);
	HO = PointD(ho.X - P.X, ho.Y - P.Y);
}

void Segment::UpdateHandles(const PointD &hi, const PointD &ho)
{
	HI = hi;
	HO = ho;
}

bool Segment::IsValid(BooleanOpType op) const
{
	if (Visited)
		return false;
	if ((bool)Inters)
		return true;
	if (op == Subtraction)
	{
		if (Id == 1 && Winding == 0)
			return true;
		else if (Id == 2 && Winding == 1)
			return true;
	}
	else if (Winding == op)
		return true;
	return false;
}

bool Segment::operator==(const Segment& other) const
{
	return (Index == other.Index) && (Id == other.Id);
}

bool Segment::operator!=(const Segment& other) const
{
	return !operator==(other);
}

Curve::Curve() :
	Segment1(Segment()),
	Segment2(Segment()){}

Curve::Curve(const Segment& segment1, const Segment& segment2) :
	Segment1(segment1),
	Segment2(segment2) {}

Curve::Curve(const std::vector<double>& values)
{
	if (values.size() == 4)
	{
		Segment1 = Segment(PointD(values[0], values[1]),
						   PointD(),
						   PointD());
		Segment2 = Segment(PointD(values[2], values[3]),
						   PointD(),
						   PointD());
	}
	else if (values.size() == 8)
	{
		Segment1 = Segment(PointD(values[0], values[1]),
						   PointD(),
						   PointD());
		Segment2 = Segment(PointD(values[6], values[7]),
						   PointD(values[2] - values[6],
								  values[3] - values[7]),
						   PointD(values[4] - values[6],
								  values[5] - values[7]));
	}
	else
	{
		Segment1 = Segment();
		Segment2 = Segment();
	}
}

Curve::Curve(double x0, double y0, double x1, double y1,
			 double x2, double y2, double x3, double y3)
{
	Segment1 = Segment(PointD(x0, y0), PointD(), PointD());
	Segment2 = Segment(PointD(x3, y3), PointD(x1, y1), PointD(x2, y2));
}

std::vector<double> Curve::GetXValues() const
{
	if (IsStraight())
		return {Segment1.P.X, Segment1.P.X, Segment2.P.X, Segment2.P.X};
	return {Segment1.P.X, Segment2.P.X + Segment2.HI.X,
			Segment2.P.X + Segment2.HO.X, Segment2.P.X};
}

std::vector<double> Curve::GetYValues() const
{
	if (IsStraight())
		return {Segment1.P.Y, Segment1.P.Y, Segment2.P.Y, Segment2.P.Y};
	return {Segment1.P.Y,
			Segment2.P.Y + Segment2.HI.Y,
			Segment2.P.Y + Segment2.HO.Y,
			Segment2.P.Y};
}

std::vector<double> Curve::GetPeeks() const
{
	std::vector<double>	x = GetXValues(),
						y = GetYValues(),
						roots;
	double	ax = -x[0] + 3 * x[1] - 3 * x[2] + x[3],
			bx =  3 * x[0] - 6 * x[1] + 3 * x[2],
			cx = -3 * x[0] + 3 * x[1],
			ay = -y[0] + 3 * y[1] - 3 * y[2] + y[3],
			by =  3 * y[0] - 6 * y[1] + 3 * y[2],
			cy = -3 * y[0] + 3 * y[1],
			tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;
	SolveCubic(9 * (ax * ax + ay * ay),
			   9 * (ax * bx + by * ay),
			   2 * (bx * bx + by * by) + 3 * (cx * ax + cy * ay),
			   (cx * bx + by * cy),
			   roots, tMin, tMax);
	std::sort(roots.begin(), roots.end());
	return roots;
}

double Curve::GetLength(double a, double b) const
{
	if(IsStraight())
	{
		Curve c = *this;
		if (b < 1)
			c = Subdivide(b)[0];
		if (a > 0)
			c = Subdivide(a)[1];
		double	dx = c.Segment2.P.X - c.Segment1.P.X,
				dy = c.Segment2.P.Y - c.Segment1.P.Y;
		return sqrt(dx * dx + dy * dy);
	}
	std::vector<double>	x = GetXValues(),
						y = GetYValues();
	double	ax = 9 * (x[1] - x[2]) + 3 * (x[3] - x[0]),
			bx = 6 * (x[0] + x[2]) - 12 * x[1],
			cx = 3 * (x[1] - x[0]),
			ay = 9 * (y[1] - y[2]) + 3 * (y[3] - y[0]),
			by = 6 * (y[0] + y[2]) - 12 * y[1],
			cy = 3 * (y[1] - y[0]);
	return integrate(ax, bx, cx, ay, by, cy, a, b);
}

double Curve::GetSquaredLineLength() const
{
	double	x = Segment2.P.X - Segment1.P.X,
			y = Segment2.P.Y - Segment1.P.Y;
	return x * x + y * y;
}

double Curve::GetTimeOf(const PointD& point) const
{
	PointD	p0 = Segment1.P,
			p3 = Segment2.P;
	double	d0 = getDistance(point.X, point.Y, p0.X, p0.Y),
			d3 = getDistance(point.X, point.Y, p3.X, p3.Y);

	if (d0 > EPSILON && d3 > EPSILON)
	{
		std::vector<double> coords = {point.X, point.Y},
							roots;
		for (int c = 0; c < 2; c++)
		{
			int count = SolveCubic(c, coords[c], roots, 0.0, 1.0);
			for (int i = 0; i < count; i++)
			{
				double u = roots[i];
				if (u > 1 || u < 0)
					return - 1;
				if (getDistance(point, GetPoint(u)) <= GEOMETRIC_EPSILON)
					return u;
			}
		}
	}
	bool firstDist = d0 <= GEOMETRIC_EPSILON,
		 secondDist = d3 <= GEOMETRIC_EPSILON;
	return firstDist ? 0 : secondDist ? 1 : - 1;
}

double Curve::GetTimeAt(double offset) const
{
	bool forward = offset > 0;
	double start = !forward ? 1 : 0;
	if (offset == 0)
		return start;

	double	a = forward ? start : 0,
			b = forward ? 1 : start,
			rangeLength = GetLength(a, b),
			diff = abs(offset) - rangeLength;

	if (abs(diff) < EPSILON)
		return forward ? b : a;
	else if (diff > EPSILON)
		return DBL_MIN;

	double	guess = offset / rangeLength,
			length = 0;
	std::vector<double>	x = GetXValues(),
						y = GetYValues();
	double	ax = 9 * (x[1] - x[2]) + 3 * (x[3] - x[0]),
			bx = 6 * (x[0] + x[2]) - 12 * x[1],
			cx = 3 * (x[1] - x[0]),
			ay = 9 * (y[1] - y[2]) + 3 * (y[3] - y[0]),
			by = 6 * (y[0] + y[2]) - 12 * y[1],
			cy = 3 * (y[1] - y[0]);
	return findRoot(length, start, offset, ax, bx, cx, 
					ay, by, cy, start + guess, a, b);
}

PointD Curve::Get(double t, int type) const
{
	std::vector<double>	x = GetXValues(),
						y = GetYValues();

	if (isZero(x[1] - x[0]) && isZero(y[1] - y[0]))
	{
		x[1] = x[0];
		y[1] = y[0];
	}
	if (isZero(x[2] - x[3]) && isZero(y[2] - y[3]))
	{
		x[2] = x[3];
		y[2] = y[3];
	}
	double	cx = 3 * (x[1] - x[0]),
			bx = 3 * (x[2] - x[1]) - cx,
			ax = x[3] - x[0] - cx - bx,
			cy = 3 * (y[1] - y[0]),
			by = 3 * (y[2] - y[1]) - cy,
			ay = y[3] - y[0] - cy - by,
			xv = ((ax * t + bx) * t + cx) * t + x[0],
			yv = ((ay * t + by) * t + cy) * t + y[0],
			x0 = t == 0 ? x[0] : t == 1 ? x[3] : xv,
			y0 = t == 0 ? y[0] : t == 1 ? y[3] : yv;

	if (type == 1)
	{
		double	tMin = CURVETIME_EPSILON,
				tMax = 1 - tMin;
	
		if (t < tMin)
		{
			x0 = cx;
			y0 = cy;
		}
		else if (t > tMax)
		{
			x0 = 3 * (x[3] - x[2]);
			y0 = 3 * (y[3] - y[2]);
		}
		else
		{
			x0 = (3 * ax * t + 2 * bx) * t + cx;
			y0 = (3 * ay * t + 2 * by) * t + cy;
		}

		if (x0 == 0.0 && y0 == 0.0 && (t < tMin || t > tMax))
		{
			x0 = x[2] - x[1];
			y0 = y[2] - y[1];
		}

		double len = sqrt(x0 * x0 + y0 * y0);
		if (len != 0.0)
		{
			x0 /= len;
			y0 /= len;
		}
	}
	return PointD(x0, y0);
}

PointD Curve::GetPoint(double t) const
{
	return Get(t, 0);
}

PointD Curve::GetTangent(double t) const
{
	return Get(t, 1);
}

PointD Curve::GetTangent(double t, double offset,
						 bool inside, const PointD& p) const
{
	if (inside)
		return GetTangent(t);
	PointD point =  GetPoint(GetTimeAt(offset));
	return PointD(point.X - p.X, point.Y - p.Y);
}

Curve Curve::GetPart(double from, double to) const
{
	Curve result = *this;
	if (from > to)
		std::swap(from, to);
	if (from > 0)
	{
		result = Subdivide(from)[1];
		result.Segment2.HI.X -= result.Segment2.P.X;
		result.Segment2.HI.Y -= result.Segment2.P.Y;
		result.Segment2.HO.X -= result.Segment2.P.X;
		result.Segment2.HO.Y -= result.Segment2.P.Y;
	}
	if (to < 1)
	{
		result = result.Subdivide((to - from) / (1 - from))[0];
		result.Segment2.HI.X -= result.Segment2.P.X;
		result.Segment2.HI.Y -= result.Segment2.P.Y;
		result.Segment2.HO.X -= result.Segment2.P.X;
		result.Segment2.HO.Y -= result.Segment2.P.Y;
	}
	if (from > to)
		result.Flip();
	return result;
}

std::vector<std::pair<int, int>> Curve::GetOverlaps(const Curve& curve) const
{
	bool swap = this->GetSquaredLineLength() <
				curve.GetSquaredLineLength(),
		 straight1 = this->IsStraight(),
		 straight2 = curve.IsStraight(),
		 straightBoth = straight1 && straight2;
	std::vector<double>	x1 = GetXValues(),
						y1 = GetYValues(),
						x2 = curve.GetXValues(),
						y2 = curve.GetYValues();
	double	px = swap ? x2[0] : x1[0],
			py = swap ? y2[0] : y1[0],
			vx = swap ? x2[3] - px : x1[3] - px,
			vy = swap ? y2[3] - py : y1[3] - py;
	
	if (swap)
	{
		std::swap(x1, x2); 
		std::swap(y1, y2);
	}
	
	double	d20 = getDistance(px, py, vx, vy, x2[0], y2[0], true),
			d23 = getDistance(px, py, vx, vy, x2[3], y2[3], true);

	if (d20 < GEOMETRIC_EPSILON && d23 < GEOMETRIC_EPSILON)
	{
		double	d11 = getDistance(px, py, vx, vy, x1[1], y1[1], true),
				d12 = getDistance(px, py, vx, vy, x1[2], y1[2], true),
				d21 = getDistance(px, py, vx, vy, x2[1], y2[1], true),
				d22 = getDistance(px, py, vx, vy, x2[2], y2[2], true);

		if (!straightBoth &&
			d11 < GEOMETRIC_EPSILON &&
			d12 < GEOMETRIC_EPSILON &&
			d21 < GEOMETRIC_EPSILON &&
			d22 < GEOMETRIC_EPSILON)
		{
			straight1 = straight2 = straightBoth = true;
		}
	}
	else if (straightBoth)
	{
		return std::vector<std::pair<int, int>>();
	}
	if (straight1 ^ straight2)
	{
		return std::vector<std::pair<int, int>>();
	}

	std::vector<std::pair<int, int>> pairs;
	for (int i = 0; i < 4 && pairs.size() < 2; i++)
	{
		int i1 = i & 1,
			t1 = i >> 1;
		double t2 = i1 == 0	? GetTimeOf(t1 == 1	? curve.Segment2.P : curve.Segment1.P)
							: curve.GetTimeOf(t1 == 1 ? Segment2.P : Segment1.P);
		if (t2 != -1)
		{
			std::pair<int, int> pair = i1 == 1 ? std::pair<int, int>(t1, t2)
											   : std::pair<int, int>(t1, t2);
			if (pairs.empty())
				pairs.push_back(pair);
			else if (abs(pair.first - pairs[0].first) > TIME_EPSILON &&
					 abs(pair.second - pairs[0].second) > TIME_EPSILON)
			pairs.push_back(pair);
		}

		if (i > 2 && pairs.empty())
			break;
	}

	if (pairs.size() != 2)
		pairs.clear();
	else if (!straightBoth)
	{
		Curve	o1 = GetPart(pairs[0].first, pairs[1].first),
				o2 = curve.GetPart(pairs[0].second, pairs[1].second);
		double	dox = abs(o2.Segment2.HO.X - o1.Segment2.HO.X),
				doy = abs(o2.Segment2.HO.Y - o1.Segment2.HO.Y),
				dix = abs(o2.Segment2.HI.X - o1.Segment2.HI.X),
				diy = abs(o2.Segment2.HI.Y - o1.Segment2.HI.Y);
		if (dox > GEOMETRIC_EPSILON ||
			doy > GEOMETRIC_EPSILON ||
			dix > GEOMETRIC_EPSILON ||
			diy > GEOMETRIC_EPSILON)
		pairs.clear();
	}
	return pairs;
}

std::vector<double> Curve::GetCurveLineIntersection(double px, double py,
													double vx, double vy) const
{
	if (isZero(vx) && isZero(vy))
	{
		double t = GetTimeOf(PointD(px, py));
		return t == -1 ? std::vector<double>() : std::vector<double>({t});
	}

	double	angle = std::atan2(-vy, vx),
			sin = std::sin(angle),
			cos = std::cos(angle);
	std::vector<double>	x = GetXValues(),
						y = GetYValues(),
						rv, roots;
	for (int i = 0; i < 4; i++)
	{
		double cx = x[i] - px,
			   cy = y[i] - py;
		rv.push_back(cx * cos - cy * sin);
		rv.push_back(cx * sin + cy * cos);
	}
	Curve curve = Curve(rv);
	curve.SolveCubic(1, 0, roots, 0, 1);
	return roots;
}

std::vector<Curve> Curve::Subdivide(double t) const
{
	std::vector<double>	x = GetXValues(),
						y = GetYValues();
	double x2[6], y2[6], u = 1 - t;

	x2[0] = u * x[0] + t * x[1], y2[0] = u * y[0] + t * y[1],
	x2[1] = u * x[1] + t * x[2], y2[1] = u * y[1] + t * y[2],
	x2[2] = u * x[2] + t * x[3], y2[2] = u * y[2] + t * y[3],
	x2[3] = u * x2[0] + t * x2[1], y2[3] = u * y2[0] + t * y2[1],
	x2[4] = u * x2[1] + t * x2[2], y2[4] = u * y2[1] + t * y2[2],
	x2[5] = u * x2[3] + t * x2[4], y2[5] = u * y2[3] + t * y2[4];

	return {Curve(x[0], y[0], x2[0], y2[0], x2[3], y2[3], x2[5], y2[5]),
			Curve(x2[5], y2[5], x2[4], y2[4], x2[2], y2[2], x[3], y[3])};
}

Curve Curve::DivideAtTime(double time)
{
	double	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;

	if (time >= tMin && time <= tMax)
	{
		std::vector<Curve> parts = Subdivide(time);
		Curve	left = parts[0],
				right = parts[1];

		Segment segment = Segment({left.Segment2.P},
								  false,
								  Segment1.Index + 1,
								  Segment1.Id,
								  Segment1.Path);

		if (!IsStraight())
		{
			segment.SetHandles(left.Segment2.HI, left.Segment2.HO);
			Segment2.SetHandles(right.Segment2.HI, right.Segment2.HO);
		}

		return Curve(segment, Segment2);
	}
	return Curve();
}

int Curve::SolveCubic(int coord, double value, std::vector<double>& roots,
					double mn, double mx) const
{
	int count = 0;
	std::vector<double> v = coord == 0 ? GetXValues() : GetYValues();
	double a, b, c, d;

	if (!((v[0] < value && v[3] < value && v[1] < value && v[2] < value) ||
		  (v[0] > value && v[3] > value && v[1] > value && v[2] > value)))
	{
		c = 3 * (v[1] - v[0]);
		b = 3 * (v[2] - v[1]) - c;
		a = v[3] - v[0] - c - b;
		d = v[0] - value;
		count = SolveCubic(a, b, c, d, roots, mn, mx);
	}
	return count;
}

int Curve::SolveCubic(double a, double b, double c, double d,
					std::vector<double>& roots, double mn, double mx) const
{
	int count = 0;
	double f = max(abs(a), abs(b), abs(c), abs(d));
	double x, b1, c2, qd, q;
	if (f < 1e-8 || f > 1e8)
	{
		f = pow(2, -round(log2(f)));
		a *= f;
		b *= f;
		c *= f;
		d *= f;
	}

	if (abs(a) < EPSILON)
	{
		a = b;
		b1 = c;
		c2 = d;
		x = DBL_MAX;
	}
	else if (abs(d) <EPSILON)
	{
		b1 = b;
		c2 = c;
		x = 0;
	}
	else
	{
		x = -(b / a) / 3;
		b1 = a * x + b;
		c2 = b1 * x + c;
		qd = (a * x + b1) * x + c2;
		q = c2 * x + d;
		double t = q / a,
			  r = pow(abs(t), 1 / 3),
			  s = t < 0 ? -1 : 1,
			  td = -qd / a,
			  rd = td > 0 ? 1.324717957244746 * std::max(r, sqrt(td)) : r,
			  x0 = x - s * rd;
		if (x0 != x)
		{
			do
			{
				x = x0;
				b1 = a * x + b;
				c2 = b1 * x + c;
				qd = (a * x + b1) * x + c2;
				q = c2 * x + d;
				x0 = qd == 0 ? x : x - q / qd / (1 + MACHINE_EPSILON);
			} while (s * x0 > s * x);

			if (abs(a) * x * x > abs(d / x))
			{
				c2 = -d / x;
				b1 = (c2 - c) / x;
			}
		}
	}
	count = solveQuadratic(a, b1, c2, roots, mn, mx);
	bool xInRoots1 = count > 0 && x != roots[0],
		 xInRoots2 = count > 1 && x != roots[1],
		 xInEps = x > mn - EPSILON && x < mx + EPSILON;
	if (x != DBL_MAX && (count == 0 || xInRoots1 || xInRoots2) && (xInEps))
	{
		roots.push_back(clamp(x, mn, mx));
		count++;
	}
	return count;
}

void Curve::Flip()
{
	std::swap(Segment1.P, Segment2.P);
	std::swap(Segment1.HI, Segment1.HO);
}

bool Curve::IsStraight() const
{
	return !Segment2.IsCurve;
}

bool Curve::operator==(const Curve& other) const
{
	return Segment1 == other.Segment1 && 
		   Segment2 == other.Segment2;
}

bool Curve::operator!=(const Curve& other) const
{
	return !operator ==(other);
}

Location::Location() :
	C(Curve()),
	S(Segment()),
	Inters(nullptr),
	Next(nullptr),
	Prev(nullptr) {}

Location::Location(const Curve& curve, double time, bool overlap) :
	C(curve),
	Time(time),
	Overlap(overlap) {}

bool Location::IsTouching()
{
	if (this->Inters == nullptr)
		return false;
	if (!isCollinear(C.GetTangent(Time), Inters->C.GetTangent(Inters->Time)))
		return false;
	Curve	c1 = this->C,
			c2 = this->Inters->C;
	bool straight = c1.IsStraight() && c2.IsStraight();
	return !straight || intersect(c1.Segment1.P.X,
								  c1.Segment1.P.Y,
								  c1.Segment2.P.X,
								  c1.Segment2.P.Y,
								  c2.Segment1.P.X,
								  c2.Segment1.P.Y,
								  c2.Segment2.P.X,
								  c2.Segment2.P.Y).Equals(PointD());
}

CBooleanOperations::CBooleanOperations(CGraphicsPath* path1,
									CGraphicsPath* path2,
									BooleanOpType op) :
	Op(op),
	Path1(path1),
	Path2(path2),
	Result(new CGraphicsPath),
	IsDeleted(false)
{
	TraceBoolean();
}

CBooleanOperations::~CBooleanOperations()
{
	if (!IsDeleted)
		delete Result;
}

CGraphicsPath *CBooleanOperations::GetResult()
{
	IsDeleted = true;
	return Result;
}

int CBooleanOperations::CheckInters(const PointD& point, const Segment& segment, const Curve& curve) const
{
	PointD pt = intersect(point.X, point.Y, segment.P.X, segment.P.Y, curve.Segment1.P.X, curve.Segment1.P.Y, curve.Segment2.P.X, curve.Segment2.P.Y);
	if (curve.Segment1.P.Equals(pt) || curve.Segment2.P.Equals(pt))
	{
		PointD newPoint = PointD(point.X + GEOMETRIC_EPSILON, point.Y);
		return CheckInters(newPoint, segment, curve);
	}
	else if (!pt.Equals(PointD()) && curve.IsStraight())
	{
		return 1;
	}
	else if (!curve.IsStraight())
	{
		std::vector<double> roots = curve.GetCurveLineIntersection(segment.P.X,segment.P.Y, point.X - segment.P.X, point.Y - segment.P.Y);
		Curve line = Curve(segment, Segment(point, PointD(), PointD()));
		int count = 0;
		for (const auto& r : roots)
			if (line.GetTimeOf(curve.GetPoint(r)) != -1)
				count++;

		return count;
	}
	return 0;
}

void CBooleanOperations::TraceBoolean()
{
	bool reverse = false;
	if ((Op == Subtraction || Op == Exclusion) ^
		Path1->IsClockwise() ^
		Path2->IsClockwise())
		reverse = true;

	PreparePath(Path1, 1, Segments1, Curves1);
	PreparePath(Path2, 2, Segments2, Curves2, reverse);

	GetIntersection();

	if (Locations.empty())
	{

		int count = 0;
		PointD minPt = GetMinPoint(Segments1);
		for (const auto& c : Curves2)
			count += CheckInters(minPt, Segments1[0], c);

		for (auto& s : Segments1)
			s.Winding = count % 2;

		for (auto& s : Segments2)
			if (count % 2 == 0)
				s.Winding = 1;
			else
				s.Winding = 0;
	}
	else
	{
		int length = Locations.size() - 1;
		for (int i = 0; i <= length; i++)
		{
			int before = Locations.size();
			InsertLocation(Locations[i]->Inters, false);
			if (before == Locations.size())
				Locations.erase(Locations.begin() + i);
		}

		DivideLocations();

		for (const auto& l : Locations)
		{
			Segment start = l->S,
				s = GetNextSegment(l->S);

			if (s == Segment() || (bool)s.Inters || s == start)
				continue;

			int count = 0;
			PointD minPoint = GetMinPoint(s.Id == 1 ? Segments2 : Segments1);
			for (const auto& c : (s.Id == 1 ? Curves2 : Curves1))
				count += CheckInters(minPoint, s, c);

			do
			{
				if (s.Id == 1 )
					Segments1[s.Index].Winding = count % 2;
				else
					Segments2[s.Index].Winding = count % 2;
				s = GetNextSegment(s);
			} while (s != Segment() && !(bool)s.Inters && s != start);
		}
	}

	TracePaths();
}

void CBooleanOperations::SetVisited(const Segment& segment)
{
	if (segment.Id == 1)
		Segments1[segment.Index].Visited = true;
	else
		Segments2[segment.Index].Visited = true;
}

void CBooleanOperations::TracePaths()
{
	size_t length = Segments1.size();
	Result->StartFigure();
	for (size_t i = 0; i < length + Segments2.size(); i++)
	{
		Segment s = i >= length ? Segments2[i - length] : Segments1[i];
		bool valid = s.IsValid(Op),
			 start = true;
		while (valid)
		{
			if (!start || (Op == Intersection && s.Inters))
				SetVisited(s);
			if (start)
			{
				Result->MoveTo(s.P.X, s.P.Y);
				start = false;
			}
			else
			{
				if (s.IsCurve)
					Result->CurveTo(s.HI.X + s.P.X, s.HI.Y + s.P.Y, s.HO.X + s.P.X, s.HO.Y + s.P.Y, s.P.X, s.P.Y);
				else
					Result->LineTo(s.P.X, s.P.Y);
			}

			Segment prev = s;
			s = GetNextSegment(s);
			valid = s.IsValid(Op);
			if ((s.Inters && prev.Inters && Op != Intersection) ||
				(!valid && prev.Inters))
			{
				s = GetNextSegment(prev.Inters->S);
				SetVisited(prev.Inters->S);
				valid = s.IsValid(Op);
			}
		}			
	}
	Result->CloseFigure();
}

void CBooleanOperations::PreparePath(CGraphicsPath* path, int id,
									 std::vector<Segment>& segments,
									 std::vector<Curve>& curves,
									 bool reverse)
{
	int length = path->Is_poly_closed() ? (path->GetPointCount() - 1) : path->GetPointCount();
	int idx = 0;
	if (reverse)
	{
		bool isCurve = false;
		for (int i = length; i > 0; i--)
		{
			std::vector<PointD> points = path->GetPoints(isCurve ? i - 2 : i, isCurve ? 3 : 1);
			std::reverse(points.begin(), points.end());
			if (isCurve && i - 2 == 0)
				segments[0].SetHandles(points[0], points[1]);
			else
				segments.push_back(Segment(points, isCurve, idx++, id, path));
			if (isCurve) i -= 2;
			isCurve = path->IsCurvePoint(i);
		}
	}
	else
	{
		for (int i = 0; i < length; i++)
		{
			bool isCurve = path->IsCurvePoint(i);
			std::vector<PointD> points = path->GetPoints(i, isCurve ? 3 : 1);
			if (isCurve && i + 2 == length)
				segments[0].SetHandles(points[0], points[1]);
			else
				segments.push_back(Segment(points, isCurve, idx++, id, path));
			if (isCurve) i += 2;
		}
	}
	length = segments.size();
	for (int i = 0; i < length; i++)
		curves.push_back(Curve(segments[i], i == (length - 1) ? segments[0] : segments[i + 1]));
}

void CBooleanOperations::InsertSegment(const Segment& segment, const Segment& handles, bool updateHandles)
{
	if (segment.Id == 1)
	{
		int length = Segments1.size(),
			index = segment.Index == length ? 0 : segment.Index;
		if (getDistance(segment.P, Segments1[index].P) <= GEOMETRIC_EPSILON)
		{
			Segments1[length ? 0 : index] = segment;
			return;
		}
		Segments1.insert(Segments1.begin() + segment.Index, segment);

		for (int i = segment.Index + 1; i < length + 1; i++)
			Segments1[i].Index++;

		if (updateHandles)
			Segments1[segment.Index == length ? 0 : segment.Index + 1].UpdateHandles(handles.HI, handles.HO);

		Curves1.clear();
		for (int i = 0; i < length + 1; i++)
			Curves1.push_back(Curve(Segments1[i], i == (Segments1.size() - 1) ?
														Segments1[0] :
														Segments1[i + 1]));
	}
	else
	{
		int length = Segments2.size(),
			index = segment.Index == length ? 0 : segment.Index;
		if (getDistance(segment.P, Segments2[index].P) <= GEOMETRIC_EPSILON)
		{
			Segments2[index] = segment;
			return;
		}
		Segments2.insert(Segments2.begin() + segment.Index, segment);

		for (int i = segment.Index + 1; i < length + 1; i++)
			Segments2[i].Index++;

		if (updateHandles)
			Segments2[segment.Index == length ? 0 : segment.Index + 1].UpdateHandles(handles.HI, handles.HO);

		Curves2.clear();
		for (int i = 0; i < length + 1; i++)
			Curves2.push_back(Curve(Segments2[i], i == (Segments2.size() - 1) ?
														Segments2[0] :
														Segments2[i + 1]));
	}

	for (auto& l : Locations)
		if (l->S.Index >= segment.Index && l->S.Id == segment.Id)
			l->S.Index++;
}

Curve CBooleanOperations::GetCurve(const Segment& segment) const
{
	return segment.Id == 1 ? Curves1[segment.Index] : Curves2[segment.Index];
}

Curve CBooleanOperations::GetPreviousCurve(const Curve& curve) const
{
	bool path1 = curve.Segment1.Id == 1;
	int length = path1 ? Curves1.size()
						: Curves2.size();
	if (curve.Segment1.Index == 0)
	{
		if (curve.Segment1.Path->Is_poly_closed())
			return path1 ? Curves1[length - 1] : Curves2[length - 1];
		else
			return Curve();
	}
	return	path1 ? Curves1[curve.Segment1.Index - 1]
				  : Curves2[curve.Segment1.Index - 1];
}

Curve CBooleanOperations::GetNextCurve(const Curve& curve) const
{
	bool path1 = curve.Segment1.Id == 1;
	int last = path1 ? Curves1.size() - 1
					 : Curves2.size() - 1;
	if (curve.Segment1.Index == last)
	{
		if (curve.Segment1.Path->Is_poly_closed())
			return path1 ? Curves1[0] : Curves2[0];
		else
			return Curve();
	}
	return	path1 ? Curves1[curve.Segment1.Index + 1]
				  : Curves2[curve.Segment1.Index + 1];
}

Segment CBooleanOperations::GetNextSegment(const Segment& segment) const
{
	bool path1 = segment.Id == 1;
	int length = path1 ? Segments1.size() : Segments2.size();
	if (segment.Index == (length - 1))
	{
		if (segment.Path->Is_poly_closed())
			return path1 ? Segments1[0] : Segments2[0];
		else
			return Segment();
	}
	return path1 ? Segments1[segment.Index + 1] : Segments2[segment.Index + 1];
}

std::vector<std::vector<double>> CBooleanOperations::GetBoundsForCurves(
	const std::vector<Curve>& curves) const
{
	std::vector<std::vector<double>> bounds;
	for (int i = 0; i < curves.size(); i++)
	{
		std::vector<double>	x = curves[i].GetXValues(),
							y = curves[i].GetYValues();
		bounds.push_back({min(x[0], x[1], x[2], x[3]),
						  min(y[0], y[1], y[2], y[3]),
						  max(x[0], x[1], x[2], x[3]),
						  max(y[0], y[1], y[2], y[3])});
	}
	return bounds;
}

std::vector<std::vector<int>> CBooleanOperations::FindCurveBoundsCollisions()
{
	std::vector<std::vector<double>> bounds1 = GetBoundsForCurves(Curves1);
	std::vector<std::vector<double>> bounds2 = GetBoundsForCurves(Curves2);

	return FindBoundsCollisions(bounds1, bounds2);
}

std::vector<std::vector<int>> CBooleanOperations::FindBoundsCollisions(
	const std::vector<std::vector<double>>& bounds1,
	const std::vector<std::vector<double>>& bounds2)
{
	bool self = bounds1 == bounds2;

	std::vector<std::vector<double>> allBounds(bounds1);
	if (!self)
		std::copy(bounds2.begin(), bounds2.end(), std::back_inserter(allBounds));

	int	allLength = allBounds.size(),
		length1 = bounds1.size();
	std::vector<int> allIdicesByPri1(allLength);
	for (int i = 0; i < allLength; i++)
		allIdicesByPri1[i] = i;

	std::sort(allIdicesByPri1.begin(),
			  allIdicesByPri1.end(),
	[&allBounds](int i1, int i2){
				  return allBounds[i1][0] < allBounds[i2][0];
	});

	std::vector<int> activeIndicesByPri2;
	std::vector<std::vector<int>> allCollisions(length1);
	for (int i = 0; i < allLength; i++)
	{
		int curIndex = allIdicesByPri1[i];
		std::vector<double> curBounds = allBounds[curIndex];
		std::vector<int> curCollisions;
		bool isCurrent1 = curIndex < length1,
			 isCurrent2 = self || !isCurrent1;
		int origIndex = self ? curIndex : curIndex - length1;

		if (!activeIndicesByPri2.empty())
		{
			int pruneCount = binarySearch(allBounds, activeIndicesByPri2, 2,
										  curBounds[0] - GEOMETRIC_EPSILON) + 1;

			activeIndicesByPri2.erase(activeIndicesByPri2.begin(),
									  activeIndicesByPri2.begin() + pruneCount);

			double	curSec2 = curBounds[3],
					curSec1 = curBounds[1];
			for (int j = 0; j < activeIndicesByPri2.size(); j++)
			{
				int activeIndex = activeIndicesByPri2[j];
				std::vector<double> activeBounds = allBounds[activeIndex];
				bool isActive1 = activeIndex < length1,
					 isActive2 = self || !isActive1,
					 isActive1Or2 = (isCurrent1 && isActive2) || (isCurrent2 && isActive1),
					 inRange1 = curSec1 <= activeBounds[3] + GEOMETRIC_EPSILON,
					 inRange2 = curSec2 >= activeBounds[1] - GEOMETRIC_EPSILON;

				if (isActive1Or2 && (inRange2 && inRange1))
				{
					if (isCurrent1 && isActive2)
						curCollisions.push_back(self ? activeIndex : activeIndex - length1);
					if (isCurrent2 && isActive1)
						allCollisions[activeIndex].push_back(origIndex);
				}
			}
		}
		if (isCurrent1)
		{
			if (bounds1 == bounds2)
				curCollisions.push_back(curIndex);
			allCollisions[curIndex] = curCollisions;
		}
		if (activeIndicesByPri2.size() > 0)
		{
			double curPri2 = curBounds[2];
			int index = binarySearch(allBounds, activeIndicesByPri2, 2, curPri2);

			activeIndicesByPri2.insert(activeIndicesByPri2.begin() + (1 + index), curIndex);
		}
		else
			activeIndicesByPri2.push_back(curIndex);
	}

	for (auto& c : allCollisions)
		std::sort(c.begin(), c.end());

	return allCollisions;
}

bool CBooleanOperations::IsCrossing(std::shared_ptr<Location> loc)
{
	if(loc->Inters == nullptr)
		return false;

	double	t1 = loc->Time,
			t2 = loc->Inters->Time,
			tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;
	bool	t1Inside = t1 >= tMin && t1 <= tMax,
			t2Inside = t2 >= tMin && t2 <= tMax;

	if (t1Inside && t2Inside)
		return !loc->IsTouching();

	Curve	c2 = loc->C,
			c1 = t1 < tMin ? GetPreviousCurve(c2) : c2,
			c4 = loc->Inters->C,
			c3 = t2 < tMin ? GetPreviousCurve(c4) : c4;

	if (t1 > tMax)
		c2 = GetNextCurve(c2);
	if (t2 > tMax)
		c4 = GetNextCurve(c4);
	
	std::vector<double> offsets;

	if (!t1Inside)
	{
		AddOffsets(offsets, c1, true);
		AddOffsets(offsets, c2, false);
	}
	if (!t2Inside)
	{
		AddOffsets(offsets, c3, true);
		AddOffsets(offsets, c4, false);
	}

	PointD	pt = loc->C.GetPoint(loc->Time);
	double	offset = DBL_MAX;
	for (const auto& o : offsets)
		if (o < offset)
			offset = o;

	PointD	v2 = c2.GetTangent(t1, offset, t1Inside, pt),
			v1 = t1Inside ? PointD(-v2.X, -v2.Y) : c1.GetTangent(t1, -offset, false, pt),
			v4 = c4.GetTangent(t2, offset, t2Inside, pt),
			v3 = t2Inside ? PointD(-v4.X, -v4.Y) : c3.GetTangent(t2, -offset, false, pt);

	double	a1 = v1.Equals(PointD()) ? 0.0 : atan2(v1.Y, v1.X) * 180 / M_PI,
			a2 = v2.Equals(PointD()) ? 0.0 : atan2(v2.Y, v2.X) * 180 / M_PI,
			a3 = v3.Equals(PointD()) ? 0.0 : atan2(v3.Y, v3.X) * 180 / M_PI,
			a4 = v4.Equals(PointD()) ? 0.0 : atan2(v4.Y, v4.X) * 180 / M_PI;

	bool	inRange34 = isInRange(a1, a3, a4) ^ isInRange(a2, a3, a4),
			inRange43 = isInRange(a1, a4, a3) ^ isInRange(a2, a4, a3),
			inRange1 = inRange34 && inRange43,
			inRange12 = isInRange(a3, a1, a2) ^ isInRange(a4, a1, a2),
			inRange21 = isInRange(a3, a2, a1) ^ isInRange(a4, a2, a1),
			inRange2 = inRange21 && inRange12;

	return t1Inside ? inRange1 : inRange2;
}

bool CBooleanOperations::FilterIntersections(std::shared_ptr<Location> loc)
{
	return loc->Overlap || IsCrossing(loc);
}

bool CBooleanOperations::IntersectsBounds()
{
	RectF_T<double> rect1, rect2;
	Path1->GetBounds(rect1.X, rect1.Y, rect1.Width, rect1.Height);
	Path2->GetBounds(rect2.X, rect2.Y, rect2.Width, rect2.Height);

	return	(rect2.X + rect2.Width > rect1.X - EPSILON) &&
			(rect2.Y + rect2.Height > rect1.Y - EPSILON) &&
			(rect2.X < rect1.X + rect1.Width + EPSILON) &&
			(rect2.Y < rect1.Y + rect2.Height + EPSILON);
}

void CBooleanOperations::GetIntersection()
{
	if (!IntersectsBounds())
		return;

	std::vector<std::vector<int>> boundsCollisions = FindCurveBoundsCollisions();
	for (int index1 = 0; index1 < Curves1.size(); index1++)
	{
		Curve curve1 = Curves1[index1];
		std::vector<int> collisions1 = boundsCollisions[index1];
		if (!collisions1.empty())
		{
			for (int j = 0; j < collisions1.size(); j++)
			{
				int index2 = collisions1[j];
				Curve curve2 = Curves2[index2];
				GetCurveIntersection(curve1, curve2);
			}
		}
	}
}

void CBooleanOperations::GetCurveIntersection(const Curve& curve1, const Curve& curve2)
{
	std::vector<double>	x1 = curve1.GetXValues(),
						y1 = curve1.GetYValues(),
						x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues();

	double	minX1 = min(x1[0], x1[1], x1[2], x1[3]),
			maxX1 = max(x1[0], x1[1], x1[2], x1[3]),
			minY1 = min(y1[0], y1[1], y1[2], y1[3]),
			maxY1 = max(y1[0], y1[1], y1[2], y1[3]),
			minX2 = min(x2[0], x2[1], x2[2], x2[3]),
			maxX2 = max(x2[0], x2[1], x2[2], x2[3]),
			minY2 = min(y2[0], y2[1], y2[2], y2[3]),
			maxY2 = max(y2[0], y2[1], y2[2], y2[3]);

	if (maxX1 + EPSILON > minX2 && minX1 - EPSILON < maxX2 &&
		maxY1 + EPSILON > minY2 && minY1 - EPSILON < maxY2)
	{
		std::vector<std::pair<int, int>> overlaps = curve1.GetOverlaps(curve2);
		if (!overlaps.empty())
		{
			for (int i = 0; i < 2; i++)
			{
				std::pair<int, int> overlap = overlaps[i];
				AddLocation(curve1, curve2, overlap.first,
							overlap.second, true);
			}
		}
		else
		{
			bool	straight1 = curve1.IsStraight(),
					straight2 = curve2.IsStraight(),
					straight = straight1 && straight2,
					flip = straight1 && !straight2;
			Curve	c1 = flip ? curve2 : curve1,
					c2 = flip ? curve1 : curve2;
			if (straight)
				AddLineIntersection(c1, c2);
			else if (straight1 || straight2)
				AddCurveLineIntersection(c1, c2, flip);
			else
				AddCurveIntersection(c1, c2, c1, c2, flip);
		}
	}
}

void CBooleanOperations::LinkIntersection(std::shared_ptr<Location> from,
										 std::shared_ptr<Location> to)
{
	std::shared_ptr<Location> prev = from;
	while (prev)
	{
		if (prev == to)
			return;
		prev = prev->Prev;
	}

	while (from->Next != nullptr && from->Next != to)
		from = from->Next;

	if (from->Next == nullptr)
	{
		while (to->Prev)
			to = to->Prev;
		from->Next = to;
		to->Prev = from;
	}
}

void CBooleanOperations::AddLineIntersection(const Curve& curve1, const Curve& curve2)
{
	std::vector<double>	x1 = curve1.GetXValues(),
						y1 = curve1.GetYValues(),
						x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues();
	PointD pt = intersect(x1[0], y1[0], x1[3], y1[3], x2[0], y2[0], x2[3], y2[3]);
	if (!pt.Equals(PointD()))
		AddLocation(curve1, curve2, curve1.GetTimeOf(pt), curve2.GetTimeOf(pt));
}

void CBooleanOperations::AddCurveLineIntersection(const Curve& curve1, const Curve& curve2, bool flip)
{
	std::vector<double>	x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues(),
						roots = curve1.GetCurveLineIntersection(x2[0], y2[0], x2[3] - x2[0], y2[3] - y2[0]);
	for (const auto& r : roots)
	{
		double	t1 = r;
		PointD	p1 = curve1.GetPoint(t1);
		double	t2 = curve2.GetTimeOf(p1);
		if (flip) std::swap(t1, t2);
		if (t2 != -1)
			AddLocation(flip ? curve2 :curve1, flip ? curve1 : curve2, t1, t2);
	}
}

int CBooleanOperations::AddCurveIntersection(Curve curve1, Curve curve2, const Curve& startCurve1,
											 const Curve& startCurve2, bool flip,
											 int recursion, int calls, double tMin,
											 double tMax, double uMin, double uMax)
{
	if (++calls >= 4096 || ++recursion >= 40)
		return calls;
	
	std::vector<double>	x1 = curve1.GetXValues(),
						y1 = curve1.GetYValues(),
						x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues();

	double	d1 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x2[1], y2[1]),
			d2 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x2[2], y2[2]),
			factor = (d1 * d2) > 0 ? 3.0 / 4.0 : 4.0 / 9.0,
			dMin = factor * min(0, d1, d2),
			dMax = factor * max(0, d1, d2),
			dp0 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[0], y1[0]),
			dp1 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[1], y1[1]),
			dp2 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[2], y1[2]),
			dp3 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[3], y1[3]);

	std::vector<std::vector<PointD>> hull = getConvexHull(dp0, dp1, dp2, dp3);
	std::vector<PointD>	top = hull[0],
						bottom = hull[1];

	double tMinClip = clipConvexHull(top, bottom, dMin, dMax);
	std::reverse(top.begin(), top.end());
	std::reverse(bottom.begin(), bottom.end());
	double tMaxClip = clipConvexHull(top, bottom, dMin, dMax);

	if ((d1 == 0 && d2 == 0 && dp0 == 0 && dp1 == 0 && dp2 == 0 && dp3 == 0)
		|| tMinClip == DBL_MIN || tMaxClip == DBL_MIN)
		return calls;
	
	double	tMinNew = tMin + (tMax - tMin) * tMinClip,
			tMaxNew = tMin + (tMax - tMin) * tMaxClip;
	
	if (std::max(uMax - uMin, tMaxNew - tMinNew) < LINE_EPSILON)
	{
		double	t = (tMinNew + tMaxNew) / 2,
				u = (uMin + uMax) / 2;
		Curve	c1 = flip ? startCurve2 : startCurve1,
				c2 = flip ? startCurve1 : startCurve2;
		double	t1 = flip ? u : t,
				t2 = flip ? t : u;
		AddLocation(c1, c2, t1, t2);
	}
	else
	{
		curve1 = curve1.GetPart(tMinClip, tMaxClip);
		double uDiff = uMax - uMin;

		if (tMaxClip - tMinClip > 0.8)
		{
			if (tMaxNew - tMinNew > uDiff)
			{
				std::vector<Curve> parts = curve1.Subdivide(0.5);
				double t = (tMinNew + tMaxNew) / 2;
				parts[0].Segment2.SetHandles(parts[0].Segment2.HI, parts[0].Segment2.HO);
				parts[1].Segment2.SetHandles(parts[1].Segment2.HI, parts[1].Segment2.HO);
				calls = AddCurveIntersection(curve2, parts[0], startCurve2, startCurve1,
											!flip, recursion, calls, uMin, uMax, tMinNew, t);
				calls = AddCurveIntersection(curve2, parts[1], startCurve2, startCurve1,
											!flip, recursion, calls, uMin, uMax, t, tMaxNew);
			}
			else
			{
				std::vector<Curve> parts = curve2.Subdivide(0.5);
				double u = (uMin + uMax) / 2;
				parts[0].Segment2.SetHandles(parts[0].Segment2.HI, parts[0].Segment2.HO);
				parts[1].Segment2.SetHandles(parts[1].Segment2.HI, parts[1].Segment2.HO);
				calls = AddCurveIntersection(parts[0], curve1, startCurve2, startCurve1,
											!flip, recursion, calls, uMin, u, tMinNew, tMaxNew);
				calls = AddCurveIntersection(parts[1], curve1, startCurve2, startCurve1,
											!flip, recursion, calls, u, uMax, tMinNew, tMaxNew);
			}
		}
		else
		{
			if (uDiff == 0 || uDiff >= LINE_EPSILON)
				calls = AddCurveIntersection(curve2, curve1, startCurve2, startCurve1,
											!flip, recursion, calls, uMin, uMax, tMinNew, tMaxNew);
			else
				calls = AddCurveIntersection(curve1, curve2, startCurve1, startCurve2,
											flip, recursion, calls, tMinNew, tMaxNew, uMin, uMax);
		}
	}
	return calls;
}

void CBooleanOperations::DivideLocations()
{
	double	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin,
			prevTime = -1.0;
	Curve	prevCurve;

	for (int i = Locations.size() - 1; i >= 0; i--)
	{
		std::shared_ptr<Location> loc = Locations[i];
		double	origTime = loc->Time,
				time = origTime;
		Segment segment;
		Curve	curve = GetCurve(loc->C.Segment1),
				newCurve;
		bool updateHandles = false;

		if (loc->C != prevCurve)
			prevCurve = loc->C;
		else if (prevTime >= tMin)
			time /= prevTime;

		prevTime = origTime;

		if (time < tMin)
		{
			segment = curve.Segment1;
		}
		else if (time > tMax)
		{
			segment = curve.Segment2;
		}
		else
		{
			if (curve.IsStraight())
				segment = Segment({loc->C.GetPoint(origTime)},
								  false,
								  curve.Segment1.Index + 1,
								  curve.Segment1.Id,
								  curve.Segment1.Path);
			else
			{
				newCurve = curve.Segment1.Id == 1 ? curve.DivideAtTime(time)
												  : curve.DivideAtTime(time);
				segment = newCurve.Segment1;
				updateHandles = true;
			}
		}
		std::shared_ptr<Location>	inter = segment.Inters,
									dest = loc->Inters;
		if ((bool)inter)
		{
			LinkIntersection(inter, dest);
			std::shared_ptr<Location> other = inter;
			while ((bool)other)
			{
				LinkIntersection(other->Inters, inter);
				other = other->Next;
			}
		}
		else
			segment.Inters = dest;

		InsertSegment(segment, newCurve.Segment2, updateHandles);
		loc->S = segment;
	}
}

void CBooleanOperations::InsertLocation(std::shared_ptr<Location> loc, bool overlap)
{
	if (Locations.empty())
	{
		Locations.push_back(loc);
		return;
	}

	int	length = Locations.size(),
			l = 0,
			r = length - 1;
	while (l <= r)
	{
		int mid = (l + r) >> 1;
		std::shared_ptr<Location> loc1 = Locations[mid];

		if (getDistance(loc->C.GetPoint(loc->Time), loc1->C.GetPoint(loc1->Time)) <= GEOMETRIC_EPSILON
			&& loc->C.Segment1.Id == loc1->C.Segment1.Id)
			return;

		if (overlap)
		{
			if (getDistance(loc->C.GetPoint(loc->Time), loc->Inters->C.GetPoint(loc->Inters->Time)) > GEOMETRIC_EPSILON)
				return;

			for (int i = mid - 1; i >= -1; i--)
			{
				std::shared_ptr<Location> loc2 = Locations[((i % length) + length) % length];
				if (getDistance(loc->C.GetPoint(loc->Time), loc2->C.GetPoint(loc2->Time)) > GEOMETRIC_EPSILON)
					break;
				if (getDistance(loc->C.GetPoint(loc->Time), loc2->C.GetPoint(loc2->Time)) <= GEOMETRIC_EPSILON &&
					loc->C.Segment1.Id == loc2->C.Segment1.Id)
					return;
			}
			for (int i = mid + 1; i <= length; i++)
			{
				std::shared_ptr<Location> loc2 = Locations[((i % length) + length) % length];
				if (getDistance(loc->C.GetPoint(loc->Time), loc2->C.GetPoint(loc2->Time)) > GEOMETRIC_EPSILON)
					break;
				if (getDistance(loc->C.GetPoint(loc->Time), loc2->C.GetPoint(loc2->Time)) <= GEOMETRIC_EPSILON &&
					loc->C.Segment1.Id == loc2->C.Segment1.Id)
					return;
			}
		}

		double	diffId = loc->C.Segment1.Id - loc1->C.Segment1.Id,
				diffT = (loc->C.Segment1.Index + loc->Time) - (loc1->C.Segment1.Index + loc1->Time);
		bool	self = loc->C.Segment1.Id == loc1->C.Segment1.Id;
		double	diff = self ? (diffT) : (diffId);
		if (diff < 0)
			r = mid - 1;
		else
			l = mid + 1;
	}
	Locations.insert(Locations.begin() + l, loc);
}

void CBooleanOperations::AddLocation(Curve curve1, Curve curve2,
									double t1, double t2, bool overlap)
{
	bool excludeStart = !overlap &&	GetPreviousCurve(curve1) == curve2,
		 excludeEnd = !overlap && curve1 != curve2 && GetNextCurve(curve1) == curve2;
	double	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;

	if (t1 >= (excludeStart ? tMin : 0) &&
		t1 <= (excludeEnd ? tMax : 1))
	{
		if (t2 >= (excludeEnd ? tMin : 0) &&
			t2 <= (excludeStart ? tMax : 1))
		{
			std::shared_ptr<Location>	loc1(new Location(curve1, t1, overlap)),
										loc2(new Location(curve2, t2, overlap));
			loc1->Inters = loc2;
			loc2->Inters = loc1;
			if (FilterIntersections(loc1))
				InsertLocation(loc1, overlap);
		}
	}
}

void CBooleanOperations::AddOffsets(std::vector<double>& offsets,
								   const Curve& curve, bool end)
{
	std::vector<double> roots = curve.GetPeeks();
	int	count = roots.size();
	bool	first = end && count != 0,
			second = !end && count != 0;
	double	offset = curve.GetLength(first ? roots[count - 1] : 0,
									second ? roots[0] : 1);
	offsets.push_back(count != 0 ? offset : offset / 32);
}

PointD CBooleanOperations::GetMinPoint(const std::vector<Segment>& segments)
{
	PointD minPoint;

	for (const auto& s : segments)
		if (s.P.X < minPoint.X || s.P.Y < minPoint.Y)
			minPoint = s.P;

	return PointD(minPoint.X - GEOMETRIC_EPSILON, minPoint.Y - GEOMETRIC_EPSILON);
}

CGraphicsPath* CalcBooleanOperation(CGraphicsPath *path1, CGraphicsPath *path2, BooleanOpType op)
{
	CBooleanOperations operation(path1, path2, op);
	return operation.GetResult();
}
}
