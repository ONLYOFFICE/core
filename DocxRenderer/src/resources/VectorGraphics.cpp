#include <algorithm>
#include <string.h>
#include <numeric>

#include "VectorGraphics.h"
#include "../DesktopEditor/common/Types.h"


namespace NSDocxRenderer
{
	CVectorGraphics::CVectorGraphics()
	{
		m_arData.resize(500);

		m_dLeftDefault = std::numeric_limits<double>().max();
		m_dTopDefault = std::numeric_limits<double>().max();
		m_dRightDefault = std::numeric_limits<double>().min();
		m_dBottomDefault = std::numeric_limits<double>().min();

		ResetBorders();
	}

	CVectorGraphics::~CVectorGraphics()
	{
		m_arData.clear();
	}

	void CVectorGraphics::ResetBorders()
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

	const std::vector<CVectorGraphics::PathCommand>& CVectorGraphics::GetData() const
	{
		return m_arData;
	}

	void CVectorGraphics::MoveTo(const double &x1, const double &y1)
	{
		Point point = {x1, y1};
		VectorGraphicsType type = vgtMove;
		m_arData.push_back({type, {point}});

		CheckPoint(point);
	}

	void CVectorGraphics::LineTo(const double &x1, const double &y1)
	{
		Point point = {x1, y1};
		VectorGraphicsType type = vgtLine;
		m_arData.push_back({type, {point}});

		CheckPoint(point);
	}

	void CVectorGraphics::CurveTo(const double &x1, const double &y1,
								  const double &x2, const double &y2,
								  const double &x3, const double &y3)
	{
		std::vector<Point> points = {{x1, y1}, {x2, y2}, {x3, y3}};
		VectorGraphicsType type = vgtCurve;
		m_arData.push_back({type, points});

		for(auto& point : points)
			CheckPoint(point);
	}

	void CVectorGraphics::Close()
	{
		VectorGraphicsType type = vgtClose;
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

	void CVectorGraphics::CheckPoint(const Point& point)
	{
		if (m_dLeft > point.x) m_dLeft = point.x;
		if (m_dRight < point.x) m_dRight = point.x;
		if (m_dTop > point.y) m_dTop = point.y;
		if (m_dBottom < point.y) m_dBottom = point.y;
	}
	void CVectorGraphics::CheckPoint(const double& x, const double& y)
	{
		Point point = {x, y};
		CheckPoint(point);
	}
}
