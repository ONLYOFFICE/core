#pragma once
#include <list>

namespace NSDocxRenderer
{
	class CVectorGraphics
	{
	public:
		enum class eVectorGraphicsType
		{
			vgtMove = 0,
			vgtLine = 1,
			vgtCurve = 2,
			vgtClose = 3
		};

		struct Point
		{
			double x;
			double y;
		};

		struct PathCommand
		{
			eVectorGraphicsType type;
			std::list<Point> points;
		};

		CVectorGraphics();
		~CVectorGraphics();

		CVectorGraphics& operator=(CVectorGraphics&& other);

		const std::list<PathCommand>& GetData() const;

		double GetLeft() const noexcept;
		double GetTop() const noexcept;
		double GetRight() const noexcept;
		double GetBottom() const noexcept;

		void MoveTo(const double& x1, const double& y1);
		void LineTo(const double& x1, const double& y1);
		void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
		void Close();
		void End();

		void Add(const PathCommand& command);
		void Join(CVectorGraphics&& other);

		void Clear();
		void CheckPoint(const Point& point);
		void CheckPoint(const double& x, const double& y);

	private:
		std::list<PathCommand> m_arData;

		double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;

		double m_dLeftDefault;
		double m_dTopDefault;
		double m_dRightDefault;
		double m_dBottomDefault;

		void ResetBorders();
	};
}
