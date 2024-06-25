#include "GraphicsPath.h"
#include "GraphicsPath_private.h"

#define EPSILON 1e-12
#define GEOMETRIC_EPSILON 1e-7

namespace Aggplus {
enum BooleanOpType
{
	Intersection = 0,
	Union = 1,
	Subtraction = 2,
	Exclusion = 3,
	Division = 4
};

struct Segment
{
	PointF P0;
	PointF H0;
	PointF H1;
	PointF P1;
	bool IsCurve;

	Segment(PointF* points, bool isCurve);
};

class CGraphicsPathClip
{
public:
	CGraphicsPathClip(CGraphicsPath path1, CGraphicsPath path2, BooleanOpType op);

protected:
	void traceBoolean();
	std::vector<Segment> getSegments(CGraphicsPath path);
	bool intersetsBounds();
	std::vector<RectF> getBoundsForSegments(std::vector<Segment> segments);
	RectF findBoundsCollisions();
	std::vector<Segment> getIntersection();

private:
	BooleanOpType Op;

	CGraphicsPath Path1;
	CGraphicsPath Path2;

	std::vector<Segment> Segments1;
	std::vector<Segment> Segments2;
};
}
