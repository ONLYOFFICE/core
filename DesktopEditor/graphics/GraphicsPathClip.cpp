#include "GraphicsPathClip.h"

namespace Aggplus {
Segment::Segment() : 
	P(PointF()),
	HI(PointF()),
	HO(PointF())
{

}

Segment::Segment(std::vector<PointF> points, bool isCurve,
				 size_t index, size_t id, CGraphicsPath* path) :
	P(points[0]),
	HI(PointF()),
	HO(PointF()),
	IsCurve(isCurve),
	Index(index), 
	Id(id),
	Path(path),
	Inters(nullptr)
{
	if (IsCurve)
	{
		IsCurve = true;
		HI = PointF(points[1].X - P.X, points[1].Y - P.Y);
		HO = PointF(points[2].X - P.X, points[2].Y - P.Y);
	}
}

Segment::Segment(PointF p, PointF hi, PointF ho) :
	P(p),
	HI(hi),
	HO(ho),
	IsCurve(false),
	Path(nullptr),
	Inters(nullptr)
{
	if (!hi.Equals(PointF()) || !ho.Equals(PointF()))
		IsCurve = true;
}

bool Segment::IsValid(BooleanOpType op)
{
	bool cmpOp1 = op == static_cast<int>(Winding["winding"]),
		 cmpOp2 = !(op == Union && Winding["winding"] == 2.0),
		 rWind = Winding["windingL"] != 0.0,
		 lWind = Winding["windingR"] != 0.0;
	return !Visited && cmpOp1 && cmpOp2 && rWind && lWind;
}

bool Segment::operator==(const Segment& other)
{
	return P.Equals(other.P) && HI.Equals(other.HI) && HO.Equals(other.HO);
}

bool Segment::operator!=(const Segment& other)
{
	return !operator==(other);
}

Curve::Curve() :
	Segment1(Segment()),
	Segment2(Segment())
{

}

Curve::Curve(Segment segment1, Segment segment2) :
	Segment1(segment1),
	Segment2(segment2)
{

}

Curve::Curve(std::vector<float> values)
{
	if (values.size() == 4)
	{
		Segment1 = Segment(PointF(values[0], values[1]), 
						   PointF(), 
						   PointF());
		Segment2 = Segment(PointF(values[2], values[3]), 
						   PointF(), 
						   PointF());
	}
	else if (values.size() == 8)
	{
		Segment1 = Segment(PointF(values[0], values[1]), 
						   PointF(), 
						   PointF());
		Segment2 = Segment(PointF(values[6], values[7]), 
						   PointF(values[2] - values[6], 
								  values[3] - values[7]),
						   PointF(values[4] - values[6], 
								  values[5] - values[7]));
	}
	else
	{
		Segment1 = Segment();
		Segment2 = Segment();
	}
}

Curve::Curve(float x0, float y0, float x1, float y1, 
			 float x2, float y2, float x3, float y3)
{
	Segment1 = Segment(PointF(x0, y0), PointF(), PointF());
	Segment2 = Segment(PointF(x1, y1), PointF(x2, y2), PointF(x3, y3)); 
}

std::vector<float> Curve::GetXValues()
{
	return {Segment1.P.X,
			Segment2.P.X + Segment2.HI.X,
			Segment2.P.X + Segment2.HO.X,
			Segment2.P.X};
}

std::vector<float> Curve::GetYValues()
{
	return {Segment1.P.Y,
			Segment2.P.Y + Segment2.HI.Y,
			Segment2.P.Y + Segment2.HO.Y,
			Segment2.P.Y};
}

std::vector<float> Curve::GetPeeks()
{
	std::vector<float>	x = GetXValues(),
						y = GetYValues(),
						roots;
	float	ax = -x[0] + 3 * x[1] - 3 * x[2] + x[3],
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

float Curve::GetLength(float a, float b)
{
	if(IsStraight())
	{
		Curve c = *this;
		if (b < 1)
			c = Subdivide(b)[0];
		if (a > 0)
			c = Subdivide(a)[1];
		float	dx = c.Segment2.P.X - c.Segment1.P.X,
				dy = c.Segment2.P.Y - c.Segment1.P.X;
		return sqrt(dx * dx + dy * dy);
	}
	std::vector<float>	x = GetXValues(),
						y = GetYValues();
	float	ax = 9 * (x[1] - x[2]) + 3 * (x[3] - x[0]),
			bx = 6 * (x[0] + x[2]) - 12 * x[1],
			cx = 3 * (x[1] - x[0]),
			ay = 9 * (y[1] - y[2]) + 3 * (y[3] - y[0]),
			by = 6 * (y[0] + y[2]) - 12 * y[1],
			cy = 3 * (y[1] - y[0]);
	return integrate(ax, bx, cx, ay, by, cy);
}

float Curve::GetSquaredLineLength()
{
	float	x = Segment2.P.X - Segment1.P.X,
			y = Segment2.P.Y - Segment1.P.Y;
	return x * x + y * y;
}

float Curve::GetTimeOf(PointF point)
{
	PointF	p0 = PointF(Segment1.P.X, Segment1.P.Y),
			p3 = PointF(Segment2.P.X, Segment2.P.Y);
	float	d0 = getDistance(point.X, point.Y, p0.X, p0.Y),
			d3 = getDistance(point.X, point.Y, p3.X, p3.Y);

	if (d0 > GEOMETRIC_EPSILON && d3 > GEOMETRIC_EPSILON)
	{
		std::vector<float> coords = {point.X, point.Y},
						   roots;
		for (size_t c = 0; c < 2; c++)
		{
			int count = SolveCubic(c, coords[c], roots, 0.0, 1.0);
			for (int i = 0; i < count; i++)
			{
				float u = roots[i];
				if (u > 1 || u < 0)
					return - 1;
				if (getDistance(point, GetPoint(u)) <=
					GEOMETRIC_EPSILON)
					return u;
			}
		}
	}
	bool firstDist = getDistance(point, p0) <= GEOMETRIC_EPSILON,
		 secondDist = getDistance(point, p3) <= GEOMETRIC_EPSILON;
	return firstDist ? 0 : secondDist ? 1 : - 1;
}

float Curve::GetTimeAt(float offset)
{
	bool forward = offset > 0;
	float start = !forward ? 1 : 0;
	if (offset == 0)
		return start;

	float	a = forward ? start : 0,
			b = forward ? 1 : start,
			rangeLength = GetLength(a, b),
			diff = abs(offset) - rangeLength;

	if (abs(diff) < EPSILON)
		return forward ? b : a;
	else if (diff > EPSILON)
		return FLT_MIN;

	float	guess = offset / rangeLength,
			length = 0;
	std::vector<float>	x = GetXValues(),
						y = GetYValues();
	float	ax = 9 * (x[1] - x[2]) + 3 * (x[3] - x[0]),
			bx = 6 * (x[0] + x[2]) - 12 * x[1],
			cx = 3 * (x[1] - x[0]),
			ay = 9 * (y[1] - y[2]) + 3 * (y[3] - y[0]),
			by = 6 * (y[0] + y[2]) - 12 * y[1],
			cy = 3 * (y[1] - y[0]);
	return findRoot(length, start, offset, ax, bx, cx, 
					ay, by, cy, start + guess, a, b);
}

PointF Curve::Get(float t, int type)
{
	std::vector<float>	x = GetXValues(),
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
	float	cx = 3 * (x[1] - x[0]),
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
		float	tMin = CURVETIME_EPSILON,
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

		float len = sqrt(x0 * x0 + y0 * y0);
		if (len != 0.0)
		{
			x0 /= len;
			y0 /= len;
		}
	}
	return PointF(x0, y0);
}

PointF Curve::GetPoint(float t)
{
	return Get(t, 0);
}

PointF Curve::GetTangent(float t)
{
	return Get(t, 1);
}

PointF Curve::GetTangent(float t, float offset,
						 bool inside, PointF p)
{
	if (inside)
		return GetTangent(t);
	PointF point =  GetPoint(GetTimeAt(offset));
	return PointF(point.X - p.X, point.Y - p.Y);
}

Curve Curve::GetPart(float from, float to)
{
	Curve result;
	if (from > to)
		std::swap(from, to);
	if (from > 0)
		result = Subdivide(from)[1];
	if (to < 1)
		result = Subdivide((to - from) / (1 - from))[0];
	if (from > to)
		result.Flip();
	return result;
}

std::vector<Curve> Curve::GetMonoCurves(bool dir)
{
	std::vector<Curve>	curves;
	std::vector<float>	x = GetXValues(),
						y = GetYValues(),
						o = dir ? x : y;
	bool cmpO1 = (o[0] >= o[1]) == (o[1] >= o[2]),
		 cmpO2 = (o[1] >= o[2]) == (o[2] >= o[3]);
	if (cmpO1 && cmpO2 || IsStraight())
		curves.push_back(*this);
	else
	{
		std::vector<float> roots;
		float	a = 3 * (o[1] - o[2]) - o[0] + o[3],
				b = 2 * (o[0] + o[2]) - 4 * o[1],
				c = o[1] - o[0],
				tMin = CURVETIME_EPSILON,
				tMax = 1 - tMin,
				n = solveQuadratic(a, b, c, roots, tMin, tMax);
		if (n == 0)
			curves.push_back(*this);
		else
		{
			std::sort(roots.begin(), roots.end());
			float t = roots[0];
			std::vector<Curve> parts = Subdivide(t);
			curves.push_back(parts[0]);
			if (n > 1)
			{
				t = (roots[1] - t) / (1- t);
				parts = Subdivide(t);
				curves.push_back(parts[0]);
			}
			curves.push_back(parts[1]);
		}
	}
	return curves;
}

std::vector<std::pair<int, int>> Curve::GetOverlaps(Curve curve)
{
	bool swap = this->GetSquaredLineLength() <
				curve.GetSquaredLineLength(),
		 straight1 = this->IsStraight(),
		 straight2 = curve.IsStraight(),
		 straightBoth = straight1 && straight2;
	std::vector<float>	x1 = GetXValues(),
						x2 = GetYValues(),
						y1 = curve.GetXValues(),
						y2 = curve.GetYValues();
	float	px = swap ? x2[0] : x1[0],
			py = swap ? y2[0] : y1[0],
			vx = swap ? x2[3] - px : x1[3] - px,
			vy = swap ? y2[3] - py : y1[3] - py;
	
	if (swap)
	{
		std::swap(x1, x2); 
		std::swap(y1, y2);
	}
	
	float	d20 = getDistance(px, py, vx, vy, x2[0], y2[0]),
			d23 = getDistance(px, py, vx, vy, x2[3], y2[3]);

	if (d20 < GEOMETRIC_EPSILON && d23 < GEOMETRIC_EPSILON)
	{
		float	d11 = getDistance(px, py, vx, vy, x1[1], y1[1]),
				d12 = getDistance(px, py, vx, vy, x1[2], y1[2]),
				d21 = getDistance(px, py, vx, vy, x2[1], y2[1]),
				d22 = getDistance(px, py, vx, vy, x2[2], y2[2]);

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
		float t2 = i1 == 0	? GetTimeOf(t1 == 1	? curve.Segment2.P : curve.Segment1.P)
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
	}

	if (pairs.size() != 2)
		pairs.clear();
	else if (!straightBoth)
	{
		Curve	o1 = GetPart(pairs[0].first, pairs[1].first),
				o2 = curve.GetPart(pairs[0].second, pairs[1].second);
		float	dox = abs(o2.Segment2.HO.X - o1.Segment2.HO.X),
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

std::vector<float> Curve::GetCurveLineIntersection(float px, float py,
													float vx, float vy)
{
	if (isZero(vx) && isZero(vy))
	{
		float t = GetTimeOf(PointF(px, py));
		return t == -1 ? std::vector<float>() : std::vector<float>({t});
	}

	float	angle = std::atan(-vy / vx),
			sin = std::sin(angle),
			cos = std::cos(angle);
	std::vector<float>	x = GetXValues(),
						y = GetYValues(),
						rv, roots;
	for (size_t i = 0; i < 8; i++)
	{
		float	cx = (i % 2) == 0 ? x[i / 2] : y[i / 2],
				cy = (i % 2) == 0 ? y[i / 2] : x[i / 2];
		rv.push_back(cx * cos - cy * sin);
		rv.push_back(cx * sin + cy * cos);
	}
	Curve curve = Curve(rv);
	SolveCubic(1, 0, roots, 0, 1);
	return roots;
}

std::vector<Curve> Curve::Subdivide(float t)
{
	std::vector<float>	x = GetXValues(),
						y = GetYValues();
	float x2[6], y2[6], u = 1 - t;

	x2[0] = u * x[0] + t * x[1], y2[0] = u * y[0] + t * y[1],
	x2[1] = u * x[1] + t * x[2], y2[1] = u * y[1] + t * y[2],
	x2[2] = u * x[2] + t * x[3], y2[2] = u * y[2] + t * y[3],

	x2[3] = u * x2[0] + t * x2[1], y2[3] = u * y2[0] + t * y2[1],
	x2[4] = u * x2[1] + t * x2[2], y2[4] = u * y2[1] + t * y2[2],
	x2[5] = u * x2[3] + t * x2[4], y2[5] = u * y2[3] + t * y2[4];

	return {Curve(x[0], y[0], x2[0], y2[0], x2[3], y2[3], x2[5], y2[5]),
			Curve(x2[5], y2[5], x2[4], y2[4], x2[2], y2[2], x[3], y[3])};
}

Curve Curve::DivideAtTime(float time, std::vector<Segment> segments,
						std::vector<Curve> curves)
{
	float	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;
	if (time >= tMin && time <= tMax)
	{
		std::vector<Curve> parts = Subdivide(time);
		Curve	left = parts[0],
				right = parts[1];
		Segment segment = Segment(left.Segment2.P,
								  left.Segment2.HO,
								  right.Segment2.HI);
		segments.insert(segments.begin() + Segment1.Index + 1, segment);
		return curves[Segment1.Index + 1];
	}
	return Curve();
}

int Curve::SolveCubic(size_t coord, int value, std::vector<float>& roots,
					float mn, float mx)
{
	int count = 0;
	std::vector<float> v = coord == 0 ? GetXValues() : GetYValues();
	float a, b, c, d;

	if (!(v[0] < value && v[3] < value && v[1] < value && v[2] < value ||
		  v[0] > value && v[3] > value && v[1] > value && v[2] > value))
	{
		c = 3 * (v[1] - v[0]);
		b = 3 * (v[2] - v[1]) - c;
		a = v[3] - v[0] - c - b;
		d = v[0] - value;
		count = SolveCubic(a, b, c, d, roots, mn, mx);
	}
	return count;
}

int Curve::SolveCubic(float a, float b, float c, float d,
					std::vector<float>& roots, float mn, float mx)
{
	int count = 0;
	float f = max(abs(a), abs(b), abs(c), abs(d));
	float x, b1, c2, qd, q;
	if (f < 1e-8 || f < 1e8)
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
		x = FLT_MAX;
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
		float t = q / a,
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
	bool xInRoots = count > 0 && x != roots[0] && x != roots[1],
		 xInEps = x > mn - EPSILON && x < mx + EPSILON;
	if (x == FLT_MAX && (count == 0 || xInRoots) && (xInEps))
		roots[count++] = x;
	return count;
}

void Curve::Flip()
{
	std::swap(Segment1.P, Segment2.P);
	std::swap(Segment1.HI, Segment1.HO);
}

void Curve::ClearHandles()
{
	Segment2.HI = PointF();
	Segment2.HO = PointF();
}

bool Curve::HasHandle()
{
	return	!isZero(Segment2.HI.X) ||
			!isZero(Segment2.HI.Y) ||
			!isZero(Segment2.HO.X) ||
			!isZero(Segment2.HO.Y);
}

bool Curve::IsStraight()
{
	return !Segment2.IsCurve;
}

bool Curve::AddWinding(std::vector<Curve> curves, bool dir, bool& onPath,
						bool& onAnyPath, Curve* cPrev, std::vector<float>& roots, 
						std::unordered_map<std::string, float>* map)
{
	std::vector<float>	x = GetXValues(),
						y = GetYValues(),
						xPrev = cPrev->GetXValues(),
						yPrev = cPrev->GetYValues(),
						o = dir ? x : y;
	bool res = false;

	if ((*map)["pointOrdinat"] < std::min(o[0], o[3]) || 
		(*map)["pointOrdinat"] > std::max(o[0], o[3]))
		return res;
	
	std::vector<float> a = dir ? y : x;

	if (o[0] == o[3])
	{
		if (a[0] < (*map)["paR"] && a[3] > (*map)["paL"] || 
			a[3] < (*map)["paR"] && a[0] > (*map)["paL"])
			onPath = true;
		return res;
	}

	int t;
	if ((*map)["pointOrdinat"] == o[0] || (*map)["pointOrdinat"] == o[3])
		t = (*map)["pointOrdinat"] == o[0] ? 0 : 1;
	else if ((*map)["paL"] > max(a[0], a[1], a[2], a[3]) || 
			 (*map)["paR"] < min(a[0], a[1], a[2], a[3]))
		t = 1;
	else
	{
		int count =	SolveCubic(dir ? 0 : 1, (*map)["pointOrdinat"],
								roots, 0, 1);
		t = count > 0 ? roots[0] : 1;
	}

	float abscis;
	if (t == 0 || t == 1)
		abscis = t == 0 ? a[0] : a[3];
	else
	{
		PointF pt = GetPoint(t);
		abscis = dir ? pt.Y : pt.X;
	}

	int		winding = o[0] > o[3] ? 1 : -1;
	bool	dirDiff = dir ? xPrev[0] > xPrev[3] : yPrev[0] > yPrev[3];
	int		windingPrev = dirDiff ? 1 : -1;
	float	a3Prev = dir ? yPrev[3] : yPrev[0];

	if ((*map)["pointOrdinat"] != o[0])
	{
		if (abscis < (*map)["paL"])
			(*map)["pathWindingL"] += winding;
		else if (abscis > (*map)["paR"])
			(*map)["pathWindingR"] += winding;
		else
			onPath = true;
		
		if (abscis > (*map)["pointAbscis"] - QUALITI_EPSILON && 
			abscis < (*map)["pointAbscis"] + QUALITI_EPSILON)
			(*map)["quality"] /= 2;
	}
	else
	{
		if (winding != windingPrev)
		{
			if (a[0] < (*map)["paL"])
				(*map)["pathWindingL"] += winding;
			else if (a[0] > (*map)["paR"])
				(*map)["pathWindingR"] += winding;
		}
		else if (a[0] != a3Prev)
		{
			if (a3Prev < (*map)["paR"] && abscis > (*map)["paR"])
			{
				(*map)["pathWindingR"] += winding;
				onPath = true;
			}
			else if (a3Prev > (*map)["paL"] && abscis < (*map)["paL"])
			{
				(*map)["pathWindingL"] += winding;
				onPath = true;
			}
		}
		(*map)["quality"] /= 4;
	}
	cPrev = this;

	PointF	pt = GetTangent(t);
	bool	inRange = abscis > (*map)["paL"] &&
					  abscis < (*map)["paR"] &&
					  dir ? pt.X : pt.Y == 0.0;
	return	inRange &&
			CGraphicsPathClip::GetWinding(curves, !dir, onPath,
		   								 onAnyPath, cPrev, roots, map);
}

bool Curve::HandleCurve(std::vector<Curve> curves, bool dir, bool& onPath,
						bool& onAnyPath, Curve* cPrev, std::vector<float>& roots, 
						std::unordered_map<std::string, float>* map)
{
	std::vector<float>	x = GetXValues(),
						y = GetYValues(),
						o = dir ? x : y;
	bool res = false;

	if ((*map)["pointOrdinat"] <= max(o[0], o[1], o[2], o[3]) && 
		(*map)["pointOrdinat"] >= min(o[0], o[1], o[2], o[3]))
	{
		std::vector<float> a = dir ? y : x;
		std::vector<Curve> monoCurves;
		if ((*map)["paL"] > max(a[0], a[1], a[2], a[3]) || 
			(*map)["paR"] < min(a[0], a[1], a[2], a[3]))
			monoCurves.push_back(*this);
		else
			monoCurves = GetMonoCurves(dir);
		for (auto c : monoCurves)
			if (res = c.AddWinding(curves, dir, onPath, onAnyPath,
									cPrev, roots, map))
				return res;
	}
	return res;
}

bool Curve::operator==(const Curve& other)
{
	return Segment1 == other.Segment1 && 
		   Segment2 == other.Segment2;
}

bool Curve::operator!=(const Curve& other)
{
	return !operator ==(other);
}

Location::Location() :
	C(Curve()),
	S(Segment()),
	Inters(nullptr),
	Next(nullptr),
	Prev(nullptr)
{

}

Location::Location(Curve curve, float time, bool overlap) :
	C(curve),
	S(Segment()),
	Time(time),
	Overlap(overlap),
	Inters(nullptr),
	Next(nullptr),
	Prev(nullptr)
{

}

bool Location::IsTouching()
{
	Location *inter = this->Inters;
	if (inter != nullptr)
	{
		Curve	c1 = this->C,
				c2 = inter->C;
		return (c1.IsStraight() &&
				c2.IsStraight() &&
				!intersect(c1.Segment1.P.X,
							c1.Segment1.P.Y,
							c1.Segment2.P.X,
							c1.Segment2.P.Y,
							c2.Segment1.P.X,
							c2.Segment1.P.Y,
							c2.Segment2.P.X,
							c2.Segment2.P.Y).Equals(PointF()));
	}
}

bool Location::operator==(const Location& other)
{
	return C == other.C && Time == other.Time && Overlap == other.Overlap; 
}

Branch::Branch() : Start(0.0), HI(PointF())
{

}

Branch::Branch(float start, std::vector<Segment> crossings, PointF hi) :
	Start(start),
	Crossings(crossings),
	HI(hi)
{

}

CGraphicsPathClip::CGraphicsPathClip(CGraphicsPath* path1, 
									CGraphicsPath* path2,
									BooleanOpType op) :
	Path1(path1),
	Path2(path2),
	Op(op)
{
	TraceBoolean();
}

CGraphicsPath CGraphicsPathClip::GetResult()
{
	return Result;
}

void CGraphicsPathClip::TraceBoolean()
{
	if ((Op == Subtraction || Op == Exclusion) && 
		Path1->IsClockwise() ^
		Path2->IsClockwise())
		Path2->Reverse();

	PreparePath(Path1, 1, Segments1, Curves1);
	PreparePath(Path2, 2, Segments2, Curves2);
	GetIntersection();
	
	// for (size_t i = Locations.size() - 1; i >=0; i--)
	// {
	// 	InsertLocation(Locations[i].C.Inters);
	// }

	DivideLocations();
	std::vector<Location> crossing = Locations;
	std::vector<Curve>*** curveCollisionsMap = nullptr;

	if (!crossing.empty())
	{
		std::vector<std::vector<int>> curveCollisions = 
			FindCurveBoundsCollisions(Curves, Curves, 0, true);
		
		curveCollisionsMap = InicializeMap(curveCollisions);

		for (const auto& l : crossing)
		{
			PropagateWinding(l.S, curveCollisionsMap);
		}
	
		for (const auto& s : Segments)
		{
			Location* loc = s.Inters;
			
			if (s.Winding.empty())
				PropagateWinding(s, curveCollisionsMap);

			if (!(loc != nullptr && loc->Overlap))
				OverlapOnly[s.Id] = false;
		}
		TracePaths();
		ClearMap(curveCollisionsMap);
	}
	else
	{
		//reorientPaths();
	}
}

void CGraphicsPathClip::TracePaths()
{
	std::vector<Segment> starts;

	std::sort(Segments.begin(),
			  Segments.end(),
	[](Segment seg1, Segment seg2){
		bool inter1 = seg1.Inters != nullptr,
			 inter2 = seg2.Inters != nullptr;
		bool over1 = inter1 && seg1.Inters->Overlap,
			 over2 = inter2 && seg1.Inters->Overlap;
		int	o1 = over1 ? 1 : -1,
			i1 = inter1 ? 1 : -1,
			id = seg1.Id != seg2.Id ? seg1.Id - seg2.Id
									: seg1.Index - seg2.Index;
		return over1 ^ over2 ? o1 : inter1 ? i1 : id;  
	});

	for (auto& s : Segments)
	{
		std::vector<Branch> branches;
		std::vector<Segment> visited;
		Branch branch;
		PointF hi = PointF();
		bool valid = s.IsValid(Op),
			 finished = false,
			 closed = true,
			 first = true;

		while (valid)
		{
			bool cross;
			std::vector<Segment> crossings = GetCrossing(s, starts, first);
			Segment other;
			if (!crossings.empty())
			{
				other = crossings[0];
				crossings.erase(crossings.begin());
				finished = !first && (s.Index == 0 || other.Index == 0);
				cross = !finished;
			}
			else
			{
				finished = !first && s.Index == 0;
				cross = false;
			}

			if (first)
			{
				Result.StartFigure();
				first = false;
			}

			if (finished)
			{
				if (s.Index == 0 || s.Index == s.Id == 1 ? Segments1.size() - 1
														 : Segments2.size() - 1)
					closed = s.Path->Is_poly_closed();
				s.Visited = true;
				break;
			}

			if (cross && !branch.Crossings.empty())
			{
				branches.push_back(branch);
				branch.Crossings.clear();
				branch.Visited.clear();
			}

			if (branch.Crossings.empty())
			{
				if (cross)
					crossings.push_back(s);
				branch = Branch(s.Id == 1	? Segments1.size()
											: Segments2.size(), crossings, hi);
			}

			if (cross)
				s = other;

			if (!s.IsValid(Op))
			{
				bool seg = false;
				do
				{
					if (!branch.Crossings.empty())
					{
						s = branch.Crossings[0];
						branch.Crossings.erase(branch.Crossings.begin());
					}
					else
					{
						if (!branches.empty())
						{
							branch = branches[branches.size() - 1];
							branches.pop_back();
							visited = branch.Visited;
							hi = branch.HI;
						}
						else
						{
							seg = true;
							break;
						}
					}
				} while (!s.IsValid(Op));
				if (seg)
					break;
			}

			if (first)
				Result.MoveTo(s.P.X, s.P.Y);
			else if (s.IsCurve)
				Result.CurveTo(s.P.X + hi.X, s.P.Y + hi.Y,
							   s.P.X + s.HO.X, s.P.Y + s.HO.Y,
							   s.P.X, s.P.Y);
			else
				Result.LineTo(s.P.X, s.P.Y);
			*s.Path = Result;
			s.Visited = true;
			visited.push_back(s);
			Segment next = GetNextSegment(s);
			s = next;
			hi = next.HI;
		}

		if (finished)
		{
			if (closed)
				Result.CloseFigure();

			if (Result.GetArea() == 0)
				Result.Reset();
		}
	}
}

void CGraphicsPathClip::PreparePath(CGraphicsPath* path, size_t id,
									std::vector<Segment>& segments, 
									std::vector<Curve>& curves)
{
	for (size_t i = 0; i < path->GetPointCount(); i++)
	{
		bool isCurve = path->IsCurvePoint(i);
		std::vector<PointF> points = path->GetPoints(i, isCurve ? 3 : 1);
		segments.push_back(Segment(points, isCurve, i, id, path));
		if (isCurve)
			i += 2;
	}

	for (size_t i = 0; i < Segments1.size(); i++)
	{
		curves.push_back(Curve(segments[i], i == (segments.size() - 1) ?
											segments[0] : 
											segments[i + 1]));
	}

	std::copy(segments.begin(), segments.end(), std::back_inserter(Segments));
	std::copy(curves.begin(), curves.end(), std::back_inserter(Curves));
}

Curve CGraphicsPathClip::GetCurve(Segment segment)
{
	if (segment.Id == 1)
		return Curves1[segment.Index];
	else if (segment.Id == 2)
		return Curves2[segment.Index];
}

Curve CGraphicsPathClip::GetPreviousCurve(Curve curve)
{
	if (curve.Segment1.Path->Is_poly_closed() && 
		curve.Segment1.Index == 0)
		return	curve.Segment1.Id == 1	? Curves1[Curves1.size() - 1]
										: Curves2[Curves2.size() - 1];
	return	curve.Segment1.Id == 1	? Curves1[curve.Segment1.Index - 1]
									: Curves2[curve.Segment1.Index - 1];
}

Curve CGraphicsPathClip::GetNextCurve(Curve curve)
{
	bool path1 = curve.Segment1.Id == 1;
	size_t last = path1 ? Curves1.size() - 1 
						: Curves2.size() - 1;
	if (curve.Segment1.Path->Is_poly_closed() && 
		curve.Segment1.Index == last)
		return path1 ? Curves1[0] : Curves2[0];
	return	path1	? Curves1[curve.Segment1.Index + 1]
					: Curves2[curve.Segment1.Index + 1];
}

Segment CGraphicsPathClip::GetNextSegment(Segment segment)
{
	bool path1 = segment.Id == 1;
	size_t last = path1 ? Segments1.size() - 1
						: Segments2.size() - 1;
	if (segment.Path->Is_poly_closed() &&
		segment.Index == last )
		return path1 ? Segments1[0] : Segments2[0];
	return	path1	? Segments1[segment.Index - 1]
					: Segments2[segment.Index - 1];
}

std::vector<Curve> CGraphicsPathClip::GetCurves(std::vector<int> indices)
{
	std::vector<Curve> list;
	for (const auto& i : indices)
		list.push_back(Curves[i]);
	return list;
}

std::vector<Segment> CGraphicsPathClip::GetCrossing(Segment seg,
													std::vector<Segment>& starts,
													bool collectStart)
{
	std::vector<Segment> crossing;
	Location	*loc = seg.Inters,
				*start = loc;
	
	if (collectStart)
		starts = {seg};

	if (loc != nullptr)
	{
		Collect(seg, crossing, starts, loc, nullptr, collectStart);

		while (loc != nullptr && loc->Prev != nullptr)
			loc = loc->Prev;
		Collect(seg, crossing, starts, loc, start, collectStart);
	}
	return crossing;
}

std::vector<std::vector<float>> CGraphicsPathClip::GetBoundsForCurves(
	std::vector<Curve> curves)
{
	std::vector<std::vector<float>> bounds;
	for (size_t i = 0; i < curves.size(); i++)
	{
	std::vector<float>	x = curves[i].GetXValues(),
						y = curves[i].GetYValues();
	bounds.push_back({min(x[0], x[1], x[2], x[3]),
					  min(y[0], y[1], y[2], y[3]),
					  max(x[0], x[1], x[2], x[3]),
					  max(y[0], y[1], y[2], y[3])});
	}
	return bounds;
}

std::vector<std::vector<int>> CGraphicsPathClip::FindCurveBoundsCollisions(
	std::vector<Curve> curves1, 
	std::vector<Curve> curves2, 
	float tolerance, 
	bool bothAxis) 
{
	std::vector<std::vector<float>> bounds1 = GetBoundsForCurves(curves1);
	std::vector<std::vector<float>> bounds2 = GetBoundsForCurves(curves2);

	if (bothAxis)
	{
		std::vector<std::vector<int>> hor = 
			FindBoundsCollisions(bounds1, bounds2, tolerance, false, true);
		std::vector<std::vector<int>> ver = 
			FindBoundsCollisions(bounds1, bounds2, tolerance, true, true);
		std::vector<std::vector<int>> list;
		for (size_t i = 0; i < hor.size(); i++)
		{
			list.push_back(hor[i]);
			list.push_back(ver[i]);
		}
		return list;
	}
	return FindBoundsCollisions(bounds1, bounds2, tolerance);
}

std::vector<std::vector<int>> CGraphicsPathClip::FindBoundsCollisions(
	std::vector<std::vector<float>> bounds1,
	std::vector<std::vector<float>> bounds2,
	float tolerance,
	bool sweepVertical,
	bool onlySweep)
{
	std::vector<std::vector<float>> allBounds(bounds1);
	std::copy(bounds2.begin(), bounds2.end(), std::back_inserter(allBounds));
	size_t	allLength = allBounds.size(),
			length1 = bounds1.size();

	size_t	pri1 = sweepVertical ? 1 : 0,
			pri2 = pri1 + 2,
			sec1 = sweepVertical ? 0 : 1,
			sec2 = sec1 + 2;

	std::vector<int> allIdicesByPri1(allLength);
	for (size_t i = 0; i < allLength; i++)
	{
		allIdicesByPri1[i] = i;
	}
	std::sort(allIdicesByPri1.begin(),
			  allIdicesByPri1.end(),
	[&allBounds, &pri1](size_t i1, size_t i2){
		return allBounds[i1][pri1] < allBounds[i2][pri1];
	});

	std::vector<int> activeIndicesByPri2;
	std::vector<std::vector<int>> allCollisions(length1);
	for (size_t i = 0; i < allLength; i++)
	{
		int curIndex = allIdicesByPri1[i];
		std::vector<float> curBounds = allBounds[curIndex];
		std::vector<int> curCollisions;
		size_t origIndex = curIndex - length1;
		bool isCurrent1 = curIndex < length1,
		isCurrent2 = !isCurrent1;
		if (!activeIndicesByPri2.empty())
		{
			size_t pruneCount = 
				binarySearch(allBounds, activeIndicesByPri2, pri2, 
							 curBounds[pri1] - GEOMETRIC_EPSILON) + 1;
			activeIndicesByPri2.erase(activeIndicesByPri2.begin(), 
									  activeIndicesByPri2.begin() + pruneCount);
			if (onlySweep)
			{
				curCollisions.insert(curCollisions.end(),
									 activeIndicesByPri2.begin(),
									 activeIndicesByPri2.end());
				for (size_t j = 0; j < activeIndicesByPri2.size(); j++)
				{
					int activeIndex = activeIndicesByPri2[j];
					allCollisions[activeIndex].push_back(origIndex);
				}
			}
		else
		{
			float	curSec1 = curBounds[sec2],
					curSec2 = curBounds[sec1];
			for (int j = 0; j < activeIndicesByPri2.size(); j++)
			{
				int activeIndex = activeIndicesByPri2[j];
				std::vector<float> activeBounds = allBounds[activeIndex];
				bool isActive1 = activeIndex < length1,
					 isActive2 = !isActive1,
					 isActive1Or2 = (isCurrent1 && isActive2 || isCurrent2 && isActive1),
					 inRange1 = curSec1 <= activeBounds[sec2] + tolerance,
					 inRange2 = curSec2 >= activeBounds[sec1] - tolerance;
				if (onlySweep || isActive1Or2 && (inRange2 && inRange1))
				{
					if (isCurrent1 && isActive2)
						curCollisions.push_back(activeIndex - length1);
					if (isCurrent2 && isActive1)
						allCollisions[activeIndex].push_back(origIndex);
				}
			}
		}
	}
	if (isCurrent1)
	{
		allCollisions[curIndex] = curCollisions;
	}
	if (activeIndicesByPri2.size() > 0)
	{
		float curPri2 = curBounds[pri2];
		int index = binarySearch(allBounds, activeIndicesByPri2, pri2, curPri2);
		activeIndicesByPri2.insert(activeIndicesByPri2.begin() + index + 1, curIndex);
	}
	else
	{
		activeIndicesByPri2.push_back(curIndex);
	}
	}
	for (auto& c : allCollisions)
	{
		if (!c.empty())
		std::sort(c.begin(), c.end());
	}
	return allCollisions;
}

bool CGraphicsPathClip::IsCrossing(Location loc)
{
	Location *inter = loc.Inters;

	if(inter == nullptr)
		return false;

	float	t1 = loc.Time,
			t2 = inter->Time,
			tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;
	bool	t1Inside = t1 >= tMin && t1 <= tMax,
			t2Inside = t2 >= tMin && t2 <= tMax;

	if (t1Inside && t2Inside)
		return !loc.IsTouching();

	Curve	c2 = loc.C,
			c1 = t1 < tMin ? GetPreviousCurve(c2) : c2,
			c4 = inter->C,
			c3 = t2 < tMin ? GetPreviousCurve(c4) : c4;

	if (t1 > tMax)
		c2 = GetNextCurve(c2);
	if (t2 > tMax)
		c4 = GetNextCurve(c4);
	
	std::vector<float> offsets;

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

	PointF	pt = loc.C.Segment1.P;
	float	offset = FLT_MAX;
	for (const auto& o : offsets)
		if (o < offset)
			offset = o;

	PointF	v2 = c2.GetTangent(t1, offset, t1Inside, pt),
			v1 = c1.GetTangent(t1, offset, t1Inside, pt),
			v4 = c4.GetTangent(t2, offset, t2Inside, pt),
			v3 = c3.GetTangent(t2, offset, t2Inside, pt);
	float	a1 = atan(v1.Y / v1.X) * 180 / M_PI,
			a2 = atan(v2.Y / v2.X) * 180 / M_PI,
			a3 = atan(v3.Y / v3.X) * 180 / M_PI,
			a4 = atan(v4.Y / v4.X) * 180 / M_PI;
	bool	inRange34 = isInRange(a1, a3, a4) ^ isInRange(a2, a3, a4),
			inRange43 = isInRange(a1, a4, a3) ^ isInRange(a2, a4, a3),
			inRange1 = inRange34 && inRange43,
			inRange12 = isInRange(a3, a1, a2) ^ isInRange(a4, a1, a2),
			inRange21 = isInRange(a3, a2, a1) ^ isInRange(a4, a2, a1),
			inRange2 = inRange21 && inRange12;

	return t1Inside ? inRange1 : inRange2;
}

bool CGraphicsPathClip::FilterIntersections(Location loc)
{
	return loc.Overlap || IsCrossing(loc);
}

bool CGraphicsPathClip::IntersectsBounds()
{
	RectF_T<double> rect1, rect2;
	Path1->GetBounds(rect1.X, rect1.Y, rect1.Width, rect1.Height);
	Path2->GetBounds(rect2.X, rect2.Y, rect2.Width, rect2.Height);

	return	(rect2.X + rect2.Width > rect1.X - EPSILON) &&
			(rect2.Y + rect2.Height > rect1.Y - EPSILON) &&
			(rect2.X < rect1.X + rect1.Width + EPSILON) &&
			(rect2.Y < rect1.Y + rect2.Height + EPSILON);
}

void CGraphicsPathClip::GetIntersection()
{
	if (!IntersectsBounds())
		return;

	std::vector<std::vector<int>> boundsCollisions =
		FindCurveBoundsCollisions(Curves1, Curves2, GEOMETRIC_EPSILON);
	for (size_t index1 = 0; index1 < Curves1.size(); index1++)
	{
		Curve curve1 = Curves1[index1];
		std::vector<int> collisions1 = boundsCollisions[index1];
		if (!collisions1.empty())
		{
			for (size_t j = 0; j < collisions1.size(); j++)
			{
				int index2 = collisions1[j];
				if (index2 > index1)
				{
					Curve curve2 = Curves2[index2];
					GetCurveIntersection(curve1, curve2);
				}
			}
		}
	}
}

void CGraphicsPathClip::GetCurveIntersection(Curve curve1, Curve curve2)
{
	std::vector<float>	x1 = curve1.GetXValues(),
						y1 = curve1.GetYValues(),
						x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues();

	float	minX1 = min(x1[0], x1[1], x1[2], x1[3]),
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
		std::vector<std::pair<int, int>> overlaps =
			curve1.GetOverlaps(curve2);
		if (!overlaps.empty())
		{
			for (size_t i = 0; i < 2; i++)
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
			size_t	before = Locations.size();
			Curve	c1 = flip ? curve2 : curve1,
					c2 = flip ? curve1 : curve2;
			if (straight)
				AddLineIntersection(c1, c2, flip);
			else if (straight1 || straight2)
				AddCurveLineIntersection(c1, c2, flip);
			else
				AddCurveIntersection(c1, c2, flip);
			if (!straight || Locations.size() == before)
			{
				for (size_t i = 0; i < 4; i++)
				{
					int t1 = i >> 1,
						t2 = i & 1;
					PointF	p1 = t1 == 0 ? curve1.Segment1.P
										 : curve1.Segment2.P,
							p2 = t2 == 0 ? curve2.Segment1.P
										 : curve2.Segment2.P;
					if (getDistance(p1, p2) <= EPSILON)
						AddLocation(curve1, curve2, t1, t2, false);
				}
			}
		}
	}
}

void CGraphicsPathClip::LinkIntersection(Location *from, Location *to)
{
	Location *prev = from;
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

void CGraphicsPathClip::AddLineIntersection(Curve curve1, Curve curve2, bool flip)
{
	std::vector<float>	x1 = curve1.GetXValues(),
						y1 = curve1.GetYValues(),
						x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues();
	PointF pt = intersect(x1[0], y1[0], x1[3], y1[3], x2[0], y2[0], x2[3], y2[3]);
	if (!pt.Equals(PointF()))
	{
		AddLocation(curve1, curve2, curve1.GetTimeOf(pt), curve2.GetTimeOf(pt));
	}
}

void CGraphicsPathClip::AddCurveLineIntersection(Curve curve1, Curve curve2, bool flip)
{
	std::vector<float>	x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues(),
						roots = curve1.GetCurveLineIntersection(x2[0], y2[0],
																x2[3] - x2[0],
																y2[3] - y2[0]);
	for (const auto& r : roots)
	{
		float	t1 = r;
		PointF	p1 = curve1.GetPoint(t1);
		float	t2 = curve2.GetTimeOf(p1);
		if (flip)
		{
			std::swap(curve1, curve2);
			std::swap(t1, t2);
		}
		if (t2 != -1)
		{
			AddLocation(curve1, curve2, t1, t2);
		}
	}
}

int CGraphicsPathClip::AddCurveIntersection(Curve curve1, Curve curve2, bool flip,
											int recursion, int calls, float tMin,
											float tMax, float uMin, float uMax)
{
	if (++calls >= 4096 || ++recursion >= 40)
		return calls;
	
	std::vector<float>	x1 = curve1.GetXValues(),
						y1 = curve1.GetYValues(),
						x2 = curve2.GetXValues(),
						y2 = curve2.GetYValues();
	float	d1 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x2[1], y2[1]),
			d2 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x2[2], y2[2]),
			factor = d1 * d2 > 0 ? 3 / 4 : 4 / 9,
			dMin = factor * min(0, d1, d2),
			dMax = factor * max(0, d1, d2),
			dp0 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[0], y1[0]),
			dp1 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[1], y1[1]),
			dp2 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[2], y1[2]),
			dp3 = getSignedDistance(x2[0], y2[0], x2[3], y2[3], x1[3], y1[3]);
	std::vector<std::vector<PointF>> hull = getConvexHull(dp0, dp1, dp2, dp3);
	std::vector<PointF>	top = hull[0],
						bottom = hull[1],
						reverseBottom = {bottom[1], bottom[0]},
						reverseTop;
	if (top.size() == 3)
		reverseTop = {top[2], top[1], top[0]};
	else
		reverseTop = {top[3], top[2], top[1], top[0]};
	float tMinClip, tMaxClip;
	if (d1 == 0 && d2 == 0 && dp0 == 0 && dp1 == 0 && dp2 == 0 && dp3 == 0 ||
		(tMinClip = clipConvexHull(top, bottom, dMin, dMax)) == FLT_MIN ||
		(tMaxClip = clipConvexHull(reverseTop, reverseBottom, dMin, dMax)) == FLT_MIN)
		return calls;
	
	float	tMinNew = tMin + (tMax - tMin) * tMinClip,
			tMaxNew = tMin + (tMax - tMin) * tMaxClip;
	
	if (std::max(uMax - uMin, tMaxNew - tMinNew) < LINE_EPSILON)
	{
		float	t = (tMinNew + tMaxNew) / 2,
				u = (uMin + uMax) / 2;
		Curve	c1 = flip ? curve2 : curve1,
				c2 = flip ? curve1 : curve2;
		float	t1 = flip ? u : t,
				t2 = flip ? t : u;
		AddLocation(c1, c2, t1, t2);
	}
	else
	{
		Curve curve = curve1.GetPart(tMinClip, tMaxClip);
		float uDiff = uMax - uMin;

		if (tMaxClip - tMinClip > 0.8)
		{
			if (tMaxNew - tMinNew > uDiff)
			{
				std::vector<Curve> parts = curve1.Subdivide(0.5);
				float t = (tMinNew + tMaxNew) / 2;
				calls = AddCurveIntersection(curve2, parts[0], !flip, recursion,
											calls, uMin, uMax, tMinNew, t);
				calls = AddCurveIntersection(curve2, parts[1], !flip, recursion,
											calls, uMin, uMax, t, tMaxNew);
			}
			else
			{
				std::vector<Curve> parts = curve2.Subdivide(0.5);
				float u = (uMin + uMax) / 2;
				calls = AddCurveIntersection(parts[0], curve1, !flip, recursion,
											calls, uMin, u, tMinNew, tMaxNew);
				calls = AddCurveIntersection(parts[1], curve1, !flip, recursion,
											calls, u, uMax, tMinNew, tMaxNew);
			}
		}
		else
		{
			if (uDiff == 0 || uDiff >= LINE_EPSILON)
				calls = AddCurveIntersection(curve1, curve2, flip, recursion,
											calls, uMin, uMax, tMinNew, tMaxNew);
			else
				calls = AddCurveIntersection(curve1, curve2, flip, recursion,
											calls, tMinNew, tMaxNew, uMin, uMax);
		}
	}
	return calls;
}

