/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Types.h"

namespace PdfWriter
{

// Проверка пересечения отрезков и нахождение точки пересечения
bool findIntersection(const CPoint& p1, const CPoint& p2, const CPoint& q1, const CPoint& q2, CPoint& intersection, double& t)
{
	double det = (p2.x - p1.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q2.x - q1.x);

	if (std::abs(det) < EPS)
		return false; // Параллельны или совпадают

	t = ((q1.x - p1.x) * (q2.y - q1.y) - (q1.y - p1.y) * (q2.x - q1.x)) / det;
	double u = ((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x)) / det;

	if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS)
	{
		intersection.x = p1.x + t * (p2.x - p1.x);
		intersection.y = p1.y + t * (p2.y - p1.y);
		return true;
	}
	return false;
}
// Проверка, находится ли точка внутри прямоугольника
bool isPointInsideRectangle(const CPoint& p, double dL, double dB, double dR, double dT)
{
	return p.x >= dL - EPS && p.x <= dR + EPS &&
		   p.y >= dB - EPS && p.y <= dT + EPS;
}
// Проверка, лежит ли прямая на стороне прямоугольника
bool isLineOnRectangleSide(double dX1, double dY1, double dX2, double dY2, double dL, double dB, double dR, double dT)
{
	// Проверяем совпадение с каждой стороной
	bool onBottom = std::abs(dY1 - dB) < EPS && std::abs(dY2 - dB) < EPS;
	bool onTop    = std::abs(dY1 - dT) < EPS && std::abs(dY2 - dT) < EPS;
	bool onLeft   = std::abs(dX1 - dL) < EPS && std::abs(dX2 - dL) < EPS;
	bool onRight  = std::abs(dX1 - dR) < EPS && std::abs(dX2 - dR) < EPS;

	if (onBottom || onTop)
	{
		double minX = std::min(dX1, dX2);
		double maxX = std::max(dX1, dX2);
		return minX <= dR + EPS && maxX >= dL - EPS;
	}

	if (onLeft || onRight)
	{
		double minY = std::min(dY1, dY2);
		double maxY = std::max(dY1, dY2);
		return minY <= dT + EPS && maxY >= dB - EPS;
	}
	return false;
}
// Основная функция: разрезание прямой прямоугольником
std::vector<CSegment> cutLineWithRectangle(double dX1, double dY1, double dX2, double dY2, double dL, double dB, double dR, double dT)
{
	std::vector<CSegment> result;
	CPoint p1(dX1, dY1);
	CPoint p2(dX2, dY2);

	// Проверяем особый случай - прямая лежит на стороне
	if (isLineOnRectangleSide(dX1, dY1, dX2, dY2, dL, dB, dR, dT))
	{
		// Вся прямая находится на границе прямоугольника
		return result; // Пустой результат - вся прямая "внутри"
	}

	// Находим все точки пересечения с прямоугольником
	std::vector<CPoint> intersections;
	std::vector<double> tValues;

	// Стороны прямоугольника
	CPoint sides[4][2] =
	{
		{ CPoint(dL, dB), CPoint(dR, dB) }, // нижняя
		{ CPoint(dR, dB), CPoint(dR, dT) }, // правая
		{ CPoint(dR, dT), CPoint(dL, dT) }, // верхняя
		{ CPoint(dL, dT), CPoint(dL, dB) }  // левая
	};

	double t;
	CPoint intersection;

	for (int i = 0; i < 4; i++)
	{
		if (findIntersection(p1, p2, sides[i][0], sides[i][1], intersection, t))
		{
			// Проверяем, нет ли уже такой точки
			bool exists = false;
			for (const auto& existing : intersections)
			{
				if (existing == intersection) {
					exists = true;
					break;
				}
			}
			if (!exists)
			{
				intersections.push_back(intersection);
				tValues.push_back(t);
			}
		}
	}

	// Если нет пересечений
	if (intersections.empty())
	{
		// Проверяем, полностью ли прямая внутри или снаружи
		bool startInside = isPointInsideRectangle(p1, dL, dB, dR, dT);
		bool endInside   = isPointInsideRectangle(p2, dL, dB, dR, dT);

		if (startInside && endInside)
		{
			// Вся прямая внутри прямоугольника - возвращаем пустой вектор
			return result;
		}
		else
		{
			// Вся прямая снаружи прямоугольника
			result.push_back(CSegment(p1, p2));
			return result;
		}
	}

	// Сортируем точки пересечения по параметру t
	for (size_t i = 0; i < tValues.size(); i++)
	{
		for (size_t j = i + 1; j < tValues.size(); j++)
		{
			if (tValues[j] < tValues[i])
			{
				std::swap(tValues[i], tValues[j]);
				std::swap(intersections[i], intersections[j]);
			}
		}
	}

	// Добавляем начальную и конечную точки для удобства
	std::vector<double> allT = { -EPS };
	std::vector<CPoint> allPoints = { p1 };

	for (size_t i = 0; i < intersections.size(); i++)
	{
		allT.push_back(tValues[i]);
		allPoints.push_back(intersections[i]);
	}

	allT.push_back(1.0 + EPS);
	allPoints.push_back(p2);

	// Проходим по всем сегментам и оставляем те, которые снаружи прямоугольника
	for (size_t i = 0; i < allPoints.size() - 1; i++)
	{
		CPoint midPoint((allPoints[i].x + allPoints[i + 1].x) / 2.0,
						(allPoints[i].y + allPoints[i + 1].y) / 2.0);

		if (!isPointInsideRectangle(midPoint, dL, dB, dR, dT))
		{
			// Этот сегмент снаружи прямоугольника
			result.push_back(CSegment(allPoints[i], allPoints[i + 1]));
		}
	}
	return result;
}

