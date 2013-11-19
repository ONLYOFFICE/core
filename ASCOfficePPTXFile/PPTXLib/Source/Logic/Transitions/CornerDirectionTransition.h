#pragma once
#ifndef PPTX_LOGIC_CORNERDIRECTION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_CORNERDIRECTION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/CornerDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class CornerDirectionTransition : public WrapperWritingElement
		{
		public:
			CornerDirectionTransition();
			virtual ~CornerDirectionTransition();			
			explicit CornerDirectionTransition(const XML::XNode& node);
			const CornerDirectionTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
			nullable_property<std::string, Limit::CornerDirectionVal> dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CORNERDIRECTION_TRANSITION_INCLUDE_H