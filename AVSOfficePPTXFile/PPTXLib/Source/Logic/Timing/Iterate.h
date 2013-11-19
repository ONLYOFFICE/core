#pragma once
#ifndef PPTX_LOGIC_ITERATE_INCLUDE_H_
#define PPTX_LOGIC_ITERATE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/IterateType.h"

namespace PPTX
{
	namespace Logic
	{
		class Iterate : public WrapperWritingElement
		{
		public:
			Iterate();
			virtual ~Iterate();			
			explicit Iterate(const XML::XNode& node);
			const Iterate& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string, Limit::IterateType> type;
			nullable_property<bool> backwards;

			nullable_property<std::string> tmAbs;
			nullable_property<int> tmPct;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ITERATE_INCLUDE_H