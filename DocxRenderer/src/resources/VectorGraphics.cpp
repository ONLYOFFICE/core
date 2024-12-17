#include "VectorGraphics.h"

#include <string.h>
#include <limits>

#include "../../../DesktopEditor/graphics/Matrix.h"

#include "Constants.h"

namespace NSDocxRenderer
{
	CVectorGraphics::CVectorGraphics() noexcept
	{
		ResetBorders();
	}
	CVectorGraphics::CVectorGraphics(const CVectorGraphics& other) noexcept
		: CVectorGraphics()
	{
		*this = other;
	}
	CVectorGraphics::CVectorGraphics(CVectorGraphics&& other) noexcept
		: CVectorGraphics()
	{
		*this = std::move(other);
	}
	CVectorGraphics::CVectorGraphics(const Aggplus::CGraphicsPath& other) noexcept
		: CVectorGraphics()
	{
		size_t close_count = other.GetCloseCount();
		size_t count = static_cast<size_t>(other.GetPointCount()) + close_count;
		std::vector<Aggplus::PointD> points = other.GetPoints(0, count);
		for (size_t idx = 0; idx < count; ++idx)
		{
			const auto& point = points[idx];
			if (other.IsMovePoint(idx))
				MoveTo(point.X, point.Y);
			else if (other.IsLinePoint(idx))
				LineTo(point.X, point.Y);	
			else if (idx < count - 2 &&
					 other.IsCurvePoint(idx) &&
					 other.IsCurvePoint(idx + 1) &&
					 other.IsCurvePoint(idx + 2))
			{
				const auto& point1 = points[idx + 1];
				const auto& point2 = points[idx + 2];
				CurveTo(point.X, point.Y, point1.X, point1.Y, point2.X, point2.Y);
				idx += 2;
			}
			else if (other.IsClosePoint(idx))
				Close();
		}
	}


	CVectorGraphics::~CVectorGraphics()
	{
		m_arData.clear();
	}
	CVectorGraphics& CVectorGraphics::operator=(CVectorGraphics&& other) noexcept
	{
		if (this == &other)
			return *this;

		m_arData = std::move(other.m_arData);
		m_dLeft = other.m_dLeft;
		m_dTop = other.m_dTop;
		m_dRight = other.m_dRight;
		m_dBottom = other.m_dBottom;

		other.Clear();
		return *this;
	}
	CVectorGraphics& CVectorGraphics::operator=(const CVectorGraphics& other) noexcept
	{
		if (this == &other)
			return *this;

		m_arData = other.m_arData;
		m_dLeft = other.m_dLeft;
		m_dTop = other.m_dTop;
		m_dRight = other.m_dRight;
		m_dBottom = other.m_dBottom;
		return *this;
	}

	bool CVectorGraphics::operator<(const CVectorGraphics& other) const noexcept
	{
		return m_dBottom < other.m_dBottom &&
				m_dTop > other.m_dTop &&
				m_dRight < other.m_dRight &&
				m_dLeft > other.m_dLeft;
	}
	bool CVectorGraphics::operator>(const CVectorGraphics& other) const noexcept
	{
		return m_dBottom > other.m_dBottom &&
				m_dTop < other.m_dTop &&
				m_dRight > other.m_dRight &&
				m_dLeft < other.m_dLeft;
	}
	bool CVectorGraphics::operator==(const CVectorGraphics& other) const noexcept
	{
		return fabs(m_dBottom - other.m_dBottom < c_dGRAPHICS_ERROR_MM) &&
				fabs(m_dTop - other.m_dTop < c_dGRAPHICS_ERROR_MM) &&
				fabs(m_dRight - other.m_dRight < c_dGRAPHICS_ERROR_MM) &&
				fabs(m_dLeft - other.m_dLeft < c_dGRAPHICS_ERROR_MM);
	}
	bool CVectorGraphics::operator!=(const CVectorGraphics& other) const noexcept
	{
		return !(*this == other);
	}
	bool CVectorGraphics::operator<=(const CVectorGraphics& other) const noexcept
	{
		return m_dBottom - c_dGRAPHICS_ERROR_MM < other.m_dBottom &&
				m_dTop + c_dGRAPHICS_ERROR_MM > other.m_dTop &&
				m_dRight - c_dGRAPHICS_ERROR_MM < other.m_dRight &&
				m_dLeft + c_dGRAPHICS_ERROR_MM > other.m_dLeft;
	}
	bool CVectorGraphics::operator>=(const CVectorGraphics& other) const noexcept
	{
		return m_dBottom + c_dGRAPHICS_ERROR_MM > other.m_dBottom &&
				m_dTop - c_dGRAPHICS_ERROR_MM < other.m_dTop &&
				m_dRight + c_dGRAPHICS_ERROR_MM > other.m_dRight &&
				m_dLeft - c_dGRAPHICS_ERROR_MM < other.m_dLeft;
	}

