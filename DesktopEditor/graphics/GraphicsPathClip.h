#ifndef GRAPHICSPATHCLIP_H
#define GRAPHICSPATHCLIP_H

#include "GraphicsPath.h"
#include "clip_math.h"
#include <unordered_map>

namespace Aggplus {
enum BooleanOpType
{
	Intersection = 0,
	Union = 1,
	Subtraction = 2,
	Exclusion = 3,
	Division = 4
};

struct Location;
struct Winding;

struct Segment
{
	PointF P;
	PointF HI;
	PointF HO;
	
	bool IsCurve;
	bool Visited;

	size_t Index;
	size_t Id;

	CGraphicsPath *Path;
	std::shared_ptr<Location> Inters;
	std::shared_ptr<Winding> Wind;
	
	Segment();
	Segment(const std::vector<PointF>& points, bool isCurve,
			size_t index, size_t id, CGraphicsPath* path);
	Segment(const PointF& p, const PointF& hi, const PointF& ho);

	bool IsValid(BooleanOpType op);
	bool operator==(const Segment& other);
	bool operator!=(const Segment& other);
}; 
 
struct Curve
{
	Segment Segment1;
	Segment Segment2;
	
	Curve();
	Curve(const Segment& segment1, const Segment& segment2);
	Curve(const std::vector<float>& values);
	Curve(float x0, float y0, float x1, float y1,
		  float x2, float y2, float x3, float y3);

	std::vector<float> GetXValues() const;
	std::vector<float> GetYValues() const;
	std::vector<float> GetPeeks() const;

	float	GetLength(float a = 0, float b = 1) const;
	float	GetSquaredLineLength() const;
	float	GetTimeOf(const PointF& point) const ;
	float	GetTimeAt(float offset) const;
	PointF	Get(float t, int type) const;
	PointF	GetPoint(float t) const;
	PointF	GetTangent(float t) const;
	PointF	GetTangent(float t, float offset, bool inside, const PointF& p) const;
	Curve	GetPart(float from, float to) const;
	std::vector<Curve> GetMonoCurves(bool dir) const;
	std::vector<std::pair<int, int>> GetOverlaps(const Curve& curve) const;
	std::vector<float> GetCurveLineIntersection(float px, float py,
												float vx, float vy) const;
	
	std::vector<Curve> Subdivide(float t) const;
	Curve	DivideAtTime(float time, std::vector<Segment>& segments,
						std::vector<Curve>& curves);
	
	int	SolveCubic(size_t coord, int value, std::vector<float>& roots,
				   float mn, float mx) const;
	int SolveCubic(float a, float b, float c, float d, std::vector<float>& roots,
				   float mn, float mx) const;

	void Flip();
	void ClearHandles();

	bool IsStraight() const;
	bool HasHandle() const ;

	bool operator==(const Curve& other);
	bool operator!=(const Curve& other);
};

struct Location
{
	Curve C;
	Segment S;
	float Time;
	bool  Overlap;

	std::shared_ptr<Location> Inters;
	std::shared_ptr<Location> Next;
	std::shared_ptr<Location> Prev;
	
	Location();
	Location(const Curve& curve, float time, bool overlap);

	bool IsTouching();
	bool operator==(const Location& other);
};

struct Branch
{
	float Start;
	std::vector<Segment> Crossings;
	std::vector<Segment> Visited;
	PointF HI;

	Branch();
	Branch(float start, const std::vector<Segment>& crosings, const PointF& hi);
};

struct Winding
{
	std::vector<float> Roots;
	std::vector<Curve> Curves;

	Curve C;
	Curve Prev;

	bool OnPath;
	bool OnAnyPath;
	bool Direction;

	float W;
	float Quality;
	float WindingLeft;
	float WindingRight;
	float PointAbscissas;
	float PointOrdinat;
	float PathWindingLeft;
	float PathWindingRight;
	float PointAbscissasLeft;
	float PointAbscissasRight;

	Winding();

