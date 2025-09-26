#include "BooleanOperations.h"
#include "boolean_operations_math.h"
#include <algorithm>
#include <iterator>

namespace Aggplus {
Segment::Segment()  noexcept
{
}

Segment::Segment(const std::vector<PointD>& points, const bool& isCurve,
				 const int& index, const int& id, const bool& polyClosed) noexcept :
	IsCurve(isCurve),
	PolyClosed(polyClosed),
	Index(index),
	Id(id)
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

Segment::Segment(const PointD& point, const bool& isCurve, const int& index,
				 const int& id, const bool& polyClosed) noexcept :
	P(point),
	IsCurve(isCurve),
	PolyClosed(polyClosed),
	Index(index),
	Id(id)
{
}

Segment::Segment(const PointD& p) noexcept :
	P(p)
{
}

Segment::Segment(const PointD& p, const PointD& hi, const PointD& ho)  noexcept :
	P(p),
	HI(hi),
	HO(ho)
{
	if (!hi.IsZero() || !ho.IsZero())
		IsCurve = true;
}

void Segment::SetHandles(const PointD &hi, const PointD &ho) noexcept
{
	IsCurve = true;
	HI = PointD(hi.X - P.X, hi.Y - P.Y);
	HO = PointD(ho.X - P.X, ho.Y - P.Y);
}

void Segment::UpdateHandles(const PointD &hi, const PointD &ho) noexcept
{
	HI = hi;
	HO = ho;
}

bool Segment::IsValid(const BooleanOpType& op) const noexcept
{
	if (Visited)
		return false;
	if (Inters)
		return true;
	if (op == Subtraction)
	{
		if (Id == 1 && Winding == 0)
			return true;
		else if (Id == 2 && Winding == 1)
			return true;
	}
	else if (Winding && op == Intersection || !Winding && op == Union)
		return true;
	return false;
}

bool Segment::IsEmpty() const noexcept
{
	return Id == 0 && Index == -1 && P.IsZero() && HI.IsZero() && HO.IsZero();
}

bool Segment::operator==(const Segment& other) const noexcept
{
	return (Index == other.Index) && (Id == other.Id);
}

bool Segment::operator!=(const Segment& other) const noexcept
{
	return !operator==(other);
}

Curve::Curve() noexcept
{
}

Curve::Curve(const Segment& segment1, const Segment& segment2) noexcept :
	Segment1(segment1),
	Segment2(segment2)
{
}

Curve::Curve(const std::vector<double>& values) noexcept
{
	if (values.size() == 4)
	{
		Segment1 = Segment(PointD(values[0], values[1]));
		Segment2 = Segment(PointD(values[2], values[3]));
	}
	else if (values.size() == 8)
	{
		Segment1 = Segment(PointD(values[0], values[1]));
		Segment2 = Segment(PointD(values[6], values[7]),
						   PointD(values[2] - values[6],
								  values[3] - values[7]),
						   PointD(values[4] - values[6],
								  values[5] - values[7]));
	}
}

Curve::Curve(const double& x0, const double& y0, const double& x1, const double& y1,
			 const double& x2, const double& y2, const double& x3, const double& y3) noexcept :
	Segment1(Segment(PointD(x0, y0))),
	Segment2(Segment(PointD(x3, y3), PointD(x1, y1), PointD(x2, y2)))
{
}

std::vector<double> Curve::GetXValues() const noexcept
{
	if (IsStraight()) return {Segment1.P.X, Segment1.P.X, Segment2.P.X, Segment2.P.X};
	return {Segment1.P.X, Segment2.P.X + Segment2.HI.X, Segment2.P.X + Segment2.HO.X, Segment2.P.X};
}

std::vector<double> Curve::GetYValues() const noexcept
{
	if (IsStraight()) return {Segment1.P.Y, Segment1.P.Y, Segment2.P.Y, Segment2.P.Y};
	return {Segment1.P.Y, Segment2.P.Y + Segment2.HI.Y, Segment2.P.Y + Segment2.HO.Y, Segment2.P.Y};
}

std::vector<double> Curve::GetBound() const noexcept
{
	return {min(Segment1.P.X, Segment2.P.X + Segment2.HI.X, Segment2.P.X + Segment2.HO.X, Segment2.P.X),
			min(Segment1.P.Y, Segment2.P.Y + Segment2.HI.Y, Segment2.P.Y + Segment2.HO.Y, Segment2.P.Y),
			max(Segment1.P.X, Segment2.P.X + Segment2.HI.X, Segment2.P.X + Segment2.HO.X, Segment2.P.X),
			max(Segment1.P.Y, Segment2.P.Y + Segment2.HI.Y, Segment2.P.Y + Segment2.HO.Y, Segment2.P.Y)};
}

std::vector<double> Curve::GetPeeks() const
{
	std::vector<double> x = GetXValues(),
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
		Curve c;
		double dx, dy;
		if (b < 1)
		{
			c = Subdivide(b)[0];
			dx = c.Segment2.P.X - c.Segment1.P.X;
			dy = c.Segment2.P.Y - c.Segment1.P.Y;
		}
		else if (a > 0)
		{
			c = Subdivide(a)[1];
			dx = c.Segment2.P.X - c.Segment1.P.X;
			dy = c.Segment2.P.Y - c.Segment1.P.Y;
		}
		else
		{
			dx = Segment2.P.X - Segment1.P.X;
			dy = Segment2.P.Y - Segment1.P.Y;
		}
		return sqrt(dx * dx + dy * dy);
	}

	std::vector<double> x = GetXValues(),
						y = GetYValues();

	double	ax = 9 * (x[1] - x[2]) + 3 * (x[3] - x[0]),
			bx = 6 * (x[0] + x[2]) - 12 * x[1],
			cx = 3 * (x[1] - x[0]),
			ay = 9 * (y[1] - y[2]) + 3 * (y[3] - y[0]),
			by = 6 * (y[0] + y[2]) - 12 * y[1],
			cy = 3 * (y[1] - y[0]);

	return integrate(ax, bx, cx, ay, by, cy, a, b);
}

double Curve::GetSquaredLineLength() const noexcept
{
	return (Segment2.P.X - Segment1.P.X) * (Segment2.P.X - Segment1.P.X) +
		   (Segment2.P.Y - Segment1.P.Y) * (Segment2.P.Y - Segment1.P.Y);
}

double Curve::GetTimeOf(const PointD& point) const noexcept
{
	double	d0 = getDistance(point.X, point.Y, Segment1.P.X, Segment1.P.Y),
			d3 = getDistance(point.X, point.Y, Segment2.P.X, Segment2.P.Y);

	if (d0 > EPSILON && d3 > EPSILON)
	{
		std::vector<double> roots;
		for (int c = 0; c < 2; c++)
		{
			int count = SolveCubic(c, c == 0 ? point.X : point.Y, roots, 0.0, 1.0);
			for (int i = 0; i < count; i++)
			{
				double u = roots[i];
				if (u > 1.0 || u < 0.0)
					return - 1.0;
				if (getDistance(point, GetPoint(u)) <= GEOMETRIC_EPSILON)
					return u;
			}
			roots.clear();
		}
	}
	bool firstDist = d0 <= GEOMETRIC_EPSILON,
		 secondDist = d3 <= GEOMETRIC_EPSILON;
	return firstDist ? 0.0 : secondDist ? 1.0 : - 1.0;
}