	void CVectorGraphics::ResetBorders() noexcept
	{
		m_dLeft = std::numeric_limits<double>().max();
		m_dTop = std::numeric_limits<double>().max();
		m_dRight = std::numeric_limits<double>::lowest();
		m_dBottom = std::numeric_limits<double>::lowest();
	}

	double CVectorGraphics::GetLeft() const noexcept
	{
		return m_dLeft;
	}
	double CVectorGraphics::GetTop() const noexcept
	{
		return m_dTop;
	}
	double CVectorGraphics::GetRight() const noexcept
	{
		return m_dRight;
	}
	double CVectorGraphics::GetBottom() const noexcept
	{
		return m_dBottom;
	}
	double CVectorGraphics::GetWidth() const noexcept
	{
		return m_dRight - m_dLeft;
	}
	double CVectorGraphics::GetHeight() const noexcept
	{
		return m_dBottom - m_dTop;
	}
	Point CVectorGraphics::GetCenter() const noexcept
	{
		return Point((m_dLeft + m_dRight) / 2, (m_dTop + m_dBottom) / 2);
	}
	bool CVectorGraphics::IsEmpty() const noexcept
	{
		return m_arData.empty();
	}

	const std::list<CVectorGraphics::PathCommand>& CVectorGraphics::GetData() const
	{
		return m_arData;
	}

	void CVectorGraphics::MoveTo(const double &x1, const double &y1)
	{
		Point point = {x1, y1};
		ePathCommandType type = ePathCommandType::pctMove;
		m_arData.push_back({type, {point}});

		CheckPoint(point);
	}

	void CVectorGraphics::LineTo(const double &x1, const double &y1)
	{
		Point point = {x1, y1};
		ePathCommandType type = ePathCommandType::pctLine;
		m_arData.push_back({type, {point}});

		CheckPoint(point);
	}

	void CVectorGraphics::CurveTo(
		const double &x1, const double &y1,
		const double &x2, const double &y2,
		const double &x3, const double &y3)
	{
		double x0 = m_arData.back().points.back().x;
		double y0 = m_arData.back().points.back().y;

		std::list<Point> points = {{x1, y1}, {x2, y2}, {x3, y3}};
		ePathCommandType type = ePathCommandType::pctCurve;
		m_arData.push_back({type, points});

		std::vector<Point> curve_points = GetPointsCurve(
			{Point{x0, y0}, Point{x1, y1}, Point{x2, y2}, Point{x3, y3}}, 0.1);

		for(auto& point : curve_points)
			CheckPoint(point);
	}

	void CVectorGraphics::Close()
	{
		ePathCommandType type = ePathCommandType::pctClose;
		m_arData.push_back({type, {}});
	}

	void CVectorGraphics::Clear()
	{
		m_arData.clear();
		ResetBorders();
	}

	void CVectorGraphics::Add(const PathCommand& command)
	{
		m_arData.push_back(command);
	}
	void CVectorGraphics::Join(CVectorGraphics&& other)
	{
		CheckPoint(other.m_dLeft, other.m_dTop);
		CheckPoint(other.m_dRight, other.m_dBottom);
		m_arData.splice(m_arData.end(), std::move(other.m_arData));
		other.Clear();
	}

