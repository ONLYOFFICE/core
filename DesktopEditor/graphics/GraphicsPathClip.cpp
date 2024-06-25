#include "GraphicsPathClip.h"

namespace Aggplus {
Segment::Segment(PointF *points, bool isCurve) : IsCurve(isCurve)
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
		P1 = points[1];
	}
}

CGraphicsPathClip::CGraphicsPathClip(CPathForClip path1, CPathForClip path2, BooleanOpType op) : Path1(path1), Path2(path2), Op(op)
{
	traceBoolean();
}

void CGraphicsPathClip::traceBoolean()
{
	if ((Op == Subtraction || Op == Exclusion) && Path1.isClockwise() && Path2.isClockwise())
		Path2.Reverse();

	Segments1 = getSegments(Path1);
	Segments2 = getSegments(Path2);
}

std::vector<Segment> CGraphicsPathClip::getSegments(CGraphicsPath path)
{
	std::vector<Segment> segments;
	for (size_t i = 0; i < path.GetPointCount(); i++)
	{
		bool isCurve = path.isCurvePoint(i + 1);
		PointF* points = path.getPoints(i, isCurve ? 4 : 2);
		segments.push_back(Segment(points, isCurve));
	}
	return segments;
}

bool CGraphicsPathClip::intersetsBounds()
{
    RectF_T<double> rect1, rect2;
	Path1.GetBounds(rect1.X, rect1.Y, rect1.Width, rect1.Height);
	Path2.GetBounds(rect2.X, rect2.Y, rect2.Width, rect2.Height);

	return (rect2.X + rect2.Width > rect1.X - EPSILON)
		   && (rect2.Y + rect2.Height > rect1.Y - EPSILON)
		   && (rect2.X < rect1.X + rect1.Width + EPSILON)
		   && (rect2.Y < rect1.Y + rect2.Height + EPSILON);
}

std::vector<RectF> CGraphicsPathClip::getBoundsForSegments(std::vector<Segment> segments)
{
	std::vector<RectF> bounds;
	for (size_t i = 0; i < segments.size(); i++)
	{
		if (segments[i].IsCurve)
			bounds.push_back(RectF(std::min(std::min(segments[i].P0.X, segments[i].P1.X),
											std::min(segments[i].H0.X, segments[i].H1.X)),
								   std::min(std::min(segments[i].P0.Y, segments[i].P1.Y),
											std::min(segments[i].H0.Y, segments[i].H1.Y)),
								   std::max(std::max(segments[i].P0.X, segments[i].P1.X),
											std::max(segments[i].H0.X, segments[i].H1.X)),
								   std::max(std::max(segments[i].P0.Y, segments[i].P1.Y),
											std::max(segments[i].H0.Y, segments[i].H1.Y))));
		else
			bounds.push_back(RectF(std::min(segments[i].P0.X, segments[i].P1.X),
								   std::min(segments[i].P0.Y, segments[i].P1.Y),
								   std::max(segments[i].P0.X, segments[i].P1.X),
								   std::max(segments[i].P0.Y, segments[i].P1.Y)));
	}
	return bounds;
}

RectF CGraphicsPathClip::findBoundsCollisions()
{
	std::vector<RectF> bounds1 = getBoundsForSegments(Segments1);
	std::vector<RectF> bounds2 = getBoundsForSegments(Segments2);
	std::vector<RectF> allBounds = {bounds1.begin(), bounds1.end(), bounds2.begin(), bounds2.end()};

	size_t primaryAxis1 = 0,
		primaryAxis2 = 2,
		secondaryAxis1 = 1,
		secondaryAxis2 = 3;
}
}