double Curve::GetTimeAt(const double& offset) const noexcept
{
	bool   forward = offset > 0;
	double start = !forward ? 1 : 0;
	if (offset == 0)
		return start;

	double	a = forward ? start : 0,
			b = forward ? 1 : start,
			rangeLength = GetLength(a, b),
			diff = fabs(offset) - rangeLength;

	if (fabs(diff) < EPSILON)
		return forward ? b : a;
	else if (diff > EPSILON)
		return MIN;

	double	guess = offset / rangeLength,
			length = 0.0;

	std::vector<double> x = GetXValues(),
						y = GetYValues();

	double	ax = 9 * (x[1] - x[2]) + 3 * (x[3] - x[0]),
			bx = 6 * (x[0] + x[2]) - 12 * x[1],
			cx = 3 * (x[1] - x[0]),
			ay = 9 * (y[1] - y[2]) + 3 * (y[3] - y[0]),
			by = 6 * (y[0] + y[2]) - 12 * y[1],
			cy = 3 * (y[1] - y[0]);

	double vx = start + guess;
	for (size_t i = 0; i < 32; i++)
	{
		length += integrate(ax, bx, cx, ay, by, cy, start, vx, getIterations(start, vx));
		start = vx;

		double dx = vx - (length - offset) / CurveLength(vx, ax, bx, cx, ay, by, cy);
		if (fabs(vx - dx) < EPSILON)
		{
			vx = dx;
			break;
		}

		if (length - offset > 0)
		{
			b = vx;
			vx = dx <= a ? (a + b) * 0.5 : dx;
		}
		else{
			a = vx;
			vx = dx >= b ? (a + b) * 0.5 : dx;
		}
	}

	return clamp(vx, a, b);
}

PointD Curve::Get(const double& t, const int& type) const noexcept
{
	std::vector<double> x = GetXValues(),
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

PointD Curve::GetPoint(const double& t) const noexcept
{
	return Get(t, 0);
}

PointD Curve::GetTangent(const double& t) const noexcept
{
	return Get(t, 1);
}

PointD Curve::GetTangent(const double& t, const double& offset,
						 const bool& inside, const PointD& p) const noexcept
{
	if (inside) return GetTangent(t);
	return GetPoint(GetTimeAt(offset)) - p;
}

Curve Curve::GetPart(double from, double to) const noexcept
{
	Curve result = *this;
	if (from > to)
		std::swap(from, to);

	if (from > 0)
	{
		result = Subdivide(from)[1];
		result.Segment2.SetHandles(result.Segment2.HI, result.Segment2.HO);
	}
	if (to < 1)
	{
		result = result.Subdivide((to - from) / (1 - from))[0];
		result.Segment2.SetHandles(result.Segment2.HI, result.Segment2.HO);
	}

	if (from > to)
		result.Flip();

	return result;
}

std::vector<std::pair<double, double>> Curve::GetOverlaps(const Curve& curve) const noexcept
{
	bool swap = GetSquaredLineLength() < curve.GetSquaredLineLength(),
		 straight1 = IsStraight(),
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

	double	d20 = fabs(getSignedDistance(px, py, vx, vy, x2[0], y2[0], true)),
			d23 = fabs(getSignedDistance(px, py, vx, vy, x2[3], y2[3], true));

	if (d20 < GEOMETRIC_EPSILON && d23 < GEOMETRIC_EPSILON)
	{
		double	d11 = fabs(getSignedDistance(px, py, vx, vy, x1[1], y1[1], true)),
				d12 = fabs(getSignedDistance(px, py, vx, vy, x1[2], y1[2], true)),
				d21 = fabs(getSignedDistance(px, py, vx, vy, x2[1], y2[1], true)),
				d22 = fabs(getSignedDistance(px, py, vx, vy, x2[2], y2[2], true));

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
		return std::vector<std::pair<double, double>>();
	}
	if (straight1 ^ straight2)
	{
		return std::vector<std::pair<double, double>>();
	}

	std::vector<std::pair<double, double>> pairs;
	for (int i = 0; i < 4 && pairs.size() < 2; i++)
	{
		int i1 = i & 1,
			t1 = i >> 1;
		double t2 = i1 == 0	? GetTimeOf(t1 == 1	? curve.Segment2.P : curve.Segment1.P)
							: curve.GetTimeOf(t1 == 1 ? Segment2.P : Segment1.P);
		if (t2 != -1)
		{
			std::pair<double, double> pair = i1 == 1 ? std::pair<double, double>(t1, t2)
													 : std::pair<double, double>(t2, t1);
			if (pairs.empty())
				pairs.push_back(pair);
			else if (fabs(pair.first - pairs[0].first) > TIME_EPSILON &&
					 fabs(pair.second - pairs[0].second) > TIME_EPSILON)
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

		double	dox = fabs(o2.Segment2.HO.X - o1.Segment2.HO.X),
				doy = fabs(o2.Segment2.HO.Y - o1.Segment2.HO.Y),
				dix = fabs(o2.Segment2.HI.X - o1.Segment2.HI.X),
				diy = fabs(o2.Segment2.HI.Y - o1.Segment2.HI.Y);

		if (dox > GEOMETRIC_EPSILON ||
			doy > GEOMETRIC_EPSILON ||
			dix > GEOMETRIC_EPSILON ||
			diy > GEOMETRIC_EPSILON)
			pairs.clear();
	}

	return pairs;
}

std::vector<double> Curve::GetCurveLineIntersection(const double& px, const double& py,
													const double& vx, const double& vy) const noexcept
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
		double	cx = x[i] - px,
				cy = y[i] - py;

		rv.push_back(cx * cos - cy * sin);
		rv.push_back(cx * sin + cy * cos);
	}

	Curve c(rv);
	c.SolveCubic(1, 0, roots, 0, 1);

	return roots;
}

std::vector<Curve> Curve::Subdivide(const double& t) const noexcept
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

Curve Curve::DivideAtTime(const double& time) noexcept
{
	double	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;

	if (time >= tMin && time <= tMax)
	{
		std::vector<Curve> parts = Subdivide(time);
		Segment segment(parts[0].Segment2.P,
						false,
						Segment1.Index + 1,
						Segment1.Id,
						Segment1.PolyClosed);

		if (!IsStraight())
		{
			segment.SetHandles(parts[0].Segment2.HI, parts[0].Segment2.HO);
			Segment2.SetHandles(parts[1].Segment2.HI, parts[1].Segment2.HO);
		}

		return Curve(segment, Segment2);
	}

	return Curve();
}