	void Reset();
	void SetDirection(bool direction);
	void SetPoint(const PointF& point);
	void SetCurve(const Curve& curve);
	void SetCurves(const std::vector<Curve>& curves);

	bool AddWinding();
	bool HandleCurve();
	bool GetWinding();

	bool IsFirst(Curve curve);
	Curve GetPrev(Curve curve);
};

class GRAPHICS_DECL CGraphicsPathClip
{
public:
	CGraphicsPathClip(CGraphicsPath* path1, CGraphicsPath* path2, BooleanOpType op);
	CGraphicsPath* GetResult();
	
	//BooleanOp
	void TraceBoolean();
	void TracePaths();
	//void reorientPaths();

	//Path
	void	PreparePath(CGraphicsPath *path, size_t id, std::vector<Segment>& segments,
						std::vector<Curve>& curves);
	void	InsertSegment(const Segment& segment);
	Curve	GetCurve(const Segment& segment) const;
	Curve	GetPreviousCurve(Curve curve) const;
	Curve	GetNextCurve(Curve curve) const;
	Segment GetNextSegment(Segment segment) const;
	std::vector<Curve> GetCurves(const std::vector<int>& indices) const;
	std::vector<Segment> GetCrossing(const Segment& seg, std::vector<Segment>& starts,
									bool collectStart) const;

	//Bounds
	std::vector<std::vector<float>> GetBoundsForCurves(const std::vector<Curve>& curves) const;
	std::vector<std::vector<int>> FindCurveBoundsCollisions(const std::vector<Curve>& curves1,
															const std::vector<Curve>& curves2,
															float tolerance,
															bool bothAxis = false);
	std::vector<std::vector<int>> FindBoundsCollisions(const std::vector<std::vector<float>>& bounds1,
													   const std::vector<std::vector<float>>& bounds2,
													   float tolerance,
													   bool sweepVertical = false,
													   bool onlySweep = false);

	//Intersection
	bool IsCrossing(std::shared_ptr<Location> loc);
	bool FilterIntersections(std::shared_ptr<Location> loc);
	bool IntersectsBounds();
	void GetIntersection();
	void GetCurveIntersection(const Curve& curve1, const Curve& curve2);
	void LinkIntersection(std::shared_ptr<Location> form, std::shared_ptr<Location> to);
	void AddLineIntersection(const Curve& curve1, const Curve& curve2, bool flip);
	void AddCurveLineIntersection(const Curve& curve1, const Curve& curve2, bool flip);
	int	 AddCurveIntersection(const Curve& curve1, const Curve& curve2, bool flip,
							  int recursion = 0, int calls = 0, float tMin = 0,
							  float tMax = 1, float uMin = 0, float uMax = 1);

	//Location
	void DivideLocations();
	void AddLocation(Curve curve1,Curve curve2, float t1, float t2, bool overlap = false);
	void InsertLocation(std::shared_ptr<Location> loc);

	//Util
	void ClearCurveHandles(std::vector<Curve>& curves);
	void AddOffsets(std::vector<float>& offsets, const Curve& curve, bool end);
	void PropagateWinding(Segment segment, std::vector<std::vector<Curve>> map);
	void Collect(const Segment& seg, std::vector<Segment>& crossing, std::vector<Segment>& starts,
				 std::shared_ptr<Location> inter, std::shared_ptr<Location> end,
				 bool collectStarts) const;
private:
	BooleanOpType Op;

	CGraphicsPath *Path1;
	CGraphicsPath *Path2;
	CGraphicsPath *Result;

	std::vector<Segment> Segments;
	std::vector<Segment> Segments1;
	std::vector<Segment> Segments2;
	
	std::vector<Curve> Curves;
	std::vector<Curve> Curves1;
	std::vector<Curve> Curves2;

	std::vector<std::shared_ptr<Location>> Locations;
	bool OverlapOnly[2] = {false, false};
};
}

#endif //GRAPHICSPATHCLIP_H