void CGraphicsPathClip::DivideLocations()
{
	std::vector<Curve> clearCurves;
	bool	ClearHandles = false;
	Curve	prevCurve;
	float	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin,
			prevTime = -1.0;

	for (size_t i = Locations.size() - 1; i >= 0; i--)
	{
		Location loc = Locations[i];
		float	origTime = loc.Time,
				time = loc.Time;
		Curve	curve = loc.C;
		Segment	segment;
		Curve	newCurve;

		if (curve != prevCurve)
		{
			ClearHandles = !curve.HasHandle();
			prevTime = -1.0;
			prevCurve = curve;
		}
		else if (prevTime >= tMin)
		{
			time /= prevTime;
		}

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
			newCurve = curve.Segment1.Id == 1 ?
						curve.DivideAtTime(time, Segments1, Curves1) :
						curve.DivideAtTime(time, Segments2, Curves2);
			if (ClearHandles)
			{
				clearCurves.push_back(curve);
				clearCurves.push_back(newCurve);
			}
			segment = newCurve.Segment1;
		}
		loc.S = segment;
		Location	*inter = segment.Inters,
					*dest = loc.Inters;
		if (inter != nullptr)
		{
			LinkIntersection(inter, dest);
			Location* other = inter;
			while (other != nullptr)
			{
				LinkIntersection(other->Inters, inter);
				other = other->Next;
			}
		}
		else
			segment.Inters = dest;
	}
	ClearCurveHandles(clearCurves);
}

