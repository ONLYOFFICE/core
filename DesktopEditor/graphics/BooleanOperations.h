#ifndef BOOLEANOPERATIONS_H
#define BOOLEANOPERATIONS_H

#include "GraphicsPath.h"
#include "boolean_operations_math.h"
#include <unordered_map>

namespace Aggplus
{
	struct Location;

	struct Segment
	{
		PointD P;
		PointD HI;
		PointD HO;

		bool IsCurve;
		bool Visited;

		int Index;
		int Id;
		int Winding;

		CGraphicsPath* Path;
		std::shared_ptr<Location> Inters;

		Segment();
		Segment(const std::vector<PointD>& points, bool isCurve, int index, int id, CGraphicsPath* path);
		Segment(const PointD& p, const PointD& hi, const PointD& ho);

		void SetHandles(const PointD& hi, const PointD& ho);
		void UpdateHandles(const PointD& hi, const PointD& ho);

		bool IsValid(BooleanOpType op) const;
		bool operator==(const Segment& other) const;
		bool operator!=(const Segment& other) const;
	};

	struct Curve
	{
		Segment Segment1;
		Segment Segment2;

		Curve();
		Curve(const Segment& segment1, const Segment& segment2);
		Curve(const std::vector<double>& values);
		Curve(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);

		std::vector<double> GetXValues() const;
		std::vector<double> GetYValues() const;
		std::vector<double> GetPeeks() const;
		double GetLength(double a = 0, double b = 1) const;
		double GetSquaredLineLength() const;
		double GetTimeOf(const PointD& point) const;
		double GetTimeAt(double offset) const;
		PointD Get(double t, int type) const;
		PointD GetPoint(double t) const;
		PointD GetTangent(double t) const;
		PointD GetTangent(double t, double offset, bool inside, const PointD& p) const;
		Curve  GetPart(double from, double to) const;
		std::vector<Curve>  GetMonoCurves(bool dir) const;
		std::vector<double> GetCurveLineIntersection(double px, double py, double vx, double vy) const;
		std::vector<std::pair<double, double>> GetOverlaps(const Curve& curve) const;

		std::vector<Curve> Subdivide(double t) const;
		Curve DivideAtTime(double time);

		int SolveCubic(int coord, double value, std::vector<double>& roots, double mn, double mx) const;
		int SolveCubic(double a, double b, double c, double d, std::vector<double>& roots, double mn, double mx) const;

		void Flip();
		void ClearHandles();

		bool IsStraight() const;
		bool HasHandle() const;
		bool operator==(const Curve& other) const;
		bool operator!=(const Curve& other) const;
	};

	struct Location
	{
		Curve C;
		Segment S;
		double Time;
		bool Overlap;

		std::shared_ptr<Location> Inters;
		std::shared_ptr<Location> Next;
		std::shared_ptr<Location> Prev;

		Location();
		Location(const Curve& curve, double time, bool overlap);

		bool IsTouching();
		// bool operator==(const Location& other);
	};

	class CBooleanOperations
	{
	public:
		CBooleanOperations(CGraphicsPath* path1, CGraphicsPath* path2, BooleanOpType op);
		~CBooleanOperations();
		CGraphicsPath* GetResult();

		// BooleanOp
		void TraceBoolean();
		void TracePaths();

		// Path
		void PreparePath(CGraphicsPath* path, int id, std::vector<Segment>& segments, std::vector<Curve>& curves, bool reverse = false);
		void InsertSegment(const Segment& segment, const Segment& handles, bool updateHandles);
		Curve GetCurve(const Segment& segment) const;
		Curve GetPreviousCurve(const Curve& curve) const;
		Curve GetNextCurve(const Curve& curve) const;
		Segment GetNextSegment(const Segment& segment) const;

		// Bounds
		std::vector<std::vector<double>> GetBoundsForCurves(const std::vector<Curve>& curves) const;
		std::vector<std::vector<int>> FindCurveBoundsCollisions();
		std::vector<std::vector<int>> FindBoundsCollisions(const std::vector<std::vector<double>>& bounds1, const std::vector<std::vector<double>>& bounds2);

		// Intersection
		bool IsCrossing(std::shared_ptr<Location> loc);
		bool FilterIntersections(std::shared_ptr<Location> loc);
		bool IntersectsBounds();
		void GetIntersection();
		void GetCurveIntersection(const Curve& curve1, const Curve& curve2);
		void LinkIntersection(std::shared_ptr<Location> form, std::shared_ptr<Location> to);
		void AddLineIntersection(const Curve& curve1, const Curve& curve2);
		void AddCurveLineIntersection(const Curve& curve1, const Curve& curve2, bool flip);
		int  AddCurveIntersection(Curve curve1, Curve curve2, const Curve& startCurve1, const Curve& startCurve2, bool flip, int recursion = 0, int calls = 0, double tMin = 0, double tMax = 1, double uMin = 0, double uMax = 1);
		int  CheckInters(const PointD& point, const Segment& segment, const Curve& curve, bool dir = false) const;

		// Location
		void DivideLocations();
		void AddLocation(Curve curve1, Curve curve2, double t1, double t2, bool overlap = false);
		void InsertLocation(std::shared_ptr<Location> loc, bool overlap);

		// Util
		void SetVisited(const Segment& segment);
		void AddOffsets(std::vector<double>& offsets, const Curve& curve, bool end);
		PointD GetMinPoint(const std::vector<Segment>& segments);

	private:
		BooleanOpType Op;

		CGraphicsPath* Path1;
		CGraphicsPath* Path2;
		CGraphicsPath* Result;

		std::vector<Segment> Segments1;
		std::vector<Segment> Segments2;

		std::vector<Curve> Curves1;
		std::vector<Curve> Curves2;

		std::vector<std::shared_ptr<Location>> Locations;

		bool IsDeleted;
	};
} // namespace Aggplus

#endif // BOOLEANOPERATIONS_H
