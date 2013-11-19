#pragma once
#ifndef PPTX_LOGIC_OUTERSHDW_INCLUDE_H_
#define PPTX_LOGIC_OUTERSHDW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"
#include "./../../Limit/RectAlign.h"

namespace PPTX
{
	namespace Logic
	{

		class OuterShdw : public WrapperWritingElement
		{
		public:
			OuterShdw();
			virtual ~OuterShdw();
			explicit OuterShdw(const XML::XNode& node);
			const OuterShdw& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;

			nullable_property<std::string, Limit::RectAlign> algn;
			nullable_property<int, setter::only_positive<int> > blurRad;
			nullable_property<int, setter::between<int, 0, 21600000> > dir;
			nullable_property<int, setter::only_positive<int> > dist;
			nullable_property<int, setter::between<int, -5400000, 5400000> > kx;
			nullable_property<int, setter::between<int, -5400000, 5400000> > ky;
			nullable_property<bool> rotWithShape;
			nullable_property<int> sx;
			nullable_property<int> sy;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_OUTERSHDW_INCLUDE_H_