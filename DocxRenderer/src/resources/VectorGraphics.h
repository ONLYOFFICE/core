/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

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
			std::vector<Point> points;
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
		bool IsClockwise() const;

		static CVectorGraphics CalcBoolean(const CVectorGraphics& vg1, const CVectorGraphics& vg2, long clipType, long fillType = c_nWindingFillMode, bool isLuminosity = false);

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