	void CVectorGraphics::CheckPoint(const Point& point) noexcept
	{
		if (m_dLeft > point.x) m_dLeft = point.x;
		if (m_dRight < point.x) m_dRight = point.x;
		if (m_dTop > point.y) m_dTop = point.y;
		if (m_dBottom < point.y) m_dBottom = point.y;
	}
	void CVectorGraphics::CheckPoint(const double& x, const double& y) noexcept
	{
		Point point = {x, y};
		CheckPoint(point);
	}
	void CVectorGraphics::Rotate(const double& rotation)
	{
		Point center(GetCenter());
		Aggplus::CMatrix rotate_matrix;
		rotate_matrix.RotateAt(rotation, center.x, center.y, Aggplus::MatrixOrderAppend);
		Transform(rotate_matrix);
	}
	void CVectorGraphics::RotateAt(const double& rotation, const Point& point)
	{
		Aggplus::CMatrix rotate_matrix;
		rotate_matrix.RotateAt(rotation, point.x, point.y, Aggplus::MatrixOrderAppend);
		Transform(rotate_matrix);
	}
	void CVectorGraphics::Transform(const Aggplus::CMatrix& matrix)
	{
		auto data = std::move(m_arData);
		Clear();

		for (auto& command : data)
			for (auto& point : command.points)
				matrix.TransformPoint(point.x, point.y);

		for (const auto& path : data)
		{
			if (path.type == ePathCommandType::pctMove)
				MoveTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctLine)
				LineTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctClose)
				Close();
			else if (path.type == ePathCommandType::pctCurve)
			{
				std::vector<Point> points;
				for (const auto& point : path.points)
					points.push_back(point);

				CurveTo(
					points[0].x, points[0].y,
					points[1].x, points[1].y,
					points[2].x, points[2].y);
			}
		}
	}
	void CVectorGraphics::DrawOnRenderer(IRenderer* renderer) const noexcept
	{
		for (const auto& path : m_arData)
		{
			if (path.type == ePathCommandType::pctMove)
				renderer->PathCommandMoveTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctLine)
				renderer->PathCommandLineTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctClose)
				renderer->PathCommandClose();
			else if (path.type == ePathCommandType::pctCurve)
			{
				std::vector<Point> points;
				for (const auto& point : path.points)
					points.push_back(point);

				renderer->PathCommandCurveTo(
					points[0].x, points[0].y,
					points[1].x, points[1].y,
					points[2].x, points[2].y);
			}
		}
	}

	// ClipRegionTypeWinding = 0x0000;
	// ClipRegionTypeEvenOdd = 0x0001;
	// ClipRegionIntersect = 0x0000;
	// ClipRegionUnion = 0x0100;
	// ClipRegionXor = 0x0200;
	// ClipRegionDiff = 0x0400;

	// c_nStroke = 0x0001;
	// c_nWindingFillMode = 0x0100;
	// c_nEvenOddFillMode = 0x0200;
	CVectorGraphics CVectorGraphics::CalcBoolean(const CVectorGraphics& vg1, const CVectorGraphics& vg2, long clipType, long fillType)
	{
		auto op = GetOpType(clipType);
		Aggplus::CGraphicsPath result = Aggplus::CalcBooleanOperation(vg1.GetGraphicsPath(), vg2.GetGraphicsPath(), op, fillType);
		return CVectorGraphics(result);
	}

	Aggplus::CGraphicsPath CVectorGraphics::GetGraphicsPath() const noexcept
	{
		Aggplus::CGraphicsPath ret_value;
		for (const auto& path : m_arData)
		{
			if (path.type == ePathCommandType::pctMove)
				ret_value.MoveTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctLine)
				ret_value.LineTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctClose)
				ret_value.CloseFigure();
			else if (path.type == ePathCommandType::pctCurve)
			{
				std::vector<Point> points;
				for (const auto& point : path.points)
					points.push_back(point);

				ret_value.CurveTo(
					points[0].x, points[0].y,
					points[1].x, points[1].y,
					points[2].x, points[2].y);
			}
		}

		return ret_value;
	}
	Aggplus::BooleanOpType CVectorGraphics::GetOpType(long nClipType)
	{
		if (nClipType & c_nClipRegionUnion)
			return Aggplus::BooleanOpType::Union;
		else if (nClipType & c_nClipRegionXor)
			return Aggplus::BooleanOpType::Exclusion;
		else if (nClipType & c_nClipRegionDiff)
			return Aggplus::BooleanOpType::Subtraction;
		else
			return Aggplus::BooleanOpType::Intersection;
	}
	std::vector<Point> CVectorGraphics::GetPointsCurve(const std::array<Point, 4>& curve, double step)
	{
		std::vector<Point> points;
		double t = 0;
		auto calc = [&curve] (double t) -> Point {
			Point point;
			point.x = pow(1 - t, 3) * curve[0].x +
					  3 * pow(1 - t, 2) * t * curve[1].x +
					  3 * (1 - t) * t * t * curve[2].x +
					  t * t * t * curve[3].x;
			point.y = pow(1 - t, 3) * curve[0].y +
					  3 * pow(1 - t, 2) * t * curve[1].y +
					  3 * (1 - t) * t * t * curve[2].y +
					  t * t * t * curve[3].y;
			return point;
		};

		while (t <= 1)
		{
			points.push_back(calc(t));
			t += step;
		}

		return points;
	}
	const std::vector<CHorVerLinesCollector::CHorVerLine>& CHorVerLinesCollector::GetHorizontal() const
	{
		return m_arHorizontal;
	}
	const std::vector<CHorVerLinesCollector::CHorVerLine>& CHorVerLinesCollector::GetVertical() const
	{
		return m_arVertical;
	}

	void CHorVerLinesCollector::AddVector(const CVectorGraphics& oVector)
	{
		double last_x{};
		double last_y{};

		for (const auto& command : oVector.GetData())
		{
			if (command.type == CVectorGraphics::ePathCommandType::pctLine)
			{
				double x = command.points.front().x;
				double y = command.points.front().y;

				if (x - last_x <= std::numeric_limits<double>::epsilon())
					m_arVertical.push_back({std::min(last_y, y), std::max(last_y, y), x});

				else if (y - last_y <= std::numeric_limits<double>::epsilon())
					m_arHorizontal.push_back({std::min(last_x, x), std::max(last_x, x), y});
			}
			if (!command.points.empty())
			{
				last_x = command.points.back().x;
				last_y = command.points.back().y;
			}
		}
	}
	void CHorVerLinesCollector::Clear()
	{
		m_arVertical.clear();
		m_arHorizontal.clear();
	}
}
