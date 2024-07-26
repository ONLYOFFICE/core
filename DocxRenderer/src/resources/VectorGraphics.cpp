#include <algorithm>
#include <string.h>
#include <numeric>
#include <limits>

#include "VectorGraphics.h"


namespace NSDocxRenderer
{
	CVectorGraphics::CVectorGraphics()
		: m_dLeftDefault(std::numeric_limits<double>().max()),
		  m_dRightDefault(std::numeric_limits<double>().min()),
		  m_dTopDefault(std::numeric_limits<double>().max()),
		  m_dBottomDefault(std::numeric_limits<double>().min())
	{
		ResetBorders();
	}

	CVectorGraphics::~CVectorGraphics()
	{
		m_arData.clear();
	}
	CVectorGraphics& CVectorGraphics::operator=(CVectorGraphics&& other)
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

	const std::list<CVectorGraphics::PathCommand>& CVectorGraphics::GetData() const
	{
		return m_arData;
	}

	void CVectorGraphics::MoveTo(const double &x1, const double &y1)
	{
		Point point = {x1, y1};
		eVectorGraphicsType type = eVectorGraphicsType::vgtMove;
		m_arData.push_back({type, {point}});

		CheckPoint(point);
	}

	void CVectorGraphics::LineTo(const double &x1, const double &y1)
	{
		Point point = {x1, y1};
		eVectorGraphicsType type = eVectorGraphicsType::vgtLine;
		m_arData.push_back({type, {point}});

		CheckPoint(point);
	}

	void CVectorGraphics::CurveTo(const double &x1, const double &y1,
								  const double &x2, const double &y2,
								  const double &x3, const double &y3)
	{
		std::list<Point> points = {{x1, y1}, {x2, y2}, {x3, y3}};
		eVectorGraphicsType type = eVectorGraphicsType::vgtCurve;
		m_arData.push_back({type, points});

		for(auto& point : points)
			CheckPoint(point);
	}

	void CVectorGraphics::Close()
	{
		eVectorGraphicsType type = eVectorGraphicsType::vgtClose;
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
