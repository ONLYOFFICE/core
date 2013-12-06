#pragma once
#ifndef OOX_LOGIC_SHAPE_STYLE_INCLUDE_H_
#define OOX_LOGIC_SHAPE_STYLE_INCLUDE_H_

#include <string>

#include "Common.h"
#include "property.h"
#include "Utility.h"
#include "nullable_property.h"

#include "./../Limit/Position.h"
#include "./../Limit/MsoRelative.h"

namespace OOX
{
	namespace Logic
	{
		class ShapeStyle
		{
		public:
			ShapeStyle();
			explicit ShapeStyle(const std::string& value);
			const ShapeStyle& operator=(const std::string& value);

		public:
			const std::string ToString() const;
			void FromString(const std::string& value);

		public:	

			property<Common::Size<Unit<double, Pt, 3> >	>			Size;
			nullable_property<Common::Point<Unit<double, Pt, 0> > >	Point;
			nullable_property<std::string, Limit::Position>			Position;
		
			nullable_property<std::string>							flip;
			nullable_property<std::string>							ZIndex;
			nullable_property<std::string>							rotation;
			nullable_property<std::string>							visibility;			
		
			nullable_property<std::string>							msoWidthPercent;
			nullable_property<std::string>							msoHeightPercent;
			
			nullable_property<std::string>							msoPositionHorizontal;
			nullable_property<std::string>							msoPositionVertical;
			nullable_property<std::string>							msoPositionHorizontalRelative;
			nullable_property<std::string>							msoPositionVerticalRelative;

			nullable_property<std::string>							textAnchor;

			nullable_property<std::string, Limit::MsoRelative>		WidthRelative;
			nullable_property<std::string, Limit::MsoRelative>		HeightRelative;
			
			nullable__<Common::Point< Unit<double, Pt> >>			distanceLeftTop;
			nullable__<Common::Point< Unit<double, Pt> >>			distanceRightBottom;
			
			// group
			nullable__<Common::Point< Unit<double, Pt> >>			leftTop;
			bool													isLocalUnit;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SHAPE_STYLE_INCLUDE_H_