int Curve::SolveCubic(const int& coord, const double& value, std::vector<double>& roots,
					  const double& mn, const double& mx) const noexcept
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
					  std::vector<double>& roots, const double& mn, const double& mx) const noexcept
{
	int count = 0;
	double f = max(fabs(a), fabs(b), fabs(c), fabs(d));
	double x, b1, c2, qd, q;
	if (f != 0 && (f < 1e-8 || f > 1e8))
	{
		f = pow(2, -round(log2(f)));
		a *= f;
		b *= f;
		c *= f;
		d *= f;
	}

	if (fabs(a) < EPSILON)
	{
		a = b;
		b1 = c;
		c2 = d;
		x = MAX;
	}
	else if (fabs(d) <EPSILON)
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

		double	t = q / a,
				r = pow(fabs(t), 1.0 / 3.0),
				s = t < 0 ? -1 : 1,
				td = -qd / a,
				rd = td > 0 ? 1.324717957244746 * std::max(r, std::sqrt(td)) : r,
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

			if (fabs(a) * x * x > fabs(d / x))
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
	if (x != MAX && (count == 0 || xInRoots1 || xInRoots2) && (xInEps))
	{
		roots.push_back(clamp(x, mn, mx));
		count++;
	}

	return count;
}

void Curve::Flip() noexcept
{
	PointD tmpHI = Segment2.P + Segment2.HI;
	PointD tmpHO = Segment2.P + Segment2.HO;
	std::swap(Segment1.P, Segment2.P);
	Segment2.SetHandles(tmpHO, tmpHI);
}

bool Curve::IsStraight() const noexcept
{
	return !Segment2.IsCurve;
}

bool Curve::operator==(const Curve& other) const noexcept
{
	return Segment1 == other.Segment1 &&
		   Segment2 == other.Segment2;
}

bool Curve::operator!=(const Curve& other) const noexcept
{
	return !operator ==(other);
}

Location::Location() noexcept
{
}

Location::Location(const Curve& curve, const double& time, const bool& overlap, const bool& ends) noexcept :
	C(curve),
	Time(time),
	Overlap(overlap),
	Ends(ends)
{
}

bool Location::IsTouching() noexcept
{
	if (!Inters)
		return false;
	if (!isCollinear(C.GetTangent(Time), Inters->C.GetTangent(Inters->Time)))
		return false;

	PointD pt;
	bool straight = C.IsStraight() && Inters->C.IsStraight();

	return !straight || !intersect({C.Segment1.P.X,
									C.Segment1.P.Y,
									C.Segment2.P.X,
									C.Segment2.P.Y,
									Inters->C.Segment1.P.X,
									Inters->C.Segment1.P.Y,
									Inters->C.Segment2.P.X,
									Inters->C.Segment2.P.Y}, pt);
}

CBooleanOperations::CBooleanOperations(const CGraphicsPath& path1,
									   const CGraphicsPath& path2,
									   BooleanOpType op,
									   long fillType,
									   bool isLuminosity) :
	Op(op),
	Close1(path1.Is_poly_closed()),
	Close2(path2.Is_poly_closed()),
	IsLuminosity(isLuminosity),
	FillType(fillType),
	Path1(path1),
	Path2(path2)
{
	TraceBoolean();
}

CBooleanOperations::~CBooleanOperations()
{
}

CGraphicsPath&& CBooleanOperations::GetResult()
{
	return std::move(Result);
}

void CBooleanOperations::TraceBoolean()
{
	bool reverse = false;
	if ((Op == Subtraction || Op == Exclusion) ^
		Path1.IsClockwise() ^
		Path2.IsClockwise())
		reverse = true;

	PreparePath(Path1, 1, Segments1, Curves1);
	PreparePath(Path2, 2, Segments2, Curves2, reverse);

	OriginCurves1 = Curves1;
	OriginCurves2 = Curves2;

	GetIntersection();

	if (!Locations.empty())
	{
		int length = static_cast<int>(Locations.size());
		for (int i = 0; i < length; i++)
		{
			int before = static_cast<int>(Locations.size());
			InsertLocation(Locations[i]->Inters, Locations[i]->Inters->Overlap);
			if (before == Locations.size())
			{
				Locations.erase(Locations.begin() + i);
				length--;
				i--;
			}
		}

		DivideLocations();

		if (Locations.size() == 2 && Locations[0]->Ends)
		{
			SetWinding();
			TraceOneInters();
			return;
		}
		if (AllOverlap())
		{
			TraceAllOverlap();
			return;
		}
		if (IsOneCurvePath(1))
		{
			TraceOneCurvePath1();
			return;
		}
		if (IsOneCurvePath(2))
		{
			TraceOneCurvePath2();
			return;
		}
	}

	SetWinding();
	TracePaths();
}

void CBooleanOperations::TraceOneInters()
{
	Segment s1, s2;

	for (const auto& s : Segments1)
		if (!s.Inters)
			s1 = s;

	for (const auto& s : Segments2)
		if (!s.Inters)
			s2 = s;

	if (Op == Intersection)
	{
		if (s1.Winding == 1)
			Result = std::move(Path1);
		else if (s2.Winding == 1)
			Result = std::move(Path2);
		else
		{
			Result.StartFigure();
			Result.MoveTo(Locations[0]->S.P.X, Locations[0]->S.P.Y);
			Result.CloseFigure();
		}
	}
	else
	{
		if (Op == Union && s1.Winding == 1)
			Result = std::move(Path2);
		else if (Op == Union && s2.Winding == 1)
			Result = std::move(Path1);
		else if (s1.Winding == 0 && s2.Winding == 0 && Op == Subtraction)
			Result = std::move(Path1);
		else
		{
			bool start = true;
			for (const auto& s : (Op == Union || s2.Winding == 1) ? Segments1 : Segments2)
			{
				if (!s.P.Equals(Locations[(Op == Union || s2.Winding == 1) ? 0 : 1]->S.P))
				{
					if (start)
					{
						Result.StartFigure();
						Result.MoveTo(s.P.X, s.P.Y);
						start = false;
					}
					else if (s.IsCurve)
						Result.CurveTo(s.HI.X + s.P.X, s.HI.Y + s.P.Y,
										s.HO.X + s.P.X, s.HO.Y + s.P.Y,
										s.P.X, s.P.Y);
					else
						Result.LineTo(s.P.X, s.P.Y);
				}
				else
				{
					if (s.IsCurve)
						Result.CurveTo(s.HI.X + s.P.X, s.HI.Y + s.P.Y,
										s.HO.X + s.P.X, s.HO.Y + s.P.Y,
										s.P.X, s.P.Y);
					else
						Result.LineTo(s.P.X, s.P.Y);

					Segment seg = GetNextSegment(Locations[1]->S);
					while (!seg.IsEmpty() && seg != Locations[(Op == Union || s2.Winding == 1) ? 1 : 0]->S)
					{
						if (seg.IsCurve)
							Result.CurveTo(seg.HI.X + seg.P.X, seg.HI.Y + seg.P.Y,
											seg.HO.X + seg.P.X, seg.HO.Y + seg.P.Y,
											seg.P.X, seg.P.Y);
						else
							Result.LineTo(seg.P.X, seg.P.Y);
						seg = GetNextSegment(seg);
					}

					if (seg.IsCurve)
						Result.CurveTo(seg.HI.X + seg.P.X, seg.HI.Y + seg.P.Y,
										seg.HO.X + seg.P.X, seg.HO.Y + seg.P.Y,
										seg.P.X, seg.P.Y);
					else
						Result.LineTo(seg.P.X, seg.P.Y);
				}
			}

			if (Close1 && Close2)
			{
				Segment firstS = (Op == Union || s2.Winding == 1) ? Segments1[0] : Segments2[0];

				if (firstS.IsCurve)
					Result.CurveTo(firstS.HI.X + firstS.P.X, firstS.HI.Y + firstS.P.Y,
								   firstS.HO.X + firstS.P.X, firstS.HO.Y + firstS.P.Y,
								   firstS.P.X, firstS.P.Y);
				else
					Result.LineTo(firstS.P.X, firstS.P.Y);

				Result.CloseFigure();
			}
		}
	}
}

