#pragma once
#ifndef PPTX_LOGIC_SPLIT_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_SPLIT_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/InOutDirectionVal.h"
#include "./../../Limit/Orient.h"

namespace PPTX
{
	namespace Logic
	{
		class SplitTransition : public WrapperWritingElement
		{
		public:
			SplitTransition();
			virtual ~SplitTransition();			
			explicit SplitTransition(const XML::XNode& node);
			const SplitTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string, Limit::InOutDirectionVal> dir;
			nullable_property<std::string, Limit::Orient> orient;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SPLIT_TRANSITION_INCLUDE_H_