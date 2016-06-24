/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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