void CBooleanOperations::TraceAllOverlap()
{
	if (AllInters(Segments1) && AllInters(Segments2))
	{
		if (Op != Subtraction)
			Result = std::move(Path1);
	}
	else if (AllInters(Segments1))
	{
		if (Op == Intersection)
			Result = std::move(Path1);
		else if (Op == Union)
			Result = std::move(Path2);
	}
	else if (AllInters(Segments2))
	{
		if (Op == Intersection)
			Result = std::move(Path2);
		else if (Op == Union)
			Result = std::move(Path1);
	}
	else
	{
		bool winding1{false},
			 winding2{false};
		for (const auto& s : Segments1)
		{
			if (!s.Inters)
			{
				winding1 = IsInside(s);
				break;
			}
		}

		for (const auto& s : Segments2)
		{
			if (!s.Inters)
			{
				winding2 = IsInside(s);
				break;
			}
		}

		if (Op == Intersection)
		{
			SetWinding();
			TracePaths();
		}
		else if (Op == Union)
		{
			if (!winding1 && !winding2)
				TracePaths();
			else if (!winding1)
				Result = std::move(Path1);
			else
				Result = std::move(Path2);
		}
		else if (!winding1 && !winding2)
			Result = std::move(Path1);
		else
		{
			Result.StartFigure();
			for (const auto& seg : !winding1 ? Segments1 : Segments2)
			{
				if (!seg.Inters && !seg.Visited)
				{
					Result.MoveTo(seg.P.X, seg.P.Y);
					SetVisited(seg);

					Segment s = GetNextSegment(seg);
					while (!s.IsEmpty() && s != seg)
					{
						if (s.IsCurve)
							Result.CurveTo(s.HI.X + s.P.X, s.HI.Y + s.P.Y,
											s.HO.X + s.P.X, s.HO.Y + s.P.Y,
											s.P.X, s.P.Y);
						else
							Result.LineTo(s.P.X, s.P.Y);

						SetVisited(s);

						if (s.Inters)
							s = GetNextSegment(s.Inters->S);
						else
							s = GetNextSegment(s);
					}

					if (s.IsCurve)
						Result.CurveTo(s.HI.X + s.P.X, s.HI.Y + s.P.Y,
										s.HO.X + s.P.X, s.HO.Y + s.P.Y,
										s.P.X, s.P.Y);
					else
						Result.LineTo(s.P.X, s.P.Y);
				}
			}
			if (Close1 && Close2) Result.CloseFigure();
		}
	}
}

void CBooleanOperations::TraceOneCurvePath1()
{
	if (Op == Intersection)
	{
		Result.StartFigure();
		for (const auto& s : Segments1)
		{
			if (s.Inters && Result.GetPointCount() == 0)
			{
				Result.MoveTo(s.P.X, s.P.Y);
				Segment s2 = GetNextSegment(s);
				if (s2.IsCurve)
					Result.CurveTo(s2.HI.X + s2.P.X, s2.HI.Y + s2.P.Y,
								   s2.HO.X + s2.P.X, s2.HO.Y + s2.P.Y,
								   s2.P.X, s2.P.Y);
				else
					Result.LineTo(s2.P.X, s2.P.Y);
				break;
			}
		}
	}
	else if (Op == Union)
	{
		Result.AddPath(Path2);
		for (size_t i = 0; i < Segments1.size(); i++)
		{
			if (i % 2 == 0)
				Result.MoveTo(Segments1[i].P.X, Segments1[i].P.Y);
			else if (Segments1[i].IsCurve)
				Result.CurveTo(Segments1[i].HI.X + Segments1[i].P.X, Segments1[i].HI.Y + Segments1[i].P.Y,
							   Segments1[i].HO.X + Segments1[i].P.X, Segments1[i].HO.Y + Segments1[i].P.Y,
							   Segments1[i].P.X, Segments1[i].P.Y);
			else
				Result.LineTo(Segments1[i].P.X, Segments1[i].P.Y);
		}
	}
	else
	{
		Result.StartFigure();

		for (size_t i = 0; i < Segments1.size(); i++)
		{
			if (i % 2 == 0)
				Result.MoveTo(Segments1[i].P.X, Segments1[i].P.Y);
			else if (Segments1[i].IsCurve)
				Result.CurveTo(Segments1[i].HI.X + Segments1[i].P.X, Segments1[i].HI.Y + Segments1[i].P.Y,
							   Segments1[i].HO.X + Segments1[i].P.X, Segments1[i].HO.Y + Segments1[i].P.Y,
							   Segments1[i].P.X, Segments1[i].P.Y);
			else
				Result.LineTo(Segments1[i].P.X, Segments1[i].P.Y);
		}
	}
}

void CBooleanOperations::TraceOneCurvePath2()
{
	if (Op == Intersection)
	{
		Result.StartFigure();
		for (const auto& s : Segments2)
		{
			if (s.Inters && Result.GetPointCount() == 0)
			{
				Result.MoveTo(s.P.X, s.P.Y);
				Segment s2 = GetNextSegment(s);
				if (s2.IsCurve)
					Result.CurveTo(s2.HI.X + s2.P.X, s2.HI.Y + s2.P.Y,
								   s2.HO.X + s2.P.X, s2.HO.Y + s2.P.Y,
								   s2.P.X, s2.P.Y);
				else
					Result.LineTo(s2.P.X, s2.P.Y);
				break;
			}
		}
	}
	else if (Op == Union)
	{
		Result.AddPath(Path1);
		for (size_t i = 0; i < Segments2.size(); i++)
		{
			if (i % 2 == 0)
				Result.MoveTo(Segments2[i].P.X, Segments2[i].P.Y);
			else if (Segments2[i].IsCurve)
				Result.CurveTo(Segments2[i].HI.X + Segments2[i].P.X, Segments2[i].HI.Y + Segments2[i].P.Y,
							   Segments2[i].HO.X + Segments2[i].P.X, Segments2[i].HO.Y + Segments2[i].P.Y,
							   Segments2[i].P.X, Segments2[i].P.Y);
			else
				Result.LineTo(Segments2[i].P.X, Segments2[i].P.Y);
		}
	}
	else
		Result = std::move(Path1);
}

