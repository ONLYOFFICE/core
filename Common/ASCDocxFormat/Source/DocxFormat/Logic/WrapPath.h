#pragma once
#ifndef OOX_LOGIC_WRAPPATH_INCLUDE_H_
#define OOX_LOGIC_WRAPPATH_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <vector>
#include "Point2D.h"


namespace OOX
{
	namespace Logic
	{
		class WrapPath : public WritingElement
		{
		public:
			WrapPath();
			virtual ~WrapPath();
			explicit WrapPath(const XML::XNode& node);
			const WrapPath& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<Point2D> start;
			property<std::vector<Point2D> > points;
			nullable_property<bool> edited;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_WRAPPATH_INCLUDE_H_