void CGraphicsPathClip::InsertLocation(Location loc)
{
	size_t	length = Locations.size(),
			l = 0,
			r = length - 1;
	while (l <= r)
	{
		size_t mid = (l + r) >> 1;
		Location loc2 = Locations[mid];
		if (loc == loc2)
			return;
		float diff = loc.C.Segment1.Id != loc2.C.Segment1.Id ? 
					(loc.C.Segment1.Id - loc2.C.Segment1.Id) :
					((loc.C.Segment1.Id + loc.Time) - 
					 (loc2.C.Segment1.Id + loc2.Time));
		if (diff < 0)
			r = mid - 1;
		else
			l = mid + 1;
	}
	Locations.insert(Locations.begin() + l, loc);
}

void CGraphicsPathClip::AddLocation(Curve curve1, Curve curve2,
									int t1, int t2, bool overlap)
{
	bool excludeStart = !overlap && 
						GetPreviousCurve(curve1) == curve2,
		 excludeEnd = !overlap && curve1 != curve2 && 
						GetNextCurve(curve1) == curve2;
	float	tMin = CURVETIME_EPSILON,
			tMax = 1 - tMin;

	if (t1 >= (excludeStart ? tMin : 0) &&
		t1 <= (excludeEnd ? tMax : 1))
	{
		if (t2 >= (excludeEnd ? tMin : 0) &&
			t2 <= (excludeStart ? tMax : 1))
		{
			Location	*loc1 = new Location(curve1, t1, overlap),
						*loc2 = new Location(curve2, t2, overlap);
			loc1->Inters = new Location;
			loc1->Inters = loc2;
			loc2->Inters = new Location;
			loc2->Inters = loc1;
			if (FilterIntersections(*loc1))
				InsertLocation(*loc1);
		}
	}
}

