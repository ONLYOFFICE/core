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
	Location *Inters;

	std::unordered_map<std::string, float> Winding;
	
	Segment();
	Segment(std::vector<PointF> points, bool isCurve,
			size_t index, size_t id, CGraphicsPath* path);
	Segment(PointF p, PointF hi, PointF ho);

	bool IsValid(BooleanOpType op);
	bool operator==(const Segment& other);
	bool operator!=(const Segment& other);
}; 
 
struct Curve
{
	Segment Segment1;
	Segment Segment2;
	
	Curve();
	Curve(Segment segment1, Segment segment2);
	Curve(std::vector<float> values);
	Curve(float x0, float y0, float x1, float y1,
		  float x2, float y2, float x3, float y3);

	std::vector<float> GetXValues();
	std::vector<float> GetYValues();
	std::vector<float> GetPeeks();

	float	GetLength(float a = 0, float b = 1);
	float	GetSquaredLineLength();
	float	GetTimeOf(PointF point);
	float	GetTimeAt(float offset);
	PointF	Get(float t, int type);
	PointF	GetPoint(float t);
	PointF	GetTangent(float t);
	PointF	GetTangent(float t, float offset, bool inside, PointF p);
	Curve	GetPart(float from, float to);
	std::vector<Curve> GetMonoCurves(bool dir);
	std::vector<std::pair<int, int>> GetOverlaps(Curve curve);
	std::vector<float> GetCurveLineIntersection(float px, float py,
												float vx, float vy);
	
	std::vector<Curve> Subdivide(float t);
	Curve	DivideAtTime(float time, std::vector<Segment> segments,
					   std::vector<Curve> curves);
	
	int	SolveCubic(size_t coord, int value, std::vector<float>& roots,
				   float mn, float mx);
	int SolveCubic(float a, float b, float c, float d, std::vector<float>& roots,
				   float mn, float mx);

	void Flip();
	void ClearHandles();

	bool IsStraight();
	bool HasHandle();
	bool AddWinding(std::vector<Curve> curves, bool dir, bool& onPath,
					bool& onAnyPath, Curve* cPrev, std::vector<float>& roots,
					std::unordered_map<std::string, float>* map);
	bool HandleCurve(std::vector<Curve> curves, bool dir, bool& onPath,
					 bool& onAnyPath, Curve* cPrev, std::vector<float>& roots,
					 std::unordered_map<std::string, float>* map);

	bool operator==(const Curve& other);
	bool operator!=(const Curve& other);
};

struct Location
{
	Curve C;
	Segment S;
	float Time;
	bool  Overlap;

	Location* Inters;
	Location* Next;
	Location* Prev;
	
	Location();
	Location(Curve curve, float time, bool overlap);

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
	Branch(float start, std::vector<Segment> crosings, PointF hi);
};

class GRAPHICS_DECL CGraphicsPathClip
{
public:
	CGraphicsPathClip(CGraphicsPath* path1, CGraphicsPath* path2, BooleanOpType op);
	CGraphicsPath GetResult();
	
	//BooleanOp
	void TraceBoolean();
	void TracePaths();
	//void reorientPaths(); TODO

	//Path
	void	PreparePath(CGraphicsPath* path, size_t id, std::vector<Segment>& segments,
					std::vector<Curve>& curves);
	Curve	GetCurve(Segment segment);
	Curve	GetPreviousCurve(Curve curve);
	Curve	GetNextCurve(Curve curve);
	Segment GetNextSegment(Segment segment);
	std::vector<Curve> GetCurves(std::vector<int> indices);
	std::vector<Segment> GetCrossing(Segment seg, std::vector<Segment>& starts,
									bool collectStart);

	//Bounds
	std::vector<std::vector<float>> GetBoundsForCurves(std::vector<Curve> curves);
	std::vector<std::vector<int>> FindCurveBoundsCollisions(std::vector<Curve> curves1,
															std::vector<Curve> curves2,
															float tolerance,
															bool bothAxis = false);
	std::vector<std::vector<int>> FindBoundsCollisions(std::vector<std::vector<float>> bounds1,
													   std::vector<std::vector<float>> bounds2,
													   float tolerance,
													   bool sweepVertical = false,
													   bool onlySweep = false);

	//Intersection
	bool IsCrossing(Location loc);
	bool FilterIntersections(Location loc);
	bool IntersectsBounds();
	void GetIntersection();
	void GetCurveIntersection(Curve curve1, Curve curve2);
	void LinkIntersection(Location *form, Location *to);
	void AddLineIntersection(Curve curve1, Curve curve2, bool flip);
	void AddCurveLineIntersection(Curve curve1, Curve curve2, bool flip);
	int	 AddCurveIntersection(Curve curve1, Curve curve2, bool flip,
							int recursion = 0, int calls = 0, float tMin = 0,
							float tMax = 1, float uMin = 0, float uMax = 1);

	//Location
	void DivideLocations();
	void AddLocation(Curve curve1, Curve curve2, int t1, int t2, bool overlap = false);
	void InsertLocation(Location loc);

	//Util
	std::unordered_map<std::string, float> InicializeMap();
	std::unordered_map<std::string, float>* InicializeMap(float x, float y);
	std::vector<Curve>*** InicializeMap(std::vector<std::vector<int>> collisions);
	void ClearMap(std::vector<Curve>*** map);
	void ClearCurveHandles(std::vector<Curve> curves);
	void AddOffsets(std::vector<float>& offsets, Curve curve, bool end);
	void PropagateWinding(Segment segment, std::vector<Curve>*** map);
	void Collect(Segment seg, std::vector<Segment>& crossing, std::vector<Segment>& starts,
				Location *inter, Location *end, bool collectStarts);

	static bool GetWinding(std::vector<Curve> curves, bool dir,
						   bool& onPath, bool& onAnyPath, Curve *cPrev,
						   std::vector<float>& roots,
						   std::unordered_map<std::string, float>* map);
private:
	BooleanOpType Op;

	CGraphicsPath* Path1;
	CGraphicsPath* Path2;
	CGraphicsPath  Result;

	std::vector<Segment> Segments;
	std::vector<Segment> Segments1;
	std::vector<Segment> Segments2;
	
	std::vector<Curve> Curves;
	std::vector<Curve> Curves1;
	std::vector<Curve> Curves2;

	std::vector<Location> Locations;
	bool OverlapOnly[2] = {false, false};
};
}
