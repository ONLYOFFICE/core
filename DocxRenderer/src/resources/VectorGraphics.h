#pragma once
#include <vector>

namespace NSDocxRenderer
{
	class CVectorGraphics
	{
	public:
		enum VectorGraphicsType
		{
			vgtMove = 0,
			vgtLine = 1,
			vgtCurve = 2,
			vgtClose = 3
		};

		struct Point
		{
			double x = 0;
			double y = 0;
		};

		struct PathCommand
		{
			VectorGraphicsType type;
			std::vector<Point> points;
		};

	public:
		std::vector<PathCommand> GetData() const;

	public:
		double GetLeft() const noexcept;
		double GetTop() const noexcept;
		double GetRight() const noexcept;
		double GetBottom() const noexcept;

	public:
		CVectorGraphics();
		~CVectorGraphics();

	public:
		void MoveTo(const double& x1, const double& y1);
		void LineTo(const double& x1, const double& y1);
		void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
		void Close();
		void End();

		void Clear();
		void CheckPoint(const Point& point);
		void CheckPoint(const double& x, const double& y);

	private:
		std::vector<PathCommand> m_arData;

		double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;

		void ResetBorders();
	};
}
