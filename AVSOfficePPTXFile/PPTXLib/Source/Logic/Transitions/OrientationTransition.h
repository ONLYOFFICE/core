#pragma once
#ifndef PPTX_LOGIC_ORIENTATION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_ORIENTATION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/Orient.h"

namespace PPTX
{
	namespace Logic
	{
		class OrientationTransition : public WrapperWritingElement
		{
		public:
			OrientationTransition();
			virtual ~OrientationTransition();			
			explicit OrientationTransition(const XML::XNode& node);
			const OrientationTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
			nullable_property<std::string, Limit::Orient> dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ORIENTATION_TRANSITION_INCLUDE_H