#pragma once
#ifndef PPTX_LOGIC_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "nullable_property.h"
#include "./../../Limit/TransitionSpeed.h"
#include "SndAc.h"
#include "TransitionBase.h"

namespace PPTX
{
	namespace Logic
	{

		class Transition : public WrapperWritingElement
		{
		public:
			Transition();
			virtual ~Transition();
			explicit Transition(const XML::XNode& node);
			const Transition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			/*nullable_property<*/TransitionBase/*>*/ base;
			nullable_property<SndAc> sndAc;

			nullable_property<bool> advClick;
			nullable_property<int, setter::only_positive<int> > advTm;
			nullable_property<std::string, Limit::TransitionSpeed> spd;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TRANSITION_INCLUDE_H_