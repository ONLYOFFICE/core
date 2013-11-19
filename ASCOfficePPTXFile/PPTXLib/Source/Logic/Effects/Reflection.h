#pragma once
#ifndef PPTX_LOGIC_REFLECTION_INCLUDE_H_
#define PPTX_LOGIC_REFLECTION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../../Limit/RectAlign.h"

namespace PPTX
{
	namespace Logic
	{

		class Reflection : public WrapperWritingElement
		{
		public:
			Reflection();
			virtual ~Reflection();
			explicit Reflection(const XML::XNode& node);
			const Reflection& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string, Limit::RectAlign> algn;
			nullable_property<int, setter::only_positive<int> > blurRad;
			nullable_property<int, setter::only_positive<int> > stA;
			nullable_property<int, setter::only_positive<int> > endA;
			nullable_property<int, setter::only_positive<int> > stPos;
			nullable_property<int, setter::only_positive<int> > endPos;
			nullable_property<int, setter::between<int, 0, 21600000> > dir;
			nullable_property<int, setter::between<int, 0, 21600000> > fadeDir;
			nullable_property<int, setter::only_positive<int> > dist;
			nullable_property<int, setter::between<int, -5400000, 5400000> > kx;
			nullable_property<int, setter::between<int, -5400000, 5400000> > ky;
			nullable_property<bool> rotWithShape;
			nullable_property<int> sx;
			nullable_property<int> sy;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_REFLECTION_INCLUDE_H_