std::unordered_map<std::string, float> CGraphicsPathClip::InicializeMap()
{
	std::unordered_map<std::string, float> result = {
		std::pair<std::string, float>("winding", 0.0),
		std::pair<std::string, float>("quality", 1.0),
		std::pair<std::string, float>("windingL", 0.0),
		std::pair<std::string, float>("windingR", 0.0),
		std::pair<std::string, float>("onPath", 0.0)
	};
	return result;
}

std::unordered_map<std::string, float>* CGraphicsPathClip::InicializeMap(
	float x, 
	float y)
{
	std::unordered_map<std::string, float>* map =
		new std::unordered_map<std::string, float>;
					
	*map = {
			std::pair<std::string, float>("quality", 1.0),
			std::pair<std::string, float>("windingL", 0.0),
			std::pair<std::string, float>("windingR", 0.0),
			std::pair<std::string, float>("pathWindingL", 0.0),
			std::pair<std::string, float>("pathWindingR", 0.0),
			std::pair<std::string, float>("pointAbscis", x),
			std::pair<std::string, float>("pointOrdinat", y),
			std::pair<std::string, float>("paL", x - WINDING_EPSILON),
			std::pair<std::string, float>("paR", x + WINDING_EPSILON),
	};
	return map;
}

std::vector<Curve>*** CGraphicsPathClip::InicializeMap(std::vector<std::vector<int>> collisions)
{
	std::vector<Curve>*** map;
	map = new std::vector<Curve>**[2];
	map[0] = new std::vector<Curve>*[Curves1.size()];
	map[1] = new std::vector<Curve>*[Curves2.size()];

	for (size_t i = 0; i < Curves.size(); i++)
	{
		Curve curve = Curves[i];
		size_t id = curve.Segment1.Id;
		map[id - 1][curve.Segment1.Index] = new std::vector<Curve>[2];
		map[id - 1][curve.Segment1.Index][0] = GetCurves(collisions[2 * i]);
		map[id - 1][curve.Segment1.Index][1] = GetCurves(collisions[2 * i + 1]);
	}
	return map;
}

