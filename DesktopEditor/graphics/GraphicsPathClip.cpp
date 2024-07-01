#include "GraphicsPathClip.h"

namespace Aggplus {
Segment::Segment() : 
	P0(PointF()), 
	H0(PointF()), 
	H1(PointF()), 
	P1(PointF())
{

}

Segment::Segment(std::vector<PointF> points, bool isCurve, size_t index, size_t path) : 
	IsCurve(isCurve), 
	Index(index), 
	Path(path) 
{
	if (IsCurve)
	{
		P0 = points[0];
		H0 = points[1];
		H1 = points[2];
		P1 = points[3];
	}
	else
	{
		P0 = points[0];
		H0 = points[0];
		H1 = points[1];
		P1 = points[1];
	}
}

Segment::Segment(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) :
	IsCurve(true)
{
	P0 = PointF(x0, y0);
	H0 = PointF(x1, y1);
	H1 = PointF(x2, y2);
	P1 = PointF(x3, y3);
}

Segment::Segment(PointF p0, PointF h0, PointF h1) : 
	P0(p0), 
	H0(h0), 
	H1(h1)
{

}

Segment::Segment(Segment seg1, Segment seg2) : 
	IsCurve(true)
{
	P0 = seg1.P0;
	H0 = seg1.H0;
	H1 = seg2.H1;
	P1 = seg2.P0;
}

PointF Segment::getPoint(float t)
{
	float x[4] = {P0.X, H0.X, H1.X, P1.X},
		  y[4] = {P0.Y, H0.Y, H1.Y, P1.Y};

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
	float cx = 3 * (x[1] - x[0]),
		  bx = 3 * (x[2] - x[1]) - cx,
		  ax = x[3] - x[0] - cx - bx,
		  cy = 3 * (y[1] - y[0]),
		  by = 3 * (y[2] - y[1]) - cy,
		  ay = y[3] - y[0] - cy - by,
		  x0 = t == 0 ? x[0] : t == 1 ? x[3] : ((ax * t + bx) * t + cx) * t + x[0],
		  y0 = t == 0 ? y[0] : t == 1 ? y[3] : ((ay * t + by) * t + cy) * t + y[0];
	return PointF(x0, y0);
}

float Segment::getSquaredLineLength()
{
	float x = P1.X - P0.X,
		y = P1.Y - P0.Y;
	return x * x + y * y;
}

Segment Segment::getPart(float from, float to)
{
	Segment result;
	bool flip = from > to;
	if (flip)
	{
		std::swap(from, to);
	}
	if (from > 0)
		result = subdivide(from)[1];
	if (to < 1)
		result = subdivide((to - from) / (1 - from))[0];
	if (flip)
		result.flip();
	return result;
}

int Segment::getTimeOf(PointF point)
{
	PointF p0 = PointF(P0.X, P0.Y),
		   p3 = PointF(P1.X, P1.Y);

	if (getDistance(point.X, point.Y, p0.X, p0.Y) > GEOMETRIC_EPSILON &&
		getDistance(point.X, point.Y, p3.X, p3.Y) > GEOMETRIC_EPSILON)
	{
		std::vector<float> coords = {point.X, point.Y},
							roots;
		for (size_t c = 0; c < 2; c++)
		{
			int count = solveCubic(c, coords[c], roots, 0.0, 1.0);
			for (int i = 0; i < count; i++)
			{
				float u = roots[i];
				if (u > 1 || u < 0)
					return - 1;
				if (getDistance(point, getPoint(u)) <= GEOMETRIC_EPSILON)
					return u;
			}
		}
	}
	return getDistance(point, p0) <= GEOMETRIC_EPSILON ? 0
		 : getDistance(point, p3) <= GEOMETRIC_EPSILON ? 1
		 : - 1;
}

std::vector<std::pair<int, int>> Segment::getOverlaps(Segment segment)
{
	bool swap = this->getSquaredLineLength() < segment.getSquaredLineLength(),
		 straight1 = !IsCurve,
		 straight2 = !segment.IsCurve,
		 straightBoth = straight1 && straight2;
	float px = swap ? segment.P0.X : P0.X, py = swap ? segment.P0.Y : P0.Y,
		  vx = swap ? segment.P1.X - px : P1.X - px, vy = swap ? segment.P1.Y - py : P1.Y - py,
		  x1[4] = {P0.X, H0.X, H1.X, P1.X},
		  x2[4] = {segment.P0.X, segment.H0.X, segment.H1.X, segment.P1.X},
		  y1[4] = {P0.Y, H0.Y, H1.Y, P1.Y},
		  y2[4] = {segment.P0.Y, segment.H0.Y, segment.H1.Y, segment.P1.Y};
	if (swap)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	if (getDistance(px, py, vx, vy, x2[0], y2[0]) < GEOMETRIC_EPSILON &&
		getDistance(px, py, vx, vy, x2[3], y2[3]) < GEOMETRIC_EPSILON)
	{
		if (!straightBoth &&
			getDistance(px, py, vx, vy, x1[1], y1[1]) < GEOMETRIC_EPSILON &&
			getDistance(px, py, vx, vy, x1[2], y1[2]) < GEOMETRIC_EPSILON &&
			getDistance(px, py, vx, vy, x2[1], y2[1]) < GEOMETRIC_EPSILON &&
			getDistance(px, py, vx, vy, x2[2], y2[2]) < GEOMETRIC_EPSILON)
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
			i2 = i1 ^ 1,
			t1 = i >> 1,
			t2 = i1 == 0 ? getTimeOf(PointF(t1 == 1 ? segment.P1 : segment.P0)) 
					   : segment.getTimeOf(PointF(t1 == 1 ? P1 : P0));
		if (t2 != -1)
		{
			std::pair<int, int> pair = i1 == 1 ? std::pair<int, int>(t1, t2) : std::pair<int, int>(t1, t2);
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
		Segment o1 = getPart(pairs[0].first, pairs[1].first),
				o2 = segment.getPart(pairs[0].second, pairs[1].second);
		if (abs(o2.H0.X - o1.H0.X) > GEOMETRIC_EPSILON ||
			abs(o2.H0.Y - o1.H0.Y) > GEOMETRIC_EPSILON ||
			abs(o2.H1.X - o1.H1.X) > GEOMETRIC_EPSILON ||
			abs(o2.H1.Y - o1.H1.Y) > GEOMETRIC_EPSILON)
			pairs.clear();
	}
	return pairs;
}

Segment Segment::divideAtTime(float time)
{
	float tMin = CURVETIME_EPSILON,
		   tMax = 1 - tMin;
	if (time >= tMin && time <= tMax)
	{
		std::vector<Segment> parts = subdivide(time);
		Segment	left = parts[0],
				right = parts[1];
		Segment subseg1 = Segment(P0, H0, PointF()),
				subseg2 = Segment(P1, PointF(), H1);
		subseg1.H0 = PointF(left.H0.X - left.P0.X, left.H0.Y - left.P0.Y);
		subseg2.H1 = PointF(right.H1.X - right.P1.X, right.H1.Y - right.P1.Y);
		float x = left.P1.X, y = left.P1.Y;
		Segment	segment = Segment(PointF(x, y),
								  PointF(left.H1.X - x, left.H1.Y - y),
								  PointF(right.H0.X - x, right.H1.Y - y));
		P1 = segment.P0;
		H1 = segment.H1;
		return Segment(segment, subseg2);
	}
	return Segment();
}

std::vector<Segment> Segment::subdivide(float t)
{
	float x[4] = {P0.X, H0.X, H1.X, P1.X},
		  y[4] = {P0.Y, H0.Y, H1.Y, P1.Y},
		  x2[6], y2[6],
		  u = 1 - t;

	x2[0] = u * x[0] + t * x[1], y2[0] = u * y[0] + t * y[1],
	x2[1] = u * x[1] + t * x[2], y2[1] = u * y[1] + t * y[2],
	x2[2] = u * x[2] + t * x[3], y2[2] = u * y[2] + t * y[3],
	x2[3] = u * x2[0] + t * x2[1], y2[3] = u * y2[0] + t * y2[1],
	x2[4] = u * x2[1] + t * x2[2], y2[4] = u * y2[1] + t * y2[2],
	x2[5] = u * x2[3] + t * x2[4], y2[5] = u * y2[3] + t * y2[4];

	return {Segment(x[0], y[0], x2[0], y2[0], x2[3], y2[3], x2[5], y2[5]),
			Segment(x2[5], y2[5], x2[4], y2[4], x2[2], y2[2], x[3], y[3])};
}

void Segment::flip()
{
	std::swap(P0, P1);
	std::swap(H0, H1);
}

bool Segment::hasHandle()
{
	return !isZero(H0.X) ||
		   !isZero(H0.Y) ||
		   !isZero(H1.X) ||
		   !isZero(H1.Y);
}

int Segment::solveCubic(size_t coord, int value, std::vector<float>& roots, float mn, float mx)
{
	int count = 0;
	float v[4];
	v[0] = coord == 0 ? P0.X : P0.Y;
	v[1] = coord == 0 ? H0.X : H0.Y;
	v[2] = coord == 0 ? H1.X : H1.Y;
	v[3] = coord == 0 ? P1.X : P1.Y;

	if (!(v[0] < value && v[3] < value && v[1] < value && v[2] < value ||
		  v[0] > value && v[3] > value && v[1] > value && v[2] > value))
	{
		float c = 3 * (v[1] - v[0]),
			   b = 3 * (v[2] - v[1]) - c,
			   a = v[3] - v[0] - c - b,
			   d = v[0] - value,
			   f = max(abs(a), abs(b), abs(c), abs(d));
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
			x = DBL_MAX;
		}
		else if (abs(d) <EPSILON)
		{
			b1 = b;
			c2 = c;
			x= 0;
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
	}
	return count;
}

bool Segment::operator==(const Segment& other)
{
	return P0.Equals(other.P0) && H0.Equals(other.H0) && H1.Equals(other.H1) && P1.Equals(other.P1);
}

bool Segment::operator!=(const Segment& other)
{
	return !operator ==(other);
}

Location::Location() : 
	Seg(Segment())
{

}

Location::Location(Segment segment, float time, bool overlap) :
	Seg(segment),
	Time(time),
	Overlap(overlap)
{

}

bool Location::hasOverlap()
{
	//TODO
}

bool Location::isCrossing()
{
	//TODO
}

bool Location::filterIntersections()
{
	//TODO: filter intersection with overlap or crossing
}

bool Location::equals(const Location& other)
{
	//TODO
}

bool Location::operator==(const Location& other)
{
	return Seg == other.Seg && Time == other.Time && Overlap == other.Overlap;
}

CGraphicsPathClip::CGraphicsPathClip(CGraphicsPath path1, CGraphicsPath path2, BooleanOpType op) : Path1(path1), Path2(path2), Op(op)
{
	traceBoolean();
}

CGraphicsPath CGraphicsPathClip::getResult()
{
	return Result;
}

void CGraphicsPathClip::traceBoolean()
{
	if ((Op == Subtraction || Op == Exclusion) && Path1.isClockwise() && Path2.isClockwise())
		Path2.Reverse();

	Segments1 = getSegments(Path1, 1);
	Segments2 = getSegments(Path2, 2);
	getIntersection();
	for (size_t i = Locations.size() - 1; i >=0; i--)
	{
		insertLocation(Locations[i].Seg.Intersects);
	}

	std::vector<Location> crossing = divideLocations();
	if (crossing.empty())
		crossing = Locations;

	if (!crossing.empty())
	{
		std::vector<Segment> curves;
		for (size_t i = 0; i < Segments1.size(); i++)
			if (Segments1[i].IsCurve)
				curves.push_back(Segments1[i]);
		for (size_t i = 0; i < Segments2.size(); i++)
			if (Segments2[i].IsCurve)
				curves.push_back(Segments2[i]);
		std::vector<std::vector<int>> curveCollisons = findSegmentBoundsCollisions(curves, curves, 0, true);
		for (size_t i = 0; i < curves.size(); i++)
		{
			Segment curve = curves[i];
			size_t id = curve.Path;
		}
	}
}

void CGraphicsPathClip::createResult()
{
	//TODO
}

Segment CGraphicsPathClip::getPreviousSegment(Segment segment)
{
	if(segment.Path == 1)
	{
		if (Path1.Is_poly_closed() && segment.Index == 0)
			return Segments1[Segments1.size() - 1];
		return Segments1[segment.Index - 1];
	}
	else
	{
		if (Path2.Is_poly_closed() && segment.Index == 0)
			return Segments2[Segments2.size() - 1];
		return Segments2[segment.Index - 1];
	}
}

Segment CGraphicsPathClip::getNextSegment(Segment segment)
{
	if(segment.Path == 1)
	{
		if (Path1.Is_poly_closed() && segment.Index == Segments1.size() - 1)
			return Segments1[0];
		return Segments1[segment.Index + 1];
	}
	else
	{
		if (Path2.Is_poly_closed() && segment.Index == Segments2.size() - 1)
			return Segments2[0];
		return Segments2[segment.Index + 1];
	}
}

std::vector<Segment> CGraphicsPathClip::getSegments(CGraphicsPath path, size_t id)
{
	std::vector<Segment> segments;
	for (size_t i = 0; i < path.GetPointCount(); i++)
	{
		bool isCurve = path.isCurvePoint(i + 1);
		std::vector<PointF> points = path.getPoints(i, isCurve ? 4 : 2);
		segments.push_back(Segment(points, isCurve, i, id));
	}
	return segments;
}

std::vector<std::vector<float>> CGraphicsPathClip::getBoundsForSegments(std::vector<Segment> segments)
{
	std::vector<std::vector<float>> bounds;
	for (size_t i = 0; i < segments.size(); i++)
	{
		if (segments[i].IsCurve)
			bounds.push_back({min(segments[i].P0.X, segments[i].P1.X, segments[i].H0.X, segments[i].H1.X),
							  min(segments[i].P0.Y, segments[i].P1.Y, segments[i].H0.Y, segments[i].H1.Y),
							  max(segments[i].P0.X, segments[i].P1.X, segments[i].H0.X, segments[i].H1.X),
							  max(segments[i].P0.Y, segments[i].P1.Y, segments[i].H0.Y, segments[i].H1.Y)});
		else
			bounds.push_back({std::min(segments[i].P0.X, segments[i].P1.X),
							  std::min(segments[i].P0.Y, segments[i].P1.Y),
							  std::max(segments[i].P0.X, segments[i].P1.X),
							  std::max(segments[i].P0.Y, segments[i].P1.Y)});
	}
	return bounds;
}

std::vector<std::vector<int>> CGraphicsPathClip::findSegmentBoundsCollisions(std::vector<Segment> segments1,
																			std::vector<Segment> segments2,
																			float tolerance,
																			bool bothAxis)
{
	std::vector<std::vector<float>> bounds1 = getBoundsForSegments(segments1);
	std::vector<std::vector<float>> bounds2 = getBoundsForSegments(segments2);

	if (bothAxis)
	{
		std::vector<std::vector<int>> hor = findBoundsCollisions(bounds1, bounds2, tolerance, false, true),
									  ver = findBoundsCollisions(bounds1, bounds2, tolerance, true, true),
									  list;
		for (size_t i = 0; i < hor.size(); i++)
		{
			list.push_back(hor[i]);
			list.push_back(ver[i]);
		}
		return list;
	}
	return findBoundsCollisions(bounds1, bounds2, tolerance);
}

std::vector<std::vector<int>> CGraphicsPathClip::findBoundsCollisions(std::vector<std::vector<float>> bounds1,
																	  std::vector<std::vector<float>> bounds2,
																	  float tolerance,
																	  bool sweepVertical,
																	  bool onlySweep)
{
	std::vector<std::vector<float>> allBounds(bounds1);
	std::copy(bounds2.begin(), bounds2.end(), std::back_inserter(allBounds));
	size_t allLength = allBounds.size(),
		   length1 = bounds1.size();

	size_t pri1 = sweepVertical ? 1 : 0,
		   pri2 = pri1 + 2,
		   sec1 = sweepVertical ? 0 : 1,
		   sec2 = sec1 + 2;


	std::vector<int> allIdicesByPri1;
	for (size_t i = 0; i < allLength; i++)
	{
		allIdicesByPri1[i] = i;
	}
	std::sort(allIdicesByPri1.begin(), allIdicesByPri1.end(), [&allBounds, &pri1](int i1, int i2){
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
			size_t pruneCount = binarySearch(allBounds, activeIndicesByPri2, pri2, curBounds[pri1] - GEOMETRIC_EPSILON) + 1;
			activeIndicesByPri2.erase(activeIndicesByPri2.begin(), activeIndicesByPri2.begin() + pruneCount);
			if (onlySweep)
			{
				curCollisions.insert(curCollisions.end(), activeIndicesByPri2.begin(), activeIndicesByPri2.end());
				for (size_t j = 0; j < activeIndicesByPri2.size(); j++)
				{
					int activeIndex = activeIndicesByPri2[j];
					allCollisions[activeIndex].push_back(origIndex);
				}
			}
			else
			{
				float curSec1 = curBounds[sec2];
				float curSec2 = curBounds[sec1];
				for (int j = 0; j < activeIndicesByPri2.size(); j++)
				{
					int activeIndex = activeIndicesByPri2[j];
					std::vector<float> activeBounds = allBounds[activeIndex];
					bool isActive1 = activeIndex < length1,
						isActive2 = !isActive1;
					if (onlySweep || (isCurrent1 && isActive2 || isCurrent2 && isActive1)
						&& (curSec2 >= activeBounds[sec1] - tolerance
							&& curSec1 <= activeBounds[sec2] + tolerance))
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
	for (auto c : allCollisions)
	{
		if (!c.empty())
			std::sort(c.begin(), c.end());
	}
	return allCollisions;
}

bool CGraphicsPathClip::intersectsBounds()
{
	RectF_T<double> rect1, rect2;
	Path1.GetBounds(rect1.X, rect1.Y, rect1.Width, rect1.Height);
	Path2.GetBounds(rect2.X, rect2.Y, rect2.Width, rect2.Height);

	return (rect2.X + rect2.Width > rect1.X - EPSILON)
		   && (rect2.Y + rect2.Height > rect1.Y - EPSILON)
		   && (rect2.X < rect1.X + rect1.Width + EPSILON)
		   && (rect2.Y < rect1.Y + rect2.Height + EPSILON);
}

void CGraphicsPathClip::getIntersection()
{
	if (!intersectsBounds())
		return;

	size_t length1 = Segments1.size();

	std::vector<std::vector<int>> boundsCollisions = findSegmentBoundsCollisions(Segments1, Segments2, GEOMETRIC_EPSILON);
	for (size_t index1 = 0; index1 < length1; index1++)
	{
		Segment segment1 = Segments1[index1];
		std::vector<int> collisions1 = boundsCollisions[index1];
		if (!collisions1.empty())
		{
			for (size_t j = 0; j < collisions1.size(); j++)
			{
				int index2 = collisions1[j];
				if (index2 > index1)
				{
					Segment segment2 = Segments2[index2];
					getSegmentIntersection(segment1, segment2);
				}
			}
		}
	}
}

void CGraphicsPathClip::getSegmentIntersection(Segment segment1, Segment segment2)
{
	float minX1 = min(segment1.P0.X, segment1.H0.X, segment1.H1.X, segment1.P1.X),
		   maxX1 = max(segment1.P0.X, segment1.H0.X, segment1.H1.X, segment1.P1.X),
		   minY1 = min(segment1.P0.Y, segment1.H0.Y, segment1.H1.Y, segment1.P1.Y),
		   maxY1 = max(segment1.P0.Y, segment1.H0.Y, segment1.H1.Y, segment1.P1.Y),
		   minX2 = min(segment2.P0.X, segment2.H0.X, segment2.H1.X, segment2.P1.X),
		   maxX2 = max(segment2.P0.X, segment2.H0.X, segment2.H1.X, segment2.P1.X),
		   minY2 = min(segment2.P0.Y, segment2.H0.Y, segment2.H1.Y, segment2.P1.Y),
		   maxY2 = max(segment2.P0.Y, segment2.H0.Y, segment2.H1.Y, segment2.P1.Y);

	if (maxX1 + EPSILON > minX2 && minX1 - EPSILON < maxX2
		&& maxY1 + EPSILON > minY2 && minY1 - EPSILON < maxY2)
	{
		std::vector<std::pair<int, int>> overlaps = segment1.getOverlaps(segment2);
		if (!overlaps.empty())
		{
			for (size_t i = 0; i < 2; i++)
			{
				std::pair<int, int> overlap = overlaps[i];
				addLocation(segment1, segment2, overlap.first, overlap.second, true);
			}
		}
		else
		{
			bool straight1 = !segment1.IsCurve,
				 straight2 = !segment2.IsCurve,
				 straight = straight1 && straight2,
				 flip = straight1 && !straight2;
			size_t before = Locations.size();
			if (straight)
				addLineIntersection(flip ? segment2 : segment1, flip ? segment1 : segment2, flip);
			else if (straight1 || straight2)
				addCurveLineIntersection(flip ? segment2 : segment1, flip ? segment1 : segment2, flip);
			else
				addCurveIntersection(flip ? segment2 : segment1, flip ? segment1 : segment2, flip);
			if (!straight || Locations.size() == before)
			{
				for (size_t i = 0; i < 4; i++)
				{
					int t1 = i >> 1, // 0, 0, 1, 1
						t2 = i & 1;  // 0, 1, 0, 1
					float x1[2] = {segment1.P0.X, segment1.P1.X},
						  y1[2] = {segment1.P0.Y, segment1.P1.Y},
						  x2[2] = {segment2.P0.X, segment2.P1.X},
						  y2[2] = {segment2.P0.Y, segment2.P1.Y};
					PointF p1 = PointF(x1[t1], y1[t1]),
						   p2 = PointF(x2[t2], y2[t2]);
					if (getDistance(p1, p2) <= EPSILON) {
						addLocation(segment1, segment2, t1, t2, false);
					}
				}
			}
		}
	}
}

void CGraphicsPathClip::linkIntersection(Location inter, Location dest)
{
	//TODO
}

void CGraphicsPathClip::addLineIntersection(Segment segment1, Segment segment2, bool flip)
{
	//TODO
}

void CGraphicsPathClip::addCurveLineIntersection(Segment segment1, Segment segment2, bool flip)
{
	//TODO
}

void CGraphicsPathClip::addCurveIntersection(Segment segment1, Segment segment2, bool flip)
{
	//TODO
}

std::vector<Location> CGraphicsPathClip::divideLocations()
{
	std::vector<Location> result,
						  renormalizedLoc;
	bool clearHandles = false;
	Segment prevSegment;
	float tMin = CURVETIME_EPSILON,
		   tMax = 1 - CURVETIME_EPSILON,
		   prevTime = -1.0;

	for (size_t i = Locations.size() - 1; i >= 0; i--)
	{
		Location loc = Locations[i];
		float origTime = loc.Time,
			   time = loc.Time;
		bool exclude = !loc.filterIntersections();
		Segment segment = loc.Seg;
		Segment subseg;
		if (segment.IsCurve)
		{
			if (segment != prevSegment)
			{
				clearHandles = !segment.hasHandle();

				prevTime = -1.0;
				if (!renormalizedLoc.empty())
					renormalizedLoc.clear();
				prevSegment = segment;
			}
			else if (prevTime >= tMin)
			{
				time /= prevTime;
			}
		}
		if (exclude)
		{
			if (!renormalizedLoc.empty())
				renormalizedLoc.push_back(loc);
			continue;
		}
		else
			result.insert(result.begin(), loc);

		prevTime = origTime;
		if (time < tMin)
		{
			subseg = Segment(segment.P0, segment.H0, PointF());
		}
		else if (time > tMax)
		{
			subseg = Segment(segment.P1, PointF(), segment.H1);
		}
		else
		{
			Segment newSegment = segment.divideAtTime(time),
					sudseg = Segment(newSegment.P0, newSegment.H0, PointF());
			for (size_t j = renormalizedLoc.size() - 1; j >= 0; j--)
			{
				Location loc = renormalizedLoc[j];
				loc.Time = (loc.Time - time) / (1 - time);
			}
		}
		loc.Seg = segment;
		Location inter = subseg.Intersects,
				 dest = loc.Seg.Intersects;
		if (!inter.equals(Location()))
		{
			linkIntersection(inter, dest);
			Location other = inter;

		}
		else
			subseg.Intersects = dest;
	}
	return result;
}

void CGraphicsPathClip::insertLocation(Location loc)
{
	size_t length = Locations.size(),
		   l = 0,
		   r = length - 1;
	while (l <= r)
	{
		size_t mid = (l + r) >> 1;
		Location loc2 = Locations[mid];
		if (loc.equals(loc2) /*? loc2 : (search(mid, -1) || search(mid, 1))*/)
			return;
		float diff = loc.Seg.Path != loc2.Seg.Path ? (loc.Seg.Path - loc2.Seg.Path)
													: ((loc.Seg.Index + loc.Time) - (loc2.Seg.Index + loc2.Time));
		if (diff < 0)
		{
			r = mid - 1;
		}
		else
		{
			l = mid + 1;
		}
	}
	Locations.insert(Locations.begin() + l, loc);
}

void CGraphicsPathClip::addLocation(Segment segment1, Segment segment2, int t1, int t2, bool overlap)
{
	bool excludeStart = !overlap && getPreviousSegment(segment1) == segment2,
		 excludeEnd = !overlap && segment1 != segment2 && getNextSegment(segment1) == segment2;
	float tMin = CURVETIME_EPSILON,
		   tMax = 1 - CURVETIME_EPSILON;

	if (t1 >= (excludeStart ? tMin : 0) &&
		t1 <= (excludeEnd ? tMax : 1))
	{
		if (t2 >= (excludeEnd ? tMin : 0) &&
			t2 <= (excludeStart ? tMax : 1))
		{
			Location loc1 = Location(segment1, t1, overlap),
					 loc2 = Location(segment2, t2, overlap);
			loc1.Seg.Intersects = loc2;
			loc2.Seg.Intersects = loc1;
			if (loc1.filterIntersections())
				insertLocation(loc1);
		}
	}
}
}