CPoint CCubicBezier::evaluate(double t) const
{
	double t2 = t * t;
	double t3 = t2 * t;
	double u = 1 - t;
	double u2 = u * u;
	double u3 = u2 * u;

	return p0 * u3 + p1 * (3 * u2 * t) + p2 * (3 * u * t2) + p3 * t3;
}
CCubicBezier CCubicBezier::split(double t) const
{
	CPoint q0 = p0;
	CPoint q1 = p0 + (p1 - p0) * t;
	CPoint q2 = p1 + (p2 - p1) * t;
	CPoint q3 = p2 + (p3 - p2) * t;

	CPoint r0 = q1 + (q2 - q1) * t;
	CPoint r1 = q2 + (q3 - q2) * t;
	CPoint r2 = r0 + (r1 - r0) * t;

	return CCubicBezier(q0, q1, r0, r2);
}
CCubicBezier CCubicBezier::getSegment(double t0, double t1) const
{
	if (std::abs(t0 - t1) < EPS)
	{
		CPoint pt = evaluate(t0);
		return CCubicBezier(pt, pt, pt, pt);
	}

	// Разделяем кривую в точке t1
	CCubicBezier firstPart = split(t1);
	// Затем разделяем первую часть в точке t0/t1
	double normalizedT = t0 / t1;
	return firstPart.split(normalizedT);
}