void CGraphicsPathClip::ClearMap(std::vector<Curve>*** map)
{
	for (size_t i = 0; i < Curves1.size(); i++)
	{
		delete[] map[0][i];
	}
	
	for (size_t i = 0; i < Curves2.size(); i++)
	{
		delete[] map[1][i];
	}
	delete[] map[0];
	delete[] map[1];
	delete[] map;
}

void CGraphicsPathClip::ClearCurveHandles(std::vector<Curve> curves)
{
	for (auto& c : curves)
		c.ClearHandles();
}

void CGraphicsPathClip::AddOffsets(std::vector<float>& offsets,
								   Curve curve, bool end)
{
	std::vector<float> roots = curve.GetPeeks();
	size_t	count = roots.size();
	bool	first = end && count != 0,
			second = !end && count != 0;
	float	offset = curve.GetLength(first ? roots[count - 1] : 0,
									second ? roots[0] : 1);
	offsets.push_back(count != 0 ? offset : offset / 32);
}

void CGraphicsPathClip::PropagateWinding(Segment segment,
										std::vector<Curve>*** curveCollisonsMap)
{
	std::vector<std::tuple<Segment, Curve, float>> chain;
	Segment start = segment;
	float totalLength = 0;
	std::unordered_map<std::string, float> winding = InicializeMap();

	do
	{
		Curve curve = GetCurve(segment);
		if (curve != Curve())
		{
			float length = curve.GetLength();
			chain.push_back({segment, curve, length});
			totalLength += length;
		}
		segment = GetNextSegment(segment);
	} while (segment.Inters == nullptr && segment != start);

	float	offsets[3] = {0.5, 0.25, 0.75},
			tMin = 1e-3,
			tMax = 1 - tMin;

	for (size_t i = 0; i < 3 && winding["quality"] < 0.5; i++)
	{
		float length = totalLength * offsets[i];
		for (size_t j = 0; j < chain.size(); j++)
		{
			std::tuple<Segment, Curve, float> entry = chain[j];
			float curveLength = std::get<2>(entry);
			std::unordered_map<std::string, float> wind = InicializeMap();

			if (length < curveLength)
			{
				Curve	curve = std::get<1>(entry);
				size_t	id = curve.Segment1.Id;
				float	t = clamp(curve.GetTimeAt(length), tMin, tMax);
				PointF	pt = curve.GetPoint(t);
				bool	dir = abs(curve.GetTangent(t).Y) < sqrt(2);
				float	pv[2] = {pt.X, pt.Y};
				bool	onPath = false,
						onAnyPath = false;
					
				std::vector<float> roots;
				std::unordered_map<std::string, float>* map =
					InicializeMap(pv[dir ? 1 : 0], pv[dir ? 0 : 1]);
				Curve* cPrev = nullptr;
				
				if (Op == Subtraction)
				{
					GetWinding(id == 1 ? Curves1 : Curves2, dir,
								onPath, onAnyPath, cPrev, roots, map);
					float windPath = (std::max((*map)["pathWindingL"], 
												(*map)["pathWindingR"]));

					if (id == 1 && windPath != 0.0 ||
						id == 2 && windPath == 0.0)
					{
						if ((*map)["quality"] < 1.0)
							continue;
						else
						{
							wind["winding"] = 0.0;
							wind["quality"] = 1.0;
						}
					}

					roots.clear();
					onPath = false;
					onAnyPath = false;
					cPrev = nullptr;
					delete map;
				}

				std::vector<Curve> newCurves; 
				newCurves = curveCollisonsMap[id - 1][curve.Segment1.Index][0];
				std::copy(
					curveCollisonsMap[id - 1][curve.Segment1.Index][1].begin(), 
					curveCollisonsMap[id - 1][curve.Segment1.Index][1].end(),
					std::back_inserter(newCurves));
				
				map = InicializeMap(pv[dir ? 1 : 0], pv[dir ? 0 : 1]);
				GetWinding(newCurves, dir, onPath, onAnyPath, cPrev, roots, map);
				
				wind["winding"] = std::max((*map)["windingL"], (*map)["windingR"]);
				wind["quality"] = (*map)["quality"];
				wind["windingL"] = (*map)["windingL"];
				wind["windingR"] = (*map)["windingR"];
				wind["onPath"] = onAnyPath ? 1.0 : 0.0;

				if (wind["quality"] > winding["quality"])
					winding = wind;
				break;
			}
			length -= curveLength;
		}
	}

	for (size_t j = chain.size() - 1; j >= 0; j--)
		std::get<0>(chain[j]).Winding = winding;
}

