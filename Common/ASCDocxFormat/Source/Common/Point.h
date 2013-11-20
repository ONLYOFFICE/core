#pragma once
#ifndef COMMON_POINT_INCLUDE_H_
#define COMMON_POINT_INCLUDE_H_

#include "Unit.h"
#include "property.h"
#include "nullable_property.h"
#include "XML.h"


namespace Common
{
	template<class Unit>
	class Point
	{
	public:
		Point() {};

		template<typename XType, typename YType>
		Point(const XType& x, const YType& y)
			: X(x), Y(y)
		{
		}

		template<typename OtherUnit>
		Point(const OtherUnit& rhs)
			: X(rhs.X), Y(rhs.Y)
		{
		}

		template<typename OtherUnit>
		const Point& operator =(const OtherUnit& rhs)
		{
			X = ths.X;
			Y = ths.Y;
			return *this;
		}

	public:
		property<Unit>	X;
		property<Unit>	Y;
	};
} // namespace Common


namespace XML
{
	template<class Unit>
	const Common::Point<Unit> XElement2Point(const XML::XElement& element, const std::string& x, const std::string& y)
	{
		Common::Point<Unit>	point;
		point.X = element.attribute(x).value().ToString();
		point.Y = element.attribute(y).value().ToString();
		return point;
	}

	template<class Unit>
	const nullable<Common::Point<Unit> > XElement2NullablePoint(const XML::XElement& element, const std::string& x, const std::string& y)
	{
		nullable<Common::Point<Unit> > point;
		if (element.attribute(x).exist() || element.attribute(y).exist())
			point.init();
		if (element.attribute(x).exist())
			point->X = element.attribute(x).value().ToString();
		if (element.attribute(y).exist())
			point->Y = element.attribute(y).value().ToString();
		return point;
	}
} // namespace XML

#endif // COMMON_POINT_INCLUDE_H_