#pragma once
#include "../../../DesktopEditor/graphics/GraphicsPath.h"

#include <list>

namespace NSDocxRenderer
{
	struct Point
	{
		double x = 0;
		double y = 0;

		Point() {}
		Point(double x, double y) : x(x), y(y) {}
		Point(const Point& point) : x(point.x), y(point.y) {}
	};

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

		struct PathCommand
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

		const std::list<PathCommand>& GetData() const;

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
		void End();

		void Add(const PathCommand& command);
		void Join(CVectorGraphics&& other);

		void Clear();
		void CheckPoint(const Point& point) noexcept;
		void CheckPoint(const double& x, const double& y) noexcept;
		void Rotate(const double& rotation);
		void Transform(const Aggplus::CMatrix& matrix);
		void DrawOnRenderer(IRenderer* renderer) const noexcept;

		static CVectorGraphics CalcBoolean(const CVectorGraphics& vg1, const CVectorGraphics& vg2, long clipType);

	private:
		std::list<PathCommand> m_arData;

		double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;

		const double m_dLeftDefault;
		const double m_dTopDefault;
		const double m_dRightDefault;
		const double m_dBottomDefault;

		void ResetBorders() noexcept;

		// pointer because of no Aggplus::CGraphicsPath(const Aggplus::CGraphicsPath&)
		Aggplus::CGraphicsPath* GetGraphicsPath() const noexcept;
		static Aggplus::BooleanOpType GetOpType(long nClipType);
	};
}
