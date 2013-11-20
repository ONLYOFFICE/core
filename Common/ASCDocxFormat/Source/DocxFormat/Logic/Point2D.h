#pragma once
#ifndef OOX_LOGIC_POINT2D_INCLUDE_H_
#define OOX_LOGIC_POINT2D_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class Point2D : public WritingElement
		{
		public:
			Point2D();
			virtual ~Point2D();
			explicit Point2D(const XML::XNode& node);
			const Point2D& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> x;
			property<int> y;
			property<std::string> name;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_POINT2D_INCLUDE_H