void CGraphicsPathClip::Collect(Segment seg, std::vector<Segment>& crossing,
								std::vector<Segment>& starts, Location* inter,
								Location* end, bool collectStarts)
{
	while (inter != nullptr, inter != end)
	{
		Segment other = inter->S,
				next;
		size_t last = other.Id == 1 ? Segments1.size() - 1 
									: Segments2.size() - 1;
		if (other.Index == last)
			next = other.Id == 1 ? Segments1[0] : Segments2[0];
		else
			next = GetNextSegment(other);

		Location* nextInter = next.Inters;

		if (other != seg && other.Index == 0 || 
			next.Index == 0 || 
			other.IsValid(Op) && next.IsValid(Op) ||
			nextInter != nullptr && nextInter->S.IsValid(Op))
			crossing.push_back(other);
		if (collectStarts)
			starts.push_back(other);
	}
	inter = inter->Next;
}

bool CGraphicsPathClip::GetWinding(std::vector<Curve> curves, bool dir,
								   bool& onPath, bool& onAnyPath, Curve *cPrev, 
								   std::vector<float>& roots,
								   std::unordered_map<std::string, float>* map)
{
	int ia = dir ? 1 : 0,
		io = ia ^ 1;

	for (size_t i = 0; i < curves.size(); i++)
	{
		Curve c = curves[i];
		size_t id = c.Segment1.Id;
		bool res;

		if (i == 0 || curves[i - 1].Segment1.Id != id)
		{
			cPrev = nullptr;
			if (cPrev == nullptr)
			{
				*cPrev = c;
				Curve prev = curves[curves.size() - 1];
				while (prev != c)
				{
					std::vector<float> v = prev.GetXValues(),
									   y = prev.GetYValues();
					std::copy(y.begin(), y.end(), std::back_inserter(v));

					if (v[io] != v[io + 6])
					{
						*cPrev = prev;
						break;
					}
					if (prev.Segment1.Index == 0)
						break;
					prev = curves[prev.Segment1.Index - 1];
				}
			}
		}

		if (res = c.HandleCurve(curves, dir, onPath, onAnyPath,
								cPrev, roots, map))
			return res;

		if (i + 1 == curves.size() || curves[i + 1].Segment1.Id != id)
			if (onPath && (*map)["pathWindingL"] == 0.0 && (*map)["pathWindingR"])
				(*map)["pathWindingL"] = 
					(*map)["pathWindingR"] = 
						c.Segment1.Path->IsClockwise() ^ dir ? 1 : -1;
		
		(*map)["windingL"] += (*map)["pathWindingL"];
		(*map)["windingR"] += (*map)["pathWindingR"];
		(*map)["pathWindingL"] = (*map)["pathWindingR"] = 0.0;
		if (onPath)
		{
			onAnyPath = true;
			onPath = false;
		}
	}

	(*map)["windingL"] = abs((*map)["windingL"]);
	(*map)["windingR"] = abs((*map)["windingR"]);
	
	return true;
}
}
