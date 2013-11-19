#pragma once
#ifndef PPTX_LOGIC_SIDEDIRECTION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_SIDEDIRECTION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/SideDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class SideDirectionTransition : public WrapperWritingElement
		{
		public:
			SideDirectionTransition();
			virtual ~SideDirectionTransition();			
			explicit SideDirectionTransition(const XML::XNode& node);
			const SideDirectionTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
			nullable_property<std::string, Limit::SideDirectionVal> dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SIDEDIRECTION_TRANSITION_INCLUDE_H