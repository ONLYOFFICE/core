#pragma once
#ifndef PPTX_LOGIC_WHEEL_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_WHEEL_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class WheelTransition : public WrapperWritingElement
		{
		public:
			WheelTransition();
			virtual ~WheelTransition();			
			explicit WheelTransition(const XML::XNode& node);
			const WheelTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int, setter::only_positive<int> > spokes;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_WHEEL_TRANSITION_INCLUDE_H