void CBooleanOperations::TracePaths()
{
	size_t length = Segments1.size();
	Result.StartFigure();
	for (size_t i = 0; i < length + Segments2.size(); i++)
	{
		Segment s = i >= length ? Segments2[i - length] : Segments1[i];
		bool valid = s.IsValid(Op),
			 start = true;
		while (valid)
		{
			if (!start || (Op == Intersection && s.Inters && !GetNextSegment(s).Inters))
				SetVisited(s);

			if (start)
				Result.MoveTo(s.P.X, s.P.Y);
			else if (s.IsCurve)
				Result.CurveTo(s.HI.X + s.P.X, s.HI.Y + s.P.Y,
							   s.HO.X + s.P.X, s.HO.Y + s.P.Y,
							   s.P.X, s.P.Y);
			else
				Result.LineTo(s.P.X, s.P.Y);

			if (!start && valid && s.Inters)
				SetVisited(s.Inters->S);

			Segment prev = s;
			s = GetNextSegment(s);
			if (s.IsEmpty()) break;
			valid = s.IsValid(Op);

			if (s.Inters && prev.Inters && (s.Inters->S.Index - prev.Inters->S.Index) != 0)
			{
				Segment tmp = GetNextSegment(prev.Inters->S);
				if (tmp.IsEmpty()) break;
				if (tmp.IsValid(Op))
					s = tmp;
			}

			if (!valid && prev.Inters)
			{
				s = GetNextSegment(prev.Inters->S);
				if (!start || Op != Intersection)
					SetVisited(prev.Inters->S);
				valid = s.IsValid(Op);
			}

			if (start)
				start = false;
		}

		if (!start && AllOverlap()) break;
	}

	if (Close1 && Close2) Result.CloseFigure();
}

void CBooleanOperations::PreparePath(const CGraphicsPath& path, int id,
									 std::vector<Segment>& segments,
									 std::vector<Curve>& curves,
									 bool reverse)
{
	bool isPolyClosed = path.Is_poly_closed();
	int length = path.GetPointCount();
	int idx = 0;
	if (reverse)
	{
		bool isCurve = false;
		for (int i = length - 1; i >= 0; i--)
		{
			std::vector<PointD> points = path.GetPoints(isCurve ? i - 2 : i, isCurve ? 3 : 1);
			if (isCurve) std::reverse(points.begin(), points.end());
			if (segments.empty() || getDistance(segments[segments.size() - 1].P, isCurve ? points[2] : points[0]) > POINT_EPSILON)
			{
				if (!segments.empty() && getDistance(segments[0].P, isCurve ? points[2] : points[0]) <= POINT_EPSILON)
				{
					if (isCurve && segments[0].HI.IsZero() && segments[0].HO.IsZero())
						segments[0].SetHandles(points[0], points[1]);
				}
				else
					segments.push_back(Segment(points, isCurve, idx++, id, isPolyClosed));
			}

			if (isCurve) i -= 2;
			isCurve = path.IsCurvePoint(i);
		}
	}
	else
	{
		for (int i = 0; i < length; i++)
		{
			bool isCurve = path.IsCurvePoint(i);
			std::vector<PointD> points = path.GetPoints(i, isCurve ? 3 : 1);
			if (segments.empty() || getDistance(segments[segments.size() - 1].P, isCurve ? points[2] : points[0]) > POINT_EPSILON)
			{
				if (!segments.empty() && getDistance(segments[0].P, isCurve ? points[2] : points[0]) <= POINT_EPSILON)
				{
					if (isCurve && segments[0].HI.IsZero() && segments[0].HO.IsZero())
						segments[0].SetHandles(points[0], points[1]);
				}
				else
					segments.push_back(Segment(points, isCurve, idx++, id, isPolyClosed));
			}

			if (isCurve) i += 2;
		}
	}

	length = isPolyClosed ? static_cast<int>(segments.size()) : static_cast<int>(segments.size()) - 1;

	for (int i = 0; i < length; i++)
		curves.push_back(Curve(segments[i], isPolyClosed && (i == (length - 1)) ? segments[0] : segments[i + 1]));
}

