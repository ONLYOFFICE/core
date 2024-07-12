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

	bool isValid(BooleanOpType op);
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

	std::vector<float> getXValues();
	std::vector<float> getYValues();
	std::vector<float> getPeeks();

	float	getLength(float a = 0, float b = 1);
	float	getSquaredLineLength();
	float	getTimeOf(PointF point);
	float	getTimeAt(float offset);
	PointF	get(float t, int type);
	PointF	getPoint(float t);
	PointF	getTangent(float t);
	PointF	getTangent(float t, float offset, bool inside, PointF p);
	Curve	getPart(float from, float to);
	std::vector<Curve> getMonoCurves(bool dir);
	std::vector<std::pair<int, int>> getOverlaps(Curve curve);
	std::vector<float> getCurveLineIntersection(float px, float py,
												float vx, float vy);
	
	std::vector<Curve> subdivide(float t);
	Curve	divideAtTime(float time, std::vector<Segment> segments,
					   std::vector<Curve> curves);
	
	int	solveCubic(size_t coord, int value, std::vector<float>& roots,
				   float mn, float mx);
	int solveCubic(float a, float b, float c, float d, std::vector<float>& roots,
				   float mn, float mx);

	void flip();
	void clearHandles();

	bool isStraight();
	bool hasHandle();
	bool addWinding(std::vector<Curve> curves, bool dir, bool& onPath,
					bool& onAnyPath, Curve* cPrev, std::vector<float>& roots,
					std::unordered_map<std::string, float>* map);
	bool handleCurve(std::vector<Curve> curves, bool dir, bool& onPath,
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

	bool isTouching();
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
	CGraphicsPath getResult();
	
	//BooleanOp
	void traceBoolean();
	void tracePaths();
	//void reorientPaths(); TODO

	//Path
	void	preparePath(CGraphicsPath* path, size_t id, std::vector<Segment>& segments,
					 std::vector<Curve>& curves);
	Curve	getCurve(Segment segment);
	Curve	getPreviousCurve(Curve curve);
	Curve	getNextCurve(Curve curve);
	Segment getNextSegment(Segment segment);
	std::vector<Curve> getCurves(std::vector<int> indices);
	std::vector<Segment> getCrossing(Segment seg, std::vector<Segment>& starts,
									 bool collectStart);

	//Bounds
	std::vector<std::vector<float>> getBoundsForCurves(std::vector<Curve> curves);
	std::vector<std::vector<int>> findCurveBoundsCollisions(std::vector<Curve> curves1,
															std::vector<Curve> curves2,
															float tolerance,
															bool bothAxis = false);
	std::vector<std::vector<int>> findBoundsCollisions(std::vector<std::vector<float>> bounds1,
													   std::vector<std::vector<float>> bounds2,
													   float tolerance,
													   bool sweepVertical = false,
													   bool onlySweep = false);

	//Intersection
	bool isCrossing(Location loc);
	bool filterIntersections(Location loc);
	bool intersectsBounds();
	void getIntersection();
	void getCurveIntersection(Curve curve1, Curve curve2);
	void linkIntersection(Location *form, Location *to);
	void addLineIntersection(Curve curve1, Curve curve2, bool flip);
	void addCurveLineIntersection(Curve curve1, Curve curve2, bool flip);
	int	 addCurveIntersection(Curve curve1, Curve curve2, bool flip, 
							  int recursion = 0, int calls = 0, float tMin = 0, 
							  float tMax = 1, float uMin = 0, float uMax = 1);

	//Location
	void divideLocations();
	void addLocation(Curve curve1, Curve curve2, int t1, int t2, bool overlap = false);
	void insertLocation(Location loc);

	//Util
	std::unordered_map<std::string, float> CGraphicsPathClip::inicializeMap();
	std::unordered_map<std::string, float>* CGraphicsPathClip::inicializeMap(float x, float y);
	void clearMap(std::vector<Curve>*** map);
	void clearCurveHandles(std::vector<Curve> curves);
	void addOffsets(std::vector<float>& offsets, Curve curve, bool end);
	void propagateWinding(Segment segment, std::vector<Curve>*** map);
	void collect(Segment seg, std::vector<Segment>& crossing, std::vector<Segment>& starts, 
				 Location *inter, Location *end, bool collectStarts);

	static bool getWinding(std::vector<Curve> curves, bool dir, 
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
