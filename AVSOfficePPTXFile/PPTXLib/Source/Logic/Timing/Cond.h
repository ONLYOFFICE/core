#pragma once
#ifndef PPTX_LOGIC_COND_INCLUDE_H_
#define PPTX_LOGIC_COND_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/TLTriggerEvent.h"
#include "./../../Limit/TLRuntimeTrigger.h"
#include "TgtEl.h"

namespace PPTX
{
	namespace Logic
	{
		class Cond : public WrapperWritingElement
		{
		public:
			Cond();
			virtual ~Cond();			
			explicit Cond(const XML::XNode& node);
			const Cond& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
//Choice
			nullable_property<std::string, Limit::TLRuntimeTrigger> rtn;
			nullable_property<TgtEl> tgtEl;
			nullable_property<int, setter::only_positive<int> > tn;

			nullable_property<std::string> delay;
			nullable_property<std::string, Limit::TLTriggerEvent> evt;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_COND_INCLUDE_H