void CBooleanOperations::InsertSegment(Segment& segment, const Segment& handles, bool updateHandles)
{
	if (segment.Id == 1)
	{
		int length = static_cast<int>(Segments1.size()),
			index = segment.Index == length ? 0 : segment.Index;
		if (getDistance(segment.P, Segments1[index].P) <= GEOMETRIC_EPSILON)
		{
			segment.Index = index;
			Segments1[index] = segment;
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
		int length = static_cast<int>(Segments2.size()),
			index = segment.Index == length ? 0 : segment.Index;
		if (getDistance(segment.P, Segments2[index].P) <= GEOMETRIC_EPSILON)
		{
			segment.Index = index;
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

Curve CBooleanOperations::GetCurve(const Segment& segment) const noexcept
{
	return segment.Id == 1 ? Curves1[segment.Index] : Curves2[segment.Index];
}

Curve CBooleanOperations::GetPreviousCurve(const Curve& curve) const noexcept
{
	bool path1 = curve.Segment1.Id == 1;
	size_t last = path1 ? Curves1.size() - 1
						: Curves2.size() - 1;

	if (curve.Segment1.Index == 0)
	{
		if (curve.Segment1.PolyClosed)
			return path1 ? Curves1[last] : Curves2[last];
		else
			return Curve();
	}

	return path1 ? Curves1[curve.Segment1.Index - 1]
				 : Curves2[curve.Segment1.Index - 1];
}

Curve CBooleanOperations::GetNextCurve(const Curve& curve) const noexcept
{
	bool path1 = curve.Segment1.Id == 1;
	int last = path1 ? static_cast<int>(Curves1.size()) - 1
					 : static_cast<int>(Curves2.size()) - 1;

	if (curve.Segment1.Index == last)
	{
		if (curve.Segment1.PolyClosed)
			return path1 ? Curves1[0] : Curves2[0];
		else
			return Curve();
	}

	return path1 ? Curves1[curve.Segment1.Index + 1]
				 : Curves2[curve.Segment1.Index + 1];
}

Segment CBooleanOperations::GetPreviousSegment(const Segment& segment) const noexcept
{
	if (segment.Index == 0)
		return segment.Id == 1 ? Segments1[Segments1.size() - 1] : Segments2[Segments2.size() - 1];
	else
		return segment.Id == 1 ? Segments1[segment.Index - 1] : Segments2[segment.Index - 1];
}

Segment CBooleanOperations::GetNextSegment(const Segment& segment) const noexcept
{
	bool path1 = segment.Id == 1;
	int last = path1 ? static_cast<int>(Segments1.size()) - 1
					 : static_cast<int>(Segments2.size()) - 1;

	if (segment.Index == last)
	{
		if (segment.PolyClosed)
			return path1 ? Segments1[0] : Segments2[0];
		else
			return Segment();
	}

	return path1 ? Segments1[segment.Index + 1]
				 : Segments2[segment.Index + 1];
}

void CBooleanOperations::SetVisited(const Segment& segment)
{
	if (segment.Id == 1)
		Segments1[segment.Index].Visited = true;
	else
		Segments2[segment.Index].Visited = true;
}

std::vector<std::vector<int>> CBooleanOperations::FindBoundsCollisions()
{
	std::vector<std::vector<double>> allBounds, bounds2;
	for (const auto& c : Curves1)
		allBounds.push_back(c.GetBound());
	for (const auto& c : Curves2)
		bounds2.push_back(c.GetBound());

	bool self = allBounds == bounds2;

	if (!self)
		for (auto it = bounds2.begin(); it != bounds2.end(); ++it)
			allBounds.push_back(*it);

	int	allLength = static_cast<int>(allBounds.size()),
		length1	  = static_cast<int>(Curves1.size());

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
		std::vector<int>	curCollisions;
		bool isCurrent1 = allIdicesByPri1[i] < length1,
			 isCurrent2 = self || !isCurrent1;
		int  origIndex = self ? allIdicesByPri1[i] : allIdicesByPri1[i] - length1;

		if (!activeIndicesByPri2.empty())
		{
			int pruneCount = binarySearch(allBounds, activeIndicesByPri2, 2,
										  allBounds[allIdicesByPri1[i]][0] - GEOMETRIC_EPSILON) + 1;

			activeIndicesByPri2.erase(activeIndicesByPri2.begin(),
									  activeIndicesByPri2.begin() + pruneCount);
			for (int j = 0; j < static_cast<int>(activeIndicesByPri2.size()); j++)
			{
				bool isActive1 = activeIndicesByPri2[j] < length1,
					 isActive2 = self || !isActive1,
					 isActive1Or2 = (isCurrent1 && isActive2) || (isCurrent2 && isActive1),
					 inRange1 = allBounds[allIdicesByPri1[i]][1] <= allBounds[activeIndicesByPri2[j]][3] + GEOMETRIC_EPSILON,
					 inRange2 = allBounds[allIdicesByPri1[i]][3] >= allBounds[activeIndicesByPri2[j]][1] - GEOMETRIC_EPSILON;

				if (isActive1Or2 && (inRange2 && inRange1))
				{
					if (isCurrent1 && isActive2)
						curCollisions.push_back(self ? activeIndicesByPri2[j] : activeIndicesByPri2[j] - length1);
					if (isCurrent2 && isActive1)
						allCollisions[activeIndicesByPri2[j]].push_back(origIndex);
				}
			}
		}
		if (isCurrent1)
		{
			if (self) curCollisions.push_back(allIdicesByPri1[i]);
			allCollisions[allIdicesByPri1[i]] = curCollisions;
		}
		if (activeIndicesByPri2.size() > 0)
		{
			int index = 1 + binarySearch(allBounds, activeIndicesByPri2, 2, allBounds[allIdicesByPri1[i]][2]);
			activeIndicesByPri2.insert(activeIndicesByPri2.begin() + index, allIdicesByPri1[i]);
		}
		else
			activeIndicesByPri2.push_back(allIdicesByPri1[i]);
	}

	for (auto& c : allCollisions)
		std::sort(c.begin(), c.end());

	return allCollisions;
}

bool CBooleanOperations::IsCrossing(std::shared_ptr<Location> loc) noexcept
{
	if(!loc->Inters)
		return false;

	double	t1 = loc->Time,
			t2 = loc->Inters->Time,
			tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;

	bool t1Inside = t1 >= tMin && t1 <= tMax,
		 t2Inside = t2 >= tMin && t2 <= tMax;

	if (t1Inside && t2Inside)
		return !loc->IsTouching();

	Curve c2 = loc->C,
		  c1 = t1 < tMin ? GetPreviousCurve(c2) : c2,
		  c4 = loc->Inters->C,
		  c3 = t2 < tMin ? GetPreviousCurve(c4) : c4;

	if (t1 > tMax) c2 = GetNextCurve(c2);
	if (t2 > tMax) c4 = GetNextCurve(c4);

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
	double	offset = MAX;
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

	bool inRange34 = isInRange(a1, a3, a4) ^ isInRange(a2, a3, a4),
		 inRange43 = isInRange(a1, a4, a3) ^ isInRange(a2, a4, a3),
		 inRange1 = inRange34 && inRange43,
		 inRange12 = isInRange(a3, a1, a2) ^ isInRange(a4, a1, a2),
		 inRange21 = isInRange(a3, a2, a1) ^ isInRange(a4, a2, a1),
		 inRange2 = inRange21 && inRange12;

	return t1Inside ? inRange1 : inRange2;
}

bool CBooleanOperations::IntersectsBounds() noexcept
{
	RectF_T<double> rect1, rect2;
	Path1.GetBounds(rect1.X, rect1.Y, rect1.Width, rect1.Height);
	Path2.GetBounds(rect2.X, rect2.Y, rect2.Width, rect2.Height);

	return	(rect2.X + rect2.Width > rect1.X - EPSILON) &&
			(rect2.Y + rect2.Height > rect1.Y - EPSILON) &&
			(rect2.X < rect1.X + rect1.Width + EPSILON) &&
			(rect2.Y < rect1.Y + rect1.Height + EPSILON);
}

void CBooleanOperations::GetIntersection()
{
	if (!IntersectsBounds()) return;

	std::vector<std::vector<int>> boundsCollisions = FindBoundsCollisions();
	for (int index1 = 0; index1 < Curves1.size(); index1++)
		for (int j = 0; j < boundsCollisions[index1].size(); j++)
			GetCurveIntersection(Curves1[index1], Curves2[boundsCollisions[index1][j]]);
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

	if (maxX1 + GEOMETRIC_EPSILON > minX2 && minX1 - GEOMETRIC_EPSILON < maxX2 &&
		maxY1 + GEOMETRIC_EPSILON > minY2 && minY1 - GEOMETRIC_EPSILON < maxY2)
	{
		std::vector<std::pair<double, double>> overlaps = curve1.GetOverlaps(curve2);
		if (!overlaps.empty())
			for (int i = 0; i < 2; i++)
				AddLocation(curve1, curve2, overlaps[i].first, overlaps[i].second, true);
		else
		{
			bool	straight1 = curve1.IsStraight(),
					straight2 = curve2.IsStraight(),
					straight = straight1 && straight2,
					flip = straight1 && !straight2;
			size_t	before = Locations.size();
			if (straight)
				AddLineIntersection(flip ? curve2 : curve1, flip ? curve1 : curve2);
			else if (straight1 || straight2)
				AddCurveLineIntersection(flip ? curve2 : curve1, flip ? curve1 : curve2, flip);
			else
				AddCurveIntersection(flip ? curve2 : curve1, flip ? curve1 : curve2,
									 flip ? curve2 : curve1, flip ? curve1 : curve2, flip);

			if (Locations.size() == before && (!straight || Locations.empty()))
			{
				double t = curve2.GetTimeOf(curve1.Segment1.P);
				if (t != -1.0)
					AddLocation(curve1, curve2, 0.0, t, Locations.empty(), false, true);
				t = curve2.GetTimeOf(curve1.Segment2.P);
				if (t != -1.0)
					AddLocation(curve1, curve2, 1.0, t, Locations.empty(), false, true);
				t = curve1.GetTimeOf(curve2.Segment1.P);
				if (t != -1.0)
					AddLocation(curve1, curve2, t, 0.0, Locations.empty(), false, true);
				t = curve1.GetTimeOf(curve2.Segment2.P);
				if (t != -1.0)
					AddLocation(curve1, curve2, t, 1.0, Locations.empty(), false, true);
			}
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

	while (from->Next && from->Next != to)
		from = from->Next;

	if (!from->Next)
	{
		while (to->Prev)
			to = to->Prev;
		from->Next = to;
		to->Prev = from;
	}
}

void CBooleanOperations::AddLineIntersection(const Curve& curve1, const Curve& curve2)
{
	PointD pt;
	if (intersect({curve1.Segment1.P.X, curve1.Segment1.P.Y, curve1.Segment2.P.X, curve1.Segment2.P.Y,
				   curve2.Segment1.P.X, curve2.Segment1.P.Y, curve2.Segment2.P.X, curve2.Segment2.P.Y}, pt))
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

int CBooleanOperations::AddCurveIntersection(const Curve& curve1, const Curve& curve2, const Curve& startCurve1,
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

	double	d1 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x2[1], y2[1], false),
			d2 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x2[2], y2[2], false),
			factor = (d1 * d2) > 0.0 ? 3.0 / 4.0 : 4.0 / 9.0,
			dMin = factor * min(0.0, d1, d2),
			dMax = factor * max(0.0, d1, d2),
			dp0 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[0], y1[0], false),
			dp1 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[1], y1[1], false),
			dp2 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[2], y1[2], false),
			dp3 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[3], y1[3], false);

	std::vector<PointD> top;
	std::vector<PointD> bottom;
	getConvexHull(dp0, dp1, dp2, dp3, top, bottom);

	double tMinClip = clipConvexHull(top, bottom, dMin, dMax);

	std::reverse(top.begin(), top.end());
	std::reverse(bottom.begin(), bottom.end());

	double tMaxClip = clipConvexHull(top, bottom, dMin, dMax);

	if ((d1 == 0 && d2 == 0 && dp0 == 0 && dp1 == 0 && dp2 == 0 && dp3 == 0)
		|| tMinClip == MIN || tMaxClip == MIN)
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
		Curve newCurve1 = curve1.GetPart(tMinClip, tMaxClip);
		double uDiff = uMax - uMin;

		if (tMaxClip - tMinClip > 0.8)
		{
			if (tMaxNew - tMinNew > uDiff)
			{
				std::vector<Curve> parts = newCurve1.Subdivide(0.5);
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

				calls = AddCurveIntersection(parts[0], newCurve1, startCurve2, startCurve1,
											 !flip, recursion, calls, uMin, u, tMinNew, tMaxNew);
				calls = AddCurveIntersection(parts[1], newCurve1, startCurve2, startCurve1,
											 !flip, recursion, calls, u, uMax, tMinNew, tMaxNew);
			}
		}
		else
		{
			if (uDiff == 0 || uDiff >= LINE_EPSILON)
				calls = AddCurveIntersection(curve2, newCurve1, startCurve2, startCurve1,
											 !flip, recursion, calls, uMin, uMax, tMinNew, tMaxNew);
			else
				calls = AddCurveIntersection(newCurve1, curve2, startCurve1, startCurve2,
											 flip, recursion, calls, tMinNew, tMaxNew, uMin, uMax);
		}
	}

	return calls;
}

