#ifndef BOOLEANOPERATIONS_H
#define BOOLEANOPERATIONS_H

#include "GraphicsPath.h"
#include <memory>

namespace Aggplus
{
	struct Location;

	struct Segment
	{
		PointD P{};
		PointD HI{};
		PointD HO{};

		bool IsCurve	= false;
		bool Visited	= false;
		bool PolyClosed = false;

		int Index	= -1;
		int Id		= 0;
		int Winding	= 0;

		std::shared_ptr<Location> Inters{nullptr};

		Segment() noexcept;
		Segment(const std::vector<PointD>& points, const bool& isCurve,
				const int& index, const int& id, const bool& polyClosed) noexcept;
		Segment(const PointD& point, const bool& isCurve, const int& index,
				const int& id, const bool& polyClosed) noexcept;
		Segment(const PointD& p) noexcept;
		Segment(const PointD& p, const PointD& hi, const PointD& ho) noexcept;

		void SetHandles(const PointD& hi, const PointD& ho) noexcept;
		void UpdateHandles(const PointD& hi, const PointD& ho) noexcept;

		bool IsValid(const BooleanOpType& op) const noexcept;
		bool IsEmpty() const noexcept;
		bool operator==(const Segment& other) const noexcept;
		bool operator!=(const Segment& other) const noexcept;
	};

	struct Curve
	{
		Segment Segment1{};
		Segment Segment2{};

		Curve() noexcept;
		Curve(const Segment& segment1, const Segment& segment2) noexcept;
		Curve(const std::vector<double>& values) noexcept;
		Curve(const double& x0, const double& y0, const double& x1, const double& y1,
			  const double& x2, const double& y2, const double& x3, const double& y3) noexcept;

		std::vector<double> GetXValues() const noexcept;
		std::vector<double> GetYValues() const noexcept;
		std::vector<double> GetBound() const noexcept;
		std::vector<double> GetPeeks() const;
		double GetLength(double a = 0, double b = 1) const;
		double GetSquaredLineLength() const noexcept;
		double GetTimeOf(const PointD& point) const noexcept;
		double GetTimeAt(const double& offset) const noexcept;
		PointD Get(const double& t, const int& type) const noexcept;
		PointD GetPoint(const double& t) const noexcept;
		PointD GetTangent(const double& t) const noexcept;
		PointD GetTangent(const double& t, const double& offset,
						  const bool& inside, const PointD& p) const noexcept;
		Curve  GetPart(double from, double to) const noexcept;
		std::vector<Curve>  GetMonoCurves(const bool& dir) const noexcept;
		std::vector<double> GetCurveLineIntersection(const double& px, const double& py,
													 const double& vx, const double& vy) const noexcept;
		std::vector<std::pair<double, double>> GetOverlaps(const Curve& curve) const noexcept;

		std::vector<Curve> Subdivide(const double& t) const noexcept;
		Curve DivideAtTime(const double& time) noexcept;

		int SolveCubic(const int& coord, const double& value, std::vector<double>& roots,
					   const double& mn, const double& mx) const noexcept;
		int SolveCubic(double a, double b, double c, double d, std::vector<double>& roots,
					   const double& mn, const double& mx) const noexcept;

		void Flip() noexcept;

		bool IsStraight()	const noexcept;
		bool operator==(const Curve& other) const noexcept;
		bool operator!=(const Curve& other) const noexcept;
	};

	struct Location
	{
		Curve C{};
		Segment S{};
		double Time = -1.0;
		bool Overlap = false;
		bool Ends = false;

		std::shared_ptr<Location> Inters{nullptr};
		std::shared_ptr<Location> Next{nullptr};
		std::shared_ptr<Location> Prev{nullptr};

		Location() noexcept;
		Location(const Curve& curve, const double& time, const bool& overlap, const bool& ends) noexcept;

		bool IsTouching() noexcept;
	};

	class CBooleanOperations
	{
	public:
		CBooleanOperations(const CGraphicsPath& path1, const CGraphicsPath& path2, BooleanOpType op, long fillType, bool isLuminosity);
		~CBooleanOperations();
		CGraphicsPath&& GetResult();

		// BooleanOp
		void TraceBoolean();
		void TraceOneInters();
		void TraceAllOverlap();
		void TracePaths();

		// Path
		void	PreparePath(const CGraphicsPath& path, int id, std::vector<Segment>& segments,
							std::vector<Curve>& curves, bool reverse = false);
		void	InsertSegment(Segment& segment, const Segment& handles, bool updateHandles);
		Curve	GetCurve(const Segment& segment) const noexcept;
		Curve	GetPreviousCurve(const Curve& curve) const noexcept;
		Curve	GetNextCurve(const Curve& curve) const noexcept;
		Segment GetNextSegment(const Segment& segment) const noexcept;
		void	SetVisited(const Segment& segment);

		// Bounds
		std::vector<std::vector<int>>	 FindBoundsCollisions();

		// Intersection
		bool IsCrossing(std::shared_ptr<Location> loc) noexcept;
		bool IntersectsBounds() noexcept;
		void GetIntersection();
		void GetCurveIntersection(const Curve& curve1, const Curve& curve2);
		void LinkIntersection(std::shared_ptr<Location> form, std::shared_ptr<Location> to);
		void AddLineIntersection(const Curve& curve1, const Curve& curve2);
		void AddCurveLineIntersection(const Curve& curve1, const Curve& curve2, bool flip);
		int  AddCurveIntersection(const Curve& curve1, const Curve& curve2, const Curve& startCurve1, const Curve& startCurve2, bool flip,
								  int recursion = 0, int calls = 0, double tMin = 0.0, double tMax = 1.0, double uMin = 0.0, double uMax = 1.0);
		int  CheckInters(const PointD& point, const Segment& segment, const Curve& curve, int& touchCount) const;
		void SetWinding();

		// Location
		void DivideLocations();
		void AddLocation(Curve curve1, Curve curve2, double t1, double t2, bool overlap = false, bool filter = true, bool bothEnds = false);
		void InsertLocation(std::shared_ptr<Location> loc, bool overlap);
		bool AllOverlap() const noexcept;
		bool AllInters(const std::vector<Segment>& segments) const noexcept;
		void AddOffsets(std::vector<double>& offsets, const Curve& curve, bool end);

	private:
		BooleanOpType Op = Intersection;

		bool Close1 = true;
		bool Close2 = true;
		bool IsLuminosity = false;

		// c_nStroke, c_nWindingFillMode, c_nEvenOddFillMode
		long FillType = c_nWindingFillMode;

		CGraphicsPath Path1;
		CGraphicsPath Path2;
		CGraphicsPath Result;

		std::vector<Segment> Segments1;
		std::vector<Segment> Segments2;

		std::vector<Curve> OriginCurves1;
		std::vector<Curve> OriginCurves2;
		std::vector<Curve> Curves1;
		std::vector<Curve> Curves2;

		std::vector<std::shared_ptr<Location>> Locations;
	};
} // namespace Aggplus

#endif // BOOLEANOPERATIONS_H
