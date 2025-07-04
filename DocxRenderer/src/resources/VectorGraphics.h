#pragma once
#include "../../../DesktopEditor/graphics/GraphicsPath.h"

#include <list>
#include <array>

namespace NSDocxRenderer
{
	struct Point
	{
		double x = 0;
		double y = 0;

		Point() {}
		Point(double x, double y) : x(x), y(y) {}
		Point(const Point& point) : x(point.x), y(point.y) {}
		Point& operator=(const Point& point) {x = point.x; y = point.y; return *this;}
	};;

	class CVectorGraphics
	{
	public:
		enum class ePathCommandType
		{
			pctMove = 0,
			pctLine = 1,
			pctCurve = 2,
			pctClose = 3
		};

		struct CPathCommand
		{
			ePathCommandType type;
			std::list<Point> points;
		};

		CVectorGraphics() noexcept;
		CVectorGraphics(const CVectorGraphics& other) noexcept;
		CVectorGraphics(CVectorGraphics&& other) noexcept;
		CVectorGraphics(const Aggplus::CGraphicsPath& other) noexcept;
		~CVectorGraphics();

		CVectorGraphics& operator=(CVectorGraphics&& other) noexcept;
		CVectorGraphics& operator=(const CVectorGraphics& other) noexcept;

		bool operator<(const CVectorGraphics& other) const noexcept;
		bool operator>(const CVectorGraphics& other) const noexcept;
		bool operator==(const CVectorGraphics& other) const noexcept;
		bool operator!=(const CVectorGraphics& other) const noexcept;
		bool operator<=(const CVectorGraphics& other) const noexcept;
		bool operator>=(const CVectorGraphics& other) const noexcept;

		const std::list<CPathCommand>& GetData() const;

		double GetLeft() const noexcept;
		double GetTop() const noexcept;
		double GetRight() const noexcept;
		double GetBottom() const noexcept;
		double GetWidth() const noexcept;
		double GetHeight() const noexcept;
		Point GetCenter() const noexcept;
		bool IsEmpty() const noexcept;

		void MoveTo(const double& x1, const double& y1);
		void LineTo(const double& x1, const double& y1);
		void CurveTo(
		        const double& x1, const double& y1,
		        const double& x2, const double& y2,
		        const double& x3, const double& y3);
		void Close();

		void Add(const CPathCommand& command);
		void Join(CVectorGraphics&& other);

		void Clear();
		void CheckPoint(const Point& point) noexcept;
		void CheckPoint(const double& x, const double& y) noexcept;
		void RotateAt(const double& rotation, const Point& point);
		void Rotate(const double& rotation);
		void Transform(const Aggplus::CMatrix& matrix);
		void DrawOnRenderer(IRenderer* renderer) const noexcept;

		static CVectorGraphics CalcBoolean(const CVectorGraphics& vg1, const CVectorGraphics& vg2, long clipType, long fillType = c_nWindingFillMode);

	private:
		std::list<CPathCommand> m_arData;

		double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;

		void ResetBorders() noexcept;

		Aggplus::CGraphicsPath GetGraphicsPath() const noexcept;
		static Aggplus::BooleanOpType GetOpType(long nClipType);
		static std::vector<Point> GetPointsCurve(const std::array<Point, 4>& curve, double step = 0.05);
	};

	// collect and contains horizontal and vertical lines
	class CHorVerLinesCollector
	{
	public:
		struct CHorVerLine
		{
			double min{};
			double max{};
			double pos{};

			CHorVerLine(const double& _min, const double& _max, const double& _pos);
		};

		CHorVerLinesCollector() = default;
		~CHorVerLinesCollector() = default;

		const std::vector<CHorVerLine>& GetHorizontal() const;
		const std::vector<CHorVerLine>& GetVertical() const;

		void AddVector(const CVectorGraphics& oVector);
		void Clear();

	private:
		std::vector<CHorVerLine> m_arHorizontal;
		std::vector<CHorVerLine> m_arVertical;
	};
}
