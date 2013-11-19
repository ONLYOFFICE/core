#pragma once
#ifndef PPTX_LOGIC_EIGHTDIRECTION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_EIGHTDIRECTION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/EightDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class EightDirectionTransition : public WrapperWritingElement
		{
		public:
			EightDirectionTransition();
			virtual ~EightDirectionTransition();			
			explicit EightDirectionTransition(const XML::XNode& node);
			const EightDirectionTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
			nullable_property<std::string, Limit::EightDirectionVal> dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EIGHTDIRECTION_TRANSITION_INCLUDE_H