int CBooleanOperations::CheckInters(const PointD& point, const Segment& segment, const Curve& curve, int& touchCount) const
{
	PointD pt{};
	if (intersect({point.X, point.Y, segment.P.X, segment.P.Y, curve.Segment1.P.X, curve.Segment1.P.Y, curve.Segment2.P.X, curve.Segment2.P.Y}, pt))
	{
		if (getDistance(segment.P, pt) <= GEOMETRIC_EPSILON) return (touchCount + 1) % 2;
		if (getDistance(curve.Segment1.P, pt) <= GEOMETRIC_EPSILON || getDistance(curve.Segment2.P, pt) <= GEOMETRIC_EPSILON)
			return ++touchCount % 2;
		else if (curve.IsStraight())
		{
			touchCount++;
			return 1;
		}
	}
	if (!curve.IsStraight())
	{
		std::vector<double> roots = curve.GetCurveLineIntersection(segment.P.X,segment.P.Y, point.X - segment.P.X, point.Y - segment.P.Y);
		Curve line(segment, Segment(point));

		if (IsLuminosity)
			return roots.size() % 2;

		int count = 0;
		for (const auto& r : roots)
			if (line.GetTimeOf(curve.GetPoint(r)) != -1)
				count++;

		return count;
	}
	return 0;
}

bool CBooleanOperations::IsInside(const Segment& segment) const
{
	int count = 0;
	int touchCount = 0;
	for(const auto& c : segment.Id == 1 ? OriginCurves2 : OriginCurves1)
		count += CheckInters(MIN_POINT, segment, c, touchCount);

	return count % 2;
}

void CBooleanOperations::SetWinding()
{
	if (Locations.empty() || (Locations.size() == 2 && Locations[0]->Ends))
	{
		Segment s1, s2;

		for (const auto& s : Segments1)
			if (!s.Inters)
				s1 = s;

		for (const auto& s : Segments2)
			if (!s.Inters)
				s2 = s;

		bool winding = IsInside(s1);

		for (auto& s : Segments1)
			s.Winding = winding;

		winding = IsInside(s2);

		for (auto& s : Segments2)
			s.Winding = winding;
	}
	else
	{
		for (const auto& l : Locations)
		{
			Segment start = l->S,
				s = GetNextSegment(l->S);

			if (s.IsEmpty() || s.Inters || s == start)
				continue;

			bool winding = IsInside(s);

			int winding1 = false;
			if (s.Id == 1 ? !Close1 : !Close2)
				winding1 = IsInside(GetPreviousSegment(start));

			do
			{
				if (s.Id == 1)
				{
					Segments1[s.Index].Winding = winding;
					if (!Close1 && s.Index == Segments1.size() - 1)
					{
						winding = winding1;
						s = Segments1[0];
					}
					else
						s = GetNextSegment(s);
				}
				else
				{
					Segments2[s.Index].Winding = winding;
					if (!Close2 && s.Index == Segments2.size() - 1)
					{
						winding = winding1;
						s = Segments2[0];
					}
					else
						s = GetNextSegment(s);
				}
			} while (!s.IsEmpty() && !s.Inters && s != start);
		}
	}

	if (FillType & c_nStroke)
		for (auto& s : Segments2)
			s.Winding = 0;
}

