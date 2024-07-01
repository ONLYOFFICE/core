#include "GraphicsPath.h"
#include "clip_math.h"

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

	size_t Index;
	size_t Path;
	Location Intersects;

	Segment();
	Segment(std::vector<PointF> points, bool isCurve, size_t index, size_t path);
	Segment(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
	Segment(PointF p0, PointF h0, PointF h1);
	Segment(Segment seg1, Segment seg2);

	PointF 	getPoint(float t);
	float 	getSquaredLineLength();
	Segment getPart(float from, float to);
	int		getTimeOf(PointF point);
	std::vector<std::pair<int, int>> getOverlaps(Segment segment);

	Segment divideAtTime(float time);
	std::vector<Segment> subdivide(float t);

	void flip();
	bool hasHandle();
	int	 solveCubic(size_t coord, int value, std::vector<float>& roots, float mn, float mx);

	bool operator==(const Segment& other);
	bool operator!=(const Segment& other);
};

struct Location
{
	Segment Seg;
	float 	Time;
	bool 	Overlap;

	Location();
	Location(Segment segment, float time, bool overlap);

	bool hasOverlap();
	bool isCrossing();

	bool filterIntersections();

	bool equals(const Location& other);
	bool operator==(const Location& other);
};

class GRAPHICS_DECL CGraphicsPathClip
{
public:
	CGraphicsPathClip(CGraphicsPath path1, CGraphicsPath path2, BooleanOpType op);
	CGraphicsPath getResult();

protected:
	//BooleanOp
	void traceBoolean();
	void propagateWinding(Segment segment);
	void createResult();

	//Segments
	Segment getPreviousSegment(Segment segment);
	Segment getNextSegment(Segment segment);
	std::vector<Segment> getSegments(CGraphicsPath path, size_t id);
	
	//Bounds
	std::vector<std::vector<float>> getBoundsForSegments(std::vector<Segment> segments);
	std::vector<std::vector<int>>	findSegmentBoundsCollisions(std::vector<Segment> segments1,
																std::vector<Segment> segments2,
																float tolerance,
																bool bothAxis = false);
	std::vector<std::vector<int>>	findBoundsCollisions(std::vector<std::vector<float>> bounds1,
														std::vector<std::vector<float>> bounds2,
														float tolerance,
														bool sweepVertical = false,
														bool onlySweep = false);

	//Intersection
	bool intersectsBounds();
	void getIntersection();
	void getSegmentIntersection(Segment segment1, Segment segment2);
	void linkIntersection(Location inter, Location dest);
	void addLineIntersection(Segment segment1, Segment segment2, bool flip);
	void addCurveLineIntersection(Segment segment1, Segment segment2, bool flip);
	void addCurveIntersection(Segment segment1, Segment segment2, bool flip);

	//Location
	std::vector<Location> divideLocations();
	void addLocation(Segment segment1, Segment segment2, int t1, int t2, bool overlap);
	void insertLocation(Location loc);
	
	float getDistance(float px, float py, float vx, float vy, float x, float y);
	float getDistance(float x1, float y1, float x2, float y2);
	float getDistance(PointF point1, PointF point2);

private:
	BooleanOpType Op;

	CGraphicsPath Path1;
	CGraphicsPath Path2;
	CGraphicsPath Result;

	std::vector<Segment> Segments1;
	std::vector<Segment> Segments2;
	std::vector<Location> Locations;
};
}
