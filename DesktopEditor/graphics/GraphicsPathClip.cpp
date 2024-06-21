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

bool CPathForClip::isClockwise()
{
	double area = 0.0;
	for (size_t i = 0; i < GetPointCount(); i++)
	{
		area += getArea(i, isCurvePoint(i + 1));
		if (isCurvePoint(i + 1)) i += 2;
	}
	return area >= 0;
}

void CPathForClip::Reverse()
{
	for (size_t i = 0; i < GetPointCount(); i++)
	{
		if (isCurvePoint(i))
		{
			double x0, y0, x1, y1;
			this->m_internal->m_agg_ps.vertex(i, &x0, &y0);
			this->m_internal->m_agg_ps.vertex(i + 1, &x1, &y1);
			this->m_internal->m_agg_ps.modify_vertex(i, x1, y1);
			this->m_internal->m_agg_ps.modify_vertex(i + 1, x0, y0);
			i += 2;
		}
	}
}

std::vector<Segment> CPathForClip::getSegments()
{
	std::vector<Segment> segments;
	for (size_t i = 0; i < GetPointCount(); i++)
	{
		bool isCurve = isCurvePoint(i + 1);
		PointF* points = getPoints(i, isCurve ? 4 : 2);
		segments.push_back(Segment(points, isCurve));
	}
	return segments;
}

bool CPathForClip::isCurvePoint(size_t idx)
{
	return this->m_internal->m_agg_ps.command(idx) == agg::path_cmd_curve4;
}

PointF* CPathForClip::getPoints(size_t idx, size_t count)
{
	PointF* points = new PointF[count];
	for (size_t i = 0; i < count; i++)
	{
        double x,y;
		this->m_internal->m_agg_ps.vertex(idx + i, &x, &y);
        points[i] = PointF(x, y);
	}
	return points;
}

double CPathForClip::getArea(size_t idx, bool isCurve)
{
	if (isCurve)
	{
		PointF* points = getPoints(idx, 4);
		return 3 * ((points[3].Y - points[0].Y)	* (points[1].X + points[2].X)
					- (points[3].X - points[0].X) * (points[1].Y * points[2].Y)
					+ points[1].Y * (points[0].X - points[2].X)
					- points[1].X * (points[0].Y - points[2].Y)
					+ points[3].Y * (points[2].X + points[0].X / 3)
					- points[3].X * (points[2].Y - points[0].Y / 3)) / 20;
	}

	PointF* points = getPoints(idx, 2);
	return (points[1].Y * points[0].X - points[1].X * points[0].Y) / 20;
}

CGraphicsPathClip::CGraphicsPathClip(CPathForClip path1, CPathForClip path2, BooleanOpType op) : Path1(path1), Path2(path2), Op(op)
{
	traceBoolean();
}

void CGraphicsPathClip::traceBoolean()
{
	if ((Op == Subtraction || Op == Exclusion) && Path1.isClockwise() && Path2.isClockwise())
		Path2.Reverse();

	Segments1 = Path1.getSegments();
	Segments2 = Path2.getSegments();
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
}
