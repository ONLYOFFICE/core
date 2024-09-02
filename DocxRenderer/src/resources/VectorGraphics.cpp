#include "VectorGraphics.h"

#include <algorithm>
#include <string.h>
#include <numeric>
#include <limits>

#include "../../../DesktopEditor/graphics/Matrix.h"

#include "Constants.h";

namespace NSDocxRenderer
{
	CVectorGraphics::CVectorGraphics() noexcept
		: m_dLeftDefault(std::numeric_limits<double>().max()),
		  m_dRightDefault(std::numeric_limits<double>().min()),
		  m_dTopDefault(std::numeric_limits<double>().max()),
		  m_dBottomDefault(std::numeric_limits<double>().min())
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
		size_t count = static_cast<size_t>(other.GetPointCount());
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
		}
		Close();
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
		m_dLeft = m_dLeftDefault;
		m_dTop = m_dTopDefault;
		m_dRight = m_dRightDefault;
		m_dBottom = m_dBottomDefault;
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

	void CVectorGraphics::CurveTo(const double &x1, const double &y1,
								  const double &x2, const double &y2,
								  const double &x3, const double &y3)
	{
		std::list<Point> points = {{x1, y1}, {x2, y2}, {x3, y3}};
		ePathCommandType type = ePathCommandType::pctCurve;
		m_arData.push_back({type, points});

		for(auto& point : points)
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

	void CVectorGraphics::End()
	{
		Clear();
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
		Point center((m_dLeft + m_dRight) / 2, (m_dTop + m_dBottom) / 2);
		ResetBorders();

		Aggplus::CMatrix rotate_matrix;
		rotate_matrix.RotateAt(rotation, center.x, center.y, Aggplus::MatrixOrderAppend);

		for (auto& command : m_arData)
			for (auto& point : command.points)
			{
				rotate_matrix.TransformPoint(point.x, point.y);
				CheckPoint(point);
			}
	}

	// ClipRegionTypeWinding = 0x0000;
	// ClipRegionTypeEvenOdd = 0x0001;
	// ClipRegionIntersect = 0x0000;
	// ClipRegionUnion = 0x0100;
	// ClipRegionXor = 0x0200;
	// ClipRegionDiff = 0x0400;
	CVectorGraphics CVectorGraphics::CalcBoolean(const CVectorGraphics& vg1, const CVectorGraphics& vg2, long clipType)
	{
		std::unique_ptr<Aggplus::CGraphicsPath> path1(vg1.GetGraphicsPath());
		std::unique_ptr<Aggplus::CGraphicsPath> path2(vg2.GetGraphicsPath());
		auto op = GetOpType(clipType);
		return CVectorGraphics(*Aggplus::CalcBooleanOperation(path1.get(), path2.get(), op));
	}

	Aggplus::CGraphicsPath* CVectorGraphics::GetGraphicsPath() const noexcept
	{
		Aggplus::CGraphicsPath* ret_value = new Aggplus::CGraphicsPath();
		for (const auto& path : m_arData)
		{
			if (path.type == ePathCommandType::pctMove)
				ret_value->MoveTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctLine)
				ret_value->LineTo(path.points.front().x, path.points.front().y);
			else if (path.type == ePathCommandType::pctClose)
				ret_value->CloseFigure();
			else if (path.type == ePathCommandType::pctCurve)
			{
				std::vector<Point> points;
				for (const auto& point : path.points)
					points.push_back(point);
				ret_value->CurveTo(points[0].x, points[0].y,
						points[1].x, points[1].y,
						points[2].x, points[2].y);
			}
		}
		// NRVO things is not working because of no Aggplus::CGraphicsPath(const Aggplus::CGraphicsPath&)
		// so pointer is used
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
}
