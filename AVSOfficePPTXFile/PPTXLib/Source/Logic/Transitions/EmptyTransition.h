#pragma once
#ifndef PPTX_LOGIC_EMPTY_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_EMPTY_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class EmptyTransition : public WrapperWritingElement
		{
		public:
			EmptyTransition();
			virtual ~EmptyTransition();			
			explicit EmptyTransition(const XML::XNode& node);
			const EmptyTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EMPTY_TRANSITION_INCLUDE_H