void CBooleanOperations::DivideLocations()
{
	double	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin,
			prevTime = -1.0;

	Curve prevCurve;

	for (int i = static_cast<int>(Locations.size()) - 1; i >= 0; i--)
	{
		double	origTime = Locations[i]->Time,
				time = origTime;

		Segment segment;
		Curve	curve = GetCurve(Locations[i]->C.Segment1),
				newCurve;

		bool updateHandles = false;

		if (Locations[i]->C != prevCurve)
			prevCurve = Locations[i]->C;
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
				segment = Segment(Locations[i]->C.GetPoint(origTime),
								  false,
								  curve.Segment1.Index + 1,
								  curve.Segment1.Id,
								  curve.Segment1.PolyClosed);
			else
			{
				newCurve = curve.Segment1.Id == 1 ? curve.DivideAtTime(time)
												  : curve.DivideAtTime(time);
				segment = newCurve.Segment1;
				updateHandles = true;
			}
		}
		std::shared_ptr<Location>	inter = segment.Inters,
									dest = Locations[i]->Inters;
		if (inter)
		{
			LinkIntersection(inter, dest);

			std::shared_ptr<Location> other = inter;
			while (other)
			{
				LinkIntersection(other->Inters, inter);
				other = other->Next;
			}
		}
		else
			segment.Inters = dest;

		InsertSegment(segment, newCurve.Segment2, updateHandles);
		Locations[i]->S = segment;
	}
}

void CBooleanOperations::InsertLocation(std::shared_ptr<Location> loc, bool overlap)
{
	if (Locations.empty())
	{
		Locations.push_back(loc);
		return;
	}

	int	length = static_cast<int>(Locations.size()),
		l = 0,
		r = length - 1;
	while (l <= r)
	{
		int mid = (l + r) >> 1;

		if (getDistance(loc->C.GetPoint(loc->Time), Locations[mid]->C.GetPoint(Locations[mid]->Time)) <= LOCATION_EPSILON
			&& loc->C.Segment1.Id == Locations[mid]->C.Segment1.Id)
			return;

		if (overlap)
		{
			if (getDistance(loc->C.GetPoint(loc->Time), loc->Inters->C.GetPoint(loc->Inters->Time)) > LOCATION_EPSILON)
				return;

			for (int i = mid - 1; i >= -1; i--)
			{
				size_t idx = ((i % length) + length) % length;
				if (getDistance(loc->C.GetPoint(loc->Time), Locations[idx]->C.GetPoint(Locations[idx]->Time)) > LOCATION_EPSILON)
					break;
				if (getDistance(loc->C.GetPoint(loc->Time), Locations[idx]->C.GetPoint(Locations[idx]->Time)) <= LOCATION_EPSILON &&
					loc->C.Segment1.Id == Locations[idx]->C.Segment1.Id)
					return;
			}
			for (int i = mid + 1; i <= length; i++)
			{
				size_t idx = ((i % length) + length) % length;
				if (getDistance(loc->C.GetPoint(loc->Time), Locations[idx]->C.GetPoint(Locations[idx]->Time)) > LOCATION_EPSILON)
					break;
				if (getDistance(loc->C.GetPoint(loc->Time), Locations[idx]->C.GetPoint(Locations[idx]->Time)) <= LOCATION_EPSILON &&
					loc->C.Segment1.Id == Locations[idx]->C.Segment1.Id)
					return;
			}
		}

		double	diffId = loc->C.Segment1.Id - Locations[mid]->C.Segment1.Id,
				diffT = (loc->C.Segment1.Index + loc->Time) - (Locations[mid]->C.Segment1.Index + Locations[mid]->Time);
		bool	self = loc->C.Segment1.Id == Locations[mid]->C.Segment1.Id;
		double	diff = self ? (diffT) : (diffId);
		if (diff < 0)
			r = mid - 1;
		else
			l = mid + 1;
	}
	Locations.insert(Locations.begin() + l, loc);
}

bool CBooleanOperations::AllOverlap() const noexcept
{
	if (Locations.empty()) return false;

	for (const auto& l : Locations)
		if (!l->Overlap)
			return false;

	return true;
}

bool CBooleanOperations::AllInters(const std::vector<Segment>& segments) const noexcept
{
	for (const auto& s : segments)
		if (!s.Inters)
			return false;

	return true;
}

bool CBooleanOperations::IsOneCurvePath(int pathIndex) const noexcept
{
	return Locations.size() == 4 && pathIndex == 1 ? (OriginCurves1.size() == 1 && OriginCurves2.size() != 1)
												   : (OriginCurves2.size() == 1 && OriginCurves1.size() != 1);
}

void CBooleanOperations::AddLocation(Curve curve1, Curve curve2, double t1,
									 double t2, bool overlap, bool filter, bool ends)
{
	bool	excludeStart = !overlap &&	GetPreviousCurve(curve1) == curve2,
			excludeEnd = !overlap && curve1 != curve2 && GetNextCurve(curve1) == curve2;
	double	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;

	if (t1 >= (excludeStart ? tMin : 0) &&
		t1 <= (excludeEnd ? tMax : 1))
	{
		if (t2 >= (excludeEnd ? tMin : 0) &&
			t2 <= (excludeStart ? tMax : 1))
		{
			std::shared_ptr<Location>	loc1(new Location(curve1, t1, overlap, ends)),
										loc2(new Location(curve2, t2, overlap, ends));

			loc1->Inters = loc2;
			loc2->Inters = loc1;

			if (!filter || loc1->Overlap || IsCrossing(loc1))
				InsertLocation(loc1, overlap);
		}
	}
}

void CBooleanOperations::AddOffsets(std::vector<double>& offsets,
									const Curve& curve, bool end)
{
	std::vector<double> roots = curve.GetPeeks();

	size_t	 count = roots.size();
	bool first = end && count != 0,
		 second = !end && count != 0;

	double	offset = curve.GetLength(first ? roots[count - 1] : 0,
									second ? roots[0] : 1);
	offsets.push_back(count != 0 ? offset : offset / 32);
}

CGraphicsPath CalcBooleanOperation(const CGraphicsPath& path1,
								   const CGraphicsPath& path2,
								   BooleanOpType op,
								   long fillType,
								   bool isLuminosity)
{
	std::vector<CGraphicsPath>	paths1 = path1.GetSubPaths(),
								paths2 = path2.GetSubPaths(),
								paths;

	if (op == Subtraction)
	{
		for (const auto& p2 : paths2)
		{
			for (const auto& p1 : paths1)
			{
				CBooleanOperations operation(p1, p2, op, fillType, isLuminosity);
				paths.push_back(operation.GetResult());
			}
			paths1 = CGraphicsPath(paths).GetSubPaths();
			paths.clear();
		}
	}
	else
	{
		for (const auto& p1 : paths1)
		{
			for (const auto& p2 : paths2)
			{
				CBooleanOperations operation(p1, p2, op, fillType, isLuminosity);
				paths.push_back(operation.GetResult());
			}
		}
	}

	return op == Subtraction ? CGraphicsPath(paths1) : CGraphicsPath(paths);
}

//For unit-tests
bool CGraphicsPath::operator==(const CGraphicsPath& other) noexcept
{
	unsigned pointsCount = GetPointCount(),
		otherPointsCount = other.GetPointCount();

	if (pointsCount != otherPointsCount)
		return false;

	std::vector<PointD> points = GetPoints(0, pointsCount),
						otherPoints = other.GetPoints(0, otherPointsCount);

	for (unsigned i = 0; i < pointsCount; i++)
		if (getDistance(points[i], otherPoints[i]) > POINT_EPSILON)
			return false;

	return true;
}
}
