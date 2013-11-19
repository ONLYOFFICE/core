#pragma once
#ifndef PPTX_LOGIC_ALPHAMODFIX_INCLUDE_H_
#define PPTX_LOGIC_ALPHAMODFIX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaModFix : public WrapperWritingElement
		{
		public:
			AlphaModFix();
			virtual ~AlphaModFix();
			explicit AlphaModFix(const XML::XNode& node);
			const AlphaModFix& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::only_positive<int> > amt;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAMODFIX_INCLUDE_H_