// Проверка пересечения отрезка с границей прямоугольника
bool findLineRectangleIntersection(const CPoint& p1, const CPoint& p2, double dL, double dB, double dR, double dT, std::vector<double>& tValues)
{
	bool found = false;

	// Проверяем пересечение с каждой стороной
	auto checkSide = [&](double x1, double y1, double x2, double y2, int side)
	{
		double det = (p2.x - p1.x) * (y2 - y1) - (p2.y - p1.y) * (x2 - x1);
		if (std::abs(det) < EPS) return;

		double t = ((x1 - p1.x) * (y2 - y1) - (y1 - p1.y) * (x2 - x1)) / det;
		double u = ((x1 - p1.x) * (p2.y - p1.y) - (y1 - p1.y) * (p2.x - p1.x)) / det;

		if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS)
		{
			tValues.push_back(t);
			found = true;
		}
	};

	// Нижняя сторона
	checkSide(dL, dB, dR, dB, 0);
	// Правая сторона
	checkSide(dR, dB, dR, dT, 1);
	// Верхняя сторона
	checkSide(dR, dT, dL, dT, 2);
	// Левая сторона
	checkSide(dL, dT, dL, dB, 3);

	return found;
}
// Рекурсивное нахождение пересечений кривой с прямоугольником
void findBezierRectangleIntersections(const CCubicBezier& bezier, double dL, double dB, double dR, double dT,
									 double t0, double t1, std::vector<double>& intersections, int depth = 0)
{
	if (depth > 20) return; // Ограничение глубины рекурсии

	CPoint p0 = bezier.evaluate(t0);
	CPoint p1 = bezier.evaluate(t1);

	// Проверяем, полностью ли сегмент внутри или снаружи
	bool inside0 = isPointInsideRectangle(p0, dL, dB, dR, dT);
	bool inside1 = isPointInsideRectangle(p1, dL, dB, dR, dT);

	if (inside0 && inside1) {
		// Весь сегмент внутри - пересечений нет
		return;
	}

	// Проверяем плоскую аппроксимацию (хорду)
	std::vector<double> chordIntersections;
	if (findLineRectangleIntersection(p0, p1, dL, dB, dR, dT, chordIntersections))
	{
		if (chordIntersections.size() == 1)
		{
			// Одно пересечение - добавляем его
			double t = t0 + chordIntersections[0] * (t1 - t0);
			intersections.push_back(t);
		}
		else if (chordIntersections.size() == 2)
		{
			// Два пересечения - добавляем оба
			double t1_val = t0 + chordIntersections[0] * (t1 - t0);
			double t2_val = t0 + chordIntersections[1] * (t1 - t0);
			intersections.push_back(t1_val);
			intersections.push_back(t2_val);
		}
		return;
	}

	// Если не нашли пересечений с хордой, но сегмент пересекает границу,
	// рекурсивно разделяем кривую
	double tMid = (t0 + t1) / 2.0;
	findBezierRectangleIntersections(bezier, dL, dB, dR, dT, t0, tMid, intersections, depth + 1);
	findBezierRectangleIntersections(bezier, dL, dB, dR, dT, tMid, t1, intersections, depth + 1);
}
// Быстрая проверка: полностью ли кривая внутри прямоугольника
bool isBezierCompletelyInside(const CCubicBezier& bezier, const TRect& rect)
{
	return isPointInsideRectangle(bezier.p0, rect.fLeft, rect.fBottom, rect.fRight, rect.fTop) &&
		   isPointInsideRectangle(bezier.p1, rect.fLeft, rect.fBottom, rect.fRight, rect.fTop) &&
		   isPointInsideRectangle(bezier.p2, rect.fLeft, rect.fBottom, rect.fRight, rect.fTop) &&
		   isPointInsideRectangle(bezier.p3, rect.fLeft, rect.fBottom, rect.fRight, rect.fTop);
}
// Быстрая проверка: полностью ли кривая снаружи прямоугольника
bool isBezierCompletelyOutside(const CCubicBezier& bezier, const TRect& rect)
{
	// Проверяем по ограничивающему прямоугольнику кривой
	double minX = std::min({bezier.p0.x, bezier.p1.x, bezier.p2.x, bezier.p3.x});
	double maxX = std::max({bezier.p0.x, bezier.p1.x, bezier.p2.x, bezier.p3.x});
	double minY = std::min({bezier.p0.y, bezier.p1.y, bezier.p2.y, bezier.p3.y});
	double maxY = std::max({bezier.p0.y, bezier.p1.y, bezier.p2.y, bezier.p3.y});

	return (maxX < rect.fLeft - EPS) || (minX > rect.fRight + EPS) ||
		   (maxY < rect.fBottom - EPS) || (minY > rect.fTop + EPS);
}
// Основная функция для разрезания кривой Безье прямоугольником
std::vector<CCubicBezier> cutBezierWithRectangle(
	double p0x, double p0y, double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
	double dL, double dB, double dR, double dT)
{
	std::vector<CCubicBezier> result;
	CCubicBezier bezier(CPoint(p0x, p0y), CPoint(p1x, p1y), CPoint(p2x, p2y), CPoint(p3x, p3y));

	// Быстрая проверка: если кривая полностью внутри или снаружи
	TRect rect(dL, dT, dR, dB);
	if (isBezierCompletelyInside(bezier, rect))
	{
		return result; // полностью внутри
	}
	if (isBezierCompletelyOutside(bezier, rect))
	{
		result.push_back(bezier);
		return result; // полностью снаружи
	}

	// Находим все параметры t пересечения с прямоугольником
	std::vector<double> intersections;
	findBezierRectangleIntersections(bezier, dL, dB, dR, dT, 0.0, 1.0, intersections);

	// Сортируем и удаляем дубликаты
	std::sort(intersections.begin(), intersections.end());
	intersections.erase(std::unique(intersections.begin(), intersections.end(),
		[](double a, double b) { return std::abs(a - b) < EPS; }), intersections.end());

	// Добавляем граничные точки
	std::vector<double> allT = {0.0};
	allT.insert(allT.end(), intersections.begin(), intersections.end());
	allT.push_back(1.0);

	// Создаем сегменты и проверяем, какие из них находятся снаружи
	for (size_t i = 0; i < allT.size() - 1; i++)
	{
		double t0 = allT[i];
		double t1 = allT[i + 1];
		double tMid = (t0 + t1) / 2.0;

		CPoint midPoint = bezier.evaluate(tMid);

		if (!isPointInsideRectangle(midPoint, dL, dB, dR, dT))
		{
			// Сегмент снаружи прямоугольника
			CCubicBezier segment = bezier.getSegment(t0, t1);
			result.push_back(segment);
		}
	}

	return result;
}

}
