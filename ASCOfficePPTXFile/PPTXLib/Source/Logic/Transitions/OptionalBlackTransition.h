#pragma once
#ifndef PPTX_LOGIC_OPTIONALBLACK_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_OPTIONALBLACK_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class OptionalBlackTransition : public WrapperWritingElement
		{
		public:
			OptionalBlackTransition();
			virtual ~OptionalBlackTransition();			
			explicit OptionalBlackTransition(const XML::XNode& node);
			const OptionalBlackTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
			nullable_property<bool> thruBlk;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_OPTIONALBLACK_TRANSITION_INCLUDE_H